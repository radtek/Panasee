#if !defined(AFX_BIOPSYPROBESETTING_H__F4B85CE0_3FE7_4BF6_9263_A4C9F30DFF96__INCLUDED_)
#define AFX_BIOPSYPROBESETTING_H__F4B85CE0_3FE7_4BF6_9263_A4C9F30DFF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BiopsyProbeSetting.h : header file
//
#include "PanaseeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CBiopsyProbeSetting dialog

class CBiopsyProbeSetting : public CPanaseeDialog
{
// Construction
public:
	CBiopsyProbeSetting(CWnd* pParent = NULL);   // standard constructor
	CString m_sProbeName;

//	int m_iNumProbeCreated; // Number of probe ever created.
// Dialog Data
	//{{AFX_DATA(CBiopsyProbeSetting)
	enum { IDD = IDD_DLG_PROBE_BIO_SETTING };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CComboBox	m_ProbeList;
	UINT	m_nLength;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiopsyProbeSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool GetProbeDiameter();
	// Generated message map functions
	//{{AFX_MSG(CBiopsyProbeSetting)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOPSYPROBESETTING_H__F4B85CE0_3FE7_4BF6_9263_A4C9F30DFF96__INCLUDED_)
