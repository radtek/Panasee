#if !defined(AFX_PANALISTCTRL_H__1A2D864A_0667_40E6_8A5C_2BF3AE92843B__INCLUDED_)
#define AFX_PANALISTCTRL_H__1A2D864A_0667_40E6_8A5C_2BF3AE92843B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanaListCtrl.h : header file
//
#include "panaskin.h"
/////////////////////////////////////////////////////////////////////////////
// CPanaListCtrl window

class PANASKIN_API CPanaHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CPanaHeaderCtrl();

	void SetListCtrl(CListCtrl *pListCtrl);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaHeaderCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetHeaderFont(LOGFONT lf);
	virtual ~CPanaHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanaHeaderCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
	CListCtrl *m_pListCtrl;
};

class PANASKIN_API CPanaListCtrl : public CListCtrl
{
// Construction
public:
	CPanaListCtrl();

	void InitHeader();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaListCtrl)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableGridLine(BOOL bGrid);
	void SetColor(COLORREF clSelected,COLORREF clUnselected);
	virtual ~CPanaListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanaListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CPanaHeaderCtrl m_header;
	CFont m_headerFont;
	CFont m_font;
	BOOL m_bMeasured;

	COLORREF m_clSelected,m_clUnselected;

	BOOL m_bGridLine; //是否在每行显示表格线
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANALISTCTRL_H__1A2D864A_0667_40E6_8A5C_2BF3AE92843B__INCLUDED_)
