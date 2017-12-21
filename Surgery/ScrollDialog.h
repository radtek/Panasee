// ScrollDialogDlg.h : header file
//

#if !defined(AFX_SCROLLDIALOGDLG_H__1D5C554C_9E3E_425C_8923_AF5FDAC49F03__INCLUDED_)
#define AFX_SCROLLDIALOGDLG_H__1D5C554C_9E3E_425C_8923_AF5FDAC49F03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CScrollDialog dialog

class CScrollDialog : public CDialog
{
// Construction
public:
	void SetScrollSize(CSize size);
	CScrollDialog(UINT IDD,CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CScrollDialog)
	//enum { IDD = IDD_SCROLLDIALOG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bInsideUpdate;
	CSize m_totalDev;
	CSize m_lineDev;
	BOOL m_bScrollMode; //FALSE:根据对话框资源的大小进行滚动 TRUE：根据指定大小滚动

	void UpdateBars();
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	void GetScrollBarSizes(CSize& sizeSb);
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	CPoint GetDeviceScrollPosition() const;
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
	void ScrollToDevicePosition(POINT ptDev);
	void ScrollPage(int xAmount, int yAmount);
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

	// Generated message map functions
	//{{AFX_MSG(CScrollDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLDIALOGDLG_H__1D5C554C_9E3E_425C_8923_AF5FDAC49F03__INCLUDED_)
