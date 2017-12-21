// ToolTipListBox.h: interface for the CToolTipListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPLISTBOX_H__38E1A7C4_AC93_4479_BD73_A0727D12DE01__INCLUDED_)
#define AFX_TOOLTIPLISTBOX_H__38E1A7C4_AC93_4479_BD73_A0727D12DE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _declspec(dllexport) CToolTipListBox : public CListBox  
{
public:
	CToolTipListBox();
	virtual ~CToolTipListBox();

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	HWND			m_hWndToolTip;
//	CToolTipCtrl m_toolTip;
	TOOLINFO		m_ToolInfo;
	
protected:
	//{{AFX_MSG(CToolTipListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TOOLTIPLISTBOX_H__38E1A7C4_AC93_4479_BD73_A0727D12DE01__INCLUDED_)
