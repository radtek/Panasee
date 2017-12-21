// Prop3DTree.cpp : implementation file
//

#include "stdafx.h"
#include "Prop3DTree.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProp3DTree

CProp3DTree::CProp3DTree()
{
	m_bDraggingNow		= FALSE;
	m_hDraggedItem		= NULL;
	m_pDragImageList	= NULL;

}

CProp3DTree::~CProp3DTree()
{
}


BEGIN_MESSAGE_MAP(CProp3DTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CProp3DTree)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProp3DTree message handlers

void CProp3DTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT uFlags;
	CPoint lpoint;
	
	GetCursorPos(&lpoint);
	this->ScreenToClient(&lpoint);
	HTREEITEM hItem = HitTest(lpoint, &uFlags);
	if(hItem && (uFlags & TVHT_ONITEM) && !(uFlags & TVHT_ONITEMRIGHT))
		SelectItem(hItem);

 	
  	HTREEITEM parent;
  	if(GetSelectedItem()!=TVI_ROOT)
  	{
  		parent=GetNextItem(GetSelectedItem(),TVGN_PARENT);
  	}else
  	{
		*pResult = 0;
  		return;
  	}
 
  	
	if(GetItemText(GetNextItem(parent,TVGN_PARENT))=="Tracer")
	{
		if (!m_bDraggingNow)
		{
			m_sOldGroup=GetItemText(parent);
			m_sTraceName=GetItemText(GetSelectedItem());
			//PanaMessageBox("afds");
			SetCapture();
			m_bDraggingNow = TRUE;
			m_hDraggedItem = GetSelectedItem();
			Select(m_hDraggedItem, TVGN_CARET);
			m_pDragImageList = CreateDragImage(m_hDraggedItem);
			m_pDragImageList->DragEnter(this, pNMTreeView->ptDrag);
			m_pDragImageList->BeginDrag(0, CPoint(0, 0));
		}
	}

	
	*pResult = 0;
	
}

void CProp3DTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//PanaMessageBox("1");
	if (m_bDraggingNow)
	{
		//PanaMessageBox("2");
		ReleaseCapture();

		m_bDraggingNow = FALSE;
		m_pDragImageList->EndDrag();
		delete m_pDragImageList;
		m_pDragImageList = NULL;

		UINT uFlags;
		CPoint lpoint;
	
		GetCursorPos(&lpoint);
		this->ScreenToClient(&lpoint);
		HTREEITEM hItem = HitTest(lpoint, &uFlags);
		if(hItem && (uFlags & TVHT_ONITEM) && !(uFlags & TVHT_ONITEMRIGHT))
		{

			SelectItem(hItem);
		}

		HTREEITEM parent;
  		if(GetSelectedItem()!=TVI_ROOT)
  		{
  			parent=GetNextItem(GetSelectedItem(),TVGN_PARENT);
  		}else
  		{
			m_hDraggedItem = NULL;
  			return;
  		}

		if(GetItemText(GetNextItem(parent,TVGN_PARENT))=="Tracer")
		{
			SelectItem(parent);
			parent=GetNextItem(parent,TVGN_PARENT);
		}
		
		if(GetItemText(parent)=="Tracer")
  		{
			m_sNewGroup=GetItemText(GetSelectedItem());
			if(m_sOldGroup==m_sNewGroup)
			{
				m_hDraggedItem=NULL;
				return;
			}
			this->GetParent()->SendMessage(WM_CHANGETRACEGROUP,(WPARAM)&m_sOldGroup,(LPARAM)&m_sTraceName);
		}

		
	}
	m_hDraggedItem=NULL;
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CProp3DTree::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDraggingNow)
	{
		m_pDragImageList->DragEnter(this, point);
		m_pDragImageList->DragMove(point);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}
