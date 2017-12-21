#if !defined(AFX_DLGBASESELECTION_H__A596DEC5_AE01_4453_9EE2_DFE42B5AB16B__INCLUDED_)
#define AFX_DLGBASESELECTION_H__A596DEC5_AE01_4453_9EE2_DFE42B5AB16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBaseSelection.h : header file
//
#include "XPStyleButtonST.h"
#include "PanaseeDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgBaseSelection dialog

class CDlgBaseSelection : public CPanaseeDialog
{
// Construction
public:
	CDlgBaseSelection(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBaseSelection)
	enum { IDD = IDD_DLG_BASESELECTION };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CXPStyleButtonST	m_btnBase;
	int		m_iRadioBase;  //记录选择的Base的索引，默认为第一个，即0
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBaseSelection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBaseSelection)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnRadioBase1();
	afx_msg void OnRadioBase2();
	afx_msg void OnRadioBase3();
	afx_msg void OnRadioBase4();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnConfirm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	CBitmap m_bmpBase[4];
	int	m_iImgID;

	void UpdateBaseStatus();
	BOOL m_bDisableTimer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBASESELECTION_H__A596DEC5_AE01_4453_9EE2_DFE42B5AB16B__INCLUDED_)
