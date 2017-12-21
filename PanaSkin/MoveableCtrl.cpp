// MoveableCtrl.cpp: implementation of the CMoveableCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveableCtrl.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMoveableCtrl::CMoveableCtrl()
{
	m_bEnableMove=FALSE;
	m_bRedraw=FALSE;
	m_bMoveMode=CtrlMove_NONE;
	m_bRanged=FALSE;
}

CMoveableCtrl::~CMoveableCtrl()
{

}

void CMoveableCtrl::OnMouseMove(CWnd *pWnd,UINT nFlags, CPoint point) 
{
	if(!pWnd)
		return;

	BOOL bLeftBtnDown=FALSE;
	GetAsyncKeyState(VK_LBUTTON)?bLeftBtnDown=TRUE:bLeftBtnDown=FALSE;
	if(bLeftBtnDown && m_bEnableMove && m_bMoveMode!=CtrlMove_NONE)
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		pWnd->GetParent()->ScreenToClient(&rc);

		int x,y;
		x=rc.left+point.x-m_ptMoveOriginPos.x;
		y=rc.top+point.y-m_ptMoveOriginPos.y;
		if(m_bMoveMode==CtrlMove_HORIZONTAL)
			y=rc.top;

		if(m_bMoveMode==CtrlMove_VERTICAL)
			x=rc.left;
		
		CPoint pt(x,y);

		if(!m_bRanged || m_rcRange.PtInRect(pt))
		{
			pWnd->MoveWindow(x,y,rc.Width(),rc.Height());
			pWnd->GetParent()->SendMessage(WM_PANA_MOVECTRL,(unsigned int)pWnd->GetSafeHwnd());
		}

		if(m_bRedraw)
			pWnd->Invalidate(FALSE);
	}

} 

void CMoveableCtrl::OnLButtonDown(CWnd *pWnd,UINT nFlags, CPoint point) 
{
	m_bEnableMove=TRUE;
	m_ptMoveOriginPos=point;
	
	pWnd->GetParent()->SendMessage(WM_PANA_MOVECTRL_LBTN_DOWN,(unsigned int)pWnd->GetSafeHwnd());
	
	
} 

void CMoveableCtrl::OnLButtonUp(CWnd *pWnd,UINT nFlags, CPoint point) 
{
	m_bEnableMove=FALSE;

	pWnd->GetParent()->SendMessage(WM_PANA_MOVECTRL_LBTN_UP,(unsigned int)pWnd->GetSafeHwnd());
	
} 

void CMoveableCtrl::EnableRedraw(BOOL bEnable)
{
	m_bRedraw=bEnable;
}

/*********** ningsj *********************
说明:设定控件移动的方式
*****************************************/
void CMoveableCtrl::SetMoveMode(CtrlMove_MoveMode mode)
{
	m_bMoveMode=mode;
}

/*********** ningsj *********************
说明:设定控件水平和垂直移动的范围
*****************************************/
void CMoveableCtrl::SetMoveRange(CRect rc)
{
	m_rcRange=rc;
	m_bRanged=TRUE;
}

CRect CMoveableCtrl::GetMoveRange()
{
	return m_rcRange;
}


