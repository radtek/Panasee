// DlgDisplayWrap.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgDisplayWrap.h"
#include "MRIRectangle.h"
#include "MRIRectSeries.h"
#include "CryotherapyDlg.h"
#include "DlgDispCompare.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayWrap dialog


CDlgDisplayWrap::CDlgDisplayWrap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDisplayWrap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDisplayWrap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bComparison = FALSE;
	m_iLinkIndex = 0;

}


void CDlgDisplayWrap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDisplayWrap)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDisplayWrap, CDialog)
	//{{AFX_MSG_MAP(CDlgDisplayWrap)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayWrap message handlers

BOOL CDlgDisplayWrap::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;

	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	return TRUE;
}

BOOL CDlgDisplayWrap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bShowCtrl = FALSE;
	
	m_pDispSeries = NULL;


	if (m_dlgDisplay.GetSafeHwnd() == NULL)
	{
		m_dlgDisplay.Create(IDD_DLG_DISPLAY, this);
	}
	m_dlgDisplay.m_pDisplayControl = &m_dlgDispControl;
	m_dlgDisplay.m_pDispWrap = this;

	
	m_dlgDispControl.m_pDlgDisplay = &m_dlgDisplay;
	m_dlgDispControl.m_pDisplayWrap = this;
	if (m_dlgDispControl.GetSafeHwnd() == NULL)
	{
		m_dlgDispControl.Create(IDD_DLG_DISPLAY_CONTROL, this);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDisplayWrap::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE */)//位置设置
{
	CDialog::MoveWindow(x,y, nWidth,nHeight,bRepaint);
	Layout();
}

void CDlgDisplayWrap::Layout()  //..布局设置
{

	if (m_dlgDispControl.GetSafeHwnd() && m_dlgDisplay.GetSafeHwnd())
	{
		CRect rs;
		CRect rsCtl;
		
//		GetWindowRect(rs); 
//		ScreenToClient(rs);
		GetClientRect(rs);
		int height = 35;
	
		m_dlgDispControl.GetWindowRect(rsCtl);
		height = rsCtl.Height();
		int iGap = 0;

		if (m_bShowCtrl)
		{
			m_dlgDisplay.MoveWindow(0, 0, rs.Width(), rs.Height()- height - iGap);
			m_dlgDispControl.MoveWindow(0, rs.Height()-height, rs.Width(), height);

//			m_dlgDispControl.MoveWindow(0, rs.Height() - rsCtl.Height(), rs.Width(), rsCtl.Height());
//			m_dlgDisplay.MoveWindow(0, 0, rs.Width(), rs.Height() - rsCtl.Height());
		}
		else
		{
			m_dlgDisplay.MoveWindow(0, 0, rs.Width(), rs.Height());
		}
		m_dlgDispControl.ShowWindow(m_bShowCtrl);
	}
}

void CDlgDisplayWrap::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (m_dlgDisplay.GetSafeHwnd())
	{
		m_dlgDisplay.ShowWindow(bShow);
	}
	if (m_dlgDispControl.GetSafeHwnd())
	{
		m_dlgDispControl.ShowWindow(bShow && m_bShowCtrl);
	}
}

void CDlgDisplayWrap::AddMRIRect(CMRIRectangle *pMRIRect, BOOL bClean /*= TRUE*/)
{
	if (pMRIRect == NULL)
		return;

	//m_dlgDisplay.AddMRIRectWithLabel(pMRIRect);
	m_pDispSeries = pMRIRect->m_pMRIRectSeries;
	m_dlgDispControl.SetMRIRectSeries(m_pDispSeries, pMRIRect->m_iIndex);
}

void CDlgDisplayWrap::AddMRIRectSeries(MRIRectSeries *pMRIRectSeries, int iInitShowImgIndex /*= 0*/)
{
	if (pMRIRectSeries == NULL)
		return;
	
	if (!m_bShowCtrl)
	{
		CConfigMgr conf;
		m_bShowCtrl = conf.Read(CONF_SET_DISPLAY, CONF_SET_DISPLAY_SHOW_CONTROL, 1);
		Layout();
	}

	m_pDispSeries = pMRIRectSeries;
	m_dlgDispControl.SetMRIRectSeries(m_pDispSeries, iInitShowImgIndex);
}

void CDlgDisplayWrap::SetWindowWidthAndLevel(CPoint prePoint, CPoint curPoint,BOOL bUpdateAllImage)
{
	if (m_pDispSeries)
	{
		int nShiftOfWindowWidth, nShiftOfWindowLevel;
		nShiftOfWindowWidth = 5 * (curPoint.y - prePoint.y);
		nShiftOfWindowLevel = 5 * (curPoint.x - prePoint.x);
		
		if (m_pDispSeries)
		{
			m_pDispSeries->GetMRISeries()->ChangeWindowWidthAndLevel(nShiftOfWindowWidth, nShiftOfWindowLevel);
			if (m_bComparison)
			{
				m_dlgDisplay.m_pDlgDispCompare->UpdateTextureSameSeries(m_pDispSeries,bUpdateAllImage);
			}
			else 
			{
				
				m_dlgDisplay.m_pCryotherapyDlg->UpdateTextureSameSeries(m_pDispSeries,bUpdateAllImage);
			}
		}
	}
}

void CDlgDisplayWrap::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	Layout();
	
}

void CDlgDisplayWrap::SetBind()
{
	m_iLinkIndex = m_dlgDispControl.m_iCurDispIndex;
}

