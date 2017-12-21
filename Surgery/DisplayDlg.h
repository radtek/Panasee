#if !defined(AFX_DISPLAYDLG_H__D3D081C8_52F4_4B9A_B49A_FA9C9DC97F46__INCLUDED_)
#define AFX_DISPLAYDLG_H__D3D081C8_52F4_4B9A_B49A_FA9C9DC97F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg dialog
//#include "scene.h"
#include "IMRIScene.h"
#include "OleDropTargetEx.h"
#include "MenuEx.h"
#include "MriImage.h"

class CDataManager;
//class CMRISeries;
class CProbe;
class CCryotherapyDlg;
class CMRIRectangle;
class CDlgDisplayControl;
class CDlgDisplayWrap;
class CVirtualTarget;
class CDlgDispCompare;

#ifndef PANA_2D_LEGEND_NUMBER
#define PANA_2D_LEGEND_NUMBER 30
#endif

class CDisplayDlg : public CDialog
{
// Construction
public:
	BOOL IsEnableDraw();
	void EnableDraw(BOOL bEnable);
	void AddVirtualTarget(CVirtualTarget *pVTarget);
	void RemoveVirtualTarget(CVirtualTarget *pVTarget);

	void UpdateAllProjectVTarget(BOOL bClean = TRUE);
	void RemoveAllProjectVTarget();

	void EnableClickTargetOnMRI(BOOL bEnable);
	void AddMesh(CMesh* pMesh);
	CDisplayDlg(CWnd* pParent = NULL);   // standard constructor
	~CDisplayDlg();

	BOOL IsRcvWnd();
	void SetRcvWnd(BOOL bRcv);
	void ResetRuler();

	//改变距离测量按钮的状态；

	//Added by Wang fengliang 070512; 
	//--begin
	//记录弹出菜单，局部
	CMenu m_popupmenu; 
    CMenu m_popupmenuTarget;
	BOOL CreatePopupMenu();//Added by Wang fengliang 070512;

	BOOL SetCheckMenu(UINT menuItemID,UINT isChecked);//Added by Wang fengliang 070512;

	//区别各个窗口，用于协调各窗口的右键菜单 
	int m_index;  //Added by wang fengliang 070515;
	static int _globalIndex;

	int m_nPointNum; //记录距离测量时，按键次数



	//--end
	CIMRIScene m_Scene;
	int  m_iMaxNum;	// Maximum display data number
	CCryotherapyDlg *m_pCryotherapyDlg;   //从此指针可以更改其它显示窗口的右键菜单的状态
	CDlgDisplayControl *m_pDisplayControl;
	CDlgDisplayWrap *m_pDispWrap;

	// For comparison window
	CDlgDispCompare* m_pDlgDispCompare;
	int DisplayMRIToCompareWindow(CString sMRIName);

	void DisplayMRIToMainDispalyWnd(CString sMRIName);
	
	CMRIRectangle *m_pCurDispMRIRect;
	BOOL m_b3DDisplay;	// Flag to identify this window is for 3D display or not
	BOOL m_bSelected;	// Flat to identify this window is selected or not

	void SelectDisplayMRISeries(BOOL bSelected);

//	void SetMaxDisplayNum(int nMaxNum);
//	void SetDisplay(int nIndex, CString sName);
//	int RemoveDisplay(CString sName);
//	void RemoveAllDisplay();

	void AddProbe(CProbe* pProbe);
	void RemoveProbe(CProbe* pProbe);

	void AddMRIRectWithLabel(CMRIRectangle *pMRIRect, BOOL bClean = TRUE, BOOL bKeepView = FALSE);
	void AddMRIRect(CMRIRectangle *pMRIRect, BOOL bClean = TRUE, BOOL bKeepView = FALSE);  // Add rectangles to scene
	void ADDMRIRectAPSI(CMRIRectangle *pMRIRect, BOOL bClean = TRUE);  // Add APSI label to scene
	void AddMRIRectLegend(CMRIImage *pImage,ST_TEXT *pTextLst =NULL,CRect *pRect =NULL,int *pLegendNum=NULL,CFont *pFont=NULL);					// Add legend
	
	void RemoveMRIRect(CMRIRectangle *pMRIRect);
	void RemoveAllText();


	void EnableLRArrow(BOOL bEnable);
	void EnableDistanceMeasure(BOOL bEnable);
	void Enable3DDistanceMeasure(BOOL bEnable);

	void ResetView();
	void ShowLegend(BOOL bShow);

	HCURSOR m_hCursor;

//	void TransformAPSI4(double** ppdtran);
	
// Dialog Data
	//{{AFX_DATA(CDisplayDlg)
	enum { IDD = IDD_DLG_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSpopupClearDist();
	afx_msg void OnUpdateSpopupClearDist(CCmdUI* pCmdUI);
	afx_msg void OnSpopupUpdate();
	afx_msg void OnSpopupResetEye();
	afx_msg void OnViewOne3d();
	afx_msg void OnViewFrameFour2d();
	afx_msg void OnSetupLegend();
	afx_msg void OnViewThree2dOne3dLeftTop();
	afx_msg void OnViewThree2dOne3dTop();
	afx_msg void OnPopupShowimglegend();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPopupSelect();
	afx_msg void OnWindowLevel_PF();
	afx_msg void OnWindowLevel_BRAIN();
	afx_msg void OnWindowLevel_IAC();
	afx_msg void OnWindowLevel_BONE();
	afx_msg void OnWindowLevel_LUNG();
	afx_msg void OnWindowLevel_ABDOMEN();
	afx_msg void OnWindowLevel_LIVERA();
	afx_msg void OnWindowLevel_LIVERB();
	afx_msg void OnWindowLevel_SPINE();
	afx_msg void OnWindowLevel_FAT();
	afx_msg void OnResetWindowLevel();
	afx_msg void OnPopupTargetSel();
	afx_msg void OnPopupTargetDel();
	afx_msg void OnDebugViewToProbe();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSpopupColorImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	COleDropTargetEx m_dropEx;
//处理OnDropEx消息是必须的，否则OnDrop函数不会被执行
//当然也可以在OnDropEx函数中处理数据
	virtual BOOL OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	virtual DROPEFFECT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	virtual DROPEFFECT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);

private:
	
	CString GetLegend(CMRIImage *pImage,ENUM_IMG_SOURCE legendType,int index);
	bool AddProjectVirtualTarget(CMRIRectangle *pMRIRect, CVirtualTarget *pVTarget);
	bool AddProjectVirtualTarget(CVirtualTarget *pVTarget);
	void GetIndex(CString sIndex, CList<int,int> *m_pList);

	BOOL m_bEnableDraw; /*表明当前是否可刷新屏幕*/
	BOOL m_bWinWidthLevelUpdate; /*表明是否需要更新序列中所有图像的窗宽窗位*/
	void OnKeyUp(UINT nChar);
	void OnKeyDown(UINT nChar);

	BOOL m_bEnableClickTargetOnMRI;
	CPoint m_ptStart, m_ptWindowLevelStart;

	int m_iTransRotateFlag;
	BOOL m_bEnableMouseMove;
	CList<CString, CString> m_lstImgNames;
	ST_TEXT m_stTextAPSI4[4];
	ST_TEXT m_stTextLegend[PANA_2D_LEGEND_NUMBER];
	int     m_iTextLegendNum[4]; /*表示4个角具有的标签个数*/
	ST_TEXT m_stRT;
	ST_TEXT m_stSel;
	int m_nNumLegendLine;
	ENUM_IMG_SOURCE m_legendType;
	BOOL m_bShowLegend; // flat to show image legend or not

	CPoint m_iLRPoint;
	BOOL m_bLRArrow;
	BOOL m_bEnalbeDistMeasure;
	BOOL m_bEnable3DDistMeasure;

	CVirtualTarget* m_pRightClickTarget;

	CList<CVirtualTarget*, CVirtualTarget*> m_lstPrjVTargets;

	BOOL GetIntersectPosWithVisibleMRIIn3D(Line3D *pLine, Vector3D& vIntersectPos);
	BOOL GetIntersectPosWithCurMRI(Line3D *pLine, Vector3D& vIntersectPos);

	BOOL IntersectMRIWithLine(Line3D *pLine, CMRIRectangle *pMRIRect, Vector3D& vInterSectPos);

	BOOL m_bUpdate;		// Flag to update this window when new MRI series sent from MRI scanner

	BOOL m_bMoveImageWhenMouseMove;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYDLG_H__D3D081C8_52F4_4B9A_B49A_FA9C9DC97F46__INCLUDED_)


