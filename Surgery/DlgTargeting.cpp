// DlgTargeting.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTargeting.h"
#include "ShowImage.h"
#include "math.h"
#include "autofont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTargeting dialog


CDlgTargeting::CDlgTargeting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTargeting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTargeting)
	//}}AFX_DATA_INIT
	m_iMaxRadius=70; //pixel
	m_bEnableTargeting=FALSE;

	m_realTargetPos[0]=m_realTargetPos[1]=m_realTargetPos[2]=0;
	m_dDis2Target=m_dTargetError=0;

	m_sError="";
	m_sDistance="";

	m_Base2Scanner.SetDimension(4,4);
	m_Base2Scanner.Identity();

	m_iCurrentRadius=1;
}


void CDlgTargeting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTargeting)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTargeting, CDialog)
	//{{AFX_MSG_MAP(CDlgTargeting)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTargeting message handlers

BOOL CDlgTargeting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgTargeting::OnEraseBkgnd(CDC* pDC)
{	
	return TRUE;
}

void CDlgTargeting::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_circlePos[0]=cx/2;
	m_circlePos[1]=cy/2;
	
	m_targetPos[0]=m_circlePos[0];
	m_targetPos[1]=m_circlePos[1];

	
	
	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	m_skinDC.EnableDrawBack(FALSE);
    m_skinDC.LoadDC(&dc, rc);	
	
	m_skinDC.FillSolidRect(0,0,rc.Width(),rc.Height(), RGB(200,200,200));
	m_skinDC.FillSolidRect(1,1,rc.Width()-2,rc.Height()-2, RGB(100,144,144));
	m_skinDC.FillSolidRect(10,10,rc.Width()-20,rc.Height()-20, RGB(10,10,10));

	CShowImage showImg;
	showImg.SetImage(AfxGetResourceHandle(), IDR_PNG_TARGETING, 1);
	showImg.SetShowRegion(m_circlePos[0]-70, m_circlePos[1]-70, 70*2,70*2);
	showImg.DrawImage(&m_skinDC, 0); 

	m_iMaxRadius=(cx-50)/2;
	if(m_iMaxRadius>(cy-50)/2)
	{
		m_iMaxRadius=(cy-50)/2;
	}
	


}

void CDlgTargeting::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(!IsWindowVisible())
		return;
	CRect rc;
	GetClientRect(&rc);
	m_skinDC.DrawBack(&dc);

	CPen pen(PS_SOLID,2,RGB(255,255,255));
	CPen *pOldPen=dc.SelectObject(&pen);

	int len=30;
	dc.MoveTo(m_targetPos[0]+len,m_targetPos[1]);
	dc.LineTo(m_targetPos[0]-len,m_targetPos[1]);
	dc.MoveTo(m_targetPos[0],m_targetPos[1]-len);
	dc.LineTo(m_targetPos[0],m_targetPos[1]+len);

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)(GetStockObject(NULL_BRUSH)));
	CBrush *pOldBrush = dc.SelectObject(pBrush);
	dc.Ellipse(m_circlePos[0]-m_iCurrentRadius,m_circlePos[1]-m_iCurrentRadius,
		m_circlePos[0]+m_iCurrentRadius,m_circlePos[1]+m_iCurrentRadius);

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	/*********** ningsj *********************
	*****************************************/
	CAutoFont font;
	font.SetHeight(25);
	CFont *pOldFont;
	pOldFont=dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);

	COLORREF oldColor;
	oldColor = dc.SetTextColor(RGB(255,255,0));
	
	rc.top=30;
	rc.bottom=rc.top+40;
	dc.DrawText(m_sError,rc, DT_CENTER | DT_SINGLELINE);
	GetClientRect(&rc);
	rc.top=rc.bottom-70;
	rc.bottom=rc.top+40;
	dc.DrawText(m_sDistance,rc, DT_CENTER | DT_SINGLELINE);
	
	CAutoFont font15;
	font15.SetHeight(15);
	dc.SelectObject(&font15);
	dc.SetTextColor(RGB(255,255,255));
	CString sTmp;
	sTmp.Format("%.1fmm",m_dTargetError);
	rc.top=m_targetPos[1]-10;
	rc.bottom=rc.top+20;
	rc.left=m_targetPos[0]+len+5;
	rc.right=rc.left+80;
	dc.DrawText(sTmp,rc,DT_LEFT | DT_SINGLELINE);

	dc.SelectObject(pOldFont);
	dc.SetTextColor(oldColor);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTargeting::SetTargetPosition(double x, double y, double z)
{
	m_bEnableTargeting=TRUE;
	
	m_realTargetPos[0]=x;
	m_realTargetPos[1]=y;
	m_realTargetPos[2]=z;
}

void CDlgTargeting::SetProbe(Matrix mProbe,double *pAccuracyOrient)
{
	if(!IsWindowVisible())
		return;
	if(!m_bEnableTargeting)
		return;
	
	
	//将坐标换算到Scanner坐标系下
	Matrix m(4,4);
	m.Identity();
	m.value[0][3]=mProbe.value[0][3];
	m.value[1][3]=mProbe.value[1][3];
	m.value[2][3]=mProbe.value[2][3];
	m.value[0][2]=pAccuracyOrient[0];
	m.value[1][2]=pAccuracyOrient[1];
	m.value[2][2]=pAccuracyOrient[2];

	m=m_Base2Scanner * m;

	Vector vx,vy,vz;
	vy.value[0]=m.value[0][2];
	vy.value[1]=m.value[1][2];
	vy.value[2]=m.value[2][2];

	vx.value[0]=1;
	vx.value[1]=0;
	vx.value[2]=0;

	vz = vx * vy;
	vx = vy * vz;

	mProbe.value[0][0]=vx.value[0];
	mProbe.value[1][0]=vx.value[1];
	mProbe.value[2][0]=vx.value[2];
	mProbe.value[0][1]=vy.value[0];
	mProbe.value[1][1]=vy.value[1];
	mProbe.value[2][1]=vy.value[2];
	mProbe.value[0][2]=vz.value[0];
	mProbe.value[1][2]=vz.value[1];
	mProbe.value[2][2]=vz.value[2];
	mProbe.value[0][3]=m.value[0][3];
	mProbe.value[1][3]=m.value[1][3];
	mProbe.value[2][3]=m.value[2][3];

	m.Identity();
	m.value[0][3]=m_realTargetPos[0];
	m.value[1][3]=m_realTargetPos[1];
	m.value[2][3]=m_realTargetPos[2];
	m=m_Base2Scanner * m;
	double dRealTargetPos[3];
	dRealTargetPos[0]=m.value[0][3];
	dRealTargetPos[1]=m.value[1][3];
	dRealTargetPos[2]=m.value[2][3];

	Matrix mScanner2Probe=mProbe.Inverse();
	Matrix mTarget2Scanner(4,4);
	mTarget2Scanner.Identity();
	mTarget2Scanner.value[0][3]=dRealTargetPos[0];
	mTarget2Scanner.value[1][3]=dRealTargetPos[1];
	mTarget2Scanner.value[2][3]=dRealTargetPos[2];

	Matrix mTarget2Probe = mScanner2Probe * mTarget2Scanner;

	double x=mTarget2Probe.value[0][3];
	double y=mTarget2Probe.value[1][3];
	double z=mTarget2Probe.value[2][3];
	double dMaxRange=m_iMaxRadius*0.5;

 	m_dTargetError = sqrt(x*x+z*z);
	m_dDis2Target= sqrt(x*x+y*y+z*z);

	if(x>dMaxRange)
		x=dMaxRange;
	if(x<-dMaxRange)
		x=-dMaxRange;
	double dRatio = x / dMaxRange;
	m_targetPos[1]=int(m_circlePos[1]+dRatio*m_iMaxRadius);

	if(z>dMaxRange)
		z=dMaxRange;
	if(z<-dMaxRange)
		z=-dMaxRange;
	dRatio = (0-z) / dMaxRange;

	m_targetPos[0]=int(m_circlePos[0]+dRatio*m_iMaxRadius);

	m_iCurrentRadius =(int)( dMaxRange * m_dDis2Target/100.0);
	if(m_iCurrentRadius>dMaxRange)
	{
		m_iCurrentRadius=(int)dMaxRange;
	}

	m_sError.Format("Aiming Accuracy:%.2fmm",m_dTargetError);
	m_sDistance.Format("Tip to Target:%.2fmm",m_dDis2Target);
	
	Invalidate(FALSE);
}

void CDlgTargeting::Disable()
{
	m_bEnableTargeting=FALSE;
	m_realTargetPos[0]=m_realTargetPos[1]=m_realTargetPos[2]=0;
	m_targetPos[0]=m_circlePos[0];
	m_targetPos[1]=m_circlePos[1];
	m_sError="";
	m_sDistance="";
}

BOOL CDlgTargeting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;    
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTargeting::SetBase2Scanner(Matrix m)
{
	m_Base2Scanner=m;
}

