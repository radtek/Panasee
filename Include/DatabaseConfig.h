#if !defined(AFX_DATABASECONFIG_H__F2E3364C_E5F1_4BAB_8C9D_9F207C54C377__INCLUDED_)
#define AFX_DATABASECONFIG_H__F2E3364C_E5F1_4BAB_8C9D_9F207C54C377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatabaseConfig.h : header file
//
#include "resource.h"
#define IDD_DIALOG_SETDATABASE          11005
/////////////////////////////////////////////////////////////////////////////
// CDatabaseConfig dialog

//Description: Set database file and folder.
//Usage: User input the path for file and folder. when clicking the "ok" button, 
//it will check if inputted paths for database are valid. if it is valid, paths for 
//database will be saved into registry
#include "PanaseeDialog.h"
#include "ImageButton.h"

class AFX_EXT_CLASS CDatabaseConfig : public CPanaseeDialog
{
// Construction
public:
	CDatabaseConfig(CWnd* pParent = NULL);   // standard constructor
	BOOL m_databaseOK;

	//CThemeHelperST		m_ThemeHelper;


// Dialog Data
	//{{AFX_DATA(CDatabaseConfig)
	enum { IDD = IDD_DIALOG_SETDATABASE };
	CImageButton	m_btnCancel;
	CImageButton	m_btnOK;
	CImageButton	m_btnChange2;
	CImageButton	m_btnChange;
	CString	m_file;
	CString	m_folder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatabaseConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDatabaseConfig)
	afx_msg void OnButtonChange();
	afx_msg void OnButtonChange2();
	afx_msg void OnButtonDatabaseReturn();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASECONFIG_H__F2E3364C_E5F1_4BAB_8C9D_9F207C54C377__INCLUDED_)
