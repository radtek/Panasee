// DlgDICOMSrv.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgDICOMSrv.h"
#include "ConfigMgr.h"
#include "dcmint.h"
#include "DICOM2XINAO.h"
#include "Log.h"
#include "DlgMain.h"
#include "DcmMessage.h"
#include "BI_ImageHeader.h"
#include "ct_ImageHeader.h"
#include "GlobalFuncs.h"
#include "DataAccess.h"
#include "DCMMESSAGE.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDICOMSrv dialog
#define MAX_PACK_LEN      1024*1024
extern CImriDBI m_DBmri;

CDlgDICOMSrv::CDlgDICOMSrv(CWnd* pParent /*=NULL*/)
: CPanaseeDialog(CDlgDICOMSrv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDICOMSrv)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bModeDICOM = FALSE;
	m_bEnableDICOM = FALSE;
	m_lpBuff = NULL;
	m_svrHandle=NULL;
	m_bToshibaCTTable=FALSE;

	m_pMessageWnd=NULL;
}

CDlgDICOMSrv::~CDlgDICOMSrv()
{
	if(m_lpBuff)
	{
		delete[] m_lpBuff ;
		m_lpBuff=NULL;
	}
}

void CDlgDICOMSrv::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDICOMSrv)
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_LIST_DICOM_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDICOMSrv, CPanaseeDialog)
//{{AFX_MSG_MAP(CDlgDICOMSrv)
	ON_MESSAGE(WM_XBDICOMMSG,CStoreSvrMsg)
	ON_MESSAGE(WM_DCMSERVICE_MSG,CDcmServiceMsg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DICOM_LIST, OnDblclkListDicomList)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDICOMSrv message handlers

BOOL CDlgDICOMSrv::OnInitDialog() 
{
	//EnableSeprateLine(TRUE);
	CPanaseeDialog::OnInitDialog();
	

	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnLoad);
	lstBtn.AddTail(&m_btnClose);
	SetBottomBtnList(&lstBtn);	

	// TODO: Add extra initialization here
	CRect rc;
	GetWindowRect(&rc);
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	MoveWindow((x-rc.Width())/2,(y-rc.Height())/2,rc.Width(),rc.Height());

	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_list.SetExtendedStyle(dwExStyle);
	m_list.InsertColumn( 0, "ID", LVCFMT_LEFT, 80);
	m_list.InsertColumn( 1, "Study", LVCFMT_LEFT, 170);
	m_list.InsertColumn( 2, "Series", LVCFMT_LEFT, 100);
	m_list.InsertColumn( 3, "Count", LVCFMT_LEFT, 80);
	m_list.InitHeader();

	
	GetClientRect(&rc);
	rc.bottom-=50;
	m_list.MoveWindow(&rc);

	
	if (m_dlgMRIBrowse.GetSafeHwnd() == NULL)
	{
		m_dlgMRIBrowse.Create(IDD_DLG_MRI_BROWSE, m_pDlgMain);
		
	}
	

	StartService();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// DICOM functions
//回调函数
int fileNum=0;
CString fileChar="A";
CString fileRootPath="D:\\Dicom\\";

int CB(char*FileName)
{
	CString strfileName;
	if (fileNum>=10)
		strfileName.Format("%s_0%d.dcm",fileChar,fileNum++);
	else
		strfileName.Format("%s_00%d.dcm",fileChar,fileNum++);
	strfileName=fileRootPath+strfileName;
	
	strcpy(FileName,strfileName);
	return 1;
}


void FCB(LPVOID pFCBI)
{
	FCBI *pCBI = (FCBI*)pFCBI;
	CDlgDICOMSrv *pDlgSrv = (CDlgDICOMSrv*)pCBI->pDlgDICOMSrv;

	MR_STRUCT_IMAGE* pMRImgData = NULL;
	CT_STRUCT_IMAGE* pCTImgData = NULL;

	if(pCBI->iModality == M_MRI)
	{
		pMRImgData = (MR_STRUCT_IMAGE*)pCBI->pDcmData;
	}else if (pCBI->iModality == M_CT)
	{
		pCTImgData = (CT_STRUCT_IMAGE*)pCBI->pDcmData;
	}

	CMRISeries *pSeries;
	CMRIImage  *pImage;
	BOOL bFind=FALSE;

	try{
		
		for(int i=0;i<pDlgSrv->m_lstSeries.GetCount();i++)
		{
			pSeries=pDlgSrv->m_lstSeries.GetAt(pDlgSrv->m_lstSeries.FindIndex(i));
			if(pCBI->iModality == M_MRI)
			{
				int nImageID = (int) pMRImgData->oHeader.imageField.Id;
				int nSeriesID = (int) pMRImgData->oHeader.seriesField.Id;
				CString sStudyID = pMRImgData->oHeader.studyField.Id;
				if(pSeries->GetStudyID()==sStudyID && pSeries->GetSeriesID()==nSeriesID)
				{
					bFind=TRUE;
					break;
				}
			}else if(pCBI->iModality == M_CT)
			{
				int nImageID = (int) pCTImgData->oHeader.imageField.Id;
				int nSeriesID = (int) pCTImgData->oHeader.seriesField.Id;
				CString sStudyID = pCTImgData->oHeader.studyField.Id;
				if(pSeries->GetStudyID()==sStudyID && pSeries->GetSeriesID()==nSeriesID)
				{
					bFind=TRUE;
					break;
				}
			}		
		}
		
		BOOL bNewSeries=FALSE;
		if(!bFind) //新序列
		{
			pSeries=new CMRISeries();
			bNewSeries=TRUE;
		}
		
		bFind=FALSE;
		for(i=0;i<pSeries->GetNumOfImages();i++)
		{
			pImage=pSeries->GetImageAt(i);
			if(pCBI->iModality == M_MRI)
			{
 				int nImageID = (int) pMRImgData->oHeader.imageField.Id;
 				if(pImage->GetImageID()==nImageID)
 				{
 					bFind=TRUE;
 					break;
 				}
			}else if(pCBI->iModality == M_CT)
			{
				int nImageID = (int) pCTImgData->oHeader.imageField.Id;
				if(pImage->GetImageID()==nImageID)
				{
					bFind=TRUE;
					break;
				}
			}
		}
		
		
		if(bFind) //说明这张图像之前已经传输过了
		{
			if(pMRImgData)
			{
				delete pMRImgData->pData;
				delete pMRImgData;
			}

			if(pCTImgData)
			{
				delete pCTImgData->pData;
				delete pCTImgData;
			}

			return;
		}
		
		pImage=new CMRIImage();
		pImage->m_pSeries = pSeries;
		if(pCBI->iModality == M_MRI)
		{
			pImage->LoadFromIMGFile(&(pMRImgData->oHeader), pMRImgData->pData);
		}else if(pCBI->iModality == M_CT)
		{
			pImage->LoadFromIMGFile((MR_ImageHeader*)&(pCTImgData->oHeader), pCTImgData->pData);
		}
		
		pSeries->InsertImage(pImage);
		
		//save this dicom image
		if (pDlgSrv->m_pDlgMain->m_dlgCryotherapy.m_sMRIStorageFolder != "")
		{
			CString sModalityFolder;
			sModalityFolder.Format("%s\\%s", pDlgSrv->m_pDlgMain->m_dlgCryotherapy.m_sMRIStorageFolder, pDlgSrv->m_pDlgMain->m_MRIWrap.m_sCurModalityID);
			CreateDirectory(sModalityFolder, 0);
			CString sStudyFolder;
			sStudyFolder.Format("%s\\%s", sModalityFolder, pSeries->GetStudyID());
			CreateDirectory(sStudyFolder, 0);
			CString sSeriesFolder;
			sSeriesFolder.Format("%s\\%d", sStudyFolder, pSeries->GetSeriesID());
			CreateDirectory(sSeriesFolder, 0);
			CString sScanFolder;
			sScanFolder.Format("%s\\%d", sSeriesFolder, pSeries->GetScanID());
			CreateDirectory(sScanFolder, 0);
			pSeries->m_sFolder = sScanFolder;
			
			CDataAccess dataAccess;
			if(pCBI->iModality == M_MRI)
			{
				dataAccess.WriteImage(pMRImgData, sScanFolder);
			}else if(pCBI->iModality == M_CT)
			{
				dataAccess.WriteImage((LPMR_STRUCT_IMAGE)pCTImgData, sScanFolder);
			}
			
		}
		
		pDlgSrv->UpdateView(pSeries);
		if(bNewSeries)
		{
			pDlgSrv->m_lstSeries.AddTail(pSeries);
		}
		
		if(pMRImgData)
		{
// 			delete pMRImgData->pData;
// 			delete pMRImgData;
		}

		if(pCTImgData)
		{
// 			delete pCTImgData->pData;
// 			delete pCTImgData;
		}
		

		pDlgSrv->SendDicomSeriesMessage();
		
	}catch(...)
	{
		pDlgSrv->SendMsg("DICOM:接收完图像数据,解析并加载图像数据过程中出错");
	}
}

BOOL CDlgDICOMSrv::StartService()
{
	CConfigMgr conf;
	m_bModeDICOM = conf.Read(CONF_DICOM,CONF_DICOM_MODE,0);
	m_bEnableDICOM = conf.Read(CONF_DICOM, CONF_DICOM_ENABLE, 0);
	m_bToshibaCTTable = conf.Read(CONF_DICOM,CONF_DICOM_TOSHIBA_TABLE,0);
	
	if (m_bEnableDICOM)
	{
		CConfigMgr conf;
		CString sPort = conf.Read(CONF_DICOM, CONF_DICOM_PORT, "104");

		if(m_bModeDICOM)
		{
			try
			{
				if (m_lpBuff)
				{
					delete m_lpBuff;
				}

				//开始服务
				SetAET("COOLPACS");
				SetPort(sPort.GetBuffer(1));
				SetRootDir("d:\\dicom\\imagedir\\");
				
				m_lpBuff = new BYTE[MAX_PACK_LEN];
				m_svrHandle=BeginSVRSCP((BYTE*)&m_CBI,CB,this->GetSafeHwnd(),m_lpBuff);
			}catch(...)
			{
				m_svrHandle=NULL;
			}
		}else
		{
			try
			{
				//开始服务
				SetSvrAET("COOLPACS");
				SetSvrPort(sPort.GetBuffer(1));
				SetSvrRootDir("d:\\dicom\\imagedir\\");
				
				m_svrHandle=BeginServer((BYTE*)&m_FCBI,FCB,this,this->GetSafeHwnd());

			}catch(...)
			{
				m_svrHandle=NULL;
			}			
		}


		if(m_svrHandle == NULL)
		{
			PanaMessageBox("Fail to start DICOM server!");
			return FALSE;
		}
		CLog::Log("DICOM server started !");
	}
	return TRUE;
}



void CDlgDICOMSrv::CDcmServiceMsg(WPARAM wParam,LPARAM lParam)
{
	CString sTmp="";
	
	switch(wParam)
	{
	case STORAGE_FAULT_MULTIPLEX:     //"DICOM 服务器","multiplex错误"
		sTmp="DICOM 服务器:multiplex错误";
		break;
	case STORAGE_FAULT_PDUREAD:       //"DICOM 服务器","pduread错误"
		sTmp="DICOM 服务器:pduread错误";
		break;
	case STORAGE_FAULT_NEW:           //"DICOM 服务器","new错误"
		sTmp="DICOM 服务器:new错误";
		break;
	case STORAGE_FAULT_CT_SAVE:       //"DICOM 服务器","本地保存CT Dicom图像失败"
		sTmp="DICOM 服务器:本地保存CT Dicom图像失败";
		break;
	case STORAGE_FAULT_MR_SAVE:       //"DICOM 服务器","本地保存MR Dicom图像失败"
		sTmp="DICOM 服务器:本地保存MR Dicom图像失败";
		break;
	case STORAGE_FAULT_STORAGEREAD:   //"DICOM 服务器","storage read错误"
		sTmp="DICOM 服务器:storage read错误";
		break;
	case STORAGE_FAULT_DELETECLOSE:   //"DICOM 服务器","delete close错误"
		sTmp="DICOM 服务器:reset delete错误";
		break;
	case STORAGE_FAULT_RESETDELETE:   //"DICOM 服务器","reset delete错误"
		sTmp="DICOM 服务器:reset delete错误";
		break;
	case STORAGE_FAULT_LISTEN:        //"DICOM 服务器","监听端口错误,服务退出"
		sTmp="DICOM 服务器:监听端口错误,服务退出";
		break;
	case STORAGE_FAULT_RESTARTLISTEN: //"DICOM 服务器","监听端口错误,重新启动服务"
		sTmp="DICOM 服务器:监听端口错误,重新启动服务";
		break;
	case STORAGE_SERVICE_START:        //"DICOM 服务器","服务启动"
		sTmp="DICOM 服务器:服务启动";
		break;
	case STORAGE_SERVICE_STOP:        //"DICOM 服务器","服务被正常停止"
		sTmp="DICOM 服务器:服务被正常停止";
		break;
	case STORAGE_FAULT_MAINTHREAD:    //"DICOM 服务器","主线程错误"
		sTmp="DICOM 服务器:主线程错误";
		break;
	case STORAGE_FAULT_CLOSEMAINSOCKET: //"DICOM 服务器","关闭主SOCKET错误"
		sTmp="DICOM 服务器:关闭主SOCKET错误";
		break;
	case STORAGE_RECEIVE_CTSERIES:      //"received CT image number is %d.\r\n"
		sTmp.Format("received CT image number is %i",lParam);
		break;
	case STORAGE_RECEIVE_MRSERIES:      //"received MRI image number is %d.\r\n"
		sTmp.Format("received MR image number is %i",lParam);
		break;
	case STORAGE_FAULT_INITIALIZENETWORK: //"ASC_initializeNetwork错误"
		sTmp="ASC_initializeNetwork错误";
		break;
	case STORAGE_FAULT_CREATEASSOCIATIONPARAMETERS: //"ASC_createAssociationParameters错误"
		sTmp="ASC_createAssociationParameters错误";
		break;
	case STORAGE_FAULT_SETTRANSPORTLAYERTYPE: //"ASC_setTransportLayerType错误"
		sTmp="ASC_setTransportLayerType错误";
		break;

	}
	
	if(sTmp=="")
		return;
	
	CLog::Log(sTmp);
	m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(sTmp);
}

void CDlgDICOMSrv::CStoreSvrMsg(WPARAM wParam,LPARAM lParam)
{
	
	int nMsg=0;
	switch(wParam)
	{
	case STORAGE_FAULT_MULTIPLEX:     //"DICOM 服务器","multiplex错误"
	case STORAGE_FAULT_PDUREAD:       //"DICOM 服务器","pduread错误"
	case STORAGE_FAULT_NEW:           //"DICOM 服务器","new错误"
	case STORAGE_FAULT_CT_SAVE:       //"DICOM 服务器","本地保存CT Dicom图像失败"
	case STORAGE_FAULT_MR_SAVE:       //"DICOM 服务器","本地保存MR Dicom图像失败"
	case STORAGE_FAULT_STORAGEREAD:   //"DICOM 服务器","storage read错误"
	case STORAGE_FAULT_DELETECLOSE:   //"DICOM 服务器","delete close错误"
	case STORAGE_FAULT_RESETDELETE:   //"DICOM 服务器","reset delete错误"
	case STORAGE_FAULT_LISTEN:        //"DICOM 服务器","监听端口错误,服务退出"
	case STORAGE_FAULT_RESTARTLISTEN: //"DICOM 服务器","监听端口错误,重新启动服务"
	case STORAGE_FAULT_MAINTHREAD:    //"DICOM 服务器","主线程错误"
	case STORAGE_FAULT_CLOSEMAINSOCKET: //"DICOM 服务器","关闭主SOCKET错误"
	case STORAGE_SERVICE_START:       //"DICOM 服务器","服务启动"
	case STORAGE_SERVICE_STOP:        //"DICOM 服务器","服务被正常停止"
	case STORAGE_FAULT_INITIALIZENETWORK: //"ASC_initializeNetwork错误"
	case STORAGE_FAULT_CREATEASSOCIATIONPARAMETERS: //"ASC_createAssociationParameters错误"
	case STORAGE_FAULT_SETTRANSPORTLAYERTYPE: //"ASC_setTransportLayerType错误"
		this->PostMessage(WM_DCMSERVICE_MSG,wParam,lParam);
		break;
	
	case STORAGE_RECEIVE_CTSERIES:
		SaveImage(1);
		//nMsg=STORAGE_RECEIVE_CTSERIES;
		//this->PostMessage(WM_DCMSERVICE_MSG,nMsg,lParam);
		break;
	case STORAGE_RECEIVE_MRSERIES:
		SaveImage(0);
		//nMsg=STORAGE_RECEIVE_MRSERIES;
		//this->PostMessage(WM_DCMSERVICE_MSG,nMsg,lParam);
		break;
	}
}


BOOL CDlgDICOMSrv::StopService() 
{
	if(m_svrHandle==NULL)
		return TRUE;

	int iCount=0;
	
RetryStopService: 
	try
	{
		if(m_bModeDICOM)
			TermSVRSCP(m_svrHandle);
		else
			TermServer(m_svrHandle);
		m_svrHandle=NULL;
	}
	catch(...)
	{
		iCount++;
		if(iCount<3)
		{
			goto RetryStopService;  //尝试再次结束服务
		}else
		{
			CLog::Log("Dicom服务非正常停止");
			return FALSE;
		}
	}

	CLog::Log("Dicom服务正常停止");
	return TRUE;
}

void CDlgDICOMSrv::SendMsg(char *pMsg)
{
	CString sTmp;
	sTmp.Format("%s",pMsg);
	SendMsg(sTmp);
}

void CDlgDICOMSrv::SendMsg(CString sMsg)
{
	CLog::Log(sMsg);
	m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(sMsg);
}


void CDlgDICOMSrv::Close()
{
	CMRISeries *pSeries;
	for(int i=0;i<m_lstSeries.GetCount();i++)
	{
		pSeries=m_lstSeries.GetAt(m_lstSeries.FindIndex(i));
		delete pSeries;
	}
	m_lstSeries.RemoveAll();
	

	StopService();

	if(m_lpBuff)
	{
		delete[] m_lpBuff ;
		m_lpBuff=NULL;
	}
	
}

void CDlgDICOMSrv::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LIST_DICOM_LIST)
		m_list.MeasureItem(lpMeasureItemStruct);
	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CDlgDICOMSrv::OnDblclkListDicomList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	
	CString studyID="";
	CString seriesID="";
	int nSeriesID=0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos) 
	{
		int i = m_list.GetNextSelectedItem(pos);
		studyID=m_list.GetItemText(i,1);
		seriesID=m_list.GetItemText(i,2);
		nSeriesID=atoi(seriesID);
	}else
	{
		return;
	}
	
	CMRISeries *pSeries=NULL;
	pos=m_lstSeries.GetHeadPosition();
	while(pos)
	{
		pSeries=m_lstSeries.GetNext(pos);
		if(pSeries->GetStudyID()==studyID && pSeries->GetSeriesID()==nSeriesID)
		{
			m_dlgMRIBrowse.SetMRISeries(pSeries);
			m_dlgMRIBrowse.ShowWindow(TRUE);
			break;
		}
	}
	
}

void CDlgDICOMSrv::SaveImage(int iType)
{
	//return ;

	LPMR_STRUCT_IMAGE pMrImgData=NULL;
	LPCT_STRUCT_IMAGE pCtImgData=NULL;
	
	int          nImgDataLen;
	
	int iResult=0;
	
	CFile        file ;
	CString      csFilePath;
	CString      csTmp;
	
	CDICOM2XINAO dcm2XinAo;	
	dcm2XinAo.m_bToshibaCTTable=m_bToshibaCTTable;
	try
	{
		if (iType == 0)
		{
			//csT.Format("End receiving the MRI series. %d images have been Received!\r\n",nImgNum);
						
			DCM_IMG_HEADER *pdcmImgHeader = (DCM_IMG_HEADER*)m_lpBuff;
			nImgDataLen = pdcmImgHeader->nImgDatalen;
			
			try
			{
				iResult=dcm2XinAo.ConvertMRDcm2XB((char*)pdcmImgHeader->data, nImgDataLen, pMrImgData);
			}catch(...)
			{
				iResult=-1;
				SendMsg("解析MR图像数据时出错,继续尝试解析下一幅.");
				return;
			}
			if (iResult != 0)
			{
				if(pMrImgData)
				{
					if(pMrImgData->pData)
					{
						delete pMrImgData->pData;
						pMrImgData->pData=NULL;
					}
					delete pMrImgData;
					pMrImgData=NULL;
				}
			}
			
		}
		else if (iType == 1)
		{
			//csT.Format("End receiving the CT series. %d images have been Received!\r\n",nImgNum);
						
			DCM_IMG_HEADER *pdcmImgHeader = (DCM_IMG_HEADER*)m_lpBuff;
			nImgDataLen = pdcmImgHeader->nImgDatalen;
			
			try
			{
				iResult=dcm2XinAo.ConvertCTDcm2XB((char*)pdcmImgHeader->data, nImgDataLen, pCtImgData);
			}catch(...)
			{
				iResult=-1;
				SendMsg("解析CT图像数据时出错,继续尝试解析下一幅.");
				return;
			}
			if (iResult != 0)
			{
				if(pCtImgData)
				{
					if(pCtImgData->pData)
					{
						delete pCtImgData->pData;
						pCtImgData->pData=NULL;
					}
					delete pCtImgData;
					pCtImgData=NULL;
				}
			}
						
		}else
		{
			return;
		}

		CMRISeries *pSeries;
		CMRIImage  *pImage;
		BOOL bFind=FALSE;

		for(int i=0;i<m_lstSeries.GetCount();i++)
		{
			pSeries=m_lstSeries.GetAt(m_lstSeries.FindIndex(i));
			if(iType==0)
			{
				int nImageID = (int) pMrImgData->oHeader.imageField.Id;
				int nSeriesID = (int) pMrImgData->oHeader.seriesField.Id;
				CString sStudyID = pMrImgData->oHeader.studyField.Id;
				if(pSeries->GetStudyID()==sStudyID && pSeries->GetSeriesID()==nSeriesID)
				{
					bFind=TRUE;
					break;
				}
			}else if(iType==1)
			{
				int nImageID = (int) pCtImgData->oHeader.imageField.Id;
				int nSeriesID = (int) pCtImgData->oHeader.seriesField.Id;
				CString sStudyID = pCtImgData->oHeader.studyField.Id;
				if(pSeries->GetStudyID()==sStudyID && pSeries->GetSeriesID()==nSeriesID)
				{
					bFind=TRUE;
					break;
				}
			}

		}
		BOOL bNewSeries=FALSE;
		if(!bFind) //新序列
		{
			pSeries=new CMRISeries();
			bNewSeries=TRUE;
		}

		bFind=FALSE;
		for(i=0;i<pSeries->GetNumOfImages();i++)
		{
			pImage=pSeries->GetImageAt(i);
			if(iType==0)
			{
				int nImageID = (int) pMrImgData->oHeader.imageField.Id;
				if(pImage->GetImageID()==nImageID)
				{
					bFind=TRUE;
					break;
				}
			}else if(iType==1)
			{
				int nImageID = (int) pCtImgData->oHeader.imageField.Id;
				if(pImage->GetImageID()==nImageID)
				{
					bFind=TRUE;
					break;
				}
			}
		}

		if(bFind) //说明这张图像之前已经传输过了
		{
			if(pMrImgData)
			{
				delete pMrImgData->pData;
				delete pMrImgData;
			}
			if(pCtImgData)
			{
				delete pCtImgData->pData;
				delete pCtImgData;
			}
			return;
		}
		
		pImage=new CMRIImage();
		pImage->m_pSeries = pSeries;
		if(iType==0)
		{
			pImage->LoadFromIMGFile(&(pMrImgData->oHeader), pMrImgData->pData);
		}else if(iType==1)
		{
			pImage->LoadFromIMGFile((MR_ImageHeader*)&(pCtImgData->oHeader), pCtImgData->pData);
		}
		
		pSeries->InsertImage(pImage);
		
		//save this dicom image
		if (m_pDlgMain->m_dlgCryotherapy.m_sMRIStorageFolder != "")
		{
			CString sModalityFolder;
			sModalityFolder.Format("%s\\%s", m_pDlgMain->m_dlgCryotherapy.m_sMRIStorageFolder, m_pDlgMain->m_MRIWrap.m_sCurModalityID);
			CreateDirectory(sModalityFolder, 0);
			CString sStudyFolder;
			sStudyFolder.Format("%s\\%s", sModalityFolder, pSeries->GetStudyID());
			CreateDirectory(sStudyFolder, 0);
			CString sSeriesFolder;
			sSeriesFolder.Format("%s\\%d", sStudyFolder, pSeries->GetSeriesID());
			CreateDirectory(sSeriesFolder, 0);
			CString sScanFolder;
			sScanFolder.Format("%s\\%d", sSeriesFolder, pSeries->GetScanID());
			CreateDirectory(sScanFolder, 0);
			pSeries->m_sFolder = sScanFolder;
			
			CDataAccess dataAccess;
			if(iType==0)
			{
				dataAccess.WriteImage(pMrImgData, sScanFolder);
			}else if(iType==1)
			{
				dataAccess.WriteImage((LPMR_STRUCT_IMAGE)pCtImgData, sScanFolder);
			}
			
		}

		UpdateView(pSeries);
		if(bNewSeries)
		{
			m_lstSeries.AddTail(pSeries);
		}

		if(pMrImgData)
		{
			delete pMrImgData->pData;
			delete pMrImgData;
		}
		if(pCtImgData)
		{
			delete pCtImgData->pData;
			delete pCtImgData;
		}
		
		SendDicomSeriesMessage();

	}catch(...)
	{
		SendMsg("DICOM:接收完图像数据,解析并加载图像数据过程中出错");
	}
	
}

void CDlgDICOMSrv::UpdateView(CMRISeries *pSeries)
{
	BOOL bFind=FALSE;
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		CString studyID=m_list.GetItemText(i,1);
		CString seriesID=m_list.GetItemText(i,2);
		int nSeriesID=atoi(seriesID);
		if(pSeries->GetStudyID()==studyID && pSeries->GetSeriesID()==nSeriesID)
		{
			bFind=TRUE;
			CString sImageCount;
			sImageCount.Format("%i",pSeries->GetNumOfImages());
			m_list.SetItemText(i,3,sImageCount);
		}
	}

	if(!bFind)
	{
		CString sTmp;
		sTmp.Format("%i",m_list.GetItemCount()+1);
		int nItem=m_list.InsertItem(0,sTmp);
		CString studyID=pSeries->GetStudyID();
		CString seriesID;
		seriesID.Format("%i",pSeries->GetSeriesID());
		sTmp.Format("%i",pSeries->GetNumOfImages());
		m_list.SetItemText(nItem,1,studyID);
		m_list.SetItemText(nItem,2,seriesID);
		m_list.SetItemText(nItem,3,sTmp);
	}

}

void CDlgDICOMSrv::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	this->PostMessage(WM_CLOSE);
}

void CDlgDICOMSrv::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	CString studyID="";
	CString seriesID="";
	int nSeriesID=0;
	int index;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos) 
	{
		index = m_list.GetNextSelectedItem(pos);
		studyID=m_list.GetItemText(index,1);
		seriesID=m_list.GetItemText(index,2);
		nSeriesID=atoi(seriesID);
	}else
	{
		return;
	}
	
	BOOL bFind=FALSE;
	CMRISeries *pSeries=NULL;
	pos=m_lstSeries.GetHeadPosition();
	int lstIndex=0;
	while(pos)
	{
		pSeries=m_lstSeries.GetNext(pos);
		if(pSeries->GetStudyID()==studyID && pSeries->GetSeriesID()==nSeriesID)
		{
			bFind=TRUE;
			break;
		}
		lstIndex++;
	}
	if(!bFind || !pSeries)
		return;

	//检查图像是否有断层
	int count=pSeries->GetNumOfImages();
	int iMinID=9999;
	int iMaxID=-9999;
	for(int i=0;i<count;i++)
	{
		int id=pSeries->GetImageAt(i)->GetImageID();
		if(iMinID>id)
		{
			iMinID=id;
		}
		if(iMaxID<id)
		{
			iMaxID=id;
		}
	}
	if((iMaxID-iMinID+1)>count)
	{
		PanaMessageBox("Dicom transfer is not completed. Please wait.");
		return;
	}

	BOOL bResult;
	try
	{
		m_pDlgMain->m_dlgCryotherapy.DeleteMRISeries(pSeries->GetName());
		pSeries->CorrectSliceDirection(); //此函数之前没有被调过，加载之前需要作此步骤
		
		bResult = m_pDlgMain->m_dlgCryotherapy.LoadMRISeries(pSeries,FALSE);
	}catch(...)
	{
		bResult = FALSE;
		SendMsg("加载图像序列时出错");
	}
	if (bResult)
	{					
		m_pDlgMain->m_dlgCryotherapy.InsertMap(pSeries->GetName(), pSeries->m_sFolder);

		CheckPatientID(pSeries);
		m_pDlgMain->m_MRIWrap.InsertImgToDB(pSeries);

		//从列表中删除已经加载到series列表中的图像
		m_list.DeleteItem(index);
		m_lstSeries.RemoveAt(m_lstSeries.FindIndex(lstIndex));
	}

	SendDicomSeriesMessage();
		
}

void CDlgDICOMSrv::SendDicomSeriesMessage()
{
	if(m_pMessageWnd)
	{
		int imageCount=0;
		CString sImageCount="0";
		for(int i=0;i<m_list.GetItemCount();i++)
		{
			sImageCount=m_list.GetItemText(i,3);
			imageCount += atoi(sImageCount);
		}
		m_pMessageWnd->SendMessage(WM_DICOM_IMAGECOUNT,(WPARAM)&imageCount);
	}
}

void CDlgDICOMSrv::CheckPatientID(CMRISeries *pSeries)
{
	if(m_pDlgMain->m_MRIWrap.m_iCurPatientUID>0)
	{
		return;
	}
	MR_ImageHeader *pMRHeader = pSeries->GetImageAt(0)->GetImageHeader();
	CT_ImageHeader *pCTHeader = (CT_ImageHeader*)pMRHeader;
	US_ImageHeader *pUSHeader = (US_ImageHeader*)pMRHeader;
	CString sID;
	CString sName;
	CString sBirth; 
	CString sWeight;
	CString sDesc;
	CString sAge;
	int nSex;
	if(pSeries->GetImageAt(0)->GetImageType()==IMS_DCMCT)
	{		
		sID.Format("%s", pCTHeader->patientField.Id);		
		sName.Format("%s", pCTHeader->patientField.Name);		
		sBirth.Format("%4d-%02d-%02d", pCTHeader->patientField.BirthDate.year, pCTHeader->patientField.BirthDate.month, pCTHeader->patientField.BirthDate.day);		
		sWeight.Format("%d", pCTHeader->patientField.Weight);		
		sDesc.Format("%s", pCTHeader->patientField.Desc);		
		sAge.Format("%d", pCTHeader->patientField.Age);
		nSex =  (pCTHeader->patientField.Sex == 'M' ? 0 : (pCTHeader->patientField.Sex == 'F' ? 1 : 2));
	}else if(pSeries->GetImageAt(0)->GetImageType()==IMS_DCMUS)
	{		
		sID.Format("%s", pUSHeader->patientField.Id);		
		sName.Format("%s", pUSHeader->patientField.Name);		
		sBirth.Format("%4d-%02d-%02d", pUSHeader->patientField.BirthDate.year, pUSHeader->patientField.BirthDate.month, pUSHeader->patientField.BirthDate.day);		
		sWeight.Format("%d", pUSHeader->patientField.Weight);		
		sDesc.Format("%s", pUSHeader->patientField.Desc);		
		sAge.Format("%d", pUSHeader->patientField.Age);
		nSex =  (pUSHeader->patientField.Sex == 'M' ? 0 : (pUSHeader->patientField.Sex == 'F' ? 1 : 2));
	}else
	{		
		sID.Format("%s", pMRHeader->patientField.Id);		
		sName.Format("%s", pMRHeader->patientField.Name);		
		sBirth.Format("%4d-%02d-%02d", pMRHeader->patientField.BirthDate.year, pMRHeader->patientField.BirthDate.month, pMRHeader->patientField.BirthDate.day);		
		sWeight.Format("%d", pMRHeader->patientField.Weight);		
		sDesc.Format("%s", pMRHeader->patientField.Desc);		
		sAge.Format("%d", pMRHeader->patientField.Age);
		nSex =  (pMRHeader->patientField.Sex == 'M' ? 0 : (pMRHeader->patientField.Sex == 'F' ? 1 : 2));
	}
	


	int iUID = m_DBmri.ExistPatient(sID, m_pDlgMain->m_MRIWrap.m_sCurModalityID);
	
	if (iUID <= 0)
	{
		iUID = m_DBmri.AddPatient(sID, sName, sBirth, sAge, nSex, sWeight, sDesc, m_pDlgMain->m_MRIWrap.m_sCurModalityID);
		
	}
	if (iUID <= 0)
	{
		PanaMessageBox("Error in inserting patient data to database");
		
		return;
	}
	m_pDlgMain->m_MRIWrap.m_iCurPatientUID = iUID;
}
