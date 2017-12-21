#if !defined(AFX_DLGTITLE_H__772CD8EE_9672_4FCE_B435_6CF31052BE8B__INCLUDED_)
#define AFX_DLGTITLE_H__772CD8EE_9672_4FCE_B435_6CF31052BE8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTitle.h : header file
//

#include "GlobalFuncs.h"

#include "DlgTitleSkin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTitle dialog

class CDlgTitle : public CDlgTitleSkin
{
// Construction
public:
	CDlgTitle(CWnd* pParent = NULL);   // standard constructor
	~CDlgTitle();

	void SetTitle(OPERATIONTYPE nOperType);

	CString m_sTitle;

// Dialog Data
	//{{AFX_DATA(CDlgTitle)
	enum { IDD = IDD_DLG_TITLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTitle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTitle)
//	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	CBitmap  m_bmpTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTITLE_H__772CD8EE_9672_4FCE_B435_6CF31052BE8B__INCLUDED_)
