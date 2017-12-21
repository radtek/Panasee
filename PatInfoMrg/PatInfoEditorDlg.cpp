// PatInfoEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatInfoEditorDlg.h"
#include "Resource.h"
#include "ImriDBI.h"
#include "PanaMetrics.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatInfoEditorDlg dialog


CPatInfoEditorDlg::CPatInfoEditorDlg(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPatInfoEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatInfoEditorDlg)
	m_csID    = _T("");
	m_csName  = _T("");
	m_nWeight = 0;
	m_csBirth = _T("");
	m_csDesc  = _T("");
	m_nSex    = -1;
	m_nAge    = 0;
	//}}AFX_DATA_INIT
	m_nEditMode = 0 ;
}

bool CPatInfoEditorDlg::CheckValidate()
{
	UpdateData(true);

	int    nY, nM, nD ;
	CEdit* pEdit ;
	if (m_csID.IsEmpty() && m_nEditMode !=2)
	{
		PanaMessageBox("Patient ID can not be empty!");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATIENT_ID);
		pEdit->SetFocus();
		return false ;
	}
	if (m_csName.IsEmpty())
	{
		PanaMessageBox("Patient Name can not be empty!");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATIENT_NAME);
		pEdit->SetFocus();
		return false ;
	}
	if(m_csName.GetLength()>30)
	{
		PanaMessageBox("Patient Name is too long.");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATIENT_NAME);
		pEdit->SetFocus();
		return false ;
	}
	if ((m_nAge <= 0) || (m_nAge > 120))
	{
		PanaMessageBox("Patient age is not valid!");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AGE);
		pEdit->SetFocus();
		return false ;
	}
	if (gCheckDateFormat(m_csBirth,nY, nM, nD) < 0 )
	{
		PanaMessageBox("Patient birthday is not valid!");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AGE);
		pEdit->SetFocus();
		return false ;
	}
	if(m_nWeight<=0 || m_nWeight>200)
	{
		PanaMessageBox("Patient weight is wrong.");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_WEIGHT);
		pEdit->SetFocus();
		return false ;
	}
	if(m_csDesc.GetLength()>48)
	{
		PanaMessageBox("Patient description is too long.\nIt should be less than 48 characters.");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATIENT_DESCRIPTION);
		pEdit->SetFocus();
		return false ;
	}
	return true;
}

void CPatInfoEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatInfoEditorDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_PATIENT_ID, m_csID);
	DDX_Text(pDX, IDC_EDIT_PATIENT_NAME, m_csName);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_nWeight);
	DDX_Text(pDX, IDC_EDIT_BIRTHDATE, m_csBirth);
	DDX_Text(pDX, IDC_EDIT_PATIENT_DESCRIPTION, m_csDesc);
	DDX_CBIndex(pDX, IDC_COMBO_SEX, m_nSex);
	DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatInfoEditorDlg, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPatInfoEditorDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_AGE, OnKillfocusEditAge)
	ON_EN_KILLFOCUS(IDC_EDIT_BIRTHDATE, OnKillfocusEditBirthdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatInfoEditorDlg message handlers
void CPatInfoEditorDlg::SetValue(CString csID, CString csName, CString csBirth,
		          CString csAge, CString csWeight, CString csSex, CString csDesc)
{
	m_csID    = csID;
	m_csName  = csName;
	m_nWeight = atoi(csWeight.GetBuffer(16));
	m_nAge    = atoi(csAge.GetBuffer(4));
	m_csBirth = csBirth;
	m_csDesc  = csDesc;

	char cSex = csSex.GetAt(0);
	if (cSex == 'M')
	{
		m_nSex    = 0;
	}
	else if (cSex == 'F')
	{
		m_nSex    = 1;
	}
	else if (cSex == 'U')
	{
		m_nSex    = 2;
	}
	else
	{
		m_nSex    = -1;
	}
}
void CPatInfoEditorDlg::GetValue(CString &csID, CString &csName, CString &csBirth,
		          CString &csAge, CString &csWeight, CString &csSex, CString &csDesc)
{
	csID    = m_csID ;
	csName  = m_csName ;
	csWeight.Format("%d", m_nWeight) ;
	csAge.Format("%d", m_nAge) ;
	csBirth = m_csBirth ;
	csDesc  = m_csDesc ;

	if (m_nSex == 0)
	{
		csSex = "M";
	}
	else if (m_nSex == 1)
	{
		csSex = "F";
	}
	else
	{
		csSex = "U";
	}
}

void  CPatInfoEditorDlg::EnableEdit(BOOL bEnable)
{
	CEdit     *pEdit ;
	CComboBox *pComb ;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATIENT_NAME);
	pEdit->EnableWindow(bEnable);
    pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATIENT_DESCRIPTION);
	pEdit->EnableWindow(bEnable);
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_WEIGHT);
	pEdit->EnableWindow(bEnable);
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_AGE);
	pEdit->EnableWindow(bEnable);
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BIRTHDATE);
	pEdit->EnableWindow(bEnable);
	pComb = (CComboBox *)GetDlgItem(IDC_COMBO_SEX);
	pComb->EnableWindow(bEnable);
}

BOOL CPatInfoEditorDlg::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	CPanaseeDialog::OnInitDialog();
	
	EnableSeprateLine(TRUE);
	// TODO: Add extra initialization here
	
	if (m_nEditMode == 0)
	{
		EnableEdit(FALSE) ;
	}

	//…Ë÷√UIΩÁ√Ê
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnOK);
	btnList.AddTail(&m_btnCancel);
	SetBottomBtnList(&btnList);

	CRect rcTmp,rcClient;
	GetClientRect(&rcClient);

	GetDlgItem(IDC_STATIC_EDITOR)->GetWindowRect(&rcTmp);
	ScreenToClient(&rcTmp);
	rcTmp.right=rcClient.right-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_RIGHT));
	rcTmp.left=CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_LEFT);
	GetDlgItem(IDC_STATIC_EDITOR)->MoveWindow(rcTmp);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatInfoEditorDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_nEditMode != 0)
	{
		if (!CheckValidate())
		{
			return ;
		}
	}
	CPanaseeDialog::OnOK();
}

void CPatInfoEditorDlg::OnKillfocusEditAge() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true) ;
	int nY, nM, nD;
	if (m_nAge > 0)
	{
		if ( m_csBirth.IsEmpty() )
		{
			m_csBirth = AgeToBirthday(m_nAge);
			UpdateData(false) ;
		}
		else
		{
			if (gCheckDateFormat(m_csBirth,nY, nM, nD) < 0 )
			{
				m_csBirth = AgeToBirthday(m_nAge);
				UpdateData(false) ;
			}
			else
			{
				int nDays ;
				if (m_nAge != BirthToAge(m_csBirth, &nDays)/12)
				{
					m_csBirth = AgeToBirthday(m_nAge);
					UpdateData(false) ;
				}
			}
		}
	}
}

void CPatInfoEditorDlg::OnKillfocusEditBirthdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true) ;
	int nMonth ,nDays;
	if (m_csBirth.IsEmpty())
	{
		return ;
	}

	nMonth = BirthToAge(m_csBirth, &nDays) ;

	if (nMonth < 0 )
	{
		CEdit* pEdit ;
		PanaMessageBox("Patient birthday is not valid!");
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BIRTHDATE);
		pEdit->SetFocus();
		return;
	}
	else
	{
	    m_nAge = nMonth/12 ;
		UpdateData(false) ;
	}
}


int CPatInfoEditorDlg::BirthToAge(CString csBirth,int* pnDays) 
{
	CString csYear;
	CString csMonth;
	CString csDay;
	CString csCurrentYear;
	CString csCurrentMonth;
	CString csCurrnetDay;
	int nLeftDay;
	int nAgeMonth;
	int nAge;
	int nYear;
	int nMonth;
	int nDay;
	int nCurrentYear;
	int nCurrentMonth;
	int nCurrentDay;

	if (gCheckDateFormat(csBirth,nYear,nMonth,nDay) < 0)
	{
		return -1 ;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	
	nCurrentYear   = sysTime.wYear;
	nCurrentMonth  = sysTime.wMonth;
	nCurrentDay    = sysTime.wDay;

	nAge    = nCurrentYear - nYear;
	if(nCurrentDay >= nDay)
	{
		nLeftDay  = nCurrentDay - nDay;
		nAgeMonth = nAge * 12 + (nCurrentMonth - nMonth);
	}
	else
	{
		if((nMonth == 1) || (nMonth == 3)  || (nMonth == 5) || (nMonth == 7) ||
	       (nMonth == 8) || (nMonth == 10) || (nMonth == 12))
		{
			nLeftDay = 31 + nCurrentDay - nDay;
		}
		else if((nMonth == 4) || (nMonth == 6) || (nMonth == 9) || (nMonth == 11))
		{
			nLeftDay = 30 + nCurrentDay - nDay;
		}
		else
		{
			nLeftDay = 30 + nCurrentDay - nDay;
		}
		nAgeMonth = nAge * 12 + (nCurrentMonth - nMonth) -1;
	}

	*pnDays = nLeftDay ;
	return nAgeMonth ;
}

CString CPatInfoEditorDlg::AgeToBirthday(int nAge)
{
    CString csAge;
	CString csYear;
	CString csBirthday;
	int nYear;
	int nBirthYear;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	nYear = sysTime.wYear;
	nBirthYear = nYear - nAge;
	if (sysTime.wMonth > 7)
	{
		nBirthYear++ ;
	}
	csBirthday.Format("%d-01-01",nBirthYear);
	return csBirthday;
}


