// DlgImportPatient.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgImportPatient.h"
#include "dlgMain.h"
#include "ConfigMgr.h"
#include "config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImportPatient dialog


CDlgImportPatient::CDlgImportPatient(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgImportPatient::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImportPatient)
	m_sCaliResult = _T("");
	//}}AFX_DATA_INIT
	
	m_pNeedTobeCalibratedSeries=NULL;
	m_bInCalibrationProcess=FALSE;
}


void CDlgImportPatient::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImportPatient)
	DDX_Control(pDX, IDC_STATIC_RESULT, m_staticResult);
	DDX_Control(pDX, IDC_LIST_SERIES, m_seriesList);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_sCaliResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImportPatient, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgImportPatient)
	ON_BN_CLICKED(IDC_BUTTON_DICOM, OnButtonDicom)
	ON_BN_CLICKED(IDC_BUTTON_DB, OnButtonDb)
	ON_BN_CLICKED(IDC_BUTTON_LOCAL, OnButtonLocal)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_CLICK, IDC_LIST_SERIES, OnClickListSeries)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, OnButtonManual)
	ON_MESSAGE(WM_NEW_SERIES, OnMsgNewSeries)
	ON_MESSAGE(WM_DICOM_IMAGECOUNT,OnDicomImageCount)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImportPatient message handlers

LRESULT CDlgImportPatient::OnDicomImageCount(WPARAM wParam, LPARAM lParam)
{
	int *p=(int*)wParam;
	int count=*p;
	CString sTmp;
	sTmp.Format("Dicom %i",count);
	GetDlgItem(IDC_BUTTON_DICOM)->SetWindowText(sTmp);
	return 0;
}

LRESULT CDlgImportPatient::OnMsgNewSeries(WPARAM wParam, LPARAM lParam)
{
	if(!IsWindowVisible())
		return 0;

	UpdateList();
	AutoCalibrate();	
	return 0;
}

void CDlgImportPatient::OnButtonDicom() 
{
	// TODO: Add your control notification handler code here
	CDlgMain *pDlgMain = g_DlgMain;

	pDlgMain->m_DICOMSrv.ShowWindow(TRUE);
}

void CDlgImportPatient::OnButtonDb() 
{
	// TODO: Add your control notification handler code here
	CDlgMain *pDlgMain = g_DlgMain;
	pDlgMain->m_dlgCryotherapy.m_dlgCmdBar.OpenDatabase();
}

void CDlgImportPatient::OnButtonLocal() 
{
	// TODO: Add your control notification handler code here
		
	CDlgMain *pDlgMain = g_DlgMain;
	if(pDlgMain->m_dlgCryotherapy.m_dlgCmdBar.SysImportSeries())
	{
		m_staticResult.SetWindowText("Loading Series...");
	}
}


void CDlgImportPatient::OnButtonConfirm() 
{
	// TODO: Add your control notification handler code here
	if(m_pCPManager->GetGlobalSeries()==NULL)
	{
		return;
	}
	CDlgMain* pDlgMain = g_DlgMain;

	if(!m_pCPManager->IsCalibrated())
	{
		ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.SetStep(WSTEP_CAL, WSRET_OK);
		return;
	}
	
	ShowWindow(FALSE);
	pDlgMain->m_dlgCryotherapy.m_tranScanner2Marker = m_pCPManager->GetGlobalSeriesCoord();
	pDlgMain->m_dlgCryotherapy.CreateTransform();
	pDlgMain->m_dlgCryotherapy.InitFrameBoxPosition();
	pDlgMain->m_dlgCryotherapy.TransformAllMRISeries();
	
	::SendMessage(pDlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_OK);
	
}

BOOL CDlgImportPatient::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDlgMain *pDlgMain = g_DlgMain;
	m_pCPManager=&pDlgMain->m_CPManager;

	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_seriesList.SetExtendedStyle(dwExStyle);
	m_seriesList.InsertColumn( 0, "Patient Name", LVCFMT_LEFT, 140);
	m_seriesList.InsertColumn( 1, "Description", LVCFMT_LEFT, 310);
	m_seriesList.InitHeader();

	int item=m_seriesList.InsertItem(0, " ");
	m_seriesList.SetItemText(item, 1, " ");

	LOGFONT logFont;
	m_staticResult.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = logFont.lfHeight * 2;
	
	cTextFont.CreateFontIndirect(&logFont);
	m_staticResult.SetFont(&cTextFont);

	CString sFilePath = CONFIG_FILES[CF_MARKER2PT];
	double **pMarker;
	pMarker=new double*[4];
	for(int i=0;i<4;i++)
	{
		pMarker[i]=new double [4];
	}
	if(!pDlgMain->m_dlgCryotherapy.LoadTransformation(sFilePath,pMarker))
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_dMarker2PT[i][j]=0;
			}
		}

		m_dMarker2PT[0][0]=1;
		m_dMarker2PT[1][1]=1;
		m_dMarker2PT[2][2]=1;
		m_dMarker2PT[3][3]=1;
	}else
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_dMarker2PT[i][j]=pMarker[i][j];
			}
		}
	}
	for(i=0;i<4;i++)
	{
		delete pMarker[i];
	}
	delete pMarker;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT gIGSCP_Calibration(LPVOID pParam )
{
	CDlgImportPatient *pDlg=(CDlgImportPatient*)pParam;
	if(pDlg==NULL)
		return 0;
	pDlg->m_bInCalibrationProcess=TRUE;

	try
	{
		pDlg->CalibrationProcess();
		
		pDlg->m_bInCalibrationProcess=FALSE;
		AfxEndThread(0,TRUE);
		return 0;
	}catch(...)
	{
		pDlg->m_bInCalibrationProcess=FALSE;
		AfxEndThread(-1,TRUE);
		return -1;
	}

	pDlg->m_bInCalibrationProcess=FALSE;
	AfxEndThread(0,TRUE);
	return 0;
}

void CDlgImportPatient::AutoCalibrate()
{
	CDlgMain *pDlgMain = g_DlgMain;
	m_pCPManager->SetGlobalSeries(NULL);
	m_pCPManager->SetGlobalSeriesCoord(NULL);

	m_sCaliResult="Please select series.";
	UpdateData(FALSE);
	POSITION pos = m_seriesList.GetFirstSelectedItemPosition();	
	if (pos)
	{
		m_sCaliResult="";
		UpdateData(FALSE);

		int iItemIndex = m_seriesList.GetNextSelectedItem(pos);
		CString sName = m_seriesList.GetItemText(iItemIndex, 1);
		int iFind=sName.Find("(",0);
		if(iFind<=0)
		{
			return;
		}
		sName=sName.Left(iFind);
		sName.TrimRight();
		sName.TrimLeft();
		if(sName=="")
		{
			return;
		}

		CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(sName);
		if(!pMRISeries)
		{
			return;
		}

		m_sCaliResult="Calibrating...";
		UpdateData(FALSE);
		m_pNeedTobeCalibratedSeries=pMRISeries;
		if(!m_bInCalibrationProcess)
		{
			AfxBeginThread(gIGSCP_Calibration,(LPVOID)this,THREAD_PRIORITY_LOWEST,0,0,NULL);
		}
	}
	

	
}

void CDlgImportPatient::OnDestroy() 
{
	CPanaseeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDlgImportPatient::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LIST_SERIES)
		m_seriesList.MeasureItem(lpMeasureItemStruct);
	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CDlgImportPatient::UpdateList()
{
	CDlgMain *pDlgMain = g_DlgMain;
	CStringArray saMRIs;
	pDlgMain->m_dlgCryotherapy.m_dlgSeries.GetMRISeriesName(saMRIs,NULL,NULL);
	
	m_seriesList.DeleteAllItems();
	CString sTmp;
	for (int i=0; i<saMRIs.GetSize(); i++)
	{
		if(saMRIs[i].Find("MPR",0)>=0)
		{
			continue; //MPR结果不作为标定序列使用
		}
		CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(saMRIs[i]);
		if(!pMRISeries)
			continue;
		if(pMRISeries->GetImageAt(0)->GetImageType()!=IMS_DCMCT)
		{
			MR_ImageHeader* pImageHeader = pMRISeries->GetImageAt(0)->GetImageHeader();
			sTmp.Format("%s",pImageHeader->patientField.Name);
			int item=m_seriesList.InsertItem(m_seriesList.GetItemCount(), sTmp);
			sTmp.Format("%s(%i.%i.%i) %i slices(%.2fmm x %.2fmm)",saMRIs[i],
				pImageHeader->studyField.Time.year,pImageHeader->studyField.Time.month,
				pImageHeader->studyField.Time.day,pMRISeries->GetNumOfImages(),
				pMRISeries->GetImageAt(0)->GetFOVX()/pMRISeries->GetImageAt(0)->GetResolutionX(),
				pMRISeries->GetImageAt(0)->GetFOVY()/pMRISeries->GetImageAt(0)->GetResolutionY());
			m_seriesList.SetItemText(item, 1, sTmp);
		}else
		{
			CT_ImageHeader* pImageHeader = (CT_ImageHeader*)pMRISeries->GetImageAt(0)->GetImageHeader();
			sTmp.Format("%s",pImageHeader->patientField.Name);
			int item=m_seriesList.InsertItem(m_seriesList.GetItemCount(), sTmp);
			sTmp.Format("%s(%i.%i.%i) %i slices(%.2fmm x %.2fmm)",saMRIs[i],
				pImageHeader->studyField.Time.year,pImageHeader->studyField.Time.month,
				pImageHeader->studyField.Time.day,pMRISeries->GetNumOfImages(),
				pMRISeries->GetImageAt(0)->GetFOVX()/pMRISeries->GetImageAt(0)->GetResolutionX(),
				pMRISeries->GetImageAt(0)->GetFOVY()/pMRISeries->GetImageAt(0)->GetResolutionY());
			m_seriesList.SetItemText(item, 1, sTmp);
		}
	}
	if(m_seriesList.GetItemCount()<=0)
	{
		int item=m_seriesList.InsertItem(0, " ");
		m_seriesList.SetItemText(item, 1, " ");
	}else
	{
		m_seriesList.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CDlgImportPatient::OnClickListSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	AutoCalibrate();
	*pResult = 0;
}

void CDlgImportPatient::CalibrationProcess()
{
	
	Matrix m(4,4);
	Matrix mMarker2PT(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mMarker2PT.value[i][j]=m_dMarker2PT[i][j];
		}
	}
	
	while(m_pNeedTobeCalibratedSeries!=NULL)
	{
		CMRISeries *pMRISeries=m_pNeedTobeCalibratedSeries;
		m_pNeedTobeCalibratedSeries=NULL;
		if(m_pCPManager->Calibrate(pMRISeries,m))
		{
			/*
			//test
			CDlgMain* pDlgMain = g_DlgMain;
			Matrix m1(4,4),m2(4,4);
			pDlgMain->m_dlgCryotherapy.LoadTransformation("d:\\Scanner2Marker.txt",m1.value);
			pDlgMain->m_dlgCryotherapy.LoadTransformation("D:\\nsj\\data\\2\\fusion_8636.2.mtx",m2.value);
			m2 = m2.Inverse();
			m1 = m1 * m2;
			m1 = m1.Inverse();
			pDlgMain->m_dlgCryotherapy.SaveTransformation("d:\\Marker2PT.txt",m1.value);
			*/

			Matrix mScanner2Marker=m;
			Matrix mScanner2PT = mMarker2PT * mScanner2Marker;
			m_pCPManager->SetGlobalSeries(pMRISeries);
			m_pCPManager->SetGlobalSeriesCoord(&mScanner2PT);
						
			m_sCaliResult="Calibration is ready.";
		}else
		{
			m_pCPManager->SetGlobalSeries(pMRISeries);
			m_pCPManager->SetGlobalSeriesCoord(NULL);
			m_sCaliResult="Calibrated is not ready.\nTry it in the next step.";
		}
		CString sTmp;
		sTmp.Format("%s %s",pMRISeries->GetName(),m_sCaliResult);
		m_sCaliResult=sTmp;
	}
	m_staticResult.SetWindowText(m_sCaliResult);
}

void CDlgImportPatient::OnButtonManual() 
{
	// TODO: Add your control notification handler code here
	if(m_pCPManager->GetGlobalSeries()==NULL)
	{
		return;
	}
	CDlgMain* pDlgMain = g_DlgMain;
			
	ShowWindow(FALSE);
	pDlgMain->m_dlgWizard.SetStep(WSTEP_CAL, WSRET_OK);
	return;
	
}

void CDlgImportPatient::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CDlgMain *pDlgMain = g_DlgMain;
	if(bShow)
	{
		
		pDlgMain->m_DICOMSrv.m_pMessageWnd=this;
	}else
	{
		pDlgMain->m_DICOMSrv.m_pMessageWnd = (CWnd*)&pDlgMain->m_dlgCryotherapy.m_dlgSeries;
	}
}
