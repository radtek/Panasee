#if !defined(AFX_DLGDISPLAYCONTROL_H__9C38B7E1_EE2D_4C3E_A482_AFC55A2765F3__INCLUDED_)
#define AFX_DLGDISPLAYCONTROL_H__9C38B7E1_EE2D_4C3E_A482_AFC55A2765F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDisplayControl.h : header file
//
//#include "XPStyleButtonST.h"
#include "DlgImageControlSkin.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayControl dialog
class CDisplayDlg;
class MRIRectSeries;
class CDlgDisplayWrap;


class CDlgDisplayControl : public CDlgImageControlSkin
{
// Construction
public:
	CDlgDisplayControl(CWnd* pParent = NULL);   // standard constructor
	~CDlgDisplayControl();
	CDisplayDlg *m_pDlgDisplay;
	CDlgDisplayWrap *m_pDisplayWrap;
	MRIRectSeries *m_pMRIRectSeries;
	
	int m_iCurDispIndex;

	void SetMRIRectSeries(MRIRectSeries* pMRIRectSeries, int iInitDispImgIndex = 0);
	void RemoveCurrentDispImg();
	void ShowImage(int iImgIndex, BOOL bKeepView = TRUE);
	void UpdateUI();

	void AddCurrentTo3D();
	void HideCurrentIn3D();
	
	void OnKeyUp(UINT nChar);
	void OnKeyDown(UINT nChar);

	void ForwardImage();
	void BackwardImage();

//	void SetButtonColor(CButtonST &btn);

// Dialog Data
	//{{AFX_DATA(CDlgDisplayControl)
	enum { IDD = IDD_DLG_DISPLAY_CONTROL };
	CImageButton	m_BtnChkShowMax;
	CImageButton	m_Btn3D;
	CImageButton	m_BtnChk3D;
	CImageButton	m_BtnPrev;
	CImageButton	m_BtnNext;
	CImageButton	m_BtnFirst;
	CImageButton	m_BtnLast;
	CImageButton	m_BtnShowMax;
	BOOL	m_bChk3D;
	BOOL	m_bMaxWin;
	BOOL	m_bSeriesDisplay;
	//}}AFX_DATA

//	CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDisplayControl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDlgDisplayControl)
	afx_msg void OnChk3d();
	afx_msg void OnChkShowMax();
	afx_msg void OnShowFirst();
	afx_msg void OnShowPre();
	afx_msg void OnShowNext();
	afx_msg void OnShowLast();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnChk3d();
	afx_msg void OnBtnChkFull();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetSliderRange(int iMin, int iMax, int iCurPos = 0);
	void AddToDisplay(int iImgIndex, BOOL bKeepView);
	
	
	void HideImageIn3D(int iImageIndex);
	
	int m_iSliderImgMin, m_iSliderImgMax;
	int m_iResolutionX, m_iResolutionY;   // Resolutino of current displayed image

	void Update3DChkBtn(BOOL bCheck);

	/*切换图像时，立即更新3D屏幕的图像
	  使用键盘快速切换图像时,不更新3D屏幕
	  可以提高图像刷新速度*/
	BOOL m_bAddTo3D; 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISPLAYCONTROL_H__9C38B7E1_EE2D_4C3E_A482_AFC55A2765F3__INCLUDED_)
