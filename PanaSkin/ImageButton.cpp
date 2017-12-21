#include "stdAfx.h"
#include "ImageButton.h"
#include "AutoFont.h"
#include "MemDC.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CImageButton

CImageButton::CImageButton()
{
    
    m_TransparentColor = RGB(255,0,0);
    
	
	m_bMouseOnButton=FALSE;

	
	m_bChecked=FALSE;

	m_ButtonMode=ImageBtn_PushButton;

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;

	m_btnMenu=NULL;

	m_bTransparentColor=FALSE;

	m_bFirstEnter=TRUE;
	m_iAniType=2;
	m_iAniStep=10;
	m_iAniTime=150; 

	m_bSecondImage=FALSE;

	m_txtPosition=DT_CENTER;

	m_bEnableMultiLine=FALSE;

	m_bEnableFontBkColor=FALSE;

	m_bBtnDown = FALSE;
	
	m_bEnableTrigger =  FALSE;
}

CImageButton::~CImageButton()
{
    
	
}


BEGIN_MESSAGE_MAP(CImageButton, CButton)
	//{{AFX_MSG_MAP(CImageButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//在对按钮初始化时，自动为按钮属性添加自画属性(OWNERDRAW)
void CImageButton::PreSubclassWindow() 
{
    SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	LoadBitmaps(IDB_BITMAP_BTN_NORMAL,8,8,8,8);
}

void CImageButton::SetBtnMenu(CMenu *pMenu)
{
	m_btnMenu=pMenu;
}

void CImageButton::EnableMultiLine(BOOL bEnable)
{
	m_bEnableMultiLine=bEnable;
}

BOOL CImageButton::OnClicked() 
{	
	if (m_csCallbacks.hWnd)	
	{
		CMenu* psub=NULL;
		if(m_btnMenu)
			psub= (CMenu*)m_btnMenu->GetSubMenu(0);

		::SendMessage(m_csCallbacks.hWnd, m_csCallbacks.nMessage, (WPARAM)psub, m_csCallbacks.lParam);
	}

	if(m_btnMenu)
	{
		CRect rc;
		GetWindowRect(&rc);
		int x=GetSystemMetrics(SM_CXSCREEN);
		POINT point;
		point.x=rc.left;
		point.y=rc.bottom;
		if((x-point.x)<100)
			point.x=rc.right;

		if(m_btnMenu->GetSubMenu(0))
		{
			m_btnMenu->GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,GetParent());
		}
	}
	return FALSE;
}

BOOL CImageButton::PreTranslateMessage(MSG* pMsg) 
{
	if(m_ToolTip.m_hWnd !=NULL)
	{
	m_ToolTip.RelayEvent(pMsg);
	}
	
	return CButton::PreTranslateMessage(pMsg);
} 

void CImageButton::InitToolTip()
{
	
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	} // if
} 

void CImageButton::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	} // if

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} 

void CImageButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} 

/*********** ningsj *********************
说明:按钮的类型，不同的类型对应不同的按钮重绘方式
*****************************************/
void CImageButton::SetButtonMode(ImageBtn_ButtonMode mode)
{
	m_ButtonMode=mode;
}

void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
    CDC xdc;
    xdc.Attach( lpDrawItemStruct->hDC );
    CRect rc;
    GetClientRect(rc);
    	
	CMemDC dc;
	if(m_bTransparentColor)
		dc.LoadDC(&xdc,rc,TRUE,m_TransparentColor);
	else
		dc.LoadDC(&xdc,rc);
	
	m_memImage.SetShowRegion(rc);

	CRect rcSndImage;
	BITMAP bmpInfo;
	if(m_bSecondImage)
	{
		m_memImage2.GetBitmap(&bmpInfo);
		rcSndImage.left=rc.left+(rc.Width()-bmpInfo.bmWidth/4)/2;
		rcSndImage.right=rcSndImage.left+bmpInfo.bmWidth/4;
		rcSndImage.top=rc.top+(rc.Height()-bmpInfo.bmHeight)/2;
		rcSndImage.bottom=rcSndImage.top+bmpInfo.bmHeight;
		m_memImage2.SetShowRegion(rcSndImage);
	}
	
	/*********** ningsj *********************
	说明:为了支持异形按钮，将按钮底部父窗口的背景重绘到按钮上
	*****************************************/
	if(GetParent())
	{		
		CRect rcParent;
		GetWindowRect(&rcParent);
		GetParent()->ScreenToClient(&rcParent);
		GetParent()->SendMessage(WM_PANA_GETPARENTBK,WPARAM(&rcParent),LPARAM(&dc));
	}

    UINT state = lpDrawItemStruct->itemState ;
    bool IsDisable = false;

	// if left mouse button was pressed?
	BOOL leftBtnDown=state & ODS_SELECTED;
	//GetAsyncKeyState(VK_LBUTTON)?leftBtnDown=TRUE:leftBtnDown=FALSE;

	if(m_ButtonMode==ImageBtn_PushButton)
	{
		if(m_bMouseOnButton && !leftBtnDown)
		{
			m_memImage.DrawImage(&dc,ImageBtn_select);
			if(m_bSecondImage)
			{
				m_memImage2.DrawImage(&dc,ImageBtn_select);
			}
			if(m_bFirstEnter)
			{
				dc.SetAnimation(m_iAniType,m_iAniStep,m_iAniTime);
				dc.SetDCWindow(this);
				m_bFirstEnter=FALSE;
			}
		}else if (m_bMouseOnButton && leftBtnDown)
		{
			m_memImage.DrawImage( &dc, ImageBtn_focus );
			if(m_bSecondImage)
			{
				rcSndImage.OffsetRect(1,1);
				m_memImage2.SetShowRegion(rcSndImage);
				m_memImage2.DrawImage(&dc,ImageBtn_focus);
			}
			rc.left+=2;
			rc.top+=2;
		}
		else if (state & ODS_DISABLED)
		{
			IsDisable = true;
    		m_memImage.DrawImage( &dc, ImageBtn_disable );
			if(m_bSecondImage)
			{
				m_memImage2.DrawImage(&dc,ImageBtn_disable);
			}

		}else{

			m_memImage.DrawImage( &dc, ImageBtn_normal );
			if(m_bSecondImage)
			{
				m_memImage2.DrawImage(&dc,ImageBtn_normal);
			}
		}
	}else if(m_ButtonMode==ImageBtn_CheckButton || m_ButtonMode==ImageBtn_RadioButton)
	{
		if(m_bChecked)
		{
			if(m_bMouseOnButton && !leftBtnDown)
			{
				m_memImage.DrawImage(&dc,ImageBtn_focus);
				if(m_bSecondImage)
				{
					rcSndImage.OffsetRect(1,1);
					m_memImage2.SetShowRegion(rcSndImage);
					m_memImage2.DrawImage(&dc,ImageBtn_focus);
				}
				rc.left+=2;
				rc.top+=2;
			
			}else if (m_bMouseOnButton && leftBtnDown)
			{
				m_memImage.DrawImage( &dc, ImageBtn_focus); 
				if(m_bSecondImage)
				{
					rcSndImage.OffsetRect(1,1);
					m_memImage2.SetShowRegion(rcSndImage);
					m_memImage2.DrawImage(&dc,ImageBtn_focus);
				}
				rc.left+=2;
				rc.top+=2;
			}
			else if (state & ODS_DISABLED)
			{
				IsDisable = true;
    			m_memImage.DrawImage( &dc, ImageBtn_disable );
				if(m_bSecondImage)
				{
					m_memImage2.DrawImage(&dc,ImageBtn_disable);
				}

			}else{

				m_memImage.DrawImage( &dc, ImageBtn_focus); 
				if(m_bSecondImage)
				{
					rcSndImage.OffsetRect(1,1);
					m_memImage2.SetShowRegion(rcSndImage);
					m_memImage2.DrawImage(&dc,ImageBtn_focus);
				}
				rc.left+=2; rc.top+=2;
			}
		}else
		{
			if(m_bMouseOnButton && !leftBtnDown)
			{
				m_memImage.DrawImage(&dc,ImageBtn_select);
				if(m_bSecondImage)
				{
					m_memImage2.DrawImage(&dc,ImageBtn_select);
				}
				if(m_bFirstEnter)
				{
					dc.SetAnimation(m_iAniType,m_iAniStep,m_iAniTime);
					dc.SetDCWindow(this);
					m_bFirstEnter=FALSE;
				}
			
			}else if (m_bMouseOnButton && leftBtnDown)
			{
				m_memImage.DrawImage( &dc, ImageBtn_focus );
				if(m_bSecondImage)
				{
					rcSndImage.OffsetRect(1,1);
					m_memImage2.SetShowRegion(rcSndImage);
					m_memImage2.DrawImage(&dc,ImageBtn_focus);
				}
				rc.left+=2;
				rc.top+=2;
			}
			else if (state & ODS_DISABLED)
			{
				IsDisable = true;
    			m_memImage.DrawImage( &dc, ImageBtn_disable );
				if(m_bSecondImage)
				{
					m_memImage2.DrawImage(&dc,ImageBtn_disable);
				}

			}else{

				m_memImage.DrawImage( &dc, ImageBtn_normal );
				if(m_bSecondImage)
				{
					m_memImage2.DrawImage(&dc,ImageBtn_normal);
				}
			}
		}
	}

    int imode = 0;
	if(m_bEnableFontBkColor)
	{
		dc.SetBkColor(m_fontBkColor);
		imode=dc.SetBkMode(OPAQUE);
	}else
	{
		imode=dc.SetBkMode(TRANSPARENT);
	}

    CFont *pOldFnt = dc.SelectObject(&m_Font);
    COLORREF oldColor;

    if(IsDisable)
      oldColor = dc.SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
	else
		oldColor=dc.SetTextColor(m_Font.GetFontColor() );

    CString txt;
    GetWindowText(txt);
	if(m_txtPosition==DT_LEFT)
		rc.left+=11;

	if(m_bEnableMultiLine)
		dc.DrawText(txt,rc,m_txtPosition | DT_VCENTER ); //| DT_SINGLELINE);
	else
		dc.DrawText(txt,rc,m_txtPosition | DT_VCENTER | DT_SINGLELINE);
      
    dc.SetTextColor( oldColor );
    dc.SelectObject(pOldFnt);
    dc.SetBkMode(imode );
	
}


bool CImageButton::LoadBitmaps(UINT id ,int Top, int Bottom,
				int Left, int Right,BOOL bRedraw)
{
	m_memImage.SetImage(id,4);
	m_memImage.SetSizingMargin(Left,Top,Right,Bottom);


	if(bRedraw)
		Invalidate(FALSE);

    return true;
}

bool CImageButton::LoadSecondBitmaps(UINT id ,int Top, int Bottom,
				int Left, int Right,BOOL bRedraw)
{
	m_memImage2.SetImage(id,4);
	m_memImage2.SetSizingMargin(Left,Top,Right,Bottom);


	if(bRedraw)
		Invalidate(FALSE);

	m_bSecondImage=TRUE;

    return true;
}

bool CImageButton::LoadBitmaps(HMODULE hMoudle,UINT id ,int Top, int Bottom,
				int Left, int Right,BOOL bRedraw)
{
	m_memImage.SetImage(hMoudle,id,4);
	m_memImage.SetSizingMargin(Left,Top,Right,Bottom);


	if(bRedraw)
		Invalidate(FALSE);

    return true;
}

bool CImageButton::LoadSecondBitmaps(HMODULE hMoudle,UINT id ,int Top, int Bottom,
				int Left, int Right,BOOL bRedraw)
{
	m_memImage2.SetImage(hMoudle,id,4);
	m_memImage2.SetSizingMargin(Left,Top,Right,Bottom);


	if(bRedraw)
		Invalidate(FALSE);

	m_bSecondImage=TRUE;

    return true;
}

void CImageButton::SetTextFont( CFont & fnt )
{ 

    LOGFONT lf;
    fnt.GetLogFont(&lf);
    m_Font.SetLogFont(lf);
}

void CImageButton::SetFontColor( COLORREF color )
{
    m_Font.SetFontColor(color);
    UpdateWindow();
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bMouseOnButton)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bMouseOnButton = _TrackMouseEvent(&tme);
		Invalidate(FALSE);
	}

	CMoveableCtrl::OnMouseMove(this,nFlags,point);
	CButton::OnMouseMove(nFlags, point);

	/*
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CMoveableCtrl::OnMouseMove(this,nFlags,point);
	CButton::OnMouseMove(nFlags, point);
		
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) return;
		
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;

			Invalidate();

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		} // if
	} else 
	{
		m_bMouseOnButton = TRUE;

		Invalidate();
	}
	*/

} // End of OnMouseMove

// Handler for WM_MOUSELEAVE
LRESULT CImageButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseOnButton = FALSE;
	Invalidate(FALSE);

	m_bFirstEnter=TRUE;
	return 0;
} // End of OnMouseLeave

void CImageButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//Analog send button messages
	if(m_bEnableTrigger)
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), NULL);

	CButton::OnTimer(nIDEvent);
}

void CImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	SetFocus();
 	SetCapture();
 	SetTimer(1,100,NULL);

	CMoveableCtrl::OnLButtonDown(this,nFlags,point);
	CButton::OnLButtonDown(nFlags, point);

	m_bBtnDown = TRUE;
}

void CImageButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	KillTimer(1);
	// TODO: Add your message handler code here and/or call default
	m_bBtnDown = FALSE;
	CMoveableCtrl::OnLButtonUp(this,nFlags,point);
	CButton::OnLButtonUp(nFlags, point);	
}


/*********** ningsj *********************
说明:表示checkbutton或radiobutton是否被按下
*****************************************/

void CImageButton::SetChecked(BOOL bStatus)
{
	m_bChecked=bStatus;
	Invalidate(FALSE);
}

DWORD CImageButton::SetClickedCallback(HWND hWnd, UINT nMessage, LPARAM lParam)
{
	m_csCallbacks.hWnd = hWnd;
	m_csCallbacks.nMessage = nMessage;
	m_csCallbacks.lParam = lParam;

	return 0;
}

void CImageButton::SetTextPosition(UINT pos)
{
	m_txtPosition=pos;
}

void CImageButton::SetFontBkColor(COLORREF color)
{
	m_bEnableFontBkColor=TRUE;
	m_fontBkColor=color;
}

void CImageButton::EnableTrigger(BOOL bEnable)
{
	m_bEnableTrigger = bEnable;
}

