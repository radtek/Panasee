#if !defined(AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_)
#define AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacRadioButton.h : header file
//

#include "panaskin.h"
/////////////////////////////////////////////////////////////////////////////
// CPanaRadioButton window

class PANASKIN_API CPanaRadioButton : public CButton
{
	DECLARE_DYNAMIC(CPanaRadioButton)

// Construction
public:
	CPanaRadioButton();

// Attributes
public:

// Operations
public:
	void EnableVisable(bool bVisable);
	void SetCheck(int nCheck);
	int GetCheck() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaRadioButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanaRadioButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanaRadioButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void DrawRadio(CDC *pDC, CRect rect, UINT nStyle, UINT nState);
	void GetColors();
	void UncheckOtherRadios();
	BOOL m_bMouseDown;
	void RedrawCheck();
	BOOL m_bChecked;
	BOOL m_bDisabled;

	COLORREF m_crFace;
	COLORREF m_crShadow;
	COLORREF m_crHilight;
	COLORREF m_crDkShadow;
	COLORREF m_crLiteShadow;
	COLORREF m_crLiteFace;
	COLORREF m_crDarkDkShadow;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_)
