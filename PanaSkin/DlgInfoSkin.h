#if !defined(AFX_DLGINFOSKIN_H__DC341794_FB4A_424F_9F38_04CD5DC2EAF6__INCLUDED_)
#define AFX_DLGINFOSKIN_H__DC341794_FB4A_424F_9F38_04CD5DC2EAF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInfoSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "PanaStatic.h"
#include "MemDc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgInfoSkin dialog

class PANASKIN_API CDlgInfoSkin : public CDialog
{
// Construction
public:
	CDlgInfoSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	void SetNextBtn(CImageButton *pBtn);
	void SetPreBtn(CImageButton *pBtn);
	void SetPromptStatic(CPanaStatic *pStatic);
	void SetBaseVisable(BOOL bVisable);
	void SetProbeVisable(BOOL bVisable);
// Dialog Data
	//{{AFX_DATA(CDlgInfoSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfoSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInfoSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	CImageButton *m_btnPre,*m_btnNext;
	BOOL m_bProbeVisable,m_bBaseVisable;
	CPanaStatic *m_sPrompt;
	CMemDC m_skinDC;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFOSKIN_H__DC341794_FB4A_424F_9F38_04CD5DC2EAF6__INCLUDED_)
