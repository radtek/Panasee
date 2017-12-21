#if !defined(AFX_PANAEDIT_H__D563C090_BA0C_4A4A_A8E8_99D1582D5FFE__INCLUDED_)
#define AFX_PANAEDIT_H__D563C090_BA0C_4A4A_A8E8_99D1582D5FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanaEdit.h : header file
//
#include "panaskin.h"
/*********** ningsj *********************
 说明:生成panasee风格的编辑框
	  编辑框除边框外，其余部分透明。
*****************************************/
/////////////////////////////////////////////////////////////////////////////
// CPanaEdit window

class PANASKIN_API CPanaEdit : public CEdit
{
// Construction
public:
	CPanaEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaEdit)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanaEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanaEdit)
	afx_msg BOOL OnChange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brHollow;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANAEDIT_H__D563C090_BA0C_4A4A_A8E8_99D1582D5FFE__INCLUDED_)
