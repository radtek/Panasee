#if !defined(AFX_APPVIEW_H__F2E58E42_091D_11D6_85E5_00D0C9236746__INCLUDED_)
#define AFX_APPVIEW_H__F2E58E42_091D_11D6_85E5_00D0C9236746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppView.h : header file
//

#include "UtilDll.h"
#include "AppDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CAppView view

class UTILDLL_API CAppView : public CView
{
protected:
	CAppView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAppView)
	virtual ~CAppView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetTitle(CString &str);
	virtual CAppDialog* CreateMyInnerDlg(UINT nID);
	virtual void Resize();
	void ChangeInnerDlg(UINT nID, const CString& szTitle);
	void ChangeInnerDlg(CAppDialog* pDlg);

protected:
    // Data
	 CAppDialog* m_pDlg;
	 CString m_szTitle;

    // Helpers
	void DestroyInnerDlg();
    void ShowInnerDlg();

// Debug
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPVIEW_H__F2E58E42_091D_11D6_85E5_00D0C9236746__INCLUDED_)
