// Dispaly4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "Dispaly4Dlg.h"
#include "MRISeries.h"
#include "MRIImage.h"
#include "Sphere.h"
#include "Probe.h"
#include "MRIRectangle.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispaly4Dlg dialog

//IMPLEMENT_DYNCREATE(CDispaly4Dlg, CDialog)



CDispaly4Dlg::CDispaly4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDispaly4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDispaly4Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//m_dispMode=DM_22_3D;//默认显示模式是：  3D 2D
						//					2D 2D	
	m_DlgDispWrap1.m_dlgDisplay.m_b3DDisplay = TRUE;
	m_DlgDispWrap1.m_dlgDisplay.m_Scene.m_b3DDisplay = TRUE;
	m_DlgDispWrap1.m_dlgDisplay.m_Scene.ShowSphereInVT(FALSE);
}

CDispaly4Dlg::~CDispaly4Dlg()
{
}

void CDispaly4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispaly4Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispaly4Dlg, CDialog)
	//{{AFX_MSG_MAP(CDispaly4Dlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispaly4Dlg message handlers

BOOL CDispaly4Dlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	return TRUE;

	
}

BOOL CDispaly4Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_DlgDispWrap1.GetSafeHwnd() == NULL)
	{
		m_DlgDispWrap1.Create(IDD_DLG_DISPLAY_WRAP, this);
	}
	if (m_DlgDispWrap2.GetSafeHwnd() == NULL)
	{
		m_DlgDispWrap2.Create(IDD_DLG_DISPLAY_WRAP, this);
	}
	if (m_DlgDispWrap3.GetSafeHwnd() == NULL)
	{
		m_DlgDispWrap3.Create(IDD_DLG_DISPLAY_WRAP, this);
	}
	if (m_DlgDispWrap4.GetSafeHwnd() == NULL)
	{
		m_DlgDispWrap4.Create(IDD_DLG_DISPLAY_WRAP, this);
	}
	if(m_DlgDispWrap5.GetSafeHwnd()==NULL)
	{
		m_DlgDispWrap5.Create(IDD_DLG_DISPLAY_WRAP, this);
	}



	m_DlgDispWrap1.m_bShowCtrl= FALSE;

	CConfigMgr conf;
	m_dispMode = (DISPLAY_MODE)conf.Read(CONF_SET_DISPLAY, CONF_SET_DISPLAY_MODE, (int)DM_22_3D);
	m_lastDispMode=(DISPLAY_MODE)conf.Read(CONF_SET_DISPLAY, CONF_SET_DISPLAY_LAST_MODE, (int)DM_22_3D);
	UINT uCheckedID;
	if(m_dispMode==DM_1||m_dispMode==DM_2||m_dispMode==DM_3||m_dispMode==DM_4||m_dispMode==DM_5)
	{
	//	if(m_lastDispMode)
		if(m_lastDispMode==DM_22_3D)
			uCheckedID=ID_VIEW_THREE2D_ONE3D_LEFT_TOP;
		else if (m_lastDispMode==DM_22_2D) 	
			uCheckedID =ID_VIEW_FRAME_FOUR_2D;
		else if(m_lastDispMode==DM_13)
			uCheckedID =ID_VIEW_THREE2D_ONE3D_TOP;
		else 
			uCheckedID=DM_22_3D;//默认
		m_dispMode=m_lastDispMode;
	}
	else if(m_dispMode==DM_22_3D)
		uCheckedID =ID_VIEW_THREE2D_ONE3D_LEFT_TOP;
	else if (m_dispMode==DM_22_2D) 	
		uCheckedID=ID_VIEW_FRAME_FOUR_2D;
	else if(m_dispMode==DM_13)
		uCheckedID=ID_VIEW_THREE2D_ONE3D_TOP;	
	
//	BOOL bShowCtl = conf.Read(CONF_SET_DISPLAY, CONF_SET_DISPLAY_SHOW_CONTROL, 1);
//	ShowControlWin(bShowCtl);

	Layout();

	UpdatePopupMenuDispMode(uCheckedID);

	
	//test
	/*	
	CMesh* m_pMesh=new CMesh();
	
	//head fse
	//double dT[3]={-120,-57.5,-129.6};
	//double dR[3]={-3.1415/2,-3.1415/2,0};

	//head axi
	//double dT[3]={-110,-47.5,-110.6};
	//double dR[3]={-3.1415/2,-3.1415/2,0};

	//head mrv
	//double dT[3]={-100,80.4,-105.6};
	//double dR[3]={3.1415/2,0,0};

	//head cali
	double dT[3]={-128,129.4,16};
	double dR[3]={-3.1415,0,0};
		
	if(m_pMesh->Load("D:\\0.obj",dT,dR))
	{
		//m_pMesh->FlipNormals();
		m_pMesh->SetUsingMaterial(true);
		m_pMesh->SetMaterial(METERIAL_TURQUOISE);
		m_pMesh->m_color[0]=1;
		m_pMesh->m_color[1]=1;
		m_pMesh->m_color[2]=1;
		m_DlgDispWrap1.m_dlgDisplay.m_Scene.AddModelMesh(m_pMesh);
	}*/
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDispaly4Dlg::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)//..位置设置
{
	Layout();
	CDialog::MoveWindow(x,y, nWidth,nHeight,bRepaint);
}

void CDispaly4Dlg::Layout()//..布局设置
{
	CRect rs;
	GetClientRect(rs);

	int iGap = 1;
	
	if(!m_DlgDispWrap1.GetSafeHwnd()||!m_DlgDispWrap2.GetSafeHwnd()||!m_DlgDispWrap3.GetSafeHwnd()||!m_DlgDispWrap4.GetSafeHwnd()||!m_DlgDispWrap1.GetSafeHwnd())
		return;

	m_DlgDispWrap1.ShowWindow(SW_HIDE);
	m_DlgDispWrap2.ShowWindow(SW_HIDE);
	m_DlgDispWrap3.ShowWindow(SW_HIDE);
	m_DlgDispWrap4.ShowWindow(SW_HIDE);
	m_DlgDispWrap5.ShowWindow(SW_HIDE);


	m_DlgDispWrap1.m_dlgDisplay.m_Scene.ShowBoxFrame(TRUE);
	
	if (m_dispMode == DM_1)//3D
	{
		m_DlgDispWrap1.MoveWindow(0, 0, rs.Width(), rs.Height());
		UpdatePopupMenuDispMode(ID_VIEW_ONE3D);
	}
	else if (m_dispMode == DM_2)//2D
	{
		m_DlgDispWrap2.MoveWindow(0, 0, rs.Width(), rs.Height());
	}
	else if (m_dispMode == DM_3)//2D
	{
		m_DlgDispWrap3.MoveWindow(0, 0, rs.Width(), rs.Height());
	}
	else if (m_dispMode == DM_4) //2D
	{
		m_DlgDispWrap4.MoveWindow(0, 0, rs.Width(), rs.Height());
	}
	else if(m_dispMode==DM_5)//2D
	{
		m_DlgDispWrap5.MoveWindow(0, 0, rs.Width(), rs.Height());
	}
	else if (m_dispMode == DM_13)	//    3D
	{								// 2D 2D 2D
		int iSmallWidth = (rs.Width() - iGap * 2) / 3;
		int iSmallHeight = iSmallWidth * 1.2;
		int iLargeWidth = rs.Width();
		int iLargetHeight = rs.Height() - iSmallHeight - iGap;
		int iSmallTop = rs.Height() - iSmallHeight;
		m_DlgDispWrap1.MoveWindow(0, 0, iLargeWidth, iLargetHeight);
		m_DlgDispWrap2.MoveWindow(0, iSmallTop, iSmallWidth, iSmallHeight);
		m_DlgDispWrap3.MoveWindow(iSmallWidth + iGap, iSmallTop, iSmallWidth, iSmallHeight);
		m_DlgDispWrap4.MoveWindow(iSmallWidth * 2 + iGap * 2, iSmallTop, rs.Width() - (iSmallWidth * 2 + iGap * 2), iSmallHeight);

		UpdatePopupMenuDispMode(ID_VIEW_THREE2D_ONE3D_TOP);
	}
	else if(m_dispMode==DM_22_2D)   // 2D 2D
	{								// 2D 2D
		m_DlgDispWrap2.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height()/2-iGap);
		m_DlgDispWrap3.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height()/2-iGap);
		m_DlgDispWrap4.MoveWindow(rs.left, rs.top+rs.Height()/2, rs.Width()/2-iGap, rs.Height() - (rs.top+rs.Height()/2));
		m_DlgDispWrap5.MoveWindow(rs.left+rs.Width()/2, rs.top+rs.Height()/2, rs.Width() - (rs.left+rs.Width()/2), rs.Height() - (rs.top+rs.Height()/2));
		UpdatePopupMenuDispMode(ID_VIEW_FRAME_FOUR_2D);

	}
	else if (m_dispMode == DM_22_3D)// 3D 2D
	{								// 2D 2D
		m_DlgDispWrap1.MoveWindow(rs.left, rs.top, rs.Width()/2-iGap, rs.Height()/2-iGap);
		m_DlgDispWrap2.MoveWindow(rs.left+rs.Width()/2, rs.top, rs.Width() - (rs.left+rs.Width()/2), rs.Height()/2-iGap);
		m_DlgDispWrap3.MoveWindow(rs.left, rs.top+rs.Height()/2, rs.Width()/2-iGap, rs.Height() - (rs.top+rs.Height()/2));
		m_DlgDispWrap4.MoveWindow(rs.left+rs.Width()/2, rs.top+rs.Height()/2, rs.Width() - (rs.left+rs.Width()/2), rs.Height() - (rs.top+rs.Height()/2));
		UpdatePopupMenuDispMode(ID_VIEW_THREE2D_ONE3D_LEFT_TOP);
	}
	else if (m_dispMode == DM_TPS)
	{
		m_DlgDispWrap2.MoveWindow(rs.left+iGap, rs.top+iGap, rs.Width()/2-iGap*2, rs.Height()/2-iGap);
		m_DlgDispWrap3.MoveWindow(rs.left+rs.Width()/2, rs.top+iGap, rs.Width()/2-iGap*2, rs.Height()/2-iGap);
		m_DlgDispWrap4.MoveWindow(rs.left+iGap, rs.top+rs.Height()/2+iGap, rs.Width()/2-iGap*2, rs.Height()/2-iGap);

	}

	m_DlgDispWrap1.ShowWindow(m_dispMode == DM_1 || m_dispMode == DM_13 || m_dispMode == DM_22_3D);
	m_DlgDispWrap2.ShowWindow(m_dispMode == DM_2  || m_dispMode == DM_13 || m_dispMode == DM_22_2D || m_dispMode == DM_22_3D || m_dispMode == DM_TPS);
	m_DlgDispWrap3.ShowWindow(m_dispMode == DM_3  || m_dispMode == DM_13 || m_dispMode == DM_22_2D || m_dispMode == DM_22_3D || m_dispMode == DM_TPS);
	m_DlgDispWrap4.ShowWindow(m_dispMode == DM_4  || m_dispMode == DM_13 || m_dispMode == DM_22_2D || m_dispMode == DM_22_3D || m_dispMode == DM_TPS);
	m_DlgDispWrap5.ShowWindow(m_dispMode == DM_5  || m_dispMode == DM_22_2D);

}	


BOOL CDispaly4Dlg::ShowWindow(int nCmdShow)//..显示或隐藏窗体
{
	
	if(nCmdShow==SW_HIDE)
	{
		m_DlgDispWrap1.ShowWindow(nCmdShow);
		m_DlgDispWrap2.ShowWindow(nCmdShow);
		m_DlgDispWrap3.ShowWindow(nCmdShow);
		m_DlgDispWrap4.ShowWindow(nCmdShow);
		m_DlgDispWrap5.ShowWindow(nCmdShow);
	}
	else 
		Layout();
	return CDialog::ShowWindow(nCmdShow);
}

void CDispaly4Dlg::AddProbe(CProbe *pProbe)  //..添加针Probe
{
	m_DlgDispWrap1.m_dlgDisplay.AddProbe(pProbe);
	m_DlgDispWrap2.m_dlgDisplay.AddProbe(pProbe);
	m_DlgDispWrap3.m_dlgDisplay.AddProbe(pProbe);
	m_DlgDispWrap4.m_dlgDisplay.AddProbe(pProbe);
	m_DlgDispWrap5.m_dlgDisplay.AddProbe(pProbe);
}

void CDispaly4Dlg::RemoveProbe(CProbe *pProbe)
{
	
	m_DlgDispWrap1.m_dlgDisplay.RemoveProbe(pProbe);
	m_DlgDispWrap2.m_dlgDisplay.RemoveProbe(pProbe);
	m_DlgDispWrap3.m_dlgDisplay.RemoveProbe(pProbe);
	m_DlgDispWrap4.m_dlgDisplay.RemoveProbe(pProbe);
	m_DlgDispWrap5.m_dlgDisplay.RemoveProbe(pProbe);
//	m_pCryoDlg->m_pDisplay4Dlg->Refresh();
}

void CDispaly4Dlg::AddTo3DDisplay(CMRIRectangle *pMRIRectangle)//..设置3D显示
{
	if (!pMRIRectangle || pMRIRectangle->m_bIn3DWindows)
	{
		return;
	}

	m_DlgDispWrap1.m_dlgDisplay.AddMRIRect(pMRIRectangle, FALSE, TRUE);
	m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	pMRIRectangle->m_bIn3DWindows = TRUE;
	Show3DDisplay(pMRIRectangle, TRUE);
//	pMRIRectangle->m_bIsVisible3D = TRUE;
}

void CDispaly4Dlg::RemoveSeriesFrom3D(MRIRectSeries *pMRIRectSeries)
{
	int iNum = pMRIRectSeries->GetRectNum();
	if (iNum > 0)
	{
		pMRIRectSeries->GetMRISeries()->SetVisible(false);
	}
	
	CMRIRectangle *pMRIRect;
	for (int i=0; i<iNum; i++)
	{
		pMRIRect = pMRIRectSeries->GetMRIRectAt(i);
		if (pMRIRect->m_bIn3DWindows)
		{
			RemoveFrom3DDisplay(pMRIRect);
		}
	} 
}

void CDispaly4Dlg::RemoveFrom3DDisplay(CMRIRectangle *pMRIRectangle)//..去除3D显示
{
	if (!pMRIRectangle || !pMRIRectangle->m_bIn3DWindows)
	{
		return;
	}
	m_DlgDispWrap1.m_dlgDisplay.RemoveMRIRect(pMRIRectangle);
	pMRIRectangle->RemoveMRIRect(); /*ningsj 2009.3.9 清除对象，减少内存使用*/
	pMRIRectangle->m_bIn3DWindows = FALSE;
	pMRIRectangle->SetVisible3D(FALSE);
}

void CDispaly4Dlg::Show3DDisplay(CMRIRectangle *pMRIRectangle, BOOL bShow)//..3D显示
{
	if (!pMRIRectangle)
		return;
	pMRIRectangle->SetVisible3D(bShow);
	//pMRIRectangle->m_pRectangle->SetVisible(bShow);
}

void CDispaly4Dlg::SimulateCryotherapySphere(CProbe* pProbe, double dbRadius)
{
	if (pProbe->IsSimulating())
	{
		CSphere* pSphere = &(pProbe->m_tipSphere);
		pSphere->SetVisible(TRUE);
		pSphere->SetRadius(dbRadius);
	}
	else
	{
		pProbe->m_tipSphere.SetVisible(FALSE);
	}
	m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);	
	m_DlgDispWrap2.m_dlgDisplay.Invalidate(FALSE);	
	m_DlgDispWrap3.m_dlgDisplay.Invalidate(FALSE);	
	m_DlgDispWrap4.m_dlgDisplay.Invalidate(FALSE);
	m_DlgDispWrap5.m_dlgDisplay.Invalidate(FALSE);

}

void CDispaly4Dlg::Refresh()//.刷新窗口. 
{
	
	if(!IsWindowVisible())
		return;
	if(m_DlgDispWrap1.m_dlgDisplay.IsWindowVisible() && m_DlgDispWrap1.m_dlgDisplay.IsEnableDraw())
		m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene();
	if(m_DlgDispWrap2.m_dlgDisplay.IsWindowVisible() && m_DlgDispWrap2.m_dlgDisplay.IsEnableDraw())
		m_DlgDispWrap2.m_dlgDisplay.m_Scene.DrawScene();
	if(m_DlgDispWrap3.m_dlgDisplay.IsWindowVisible() && m_DlgDispWrap3.m_dlgDisplay.IsEnableDraw())
		m_DlgDispWrap3.m_dlgDisplay.m_Scene.DrawScene();
	if(m_DlgDispWrap4.m_dlgDisplay.IsWindowVisible() && m_DlgDispWrap4.m_dlgDisplay.IsEnableDraw())
		m_DlgDispWrap4.m_dlgDisplay.m_Scene.DrawScene();
	if(m_DlgDispWrap5.m_dlgDisplay.IsWindowVisible() && m_DlgDispWrap5.m_dlgDisplay.IsEnableDraw())
		m_DlgDispWrap5.m_dlgDisplay.m_Scene.DrawScene();
	
}

void CDispaly4Dlg::ShowControlWin(BOOL bShow)//..
{
	Layout();
}

void CDispaly4Dlg::MaxDispWindow(CDisplayDlg *pDispDlg, bool bMax)//..
{
	if (!bMax) //bMax=FALSE
	{
		SetDispMode(m_lastDispMode);
	}
	else if (pDispDlg==&(m_DlgDispWrap1.m_dlgDisplay))
	{
		SetDispMode(DM_1);
	}
	else if (pDispDlg == &(m_DlgDispWrap2.m_dlgDisplay))
	{
		SetDispMode(DM_2);
	}
	else if (pDispDlg == &(m_DlgDispWrap3.m_dlgDisplay))
	{
		SetDispMode(DM_3);
	}
	else if (pDispDlg == &(m_DlgDispWrap4.m_dlgDisplay))
	{
		SetDispMode(DM_4);
	}
	else if (pDispDlg == &(m_DlgDispWrap5.m_dlgDisplay))
	{
		SetDispMode(DM_5);
	}
}

void CDispaly4Dlg::SetDispMode(DISPLAY_MODE dispMode)//..
{
	if(m_dispMode==dispMode) return;//如果一样，则直接返回；

	m_lastDispMode = m_dispMode;
	m_dispMode = dispMode;

	Layout();
	
	if (dispMode != DM_TPS)
	{
		CConfigMgr conf;
		conf.Write(CONF_SET_DISPLAY, CONF_SET_DISPLAY_MODE, m_dispMode);
		conf.Write(CONF_SET_DISPLAY, CONF_SET_DISPLAY_LAST_MODE,m_lastDispMode);
	}
}

DISPLAY_MODE CDispaly4Dlg::GetDispMode()
{
	return m_dispMode;
}

CDlgDisplayWrap* CDispaly4Dlg::GetDispWrap(int iWndIndex)
{
	if(iWndIndex==0)
	{
		return &(m_DlgDispWrap1);
	}
	else if (iWndIndex == 1)
	{
		return &(m_DlgDispWrap2);
	}
	else if (iWndIndex == 2)
	{
		return &(m_DlgDispWrap3);
	}
	else if (iWndIndex == 3)
	{
		return &(m_DlgDispWrap4);
	}
	else if (iWndIndex == 4)
	{
		return &(m_DlgDispWrap5);
	}
	return NULL;
}

int CDispaly4Dlg::GetWndIndex(CDisplayDlg *pDispDlg)
{
	if (pDispDlg == &(m_DlgDispWrap1.m_dlgDisplay))
	{
		return 0;
	}
	else if (pDispDlg == &(m_DlgDispWrap2.m_dlgDisplay))
	{
		return 1;
	}
	else if (pDispDlg == &(m_DlgDispWrap3.m_dlgDisplay))
	{
		return 2;
	}
	else if (pDispDlg == &(m_DlgDispWrap4.m_dlgDisplay))
	{
		return 3;
	}
	else if (pDispDlg == &(m_DlgDispWrap5.m_dlgDisplay))
	{
		return 4;
	}
	return -1;
}

BOOL CDispaly4Dlg::SetSelectedWnd(CDisplayDlg *pDispDlg)
{
	if (pDispDlg != &(m_DlgDispWrap1.m_dlgDisplay))
	{
		m_DlgDispWrap2.m_dlgDisplay.SelectDisplayMRISeries(FALSE);
		m_DlgDispWrap3.m_dlgDisplay.SelectDisplayMRISeries(FALSE);
		m_DlgDispWrap4.m_dlgDisplay.SelectDisplayMRISeries(FALSE);
		m_DlgDispWrap5.m_dlgDisplay.SelectDisplayMRISeries(FALSE);
		m_pCryoDlg->m_pCurSelSeries = NULL;
		pDispDlg->SelectDisplayMRISeries(TRUE);
	}
	return TRUE;
}

void CDispaly4Dlg::AddMRIRectSeries(MRIRectSeries *pMRIRectSeries, int iWndIndex, int iInitDispImgIndex /*= 0*/)//..添加MRI 序列Series
{
	if (pMRIRectSeries == NULL || iWndIndex < 1 || iWndIndex > 5)
		return;

	CDlgDisplayWrap *pDispWrap = GetDispWrap(iWndIndex);
	pDispWrap->AddMRIRectSeries(pMRIRectSeries, iInitDispImgIndex);
	m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
}

void CDispaly4Dlg::EnableLR(BOOL bEnable)
{
	m_DlgDispWrap1.m_dlgDisplay.EnableLRArrow(bEnable);
	m_DlgDispWrap2.m_dlgDisplay.EnableLRArrow(bEnable);
	m_DlgDispWrap3.m_dlgDisplay.EnableLRArrow(bEnable);
	m_DlgDispWrap4.m_dlgDisplay.EnableLRArrow(bEnable);
	m_DlgDispWrap5.m_dlgDisplay.EnableLRArrow(bEnable);
}

void CDispaly4Dlg::EnableDistMeasure(BOOL bEnable)
{
	m_DlgDispWrap1.m_dlgDisplay.Enable3DDistanceMeasure(bEnable);
	m_DlgDispWrap2.m_dlgDisplay.EnableDistanceMeasure(bEnable);
	m_DlgDispWrap3.m_dlgDisplay.EnableDistanceMeasure(bEnable);
	m_DlgDispWrap4.m_dlgDisplay.EnableDistanceMeasure(bEnable);
	m_DlgDispWrap5.m_dlgDisplay.EnableDistanceMeasure(bEnable);

	m_DlgDispWrap1.m_dlgDisplay.m_nPointNum = 0;
	m_DlgDispWrap2.m_dlgDisplay.m_nPointNum = 0;
	m_DlgDispWrap3.m_dlgDisplay.m_nPointNum = 0;
	m_DlgDispWrap4.m_dlgDisplay.m_nPointNum = 0;
	m_DlgDispWrap5.m_dlgDisplay.m_nPointNum = 0;

	m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene();
	m_DlgDispWrap2.m_dlgDisplay.m_Scene.DrawScene();
	m_DlgDispWrap3.m_dlgDisplay.m_Scene.DrawScene();
	m_DlgDispWrap4.m_dlgDisplay.m_Scene.DrawScene();
	m_DlgDispWrap5.m_dlgDisplay.m_Scene.DrawScene();


}
BOOL CDispaly4Dlg::UpdatePopupMenuDispMode(UINT menuItemID)// 更新所有窗口的显示方式菜单的状态
{
	CDlgDisplayWrap *pDispWrap;
	for(int i=0;i<5;i++)
	{
		pDispWrap=GetDispWrap(i);
		if(menuItemID==ID_VIEW_FRAME_FOUR_2D)
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_FRAME_FOUR_2D,MF_CHECKED);
		else
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_FRAME_FOUR_2D,MF_UNCHECKED);
		if(menuItemID==ID_VIEW_THREE2D_ONE3D_LEFT_TOP)
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_THREE2D_ONE3D_LEFT_TOP,MF_CHECKED);
		else
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_THREE2D_ONE3D_LEFT_TOP,MF_UNCHECKED);
		if(menuItemID==ID_VIEW_THREE2D_ONE3D_TOP)
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_THREE2D_ONE3D_TOP,MF_CHECKED);
		else
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_THREE2D_ONE3D_TOP,MF_UNCHECKED);
		if(menuItemID==ID_VIEW_ONE3D)
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_ONE3D,MF_CHECKED);
		else
			pDispWrap->m_dlgDisplay.m_popupmenu.CheckMenuItem(ID_VIEW_ONE3D,MF_UNCHECKED);								
	}
	return TRUE;
}

void CDispaly4Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	Layout();
}


void CDispaly4Dlg::SetDispWndCursor(LPCTSTR lpszCursorName)
{
	m_DlgDispWrap1.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadStandardCursor(lpszCursorName);
	m_DlgDispWrap2.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadStandardCursor(lpszCursorName);
	m_DlgDispWrap3.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadStandardCursor(lpszCursorName);
	m_DlgDispWrap4.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadStandardCursor(lpszCursorName);
	m_DlgDispWrap5.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadStandardCursor(lpszCursorName);

	if (m_DlgDispWrap1.m_dlgDisplay.m_hCursor == NULL)
	{
		m_DlgDispWrap1.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadCursor(lpszCursorName);
		m_DlgDispWrap2.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadCursor(lpszCursorName);
		m_DlgDispWrap3.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadCursor(lpszCursorName);
		m_DlgDispWrap4.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadCursor(lpszCursorName);
		m_DlgDispWrap5.m_dlgDisplay.m_hCursor = AfxGetApp()->LoadCursor(lpszCursorName);


	}

	m_DlgDispWrap1.m_dlgDisplay.PostMessage(WM_SETCURSOR);
	m_DlgDispWrap2.m_dlgDisplay.PostMessage(WM_SETCURSOR);
	m_DlgDispWrap3.m_dlgDisplay.PostMessage(WM_SETCURSOR);
	m_DlgDispWrap4.m_dlgDisplay.PostMessage(WM_SETCURSOR);
	m_DlgDispWrap5.m_dlgDisplay.PostMessage(WM_SETCURSOR);

}

void CDispaly4Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC *pDC=&dc;

	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	

	COLORREF bkColor=RGB(232,244,244);
	int iGap=1;

	if(m_dispMode==DM_22_2D || m_dispMode == DM_22_3D)
	{								
		CPen newPen(PS_SOLID,5,bkColor);
		CPen *pOldPen=pDC->SelectObject(&newPen);

		int x=rs.Width()/2;
		int y=rs.Height()/2;
		pDC->MoveTo(x,0);
		pDC->LineTo(x,rs.Height());
		pDC->MoveTo(0,y);
		pDC->LineTo(rs.Width(),y);

		pDC->SelectObject(pOldPen);
		
	}else if(m_dispMode == DM_13)
	{
		CPen newPen(PS_SOLID,5,bkColor);
		CPen *pOldPen=pDC->SelectObject(&newPen);

		int iSmallWidth = (rs.Width() - iGap * 2) / 3;
		int iSmallHeight = iSmallWidth * 1.2;
		int x=iSmallWidth;
		int y=rs.Height()-iSmallHeight;
		pDC->MoveTo(x,y);
		pDC->LineTo(x,rs.Height());
		x+=iSmallWidth+iGap;
		pDC->MoveTo(x,y);
		pDC->LineTo(x,rs.Height());
		
		pDC->MoveTo(0,y);
		pDC->LineTo(rs.Width(),y);

		pDC->SelectObject(pOldPen);
	}
	// Do not call CDialog::OnPaint() for painting messages
}
