#if !defined(AFX_DLGDISPCOMPARE_H__C33F444F_1010_4C13_B52C_692854F64356__INCLUDED_)
#define AFX_DLGDISPCOMPARE_H__C33F444F_1010_4C13_B52C_692854F64356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDispCompare.h : header file
//

#include "DlgDisplayWrap.h"
#include "MRIRectSeries.h"

class CMRISeries;

/////////////////////////////////////////////////////////////////////////////
// CDlgDispCompare dialog

class CDlgDispCompare : public CDialog
{
	
// Construction
public:
	CDlgDispCompare(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDispCompare)
	enum { IDD = IDD_DLG_COMPARE_DISP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void Clean();		// Clean all windows

	int DisplayMRISeriesToWindow(CMRISeries* pMRISeries, CDisplayDlg* pDispDlg);
	void BindMRISeries(BOOL bBind);
	void ShowBindedMRIs(int nStep, CDlgDisplayWrap* pSendWrap);  // nStep = difference from linked base image
	void UpdateTextureSameSeries(MRIRectSeries *pMRIRectSeries,BOOL bUpdateAllImage);

	CDlgDisplayWrap m_DlgCmpDispWrap[4];		//4¸ö2DÏÔÊ¾´°
	MRIRectSeries m_MRIRectSeriesCmp[4];

	BOOL  m_bBinded;		// bind all window or not;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDispCompare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDispCompare)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISPCOMPARE_H__C33F444F_1010_4C13_B52C_692854F64356__INCLUDED_)
