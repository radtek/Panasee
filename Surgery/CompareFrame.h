#if !defined(AFX_COMPAREFRAME_H__84A6FE4C_6FB5_4C9C_9233_9663588E7EAB__INCLUDED_)
#define AFX_COMPAREFRAME_H__84A6FE4C_6FB5_4C9C_9233_9663588E7EAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompareFrame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCompareFrame frame

#include "DlgDispCompare.h"
#include "DlgCtrlCompare.h"

class CCompareFrame : public CDialog
{
public:
	CCompareFrame(CWnd* pParent =NULL);           // protected constructor used by dynamic creation

// Attributes
public:
	enum { IDD = IDD_DLG_COMPARE_FRAME };
	
	CDlgCtrlCompare m_DlgCtrl;
	CDlgDispCompare m_DlgDisp;

// Operations
public:
	void Init(CStringArray& saMRIs,CStringArray& saDesc);
	int Clean();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompareFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompareFrame();

	// Generated message map functions
	//{{AFX_MSG(CCompareFrame)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_saMRIs;
	CStringArray m_saDesc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPAREFRAME_H__84A6FE4C_6FB5_4C9C_9233_9663588E7EAB__INCLUDED_)
