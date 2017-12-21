// DlgMovePT.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMovePT.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMovePT dialog


CDlgMovePT::CDlgMovePT(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgMovePT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMovePT)
	m_dCurPos = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgMovePT::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMovePT)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_PT_CUR_POS, m_dCurPos);
	DDV_MinMaxDouble(pDX, m_dCurPos, -2000., 2000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMovePT, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgMovePT)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMovePT message handlers
BOOL CDlgMovePT::OnInitDialog() 
{
	EnableSeprateLine(TRUE);
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	//m_btnOK.SetThemeHelper(&m_ThemeHelper);

	//…Ë÷√UI
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMovePT::OnOK() 
{
	// TODO: Add extra validation here
	//UpdateData(TRUE);
	if(fabs(m_dCurPos)>3000.0)
	{
		MessageBox("The position of patient table is too large.");
		return;
	}

	CDialog::OnOK();
}

void CDlgMovePT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
