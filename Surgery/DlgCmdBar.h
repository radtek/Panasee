#if !defined(AFX_DLGCMDBAR_H__88077D34_7A66_449D_9187_B7E4B6C54A75__INCLUDED_)
#define AFX_DLGCMDBAR_H__88077D34_7A66_449D_9187_B7E4B6C54A75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCmdBar.h : header file
//
//#include "BtnSt.h"
#include "XPStyleButtonST.h"

#include "DlgToolSkin.h"
#include "PatientInfoMrgDlg.h"
#include "MenuEx.h"

class CCryotherapyDlg;


/////////////////////////////////////////////////////////////////////////////
// CDlgCmdBar dialog

class CDlgCmdBar : public CDlgToolSkin
{
// Construction
public:
	CDlgCmdBar(CWnd* pParent = NULL);   // standard constructor

	CCryotherapyDlg* m_pCryDlg;
	CPatientInfoMrgDlg  m_dlgPatSearch;
	void UpdateUI();

	void EnableLR(BOOL bEnable);
	void EnableMeasure(BOOL bEnable);
	void EnableCreatVTarget(BOOL bEnable);

	BOOL SysImportSeries();
	void OpenDatabase();
	
	BOOL m_bTuneInNextScan;
	BOOL m_bTableCalibrated;

// Dialog Data
	//{{AFX_DATA(CDlgCmdBar)
	enum { IDD = IDD_CMD_BAR };

	CImageButton	m_BtnSys;
	CImageButton	m_btnMovePT;
	CImageButton	m_btnVTarget;
	CImageButton	m_btnMeasure;
	CImageButton	m_btnImgDisplay;
	CImageButton	m_btnScreenSnap;
	CImageButton	m_btnImgCompare;
	CImageButton	m_btnCameraMove;
	CImageButton	m_btnTracking;
	BOOL	m_bChkMeasure;
	BOOL	m_bChkVTarget;
	BOOL	m_bChkLR;
	//}}AFX_DATA

	CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCmdBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCmdBar)
	afx_msg void OnCmdCompare();
	afx_msg void OnCmdStartTracking();
	afx_msg void OnBtnSeriesDisplay();
	afx_msg void OnChkMeasure();
	afx_msg void OnCmdCamerMove();
	afx_msg void OnCmdScreenCap();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkTarget();
	afx_msg void OnCmdMovePt();
	afx_msg void OnSysExit();
	afx_msg void OnSysImportSeries();
	afx_msg void OnSysOpenDb();
	afx_msg void OnSysAbout();
	afx_msg void OnSysCaliPt();
	afx_msg void OnSysConnectCam();
	afx_msg LRESULT On_MenuCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSysDBOK(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysConnectMri();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMsgProtoclFamily(UINT nID);
	afx_msg void OnSysStartMonitor();
	afx_msg void OnSysCaliTune();
	afx_msg void OnSysSurgeryWizard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMenuEx m_sysMenu;

	BOOL m_bLastTrackingStatus;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCMDBAR_H__88077D34_7A66_449D_9187_B7E4B6C54A75__INCLUDED_)
