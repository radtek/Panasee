// DlgTpsCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTpsCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTpsCtrl dialog


CDlgTpsCtrl::CDlgTpsCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTpsCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTpsCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTpsCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTpsCtrl)
	DDX_Control(pDX, IDC_STATIC_CTRL, m_ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTpsCtrl, CDialog)
	//{{AFX_MSG_MAP(CDlgTpsCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTpsCtrl message handlers

BOOL CDlgTpsCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTpsCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_ctrl.GetSafeHwnd())
		return;
	m_ctrl.MoveWindow(0,0,cx-3,cy-3);
}

void CDlgTpsCtrl::display(CAppDialog *pDlg)
{
	m_ctrl.ChangeInnerDlg(pDlg);
}