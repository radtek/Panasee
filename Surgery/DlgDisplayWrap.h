#if !defined(AFX_DLGDISPLAYWRAP_H__79F141F7_92A4_443B_AF74_CB7DDA65BC83__INCLUDED_)
#define AFX_DLGDISPLAYWRAP_H__79F141F7_92A4_443B_AF74_CB7DDA65BC83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDisplayWrap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayWrap dialog

#include "DisplayDlg.h"
#include "DlgDisplayControl.h"

class MRIRectSeries;

class CDlgDisplayWrap : public CDialog
{
// Construction
public:
	void SetWindowWidthAndLevel(CPoint prePoint, CPoint curPoint,BOOL bUpdateAllImage);

	void AddMRIRect(CMRIRectangle *pMRIRect, BOOL bClean = TRUE);
	void AddMRIRectSeries(MRIRectSeries *pMRIRectSeries, int iInitShowImgIndex = 0);
	BOOL m_bShowCtrl;
	void Layout();
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	CDlgDisplayWrap(CWnd* pParent = NULL);   // standard constructor

	CDisplayDlg m_dlgDisplay;
	CDlgDisplayControl m_dlgDispControl;

	MRIRectSeries *m_pDispSeries;


	// For compare window
	int  m_iLinkIndex;
	void SetBind();
	BOOL m_bComparison;

// Dialog Data
	//{{AFX_DATA(CDlgDisplayWrap)
	enum { IDD = IDD_DLG_DISPLAY_WRAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDisplayWrap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDisplayWrap)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISPLAYWRAP_H__79F141F7_92A4_443B_AF74_CB7DDA65BC83__INCLUDED_)
