// OleDropTargetEx.cpp: implementation of the COleDropTargetEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDropTargetEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//COleDropInfo Construction/Destruction
IMPLEMENT_DYNAMIC( COleDropInfo, CObject );

COleDropInfo::COleDropInfo()
{
	m_pDataObject = NULL;
	m_dwKeyState = 0;
	m_dropEffect = (DROPEFFECT)-1;
	m_dropEffectList = DROPEFFECT_NONE;
}

COleDropInfo::COleDropInfo(COleDataObject* pDataObject,DWORD dwKeyState,
				DROPEFFECT dropEffect,DROPEFFECT dropEffectList,CPoint point )
{
	m_pDataObject = pDataObject;
	m_dwKeyState = dwKeyState;
	m_dropEffect = dropEffect;
	m_dropEffectList = dropEffectList;
	m_point = point;
}

COleDropInfo::~COleDropInfo()
{
	m_pDataObject = NULL;
}

//////////////////////////////////////////////////////////////////////
//COleDropTargetEx Construction/Destruction
COleDropTargetEx::COleDropTargetEx()
{
}

COleDropTargetEx::~COleDropTargetEx()
{
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
DROPEFFECT COleDropTargetEx::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
			DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))//if CView,Call base class
		return COleDropTarget::OnDragEnter(pWnd,pDataObject, dwKeyState, point);

	COleDropInfo DropInfo( pDataObject, dwKeyState, 0, 0, point );
	
	return ::SendMessage( pWnd->GetSafeHwnd(), DROPM_DRAGENTER, (WPARAM)&DropInfo, 0 );
}

DROPEFFECT COleDropTargetEx::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
			DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))//if CView,Call base class
		return COleDropTarget::OnDragOver(pWnd,pDataObject, dwKeyState, point);
	
	COleDropInfo DropInfo( pDataObject, dwKeyState, 0, 0, point );

	return ::SendMessage( pWnd->GetSafeHwnd(), DROPM_DRAGOVER, (WPARAM)&DropInfo, 0 );
}

BOOL COleDropTargetEx::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
			DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))//if CView,Call base class
		return COleDropTarget::OnDrop(pWnd, pDataObject,dropEffect,point);

	COleDropInfo DropInfo( pDataObject, 0, dropEffect, 0, point );

	return ::SendMessage( pWnd->GetSafeHwnd(), DROPM_DROP, (WPARAM)&DropInfo, 0 );
}

DROPEFFECT COleDropTargetEx::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
			DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))//if CView,Call base class
		return COleDropTarget::OnDropEx( pWnd, pDataObject, dropEffect, dropEffectList, point) ;

	COleDropInfo DropInfo( pDataObject, 0, dropEffect, dropEffectList, point );

	return ::SendMessage( pWnd->GetSafeHwnd(), DROPM_DROPEX, (WPARAM)&DropInfo, 0 );
}

void COleDropTargetEx::OnDragLeave(CWnd* pWnd)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))//if CView,Call base class
	{
		COleDropTarget::OnDragLeave( pWnd );
		return;
	}

	::SendMessage( pWnd->GetSafeHwnd(), DROPM_DRAGLEAVE, 0, 0 );

	return;
}
