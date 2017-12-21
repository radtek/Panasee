#if !defined(AFX_DLGMOVEPT_H__1C8A6EB9_07F2_4EBA_B096_7970313540D2__INCLUDED_)
#define AFX_DLGMOVEPT_H__1C8A6EB9_07F2_4EBA_B096_7970313540D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMovePT.h : header file
//
#include "PanaseeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMovePT dialog

class CDlgMovePT : public CPanaseeDialog
{
// Construction
public:
	CDlgMovePT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMovePT)
	enum { IDD = IDD_DLG_MOVE_PT };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	double	m_dCurPos;
	//}}AFX_DATA

	//CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMovePT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMovePT)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOVEPT_H__1C8A6EB9_07F2_4EBA_B096_7970313540D2__INCLUDED_)
