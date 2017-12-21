// ProbeControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProbeControlDlg.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeControlDlg dialog


CProbeControlDlg::CProbeControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProbeControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProbeControlDlg)
	m_iMode = 2;
	//}}AFX_DATA_INIT
	
}


void CProbeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeControlDlg)
	DDX_Radio(pDX, IDC_RADIO_TIP, m_iMode);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CProbeControlDlg, CDialog)
	//{{AFX_MSG_MAP(CProbeControlDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_RADIO_TIP, OnRadioTip)
	ON_BN_CLICKED(IDC_RADIO_NONE, OnRadioNone)
	ON_BN_CLICKED(IDC_RADIO_END, OnRadioEnd)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeControlDlg message handlers

BOOL CProbeControlDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_bChangeToolByMouse=FALSE;
	m_bOperation=1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeControlDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;

	
	GetDlgItem(IDC_TOOL_REGION)->GetWindowRect( &rc );
	ScreenToClient( &rc );
	
	UpdateData(TRUE);
	
	
	if(point.x>rc.left && point.x < rc.right && point.y > rc.top && point.y <rc.bottom)
	{
		m_bChangeToolByMouse=TRUE;
		m_pInitialPos=point;
		
		
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CProbeControlDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bChangeToolByMouse=FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CProbeControlDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bChangeToolByMouse)
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	CRect rc;
	GetDlgItem(IDC_TOOL_REGION)->GetWindowRect( &rc );
	ScreenToClient( &rc );
	
	if(point.x>rc.left && point.x < rc.right && point.y > rc.top && point.y <rc.bottom)
	{
		if(m_bChangeToolByMouse)
		{
			double dpos[3];
			double dori[3];
			dpos[0] = 0;
			dpos[1] = 0;
			dpos[2] = 0;
			dori[0] = 0;
			dori[1] = 0;
			dori[2] = 0;
			if(m_bOperation==0)
			{
				
				dpos[1] = point.x-m_pInitialPos.x; //ÑØÕëµÀ
				
			}
			if(m_bOperation==1)
			{
				dori[1] = point.x-m_pInitialPos.x; //ÑØÕëµÀ
			}
			m_pInitialPos=point;
			CGlobalManager::Manager()->ReplaceProbe(dpos,dori);
			
			
			CGlobalManager::Manager()->m_2DView1->InvalidateRgn(&CGlobalManager::Manager()->m_2DView1->m_nonClientRegion,FALSE);
			CGlobalManager::Manager()->m_2DView2->InvalidateRgn(&CGlobalManager::Manager()->m_2DView2->m_nonClientRegion,FALSE);
			CGlobalManager::Manager()->m_2DView3->InvalidateRgn(&CGlobalManager::Manager()->m_2DView3->m_nonClientRegion,FALSE);
			
			CGlobalManager::Manager()->m_3DView->InvalidateRgn(&CGlobalManager::Manager()->m_3DView->m_nonClientRegion,FALSE);
		}

	}
	CDialog::OnMouseMove(nFlags, point);
}

void CProbeControlDlg::OnRadioTip() 
{
	// TODO: Add your control notification handler code here
	CGlobalManager::Manager()->LocateProbeTip();
}

void CProbeControlDlg::OnRadioNone() 
{
	// TODO: Add your control notification handler code here
	CGlobalManager::Manager()->LocateProbeNone();
}

void CProbeControlDlg::OnRadioEnd() 
{
	// TODO: Add your control notification handler code here
	CGlobalManager::Manager()->LocateProbeTail();
}

void CProbeControlDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow==TRUE)
	{
		if(CGlobalManager::Manager()->IsLocateProbeTip())
		{
			m_iMode=0;
		}else if(CGlobalManager::Manager()->IsLocateProbeTail())
		{
			m_iMode=1;
		}else
		{
			m_iMode=2;
		}
		UpdateData(FALSE);
	}else
	{
		CGlobalManager::Manager()->LocateProbeNone();
	}
}
