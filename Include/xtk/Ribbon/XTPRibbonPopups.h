// XTPRibbonBar.h: interface for the CXTPRibbonBar class.
//
// This file is a part of the XTREME RIBBON MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPRIBBONPOPUPS_H__)
#define __XTPRIBBONPOPUPS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPRibbonTheme;
class CXTPRibbonQuickAccessControls;
class CXTPRibbonTab;
class CXTPRibbonControlTab;
class CXTPTabPaintManager;
class CXTPRibbonBar;
class CXTPRibbonGroup;
class CXTPOffice2007FrameHook;
class CXTPRibbonTabContextHeaders;
class CXTPRibbonGroups;

#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"

class CXTPRibbonScrollableBar
{
public:
	CXTPRibbonScrollableBar();
private:
	class CControlGroupsScroll;

protected:
	void InitScrollableBar(CXTPCommandBar* pParent);

	void ShowScrollableRect(CXTPRibbonGroups* pGroups, CRect rc);

public:
	void EnableGroupsScroll(BOOL bScrollLeft, BOOL bScrollRight);
	virtual void OnGroupsScroll(BOOL bScrollLeft) = 0;
	virtual CRect GetGroupsRect() const = 0;
	virtual CXTPRibbonGroup* HitTestGroup(CPoint point) const = 0;

	void CreateGroupKeyboardTips(CXTPRibbonTab* pSelectedTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get highlighted group
	// Returns:
	//     CXTPRibbonGroup object under mouse cursor
	// See Also: CXTPRibbonGroup
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* GetHighlightedGroup() const;

	void HighlightGroup(CXTPRibbonGroup* pHighlightedGroup);

protected:
	void CreateControlKeyboardTips(CXTPControl* pControl, LPCTSTR lpszPrefix, BOOL bVisible);

public:
	int m_nGroupsScrollPos;         // Groups scroll position

protected:
	CXTPCommandBar* m_pParent;
	CXTPControl* m_pControlScrollGroupsLeft;    // Control to draw left scroll
	CXTPControl* m_pControlScrollGroupsRight;   // Control to draw right scroll
	CXTPRibbonGroup* m_pHighlightedGroup;       // Currently highlighted group
};

CXTPRibbonScrollableBar* AFX_CDECL GetScrollableBar(CXTPCommandBar* pCommandBar);

//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPRibbonTabPopupToolBar : public CXTPPopupToolBar, public CXTPRibbonScrollableBar
{
	DECLARE_DYNAMIC(CXTPRibbonTabPopupToolBar)
public:
	CXTPRibbonTabPopupToolBar(CXTPRibbonTab* pRibbonTab);

	~CXTPRibbonTabPopupToolBar();

public:
	CRect GetGroupsRect() const;
	void OnGroupsScroll(BOOL bScrollLeft);
	void EnsureVisible(CXTPControl* pControl);
	void CreateKeyboardTips();
	CSize CalcDynamicLayout(int, DWORD /*nMode*/);
	void DrawCommandBar(CDC* pDC, CRect rcClipBox);
	virtual CSize GetButtonSize() const;
	virtual CSize GetIconSize() const;
	virtual CRect CalculatePopupRect(CSize sz);
	CXTPRibbonGroup* HitTestGroup(CPoint point) const;
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	void OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip);
	BOOL IsKeyboardCuesVisible() const;
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

public:
	CXTPRibbonTab* m_pRibbonTab;
	CXTPRibbonBar* m_pRibbonBar;
	CRect m_rcGroups;

};

class CXTPRibbonBarMorePopupToolBar : public CXTPPopupToolBar
{
	DECLARE_DYNAMIC(CXTPRibbonBarMorePopupToolBar)

public:
	CXTPRibbonBarMorePopupToolBar(CXTPRibbonBar* pRibbonBar);

public:
	void FillCommandBarEntry(CDC* pDC);
	CSize GetIconSize() const;
	CSize GetButtonSize() const;

public:
	CXTPRibbonBar* m_pRibbonBar;
};


class CXTPRibbonGroupPopupToolBar : public CXTPPopupToolBar, public CXTPRibbonScrollableBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonGroupPopupToolBar)
public:
	CXTPRibbonGroupPopupToolBar(CXTPRibbonGroup* pRibbonGroup = NULL);
	~CXTPRibbonGroupPopupToolBar();

public:
	CSize CalcDynamicLayout(int, DWORD /*nMode*/);
	void DrawCommandBar(CDC* pDC, CRect rcClipBox);
	void CreateKeyboardTips();
	CSize GetIconSize() const;
	CSize GetButtonSize() const;
	virtual void OnGroupsScroll(BOOL bScrollLeft);
	virtual CRect GetGroupsRect() const;
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	virtual CXTPRibbonGroup* HitTestGroup(CPoint point) const;
	BOOL IsKeyboardCuesVisible() const;
protected:
	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

public:
	CXTPRibbonGroup* m_pRibbonGroup;
	CXTPRibbonBar* m_pRibbonBar;

	CXTPRibbonGroup* m_pParentGroup;
};


//}}AFX_CODEJOCK_PRIVATE


#endif // !defined(__XTPRIBBONPOPUPS_H__)
