#if !defined(AFX_DLGTRACKINGSTATUS_H__24B7F581_5528_4521_A18B_E4B30838222A__INCLUDED_)
#define AFX_DLGTRACKINGSTATUS_H__24B7F581_5528_4521_A18B_E4B30838222A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTrackingStatus.h : header file
//
//#include "XPStyleButtonST.h"
//#include "DlgMsgs.h"
//#include "ToolTipComboBox.h"
//#include "TTComboBox.h"

#include "DlgInfoSkin.h"
#include "DlgInfoHist.h"
#include "DlgTracker.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTrackingStatus dialog

class CCryotherapyDlg;
class CDlgTrackingStatus : public CDlgInfoSkin
{
// Construction
public:
	void SetBaseRange(double *pos,int range);
	CDlgTrackingStatus(CWnd* pParent = NULL);   // standard constructor

	void SetBaseIndex(int nBaseIndex);
	void SetBaseStatus(BOOL bIsVisible);
	void SetProbeStatus(BOOL bIsVisible);
	void AddMessage(CString sMsg);

	CCryotherapyDlg* m_pCryoDlg;
	CDlgTracker  m_dlgTracker;
	
// Dialog Data
	//{{AFX_DATA(CDlgTrackingStatus)
	enum { IDD = IDD_DLG_TRACKING_STATUS };
	CImageButton	m_btnPrev;
	CImageButton	m_btnNext;
	CPanaStatic	m_lblInfo;
	//}}AFX_DATA

	//CThemeHelperST		m_ThemeHelper;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTrackingStatus)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTrackingStatus)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	virtual void OnOK();
	afx_msg void OnCmdNxt();
	afx_msg void OnCmdPrv();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_sBaseTip;

	BOOL m_bLastBaseVisible;
	BOOL m_bLastProbeVisible;

	CDlgInfoHist m_dlgInfoHist;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRACKINGSTATUS_H__24B7F581_5528_4521_A18B_E4B30838222A__INCLUDED_)
