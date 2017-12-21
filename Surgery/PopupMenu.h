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

	int m_nCount;//��ǰ����

	BOOL Add(UINT,int,CString);//���һ��,�������˵�ID ,subItem,�������ƣ��ؼ��֣�

	//Added by Wang fengliang 070808;
	BOOL RemoveAt(CString);// ɾ�����е�һ���˵�����������������
	BOOL RemoveAll();

    CMenu* GetAt(int);//�õ�һ��
	CMenu* GetAt(CString sName);


	BOOL UpdateOthers(int,int);//���������˵�״̬

	BOOL SetCheck(int num,int menuItemIndex,BOOL flag);//��ǲ˵���
	//Added by Wang fengliang 070809;
	BOOL SetCheck(CString sName,int menuItemIndex,BOOL flag);
	BOOL UpdateOthers(CString,int);//���������˵�״̬

	BOOL ShowMenu(int,CPoint*,CWnd*);//��ʾ�˵�
	
	int GetSize(){return m_nCount;}
	int IsEmpty(){return m_nCount==0?TRUE:FALSE;}

};

#endif // !defined(AFX_POPUPMENU_H__6B412715_CEDA_4274_9EC6_2081374BCF02__INCLUDED_)
