/////////////////////////////////////////////////////////////////////////////////
// File Name:   FrmMgr.h - header file
// Description: Basic class to manage a list of AppDialog class pointer
//				SendCommand() and DispatchCommand() among the pointers.
//
// Auther:	Y.L. Zhou
// History: YLZ 2/19/02 Create
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRMMGR_H__F2E58E44_091D_11D6_85E5_00D0C9236746__INCLUDED_)
#define AFX_FRMMGR_H__F2E58E44_091D_11D6_85E5_00D0C9236746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrmMgr.h : header file
//

#include "afxTempl.h"    // MFC template supports
#include "UtilDll.h"



class CAppDialog;

/////////////////////////////////////////////////////////////////////////////
// CFrmMgr command target
class UTILDLL_API CFrmMgr : public CObject
{
	DECLARE_DYNCREATE(CFrmMgr)

	CFrmMgr();           // protected constructor used by dynamic creation
	virtual ~CFrmMgr();

	// Implementation
public:
	
	virtual void CreateContents();
	void DestroyContents();
	void ShowContents(int nCmdShow);

	void SendCommand(CWnd* pSender, int nCmdID,CString& sPara);

	void SetType(CString szType) {m_szType = szType;}
	CString GetType() {return m_szType;}
	int GetSize() { return m_appDlgArr.GetSize();}
	CAppDialog* GetAt(int nIndex);

protected:
	void RemoveAt(int nIndex);
	void InsertAt(int nIndex, CAppDialog* pDlg);
	void Add(CAppDialog* pDlg);

private:
	CString m_szType;
	CArray<CAppDialog*, CAppDialog*> m_appDlgArr;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMMGR_H__F2E58E44_091D_11D6_85E5_00D0C9236746__INCLUDED_)
