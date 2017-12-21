// PanaEdit.cpp : implementation file
//

#include "stdafx.h"
#include "PanaEdit.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaEdit


CPanaEdit::CPanaEdit()
{
	m_brHollow.CreateStockObject(HOLLOW_BRUSH);	
}

CPanaEdit::~CPanaEdit()
{
	
}


BEGIN_MESSAGE_MAP(CPanaEdit, CEdit)
	//{{AFX_MSG_MAP(CPanaEdit)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaEdit message handlers


BOOL CPanaEdit::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	Invalidate();
	return FALSE;
}

void CPanaEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CEdit::OnLButtonUp(nFlags, point);
}

HBRUSH CPanaEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	
	pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(RGB(255,0,0));
	return m_brHollow;
}

void CPanaEdit::OnPaint() 
{
	CPaintDC dc(this);

	int iDCSave=dc.SaveDC();

	if(GetParent())
	{
		
		CRect rcParent;
		GetWindowRect(&rcParent);
		GetParent()->ScreenToClient(&rcParent);
		GetParent()->SendMessage(WM_PANA_GETPARENTBK,WPARAM(&rcParent),LPARAM(&dc));
		
	}

	CRect rc,rcParent;
	GetClientRect(&rc);

	CRgn rgn;
	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);
	dc.SelectClipRgn(&rgn,RGN_COPY);

	CString sTmp;
	int iSelStart,iSelEnd;
	GetWindowText(sTmp);
	GetSel(iSelStart,iSelEnd);

	dc.SetBkMode(TRANSPARENT);
	CFont *pOldFont;
	pOldFont=dc.SelectObject(GetFont());
	
	COLORREF oldColor;

	if(iSelStart==sTmp.GetLength())
	{
		if(!this->IsWindowEnabled())
			oldColor=dc.SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
		else
			oldColor=dc.SetTextColor(RGB(0,0,0));

		dc.TextOut(0,0,sTmp,sTmp.GetLength());
		dc.SetTextColor(oldColor);
	}else
	{
		CSize  size;	
		int x=0;
		CString sOutput;
		if(iSelStart>0)
		{
			sOutput=sTmp.Left(iSelStart);
			size=dc.GetTextExtent(sOutput);
			oldColor=dc.SetTextColor(RGB(0,0,0));
			dc.TextOut(0,0,sOutput,sOutput.GetLength());
			sTmp=sTmp.Right(sTmp.GetLength()-iSelStart);
			iSelEnd-=iSelStart;
			iSelStart=0;
			x=size.cx;
		}
		if(iSelEnd<=sTmp.GetLength())
		{
			COLORREF oldBkColor;
			oldBkColor=dc.SetBkColor(RGB(49,106,197));
			dc.SetBkMode(OPAQUE);
			dc.SetTextColor(RGB(255,255,255));
			sOutput=sTmp.Left(iSelEnd);
			size=dc.GetTextExtent(sOutput);
			dc.TextOut(x,0,sOutput,sOutput.GetLength());
			x+=size.cx;
			sTmp=sTmp.Right(sTmp.GetLength()-iSelEnd);
			dc.SetBkColor(oldBkColor);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0,0,0));
		}
		
		dc.TextOut(x,0,sTmp,sTmp.GetLength());
		dc.SetTextColor(oldColor);

	}

	dc.SelectObject(pOldFont);
	dc.RestoreDC(iDCSave);
	//dc.FillSolidRect(rc,RGB(255,0,0));
}


void CPanaEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetSel(-1,0);
	Invalidate(FALSE);
	CEdit::OnLButtonDown(nFlags, point);
}

void CPanaEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
