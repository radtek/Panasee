#if !defined(AFX_DLGTPSSKIN_H__D3C798AA_E7FC_4B6A_B3CD_32D342B17F4E__INCLUDED_)
#define AFX_DLGTPSSKIN_H__D3C798AA_E7FC_4B6A_B3CD_32D342B17F4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTPSSkin.h : header file
//
#include "panaskin.h"
#include "MemDC.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTPSSkin dialog

class PANASKIN_API CDlgTPSSkin
{
// Construction
public:
	CDlgTPSSkin();   // standard constructor

	BOOL funOnEraseBkgnd( CDC* pDC );
	BOOL funOnInitDialog();
	void funCreateDraw();
	void funSetHandle(CWnd *pWnd);
	void SetEvaluateBtn(CImageButton *pBtn);
	void SetOpenBtn(CImageButton *pBtn);
	void SetSaveBtn(CImageButton *pBtn);
	void SetProbeBtn(CImageButton *pBtn);
	LONG OnGetParentBK(WPARAM w,LPARAM l);

private:
	CMemDC m_skinDC;
	CWnd *m_pWnd;
	CImageButton *m_btnEvaluate,*m_btnOpen,*m_btnSave,*m_btnProbe;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTPSSKIN_H__D3C798AA_E7FC_4B6A_B3CD_32D342B17F4E__INCLUDED_)
