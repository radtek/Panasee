#if !defined(AFX_PATINFOEDITORDLG_H__4DF38DE6_4D71_4ABC_8FA6_FE10F8B53EFD__INCLUDED_)
#define AFX_PATINFOEDITORDLG_H__4DF38DE6_4D71_4ABC_8FA6_FE10F8B53EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatInfoEditorDlg.h : header file

#include "PanaseeDialog.h"
//
#define IDD_DLG_PATINFOEDITOR              11031
/////////////////////////////////////////////////////////////////////////////
// CPatInfoEditorDlg dialog

class AFX_EXT_CLASS CPatInfoEditorDlg : public CPanaseeDialog
{
// Construction
public:
	CPatInfoEditorDlg(CWnd* pParent = NULL);   // standard constructor

	void SetValue(CString csID, CString csName, CString csBirth,
		          CString csAge, CString csWeight, CString csSex, CString csDesc);
	void GetValue(CString &csID, CString &csName, CString &csBirth,
		          CString &csAge, CString &csWeight, CString &csSex, CString &csDesc);
	void EnableEdit(BOOL bEnable) ;
	void SetEditMode(int nMode){m_nEditMode = nMode;};
// Dialog Data
	//{{AFX_DATA(CPatInfoEditorDlg)
	enum { IDD = IDD_DLG_PATINFOEDITOR };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CString	m_csID;
	CString	m_csName;
	int		m_nWeight;
	CString	m_csBirth;
	CString	m_csDesc;
	int		m_nSex;
	int		m_nAge;
	//}}AFX_DATA

	
	
	int   m_nEditMode ;    //0: No Edit; 1: Edit; 2: New .
protected:
	bool    CheckValidate();
	CString AgeToBirthday(int nAge);
	int     BirthToAge(CString csBirth,int* pnDays);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatInfoEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatInfoEditorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditAge();
	afx_msg void OnKillfocusEditBirthdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATINFOEDITORDLG_H__4DF38DE6_4D71_4ABC_8FA6_FE10F8B53EFD__INCLUDED_)
