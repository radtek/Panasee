#if !defined(AFX_DLGMYABOUT_H__1389DC51_450F_46BE_A962_B406AA7E2065__INCLUDED_)
#define AFX_DLGMYABOUT_H__1389DC51_450F_46BE_A962_B406AA7E2065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMyAbout.h : header file
//
#include "PanaseeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMyAbout dialog

class CDlgMyAbout : public CPanaseeDialog
{
// Construction
public:
	CDlgMyAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMyAbout)
	enum { IDD = IDD_MYABOUTBOX };
	CButton	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMyAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMyAbout)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBitmap m_bmpLogo;
	HBITMAP m_hBmpLogo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYABOUT_H__1389DC51_450F_46BE_A962_B406AA7E2065__INCLUDED_)
