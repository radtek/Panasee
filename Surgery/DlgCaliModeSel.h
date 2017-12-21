#if !defined(AFX_DLGCALIMODESEL_H__5AE9F6CD_8AED_4346_8302_651AD7CC91F8__INCLUDED_)
#define AFX_DLGCALIMODESEL_H__5AE9F6CD_8AED_4346_8302_651AD7CC91F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliModeSel.h : header file
//
#include "GlobalFuncs.h"
//#include "XPStyleButtonST.h"
#include "PanaseeDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCaliModeSel dialog

class CDlgCaliModeSel : public CPanaseeDialog
{
// Construction
public:
	CDlgCaliModeSel(CWnd* pParent = NULL);   // standard constructor

	CALIBRATION_TYPE m_CalibrationType;					//校准类型

// Dialog Data
	//{{AFX_DATA(CDlgCaliModeSel)
	enum { IDD = IDD_DLG_CALI_MODE_SEL };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	int		m_iCalMode;
	//}}AFX_DATA

	//CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliModeSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliModeSel)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCaliLast();
	afx_msg void OnRadioCaliManual();
	afx_msg void OnRadioCaliNow();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIMODESEL_H__5AE9F6CD_8AED_4346_8302_651AD7CC91F8__INCLUDED_)
