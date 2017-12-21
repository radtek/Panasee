#if !defined(AFX_DLGIMPORTPATIENT_H__E2E4D98D_5117_478A_A58A_C676774BA430__INCLUDED_)
#define AFX_DLGIMPORTPATIENT_H__E2E4D98D_5117_478A_A58A_C676774BA430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImportPatient.h : header file
//
#include "PanaseeDialog.h"
#include "MRISeries.h"
#include "PanaListCtrl.h"
#include "IGSCPManager.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImportPatient dialog

class CDlgImportPatient : public CPanaseeDialog
{
// Construction
public:
	void CalibrationProcess();
	BOOL m_bInCalibrationProcess;

	CDlgImportPatient(CWnd* pParent = NULL);   // standard constructor
	
	
	
// Dialog Data
	//{{AFX_DATA(CDlgImportPatient)
	enum { IDD = IDD_DLG_IMPORT_PATIENT };
	CStatic	m_staticResult;
	CPanaListCtrl	m_seriesList;
	CString	m_sCaliResult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImportPatient)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImportPatient)
	afx_msg void OnButtonDicom();
	afx_msg void OnButtonDb();
	afx_msg void OnButtonLocal();
	afx_msg void OnButtonConfirm();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClickListSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonManual();
	afx_msg LRESULT OnMsgNewSeries(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDicomImageCount(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateList();
	void AutoCalibrate();

	CFont cTextFont;
	CIGSCPManager *m_pCPManager;
	CString m_sFolderName;
	CMRISeries *m_pNeedTobeCalibratedSeries;
	
	double m_dMarker2PT[4][4];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORTPATIENT_H__E2E4D98D_5117_478A_A58A_C676774BA430__INCLUDED_)
