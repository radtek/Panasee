#if !defined(AFX_DLGTPSCTRL_H__84D22544_8932_4518_9B66_F5F6B90A6A2D__INCLUDED_)
#define AFX_DLGTPSCTRL_H__84D22544_8932_4518_9B66_F5F6B90A6A2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTpsCtrl.h : header file
//
#include "AppStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTpsCtrl dialog

class CDlgTpsCtrl : public CDialog
{
// Construction
public:
	CDlgTpsCtrl(CWnd* pParent = NULL);   // standard constructor

	void display(CAppDialog *pDlg);

// Dialog Data
	//{{AFX_DATA(CDlgTpsCtrl)
	enum { IDD = IDD_DLG_TPS_CTRL };
	CAppStatic	m_ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTpsCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTpsCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTPSCTRL_H__84D22544_8932_4518_9B66_F5F6B90A6A2D__INCLUDED_)
