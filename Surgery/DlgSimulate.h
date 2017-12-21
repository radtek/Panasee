#if !defined(AFX_DLGSIMULATE_H__A6DE6BA1_F26D_427C_AD03_E7E310A6F6EC__INCLUDED_)
#define AFX_DLGSIMULATE_H__A6DE6BA1_F26D_427C_AD03_E7E310A6F6EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSimulate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSimulate dialog
#include "CryotherapyDlg.h"
class CTrackTool;
class CProbe;

class CDlgSimulate : public CDialog
{
// Construction
public:
	CDlgSimulate(CWnd* pParent = NULL);   // standard constructor

	CCryotherapyDlg* m_pCryotherapyDlg;
	CTrackTool* m_pTrackTool;
	CProbe* m_pProbe;

// Dialog Data
	//{{AFX_DATA(CDlgSimulate)
	enum { IDD = IDD_DIG_SIMULATE };
	double	m_dbIceballOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSimulate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSimulate)
	afx_msg void OnSimulate();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSIMULATE_H__A6DE6BA1_F26D_427C_AD03_E7E310A6F6EC__INCLUDED_)
