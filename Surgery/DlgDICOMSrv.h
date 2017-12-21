#if !defined(AFX_DLGDICOMSRV_H__8EDED2E6_3D9C_45E4_8F61_1D2A11DE34E7__INCLUDED_)
#define AFX_DLGDICOMSRV_H__8EDED2E6_3D9C_45E4_8F61_1D2A11DE34E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDICOMSrv.h : header file
//
#include "Dicom\util.h"

#include "PanaseeDialog.h"
#include "PanaListCtrl.h"
#include "MRISeries.h"
#include "DlgMRIBrowse.h"

#include "..\DcmTk\config\include\dcmtk\config\osconfig.h"
#include "dcmtk.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgDICOMSrv dialog
class CDlgMain;
class CDlgDICOMSrv : public CPanaseeDialog
{
// Construction
public:
	void Close();
	CDlgDICOMSrv(CWnd* pParent = NULL);   // standard constructor
	~CDlgDICOMSrv();

	CDlgMain* m_pDlgMain;
	CWnd* m_pMessageWnd;
	
// Dialog Data
	//{{AFX_DATA(CDlgDICOMSrv)
	enum { IDD = IDD_DLG_DICOM_SRV };
	CButton	m_btnLoad;
	CButton	m_btnClose;
	CPanaListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDICOMSrv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDICOMSrv)
	afx_msg void CStoreSvrMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void CDcmServiceMsg(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDblclkListDicomList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClose();
	afx_msg void OnButtonLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//private:
public:
	void CheckPatientID(CMRISeries *pSeries);
	void SendDicomSeriesMessage();
	void UpdateView(CMRISeries *pSeries);
	CList<CMRISeries*, CMRISeries*> m_lstSeries;
	void SaveImage(int iType);

	void SendMsg(CString sMsg);
	void SendMsg(char *pMsg);
	
	// DICOM
	BOOL m_bModeDICOM;
	BOOL m_bEnableDICOM;
	HANDLE m_svrHandle;
	BYTE	*m_lpBuff;
	BOOL StartService();
	BOOL StopService();
	FCBI m_FCBI;
	CBI m_CBI;

	CDlgMRIBrowse m_dlgMRIBrowse;
	
	BOOL m_bToshibaCTTable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDICOMSRV_H__8EDED2E6_3D9C_45E4_8F61_1D2A11DE34E7__INCLUDED_)
