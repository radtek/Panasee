// PageSetDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "PageSetDisplay.h"
#include "ConfigMgr.h"
#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSetDisplay property page

IMPLEMENT_DYNCREATE(CPageSetDisplay, CPropertyPage)

CPageSetDisplay::CPageSetDisplay() : CPropertyPage(CPageSetDisplay::IDD)
{
	//{{AFX_DATA_INIT(CPageSetDisplay)
	m_bShowControl = FALSE;
	//}}AFX_DATA_INIT
}

CPageSetDisplay::~CPageSetDisplay()
{
}

void CPageSetDisplay::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetDisplay)
	DDX_Check(pDX, IDC_CHK_DISP_SETTING_SHOW_CTL, m_bShowControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSetDisplay, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSetDisplay)
	ON_BN_CLICKED(IDC_CHK_DISP_SETTING_SHOW_CTL, OnChkDispSettingShowCtl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSetDisplay message handlers

BOOL CPageSetDisplay::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CConfigMgr conf;
	m_bShowControl = conf.Read(CONF_SET_DISPLAY, CONF_SET_DISPLAY_SHOW_CONTROL, 1);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageSetDisplay::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CConfigMgr conf;
	conf.Write(CONF_SET_DISPLAY, CONF_SET_DISPLAY_SHOW_CONTROL, m_bShowControl);	
	m_pCryoDlg->m_pDisplay4Dlg->ShowControlWin(m_bShowControl);

	return CPropertyPage::OnApply();
}

void CPageSetDisplay::OnChkDispSettingShowCtl() 
{
	// TODO: Add your control notification handler code here
	SetModified(TRUE);	
}
