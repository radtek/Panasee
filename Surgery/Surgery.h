// Surgery.h : main header file for the Surgery application
//

#if !defined(AFX_Surgery_H__2A3484D0_313A_4F2E_A337_CC40C9007801__INCLUDED_)
#define AFX_Surgery_H__2A3484D0_313A_4F2E_A337_CC40C9007801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "GlobalFuncs.h"

/////////////////////////////////////////////////////////////////////////////
// CSurgeryApp:
// See Surgery.cpp for the implementation of this class
//

extern CString g_ExecPath;

class CSurgeryApp : public CWinApp
{
public:
	CSurgeryApp();

	//CAppFace m_af ; //deleted by ningsj
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurgeryApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSurgeryApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
private:
	BOOL SysValidate();

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Surgery_H__2A3484D0_313A_4F2E_A337_CC40C9007801__INCLUDED_)
