#if !defined(AFX_PROBEPROPDLG_H__C710CBDD_7624_4514_9585_2D2F66417E7E__INCLUDED_)
#define AFX_PROBEPROPDLG_H__C710CBDD_7624_4514_9585_2D2F66417E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbePropDlg.h : header file
//
#include "resource.h"
#include <XTToolkitPro.h> 		// Xtreme Toolkit support
#include "FrmMgr.h"
#include "PanaseeDialog.h"

#include "TPSGlobalConst.h"
/////////////////////////////////////////////////////////////////////////////
// CPropDlgProbe dialog

class CPropDlgProbe : public CPanaseeDialog
{
// Construction
public:
	CPropDlgProbe(CWnd* pParent = NULL);   // standard constructor
	CString m_probeName;
	void SetFrmMgr(CFrmMgr* pFrmMgr){m_pFrmMgr = pFrmMgr;}
	

// Dialog Data
	//{{AFX_DATA(CPropDlgProbe)
	enum { IDD = IDD_PROBE_PROPERTY };
	CButton	m_btnClose;
	CStatic	m_placeHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlgProbe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropDlgProbe)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnChangeBallShellColor(CString sPara);
	void OnChangeBallShellOpacity(CString sPara);
	COLORREF StringToRGB(CString str)
	{	
		TCHAR *stopstring;
		int nValue = _tcstoul((LPCTSTR)str, &stopstring, 16);
		return RGB(GetBValue(nValue), GetGValue(nValue), GetRValue(nValue));
	}
	
	CString RGBToString(COLORREF clr)
	{
		CString str;
		str.Format(_T("%2X%2X%2X"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
#if _MSC_VER < 1200 // MFC 5.0
		for (int i = 0; i < str.GetLength(); i++) if (str[i] == _T(' ')) str.SetAt(i, _T('0'));
#else
		str.Replace(_T(" "), _T("0"));
#endif
		return str;
	}
	void OnChangeBallStyle(CString sPara);
	void ChangeBallStyle(CString sPara);
	void OnChangeBallShellVisible(CString sPara);
	void OnChangeName(CString para);
	void OnChangeBallVisible(CString para);
	CFrmMgr *m_pFrmMgr;
	void InitialPara();
	void OnChangeStyle(CString para);
	CXTPPropertyGrid m_PropGrid;
	CXTPPropertyGridItem* pSettings;
	CXTPPropertyGridItem *m_pItemType;
	CXTPPropertyGridItem *m_pItemName,*m_pItemStyle,*m_pItemLength,*m_pItemDiameter;
	CXTPPropertyGridItem *m_pItemIceBall,*m_pItemBallShell1,*m_pItemBallShell2,*m_pItemBallShell3;
	CXTPPropertyGridItem *m_pBS1Visible,*m_pBS2Visible,*m_pBS3Visible;
	CXTPPropertyGridItem *m_pBS1Opacity,*m_pBS2Opacity,*m_pBS3Opacity;
	CXTPPropertyGridItem *m_pBS1Color,*m_pBS2Color,*m_pBS3Color;
	BOOL m_bInitial;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEPROPDLG_H__C710CBDD_7624_4514_9585_2D2F66417E7E__INCLUDED_)
