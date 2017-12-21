#if !defined(AFX_TRACERPROPDLG_H__2B1E4325_C285_49A7_A901_B34CE061F989__INCLUDED_)
#define AFX_TRACERPROPDLG_H__2B1E4325_C285_49A7_A901_B34CE061F989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracerPropDlg.h : header file
//
#include "resource.h"
#include <XTToolkitPro.h> 		// Xtreme Toolkit support
#include "PanaseeDialog.h"
#include "TpsGlobalConst.h"
/////////////////////////////////////////////////////////////////////////////
// CPropDlgTracer dialog

class CPropDlgTracer : public CPanaseeDialog
{
// Construction
public:
	CPropDlgTracer(CWnd* pParent = NULL);   // standard constructor
	CString m_TraceName;
	CString m_groupName;

// Dialog Data
	//{{AFX_DATA(CPropDlgTracer)
	enum { IDD = IDD_TRACER_PROPERTY };
	CButton	m_btnClose;
	CStatic	m_placeHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlgTracer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropDlgTracer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CList<ZONEGROUP_INFO,ZONEGROUP_INFO&> *m_tracerGrpList;
	
	void OnChangeGroup(CString para);
	void OnChangeName(CString para);
	CXTPPropertyGrid m_PropGrid;
	CXTPPropertyGridItem *m_pItemName,*m_pItemStyle,*m_pItemGroup,
		*m_pItemColor,*m_pItemSliceIndex,*m_pItemTextScale,
		*m_pItemTextVisible,*m_pItemTextColor;
	POLY_INFO m_defaultPolyInfo;
protected:
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACERPROPDLG_H__2B1E4325_C285_49A7_A901_B34CE061F989__INCLUDED_)
