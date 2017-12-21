// PanaStatic.cpp : implementation file
//

#include "stdafx.h"
#include "PanaStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaStatic

CPanaStatic::CPanaStatic()
{
	m_cTextColor=RGB(0,0,0);
	m_bkColorEnable=FALSE;
}

CPanaStatic::~CPanaStatic()
{
}


BEGIN_MESSAGE_MAP(CPanaStatic, CStatic)
	//{{AFX_MSG_MAP(CPanaStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaStatic message handlers

BOOL CPanaStatic::OnEraseBkgnd(CDC* pDC)
{	
	return TRUE;
}
void CPanaStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CString sTmp;
	CRect rc;

	GetClientRect(&rc);
	this->GetWindowText(sTmp);

	/*********** ningsj *********************
	说明:使用父窗口的字体
	*****************************************/
	CFont *pParentFont=GetParent()->GetFont(); 
	CFont *pOldFont;
	pOldFont=dc.SelectObject(pParentFont);
	if(m_bkColorEnable==FALSE)
	{
		dc.SetBkMode(TRANSPARENT);
	}else
	{
		dc.FillSolidRect(&rc,m_cTextBKColor);
		dc.SetBkColor(m_cTextBKColor);
	}
	COLORREF oldColor;
	if(this->IsWindowEnabled())
		oldColor = dc.SetTextColor(m_cTextColor);
	else
		oldColor= dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));

	//dc.DrawText(sTmp,rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc.TextOut(0,0,sTmp,sTmp.GetLength());
	dc.SelectObject(pOldFont);
	dc.SetTextColor(oldColor);
	// Do not call CStatic::OnPaint() for painting messages
}

void CPanaStatic::SetTextColor(COLORREF color)
{
	m_cTextColor=color;
}



void CPanaStatic::SetText(CString sText)
{
	SetWindowText(sText);
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);
	GetParent()->InvalidateRect(&rc,FALSE);
	Invalidate(FALSE);
}

CString CPanaStatic::GetText()
{
	CString sText;
	GetWindowText(sText);
	return sText;
}

void CPanaStatic::SetTextBKColor(COLORREF color)
{
	m_bkColorEnable=TRUE;
	m_cTextBKColor=color;
}
