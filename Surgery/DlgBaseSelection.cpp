// DlgBaseSelection.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgBaseSelection.h"
#include "ConfigMgr.h"//Add by Dai Liang 2007.6.5
#include "CommandHandling.h"
#include "DlgWizard.h"
#include "DlgMain.h"
#include "Log.h"
#include "globalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgBaseSelection dialog

CDlgBaseSelection::CDlgBaseSelection(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgBaseSelection::IDD, pParent)
{
	m_iRadioBase = 0;
	m_iImgID = IDR_PNG_BASE1;

	m_bDisableTimer=FALSE;
}

void CDlgBaseSelection::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBaseSelection)
	DDX_Control(pDX, ID_CONFIRM, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_BASE_INDICATOR, m_btnBase);
	DDX_Radio(pDX, IDC_RADIO_BASE_1, m_iRadioBase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBaseSelection, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgBaseSelection)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_BASE_1, OnRadioBase1)
	ON_BN_CLICKED(IDC_RADIO_BASE_2, OnRadioBase2)
	ON_BN_CLICKED(IDC_RADIO_BASE_3, OnRadioBase3)
	ON_BN_CLICKED(IDC_RADIO_BASE_4, OnRadioBase4)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_CONFIRM, OnConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBaseSelection message handlers

BOOL CDlgBaseSelection::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	int nCount=4;
	CConfigMgr conf;
	m_iRadioBase = conf.Read(CONF_BASE, CONF_BASE_INDEX, 0);
	if (m_iRadioBase<0 || m_iRadioBase>=nCount)
	{
		m_iRadioBase = 0;
	}
	
	if (m_iRadioBase == 0)
	{
		m_iImgID = IDR_PNG_BASE1;
	}
	else if (m_iRadioBase == 1)
	{
		m_iImgID = IDR_PNG_BASE2;
	}
	else if (m_iRadioBase == 2)
	{
		m_iImgID = IDR_PNG_BASE3;
	}
	else if (m_iRadioBase == 3)
	{
		m_iImgID = IDR_PNG_BASE4;
	}


	m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnBase.SizeToContent();
	//CString sTip;
	//sTip.Format("Base %d", m_iRadioBase + 1);
	//m_btnBase.SetTooltipText(sTip);

	UpdateData(FALSE);
	m_btnOK.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBaseSelection::OnCancel() 
{
	CDlgMain *pDlgMain = g_DlgMain;
	::SendMessage(pDlgMain->m_dlgWizard.GetSafeHwnd(), WM_WIZARD_STEP, WSTEP_CAL, WSRET_ERR);

	CPanaseeDialog::OnCancel();
}

void CDlgBaseSelection::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CShowImage showImg;
	showImg.SetImage(AfxGetResourceHandle(), m_iImgID, 1);

	BITMAP bmp;
	showImg.GetBitmap(&bmp);

	double radio=0.9;
	CRect rect;
	rect.left = 10;
	rect.top = 10;
	rect.right = 10	+ bmp.bmWidth*radio;
	rect.bottom = 10 + bmp.bmHeight*radio;

	CMemDC dcMem1;
    int nSavedDC;
    nSavedDC = dc.SaveDC();
    dcMem1.LoadDC(&dc, rect);

	showImg.SetShowRegion(0, 0, bmp.bmWidth*radio, bmp.bmHeight*radio);

	dcMem1.FillSolidRect(0, 0, bmp.bmWidth*radio, bmp.bmHeight*radio, RGB(231,244,244));

	showImg.DrawImage(&dcMem1, 0); 
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgBaseSelection::OnDestroy() 
{
	CPanaseeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	for (int i=0; i<4; i++)
//	{
//		m_bmpBase[i].Detach();
//	}
}

void CDlgBaseSelection::OnRadioBase1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgMain *pDlgMain = g_DlgMain;
	pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(m_iRadioBase);
	m_iImgID = IDR_PNG_BASE1;
	Invalidate(FALSE);
	UpdateWindow();
	UpdateBaseStatus();
}

void CDlgBaseSelection::OnRadioBase2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgMain *pDlgMain = g_DlgMain;
	pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(m_iRadioBase);
	m_iImgID = IDR_PNG_BASE2;
	Invalidate(FALSE);
	UpdateWindow();
	UpdateBaseStatus();
}

void CDlgBaseSelection::OnRadioBase3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgMain *pDlgMain = g_DlgMain;
	pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(m_iRadioBase);
	m_iImgID = IDR_PNG_BASE3;
	Invalidate(FALSE);
	UpdateWindow();
	UpdateBaseStatus();
}

void CDlgBaseSelection::OnRadioBase4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgMain *pDlgMain = g_DlgMain;
	pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(m_iRadioBase);
	m_iImgID = IDR_PNG_BASE4;
	Invalidate(FALSE);
	UpdateWindow();
	UpdateBaseStatus();
}

void CDlgBaseSelection::UpdateBaseStatus()
{
	CDlgMain *pDlgMain = g_DlgMain;

	BOOL bVisible = FALSE;
	bVisible = pDlgMain->m_dlgCryotherapy.IsBaseCouldbeVisible(m_iRadioBase);
	if (bVisible)
	{
		m_btnBase.SetIcon(IDI_LEDON, IDI_LEDON);
		m_btnOK.EnableWindow(TRUE);
	}
	else
	{
		m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	
		BOOL bHasCamera = pDlgMain->m_bHasCamera;
		m_btnOK.EnableWindow(!bHasCamera);
	}

#ifdef _DEBUG
	m_btnOK.EnableWindow(TRUE);
#endif
	if(pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER)
	{
		m_btnOK.EnableWindow(TRUE);
	}

	//CString sTip;
	//sTip.Format("Base %d", m_iRadioBase + 1);
	//m_btnBase.SetTooltipText(sTip);
	Invalidate(FALSE);
	UpdateWindow();
}

void CDlgBaseSelection::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		CDlgMain *pDlgMain = g_DlgMain;
		pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(m_iRadioBase);
		UpdateBaseStatus();
		SetTimer(INDICATOR_UPDATE_TIMER2,MY_ELAPSE200,NULL);
	}else
	{
		KillTimer(INDICATOR_UPDATE_TIMER2);
	}
}

void CDlgBaseSelection::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == INDICATOR_UPDATE_TIMER2 && !m_bDisableTimer)
	{
		UpdateBaseStatus();
	}
	CPanaseeDialog::OnTimer(nIDEvent);
}

void CDlgBaseSelection::OnConfirm() 
{
	// TODO: Add your control notification handler code here
	m_bDisableTimer=TRUE;
	CString sMsg;
	sMsg.Format("Are you sure to select base %d ?", m_iRadioBase+1);	// Modified by Wei Wei [2007.12.24]
	if (PanaMessageBox(sMsg, MB_OKCANCEL) == IDCANCEL)
	{
		m_bDisableTimer=FALSE;
		return;
	}
	m_bDisableTimer=FALSE;

	sMsg.Format("Ñ¡ÔñBase %d¡£", m_iRadioBase+1);
	CLog::Log(sMsg);
	CConfigMgr conf;
	conf.Write(CONF_BASE, CONF_BASE_INDEX, m_iRadioBase);

	CDlgMain *pDlgMain = g_DlgMain;
	::SendMessage(pDlgMain->m_dlgWizard.GetSafeHwnd(), WM_WIZARD_STEP, WSTEP_CAL, WSRET_OK);

	CPanaseeDialog::OnOK();
}
