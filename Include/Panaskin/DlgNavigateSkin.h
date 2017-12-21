#if !defined(AFX_DLGNAVIGATESKIN_H__49B06A25_A730_4469_BBB9_7138EB702FAF__INCLUDED_)
#define AFX_DLGNAVIGATESKIN_H__49B06A25_A730_4469_BBB9_7138EB702FAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNavigateSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "PanaStatic.h"
#include "PanaEdit.h"
#include "MemDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNavigateSkin dialog

class PANASKIN_API CDlgNavigateSkin : public CDialog
{
// Construction
public:
	void EnableProbeTool(BOOL bEnable);
	CDlgNavigateSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	int GetPushDepth();
	void SetPushDepth(int dDepth);
	void SetPushDepthRange(int iMin,int iMax);


	void SetScrollBtn(CImageButton *pBtn);
	void SetAddBtn(CImageButton *pBtn);
	void SetDelBtn(CImageButton *pBtn);
	void SetProbeDepthEdit(CPanaEdit *pEdit);
	void SetProbeDepthUnitStatic(CPanaStatic *pStatic);

	void SetView1(CImageButton *pBtn);
	void SetView2(CImageButton *pBtn);
	void SetView3(CImageButton *pBtn);
	void SetView4(CImageButton *pBtn);

// Dialog Data
	//{{AFX_DATA(CDlgNavigateSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNavigateSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNavigateSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LONG OnScrollPushDepth(WPARAM w,LPARAM l);
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	CMemDC m_skinDC;

	CImageButton	*m_btnView4;
	CImageButton	*m_btnView3;
	CImageButton	*m_btnView2;
	CImageButton	*m_btnView1;
	CImageButton *m_btnAdd,*m_btnDel,*m_btnScroll;
	CPanaEdit *m_editProbeDepth;
	CPanaStatic *m_sProbeDepthUnit;

	CRect m_rcPushDepthScroll;
	int m_iMinPushDepth,m_iMaxPushDepth,m_iCurrentPushDepth;

	BOOL m_bEnableProbeTool;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNAVIGATESKIN_H__49B06A25_A730_4469_BBB9_7138EB702FAF__INCLUDED_)
