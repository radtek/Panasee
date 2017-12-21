// DlgWaiting.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgWaiting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWaiting dialog


CDlgWaiting::CDlgWaiting(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgWaiting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWaiting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgWaiting::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWaiting)
	DDX_Control(pDX, IDC_STATIC_WAITING, m_wait);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWaiting, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgWaiting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWaiting message handlers

BOOL CDlgWaiting::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	int w=350;
	int h=50;

	CRect rc;
	rc.left=(x-w)/2;
	rc.right=x-rc.left;
	rc.top=(y-h)/3;
	rc.bottom=rc.top+50;
	MoveWindow(rc);

	m_wait.MoveWindow(20,10,w-40,h-20);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWaiting::showText(CString str)
{
	m_wait.SetWindowText(str);
}
