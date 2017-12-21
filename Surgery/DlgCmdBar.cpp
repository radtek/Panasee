// DlgCmdBar.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgCmdBar.h"
#include "CryotherapyDlg.h"
#include "DlgBaseSelection.h"
#include "Dispaly4Dlg.h"
#include "Display.h"
#include "DlgMovePT.h"
#include "DlgMyAbout.h"
#include "DlgMain.h"
#include "GlobalFuncs.h"
#include "Log.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLBTNBKC RGB(236, 233, 216)

extern OPERATIONTYPE gStarOperType;    // get from command line argument           
extern CImriDBI m_DBmri;
extern CString m_sLogPicFolder;
extern CString g_ExecPath;
BOOL g_InOpenSeriesFromDBThread;
/////////////////////////////////////////////////////////////////////////////
// CDlgCmdBar dialog


CDlgCmdBar::CDlgCmdBar(CWnd* pParent /*=NULL*/)
	: CDlgToolSkin(CDlgCmdBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCmdBar)
	m_bChkMeasure = FALSE;
	m_bChkVTarget = FALSE;
	m_bChkLR = FALSE;
	//}}AFX_DATA_INIT

	g_InOpenSeriesFromDBThread=FALSE;
	m_bTuneInNextScan=FALSE;
	m_bTableCalibrated = FALSE;
}


void CDlgCmdBar::DoDataExchange(CDataExchange* pDX)
{
	CDlgToolSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCmdBar)
	DDX_Control(pDX, IDC_BTN_SYS2, m_BtnSys);
	DDX_Control(pDX, IDC_CMD_MOVE_PT, m_btnMovePT);
	DDX_Control(pDX, IDC_CHK_TARGET, m_btnVTarget);
	DDX_Control(pDX, IDC_CHK_MEASURE, m_btnMeasure);
	DDX_Control(pDX, IDC_BTN_SERIES_DISPLAY, m_btnImgDisplay);
	DDX_Control(pDX, IDC_CMD_SCREEN_CAP, m_btnScreenSnap);
	DDX_Control(pDX, IDC_CMD_COMPARE, m_btnImgCompare);
	DDX_Control(pDX, IDC_CMD_CAMER_MOVE, m_btnCameraMove);
	DDX_Control(pDX, IDC_CMD_START_TRACKING, m_btnTracking);
	DDX_Check(pDX, IDC_CHK_MEASURE, m_bChkMeasure);
	DDX_Check(pDX, IDC_CHK_TARGET, m_bChkVTarget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCmdBar, CDlgToolSkin)
	//{{AFX_MSG_MAP(CDlgCmdBar)
	ON_BN_CLICKED(IDC_CMD_COMPARE, OnCmdCompare)
	ON_BN_CLICKED(IDC_CMD_START_TRACKING, OnCmdStartTracking)
	ON_BN_CLICKED(IDC_BTN_SERIES_DISPLAY, OnBtnSeriesDisplay)
	ON_BN_CLICKED(IDC_CHK_MEASURE, OnChkMeasure)
	ON_BN_CLICKED(IDC_CMD_CAMER_MOVE, OnCmdCamerMove)
	ON_BN_CLICKED(IDC_CMD_SCREEN_CAP, OnCmdScreenCap)
	ON_BN_CLICKED(IDC_CHK_TARGET, OnChkTarget)
	ON_BN_CLICKED(IDC_CMD_MOVE_PT, OnCmdMovePt)
	ON_COMMAND(ID_SYS_EXIT, OnSysExit)
	ON_COMMAND(ID_SYS_IMPORT_SERIES, OnSysImportSeries)
	ON_COMMAND(ID_SYS_OPEN_DB, OnSysOpenDb)
	ON_COMMAND(ID_SYS_ABOUT, OnSysAbout)
	ON_COMMAND(ID_SYS_CALI_PT, OnSysCaliPt)
	ON_COMMAND(ID_SYS_CONNECT_CAM, OnSysConnectCam)
	ON_MESSAGE(WM_SYS_MENU_POPUP, On_MenuCallback)
	ON_MESSAGE(WM_SYS_DB_OK, OnSysDBOK)
	ON_COMMAND(ID_SYS_CONNECT_MRI, OnSysConnectMri)
	ON_WM_MEASUREITEM()
	ON_COMMAND_RANGE(WM_PROTCOL_FAMILY_START, WM_PROTCOL_FAMILY_END, OnMsgProtoclFamily)
	ON_COMMAND(ID_SYS_START_MONITOR, OnSysStartMonitor)
	ON_COMMAND(ID_SYS_CALI_TUNE, OnSysCaliTune)
	ON_COMMAND(ID_SYS_SURGERY_WIZARD, OnSysSurgeryWizard)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCmdBar message handlers

void CDlgCmdBar::OnMsgProtoclFamily(UINT nID)
{
	CDlgMain* pDlgMain = g_DlgMain;
	
	if(pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_bProgress==TRUE||
		pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_ePSBT != PSBT_SCAN)
	{
		if(pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_bProgress==TRUE)
		{
			PanaMessageBox("Please stop current scanning firstly.");
		}else
		{
			PanaMessageBox("Please stop and quit circle scanning firstly.");
		}
		return;
	} //nsj 11.07.08

	CString sTmp;
	int iIndex=nID-WM_PROTCOL_FAMILY_START;
	POSITION pos = pDlgMain->m_MRIWrap.m_lstProtocolFamily.FindIndex(iIndex);
	
	if(pos)
	{
		IMRI_PROTOCOL_FAMILY family=pDlgMain->m_MRIWrap.m_lstProtocolFamily.GetNext(pos);
		IMRI_PROTOCOLGROUP_DESC proFamily;
		proFamily.nProtocolGroupNo=family.nID;
		sprintf((char*)proFamily.cProtocolGroupDesc,"%s",family.sDesc);
		pDlgMain->m_MRIWrap.ChangeProtocolFamily(proFamily);
		ShowWaiting(1,"Changing protocol family..."); //显示等待窗口
	}
}


void CDlgCmdBar::OnCmdCompare() 
{
	// TODO: Add your control notification handler code here
	
	CLog::Log("比较序列按钮被按下");
	CDlgMain* pDlgMain = g_DlgMain;
	pDlgMain->ShowCompareDlg();
}

void CDlgCmdBar::OnCmdStartTracking() 
{
	// TODO: Add your control notification handler code here
	CLog::Log("转换跟踪状态按钮被按下");
	if (  !m_pCryDlg->m_bTracking )
	{
		m_pCryDlg->StartTracking();
	}
	else
	{
		m_pCryDlg->StopTracking();
	}

	if (m_pCryDlg->m_bTracking)
	{
//		m_btnTracking.SetBitmaps(IDB_TRACKING2, RGB(236, 233, 216), (int)BTNST_AUTO_DARKER);
		m_btnTracking.SetChecked(TRUE);
		m_btnTracking.SetTooltipText("Stop tracking", TRUE);
	}
	else
	{
//		m_btnTracking.SetBitmaps(IDB_TRACKING, RGB(236, 233, 216), (int)BTNST_AUTO_DARKER);
		m_btnTracking.SetChecked(FALSE);
		m_btnTracking.SetTooltipText("Start tracking", TRUE);
	}
}

void CDlgCmdBar::OnBtnSeriesDisplay() 
{
	// TODO: Add your control notification handler code here
	CLog::Log("显示序列按钮被按下");
	CDlgMain* pDlgMain = g_DlgMain;
	CMRISeries *pSeries;
	if (pDlgMain->m_dlgCryotherapy.m_pCurSelSeries)
	{
		pSeries=pDlgMain->m_dlgCryotherapy.m_pCurSelSeries;
	}
	else
	{
		pSeries=pDlgMain->m_dlgCryotherapy.m_MRIBuffer.GetDisp();
	}
	pDlgMain->ShowSeriesDisplay(pSeries,TRUE);
	
}

void CDlgCmdBar::EnableMeasure(BOOL bEnable)
{
	m_bChkMeasure = bEnable;
	CDlgMain* pDlgMain = g_DlgMain;
	pDlgMain->m_display4Dlg.EnableDistMeasure(m_bChkMeasure);
	m_btnMeasure.SetChecked(m_bChkMeasure);
	m_pCryDlg->m_pDisplay4Dlg->SetDispWndCursor(m_bChkMeasure ? MAKEINTRESOURCE(IDC_CUR_MEASURE) : IDC_ARROW);
}

void CDlgCmdBar::OnChkMeasure() 
{
	// TODO: Add your control notification handler code here
	m_bChkMeasure = !m_bChkMeasure;

	if (m_bChkMeasure)
	{
		EnableCreatVTarget(FALSE);
		EnableLR(FALSE);
	}

	EnableMeasure(m_bChkMeasure);

	if (m_bChkMeasure)
	{
		CLog::Log("测量按钮被按下,开始测量");
	}
	else
	{
		CLog::Log("测量按钮被按下,取消测量");
	}
}

void CDlgCmdBar::OnCmdCamerMove() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pDlgMain = g_DlgMain;
	if(pDlgMain->m_DeviceType==DEVICE_IGS_MF)
	{
	CLog::Log("移动相机按钮被按下");	
	m_pCryDlg->CameraMove();	
	}else
	{
		m_bChkLR = !m_bChkLR;
		if (m_bChkLR)
		{
			EnableMeasure(FALSE);
			EnableCreatVTarget(FALSE);
		}
		
		EnableLR(m_bChkLR);
	}

}

void CDlgCmdBar::OnCmdScreenCap() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pMainFrm = g_DlgMain;
	CDC* hSrcCDC;
	HDC   hScrDC,   hMemDC;           
	HBITMAP   hBitmap,hOldBitmap;     
	int               nX,   nY,   nX2,   nY2;           
	int               nWidth,   nHeight;           
	int               xScrn,   yScrn;                 
	
	//为屏幕创建设备描述表 
	hSrcCDC   =   pMainFrm->GetWindowDC();
	if   (hSrcCDC==0)     
	{
		return;
	}

	hScrDC = hSrcCDC->m_hDC;
	
	//为屏幕设备描述表创建兼容的内存设备描述表 
	hMemDC   =   CreateCompatibleDC(hScrDC); 
	if   (hMemDC==0)     
	{
		return;
	}

	CRect rect;
	pMainFrm->GetWindowRect(&rect);
	//   获得屏幕分辨率 
	xScrn   =   GetDeviceCaps(hScrDC,   HORZRES); 
	yScrn   =   GetDeviceCaps(hScrDC,   VERTRES); 
	
	//   获得选定区域坐标 
	nX   =   rect.left > 0 ? rect.left : 0; 
	nY   =   rect.top > 0 ? rect.top : 0; 
	nX2   =   rect.right; 
	nY2   =   rect.bottom; 
	nWidth   =   nX2   -   nX+1; 
	nHeight   =   nY2   -   nY+1; 

	//   创建一个与屏幕设备描述表兼容的位图 
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight); 
	if   (hBitmap==0)     
	{
		return;
	}
	//   把新位图选到内存设备描述表中 
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap); 
	//   把屏幕设备描述表拷贝到内存设备描述表中 
	BitBlt(hMemDC,0,0,   nWidth,nHeight,hScrDC,   nX,   nY,   SRCCOPY); 
	//得到屏幕位图的句柄 
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap); 
	
	//清除 
	ReleaseDC(hSrcCDC); 
	DeleteDC(hMemDC); 
	DeleteObject(hOldBitmap);
	
	// Get bmp store file name
	CString sFileName;
	CString sPath;
	CString sPicPath = m_dlgPatSearch.m_pdbImri->GetDatabaseFolder();
	CTime c = CTime::GetCurrentTime();

	if (gStarOperType > OPERATION_TYPE_NONE && gStarOperType < OPERATION_TYPE_TPS)
	{
		CDlgMain* pDlgMain = g_DlgMain;		
		IGT_PatientField* pPatient = &(pDlgMain->m_MRIWrap.m_CurPatient);

		sPicPath = sPicPath.Left(sPicPath.GetLength() - 8)+"Capture\\"+pPatient->Id;
		CreateDirectory(sPicPath, 0);
		sFileName.Format("%s\\%02d%02d%02d.bmp", sPicPath, c.GetHour(), c.GetMinute(), c.GetSecond());	
	}else
	{
		sPicPath = sPicPath.Left(sPicPath.GetLength() - 8)+"Log";	
		sPath.Format("%s\\%d%02d%02d-%02d%02d", sPicPath, c.GetYear(), c.GetMonth(), c.GetDay(), c.GetHour(), c.GetMinute());

		CreateDirectory(sPath, 0);
		sFileName.Format("%s\\%02d%02d%02d.bmp", sPath, c.GetHour(), c.GetMinute(), c.GetSecond());
	}
	
	CBitmap bmp;
	bmp.Attach(hBitmap);
	CString sMsg;
	if (CGlobalFuncs::SaveBitmapFile(sFileName, bmp))
	{
		sMsg.Format("保存屏幕图像到 %s", sFileName);
	}
	else
	{
		sMsg = "保存屏幕图像失败!";
	}
	CLog::Log(sMsg);
	bmp.Detach();
	DeleteObject(hBitmap);
}


BOOL CDlgCmdBar::OnInitDialog() 
{
	SetCameraBtn(&m_btnTracking);
	SetMeasureBtn(&m_btnMeasure);
	SetTargetBtn(&m_btnVTarget);
	SetPatientTableBtn(&m_btnMovePT);
	SetMoveCameraBtn(&m_btnCameraMove);
	SetBrowseImageBtn(&m_btnImgDisplay);
	SetCompareImageBtn(&m_btnImgCompare);
	SetCaptureBtn(&m_btnScreenSnap);
	SetSystemBtn(&m_BtnSys);

	m_btnTracking.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_btnMeasure.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_btnVTarget.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_btnCameraMove.SetButtonMode(CImageButton::ImageBtn_CheckButton);

	CDlgToolSkin::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bLastTrackingStatus = FALSE;

	m_btnTracking.SetWindowText("");
	m_btnMeasure.SetWindowText("");
	m_btnVTarget.SetWindowText("");
	m_btnMovePT.SetWindowText("");
	m_btnCameraMove.SetWindowText("");
	m_btnImgDisplay.SetWindowText("");
	m_btnImgCompare.SetWindowText("");
	m_btnScreenSnap.SetWindowText("");
	m_BtnSys.SetWindowText("");


	m_btnTracking.SetTooltipText("Start tracking", TRUE);
	//m_btnTracking.ActivateTooltip(TRUE);
	m_btnTracking.EnableWindow(FALSE);
	
	m_btnMeasure.SetTooltipText("Measure distance", TRUE);

	m_btnMovePT.SetTooltipText("Move patient table", TRUE);


	m_btnCameraMove.SetTooltipText("Move camera", TRUE);
	m_btnCameraMove.EnableWindow(FALSE);


	m_btnImgDisplay.SetTooltipText("Display series", TRUE);


	m_btnImgCompare.SetTooltipText("Compare series", TRUE);


	m_btnVTarget.SetTooltipText("Create target", TRUE);


	m_btnScreenSnap.SetTooltipText("Capture screen", TRUE);

	CDlgMain* pDlgMain = g_DlgMain;
	BOOL bIsIGS_CP=FALSE;
	if(pDlgMain->m_DeviceType == DEVICE_IGS_CP)
	{
		bIsIGS_CP=TRUE;
	}
	m_sysMenu.SetPopupMenu(TRUE);
	if(bIsIGS_CP)
	{
		m_sysMenu.LoadMenu(IDR_SYS_CP);
	}else
	{
		m_sysMenu.LoadMenu(IDR_SYS);
	}
		
	m_sysMenu.GetSubMenu(0)->AdjustMenuItem();
	m_BtnSys.SetBtnMenu(&m_sysMenu);
	m_BtnSys.SetClickedCallback(this->GetSafeHwnd(), WM_SYS_MENU_POPUP, 1);


	CConfigMgr conf;
	CString sModalityID = conf.Read(SYSINFO, SYS_LAST_MODALITY, "001");

	m_dlgPatSearch.SetDB(&m_DBmri);
	m_dlgPatSearch.SetInitialModalityID(sModalityID);
	m_dlgPatSearch.SetOnOKCallbackMessage(this, WM_SYS_DB_OK);
	if (m_dlgPatSearch.GetSafeHwnd() == NULL)
	{
		m_dlgPatSearch.Create(CPatientInfoMrgDlg::IDD, this);
	}
	m_dlgPatSearch.ShowWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCmdBar::UpdateUI()
{
	m_btnMovePT.EnableWindow(gStarOperType>OPERATION_TYPE_NONE &&
		                     gStarOperType<OPERATION_TYPE_TPS  &&
							 m_bTableCalibrated == TRUE);

	if(m_pCryDlg->m_dlgNavigate.IsTracking())
	{
		m_btnVTarget.EnableWindow(FALSE);
		m_btnMeasure.EnableWindow(FALSE);
	}else
	{
		m_btnVTarget.EnableWindow(TRUE);
		m_btnMeasure.EnableWindow(TRUE);
	}

	if(!m_pCryDlg->m_pTrackerHandler)
	{
		return;
	}
	m_btnTracking.EnableWindow( m_pCryDlg->m_pTrackerHandler->IsValidDevice());

	m_bLastTrackingStatus = !m_pCryDlg->m_bTracking;
	if (m_pCryDlg->m_bTracking)
	{
		
		m_btnTracking.SetTooltipText("Stop tracking", TRUE);
		m_btnTracking.SetChecked(TRUE);
	}
	else
	{
		
		m_btnTracking.SetTooltipText("Start tracking", TRUE);
		m_btnTracking.SetChecked(FALSE);
	}


	// - End
	m_btnCameraMove.EnableWindow(m_pCryDlg->m_pTrackerHandler->IsValidDevice());
	
	CDlgMain* pDlgMain = g_DlgMain;
	if(pDlgMain->m_DeviceType==DEVICE_IGS_MF) // 此时这个按钮的作用是切换图片当前序列
	{
		m_btnCameraMove.EnableWindow(TRUE);
		m_btnMovePT.EnableWindow(FALSE);
	}

	
}

void CDlgCmdBar::OnChkTarget() 
{
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
	m_bChkVTarget = !m_bChkVTarget;
//	m_btnVTarget.SetChecked(m_bChkVTarget);
	if (m_bChkVTarget)
	{
		EnableMeasure(FALSE);
		EnableLR(FALSE);
	}

	EnableCreatVTarget(m_bChkVTarget);

}

void CDlgCmdBar::EnableLR(BOOL bEnable)
{
	m_bChkLR = bEnable;
	
	m_btnCameraMove.SetChecked(m_bChkLR);
	
	m_pCryDlg->m_pDisplay4Dlg->EnableLR(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->EnableLR(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->EnableLR(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->EnableLR(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->EnableLR(bEnable);
	
	if (m_bChkLR)
	{
		CLog::Log("Start click Left Right Arrow");
	}
	else
	{
		CLog::Log("Cancel Left Right Arrow");
	}
	m_pCryDlg->m_pDisplay4Dlg->SetDispWndCursor(m_bChkLR ? MAKEINTRESOURCE(IDC_CUR_LR) : IDC_ARROW);
}

void CDlgCmdBar::EnableCreatVTarget(BOOL bEnable)
{
	m_bChkVTarget = bEnable;
//	UpdateData(FALSE);

	m_btnVTarget.SetChecked(m_bChkVTarget);

	m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.EnableClickTargetOnMRI(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.EnableClickTargetOnMRI(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.EnableClickTargetOnMRI(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.EnableClickTargetOnMRI(bEnable);
	m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.EnableClickTargetOnMRI(bEnable);

	if (m_bChkVTarget)
	{
		CLog::Log("Start click virtual target");
	}
	else
	{
		CLog::Log("Cancel create virtual target");
	}
	m_pCryDlg->m_pDisplay4Dlg->SetDispWndCursor(m_bChkVTarget ? MAKEINTRESOURCE(IDC_TARGET) : IDC_ARROW);
}


void CDlgCmdBar::OnCmdMovePt() 
{
	// TODO: Add your control notification handler code here
	CString sLog;
	CDlgMovePT dlgMovePt;
	dlgMovePt.m_dCurPos = m_pCryDlg->m_dbRealPTPos;
	if (m_pCryDlg->m_patientMarker==PATIENT_NO_MARKER) //NAVI MODE
	{
		if (dlgMovePt.DoModal() == IDOK)
		{
			m_pCryDlg->m_dbRealPTPos = dlgMovePt.m_dCurPos;
			m_pCryDlg->TransformEverythingToFinalCor();	
			m_pCryDlg->m_dlgTrackingStatus.AddMessage("Move table successfully.");
			sLog.Format("Currect table position:%.2f",dlgMovePt.m_dCurPos);
			m_pCryDlg->m_dlgTrackingStatus.AddMessage(sLog);
			sLog.Format("病床移动成功,当前病床位置:%.2f",dlgMovePt.m_dCurPos);
			CLog::Log(sLog);
		}
	}

	
	if (m_pCryDlg->m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
	{
		if(m_pCryDlg->m_TableShiftMode == NO_SHIFT_IN_SCANING)
		{
			if (m_pCryDlg->m_bMarkerVisible == FALSE)
			{
				PanaMessageBox("Can't find Table, please check camera position and redo!");
			
				return;
			}
			Matrix tTrack2Marker = m_pCryDlg->m_tMarker.Inverse();
			m_pCryDlg->m_tranScanner2Marker = tTrack2Marker * m_pCryDlg->m_tranScanner2Tracker;
			m_pCryDlg->TransformEverythingToFinalCor();	
			m_pCryDlg->m_dlgTrackingStatus.AddMessage("Move table successfully.");
			sLog.Format("病床移动成功,病床上有定位标记.");
			CLog::Log(sLog);
		}
	}
	

	
}

BOOL CDlgCmdBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;     
	
	return CDialog::PreTranslateMessage(pMsg);
}



LRESULT CDlgCmdBar::On_MenuCallback(WPARAM wParam, LPARAM lParam)
{
	CDlgMain* pDlgMain = g_DlgMain;
	CConfigMgr conf;
	BOOL bHasCamera = pDlgMain->m_bHasCamera;

	if ((int)lParam == 1)
	{
		BOOL bIsIGS_CP=FALSE;
		if(pDlgMain->m_DeviceType == DEVICE_IGS_CP)
		{
			bIsIGS_CP=TRUE;
		}

		m_sysMenu.SetPopupMenu(TRUE);
		if(bIsIGS_CP)
		{
			m_sysMenu.LoadMenu(IDR_SYS_CP);
			m_sysMenu.GetSubMenu(0)->AdjustMenuItem();

		}else
		{
			m_sysMenu.LoadMenu(IDR_SYS);

			CMenuEx subMenu;
			subMenu.SetPopupMenu(TRUE);
			subMenu.CreatePopupMenu();
			
			
			CString sTmp;
			int iIndex=0;
			
			POSITION pos=pDlgMain->m_MRIWrap.m_lstProtocolFamily.GetHeadPosition();
			while(pos)
			{
				IMRI_PROTOCOL_FAMILY family=pDlgMain->m_MRIWrap.m_lstProtocolFamily.GetNext(pos);
				sTmp.Format("%s",family.sDesc);
				subMenu.AppendMenu(MF_STRING, WM_PROTCOL_FAMILY_START + iIndex, sTmp);
				iIndex++;
			}
			
			if(iIndex>0 && m_pCryDlg->m_pDlgMain->m_MRIWrap.m_bMRIConneted)
			{
				m_sysMenu.GetSubMenu(0)->InsertMenu(8,MF_POPUP |MF_BYPOSITION, (UINT)subMenu.m_hMenu, "Change Protocol Family");
			}
			
			m_sysMenu.GetSubMenu(0)->AdjustMenuItem();

			m_sysMenu.EnableMenuItem(ID_SYS_CONNECT_MRI, 
				(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS
				&& m_pCryDlg->m_pDlgMain->m_MRIWrap.m_bEnableMRICtrl 
				&& !m_pCryDlg->m_pDlgMain->m_MRIWrap.m_bMRIConneted && !bIsIGS_CP) 
				? MF_ENABLED : MF_GRAYED);

			m_sysMenu.EnableMenuItem(ID_SYS_CALI_PT, 
				(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS
				&& (m_pCryDlg->m_pTrackerHandler && m_pCryDlg->m_pTrackerHandler->IsValidDevice()) && !bIsIGS_CP) 
				? MF_ENABLED : MF_GRAYED);
			
			
			m_sysMenu.EnableMenuItem(ID_SYS_CALI_TUNE, 
				(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS
				&& m_pCryDlg->m_pDlgMain->m_MRIWrap.m_bEnableMRICtrl 
				&& m_pCryDlg->m_pDlgMain->m_MRIWrap.m_bMRIConneted && !bIsIGS_CP) 
				? MF_ENABLED : MF_GRAYED);			
			
			m_sysMenu.CheckMenuItem(ID_SYS_CALI_TUNE, m_bTuneInNextScan ? MF_CHECKED : MF_UNCHECKED);
		}
		
		
		m_BtnSys.SetBtnMenu(&m_sysMenu);

		
		m_sysMenu.EnableMenuItem(ID_SYS_CONNECT_CAM, 
			(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS
			&& bHasCamera 
			&& !(m_pCryDlg->m_pTrackerHandler && m_pCryDlg->m_pTrackerHandler->IsValidDevice())) 
			? MF_ENABLED : MF_GRAYED);

		

		m_sysMenu.EnableMenuItem(ID_SYS_SURGERY_WIZARD, 
			(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS) 
			? MF_ENABLED : MF_GRAYED);

		m_sysMenu.EnableMenuItem(ID_SYS_START_MONITOR, 
			(gStarOperType>OPERATION_TYPE_NONE && gStarOperType<OPERATION_TYPE_TPS) 
			? MF_ENABLED : MF_GRAYED);		
		
	}
	return 0;
}

void CDlgCmdBar::OnSysExit() 
{
	// TODO: Add your command handler code here
	::SendMessage(g_DlgMain->m_hWnd,WM_CLOSE,0,0);
}

void CDlgCmdBar::OnSysImportSeries() 
{
	// TODO: Add your command handler code here
	SysImportSeries();
}

void CDlgCmdBar::OnSysOpenDb() 
{
	// TODO: Add your command handler code here
	OpenDatabase();
}

void CDlgCmdBar::OnSysAbout() 
{
	// TODO: Add your command handler code here
	CDlgMyAbout aboutDlg;
	aboutDlg.DoModal();
}

void CDlgCmdBar::OnSysCaliPt() 
{
	// TODO: Add your command handler code here
	m_pCryDlg->CalibratePT();	
}

void CDlgCmdBar::OnSysConnectCam() 
{
	// TODO: Add your command handler code here
	
	if(m_pCryDlg->m_pTrackerHandler==NULL)
		return;

	if (m_pCryDlg->m_pTrackerHandler->InitNDI())
	{
		m_pCryDlg->StartTracking();
	}else
	{
		PanaMessageBox(m_pCryDlg->m_pTrackerHandler->GetErrorInfo());
	}
}

void CDlgCmdBar::OnSysConnectMri() 
{
	// TODO: Add your command handler code here
	CDlgMain *pDlgMain = g_DlgMain;

	if (pDlgMain->m_MRIWrap.ConnectMRI())
	{
		 //再次确认病人信息MED联调修改 nsj 11.07.08
		pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_MODALITY);
		CLog::Log("请求Modality信息");
			pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_PROTOCOLS);
		pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_PATIENT);
		
	}
}

INT CALLBACK BrowseCallbackProc(HWND hwnd, 
                                UINT uMsg,
                                LPARAM lp, 
                                LPARAM pData) 
{
   TCHAR szDir[MAX_PATH];

   switch(uMsg) 
   {
   case BFFM_INITIALIZED:
	  {
			CConfigMgr conf;
			CString sFolder = conf.Read(SYSINFO, SYS_OPEN_DIALOG_INIT_FOLDER, "C:");
			memcpy(szDir, sFolder.GetBuffer(2), sFolder.GetLength());
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
	  }
      break;

   case BFFM_SELCHANGED: 
      // Set the status window to the currently selected path.
      if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir))
      {
         SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
      }
      break;
   }
   return 0;
}

CString g_sFolderName="";
UINT OpenSeriesFromFolder(LPVOID pParam )
{
	CCryotherapyDlg *pCryoDlg=(CCryotherapyDlg*)pParam;
	if(pCryoDlg==NULL || g_sFolderName=="")
		return 0;

	try
	{
		pCryoDlg->AddMRISeries(g_sFolderName);
		
		AfxEndThread(0,TRUE);
		return 0;
	}catch(...)
	{
		
		AfxEndThread(-1,TRUE);
		return -1;
	}

	return 0;
}


BOOL CDlgCmdBar::SysImportSeries()
{
	HRESULT hr;
	LPITEMIDLIST pItemList;
	BROWSEINFO browseinfo;
	TCHAR path[MAX_PATH];

	IMalloc *pMalloc = NULL;
	hr = SHGetMalloc(&pMalloc);
	if (FAILED(hr))
	{
		
		return FALSE;
	}

	memset(&browseinfo, 0, sizeof(BROWSEINFO));
	CString sCaption;
	browseinfo.hwndOwner = GetSafeHwnd();
	browseinfo.pszDisplayName = path;
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browseinfo.lpszTitle = "Select MRI folder";
	browseinfo.lpfn = BrowseCallbackProc;

	TCHAR pszPath[MAX_PATH];
	pItemList = ::SHBrowseForFolder(&browseinfo);
	
	CString sFolderName="";
	if (pItemList)
	{
		::SHGetPathFromIDList(pItemList, pszPath);
		sFolderName = pszPath;
		pMalloc->Free(pItemList);
		UpdateData(FALSE);

		CConfigMgr conf;
		conf.Write(SYSINFO, SYS_OPEN_DIALOG_INIT_FOLDER, sFolderName);
	}

	pMalloc->Release();
	pMalloc = NULL;

	if (sFolderName != "")
	{
		g_sFolderName=sFolderName;
		CDlgMain *pDlgMain = g_DlgMain;
		AfxBeginThread(OpenSeriesFromFolder,(LPVOID)(&(pDlgMain->m_dlgCryotherapy)),THREAD_PRIORITY_LOWEST,0,0,NULL);
		return TRUE;
	}	
	return FALSE;
}


void CDlgCmdBar::OpenDatabase()
{
	
	m_dlgPatSearch.ShowWindow(TRUE);
}

UINT OpenSeriesFromDB(LPVOID pParam )
{
	CDlgCmdBar *pDlgCmdBar=(CDlgCmdBar*)pParam;
	if(pDlgCmdBar==NULL)
		return 0;
	
	g_InOpenSeriesFromDBThread=TRUE;
	char sTmp[512];
	
	try
	{
		char csSutId[100][256];
		int     nSerId[100] ;
		char csMId[100][256];
		int nNum = pDlgCmdBar->m_dlgPatSearch.GetSelectedSeriesNum() ;
		if(nNum>100)
		{
			nNum=100;
		}
		sprintf(sTmp,"打开数据库：选择%d个序列图像.", nNum);
		CLog::Log(sTmp);

		pDlgCmdBar->m_dlgPatSearch.GetSelectedSeries(csSutId,nSerId, csMId,nNum);
		if (nNum > 0)
		{
			pDlgCmdBar->m_pCryDlg->AddDatabaseResult(nNum, csSutId, nSerId, csMId);
		}

		

		g_InOpenSeriesFromDBThread=FALSE;
		AfxEndThread(0,TRUE);
		
		return 0;
	}catch(...)
	{
		sprintf(sTmp,"从数据库打开图像序列失败.");
		CLog::Log(sTmp);

		g_InOpenSeriesFromDBThread=FALSE;
		AfxEndThread(-1,TRUE);
		
		return -1;
	}

	g_InOpenSeriesFromDBThread=FALSE;
	AfxEndThread(0,TRUE);
	
	return 0;
}

LRESULT CDlgCmdBar::OnSysDBOK(WPARAM wParam, LPARAM lParam)
{
	
	AfxBeginThread(OpenSeriesFromDB,(LPVOID)this,THREAD_PRIORITY_LOWEST,0,0,NULL);
	return 0;
}

void CDlgCmdBar::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_sysMenu.MeasureItem(lpMeasureItemStruct);
	
	CDlgToolSkin::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CDlgCmdBar::OnSysStartMonitor() 
{
	// TODO: Add your command handler code here
	CString sTmp;
#ifdef _DEBUG
	sTmp.Format("%s..\\..\\Monitor\\MonitorD.exe",g_ExecPath);
#else
	sTmp.Format("%s..\\..\\Monitor\\Monitor.exe",g_ExecPath);
#endif

	CFileFind finder;
	if (!finder.FindFile(sTmp))
	{
		CString sMsg;
		sMsg.Format("Can't find monitor program.\nIt should be in:\n%s",sTmp);
		PanaMessageBox(sMsg);
		return;
	}

	if(FindWindow(NULL,_T("Multi-Monitor")))
	{
		CWnd *pWnd=FindWindow(NULL,_T("Multi-Monitor"));
		pWnd->SendMessage(WM_CLOSE);
	}
	else
	{
		ShellExecute(0,"open",sTmp,"1","",SW_SHOW ); 
	}
}

void CDlgCmdBar::OnSysCaliTune() 
{
	// TODO: Add your command handler code here
	if(m_bTuneInNextScan)
	{
		m_bTuneInNextScan=FALSE;
	}else
	{
		m_bTuneInNextScan=TRUE;
	}
}

void CDlgCmdBar::OnSysSurgeryWizard() 
{
	// TODO: Add your command handler code here
	CDlgMain *pDlgMain = g_DlgMain;
 
	if(pDlgMain->m_dlgCryotherapy.m_dlgNavigate.IsTracking())
	{
		PanaMessageBox("Please stop navigating firstly.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_bProgress==TRUE||
		pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_ePSBT != PSBT_SCAN)
	{
		if(pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_bProgress==TRUE)
		{
			PanaMessageBox("Please stop current scanning firstly.");
		}else
		{
			PanaMessageBox("Please stop and quit circle scanning firstly.");
		}
		return;
	}

	pDlgMain->m_dlgWizard.ShowWindow(TRUE);
	pDlgMain->m_dlgWizard.m_bExitWholeProgram=FALSE;
	pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.m_bEnableEdit = FALSE;
	pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.m_bSendCreateStudyCommand=FALSE;
	pDlgMain->m_dlgCryotherapy.ShowWindow(FALSE);
	pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowCtrl(FALSE);
	pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowDisp(FALSE);
	pDlgMain->m_display4Dlg.ShowWindow(FALSE);
	pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_OK); 
}


