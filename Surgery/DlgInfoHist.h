#if !defined(AFX_DLGINFOHIST_H__58DDFA1F_7EAB_41AD_8E37_9721ADFC3615__INCLUDED_)
#define AFX_DLGINFOHIST_H__58DDFA1F_7EAB_41AD_8E37_9721ADFC3615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInfoHist.h : header file
//
#include "PanaListCtrl.h"
#include "PanaseeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoHist dialog

class CDlgInfoHist : public CPanaseeDialog
{
// Construction
public:
	CDlgInfoHist(CWnd* pParent = NULL);   // standard constructor

	void AddAMessage(CString sMsg);

// Dialog Data
	//{{AFX_DATA(CDlgInfoHist)
	enum { IDD = IDD_DLG_INFO_HIST };
	CPanaListCtrl	m_lstInfoHist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfoHist)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInfoHist)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFOHIST_H__58DDFA1F_7EAB_41AD_8E37_9721ADFC3615__INCLUDED_)
