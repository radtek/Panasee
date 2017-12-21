#if !defined(AFX_DLGTRACKER_H__6EC78F27_FDAE_4200_B69A_5C9752CD0447__INCLUDED_)
#define AFX_DLGTRACKER_H__6EC78F27_FDAE_4200_B69A_5C9752CD0447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTracker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTracker dialog
#include "memDC.h"

class CDlgTracker : public CDialog
{
// Construction
public:
	void SetBaseRange(double *pos,int range);
	void Enable(BOOL bEnable);
	void SetBasePos(double x,double y,double z);
	CDlgTracker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTracker)
	enum { IDD = IDD_DLG_TRACKER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTracker)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTracker)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	int m_circlePos[2];
	int m_circleRadius;
	int m_barPos[2];
	int m_barLen;
	int m_barHeight;
	int m_edge;

	int m_trackerPos[3];
	double m_basePos[3];
	int    m_baseRange;

	CMemDC m_skinDC;
	BOOL m_bEnable;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRACKER_H__6EC78F27_FDAE_4200_B69A_5C9752CD0447__INCLUDED_)
