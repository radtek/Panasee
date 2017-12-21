#if !defined(AFX_DLGMRI2DDISPLAY_H__7CF8B2A1_865D_47A8_9D8D_BE8A0AAE26B3__INCLUDED_)
#define AFX_DLGMRI2DDISPLAY_H__7CF8B2A1_865D_47A8_9D8D_BE8A0AAE26B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMRI2DDisplay.h : header file
//
#include <afxTempl.h>
#include "MRIImage.h"
#include "DisplayData.h"
#include "MriCalibration.h"
#include "MemDC.h"
#include "ModelObject.h"

class CCryotherapyDlg;
class CDlgMRIBrowse;

#ifndef PANA_2D_LEGEND_NUMBER
#define PANA_2D_LEGEND_NUMBER 30
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgMRI2DDisplay dialog

class CDlgMRI2DDisplay : public CDialog
{
// Construction
public:
	CDlgMRI2DDisplay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMRI2DDisplay();

	CGenerateBmpOfImage *m_pGenerateBmpOfImage;
	CDisplayBmpOfImage *m_pDisplayBmpOfImage;
	
	CList<POINT,POINT>	m_lpCaliCircle;		// Circle used in base calibration		
	
	CCryotherapyDlg *m_pCryDlg;
	CDlgMRIBrowse *m_pDlgBrowse;

	void SetCaliResult(CList<POINT, POINT>* lstCirclePnts, CALI_ERR_RESULT& CaliResult );

	void LoadMRIImage();
	void LoadMRIImage(CMRIImage* pMriImage);
	void LoadMRIImage(CMRISeries* pMRISeries, int iImgIndex,BOOL bUpdate=TRUE);

		
	// Dialog Data
	//{{AFX_DATA(CDlgMRI2DDisplay)
	enum { IDD = IDD_DLG_MRI_2D_DISPLAY };
	BOOL	m_bPolyline;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMRI2DDisplay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMRI2DDisplay)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCheckPolyline();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonCut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	void OnKeyDown(UINT nChar);
	void CreateDraw(CBitmap* pBitmap);
	CMRIImage* m_pMriImage;
	CString m_sMRIOriChar[4];
	int     m_ppiMRIOriCharPos[4][2];
	int m_iImgIndex; // image index in the series
	CPoint m_ptWindowLevelStart;   // used for window width and level adjust


	BOOL GetClosestCircleCenter(CPoint point, int iCenterNum, double *pdbCenterXY, CPoint &centerPoint);

	// Calibration result for display
	CALI_ERR_RESULT m_CaliResult;
	BOOL m_bShowCaliResult; 
	ST_TEXT m_stTextLegend[PANA_2D_LEGEND_NUMBER];
	int m_iLegendNum;
	CMemDC m_skinDC;

	CList<POINT,POINT>	m_lpPolyline;		// Circle used in base calibration		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRI2DDISPLAY_H__7CF8B2A1_865D_47A8_9D8D_BE8A0AAE26B3__INCLUDED_)
