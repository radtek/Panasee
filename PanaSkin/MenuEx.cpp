#include "stdafx.h"
#include "MenuEx.h"
#include "MemDC.h"
#include "ShowImage.h"
#include "PanaMetrics.h"
#include "resource.h"

IMPLEMENT_DYNAMIC( CMenuEx, CMenu )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMenuEx::CMenuEx()
{
//	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bPopupMenu=FALSE;					// initialize this menu flag
										// FALSE indicate this menu is top-level menu
										// TRUE means this menu is popupmenu
	m_iCount=0;
	m_nSeparator = 10;					// separator menu item 's height

	// initialize color attribute
	m_crBackground = RGB(232,244,244); //::GetSysColor(COLOR_MENU);	
	m_crText =::GetSysColor(COLOR_MENUTEXT);
	m_crTextSelected =::GetSysColor(COLOR_MENUTEXT);

	m_crSelectedBroder=RGB(49,106,197);
	m_crSelectedFill=RGB(206,231,231); //RGB(255,255,255);

	COLORREF colSelected=RGB(206,231,231);
	COLORREF colNormal=RGB(232,244,244);

	// no normal imagelist
	m_pImageList=NULL;					
	m_pDisabledImageList=NULL;			
	m_pHotImageList=NULL;		
	
	// default menu image size
	m_szImage=CSize(20,20);
	SetImageID(IDB_BITMAP_MENUSKIN);
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx::~CMenuEx()
{	
	// destroy menu and free all variable
	DestroyMenu();	
}
/////////////////////////////////////////////////////////////////////////////

BOOL CMenuEx::DestroyMenu()
{	
	// delete all imagelist
	if(m_pImageList!=NULL)
		m_pImageList->DeleteImageList();
	m_pImageList=NULL;

	if(m_pDisabledImageList!=NULL)
		m_pDisabledImageList->DeleteImageList();
	m_pDisabledImageList=NULL;

	if(m_pHotImageList!=NULL)
		m_pHotImageList->DeleteImageList();	
	m_pHotImageList=NULL;

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		MENUITEM *pMenuItem = m_MenuItemArr.GetAt(i);
		
		// destroy submenu
		if(pMenuItem->bIsSubMenu)
		{
			pMenuItem->pSubMenu->DestroyMenu();
			delete pMenuItem->pSubMenu;
			pMenuItem->pSubMenu=NULL;
		}
				
		// free menuitem struct
		delete pMenuItem;
	}	
	m_MenuItemArr.RemoveAll();
	return CMenu::DestroyMenu();
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(UINT nIDResource)
{
	// load menu from menu editor created resouce
	CMenu Menu;
	Menu.LoadMenu(nIDResource);

	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
		
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}		
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(HMENU hMenu)
{
	// load menu from global HMENU handle
	CMenu Menu;
	BOOL bResult=Menu.Attach(hMenu);
	ASSERT(bResult);
	ASSERT(::IsMenu(Menu.m_hMenu));

	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
	
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}	
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(LPCTSTR lpszResourceName)
{
	// load menu from resource string
	CMenu Menu;
	Menu.LoadMenu(lpszResourceName);
	
	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
	
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}	
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(CMenu& Menu)
{	
	UINT nID;
	UINT nIndex;	
	UINT nState;
	CString strMenu;

	ASSERT(::IsMenu(Menu.m_hMenu));
	// fill this menu use temp menu
	for(nIndex = 0; nIndex < Menu.GetMenuItemCount(); nIndex++)
	{		
		nID=Menu.GetMenuItemID(nIndex);					// get menu ID
		nState=Menu.GetMenuState(nIndex,MF_BYPOSITION);	// get menu state
		Menu.GetMenuString(nIndex,strMenu,MF_BYPOSITION);// get menu name
		
		if(nID==0)
		{	
			// insert separator
			if(!AppendMenu(MF_SEPARATOR))
				return FALSE;
		}
		else if(nID==-1)
		{
			// bypass popupmenu's item number	
			nState&=0x00FFL;	
			
			// insert popupmenu
			if(!AppendMenu(MF_POPUP|nState,(UINT)Menu.GetSubMenu(nIndex)->Detach(),strMenu))
				return FALSE;
		}
		else
		{		
			// insert normal menu item
			if(!AppendMenu(MF_STRING|nState,nID,strMenu))
				return FALSE;
		}
	}		
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::AppendMenu(UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem )
{	
	// append ownerdrawn style menu item to this menu
	// get last position of this menu
	UINT nIndex=m_MenuItemArr.GetSize();
	
	// insert menu item to last position
	// means append menu item
	return InsertMenu(nIndex,nFlags|MF_BYPOSITION,nIDNewItem,lpszNewItem);	
}

/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem , LPCTSTR lpszNewItem )
{
	// insert menu item to specific position or command ID
	LPMENUITEM pMenuItem;
	UINT nIndex;
	CSize  size;	
	
	if(nFlags & MF_BYPOSITION)
	{
		// insert menu item by position
		pMenuItem=new MENUITEM;
		
		if(nFlags&MF_SEPARATOR)
		{
			// insert separator
			pMenuItem->strText = "";						// no name
			pMenuItem->nID = 0;								// separator's ID
			pMenuItem->itemSize=CSize(0,m_nSeparator);		// separator's default size
			pMenuItem->pImageList=NULL;						// separator has no image
			pMenuItem->pDisabledImageList=NULL;
			pMenuItem->pHotImageList=NULL;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;					// separator must not be popupmenu
			pMenuItem->pSubMenu=NULL;		
			
			
			// insert menu item to this menu
			if(!CMenu::InsertMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
			{
				delete pMenuItem;
				return FALSE;
			}
			
			if(m_bPopupMenu)
			{			
				// modify menu item to ownerdrawn style
				// if this menu is not a top-level menu
				if(!CMenu::ModifyMenu(nPosition,MF_SEPARATOR|MF_BYPOSITION|MF_OWNERDRAW,0,(LPCTSTR)pMenuItem))
				{
					delete pMenuItem;
					return FALSE;
				}
			}			
			// insert menu item to array
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}
		else if(nFlags&MF_POPUP)
		{
			// insert popupmenu
			CMenuEx* pSubMenu=new CMenuEx;

			// create new popupmenu
			pSubMenu->SetPopupMenu(TRUE);
			pSubMenu->SetImageID(m_imageID);
			// load popupmenu from specific handle
			pSubMenu->LoadMenu((HMENU)nIDNewItem);
			CDC	 *pDC =AfxGetMainWnd()->GetDC();
			size=pDC->GetTextExtent(lpszNewItem);
			AfxGetMainWnd()->ReleaseDC(pDC);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = -1;
			
			// specified menu item size,do not include image's size
			pMenuItem->itemSize.cx=size.cx+25;
			pMenuItem->itemSize.cy=2;
			
			// specified all imagelist for new menu item
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=TRUE;
			pMenuItem->pSubMenu=pSubMenu;		// popupmenu's pointor	
			
			
			if(!CMenu::InsertMenu(nPosition,nFlags,(UINT)(pSubMenu->m_hMenu),lpszNewItem))
			{
				pSubMenu->DestroyMenu();
				delete pSubMenu;
				delete pMenuItem;
				return FALSE;
			}
			
			if(m_bPopupMenu)
			{			
				if(!CMenu::ModifyMenu(nPosition,nFlags|MF_BYPOSITION|MF_OWNERDRAW,(UINT)(pSubMenu->m_hMenu),(LPCTSTR)pMenuItem))
				{
					pSubMenu->DestroyMenu();
					delete pSubMenu;
					delete pMenuItem;
					return FALSE;
				}
			}			
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}
		else // if(nFlags & MF_STRING)
		{
			// insert normal menu item
			CDC	 *pDC =AfxGetMainWnd()->GetDC();
			size=pDC->GetTextExtent(lpszNewItem);
			AfxGetMainWnd()->ReleaseDC(pDC);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = nIDNewItem;			
			pMenuItem->itemSize.cx=size.cx+10;
			pMenuItem->itemSize.cy=2;		
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;					
			
			
			if(!CMenu::InsertMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
			{
				delete pMenuItem;
				return FALSE;
			}			
			if(m_bPopupMenu)
			{			
				if(!CMenu::ModifyMenu(nPosition,MF_BYPOSITION|MF_OWNERDRAW,nIDNewItem,(LPCTSTR)pMenuItem))
				{
					delete pMenuItem;
					return FALSE;
				}
			}
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}			

		
		return TRUE;
	}
	else if(nFlags & MF_BYCOMMAND)		// insert menu item by command ID
	{		
		for(nIndex = 0; nIndex < (UINT)m_MenuItemArr.GetSize(); nIndex++)
		{
			pMenuItem=m_MenuItemArr.GetAt(nIndex);
			
			// insert menu item with position
			// if the position match command ID
			if(pMenuItem->nID == nPosition)
				return InsertMenu(nIndex,MF_BYPOSITION,nIDNewItem,lpszNewItem);					
		}

		// do not found menu item in this menu
		CMenuEx* pSubMenu=FindPopupMenuFromID(nPosition);

		if(pSubMenu==NULL)
		{
			return FALSE;
		}
		else
		{
			// insert menu item to submenu
			return pSubMenu->InsertMenu(nPosition,nFlags,nIDNewItem ,lpszNewItem);
		}		
	}	

	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem)
{
	UINT nIndex;
	LPMENUITEM pMenuItem;
	
	CSize  size;

	// modify menu item by position
	if(nFlags & MF_BYPOSITION)
	{
		if(!CMenu::ModifyMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
		{	
			// fail to modify menu
			return FALSE;
		}				
		pMenuItem=GetMenuItem(nPosition);
		
		if(pMenuItem==NULL)
			return FALSE;

		if(nFlags&MF_SEPARATOR)
		{
			// modify menu item to separator
			pMenuItem->strText = "";
			pMenuItem->nID = 0;		
			pMenuItem->itemSize=CSize(0,m_nSeparator);
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;			
		}
		else if(nFlags&MF_POPUP)
		{
			// modify menu item to popupmenu item
			CMenuEx* pSubMenu=new CMenuEx;
			pSubMenu->SetPopupMenu(TRUE);
			pSubMenu->LoadMenu((HMENU)nIDNewItem);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = -1;

			CDC	 *pDC =AfxGetMainWnd()->GetDC();	
			size=pDC->GetTextExtent(lpszNewItem);
			AfxGetMainWnd()->ReleaseDC(pDC);

			pMenuItem->itemSize.cx=size.cx+25;
			pMenuItem->itemSize.cy=2;		
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=TRUE;
			pMenuItem->pSubMenu=pSubMenu;					
		}
		else // MF_STRING
		{
			// modify menu item to normal menu item
			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = nIDNewItem;

			CDC	 *pDC =AfxGetMainWnd()->GetDC();	
			size=pDC->GetTextExtent(lpszNewItem);
			AfxGetMainWnd()->ReleaseDC(pDC);

			pMenuItem->itemSize.cx=size.cx+10;
			pMenuItem->itemSize.cy=2;	
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;			
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;			
		}			
		return TRUE;		
	}
	else if(nFlags & MF_BYCOMMAND)
	{
		// modify menu item by command ID
		for(nIndex = 0; nIndex < (UINT)m_MenuItemArr.GetSize(); nIndex++)
		{
			pMenuItem=m_MenuItemArr.GetAt(nIndex);	
			
			// modify ID matched menu item by position
			if(pMenuItem->nID == nPosition)						
				return ModifyMenu(nIndex,MF_BYPOSITION,nIDNewItem,lpszNewItem);							
		}
		
		// walk through all items, looking for ID matched popupmenu
		CMenuEx* pSubMenu=FindPopupMenuFromID(nPosition);
		
		if(pSubMenu==NULL)					
			return FALSE;		// not found	
		else		
			return pSubMenu->ModifyMenu(nPosition,nFlags,nIDNewItem,lpszNewItem);			
	}		
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx* CMenuEx::GetSubMenu(int nPosition)
{
	// get submenu by position based zero
	// return NULL if do not exist
	if(nPosition>(m_MenuItemArr.GetSize()-1))
		return NULL;
	
	return m_MenuItemArr.GetAt(nPosition)->pSubMenu;
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx* CMenuEx::FindPopupMenuFromID(UINT nID)
{
	ASSERT_VALID(this);
	
	// walk through all items, looking for ID match
	UINT nItems = GetMenuItemSize();
	for (int iItem = 0; iItem < (int)nItems; iItem++)
	{
		CMenuEx* pPopup = GetSubMenu(iItem);
		if (pPopup != NULL)
		{
			// recurse to child popup
			pPopup = FindPopupMenuFromID(nID);
			// check popups on this popup
			if (pPopup != NULL)
				return pPopup;
		}
		else if (GetMenuItemID(iItem) == nID)
		{
			// it is a normal item inside this popup			
			return this;
		}
	}
	// not found
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	m_iCount++;
	// chech this is ownerdrawn menu or not
	if(lpDIS->CtlType!=ODT_MENU)
		return;	
	  

	// drawn device context for draw menu
	CDC dc;
	dc.Attach(lpDIS->hDC);
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;

	// rect region of device context
	CRect rect=CRect(lpDIS->rcItem);	

	DrawBackground(&dc,rect);	
			
	
	DrawMenuImage(&dc,rect,lpDIS);	

	DrawSelected(&dc,rect,lpDIS);	
	
	
	DrawMenuText(&dc,rect,lpDIS);


	dc.Detach();
	
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawBackground(CDC* pDC,CRect rect)
{
	CBrush BGBrush(m_crBackground);	
	pDC->FillRect(rect,&BGBrush);
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawMenuImage(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rc=rect;
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;
		
	CShowImage showImage;
	showImage.SetImage(m_imageID,3);
	showImage.SetShowRegion(rect);
	showImage.SetSizingMargin(8,6,8,6);
	//showImage.SetTransparentColor(RGB(255,0,0));
			

	if(lpMenuItem->nID==-2)
	{
		showImage.DrawImage(pDC,0);
		
	}else if(lpMenuItem->nID==-3)
	{
		showImage.DrawImage(pDC,2);

	}else
	{
		
		showImage.DrawImage(pDC,1);
	}

	

}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawMenuText(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcText=rect;
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;	
	
	if(lpMenuItem->nID== -2 || lpMenuItem->nID== -3)
		return;

	rcText.left+=8;
	rcText.right-=8;
	
	// not use background color when draw text
	pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont=pDC->SelectObject(CPanaMetrics::GetDlgFont());

	if(lpMenuItem->nID==0)
	{
		// draw separator
		rcText.top =rcText.Height()/2+rcText.top-1;
		rcText.bottom =rcText.top +2;

		CShowImage memImage;
		memImage.SetImage(IDB_BITMAP_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcText);
		memImage.DrawImage(pDC,0);		
	}
	else
	{	
		// calculate text position
		CSize size=pDC->GetTextExtent(lpMenuItem->strText);		
		rcText.top=rcText.top+(rcText.Height()-size.cy)/2;
		rcText.left+=20; //(rcText.Width()-size.cx)/2;

		if(lpDIS->itemState & ODS_GRAYED)
		{	
			// draw grayed text
			pDC->DrawState(rcText.TopLeft(), rcText.Size(), lpMenuItem->strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		else
		{
			// set text color
			if(lpDIS->itemState & ODS_SELECTED)			
				pDC->SetTextColor(m_crTextSelected);			
			else			
				pDC->SetTextColor(m_crText);				
			
			// draw normal text
			pDC->DrawText(lpMenuItem->strText,rcText,DT_LEFT | DT_EXPANDTABS | DT_VCENTER);

			//Draw Check flag
			if(lpDIS->itemState & ODS_CHECKED)	
			{
				rcText.left-=15;
				rcText.right=rcText.left+7;
				rcText.top+=4;
				rcText.bottom=rcText.top+7;

				CShowImage showImage;
				showImage.SetImage(IDB_BITMAP_CHECK_FLAG,1);
				showImage.SetShowRegion(rcText);
				showImage.SetSizingMargin(0,0,0,0);
				showImage.SetTransparentColor(RGB(255,0,255));
				showImage.DrawImage(pDC,0);
			}
		}
	}	
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OPAQUE);
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawSelected(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcText=rect;
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;

	if(lpMenuItem->nID== -2 || lpMenuItem->nID== -3)
		return;

	rcText.left+=6;
	rcText.right-=5;
	rcText.bottom-=1;

	// separator,grayed menu item,disabled menu item
	// can not be selected
	if((lpMenuItem->nID==0)||(lpDIS->itemState & ODS_GRAYED)||(lpDIS->itemState & ODS_DISABLED))
		return;	

	// draw selected recatangle use special color
	// if current menu item is selected
	if(lpDIS->itemState & ODS_SELECTED )
	{
		pDC->FillSolidRect(rcText,m_crSelectedFill);

		CPen BorderPen(PS_SOLID,1,RGB(157,163,163)); //m_crSelectedBroder);
		CPen* pOldPen= pDC->SelectObject(&BorderPen);

		pDC->MoveTo(rcText.left,rcText.top);
		pDC->LineTo(rcText.right,rcText.top);
		pDC->MoveTo(rcText.left,rcText.bottom);
		pDC->LineTo(rcText.right,rcText.bottom);

		CPen BorderPen2(PS_SOLID,1,RGB(255,255,255)); //m_crSelectedBroder);
		pDC->SelectObject(&BorderPen2);
		rcText.left+=1;
		rcText.top+=1;
		rcText.bottom-=1;
		rcText.right-=1;

		pDC->MoveTo(rcText.left,rcText.top);
		pDC->LineTo(rcText.right,rcText.top);
		pDC->MoveTo(rcText.left,rcText.bottom);
		pDC->LineTo(rcText.right,rcText.bottom);

		pDC->SelectObject(pOldPen);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetImageList(CImageList* pImageList)
{
	if(pImageList==NULL)
		return;		

	IMAGEINFO info;
	LPMENUITEM lpMenuItem;	
	m_pImageList =pImageList;
	
	// get imagelist size
	pImageList->GetImageInfo(0,&info);	
	m_szImage.cx=info.rcImage.right-info.rcImage.left;	
	m_szImage.cy=info.rcImage.bottom-info.rcImage.top;

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);

		// set normal imagelist for menu item
		lpMenuItem->pImageList=pImageList;
		
		// set submenu imagelist
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetImageList(pImageList);				
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetDisabledImageList(CImageList* pImageList)
{
	if(pImageList==NULL)
		return;
	
	LPMENUITEM lpMenuItem;
	// update disabled imagelist
	m_pDisabledImageList =pImageList;
	
	if(m_pImageList==NULL&&m_pHotImageList==NULL)
	{		
		IMAGEINFO info;
		
		// only set disabled imagelist
		// get imagelist size
		pImageList->GetImageInfo(0,&info);		
		m_szImage.cx=info.rcImage.right-info.rcImage.left;	
		m_szImage.cy=info.rcImage.bottom-info.rcImage.top;
	}
	
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		lpMenuItem->pDisabledImageList=pImageList;
		
		// set submenu disabled imagelis
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetDisabledImageList(pImageList);
				
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetHotImageList(CImageList* pImageList)
{
	if(pImageList==NULL)
		return;
	
	LPMENUITEM lpMenuItem;
	m_pHotImageList =pImageList;
	
	if(m_pImageList==NULL&&m_pDisabledImageList==NULL)
	{		
		IMAGEINFO info;		

		// only hot imagelist is set
		// get imagelist size
		pImageList->GetImageInfo(0,&info);			
		m_szImage.cx=info.rcImage.right-info.rcImage.left;	
		m_szImage.cy=info.rcImage.bottom-info.rcImage.top;
	}

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		lpMenuItem->pHotImageList=pImageList;
		
		// set submenu hot imagelis
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetHotImageList(pImageList);		
	}	
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetImageIndex(const UINT* nIDResource,UINT nIDCount)
{
	// set imagelist index from specific table "nIDResource[nIDCount]"
	// only can SetImageIndex after set one imagelist
	if(m_pImageList==NULL&&m_pHotImageList==NULL&&m_pDisabledImageList==NULL)
		return;

	LPMENUITEM lpMenuItem;
	for(UINT nIndex=0;nIndex<(UINT)m_MenuItemArr.GetSize();nIndex++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(nIndex);

		// call submenu 's loadtoolbar
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetImageIndex(nIDResource,nIDCount);		
		else
		{
			for(UINT i=0;i<nIDCount;i++)
			{
				// if table's commandID equal menu item's command ID
				// set nImageIndex to table's index
				if(lpMenuItem->nID==nIDResource[i])
					lpMenuItem->nImageIndex=i;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::LoadToolBar(const CToolBar* pToolBar)
{
	
	// load all imagelist from pToolBar
	SetImageList(pToolBar->GetToolBarCtrl().GetImageList());
	SetDisabledImageList(pToolBar->GetToolBarCtrl().GetDisabledImageList());
	SetHotImageList(pToolBar->GetToolBarCtrl().GetHotImageList());
	
	// pToolBar don't have imagelist
	if(m_pImageList==NULL&&m_pDisabledImageList==NULL&&m_pHotImageList==NULL)
		return;

	LPMENUITEM pItem;

	for(UINT nIndex=0;nIndex<(UINT)m_MenuItemArr.GetSize();nIndex++)
	{
		pItem=m_MenuItemArr.GetAt(nIndex);		
		
		// load submenu toolbar
		if(pItem->bIsSubMenu)		
			pItem->pSubMenu->LoadToolBar(pToolBar);		
		else
		{	
			for(UINT i=0;i<(UINT)pToolBar->GetToolBarCtrl().GetButtonCount();i++)
			{				 
				if(pItem->nID==pToolBar->GetItemID(i))
				{
					// if toolbar's commandID equal menu item's command ID
					// set nImageIndex to toolbar's index
					pItem->nImageIndex=i;

					// set all imagelist to toolbar's imagelist
					pItem->pImageList=m_pImageList;
					pItem->pDisabledImageList=m_pDisabledImageList;
					pItem->pHotImageList=m_pHotImageList;						
				}
			}
		}		
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetTextColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set normal menu text color
	m_crText =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		
		// set submenu text color
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetTextColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetBackgroundColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set menu background color
	m_crBackground =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetBackgroundColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetTextSelectedColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected menu text color
	m_crTextSelected =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetTextSelectedColor(color);	
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetLeftColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set menu left side color
	m_crLeft =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetLeftColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetSelectedBroderColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected rectangle border color
	m_crSelectedBroder =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetSelectedBroderColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetSelectedFillColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected rectangle fill color
	m_crSelectedFill =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetSelectedFillColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetPopupMenu(BOOL bPopupMenu)
{
	// set top-level menu indicator	
	m_bPopupMenu=bPopupMenu;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT  lpMIS)
{
	// check measure ownerdraw menu or other controler
	
	if(lpMIS->CtlType!=ODT_MENU)		
		return;

	
	MENUITEM *lpMenuItem =(LPMENUITEM)lpMIS->itemData;
	
	// initialize width and height with itemSize
	lpMIS->itemWidth= lpMenuItem->itemSize.cx;
	lpMIS->itemHeight=lpMenuItem->itemSize.cy;

	// not separator,may be popupmenu or normal menu item
	if(lpMenuItem->nID!=0)
	{		
		IMAGEINFO info;

		if(lpMenuItem->pImageList!=NULL)
		{	
			// first add normal imagelist size
			lpMenuItem->pImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);			
		}
		else if(lpMenuItem->pDisabledImageList!=NULL)
		{
			// or add disabled imagelist size
			lpMenuItem->pDisabledImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);
		}
		else if(lpMenuItem->pHotImageList!=NULL)
		{
			// or add hot imagelist size
			lpMenuItem->pHotImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);
		}
		else if(lpMenuItem->nID==-2 || lpMenuItem->nID == -3)
		{
			lpMIS->itemHeight=14;
		}else
		{		
			// add default size,if do not have imagelist
			lpMIS->itemWidth+=20;
			lpMIS->itemHeight+=20;
		}		

	}		


}
/////////////////////////////////////////////////////////////////////////////
UINT  CMenuEx::GetMenuItemSize() const 
{
	// get menu item counter
	return m_MenuItemArr.GetSize();
}
/////////////////////////////////////////////////////////////////////////////
LPMENUITEM CMenuEx::GetMenuItem(UINT nPosition)
{
	// get menu item by position
	if(nPosition>(UINT)m_MenuItemArr.GetUpperBound())
		return NULL;

	return m_MenuItemArr.GetAt(nPosition);
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::AdjustMenuItem()
{
	LPMENUITEM pMenuItem;
	UINT nIndex=m_MenuItemArr.GetSize();
	for (int i=0;i<(int)nIndex;i++)
	{
		pMenuItem= m_MenuItemArr.GetAt(i);
		if(pMenuItem->bIsSubMenu)
			pMenuItem->pSubMenu->AdjustMenuItem();
	}

	InsertMenu(0,MF_STRING|MF_BYPOSITION,-2,"first");
	InsertMenu(nIndex+1,MF_STRING|MF_BYPOSITION,-3,"second");
}
