#if !defined(AFX_DLGAUTOTRACE_H__903902D4_A779_43AB_A3B2_3F8B8C206BEE__INCLUDED_)
#define AFX_DLGAUTOTRACE_H__903902D4_A779_43AB_A3B2_3F8B8C206BEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTrace.h : header file
//
#include "Resource.h"
#include "FrmMgr.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTrace dialog
 
class CDlgAutoTrace : public CDialog
{
// Construction
public:
	void SetActiveWnd(CString sPara);
	CDlgAutoTrace(CWnd* pParent = NULL);   // standard constructor

	CFrmMgr *m_pFrmMgr;

// Dialog Data
	//{{AFX_DATA(CDlgAutoTrace)
	enum { IDD = IDD_DIALOG_AUTO_TRACE };
	CButton	m_btnDelete;
	CButton	m_btnNew;
	CSliderCtrl	m_pSlider;
	BOOL	m_bAuto;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTrace)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iActiveWnd;

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTrace)
	afx_msg void OnButtonApply();
	afx_msg void OnCustomdrawSliderGrey(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnCheckAuto();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTRACE_H__903902D4_A779_43AB_A3B2_3F8B8C206BEE__INCLUDED_)
