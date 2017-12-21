#if !defined(AFX_DLGPATIENTINFO_H__2679571F_EC17_4010_951E_EE67977AAB50__INCLUDED_)
#define AFX_DLGPATIENTINFO_H__2679571F_EC17_4010_951E_EE67977AAB50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatientInfo.h : header file
//
#include "IGTImageHeader.h"
#include "ImriDBI.h"
#include "PanaseeDialog.h"
#include "IMriDefine.h"
#include "PatientInfoDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPatientInfo dialog

class CDlgPatientInfo : public CPanaseeDialog
{
// Construction
public:
	
	CDlgPatientInfo(CWnd* pParent = NULL);   // standard constructor
	
	void SetPatientInfo(IGT_PatientField* pPatientField);
	void SetStudyInfo(IMRI_STUDY *pStudy);
	void SetModalityID(CString sModalityID);
	void SetDataBase(CImriDBI* pDBmri);
	void SetMsgRcvr(HWND hwnd);
	BOOL GetNewPatientUID(int& iPatientUID);
	void GetPatientInfo(IGT_PatientField* pPatientField);
	void GetStudyInfo(IMRI_STUDY *pStudy);
	void RespondMRIStatus(ENUM_MRI_SYS_STATUS nSysStatus, unsigned short nSeriesStatus , int nOperResult);
	
	BOOL	m_bEnableEdit;
	BOOL m_bSendCreateStudyCommand;

// Dialog Data
	//{{AFX_DATA(CDlgPatientInfo)
	enum { IDD = IDD_PATIENT_INFOR };
	CButton	m_btnNew;
	CComboBox	m_cboBodyEntry;
	CComboBox	m_cboBodyPart;
	CComboBox	m_cboLandmarkPos;
	CComboBox	m_cboLyingPos;
	CComboBox	m_cboRadiologist;
	CComboBox	m_cboRefPhy;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CString	m_sPatient;
	CString	m_sMRNo;
	CString	m_sOperator;
	CString	m_sTime;
	CString	m_sID;
	CString	m_sDesc;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatientInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatientInfo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnSelectPatientLocal();
	afx_msg void OnBtnRegisterPatient();
	afx_msg LRESULT OnDLGPatientInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImriDBI* m_pDBmri;
	IGT_PatientField m_patInfo;
	IMRI_STUDY m_studyInfo;
	CString m_sCurModalityID;
	int   m_iCurPatientUID;
	BOOL    m_bInsertDatabase;

	CPatientInfoDlg m_dlgPatInfo;
	
private:
	int GetMaxStudyID();
	HWND	m_hwndMsgRcvr;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATIENTINFO_H__2679571F_EC17_4010_951E_EE67977AAB50__INCLUDED_)
