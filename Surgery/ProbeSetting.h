#if !defined(AFX_PROBESETTING_H__32D327E4_D7B8_4596_A4BC_970C81B10603__INCLUDED_)
#define AFX_PROBESETTING_H__32D327E4_D7B8_4596_A4BC_970C81B10603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProbeSetting dialog

class CProbeSetting : public CDialog
{
// Construction
public:
	CProbeSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProbeSetting)
	enum { IDD = IDD_DLG_CRYPTHERAPY_PROBE_SETTING };
	int		m_nProbeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbeSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProbeSetting)
	afx_msg void OnRadioPreScanningProbe();
	afx_msg void OnRadioBiopsyProbe();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBESETTING_H__32D327E4_D7B8_4596_A4BC_970C81B10603__INCLUDED_)
