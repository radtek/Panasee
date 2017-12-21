#if !defined(AFX_DLGTIMESKIN_H__CF14F9EC_D895_418F_9B3E_271F40FDF87A__INCLUDED_)
#define AFX_DLGTIMESKIN_H__CF14F9EC_D895_418F_9B3E_271F40FDF87A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTimeSkin.h : header file
//
#include "panaskin.h"
#include "MemDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSkin dialog

class PANASKIN_API CDlgTimeSkin : public CDialog
{
// Construction
public:
	CRect GetTimeRegion();
	CDlgTimeSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTimeSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTimeSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void DrawTime();
	// Generated message map functions
	//{{AFX_MSG(CDlgTimeSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	CMemDC m_skinDC;
	BOOL m_bWaitingFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIMESKIN_H__CF14F9EC_D895_418F_9B3E_271F40FDF87A__INCLUDED_)
