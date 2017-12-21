#if !defined(AFX_COMSETPAGE_H__CC1FAF57_322E_49F6_9E4B_502C35BBBA4A__INCLUDED_)
#define AFX_COMSETPAGE_H__CC1FAF57_322E_49F6_9E4B_502C35BBBA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComSetPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSetPage dialog

class CComSetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CComSetPage)

// Construction
public:
	CComSetPage();
	~CComSetPage();

	void SaveChange();

// Dialog Data
	//{{AFX_DATA(CComSetPage)
	enum { IDD = IDD_COM_PORT_SETTINGS };
	CComboBox	m_ctlStopBits;
	CComboBox	m_ctlParity;
	CComboBox	m_ctlDataBits;
	CComboBox	m_ctlCOMPort;
	CComboBox	m_ctlBaudRate;
	BOOL	m_bHardwareHandShake;
	BOOL	m_bResetHardware;
	BOOL	m_bWireless;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CComSetPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CComSetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBaudRate();
	afx_msg void OnSelchangeComPorts();
	afx_msg void OnSelchangeDataBits();
	afx_msg void OnSelchangeParity();
	afx_msg void OnSelchangeStopBits();
	afx_msg void OnWireless();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iStopBitsSel;
	int m_iParitySel;
	int m_iDataBitsSel;
	int m_iCOMPortSel;
	int m_iBaudRateSel;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSETPAGE_H__CC1FAF57_322E_49F6_9E4B_502C35BBBA4A__INCLUDED_)
