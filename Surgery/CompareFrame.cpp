// CompareFrame.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "CompareFrame.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompareFrame

CCompareFrame::CCompareFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CCompareFrame::IDD, pParent)
{

}

CCompareFrame::~CCompareFrame()
{

}


BEGIN_MESSAGE_MAP(CCompareFrame, CDialog)
	//{{AFX_MSG_MAP(CCompareFrame)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompareFrame message handlers


void CCompareFrame::Init(CStringArray& saMRIs,CStringArray& saDesc)
{
	m_saMRIs.RemoveAll();
	m_saMRIs.Copy(saMRIs);
	m_saDesc.RemoveAll();
	m_saDesc.Copy(saDesc);
}

int CCompareFrame::Clean()
{
	m_DlgCtrl.Clean();
	m_DlgDisp.Clean();
	return 0;
}

BOOL CCompareFrame::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	
	m_DlgCtrl.Create(IDD_DLG_COMPARE_CONTROL, this);
	m_DlgCtrl.SetParent(this);
	//m_DlgCtrl.MoveWindow(x-270,0,270,y);
	m_DlgCtrl.ShowWindow(SW_SHOW);

	m_DlgDisp.Create(IDD_DLG_COMPARE_DISP, this);
	m_DlgDisp.SetParent(this);
	//m_DlgDisp.MoveWindow(0,0,x-270,y);
	
	m_DlgDisp.ShowWindow(SW_SHOW);
	MoveWindow(0,1,x,y);
	
	m_DlgCtrl.m_pCmpFrm = this;
	m_DlgCtrl.InitMIRList(m_saMRIs,m_saDesc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCompareFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_DlgCtrl.GetSafeHwnd())
		{
			m_DlgCtrl.MoveWindow(rect.Width()-270,0,270,rect.Height());
		}
		if (m_DlgDisp.GetSafeHwnd())
		{
			m_DlgDisp.MoveWindow(0,0,rect.Width()-270,rect.Height());
		}
	}

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
