#if !defined(AFX_PANASTATIC_H__CC549AEC_E371_4CB4_9A84_B8970F6BDDB7__INCLUDED_)
#define AFX_PANASTATIC_H__CC549AEC_E371_4CB4_9A84_B8970F6BDDB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanaStatic.h : header file
//
#include "panaskin.h"
/*********** ningsj *********************
 说明:生成panasee风格的编辑框
	  编辑框除边框外，其余部分透明。
*****************************************/

/////////////////////////////////////////////////////////////////////////////
// CPanaStatic window

class PANASKIN_API CPanaStatic : public CStatic
{
// Construction
public:
	CPanaStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextBKColor(COLORREF color);
	CString GetText();
	void SetText(CString sText);
	void SetTextColor(COLORREF color);
	virtual ~CPanaStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanaStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_cTextColor;
	COLORREF m_cTextBKColor;
	BOOL m_bkColorEnable;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANASTATIC_H__CC549AEC_E371_4CB4_9A84_B8970F6BDDB7__INCLUDED_)
