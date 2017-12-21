#if !defined(AFX_DLGCALIPT_H__12AF77DB_557F_4D2D_B34D_C5773B8FDF13__INCLUDED_)
#define AFX_DLGCALIPT_H__12AF77DB_557F_4D2D_B34D_C5773B8FDF13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliPT.h : header file
//
#include <afxtempl.h>
#include "Geometry.h"
#include "XPStyleButtonST.h"
#include "PanaseeDialog.h"

class CCryotherapyDlg;

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliPT dialog

class CDlgCaliPT : public CPanaseeDialog
{
// Construction
public:
	CDlgCaliPT(CWnd* pParent = NULL);   // standard constructor
	~CDlgCaliPT();

	CCryotherapyDlg* m_pCryoDlg;
	Vector3D	m_vPTDirInScan;		// Calibration result

// Dialog Data
	//{{AFX_DATA(CDlgCaliPT)
	enum { IDD = IDD_DLG_CALI_PT };
	CXPStyleButtonST	m_btnIndicator;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnReadPos;
	CButton	m_btnCaliPT;
	CButton	m_btnClean;
	//}}AFX_DATA

	CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliPT)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliPT)
	virtual BOOL OnInitDialog();
	afx_msg void OnReadPos();
	afx_msg void OnCaliPt();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCaliClean();
	afx_msg LONG OnCollectionDone( UINT wParam, LONG lParam );
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int			m_nPosCnt;
	CArray<Vector3D, Vector3D> m_arrVPos;

	BOOL		m_bInCollecting;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIPT_H__12AF77DB_557F_4D2D_B34D_C5773B8FDF13__INCLUDED_)
