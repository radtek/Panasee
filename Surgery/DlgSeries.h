#if !defined(AFX_DLGSERIES_H__CDD96303_8087_4865_A966_66A466E7465B__INCLUDED_)
#define AFX_DLGSERIES_H__CDD96303_8087_4865_A966_66A466E7465B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSeries.h : header file

#include "PanaListCtrl.h"
#include "DlgSeriesSkin.h"
//

class CCryotherapyDlg;

/////////////////////////////////////////////////////////////////////////////
// CDlgSeries dialog

class CDlgSeries : public CDlgSeriesSkin
{
// Construction
public:
	CDlgSeries(CWnd* pParent = NULL);   // standard constructor

	int  GetMRISeriesName(CStringArray& saMRIs, CStringArray* psaProbeNames = NULL, CStringArray* psaDesc = NULL);
	int  InsertMRIList(CStringArray& saMRIs, CStringArray* psaProbeNames = NULL);

	void DeleteMRIs(CStringArray& saMRIs);
	int  GetSelectedMRIName(CStringArray& saMRIs);

	BOOL IsInList(CString sMRISeriesName);

	CCryotherapyDlg* m_pCryDlg;

	

// Dialog Data
	//{{AFX_DATA(CDlgSeries)
	enum { IDD = IDD_DLG_SERIES };
	CImageButton	m_btnFusion;
	CImageButton	m_btnDicomList;
	CPanaListCtrl	m_ctrlSeriesList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSeries)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSeries)
	virtual BOOL OnInitDialog();
	afx_msg LONG OnGetSeriesList(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGetTrackData(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGetSeries(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDicomImageCount(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCrashUpdate( UINT wParam, LONG lParam );
	afx_msg void OnBegindragLstSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownLstSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDblclkLstSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDicomlist();
	afx_msg void OnButtonFusion();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DeleteSelectedMRISeries();

	double m_preImageTip[3];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIES_H__CDD96303_8087_4865_A966_66A466E7465B__INCLUDED_)
