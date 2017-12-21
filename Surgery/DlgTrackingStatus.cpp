// DlgTrackingStatus.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTrackingStatus.h"

#include "cryotherapyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTrackingStatus dialog


CDlgTrackingStatus::CDlgTrackingStatus(CWnd* pParent /*=NULL*/)
	: CDlgInfoSkin(CDlgTrackingStatus::IDD, pParent)
{
	m_pCryoDlg=NULL;
	m_sBaseTip = "Base";
}

void CDlgTrackingStatus::DoDataExchange(CDataExchange* pDX)
{
	CDlgInfoSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTrackingStatus)
	DDX_Control(pDX, IDC_CMD_PRV, m_btnPrev);
	DDX_Control(pDX, IDC_CMD_NXT, m_btnNext);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lblInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTrackingStatus, CDlgInfoSkin)
	//{{AFX_MSG_MAP(CDlgTrackingStatus)
	ON_BN_CLICKED(IDC_CMD_NXT, OnCmdNxt)
	ON_BN_CLICKED(IDC_CMD_PRV, OnCmdPrv)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgTrackingStatus::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CDlgTrackingStatus::OnInitDialog() 
{
	SetNextBtn(&m_btnNext);
	SetPreBtn(&m_btnPrev);
	SetPromptStatic(&m_lblInfo);
	SetBaseVisable(FALSE);
	SetProbeVisable(FALSE);
	m_lblInfo.SetTextColor(RGB(255,255,255));
	m_lblInfo.SetTextBKColor(RGB(51,51,51));

	CDlgInfoSkin::OnInitDialog();

	m_btnNext.SetWindowText("");
	m_btnPrev.SetWindowText("");
	m_lblInfo.SetWindowText("");

	m_bLastBaseVisible = TRUE;
	SetBaseStatus(FALSE);
	m_bLastProbeVisible = TRUE;
	SetProbeStatus(FALSE);
	
	// TODO: Add extra initialization here
		
	if(m_dlgTracker.GetSafeHwnd()==NULL)
	{
		m_dlgTracker.Create(CDlgTracker::IDD,this);
	}
	m_dlgTracker.ShowWindow(TRUE);
	CRect rc;
	GetClientRect(&rc);
	m_dlgTracker.MoveWindow(14,42,rc.right-13*2,rc.bottom-72);
	
	
	if (m_dlgInfoHist.GetSafeHwnd()==NULL)
	{
		m_dlgInfoHist.Create(CDlgInfoHist::IDD, this);
	}
	m_dlgInfoHist.ShowWindow(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTrackingStatus::SetBaseStatus(BOOL bIsVisible)
{
	if (m_bLastBaseVisible != bIsVisible)
	{
		m_bLastBaseVisible = bIsVisible;
//		if (bIsVisible)
//		{
//			m_btnStatusBase.SetIcon(IDI_LEDON, IDI_LEDON);
//		}
//		else
//		{
//			m_btnStatusBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
//		}
		SetBaseVisable(bIsVisible);
	}
}

void CDlgTrackingStatus::SetProbeStatus(BOOL bIsVisible)
{
	if (m_bLastProbeVisible != bIsVisible)
	{
		m_bLastProbeVisible = bIsVisible;
//		if (bIsVisible)
//		{
//			m_btnStatusProbe.SetIcon(IDI_LEDON, IDI_LEDON);
//		}
//		else
//		{
//			m_btnStatusProbe.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
//		}
		SetProbeVisable(bIsVisible);
	}
}

void CDlgTrackingStatus::SetBaseIndex(int nBaseIndex)
{
	m_sBaseTip.Format("Base %d", nBaseIndex+1);
//	m_btnStatusBase.SetTooltipText(m_sBaseTip);
}

void CDlgTrackingStatus::AddMessage(CString sMsg)
{
	CTime c = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format("%02d:%02d:%02d", c.GetHour(), c.GetMinute(), c.GetSecond());
	
	CString sMsg1;
	sMsg1.Format("%s: %s", sDate, sMsg);
	
	m_lblInfo.SetWindowText(sMsg1);
	CRect rc;
	m_lblInfo.GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	m_dlgInfoHist.AddAMessage(sMsg1);

}



BOOL CDlgTrackingStatus::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;     
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTrackingStatus::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTrackingStatus::OnCmdNxt() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	m_btnNext.GetWindowRect(rect);
	CRect wnRect;
	GetWindowRect(wnRect);

	m_dlgInfoHist.MoveWindow(wnRect.left, rect.top - 410, wnRect.Width(), 410);
	m_dlgInfoHist.ShowWindow(!m_dlgInfoHist.IsWindowVisible());
}

void CDlgTrackingStatus::OnCmdPrv() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	m_btnNext.GetWindowRect(rect);
	CRect wnRect;
	GetWindowRect(wnRect);

	m_dlgInfoHist.MoveWindow(wnRect.left, rect.top - 400, wnRect.Width(), 400);
	m_dlgInfoHist.ShowWindow(!m_dlgInfoHist.IsWindowVisible());	
}

void CDlgTrackingStatus::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	m_btnNext.GetWindowRect(rect);
	CRect wnRect;
	GetWindowRect(wnRect);

	m_dlgInfoHist.MoveWindow(wnRect.left, rect.top - 410, wnRect.Width(), 410);
	m_dlgInfoHist.ShowWindow(!m_dlgInfoHist.IsWindowVisible());

	CDlgInfoSkin::OnLButtonDblClk(nFlags, point);
}

void CDlgTrackingStatus::SetBaseRange(double *pos, int range)
{
	m_dlgTracker.SetBaseRange(pos,range);
}
