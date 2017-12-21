#if !defined(AFX_PANASEEDIALOG_H__802C80CF_7A47_4DF8_B1B1_FA76A97D4A5C__INCLUDED_)
#define AFX_PANASEEDIALOG_H__802C80CF_7A47_4DF8_B1B1_FA76A97D4A5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanaseeDialog.h : header file
//
#include "panaskin.h"
#include "MemDC.h"
#include <afxtempl.h>
/*********** ningsj *********************
 说明:生成panasee风格的窗口
      RGB(255,0,0)为默认透明色
*****************************************/

/////////////////////////////////////////////////////////////////////////////
// CPanaseeDialog dialog

class PANASKIN_API CPanaseeDialog : public CDialog
{
// Construction
public:
	void EnableSeprateLine(BOOL bEnable);
	/*********** ningsj *********************
	说明:表示窗口是否包含标题栏以及文字
	*****************************************/
	void SetTitleBarVisable(BOOL bVisable);
	void SetBottomBtnList(CList<CButton *,CButton *> *btnList,BOOL bCenter=FALSE);

	CPanaseeDialog(UINT IDD,CWnd* pParent = NULL);   // standard constructor
	virtual ~CPanaseeDialog();
	
// Dialog Data
	//{{AFX_DATA(CPanaseeDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanaseeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanaseeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnNcActivate( BOOL bActive );
	afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Layout();
	void CalcFrameRegion();

	HBRUSH m_hbrCtrlColor;
	CRect m_marginLeftTop,m_marginRightTop,m_marginLeftBottom,m_marginRightBottom,
		m_marginCenterTop;

	/*********** ningsj *********************
	说明:表示窗口是否包含标题栏以及文字
	*****************************************/
	BOOL m_bTitleBarVisable;
	CList<CButton *,CButton *> m_btnList;
	BOOL m_bSeprateLine;
	BOOL m_bCenterBtn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANASEEDIALOG_H__802C80CF_7A47_4DF8_B1B1_FA76A97D4A5C__INCLUDED_)
