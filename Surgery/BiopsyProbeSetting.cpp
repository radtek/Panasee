// BiopsyProbeSetting.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "BiopsyProbeSetting.h"
#include "log.h"
#include "Configmgr.h" //Add  by Dai Liang 2007/6/7
#include "GlobalFuncs.h"
#include "Config.h"
#include "DlgMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBiopsyProbeSetting dialog


CBiopsyProbeSetting::CBiopsyProbeSetting(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CBiopsyProbeSetting::IDD, pParent)
{
	m_sProbeName = "";
	//{{AFX_DATA_INIT(CBiopsyProbeSetting)
	m_nLength = 100;
	//}}AFX_DATA_INIT
}


void CBiopsyProbeSetting::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBiopsyProbeSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PROBE_TYPE, m_ProbeList);
	DDX_Text(pDX, IDC_EDT_BIO_LENGTH, m_nLength);
	DDV_MinMaxUInt(pDX, m_nLength, 0, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBiopsyProbeSetting, CPanaseeDialog)
	//{{AFX_MSG_MAP(CBiopsyProbeSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiopsyProbeSetting message handlers

void CBiopsyProbeSetting::OnOK() 
{
	// TODO: Add extra validation here
	if (!GetProbeDiameter())
	{
		return;
	}	
	CPanaseeDialog::OnOK();
}

void CBiopsyProbeSetting::OnCancel() 
{
	CPanaseeDialog::OnCancel();
}

BOOL CBiopsyProbeSetting::OnInitDialog() 
{
	SetWindowText("Biopsy Probe Setting");
	SetTitleBarVisable(TRUE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	
	GetDlgItem(IDC_EDT_BIO_LENGTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PROBE_TYPE)->EnableWindow(TRUE); 
	
	CDlgMain* pDlgMain = g_DlgMain;
	CArray<struct PROBE_PAR_BIO*, struct PROBE_PAR_BIO*>* pArrPar = pDlgMain->m_dlgCryotherapy.m_ProbeParPool.GetProbeParBio();
	for (int i=0;i<pArrPar->GetSize();i++)
	{
		m_ProbeList.AddString(pArrPar->GetAt(i)->sName);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CBiopsyProbeSetting::GetProbeDiameter()
{
	if (CB_ERR == m_ProbeList.GetCurSel())
	{
		PanaMessageBox("Please select a probe!", MB_ICONWARNING);
		return false;
	}
	else
	{
		m_ProbeList.GetLBText(m_ProbeList.GetCurSel(), m_sProbeName);
		CString sLog;
		sLog.Format("Probe£º%d",m_sProbeName);
		CLog::Log(sLog);
		return true;
	}
}
