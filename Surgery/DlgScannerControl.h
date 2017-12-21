#if !defined(AFX_DLGSCANNERCONTROL_H__CD3A507D_3AB5_41A1_B0F7_D90AD8DC5352__INCLUDED_)
#define AFX_DLGSCANNERCONTROL_H__CD3A507D_3AB5_41A1_B0F7_D90AD8DC5352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScannerControl.h : header file
//

#include <afxtempl.h>
#include "header.h"
#include "Geometry.h"
#include "MV.h"
//#include "XPStyleButtonST.h"
#include "ImriDefine.h"
#include "Probe.h"
#include "MenuEx.h"
#include "DlgMriScanOptions.h"
#include "DlgMriCtrlSkin.h"

class CCryotherapyDlg;
class CDlgMRIWrap;

enum UI_PRESCANSCAN_BTN_STATUS {
	PSBT_SCAN,
	PSBT_PAUSE,
	PSBT_RESUME,
};

enum UI_STATUS {
	US_NO_MRI,
	US_MRI_CONN_LOST,
	US_READY_TO_CHOOSE_PROTOCOL,
	US_READY_TO_LOAD,
	US_READY_TO_PRESCAN,
	US_READY_TO_SCAN,
	US_READY_TO_CYCLE_SCAN,			// cycle scan after first scan
	US_READY_TO_PAUSE,
	US_READY_TO_RESUME,
	US_READY_TO_STOP_PRESCAN,
	US_READY_TO_STOP_SCAN,
};

/////////////////////////////////////////////////////////////////////////////
// CDlgScannerControl dialog

class CDlgScannerControl : public CDlgMriCtrlSkin
{
// Construction
public:
	CDlgScannerControl(CWnd* pParent = NULL);   // standard constructor

	CDlgMriScanOptions m_dlgScanOption;
// Dialog Data
	//{{AFX_DATA(CDlgScannerControl)
	enum { IDD = IDD_DLG_SCANNER_CONTROL };
	CPanaStatic	m_scanTime;
	CImageButton	m_btnSubOffset;
	CImageButton	m_btnScrollOffset;
	CImageButton	m_btnAddOffset;
	CPanaEdit	m_edtOffset;
	CPanaStatic	m_lblLengthUnit;
	CPanaStatic	m_lblProtocol;
	CPanaStatic	m_lblOrientation;
	CPanaStatic	m_lblPosition;
	CPanaStatic	m_lblSwap;
	CImageButton	m_btnSwapOff;
	CImageButton	m_btnSwapOn;
	CImageButton	m_btnChkAdv;
	CImageButton	m_btnPlane3;
	CImageButton	m_btnPlane2;
	CImageButton	m_btnPlane1;
	CImageButton	m_btnDelProbe;
	CImageButton	m_btnSimulate;
	CImageButton	m_btnNewProbe;
	CImageButton	m_btnProbeSel;
	CImageButton	m_btnRadioProbe;
	CImageButton	m_btnRadioLastPos;
	CImageButton	m_BtnProtocol;
	CImageButton	m_BtnStopScan;
	CImageButton	m_BtnScan;
	CImageButton	m_BtnPrescanScan;
	CImageButton	m_BtnPrescan;
	CProgressCtrl m_ctlScanProgress;
	//}}AFX_DATA

	int		m_iLocalizationMode;
	int		m_iLocalizationPlane;
	int		m_nOffset;
	BOOL	m_bChkSwap;

	//CThemeHelperST		m_ThemeHelper;

	CCryotherapyDlg *m_pCryDlg;
	int		m_iCntCycleScan;	// Used to remember how many scan done in cycle scanning
	MRIScanOptions m_ScanOption;

	BOOL m_bProgress;
	void StopScanProgress();
	void StopPreScanProgress();

	void CreateProtocolMenu(CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* pBdyPrtLst);

	void UpdateMRIScannerControlUI2(ENUM_MRI_SYS_STATUS nSysStatus, 
		unsigned short nSeriesStatus = 0, int nOperResult = 0);

	void UpdateSelectedProtocol(void *pProtocol, IMRI_PROTOCOL_GROUP *pBdyPrtProt);

	void SetMRIScannerControlUI(enum UI_STATUS eStatus);

	void EnableProbeControl(BOOL bEnable);

	// Get the Scan Control parameters, all values are in scanner coordinate systems
	void GetScanControlPar( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix,
							Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir );
	
	CProbe* m_pCurRealProbe;
	void InitPreimageProbe();
	void DeleteCurrentProbe();
	int SetProbeUIStatus();

	void CreateProbeMenu();
	void SelecteProbe(CProbe *pProbe);

	enum UI_PRESCANSCAN_BTN_STATUS m_ePSBT; // indicate button status

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScannerControl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScannerControl)
	afx_msg LONG OnMsgScanProgress( UINT wParam, LONG lParam);
	
	afx_msg void OnCmdPreScan();
	afx_msg void OnCmdScan();
	afx_msg void OnCmdPrescanScan();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCmdScanStop();
	afx_msg void OnChangeEdtOffset2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnProbeNew();
	afx_msg void OnBtnProbeSimulate2();
	afx_msg void OnBtnProbeDelete();
	afx_msg void OnCmdRadioProbe();
	afx_msg void OnCmdRadioLastPos();
	afx_msg void OnCmdRadioPlane1();
	afx_msg void OnCmdRadioPlane2();
	afx_msg void OnCmdRadioPlane3();
	afx_msg void OnCmdChkAdvance();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdSub();
	afx_msg LONG OnMoveCtrl(WPARAM w,LPARAM l);
	afx_msg LONG OnMoveCtrl_LUP(WPARAM w,LPARAM l);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnCmdChkSwapOff();
	afx_msg void OnCmdChkSwapOn();
	afx_msg void OnCmdProtocl(UINT nID);
	afx_msg void OmCmdProbe(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	int m_iScanNeedTime;
	void LoadProtocol();

	void *m_pCurMRIProtocol;
	Vector3D m_vLastScanCenter;
	Vector3D m_vLastScanPhaseDir;
	Vector3D m_vLastScanFreqDir;
	
	bool CalculateAndSendMRILocalization();
	bool SendLastScanMRILocalization();

	// SCANPLANETYPE m_iScanPlaneType; // Plane on which the target is scanned

	void GetMarkerPlane( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix,
							Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir );
	void GetPlanePerpendicularToMarker( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix,
										Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir );
	void GetPlanePerpendicularToProbe( Vector3D vProbeCenter, Vector3D vProbeOri, Matrix mProbeMatrix,
										Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir );

	bool IsLocParValid( Vector3D vCenter );


	void StartPreScanProgress();

//	int  m_iRevStartCnt;
	Matrix m_mLastBase2Tracker; //Add by Dai Liang 2007/6/12

	CMenuEx m_menuProtocol;
	CMenuEx m_menuProbe;


	BOOL m_bLoopCurrScan;	// indicate current scan is loop scan or nor
	

	void StartPrescanScan();
	void PauseScan();
	void ResumeScan();

	void SetPrescanScanStatus(enum UI_PRESCANSCAN_BTN_STATUS ePSBT);

	BOOL	m_bPreScanDone;		// Used to remember whether prescan been done or not
	BOOL	m_bInPause;			// Remember whether Pause has been pressed

	
//	m_arProbeFinishedToolMatrix.Add(m_pCurRealProbe->m_mProbeMarker2Tracker);
//		m_arProbeFinishedTip.Add(m_pCurRealProbe->GetTip());
//		m_arProbeFinishedOri.Add(m_pCurRealProbe->GetOrientation()); 
	CDlgMRIWrap* m_pMRIWrap;

	void SetOffsetEdtValue(int nOffset);
	BOOL m_bNoRefreshInScroll;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCANNERCONTROL_H__CD3A507D_3AB5_41A1_B0F7_D90AD8DC5352__INCLUDED_)
