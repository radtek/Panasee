#if !defined(AFX_DLGWAITING_H__48F65478_591C_46BD_93D4_BFD752A12208__INCLUDED_)
#define AFX_DLGWAITING_H__48F65478_591C_46BD_93D4_BFD752A12208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWaiting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWaiting dialog
#include "PanaseeDialog.h"

class CDlgWaiting : public CPanaseeDialog
{
// Construction
public:
	void showText(CString str);
	CDlgWaiting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWaiting)
	enum { IDD = IDD_DLG_WAITING };
	CStatic	m_wait;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWaiting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWaiting)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWAITING_H__48F65478_591C_46BD_93D4_BFD752A12208__INCLUDED_)
