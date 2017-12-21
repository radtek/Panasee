#if !defined(AFX_DLGMRISCANOPTIONS_H__2AAAEB56_FB79_483F_B048_683BB0B0A41F__INCLUDED_)
#define AFX_DLGMRISCANOPTIONS_H__2AAAEB56_FB79_483F_B048_683BB0B0A41F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMriScanOptions.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgMriScanOptions dialog
struct MRIScanOptions
{
	
	BOOL	bAdvanced;
	BOOL	bCycle;
	UINT	iInterval;
	
	float	fGap;
	UINT	iNex;
	UINT	iSlice;
	float	fThickness;
	
	int		iOrient;  //0 cor ,1 sag ,2 axi ,3 probe
	int		iReferPos; //0 probe tip, 1 mri center

	BOOL    bSwap;

	BOOL    bNewOrient;
};


class CDlgMriScanOptions : public CDialog
{
// Construction
public:
	void Refresh(int iMode);
	CDlgMriScanOptions(CWnd* pParent = NULL);   // standard constructor

	MRIScanOptions *m_pScanOption;
	CWnd *m_pDlgScannerControl;

// Dialog Data
	//{{AFX_DATA(CDlgMriScanOptions)
	enum { IDD = IDD_DLG_SCANNER_CONTROL_OPTION };
	BOOL	m_bCycle;
	float	m_fGap;
	UINT	m_iInterval;
	UINT	m_iNex;
	UINT	m_iSlice;
	float	m_fThickness;
	int		m_iOrient;
	int		m_iReferPos;
	BOOL	m_bNew;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMriScanOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMriScanOptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnMrioptionRadioSag();
	afx_msg void OnMrioptionRadioProbe();
	afx_msg void OnMrioptionRadioCor();
	afx_msg void OnMrioptionRadioAxi();
	afx_msg void OnMrioptionCheckCycleScan();
	afx_msg void OnClose();
	afx_msg void OnButtonClose();
	afx_msg void OnPaint();
	afx_msg void OnMrioptionRadioMriCenter();
	afx_msg void OnMrioptionRadioProbeCenter();
	afx_msg void OnMrioptionCheckNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRISCANOPTIONS_H__2AAAEB56_FB79_483F_B048_683BB0B0A41F__INCLUDED_)
