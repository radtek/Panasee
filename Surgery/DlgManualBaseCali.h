#if !defined(AFX_DLGMANUALBASECALI_H__63E2D71E_5618_4A86_884A_D7D39D98B8BC__INCLUDED_)
#define AFX_DLGMANUALBASECALI_H__63E2D71E_5618_4A86_884A_D7D39D98B8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgManualBaseCali.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgManualBaseCali dialog
#include "MV.h"
#include "DlgMRIBrowse.h"
#include "XPStyleButtonST.h"
#include "PanaseeDialog.h"

class CMRISeries;
class CCryotherapyDlg;

class CDlgManualBaseCali : public CPanaseeDialog
{
// Construction
public:
	CDlgManualBaseCali(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgManualBaseCali)
	enum { IDD = IDD_DLG_OPTIC_MANUAL_CALIBRATION };
	CXPStyleButtonST	m_btnPhantom;
	CXPStyleButtonST	m_btnBase;
	CButton	m_btnCali;
	CButton	m_btnOK;
	CButton	m_btnLoadDB;
	CButton	m_btnLoadImg;
	CButton	m_btnLoadPhantom;
	CButton	m_btnLoadBase;
	CButton	m_btnManualCali;
	CButton	m_btnCir3;
	CButton	m_btnCir2;
	CButton	m_btnCir1;
	CButton	m_btnCancel;
	CComboBox	m_comboSeries3;
	CComboBox	m_comboSeries2;
	CComboBox	m_comboSeries1;
	//}}AFX_DATA

	CThemeHelperST		m_ThemeHelper;

	CCryotherapyDlg* m_pCryoDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgManualBaseCali)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMRISeries* m_pMRISeries[3];
	Matrix m_mPhatomGeo;
	Matrix m_mBaseINScan;
	Matrix m_mPhatomRefINTracker;				
	Matrix m_mBaseINTracker;					

	// Generated message map functions
	//{{AFX_MSG(CDlgManualBaseCali)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnManualCaliS1();
	afx_msg void OnManualCali();
	afx_msg void OnManualCaliLoadBaseintracker();
	afx_msg void OnManualCaliLoadPhantomref();
	afx_msg void OnManualCaliS2();
	afx_msg void OnManualCaliS3();
	afx_msg LRESULT OnMsgNewSeries(WPARAM wParam, LPARAM lParam);
	afx_msg void OnManualCaliLoadImg();
	afx_msg void OnManualCaliLoadDb();
	afx_msg void OnManualCaliCali();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool ManualCaliBase();

	CDlgMRIBrowse m_dlgMRIBrowse;

private:
	double m_dbCaliThreshold;	// Threshold to accept calibration result
	int		m_nImgNums;			// Number of images loaded
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMANUALBASECALI_H__63E2D71E_5618_4A86_884A_D7D39D98B8BC__INCLUDED_)
