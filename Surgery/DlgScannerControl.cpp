// DlgScannerControl.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgScannerControl.h"
#include "CryotherapyDlg.h"
#include "Probe.h"
#include "GlobalFuncs.h"
#include "CrashRstore.h"
#include "CryotherapyProbeSetting.h"
#include "BiopsyProbeSetting.h"
#include "Config.h"
#include "ConfigMgr.h"
#include "TrackTool.h"
#include "DlgMRIWrap.h"
#include "DlgMain.h"
#include "Log.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_PUSH_DEPTH (-200)
#define MAX_PUSH_DEPTH (200)


// For scan progress bar
#define SCAN_MAX_RANGE 500
UINT PreScanProgress(LPVOID pParam);
int  m_iEstimatedScanTime;    // ms


int  m_iScanedTime;

CCryotherapyDlg* g_pCryDlg;

extern OPERATIONTYPE gStarOperType;

/////////////////////////////////////////////////////////////////////////////
// CDlgScannerControl dialog


CDlgScannerControl::CDlgScannerControl(CWnd* pParent /*=NULL*/)
	: CDlgMriCtrlSkin(CDlgScannerControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScannerControl)
	m_iLocalizationMode = 0;
	m_iLocalizationPlane = 0;
	m_bChkSwap = FALSE;
	m_nOffset = 0;
	//}}AFX_DATA_INIT

	m_pCurMRIProtocol = NULL;
	m_pCryDlg = NULL;

	m_pCurRealProbe = NULL;

	m_ScanOption.bCycle=FALSE;
	m_ScanOption.iInterval=50;

	m_ScanOption.bSwap=m_bChkSwap;
	
	m_ScanOption.fGap=2;
	m_ScanOption.iNex=1;
	m_ScanOption.iSlice=10;
	m_ScanOption.fThickness=2;
	
	m_ScanOption.iOrient=3;
	m_ScanOption.iReferPos=0;

	m_iCntCycleScan  = 0;

	m_bPreScanDone = FALSE;
	m_bInPause = FALSE;

	m_bProgress=FALSE;

	m_bNoRefreshInScroll=FALSE;
}


void CDlgScannerControl::DoDataExchange(CDataExchange* pDX)
{
	CDlgMriCtrlSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScannerControl)
	DDX_Control(pDX, IDC_SCAN_TIME, m_scanTime);
	DDX_Control(pDX, IDC_CMD_SUB, m_btnSubOffset);
	DDX_Control(pDX, IDC_CMD_SCROLL, m_btnScrollOffset);
	DDX_Control(pDX, IDC_CMD_ADD, m_btnAddOffset);
	DDX_Control(pDX, IDC_EDT_OFFSET2, m_edtOffset);
	DDX_Control(pDX, IDC_LBL_UNIT, m_lblLengthUnit);
	DDX_Control(pDX, IDC_LBL_PROTOCOL, m_lblProtocol);
	DDX_Control(pDX, IDC_STATIC_ORIENTATION, m_lblOrientation);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_lblPosition);
	DDX_Control(pDX, IDC_STATIC_SWAP, m_lblSwap);
	DDX_Control(pDX, IDC_CMD_CHK_SWAP_OFF, m_btnSwapOff);
	DDX_Control(pDX, IDC_CMD_CHK_SWAP_ON, m_btnSwapOn);
	DDX_Control(pDX, IDC_CMD_CHK_ADVANCE, m_btnChkAdv);
	DDX_Control(pDX, IDC_CMD_RADIO_PLANE3, m_btnPlane3);
	DDX_Control(pDX, IDC_CMD_RADIO_PLANE2, m_btnPlane2);
	DDX_Control(pDX, IDC_CMD_RADIO_PLANE1, m_btnPlane1);
	DDX_Control(pDX, IDC_BTN_PROBE_DELETE, m_btnDelProbe);
	DDX_Control(pDX, IDC_BTN_PROBE_SIMULATE2, m_btnSimulate);
	DDX_Control(pDX, IDC_BTN_PROBE_NEW, m_btnNewProbe);
	DDX_Control(pDX, IDC_CMD_PROBE_SEL, m_btnProbeSel);
	DDX_Control(pDX, IDC_CMD_RADIO_PROBE, m_btnRadioProbe);
	DDX_Control(pDX, IDC_CMD_RADIO_LAST_POS, m_btnRadioLastPos);
	DDX_Control(pDX, IDC_CMD_PROTOCOL, m_BtnProtocol);
	DDX_Control(pDX, IDC_CMD_SCAN_STOP, m_BtnStopScan);
	DDX_Control(pDX, IDC_CMD_SCAN, m_BtnScan);
	DDX_Control(pDX, IDC_CMD_PRESCAN_SCAN, m_BtnPrescanScan);
	DDX_Control(pDX, IDC_CMD_PRE_SCAN, m_BtnPrescan);
	DDX_Control(pDX, IDC_PROGRESS_SCAN, m_ctlScanProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScannerControl, CDlgMriCtrlSkin)
	//{{AFX_MSG_MAP(CDlgScannerControl)
	ON_MESSAGE(WM_SCAN_PROGRESS, OnMsgScanProgress)
	ON_BN_CLICKED(IDC_CMD_PRE_SCAN, OnCmdPreScan)
	ON_BN_CLICKED(IDC_CMD_SCAN, OnCmdScan)
	ON_BN_CLICKED(IDC_CMD_PRESCAN_SCAN, OnCmdPrescanScan)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CMD_SCAN_STOP, OnCmdScanStop)
	ON_EN_CHANGE(IDC_EDT_OFFSET2, OnChangeEdtOffset2)
	ON_WM_HSCROLL()
	ON_COMMAND_RANGE(WM_PROTCOL_START, WM_PROTCOL_END, OnCmdProtocl)
	ON_COMMAND_RANGE(WM_PROBE_START, WM_PROBE_END, OmCmdProbe)
	ON_BN_CLICKED(IDC_BTN_PROBE_NEW, OnBtnProbeNew)
	ON_BN_CLICKED(IDC_BTN_PROBE_SIMULATE2, OnBtnProbeSimulate2)
	ON_BN_CLICKED(IDC_BTN_PROBE_DELETE, OnBtnProbeDelete)
	ON_BN_CLICKED(IDC_CMD_RADIO_PROBE, OnCmdRadioProbe)
	ON_BN_CLICKED(IDC_CMD_RADIO_LAST_POS, OnCmdRadioLastPos)
	ON_BN_CLICKED(IDC_CMD_RADIO_PLANE1, OnCmdRadioPlane1)
	ON_BN_CLICKED(IDC_CMD_RADIO_PLANE2, OnCmdRadioPlane2)
	ON_BN_CLICKED(IDC_CMD_RADIO_PLANE3, OnCmdRadioPlane3)
	ON_BN_CLICKED(IDC_CMD_CHK_ADVANCE, OnCmdChkAdvance)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_SUB, OnCmdSub)
	ON_MESSAGE(WM_PANA_MOVECTRL, OnMoveCtrl)
	ON_MESSAGE(WM_PANA_MOVECTRL_LBTN_UP,OnMoveCtrl_LUP)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_CMD_CHK_SWAP_OFF, OnCmdChkSwapOff)
	ON_BN_CLICKED(IDC_CMD_CHK_SWAP_ON, OnCmdChkSwapOn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScannerControl message handlers
BOOL CDlgScannerControl::OnInitDialog() 
{
	SetSelectProbeBtn(&m_btnProbeSel);
	SetNewProbeBtn(&m_btnNewProbe);
	SetSimProbeBtn(&m_btnSimulate);
	SetDelProbeBtn(&m_btnDelProbe);

	

	SetScrollBtn(&m_btnScrollOffset);
	SetAddBtn(&m_btnAddOffset);
	SetDelBtn(&m_btnSubOffset);
	SetProbeDepthEdit(&m_edtOffset);
	SetProbeDepthUnitStatic(&m_lblLengthUnit);

	SetPosition1Btn(&m_btnRadioProbe);
	SetPosition2Btn(&m_btnRadioLastPos);
	SetOrient1Btn(&m_btnPlane1);
	SetOrient2Btn(&m_btnPlane2);
	SetOrient3Btn(&m_btnPlane3);
	SetSwapOffBtn(&m_btnSwapOff);
	SetSwapOnBtn(&m_btnSwapOn);
	SetOrientStatic(&m_lblOrientation);
	SetPositionStatic(&m_lblPosition);
	SetSwapStatic(&m_lblSwap);

	m_btnRadioProbe.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnRadioLastPos.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnPlane1.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnPlane2.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnPlane3.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnSwapOn.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	m_btnSwapOff.SetButtonMode(CImageButton::ImageBtn_RadioButton);
	


	SetProtocalStatic(&m_lblProtocol);
	SetProtocalTimeStatic(&m_scanTime);

	SetProgressCtrl(&m_ctlScanProgress);

	SetLoadBtn(&m_BtnProtocol);
	SetPrescanBtn(&m_BtnPrescan);
	SetScanBtn(&m_BtnScan);
	SetPrescanScanBtn(&m_BtnPrescanScan);
	SetStopBtn(&m_BtnStopScan);

	
	SetAdvanceBtn(&m_btnChkAdv);
	SetCycleEditVisible(FALSE);

	
	CDlgMriCtrlSkin::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_btnProbeSel.SetTooltipText("Select probe", TRUE);
	m_btnNewProbe.SetTooltipText("Create a new probe", TRUE);
	m_btnSimulate.SetTooltipText("Simulate probe", TRUE);
	m_btnDelProbe.SetTooltipText("Delete current selected probe", TRUE);

	m_btnRadioProbe.SetWindowText("");
	m_btnRadioProbe.SetTooltipText("Current probe position as reference", TRUE);
	m_btnRadioLastPos.SetWindowText("");
	m_btnRadioLastPos.SetTooltipText("Last scan position as reference", TRUE);
	m_btnPlane1.SetWindowText("");
	m_btnPlane1.SetTooltipText("Plane 1", TRUE);
	m_btnPlane2.SetWindowText("");
	m_btnPlane2.SetTooltipText("Plane 2", TRUE);
	m_btnPlane3.SetWindowText("");
	m_btnPlane3.SetTooltipText("Plane 3", TRUE);
	m_btnSwapOn.SetWindowText("");
	m_btnSwapOn.SetTooltipText("Swap on", TRUE);
	m_btnSwapOff.SetWindowText("");
	m_btnSwapOff.SetTooltipText("Swap off", TRUE);

	m_BtnProtocol.SetWindowText("");
	m_BtnProtocol.SetTooltipText("Select protocol", TRUE);
	m_BtnPrescan.SetWindowText("");
	m_BtnPrescan.SetTooltipText("Prescan", TRUE);
	m_BtnScan.SetWindowText("");
	m_BtnScan.SetTooltipText("Scan", TRUE);
	m_BtnPrescanScan.SetWindowText("");
	m_BtnPrescanScan.SetTooltipText("Prescan and scan", TRUE);
	m_BtnStopScan.SetWindowText("");
	m_BtnStopScan.SetTooltipText("Stop", TRUE);
	m_btnChkAdv.SetWindowText("");
	m_btnChkAdv.SetTooltipText("Advanced option", TRUE);

	m_pMRIWrap = &(g_DlgMain)->m_MRIWrap;
	
	// Scanner control UI
	m_ePSBT = PSBT_SCAN;
	SetPrescanScanStatus(m_ePSBT);

	CString sLbl;


	
	g_pCryDlg = m_pCryDlg;

	m_ctlScanProgress.SetRange(0, SCAN_MAX_RANGE);



	// New UI 
	SetPushDepthRange(MIN_PUSH_DEPTH, MAX_PUSH_DEPTH);
	GetDlgItem(IDC_EDT_OFFSET2)->SetWindowText("0");

	OnCmdChkSwapOff();
	OnCmdRadioPlane1();
	OnCmdRadioProbe();

	SetProbeUIStatus();
	SetMRIScannerControlUI(US_NO_MRI);

	SetPushDepth(0);

	m_scanTime.SetWindowText("0 s");

	m_dlgScanOption.Create(IDD_DLG_SCANNER_CONTROL_OPTION);
	m_dlgScanOption.ShowWindow(FALSE);
	m_dlgScanOption.m_pScanOption=&m_ScanOption;
	m_dlgScanOption.m_pDlgScannerControl=this;

	GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMD_SCAN)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgScannerControl::EnableProbeControl(BOOL bEnable)
{	

	GetDlgItem(IDC_EDT_OFFSET2)->EnableWindow(bEnable /*&& pMainFrm->m_bEnableMRICtrl*/);
	GetDlgItem(IDC_LBL_UNIT)->EnableWindow(bEnable /*&& pMainFrm->m_bEnableMRICtrl*/);

	m_btnProbeSel.EnableWindow(bEnable);
	m_btnSubOffset.EnableWindow(bEnable);
	m_btnAddOffset.EnableWindow(bEnable);
	m_btnScrollOffset.EnableWindow(bEnable);

	SetPushDepthEditEnable(bEnable);

	m_btnPlane1.EnableWindow(m_ScanOption.iOrient==3 && bEnable && m_pMRIWrap->m_bEnableMRICtrl);
	m_btnPlane2.EnableWindow(m_ScanOption.iOrient==3 && bEnable && m_pMRIWrap->m_bEnableMRICtrl);
	m_btnPlane3.EnableWindow(m_ScanOption.iOrient==3 && bEnable && m_pMRIWrap->m_bEnableMRICtrl);
	m_btnRadioProbe.EnableWindow((m_ScanOption.iOrient==3 || m_ScanOption.iReferPos==0) && bEnable && m_pMRIWrap->m_bEnableMRICtrl);
	m_btnRadioLastPos.EnableWindow((m_ScanOption.iOrient==3 || m_ScanOption.iReferPos==0) && bEnable && m_pMRIWrap->m_bEnableMRICtrl && 
		m_pCurRealProbe && m_pCurRealProbe->IsLastScanOK());
	m_btnSwapOn.EnableWindow(bEnable && m_pMRIWrap->m_bEnableMRICtrl);
	m_btnSwapOff.EnableWindow(bEnable && m_pMRIWrap->m_bEnableMRICtrl);

	m_btnChkAdv.EnableWindow(bEnable && m_pMRIWrap->m_bEnableMRICtrl);

	if (m_pCurRealProbe)
	{
		m_nOffset = (int) m_pCurRealProbe->GetOffset();
		SetOffsetEdtValue(m_nOffset);
	}

	OnChangeEdtOffset2();
}


void CDlgScannerControl::OnCmdPreScan() //"Prescan"
{
	CLog::Log("Prescan按钮被按下");
	UpdateData(TRUE);

	LoadProtocol();
	Sleep(500);

	bool bProbeValid = false;
	if (m_iLocalizationMode == 0)
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);
		{
			if (m_pCryDlg->m_pDlgMain->m_bDebugNoCamera)
			{
				bProbeValid = TRUE;
			}
			else
			{
				if (!m_pCryDlg->m_bTracking)
				{
					if(!m_pCryDlg->StartTracking())
					{
						CLog::Log("在停止跟踪状态下，启动跟踪失败，未进行扫描");
						return;
					}
					
				}
				bProbeValid = CalculateAndSendMRILocalization(); 
				if (m_pCryDlg->m_bTracking && bProbeValid) /*如果计算针位置信息失败，则不必关闭跟踪*/
				{
					m_pCryDlg->StopTracking();
				}
			}
		}
	}
	else
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);

		if (m_pCryDlg->m_bTracking) //Add by Dai Liang for stop tracking before last scan 2007.7.30
		{
			m_pCryDlg->StopTracking();
		}
		bProbeValid = SendLastScanMRILocalization();
	}
	if (bProbeValid)
	{
		CLog::Log("针方位信息有效，启动预扫描");
		m_pMRIWrap->SendIMRICommand(COMMAND_STARTPRESCAN);
		
	}
	else
	{
		CLog::Log("针方位信息无效，未进行扫描");
	}
}

void CDlgScannerControl::OnCmdScan() //"Scan"
{
	CLog::Log("Scan按钮被按下");
	//Add Probe information to record the last scan position realized by WeiWei.
	UpdateData(TRUE);

	if(!m_bPreScanDone && !m_ScanOption.bCycle)
	{
		LoadProtocol();
		Sleep(500);

		OnCmdPrescanScan();
		return;
	}else if(!m_bPreScanDone && m_ScanOption.bCycle)
	{
		LoadProtocol();
		Sleep(500);
	}

	bool bProbeValid = false;
//	m_iRevStartCnt = 1;
	if (m_iLocalizationMode == 0)
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);
		if (m_pCryDlg->m_pDlgMain->m_bDebugNoCamera)
		{
			bProbeValid = TRUE;
		}
		else
		{
			if (!m_pCryDlg->m_bTracking)
			{
				if(!m_pCryDlg->StartTracking())
				{
					CLog::Log("在停止跟踪状态下，启动跟踪失败，未进行扫描");
					return;
				}
				
			}
			bProbeValid = CalculateAndSendMRILocalization();
			if (m_pCryDlg->m_bTracking && bProbeValid) /*如果计算针位置信息失败，则不必关闭跟踪*/
			{
				m_pCryDlg->StopTracking();
			}
		}
	}
	else
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);
		if (m_pCryDlg->m_bTracking)
		{
			m_pCryDlg->StopTracking();
		}
		bProbeValid = SendLastScanMRILocalization();
	}
	if (bProbeValid)
	{
		CLog::Log("针方位信息有效，启动扫描");
		
			if (m_ScanOption.bCycle)
			{
				if (m_ScanOption.iInterval <= 0 || m_ScanOption.iInterval > 250)
				{
					CLog::Log("Invalid cycle scan gap value");
					PanaMessageBox("Invalid cycle scan gap value! Scan gap is valid only between 0 and 250");
					
					return;
				}
				m_pMRIWrap->SendIMRICommand(COMMAND_STARTLOOPSCAN);
			}
			else 
			{
				m_pMRIWrap->SendIMRICommand(COMMAND_STARTSCAN);
			}
		
	}
	else
	{
		CLog::Log("针方位信息无效，未进行扫描");
	}
}


void CDlgScannerControl::OnCmdPrescanScan() //"Prescan + Scan"
{
	if (m_ePSBT == PSBT_SCAN)
	{
		StartPrescanScan();
	}
	else if (m_ePSBT == PSBT_PAUSE)
	{
		PauseScan();
	}
	else if (m_ePSBT == PSBT_RESUME)
	{
		ResumeScan();
	}
}

void CDlgScannerControl::StartPrescanScan()
{
	CLog::Log("Prescan＋scan按钮被按下");
	UpdateData(TRUE);
	bool bProbeValid = false;
//	m_iRevStartCnt = 2;
	if (m_iLocalizationMode == 0)
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);
		{
			if (m_pCryDlg->m_pDlgMain->m_bDebugNoCamera)
			{
				bProbeValid = TRUE;
			}
			else
			{
				if (!m_pCryDlg->m_bTracking)
				{
					if(!m_pCryDlg->StartTracking())
					{
						CLog::Log("在停止跟踪状态下，启动跟踪失败，未进行扫描");
						return;
					}
					
				}
				
				bProbeValid = CalculateAndSendMRILocalization();
			
				if (m_pCryDlg->m_bTracking && bProbeValid) /*如果计算针位置信息失败，则不必关闭跟踪*/
				{
					m_pCryDlg->StopTracking();
				}
			
				if (m_pCryDlg->m_bMarkerVisible)
				{
					m_pCryDlg->m_bScanningPosRecorded = TRUE;
				}
			}
		}
	}
	else
	{
		CString sLog;
		sLog.Format("定位方式(0-跟踪数据，1-上次数据):%d", m_iLocalizationMode);
		CLog::Log(sLog);
		if (m_pCryDlg->m_bTracking)
		{
			m_pCryDlg->StopTracking();
		}
		bProbeValid = SendLastScanMRILocalization();
	}
	if (bProbeValid)
	{
		CLog::Log("针方位信息有效，进行扫描");
			m_pMRIWrap->SendIMRICommand(COMMAND_STARTPRESCANSCAN);
		
	}
	else
	{
		CLog::Log("针方位信息无效，未进行扫描");
	}

}

void CDlgScannerControl::PauseScan()
{
	CLog::Log("扫描暂停按钮被按下");
	m_pMRIWrap->SendIMRICommand(COMMAND_PAUSESCAN);
	
	SetPrescanScanStatus(PSBT_RESUME);
}

void CDlgScannerControl::ResumeScan()
{
	CLog::Log("扫描继续按钮被按下");
	m_pMRIWrap->SendIMRICommand(COMMAND_RESUMESCAN);
	
	SetPrescanScanStatus(PSBT_PAUSE);
}

void CDlgScannerControl::OnCmdScanStop() 
{
	CLog::Log("停止扫描按钮被按下");
	m_pMRIWrap->SendIMRICommand(COMMAND_STOPSCAN);
	

	StopScanProgress();

	if (!m_pCryDlg->m_bTracking)
	{
		if(!m_pCryDlg->StartTracking())
		{
			CLog::Log("启动跟踪失败");
			
		}else
		{
			
			CLog::Log("启动跟踪成功");
		}
		
	}
}


void CDlgScannerControl::CreateProbeMenu()
{
	
	m_menuProbe.DestroyMenu();

	CMenuEx subMenu;
	subMenu.SetPopupMenu(TRUE);
	subMenu.CreatePopupMenu();

	m_menuProbe.SetPopupMenu(TRUE);
	m_menuProbe.CreatePopupMenu();

	CArray<CProbe*, CProbe*>* pcaProbes = m_pCryDlg->m_ProbePool.GetProbePool();
	CProbe *pProbe;
	pProbe = m_pCryDlg->m_ProbePool.GetPreimageProbe();
	subMenu.AppendMenu(MF_STRING, WM_PROBE_START, pProbe->GetName());

	if (pcaProbes->GetSize() > 0)
	{

		for (int i=0; i<pcaProbes->GetSize(); i++)
		{
			pProbe = pcaProbes->GetAt(i);
			subMenu.AppendMenu(MF_STRING, WM_PROBE_START + 1 + i, pProbe->GetName());
		}
	}
	m_menuProbe.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, "d");

	m_menuProbe.GetSubMenu(0)->AdjustMenuItem();

	m_btnProbeSel.SetBtnMenu(&m_menuProbe);
}

void CDlgScannerControl::CreateProtocolMenu(CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* pBdyPrtLst)
{
	if (pBdyPrtLst->GetCount() > 0)
	{
		// New UI, Create protocol menu
		m_menuProtocol.DestroyMenu();

		CMenuEx subMenu;
		subMenu.SetPopupMenu(TRUE);
		subMenu.CreatePopupMenu();

		m_menuProtocol.SetPopupMenu(TRUE);
		m_menuProtocol.CreatePopupMenu();

		m_menuProtocol.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, "d");
		

		POSITION pos = pBdyPrtLst->GetHeadPosition();
		int iCnt = 0;
		IMRI_PROTOCOL_GROUP *pBdyPrtProt;
		for (;pos;)
		{
			pBdyPrtProt = pBdyPrtLst->GetNext(pos);
			CMenuEx subMenu1;
			subMenu1.SetPopupMenu(TRUE);
			subMenu1.CreatePopupMenu();
			
				IMRI_PROTOCOL *pProtocol;
				for (int i=0 ; i<pBdyPrtProt->nProtocolSum; i++)
				{
					pProtocol = (IMRI_PROTOCOL*)pBdyPrtProt->pProtocols[i];
					if (pProtocol)
					{
						subMenu1.AppendMenu(MF_STRING, WM_PROTCOL_START + iCnt, (const char*)pProtocol->oProtDesc.cProtocolDesc);
						iCnt ++;
					}
				}
			

			m_menuProtocol.GetSubMenu(0)->AppendMenu(MF_POPUP, (UINT)subMenu1.m_hMenu, (const char*)pBdyPrtProt->sDesc);
		}

		m_menuProtocol.GetSubMenu(0)->AdjustMenuItem();
		m_BtnProtocol.SetBtnMenu(&m_menuProtocol);
	}
}




void CDlgScannerControl::UpdateMRIScannerControlUI2(ENUM_MRI_SYS_STATUS nSysStatus, unsigned short nSeriesStatus, int nOperResult)
{


	CString sStatus;
	sStatus.Format("Rcv Status: %d:%d %d\n", nSysStatus, nSeriesStatus, nOperResult);
	
	switch (nSysStatus)
	{
	case MSYS_IDLE:
		{
			switch (nSeriesStatus)
			{
			case 0x0000:
				break;
			case 0x0002:
				break;
			case 0x0004:
				break;
			case 0x0006:
				break;
			case 0x0008:	// 已经设置好待扫描序列的参数 后面紧接操作结果
				{
				}
				break;
			case 0x0009:	// 已经设置好待扫描序列的参数 接扫描时间预计
				{

					StopScanProgress();
					m_iEstimatedScanTime = nOperResult * 1000;
					CString sTime;
					sTime.Format("%d s",m_iEstimatedScanTime/1000);
					m_iScanNeedTime = nOperResult;
					m_scanTime.SetText(sTime);
				}
				break;
			case 0x000A:	//序列处于等待扫描列队等待 后面紧接操作结果
				{
					if (m_pMRIWrap->m_pImriSrv->CheckOperResult(nOperResult))
					{
						if (!m_bPreScanDone)
						{
							SetMRIScannerControlUI(US_READY_TO_PRESCAN);
						}
						else 
						{
							if ( m_ScanOption.bCycle && m_iCntCycleScan > 0)
							{
								SetMRIScannerControlUI(US_READY_TO_CYCLE_SCAN);
							}
							else
							{
								SetMRIScannerControlUI(US_READY_TO_SCAN);
							}
						}
					}
//					StopScanProgress();
				}
				break;
			case 0x000B:	// 序列处于等待扫描列队等待 接扫描时间预计
				{
					SetMRIScannerControlUI(US_READY_TO_PRESCAN);

					StopScanProgress();
					m_iEstimatedScanTime = nOperResult * 1000;
					CString sTime;
					sTime.Format("%d s",m_iEstimatedScanTime/1000);
					m_scanTime.SetText(sTime);
				}
				break;
			case 0x000C:
				break;
			default:
				break;
			}
		}
		break;
	case MSYS_IDLE_TO_SCAN:
		{
			if (nSeriesStatus == 0x0000 || nSeriesStatus == 0x0001)
			{
				// Start prescan
				m_iScanedTime = 0;
				m_bPreScanDone = FALSE;
				
				if(m_pMRIWrap->m_pImriSrv->GetIMRIACPHiVer()!=2 && m_pMRIWrap->m_pImriSrv->GetIMRIACPLoVer()!=3)
				{
					StartPreScanProgress();
				}

				SetMRIScannerControlUI(US_READY_TO_STOP_PRESCAN);
			}
			else if (nSeriesStatus == 0x0002 || nSeriesStatus == 0x0003)
			{
				// Start scan
				SetMRIScannerControlUI(US_READY_TO_STOP_SCAN);
			
				m_iScanedTime = 0;

				CString sTime;
				sTime.Format("%d s",m_iEstimatedScanTime/1000);
				m_scanTime.SetText(sTime);
			}
		}
		break;
	case MSYS_SCAN_TO_PAUSE:
		{
		}
		break;
	case MSYS_PAUSE_TO_SCAN:
		{
		}
		break;
	case MSYS_SCANNING:
		{
			m_bProgress=TRUE;
			switch (nSeriesStatus)
			{
			case 0x0000:	// Prescanning
				break;
			case 0x0001:	// Prescanning
				break;
			case 0x0002:	// Scanning
				break;
			case 0x0003:	// Scanning
				{
				/*	int iRcvedScanTime;
					if (nOperResult >= m_iScanedTime)
					{
						m_iScanedTime = nOperResult;
						iRcvedScanTime = nOperResult;
					}
					else
					{
						iRcvedScanTime = m_iScanedTime + nOperResult;
					}
				
					float fRate = iRcvedScanTime >= m_iEstimatedScanTime ? 0.99 : ((float)iRcvedScanTime) / m_iEstimatedScanTime;
					m_ctlScanProgress.SetPos(SCAN_MAX_RANGE * fRate);

					CString sTime;
					sTime.Format("%d s", (m_iEstimatedScanTime - iRcvedScanTime) > 0 ? (m_iEstimatedScanTime - iRcvedScanTime)/1000 : 0);*/

					CString sTime;
					sTime.Format("%d s",nOperResult);

					double fRate = SCAN_MAX_RANGE*(1-nOperResult*0.99/m_iScanNeedTime);
					m_ctlScanProgress.SetPos(fRate);
					m_scanTime.SetText(sTime);
				}
				break;
			default:
				break;
			}
		}
		break;
	case MSYS_SCAN_TO_IDLE:
		m_bProgress=FALSE;
		switch (nSeriesStatus)
		{
		case 0x0000:	// Finish prescan
			{
				m_bPreScanDone = TRUE;

				SetMRIScannerControlUI(US_READY_TO_SCAN);

				StopPreScanProgress();
			}
			break;
		case 0x0002:	// Finish scan
			{
				CLog::Log("MRI status: scanning complete");
//					GetDlgItem(IDC_RADIO_SC_LOC_LAST_SCAN)->EnableWindow(TRUE);
				m_btnRadioLastPos.EnableWindow(TRUE);
				if(!m_pCryDlg->m_bTracking)
				{
					m_pCryDlg->StartTracking();
				}
			}
			break;
		case 0x0004:	// Stop prescan normally
			{
				SetMRIScannerControlUI(US_READY_TO_LOAD);
				
				StopPreScanProgress();

				m_bPreScanDone=FALSE;

				
			}
			break;
		case 0x0006:	// Stop scan normally
			{
				SetMRIScannerControlUI(US_READY_TO_LOAD);

				StopScanProgress();

				m_bPreScanDone=FALSE;

				
			}
			break;
		case 0x0008:	// Stop prescan abnormally
			{
				CLog::Log("MRI status: stop abnormally");
				PanaMessageBox("MRI scanner stop abnormally. Please check it.");

				SetMRIScannerControlUI(US_READY_TO_LOAD);

				StopPreScanProgress();

				m_bPreScanDone=FALSE;

				
			}
			break;
		case 0x000A:	// Stop scan abnormally
			{
				CLog::Log("MRI status: stop abnormally");
				PanaMessageBox("MRI scanner stop abnormally. Please check it.");

				SetMRIScannerControlUI(US_READY_TO_LOAD);
				
				StopScanProgress();

				m_bPreScanDone=FALSE;

				
			}
			break;
		case 0x000C:
			{
				CLog::Log("MRI status: start recon");

				SetMRIScannerControlUI(US_NO_MRI);
			}
			break;
		case 0x000E:	// Finish recon
			{
				CLog::Log("MRI status: recon complete");
				if ( m_ScanOption.bCycle)
				{
					if (m_ePSBT == PSBT_PAUSE)
					{
						SetMRIScannerControlUI(US_READY_TO_PAUSE);
					}
					else
					{
						SetMRIScannerControlUI(US_READY_TO_RESUME);
					}
				}
				else
				{
					SetMRIScannerControlUI(US_READY_TO_LOAD);
				}

				m_bPreScanDone=FALSE;
				
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CDlgScannerControl::SetMRIScannerControlUI(enum UI_STATUS eStatus)
{
	switch (eStatus)
	{
	case US_NO_MRI:
		{
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);

			GetDlgItem(IDC_CMD_CHK_ADVANCE)->EnableWindow(FALSE);
		}
		break;
	case US_MRI_CONN_LOST:
		{
			m_lblProtocol.SetText("");
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);

			GetDlgItem(IDC_CMD_CHK_ADVANCE)->EnableWindow(FALSE);
		}
		break;
	case US_READY_TO_CHOOSE_PROTOCOL:
		{
			m_BtnProtocol.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);

			GetDlgItem(IDC_CMD_CHK_ADVANCE)->EnableWindow(TRUE);
		}
		break;
	case US_READY_TO_LOAD:
		{
			m_BtnProtocol.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);
			
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);
			
			GetDlgItem(IDC_CMD_CHK_ADVANCE)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(TRUE);
			SetPrescanScanStatus(PSBT_SCAN);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(FALSE);
		}
		break;
	case US_READY_TO_PRESCAN:
		{
			m_BtnProtocol.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);
			
			if (m_ScanOption.bCycle && m_iCntCycleScan > 0)
			{
				SetPrescanScanStatus(PSBT_PAUSE);
				GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(TRUE);
				GetDlgItem(IDC_CMD_SCAN)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_CMD_SCAN)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(SW_HIDE);
			}
		}
		break;
	case US_READY_TO_SCAN:
		{
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(FALSE);
			m_BtnProtocol.EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);

			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(TRUE);
			SetPrescanScanStatus(PSBT_SCAN);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(FALSE);

		}
		break;
	case US_READY_TO_CYCLE_SCAN:
		{
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(TRUE);
			m_BtnProtocol.EnableWindow(FALSE);
			
			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(TRUE);
		}
		break;
	case US_READY_TO_PAUSE:
		{
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(TRUE);
		}
		break;
	case US_READY_TO_RESUME:
		{
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(TRUE);
		}
		break;
	case US_READY_TO_STOP_PRESCAN:
		{
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(TRUE);

			GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(FALSE);

			GetDlgItem(IDC_CMD_SCAN)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(TRUE);
			SetPrescanScanStatus(PSBT_SCAN);
			GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(FALSE);
		}
		break;
	case US_READY_TO_STOP_SCAN:
		{
			m_BtnProtocol.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SCAN_STOP)->EnableWindow(TRUE);

			if (m_ScanOption.bCycle)
			{
				GetDlgItem(IDC_CMD_PRESCAN_SCAN)->EnableWindow(TRUE);
				SetPrescanScanStatus(PSBT_PAUSE);
				
				GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(TRUE);
				GetDlgItem(IDC_CMD_SCAN)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_CMD_SCAN)->EnableWindow(FALSE);
				GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
				
				GetDlgItem(IDC_CMD_SCAN)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CMD_PRE_SCAN)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CMD_PRESCAN_SCAN)->ShowWindow(SW_HIDE);
			}
		}
		break;
	default:
		break;
	}
	
	//nsj 11.07.08
	//新版通讯协议下prescan按钮已失效
	if(m_pCryDlg && m_pCryDlg->m_pDlgMain->m_MRIWrap.m_pImriSrv && 
		m_pCryDlg->m_pDlgMain->m_MRIWrap.m_pImriSrv->GetIMRIACPHiVer()==2 && 
	   m_pCryDlg && m_pCryDlg->m_pDlgMain->m_MRIWrap.m_pImriSrv->GetIMRIACPHiVer()==3)
	{
		GetDlgItem(IDC_CMD_PRE_SCAN)->EnableWindow(FALSE);
	}
	
}

bool CDlgScannerControl::CalculateAndSendMRILocalization()
{	
	CConfigMgr conf;
	BOOL bDebugNoProbe = !(conf.Read(SYSINFO, SYS_HAS_PROBE, 1));;
	if (m_pCryDlg->m_pDlgMain->m_bDebugNoCamera || bDebugNoProbe)
	{
		return true;
	}

	UpdateData(TRUE);

	Matrix mTranNeedle2Scanner;
	Vector3D vTip2Scanner;
	Vector3D vOri2Scanner;
	Matrix mTranTracker2Scanner;
	mTranTracker2Scanner.SetDimension(4,4);

	mTranTracker2Scanner = m_pCryDlg->m_tranScanner2Tracker.Inverse();

	mTranNeedle2Scanner.SetDimension(4,4);
	if (!m_pCurRealProbe)
	{
		CLog::Log("错误：未创建针");
		PanaMessageBox("Please new a needle first!");
		return false;
	}
	if (!(m_pCurRealProbe->IsVisible() && m_pCryDlg->m_bProbeVisible))
	{
		if(!(m_ScanOption.iReferPos==1 && m_ScanOption.iOrient!=3))
		{
			CLog::Log("错误：无法跟踪针");
			PanaMessageBox("Please check needle status!");
			return false;
		}
	}
	mTranNeedle2Scanner = mTranTracker2Scanner * m_pCurRealProbe->m_mProbeMarker2Tracker;
	vTip2Scanner = m_pCurRealProbe->GetVirtualTipInTrack();
	vTip2Scanner.Transformation(mTranTracker2Scanner.value);
	vOri2Scanner = m_pCurRealProbe->GetOrientationInTrack();
	vOri2Scanner.Rotate(mTranTracker2Scanner.value);

	if(m_pCryDlg->m_patientMarker == PATIENT_WITH_MARKER ) //navi mode
	{
		/*导航模式==TABLE_WITH_MARKER时
		   GetTipInTrack()和GetOrientationInTrack()实际得到是针在病床坐标系下的
		   坐标
		*/
		Matrix mPT2Scanner = m_pCryDlg->m_tranScanner2Marker.Inverse();
		vTip2Scanner = m_pCurRealProbe->GetVirtualTipInTrack();
		vTip2Scanner.Transformation(mPT2Scanner.value);
		vOri2Scanner = m_pCurRealProbe->GetOrientationInTrack();
		vOri2Scanner.Rotate(mPT2Scanner.value);
	}
	
	Vector3D vCenter, vPhaseDir, vFreqDir;
	GetScanControlPar(vTip2Scanner, vOri2Scanner, mTranNeedle2Scanner, vCenter, vPhaseDir, vFreqDir);

	
	if ( IsLocParValid(vCenter)  || 
		(m_ScanOption.iReferPos==1 && m_ScanOption.iOrient!=3) )
	{
		
		m_pMRIWrap->SendMRIScannerLocalization(vCenter, vPhaseDir, vFreqDir);
		m_pCurRealProbe->SaveLastScanPar(vTip2Scanner, vOri2Scanner, mTranNeedle2Scanner, m_pCurRealProbe->GetOffset());
		SetProbeUIStatus();
		CCrashRstore::SaveCrashState(CI_PROBES);
		return true;
	}
	else
	{
		CLog::Log("计算MRI扫描参数失败，原因：方位参数距离扫描中心大于300mm");
		PanaMessageBox( "Probe is too far from MRI center!" );
		return false;
	}
}

bool CDlgScannerControl::SendLastScanMRILocalization()
{
	Matrix mTranNeedle2Scanner;
	Vector3D vTip2Scanner;
	Vector3D vOri2Scanner;
	

	m_pCurRealProbe->GetLastScanPar(vTip2Scanner, vOri2Scanner, mTranNeedle2Scanner);

	Vector3D vCenter, vPhaseDir, vFreqDir;
	GetScanControlPar(vTip2Scanner, vOri2Scanner, mTranNeedle2Scanner, vCenter, vPhaseDir, vFreqDir);

	if ( IsLocParValid(vTip2Scanner)  || 
		(m_ScanOption.iReferPos==1 && m_ScanOption.iOrient!=3) )
	{
		m_pMRIWrap->SendMRIScannerLocalization(vCenter, vPhaseDir, vFreqDir);
		return true;
	}
	else
	{
		CLog::Log("计算MRI扫描参数失败，原因：方位参数距离扫描中心大于300mm");
		PanaMessageBox( "Probe is too far from MRI center!" );
		return false;
	}
}

void CDlgScannerControl::StartPreScanProgress()
{
	m_scanTime.SetText("");
	m_bProgress = TRUE;
	AfxBeginThread( PreScanProgress,
				  this,
				  THREAD_PRIORITY_NORMAL,
				  0,
				  0 );
}

void CDlgScannerControl::StopScanProgress()
{
	m_bProgress = FALSE;
	m_ctlScanProgress.SetPos(0);
	m_scanTime.SetText("");
	
	//UpdateData(FALSE);
}

void CDlgScannerControl::StopPreScanProgress()
{
	m_bProgress = FALSE;
	m_ctlScanProgress.SetPos(0);
	CString sTime;
//	sTime.Format("%d s",m_iEstimatedScanTime/1000);
	sTime = "";
	m_scanTime.SetText(sTime);
}

UINT PreScanProgress( LPVOID pParam)
{
	//HWND hWnd = (HWND)pParam;
	CDlgScannerControl* pDlg = (CDlgScannerControl*)pParam;


	// Prescan progress
	int iGapCnt = 10;
	int iGap = SCAN_MAX_RANGE/iGapCnt;
	
	for (int i=0; i<=iGapCnt; i++)
	{
		if (!pDlg->m_bProgress)
			break;

		if (i == iGapCnt)
		{
			pDlg->m_ctlScanProgress.SetPos(i * iGap * 0.99);
			i = -1;
			
		}
		else
		{
			pDlg->m_ctlScanProgress.SetPos(i * iGap);
			
		}
		Sleep(500);
	}
	pDlg->m_ctlScanProgress.SetPos(0);

	AfxEndThread( 0, TRUE );
	return 0;
} 

LONG CDlgScannerControl::OnMsgScanProgress( UINT wParam, LONG lParam)
{
	int iTime = (int)wParam;
	int iLoopTime = (int)(m_iEstimatedScanTime);
	m_ctlScanProgress.SetPos(SCAN_MAX_RANGE * ((float)iTime / iLoopTime));

	CString sTime;
	sTime.Format("%d s", (m_iEstimatedScanTime - iTime)/1000);
	m_scanTime.SetText(sTime);

	return 1;
}


void CDlgScannerControl::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
//	CRect rect;
//	GetClientRect(&rect);	
}


void CDlgScannerControl::GetScanControlPar( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix, 
										   Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir )
{
	switch( m_iLocalizationPlane ) {
	case 0:
		{
			if ( m_bChkSwap )
			{
				GetMarkerPlane( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vPhaseDir, vFreqDir );
			}
			else
			{
				GetMarkerPlane( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vFreqDir, vPhaseDir );
			}
		}
		break;
	case 1:
		{
			if ( m_bChkSwap )
			{
				GetPlanePerpendicularToMarker( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vPhaseDir, vFreqDir );
			}
			else
			{
				GetPlanePerpendicularToMarker( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vFreqDir, vPhaseDir );
			}
		}
		break;
	case 2:
		{
			if ( m_bChkSwap )
			{
				GetPlanePerpendicularToProbe( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vPhaseDir, vFreqDir );
			}
			else
			{
				GetPlanePerpendicularToProbe( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vFreqDir, vPhaseDir );
			}
		}
		break;
	default:
		GetMarkerPlane( vProbeCenter, vProbeOri, mProbeMatrix, vCenter, vFreqDir, vPhaseDir );
		break;
	}

	/*ningsj 2009.3.31
	  计算最新的Offset
	  
	*/
	if (m_iLocalizationMode == 1)
	{
		int nOffset = m_pCurRealProbe->GetOffset() - m_pCurRealProbe->GetLastOffset();
		vCenter = vCenter - nOffset * vProbeOri;
	}
}

void CDlgScannerControl::GetMarkerPlane( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix, 
										   Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir )
{
	vCenter = vProbeCenter;
	vFreqDir = vProbeOri;
	Vector3D vTmp( mProbeMatrix.value[0][2], mProbeMatrix.value[1][2], mProbeMatrix.value[2][2] );
	vPhaseDir = vTmp ^ vFreqDir;
	vPhaseDir.SetUnit();
}

void CDlgScannerControl::GetPlanePerpendicularToMarker( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix, 
										   Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir )
{
	vCenter = vProbeCenter;
	vFreqDir = vProbeOri;
	Vector3D vTmp( mProbeMatrix.value[0][2], mProbeMatrix.value[1][2], mProbeMatrix.value[2][2] );
	vTmp = vTmp ^ vFreqDir;
	vPhaseDir = vTmp ^ vFreqDir;
	vPhaseDir.SetUnit();
}

void CDlgScannerControl::GetPlanePerpendicularToProbe( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix, 
										   Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir )
{
	vCenter = vProbeCenter;
	Vector3D vTmp( mProbeMatrix.value[0][2], mProbeMatrix.value[1][2], mProbeMatrix.value[2][2] );
	vPhaseDir = vTmp ^ vProbeOri;
	vPhaseDir.SetUnit();
	vFreqDir = vPhaseDir ^ vProbeOri;
	vFreqDir.SetUnit();
}

bool CDlgScannerControl::IsLocParValid( Vector3D vCenter )
{
	bool bValid = false;
	
	if ( fabs(vCenter.x) < 300.00
		&& fabs(vCenter.y) < 300.00
		&& fabs(vCenter.z) < 300.00 ) // 400.00 is to be added into CONFIG
	{
		bValid = true;
	}

	return bValid;
}

BOOL CDlgScannerControl::PreTranslateMessage(MSG* pMsg) 
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

void CDlgScannerControl::OnChangeEdtOffset2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
	
	CString sVal;
	GetDlgItemText(IDC_EDT_OFFSET2, sVal);
	int iOffset = atoi(sVal.GetBuffer(1));

	if (iOffset<MIN_PUSH_DEPTH || iOffset>MAX_PUSH_DEPTH)
	{
		PanaMessageBox("Invalid value. Push depth should be between -200 and 200");
		if(iOffset<MIN_PUSH_DEPTH)
		{
			m_nOffset = MIN_PUSH_DEPTH;
		}
		if (iOffset>MAX_PUSH_DEPTH)	
		{
			m_nOffset = MAX_PUSH_DEPTH;
		}
		SetOffsetEdtValue(m_nOffset);
//		return;
	}
	else
	{
		m_nOffset = iOffset;
	}
	

	if (m_pCurRealProbe)
	{
		m_pCurRealProbe->SetOffset(m_nOffset);
		if(!m_bNoRefreshInScroll)
		{
			m_pCryDlg->m_pDisplay4Dlg->Refresh();
		}
	}

	SetPushDepth(m_nOffset);
	m_edtOffset.SetFocus();
}

void CDlgScannerControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	
	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;
		
	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINELEFT:      // Scroll left.
		if (curpos > minpos)
			curpos--;
		break;
		
	case SB_LINERIGHT:   // Scroll right.
		if (curpos < maxpos)
			curpos++;
		break;
		
	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	
	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(curpos);

	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar); 
}

void CDlgScannerControl::OnCmdProtocl(UINT nID)
{
	CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* pBdyPrtLst
		= m_pCryDlg->m_ProtMng.GetBdyPrtLst();

	POSITION pos = pBdyPrtLst->GetHeadPosition();
	IMRI_PROTOCOL_GROUP *pBdyPrtProt = NULL;
	int iIndex = nID - WM_PROTCOL_START;
	int iSum = 0;
	void *pProtocol = NULL;
	for (;pos;)
	{
		pBdyPrtProt = pBdyPrtLst->GetNext(pos);
		if (iIndex - iSum < pBdyPrtProt->nProtocolSum)
		{
			pProtocol = pBdyPrtProt->pProtocols[iIndex - iSum];
			break;
		}
		iSum += pBdyPrtProt->nProtocolSum;
	}

	UpdateSelectedProtocol(pProtocol, pBdyPrtProt);
}

void CDlgScannerControl::UpdateSelectedProtocol(void *pProtocol, IMRI_PROTOCOL_GROUP *pBdyPrtProt)
{
	m_pCurMRIProtocol = pProtocol;

	if(m_pCurMRIProtocol)
	{
		m_ScanOption.fThickness=((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.fThickness;
		m_ScanOption.fGap=((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.fGap;
		m_ScanOption.iSlice=((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.iSlice;
		m_ScanOption.iNex=((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.iNex;
		if(m_ScanOption.iOrient!=3)
		{
			if(((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.iPlane==0)
			{
				m_ScanOption.iOrient=2;
			}else if(((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.iPlane==1)
			{
				m_ScanOption.iOrient=1;
			}else
			{
				m_ScanOption.iOrient=0;
			}
		}
	}

	CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* pBdyPrtLst
		= m_pCryDlg->m_ProtMng.GetBdyPrtLst();

	if (m_pCurMRIProtocol && pBdyPrtProt)
	{
		SetMRIScannerControlUI(US_READY_TO_LOAD);
	
			m_lblProtocol.SetText((const char *)((IMRI_PROTOCOL*)m_pCurMRIProtocol)->oProtDesc.cProtocolDesc);

		
		UpdateData(FALSE);
		
	}
	else
	{
		SetMRIScannerControlUI(US_MRI_CONN_LOST);		
	}
}

void CDlgScannerControl::OnBtnProbeNew() 
{
	// TODO: Add your control notification handler code here
	CLog::Log("新建针按钮被按下");
	if( m_pCryDlg->m_pDlgMain->m_enumOperationType==OPERATION_TYPE_NONE )
	{
		CLog::Log("创建针失败，原因：当前手术类型状态为None");
		PanaMessageBox("Please select the operation type");
		return;
	}
	
	double dTip[3],dOri[3];
	double dLength, dDiameter;
	CString sProbeName;
	
	if( m_pCryDlg->m_pDlgMain->m_enumOperationType == OPERATION_TYPE_CRYOTHERAPY )
	{
		CLog::Log("当前手术类型为冷冻消融");
		CCryotherapyProbeSetting dlgCryProbeSet;
		if(dlgCryProbeSet.DoModal()!=IDOK) 
		{
			CLog::Log("用户取消创建针");
			return;
		}
		dLength = dlgCryProbeSet.m_nLength;
		sProbeName = dlgCryProbeSet.m_sProbeName;
		dDiameter = m_pCryDlg->m_ProbeParPool.GetProbeParCryByName(sProbeName)->dbRadius*2;
	}
	else if( m_pCryDlg->m_pDlgMain->m_enumOperationType == OPERATION_TYPE_BIOPSY )
	{
		CLog::Log("当前手术类型为穿刺");
		CBiopsyProbeSetting dlgBioProbeSet;
		if(dlgBioProbeSet.DoModal()!=IDOK)
		{
			CLog::Log("用户取消创建针");
			return;
		}
		dLength = dlgBioProbeSet.m_nLength;
		sProbeName = dlgBioProbeSet.m_sProbeName;
		dDiameter = m_pCryDlg->m_ProbeParPool.GetProbeParBioByName(sProbeName)->dbRadius*2;
	}
	
//	CConfigMgr con;
	CString sFilePath = CONFIG_FILES[CF_PROBE_CALI];

	double dSampleTip[3],dSampleOri[3];
	double dSampleLength,dSampleDiameter;

	if(CGlobalFuncs::LoadProbeFile(sFilePath,dSampleOri,dSampleTip,dSampleLength,dSampleDiameter))
	{
		CString sLog;
		sLog.Format("装载Sample Probe成功，针文件路径：%s",sFilePath);
		CLog::Log(sLog);
	}
	else
	{
		CString sLog;
		sLog.Format("装载Sample Probe失败，针文件路径：%s",sFilePath);
		CLog::Log(sLog);
		return;
	}

	CConfigMgr con;
	double dHolderOri[3];
	double dHolderAngle = con.Read(CONF_PROBEHOLDER,CONF_PROBEHOLDER_GROOVEANGLE,120);
	dHolderOri[0] = con.Read(CONF_PROBEHOLDER,CONF_PROBEHOLDER_ORI_X,0);
	dHolderOri[1] = con.Read(CONF_PROBEHOLDER,CONF_PROBEHOLDER_ORI_Y,0);
	dHolderOri[2] = con.Read(CONF_PROBEHOLDER,CONF_PROBEHOLDER_ORI_Z,1);
	dHolderAngle = dHolderAngle/180*PI;
	CMriCalibration MriCalibration;
	MriCalibration.CalToolTipOrientation(dHolderAngle,dHolderOri,dSampleOri,dSampleTip,dSampleLength,
						dSampleDiameter,dLength,dDiameter,dTip,dOri);

	CProbe *pProbe = new CProbe();
	pProbe->SetProbePar(m_pCryDlg->m_pDlgMain->m_enumOperationType, sProbeName);
	pProbe->SetTipInProbe(dTip[0], dTip[1], dTip[2]);
	pProbe->SetOrientationInProbe(dOri[0], dOri[1], dOri[2]);
	pProbe->SetLength(dLength);
	pProbe->UpdatePosOri();

	pProbe->SetIndex(m_pCryDlg->m_ProbePool.GetNextNewProbeIndex());
	m_pCryDlg->m_ProbePool.SetNextNewProbeIndex(m_pCryDlg->m_ProbePool.GetNextNewProbeIndex()+1);
	
	m_pCryDlg->m_ProbePool.AddNewProbe(pProbe);

//	m_cboProbes.AddString(pProbe->GetName());
//	m_cboProbes.SetCurSel(m_cboProbes.GetCount()-1);
//	OnSelchangeCboProbe();

	CreateProbeMenu();
	SelecteProbe(pProbe);
	CCrashRstore::SaveCrashState(CI_PROBES);
}

void CDlgScannerControl::OnBtnProbeSimulate2() 
{
	// TODO: Add your control notification handler code here
	if (m_pCurRealProbe)
	{
		m_pCurRealProbe->EnableSimulate(!m_pCurRealProbe->IsSimulating());
		
		m_pCryDlg->m_pDisplay4Dlg->Refresh();

		if (m_pCurRealProbe->IsSimulating())
		{
			SetDlgItemText(IDC_BTN_PROBE_SIMULATE2,"Stop Sim");
		}
		else
		{
			SetDlgItemText(IDC_BTN_PROBE_SIMULATE2,"Sim");
		}
	}	
}

void CDlgScannerControl::OnBtnProbeDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pCurRealProbe)
	{
		if(PanaMessageBox("Are you sure to delete the probe " + m_pCurRealProbe->GetName() + "?","Delete probe",MB_OKCANCEL) == IDOK)
		{
			DeleteCurrentProbe();
		}
		else
		{
			CLog::Log("User cancel probe deletion.");
			return;
		}
	}
}

void CDlgScannerControl::SelecteProbe(CProbe *pProbe)
{
	if (pProbe == NULL)
		return;
	
	SetDlgItemText(IDC_CMD_PROBE_SEL, pProbe->GetName());

	if (m_pCurRealProbe)
	{
		m_pCurRealProbe->SetVisible(FALSE);
	}
	m_pCurRealProbe = pProbe;
	if (m_pCryDlg->m_ppTrackTool)
	{
		if(m_pCryDlg->m_pTrackerHandler->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
		{
			m_pCryDlg->m_ppTrackTool[TH_NDI_OPTIC_PROBE]->AttachTo(m_pCurRealProbe);
		}else
		{
			m_pCryDlg->m_ppTrackTool[TH_NDI_EM_PROBE]->AttachTo(m_pCurRealProbe);
		}
	}
	m_pCurRealProbe->SetVisible(TRUE);
	m_pCryDlg->m_pDisplay4Dlg->Refresh();

	SetProbeUIStatus();
	if (!m_pCryDlg->m_bTracking)
	{
		m_pCryDlg->StartTracking();
	}

	//将针添加到Tracking窗口中
	m_pCryDlg->m_pDlgMain->m_dlgFusion.SetProbe(m_pCurRealProbe);
	m_pCryDlg->m_pDlgMain->m_dlgTracking.SetProbe(m_pCurRealProbe);
}

void CDlgScannerControl::OmCmdProbe(UINT nID)
{
	int iIndex = nID - WM_PROBE_START;
	CProbe *pProbe = NULL;
	if (iIndex == 0)
	{
		pProbe = m_pCryDlg->m_ProbePool.GetPreimageProbe();
	}
	else if (iIndex > 0)
	{
		pProbe = m_pCryDlg->m_ProbePool.GetProbeByArrIndex(iIndex - 1);
	}
	SelecteProbe(pProbe);
	CCrashRstore::SaveCrashState(CI_PROBES);
}


void CDlgScannerControl::InitPreimageProbe()
{
	m_pCryDlg->m_ProbePool.InitPreimageProbe();

	m_pCurRealProbe = m_pCryDlg->m_ProbePool.GetPreimageProbe();
	m_pCryDlg->m_dlgNavigate.m_pCurRealProbe=m_pCurRealProbe;

	CreateProbeMenu();
	SelecteProbe(m_pCurRealProbe);

}

void CDlgScannerControl::DeleteCurrentProbe()
{
	if (m_pCurRealProbe)
	{
		CString sDelProbeName = m_pCurRealProbe->GetName();
		if (m_pCryDlg->m_ppTrackTool)
		{
			if(m_pCryDlg->m_pTrackerHandler->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
			{
				m_pCryDlg->m_ppTrackTool[TH_NDI_OPTIC_PROBE]->Detach();
			}else
			{
				m_pCryDlg->m_ppTrackTool[TH_NDI_EM_PROBE]->Detach();
			}
		}
		m_pCryDlg->m_ProbePool.DeleteProbe(m_pCurRealProbe);
		m_pCurRealProbe = NULL;
//		m_cboProbes.DeleteString(m_cboProbes.FindString(0, sDelProbeName));
//		m_cboProbes.SetCurSel(m_cboProbes.GetCount()-1);
//		OnSelchangeCboProbe();
		CreateProbeMenu();
		SelecteProbe(m_pCryDlg->m_ProbePool.GetLastProbe());
		CCrashRstore::SaveCrashState(CI_PROBES);
	}
}

int CDlgScannerControl::SetProbeUIStatus()
{
	BOOL bEnable = gStarOperType > OPERATION_TYPE_NONE && gStarOperType < OPERATION_TYPE_TPS;

	EnableProbeControl(bEnable);
	
//	GetDlgItem(IDC_CBO_PROBE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PROBE_NEW)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PROBE_DELETE)->EnableWindow(bEnable && m_pCurRealProbe && m_pCurRealProbe->GetName() != PREPROBE_NAME);
	GetDlgItem(IDC_BTN_PROBE_SIMULATE2)->EnableWindow(bEnable && m_pCurRealProbe && m_pCurRealProbe->m_iceBall40 != NULL);

	if (m_pCurRealProbe && m_pCurRealProbe->IsSimulating())
	{
		SetDlgItemText(IDC_BTN_PROBE_SIMULATE2,"Stop Sim");
	}
	else
	{
		SetDlgItemText(IDC_BTN_PROBE_SIMULATE2,"Sim");
	}

//	if ( m_bPreImaging == TRUE )
//	{			
		if ( m_pCurRealProbe && m_pCurRealProbe->IsLastScanOK() )
		{
//			pDlgScannerControl->GetDlgItem(IDC_RADIO_SC_LOC_LAST_SCAN)->EnableWindow(TRUE);
			m_btnRadioLastPos.EnableWindow(TRUE);
		}
		else
		{
			OnCmdRadioProbe();
			m_btnRadioLastPos.EnableWindow(FALSE);
//			pDlgScannerControl->GetDlgItem(IDC_RADIO_SC_LOC_LAST_SCAN)->EnableWindow(FALSE);
			
		}

//	}
	return 1;
}


void CDlgScannerControl::OnCmdRadioProbe() 
{
	// TODO: Add your control notification handler code here
	m_iLocalizationMode = 0;
	m_btnRadioProbe.SetChecked(TRUE);
	m_btnRadioLastPos.SetChecked(FALSE);
}

void CDlgScannerControl::OnCmdRadioLastPos() 
{
	// TODO: Add your control notification handler code here
	m_iLocalizationMode = 1;
	m_btnRadioProbe.SetChecked(FALSE);
	m_btnRadioLastPos.SetChecked(TRUE);
}

void CDlgScannerControl::OnCmdRadioPlane1() 
{
	// TODO: Add your control notification handler code here
	m_iLocalizationPlane = 0;
	m_btnPlane1.SetChecked(TRUE);
	m_btnPlane2.SetChecked(FALSE);
	m_btnPlane3.SetChecked(FALSE);
}

void CDlgScannerControl::OnCmdRadioPlane2() 
{
	// TODO: Add your control notification handler code here
	m_iLocalizationPlane = 1;
	m_btnPlane1.SetChecked(FALSE);
	m_btnPlane2.SetChecked(TRUE);
	m_btnPlane3.SetChecked(FALSE);
}

void CDlgScannerControl::OnCmdRadioPlane3() 
{
	// TODO: Add your control notification handler code here
	m_iLocalizationPlane = 2;
	m_btnPlane1.SetChecked(FALSE);
	m_btnPlane2.SetChecked(FALSE);
	m_btnPlane3.SetChecked(TRUE);
}

void CDlgScannerControl::OnCmdChkSwapOff() 
{
	// TODO: Add your control notification handler code here
	m_bChkSwap = FALSE;
	m_btnSwapOn.SetChecked(FALSE);
	m_btnSwapOff.SetChecked(TRUE);
	m_ScanOption.bSwap=m_bChkSwap;
}

void CDlgScannerControl::OnCmdChkSwapOn() 
{
	// TODO: Add your control notification handler code here
	m_bChkSwap = TRUE;
	m_btnSwapOff.SetChecked(FALSE);
	m_btnSwapOn.SetChecked(TRUE);
	m_ScanOption.bSwap=m_bChkSwap;
}

void CDlgScannerControl::OnCmdChkAdvance() 
{
	// TODO: Add your control notification handler code here
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
		
	m_dlgScanOption.MoveWindow(776+x-1024,418,203,200);
	m_dlgScanOption.Refresh(0);
	m_dlgScanOption.ShowWindow(TRUE);
}


void CDlgScannerControl::SetPrescanScanStatus(enum UI_PRESCANSCAN_BTN_STATUS ePSBT)
{
	m_ePSBT = ePSBT;
	switch (m_ePSBT)
	{
	case PSBT_PAUSE:
		{
			m_bInPause = TRUE;
			SetPrescanScanBtnStatus(PASK_PSBT_PAUSE);
		}
		break;
	case PSBT_RESUME:
		SetPrescanScanBtnStatus(PASK_PSBT_RESUME);
		break;
	case PSBT_SCAN:
		SetPrescanScanBtnStatus(PASK_PSBT_SCAN);
		break;
	default:
		SetPrescanScanBtnStatus(PASK_PSBT_SCAN);
		break;
	}
}



void CDlgScannerControl::OnCmdAdd() 
{
	// TODO: Add your control notification handler code here
	double dbPos = GetPushDepth();
	int iPos = (int)GetPushDepth();
	if (iPos + 1 <= MAX_PUSH_DEPTH)
	{
		SetPushDepth(iPos + 1);
	}
}

void CDlgScannerControl::OnCmdSub() 
{
	// TODO: Add your control notification handler code here
	int iPos = (int)GetPushDepth();
	if (iPos - 1 >= MIN_PUSH_DEPTH)
	{
		SetPushDepth(iPos - 1);
	}
}

LONG CDlgScannerControl::OnMoveCtrl_LUP(WPARAM w,LPARAM l)
{
	if(m_pCryDlg && m_pCurRealProbe)
	{
		m_pCurRealProbe->SetOffset(m_nOffset);
		m_pCryDlg->m_pDisplay4Dlg->Refresh();
	}

	return 0;
}

LONG CDlgScannerControl::OnMoveCtrl(WPARAM w,LPARAM l)
{
	CDlgMriCtrlSkin::OnScrollPushDepth(w,l);

	int iPushDepth = GetPushDepth();
	m_bNoRefreshInScroll=TRUE;
	SetOffsetEdtValue(iPushDepth);
	m_bNoRefreshInScroll=FALSE;

	return 0;
}

void CDlgScannerControl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_menuProbe.MeasureItem(lpMeasureItemStruct);
	m_menuProtocol.MeasureItem(lpMeasureItemStruct);
	
	CDlgMriCtrlSkin::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CDlgScannerControl::SetOffsetEdtValue(int nOffset)
{
	CString str;
	m_edtOffset.GetWindowText(str);
	if( atoi(str) != nOffset)
	{
		str.Format("%i",nOffset);
		m_edtOffset.SetWindowText(str);
	}
}

void CDlgScannerControl::LoadProtocol()
{
	if (m_pCurMRIProtocol == NULL)
		return;

	UpdateData(TRUE);
	CLog::Log("Load protocol");

	m_pMRIWrap->SendIMRICommand(COMMAND_CREATESERIES, (IMRI_PROTOCOL*)m_pCurMRIProtocol);
	
	m_iCntCycleScan = 0;
	m_bPreScanDone = FALSE;
}
