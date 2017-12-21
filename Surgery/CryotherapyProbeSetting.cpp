// CryotherapyProbeSetting.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "CryotherapyProbeSetting.h"
#include "configmgr.h" //Add by Dai Liang 2007.6.5
#include "Log.h"
#include "GlobalFuncs.h"
#include "Config.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCryotherapyProbeSetting dialog


CCryotherapyProbeSetting::CCryotherapyProbeSetting(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CCryotherapyProbeSetting::IDD, pParent)
{
	m_sProbeName = "";
	//{{AFX_DATA_INIT(CCryotherapyProbeSetting)
	m_nLength = 0;
	//}}AFX_DATA_INIT
}


void CCryotherapyProbeSetting::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCryotherapyProbeSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PROBE_TYPE, m_ProbeList);
	DDX_Text(pDX, IDC_EDT_LENGTH, m_nLength);
	DDV_MinMaxUInt(pDX, m_nLength, 0, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCryotherapyProbeSetting, CPanaseeDialog)
	//{{AFX_MSG_MAP(CCryotherapyProbeSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryotherapyProbeSetting message handlers

BOOL CCryotherapyProbeSetting::OnInitDialog() 
{
	SetWindowText("Cryotherapy Probe Setting");
	SetTitleBarVisable(TRUE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	GetDlgItem(IDC_EDT_LENGTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PROBE_TYPE)->EnableWindow(TRUE);
	
	CDlgMain* pDlgMain = g_DlgMain;
	CArray<struct PROBE_PAR_CRY*, struct PROBE_PAR_CRY*>* pArrPar = pDlgMain->m_dlgCryotherapy.m_ProbeParPool.GetProbeParCry();
	for (int i=0;i<pArrPar->GetSize();i++)
	{
		m_ProbeList.AddString(pArrPar->GetAt(i)->sName);
	}
	
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	//m_btnOK.SetThemeHelper(&m_ThemeHelper);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCryotherapyProbeSetting::OnOK()  
{
	CLog::Log("’Î¿‡–Õ£∫¿‰∂≥’Î");
	if (!GetProbeDiameter())
	{
		return;
	}
	CPanaseeDialog::OnOK();
}

bool CCryotherapyProbeSetting::GetProbeDiameter()
{
	if (CB_ERR == m_ProbeList.GetCurSel())
	{
		CLog::Log("Œ¥—°‘Ò’Î–Õ∫≈");
		PanaMessageBox("Please select a probe!");
		return false;
	}
	else
	{
		m_ProbeList.GetLBText(m_ProbeList.GetCurSel(), m_sProbeName);
		CString sLog;
		sLog.Format("Probe£∫%s",m_sProbeName);
		CLog::Log(sLog);
		return true;
	}
}

