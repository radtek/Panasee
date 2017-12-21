// CryotherapyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "CryotherapyDlg.h"
#include "CommandHandling.h"
#include "Dispaly4Dlg.h"
#include "Probe.h"
#include "ConfigMgr.h"
#include "MRISeries.h"
#include "DlgSimulate.h"
#include "ColorF.h"
#include "ErrorCodes.h"
#include "Texture2D.h"
#include "DragData.h"
#include "TrackTool.h"
#include "TCPCom.h"
#include <winsock2.h>	
#include <ws2tcpip.h>
#include "DlgScannerControl.h"
#include "CompareFrame.h"
#include "DlgDispCompare.h"
#include "GlobalFuncs.h"
#include "DlgCaliPT.h"
#include "Config.h"
#include "CrashRstore.h"
#include "DlgMain.h"
#include "Log.h"
#include "PanaGlobal.h"
#include "SeriesLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CImriDBI m_DBmri;

#define EXPORT_BINARY 0



/////////////////////////////////////////////////////////////////////////////
// CCryotherapyDlg dialog


bool m_bStopSphereSimulate, m_bIsSimulating;


UINT TrackingProcess(LPVOID pParam); /* TrackingProcess thread declaration */
UINT SphereSimulating(LPVOID pParam);

CCryotherapyDlg::CCryotherapyDlg(CWnd* pParent /*=NULL*/)
: CDlgMainSkin(CCryotherapyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCryotherapyDlg)
	//}}AFX_DATA_INIT
		
	m_pTrackerHandler=NULL;

	m_iProbeHandle=-1;
		
	m_patientMarker = PATIENT_NO_MARKER;	// For two modes of navigation // Added by Wei Wei [2007.1.30]
	m_TableShiftMode = NO_SHIFT_IN_SCANING; 
	
	m_pCurSelSeries =NULL;
	
	
	m_dbCurrentPTPos = 0.0;
	m_dbRealPTPos = 0.0;
	m_vPTDirInScan.x = m_vPTDirInScan.y = m_vPTDirInScan.z = 0;
	m_vPTMovInTracker.x = m_vPTMovInTracker.y = m_vPTMovInTracker.z = 0;
	
	for (int i=0; i<MAX_BASE_HANDLES; i++)
	{
		m_bAllBaseVisible[i] = FALSE;
	}
	
	m_TargetPool.m_pCryoDlg = this;

	m_bRecordBase=FALSE;
	m_bPromptCameraMove=FALSE;

	m_enumOperateType = OPERATION_TYPE_REVIEW;
		
	m_iCurBaseIndex=0;

	m_updateRate=160;

	m_bDlgInitialized=FALSE;
}

CCryotherapyDlg::~CCryotherapyDlg()
{
				
	CleanTrackingTool();
	
}


void CCryotherapyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgMainSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCryotherapyDlg)
	//	DDX_Control(pDX, IDC_MRI_LIST, m_ctlMRIList);
	//	DDX_Control(pDX, IDC_TRACKING_LIST, m_ctlTrackList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCryotherapyDlg, CDlgMainSkin)
//{{AFX_MSG_MAP(CCryotherapyDlg)
ON_WM_SHOWWINDOW()
ON_MESSAGE(WM_TRACKING, NDITracking)
ON_MESSAGE(WM_STOPTRACKING_FROMTHREAD,StopTrackingFromThread)
ON_MESSAGE(WM_SIMULATE_SPHERE, SimulateSphere)
ON_COMMAND(IDC_POPUP1_CHANGE_IMG_COORD, OnPopup1ChangeImgCoord)
ON_COMMAND(ID_PROBE_SIMULATE_POP_DLG, OnProbeSimulatePopDlg)
ON_COMMAND(ID_PROBE_SIMULATE, OnProbeSimulate)
ON_COMMAND(ID_PROBE_STOP_SIMULATE, OnProbeStopSimulate)
ON_COMMAND(POPUP1_DISP_2D_ONE, OnDisp2dOne)
ON_COMMAND(POPUP1_DISP_2D_FOUR, OnDisp2dFour)
ON_COMMAND(POPUP1_DISP_2D_THREE, OnDisp2dThree)
ON_COMMAND(POPUP1_DISP_2D_TWO, OnDisp2dTwo)
ON_WM_SIZE()
ON_WM_TIMER()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryotherapyDlg message handlers

void CCryotherapyDlg::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE */)
{
	CDialog::MoveWindow(x,y, nWidth,nHeight,bRepaint);
	
	
	CRect rect;
	int iTop = 0;
	
	GetClientRect(&rect);
	int iWndHeight = rect.Height();
	int iWndWidth = rect.Width();
	
	
}

BOOL CCryotherapyDlg::OnInitDialog() 
{
	CDlgMain* pDlgMain =m_pDlgMain;
	if(pDlgMain->m_DeviceType==DEVICE_IGS_CP)
	{
		SetUIMode(2);
	}else
	{
		SetUIMode(1);
	}

	CDlgMainSkin::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	InitializeCriticalSection(&m_csLoadSeries);
	m_dropEx.Register(this);
	
	m_pDisplay4Dlg = &(g_DlgMain->m_display4Dlg);
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_pCryotherapyDlg = this;
	m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.m_pCryotherapyDlg = this;
	m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.m_pCryotherapyDlg = this;
	m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.m_pCryotherapyDlg = this;
	m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.m_pCryotherapyDlg = this;
	
	if (m_dlgCmdBar.GetSafeHwnd() == NULL)
	{
		m_dlgCmdBar.Create(IDD_CMD_BAR, this);
	}
	m_dlgCmdBar.ShowWindow(TRUE);
	m_dlgCmdBar.m_pCryDlg = this;
	
	if (m_dlgPatient.GetSafeHwnd() == NULL)
	{
		m_dlgPatient.Create(IDD_DLG_PATIENT, this);
	}
	m_dlgPatient.ShowWindow(FALSE);
	
	if (m_dlgSeries.GetSafeHwnd() == NULL)
	{
		m_dlgSeries.Create(IDD_DLG_SERIES, this);
	}
	m_dlgSeries.ShowWindow(FALSE);
	m_dlgSeries.m_pCryDlg = this;
	
	m_dlgScannerControl.m_pCryDlg = this;
	if (m_dlgScannerControl.GetSafeHwnd() == NULL)
	{
		m_dlgScannerControl.Create(IDD_DLG_SCANNER_CONTROL, this);
	}
	m_dlgScannerControl.ShowWindow(FALSE);
	//	m_dlgScannerControl.SetParent(this);
	
	m_dlgTrackingStatus.m_pCryoDlg = this;
	if(m_dlgTrackingStatus.GetSafeHwnd()==NULL)
	{
		m_dlgTrackingStatus.Create(IDD_DLG_TRACKING_STATUS,this);
	}
	m_dlgTrackingStatus.ShowWindow(TRUE);
		
	if (m_dlgTitle.GetSafeHwnd() == NULL)
	{
		m_dlgTitle.Create(IDD_DLG_TITLE, this);
	}
	m_dlgTitle.ShowWindow(TRUE);
	
	if (m_dlgTime.GetSafeHwnd() == NULL)
	{
		m_dlgTime.Create(IDD_DLG_TIME, this);
	}
	//	m_dlgTime.SetParent(this);
	m_dlgTime.ShowWindow(TRUE);

	if (m_dlgNavigate.GetSafeHwnd() == NULL)
	{
		m_dlgNavigate.Create(IDD_DLG_NAVIGATE, this);
	}
	m_dlgNavigate.ShowWindow(FALSE);

	if (m_dlgTps.GetSafeHwnd() == NULL)
	{
		m_dlgTps.Create(IDD_DLG_TPS, this);
	}
	m_dlgTps.ShowWindow(FALSE);
	
	
	m_sMRIStorageFolder = m_DBmri.GetDatabaseFolder();
	
	m_bStopTracking = FALSE;
	m_bTracking = FALSE;
	
	m_bScanningPosRecorded = FALSE;
	m_bBaseVisible = FALSE; 
	m_bMarkerVisible = FALSE;
	m_bProbeVisible = FALSE;
	m_bPhantomVisible = FALSE;
	
	m_hTrackingStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTrackingStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	m_bStopSphereSimulate = FALSE;
	m_bIsSimulating = FALSE;
	
	m_bPreSurgeryCaliDone = FALSE;
	
	CConfigMgr conf;
	CString sIndex;
	CString sName;
	
	m_ppTrackTool = NULL;
	m_iMaxNumOfTrackTool = 0;
	
	
	m_bTracking = FALSE;
	m_uiFlag = FLAG_NONE;
	m_iDataCollectionHandlerID = -1;
	
	m_pMRISeries = NULL;
	
	
	m_tBase.SetDimension(4,4);
	m_tBase.Identity();
	m_tBaseLast.SetDimension(4,4);
	m_tBaseLast.Identity();
	m_tPhatom.SetDimension(4,4);
	m_tPhatom.Identity();
	m_tProbe.SetDimension(4,4);
	m_tProbe.Identity();
	m_tMarker.SetDimension(4,4);
	m_tMarker.Identity();
	m_tranScanner2Base.SetDimension(4,4);
	m_tranScanner2Base.Identity();	
	m_tranScanner2Marker.SetDimension(4,4); 
	m_tranScanner2Marker.Identity();
	m_tranScanner2Tracker.SetDimension(4,4);
	m_tranScanner2Tracker.Identity();
	
		
	for (int i=0; i<MAX_BASE_HANDLES; i++)
	{
		m_baseHandles[i].bh_mToBase0.SetDimension(4,4);
		m_baseHandles[i].bh_mToBase0.Identity();
		m_baseHandles[i].bh_Handle = -1;
		m_baseHandles[i].bh_BaseIndex = i;
	}
	
	m_iSelectHandle = -1;
	
	
	
	//	CString sFile = conf.Read(SYSINFO, SYS_PT_DIR_FILE, "")
	CString sFile = CONFIG_FILES[CF_PT_DIR];
	if (!LoadPatientTableDirFile(sFile))
	{
		m_dlgCmdBar.GetDlgItem(IDC_CMD_MOVE_PT)->EnableWindow(FALSE);
		m_dlgCmdBar.m_bTableCalibrated = FALSE;
	}else
	{
		m_dlgCmdBar.m_bTableCalibrated = TRUE;
	}
	
	m_patientMarker = (PATIENT_MARKER)conf.Read(SYSINFO, SYS_NAVI_MODE, 0);
	m_TableShiftMode = (TABLE_SHIFT_MODE)conf.Read(SYSINFO, SYS_AUTO_TABLE_SHIFT, NO_SHIFT_IN_SCANING);
	
	if(m_patientMarker!=PATIENT_NO_MARKER && m_patientMarker!=PATIENT_WITH_MARKER )
	{
		m_patientMarker=PATIENT_NO_MARKER;
	}
	
	if(pDlgMain->m_DeviceType==DEVICE_IGS_CP) //病床上必须有Marker
	{
		m_patientMarker=PATIENT_WITH_MARKER;
	}

	if(m_TableShiftMode!=NO_SHIFT_IN_SCANING && m_TableShiftMode!=AUTO_SHIFT_IN_SCANING )
	{
		m_TableShiftMode=NO_SHIFT_IN_SCANING;
	}
	
	m_mPopupMenu.LoadMenu(IDR_POP_MENU);
			
	m_bModePreCali = FALSE;
	
	//	m_pCurTrackingProbe = NULL;
	//	
	//	m_pProbeControlMRIScanner = NULL;
	
	for (i=0; i<5; i++)
	{
		CString sName;
		sName.Format("RS%02d", i);
		m_MRIRectSeries[i].SetName(sName);
	}
	
	
	SetTimer(STATUSBAR_UPDATE_TIMER,MY_ELAPSE200,NULL);
	
	GetTabPosition(PASK_Tab_Patient,m_rcTabPat);
	GetTabPosition(PASK_Tab_Series,m_rcTabSeries);
	GetTabPosition(PASK_Tab_MriControl,m_rcTabMriCtrl);
	GetTabPosition(PASK_Tab_Navigate,m_rcTabNavCtrl);
	GetTabPosition(PASK_Tab_Tps,m_rcTabTpsCtrl);
	
	m_bDlgInitialized=TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCryotherapyDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_pDisplay4Dlg->ShowWindow(bShow);
	if (bShow)
	{
		//		InitTrackingList();
		
		if (m_pDlgMain->m_MRIWrap.m_bProtocolsReceived)//添加Body
		{
			
			m_dlgScannerControl.CreateProtocolMenu(m_ProtMng.GetBdyPrtLst());
			m_pDlgMain->m_MRIWrap.m_bProtocolsReceived = FALSE;
		}
	}
}


/*****************************************************************
Name:				nStartTracking

  Inputs:
  None.
  
	Return Value:
	int - 1 if successful, 0 otherwise
	
	  Description:   This routine starts the System tracking
*****************************************************************/
int CCryotherapyDlg::StartTracking() 
{
	if(m_bTracking)
		return 1;

	m_bTracking=FALSE;

	CConfigMgr conf;
		
	for (int i=0; i<MAX_HANDLES; i++)
		m_lFrameNo[i] = 0;
		
	m_lstTrackingData.RemoveAll();	
	
	if (m_pTrackerHandler && m_pTrackerHandler->StartTracking())
	{
	/*
			 * if we can start tracking, set the appropriate window text,
			 * start the tracking thread and set the mode.
		*/
		//GetDlgItem( IDC_CRYO_TRACKING )->SetWindowText("Stop Tracking");
		
		for (int i=0; i<MAX_HANDLES; i++)
			m_lFrameNo[i] = 0;
		
		
		ResetEvent(m_hTrackingStartEvent);

		AfxBeginThread(TrackingProcess,(LPVOID)this,THREAD_PRIORITY_LOWEST,0,0,NULL);
		
		WaitForSingleObject(m_hTrackingStartEvent, INFINITE);
		
		m_lstTrackingData.RemoveAll();
		
		
		CLog::Log("Start tracking success.");
		
		return 1;
	} /* if */
	
	BOOL bDebugNoCamera = m_pDlgMain->m_bDebugNoCamera;
	if (!bDebugNoCamera)
	{
		PanaMessageBox("Start tracking failed. Please check hardware.");
	}
	CLog::Log("Start tracking failed.");
	return 0;
} /* nStartTracking */


  /*****************************************************************
  Name:				StopTracking
  
	Inputs:
	None.
	
	  Return Value:
	  int - 1 if successful, 0 otherwise
	  
		Description:   
		This routine stops the tracking procedure.
*****************************************************************/
int CCryotherapyDlg::StopTracking() 
{
	if (!m_bTracking)
		return 0;
	
		
	ResetEvent(m_hTrackingStopEvent);
			
	m_bStopTracking = TRUE;
	/*等待线程结束*/
	WaitForSingleObject(m_hTrackingStopEvent, INFINITE);
	
	if(!m_pTrackerHandler->StopTracking())
	{
		CLog::Log("Stop tracking failed. Thread exit");
		m_dlgTrackingStatus.AddMessage("Stop tracking failed. Thread exit");
		
	}else
	{
		CLog::Log("Stop tracking success.");
	}
		
		
	m_bTracking = FALSE;

	m_bBaseVisible = FALSE;
	m_bMarkerVisible = FALSE;
	m_bProbeVisible = FALSE;
	m_bPhantomVisible = FALSE;
	for (int iHandle = 0; iHandle<MAX_BASE_HANDLES; iHandle++)
	{
		m_bAllBaseVisible[iHandle] = FALSE;
	}
	
	return 0;
} /* nStopTracking */



/*****************************************************************
Name:				TrackingProcess

  Inputs:
  input LPVOID pParam - normal thread input
  
	Return Value:
	UINT - normal thread return
	
	  Description:  
	  This is the thread call that controls the collection of data
	  
		This thread allows the user to perform other tasks
		within the program while data is being collected
*****************************************************************/
UINT TrackingProcess( LPVOID pParam)
{
	//	HWND hWnd = (HWND)pParam;
	CCryotherapyDlg* pCryDlg = (CCryotherapyDlg*)pParam;
		
	pCryDlg->m_bTracking = TRUE;
	pCryDlg->m_bStopTracking = FALSE;
	int nCnt = 0;
	pCryDlg->m_nGetTXTransformsErrorCount = 0;
	BOOL bSendOK=FALSE;
	while ( !pCryDlg->m_bStopTracking )
	{
				
		//::SendMessage( pCryDlg->m_hWnd, WM_TRACKING, 0, 0 );
		try
		{
			pCryDlg->NDITracking();
		}catch(...)
		{
			int i=0;
		}
		nCnt ++;
		
		Sleep(pCryDlg->m_updateRate);   // Tracking update rate
		if (nCnt >2 && bSendOK==FALSE)
		{
			SetEvent(pCryDlg->m_hTrackingStartEvent);
			bSendOK=TRUE;
		}
	}/* while */
	
	pCryDlg->m_bTracking = FALSE;

	/* when tracking stopped, kill the thread */
	pCryDlg->m_bStopTracking = FALSE;
	SetEvent(pCryDlg->m_hTrackingStopEvent);

	
	
	
	AfxEndThread( 0, TRUE );
	return 0;
} 

LONG CCryotherapyDlg::StopTrackingFromThread( UINT wParam, LONG lParam )
{
	StopTracking();
	CLog::Log("Failed to connect camera.");
	m_dlgTrackingStatus.AddMessage("Failed to connect camera.");
	m_dlgCmdBar.UpdateUI();
	PanaMessageBox("Failed to connect camera.");
	return 0;
}


/*****************************************************************
Name:				NDITracking

  Inputs:
  UNIT wParam and LONG lParam are normal message handling inputs
  
	Return Value:
	LONG is the normal return for a message handling routine
	
	  Description:   
	  This routine is the message handler for a WM_TRACKING posting.
	  This routine gets the next set of transformation data and displays
	  it in the main dialog.
*****************************************************************/
LONG CCryotherapyDlg::NDITracking( UINT wParam, LONG lParam )
{
	//NDITracking();
	return 1;
}

int CCryotherapyDlg::NDITracking()
{
	CDlgMain* pDlgMain =m_pDlgMain;
	if(pDlgMain==NULL)
		return 0;
	
	CString		szCBHandle,	szPortNo;
	int			nRow = -1;
	QUAT        mFlagMatrixQuat;
	Matrix		mFlagMatrix;		// Marker origin position and orientation
	
	if (!m_bTracking)
		return 0;
	
		
	BOOL bBaseVisible = FALSE; 
	BOOL bMarkerVisible = FALSE;
	BOOL bProbeVisible = FALSE;
	BOOL bPhantomVisible = FALSE;
	BOOL bAllBaseVisible[MAX_BASE_HANDLES];
	for (int iHandle = 0; iHandle<MAX_BASE_HANDLES; iHandle++)
	{
		bAllBaseVisible[iHandle] = FALSE;
	}
	
	if(m_nGetTXTransformsErrorCount>0)
	{
		m_bBaseVisible=bBaseVisible;
		m_bMarkerVisible=bMarkerVisible;
		m_bProbeVisible=bProbeVisible;
		m_bPhantomVisible=bPhantomVisible;
		for (int iHandle = 0; iHandle<MAX_BASE_HANDLES; iHandle++)
		{
			m_bAllBaseVisible[iHandle] = bAllBaseVisible[iHandle];
		}
		
		return 0;
	}
		
	if ( !m_pTrackerHandler->RefreshMatrix()) 
	{
		m_nGetTXTransformsErrorCount ++;
		

		if (m_nGetTXTransformsErrorCount == 1)
		{
			this->PostMessage(WM_STOPTRACKING_FROMTHREAD);
		}
		
		m_bBaseVisible=bBaseVisible;
		m_bMarkerVisible=bMarkerVisible;
		m_bProbeVisible=bProbeVisible;
		m_bPhantomVisible=bPhantomVisible;
		for (int iHandle = 0; iHandle<MAX_BASE_HANDLES; iHandle++)
		{
			m_bAllBaseVisible[iHandle] = bAllBaseVisible[iHandle];
		}
		return 0;
	}
	
	
			
	for ( int i = 0; i < MAX_HANDLES; i ++ )
	{
		if ( m_pTrackerHandler->IsValidTool(i))
		{			
			if ( m_pTrackerHandler->GetMatrix(i,mFlagMatrix))
			{
								
				Convertor cn;
				mFlagMatrixQuat = cn.Matrix2QUAT(mFlagMatrix);
				for (int iHandle = 0 ; iHandle < MAX_BASE_HANDLES; iHandle++)
				{
					if (i == m_baseHandles[iHandle].bh_Handle)
					{
						bAllBaseVisible[iHandle] = TRUE;
					} 
				}
				
				
				if (i == m_iMarkerHandle)
				{
					m_tMarker = mFlagMatrix;
					bMarkerVisible = TRUE;
					m_dlgTrackingStatus.m_dlgTracker.SetBasePos(m_tMarker.value[0][3],m_tMarker.value[1][3],m_tMarker.value[2][3]);
				}
				else if (i == m_baseHandles[m_iCurBaseIndex].bh_Handle)		
				{
					m_tBase = mFlagMatrix;
					bBaseVisible = TRUE; 
					if(m_patientMarker==PATIENT_NO_MARKER)
					{
						CString sTmp;
						double thetaX=fabs(m_tBase.value[0][3]-m_tBaseLast.value[0][3]);
						double thetaY=fabs(m_tBase.value[1][3]-m_tBaseLast.value[1][3]);
						double thetaZ=fabs(m_tBase.value[2][3]-m_tBaseLast.value[2][3]);
						if( m_bPromptCameraMove && (thetaX>0.5 || thetaY> 0.5 || thetaZ>0.5))
						{
							sTmp.Format("Difference:%.2f,%.2f,%.2f",thetaX,thetaY,thetaZ);
							m_dlgTrackingStatus.AddMessage(sTmp);
							m_bPromptCameraMove=FALSE;
							//PanaMessageBox("Camera is moving.");
						
							CameraMove();
						}
						if(m_bRecordBase)
						{
							m_tBaseLast=m_tBase;
							m_bRecordBase=FALSE;
							m_bPromptCameraMove=TRUE;
						}
					}
					
					//
				}
				else if ( i == m_iPhatomHandle )
				{
					m_tPhatom = mFlagMatrix;
					bPhantomVisible = TRUE;
				}
				
								
				if( i == m_iProbeHandle)
				{
					m_tProbe = mFlagMatrix;
					bProbeVisible = TRUE;
				}
				/** Get probe tip coordinates **/
				if (m_ppTrackTool[i]->m_pAttachedProbe)
				{
					
					// Update probe position and orientation
					Matrix tProbeTargeting(4,4);
					double dProbeTargetOri[3];
					if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
					{
						m_ppTrackTool[i]->m_pAttachedProbe->UpdatePosOri(&mFlagMatrix);
						
						tProbeTargeting=mFlagMatrix;
					}
					if (m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
					{
						Matrix tTrack2Marker = m_tMarker.Inverse();
						m_ppTrackTool[i]->m_pAttachedProbe->UpdatePosOri(&mFlagMatrix,tTrack2Marker);
						tProbeTargeting = tTrack2Marker * mFlagMatrix;
						
					}
					
					//将针定位信息发送给Tps，需要将针位置转换到scanner下。
					double pos[3];
					double ori[3];
					if(m_patientMarker==PATIENT_NO_MARKER)
					{
						Vector3D oriInTracker = m_ppTrackTool[i]->m_pAttachedProbe->GetOrientationInTrack();
						Vector3D tipInTracker = m_ppTrackTool[i]->m_pAttachedProbe->GetRealTipInTrack();
						//根据病床移动距离修正tip信息
						Vector3D vPTMovInTracker;
						vPTMovInTracker = GetPTMoveVectorInTracker(m_dbCurrentPTPos);
						tipInTracker.x-=vPTMovInTracker.x;
						tipInTracker.y-=vPTMovInTracker.y;
						tipInTracker.z-=vPTMovInTracker.z;
						
						Vector oriInTracker2(4);
						oriInTracker2.value[0]=oriInTracker.x;
						oriInTracker2.value[1]=oriInTracker.y;
						oriInTracker2.value[2]=oriInTracker.z;
						oriInTracker2.value[3]=0;
						Vector tipInTracker2(4);
						tipInTracker2.value[0]=tipInTracker.x;
						tipInTracker2.value[1]=tipInTracker.y;
						tipInTracker2.value[2]=tipInTracker.z;
						tipInTracker2.value[3]=1;
						
						tProbeTargeting.value[0][3]=tipInTracker2.value[0];
						tProbeTargeting.value[1][3]=tipInTracker2.value[1];
						tProbeTargeting.value[2][3]=tipInTracker2.value[2];
						dProbeTargetOri[0]=oriInTracker2.value[0];
						dProbeTargetOri[1]=oriInTracker2.value[1];
						dProbeTargetOri[2]=oriInTracker2.value[2];

						Matrix tracker2Scanner = m_tranScanner2Tracker.Inverse();
						Vector oriInScanner=tracker2Scanner * oriInTracker2;
						Vector tipInScanner=tracker2Scanner * tipInTracker2;
						
						pos[0]=tipInScanner.value[0];
						pos[1]=tipInScanner.value[1];
						pos[2]=tipInScanner.value[2];
						ori[0]=oriInScanner.value[0];
						ori[1]=oriInScanner.value[1];
						ori[2]=oriInScanner.value[2];
						
					}else
					{
						//实际oriInTracker tipInTracker代表针在病床marker坐标系下的方向和位置
						Vector3D oriInTracker = m_ppTrackTool[i]->m_pAttachedProbe->GetOrientationInTrack();
						Vector3D tipInTracker = m_ppTrackTool[i]->m_pAttachedProbe->GetRealTipInTrack();
												
						Vector oriInPT(4);
						oriInPT.value[0]=oriInTracker.x;
						oriInPT.value[1]=oriInTracker.y;
						oriInPT.value[2]=oriInTracker.z;
						oriInPT.value[3]=0;
						Vector tipInPT(4);
						tipInPT.value[0]=tipInTracker.x;
						tipInPT.value[1]=tipInTracker.y;
						tipInPT.value[2]=tipInTracker.z;
						tipInPT.value[3]=1;
						
						tProbeTargeting.value[0][3]=tipInPT.value[0];
						tProbeTargeting.value[1][3]=tipInPT.value[1];
						tProbeTargeting.value[2][3]=tipInPT.value[2];
						dProbeTargetOri[0]=oriInPT.value[0];
						dProbeTargetOri[1]=oriInPT.value[1];
						dProbeTargetOri[2]=oriInPT.value[2];
												
						pos[0]=tipInPT.value[0];
						pos[1]=tipInPT.value[1];
						pos[2]=tipInPT.value[2];
						ori[0]=oriInPT.value[0];
						ori[1]=oriInPT.value[1];
						ori[2]=oriInPT.value[2];
						
					}
					
					pDlgMain->m_dlgCryotherapy.m_dlgTps.UpdateProbeInfo(pos,ori);
					
					if(pDlgMain->m_dlgTracking.IsWindowVisible())
					{
						pDlgMain->m_dlgTracking.m_dlgTargeting.SetProbe(tProbeTargeting,dProbeTargetOri);
					}
					
				}
								
								
				if (m_ppTrackTool[i]->m_pAttachedProbe)
				{
					if (m_lFrameNo[i] == 0)
					{						
						m_ppTrackTool[i]->m_pAttachedProbe->SetVisible(TRUE);						
					}
					else
					{
						m_pDisplay4Dlg->Refresh(); 
					}
				}
				
				m_lFrameNo[i] ++;
				
				// For probe calibration data collection
				if (m_bInDataTracking && m_iDataCollectionHandlerID == i)
				{
					if (m_uiFlag == FLAG_PT_CALI)
					{
						m_lstTrackingData.AddTail(mFlagMatrixQuat.x);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.y);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.z);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.q1);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.q2);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.q3);
						m_lstTrackingData.AddTail(mFlagMatrixQuat.q4);
						m_nCollectCnt ++;
						if (m_nCollectCnt >= m_nCollectNum)
						{
							m_bInDataTracking = FALSE;
							::PostMessage(m_wHandle, WM_COLLECTION_DONE, 0, 0);
						}
					}
				}
				
			}/* if */
			else 
			{
				if (m_ppTrackTool[i]->m_pAttachedProbe && m_ppTrackTool[i]->m_pAttachedProbe->IsVisible()) // Added by Wei Wei [2007.6.26]
				{
					bProbeVisible = FALSE;
					
					m_ppTrackTool[i]->m_pAttachedProbe->SetVisible(FALSE);
					m_lFrameNo[i] = 0;
					m_pDisplay4Dlg->Refresh();
				}
			}/* else */					
		}/* if */
	}/* for */
	
	if(pDlgMain->m_dlgCryotherapy.m_dlgNavigate.IsTracking())
	{
		if(m_pTrackerHandler &&	bMarkerVisible && bProbeVisible)
		{
			//使用针平面重切图像
			pDlgMain->m_dlgCryotherapy.m_dlgNavigate.ResliceSeries();
			
			//更新投影在屏幕上的target坐标
			pDlgMain->m_dlgTracking.EnableTrackingFlag(TRUE);
			pDlgMain->m_dlgTracking.UpdateProjectVirtualTarget();
			pDlgMain->m_dlgTracking.Refresh(); //刷新屏幕显示
		}else
		{
			pDlgMain->m_dlgTracking.EnableTrackingFlag(FALSE);
			pDlgMain->m_dlgTracking.Refresh(); //刷新屏幕显示
		}
	}

	m_bBaseVisible=bBaseVisible;
	m_bMarkerVisible=bMarkerVisible;
	m_bProbeVisible=bProbeVisible;
	m_bPhantomVisible=bPhantomVisible;
	for (iHandle = 0; iHandle<MAX_BASE_HANDLES; iHandle++)
	{
		m_bAllBaseVisible[iHandle] = bAllBaseVisible[iHandle];
	}
	
	return 1;
} 

void CCryotherapyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==STATUSBAR_UPDATE_TIMER)
	{
		if (m_patientMarker==PATIENT_NO_MARKER ) //NAVI MODE
		{
			m_dlgTrackingStatus.SetBaseStatus(m_bTracking && m_bBaseVisible);
		}
		if (m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
		{
			m_dlgTrackingStatus.SetBaseStatus(m_bTracking && m_bMarkerVisible);
		}	
		
		m_dlgTrackingStatus.SetProbeStatus(m_bTracking && m_bProbeVisible);
		m_dlgCmdBar.UpdateUI();

		if(!m_bTracking && m_pTrackerHandler && m_pTrackerHandler->GetDeviceType()==TRACK_DEVICE_NDI_EM)
		{
			m_pDlgMain->m_dlgTracking.EnableTrackingFlag(FALSE);
			m_pDlgMain->m_dlgTracking.Refresh(); //刷新屏幕显示
		}
	}
	CDlgMainSkin::OnTimer(nIDEvent);
}

void CCryotherapyDlg::ChangeWidthLevel(CMRISeries* pMRISeries)
{
	CMRISeries* pLastSeries = NULL;
	//pLastSeries = m_MRIBuffer.GetSeries(pMRISeries->GetStudyID(), pMRISeries->GetSeriesID());
	pLastSeries = m_pMRISeries;		
	
	if (pLastSeries)
	{
		int nWindowWidth = pLastSeries->GetWindowWidth();
		int nWindowLevel = pLastSeries->GetWindowLevel();
		pMRISeries->SetWindowWidthAndLevel(nWindowWidth, nWindowLevel);
	}
}


void CCryotherapyDlg::UpdateMRIPosition()
{
	if (m_pMRISeries)
	{
		
	}
}

void CCryotherapyDlg::UpdateFrameBoxPosition()
{
	char *pBoxLabel;
	CString sTmpBoxLabel;
	if ( m_pMRISeries )
	{
		pBoxLabel = m_pMRISeries->GetNewBoxOrientChar();
	}
	else
	{
		sTmpBoxLabel = m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.GetBoxLabel();
		pBoxLabel = sTmpBoxLabel.GetBuffer(0);
	}
	
	
	if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
	{
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.UpdateFrameBox(pBoxLabel, &m_tranScanner2Tracker, &m_vPTMovInTracker, FALSE);
	}
	if (m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
	{
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.UpdateFrameBox(pBoxLabel, &m_tranScanner2Marker,NULL,FALSE); 
	}
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
}

void CCryotherapyDlg::InitFrameBoxPosition()
{
	char *pBoxLabel;
	CString sTmpBoxLabel;
	if ( m_pMRISeries )
	{
		pBoxLabel = m_pMRISeries->GetNewBoxOrientChar();
	}
	else
	{
		sTmpBoxLabel = m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.GetBoxLabel();
		pBoxLabel = sTmpBoxLabel.GetBuffer(0);
	}
	if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
	{
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.UpdateFrameBox(pBoxLabel, &m_tranScanner2Tracker, &m_vPTMovInTracker); 
	}
	if (m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
	{
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.UpdateFrameBox(pBoxLabel, &m_tranScanner2Marker,NULL); 
	}
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	
	//	delete []pBoxLabel;
}

void CCryotherapyDlg::DeleteMRISeries(CString sMRIName)
{
	CString sName = sMRIName;
	
	CMRISeries* pMRISeries = m_MRIBuffer.GetSeries(sName);
	
	if(pMRISeries!=NULL)
	{
		m_mapMRIName2Folder.RemoveKey(pMRISeries->m_sFolder);
	}

	// Update pointer
	if (pMRISeries == m_pCurSelSeries)
	{
		m_pCurSelSeries = NULL;
	}
	if (pMRISeries == m_pMRISeries)
	{
		m_pMRISeries = NULL;
	}
	
	// delete display
	CDlgDisplayWrap *pDispWrap;
	for(int i=0;i<5;i++)
	{
		pDispWrap= m_pDisplay4Dlg->GetDispWrap(i);
		if (pDispWrap->m_dlgDispControl.m_pMRIRectSeries && 
			pDispWrap->m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()->GetName() == sName)
		{
			m_pDisplay4Dlg->RemoveSeriesFrom3D(&(m_MRIRectSeries[i]));
			pDispWrap->m_dlgDisplay.m_Scene.RemoveAllDistMeasurement();
			pDispWrap->m_dlgDispControl.RemoveCurrentDispImg();
			pDispWrap->m_pDispSeries = NULL;
			pDispWrap->m_dlgDispControl.m_pMRIRectSeries=NULL;
			pDispWrap->m_dlgDisplay.m_pCurDispMRIRect=NULL;
			m_MRIRectSeries[i].Clean();
			CCrashRstore::SaveCrashState(CI_DISPLAY);
		}
	}
	m_MRIBuffer.Delete(sName);      //删除序列
}

void CCryotherapyDlg::AddMRISeries(CString sPath, BOOL bUpdateList /* = TRUE*/)
{
	CSeriesLoader loader;
	loader.LoadSeries(sPath);
	for(int i=0;i<loader.GetCount();i++)
	{
		CMRISeries *pMRISeries=loader.GetAt(i);	
				
		if (bUpdateList && m_dlgSeries.IsInList(pMRISeries->GetName()))
		{
			PanaMessageBox("Series " + pMRISeries->GetName() + " was already opened.", MB_ICONWARNING);
			delete pMRISeries;
			continue;
		}
		
		
		/*避免多线程程序在此处死锁*/
		EnterCriticalSection(&m_csLoadSeries);
		// Update window width and level
		ChangeWidthLevel(pMRISeries);
		
		if ( !m_MRIBuffer.Push(pMRISeries) ) //保存Series
		{	
			delete pMRISeries;
			continue;
		}
		
		InsertMap(pMRISeries->GetName(),sPath);	
		
		UpdateCurrentMRI(FALSE);
		
		if (bUpdateList)
		{
			UpdateMRIList(pMRISeries);
		}
		
		m_pDlgMain->BroadcastMsg(WM_NEW_SERIES, 1, (long)m_pMRISeries);
		
		
		CString sLog;
		sLog.Format("Open MRI series %s from %s", pMRISeries->GetName(), sPath);
		CLog::Log(sLog);
		
		LeaveCriticalSection(&m_csLoadSeries);
	
	}
}


BOOL CCryotherapyDlg::LoadMRISeries(CMRISeries* pMRISeries,BOOL bSave /*=TRUE*/)	
{
	// Update window width and level
	ChangeWidthLevel(pMRISeries);
	//m_popupMenu.Add(IDR_POP_MENU,0,pMRISeries->GetName()); //Add by Dai Liang 2007.6.22 for solve the popup menu 
	if ( !m_MRIBuffer.Push(pMRISeries) )
	{
		delete pMRISeries;
		return FALSE;
	}
	
	UpdateCurrentMRI();
	UpdateMRIList(pMRISeries);
	
	if (m_sMRIStorageFolder != "" && bSave)
	{
		CString sModalityFolder;
		sModalityFolder.Format("%s\\%s", m_sMRIStorageFolder, m_pDlgMain->m_MRIWrap.m_sCurModalityID);
		CreateDirectory(sModalityFolder, 0);
		CString sStudyFolder;
		sStudyFolder.Format("%s\\%s", sModalityFolder, pMRISeries->GetStudyID());
		CreateDirectory(sStudyFolder, 0);
		CString sSeriesFolder;
		sSeriesFolder.Format("%s\\%d", sStudyFolder, pMRISeries->GetSeriesID());
		CreateDirectory(sSeriesFolder, 0);
		CString sScanFolder;
		sScanFolder.Format("%s\\%d", sSeriesFolder, pMRISeries->GetScanID());
		CreateDirectory(sScanFolder, 0);
		pMRISeries->Save(sScanFolder);
		
		InsertMap(pMRISeries->GetName(), sScanFolder);
	}
	
	m_pDlgMain->BroadcastMsg(WM_NEW_SERIES, 1, (long)m_pMRISeries);
	
	return TRUE;
}

void CCryotherapyDlg::TransformEverythingToFinalCor()
{
	if (m_patientMarker==PATIENT_NO_MARKER) 
	{
		// Move target to scanner coordinates
		Matrix mTracker2Scanner = m_tranScanner2Tracker.Inverse();
		m_TargetPool.TranslateAllVTarget(m_dbCurrentPTPos,TRUE);
		m_TargetPool.TransformAllVTarget(mTracker2Scanner.value);

		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,TRUE);
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(mTracker2Scanner.value);
		m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,TRUE);
		m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(mTracker2Scanner.value);
		m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,TRUE);
		m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(mTracker2Scanner.value);
		m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,TRUE);
		m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(mTracker2Scanner.value);
		m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,TRUE);
		m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(mTracker2Scanner.value);
	}
	
	
	// Create new tranformation matrix
	CreateTransform();

	if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
	{
		/*此函数移动图像序列时，已经考虑到病床移动带来的影响 ningsj 2009.2.19*/
		TransformAllMRISeries();

		m_TargetPool.TransformAllVTarget(m_tranScanner2Tracker.value);
		m_TargetPool.TranslateAllVTarget(m_dbCurrentPTPos,FALSE);	

		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(m_tranScanner2Tracker.value);
		m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,FALSE);
		m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(m_tranScanner2Tracker.value);
		m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,FALSE);
		m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(m_tranScanner2Tracker.value);
		m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,FALSE);
		m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(m_tranScanner2Tracker.value);
		m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,FALSE);
		m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.m_Scene.TransformAllDistanceSegment(m_tranScanner2Tracker.value);
		m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.m_Scene.TranslateAllDistanceSegment(this,m_dbCurrentPTPos,FALSE);
	
		m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.UpdateAllProjectVTarget();
	}
	
	UpdateFrameBoxPosition();
	
	m_pDisplay4Dlg->Refresh();
}

/* 移动图像位置时，考虑到病床的移动*/
void CCryotherapyDlg::TransformMRISeriesToFinalCor(CMRISeries* pMRISeries)
{
	Vector3D vPTMovInTracker;
	if (pMRISeries)
	{
		if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
		{
			pMRISeries->Transformation(m_tranScanner2Tracker.value);
			vPTMovInTracker = this->GetPTMoveVectorInTracker(this->m_dbCurrentPTPos);
			if (vPTMovInTracker.x != 0 || vPTMovInTracker.y != 0 || vPTMovInTracker.z != 0)
			{
				double dbMov[3];
				dbMov[0] = vPTMovInTracker.x;
				dbMov[1] = vPTMovInTracker.y;
				dbMov[2] = vPTMovInTracker.z;
				pMRISeries->Translation(dbMov);
			}
		}else if(m_patientMarker==PATIENT_WITH_MARKER)
		{
			Matrix mFusion=pMRISeries->GetFusionCoord();
			pMRISeries->Transformation(mFusion.value);
		}
	}
}

void CCryotherapyDlg::TransformAllMRISeries()
{
	CMRISeries* pMRISeries = NULL;
	for (int i=0; i<IMG_BUFFER_SIZE; i++)
	{
		pMRISeries = m_MRIBuffer.GetSeriesAt(i);
		if (pMRISeries)
		{
			TransformMRISeriesToFinalCor(pMRISeries);
			
			MRIRectSeries *pMRIRectSeries;
			for (int i=1; i<5; i++)
			{
				pMRIRectSeries = &(m_MRIRectSeries[i]);
				if (pMRIRectSeries && pMRIRectSeries->GetMRISeries() == pMRISeries)
				{
					pMRIRectSeries->UpdatePos();
				}
			}
		}
	}
	
	CMRIRectangle *pMRIRect;
	for (i=1; i<5; i++)
	{
		pMRIRect = m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.m_pCurDispMRIRect;
		if (pMRIRect)
		{
			pMRIRect->UpdatePos();
			m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.ResetView();
		}
	}
}

void CCryotherapyDlg::UpdateCurrentMRI(BOOL bUpdateRTWnd)
{
	m_pMRISeries = m_MRIBuffer.GetDisp();
	
	if (m_pMRISeries == NULL)
		return;
	
	// Transfer coordinates
	TransformMRISeriesToFinalCor(m_pMRISeries);
	
	CMRISeries *pMRISeries = NULL;
	for (int i=1; i<5; i++)
	{
		// Modified by Huagen Liu 2007.02.05 to display received MRI to windows specified by user
		if (bUpdateRTWnd && m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.IsRcvWnd())
		{
			DisplayMRISeriesToWindow(m_pMRISeries, i, m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDispControl.m_iCurDispIndex);
		}
		
	}
	
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	
}

void CCryotherapyDlg::UpdateMRIList(CMRISeries *pMRISeries)
{
	CStringArray saMRIs;
	saMRIs.Add(pMRISeries->GetName());
	m_dlgSeries.InsertMRIList(saMRIs);
	
	if (!m_dlgSeries.IsWindowVisible())
	{
		SetNewSeriesAlarm(TRUE);
	}
}


void CCryotherapyDlg::OnPopup1ChangeImgCoord() 
{
}

void CCryotherapyDlg::OnProbeSimulate() 
{
	// TODO: Add your command handler code here
	if (m_iSelectHandle > 0 && m_ppTrackTool[m_iSelectHandle]->m_pAttachedProbe)
	{
		UserSimulateProbeIceBall(m_ppTrackTool[m_iSelectHandle]->m_pAttachedProbe->GetIndex());
	}
}


void CCryotherapyDlg::UserSimulateProbeIceBall(int iProbeID)
{
	CProbe* pProbe;
	pProbe = m_ProbePool.GetProbeByIndex(iProbeID);
	if (pProbe == NULL)
		return;
	
	pProbe->EnableSimulate(TRUE);
	m_pDisplay4Dlg->Refresh();
	
	//m_pDisplay4Dlg->SimulateIceBall(pProbe);
	
	// Following is for simulate sphere growing process, commented by Huagen Liu 2006.08.09
	/*	pProbe->m_tipSphere.SetRadius(0.0);
	AfxBeginThread( SphereSimulating,
	m_hWnd,
	THREAD_PRIORITY_NORMAL,
	0,
	0 );
	m_bStopSphereSimulate = FALSE;
	m_bIsSimulating = TRUE; */
}

UINT SphereSimulating( LPVOID pParam)
{
	HWND hWnd = (HWND)pParam;
	
	long lRadius = 0;
	while ( !m_bStopSphereSimulate )
	{
	/*
	* while tracking, post messages to fill the list
		*/
		lRadius += 2;
		if ( m_bIsSimulating )
			::SendMessage( hWnd, WM_SIMULATE_SPHERE, 0,  lRadius);
		else
			m_bStopSphereSimulate = TRUE;
		Sleep(100);
	}/* while */
	
	/* when tracking stopped, kill the thread */
	m_bStopSphereSimulate = FALSE;
	AfxEndThread( 0, TRUE );
	return 0;
} 

LONG CCryotherapyDlg::SimulateSphere( UINT wParam, LONG lParam )
{
	int iRadius = (int)lParam;
	double dbRadius = ((double)iRadius)/10;
	CProbe *pProbe;
	
	pProbe = m_ProbePool.GetProbeByIndex(m_iSelectHandle);
	if (pProbe->IsSimulating())
	{
		if (dbRadius <= pProbe->GetMaxSphereRadius())
		{
			m_pDisplay4Dlg->SimulateCryotherapySphere(pProbe,dbRadius);
		}
		else
		{
			m_bIsSimulating = FALSE;
			m_bStopSphereSimulate = TRUE;
		}
	}
	//}
	return 1;
}

void CCryotherapyDlg::OnProbeStopSimulate() 
{
	// TODO: Add your command handler code here
	if (m_iSelectHandle > 0 && m_ppTrackTool[m_iSelectHandle]->m_pAttachedProbe)
	{
		StopSimulateProbeIceBall(m_ppTrackTool[m_iSelectHandle]->m_pAttachedProbe->GetIndex());
		m_pDisplay4Dlg->Refresh();
	}
}

void CCryotherapyDlg::StopSimulateProbeIceBall(int iProbeID)
{
	CProbe* pProbe;
	pProbe = m_ProbePool.GetProbeByIndex(iProbeID);
	if (pProbe == NULL)
		return;
	
	pProbe->EnableSimulate(FALSE);
	m_bStopSphereSimulate = TRUE;
	m_bIsSimulating = FALSE;
	//m_pDisplay4Dlg->SimulateIceBall(pProbe);
}

void CCryotherapyDlg::OnProbeSimulatePopDlg() 
{
	// TODO: Add your command handler code here
	if (m_iSelectHandle>0 && m_iSelectHandle<MAX_HANDLES)
	{
		CDlgSimulate dlgSimualte;
		dlgSimualte.m_pCryotherapyDlg = this;
		int iRet = dlgSimualte.DoModal();
		if (iRet==IDOK)
		{
		}
		else if (iRet==IDCANCEL)
		{
			OnProbeStopSimulate();
		}
	}
}

BEGIN_EVENTSINK_MAP(CCryotherapyDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CCryotherapyDlg)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


bool CCryotherapyDlg::LoadTransformation(CString sPathFile,double** pValue) //Add by Dai Liang 2007.6.5
{
	try
	{
		if (EXPORT_BINARY)
		{
			CFile f;
			char buf[512];
			if( !f.Open( sPathFile, CFile::modeRead) ) 
			{
				//	AfxMessageBox("Can't open file !");
				return false;
			}
			
			CArchive ar(&f, CArchive::load, 512, buf);
			for (int i=0; i<4; i++)
				for (int j=0; j<4; j++)
					ar >> pValue[i][j];
				ar.Close();
				f.Close();
				return true;
		}
		else 
		{
			FILE *fp;
			fp = fopen(sPathFile, "r");
			if (!fp)
			{
				//AfxMessageBox("Can't open file !");
				return false;
			}
			float db1, db2, db3, db4;
			for (int i=0; i<4; i++)	
			{
				fscanf(fp, "%f  %f  %f  %f\n", &db1, &db2, &db3, &db4);
				pValue[i][0] = db1;
				pValue[i][1] = db2;
				pValue[i][2] = db3;
				pValue[i][3] = db4;
			}
			fclose(fp);
			return true;
		}
	}catch(...)
	{
		return false;
	}
	return false;

}






void CCryotherapyDlg::SaveTransformation(CString sPathFile, double** pValue)//Add by Dai Liang 2007.6.5
{
	
	if (EXPORT_BINARY)
	{
		CFile f;
		char buf[512];
		if( !f.Open( sPathFile, CFile::modeCreate | CFile::modeWrite) ) 
		{
			CString sTmp;
			sTmp.Format("Can't open file %s",sPathFile);
			PanaMessageBox(sTmp);
			return;
		}
		CArchive ar(&f, CArchive::store, 512, buf);
		for (int i=0; i<4; i++)
			for (int j=0; j<4; j++)
				ar << pValue[i][j];
		ar.Close();
		f.Close();
	}
	else
	{
		FILE *fp;
		fp = fopen(sPathFile, "w");
		if (fp)
		{
			for (int i=0; i<4; i++)	
			{
				fprintf(fp, "%.6f  %.6f  %.6f  %.6f\n", pValue[i][0], pValue[i][1], pValue[i][2], pValue[i][3]);
			}
			fclose(fp);
		}else
		{
			CString sTmp;
			sTmp.Format("Can't open file %s",sPathFile);
			PanaMessageBox(sTmp);
			return;
		}
	}
}

bool CCryotherapyDlg::LoadPatientTableDirFile(CString sPathFile)
{
	if (EXPORT_BINARY)
	{
	}
	else
	{
		FILE *fp;
		fp = fopen(sPathFile, "r");
		if (!fp)
		{
			return false;
		}
		float db1, db2, db3;
		fscanf(fp, "%f  %f  %f", &db1, &db2, &db3);
		m_vPTDirInScan.x = db1;
		m_vPTDirInScan.y = db2;
		m_vPTDirInScan.z = db3;
		fclose(fp);
		return true;
	}
}

bool CCryotherapyDlg::SavePatientTableDirFile(CString sPathFile)
{
	if (EXPORT_BINARY)
	{
	}
	else
	{
		BOOL bOpened=FALSE;
		FILE *fp=NULL;
		try
		{
			
			fp = fopen(sPathFile, "w");
			if (!fp)
				return false;
		
			bOpened=TRUE;
			fprintf(fp, "%f %f %f", m_vPTDirInScan.x, m_vPTDirInScan.y, m_vPTDirInScan.z);
			fclose(fp);
			bOpened=FALSE;
			return true;
		}catch(...)
		{
			if(bOpened)
			{
				fclose(fp);
			}
			return false;
		}
	}
}


void CCryotherapyDlg::UpdateTextureSameSeries(MRIRectSeries *pMRIRectSeries,BOOL bUpdateAllImage)
{
	for (int i=1; i<5; i++)
	{
		CMRISeries *pMRISeries = NULL;
		pMRISeries = m_MRIRectSeries[i].GetMRISeries();
		if (pMRISeries && 
			pMRISeries->GetName() == pMRIRectSeries->GetMRISeries()->GetName())
		{
			m_MRIRectSeries[i].UpdateTexture(bUpdateAllImage);
			m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.AddMRIRectLegend(
				m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.m_pCurDispMRIRect->GetMRIImg());
			
			if(m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.IsWindowVisible())
			{
				m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.m_Scene.DrawScene();
			}
		}
	}
	if(m_pDisplay4Dlg->GetDispWrap(0)->m_dlgDisplay.IsWindowVisible())
	{
		m_pDisplay4Dlg->GetDispWrap(0)->m_dlgDisplay.m_Scene.DrawScene();
	}
}


void CCryotherapyDlg::DisplayMRISeriesToWindow(CMRISeries *pMRISeries, int iWndIndex, int iInitDispImgIndex /*= -1*/)
{
	if (pMRISeries == NULL || iWndIndex < 1 || iWndIndex > 4)
		return;
	
	MRIRectSeries *pMRIRectSeries = &(m_MRIRectSeries[iWndIndex]);
	
	// Remember rectangles index which are showing in 3D window
	int iNum = pMRIRectSeries->GetRectNum();
	if (iNum > 0)
	{
		pMRIRectSeries->GetMRISeries()->SetVisible(false);
	}
	
	BOOL bReload = FALSE;
	CList<int, int> lst3DIndex;
	
	/*
	修改人：宁随军 
	修改时间：2009.2.10
	将下面这一行注释掉
	作用:实时更新时,更换序列没有从3D窗口中将旧序列删掉，导致内存漏洞
	同时引起系统崩溃
	*/
	//if (iNum != pMRISeries->GetNumOfImages())
	{
		CMRIRectangle *pMRIRect;
		for (int i=0; i<iNum; i++)
		{
			pMRIRect = pMRIRectSeries->GetMRIRectAt(i);
			//if (pMRIRect->IsVisible3D())
			if (pMRIRect->m_bIn3DWindows)
			{
				if (pMRIRect->IsVisible3D())
				{
					lst3DIndex.AddTail(i);
				}
				m_pDisplay4Dlg->RemoveFrom3DDisplay(pMRIRect);
			}
		} 
		bReload = TRUE;
	}
	// end remember
	
	pMRIRectSeries->Init(pMRISeries);
	
	// Redisplay rectangles in 3D window
	if (bReload)
	{
		POSITION pos = lst3DIndex.GetHeadPosition();
		int iIndex;
		iNum = pMRIRectSeries->GetRectNum();
		for (;pos;)
		{
			iIndex = lst3DIndex.GetNext(pos);
			if (iIndex >= 0 && iIndex < iNum)
			{
				m_pDisplay4Dlg->AddTo3DDisplay(pMRIRectSeries->GetMRIRectAt(iIndex));
			}
		} 
	}
	// end redisplay
	
	// Update frame box label according to this MRI series
	char *pBoxLabel;
	pBoxLabel = pMRIRectSeries->GetMRISeries()->GetNewBoxOrientChar();
	//	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.UpdateFrameBox(pBoxLabel, NULL);
	m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.SetBoxFrameLabel(pBoxLabel);
	// end update frame box label
	
	// Update 2D display window
	//	if (iInitDispImgIndex >= 0 && iInitDispImgIndex < pMRIRectSeries->GetRectNum())
	//	{
	//		m_pDisplay4Dlg->AddMRIRectSeries(pMRIRectSeries, iWndIndex, iInitDispImgIndex);		
	//	}
	//	else
	//	{
	//		m_pDisplay4Dlg->AddMRIRectSeries(pMRIRectSeries, iWndIndex, 0/* pMRIRectSeries->GetDispIndex()*/);
	//	}
	m_pDisplay4Dlg->AddMRIRectSeries(pMRIRectSeries, iWndIndex, iInitDispImgIndex);
	
	pMRISeries->SetVisible(true);
	
	if (m_pDisplay4Dlg->GetDispWrap(iWndIndex)->m_dlgDisplay.m_bSelected)
	{
		m_pCurSelSeries = pMRISeries;
	}
	
	CCrashRstore::SaveCrashState(CI_DISPLAY);
}

void CCryotherapyDlg::DisplaySelectMRIImageToWindow(int iWndIndex)
{
	
}

void CCryotherapyDlg::ImportBasePattern(CString szFileName)
{
	CFile f;
	char buf[512];
	if( !f.Open( szFileName, CFile::modeRead) ) 
	{
		PanaMessageBox("Can't open file !");
		return;
	}
	
	CArchive ar(&f, CArchive::load, 512, buf);
	
	for (int i=0; i<MAX_BASE_HANDLES; i++)
	{
		ar >> m_baseHandles[i].bh_Handle;
		for (int j=0; j<4; j++)
			for (int k=0;k<4;k++)
				ar >> m_baseHandles[i].bh_mToBase0.value[j][k];
			ChangeBaseTracking(m_baseHandles[i].bh_Handle, i);
	}
	ar.Close();
	f.Close();
	
}

void CCryotherapyDlg::ExportBasePattern(CString szFileName)
{
	CFile f;
	char buf[512];
	if( !f.Open( szFileName, CFile::modeCreate | CFile::modeWrite) ) 
	{
		PanaMessageBox("Can't open file !");
		return;
	}
	
	CArchive ar(&f, CArchive::load, 512, buf);
	
	for (int i=0; i<MAX_BASE_HANDLES; i++)
	{
		ar << m_baseHandles[i].bh_Handle;
		for (int j=0; j<4; j++)
			for (int k=0;k<4;k++)
				ar << m_baseHandles[i].bh_mToBase0.value[j][k];
	}
	
	ar.Close();
	f.Close();
}


void CCryotherapyDlg::InitTrackingTool()
{
	if(m_pTrackerHandler!=NULL)
		return;

	CleanTrackingTool(true);
	
	if (m_pTrackerHandler==NULL)
	{
		m_pTrackerHandler = m_pDlgMain->m_trackerManager.GetHandler();
	}
	m_iMaxNumOfTrackTool = m_pTrackerHandler->GetToolCount();
	m_ppTrackTool = new CTrackTool*[m_iMaxNumOfTrackTool];
	for (int i=0; i<m_iMaxNumOfTrackTool; i++)
	{
		m_ppTrackTool[i] = NULL;
	}
	
	CConfigMgr conf;
	CString szPort;
	for (i=0; i<m_iMaxNumOfTrackTool; i++)
	{
		if (m_pTrackerHandler->IsValidTool(i) ) 
		{
			m_ppTrackTool[i] = new CTrackTool;
			m_ppTrackTool[i]->m_iNDIPort = i;
		} 
	}
	
	if(m_pTrackerHandler->GetDeviceType()==TRACK_DEVICE_NDI_EM)
	{
		m_iMarkerHandle = TH_NDI_EM_MARKER;
		m_iProbeHandle = TH_NDI_EM_PROBE;
		double pos[]={0,0,-350};
		m_dlgTrackingStatus.SetBaseRange(pos,250);
	}else
	{
		m_iProbeHandle = TH_NDI_OPTIC_PROBE;

		m_iPhatomHandle = TH_NDI_OPTIC_PHANTOM;
		ChangePhatomTrackingTo(m_iPhatomHandle);
		
		m_iMarkerHandle = TH_NDI_OPTIC_MARKER;
		
		m_baseHandles[0].bh_Handle = TH_NDI_OPTIC_BASE1; 
		ChangeBaseTracking(m_baseHandles[0].bh_Handle, 0);
		
		m_baseHandles[1].bh_Handle = TH_NDI_OPTIC_BASE2; 
		ChangeBaseTracking(m_baseHandles[1].bh_Handle, 1);
		
		m_baseHandles[2].bh_Handle = TH_NDI_OPTIC_BASE3; 
		ChangeBaseTracking(m_baseHandles[2].bh_Handle, 2);
		
		m_baseHandles[3].bh_Handle = TH_NDI_OPTIC_BASE4; 
		ChangeBaseTracking(m_baseHandles[3].bh_Handle, 3);

		double pos[]={0,0,-350};
		m_dlgTrackingStatus.SetBaseRange(pos,1000);
	}
}

void CCryotherapyDlg::CleanTrackingTool(bool bDetachFirst /*= false*/)
{
	if (m_iMaxNumOfTrackTool > 0)
	{
		for (int i=0; i<m_iMaxNumOfTrackTool; i++)
		{
			if (m_ppTrackTool[i])
			{
				if (m_ppTrackTool[i]->m_pAttachedProbe && bDetachFirst)
				{
					m_ppTrackTool[i]->Detach(false);
				}
				delete m_ppTrackTool[i];
				m_ppTrackTool[i] = NULL;
			}
		}
		delete m_ppTrackTool;
	}
}



void CCryotherapyDlg::ChangeBaseTracking(int iPortHandle, int iBaseNumber)
{
	if (iPortHandle < 0)
	{
		return;
	}
	
	m_ppTrackTool[iPortHandle]->m_TrackType = TT_BASE;
	m_baseHandles[iBaseNumber].bh_Handle = iPortHandle;	
}



void CCryotherapyDlg::InsertMap(CString sKey, CString sData)
{
	
	m_mapMRIName2Folder.SetAt(sKey, sData);
	
}

BOOL CCryotherapyDlg::LookupMap(CString sKey, CString &sRetData)
{
	return m_mapMRIName2Folder.Lookup(sKey, sRetData);
}

void CCryotherapyDlg::OnDisp2dOne() 
{
	DisplaySelectMRIImageToWindow(1);		
}
void CCryotherapyDlg::OnDisp2dTwo() 
{
	DisplaySelectMRIImageToWindow(2);		
}
void CCryotherapyDlg::OnDisp2dThree() 
{
	DisplaySelectMRIImageToWindow(3);		
}

void CCryotherapyDlg::OnDisp2dFour() 
{
	DisplaySelectMRIImageToWindow(4);		
}

void CCryotherapyDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect;
	GetClientRect(&rect);
	//MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	
	CDialog::OnSize(nType, cx, cy);		
}
/*void CCryotherapyDlg::SetOperationType(OPERATIONTYPE eType)//设置手术操作类型
{
m_dlgSgcPlan.SetOperationType(eType);
}*/
BOOL CCryotherapyDlg::ShowMRIImage(CMRISeries* pMRISeries,int iPos,int wndIndex)//显示MRI图象
{
	DisplayMRISeriesToWindow(pMRISeries,wndIndex,iPos);
	return TRUE;
}
BOOL CCryotherapyDlg::ShowMRIImage(CMRIImage* pMRIImage,int wndIndex)
{
	return TRUE;
}

void CCryotherapyDlg::ChangePhatomTrackingTo(int iPortHandle) //Add by Dai Liang 2007.6.5
{
	if (iPortHandle == -1)
	{
		return;
	}
	
	m_iPhatomHandle = iPortHandle;
	//	CConfigMgr conf;
	//	conf.Write(SYSINFO, SYS_PHANTOM_HANDLE, m_iPhatomHandle);
	
	for (int i=0; i<m_iMaxNumOfTrackTool; i++)
	{
		if (m_ppTrackTool[i])
		{
			if (m_ppTrackTool[i]->m_TrackType == TT_PHATOM)
			{
				m_ppTrackTool[i]->m_TrackType = TT_EMPTY;
			}
			if (m_ppTrackTool[i]->m_iNDIPort == iPortHandle)
			{
				m_ppTrackTool[i]->m_TrackType = TT_PHATOM;
			}
		}
	}
}

void CCryotherapyDlg::CreateTransform()
{
	if (m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
	{
		m_tranScanner2Tracker = m_tBase*m_tranScanner2Base;
		
		m_dbCurrentPTPos = m_dbRealPTPos;
		m_vPTMovInTracker = m_dbCurrentPTPos * m_vPTDirInScan;

		Matrix mRotateM = m_tranScanner2Tracker.GetSubMatrix(3,3);
		m_vPTMovInTracker.Rotate(mRotateM.value);

				
	}

	if (m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
	{
		if(m_TableShiftMode == NO_SHIFT_IN_SCANING)
		{
			m_tranScanner2Tracker = m_tMarker*m_tranScanner2Marker;
		}

		if(m_TableShiftMode == AUTO_SHIFT_IN_SCANING)
		{
			if(this->m_bBaseVisible)
			{
				m_tranScanner2Tracker = m_tBase*m_tranScanner2Base;
			}else
			{
				PanaMessageBox("Can't see the base. Please move the camera, try again.");
			}
		}
	}

			
}

Vector3D CCryotherapyDlg::GetPTMoveVectorInTracker(double dCurPTPos)
{
	Vector3D vPTMovInTracker;
	vPTMovInTracker = dCurPTPos  * m_vPTDirInScan;
	Matrix mRotateM = m_tranScanner2Tracker.GetSubMatrix(3,3);
	vPTMovInTracker.Rotate(mRotateM.value);

	return vPTMovInTracker;
}


BOOL CCryotherapyDlg::PreTranslateMessage(MSG* pMsg) 
{
	//Added by Wang fengliang 070809;
	//避免 按[回车]和[Esc] Dialog 消失
	if((pMsg->message==WM_KEYDOWN))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
			return TRUE;   
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}


CMRISeries* CCryotherapyDlg::GetMRISeries(CString sName,BOOL bFindInDatabase /*=TRUE*/)
{
	CMRISeries* pMRISeries = NULL;
	pMRISeries = m_MRIBuffer.GetSeries(sName);
	if (pMRISeries == NULL)
	{
		CString sFolder;
		if (LookupMap(sName, sFolder))
		{
			AddMRISeries(sFolder, FALSE);
			pMRISeries = m_MRIBuffer.GetSeries(sName);
		}
	}
	if (bFindInDatabase && pMRISeries == NULL)
	{
		CString sFolder;
		sFolder = CGlobalFuncs::MRINameToFolder(sName);
		AddMRISeries(m_sMRIStorageFolder + "\\" + m_pDlgMain->m_MRIWrap.m_sCurModalityID + "\\" + sFolder, FALSE);
		pMRISeries = m_MRIBuffer.GetSeries(sName);
	}
	return pMRISeries;
}

void CCryotherapyDlg::AddDatabaseResult(int iNum, char csSutId[100][256], int nSerId[100], char csMId[100][256])
{
	CString sStudyID[100];
	CString sMID[100];
	for(int i=0;i<iNum;i++)
	{
		sStudyID[i].Format("%s",csSutId[i]);
		sMID[i].Format("%s",csMId[i]);
	}
	AddDatabaseResult(iNum,sStudyID,nSerId,sMID);
}

void CCryotherapyDlg::AddDatabaseResult(int iNum, CString csSutId[], int nSerId[], CString csMId[])
{
	for (int i=0; i<iNum; i++)
	{
		CString strWildcard;
		strWildcard.Format("%s\\%s\\%s\\%d\\*", m_sMRIStorageFolder, csMId[i], csSutId[i], nSerId[i]);
		CFileFind finder;
		
		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			CString sScan = finder.GetFilePath();
			if (finder.IsDirectory() && sScan.Find(".", 0) < 0)
			{
				AddMRISeries(sScan);
			}
		}
		finder.Close();
	}	

}


void CCryotherapyDlg::SetOperType(OPERATIONTYPE nType)
{
	if (nType > OPERATION_TYPE_NONE && nType < OPERATION_TYPE_TPS )
	{
		m_dlgTitle.SetTitle(nType);
		if(m_pDlgMain->m_DeviceType==DEVICE_IGS_CP)
		{
			SelTab(PASK_Tab_Navigate);
		}else
		{
			SelTab(PASK_Tab_MriControl);
		}
		m_dlgTrackingStatus.m_dlgTracker.Enable(TRUE);
	}
	else
	{
		if(m_enumOperateType != OPERATION_TYPE_TPS)
		{
			SelTab(PASK_Tab_Series);
		}else
		{
			SelTab(PASK_Tab_Tps);
		}
		m_dlgTrackingStatus.m_dlgTracker.Enable(FALSE);
	}

	m_bOperateType = (PASK_OPERATIONTYPE)nType;
	m_enumOperateType = nType;
}

BOOL CCryotherapyDlg::IsBaseCouldbeVisible(int iBaseIndex)
{
	if (!m_bTracking)
	{
		StartTracking();
		
		Sleep(500);
	}
	return m_bAllBaseVisible[iBaseIndex];
}

void CCryotherapyDlg::StartCollectTrackingData(int iTrackingHandle, HWND wHandle)
{
	m_iDataCollectionHandlerID = iTrackingHandle;
	m_lstTrackingData.RemoveAll();
	m_wHandle = wHandle;
	m_nCollectNum = 20;
	m_nCollectCnt = 0;
	m_bInDataTracking = TRUE;
	if (!m_bTracking)
	{
		StartTracking();
	}
}

void CCryotherapyDlg::StopCollectTrackingData()
{
	if (m_bTracking)
	{
		StopTracking();
	}
	m_bInDataTracking = FALSE;
	m_iDataCollectionHandlerID = -1;
}

void CCryotherapyDlg::CalibratePT()
{
	CDlgCaliPT dlgCaliPT;
	dlgCaliPT.m_pCryoDlg = this;
	dlgCaliPT.m_vPTDirInScan=m_vPTDirInScan;
	m_uiFlag = FLAG_PT_CALI;
	if (dlgCaliPT.DoModal() == IDOK)
	{
		m_vPTDirInScan = dlgCaliPT.m_vPTDirInScan;
		
		m_dlgCmdBar.GetDlgItem(IDC_CMD_MOVE_PT)->EnableWindow(m_pTrackerHandler->IsValidDevice());
		m_dlgCmdBar.m_bTableCalibrated = TRUE;

		//CConfigMgr conf;
		//CString sFile = conf.Read(SYSINFO, SYS_PT_DIR_FILE, "");
		CString sFile = CONFIG_FILES[CF_PT_DIR];
		if(SavePatientTableDirFile(sFile))
		{
			CLog::Log("标定结果保存到日志文件.");
		}else
		{
			PanaMessageBox("标定结果无法保存到日志文件中.");
			CLog::Log("标定结果无法保存到日志文件中.");
		}
		
	}
	m_uiFlag = FLAG_NONE; 
}



void CCryotherapyDlg::CameraMove()
{
	if (!m_bTracking)
	{
		if (!StartTracking())
		{
			CLog::Log("移动相机后，相机开始跟踪启动失败");
			return;
		}
		
	}
	
	if (m_patientMarker == PATIENT_NO_MARKER ) //NAVI MODE
	{
		if (m_bBaseVisible == FALSE)
		{
			PanaMessageBox("Can't find base, please check camera position and redo!");
			CString sLog;
			sLog.Format("相机移动操作失败，失效原因：无法跟踪Base");
			CLog::Log(sLog);
			return;
		}
	}
	if (m_patientMarker == PATIENT_WITH_MARKER) //NAVI MODE
	{
		if (m_bMarkerVisible == FALSE)
		{
			PanaMessageBox("Can't find patient marker, please check camera position and redo!");
			CString sLog;
			sLog.Format("相机移动操作失败，失效原因：无法跟踪病床");
			CLog::Log(sLog);
			return;
		}
	}
	
	TransformEverythingToFinalCor();
	
	
	//PanaMessageBox("Move camera successfully.");
	m_dlgTrackingStatus.AddMessage("Move camera successfully.");
	CString sLog;
	sLog.Format("相机移动操作成功");
	CLog::Log(sLog);

	m_bRecordBase=TRUE;
}

void CCryotherapyDlg::SelTab(enum PASK_TabMode eTab)
{
	if (GetSafeHwnd())
	{
		m_tabMode=eTab;
		SetTabMode(eTab);
		switch (eTab)
		{
		case PASK_Tab_Patient:
			m_dlgPatient.ShowWindow(TRUE);
			m_dlgSeries.ShowWindow(FALSE);
			m_dlgScannerControl.ShowWindow(FALSE);
			m_dlgNavigate.ShowWindow(FALSE);
			m_dlgTps.ShowCtrl(FALSE);
			m_dlgScannerControl.m_dlgScanOption.ShowWindow(FALSE);
			break;
		case PASK_Tab_Series:
			m_dlgSeries.ShowWindow(TRUE);
			m_dlgPatient.ShowWindow(FALSE);
			m_dlgScannerControl.ShowWindow(FALSE);
			m_dlgTps.ShowCtrl(FALSE);
			m_dlgNavigate.ShowWindow(FALSE);
			m_dlgScannerControl.m_dlgScanOption.ShowWindow(FALSE);
			break;
		case PASK_Tab_MriControl:
			m_dlgScannerControl.ShowWindow(TRUE);
			m_dlgPatient.ShowWindow(FALSE);
			m_dlgSeries.ShowWindow(FALSE);
			m_dlgTps.ShowCtrl(FALSE);
			if(!m_pDisplay4Dlg->IsWindowVisible())
			{
				m_dlgTps.ShowDisp(FALSE);
			}
			m_dlgNavigate.ShowWindow(FALSE);
			break;
		case PASK_Tab_Navigate:
			m_dlgNavigate.ShowWindow(TRUE);
			m_dlgScannerControl.ShowWindow(FALSE);
			m_dlgPatient.ShowWindow(FALSE);
			m_dlgSeries.ShowWindow(FALSE);
			m_dlgTps.ShowCtrl(FALSE);
			if(!m_pDisplay4Dlg->IsWindowVisible())
			{
				m_dlgTps.ShowDisp(FALSE);
			}
			m_dlgScannerControl.m_dlgScanOption.ShowWindow(FALSE);
			break;
		case PASK_Tab_Tps:
			m_dlgTps.ShowCtrl(TRUE);
			m_dlgTps.ShowDisp(TRUE);
			m_dlgScannerControl.ShowWindow(FALSE);
			m_dlgPatient.ShowWindow(FALSE);
			m_dlgSeries.ShowWindow(FALSE);
			m_dlgNavigate.ShowWindow(FALSE);
			m_dlgScannerControl.m_dlgScanOption.ShowWindow(FALSE);
			break;
		default:
			m_dlgScannerControl.ShowWindow(FALSE);
			m_dlgScannerControl.m_dlgScanOption.ShowWindow(FALSE);
			m_dlgPatient.ShowWindow(FALSE);
			m_dlgSeries.ShowWindow(FALSE);
			m_dlgNavigate.ShowWindow(FALSE);
			m_dlgTps.ShowCtrl(FALSE);
			m_dlgTps.ShowDisp(FALSE);
			break;
		}
	}
}


void CCryotherapyDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDlgMainSkin::OnLButtonUp(nFlags, point);
	
	if(m_dlgNavigate.IsTracking())
		return;

	if(m_rcTabPat.PtInRect(point) && m_enumOperateType != OPERATION_TYPE_TPS 
		                          && m_enumOperateType != OPERATION_TYPE_REVIEW)
	{
		SelTab(PASK_Tab_Patient);
		return;
	}
	
	if(m_rcTabSeries.PtInRect(point))
	{
		SelTab(PASK_Tab_Series);
		SetNewSeriesAlarm(FALSE);
		return;
	}
	
	if(m_rcTabMriCtrl.PtInRect(point) && m_enumOperateType != OPERATION_TYPE_TPS
		                              && m_enumOperateType != OPERATION_TYPE_REVIEW)
	{
		SelTab(PASK_Tab_MriControl);
		return;
	}
	
	if(m_rcTabNavCtrl.PtInRect(point) && m_enumOperateType != OPERATION_TYPE_TPS &&
		                                 m_enumOperateType != OPERATION_TYPE_REVIEW)
	{
		SelTab(PASK_Tab_Navigate);
		return;
	}

	if(m_rcTabTpsCtrl.PtInRect(point) && m_enumOperateType != OPERATION_TYPE_REVIEW)
	{
		SelTab(PASK_Tab_Tps);
		return;
	}
}

void CCryotherapyDlg::RestoreTabDisplay()
{
	SelTab(m_tabMode);
}

void CCryotherapyDlg::SetCurBaseIndex(int index)
{
	m_dlgTrackingStatus.SetBaseIndex(index);
	m_iCurBaseIndex = index;
}


