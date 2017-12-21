#if !defined(AFX_DLGTITLESKIN_H__C7CDB0D0_6065_40F1_9BF2_882967E12C5F__INCLUDED_)
#define AFX_DLGTITLESKIN_H__C7CDB0D0_6065_40F1_9BF2_882967E12C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTitleSkin.h : header file
//
#include "panaskin.h"
#include "MemDC.h"
#include "panaGlobal.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTitleSkin dialog
/*********** ningsj *********************
 ËµÃ÷:Title´°¿ÚÆ¤·ô
*****************************************/
class PANASKIN_API CDlgTitleSkin : public CDialog
{
// Construction
public:
	CDlgTitleSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	
	void SetOperation(PASK_OPERATIONTYPE iOperationType);

// Dialog Data
	//{{AFX_DATA(CDlgTitleSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTitleSkin)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTitleSkin)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateDraw();

	CMemDC m_skinDC;
	PASK_OPERATIONTYPE m_iOperationType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTITLESKIN_H__C7CDB0D0_6065_40F1_9BF2_882967E12C5F__INCLUDED_)
