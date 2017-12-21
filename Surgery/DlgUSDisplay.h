#if !defined(AFX_DLGUSDISPLAY_H__098965E4_60A5_4578_B74B_43FF9B62D44B__INCLUDED_)
#define AFX_DLGUSDISPLAY_H__098965E4_60A5_4578_B74B_43FF9B62D44B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUSDisplay.h : header file
//
#include "VirtualTarget.h"
#include "IMRIScene.h"
#include "USRectangle.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgUSDisplay dialog

class CDlgUSDisplay : public CDialog
{
// Construction
public:
	BOOL IsSliding(); //滑块是否在滑动
	void SetTitle(CString text);
	void ReshapeScene();
	void EnableSlider(CWnd *pWnd,int mode);
	void EnableTrackingFlag(BOOL bTracking);
	void CheckSelectedTarget();
	void SetSelectedTarget(int iSelected);
	void SetWL(int width, int level);
	void SetUSImage(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);
	void ChangeView2USImage();
	void Redraw();
	void RemoveProbe();
	void SetProbe(CProbe *pProbe);

	void UpdateProjectVirtualTarget();
	void AddVirtualTarget(CVirtualTarget *pVTarget);
	void RemoveVirtualTarget();

	//是否显示针和图像的交点但不显示针
	BOOL m_bUseCrossReplaceProbe;

	
	CDlgUSDisplay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUSDisplay();

// Dialog Data
	//{{AFX_DATA(CDlgUSDisplay)
	enum { IDD = IDD_DLG_USDISPLAY };
	CSliderCtrl	m_depthSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUSDisplay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUSDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnReleasedcaptureSliderDepth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderDepth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	ST_TEXT m_stTextAPSI4[4];
	// Positions of the orientation characters
	double** m_ppdOrientCharPos;
	void ComputeOrientCharPosition(US_STRUCT_IMAGE *pImage);
	
	CPoint m_ptWindowLevelStart;
	BOOL m_bEnableMouseMove;
	BOOL m_bWinWidthLevelUpdate;


	CWnd *m_pHandleDepthEventWnd;
	ST_TEXT m_txtWL,m_txtDepth,m_txtTitle;
	BOOL IntersectMRIWithLine(Line3D *pLine, CRectangle *pMRIRect, Vector3D& vInterSectPos);
	CIMRIScene m_Scene;
	CUSRectangle m_usRect;
	CProbe *m_pProbe;

	CList<CVirtualTarget*, CVirtualTarget*> m_lstPrjVTargets;
	CList<CVirtualTarget*, CVirtualTarget*> m_lstTarget;
	int m_imageWidth,m_imageLevel;

	int m_iSelectedTargetIndex;

	BOOL m_bTrackingFlag;

	int m_iSlideMode; //0:depth 1:Angle
	BOOL m_bIsSliding;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSDISPLAY_H__098965E4_60A5_4578_B74B_43FF9B62D44B__INCLUDED_)
