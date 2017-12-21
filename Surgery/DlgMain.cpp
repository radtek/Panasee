// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMain.h"
#include "CrashRstore.h"
#include "DlgDispCompare.h"
#include "Log.h"
#include "XTToolkitPro.h"
#include "PanaGlobal.h"
#include "ConfigMgr.h"
#include "GlobalFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CString g_ExecPath;

OPERATIONTYPE gStarOperType = OPERATION_TYPE_NONE;    // get from command line argument           


/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog


CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CString	m_strStylesPath = g_ExecPath + _T("\\CommonSkin.cjstyles");
	XTPSkinManager()->LoadSkin(m_strStylesPath);

	m_enumOperationType = OPERATION_TYPE_NONE;

	CGlobalFuncs::StartLog();

	
	m_bInSurgery = FALSE;

	m_pBaseCaliDlg = NULL;

	m_DeviceType = DEVICE_IGS_MF;

	CConfigMgr conf;
	m_bHasCamera = conf.Read(SYSINFO, SYS_HAS_CAMERA, 1);
	m_bDebugNoCamera=!m_bHasCamera;
	m_bEnableMRICtrl = conf.Read(SYSINFO, SYS_HAS_MRI, 1);
	int iType = conf.Read(COM,"Tracker Type", 1);
	if(iType==1) 
	{
		m_CPManager.SetDeviceType(TRACK_DEVICE_NDI_OPTIC);
	}else if(iType==2)
	{
		m_CPManager.SetDeviceType(TRACK_DEVICE_NDI_EM);
	} 
}

CDlgMain::~CDlgMain()
{
	CGlobalFuncs::Clean();
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	//{{AFX_MSG_MAP(CDlgMain)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_TPS_GET_SERIES,OnPutSeries)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMain message handlers

LRESULT CDlgMain::OnPutSeries(WPARAM wParam, LPARAM lParam)
{
	char* lp = (char *)wParam;
	CWnd *pWnd=(CWnd*)lParam;
	CString sTmp;
	sTmp.Format("%s",lp);

	CMRISeries *pSeries=m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	if(pSeries)
	{
		pWnd->SendMessage(WM_TPS_RECEIVE_SERIES,(WPARAM)pSeries);
	}
	return 0;
}

BOOL CDlgMain::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;

	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	return TRUE;
}

BOOL CDlgMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDeviceType((PASK_DEVICE_TYPE)m_DeviceType);
	if(m_DeviceType == DEVICE_IGS_CP)
	{
		m_bEnableMRICtrl = FALSE;	
	}

	m_enumOperationType = gStarOperType;

	m_dlgCryotherapy.m_pDlgMain = this;
	m_dlgCryotherapy.SetOperType(m_enumOperationType);
	if(m_dlgCryotherapy.GetSafeHwnd()==NULL)
	{
		m_dlgCryotherapy.Create(IDD_DLG_CRYOTHERAPY,this);
	}
	m_dlgCryotherapy.ShowWindow(FALSE);
	m_dlgCryotherapy.m_pDisplay4Dlg = &m_display4Dlg;

	

	m_dlgWizard.SetOperType(m_enumOperationType);
	if (m_dlgWizard.GetSafeHwnd()==NULL)
	{
		m_dlgWizard.Create(CDlgWizard::IDD, this);
	}
	m_dlgWizard.ShowWindow(FALSE);
	m_dlgWizard.m_pDlgMain = this;

	if (m_display4Dlg.GetSafeHwnd()==NULL)
	{
		m_display4Dlg.Create(IDD_DLG_DISPLAY4,this);
	}
	m_display4Dlg.ShowWindow(FALSE);
	m_display4Dlg.m_pCryoDlg = &m_dlgCryotherapy;

	
	m_MRIWrap.m_pDlgMain = this;
	if (m_MRIWrap.GetSafeHwnd()==NULL)
	{
		m_MRIWrap.Create(CDlgMRIWrap::IDD, this);
	}
	
	m_MRIWrap.ShowWindow(FALSE);

	m_DICOMSrv.m_pDlgMain = this;
	if (m_DICOMSrv.GetSafeHwnd()==NULL)
	{
		m_DICOMSrv.Create(CDlgDICOMSrv::IDD, this);
	}
	
	m_DICOMSrv.ShowWindow(FALSE);

	if (m_dlgMRIBrowse.GetSafeHwnd() == NULL)
	{
		m_dlgMRIBrowse.Create(IDD_DLG_MRI_BROWSE, this);
		m_dlgMRIBrowse.m_pCryDlg = &m_dlgCryotherapy;
	}
	
	if(m_dlgTracking.GetSafeHwnd()==NULL)
	{
		m_dlgTracking.Create(CDlgTracking::IDD,this);
		m_dlgTracking.ShowWindow(FALSE);
	}

	if(m_dlgFusion.GetSafeHwnd() == NULL)
	{
		m_dlgFusion.Create(CDlgFusion::IDD,this);
		m_dlgFusion.ShowWindow(FALSE);
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText("Panasee - XinAoMDT");

	if (CCrashRstore::IsRestoredFromCrash())
	{
		if (PanaMessageBox("System detects that software exited improperly last time. \nDo you want to recover from last status?", MB_OKCANCEL) == IDOK)
		{
			CCrashRstore::UserConfirmRestore(TRUE);
		}
	}

	ShowWindow(SW_SHOWMAXIMIZED);


	m_dlgCryotherapy.SetOperType(m_enumOperationType);

	if (gStarOperType > OPERATION_TYPE_NONE && gStarOperType < OPERATION_TYPE_TPS)
	{
		m_dlgWizard.SetOperType(m_enumOperationType);
		m_dlgWizard.ShowWindow(TRUE);
		m_dlgWizard.SetStep(WSTEP_CAM, WSRET_OK);
	}
	else
	{
		//m_enumOperationType = OPERATION_TYPE_REVIEW;
		EnterSurgery();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMain::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	CRect rs;
//	GetClientRect(rs);

	if (m_display4Dlg.GetSafeHwnd())
	{
		m_display4Dlg.MoveWindow(0, 0, cx - 270, cy);
	}

	if (m_dlgTracking.GetSafeHwnd())
	{
		m_dlgTracking.MoveWindow(0, 0, cx - 270, cy);
	}

	if (m_dlgCryotherapy.GetSafeHwnd())
	{
		m_dlgCryotherapy.MoveWindow(cx - 270, 0, 270, cy);
		m_dlgCryotherapy.m_dlgTps.ChangeDisplaySize(0,0,cx - 270,cy);
	}

	if (m_dlgWizard.GetSafeHwnd())
	{
		m_dlgWizard.MoveWindow(0, 0, cx, cy);
	}
}

void CDlgMain::AddAMsgRcvWnd(CWnd* pWnd)
{
	m_lstMsgRvsWnds.AddTail(pWnd);
}

void CDlgMain::RemoveAMsgRcvWnd(CWnd* pWnd)
{
	if (m_lstMsgRvsWnds.Find(pWnd))
	{
		m_lstMsgRvsWnds.RemoveAt(m_lstMsgRvsWnds.Find(pWnd));
	}
}

void CDlgMain::BroadcastMsg(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	POSITION pos = m_lstMsgRvsWnds.GetHeadPosition();
	for (;pos;)
	{
		CWnd* pWnd = m_lstMsgRvsWnds.GetNext(pos);
		if (pWnd && pWnd->GetSafeHwnd())
		{
			::SendMessage(pWnd->GetSafeHwnd(), nMessage, wParam, lParam);
		}
	}
}

void CDlgMain::CheckPatientID(CMRISeries *pSeries)
{
	
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
	
	sID.TrimLeft();
	sID.TrimRight();
	sprintf(m_MRIWrap.m_CurPatient.Id,"%s",sID);
	sprintf(m_MRIWrap.m_CurPatient.Name,"%s",sName);
	m_MRIWrap.m_CurPatient.Weight = atoi(sWeight);
	sprintf(m_MRIWrap.m_CurPatient.Desc,"%s",sDesc);
	m_MRIWrap.m_CurPatient.Sex=nSex;
}

void CDlgMain::EnterSurgery()
{
	m_dlgCryotherapy.m_dlgPatient.Update();
	m_dlgWizard.ShowWindow(FALSE);
	m_dlgCryotherapy.ShowWindow(TRUE);
	m_display4Dlg.ShowWindow(TRUE);
	
	if (m_enumOperationType > OPERATION_TYPE_NONE && m_enumOperationType < OPERATION_TYPE_TPS )
	{
		m_dlgCryotherapy.m_dlgScannerControl.InitPreimageProbe();
		m_bInSurgery = TRUE;

		if(m_DeviceType == DEVICE_IGS_CP)
		{
			CMRISeries *pMRISeries=m_CPManager.GetGlobalSeries();
			if(pMRISeries)
			{
				Matrix mScanner2Marker=m_CPManager.GetGlobalSeriesCoord();
				pMRISeries->SetFusionCoord(mScanner2Marker.value);
				m_dlgCryotherapy.m_dlgNavigate.m_sSeriesName=pMRISeries->GetName();
				if(m_dlgCryotherapy.m_dlgNavigate.MPR(pMRISeries->GetName()))
				{
					m_dlgCryotherapy.m_dlgNavigate.m_sSeriesName=pMRISeries->GetName();
					
				}else
				{
					PanaMessageBox("Failed to reconstruct image. Navigation should not continue.");
				}
				m_dlgCryotherapy.TransformAllMRISeries();

				Matrix mFusion=pMRISeries->GetFusionCoord();
				m_dlgTracking.m_dlgTargeting.SetBase2Scanner(mFusion.Inverse());

				CheckPatientID(pMRISeries); //Ìî³ä²¡ÈËÐÅÏ¢
				m_dlgCryotherapy.m_dlgPatient.Update();
			}
		}
	}
	m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Initialized.");
	m_dlgCryotherapy.m_bRecordBase=TRUE; //ÔÊÐí¼ÇÂ¼baseÎ»ÖÃ
		

	if (CCrashRstore::IsUserConfirmedRestore())
	{
		CCrashRstore::RestoreCrashState();
	}
}

void CDlgMain::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	
	if (PanaMessageBox("Do you want to exit?", MB_ICONWARNING|MB_OKCANCEL ) != IDOK)
		return;

	m_DICOMSrv.Close();

	CCrashRstore::Clean();

	if(FindWindow(NULL,_T("Multi-Monitor")))
	{
		CWnd *pWnd=FindWindow(NULL,_T("Multi-Monitor"));
		pWnd->SendMessage(WM_CLOSE);
	}

	if (m_dlgCryotherapy.m_bTracking)
	{
		m_dlgCryotherapy.StopTracking(); //Í£Ö¹¸ú×Ù
		
	}

	m_dlgCryotherapy.m_dlgTime.SendMessage(WM_CLOSE);

	CLog::Log("User confirm exit. Panasee closed.");
	CLog::StopLog();
	
	CDialog::OnClose();
}


void CDlgMain::ShowSeriesDisplay(CMRISeries *pSeries,BOOL bValidCryDlg)
{
	m_dlgMRIBrowse.SetMRISeries(pSeries);
	if(bValidCryDlg)
	{
		m_dlgMRIBrowse.m_pCryDlg = &m_dlgCryotherapy;
	}else
	{
		m_dlgMRIBrowse.m_pCryDlg = NULL;
	}
	m_dlgMRIBrowse.ShowWindow(TRUE);

	/*BOOL bShow = m_dlgMRIBrowse.IsWindowVisible();
	if (!bShow)
	{
		if (m_dlgCryotherapy.m_pCurSelSeries)
		{
			m_dlgMRIBrowse.SetMRISeries(m_dlgCryotherapy.m_pCurSelSeries);
		}
		else
		{
			m_dlgMRIBrowse.SetMRISeries(m_dlgCryotherapy.m_MRIBuffer.GetDisp());
		}
	}
	m_dlgMRIBrowse.ShowWindow(!bShow);*/
}

void CDlgMain::ShowCompareDlg()
{
	CCompareFrame dlgCompare;
	
	dlgCompare.m_DlgDisp.m_DlgCmpDispWrap[0].m_dlgDisplay.m_pCryotherapyDlg = &m_dlgCryotherapy;
	dlgCompare.m_DlgDisp.m_DlgCmpDispWrap[1].m_dlgDisplay.m_pCryotherapyDlg = &m_dlgCryotherapy;
	dlgCompare.m_DlgDisp.m_DlgCmpDispWrap[2].m_dlgDisplay.m_pCryotherapyDlg = &m_dlgCryotherapy;
	dlgCompare.m_DlgDisp.m_DlgCmpDispWrap[3].m_dlgDisplay.m_pCryotherapyDlg = &m_dlgCryotherapy;

	CStringArray saMris,saDesc;
	m_dlgCryotherapy.m_dlgSeries.GetMRISeriesName(saMris,NULL,&saDesc);
	dlgCompare.Init(saMris,saDesc);
	dlgCompare.DoModal();
		
}



