#if !defined(AFX_PAGESETDISPLAY_H__220E8BBF_AE14_4531_B833_CF2DFF10DD6B__INCLUDED_)
#define AFX_PAGESETDISPLAY_H__220E8BBF_AE14_4531_B833_CF2DFF10DD6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSetDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSetDisplay dialog
class CCryotherapyDlg;

class CPageSetDisplay : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSetDisplay)

// Construction
public:
	CPageSetDisplay();
	~CPageSetDisplay();

	CCryotherapyDlg *m_pCryoDlg;

// Dialog Data
	//{{AFX_DATA(CPageSetDisplay)
	enum { IDD = IDD_DLG_SETTING_DISPLAY };
	BOOL	m_bShowControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSetDisplay)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSetDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkDispSettingShowCtl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESETDISPLAY_H__220E8BBF_AE14_4531_B833_CF2DFF10DD6B__INCLUDED_)
