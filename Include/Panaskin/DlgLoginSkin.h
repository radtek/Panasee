#if !defined(AFX_DLGLOGINSKIN_H__2E850A56_E772_4249_A046_A087C8F93607__INCLUDED_)
#define AFX_DLGLOGINSKIN_H__2E850A56_E772_4249_A046_A087C8F93607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoginSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "MemDC.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginSkin dialog
/*********** ningsj *********************
 说明:启动窗口皮肤
*****************************************/
class PANASKIN_API CDlgLoginSkin : public CDialog
{
// Construction
public:
	void CreateDraw();
	CDlgLoginSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	void SetBiopsyBtn(CImageButton *pBtn);
	void SetCryotherapyBtn(CImageButton *pBtn);
	void SetQuitBtn(CImageButton *pBtn);
	void SetReviewBtn(CImageButton *pBtn);
	void SetConfigBtn(CImageButton *pBtn);
	void SetTPSBtn(CImageButton *pBtn);

// Dialog Data
	//{{AFX_DATA(CDlgLoginSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoginSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoginSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	

	void DrawBackGround(CDC *pDC);
	enum PASK_SelectedStatus{
		PASK_Sel_Tools,
		PASK_Sel_Tps,
		PASK_Sel_PatientDB,
		PASK_Sel_Applications,
		PASK_Sel_Config,
		PASK_Sel_None
	};

	void DrawTools(CDC *pDC);
	void DrawConfig(CDC *pDC);
	void DrawPatientDB(CDC *pDC);
	void DrawApplications(CDC *pDC);
	void DrawTPS(CDC *pDC);

	CRect m_rcApplication,m_rcTools,m_rcConfig,m_rcPatientDB,m_rcTps;
	CRect m_rcApplicationSelected,m_rcToolsSelected,
		m_rcConfigSelected,m_rcPatientDBSelected,m_rcTpsSelected;
	CRect m_rcQuitBtn;
	PASK_SelectedStatus m_SelectedStatus;

	CImageButton *m_btnBiopsy,*m_btnCryotherapy;
	CImageButton *m_btnQuit,*m_btnReview,*m_btnConfig,*m_btnTPS;

	CMemDC m_skinDC;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGINSKIN_H__2E850A56_E772_4249_A046_A087C8F93607__INCLUDED_)
