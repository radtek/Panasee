// MoveableCtrl.h: interface for the CMoveableCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEABLECTRL_H__3A8BA41F_799F_444B_AC2C_D68A7BC31555__INCLUDED_)
#define AFX_MOVEABLECTRL_H__3A8BA41F_799F_444B_AC2C_D68A7BC31555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panaskin.h"


/*********** ningsj *********************
 说明:基础类，使控件具有鼠标拖动改变位置的特性
*****************************************/
//
class PANASKIN_API CMoveableCtrl  
{
public:
	void SetMoveRange(CRect rc);
	CRect GetMoveRange();
	
	CMoveableCtrl();
	virtual ~CMoveableCtrl();
	enum CtrlMove_MoveMode{
		CtrlMove_BOTH,
		CtrlMove_HORIZONTAL,
		CtrlMove_VERTICAL,
		CtrlMove_NONE
	};
	
	void SetMoveMode(CtrlMove_MoveMode mode);
	//移动过程中是否重画父窗口 频繁重画容易引起闪烁
	void EnableRedraw(BOOL bEnable);
	void OnMouseMove(CWnd *pWnd,UINT nFlags, CPoint point);
	void OnLButtonDown(CWnd *pWnd,UINT nFlags, CPoint point);
	void OnLButtonUp(CWnd *pWnd,UINT nFlags, CPoint point);

private:
	BOOL m_bEnableMove;
	CPoint m_ptMoveOriginPos;
	BOOL m_bRedraw; //移动过程中是否重画
	CtrlMove_MoveMode m_bMoveMode;
	CRect m_rcRange;
	BOOL m_bRanged;
};

#endif // !defined(AFX_MOVEABLECTRL_H__3A8BA41F_799F_444B_AC2C_D68A7BC31555__INCLUDED_)
