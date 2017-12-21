// DlgPatient.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgPatient.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatient dialog


CDlgPatient::CDlgPatient(CWnd* pParent /*=NULL*/)
: CDlgPatientSkin(CDlgPatient::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPatient)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPatient::DoDataExchange(CDataExchange* pDX)
{
	CDlgPatientSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatient)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPatient, CDlgPatientSkin)
	//{{AFX_MSG_MAP(CDlgPatient)
	ON_WM_SHOWWINDOW()
//	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatient message handlers

BOOL CDlgPatient::OnInitDialog() 
{
	CDlgPatientSkin::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatient::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDlgPatientSkin::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{

	}
}



BOOL CDlgPatient::PreTranslateMessage(MSG* pMsg) 
{
	//避免 按[回车]和[Esc] Dialog 消失
	if((pMsg->message==WM_KEYDOWN))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
		return TRUE;   
    }
	
	return CDlgPatientSkin::PreTranslateMessage(pMsg);
}

void CDlgPatient::Update()
{
	CDlgMain* pDlgMain = g_DlgMain;

	IGT_PatientField* pPatient = &(pDlgMain->m_MRIWrap.m_CurPatient);

	SetName(pPatient->Name);
	SetID(pPatient->Id);
	SetSex(pPatient->Sex == 'M' ? "Male" : (pPatient->Sex == 'F' ? "Female" : "Unknown"));
	CString sOut;
	sOut.Format("%d", pPatient->Age);
	SetAge(sOut);
	sOut.Format("%d", pPatient->Weight);
	SetWeight(sOut);
	SetDesc(pPatient->Desc);
}
