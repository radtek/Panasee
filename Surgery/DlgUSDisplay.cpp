// DlgUSDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgUSDisplay.h"
#include "Intersection.h"
#include "probe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUSDisplay dialog


CDlgUSDisplay::CDlgUSDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUSDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUSDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Scene.SetUsingPCColor(TRUE);
	m_pProbe=NULL;
	m_bUseCrossReplaceProbe=FALSE;
	m_imageWidth=2048;
	m_imageLevel=1024;

	m_bEnableMouseMove=FALSE;
	m_bWinWidthLevelUpdate=FALSE;
	m_iSelectedTargetIndex=0;

	m_bTrackingFlag=FALSE;
	m_pHandleDepthEventWnd=NULL;

	m_ppdOrientCharPos = new double*[4];
	for (int i = 0; i < 4; i++ )
	{
		m_ppdOrientCharPos[i] = new double[3];
	}

	m_iSlideMode=0;
	m_bIsSliding=FALSE;
	
}

CDlgUSDisplay::~CDlgUSDisplay()
{
	if ( NULL != m_ppdOrientCharPos )
	{
		for ( int i = 0; i <4; i++ )
		{
			delete []m_ppdOrientCharPos[i];
			m_ppdOrientCharPos[i] = NULL;
		}
		delete []m_ppdOrientCharPos;
		m_ppdOrientCharPos = NULL;
	}

	RemoveVirtualTarget();
	m_Scene.RemoveAllLine();
}

void CDlgUSDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUSDisplay)
	DDX_Control(pDX, IDC_SLIDER_DEPTH, m_depthSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUSDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgUSDisplay)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DEPTH, OnReleasedcaptureSliderDepth)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DEPTH, OnCustomdrawSliderDepth)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUSDisplay message handlers

BOOL CDlgUSDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hwnd = this->GetSafeHwnd();
	m_Scene.Create(hwnd);
	
	m_Scene.SetRenderMode( RENDER_SMOOTHSHADE);
	m_Scene.m_Display.SetBGColor( 0.0f, 0.0f, 0.0f );
	m_Scene.ResetView();

	//Init legend
	m_txtWL.sText = "";
	m_txtWL.cfColor.SetColor(1.0, 1.0, 1.0, 1.0);
	m_txtWL.bShow = TRUE;	
	
	m_txtWL.pFont = CGlobalFuncs::GetFont();
	m_txtWL.bShow = TRUE;
	
	m_Scene.Add2DText(&m_txtWL);

	for (int i=0; i<4; i++)
	{
		m_stTextAPSI4[i].sText = "";
		m_stTextAPSI4[i].cfColor.SetColor(1.0, 1.0, 1.0, 1.0);
		m_stTextAPSI4[i].bShow = FALSE;
		
		m_stTextAPSI4[i].pFont = CGlobalFuncs::GetFont();
		
		m_Scene.Add2DText(&(m_stTextAPSI4[i]));
	}

	m_txtTitle.sText = "";
	m_txtTitle.cfColor.SetColor(0.0, 1.0, 0.0, 1.0);
	m_txtTitle.bShow = TRUE;	
	
	m_txtTitle.pFont = CGlobalFuncs::GetFont20();
	m_txtTitle.bShow = TRUE;
	
	m_Scene.Add2DText(&m_txtTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUSDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_bTrackingFlag)
	{
		if(IsWindowVisible())
		{
			m_Scene.DrawScene();
		}
	}else
	{
		CRect rc;
		GetClientRect(&rc);
		dc.SetBkColor(RGB(0,0,0));
		dc.SetTextColor(RGB(255,255,255));
		dc.TextOut(rc.Width()/2,rc.Height()/2,"Not Navigating");
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgUSDisplay::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,RGB(0,0,0));
 	return TRUE;
}

void CDlgUSDisplay::AddVirtualTarget(CVirtualTarget *pVTarget)
{
	double dSliceDirection[]={1,0,0};
	CVirtualTarget *pTarget = new CVirtualTarget(pVTarget->GetPos(),pVTarget->GetColor(),dSliceDirection);
	pTarget->SetIndex(pVTarget->GetIndex());
	m_lstTarget.AddTail(pTarget);
	
}


void CDlgUSDisplay::RemoveVirtualTarget()
{
	CVirtualTarget *pVTarget;
	POSITION pos=m_lstPrjVTargets.GetHeadPosition();
	while(pos)
	{
		pVTarget=m_lstPrjVTargets.GetNext(pos);
		m_Scene.RemoveVirtualTarget(pVTarget);
		delete pVTarget;
	}
	m_lstPrjVTargets.RemoveAll();

	pos=m_lstTarget.GetHeadPosition();
	while(pos)
	{
		pVTarget=m_lstTarget.GetNext(pos);
		delete pVTarget;
	}
	m_lstTarget.RemoveAll();
}

void CDlgUSDisplay::UpdateProjectVirtualTarget()
{
	if(m_usRect.m_pRectangle==NULL)
		return;

	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;
	m_Scene.GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );

	//删除旧的Taget
	CVirtualTarget *pVTarget;
	POSITION pos=m_lstPrjVTargets.GetHeadPosition();
	while(pos)
	{
		pVTarget=m_lstPrjVTargets.GetNext(pos);
		m_Scene.RemoveVirtualTarget(pVTarget);
		delete pVTarget;
	}
	m_lstPrjVTargets.RemoveAll();

	m_Scene.RemoveAllLine();

	CString text;
	int iCount=0;
	Vector3D vNormal=m_usRect.m_pRectangle->GetNormal();
	Vector3D vEyePos = m_Scene.GetEyePoint();
	CVirtualTarget *pTargetPos;
	pos=m_lstTarget.GetHeadPosition();
	while(pos)
	{
		pTargetPos=m_lstTarget.GetNext(pos);		
		
		Line3D *plLine = new Line3D(pTargetPos->GetPos(), vNormal);
		Vector3D vPrjTargetPos;
		
		if (IntersectMRIWithLine(plLine, m_usRect.m_pRectangle, vPrjTargetPos))
		{
			double dSliceDirection[3]={vNormal.x,vNormal.y,vNormal.z};
			CVirtualTarget *pVPrjTarget = new CVirtualTarget(vPrjTargetPos,pTargetPos->GetColor(),dSliceDirection);
			pVPrjTarget->SetSelected(FALSE);
			pVPrjTarget->SetIndex(pTargetPos->GetIndex());
			
			
			double dbDisEyeTarget = (vEyePos - pTargetPos->GetPos()).GetLen();
			double dbDisEyeProTarget = (vEyePos - vPrjTargetPos).GetLen();
			double dbDisVTarget2VPrjTarget = (vPrjTargetPos - pTargetPos->GetPos()).GetLen();
			if (fabs(dbDisEyeProTarget - dbDisEyeTarget) < 0.001 
				|| dbDisVTarget2VPrjTarget <= 0.5)
			{
				pVPrjTarget->SetRelativePosition(0);
			}
			else if (dbDisEyeProTarget > dbDisEyeTarget)
			{
				pVPrjTarget->SetRelativePosition(2);
			}
			else if (dbDisEyeProTarget < dbDisEyeTarget)
			{
				pVPrjTarget->SetRelativePosition(1);
			}
			
			Vector3D vLabelPos;		
			Vector3D vAB=m_usRect.m_pRectangle->GetVectorAB();
			vAB.normalize();
			Vector3D vAD=m_usRect.m_pRectangle->GetVectorAD();
			vAD.normalize();
			Vector3D vNormal=m_usRect.m_pRectangle->GetNormal();
			vNormal.normalize();
			vLabelPos =vPrjTargetPos + 10*vAB+10*vAD+10*vNormal;
			text.Format("%.1fmm",dbDisVTarget2VPrjTarget);
			pVPrjTarget->SetLabelText(text,vLabelPos);			
			m_Scene.AddVirtualTarget(pVPrjTarget);

			//增加被选择Target的十字线
			if(iCount==m_iSelectedTargetIndex)
			{				
				Vector3D a=vPrjTargetPos + 10*vAB;
				Vector3D b=vPrjTargetPos - 10*vAB;
				m_Scene.AddLine(a,b);
				a=vPrjTargetPos + 10*vAD;
				b=vPrjTargetPos - 10*vAD;
				m_Scene.AddLine(a,b);


			}

			
			m_lstPrjVTargets.AddTail(pVPrjTarget);			
		}
		iCount++;
		delete plLine;
	}

	m_Scene.SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	return;
}

void CDlgUSDisplay::SetProbe(CProbe *pProbe)
{
	if(m_pProbe)
	{
		if(!m_bUseCrossReplaceProbe)
		{
			m_Scene.RemoveProbe(m_pProbe);
		}
		m_pProbe=NULL;
	}
	if(!pProbe)
	{
		return;
	}

	m_pProbe=pProbe;
	if(!m_bUseCrossReplaceProbe)
	{
		m_Scene.AddProbe(m_pProbe);
	}
	
}

void CDlgUSDisplay::RemoveProbe()
{
	if(m_pProbe)
	{
		if(!m_bUseCrossReplaceProbe)
		{
			m_Scene.RemoveProbe(m_pProbe);
		}
		m_pProbe=NULL;
	}
}

void CDlgUSDisplay::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!this->GetSafeHwnd())
		return;

	m_Scene.Reshape(cx,cy);
	SetWL(m_imageWidth,m_imageLevel);

	int Legend_iX = 2;
	int Legend_iY = 11;
	int Legend_iYGap = 11;
	int Legend_iZ =0;	

	m_txtTitle.vPosition = Vector3D(2,Legend_iY+15,0);
	
	m_txtDepth.vPosition = Vector3D(Legend_iX, cy-Legend_iYGap, Legend_iZ);

	m_stTextAPSI4[0].vPosition = Vector3D(cx/2, 20, 0);
	m_stTextAPSI4[1].vPosition = Vector3D(cx-30, cy/2, 0);
	m_stTextAPSI4[2].vPosition = Vector3D(cx/2, cy-20, 0);
	m_stTextAPSI4[3].vPosition = Vector3D(20, cy/2, 0);	

	if(m_depthSlider.GetSafeHwnd())
	{
		m_depthSlider.MoveWindow(102,cy-38,150,35);
	}
	
}
BOOL CDlgUSDisplay::IntersectMRIWithLine(Line3D *pLine, CRectangle *pMRIRect, Vector3D& vInterSectPos)
{
	BOOL bRet = FALSE;
	CIntersection interOper;
	if (0 == interOper.Intersect(pLine, pMRIRect, vInterSectPos))
	{
		bRet = TRUE;
	}
	return bRet; 
}

void CDlgUSDisplay::Redraw()
{
	if(IsWindowVisible())
	{
		m_Scene.DrawScene();
	}
}

void CDlgUSDisplay::ChangeView2USImage()
{
	if(!m_usRect.m_pRectangle)
		return;

	
	m_Scene.ChangeViewToUSRect(&m_usRect);
	
}


void CDlgUSDisplay::ComputeOrientCharPosition(US_STRUCT_IMAGE *pImage)
{
	// End of compute box orient char
	double dFreqDirection[3],dPhaseDirection[3],dCenter[3],dFOV[3];
	// Freq direction
	dFreqDirection[0] = (double) pImage->oHeader.scanField.FreqDirX;
	dFreqDirection[1] = (double) pImage->oHeader.scanField.FreqDirY;
	dFreqDirection[2] = (double) pImage->oHeader.scanField.FreqDirZ;

	// Phase direction
	dPhaseDirection[0] = (double) pImage->oHeader.scanField.PhaseDirX;
	dPhaseDirection[1] = (double) pImage->oHeader.scanField.PhaseDirY;
	dPhaseDirection[2] = (double) pImage->oHeader.scanField.PhaseDirZ;

	// Center
	dCenter[0] = (double) pImage->oHeader.imageField.fCenterPointX;	
	dCenter[1] = (double) pImage->oHeader.imageField.fCenterPointY;	
	dCenter[2] = (double) pImage->oHeader.imageField.fCenterPointZ;	

	// Field of view
	dFOV[0] = (double) pImage->oHeader.seriesField.FovX;
	dFOV[1] = (double) pImage->oHeader.seriesField.FovY;
	dFOV[2] = (double) pImage->oHeader.seriesField.FovZ;

	//计算显示位置
	Vector3D vCenter( dCenter[0], dCenter[1], dCenter[2] );
	Vector3D vXVector( dFreqDirection[0]*dFOV[0], dFreqDirection[1]*dFOV[0], dFreqDirection[2]*dFOV[0] );
	Vector3D vYVector( dPhaseDirection[0]*dFOV[1], dPhaseDirection[1]*dFOV[1], dPhaseDirection[2]*dFOV[1] );
	Vector3D vPosition;

	// Char 0
	vPosition = vCenter - 0.5 * vYVector;
	m_ppdOrientCharPos[0][0] = vPosition.x;
	m_ppdOrientCharPos[0][1] = vPosition.y;
	m_ppdOrientCharPos[0][2] = vPosition.z;
	// Char 1
	vPosition = vCenter + 0.5 * vXVector;
	m_ppdOrientCharPos[1][0] = vPosition.x;
	m_ppdOrientCharPos[1][1] = vPosition.y;
	m_ppdOrientCharPos[1][2] = vPosition.z;
	// Char 2
	vPosition = vCenter + 0.5 * vYVector;
	m_ppdOrientCharPos[2][0] = vPosition.x;
	m_ppdOrientCharPos[2][1] = vPosition.y;
	m_ppdOrientCharPos[2][2] = vPosition.z;
	// Char 3
	vPosition = vCenter - 0.5 * vXVector;
	m_ppdOrientCharPos[3][0] = vPosition.x;
	m_ppdOrientCharPos[3][1] = vPosition.y;
	m_ppdOrientCharPos[3][2] = vPosition.z;
}



void CDlgUSDisplay::SetUSImage(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	//计算APSI并显示
	ComputeOrientCharPosition(pUSImgData);
	for ( int j = 0; j < 4; j++ )
	{		
		m_stTextAPSI4[j].sText = pAPSI[j];
		m_stTextAPSI4[j].bShow = TRUE;
	}

	pUSImgData->oHeader.imageField.WindowLevel=m_imageLevel;
	pUSImgData->oHeader.imageField.WindowWidth=m_imageWidth;
	for(j=0;j<4;j++)
	{
		for(int k=0;k<10;k++)
		{
			m_usRect.m_ppcOrientation[j][k]=ppcOrientation[j][k];
		}
		for(k=0;k<3;k++)
		{
			m_usRect.m_ppdOrientCharPos[j][k]=m_ppdOrientCharPos[j][k];
		}
	}

	m_usRect.SetImg(pUSImgData);

	m_Scene.RemoveUSRect(&m_usRect);
	m_Scene.AddUSRect(&m_usRect, FALSE);

	if(!m_pProbe)
		return;

	m_usRect.m_pRectangle->SetUsingTransparency(TRUE);
	//get probe line
	Line3D Line;
	if (PROBE_DISPLAY_CYLINDER)
	{
		Line.SetOrigin(m_pProbe->m_cylBody.GetCentroid());
		Line.SetOrientation(m_pProbe->m_cylBody.GetAxis());		
	}
	else 
	{
		Line.SetOrigin(m_pProbe->m_sgBody.GetFirstEndpoint());
		Line.SetOrientation(m_pProbe->m_sgBody.GetVector());
	}

	Vector3D vInterSectPos;
	BOOL bRet = FALSE;
	CIntersection interOper;
	if (interOper.Intersect(&Line,m_usRect.m_pRectangle, vInterSectPos) == 0)
	{
		Vector3D vAD = m_usRect.m_pRectangle->GetVectorAD();
		Vector3D vAB = m_usRect.m_pRectangle->GetVectorAB();
		vAD.SetUnit();
		vAB.SetUnit();
		Vector3D p1 = vInterSectPos + vAD * 10;
		Vector3D p2 = vInterSectPos - vAD * 10;
		Vector3D p3 = vInterSectPos + vAB * 10;
		Vector3D p4 = vInterSectPos - vAB * 10;

		m_Scene.SetCross(p1,p2,p3,p4);
		bRet = TRUE;
	}
	
}

BOOL CDlgUSDisplay::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;    

	return CDialog::PreTranslateMessage(pMsg);
}



void CDlgUSDisplay::SetWL(int width, int level)
{
	m_imageWidth=width;
	m_imageLevel=level;

	CRect rect;
	GetClientRect(rect);
		
	CClientDC dc(this);
	CDC	 *pDC =&dc;
	CFont *pOldFont=NULL;

	pOldFont=pDC->SelectObject(m_txtWL.pFont);

	int Legend_iX = 2;
	int Legend_iY = 11;
	int Legend_iYGap = 11;
	int Legend_iZ =0;	
	
	m_txtWL.sText.Format("W=%i,L=%i",m_imageWidth,m_imageLevel);
	m_txtWL.vPosition = Vector3D(rect.Width()-100, rect.Height()-(1)*Legend_iYGap, Legend_iZ);
	pDC->SelectObject(pOldFont);
	
}

void CDlgUSDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bEnableMouseMove=TRUE;
	m_bWinWidthLevelUpdate=FALSE;
	m_ptWindowLevelStart=point;

	m_depthSlider.ShowWindow(FALSE);
	m_bIsSliding=FALSE;
	if(m_iSlideMode==0)
	{
		m_pHandleDepthEventWnd->SendMessage(WM_SYS_DEPTHSLIDER_HIDE);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgUSDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bEnableMouseMove=FALSE;
	
	if(m_bWinWidthLevelUpdate)
	{
		if(GetParent())
		{
			GetParent()->PostMessage(WM_SYS_UPDATE_WL,(WPARAM)&m_imageWidth,(LPARAM)&m_imageLevel);
		}
	}
	m_bWinWidthLevelUpdate=FALSE;

	int nSeleted = -1;
	nSeleted = m_Scene.SelectVirtualTargetByClicking(&m_lstPrjVTargets, point);
	if(nSeleted>=0)
	{
		int iCount=0;
		double dPos[3];
		CVirtualTarget *pTarget;
		int iRealIndex=m_lstPrjVTargets.GetAt(m_lstPrjVTargets.FindIndex(nSeleted))->GetIndex();
		POSITION pos=m_lstTarget.GetHeadPosition();
		while(pos)
		{
			pTarget=m_lstTarget.GetNext(pos);
			if(pTarget->GetIndex()==iRealIndex)
			{				
				dPos[0]=pTarget->GetPos().x;
				dPos[1]=pTarget->GetPos().y;
				dPos[2]=pTarget->GetPos().z;

				m_iSelectedTargetIndex=iCount;
				GetParent()->SendMessage(WM_SYS_UPDATE_TARGET,(WPARAM)&m_iSelectedTargetIndex,(LPARAM)dPos);
			}
			iCount++;
		}

		
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgUSDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	if(point.x>2 && point.x<100 && point.y>rc.Height()-30 && m_pHandleDepthEventWnd)
	{
		m_depthSlider.ShowWindow(TRUE);
		m_bIsSliding=TRUE;
		if(m_iSlideMode==0)
		{
			m_pHandleDepthEventWnd->SendMessage(WM_SYS_DEPTHSLIDER_SHOW);
		}
	}

	if (!m_bEnableMouseMove)
		return;
		 
	if( (nFlags & MK_LBUTTON) )  // 改变窗宽窗位 ningsj 2009.2.12
	{		
		if(m_ptWindowLevelStart.x!=point.x || m_ptWindowLevelStart.y!=point.y)
		{
			m_imageWidth += 5 * (point.y - m_ptWindowLevelStart.y);
			m_imageLevel += 5 * (point.x - m_ptWindowLevelStart.x);
			SetWL(m_imageWidth,m_imageLevel);
		}
		m_ptWindowLevelStart = point;
		m_bWinWidthLevelUpdate=TRUE;
	}
	
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgUSDisplay::SetSelectedTarget(int iSelected)
{
	m_iSelectedTargetIndex=iSelected;
}

void CDlgUSDisplay::CheckSelectedTarget()
{
	if(m_iSelectedTargetIndex<0 ||
		m_iSelectedTargetIndex>=m_lstTarget.GetCount())
	{
		m_iSelectedTargetIndex=0;
	}

	if(m_lstTarget.GetCount()<1)
		return;

	double dPos[3];
	CVirtualTarget *pTarget=m_lstTarget.GetAt(m_lstTarget.FindIndex(m_iSelectedTargetIndex));
	dPos[0]=pTarget->GetPos().x;
	dPos[1]=pTarget->GetPos().y;
	dPos[2]=pTarget->GetPos().z;
	GetParent()->SendMessage(WM_SYS_UPDATE_TARGET,(WPARAM)&m_iSelectedTargetIndex,(LPARAM)dPos);
}

void CDlgUSDisplay::EnableTrackingFlag(BOOL bTracking)
{
	m_bTrackingFlag=bTracking;
}

void CDlgUSDisplay::EnableSlider(CWnd *pWnd,int mode)
{
	
	m_pHandleDepthEventWnd=pWnd;

	m_depthSlider.SetRange(0,100);
	m_depthSlider.SetPageSize(1);
	

	m_iSlideMode=mode;
	if(mode==0)
	{
		//Init depth text
		m_txtDepth.sText = "Look Ahead: 0 mm";
		m_depthSlider.SetPos(0);
	}else
	{
		m_txtDepth.sText = "Angle: 0 deg";
		m_depthSlider.SetPos(50);
	}

	m_txtDepth.cfColor.SetColor(1.0, 1.0, 1.0, 1.0);
	m_txtDepth.bShow = TRUE;	
	
	m_txtDepth.pFont = CGlobalFuncs::GetFont();
	m_txtDepth.bShow = TRUE;
	
	m_Scene.Add2DText(&m_txtDepth);
	
}

void CDlgUSDisplay::OnReleasedcaptureSliderDepth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	if(m_iSlideMode==1)
	{
		int i=0;
		m_depthSlider.SetPos(50);
		m_pHandleDepthEventWnd->SendMessage(WM_SYS_UPDATE_ANGLE,WPARAM(&i));
	}
	
	*pResult = 0;
}

void CDlgUSDisplay::OnCustomdrawSliderDepth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	

	int i=m_depthSlider.GetPos();
	if(m_iSlideMode==0)
	{
		m_txtDepth.sText.Format("Look Ahead: %i mm",i);
	}else
	{
		i=(int)(90.0*(i-50)/50.0);
		m_txtDepth.sText.Format("Angle: %i deg",i);
	}

	if(m_pHandleDepthEventWnd)
	{
		if(m_iSlideMode==0)
		{
			m_pHandleDepthEventWnd->SendMessage(WM_SYS_UPDATE_DEPTH,WPARAM(&i));
		}else
		{
			m_pHandleDepthEventWnd->SendMessage(WM_SYS_UPDATE_ANGLE,WPARAM(&i));
		}
	}
	*pResult = 0;
}

void CDlgUSDisplay::ReshapeScene()
{
	CRect rc;
	GetClientRect(&rc);
	m_Scene.Reshape(rc.Width(),rc.Height());
}

void CDlgUSDisplay::SetTitle(CString text)
{
	m_txtTitle.sText = text;
}

BOOL CDlgUSDisplay::IsSliding()
{
	return m_bIsSliding;
}

void CDlgUSDisplay::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetParent())
	{
		GetParent()->PostMessage(WM_SYS_CHILDWINDOW_LBTNUP);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}
