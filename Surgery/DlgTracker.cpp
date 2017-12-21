// DlgTracker.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTracker.h"
#include "showImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTracker dialog


CDlgTracker::CDlgTracker(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTracker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTracker)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_barHeight=10; 
	m_edge=10;
	m_basePos[0]=m_basePos[1]=m_basePos[2]=0;
	
	m_baseRange=1000;
	m_basePos[0]=0;
	m_basePos[1]=0;
	m_basePos[2]=-350;

	m_bEnable=FALSE;
}


void CDlgTracker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTracker)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTracker, CDialog)
	//{{AFX_MSG_MAP(CDlgTracker)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTracker message handlers

BOOL CDlgTracker::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTracker::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CreateDraw();
}

BOOL CDlgTracker::OnEraseBkgnd(CDC* pDC)
{	
	return TRUE;
}

void CDlgTracker::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	if(!IsWindowVisible())
		return;
	m_skinDC.DrawBack(&dc);

	if(!m_bEnable)
		return;

	CPen pen(PS_SOLID,2,RGB(255,255,255));
	CPen *pOldPen=dc.SelectObject(&pen);

	int len=30;
	dc.MoveTo(m_trackerPos[0]+len,m_trackerPos[1]);
	dc.LineTo(m_trackerPos[0]-len,m_trackerPos[1]);
	dc.MoveTo(m_trackerPos[0],m_trackerPos[1]-len);
	dc.LineTo(m_trackerPos[0],m_trackerPos[1]+len);

	dc.MoveTo(m_trackerPos[2],m_barPos[1]-len/2);
	dc.LineTo(m_trackerPos[2],m_barPos[1]);
	dc.MoveTo(m_trackerPos[2]-6,m_barPos[1]-len/2);
	dc.LineTo(m_trackerPos[2],m_barPos[1]-len/4);
	dc.MoveTo(m_trackerPos[2]+6,m_barPos[1]-len/2);
	dc.LineTo(m_trackerPos[2],m_barPos[1]-len/4);
	dc.MoveTo(m_trackerPos[2]-6,m_barPos[1]-len/2);
	dc.LineTo(m_trackerPos[2]+6,m_barPos[1]-len/2);

	dc.SelectObject(pOldPen);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTracker::SetBasePos(double x, double y, double z)
{
	if(!m_bEnable)
		return;

	
	double dRatio=(y-m_basePos[0])/m_baseRange;
	if(dRatio>1)
		dRatio=1.0;
	if(dRatio<-1)
	{
		dRatio=-1.0;
	}
	m_trackerPos[0]=m_circlePos[0]+(int)(m_circleRadius*dRatio);
	dRatio=(x-m_basePos[1])/m_baseRange;
	if(dRatio>1)
		dRatio=1.0;
	if(dRatio<-1)
	{
		dRatio=-1.0;
	}
	m_trackerPos[1]=m_circlePos[1]+(int)(m_circleRadius*dRatio);

	dRatio=(z-m_basePos[2])/m_baseRange;
	if(dRatio>1)
		dRatio=1.0;
	if(dRatio<-1)
	{
		dRatio=-1.0;
	}
	
	m_trackerPos[2]=m_barPos[0]+(int)(m_barLen/2)+(int)(m_barLen*0.5*dRatio);

	Invalidate(FALSE);
}

void CDlgTracker::Enable(BOOL bEnable)
{
	m_bEnable=bEnable;
	//CreateDraw();
}

BOOL CDlgTracker::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;    

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTracker::SetBaseRange(double *pos, int range)
{
	m_baseRange=range;
	m_basePos[0]=pos[0];
	m_basePos[1]=pos[1];
	m_basePos[2]=pos[2];
}

void CDlgTracker::CreateDraw()
{
	CRect rc;
	GetClientRect(&rc);
	int cx=rc.Width();
	int cy=rc.Height();
	if(cx>cy)
	{
		int iLeft=(cx-cy)/2;
		m_barPos[0]=iLeft+m_edge;
		m_barPos[1]=cy-m_barHeight;
		m_barLen=cx-iLeft*2-m_edge*2;

		int r1=(cy-m_barHeight)/2;
		int r2=m_barLen/2;
		m_circleRadius=r1;
		if(r1>r2)
			m_circleRadius=r2;
		m_circlePos[0]=cx/2;
		m_circlePos[1]=r1;
	}else
	{
		m_barPos[0]=m_edge;
		m_barPos[1]=cy-(cy-cx)/2-m_barHeight;
		m_barLen=cx-m_edge*2;

		int r1=(cy-(cy-cx)-m_barHeight)/2;
		int r2=m_barLen/2;
		m_circleRadius=r1;
		if(r1>r2)
			m_circleRadius=r2;
		m_circlePos[0]=cx/2;
		m_circlePos[1]=r1;
	}
		
	m_circleRadius=(int)(m_circleRadius*0.7);
		
	m_trackerPos[0]=m_circlePos[0];
	m_trackerPos[1]=m_circlePos[1];
	m_trackerPos[2]=m_barPos[0]+m_barLen/2;

	
	
	CClientDC dc(this);
	
	m_skinDC.EnableDrawBack(FALSE);
    m_skinDC.LoadDC(&dc, rc);	
	
	m_skinDC.FillSolidRect(0,0,rc.Width(),rc.Height(), RGB(10,10,10));

	CShowImage showImg;
	
	if(m_bEnable)
	{
		showImg.SetImage(AfxGetResourceHandle(), IDR_PNG_TRACKER_BASE, 1);
		if(m_circleRadius>=70)
		{
			showImg.SetShowRegion(m_circlePos[0]-85, m_circlePos[1]-85, 170,170);
		}else
		{
			showImg.SetShowRegion(int(m_circlePos[0]-m_circleRadius), int(m_circlePos[1]-m_circleRadius), int(m_circleRadius*2),int(m_circleRadius*2));
		}
	}else
	{
		showImg.SetImage(AfxGetResourceHandle(), IDR_PNG_GENERAL_BK, 1);
		showImg.SetShowRegion(0,0,rc.Width(),rc.Height());
	}
	showImg.DrawImage(&m_skinDC, 0); 

	if(m_bEnable)
	{
		rc.left=m_barPos[0];
		rc.top=m_barPos[1];
		rc.bottom=m_barPos[1]+m_barHeight;
		rc.right=rc.left+2*m_barLen/5;
		m_skinDC.FillSolidRect(&rc,RGB(146,146,146));
		
		rc.left=rc.right;
		rc.right=rc.left+m_barLen/5;
		m_skinDC.FillSolidRect(&rc,RGB(0,255,0));
		
		rc.left=rc.right;
		rc.right=rc.left+2*m_barLen/5;
		m_skinDC.FillSolidRect(&rc,RGB(146,146,146));
	}
}
