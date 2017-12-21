// PopupMenu.cpp: implementation of the CPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "PopupMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPopupMenu::CPopupMenu()
{
	m_nCount=0;
	m_pPopupMenuHead=NULL;
	m_pPopupMenuTail=NULL;
}

CPopupMenu::~CPopupMenu()
{
	RemoveAll();		
}

BOOL CPopupMenu::Add(UINT menuID,int num,CString strName)
{
	pPOPUPMENU temp;
	temp=new POPUPMENU;

	temp->menu.CreatePopupMenu();
	if(menuID==IDR_POP_MENU)
	{		
		if(num==0)//第一子菜单
		{		
			//temp->menu.AppendMenu(MF_STRING, POPUP1_DISP_3D, (LPCTSTR)"Display in &3D");
			temp->menu.AppendMenu(MF_STRING, POPUP1_DISP_2D_ONE, (LPCTSTR)"Display in 2D-&One");
			temp->menu.AppendMenu(MF_STRING, POPUP1_DISP_2D_TWO, (LPCTSTR)"Display in 2D-&Two");
			temp->menu.AppendMenu(MF_STRING, POPUP1_DISP_2D_THREE, (LPCTSTR)"Display in 2D-&Three");
			temp->menu.AppendMenu(MF_STRING, POPUP1_DISP_2D_FOUR, (LPCTSTR)"Display in 2D-&Four");
	
			//Added by Wang fengliang 070808;
			temp->menu.AppendMenu(MF_SEPARATOR);
			temp->menu.AppendMenu(MF_STRING, POPUP1_SERIES_DEL, (LPCTSTR)"Delete");  //删除 序列

		}
	}
	else 
		return FALSE;
	temp->sName=strName; //Added by Wang fengliang 070808;
	temp->nNo=++m_nCount;
	temp->next=NULL;

	if(m_pPopupMenuHead==NULL)
	{
		m_pPopupMenuHead=temp;
	}
	else
	{		
		m_pPopupMenuTail->next=temp;
	}
	m_pPopupMenuTail=temp;


	return TRUE;
}
BOOL CPopupMenu::RemoveAt(CString strName)// 删除其中的一个菜单，参数：序列名称
{
	pPOPUPMENU temp = NULL, pro = NULL;
	temp=m_pPopupMenuHead;
	while(temp)//删除资源
	{
		if(temp->sName==strName)
			break;
		pro=temp;
		temp=temp->next;
	}

	if(temp != NULL) //找到
	{
		if(temp != m_pPopupMenuHead && temp != m_pPopupMenuTail)//既不是头也不是尾
			pro->next=temp->next;

		if(temp == m_pPopupMenuHead)
			m_pPopupMenuHead=temp->next;
		if(temp == m_pPopupMenuTail)
			m_pPopupMenuTail = pro;
		if(m_pPopupMenuTail)
			m_pPopupMenuTail->next=NULL;
		temp->menu.Detach();
		delete temp;
		
		return TRUE;
	}
	return FALSE;
}
BOOL CPopupMenu::RemoveAll()
{
	while(m_pPopupMenuHead)//删除资源
	{
		m_pPopupMenuTail=m_pPopupMenuHead;
		m_pPopupMenuHead=m_pPopupMenuHead->next;
		m_pPopupMenuTail->menu.Detach();
		delete m_pPopupMenuTail;
	}
	m_pPopupMenuTail=m_pPopupMenuHead=NULL;
	
	return TRUE ;
}
CMenu* CPopupMenu::GetAt(int num)
{
	if(num<=0||num>m_nCount) return NULL;
	pPOPUPMENU temp=m_pPopupMenuHead;
	if(temp==NULL) return NULL;
	while(temp&&temp->nNo!=num)
	{
		temp=temp->next;		
	}
	if(temp==NULL) return NULL;

	
	return &(temp->menu);

}

CMenu* CPopupMenu::GetAt(CString sName)
{
	if(sName == "") return NULL;
	pPOPUPMENU temp=m_pPopupMenuHead;
	if(temp==NULL) return NULL;
	while(temp&&temp->sName	!= sName)
	{
		temp=temp->next;		
	}
	if(temp==NULL) return NULL;

	
	return &(temp->menu);

}
//第num个序列的菜单的menuIndex子菜单项，标志为flag；
BOOL CPopupMenu::SetCheck(int num,int menuItemIndex,BOOL flag)
{
	if(num<=0||num>m_nCount) return FALSE;
	
	pPOPUPMENU temp=m_pPopupMenuHead;
	while(temp&&temp->nNo!=num)
	{
		temp=temp->next;
	}
	UINT menuItemID=0;
	if(menuItemIndex==1)
		menuItemID=POPUP1_DISP_3D;
	else if(menuItemIndex==2)
		menuItemID=POPUP1_DISP_2D_ONE;
	else if(menuItemIndex==3)
		menuItemID=POPUP1_DISP_2D_TWO;
	else if(menuItemIndex==4)
		menuItemID=POPUP1_DISP_2D_THREE;
	else if(menuItemIndex==5)
		menuItemID=POPUP1_DISP_2D_FOUR;
//	else if(menuItemIndex==)
	
	if(temp==NULL)
		return FALSE;
	else
	{
		if(flag)
			temp->menu.CheckMenuItem(menuItemID,MF_CHECKED);
		else 
			temp->menu.CheckMenuItem(menuItemID,MF_UNCHECKED);
	}
	UpdateOthers(num,menuItemIndex);
	return TRUE;
}
//第num个序列的菜单的menuIndex子菜单项，标志为flag；
BOOL CPopupMenu::SetCheck(CString sName,int menuItemIndex,BOOL flag)
{
	if(sName=="") return FALSE;
	
	pPOPUPMENU temp=m_pPopupMenuHead;
	while(temp&&temp->sName!=""&&temp->sName!=sName)
	{
		temp=temp->next;
	}
	UINT menuItemID=0;
	if(menuItemIndex==1)
		menuItemID=POPUP1_DISP_3D;
	else if(menuItemIndex==2)
		menuItemID=POPUP1_DISP_2D_ONE;
	else if(menuItemIndex==3)
		menuItemID=POPUP1_DISP_2D_TWO;
	else if(menuItemIndex==4)
		menuItemID=POPUP1_DISP_2D_THREE;
	else if(menuItemIndex==5)
		menuItemID=POPUP1_DISP_2D_FOUR;
//	else if(menuItemIndex==)
	
	if(temp==NULL)
		return FALSE;
	else
	{
		if(flag)
			temp->menu.CheckMenuItem(menuItemID,MF_CHECKED);
		else 
			temp->menu.CheckMenuItem(menuItemID,MF_UNCHECKED);
	}
	UpdateOthers(sName,menuItemIndex);
	return TRUE;
}

BOOL CPopupMenu::UpdateOthers(CString sName , int menuItemIndex)
{
	if(sName=="") return FALSE;
	pPOPUPMENU temp=m_pPopupMenuHead;
	BOOL flag=FALSE;

	if(temp==NULL) return FALSE;
	
	UINT menuItemID;
	if(menuItemIndex==1)
		menuItemID=POPUP1_DISP_1;
	else if(menuItemIndex==2)
		menuItemID=POPUP1_DISP_2;
	else if(menuItemIndex==3)
		menuItemID=POPUP1_DISP_3;

	while(temp)
	{
		if(temp->sName!=sName) 
		{			
			temp->menu.CheckMenuItem(menuItemID,MF_UNCHECKED);
			flag=TRUE;
		}
		temp=temp->next;
	}
	return flag;
}
BOOL CPopupMenu::UpdateOthers(int num,int menuItemIndex)
{
	if(num<=0||num>m_nCount) return FALSE;
	pPOPUPMENU temp=m_pPopupMenuHead;
	BOOL flag=FALSE;

	if(temp==NULL) return FALSE;
	
	UINT menuItemID;
	if(menuItemIndex==1)
		menuItemID=POPUP1_DISP_1;
	else if(menuItemIndex==2)
		menuItemID=POPUP1_DISP_2;
	else if(menuItemIndex==3)
		menuItemID=POPUP1_DISP_3;

	while(temp)
	{
		if(temp->nNo!=num) 
		{			
			temp->menu.CheckMenuItem(menuItemID,MF_UNCHECKED);
			flag=TRUE;
		}
		temp=temp->next;
	}
	return flag;
}
BOOL CPopupMenu::ShowMenu(int num,CPoint *point,CWnd* hwnd)
{
	pPOPUPMENU temp=m_pPopupMenuHead;
	BOOL flag=FALSE;

	if(temp==NULL) return FALSE;

	while(temp)
	{
		if(temp->nNo==num) 
		{
			temp->menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point->x, point->y, hwnd);	
			flag=TRUE;
		}
	}
	return flag;
}
