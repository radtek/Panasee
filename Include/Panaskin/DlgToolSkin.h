#if !defined(AFX_DLGTOOLSKIN_H__1690FC0B_D5D2_44CF_877C_C4F8E77804DA__INCLUDED_)
#define AFX_DLGTOOLSKIN_H__1690FC0B_D5D2_44CF_877C_C4F8E77804DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgToolSkin.h : header file
//
#include "panaskin.h"
#include "ImageButton.h"
#include "MemDC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTitleSkin dialog
/*********** ningsj *********************
 ËµÃ÷:ToolBar´°¿ÚÆ¤·ô
*****************************************/
/////////////////////////////////////////////////////////////////////////////
// CDlgToolSkin dialog

class PANASKIN_API CDlgToolSkin : public CDialog
{
// Construction
public:
	

	CDlgToolSkin(UINT IDD,CWnd* pParent = NULL);   // standard constructor
	
	void SetCameraBtn(CImageButton *pBtn);
	void SetTargetBtn(CImageButton *pBtn);
	void SetMeasureBtn(CImageButton *pBtn);
	void SetPatientTableBtn(CImageButton *pBtn);
	void SetMoveCameraBtn(CImageButton *pBtn);
	void SetBrowseImageBtn(CImageButton *pBtn);
	void SetCompareImageBtn(CImageButton *pBtn);
	void SetCaptureBtn(CImageButton *pBtn);
	void SetSystemBtn(CImageButton *pBtn);

// Dialog Data
	//{{AFX_DATA(CDlgToolSkin)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgToolSkin)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgToolSkin)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg LONG OnGetParentBK(WPARAM w,LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	void CreateDraw();
	CImageButton *m_btnCamera,*m_btnTarget,*m_btnPatientTable,
		*m_btnMeasure,*m_btnMoveCamera,*m_btnBrowseImage,
		*m_btnCompareImage,*m_btnCapture,*m_btnSystem;

	CMemDC m_skinDC;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTOOLSKIN_H__1690FC0B_D5D2_44CF_877C_C4F8E77804DA__INCLUDED_)
