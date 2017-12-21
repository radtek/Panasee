// DlgSimulate.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "DlgSimulate.h"
#include "Probe.h"
#include "TrackTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSimulate dialog


CDlgSimulate::CDlgSimulate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSimulate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSimulate)
	m_dbIceballOffset = 0.0;
	//}}AFX_DATA_INIT
	m_pCryotherapyDlg = NULL;
	m_pTrackTool = NULL;
	m_pProbe = NULL;
}


void CDlgSimulate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSimulate)
	DDX_Text(pDX, IDC_EDT_ICEBALL_OFFSET, m_dbIceballOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSimulate, CDialog)
	//{{AFX_MSG_MAP(CDlgSimulate)
	ON_BN_CLICKED(ID_SIMULATE, OnSimulate)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSimulate message handlers

void CDlgSimulate::OnSimulate() 
{
	// TODO: Add your control notification handler code here
	if (m_pCryotherapyDlg)
	{
		OnApply();
		m_pCryotherapyDlg->OnProbeSimulate();
	}
}

void CDlgSimulate::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDlgSimulate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_pCryotherapyDlg)
	{
		m_pTrackTool = m_pCryotherapyDlg->m_ppTrackTool[m_pCryotherapyDlg->m_iSelectHandle];
		if (m_pTrackTool)
		{
			if (m_pTrackTool->m_TrackType == TT_NEEDLE ||
				m_pTrackTool->m_TrackType == TT_EMPTY)
			{
				GetDlgItem(ID_SIMULATE)->EnableWindow(TRUE);
				GetDlgItem(ID_APPLY)->EnableWindow(TRUE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(ID_SIMULATE)->EnableWindow(FALSE);
				GetDlgItem(ID_APPLY)->EnableWindow(FALSE);
				GetDlgItem(IDOK)->EnableWindow(FALSE);
			}

			m_pProbe = m_pTrackTool->m_pAttachedProbe;
			if (m_pProbe)
			{
//				m_dbIceballOffset = m_pProbe->GetIceBallOffset();
			}
		}
		UpdateData(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSimulate::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_pProbe)
	{
//		m_pProbe->SetIceBallOffset(m_dbIceballOffset);
	}
}
