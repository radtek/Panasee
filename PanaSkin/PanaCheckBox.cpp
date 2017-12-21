// PanaCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "PanaCheckBox.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaCheckBox

//-------------------------------------------------------------------
//
CPanaCheckBox::CPanaCheckBox()
{
	m_bChecked = FALSE;
	m_bMouseDown = FALSE;
	m_bDisabled = FALSE;
	GetColors();
}

CPanaCheckBox::~CPanaCheckBox()
{
}	

BEGIN_MESSAGE_MAP(CPanaCheckBox, CButton)
	//{{AFX_MSG_MAP(CPanaCheckBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaCheckBox message handlers

void CPanaCheckBox::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CPanaCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if(GetParent())
	{		
		CRect rcParent;
		GetWindowRect(&rcParent);
		GetParent()->ScreenToClient(&rcParent);
		GetParent()->SendMessage(WM_PANA_GETPARENTBK,WPARAM(&rcParent),LPARAM(pDC));		
	}

	CRect rectItem(lpDrawItemStruct->rcItem);
	CRect rectClip;
	pDC->GetClipBox(rectClip);

  	UINT nState = lpDrawItemStruct->itemState;
	UINT nStyle = GetStyle();
	BOOL bDisabled = BOOL(nState & ODS_DISABLED);

	// Determine the rect for the check mark.
	CRect rectCheck;
	int nHeight = 14;
	int nAdjust = (rectItem.Height() - nHeight) >> 1;
	if (nStyle & BS_LEFTTEXT)
		rectCheck.SetRect(rectItem.right - nHeight, rectItem.top + nAdjust, 
								rectItem.right, rectItem.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rectItem.left, rectItem.top + nAdjust, 
								rectItem.left + nHeight, rectItem.top + nHeight + nAdjust);

	int nSaveDC = pDC->SaveDC();
	
//	pDC->SelectStockObject(NULL_BRUSH);
//	pDC->FillSolidRect(rectClip, m_crFace);

	//DrawCheckMark(pDC, rectCheck, nStyle, nState);

	if(!m_bDisabled)
		DrawFrameControl(lpDrawItemStruct->hDC,&rectCheck,DFC_BUTTON,DFCS_INACTIVE); //disabled  
	else
	{
		if(m_bChecked)
			DrawFrameControl(lpDrawItemStruct->hDC,&rectCheck,DFC_BUTTON,DFCS_CHECKED);//已选中
		else
			DrawFrameControl(lpDrawItemStruct->hDC,&rectCheck,DFC_BUTTON,DFCS_BUTTONCHECK);//未选中
	}

	// Don't draw more than is necessary.
	if (rectClip != rectItem)
		return;

	// Determine the rect for the text.
	if (nStyle & BS_LEFTTEXT)
		rectItem.left = lpDrawItemStruct->rcItem.left + 2;
	else
		rectItem.left += nHeight + 5;
	rectItem.top++;

	CString sText;
	GetWindowText(sText);
	pDC->SetBkMode(TRANSPARENT);

	CSize size = pDC->GetTextExtent(sText);
  	rectItem.right = rectItem.left + size.cx + (nStyle & BS_LEFTTEXT ? 2 : 0);
	rectItem.top = (rectItem.Height() - size.cy) >> 1;
	rectItem.bottom = rectItem.top + size.cy;

	// Draw the text.
	if (bDisabled)
		pDC->DrawState(rectItem.TopLeft(), rectItem.Size(), sText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
	else
		pDC->DrawText(sText, rectItem, DT_LEFT);

	// Draw the focus rect.
	rectItem.InflateRect(1, 1, 0, 2);

	if (nState & ODS_FOCUS)
		pDC->DrawFocusRect(rectItem); 

	pDC->RestoreDC(nSaveDC);
}	

void CPanaCheckBox::DrawCheckMark(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
{
	CBrush br(nState & ODS_DISABLED ? m_crShadow : m_crDkShadow);
	pDC->FrameRect(rect, &br);
	rect.DeflateRect(1, 1);

	// Draw a 3D rect if the button's not flat.
	if (!(nStyle & BS_FLAT))
	{
		if (m_bMouseDown)
			pDC->Draw3dRect(rect, m_crLiteShadow, m_crHilight);
			//pDC->DrawEdge(rect, BDR_RAISEDINNER/*BDR_SUNKENOUTER*//*BDR_SUNKENINNER*/, /*BF_RIGHTBF_TOP*/BF_LEFT);
		else
			pDC->Draw3dRect(rect, m_crHilight, m_crLiteShadow);

		pDC->SetPixel(CPoint(rect.right - 1, rect.top), m_crFace);
		pDC->SetPixel(CPoint(rect.left, rect.bottom - 1), m_crFace);
	}

	// Draw the check mark.
	if (m_bChecked)
	{
		CPen penDkShadow(PS_SOLID, 1, (nState & ODS_DISABLED ? m_crShadow: m_crDkShadow));
		CPen *pOldPen = (CPen *)pDC->SelectObject(&penDkShadow);
		pDC->MoveTo(rect.left + 1, rect.top + 4);
		pDC->LineTo(rect.left + 4, rect.top + 7);
		pDC->LineTo(rect.left + 12,rect.top - 1);

		pDC->MoveTo(rect.left + 2, rect.top + 4);
		pDC->LineTo(rect.left + 4, rect.top + 6);
		pDC->LineTo(rect.left + 10, rect.top + 0);

		// If not flat, draw the check mark's shadow
		if (!(nStyle & BS_FLAT) && !(nState & ODS_DISABLED))
		{
			CPen pen3DShadow(PS_SOLID, 1, m_crShadow);
			pDC->SelectObject(&pen3DShadow);
			pDC->MoveTo(rect.left + 4, rect.top + 8);
			pDC->LineTo(rect.left + 9, rect.top + 3);
			pDC->LineTo(rect.left + 9, rect.top + 5);

			CPen penLiteShadow(PS_SOLID, 1, m_crLiteShadow);
			pDC->SelectObject(&penLiteShadow);
			pDC->MoveTo(rect.left + 2, rect.top + 6);
			pDC->LineTo(rect.left + 4, rect.top + 8);
			pDC->MoveTo(rect.left + 5, rect.top + 8);
			pDC->LineTo(rect.left + 9, rect.top + 4);

			pDC->SetPixel(rect.left + 11, rect.top + 1, m_crShadow);
			pDC->SetPixel(rect.left + 12, rect.top + 1, m_crLiteShadow);
			pDC->SetPixel(rect.left + 11, rect.top + 2, m_crLiteShadow);
		}

		pDC->SelectObject(pOldPen);
	}
}

void CPanaCheckBox::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
{
	ReleaseCapture();

	if (!(GetStyle() & BS_AUTOCHECKBOX))
		return;

	CRect rect;
	GetClientRect(rect);

	if (rect.PtInRect(point))
		m_bChecked = !m_bChecked;

	m_bMouseDown = FALSE;

	RedrawCheck();
}

void CPanaCheckBox::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
	m_bMouseDown = TRUE;
	SetFocus();
	SetCapture();
	RedrawCheck();
}

void CPanaCheckBox::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}

void CPanaCheckBox::OnMouseMove(UINT /*nFlags*/, CPoint point) 
{
	if (GetCapture() != this)
		return;

	BOOL bWasMouseDown = m_bMouseDown;

	CRect rect;
	GetClientRect(rect);
	m_bMouseDown = rect.PtInRect(point);

	if (bWasMouseDown != m_bMouseDown)
		RedrawCheck();
}

void CPanaCheckBox::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{		
	if (nChar == VK_SPACE)
	{
		m_bMouseDown = FALSE;
		m_bChecked = !m_bChecked;
		RedrawCheck();
	}
}

void CPanaCheckBox::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{
	if (nChar == VK_SPACE)
	{
		m_bMouseDown = TRUE;
		RedrawCheck();
	}
}

void CPanaCheckBox::EnableVisable(bool bVisable)
{
	m_bDisabled = bVisable;
}

void CPanaCheckBox::SetCheck(int nCheck)
{
	BOOL bWasChecked = m_bChecked;

	m_bChecked = nCheck;

	if (m_bChecked != bWasChecked)
		RedrawCheck();
}	

int CPanaCheckBox::GetCheck() const
{
	return m_bChecked;
}

void CPanaCheckBox::RedrawCheck()
{
	CRect rect;
	GetClientRect(rect);
	int nHeight = 12;
	int nAdjust = (rect.Height() - nHeight) >> 1;
	CRect rectCheck;
	// The +2 adjustment is needed because the check mark extends 
	// two pixels beyond the right side of 12x12 square of the check box.
	if (GetStyle() & BS_LEFTTEXT)
		rectCheck.SetRect(rect.right - nHeight, rect.top + nAdjust, 
								rect.right + 2, rect.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rect.left, rect.top + nAdjust, 
								rect.left + nHeight + 2, rect.top + nHeight + nAdjust);

	InvalidateRect(rectCheck);
}

void CPanaCheckBox::GetColors()
{
	m_crFace = ::GetSysColor(COLOR_3DFACE);
	m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);
	m_crDkShadow	 = ::GetSysColor(COLOR_3DDKSHADOW);

	// Create some intermediate colors. This is done by averaging two colors.
	BYTE byRedDark = GetRValue(m_crShadow);
	BYTE byRedLite = GetRValue(m_crFace);
	BYTE byGreenDark = GetGValue(m_crShadow);
	BYTE byGreenLite = GetGValue(m_crFace);
	BYTE byBlueDark = GetBValue(m_crShadow);
	BYTE byBlueLite = GetBValue(m_crFace);

	m_crLiteShadow = RGB(byRedLite + ((byRedDark - byRedLite) >> 1),
					 byGreenLite + ((byGreenDark - byGreenLite) >> 1),
					 byBlueLite + ((byBlueDark - byBlueLite) >> 1));

}	



