#if !defined(AFX_DLGBASECALIBRATION_H__FF4DCCDE_5F74_4556_8B07_E24CB96D7123__INCLUDED_)
#define AFX_DLGBASECALIBRATION_H__FF4DCCDE_5F74_4556_8B07_E24CB96D7123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "header.h"					//Add by Dai Liang 2007.6.5
#include "MRISeries.h"				//Add by Dai Liang 2007.6.5
#include "MV.h"						//Add by Dai Liang 2007.6.5
#include "MriCalibration.h"			//Add by Dai Liang 2007.6.5
#include "ErrorCodes.h"
#include "XPStyleButtonST.h"
#include "GlobalFuncs.h"
#include "PanaseeDialog.h"
// DlgBaseCalibration.h : header file
//

class CCryotherapyDlg;
class CDlgMRIWrap;

/////////////////////////////////////////////////////////////////////////////
// CDlgBaseCalibration dialog

class CDlgBaseCalibration : public CPanaseeDialog
{
// Construction
public:
	CDlgBaseCalibration(CWnd* pParent = NULL);   // standard constructor
	CMRISeries* m_pMRISeries[3];					//Add by Dai Liang 2007.6.5
	Matrix m_mPhatomGeo;							//Add by Dai Liang 2007.6.5
	Matrix m_mBaseINScan;							// Calibration result//Add by Dai Liang 2007.6.5
	int m_iStep;									//记录操作步骤，分2步;
	int m_iNumOfImage;								//Add by Dai Liang 2007.6.5
	void OnScanFinished();							//Add by Dai Liang 2007.6.5
	void LogCaliResult(ERR_CODE eCalErr, double* pderr, Matrix m_mBaseINScan, CALI_ERR_RESULT* pCaliErr, CMRISeries* pMRISeries[3]);

	void RespondMRIStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
		unsigned short nSeriesStatus = 0, int nOperResult = 0);
	
// Dialog Data
	//{{AFX_DATA(CDlgBaseCalibration)
	enum { IDD = IDD_DLG_BASECALIBRATION };
	CXPStyleButtonST	m_btnPhantom;
	CXPStyleButtonST	m_btnBase;
	CProgressCtrl	m_ctlProgressCaliScan;
	CProgressCtrl	m_ctlProGressCalibrate;
	CButton	m_btnStart;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	//}}AFX_DATA

	CThemeHelperST		m_ThemeHelper;


	CCryotherapyDlg* m_pCryoDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBaseCalibration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBaseCalibration)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg LRESULT OnMsgNewSeries(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCaliStart();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double m_dbCaliThreshold;	// Threshold to accept calibration result
	void ShowMsg(CString sMsg);

	int m_iScanTime;
	int m_iEstScanTime;
	int m_iScanedTime;

	CDlgMRIWrap* m_pMRIWrap;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBASECALIBRATION_H__FF4DCCDE_5F74_4556_8B07_E24CB96D7123__INCLUDED_)
