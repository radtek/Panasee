#if !defined(AFX_APPDIALOG_H__3B701907_1436_11D6_9F1F_0000E89676F0__INCLUDED_)
#define AFX_APPDIALOG_H__3B701907_1436_11D6_9F1F_0000E89676F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppDialog.h : header file
//

#include "stdafx.h"
#include "UtilDll.h"

class CFrmMgr;

/////////////////////////////////////////////////////////////////////////////
// CAppDialog dialog

class UTILDLL_API CAppDialog : public CDialog
{
// Construction
public:
	CAppDialog(UINT nID=9002, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAppDialog();
// Dialog Data
	//{{AFX_DATA(CAppDialog)
	//enum { IDD = IDD_APP_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTitle(const CString& szTitle) {m_szTitle = szTitle;}
	void SetID(UINT nID) {m_nID = nID;};
	UINT  GetID() { return m_nID;}
    void Resize();

	void SetFrameMgr(CFrmMgr* pMgr);
	void SendCommand(CWnd* pSender, int nCmdID,CString& sPara);
	void SendCommand(CWnd* pSender, int nCmdID);
	virtual void DispatchCommand(CWnd* pSender, int nCmdID,CString& sPara);
	virtual void DeactivateView() {};	// stop activities before 
										// a view goes hide

protected:
	CFrmMgr* m_pFrmMgr;

	CFrmMgr* GetFrameMgr() {return m_pFrmMgr;}

private:
	CString m_szTitle;
	UINT  m_nID;    // Dialog ID


public:

	// Generated message map functions
	//{{AFX_MSG(CAppDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDIALOG_H__3B701907_1436_11D6_9F1F_0000E89676F0__INCLUDED_)
