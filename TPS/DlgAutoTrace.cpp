// DlgAutoTrace.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAutoTrace.h"
#include "Commands.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTrace dialog


CDlgAutoTrace::CDlgAutoTrace(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTrace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTrace)
	m_bAuto = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAutoTrace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTrace)
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_btnNew);
	DDX_Control(pDX, IDC_SLIDER_GREY, m_pSlider);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bAuto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTrace, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTrace)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GREY, OnCustomdrawSliderGrey)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_AUTO, OnCheckAuto)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTrace message handlers

void CDlgAutoTrace::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(m_bAuto)
	{
		int i=m_pSlider.GetPos();
		CString sTmp;
		sTmp.Format("%i|%i",m_iActiveWnd,i);
		m_pFrmMgr->SendCommand(this,CMD_TPS_EXTRACT_CONTOUR,sTmp);
	}else
	{
		CString sTmp;
		m_pFrmMgr->SendCommand(this,CMD_TPS_CREATE_TRACE,sTmp);
	}
}

void CDlgAutoTrace::OnCustomdrawSliderGrey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i=m_pSlider.GetPos();
	CString sTmp;
	sTmp.Format("%i|%i",m_iActiveWnd,i);
	m_pFrmMgr->SendCommand(this,CMD_TPS_SETUP_TRACE_FILTER,sTmp);
	*pResult = 0;
}

BOOL CDlgAutoTrace::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN){   
		switch(pMsg->wParam){   
		case   VK_ESCAPE:   
			 return   TRUE;   
			 break;
		case VK_RETURN:
			return TRUE;
			break;
		}   
	}   
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgAutoTrace::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pSlider.SetRange(0,255);
	m_pSlider.SetPos(100);
	m_pSlider.SetPageSize(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoTrace::SetActiveWnd(CString sPara)
{
	m_iActiveWnd=atoi(sPara);

}

void CDlgAutoTrace::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	//先清除目前已有的勾画
	CString sTmp;
	m_pFrmMgr->SendCommand(this,CMD_TPS_DELETE_TRACE,sTmp);
	CGlobalManager::Manager()->m_bCanAddNewTrace=TRUE;
}

void CDlgAutoTrace::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CString sTmp;
	m_pFrmMgr->SendCommand(this,CMD_TPS_DELETE_TRACE,sTmp);
}

void CDlgAutoTrace::OnCheckAuto() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bAuto)
	{
		m_pSlider.EnableWindow(TRUE);
		m_btnNew.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE);

		CString sTmp;
		m_pFrmMgr->SendCommand(this,CMD_TPS_DELETE_TRACE,sTmp);
		sTmp="0";
		m_pFrmMgr->SendCommand(this,CMD_TPS_CHANGE_AUTOTRACE_STATUS,sTmp);
	}else
	{
		m_pSlider.EnableWindow(FALSE);
		m_btnNew.EnableWindow(TRUE);
		m_btnDelete.EnableWindow(TRUE);

		CString sTmp;
		sTmp="1";
		m_pFrmMgr->SendCommand(this,CMD_TPS_CHANGE_AUTOTRACE_STATUS,sTmp);
	}
}

void CDlgAutoTrace::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)
	{
		if(m_bAuto)
		{
			CString sTmp;
			sTmp="0";
			m_pFrmMgr->SendCommand(this,CMD_TPS_CHANGE_AUTOTRACE_STATUS,sTmp);
		}
	}else
	{
		if(m_bAuto)
		{
			CString sTmp;
			sTmp="1";
			m_pFrmMgr->SendCommand(this,CMD_TPS_CHANGE_AUTOTRACE_STATUS,sTmp);
		}
		CString sTmp;
		m_pFrmMgr->SendCommand(this,CMD_TPS_DELETE_TRACE,sTmp);
		CGlobalManager::Manager()->m_bCanAddNewTrace=FALSE;
	}
}
