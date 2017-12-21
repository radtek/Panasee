// PanaListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PanaListCtrl.h"
#include "ShowImage.h"
#include "Resource.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaListCtrl

CPanaHeaderCtrl::CPanaHeaderCtrl()
{
	LOGFONT lf;
	lf.lfHeight=-15;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_BOLD;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=DEFAULT_CHARSET; //ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Tahoma");

	m_font.CreateFontIndirect(&lf);

	m_pListCtrl=NULL;
}

CPanaHeaderCtrl::~CPanaHeaderCtrl()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPanaHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CPanaHeaderCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHeaderCtrl message handlers

void CPanaHeaderCtrl::SetListCtrl(CListCtrl *pListCtrl)
{
	m_pListCtrl=pListCtrl;
}

void CPanaHeaderCtrl::OnPaint() 
{
	
	CPaintDC pDC(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	CMemDC dc;
	dc.LoadDC(&pDC,rc);

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_HEADERCTRL_SKIN,2);
	memImage.SetSizingMargin(2,2,2,2);

	memImage.SetShowRegion(rc);
	memImage.SetTransparentColor(RGB(0,0,0));
	memImage.DrawImage(&dc,1);

	int    nCount = GetItemCount();
	HDITEM hdi;
	TCHAR  lpBuffer[256];

	hdi.mask = HDI_TEXT | HDI_WIDTH;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;

	dc.SetBkMode(TRANSPARENT);
	
	CFont *pOldFont;
	pOldFont=dc.SelectObject(&m_font);
	

	for(int i=0;i<nCount;i++)
	{
		GetItem(i, &hdi);
		rc.right=rc.left+hdi.cxy;
		if(m_pListCtrl)
		{
			LVCOLUMN col;
			col.mask=LVCF_FMT;
			m_pListCtrl->GetColumn(i, &col);
		
			if(col.fmt & LVCFMT_LEFT)
				dc.DrawText(hdi.pszText,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
			else if(col.fmt & LVCFMT_RIGHT)
				dc.DrawText(hdi.pszText,-1,&rc,DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
			else if(col.fmt & LVCFMT_CENTER)
				dc.DrawText(hdi.pszText,-1,&rc,DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
			else
			{
				rc.left+=2;
				dc.DrawText(hdi.pszText,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
			}
		}else
		{
		
			dc.DrawText(hdi.pszText,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}

		rc.left=rc.right;
	}

	dc.SelectObject(pOldFont);
	

}

CPanaListCtrl::CPanaListCtrl()
{
	LOGFONT lf;
	lf.lfHeight=-15;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=DEFAULT_CHARSET; //ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Tahoma");

	m_font.CreateFontIndirect(&lf);
	m_headerFont.CreateFontIndirect(&lf); 

	m_bMeasured=FALSE;

	m_clSelected=RGB(200,250,255);
	m_clUnselected=RGB(255,255,255);

	m_bGridLine=TRUE;
}

CPanaListCtrl::~CPanaListCtrl()
{
	m_headerFont.DeleteObject();
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPanaListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CPanaListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaListCtrl message handlers
void CPanaListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
	LPDRAWITEMSTRUCT lpDIS=lpDrawItemStruct;
	
	// drawn device context for draw menu
	CDC dc;
	dc.Attach(lpDIS->hDC);

	// rect region of device context
	CRect rect=CRect(lpDIS->rcItem);


	

	BOOL bFocus=(GetFocus()==this);
	int nItem=lpDrawItemStruct->itemID;
	// get item data

	_TCHAR szBuff[MAX_PATH];
	LVITEM lvi;
	lvi.mask=LVIF_TEXT | HDI_WIDTH | LVIF_STATE;
	lvi.iItem=nItem;
	lvi.iSubItem=0;
	lvi.pszText=szBuff;
	lvi.cchTextMax=sizeof(szBuff);
	lvi.stateMask=0xFFFF;		// get all state flags

	HDITEM hdi;
	hdi.mask = HDI_WIDTH;

	GetItem(&lvi);

	BOOL bSelected=(bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected=bSelected || (lvi.state & LVIS_DROPHILITED);


	//Draw Item
	//rect.top+=1;
	//rect.bottom-=1;
	if(bSelected)
	{
		dc.FillSolidRect(&rect,m_clSelected); //206,231,231));

	}else
	{
		dc.FillSolidRect(&rect,m_clUnselected);
	}

	CPen pen(PS_SOLID,1,RGB(157,163,163));
	CPen *pOldPen;
	pOldPen=dc.SelectObject(&pen);

	if(m_bGridLine)
	{
		dc.MoveTo(rect.left,rect.top);
		dc.LineTo(rect.right,rect.top);
		dc.MoveTo(rect.left,rect.bottom);
		dc.LineTo(rect.right,rect.bottom);
	}else if(bSelected)
	{
		dc.MoveTo(rect.left,rect.top);
		dc.LineTo(rect.right,rect.top);
		dc.MoveTo(rect.left,rect.bottom-1);
		dc.LineTo(rect.right,rect.bottom-1);
	}

	dc.SelectObject(pOldPen);

	CFont *pOldFont=dc.SelectObject(&m_font);

	for(int i=0;i<m_header.GetItemCount();i++)
	{
		lvi.iSubItem=i;
		GetItem(&lvi);
		LVCOLUMN col;
		col.mask=LVCF_FMT;
		GetColumn(i, &col);
		
		m_header.GetItem(i,&hdi);
		rect.right=rect.left+hdi.cxy;
		if(col.fmt & LVCFMT_LEFT)
			dc.DrawText(szBuff,-1,&rect,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		else if(col.fmt & LVCFMT_RIGHT)
			dc.DrawText(szBuff,-1,&rect,DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		else if(col.fmt & LVCFMT_CENTER)
			dc.DrawText(szBuff,-1,&rect,DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		else
		{
			rect.left+=2;
			dc.DrawText(szBuff,-1,&rect,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		}
		rect.left=rect.right;
	}

	dc.SelectObject(pOldFont);
	dc.Detach();
}


void CPanaListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	if(!m_bMeasured)
	{
		lpMIS->itemHeight=(int)(lpMIS->itemHeight * 1.8);
		m_bMeasured=TRUE;
	}
	
}

void CPanaListCtrl::InitHeader()
{
	LOGFONT logFont;

	
	if(GetParent())
	{
		CFont *pFont=GetParent()->GetFont();
		pFont->GetLogFont(&logFont);

		m_font.DeleteObject();
		m_font.CreateFontIndirect(&logFont);

		logFont.lfWeight=FW_BOLD;
		logFont.lfHeight=int(logFont.lfHeight * 1.1);
		m_header.SetHeaderFont(logFont);

		logFont.lfHeight=20;
		logFont.lfWeight=FW_NORMAL;
		m_headerFont.DeleteObject();
		m_headerFont.CreateFontIndirect(&logFont);
		SetFont(&m_headerFont); //ListCtrl字体的大小决定了header control的高度
	}else
	{
	
		m_font.GetLogFont(&logFont);

		logFont.lfHeight=20;
		m_headerFont.DeleteObject();
		m_headerFont.CreateFontIndirect(&logFont);
		SetFont(&m_headerFont);
	}

}

void CPanaListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
    CHeaderCtrl* pCtrl=GetHeaderCtrl();   
	m_header.SubclassWindow(pCtrl->m_hWnd);

	m_header.SetListCtrl(this);
	CListCtrl::PreSubclassWindow();
}

void CPanaHeaderCtrl::SetHeaderFont(LOGFONT lf)
{
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);
}

void CPanaListCtrl::SetColor(COLORREF clSelected, COLORREF clUnselected)
{
	m_clSelected=clSelected;
	m_clUnselected=clUnselected;
}

void CPanaListCtrl::EnableGridLine(BOOL bGrid)
{
	m_bGridLine=bGrid;
}
