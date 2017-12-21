#if !defined(AFX_DLGMANUALCALIBRATIONCP_H__5135342D_2179_4C82_81AB_77F73D8DBB5D__INCLUDED_)
#define AFX_DLGMANUALCALIBRATIONCP_H__5135342D_2179_4C82_81AB_77F73D8DBB5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgManualCalibrationCP.h : header file
//
#include "mv.h"
#include "PanaseeDialog.h"
#include "PanaListCtrl.h"
#include "Show2DSeries.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgManualCalibrationCP dialog

class CDlgManualCalibrationCP : public CPanaseeDialog
{
// Construction
public:
	CDlgManualCalibrationCP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgManualCalibrationCP)
	enum { IDD = IDD_DLG_EM_MANUAL_CALIBRATION };
	CPanaListCtrl	m_pntList;
	CButton	m_btnQuit;
	CButton	m_btnRecord;
	CButton	m_btnCalibrate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgManualCalibrationCP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgManualCalibrationCP)
	afx_msg void OnButtonQuit();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonCalibrate();
	afx_msg LRESULT OnMarkerResult(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClickListPnt(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	double Dis(double *p1,double *p2);
	BOOL FindCircleIndex(double pdCenter[3][4], int &index1, int &index2, int &index3);
	double GetAngle(Vector v1, Vector v2);
	void GetMatrix(double dTip[][3], double matrix[][4]);
	double m_dTip[4][3];
	double m_Scanner2Marker[4][4];
	double m_Series2Scanner[4][4];
	int m_iIndex;
	double m_preImageTip[3];

	BOOL m_bShowCalibrateTool;

	CShow2DSeries m_showSeries;

	double m_dMarker2PT[4][4];
	double m_dFlagPoint[3][4];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMANUALCALIBRATIONCP_H__5135342D_2179_4C82_81AB_77F73D8DBB5D__INCLUDED_)
