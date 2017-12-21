#if !defined(AFX_DLGMPRSKIN_H__53549424_BB94_4AE6_939D_4F7CEB6A628D__INCLUDED_)
#define AFX_DLGMPRSKIN_H__53549424_BB94_4AE6_939D_4F7CEB6A628D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMPRSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMPRSkin dialog

class PANASKIN_API CDlgMPRSkin : public CDialog
{
// Construction
public:
	void Set3DShowImageBtn(CImageButton *pBtn);
	CDlgMPRSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMPRSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMPRSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMPRSkin)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMPRSKIN_H__53549424_BB94_4AE6_939D_4F7CEB6A628D__INCLUDED_)
