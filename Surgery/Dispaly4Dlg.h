#if !defined(AFX_DISPALY4DLG_H__E5A514E5_DE0B_4A06_8731_16C952E924A8__INCLUDED_)
#define AFX_DISPALY4DLG_H__E5A514E5_DE0B_4A06_8731_16C952E924A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dispaly4Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDispaly4Dlg dialog

#include "DisplayDlg.h"
#include "CryotherapyDlg.h"
#include "DlgDisplayWrap.h"

class CProbe;
class CMRISeries;
class CMRIRectangle;

enum DISPLAY_MODE 
{
	DM_22_2D,
	DM_22_3D,
	DM_13,
	DM_1,
	DM_2,
	DM_3,
	DM_4,
	DM_5,
	DM_TPS
};

class CDispaly4Dlg : public CDialog
{
//	DECLARE_DYNCREATE(CDispaly4Dlg)
// Construction
public:
	void EnableLR(BOOL bEnable);
	void EnableDistMeasure(BOOL bEnable);
	CDispaly4Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDispaly4Dlg();

	void SetDispMode(DISPLAY_MODE dispMode);      //Error：没有实现体
	void MaxDispWindow(CDisplayDlg *pDispDlg, bool bMax);
	DISPLAY_MODE GetDispMode();

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	BOOL ShowWindow(int nCmdShow);
	void Refresh();

	void ShowControlWin(BOOL bShow);
	void Layout();

	void AddProbe(CProbe *pProbe);
	void RemoveProbe(CProbe *pProbe);
	

	
	void RemoveSeriesFrom3D(MRIRectSeries *pMRIRectSeries);

//	void DisplayMRIRect(CMRIRectangle *pMRIRect, int iWndIndex, BOOL bClean = TRUE);
	void AddTo3DDisplay(CMRIRectangle *pMRIRectangle);
	void RemoveFrom3DDisplay(CMRIRectangle *pMRIRectangle);

	void Show3DDisplay(CMRIRectangle *pMRIRectangle, BOOL bShow);

	// Add MRIRectSeries to display window, this not affect 3D display
	void AddMRIRectSeries(MRIRectSeries *pMRIRectSeries, int iWndIndex, int iInitDispImgIndex = 0);

	void SetDispWndCursor(LPCTSTR lpszCursorName);

//	void RemoveAllMRI();

	void SimulateCryotherapySphere(CProbe* pProbe, double dbRadius);
//	void SimulateIceBall(CProbe* pProbe);

	BOOL UpdatePopupMenuDispMode(UINT);// 更新所有窗口的显示方式菜单的状态
	CCryotherapyDlg* m_pCryoDlg;

	CDlgDisplayWrap* GetDispWrap(int iWndIndex);   //得到当前显示窗口
	CDlgDisplayWrap m_DlgDispWrap1;	    //3D窗口
	CDlgDisplayWrap m_DlgDispWrap2;		//4个2D显示窗口
	CDlgDisplayWrap m_DlgDispWrap3;    
	CDlgDisplayWrap m_DlgDispWrap4;
	CDlgDisplayWrap m_DlgDispWrap5; 
	

	
	int GetWndIndex(CDisplayDlg *pDispDlg);
	BOOL SetSelectedWnd(CDisplayDlg *pDispDlg);

// Dialog Data
	//{{AFX_DATA(CDispaly4Dlg)
	enum { IDD = IDD_DLG_DISPLAY4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDispaly4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDispaly4Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	DISPLAY_MODE m_dispMode;
	DISPLAY_MODE m_lastDispMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPALY4DLG_H__E5A514E5_DE0B_4A06_8731_16C952E924A8__INCLUDED_)
