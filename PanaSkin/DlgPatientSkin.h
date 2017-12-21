#if !defined(AFX_DLGPATIENTSKIN_H__6736CBE2_9CA2_459A_9C09_E16B1F3502E4__INCLUDED_)
#define AFX_DLGPATIENTSKIN_H__6736CBE2_9CA2_459A_9C09_E16B1F3502E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatientSkin.h : header file
//
#include "panaskin.h"
#include "MemDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPatientSkin dialog

class PANASKIN_API CDlgPatientSkin : public CDialog
{
// Construction
public:
	CDlgPatientSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	void SetName(CString sName);
	void SetID(CString sID);
	void SetSex(CString sSex);
	void SetAge(CString sAge);
	void SetWeight(CString sWeight);
	void SetDesc(CString sDesc);

// Dialog Data
	//{{AFX_DATA(CDlgPatientSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatientSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatientSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	CMemDC m_skinDC;

	CString m_sName,m_sAge,m_sSex,m_sID,m_sWeight,m_sDesc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATIENTSKIN_H__6736CBE2_9CA2_459A_9C09_E16B1F3502E4__INCLUDED_)
