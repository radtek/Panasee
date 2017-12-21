#if !defined(AFX_DLGFUSION_H__4804BEEE_8387_48AF_9B93_249575EF69CC__INCLUDED_)
#define AFX_DLGFUSION_H__4804BEEE_8387_48AF_9B93_249575EF69CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFusion.h : header file
//
#include "PanaseeDialog.h"
#include "DlgUSDisplay.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFusion dialog

class CDlgFusion : public CPanaseeDialog
{
// Construction
public:
	void RemoveProbe();
	void SetProbe(CProbe *pProbe);

	void SetTitle(CString sTitle);
	void SetImage(LPUS_STRUCT_IMAGE pUSImgData,CString *pAPSI,char **ppcOrientation);

	CDlgFusion(CWnd* pParent = NULL);   // standard constructor

	enum FUSION_MODE
	{
		FUSION_AXIAL,
		FUSION_SAGITAL,
		FUSION_PROBEEYE,
		FUSION_VIRTUALUS
	};
	FUSION_MODE m_fusionMode;
// Dialog Data
	//{{AFX_DATA(CDlgFusion)
	enum { IDD = IDD_DLG_FUSION };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFusion)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFusion)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LONG OnFusionWndClicked(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDlgUSDisplay m_dlgFusion;
	BOOL m_bInitialized;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFUSION_H__4804BEEE_8387_48AF_9B93_249575EF69CC__INCLUDED_)
