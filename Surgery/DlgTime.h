#if !defined(AFX_DLGTIME_H__1E016754_6029_4F7F_A7D0_636071EDF1DC__INCLUDED_)
#define AFX_DLGTIME_H__1E016754_6029_4F7F_A7D0_636071EDF1DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTime.h : header file
//

#include "DlgTimeSkin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTime dialog

class CDlgTime : public CDlgTimeSkin
{
// Construction
public:
	CDlgTime(CWnd* pParent = NULL);   // standard constructor
	~CDlgTime();

	HANDLE hCloseEvent;

	void StopTime();

// Dialog Data
	//{{AFX_DATA(CDlgTime)
	enum { IDD = IDD_DLG_TIME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTime)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMsgTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hTimeThread;

	void DrawTime();

	CRect m_rectTime;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIME_H__1E016754_6029_4F7F_A7D0_636071EDF1DC__INCLUDED_)
