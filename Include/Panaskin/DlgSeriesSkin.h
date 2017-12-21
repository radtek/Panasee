#if !defined(AFX_DLGSERIESSKIN_H__AB495667_CB56_457F_BD84_6BAA2720841D__INCLUDED_)
#define AFX_DLGSERIESSKIN_H__AB495667_CB56_457F_BD84_6BAA2720841D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSeriesSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "MemDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSeriesSkin dialog

class PANASKIN_API CDlgSeriesSkin : public CDialog
{
// Construction
public:
	void SetFusionBtn(CImageButton *pBtn);
	void SetDicomBtn(CImageButton *pBtn);
	CDlgSeriesSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	void SetSeriesListCtrl(CListCtrl *pListCtrl);

// Dialog Data
	//{{AFX_DATA(CDlgSeriesSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSeriesSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSeriesSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateDraw();
	CMemDC m_skinDC;

	CListCtrl *m_pSeriesList;
	CImageButton *m_btnDicom,*m_btnFusion;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIESSKIN_H__AB495667_CB56_457F_BD84_6BAA2720841D__INCLUDED_)
