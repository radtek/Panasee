#if !defined(AFX_PATIENTINFODLG_H__4B0D79F6_67BC_4C84_A38D_03D5B937F8E3__INCLUDED_)
#define AFX_PATIENTINFODLG_H__4B0D79F6_67BC_4C84_A38D_03D5B937F8E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatientInfoDlg.h : header file
//

#include "ImriDBI.h"
#include "IGTImageHeader.h"
#include "PanaseeDialog.h"
#include "PanaListCtrl.h"


#define IDD_DIALOG_PATINFO              11022
/////////////////////////////////////////////////////////////////////////////
// CPatientInfoDlg dialog

class AFX_EXT_CLASS CPatientInfoDlg : public CPanaseeDialog
{
//Interface
public:
	void  SetDB(CImriDBI* pdbImri);
	void  SetMID(CString csMID);
	bool  GetSelectedPID(int& dwPID);
	bool  GetSelectedPUID(int& dwPUID);
	bool  GetSelectedPatientInfo(struct IGT_PatientField &patField);
	UINT  GetMaxPatId();
	void  SetMsgRcvr(HWND hwnd);
// Construction
public:
	void Requery();
	void EnableRequery(){ m_bMustRequery=TRUE;}
	void QueryPatientByName(CString sName);
	void EnableEdit(BOOL bEdit);
	CPatientInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatientInfoDlg)
	enum { IDD = IDD_DIALOG_PATINFO };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnSearch;
	CButton	m_btnNew;
	CButton	m_btnEdit;
	CPanaListCtrl	m_listPatInfo;
	CString	m_csPID;
	CString	m_csPatName;
	COleDateTime	m_otimeBirthFrom;
	COleDateTime	m_otimeBirthTo;
	CString	m_csAge;
	int		m_nSearchMode;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatientInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    
// Implementation
protected:
	CImriDBI*   m_pdbImri ;
	CString     m_csMID;
	int       m_dwSelPID ;
	int       m_dwSelPUID;
	struct IGT_PatientField m_selPatField;
	int       m_dwMaxPID ;
	//int         m_nSearchMode ; //0: Id; 1: Name; 2: Age; 3: Birth;
	int         m_nSortIndex;
	bool        m_bAsc ;

	int   SearchPatient(CString csConditions);
	int   SearchPatient(COleDateTime timeFrom, COleDateTime timeTo);
	void  ClearAllSelMark();
	void  ReSortPatientItem(int nColIndex, CListCtrl *pList,bool bAsc) ;
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatientInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickListPatinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPatinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListPatinfo(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnNew();
	afx_msg void OnBtnSearchpat();
	afx_msg void OnCheckPattest();
	afx_msg void OnRadioAge();
	afx_msg void OnRadioId();
	afx_msg void OnRadioName();
	afx_msg void OnSetfocusEditPatage();
	afx_msg void OnSetfocusEditPid();
	afx_msg void OnSetfocusEditPname();
	afx_msg void OnSetfocusDatetimepickerPatfrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusDatetimepickerPatto(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditPatage();
	afx_msg void OnKillfocusEditPid();
	afx_msg void OnKillfocusEditPname();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HWND	m_hwndMsgRcvr;
	CString m_sLastSearch;
	BOOL m_bMustRequery;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATIENTINFODLG_H__4B0D79F6_67BC_4C84_A38D_03D5B937F8E3__INCLUDED_)
