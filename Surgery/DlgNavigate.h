#if !defined(AFX_DLGNAVIGATE_H__02C73B5F_CCA4_4216_8268_1CE777AD0222__INCLUDED_)
#define AFX_DLGNAVIGATE_H__02C73B5F_CCA4_4216_8268_1CE777AD0222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNavigate.h : header file
//
#include "DlgNavigateSkin.h"
#include "ImageButton.h"
#include "MRISeries.h"
#include "mv.h"
#include "probe.h"
#include "PanaStatic.h"
#include "PanaEdit.h"
#include "Header.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNavigate dialog

class CDlgNavigate : public CDlgNavigateSkin
{
// Construction
public:
	BOOL MPR(CString sSeriesName);
	BOOL IsTracking();
	CDlgNavigate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNavigate();

	void ResliceSeries();
	CProbe *m_pCurRealProbe;
	CString m_sSeriesName;
	

// Dialog Data
	//{{AFX_DATA(CDlgNavigate)
	enum { IDD = IDD_DLG_NAVIGATE };
	CButton	m_btnFreeze;
	CButton	m_staticProbe;
	CButton	m_chkShowFusion;
	CButton	m_btnSelectFusion;
	CImageButton	m_btnView4;
	CImageButton	m_btnView3;
	CImageButton	m_btnView2;
	CImageButton	m_btnView1;
	CButton	m_chkSameWL;
	CPanaStatic	m_lblLengthUnit;
	CPanaEdit	m_edtOffset;
	CImageButton	m_btnSubOffset;
	CImageButton	m_btnAddOffset;
	CImageButton	m_btnScrollOffset;
	CButton	m_btnTracking;
	CButton	m_btnSelect;
	CImageButton	m_btnCalibrateImage;
	BOOL	m_bSameWWWL;
	BOOL	m_bShowFusion;
	int		m_bCurTipType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNavigate)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNavigate)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSeries();
	afx_msg void OnCheckSameWwwl();
	afx_msg void OnButtonCtCalibrate();
	afx_msg void OnButtonTracking();
	afx_msg void OnBtnAddOffset();
	afx_msg void OnBtnSubOffset();
	afx_msg LONG OnMoveCtrl(WPARAM w,LPARAM l);
	afx_msg LONG OnMoveCtrl_LUP(WPARAM w,LPARAM l);
	afx_msg LONG OnMoveCtrl_LDOWN(WPARAM w,LPARAM l);
	afx_msg LONG OnUpdateDepth(WPARAM w,LPARAM l);
	afx_msg LONG OnUpdateAngle(WPARAM w,LPARAM l);
	afx_msg LONG OnDepthSliderShow(WPARAM w,LPARAM l);
	afx_msg LONG OnDepthSliderHide(WPARAM w,LPARAM l);
	afx_msg void OnButtonViewAv();
	afx_msg void OnButtonViewAsvp();
	afx_msg void OnButtonViewAst();
	afx_msg void OnButtonViewTapv();
	afx_msg void OnButtonSelectFusion();
	afx_msg void OnCheckShowFusion();
	afx_msg void OnButtonFreeze();
	afx_msg void OnRadioRealtip();
	afx_msg void OnRadioVirtualtip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ResliceFusion();
	//¶àÏß³ÌËø

	CString m_sDispOri[4];
	// Characters for orientation (A, P, S, I, R, L)
	char** m_ppcOrientation;	
	void ComputeOrientChar(US_STRUCT_IMAGE *pImage);
	void CorrectImageDispOrientation();

	void SetUI();
	void Transform(LPUS_STRUCT_IMAGE pImage,Matrix m);
	LPUS_STRUCT_IMAGE GetUSImage(CMRISeries *pSeries,int index);
	void ResliceProbeEyeAndVirtualUS();
	BOOL GetProbeInScanner(Matrix &mProbe2Scanner,CMRISeries *pSeries);
	void ResliceSeries2D();
	void ResliceSeries3D();
	void ResliceSeries2D_AxisRelative();
	
	BOOL m_bFreezing;
	BOOL m_bTracking;
	CString m_sFusionSeriesName;

	BOOL m_bNoRefreshInScroll;
	
	double m_dStandardProbeLength;
	int m_iPushDepth;
	int m_iVirtualUSAngle;

	int m_nOffset;
	double m_dPreImageProbeTip[3];
	BOOL m_bIsPausing;

	int m_iLookDepth;

	TRACKING_VIEW_MODE m_viewMode,m_newViewMode;

	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNAVIGATE_H__02C73B5F_CCA4_4216_8268_1CE777AD0222__INCLUDED_)
