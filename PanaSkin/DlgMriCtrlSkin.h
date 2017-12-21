#if !defined(AFX_DLGMRICTRLSKIN_H__ED479F84_B00E_4028_857B_E7B49800F721__INCLUDED_)
#define AFX_DLGMRICTRLSKIN_H__ED479F84_B00E_4028_857B_E7B49800F721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMriCtrlSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "PanaStatic.h"
#include "PanaEdit.h"
#include "memDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMriCtrlSkin dialog

/*********** ningsj *********************
 ËµÃ÷:Mri Control Panel´°¿ÚÆ¤·ô
*****************************************/
class PANASKIN_API CDlgMriCtrlSkin : public CDialog
{

// Construction
public:
	CDlgMriCtrlSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	int GetPushDepth();
	void SetPushDepth(int dDepth);
	void SetPushDepthRange(int iMin,int iMax);

	void SetNewProbeBtn(CImageButton *pBtn);
	void SetSimProbeBtn(CImageButton *pBtn);
	void SetDelProbeBtn(CImageButton *pBtn);
	void SetSelectProbeBtn(CImageButton *pBtn);
	void SetScrollBtn(CImageButton *pBtn);
	void SetAddBtn(CImageButton *pBtn);
	void SetDelBtn(CImageButton *pBtn);
	void SetProbeDepthEdit(CPanaEdit *pEdit);
	void SetProbeDepthUnitStatic(CPanaStatic *pStatic);

	void SetProgressCtrl(CProgressCtrl *pCtrl);

	void SetOrient1Btn(CImageButton *pBtn);
	void SetOrient2Btn(CImageButton *pBtn);
	void SetOrient3Btn(CImageButton *pBtn);
	void SetPosition1Btn(CImageButton *pBtn);
	void SetPosition2Btn(CImageButton *pBtn);
	void SetSwapOffBtn(CImageButton *pBtn);
	void SetSwapOnBtn(CImageButton *pBtn);
	void SetOrientStatic(CPanaStatic *pStatic);
	void SetPositionStatic(CPanaStatic *pStatic);
	void SetSwapStatic(CPanaStatic *pStatic);
	void SetProtocalStatic(CPanaStatic *pStatic);
	void SetProtocalBtn(CImageButton *pBtn);

	void SetLoadBtn(CImageButton *pBtn);
	void SetPrescanBtn(CImageButton *pBtn);
	void SetScanBtn(CImageButton *pBtn);
	void SetPrescanScanBtn(CImageButton *pBtn);
	void SetStopBtn(CImageButton *pBtn);
	void SetAdvanceBtn(CImageButton *pBtn);
	void SetCycleBtn(CImageButton *pBtn);
	void SetCycleTimeEdit(CPanaEdit *pEdit);
	void SetCycleTimeUnitStatic(CPanaStatic *pStatic);

	
	enum PASK_UI_PRESCANSCAN_BTN_STATUS {
		PASK_PSBT_SCAN,
		PASK_PSBT_PAUSE,
		PASK_PSBT_RESUME,
	};

	void SetPrescanScanBtnStatus(PASK_UI_PRESCANSCAN_BTN_STATUS status);

	void SetCycleEditVisible(BOOL bVisable);
	void SetCycleEditEnable(BOOL bEnable);
	void SetPushDepthEditEnable(BOOL bEnable);
	void SetProtocalTimeStatic(CPanaStatic *pStatic);

// Dialog Data
	//{{AFX_DATA(CDlgMriCtrlSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMriCtrlSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMriCtrlSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg LONG OnScrollPushDepth(WPARAM w,LPARAM l);
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	void CreateDraw();
	

	CImageButton *m_btnNewProbe;
	CImageButton *m_btnSimProbe;
	CImageButton *m_btnDelProbe;
	CImageButton *m_btnSelectProbe;
	CImageButton *m_btnAdd,*m_btnDel,*m_btnScroll;
	CPanaEdit *m_editProbeDepth;
	CPanaStatic *m_sProbeDepthUnit;

	CPanaStatic *m_sProtocalTime;

	CRect m_rcPushDepthScroll;
	int m_iMinPushDepth,m_iMaxPushDepth,m_iCurrentPushDepth;

	

	CPanaStatic *m_sPosition,*m_sOrient,*m_sSwap;
	CImageButton *m_btnOrient1,*m_btnOrient2,*m_btnOrient3,
		*m_btnPosition1,*m_btnPosition2,*m_btnSwap1,*m_btnSwap2;

	CPanaStatic* m_sProtocal;
	CImageButton* m_btnProtocal;
	CProgressCtrl *m_progressCtrl;

	CImageButton *m_btnLoad,*m_btnPrescan,*m_btnScan,*m_btnPrescanScan,
		*m_btnStop,*m_btnAdvance,*m_btnCycle;
	CPanaEdit *m_editCycleTime;
	CPanaStatic *m_sCycleTimeUnit;

	CMemDC m_skinDC;
	PASK_UI_PRESCANSCAN_BTN_STATUS m_prescanScanBtnStatus;

	BOOL m_bCycleEditVisable;
	BOOL m_bCycleEditEnable;
	BOOL m_bPushDepthEditEnable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRICTRLSKIN_H__ED479F84_B00E_4028_857B_E7B49800F721__INCLUDED_)
