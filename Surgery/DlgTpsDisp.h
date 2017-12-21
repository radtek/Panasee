#if !defined(AFX_DLGTPSDISP_H__EC08C803_F95C_4C0D_A62F_9FC309DA6D6B__INCLUDED_)
#define AFX_DLGTPSDISP_H__EC08C803_F95C_4C0D_A62F_9FC309DA6D6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTpsDisp.h : header file
//
#include "AppStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTpsDisp dialog
 
class CDlgTpsDisp : public CDialog
{
// Construction
public:
	CDlgTpsDisp(CWnd* pParent = NULL);   // standard constructor

	void display(CAppDialog *pDlg);

// Dialog Data
	//{{AFX_DATA(CDlgTpsDisp)
	enum { IDD = IDD_DLG_TPS_DISP };
	CAppStatic	m_disp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTpsDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTpsDisp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTPSDISP_H__EC08C803_F95C_4C0D_A62F_9FC309DA6D6B__INCLUDED_)
