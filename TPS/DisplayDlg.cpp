// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayDlg.h"
#include "Commands.h"
#include "vImage1.h"
#include "mv.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)
/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg dialog


CDisplayDlg::CDisplayDlg(CWnd* pParent /*=NULL*/)
	: CAppDialog(CDisplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_2DView1=NULL;
	m_2DView2=NULL;
	m_2DView3=NULL;
	
	m_layoutStyle=CMD_TPS_DISPLAY_AVERAGE;
	m_layoutPara=1;

	m_bDisplayMPR=FALSE;

	m_sMPRProbe="";
	m_pMPRImageMatrix=NULL;

	m_iMPRImageWindow=-1;
	m_bRealProbePosChanged=FALSE;
	
}

CDisplayDlg::~CDisplayDlg()
{
	
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CAppDialog)
	//{{AFX_MSG_MAP(CDisplayDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg message handlers

BOOL CDisplayDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,RGB(232,244,244));
 	return TRUE;
}

BOOL CDisplayDlg::OnInitDialog() 
{
	CAppDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CGlobalManager::Manager()->ReadProbeInfo();

	m_2DView1=new CTPS2DView(NULL);
	if(m_2DView1->m_hWnd == NULL)
	{
		//Create its window if not yet
		m_2DView1->Create(IDD_DIALOG_2DVIEW,this);
	}

	if(m_2DView1->m_hWnd && this->m_hWnd)
	{
		m_2DView1->SetParent(this);
		//m_2DView1->ShowWindow(SW_SHOW);
	}

	m_2DView2=new CTPS2DView(NULL);
	if(m_2DView2->m_hWnd == NULL)
	{
		//Create its window if not yet
		m_2DView2->Create(IDD_DIALOG_2DVIEW,this);
	}

	if(m_2DView2->m_hWnd && this->m_hWnd)
	{
		m_2DView2->SetParent(this);
		//m_2DView2->ShowWindow(SW_SHOW);
	}
	
	
	m_2DView3=new CTPS2DView(NULL);
	if(m_2DView3->m_hWnd == NULL)
	{
		//Create its window if not yet
		m_2DView3->Create(IDD_DIALOG_2DVIEW,this);
	}

	if(m_2DView3->m_hWnd && this->m_hWnd)
	{
		m_2DView3->SetParent(this);
		//m_2DView3->ShowWindow(SW_SHOW);
	}

		
	
	m_3DView=new CTPS3DView(NULL);
	if(m_3DView->m_hWnd == NULL)
	{
		//Create its window if not yet
		m_3DView->Create(IDD_DIALOG_3DVIEW,this);
	}

	if(m_3DView->m_hWnd && this->m_hWnd)
	{
		m_3DView->SetParent(this);
		//m_3DView->ShowWindow(SW_SHOW);
	}

	CGlobalManager::Manager()->m_pFrmMgr=GetFrameMgr();
	CGlobalManager::Manager()->m_2DView1=m_2DView1;
	CGlobalManager::Manager()->m_2DView2=m_2DView2;
	CGlobalManager::Manager()->m_2DView3=m_2DView3;
	CGlobalManager::Manager()->m_3DView=m_3DView;

	m_3DView->SetFrmMgr(GetFrameMgr());
	m_2DView1->SetFrmMgr(GetFrameMgr());
	m_2DView2->SetFrmMgr(GetFrameMgr());
	m_2DView3->SetFrmMgr(GetFrameMgr());

	m_2DView1->m_instanceIndex=1;
	m_2DView2->m_instanceIndex=2;
	m_2DView3->m_instanceIndex=3;
	m_3DView->m_instanceIndex=4;
	
	m_pMPRView1=new CMPRView(this);
	if(m_pMPRView1->m_hWnd==NULL)
	{
		m_pMPRView1->Create(IDD_DIALOG_MPR,this);
	}
	if(m_pMPRView1->m_hWnd && this->m_hWnd)
	{
		m_pMPRView1->SetParent(this);
		m_pMPRView1->ShowWindow(m_bDisplayMPR);
	}
	m_pMPRView1->SetFrmMgr(GetFrameMgr());

	m_pMPRView2=new CMPRView(this);
	if(m_pMPRView2->m_hWnd==NULL)
	{
		m_pMPRView2->Create(IDD_DIALOG_MPR,this);
	}
	if(m_pMPRView2->m_hWnd && this->m_hWnd)
	{
		m_pMPRView2->SetParent(this);
		m_pMPRView2->ShowWindow(m_bDisplayMPR);
	}
	m_pMPRView2->SetFrmMgr(GetFrameMgr());
	
	m_pMPRView1->m_instanceIndex=1;
	m_pMPRView2->m_instanceIndex=2;

	SetTimer(INDICATOR_TPS_PROBE,MY_ELAPSE100,NULL);
	SetTimer(INDICATOR_LOAD_TPS,MY_ELAPSE200,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDisplayDlg::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == this||m_2DView1==NULL)
		return;


	switch(nCmdID)
	{
	case CMD_TPS_DELETE_TRACE:
	case CMD_TPS_CREATE_TRACE:
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
	case CMD_TPS_RENDERREFRESHALL:
		m_pMPRView1->DispatchCommand(pSender,nCmdID,sPara);
		m_pMPRView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView1->Invalidate(FALSE);
		CGlobalManager::Manager()->m_2DView2->Invalidate(FALSE);		
		CGlobalManager::Manager()->m_2DView3->Invalidate(FALSE);
		CGlobalManager::Manager()->m_3DView->Invalidate(FALSE);
		CGlobalManager::Manager()->m_3DView->UpdateWindow();
		break;
	case CMD_TPS_SELECTPROBE:
		CGlobalManager::Manager()->DispatchCommand(pSender,nCmdID,sPara);
		m_pMPRView1->Invalidate(FALSE);
		m_pMPRView2->Invalidate(FALSE);
		break;
	case CMD_TPS_DELETEPROBE:
		DeleteProbe(sPara);
		CGlobalManager::Manager()->DeleteProbe(sPara);
		break;	
	case CMD_TPS_MPR_SHOWSTATUS:
		if(m_bDisplayMPR)
		{
			sPara="1";
		}else
		{
			sPara="0";
		}
		break;
	case CMD_TPS_MPR_PROBENAME:
		sPara=m_sMPRProbe;
		break;
	case CMD_TPS_MPR_IMAGE:
		SetMPRImage(sPara);
		break;
	case CMD_TPS_MPR_PROBE:
		SetMPRProbe(sPara);
		break;
	case CMD_TPS_PROBE_MOVING:
		SetMPRProbePosition();
		break;
	case CMD_TPS_MPR:
		m_bDisplayMPR=!m_bDisplayMPR;
		Layout(m_layoutStyle,m_layoutPara);
		SetMPRProbePosition();
		if(!m_bDisplayMPR)
		{
			//CGlobalManager::Manager()->m_3DView->m_pImageActorMPR1->SetVisibility(0);
			//CGlobalManager::Manager()->m_3DView->m_pImageActorMPR2->SetVisibility(0);
		}else
		{
			//CGlobalManager::Manager()->m_3DView->m_pImageActorMPR1->SetVisibility(1);
			//CGlobalManager::Manager()->m_3DView->m_pImageActorMPR2->SetVisibility(1);
			//CGlobalManager::Manager()->m_3DView->Invalidate(FALSE);
		}
		break;
	case CMD_TPS_MPR_HIDE:
		m_bDisplayMPR=FALSE;
		Layout(m_layoutStyle,m_layoutPara);
				
		break;
	case CMD_TPS_EXTRACT_CONTOUR:
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
	case CMD_TPS_CHANGE_AUTOTRACE_STATUS:
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
	case CMD_TPS_SETUP_TRACE_FILTER:
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
		
	case CMD_TPS_DISPLAY_AVERAGE:
		Layout(CMD_TPS_DISPLAY_AVERAGE,atoi(sPara));
		break;
	case CMD_TPS_DISPLAY_2DTOP:
		Layout(CMD_TPS_DISPLAY_2DTOP,atoi(sPara));
		break;
	case CMD_TPS_DISPLAY_MAX:
		Layout(CMD_TPS_DISPLAY_MAX,atoi(sPara));
		break;
	
	case CMD_TPS_REMOVEALL:
		m_pMPRView1->DispatchCommand(pSender,nCmdID,sPara);
		m_pMPRView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_3DView->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
	case CMD_TPS_RENDERREFRESH:
		CGlobalManager::Manager()->m_2DView1->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_3DView->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView2->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->m_2DView3->DispatchCommand(pSender,nCmdID,sPara);
		break;
	default:
		CGlobalManager::Manager()->m_3DView->DispatchCommand(pSender,nCmdID,sPara);
		CGlobalManager::Manager()->DispatchCommand(pSender,nCmdID,sPara);
		break;
	}
}


void CDisplayDlg::OnSize(UINT nType, int cx, int cy) 
{
	CAppDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Layout(m_layoutStyle,m_layoutPara);
	
}

void CDisplayDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == INDICATOR_TPS_PROBE)
	{
		UpdateRealProbe();
	}
	if(nIDEvent == INDICATOR_LOAD_TPS)
	{
		if(CGlobalManager::Manager()->m_sTpsFileLoad!="")
		{
			CString sFile=CGlobalManager::Manager()->m_sTpsFileLoad;
			CGlobalManager::Manager()->m_sTpsFileLoad="";
			if(CGlobalManager::Manager()->LoadTPSInfoFile(sFile))
			{
				CGlobalManager::Manager()->m_sTpsFileLoaded=sFile;
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}



void CDisplayDlg::Layout(CMD_TPS_COMMAND command,int para)
{
	m_layoutStyle=command;
	m_layoutPara=para;

	CRect rs;
	GetClientRect(rs);
	
	int iSmallWidth;
	int iSmallHeight;
	int iLargeWidth;
	int iLargetHeight;
	int iSmallTop ;

	int iGap = 1;
	
	if (!m_2DView1->GetSafeHwnd() || !m_2DView2->GetSafeHwnd() 
		|| !m_2DView3->GetSafeHwnd()||!m_3DView->GetSafeHwnd())
	{
		return;
	}

	if(!m_bDisplayMPR)
	{
		

		m_2DView1->ShowWindow(SW_SHOW);
		m_2DView2->ShowWindow(SW_SHOW);
		m_2DView3->ShowWindow(SW_SHOW);
		m_3DView->ShowWindow(SW_SHOW);
		
		switch(m_layoutStyle)
		{
		case CMD_TPS_DISPLAY_2DTOP:
			iSmallWidth = (rs.Width() - iGap * 2) / 3;
			iSmallHeight = iSmallWidth;
			iLargeWidth = rs.Width();
			iLargetHeight = rs.Height() - iSmallHeight - iGap;
			iSmallTop = iLargetHeight+iGap;
			
			if(m_layoutPara==1)
			{
				m_2DView1->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(0, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView2->MoveWindow(iSmallWidth + iGap, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView3->MoveWindow(iSmallWidth * 2 + iGap * 2, iSmallTop, 
					rs.Width()-(iSmallWidth * 2 + iGap * 2), iSmallHeight,FALSE);
			}
			if(m_layoutPara==2)
			{
				m_2DView2->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(0, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView3->MoveWindow(iSmallWidth + iGap, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView1->MoveWindow(iSmallWidth * 2 + iGap * 2, iSmallTop, 
					rs.Width()-(iSmallWidth * 2 + iGap * 2), iSmallHeight,FALSE);
			}
			if(m_layoutPara==3)
			{
				m_2DView3->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(0, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView1->MoveWindow(iSmallWidth + iGap, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView2->MoveWindow(iSmallWidth * 2 + iGap * 2, iSmallTop, 
					rs.Width()-(iSmallWidth * 2 + iGap * 2), iSmallHeight,FALSE);
			}
			if(m_layoutPara==4)
			{
				m_3DView->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_2DView1->MoveWindow(0, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView2->MoveWindow(iSmallWidth + iGap, iSmallTop, iSmallWidth, iSmallHeight,FALSE);
				m_2DView3->MoveWindow(iSmallWidth * 2 + iGap * 2, iSmallTop, 
					rs.Width()-(iSmallWidth * 2 + iGap * 2), iSmallHeight,FALSE);
			}
			break;
		case CMD_TPS_DISPLAY_AVERAGE:
			iSmallWidth = (rs.Width() - iGap) / 2;
			iSmallHeight = (rs.Height() - iGap ) / 2;
			
			
			m_3DView->MoveWindow(0, 0, iSmallWidth, iSmallHeight,FALSE);
			m_2DView1->MoveWindow(iGap+iSmallWidth, 0, rs.Width()-(iSmallWidth+iGap), iSmallHeight,FALSE);
			m_2DView2->MoveWindow(0, iGap+iSmallHeight, iSmallWidth, rs.Height()-iSmallHeight-iGap,FALSE);
			m_2DView3->MoveWindow(iGap+iSmallWidth, iGap+iSmallHeight, rs.Width()-(iSmallWidth+iGap), rs.Height()-iSmallHeight-iGap,FALSE);
			
			break;
		case CMD_TPS_DISPLAY_MAX:
			
			iLargeWidth = rs.Width() ;
			iLargetHeight = rs.Height() ;
			
			if(m_layoutPara==1)
			{
				m_2DView1->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView2->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView3->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				
			}
			if(m_layoutPara==2)
			{
				m_2DView2->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView3->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView1->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				
			}
			if(m_layoutPara==3)
			{
				m_2DView3->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_3DView->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView1->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView2->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				
			}
			if(m_layoutPara==4)
			{
				m_3DView->MoveWindow(0, 0, iLargeWidth, iLargetHeight,FALSE);
				m_2DView1->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView2->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				m_2DView3->MoveWindow(iGap, iGap+iLargetHeight, 1, 1,FALSE);
				
			}
			break;
		default:
			break;
		}
		
		
		
		m_3DView->Invalidate(FALSE);
		m_2DView1->Invalidate(FALSE);
		m_2DView2->Invalidate(FALSE);
		m_2DView3->Invalidate(FALSE);

		m_pMPRView1->ShowWindow(FALSE);
		m_pMPRView2->ShowWindow(FALSE);
	}else
	{
		m_pMPRView1->ShowWindow(TRUE);
		m_pMPRView2->ShowWindow(TRUE);
		
		m_3DView->ShowWindow(SW_SHOW);

		CTPS2DView *p2DView;
		if(m_iMPRImageWindow==1)
		{
			p2DView=m_2DView1;
		}else if(m_iMPRImageWindow==2)
		{
			p2DView=m_2DView2;
		}else
		{
			p2DView=m_2DView3;
		}
		p2DView->ShowWindow(TRUE);		
		
		iSmallWidth = (rs.Width() - iGap) / 2;
		iSmallHeight = (rs.Height() - iGap ) / 2;
		
		
		m_3DView->MoveWindow(0, 0, iSmallWidth, iSmallHeight,FALSE);
		p2DView->MoveWindow(iGap+iSmallWidth, 0, rs.Width()-(iSmallWidth+iGap), iSmallHeight,FALSE);
		m_pMPRView1->MoveWindow(0, iGap+iSmallHeight, iSmallWidth, rs.Height()-iSmallHeight-iGap,FALSE);
		m_pMPRView2->MoveWindow(iGap+iSmallWidth, iGap+iSmallHeight, rs.Width()-(iSmallWidth+iGap), rs.Height()-iSmallHeight-iGap,FALSE);

		if(m_iMPRImageWindow==1)
		{
			m_2DView2->ShowWindow(FALSE);
			m_2DView3->ShowWindow(FALSE);
		}else if(m_iMPRImageWindow==2)
		{
			m_2DView1->ShowWindow(FALSE);
			m_2DView3->ShowWindow(FALSE);
		}else
		{
			m_2DView1->ShowWindow(FALSE);
			m_2DView2->ShowWindow(FALSE);
		}
		
	}

	CClientDC dc(this);
	dc.FillSolidRect(rs,RGB(255,255,255));
		
}

void CDisplayDlg::OnDestroy() 
{
	KillTimer(INDICATOR_TPS_PROBE);
	KillTimer(INDICATOR_LOAD_TPS);

	CVImage vImage;
	vImage.Clear();

	if(m_2DView1!=NULL)
	{
		if(m_2DView1->m_hWnd)
		{
			m_2DView1->DestroyWindow();
		}
		delete m_2DView1;
	}
	if(m_2DView2!=NULL)
	{
		if(m_2DView2->m_hWnd)
		{
			m_2DView2->DestroyWindow();
		}
		delete m_2DView2;
	}
	if(m_2DView3!=NULL)
	{
		if(m_2DView3->m_hWnd)
		{
			m_2DView3->DestroyWindow();
		}
		delete m_2DView3;
	}
	if(m_3DView!=NULL)
	{
		if(m_3DView->m_hWnd)
		{
			m_3DView->DestroyWindow();
		}
		delete m_3DView;
	}

	m_2DView1=NULL;
	m_2DView2=NULL;
	m_2DView3=NULL;
	m_3DView=NULL;
	

	if(m_pMPRView1!=NULL)
	{
		if(m_pMPRView1->m_hWnd)
		{
			m_pMPRView1->DestroyWindow();
		}
		delete m_pMPRView1;
		m_pMPRView1=NULL;
	}

	if(m_pMPRView2!=NULL)
	{
		if(m_pMPRView2->m_hWnd)
		{
			m_pMPRView2->DestroyWindow();
		}
		delete m_pMPRView2;
		m_pMPRView2=NULL;
	}

	CAppDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CDisplayDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN){   
		switch(pMsg->wParam){   
		case   VK_ESCAPE:   
			 return   TRUE;   
			 break;
		case   VK_RETURN:   
			return   TRUE;   
			break;
		}   
	}   
	return CAppDialog::PreTranslateMessage(pMsg);
}



void CDisplayDlg::SetMPRImage(CString sPara)
{
	

	if(sPara=="1")
	{
		m_pMPRView1->InitImage(m_2DView1->GetImage(),m_2DView1->m_pImageFrame_scanner);
		m_pMPRView2->InitImage(m_2DView1->GetImage(),m_2DView1->m_pImageFrame_scanner);
		m_pMPRImageMatrix=m_2DView1->m_pImageFrame_scanner;
		
	}else if(sPara=="2")
	{
		m_pMPRView1->InitImage(m_2DView2->GetImage(),m_2DView2->m_pImageFrame_scanner);
		m_pMPRView2->InitImage(m_2DView2->GetImage(),m_2DView2->m_pImageFrame_scanner);
		m_pMPRImageMatrix=m_2DView2->m_pImageFrame_scanner;
	}else if(sPara=="3")
	{
		m_pMPRView1->InitImage(m_2DView3->GetImage(),m_2DView3->m_pImageFrame_scanner);
		m_pMPRView2->InitImage(m_2DView3->GetImage(),m_2DView3->m_pImageFrame_scanner);
		m_pMPRImageMatrix=m_2DView3->m_pImageFrame_scanner;
	}else
	{
		return;
	}

	m_iMPRImageWindow=atoi(sPara);
	m_bDisplayMPR=TRUE;
	Layout(m_layoutStyle,m_layoutPara);

	SetMPRProbePosition();
}

void CDisplayDlg::SetMPRProbe(CString sPara)
{
	

	m_sMPRProbe=sPara;
	CProbeCryo *pProbe=CGlobalManager::Manager()->GetProbeByName(m_sMPRProbe);
	if(!pProbe)
		return;
	
	m_pMPRView1->DeleteProbe();
	m_pMPRView1->AddProbe(pProbe);
	m_pMPRView2->DeleteProbe();
	m_pMPRView2->AddProbe(pProbe);

	if(m_bDisplayMPR==FALSE)
		return;

	SetMPRProbePosition();
}

void CDisplayDlg::SetMPRProbePosition()
{
	if(m_sMPRProbe=="" || m_bDisplayMPR==FALSE)
		return;

	CProbeCryo *pProbe=CGlobalManager::Manager()->GetProbeByName(m_sMPRProbe);
	if(!pProbe)
		return;

	double d1[16]={1,0,0,0,
		           0,1,0,0,
				   0,0,1,70,
				   0,0,0,1};
	double d2[16]={0,1,0,120,
		           0,0,1,120,
				   1,0,0,70,
				   0,0,0,1};

	vtkMatrix4x4 *pProbeMatrix=pProbe->GetMatrix();
	Matrix m1(4,4),m2(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m1.value[i][j]=pProbeMatrix->GetElement(i,j);
			m2.value[i][j]=m_pMPRImageMatrix->GetElement(i,j);
		}
	}

	Matrix mProbeInImage=m2.Inverse() * m1;

	d1[0]=mProbeInImage.value[0][0];
	d1[1]=mProbeInImage.value[0][1];
	d1[2]=mProbeInImage.value[0][2];
	d1[3]=mProbeInImage.value[0][3];
	d1[4]=mProbeInImage.value[1][0];
	d1[5]=mProbeInImage.value[1][1];
	d1[6]=mProbeInImage.value[1][2];
	d1[7]=mProbeInImage.value[1][3];
	d1[8]=mProbeInImage.value[2][0];
	d1[9]=mProbeInImage.value[2][1];
	d1[10]=mProbeInImage.value[2][2];
	d1[11]=mProbeInImage.value[2][3];

	mProbeInImage=mProbeInImage.RotateY(3.1415926/2.0);
	d2[0]=mProbeInImage.value[0][0];
	d2[1]=mProbeInImage.value[0][1];
	d2[2]=mProbeInImage.value[0][2];
	d2[3]=mProbeInImage.value[0][3];
	d2[4]=mProbeInImage.value[1][0];
	d2[5]=mProbeInImage.value[1][1];
	d2[6]=mProbeInImage.value[1][2];
	d2[7]=mProbeInImage.value[1][3];
	d2[8]=mProbeInImage.value[2][0];
	d2[9]=mProbeInImage.value[2][1];
	d2[10]=mProbeInImage.value[2][2];
	d2[11]=mProbeInImage.value[2][3];

	m_pMPRView1->Reslice(d1);
	m_pMPRView2->Reslice(d2);
		
}

void CDisplayDlg::DeleteProbe(CString sProbe)
{
	if(m_sMPRProbe!=sProbe)
		return;

	m_pMPRView1->DeleteProbe();
	m_pMPRView1->Invalidate(FALSE);
	m_pMPRView2->DeleteProbe();
	m_pMPRView2->Invalidate(FALSE);
}

//从导航系统传过来的针的位置
//针的位置应该是相对于影像扫描设备的坐标
void CDisplayDlg::UpdateRealProbeInfo(double *tipPos, double *ori)
{
	m_RealProbeTipPos[0]=tipPos[0];
	m_RealProbeTipPos[1]=tipPos[1];
	m_RealProbeTipPos[2]=tipPos[2];

	m_RealProbeOri[0]=ori[0];
	m_RealProbeOri[1]=ori[1];
	m_RealProbeOri[2]=ori[2];

	if(CGlobalManager::Manager()->m_3DView->m_bMouseDown) //按下鼠标时，进行更新,有时候会发生体绘制失败
		return;

	m_bRealProbePosChanged=TRUE;

	return;
	
}

void CDisplayDlg::UpdateRealProbe()
{
	if(!m_bRealProbePosChanged)
		return;

	m_bRealProbePosChanged=FALSE;

	if(CGlobalManager::Manager()->m_3DView->m_bMouseDown) //按下鼠标时，进行更新,有时候会发生体绘制失败
		return;

	CProbeCryo *pProbe=CGlobalManager::Manager()->GetRealProbe();
	if(!pProbe)
		return;
	
	double endPos[3];
	endPos[0]=m_RealProbeTipPos[0]+m_RealProbeOri[0]*100;
	endPos[1]=m_RealProbeTipPos[1]+m_RealProbeOri[1]*100;
	endPos[2]=m_RealProbeTipPos[2]+m_RealProbeOri[2]*100;

	

	pProbe->ChangePositionByTarget(m_RealProbeTipPos,0); //改变针尖
	pProbe->ChangePositionByTarget(endPos,1); //改变针尾
	
	CGlobalManager::Manager()->m_2DView1->AddRealProbe(pProbe);
	CGlobalManager::Manager()->m_2DView2->AddRealProbe(pProbe);
	CGlobalManager::Manager()->m_2DView3->AddRealProbe(pProbe);
	CGlobalManager::Manager()->m_3DView->AddRealProbe(pProbe);
	m_pMPRView1->AddRealProbe(pProbe);
	m_pMPRView2->AddRealProbe(pProbe);

	
			
	return;
}
