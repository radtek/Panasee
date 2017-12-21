// DatabaseConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DatabaseConfig.h"
#include "ImriDBI.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatabaseConfig dialog


CDatabaseConfig::CDatabaseConfig(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDatabaseConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatabaseConfig)
	m_file = _T("");
	m_folder = _T("");
	//}}AFX_DATA_INIT
	m_databaseOK=FALSE;
}


void CDatabaseConfig::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatabaseConfig)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_DATABASE_RETURN, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CHANGE2, m_btnChange2);
	DDX_Control(pDX, IDC_BUTTON_CHANGE, m_btnChange);
	DDX_Text(pDX, IDC_EDIT_DATABASE_FILE, m_file);
	DDX_Text(pDX, IDC_EDIT_DATABASE_FOLDER, m_folder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDatabaseConfig, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDatabaseConfig)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE2, OnButtonChange2)
	ON_BN_CLICKED(IDC_BUTTON_DATABASE_RETURN, OnButtonDatabaseReturn)
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseConfig message handlers

void CDatabaseConfig::OnButtonChange() 
{
	// TODO: Add your control notification handler code here
	CString szFileName,szFilter = _T("MDB File(*.mdb)|*.mdb||");
	CFileDialog fileDlg(TRUE,									// Save 
		_T("mdb"),								// Default Extension 
		_T("*.mdb"),							// Default file name
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// Flag
		szFilter);								// Filters
	
	fileDlg.m_ofn.nFilterIndex = 1;
	fileDlg.m_ofn.lpstrInitialDir = ".";		// Current dir or set other path
	
	fileDlg.m_ofn.lpstrTitle = _T("Database:");
	if (fileDlg.DoModal() == IDOK) 
		szFileName = fileDlg.GetPathName();
	else
		return;
	m_file=szFileName;

	UpdateData(FALSE);
}

void CDatabaseConfig::OnButtonChange2() 
{
	// TODO: Add your control notification handler code here
	HRESULT hr;
	LPITEMIDLIST pItemList;
	BROWSEINFO browseinfo;
	TCHAR path[MAX_PATH];

	IMalloc *pMalloc = NULL;
	hr = SHGetMalloc(&pMalloc);
	if (FAILED(hr))
	{
		TRACE("Can't retrieve systems's IMalloc interface: %x\n", hr);
		return;
	}

	memset(&browseinfo, 0, sizeof(BROWSEINFO));
	CString sCaption;
	browseinfo.hwndOwner = GetSafeHwnd();
	browseinfo.pszDisplayName = path;
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browseinfo.lpszTitle = "Select MRI folder";

	TCHAR pszPath[MAX_PATH];
	pItemList = ::SHBrowseForFolder(&browseinfo);
	CString sFolderName = "";
	if (pItemList)
	{
		::SHGetPathFromIDList(pItemList, pszPath);
		sFolderName = pszPath;
		pMalloc->Free(pItemList);
		pMalloc->Release();
		pMalloc = NULL;
			
	}else
	{
		pMalloc->Release();
		pMalloc = NULL;
		return;
	}


	
	m_folder=sFolderName;
	UpdateData(FALSE);


}

void CDatabaseConfig::OnButtonDatabaseReturn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CImriDBI m_pdbImri;

	if(m_pdbImri.Validate(m_file,m_folder)!=TRUE)
	{
		PanaMessageBox("Can not connect DB! Please inform provider.");
	}else
	{
		if(m_pdbImri.SaveDBPath(m_file,m_folder))
		{
			m_databaseOK=TRUE;
			CDialog::OnOK();
		}
	}
}

void CDatabaseConfig::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_databaseOK=FALSE;
	CDialog::OnCancel();
}






BOOL CDatabaseConfig::OnInitDialog() 
{
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CImriDBI m_pdbImri;

	m_pdbImri.GetDBPath(m_file,m_folder);
	UpdateData(FALSE);

	EnableSeprateLine(TRUE);
	this->CenterWindow(CWnd::GetDesktopWindow());

	//…Ë÷√UIΩÁ√Ê
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnOK);
	btnList.AddTail(&m_btnCancel);
	SetBottomBtnList(&btnList);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDatabaseConfig::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CPanaseeDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	lpncsp->rgrc[0].top += 8;
}
