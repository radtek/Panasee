#if !defined(AFX_PATIENTINFOMRGDLG_H__29D5FDD9_D39B_43FF_8151_5192F92F512A__INCLUDED_)
#define AFX_PATIENTINFOMRGDLG_H__29D5FDD9_D39B_43FF_8151_5192F92F512A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatientInfoMrgDlg.h : header file

#include <vector>
//using namespace std;

#include "ImriDBI.h"
#include "PanaseeDialog.h"
#include "PanaListCtrl.h"
#include "MRISeries.h"
#include "DlgMRI2DDisplay.h"

#define IDD_DIALOG_PATINFOMRG           11000
/////////////////////////////////////////////////////////////////////////////
// CPatientInfoMrgDlg dialog

class  AFX_EXT_CLASS CPatientInfoMrgDlg : public CPanaseeDialog
{
// Construction
public:
	CPatientInfoMrgDlg(CWnd* pParent = NULL);   // standard constructor
	~CPatientInfoMrgDlg();

// Dialog Data
	//{{AFX_DATA(CPatientInfoMrgDlg)
	enum { IDD = IDD_DIALOG_PATINFOMRG };
	CComboBox	m_ctrlScanIDComb;
	CSliderCtrl	m_ctrlScanIDSlider;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnSearchP;
	CButton	m_btnSearch;
	CDateTimeCtrl	m_tmctrlTo;
	CDateTimeCtrl	m_tmctrlFrom;
	CPanaListCtrl	m_listStudy;
	CPanaListCtrl	m_listSeries;
	CPanaListCtrl	m_listPatient;
	CPanaListCtrl	m_listModality;
	int		m_nStudyQueryId;
	int		m_nPatQueryId;
	CString	m_csStudyParam;
	CString	m_csPatientParam;
	CString	m_csBodyPart;
	COleDateTime	m_tmFrom;
	COleDateTime	m_tmTo;
	//}}AFX_DATA

	//CThemeHelperST		m_ThemeHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatientInfoMrgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetOnOKCallbackMessage(CWnd *pMessageWnd,UINT uMessageID);
	void SetInitialModalityID(CString sModalityID);
    //void       SetDBPathName(CString csDBPathName);
	void	   SetDB(CImriDBI* pdbIMri);
	int        ModalitySelectChanged();
	int        PatientSelectChanged() ;
	int        StudySelectChanged() ;
	void       ReSortPatientItem(int nColIndex, CListCtrl *pList,bool bAsc);
	void       ReSortStudyItem(int nColIndex, CListCtrl *pList,bool bAsc);
    int        GetSelectedSeriesNum();
	int        SetSelectedSeries();
	int        GetSelectedSeries(CString csStudyId[],int nSeriesId[],CString csMId[],int nNum);
	int        GetSelectedSeries(CString csStudyId[],int nSeriesId[],CString csMId[]);
	int        GetSelectedSeries(char csStudyId[100][256],int nSeriesId[100],char csMId[100][256],int nNum); 
	int        FindAndSelPatient(CString csPId,CString csMId);
protected:
	
	CString m_sLastSearch;
	int         m_nListImageNum;
	int         m_nListSeriesNum;
	int         m_nListPatientNum;
	int         m_nListStudyNum;
	int         m_nListModalityNum ;
	bool        m_bPatientAll;
	bool        m_bModalityAll ;
	bool        m_bAsc;
	bool        m_bAsignPatient ;
	int         m_nSortIndex;
	int         m_nSelSeriesNum ;
	CString     m_csSelStudyId[MAX_SEL_SERIES];
	CString     m_csSelMId[MAX_SEL_SERIES];
	int         m_nSelSeriesId[MAX_SEL_SERIES];
	CString m_sInitialModalityID; //≥ı ºModalityID
	CWnd* m_pCallbackMessageWnd;
	UINT m_OnOKButtonMessage;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatientInfoMrgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCheckModalityAll();
	afx_msg void OnClickListPatient(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListModality(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListStudy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckPatall();
	afx_msg void OnColumnclickListPatient(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListStudy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListSeries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListModality(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSearchp();
	afx_msg void OnCheckPattest();
	afx_msg void OnRadioDesc();
	afx_msg void OnRadioId();
	afx_msg void OnRadioPatid();
	afx_msg void OnRadioPatname();
	afx_msg void OnSetfocusComboBody();
	afx_msg void OnSetfocusDatetimepickerFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusDatetimepickerTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditPatientparam();
	afx_msg void OnKillfocusEditStudyparam();
	afx_msg void OnSetfocusEditStudyparam();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelchangeComboScanid();
	afx_msg void OnReleasedcaptureSliderScan(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CheckInitialModality();

	//add load series 
private:

	int	m_listStudyIndex;
	int	m_listSeriesIndex;
	int	m_listPatientIndex;
	int	m_listModalityIndex;

	static bool SortbyNumASC(const int& x, const int& y);
	static bool SortbyNumDESC(const int& x, const int& y);

	CString m_iLastClickPatientID;
	
	std::vector<int> m_SeriesNameTitleVect;
	std::vector<CString> m_SeriesNamePathVect;
 	CArray<CMRIImage*, CMRIImage*> m_ImgArray;
	int m_iColumn,m_iRow;
	int m_iBeginIndex;
	int m_iSliderPos;
	CMRISeries* m_pMRISeries;
	CList<CDlgMRI2DDisplay*, CDlgMRI2DDisplay*> m_lstDispMRIDlgs;

	void AddFile(CString StrPath);
	void Clean();
	void LoadSeries(int iIndex=0);
	void SetMRISeries(int iIndex=0);


public:
	CString m_sPetientID;

	CImriDBI*   m_pdbImri;
	CString m_sDBFolder;

	void UpdateSeries();
	void Layout(int iImage=0);

	void LoadThumbnail();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATIENTINFOMRGDLG_H__29D5FDD9_D39B_43FF_8151_5192F92F512A__INCLUDED_)
