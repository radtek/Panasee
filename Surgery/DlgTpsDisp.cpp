// DlgTpsDisp.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTpsDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTpsDisp dialog


CDlgTpsDisp::CDlgTpsDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTpsDisp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTpsDisp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTpsDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTpsDisp)
	DDX_Control(pDX, IDC_STATIC_DISP, m_disp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTpsDisp, CDialog)
	//{{AFX_MSG_MAP(CDlgTpsDisp)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTpsDisp message handlers

BOOL CDlgTpsDisp::OnEraseBkgnd(CDC* pDC)
{
 	return TRUE;
}

BOOL CDlgTpsDisp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTpsDisp::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_disp.GetSafeHwnd())
		return;
	m_disp.MoveWindow(0,0,cx-3,cy-3);
}

void CDlgTpsDisp::display(CAppDialog *pDlg)
{
	m_disp.ChangeInnerDlg(pDlg);
}

void CDlgTpsDisp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc,RGB(0,0,0)); //232,244,244));
	// Do not call CDialog::OnPaint() for painting messages
}
