#if !defined(AFX_DLGTARGETING_H__1F93E4E0_2513_402B_9EAD_58A6290CCDF4__INCLUDED_)
#define AFX_DLGTARGETING_H__1F93E4E0_2513_402B_9EAD_58A6290CCDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTargeting.h : header file
//
#include "memDC.h"
#include "mv.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTargeting dialog

class CDlgTargeting : public CDialog
{
// Construction
public:
	//ͨ���˾�������ϵת����Scanner����ϵ
	void SetBase2Scanner(Matrix m);
	//ͨ���˾�������ϵת������ʵScanner����ϵ
	//��Ҫ���ǵ�ͼ���ں������Ӱ��

	void Disable();
	//pAccuracyOrient :У׼���ľ�ȷ�����巽��
	//mProbe�е�Y����Ҳ�������巽�򣬵���û��У׼��
	void SetProbe(Matrix mProbe,double *pAccuracyOrient);
	void SetTargetPosition(double x,double y,double z);

	CDlgTargeting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTargeting)
	enum { IDD = IDD_DLG_TARGETING };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTargeting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTargeting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	double m_realTargetPos[3];
	double m_dDis2Target,m_dTargetError;

	int m_targetPos[2];
	int m_circlePos[2];
	int m_iMaxRadius;
	int m_iCurrentRadius;

	BOOL m_bEnableTargeting;
	CMemDC m_skinDC;

	CString m_sDistance,m_sError;
	Matrix m_Base2Scanner;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTARGETING_H__1F93E4E0_2513_402B_9EAD_58A6290CCDF4__INCLUDED_)
