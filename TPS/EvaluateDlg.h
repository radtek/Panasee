#if !defined(AFX_EVALUATEDLG_H__F682E189_652F_4A5B_8B43_949E793C35EA__INCLUDED_)
#define AFX_EVALUATEDLG_H__F682E189_652F_4A5B_8B43_949E793C35EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvaluateDlg.h : header file
//
#include "resource.h"
#include "VtkHeader.h"
#include "PanaseeDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CEvaluateDlg dialog

class CEvaluateDlg : public CPanaseeDialog
{
// Construction
public:
	CEvaluateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEvaluateDlg();
// Dialog Data
	//{{AFX_DATA(CEvaluateDlg)
	enum { IDD = IDD_DLG_EVALUATE };
	CEdit	m_textCtrl;
	CListCtrl	m_lstProbe;
	CListCtrl	m_lstModal;
	CString	m_sResult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvaluateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEvaluateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBntEvaReturn();
	afx_msg void OnBntEvaRefreshlist();
	afx_msg void OnButtonModalVolume();
	afx_msg void OnButtonProbeVolume();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double GetShellVolume(int iIndex);
	vtkActor *m_pActor ;
private:
	void ShowText(CString sText);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVALUATEDLG_H__F682E189_652F_4A5B_8B43_949E793C35EA__INCLUDED_)
