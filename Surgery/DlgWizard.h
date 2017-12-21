#if !defined(AFX_DLGWIZARD_H__85896D00_122A_4125_99AF_B120279D80FE__INCLUDED_)
#define AFX_DLGWIZARD_H__85896D00_122A_4125_99AF_B120279D80FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWizard.h : header file
//
//#include "FontStatic.h"
#include "DlgBaseSelection.h"
#include "DlgCaliModeSel.h"
#include "GlobalFuncs.h"
#include "PatientInfoDlg.h"
#include "DlgPatientInfo.h"
#include "DlgImportPatient.h"
#include "DlgWizardSkin.h"
#include "DlgManualCalibrationCP.h"

class CDlgMain;

enum WIZARD_STEP {
	WSTEP_CAM = 1,
	WSTEP_MRI,
	WSTEP_PAT,
	WSTEP_BAS,
	WSTEP_CAL,
	WSTEP_END
};

enum WIZARD_STEP_RESULT {
	WSRET_OK = 0,
	WSRET_ERR
};

/////////////////////////////////////////////////////////////////////////////
// CDlgWizard dialog

class CDlgWizard : public CDlgWizardSkin
{
// Construction
public:
	CDlgWizard(CWnd* pParent = NULL);   // standard constructor

	void SetStep(WIZARD_STEP nStep, WIZARD_STEP_RESULT nRet);
	void SetOperType(OPERATIONTYPE nOperType);

	void Redo();

	void ShowMRIPatientConfirmDlg(IGT_PatientField *patInfor,IMRI_STUDY *pStudy, CString sModalityID);
	BOOL ManualInputPatientInfo();

	CDlgBaseSelection m_dlgBaseSel;
	CDlgCaliModeSel m_dlgCaliModeSel;
	CPatientInfoDlg m_dlgPatInfo;
	CDlgPatientInfo m_dlgMRIPatInfo;
	CDlgImportPatient m_dlgImportPatient;
	CDlgManualCalibrationCP m_dlgManualCP;
	BOOL m_bExitWholeProgram;

	CDlgMain* m_pDlgMain;

// Dialog Data
	//{{AFX_DATA(CDlgWizard)
	enum { IDD = IDD_DLG_WIZARD };
	CImageButton	m_btnRedo;
	CImageButton	m_btnExit;
	//}}AFX_DATA

	//CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual CSize CalcDynamicLayout(int nLength,DWORD nMode);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizard)
	afx_msg LONG OnInitDialog(UINT wParam, LONG lParam);	
	afx_msg LRESULT OnWizardStep(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDLGPatientInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMRIPatientInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCmdRedo();
	afx_msg void OnBtnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:


	
	BOOL m_bEnableMRICtrl;
	
	enum WIZARD_STEP m_iStep;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWIZARD_H__85896D00_122A_4125_99AF_B120279D80FE__INCLUDED_)
