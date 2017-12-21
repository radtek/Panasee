// DlgFusion.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgFusion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFusion dialog


CDlgFusion::CDlgFusion(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgFusion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFusion)
	//}}AFX_DATA_INIT
	m_bInitialized=FALSE;
	m_fusionMode=FUSION_AXIAL;
}


void CDlgFusion::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFusion)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFusion, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgFusion)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SYS_CHILDWINDOW_LBTNUP,OnFusionWndClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFusion message handlers

LONG CDlgFusion::OnFusionWndClicked(WPARAM w,LPARAM l)
{
	
	if(m_fusionMode==FUSION_AXIAL)
	{
		m_fusionMode=FUSION_SAGITAL;
		m_dlgFusion.SetTitle("Sagital");
	}else if(m_fusionMode==FUSION_SAGITAL)
	{
		m_fusionMode=FUSION_VIRTUALUS;
		m_dlgFusion.SetTitle("Virtual US");
	}else if(m_fusionMode==FUSION_VIRTUALUS)
	{
		m_fusionMode=FUSION_PROBEEYE;
		m_dlgFusion.SetTitle("Probe Eye");
	}else if(m_fusionMode==FUSION_PROBEEYE)
	{
		m_fusionMode=FUSION_AXIAL;
		m_dlgFusion.SetTitle("Axial");
	}
	return 0;
}

BOOL CDlgFusion::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;    

	return CPanaseeDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgFusion::OnInitDialog() 
{
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_dlgFusion.GetSafeHwnd() == NULL)
	{
		m_dlgFusion.Create(CDlgUSDisplay::IDD,this);
	}
	m_dlgFusion.ShowWindow(TRUE);
	m_dlgFusion.SetTitle("Axial");

	m_bInitialized=TRUE;

	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	int width=400;
	int height=400;
	MoveWindow((x-width)/2,(y-height)/2,width,height);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFusion::OnSize(UINT nType, int cx, int cy) 
{
	CPanaseeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_bInitialized)
		return;
	m_dlgFusion.MoveWindow(1,1,cx-2,cy-2);
}

void CDlgFusion::SetProbe(CProbe *pProbe)
{
	m_dlgFusion.SetProbe(pProbe);
}

void CDlgFusion::RemoveProbe()
{
	m_dlgFusion.RemoveProbe();
}

void CDlgFusion::SetImage(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation)
{
	if(!this->IsWindowVisible())
		return;
	m_dlgFusion.SetUSImage(pUSImgData,pAPSI,ppcOrientation);
	m_dlgFusion.Redraw();
}

void CDlgFusion::SetTitle(CString sTitle)
{
	m_dlgFusion.SetTitle(sTitle);
}


