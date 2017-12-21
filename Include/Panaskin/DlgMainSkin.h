#if !defined(AFX_DLGMAINSKIN_H__F0431C90_6327_489D_9440_7F34118B2AD5__INCLUDED_)
#define AFX_DLGMAINSKIN_H__F0431C90_6327_489D_9440_7F34118B2AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "MemDC.h"
#include "panaGlobal.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMainSkin dialog
/*********** ningsj *********************
 说明:主控制面板皮肤
*****************************************/
class PANASKIN_API CDlgMainSkin : public CDialog
{
// Construction
public:
	void SetUIMode(int mode);
	void SetNewSeriesAlarm(BOOL bAlarm);
	CDlgMainSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	
	enum PASK_TabMode{
		PASK_Tab_Patient,
		PASK_Tab_Series,
		PASK_Tab_MriControl,
		PASK_Tab_Navigate,
		PASK_Tab_Tps
	};

	void SetTabMode(PASK_TabMode mode);
	void GetTabPosition(PASK_TabMode mode,CRect &rc);

	PASK_OPERATIONTYPE m_bOperateType;

// Dialog Data
	//{{AFX_DATA(CDlgMainSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainSkin)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMainSkin)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	PASK_TabMode m_tabMode;
	CRect m_rcPatientBtn,m_rcSeriesBtn,m_rcMriControlBtn,m_rcNavigateBtn,m_rcTpsBtn;
	CMemDC m_skinDC;

	BOOL m_bNewSeriesAlarm;

	int m_iUIMode; //1:包含MriScan 2://不包含MriScan
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINSKIN_H__F0431C90_6327_489D_9440_7F34118B2AD5__INCLUDED_)
