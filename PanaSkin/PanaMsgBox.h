#if !defined(AFX_PANAMSGBOX_H__2A4E0B3C_5FDB_4034_88CA_E88BAC554881__INCLUDED_)
#define AFX_PANAMSGBOX_H__2A4E0B3C_5FDB_4034_88CA_E88BAC554881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanaMsgBox.h : header file
//
#include "PanaseeDialog.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPanaMsgBox dialog


class CPanaMsgBox : public CPanaseeDialog
{
// Construction
public:
	CPanaMsgBox(CWnd* pParent = NULL);   // standard constructor

	void SetMessage(CString szMsg,UINT nType);
	void SetTitle(CString szTitle);
// Dialog Data
	//{{AFX_DATA(CPanaMsgBox)
	enum { IDD = IDD_DLG_MSGBOX };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaMsgBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanaMsgBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_szMsg,m_szTitle;
	UINT m_nMsgType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANAMSGBOX_H__2A4E0B3C_5FDB_4034_88CA_E88BAC554881__INCLUDED_)
