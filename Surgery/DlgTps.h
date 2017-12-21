#if !defined(AFX_DLGTPS_H__3E74F55B_26B3_4DEF_99F6_2249818C0086__INCLUDED_)
#define AFX_DLGTPS_H__3E74F55B_26B3_4DEF_99F6_2249818C0086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTps.h : header file
//
#include "DlgTpsDisp.h"
#include "DlgTpsCtrl.h"
#include "TPSFrmMgr.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTps dialog

class CDlgTps : public CDialog
{
// Construction
public:
	void GetProbeInfo(int &count, double *pProbeData);
	void UpdateProbeInfo(double *tipPos, double *ori);
	void ChangeDisplaySize(int left,int top,int width,int height);
	void ShowCtrl(BOOL bShow);
	void ShowDisp(BOOL bShow);
	CDlgTps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTps)
	enum { IDD = IDD_DLG_TPS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTps)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDlgTpsCtrl m_ctrl;
	CDlgTpsDisp m_disp;

	CTPSFrmMgr *m_pMgr;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTPS_H__3E74F55B_26B3_4DEF_99F6_2249818C0086__INCLUDED_)
