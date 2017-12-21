#if !defined(AFX_DLGSETUPLEGEND_H__94EE37A6_8714_4D1E_83A6_999AD4C791F4__INCLUDED_)
#define AFX_DLGSETUPLEGEND_H__94EE37A6_8714_4D1E_83A6_999AD4C791F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupLegend.h : header file
//
#include "PanaseeDialog.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupLegend dialog

class CDlgSetupLegend : public CPanaseeDialog
{
// Construction
public:
	CDlgSetupLegend(CWnd* pParent = NULL);   // standard constructor

	CList<int, int> lstCTLeftTop;
	CList<int, int> lstCTLeftBottom;
	CList<int, int> lstCTRightTop;
	CList<int, int> lstCTRightBottom;
	CList<int, int> lstMRLeftTop;
	CList<int, int> lstMRLeftBottom;
	CList<int, int> lstMRRightTop;
	CList<int, int> lstMRRightBottom;

// Dialog Data
	//{{AFX_DATA(CDlgSetupLegend)
	enum { IDD = IDD_DLG_SETUP_LEGEND };
	CListBox	m_lstRight;
	CListBox	m_lstLeft;
	CListBox	m_lstCenter;
	CComboBox	m_cmbCategory;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnDefault;
	CImageButton	m_btnRightUp;
	CImageButton	m_btnRightRight;
	CImageButton	m_btnRightLeft;
	CImageButton	m_btnRightDown;
	CImageButton	m_btnLeftUp;
	CImageButton	m_btnLeftRight;
	CImageButton	m_btnLeftLeft;
	CImageButton	m_btnLeftDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetupLegend)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetupLegend)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnLeftUp();
	afx_msg void OnBtnLeftDown();
	afx_msg void OnBtnLeftLeft();
	afx_msg void OnBtnLeftRight();
	afx_msg void OnBtnRightLeft();
	afx_msg void OnBtnRightRight();
	afx_msg void OnBtnRightUp();
	afx_msg void OnBtnRightDown();
	afx_msg void OnBtnDefault();
	virtual void OnOK();
	afx_msg void OnSelchangeComboCategory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Resort();
	void Initial();
	void GetIndex(CString sIndex,CList<int,int> *m_pList);
	void SetIndexStr(CString &sIndex, CList<int,int> *m_pList);

	CList<CString, CString> lstCTLegend;
	CList<CString, CString> lstMRLegend;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPLEGEND_H__94EE37A6_8714_4D1E_83A6_999AD4C791F4__INCLUDED_)
