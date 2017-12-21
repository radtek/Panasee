#if !defined(AFX_DLGTRACKING_H__E8CE51D2_C2A7_4DEF_B1F5_333E0A17C121__INCLUDED_)
#define AFX_DLGTRACKING_H__E8CE51D2_C2A7_4DEF_B1F5_333E0A17C121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTracking.h : header file
//

#include "DlgUSDisplay.h"
#include "DlgTargeting.h"
#include "header.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTracking dialog

class CDlgTracking : public CDialog
{
// Construction
public:
	void ReshapeScene();
	void ChangeView2USImage();
	
	void EnableTrackingFlag(BOOL bTracking);
	void CheckSelectedTarget();
	void SetWL(int width,int level);
	void Refresh();

	void RemoveProbe();
	void SetProbe(CProbe *pProbe);

	void AddVirtualTarget(CVirtualTarget *pVTarget);
	void RemoveVirtualTarget();
	void UpdateProjectVirtualTarget();

	void SetVirtualUS(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);
	void SetProbeEye(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);
	void SetAxial(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);
	void SetSagittal(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);
	CDlgTracking(CWnd* pParent = NULL);   // standard constructor

	
	CDlgTargeting m_dlgTargeting;
	CDlgUSDisplay m_dlgVirtualUS;

	void Layout();
	void SetViewMode(TRACKING_VIEW_MODE mode);
	

// Dialog Data
	//{{AFX_DATA(CDlgTracking)
	enum { IDD = IDD_DLG_TRACKING_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTracking)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTracking)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT OnUpdateWL(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateTargetIndex(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	CDlgUSDisplay m_dlgProbeEye;
	
	CDlgUSDisplay m_dlgAxial;
	CDlgUSDisplay m_dlgSagittal;

	BOOL m_bTrackingFlag;

	TRACKING_VIEW_MODE m_viewMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRACKING_H__E8CE51D2_C2A7_4DEF_B1F5_333E0A17C121__INCLUDED_)
