#if !defined(AFX_APPSTATIC_H__15210FAA_05F0_46A1_AA00_DD2146F4A9EC__INCLUDED_)
#define AFX_APPSTATIC_H__15210FAA_05F0_46A1_AA00_DD2146F4A9EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAppStatic window
#include "UtilDll.h"
#include "AppDialog.h"

class CAppStatic : public CStatic
{
// Construction
public:
	CAppStatic();
	void ChangeInnerDlg(CAppDialog* pNewDlg);

// Attributes
public:

// Operations
protected:
	CAppDialog* m_pDlg;
	 CString m_szTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppStatic)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void Resize();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPSTATIC_H__15210FAA_05F0_46A1_AA00_DD2146F4A9EC__INCLUDED_)
