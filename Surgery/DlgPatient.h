#if !defined(AFX_DLGPATIENT_H__9D15FFC5_1564_4177_8EB4_2F0AC5CB917D__INCLUDED_)
#define AFX_DLGPATIENT_H__9D15FFC5_1564_4177_8EB4_2F0AC5CB917D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatient.h : header file
//

#include "DlgPatientSkin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPatient dialog

class CDlgPatient : public CDlgPatientSkin
{
// Construction
public:
	void Update();
	CDlgPatient(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPatient)
	enum { IDD = IDD_DLG_PATIENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatient)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatient)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATIENT_H__9D15FFC5_1564_4177_8EB4_2F0AC5CB917D__INCLUDED_)
