// DlgTracking.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTracking.h"
#include "dlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTracking dialog


CDlgTracking::CDlgTracking(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTracking::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTracking)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTrackingFlag=FALSE;
	m_viewMode =TVIEW_TAPV;
}


void CDlgTracking::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTracking)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTracking, CDialog)
	//{{AFX_MSG_MAP(CDlgTracking)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SYS_UPDATE_WL,OnUpdateWL)
	ON_MESSAGE(WM_SYS_UPDATE_TARGET,OnUpdateTargetIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTracking message handlers
BOOL CDlgTracking::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,RGB(0,0,0));
 	return TRUE;
}

LRESULT CDlgTracking::OnUpdateTargetIndex(WPARAM wParam, LPARAM lParam)
{
	int *pIndex=(int*)wParam;
	m_dlgSagittal.SetSelectedTarget(*pIndex);
	m_dlgAxial.SetSelectedTarget(*pIndex);
	m_dlgProbeEye.SetSelectedTarget(*pIndex);
	m_dlgVirtualUS.SetSelectedTarget(*pIndex);
	double *pTargetPos=(double*)lParam;
	m_dlgTargeting.SetTargetPosition(pTargetPos[0],pTargetPos[1],pTargetPos[2]);
	return 0;
}

LRESULT CDlgTracking::OnUpdateWL(WPARAM wParam, LPARAM lParam)
{
	int *pWidth=(int*)wParam;
	int *pLevel=(int*)lParam;
	SetWL(*pWidth,*pLevel);
	return 0;
}

BOOL CDlgTracking::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_dlgTargeting.GetSafeHwnd() == NULL)
	{
		m_dlgTargeting.Create(CDlgTargeting::IDD,this);
		
	}
	m_dlgTargeting.ShowWindow(FALSE);
	

	if(m_dlgProbeEye.GetSafeHwnd() == NULL)
	{
		m_dlgProbeEye.Create(IDD_DLG_USDISPLAY,this);
	}
	m_dlgProbeEye.ShowWindow(FALSE);
	m_dlgProbeEye.SetTitle("Probe Eye");
	m_dlgProbeEye.m_bUseCrossReplaceProbe=TRUE;
	m_dlgProbeEye.EnableSlider(&(g_DlgMain->m_dlgCryotherapy.m_dlgNavigate),0);

	if(m_dlgVirtualUS.GetSafeHwnd() == NULL)
	{
		m_dlgVirtualUS.Create(CDlgUSDisplay::IDD,this);
	}
	m_dlgVirtualUS.ShowWindow(FALSE);
	m_dlgVirtualUS.SetTitle("Virtual US");
	m_dlgVirtualUS.EnableSlider(&(g_DlgMain->m_dlgCryotherapy.m_dlgNavigate),1);

	if(m_dlgAxial.GetSafeHwnd() == NULL)
	{
		m_dlgAxial.Create(CDlgUSDisplay::IDD,this);
	}
	m_dlgAxial.ShowWindow(FALSE);
	m_dlgAxial.SetTitle("Axial");

	if(m_dlgSagittal.GetSafeHwnd() == NULL)
	{
		m_dlgSagittal.Create(CDlgUSDisplay::IDD,this);
	}
	m_dlgSagittal.ShowWindow(FALSE);
	m_dlgSagittal.SetTitle("Sagittal");

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgTracking::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;    

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTracking::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Layout();
}

void CDlgTracking::SetSagittal(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	m_dlgSagittal.SetUSImage(pUSImgData,pAPSI,ppcOrientation);
}

void CDlgTracking::SetAxial(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	m_dlgAxial.SetUSImage(pUSImgData,pAPSI,ppcOrientation);
}

void CDlgTracking::SetProbeEye(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	m_dlgProbeEye.SetUSImage(pUSImgData,pAPSI,ppcOrientation);
}

void CDlgTracking::SetVirtualUS(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	m_dlgVirtualUS.SetUSImage(pUSImgData,pAPSI,ppcOrientation);
}

void CDlgTracking::AddVirtualTarget(CVirtualTarget *pVTarget)
{
	m_dlgSagittal.AddVirtualTarget(pVTarget);
	m_dlgAxial.AddVirtualTarget(pVTarget);
	m_dlgProbeEye.AddVirtualTarget(pVTarget);
	m_dlgVirtualUS.AddVirtualTarget(pVTarget);
}

void CDlgTracking::RemoveVirtualTarget()
{
	m_dlgSagittal.RemoveVirtualTarget();
	m_dlgAxial.RemoveVirtualTarget();
	m_dlgProbeEye.RemoveVirtualTarget();
	m_dlgVirtualUS.RemoveVirtualTarget();
}

void CDlgTracking::CheckSelectedTarget()
{
	m_dlgSagittal.CheckSelectedTarget();
	m_dlgAxial.CheckSelectedTarget();
	m_dlgProbeEye.CheckSelectedTarget();
	m_dlgVirtualUS.CheckSelectedTarget();
}
void CDlgTracking::UpdateProjectVirtualTarget()
{
	m_dlgSagittal.UpdateProjectVirtualTarget();
	m_dlgAxial.UpdateProjectVirtualTarget();
	m_dlgProbeEye.UpdateProjectVirtualTarget();
	m_dlgVirtualUS.UpdateProjectVirtualTarget();
}

void CDlgTracking::SetProbe(CProbe *pProbe)
{
	m_dlgSagittal.SetProbe(pProbe);
	m_dlgAxial.SetProbe(pProbe);
	m_dlgProbeEye.SetProbe(pProbe);
	m_dlgVirtualUS.SetProbe(pProbe);
}

void CDlgTracking::RemoveProbe()
{
	m_dlgSagittal.RemoveProbe();
	m_dlgAxial.RemoveProbe();
	m_dlgProbeEye.RemoveProbe();
	m_dlgVirtualUS.RemoveProbe();
}

void CDlgTracking::Refresh()
{
	if(IsWindowVisible())
	{
		if(m_bTrackingFlag)
		{
			if(m_dlgSagittal.IsWindowVisible())
			{
				m_dlgSagittal.Redraw();
			}
			if(m_dlgAxial.IsWindowVisible())
			{
				m_dlgAxial.Redraw();
			}
			if(m_dlgProbeEye.IsWindowVisible())
			{
				m_dlgProbeEye.Redraw();
			}
			if(m_dlgVirtualUS.IsWindowVisible())
			{
				m_dlgVirtualUS.Redraw();
			}
			
		}else
		{
			if(m_dlgSagittal.IsWindowVisible())
			{
				m_dlgSagittal.Invalidate(FALSE);
			}
			if(m_dlgAxial.IsWindowVisible())
			{
				m_dlgAxial.Invalidate(FALSE);
			}
			if(m_dlgProbeEye.IsWindowVisible())
			{
				m_dlgProbeEye.Invalidate(FALSE);
			}
			if(m_dlgVirtualUS.IsWindowVisible())
			{
				m_dlgVirtualUS.Invalidate(FALSE);
			}
			
		}
	}
}

void CDlgTracking::Layout()//..²¼¾ÖÉèÖÃ
{
	CRect rs;
	GetClientRect(rs);

	int iGap = 1;
	
	if(!m_dlgSagittal.GetSafeHwnd() || !m_dlgAxial.GetSafeHwnd()||!m_dlgProbeEye.GetSafeHwnd()||
		!m_dlgVirtualUS.GetSafeHwnd()||!m_dlgTargeting.GetSafeHwnd())
		return;

	m_dlgAxial.ShowWindow(SW_HIDE);
	m_dlgSagittal.ShowWindow(SW_HIDE);
	m_dlgProbeEye.ShowWindow(SW_HIDE);
	m_dlgVirtualUS.ShowWindow(SW_HIDE);
	m_dlgTargeting.ShowWindow(SW_HIDE);
	
		
	if(m_viewMode == TVIEW_TAPV)
	{
		m_dlgTargeting.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height()/2-iGap);
		m_dlgAxial.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height()/2-iGap);
		m_dlgProbeEye.MoveWindow(rs.left, rs.top+rs.Height()/2, rs.Width()/2-iGap, rs.Height() - (rs.top+rs.Height()/2));
		m_dlgVirtualUS.MoveWindow(rs.left+rs.Width()/2, rs.top+rs.Height()/2, rs.Width() - (rs.left+rs.Width()/2), rs.Height() - (rs.top+rs.Height()/2));

		m_dlgAxial.ShowWindow(TRUE);
		m_dlgProbeEye.ShowWindow(TRUE);
		m_dlgVirtualUS.ShowWindow(TRUE);
		m_dlgTargeting.ShowWindow(TRUE);
	}else if(m_viewMode == TVIEW_ASPV)
	{
		m_dlgAxial.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height()/2-iGap);
		m_dlgSagittal.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height()/2-iGap);
		m_dlgProbeEye.MoveWindow(rs.left, rs.top+rs.Height()/2, rs.Width()/2-iGap, rs.Height() - (rs.top+rs.Height()/2));
		m_dlgVirtualUS.MoveWindow(rs.left+rs.Width()/2, rs.top+rs.Height()/2, rs.Width() - (rs.left+rs.Width()/2), rs.Height() - (rs.top+rs.Height()/2));

		m_dlgAxial.ShowWindow(TRUE);
		m_dlgSagittal.ShowWindow(TRUE);
		m_dlgProbeEye.ShowWindow(TRUE);
		m_dlgVirtualUS.ShowWindow(TRUE);
		
	}else if(m_viewMode == TVIEW_AST)
	{
		m_dlgAxial.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height()/2-iGap);
		m_dlgSagittal.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height()/2-iGap);
		m_dlgTargeting.MoveWindow(rs.left, rs.top+rs.Height()/2, rs.Width(), rs.Height() - (rs.top+rs.Height()/2));
		

		m_dlgAxial.ShowWindow(TRUE);
		m_dlgSagittal.ShowWindow(TRUE);
		m_dlgTargeting.ShowWindow(TRUE);		
	}else
	{
		m_dlgAxial.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height());
		m_dlgVirtualUS.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height());
		
		m_dlgAxial.ShowWindow(TRUE);
		m_dlgVirtualUS.ShowWindow(TRUE);	
	}
		

}	

void CDlgTracking::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC *pDC=&dc;

	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	

	COLORREF bkColor=RGB(232,244,244);
	int iGap=1;
	
	
	CPen newPen(PS_SOLID,5,bkColor);
	CPen *pOldPen=pDC->SelectObject(&newPen);
	
	int x=rs.Width()/2;
	int y=rs.Height()/2;
	
	if(m_viewMode == TVIEW_TAPV || m_viewMode== TVIEW_ASPV)
	{
		pDC->MoveTo(x,0);
		pDC->LineTo(x,rs.Height());
		pDC->MoveTo(0,y);
		pDC->LineTo(rs.Width(),y);
	}else if(m_viewMode == TVIEW_AST)
	{
		pDC->MoveTo(x,0);
		pDC->LineTo(x,rs.Height()/2);
		pDC->MoveTo(0,y);
		pDC->LineTo(rs.Width(),y);
	}else
	{
		pDC->MoveTo(x,0);
		pDC->LineTo(x,rs.Height());
	}
	
	pDC->SelectObject(pOldPen);	
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTracking::SetWL(int width, int level)
{
	m_dlgSagittal.SetWL(width,level);
	m_dlgAxial.SetWL(width,level);
	m_dlgProbeEye.SetWL(width,level);
	m_dlgVirtualUS.SetWL(width,level);
}



void CDlgTracking::EnableTrackingFlag(BOOL bTracking)
{
	m_bTrackingFlag=bTracking;
	m_dlgSagittal.EnableTrackingFlag(m_bTrackingFlag);
	m_dlgAxial.EnableTrackingFlag(m_bTrackingFlag);
	m_dlgProbeEye.EnableTrackingFlag(m_bTrackingFlag);
	m_dlgVirtualUS.EnableTrackingFlag(m_bTrackingFlag);
}

void CDlgTracking::SetViewMode(TRACKING_VIEW_MODE mode)
{
	m_viewMode=mode;
	Layout();
}

void CDlgTracking::ChangeView2USImage()
{
	m_dlgSagittal.ChangeView2USImage();
	m_dlgAxial.ChangeView2USImage();
	m_dlgProbeEye.ChangeView2USImage();
	m_dlgVirtualUS.ChangeView2USImage();
}

void CDlgTracking::ReshapeScene()
{
	m_dlgSagittal.ReshapeScene();
	m_dlgAxial.ReshapeScene();
	m_dlgProbeEye.ReshapeScene();
	m_dlgVirtualUS.ReshapeScene();
}
