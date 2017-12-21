#if !defined(AFX_DLGIMAGECONTROLSKIN_H__800DD6E0_D2AA_4E96_A400_E11315FF6FC1__INCLUDED_)
#define AFX_DLGIMAGECONTROLSKIN_H__800DD6E0_D2AA_4E96_A400_E11315FF6FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageControlSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageControlSkin dialog
/*********** ningsj *********************
 ËµÃ÷:Í¼ÏñÇÐ»»´°¿ÚÆ¤·ô
*****************************************/
class PANASKIN_API CDlgImageControlSkin : public CDialog
{
// Construction
public:
	CDlgImageControlSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	void SetFirstBtn(CImageButton *pBtn);
	void SetLastBtn(CImageButton *pBtn);
	void SetPreBtn(CImageButton *pBtn);
	void SetNextBtn(CImageButton *pBtn);
	void SetThreeDBtn(CImageButton *pBtn);
	void SetMaxBtn(CImageButton *pBtn);
// Dialog Data
	//{{AFX_DATA(CDlgImageControlSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageControlSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageControlSkin)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageButton *m_btnFirst,*m_btnLast,*m_btnPre,*m_btnNext,*m_btn3D,*m_btnMax;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGECONTROLSKIN_H__800DD6E0_D2AA_4E96_A400_E11315FF6FC1__INCLUDED_)
