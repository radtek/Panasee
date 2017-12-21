#if !defined(AFX_DLGMAIN_H__11B3FD00_33F3_444F_A0C5_37392BC0815A__INCLUDED_)
#define AFX_DLGMAIN_H__11B3FD00_33F3_444F_A0C5_37392BC0815A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMain.h : header file
//


#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"
#include "DlgWizard.h"
#include "DlgBaseCalibration.h"
#include "TrackerManager.h"
#include "DlgMRIWrap.h"
#include "DlgDICOMSrv.h"
#include "DlgMRIBrowse.h"
#include "CompareFrame.h" 
#include "IGSCPManager.h"
#include "DlgTracking.h"
#include "DlgFusion.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog
#define DEVICE_IGS_MF 1
#define DEVICE_IGS_CP 2

extern CDlgMain *g_DlgMain;

class CDlgMain : public CDialog
{
// Construction
public:
	CDlgMain(CWnd* pParent = NULL);   // standard constructor
	~CDlgMain();

// Dialog Data
	//{{AFX_DATA(CDlgMain)
	enum { IDD = IDD_DLG_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void AddAMsgRcvWnd(CWnd* pWnd);
	void RemoveAMsgRcvWnd(CWnd* pWnd);
	void BroadcastMsg(UINT nMessage, WPARAM wParam, LPARAM lParam);

	void EnterSurgery();		// Enter surgery after hardware is ready
	BOOL m_bInSurgery;                                

	void ShowSeriesDisplay(CMRISeries *pSeries,BOOL bValidCryDlg);
	void ShowCompareDlg();

//	void Close();

	CCryotherapyDlg m_dlgCryotherapy;
	CDispaly4Dlg m_display4Dlg;
	CDlgWizard  m_dlgWizard;
	CDlgBaseCalibration *m_pBaseCaliDlg;
	CDlgMRIBrowse m_dlgMRIBrowse;
	CIGSCPManager m_CPManager;
	CDlgTracking  m_dlgTracking;
	CDlgFusion    m_dlgFusion;

	int m_DeviceType;
	BOOL m_bDebugNoCamera;
	BOOL m_bEnableMRICtrl;
	BOOL m_bHasCamera;

	CTrackerManager m_trackerManager;
	CDlgMRIWrap m_MRIWrap;
	CDlgDICOMSrv m_DICOMSrv;

	OPERATIONTYPE m_enumOperationType;                //记录操作类型
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDlgMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg LRESULT OnPutSeries(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CList<CWnd*, CWnd*> m_lstMsgRvsWnds;

	void CheckPatientID(CMRISeries *pSeries);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAIN_H__11B3FD00_33F3_444F_A0C5_37392BC0815A__INCLUDED_)
