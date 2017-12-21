#if !defined(AFX_DLGMRIBROWSE_H__F1D5D09A_561B_4D9A_ADEC_196A8C4E9E92__INCLUDED_)
#define AFX_DLGMRIBROWSE_H__F1D5D09A_561B_4D9A_ADEC_196A8C4E9E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMRIBrowse.h : header file
//

#include <afxtempl.h>
#include "MRISeries.h"
#include "DlgMRI2DDisplay.h"
#include "ImageButton.h"

class CCryotherapyDlg;

/////////////////////////////////////////////////////////////////////////////
// CDlgMRIBrowse dialog

class CDlgMRIBrowse : public CDialog
{
// Construction
public:
	CDlgMRIBrowse(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMRIBrowse();

	CCryotherapyDlg* m_pCryDlg;

	void SetMRISeries(CMRISeries* pMRISeries,int iIndex=0);
	void UpdateImageBMPs();
	void UpdateImage3Ds(int iInitImgIndex = -1);

	int GetDlgNum(); 
	CDlgMRI2DDisplay* GetDispDlgAt(int iIndex);

	BOOL m_bDisplayPolylineTool;

	void OnKeyDown(UINT nChar);

// Dialog Data
	//{{AFX_DATA(CDlgMRIBrowse)
	enum { IDD = IDD_DLG_MRI_BROWSE };
	CImageButton	m_btnPrtScr;
	CImageButton	m_btnQuit;
	CImageButton	m_btnPageUp;
	CImageButton	m_btnPageDown;
	CImageButton	m_btnF9;
	CImageButton	m_btnF8;
	CImageButton	m_btnF7;
	CImageButton	m_btnF6;
	CImageButton	m_btnF5;
	CImageButton	m_btnF4;
	CImageButton	m_btnF3;
	CImageButton    m_btnF2;
	CImageButton	m_btnF1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMRIBrowse)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMRIBrowse)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAddRow();
	afx_msg void OnDeleteRow();
	afx_msg void OnAddColumn();
	afx_msg void OnDeleteColumn();
	afx_msg void OnBtnBrowF1();
	afx_msg void OnBtnBrowF2();
	afx_msg void OnBtnBrowF3();
	afx_msg void OnBtnBrowF4();
	afx_msg void OnBtnBrowF5();
	afx_msg void OnBtnBrowF6();
	afx_msg void OnBtnBrowF7();
	afx_msg void OnBtnBrowF8();
	afx_msg void OnBtnBrowF9();
	afx_msg void OnBtnBrowPagedown();
	afx_msg void OnBtnBrowPageup();
	afx_msg void OnBtnBrowQuit();
	afx_msg void OnBtnBrowPrtScr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMRISeries* m_pDispMRISeries;
	CList<CDlgMRI2DDisplay*, CDlgMRI2DDisplay*> m_lstDispMRIDlgs;
	void OnPageDown();
	void OnPageUp();
	void Clean();
	void Layout();
	int m_iColumn,m_iRow;
	int m_iBeginIndex;
	CRect m_rcTextLabel;
	BOOL m_bLayouted;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRIBROWSE_H__F1D5D09A_561B_4D9A_ADEC_196A8C4E9E92__INCLUDED_)
