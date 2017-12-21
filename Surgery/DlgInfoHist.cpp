// DlgInfoHist.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgInfoHist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoHist dialog


CDlgInfoHist::CDlgInfoHist(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgInfoHist::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfoHist)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgInfoHist::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfoHist)
	DDX_Control(pDX, IDC_LST_INFO_HIST, m_lstInfoHist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfoHist, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgInfoHist)
	ON_WM_SIZE()
	ON_WM_MEASUREITEM()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoHist message handlers

BOOL CDlgInfoHist::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_lstInfoHist.SetExtendedStyle(dwExStyle);
	m_lstInfoHist.InsertColumn( 0, "", LVCFMT_LEFT, 400);
	m_lstInfoHist.InitHeader();
	m_lstInfoHist.EnableGridLine(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInfoHist::OnSize(UINT nType, int cx, int cy) 
{
	CPanaseeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect);

	if (m_lstInfoHist.GetSafeHwnd())
	{
		m_lstInfoHist.MoveWindow(rect);
		m_lstInfoHist.SetColumnWidth(0, rect.Width() - 10);
		CRgn rgn;
		rect.OffsetRect(-rect.left,-rect.top);
		rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,5,5);

		m_lstInfoHist.SetWindowRgn(rgn,FALSE);
		rgn.DeleteObject();
//		m_lstInfoHist.GetColumn(0, pCol);
//		pCol->cx = rect.Width() - 10;
	}
}

void CDlgInfoHist::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_lstInfoHist.MeasureItem(lpMeasureItemStruct);
	
	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CDlgInfoHist::AddAMessage(CString sMsg)
{		
	int iButtom = m_lstInfoHist.GetItemCount();
	int nItem = m_lstInfoHist.InsertItem(iButtom, sMsg);
}

void CDlgInfoHist::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		CRect rect;
		GetClientRect(rect);

		if (m_lstInfoHist.GetSafeHwnd())
		{
			m_lstInfoHist.MoveWindow(rect);
		}
	}
}
