#if !defined(AFX_CRYOTHERAPYPROBESETTING_H__E122C530_5854_455E_922D_85FB26302557__INCLUDED_)
#define AFX_CRYOTHERAPYPROBESETTING_H__E122C530_5854_455E_922D_85FB26302557__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CryotherapyProbeSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCryotherapyProbeSetting dialog
#include "PanaseeDialog.h"

class CCryotherapyProbeSetting : public CPanaseeDialog
{
// Construction
public:
	CCryotherapyProbeSetting(CWnd* pParent = NULL);   // standard constructor
	bool GetProbeDiameter();//Add by Dai Liang 2007.6.5


public:
	CString m_sProbeName;

// Dialog Data
	//{{AFX_DATA(CCryotherapyProbeSetting)
	enum { IDD = IDD_DLG_PROBE_CRY_SETTING };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CComboBox	m_ProbeList;
	UINT	m_nLength;
	//}}AFX_DATA

//	CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryotherapyProbeSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCryotherapyProbeSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();//Add by Dai Liang 2007.6.5
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYOTHERAPYPROBESETTING_H__E122C530_5854_455E_922D_85FB26302557__INCLUDED_)
