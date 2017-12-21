#if !defined(AFX_DLGCTRLCOMPARE_H__ED2A4C05_A961_4ADA_B1EA_03247C6AB538__INCLUDED_)
#define AFX_DLGCTRLCOMPARE_H__ED2A4C05_A961_4ADA_B1EA_03247C6AB538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCtrlCompare.h : header file
//
#include "OleDropTargetEx.h"
#include "PanaListCtrl.h"

class CCompareFrame;

/////////////////////////////////////////////////////////////////////////////
// CDlgCtrlCompare dialog

class CDlgCtrlCompare : public CDialog
{
	
// Construction
public:
	CDlgCtrlCompare(CWnd* pParent = NULL);   // standard constructor

	int InitMIRList(CStringArray& saMRIs,CStringArray& saDesc);
	void Clean();

	void Bind();
	void Unbind();

	CCompareFrame* m_pCmpFrm;
	BOOL m_bBinded;

// Dialog Data
	//{{AFX_DATA(CDlgCtrlCompare)
	enum { IDD = IDD_DLG_COMPARE_CONTROL };
	CButton	m_btnOK;
	CButton	m_btnBind;
	CPanaListCtrl	m_ctlCmpMRIList;
	//}}AFX_DATA
	//CThemeHelperST		m_ThemeHelper;

//	CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCtrlCompare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	COleDropTargetEx m_dropEx;
	virtual BOOL OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	virtual DROPEFFECT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	virtual DROPEFFECT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);
	
	// Generated message map functions
	//{{AFX_MSG(CDlgCtrlCompare)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	afx_msg void OnCmpCtrlAssociate();
	virtual BOOL OnInitDialog();
	afx_msg void OnBegindragCmpListSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCTRLCOMPARE_H__ED2A4C05_A961_4ADA_B1EA_03247C6AB538__INCLUDED_)
