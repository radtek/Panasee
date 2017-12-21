// PopupMenu.h: interface for the CPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPMENU_H__6B412715_CEDA_4274_9EC6_2081374BCF02__INCLUDED_)
#define AFX_POPUPMENU_H__6B412715_CEDA_4274_9EC6_2081374BCF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagPOPUPMENU
{
	int nNo;   
	CString sName; //Added by Wang fengliang 070808;
	CMenu menu;
	tagPOPUPMENU *next;
}POPUPMENU,*pPOPUPMENU;
class CPopupMenu  
{
public:
	CPopupMenu();
	virtual ~CPopupMenu();

	pPOPUPMENU m_pPopupMenuHead;
	pPOPUPMENU m_pPopupMenuTail;

	int m_nCount;//当前个数

	BOOL Add(UINT,int,CString);//添加一项,参数：菜单ID ,subItem,序列名称（关键字）

	//Added by Wang fengliang 070808;
	BOOL RemoveAt(CString);// 删除其中的一个菜单，参数：序列名称
	BOOL RemoveAll();

    CMenu* GetAt(int);//得到一项
	CMenu* GetAt(CString sName);


	BOOL UpdateOthers(int,int);//更改其它菜单状态

	BOOL SetCheck(int num,int menuItemIndex,BOOL flag);//标记菜单项
	//Added by Wang fengliang 070809;
	BOOL SetCheck(CString sName,int menuItemIndex,BOOL flag);
	BOOL UpdateOthers(CString,int);//更改其它菜单状态

	BOOL ShowMenu(int,CPoint*,CWnd*);//显示菜单
	
	int GetSize(){return m_nCount;}
	int IsEmpty(){return m_nCount==0?TRUE:FALSE;}

};

#endif // !defined(AFX_POPUPMENU_H__6B412715_CEDA_4274_9EC6_2081374BCF02__INCLUDED_)
