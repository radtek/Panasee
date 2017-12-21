// CPanaRadioButton.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "PanaRadioButton.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Funtion prototypes.
void LightenColorref(COLORREF &crColor, BYTE byIncreaseVal);
void DarkenColorref(COLORREF &crColor, BYTE byReduceVal);

//-------------------------------------------------------------------
//
void LightenColorref(COLORREF &crColor, BYTE byIncreaseVal)
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if ((byRed + byIncreaseVal) <= 255)
		byRed = BYTE(byRed + byIncreaseVal);
	if ((byGreen + byIncreaseVal)	<= 255)
		byGreen = BYTE(byGreen + byIncreaseVal);
	if ((byBlue + byIncreaseVal) <= 255)
		byBlue = BYTE(byBlue + byIncreaseVal);

	crColor = RGB(byRed, byGreen, byBlue);
}	// LightenColorref

void DarkenColorref(COLORREF &crColor, BYTE byReduceVal)
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if (byRed >= byReduceVal)
		byRed = BYTE(byRed - byReduceVal);
	if (byGreen >= byReduceVal)
		byGreen = BYTE(byGreen - byReduceVal);
	if (byBlue >= byReduceVal)
		byBlue = BYTE(byBlue - byReduceVal);

	crColor = RGB(byRed, byGreen, byBlue);
}	// DarkenColorref


/////////////////////////////////////////////////////////////////////////////
// CPanaRadioButton

IMPLEMENT_DYNAMIC(CPanaRadioButton, CButton)

//-------------------------------------------------------------------
//
CPanaRadioButton::CPanaRadioButton()
{
	m_bChecked = FALSE;
	m_bMouseDown = FALSE;
	m_bDisabled = FALSE;

	GetColors();
}

CPanaRadioButton::~CPanaRadioButton()
{
}	

BEGIN_MESSAGE_MAP(CPanaRadioButton, CButton)
	//{{AFX_MSG_MAP(CPanaRadioButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaRadioButton message handlers

void CPanaRadioButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}	// PreSubclassWindow

void CPanaRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
	int nHeight = 12;
	int nAdjust = (rectItem.Height() - nHeight) >> 1;
	if (nStyle & BS_LEFTTEXT)
		rectCheck.SetRect(rectItem.right - nHeight, rectItem.top + nAdjust, 
								rectItem.right, rectItem.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rectItem.left, rectItem.top + nAdjust, 
								rectItem.left + nHeight, rectItem.top + nHeight + nAdjust);

	int nSaveDC = pDC->SaveDC();
	
// 	pDC->SelectStockObject(NULL_BRUSH);
// 	pDC->FillSolidRect(rectClip, m_crFace);

	DrawRadio(pDC, rectCheck, nStyle, nState);

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

void CPanaRadioButton::DrawRadio(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
{
	// Give the user a visual indication that the button is pressed by darkening some colors.
	BOOL bDarkened = FALSE;
	if (m_bMouseDown && !(nStyle & BS_FLAT))
	{
		DarkenColorref(m_crFace, 40);
		DarkenColorref(m_crShadow, 40);
		DarkenColorref(m_crHilight, 40);
		DarkenColorref(m_crLiteShadow, 40);
		DarkenColorref(m_crLiteFace, 40);
		bDarkened = TRUE;
	}

	// Draw the outer round rect.
	CPen penDkShadow(PS_SOLID, 1, m_crDkShadow);
	CPen *pOldPen = (CPen *)pDC->SelectObject(&penDkShadow);
	if (nStyle & BS_FLAT)
	{
		CBrush br(m_bMouseDown ? m_crFace : m_crHilight);
		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&br);
		pDC->RoundRect(rect, CPoint(10, 10));
		pDC->SelectObject(pOldBrush);
	}
	else
		pDC->RoundRect(rect, CPoint(10, 10));

	// Draw some shadowing
	if (!(nState & ODS_DISABLED) && !(nStyle & BS_FLAT))
	{
		int nAdjust = (m_bChecked || m_bMouseDown ? -11 : 0);
		int nSign = (m_bChecked || m_bMouseDown ? -1 : 1);

		CPen penDarkDkShadow(PS_SOLID, 1, m_crDarkDkShadow);
		pDC->SelectObject(&penDarkDkShadow);

		pDC->MoveTo(rect.left + 0, rect.top + 4);
		pDC->LineTo(rect.left + 5, rect.top - 1);
		pDC->MoveTo(rect.left + 7, rect.top);
		pDC->LineTo(rect.left + 12, rect.top + 5);
		pDC->MoveTo(rect.left + 11, rect.top + 7);
		pDC->LineTo(rect.left + 6, rect.top + 12);
		pDC->MoveTo(rect.left + 4, rect.top + 11);
		pDC->LineTo(rect.left - 1, rect.top + 6);

		pDC->SetPixel(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 2)), m_crFace);

		CPen penLiteFace(PS_SOLID, 1, m_crLiteFace);
		pDC->SelectObject(&penLiteFace);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 1)), rect.top + (nSign * (nAdjust + 6)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 1)), rect.top + (nSign * (nAdjust + 4)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 1)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 1)));
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 3)), m_crLiteFace);

		CPen penLite(PS_SOLID, 1, m_crHilight);
		pDC->SelectObject(&penLite);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 7)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 4)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 4)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 5)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 2)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 8)), rect.top + (nSign * (nAdjust + 2)));
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 5)), m_crHilight);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 5)), rect.top + (nSign * (nAdjust + 3)), m_crHilight);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 6)), m_crLiteFace);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 3)), m_crLiteFace);

		pDC->SelectObject(&penLiteFace);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 8)));
  		pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 1)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 8)));
  		pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 2)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 8)));
  		pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 3)));

		CPen penLiteShadow(PS_SOLID, 1, m_crLiteShadow);
		pDC->SelectObject(&penLiteShadow);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 9))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 5))); 
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 9))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 6))); 

		CPen penShadow(PS_SOLID, 1, m_crShadow);
		pDC->SelectObject(&penShadow);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 10)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 10))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 7))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 3))); 
	}	// if not disabled and not flat

	// Draw the check if necessary.
	if (m_bChecked)
	{
		CBrush br(nState & ODS_DISABLED ? m_crShadow : m_crDkShadow);
		CBrush *pOldBrush = pDC->SelectObject(&br);
		CPen pen(PS_SOLID, 1, (nState & ODS_DISABLED ? m_crShadow : m_crDkShadow));
		pDC->SelectObject(&pen);
		rect.DeflateRect(3, 3, 3, 3);
		pDC->RoundRect(rect, CPoint(2, 2));
		pDC->SelectObject(pOldBrush);
	}

	// Lighten the colors if they were darkened.
	if (bDarkened)
	{
		LightenColorref(m_crFace, 40);
		LightenColorref(m_crShadow, 40);
		LightenColorref(m_crHilight, 40);
		LightenColorref(m_crLiteShadow, 40);
		LightenColorref(m_crLiteFace, 40);
	}

	pDC->SelectObject(pOldPen);
}	

BOOL CPanaRadioButton::PreTranslateMessage(MSG* pMsg) 
{
//	if (GetCapture() == this)
//		return CButton::PreTranslateMessage(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP ||
			 pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
		{
			UncheckOtherRadios();

			// Get the previous/next button and check it.
			CWnd *pParent = GetParent();
			ASSERT(pParent);

			CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this, (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP ? TRUE : FALSE));
			if (pWnd && (pWnd->GetStyle() & BS_AUTORADIOBUTTON))
			{
				if (pWnd->IsKindOf(RUNTIME_CLASS(CPanaRadioButton)))
					((CPanaRadioButton*)pWnd)->SetCheck(1);
				else if (pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
					((CButton*)pWnd)->SetCheck(1);
			}
			SetCheck(0);
			return FALSE;
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}	

void CPanaRadioButton::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
{
	ReleaseCapture();

	CRect rect;
	GetClientRect(rect);
	if (rect.PtInRect(point))
	{
		if (!m_bChecked)
			m_bChecked = TRUE;

		// Uncheck all other radios in the group
		if ((GetStyle() & BS_AUTORADIOBUTTON) && m_bChecked)
		{
			UncheckOtherRadios();
		}
	}

	m_bMouseDown = FALSE;

	RedrawCheck();

//	CButton::OnLButtonUp(nFlags, point);
}	

void CPanaRadioButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
	m_bMouseDown = TRUE;
	SetFocus();
	SetCapture();
	RedrawCheck();
//	CButton::OnLButtonDown(nFlags, point);
}	

void CPanaRadioButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture() != this)
		return;

	BOOL bWasMouseDown = m_bMouseDown;

	CRect rect;
	GetClientRect(rect);
	m_bMouseDown = rect.PtInRect(point);

	if (bWasMouseDown != m_bMouseDown)
		RedrawCheck();

	CButton::OnMouseMove(nFlags, point);
}	

void CPanaRadioButton::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 

{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}	

void CPanaRadioButton::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{
	if (nChar == VK_SPACE)
	{
		m_bMouseDown = FALSE;
		RedrawCheck();
	}
}	

void CPanaRadioButton::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{
	if (nChar == VK_SPACE)
	{
		if (!m_bMouseDown)
		{
			m_bMouseDown = TRUE;
			RedrawCheck();
		}
	}
}	

void CPanaRadioButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	
	GetColors();
}	

void CPanaRadioButton::EnableVisable(bool bVisable)
{
	m_bDisabled = bVisable;
}

void CPanaRadioButton::SetCheck(int nCheck)
{
	BOOL bWasChecked = m_bChecked;

	m_bChecked = nCheck;

	if (m_bChecked != bWasChecked)
		RedrawCheck();
}

int CPanaRadioButton::GetCheck() const
{
	return m_bChecked;
}	

void CPanaRadioButton::RedrawCheck()
{
	CRect rect;
	GetClientRect(rect);
	int nHeight = 12;
	int nAdjust = (rect.Height() - nHeight) >> 1;
	CRect rectCheck;
	if (GetStyle() & BS_LEFTTEXT)
		rectCheck.SetRect(rect.right - nHeight, rect.top + nAdjust, 
								rect.right/* + 2*/, rect.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rect.left, rect.top + nAdjust, 
								rect.left + nHeight/* + 2*/, rect.top + nHeight + nAdjust);

	InvalidateRect(rectCheck);
}	

void CPanaRadioButton::UncheckOtherRadios()
{
	// Go backwards (GetNextDlgItem will wrap around to the end of the group).
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this, TRUE);
	while (pWnd && pWnd != (CWnd *)this)
	{
		if (pWnd->GetStyle() & BS_AUTORADIOBUTTON)
		{
			if (pWnd->IsKindOf(RUNTIME_CLASS(CPanaRadioButton)))
				((CPanaRadioButton*)pWnd)->SetCheck(0);
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
				((CButton*)pWnd)->SetCheck(0);
		}
		pWnd = pParent->GetNextDlgGroupItem(pWnd, TRUE);
	}
}

void CPanaRadioButton::GetColors()
{
	m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);
	m_crFace = ::GetSysColor(COLOR_3DFACE);
	m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_crLiteFace = ::GetSysColor(COLOR_3DLIGHT);
	m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);

	// Create some intermediate colors. This is done by averaging two colors.
	BYTE byRedFace = GetRValue(m_crFace);
	BYTE byGreenFace = GetGValue(m_crFace);
	BYTE byBlueFace = GetBValue(m_crFace);
	BYTE byRedShadow = GetRValue(m_crShadow);
	BYTE byGreenShadow = GetGValue(m_crShadow);
	BYTE byBlueShadow = GetBValue(m_crShadow);
	BYTE byRedDkShadow = GetRValue(m_crDkShadow);
	BYTE byGreenDkShadow = GetGValue(m_crDkShadow);
	BYTE byBlueDkShadow = GetBValue(m_crDkShadow);

	m_crLiteShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
										 byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
										 byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

	m_crDarkDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
											byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
											byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
}