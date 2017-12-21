#if !defined(AFX_DLGSELECTSERIES_H__0148DF51_A188_43C5_8355_1B0794006B1E__INCLUDED_)
#define AFX_DLGSELECTSERIES_H__0148DF51_A188_43C5_8355_1B0794006B1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectSeries.h : header file
//
#include "PanaListCtrl.h"
#include "PanaseeDialog.h"
#include "MriSeries.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSelectSeries dialog

class CDlgSelectSeries : public CPanaseeDialog
{
// Construction
public:
	CDlgSelectSeries(CWnd* pParent = NULL);   // standard constructor

	CString m_sName;
	CMRISeries *m_pSeries;
// Dialog Data
	//{{AFX_DATA(CDlgSelectSeries)
	enum { IDD = IDD_DIALOG_SERIES_SELECT };
	CButton	m_btnCancel;
	CPanaListCtrl	m_seriesList;
	CButton	m_btnSelect;
	CButton	m_btnImport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectSeries)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectSeries)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Refresh();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTSERIES_H__0148DF51_A188_43C5_8355_1B0794006B1E__INCLUDED_)
