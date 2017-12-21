#if !defined(AFX_DLGWIZARDSKIN_H__CCC53DD7_5342_4135_9CD7_6B1FDCC427F9__INCLUDED_)
#define AFX_DLGWIZARDSKIN_H__CCC53DD7_5342_4135_9CD7_6B1FDCC427F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWizardSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "MemDC.h"
#include "panaGlobal.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgWizardSkin dialog
/*********** ningsj *********************
 说明:登录步骤窗口皮肤
*****************************************/
class PANASKIN_API CDlgWizardSkin : public CDialog
{
// Construction
public:
	void SetUIMode(int mode);
	void SetStatusMsg(CString sMsg);
	CDlgWizardSkin(UINT IDD, CWnd* pParent = NULL);   // standard constructor

	void SetQuitBtn(CImageButton *pBtn);
	void SetPreBtn(CImageButton *pBtn);

	enum PASK_WIZARD_STEP {
		PASK_WSTEP_CAM = 1,
		PASK_WSTEP_MRI,
		PASK_WSTEP_PAT,
		PASK_WSTEP_BAS,
		PASK_WSTEP_CAL,
		PASK_WSTEP_END
	};

	enum PASK_WIZARD_STEP_RESULT {
		PASK_WSRET_OK = 0,
		PASK_WSRET_ERR,
		PASK_WSRET_RUNNING,
		PASK_WSRET_WAIT
	};

	void SetWizardStatus(PASK_WIZARD_STEP iStep,PASK_WIZARD_STEP_RESULT iResult);

	

	void SetOperation(PASK_OPERATIONTYPE iOperationType);
// Dialog Data
	//{{AFX_DATA(CDlgWizardSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardSkin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DrawWizardStatus(CDC *pDC,CRect rc,CString sText,PASK_WIZARD_STEP_RESULT status);
	
	void CreateDraw();
	CImageButton *m_btnPre;
	CImageButton *m_btnQuit;
	CMemDC m_skinDC;

	PASK_WIZARD_STEP_RESULT m_iResult[6];
	CRect m_rcUpdate;
	CString m_sStatus;
	PASK_OPERATIONTYPE m_iOperationType;

	BOOL m_iUIMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWIZARDSKIN_H__CCC53DD7_5342_4135_9CD7_6B1FDCC427F9__INCLUDED_)
