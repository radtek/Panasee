#if !defined(AFX_PROBECONTROLDLG_H__4594F3F7_2287_4DC1_9042_11B628595ED5__INCLUDED_)
#define AFX_PROBECONTROLDLG_H__4594F3F7_2287_4DC1_9042_11B628595ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeControlDlg.h : header file
//
#include "resource.h"
#include "BtnST.h"
#include "PanaseeDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CProbeControlDlg dialog

class CProbeControlDlg : public CDialog
{
// Construction
public:
	CProbeControlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProbeControlDlg)
	enum { IDD = IDD_DIALOG_PROBE_CONTROL };
	int		m_iMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbeControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProbeControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRadioTip();
	afx_msg void OnRadioNone();
	afx_msg void OnRadioEnd();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bChangeToolByMouse;
	CPoint m_pInitialPos;
			
	int m_bOperation;  //0:position 1:orientation
	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBECONTROLDLG_H__4594F3F7_2287_4DC1_9042_11B628595ED5__INCLUDED_)
