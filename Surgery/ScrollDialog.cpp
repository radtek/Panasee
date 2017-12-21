// ScrollDialogDlg.cpp : implementation file
//
#include "StdAfx.h"
#include "ScrollDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////////////
// CScrollDialog dialog

CScrollDialog::CScrollDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CScrollDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	m_bInsideUpdate = FALSE;
	m_totalDev = CSize(0, 0);
	m_bScrollMode=FALSE;
}

void CScrollDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScrollDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScrollDialog, CDialog)
	//{{AFX_MSG_MAP(CScrollDialog)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollDialog message handlers

BOOL CScrollDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	if(!m_bScrollMode)
	{
		GetWindowRect(&rect);
		m_totalDev=rect.Size();
	}
	
	CPoint baseXY(::GetDialogBaseUnits());
	CPoint pixXY(0,0); 
	m_lineDev.cx = ::MulDiv(5, baseXY.x, 4);
	m_lineDev.cy = ::MulDiv(5, baseXY.y, 8);


	UpdateBars();
			
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScrollDialog::UpdateBars()
{
	if (m_totalDev == CSize(0, 0))
		return;

	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	CSize sizeClient;
	CSize sizeSb;

	// get client rect
	if (!GetTrueClientSize(sizeClient, sizeSb))
	{
		// no room for scroll bars (common for zero sized elements)
		CRect rect;
		GetClientRect(&rect);
		if (rect.right > 0 && rect.bottom > 0)
		{
			// if entire client area is not invisible, assume we have
			//  control over our scrollbars
			EnableScrollBarCtrl(SB_BOTH, FALSE);
		}
		m_bInsideUpdate = FALSE;
		return;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void CScrollDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	UpdateBars();
}

void CScrollDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScrollDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
	//CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CScrollDialog::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;
	CRect rect;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		GetClientRect(&rect);
		x -= rect.Width();
		break;
	case SB_PAGEDOWN:
		GetClientRect(&rect);
		x += rect.Width();
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		GetClientRect(&rect);
		y -= rect.Height();
		break;
	case SB_PAGEDOWN:
		GetClientRect(&rect);
		y += rect.Height();
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}

BOOL CScrollDialog::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollPage(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}


void CScrollDialog::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = GetSystemMetrics(SM_CXVSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= 1; //CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = GetSystemMetrics(SM_CYHSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= 1; //CY_BORDER;
	}
}

BOOL CScrollDialog::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

CPoint CScrollDialog::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	return pt;
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CScrollDialog::GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

void CScrollDialog::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CXTPPropertyPage is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollPage(xOrig - ptDev.x, yOrig - ptDev.y);
}

void CScrollDialog::ScrollPage(int xAmount, int yAmount)
{
	if (xAmount == 0 && yAmount == 0)
		return;

	ScrollWindow(xAmount, yAmount);

	
}

BOOL CScrollDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	// we don't handle anything but scrolling just now
	if (nFlags & (MK_SHIFT | MK_CONTROL))
		return FALSE;

	// we can't get out of it--perform the scroll ourselves
	return DoMouseWheel(nFlags, zDelta, pt);

	//return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CScrollDialog::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	// if we have a vertical scroll bar, the wheel scrolls that
	// if we have _only_ a horizontal scroll bar, the wheel scrolls that
	// otherwise, don't do any work at all

	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	pBar = GetScrollBarCtrl(SB_HORZ);
	BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_HSCROLL);

	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines = 3;

	if (bHasVertBar)
	{
		bResult = OnScrollBy(CSize(0, (zDelta < 0 ? 1 : -1) * m_lineDev.cy * uWheelScrollLines), TRUE);
	}
	else if (bHasHorzBar)
	{
		bResult = OnScrollBy(CSize((zDelta < 0 ? 1 : -1) * m_lineDev.cy * uWheelScrollLines, 0), TRUE);
	}

	if (bResult)
		UpdateWindow();

	return bResult;
}

void CScrollDialog::SetScrollSize(CSize size)
{
	m_bScrollMode=TRUE;
	m_totalDev=size;
	UpdateBars();

	CPoint ptMove;
	ptMove.x=0;
	ptMove.y=0;
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too
}
