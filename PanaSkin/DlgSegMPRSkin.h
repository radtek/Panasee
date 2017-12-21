#if !defined(AFX_DLGSEGMPRSKIN_H__48747429_D0C9_439C_BD12_91CD835A8124__INCLUDED_)
#define AFX_DLGSEGMPRSKIN_H__48747429_D0C9_439C_BD12_91CD835A8124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSegMPRSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSegMPRSkin dialog

class PANASKIN_API CDlgSegMPRSkin : public CDialog
{
// Construction
public:
	void SetMax3Btn(CImageButton *pBtn);
	void SetMax2Btn(CImageButton *pBtn);
	void SetMax1Btn(CImageButton *pBtn);
	CDlgSegMPRSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSegMPRSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSegMPRSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSegMPRSkin)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEGMPRSKIN_H__48747429_D0C9_439C_BD12_91CD835A8124__INCLUDED_)
