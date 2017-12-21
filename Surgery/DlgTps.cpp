// DlgTps.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTps.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTps dialog


CDlgTps::CDlgTps(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTps)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTps, CDialog)
	//{{AFX_MSG_MAP(CDlgTps)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTps message handlers


BOOL CDlgTps::OnEraseBkgnd(CDC* pDC)
{
 	return TRUE;
}

BOOL CDlgTps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CWnd* pDlgMain = (CWnd*)g_DlgMain;
	m_disp.Create(IDD_DLG_TPS_DISP,pDlgMain);
	m_disp.SetParent(pDlgMain);
	m_disp.ShowWindow(FALSE);
	m_disp.MoveWindow(0,0,1024-268,768);

	m_ctrl.Create(IDD_DLG_TPS_CTRL,this);
	m_ctrl.SetParent(this);
	m_ctrl.ShowWindow(FALSE);

	m_pMgr = new CTPSFrmMgr();

	CAppDialog* pGraDlg = NULL;
	CAppDialog* pCtrlDlg = NULL;
	if(m_pMgr)
	{
		pGraDlg	= (CAppDialog*)m_pMgr->GetAt(0);
		pCtrlDlg = (CAppDialog*)m_pMgr->GetAt(1);
	}
	
	m_disp.display(pGraDlg);
	m_ctrl.display(pCtrlDlg);


	CRect rc(26,188,225,615);
	MoveWindow(rc);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTps::OnDestroy() 
{
	delete m_pMgr;

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDlgTps::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_ctrl.GetSafeHwnd())
		return;

	
	m_ctrl.MoveWindow(0,0,cx,cy);
}

void CDlgTps::ShowDisp(BOOL bShow)
{
	CDlgMain* pDlgMain = g_DlgMain;
	if(bShow )
	{
		m_disp.ShowWindow(TRUE);
		pDlgMain->m_display4Dlg.ShowWindow(FALSE);
		
		
	}else
	{
		m_disp.ShowWindow(FALSE);
		pDlgMain->m_display4Dlg.ShowWindow(TRUE);
		
	}
}

void CDlgTps::ShowCtrl(BOOL bShow)
{
	CDlgMain* pDlgMain = g_DlgMain;
	if(bShow )
	{
		if(!this->IsWindowVisible())
		{
			ShowWindow(TRUE);
			m_ctrl.ShowWindow(TRUE);
		}
		
	}else
	{
		if(this->IsWindowVisible())
		{
			
			ShowWindow(FALSE);
			m_ctrl.ShowWindow(FALSE);
		}
	}
}

void CDlgTps::ChangeDisplaySize(int left,int top,int width, int height)
{
	m_disp.MoveWindow(left,top,width,height);
}

void CDlgTps::UpdateProbeInfo(double *tipPos, double *ori)
{
	if(m_disp.IsWindowVisible()==TRUE)
	{
		m_pMgr->UpdateProbeInfo(tipPos,ori);
	}
}

void CDlgTps::GetProbeInfo(int &count, double *pProbeData)
{
	m_pMgr->GetProbeInfo(count,NULL);
	if(count<1)
		return;
	
	m_pMgr->GetProbeInfo(count,pProbeData);
}
