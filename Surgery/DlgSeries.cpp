// DlgSeries.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgSeries.h"
#include "CryotherapyDlg.h"
#include "CrashRstore.h"
#include "DlgMain.h"
#include "GlobalFuncs.h"
#include "ConfigMgr.h"
#include "ImageFusion.h"
#include "config.h"
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_InOpenSeriesFromDBThread;
/////////////////////////////////////////////////////////////////////////////
// CDlgSeries dialog

enum SERIES_LIST 
{
	SL_NAME,
	SL_DESC,
	SL_PROBE
};


CDlgSeries::CDlgSeries(CWnd* pParent /*=NULL*/)
	: CDlgSeriesSkin(CDlgSeries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSeries)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSeries::DoDataExchange(CDataExchange* pDX)
{
	CDlgSeriesSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSeries)
	DDX_Control(pDX, IDC_BUTTON_FUSION, m_btnFusion);
	DDX_Control(pDX, IDC_BUTTON_DICOMLIST, m_btnDicomList);
	DDX_Control(pDX, IDC_LST_SERIES, m_ctrlSeriesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSeries, CDlgSeriesSkin)
	//{{AFX_MSG_MAP(CDlgSeries)
	ON_MESSAGE(WM_FUSION_GETSERIESLIST,OnGetSeriesList)
	ON_MESSAGE(WM_FUSION_TRACKDATA,OnGetTrackData)
	ON_MESSAGE(WM_FUSION_GETSERIES,OnGetSeries)
	ON_MESSAGE(WM_DICOM_IMAGECOUNT,OnDicomImageCount)
	ON_MESSAGE(WM_SERIES_CRASH_UPDATE, OnCrashUpdate)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LST_SERIES, OnBegindragLstSeries)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LST_SERIES, OnKeydownLstSeries)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LST_SERIES, OnDblclkLstSeries)
	ON_BN_CLICKED(IDC_BUTTON_DICOMLIST, OnButtonDicomlist)
	ON_BN_CLICKED(IDC_BUTTON_FUSION, OnButtonFusion)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSeries message handlers
LRESULT CDlgSeries::OnGetTrackData(WPARAM wParam, LPARAM lParam)
{
	TRACKING_DATA *pData=(TRACKING_DATA*)wParam;
	Matrix mProbe(4,4),mBase(4,4),mMarker(4,4),mScanner2Marker(4,4);
	mProbe.Identity();
	try
	{
		mProbe.value[0][3]=m_preImageTip[0];
		mProbe.value[1][3]=m_preImageTip[1];
		mProbe.value[2][3]=m_preImageTip[2];
		mProbe = m_pCryDlg->m_tProbe * mProbe;

		mBase=m_pCryDlg->m_tBase;
		mScanner2Marker=m_pCryDlg->m_tranScanner2Marker;
		mMarker=m_pCryDlg->m_tMarker;
	}catch(...)
	{
		return 0;
	}

	pData->bBaseVisible = m_pCryDlg->m_bBaseVisible;
	pData->bMarkerVisible = m_pCryDlg->m_bMarkerVisible;
	pData->bProbeVisible = m_pCryDlg->m_bProbeVisible;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			pData->dBase2Tracker[i][j]=mBase.value[i][j];
			pData->dScanner2Marker[i][j]=mScanner2Marker.value[i][j];
			pData->dProbe2Tracker[i][j]=mProbe.value[i][j];
			pData->dMarker2Tracker[i][j]=mMarker.value[i][j];
		}
	}
	return 0;
}

LRESULT CDlgSeries::OnDicomImageCount(WPARAM wParam, LPARAM lParam)
{
	int *p=(int*)wParam;
	int count=*p;
	CString sTmp;
	sTmp.Format("Dicom %i",count);
	m_btnDicomList.SetWindowText(sTmp);
	return 0;
}

LRESULT CDlgSeries::OnGetSeries(WPARAM wParam, LPARAM lParam)
{
	CString *pName=(CString*)wParam;
	CMRISeries **pSeries=(CMRISeries**)lParam;
	CMRISeries *pMRISeries = m_pCryDlg->GetMRISeries(*pName);
	if(pMRISeries)
	{
		*pSeries=pMRISeries;
	}else
	{
		*pSeries=NULL;
	}
	return 0;
}

LRESULT CDlgSeries::OnGetSeriesList(WPARAM wParam, LPARAM lParam)
{
	CStringArray *saMRI=(CStringArray*)wParam;
	CStringArray *saDesc=(CStringArray*)lParam;
	CStringArray sMRI,sDesc;
	GetMRISeriesName(sMRI,NULL,&sDesc);
	for (int i=0; i<sMRI.GetSize(); i++)
	{
		if(sMRI[i].Find("MPR",0)>=0)
		{
			continue;
		}
		saMRI->Add(sMRI[i]);
		saDesc->Add(sDesc[i]);
	}
	return 0;
}

LONG CDlgSeries::OnCrashUpdate( UINT wParam, LONG lParam )
{
	CCrashRstore::SaveCrashState(CI_IMAGES);
	return 0;
}

BOOL CDlgSeries::OnInitDialog() 
{
	CConfigMgr conf;
	BOOL bEnableDICOM = conf.Read(CONF_DICOM, CONF_DICOM_ENABLE, 0);
	if(bEnableDICOM)
	{
		SetDicomBtn(&m_btnDicomList);
	}
	SetFusionBtn(&m_btnFusion);
	SetSeriesListCtrl(&m_ctrlSeriesList);
	
	CDlgSeriesSkin::OnInitDialog();
	
	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_ctrlSeriesList.SetExtendedStyle(dwExStyle);
	m_ctrlSeriesList.InsertColumn( SL_NAME, "Series", LVCFMT_LEFT, 80);
	m_ctrlSeriesList.InsertColumn( SL_DESC, "Description", LVCFMT_LEFT, 130);
	m_ctrlSeriesList.InsertColumn( SL_PROBE, "Probe", LVCFMT_LEFT, 65);
	m_ctrlSeriesList.InitHeader();
	
	//获取预扫描针的参数
	double dTip[3],dOri[3];
	double dLength,dDiameter;
	CConfigMgr con;
	CString sFilePath = CONFIG_FILES[CF_PROBE_PREIMAGE];
	if( CGlobalFuncs::LoadProbeFile(sFilePath, dOri, dTip, dLength, dDiameter) )
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s successfully. ", sFilePath);
		CLog::Log(sLog);

		m_preImageTip[0]=dTip[0];
		m_preImageTip[1]=dTip[1];
		m_preImageTip[2]=dTip[2];
	}
	else
	{
		m_preImageTip[0]=0;
		m_preImageTip[1]=0;
		m_preImageTip[2]=0;
		CString sLog;
		sLog.Format("Load pre-scan probe from %s fail. ", sFilePath);
		CLog::Log(sLog);
		PanaMessageBox("Fail to load prescan probe!");
	}

	if(!bEnableDICOM)
	{
		m_btnDicomList.MoveWindow(0,0,1,1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CDlgSeries::InsertMRIList(CStringArray& saMRIs, CStringArray* psaProbeNames /*= NULL*/)
{
	int iLineIndex = 0;
	int iAddedItem = 0;

	g_DlgMain->BeginWaitCursor();
	for (int i=0; i<saMRIs.GetSize(); i++)
	{
		if (IsInList(saMRIs[i]))
		{
			continue;
		}

		CMRISeries *pMRISeries = m_pCryDlg->GetMRISeries(saMRIs[i]);
		if (pMRISeries)
		{
			int nItem = m_ctrlSeriesList.InsertItem(iLineIndex++, saMRIs[i]);
			if(pMRISeries->GetImageAt(0)->GetImageType()!=IMS_DCMCT)
			{
				MR_ImageHeader* pImageHeader = pMRISeries->GetImageAt(0)->GetImageHeader();
				if(strlen(pImageHeader->seriesField.Desc)>0)
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_DESC, pImageHeader->seriesField.Desc);
				}else
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_DESC, pImageHeader->studyField.Desc);
				}
			}else
			{
				CT_ImageHeader* pImageHeader = (CT_ImageHeader*)pMRISeries->GetImageAt(0)->GetImageHeader();
				if(strlen(pImageHeader->seriesField.Desc)>0)
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_DESC, pImageHeader->seriesField.Desc);
				}else
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_DESC, pImageHeader->studyField.Desc);
				}
			}
		
			
			if(m_pCryDlg->m_pDlgMain->m_bInSurgery) 
			{
				if (psaProbeNames == NULL)
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_PROBE, m_pCryDlg->m_dlgScannerControl.m_pCurRealProbe->GetName());
				}
				else
				{
					m_ctrlSeriesList.SetItemText(nItem, SL_PROBE, psaProbeNames->GetAt(i));
				}
			}
			iAddedItem ++;
		}
	}
	g_DlgMain->EndWaitCursor();
	
	this->PostMessage(WM_SERIES_CRASH_UPDATE);
	//CCrashRstore::SaveCrashState(CI_IMAGES);

	return iAddedItem;
}

int CDlgSeries::GetMRISeriesName(CStringArray& saMRIs, CStringArray* psaProbeNames /*= NULL*/, CStringArray* psaDesc /*= NULL*/)
{
	for (int i=0; i<m_ctrlSeriesList.GetItemCount(); i++)
	{
		saMRIs.Add(m_ctrlSeriesList.GetItemText(i, SL_NAME));
		if (psaProbeNames)
		{
			psaProbeNames->Add(m_ctrlSeriesList.GetItemText(i, SL_PROBE));
		}
		if (psaDesc)
		{
			psaDesc->Add(m_ctrlSeriesList.GetItemText(i, SL_DESC));
			CString sTmp=m_ctrlSeriesList.GetItemText(i, SL_DESC);
		}
	}
	return 0;
}


 // Identify whether MRI series name is in the list or not
BOOL CDlgSeries::IsInList(CString sMRISeriesName) 
{
		
	CStringArray saMRIs;
	int iNum = m_ctrlSeriesList.GetItemCount();
	for (int i=0; i<iNum; i++)
	{
		CString sName = m_ctrlSeriesList.GetItemText(i, SL_NAME);
		if (sName == sMRISeriesName)
			return TRUE;
	}
	return FALSE;
}

void CDlgSeries::OnBegindragLstSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(g_InOpenSeriesFromDBThread) /*正在使用另一线程打开数据库图像*/
	{
		*pResult=0;
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlSeriesList.GetFirstSelectedItemPosition();	
	if (pos)
	{
		int iItemIndex = m_ctrlSeriesList.GetNextSelectedItem(pos);
		CString sName = m_ctrlSeriesList.GetItemText(iItemIndex, SL_NAME);
		
		HGLOBAL hData = GlobalAlloc(GHND|GMEM_SHARE, sName.GetLength()+1 );
		char *pS = (char *)GlobalLock( hData );
		memcpy( pS, (LPCTSTR)sName, sName.GetLength()+1 );
		GlobalUnlock( hData );

		//m_bDraging = TRUE;	//开始拖动
		COleDataSource source;					  //用于启动一次拖放操作，并向系统提供拖放对象的数据
		source.CacheGlobalData( CF_TEXT, hData ); //使用即时方式提供数据，亦可用DelayRenderData指定延时方式提供数据
		source.DoDragDrop( DROPEFFECT_MOVE );     //开始实施拖放操作
		GlobalFree( hData );
		//m_bDraging = FALSE;	//完成拖动
	}
	
	*pResult = 0;
}

void CDlgSeries::OnKeydownLstSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(g_InOpenSeriesFromDBThread) /*正在使用另一线程打开数据库图像*/
	{
		*pResult=0;
		return;
	}

	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pLVKeyDow->wVKey == 46)
	{
		DeleteSelectedMRISeries();
	}
	
	*pResult = 0;
}

void CDlgSeries::DeleteMRIs(CStringArray& saMRIs)
{
	for (int i=0; i<saMRIs.GetSize(); i++)
	{
		CString sName = saMRIs[i];
		
		// Delete all of the items that begin with the string lpszmyString.
		LVFINDINFO info;
		int nIndex;
		info.flags = LVFI_STRING;
		info.psz = sName;
		if ((nIndex=m_ctrlSeriesList.FindItem(&info)) != -1)
		{
		   m_ctrlSeriesList.DeleteItem(nIndex);
		}

		// Delete image
		m_pCryDlg->DeleteMRISeries(sName);
	}	

	CCrashRstore::SaveCrashState(CI_IMAGES);
}

void CDlgSeries::DeleteSelectedMRISeries()
{
	POSITION pos = m_ctrlSeriesList.GetFirstSelectedItemPosition();	
	CStringArray saMRIs;
	for (;pos;)
	{
		int iItemIndex = m_ctrlSeriesList.GetNextSelectedItem(pos);
		CString sName = m_ctrlSeriesList.GetItemText(iItemIndex, SL_NAME);
		saMRIs.Add(sName);
	}
	
	if (saMRIs.GetSize() <= 0)
		return;
	
	CString sMsg = "Are you sure to delete  ";
	for (int i=0; i<saMRIs.GetSize()-1; i++)
	{
		sMsg += saMRIs[i] + ", ";
	}
	sMsg += saMRIs[i] + " ?";

	if (PanaMessageBox(sMsg, "Panasee", MB_OKCANCEL) != IDOK)
		return;

	DeleteMRIs(saMRIs);
}

int CDlgSeries::GetSelectedMRIName(CStringArray& saMRIs)
{
	POSITION pos = m_ctrlSeriesList.GetFirstSelectedItemPosition();	
	for (;pos;)
	{
		int iItemIndex = m_ctrlSeriesList.GetNextSelectedItem(pos);
		CString sName = m_ctrlSeriesList.GetItemText(iItemIndex, SL_NAME);
		saMRIs.Add(sName);
	}
	return saMRIs.GetSize();
}

BOOL CDlgSeries::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;     
	
	return CDlgSeriesSkin::PreTranslateMessage(pMsg);
}


void CDlgSeries::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LST_SERIES)
		m_ctrlSeriesList.MeasureItem(lpMeasureItemStruct);
	
	CDlgSeriesSkin::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CDlgSeries::OnDblclkLstSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	POSITION pos = m_ctrlSeriesList.GetFirstSelectedItemPosition();	
	if(pos)
	{
		int iItemIndex = m_ctrlSeriesList.GetNextSelectedItem(pos);
		CString sName = m_ctrlSeriesList.GetItemText(iItemIndex, SL_NAME);
		CMRISeries *pMRISeries = m_pCryDlg->GetMRISeries(sName);
		if(pMRISeries)
		{
			m_pCryDlg->m_pDlgMain->ShowSeriesDisplay(pMRISeries,FALSE);
		}
	}
}

void CDlgSeries::OnButtonDicomlist() 
{
	// TODO: Add your control notification handler code here
	CDlgMain *pDlgMain = g_DlgMain;

	pDlgMain->m_DICOMSrv.ShowWindow(TRUE);
}

void CDlgSeries::OnButtonFusion() 
{
	// TODO: Add your control notification handler code here
	CImageFusion fusion;
	fusion.ShowFusionWnd(this);

	//自动删除MPR后的序列，因为融合坐标改动，必须重新产生
	CString sTmp;
	CStringArray list;
	for (int i=0; i<m_ctrlSeriesList.GetItemCount(); i++)
	{
		sTmp = m_ctrlSeriesList.GetItemText(i, SL_NAME);
		if(sTmp.Find("MPR",0)>=0)
		{
			list.Add(sTmp);
		}
	}
	DeleteMRIs(list);

	//重新定位图像
	m_pCryDlg->TransformAllMRISeries();
}

void CDlgSeries::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDlgSeriesSkin::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CDlgMain *pDlgMain = g_DlgMain;
	if(bShow)
	{		
		pDlgMain->m_DICOMSrv.m_pMessageWnd=this;
	}
}
