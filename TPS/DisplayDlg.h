#if !defined(AFX_DISPLAYDLG_H__79EEB526_0AE9_4383_95F7_7D8C5C9DCBE9__INCLUDED_)
#define AFX_DISPLAYDLG_H__79EEB526_0AE9_4383_95F7_7D8C5C9DCBE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayDlg.h : header file
//
#include "resource.h"
#include "AppDialog.h"
#include "TPS2DView.h"
#include "TPS3DView.h"
#include "Commands.h"
#include "MPRView.h"
#include "TPSGlobalConst.h"
/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg dialog

class CDisplayDlg : public CAppDialog
{
// Construction
public:
	
	CDisplayDlg(CWnd* pParent = NULL);   // standard constructor
	void DispatchCommand(CWnd* pSender, int nCmdID,CString& sPara);
	virtual ~CDisplayDlg();	
		
	void UpdateRealProbeInfo(double *tipPos, double *ori);

// Dialog Data
	//{{AFX_DATA(CDisplayDlg)
	enum { IDD = IDD_DIALOG_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateRealProbe();

	void DeleteProbe(CString sProbe);
	void SetMPRProbePosition();
	void SetMPRProbe(CString sPara);
	void SetMPRImage(CString sPara);
	void Layout(CMD_TPS_COMMAND command,int para);
	
	CMD_TPS_COMMAND m_layoutStyle;
	int m_layoutPara;
	
	CMPRView *m_pMPRView1;
	CMPRView *m_pMPRView2;
	BOOL m_bDisplayMPR;

	CString m_sMPRProbe; //正在进行mpr的针的名字
	vtkMatrix4x4 *m_pMPRImageMatrix; //正在进行mpr的图像的坐标
	int m_iMPRImageWindow;

	CTPS2DView *m_2DView1,*m_2DView2,*m_2DView3;
	CTPS3DView *m_3DView;

	BOOL m_bRealProbePosChanged; //实时针的位置是否变化
	double m_RealProbeTipPos[3], m_RealProbeOri[3];
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYDLG_H__79EEB526_0AE9_4383_95F7_7D8C5C9DCBE9__INCLUDED_)
