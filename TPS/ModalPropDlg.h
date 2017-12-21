#if !defined(AFX_MODALPROPDLG_H__9B92D7E1_FB67_49AF_8963_1FAB6CA643A4__INCLUDED_)
#define AFX_MODALPROPDLG_H__9B92D7E1_FB67_49AF_8963_1FAB6CA643A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModalPropDlg.h : header file
//
#include "resource.h"
#include <XTToolkitPro.h> 		// Xtreme Toolkit support
#include "FrmMgr.h"
#include "PanaseeDialog.h"
#include "modal.h"
#include "TpsGlobalConst.h"
/////////////////////////////////////////////////////////////////////////////
// CPropDlgModal dialog

class CPropDlgModal : public CPanaseeDialog
{
// Construction
public:
	CPropDlgModal(CWnd* pParent = NULL);   // standard constructor
	CString m_modalName;
	void SetFrmMgr(CFrmMgr* pFrmMgr){m_pFrmMgr = pFrmMgr;}

// Dialog Data
	//{{AFX_DATA(CPropDlgModal)
	enum { IDD = IDD_MODAL_PROPERTY };
	CButton	m_btnClose;
	CStatic	m_placeHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlgModal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropDlgModal)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnChangeOpacity(CString sPara);
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
	void OnChangeModalColor(CString sPara);
	void OnChangeStyle(CString sPara);
	void OnChangeName(CString sPara);
	void InitialPara();
	CFrmMgr *m_pFrmMgr;
	CXTPPropertyGrid m_PropGrid;
	CModal* m_modalInfo;
	CXTPPropertyGridItem* m_pItemName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODALPROPDLG_H__9B92D7E1_FB67_49AF_8963_1FAB6CA643A4__INCLUDED_)
