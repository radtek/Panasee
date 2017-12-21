// DlgCaliModeSel.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgCaliModeSel.h"
#include "ConfigMgr.h"
#include "Log.h"
#include "CryotherapyDlg.h"
#include "DlgManualBaseCali.h"
#include "DlgBaseCalibration.h"
#include "Config.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliModeSel dialog


CDlgCaliModeSel::CDlgCaliModeSel(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgCaliModeSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliModeSel)
	m_iCalMode = -1;
	//}}AFX_DATA_INIT
}


void CDlgCaliModeSel::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliModeSel)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Radio(pDX, IDC_RADIO_CALI_NOW, m_iCalMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliModeSel, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgCaliModeSel)
	ON_BN_CLICKED(IDC_RADIO_CALI_LAST, OnRadioCaliLast)
	ON_BN_CLICKED(IDC_RADIO_CALI_MANUAL, OnRadioCaliManual)
	ON_BN_CLICKED(IDC_RADIO_CALI_NOW, OnRadioCaliNow)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliModeSel message handlers

BOOL CDlgCaliModeSel::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);	
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);		

	// TODO: Add extra initialization here
	m_CalibrationType=CALIBRATION_NOW;

	//m_btnOK.SetThemeHelper(&m_ThemeHelper);
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);

		CDlgMain *pDlgMain = g_DlgMain;
		
		BOOL bEnableMRICtrl = pDlgMain->m_bEnableMRICtrl;
		BOOL bDebugNoCamera = pDlgMain->m_bDebugNoCamera;
		if (!bEnableMRICtrl || !pDlgMain->m_MRIWrap.m_bMRIConneted
			|| bDebugNoCamera || !pDlgMain->m_trackerManager.GetHandler()->IsValidDevice()
			|| !CGlobalFuncs::IsFileExist(CONFIG_FILES[CF_PHANTOM]))
		{
			m_CalibrationType = CALIBRATION_MANUAL;
		}

	m_iCalMode = m_CalibrationType;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCaliModeSel::OnRadioCaliLast() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_CalibrationType = CALIBRATION_USE_LAST;	
}

void CDlgCaliModeSel::OnRadioCaliManual() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_CalibrationType = CALIBRATION_MANUAL;		
}

void CDlgCaliModeSel::OnRadioCaliNow() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_CalibrationType = CALIBRATION_NOW;		
}

void CDlgCaliModeSel::OnOK() 
{
	// TODO: Add extra validation here
	CCryotherapyDlg *pCryDlg;
	CDlgMain* pDlgMain = g_DlgMain;
	pCryDlg = &(pDlgMain->m_dlgCryotherapy);

	if (m_CalibrationType==CALIBRATION_USE_LAST)
	{
		if (pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER &&
			pDlgMain->m_dlgCryotherapy.m_bMarkerVisible == FALSE) //NAVI MODE
		{
			PanaMessageBox("Patient Marker is not visible. Please reposition camera.");
			return;
		}
		CLog::Log("用户选择使用上次标定结果后点击OK");
		//读、存 标定结果（到内存）
		CString sFilePath;
		int nBaseIndex;
		CConfigMgr conf;
		nBaseIndex = conf.Read(CONF_BASE,CONF_BASE_INDEX,0);
		switch(nBaseIndex) {
		case 0:
//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE1,"0");
			sFilePath = CONFIG_FILES[CF_BASE1];
			break;
		case 1:
//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE2,"0");
			sFilePath = CONFIG_FILES[CF_BASE2];
			break;
		case 2:
//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE3,"0");
			sFilePath = CONFIG_FILES[CF_BASE3];
			break;
		case 3:
//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE4,"0");
			sFilePath = CONFIG_FILES[CF_BASE4];
			break;			
		}	
		CCryotherapyDlg *pCryDlg;
		pCryDlg = &(g_DlgMain->m_dlgCryotherapy);
		if(!(pCryDlg->LoadTransformation(sFilePath,pCryDlg->m_tranScanner2Base.value))) 
		{
			CString sMsg;
			sMsg.Format("上次标定结果装载失败，文件路径： %s", sFilePath);
			CLog::Log(sMsg);
			PanaMessageBox("Can't open base calibration result!");
			return;
		}
		if (pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
		{
			Matrix tTrack2Marker = pDlgMain->m_dlgCryotherapy.m_tMarker.Inverse();
			pDlgMain->m_dlgCryotherapy.m_tranScanner2Marker = tTrack2Marker * pDlgMain->m_dlgCryotherapy.m_tBase * pDlgMain->m_dlgCryotherapy.m_tranScanner2Base;
		}

		pCryDlg->CreateTransform();
		pCryDlg->InitFrameBoxPosition();
	
		::SendMessage(g_DlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_OK);
		
		CLog::Log("上次标定结果装载成功");
		CDialog::OnOK();
	}
	else if (m_CalibrationType == CALIBRATION_MANUAL)
	{
		CString sFilePath = CONFIG_FILES[CF_PHANTOM];
		Matrix mPhatomGeo(4,4);
		if (!pCryDlg->LoadTransformation(sFilePath, mPhatomGeo.value))
		{
			PanaMessageBox("Error in loading phatom file. Please check it.");
			return;
		}

		CDlgManualBaseCali dlgManualBaseCali;
		dlgManualBaseCali.m_pCryoDlg = pCryDlg;
		pDlgMain->AddAMsgRcvWnd(&dlgManualBaseCali);
		if ( dlgManualBaseCali.DoModal() == IDOK ) 
		{
			pDlgMain->RemoveAMsgRcvWnd(&dlgManualBaseCali);
			::SendMessage(pDlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_OK);
			CDialog::OnOK();
		}
		else
		{
			pDlgMain->RemoveAMsgRcvWnd(&dlgManualBaseCali);
			return;
		}
	}
	else if (m_CalibrationType == CALIBRATION_NOW) 
	{
		CString sFilePath = CONFIG_FILES[CF_PHANTOM];
		Matrix mPhatomGeo(4,4);
		if (!pCryDlg->LoadTransformation(sFilePath, mPhatomGeo.value))
		{
			PanaMessageBox("Error in loading phatom file. Please check it.");
			return;
		}

		CDlgBaseCalibration dlgOnlineCali;
		dlgOnlineCali.m_pCryoDlg = pCryDlg;
		pDlgMain->AddAMsgRcvWnd(&dlgOnlineCali);
		pDlgMain->m_pBaseCaliDlg = &dlgOnlineCali;
		if (dlgOnlineCali.DoModal() == IDOK)
		{
			pDlgMain->RemoveAMsgRcvWnd(&dlgOnlineCali);
			pDlgMain->m_pBaseCaliDlg = NULL;
			::SendMessage(pDlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_OK);
			CDialog::OnOK();
		}
		else
		{
			pDlgMain->RemoveAMsgRcvWnd(&dlgOnlineCali);
			pDlgMain->m_pBaseCaliDlg = NULL;
			return;
		}
	}
	
	CPanaseeDialog::OnOK();
}

void CDlgCaliModeSel::OnCancel() 
{
	// TODO: Add extra cleanup here

	::SendMessage(g_DlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_ERR);
	
	CPanaseeDialog::OnCancel();
}

void CDlgCaliModeSel::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		
		CDlgMain *pDlgMain = g_DlgMain;
		
		BOOL bEnableMRICtrl = pDlgMain->m_bEnableMRICtrl;
		BOOL bDebugNoCamera = pDlgMain->m_bDebugNoCamera;
		if (!bEnableMRICtrl || !pDlgMain->m_MRIWrap.m_bMRIConneted
			|| bDebugNoCamera || !pDlgMain->m_trackerManager.GetHandler()->IsValidDevice()
			|| !CGlobalFuncs::IsFileExist(CONFIG_FILES[CF_PHANTOM]))
		{
			GetDlgItem(IDC_RADIO_CALI_NOW)->EnableWindow(FALSE);
		}
		else
		{
			m_CalibrationType = CALIBRATION_NOW;
			m_iCalMode = m_CalibrationType;
		}

		CConfigMgr conf;
		int iBaseIndex = conf.Read(CONF_BASE, CONF_BASE_INDEX, 0);
		CString sBaseFile = CONFIG_FILES[iBaseIndex - CF_BASE1];
		if (!CGlobalFuncs::IsFileExist(sBaseFile))
		{
			GetDlgItem(IDC_RADIO_CALI_LAST)->EnableWindow(FALSE);
			if(m_CalibrationType != CALIBRATION_NOW)
			{
				m_CalibrationType = CALIBRATION_MANUAL;
			}
			m_iCalMode = m_CalibrationType;
		}else
		{
			GetDlgItem(IDC_RADIO_CALI_LAST)->EnableWindow(TRUE);
			m_CalibrationType = CALIBRATION_USE_LAST;
			m_iCalMode = m_CalibrationType;
		}
		UpdateData(FALSE);
	}
}

void CDlgCaliModeSel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	CRect rc;
//	GetClientRect(&rc);
//	
//	CShowImage showImage;
//	showImage.SetImage(AfxGetResourceHandle(),IDR_PNG_INFO_PROMPT,1);
//	BITMAP bitInfo;
//	showImage.GetBitmap(&bitInfo);
//	rc.left=20;
//	rc.top=20;
//	rc.right=rc.left+bitInfo.bmWidth;
//	rc.bottom=rc.top+bitInfo.bmHeight;
//	showImage.SetShowRegion(rc);
//	showImage.DrawImage(&dc,0);
	// Do not call CPanaseeDialog::OnPaint() for painting messages
}
