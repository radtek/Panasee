//*************************************************************************
// MenuEx.h : header file
// Version : 1.00
// Date : 2004.12.15
// Author : geforce_zf
// Email :  geforce_zf@163.com
// 
// You are free to use/modify this code but leave this header intact.
// This class is public domain so you are free to use it any of
// your applications (Freeware,Shareware,Commercial). All I ask is
// that you let me know so that if you have a real winner I can
// brag to my buddies that some of my code is in your app. I also
// wouldn't mind if you sent me a copy of your application since I
// like to play with new stuff.
//*************************************************************************
#if !defined(MENUEX_H)
#define MENUEX_H

#include <afxtempl.h>
#include "panaskin.h"

/*********** ningsj *********************
 说明:生成panasee风格的菜单
 使用方法:
 第一:使用资源ID
	
	CMenuEx m_menu;
	m_menu.SetPopupMenu(TRUE);
	m_menu.LoadMenu(IDR_MENU1);
	m_menu.GetSubMenu(0)->AdjustMenuItem();
	m_menu.GetSubMenu(0)->TrackPopupMenu(...);
 第二:动态菜单
    CMenuEx m_menu;
	m_menu.SetPopupMenu(TRUE);
	m_menu.CreatePopupMenu();
	CString sTmp;
	for(int i=0;i<25;i++)
	{
		CMenuEx subMenu;
		subMenu.SetPopupMenu(TRUE);
		subMenu.CreatePopupMenu();
		for(int j=0;j<8;j++)
		{
			
			sTmp.Format("menu_%i_%i",i,j);
			subMenu.AppendMenu(MF_STRING,i*5+j+1,sTmp);
		}
		
		sTmp.Format("menu_%i",i);
		m_menu.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, sTmp);
	}
	m_menu.AdjustMenuItem();
	m_menu.GetSubMenu(0)->TrackPopupMenu(...);
*****************************************/
class CMenuEx;

//MENUITEM and LPMENUITEM typedef
typedef struct tagMENUITEM
{
	CString		strText;			// menu item text
	UINT		nID;				// menu item command ID
									// separator's ID always be 0
									// popupmenu's ID always be -1
				
	CSize		itemSize;			// menu item's size,don't include image's size
	
	CImageList* pImageList;			// imagelist for normal menu item
	CImageList* pDisabledImageList;	// imagelist for disabled(grayed) menu item
	CImageList* pHotImageList;		// imagelist for selected(hot) menu item
	UINT		nImageIndex;		// index of imagelist,-1 means no image
	
	BOOL		bIsSubMenu;		// normal menu item if FALSE
									// TRUE means this item include submenu
	CMenuEx*	pSubMenu;			// always be NULL,when bIsPopupMenu is FALSE
									// if bIsPopupMenu is TRUE,contain popupmenu's pointor

} MENUITEM,*LPMENUITEM;

///////////////////////////////////////////
class PANASKIN_API CMenuEx : public CMenu
{
	DECLARE_DYNAMIC( CMenuEx )
	
// Constructor
public:	
	CMenuEx();	
	virtual ~CMenuEx();
	virtual BOOL DestroyMenu();

	void SetImageID(UINT uID){ m_imageID=uID; }
	int m_iCount;
// Operation
public:
	// initialize menu operation
	BOOL LoadMenu(UINT nIDResource);
	BOOL LoadMenu(LPCTSTR lpszResourceName);
	BOOL LoadMenu(HMENU hMenu);
	BOOL LoadMenu(CMenu & Menu);

	// menu item operation
	BOOL AppendMenu(UINT nFlags, UINT nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL );
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL );	

	// load image operation
	void SetImageIndex(const UINT* nIDResource,UINT nIDCount);
	void LoadToolBar(const CToolBar* pToolBar);

	// menu item operation
	UINT  GetMenuItemSize() const;
	LPMENUITEM GetMenuItem(UINT nPosition);	

	//submenu operation
	CMenuEx* GetSubMenu(int nPosition);
	CMenuEx* FindPopupMenuFromID(UINT nID);

// Attributes
protected:
	// top-level menu indicator
	BOOL m_bPopupMenu;

	// separator height
	int m_nSeparator;
	
	// color for ownerdrawn
	COLORREF m_crBackground;			// menu background color	
	COLORREF m_crTextSelected;			// menu selected text color
	COLORREF m_crText;					// menu normal text color
	COLORREF m_crLeft;					// color on menu left side
	COLORREF m_crSelectedBroder;		// menu selected rectangle border color
	COLORREF m_crSelectedFill;			// menu selected rectangle fill color

	// all imagelist should be same size 	
	CSize m_szImage;

	// menu item image for ownerdrawn
	CImageList* m_pImageList;			// menu item's normal imagelist
	CImageList* m_pDisabledImageList;	// menu item's disabled imagelist
	CImageList* m_pHotImageList;		// menu item's selected imagelist

protected:
	// contain all menu item of this menu
	CArray<LPMENUITEM,LPMENUITEM> m_MenuItemArr;

	UINT m_imageID;

public:
	// color attribute operation
	void SetTextSelectedColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetLeftColor(COLORREF color);
	void SetSelectedBroderColor(COLORREF color);
	void SetSelectedFillColor(COLORREF color);
	
	// set imagelist operation
	void SetImageList(CImageList* pImageList);
	void SetDisabledImageList(CImageList* pImageList);
	void SetHotImageList(CImageList* pImageList);
	
	// set top-level menu indicator
	void SetPopupMenu(BOOL bPopupMenu);

// Implementation
public:
	// virtual drawitem operation,call by MFC frameworks
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	
	// update all menu item to owerdrawn style
	// because frameworks insert some non-ownerdrawn menu item some times

protected:
	// drawn menu item helper operation
	void DrawBackground(CDC* pDC,CRect rect);
	void DrawMenuImage(CDC* pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);
	void DrawMenuText(CDC*  pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);
	void DrawSelected(CDC*  pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);

// Static Member
public:
	void AdjustMenuItem();
	// call this fuction in mainframe's OnMeasureItem() message mapping handler
	// measure all ownerdrawn menu item's size
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	
	// call this fuction in mainframe's OnInitPopupMenu() message mapping handler
	// update non-ownerdrawn to ownerdrawn style

};
#endif // !defined(MENUEX_H)
