// OleDropTargetEx.h: interface for the COleDropTargetEx class.

#if !defined(AFX_OLEDROPTARGETEX_H__I_LOVE_MY_MOTHERLAND)
#define AFX_OLEDROPTARGETEX_H__I_LOVE_MY_MOTHERLAND

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OleDropTargetEx.h header file
//

// WangJiaBao,ShenYang,China
// 王加宝，中国沈阳

// Copyright (c) 2001-2002, WangJiaBao
// May be freely used provided this comment is included with the
// source and all derived versions of this source

// Usage:
// 1.in your CWnd derived class,define a COleDropTargetEx member
//   variable (for example m_dropTarget)
// 2.define a member function Register(),in it call 
//   m_dropTarget.Register(this)
// 3.map user message DROPM_XXXX,message's WPARAM is a COleDropInfo
//   pointer,you can get detail drop information in it
// 4.in OnInitDialog() or OnInitialUpdate() call the class' Register()
// remember call AfxOleInit() in CXXXXAPP::InitInstance

///////////////////////////////////////////////////////////////////////////
//Drag & Drop User define Messages
#define DROPM_DRAGENTER				WM_USER + 1976
#define DROPM_DRAGOVER				WM_USER + 1977
#define DROPM_DROP					WM_USER + 1978
#define DROPM_DROPEX				WM_USER + 1979
#define DROPM_DRAGLEAVE				WM_USER + 1980

/////////////////////////////////////////////////////////////////////////////
//Drop info class
//为拖动消息响应函数提供拖动信息
class COleDropInfo : public CObject
{
public:
	COleDropInfo();
	COleDropInfo(COleDataObject* pDataObject,DWORD dwKeyState,
		DROPEFFECT dropEffect,DROPEFFECT dropEffectList,CPoint point );
	virtual ~COleDropInfo();

public:
	//COleDropTargetEx member functions params
	COleDataObject* m_pDataObject;
	DWORD m_dwKeyState;
	DROPEFFECT m_dropEffect;
	DROPEFFECT m_dropEffectList;
	CPoint m_point;

	DECLARE_DYNAMIC( COleDropInfo );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//COleDropTargetEx class 
class COleDropTargetEx : public COleDropTarget
{
public:
	COleDropTargetEx();
	virtual ~COleDropTargetEx();

protected:
//处理OnDropEx消息是必须的，否则OnDrop函数不会被执行
//当然也可以在OnDropEx函数中处理数据
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
};

#endif // !defined(AFX_OLEDROPTARGETEX_H__I_LOVE_MY_MOTHERLAND)
