// ProbeSetting.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "ProbeSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeSetting dialog


CProbeSetting::CProbeSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CProbeSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProbeSetting)
	m_nProbeType = -1;
	//}}AFX_DATA_INIT
}


void CProbeSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeSetting)
	DDX_Radio(pDX, IDC_RADIO_PRE_SCANNING_PROBE, m_nProbeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeSetting, CDialog)
	//{{AFX_MSG_MAP(CProbeSetting)
	ON_BN_CLICKED(IDC_RADIO_PRE_SCANNING_PROBE, OnRadioPreScanningProbe)
	ON_BN_CLICKED(IDC_RADIO_BIOPSY_PROBE, OnRadioBiopsyProbe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeSetting message handlers

void CProbeSetting::OnRadioPreScanningProbe() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_STATIC_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_LENGTH)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_PROBE_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_LENGTH)->EnableWindow(FALSE);
	
}

void CProbeSetting::OnRadioBiopsyProbe() 
{
	GetDlgItem(IDC_STATIC_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_LENGTH)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_PROBE_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_LENGTH)->EnableWindow(FALSE);
}

void CProbeSetting::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
