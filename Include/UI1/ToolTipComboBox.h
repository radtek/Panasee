// ToolTipComboBox.h: interface for the CToolTipComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPCOMBOBOX_H__7DD42368_89F4_4E4A_9A75_BD8920EE1009__INCLUDED_)
#define AFX_TOOLTIPCOMBOBOX_H__7DD42368_89F4_4E4A_9A75_BD8920EE1009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolTipListBox.h"

class _declspec(dllexport) CToolTipComboBox : public CComboBox  
{
public:
	CToolTipComboBox();
	virtual ~CToolTipComboBox();
	virtual void PreSubclassWindow();
protected:
	HWND m_hWndToolTip;
	TOOLINFO m_ToolInfo;
	CToolTipListBox	m_listbox;
protected:
	//{{AFX_MSG(CToolTipListBox)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TOOLTIPCOMBOBOX_H__7DD42368_89F4_4E4A_9A75_BD8920EE1009__INCLUDED_)
