// PatientInfoMrgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PatientInfoMrgDlg.h"
#include "PanaMetrics.h"
#include "PanaGlobal.h"
#include <math.h>
#include "Shlwapi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatientInfoMrgDlg dialog


CPatientInfoMrgDlg::CPatientInfoMrgDlg(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPatientInfoMrgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatientInfoMrgDlg)
	m_nStudyQueryId = 0;
	m_nPatQueryId   = 0;
	m_csStudyParam = _T("");
	m_csPatientParam = _T("");
	m_csBodyPart = _T("");
	m_tmFrom = COleDateTime::GetCurrentTime();
	m_tmTo   = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_nListImageNum    = 0 ;
	m_nListSeriesNum   = 0 ;
	m_nListPatientNum  = 0 ;
	m_nListStudyNum    = 0 ;
	m_nListModalityNum = 0 ;
	m_bPatientAll      = false ;
	m_bModalityAll     = true ;
	m_bAsc             = false;
	m_nSortIndex       = -1;
	m_nSelSeriesNum    = 0 ;

	m_tmFrom.GetCurrentTime();
	m_tmTo.GetCurrentTime();
	//m_pdbImri = new CImriDBI ;

	m_bAsignPatient = false ;
	m_sInitialModalityID="";
	m_pCallbackMessageWnd=NULL;
	m_OnOKButtonMessage=0;

	m_pMRISeries = NULL;
	m_iColumn=4;
	m_iRow=1;
	m_iBeginIndex=0;
	m_iSliderPos=0;
	m_sDBFolder="";

	m_listStudyIndex=0;
	m_listSeriesIndex=0;
	m_listPatientIndex=0;
	m_listModalityIndex=0;

}

CPatientInfoMrgDlg:: ~CPatientInfoMrgDlg()
{
	Clean();		
}

void CPatientInfoMrgDlg::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatientInfoMrgDlg)
	DDX_Control(pDX, IDC_COMBO_SCANID, m_ctrlScanIDComb);
	DDX_Control(pDX, IDC_SLIDER_SCAN, m_ctrlScanIDSlider);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SEARCHP, m_btnSearchP);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TO, m_tmctrlTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FROM, m_tmctrlFrom);
	DDX_Control(pDX, IDC_LIST_STUDY, m_listStudy);
	DDX_Control(pDX, IDC_LIST_SERIES, m_listSeries);
	DDX_Control(pDX, IDC_LIST_PATIENT, m_listPatient);
	DDX_Control(pDX, IDC_LIST_MODALITY, m_listModality);
	DDX_Radio(pDX, IDC_RADIO_ID, m_nStudyQueryId);
	DDX_Radio(pDX, IDC_RADIO_PATID, m_nPatQueryId);
	DDX_Text(pDX, IDC_EDIT_STUDYPARAM, m_csStudyParam);
	DDX_Text(pDX, IDC_EDIT_PATIENTPARAM, m_csPatientParam);
	DDX_CBString(pDX, IDC_COMBO_BODY, m_csBodyPart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_FROM, m_tmFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TO, m_tmTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatientInfoMrgDlg, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPatientInfoMrgDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_MODALL, OnCheckModalityAll)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PATIENT, OnClickListPatient)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SERIES, OnClickListSeries)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODALITY, OnClickListModality)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STUDY, OnClickListStudy)
	ON_BN_CLICKED(IDC_CHECK_PATALL, OnCheckPatall)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PATIENT, OnColumnclickListPatient)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STUDY, OnColumnclickListStudy)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SERIES, OnColumnclickListSeries)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_MODALITY, OnColumnclickListModality)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SEARCHP, OnBtnSearchp)
	ON_BN_CLICKED(IDC_CHECK_PATTEST, OnCheckPattest)
	ON_BN_CLICKED(IDC_RADIO_DESC, OnRadioDesc)
	ON_BN_CLICKED(IDC_RADIO_ID, OnRadioId)
	ON_BN_CLICKED(IDC_RADIO_PATID, OnRadioPatid)
	ON_BN_CLICKED(IDC_RADIO_PATNAME, OnRadioPatname)
	ON_CBN_SETFOCUS(IDC_COMBO_BODY, OnSetfocusComboBody)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER_FROM, OnSetfocusDatetimepickerFrom)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER_TO, OnSetfocusDatetimepickerTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PATIENTPARAM, OnKillfocusEditPatientparam)
	ON_EN_KILLFOCUS(IDC_EDIT_STUDYPARAM, OnKillfocusEditStudyparam)
	ON_EN_SETFOCUS(IDC_EDIT_STUDYPARAM, OnSetfocusEditStudyparam)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_SCANID, OnSelchangeComboScanid)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SCAN, OnReleasedcaptureSliderScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatientInfoMrgDlg message handlers

BOOL CPatientInfoMrgDlg::OnInitDialog() 
{
	

	CPanaseeDialog::OnInitDialog();
	
	EnableSeprateLine(FALSE);
	
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	MoveWindow((x-1024)/2,(y-768)/2,1024,768);

	m_pdbImri->SetQueryTestFlag(FALSE);

	// TODO: Add extra initialization here
	//Init the List Controls.
	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_listModality.SetExtendedStyle(dwExStyle);
	m_listModality.InsertColumn(0, "ID",LVCFMT_CENTER, 30);
	m_listModality.InsertColumn(1, "Type", LVCFMT_CENTER, 60);
	m_listModality.InsertColumn(2, "Name", LVCFMT_CENTER, 100);
	m_listModality.InsertColumn(3, "Description", LVCFMT_CENTER, 100);

	

	m_listPatient.SetExtendedStyle(dwExStyle);
	m_listPatient.InsertColumn(0, "ID",LVCFMT_CENTER, 80);
	m_listPatient.InsertColumn(1, "Name", LVCFMT_CENTER, 180);
	m_listPatient.InsertColumn(2, "Sex", LVCFMT_CENTER,  50);
	m_listPatient.InsertColumn(3, "Birthday", LVCFMT_CENTER, 200);
	m_listPatient.InsertColumn(4, "MId", LVCFMT_CENTER, 50);
	m_listPatient.InsertColumn(5, "Age", LVCFMT_CENTER, 50);
	m_listPatient.InsertColumn(6, "Desc.", LVCFMT_CENTER, 50);
	m_listPatient.InsertColumn(7, "Weight", LVCFMT_CENTER, 67);
		
	m_listStudy.SetExtendedStyle(dwExStyle);
	m_listStudy.InsertColumn(0, "ID", LVCFMT_CENTER, 60);
	m_listStudy.InsertColumn(1, "Type", LVCFMT_CENTER, 60);
	m_listStudy.InsertColumn(2, "MId", LVCFMT_CENTER, 40);
	m_listStudy.InsertColumn(3, "Name", LVCFMT_CENTER, 120);
	m_listStudy.InsertColumn(4, "Body Part", LVCFMT_CENTER, 150);
	m_listStudy.InsertColumn(5, "Build Time", LVCFMT_CENTER, 140);
	m_listStudy.InsertColumn(6, "Description", LVCFMT_CENTER, 157);
		
	m_listSeries.SetExtendedStyle(dwExStyle);
	m_listSeries.InsertColumn(0, "ID", LVCFMT_CENTER, 60);
	m_listSeries.InsertColumn(1, "Protocol Name", LVCFMT_CENTER, 180);
	m_listSeries.InsertColumn(2, "StudyID", LVCFMT_CENTER, 60);
	m_listSeries.InsertColumn(3, "Description", LVCFMT_CENTER, 360);
	m_listSeries.InsertColumn(4, "MId", LVCFMT_CENTER, 67);

	m_listModality.InitHeader();
	m_listPatient.InitHeader();
	m_listSeries.InitHeader();
	m_listStudy.InitHeader();

	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_MODALL);
	pBtn->SetCheck(1);

	//m_btnOK.SetThemeHelper(&m_ThemeHelper);
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	//m_btnSearchP.SetThemeHelper(&m_ThemeHelper);
	//m_btnSearch.SetThemeHelper(&m_ThemeHelper);
	
	//Open dadabase to show in these lists
//	m_pdbImri->SetDBPathName("E:\\ProspectUpdate\\3.5.4.03\\XBIMRIDB.mdb");
	m_nListModalityNum = m_pdbImri->OpenSiteTable(m_listModality);
	if (m_nListModalityNum <= 0)
	{
		switch (m_nListModalityNum)
		{
		case 0 :
			//PanaMessageBox("The modality table is empty! Please inform provider.");
			break ;
		case -1:
			//PanaMessageBox("Can not connect DB! Please inform provider.");
			break ;
		case -2:
			//PanaMessageBox("Can not open modality table! Please inform provider.");
			break ;
		default:
			break ;
		}
		return false ;
	}
	else
	{
		CheckInitialModality() ;
	}
/*
	m_nListPatientNum = m_pdbImri->OpenPatientTable( m_listPatient );
	if (m_nListPatientNum >=0 )
	{
		m_listPatient.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		int nStudyNum = PatientSelectChanged();
//		m_nListPatientNum = nCount;
//		CString szItemNum;
//		szItemNum.Format("%d items",nCount);
//		GetDlgItem(IDC_STATIC_PATIENTNUM)->SetWindowText(szItemNum);
	}
*/

	COleDateTime tm1(1971,1,1,0,0,0);
	COleDateTime tm2(2100,1,1,0,0,0);
	m_tmctrlFrom.SetRange(&tm1,&tm2);
	m_tmctrlTo.SetRange(&tm1,&tm2);

	//设置UI界面
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnOK);
	btnList.AddTail(&m_btnCancel);
	SetBottomBtnList(&btnList);
	
	
	m_ctrlScanIDSlider.SetTicFreq(1);
	m_ctrlScanIDSlider.SetLineSize(1);
	m_ctrlScanIDSlider.SetPageSize(1);
	m_ctrlScanIDComb.EnableWindow(FALSE);
	m_ctrlScanIDSlider.EnableWindow(FALSE);



	int iDisplayLen=191;
	int iGap=1;
	int nLeft=0;
	int nTop=0;	
	int iIndex = 0;
	int nTmpLeft=19;
	int nTmpTop=510;
	CRect rs;
	GetClientRect(&rs);

	while(m_lstDispMRIDlgs.GetCount() < 4)
	{
		CDlgMRI2DDisplay* pDlg = new CDlgMRI2DDisplay;
		pDlg->Create(IDD_DLG_MRI_2D_DISPLAY, this);
		pDlg->SetParent(this);
		m_lstDispMRIDlgs.AddTail(pDlg);		
		
		rs.left=nTmpLeft+iGap;
		rs.top = nTmpTop;
		rs.right  = rs.left + iDisplayLen;
		rs.bottom = rs.top + iDisplayLen;			
		pDlg->MoveWindow(rs,FALSE);
		pDlg->ShowWindow(SW_SHOW);
		
		iIndex ++;
		if(iIndex % m_iColumn == 0) 
		{
			nTmpLeft=nLeft;
			nTop += iDisplayLen + iGap;
		}
		else 
		{
			nTmpLeft += iDisplayLen + iGap;
		}	
	}

	Layout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CPatientInfoMrgDlg::SetDBPathName(CString csDBPathName)
//{
//	m_pdbImri->SetDBPathName(csDBPathName);
//}

void CPatientInfoMrgDlg::LoadSeries(int iIndex/*=0*/)
{
	m_ImgArray.RemoveAll();
	int Number = m_SeriesNamePathVect.size();

	if (m_pMRISeries)
	{
		for(int i=0;i<m_pMRISeries->GetNumOfImages();i++)
		{
			m_pMRISeries->GetImageAt(i)->RemoveRotatedData();
		}
		
		delete m_pMRISeries;
		m_pMRISeries=NULL;
	}


	std::vector<CString>::iterator iter=m_SeriesNamePathVect.begin();
	CString filePath;
	int i;
	BOOL bSh=FALSE;
	
	for (i=0,iter=iter+iIndex;iter!=m_SeriesNamePathVect.end();iter++,i++)
	{			
		if(i==4)
		{
			bSh = TRUE;
			filePath += "|";
			break;
		}
		
		CString FileName = (*iter);
		
		if(i==0)
			filePath = FileName;
		else
			filePath = filePath + "|" + FileName;
	}


	if(!bSh)
		filePath += "|";


	m_pMRISeries = new CMRISeries;
	//if (m_pMRISeries->LoadImg(m_ImgArray,m_SeriesNamePathVect,iIndex/*filePath*/))
	if (m_pMRISeries->LoadImg(m_ImgArray,filePath))
	{
		delete m_pMRISeries;
		m_pMRISeries = NULL;
		return;
	}


	double dStep = Number/4.0;//m_iColumn;
	int iStep = (int)dStep;
	if(dStep*1000000 == iStep*1000000)
		iStep -=1;


	m_ctrlScanIDSlider.SetRange(0,iStep);
}

void CPatientInfoMrgDlg::SetMRISeries(int iIndex /*=0*/)
{
	if(!m_pMRISeries) return;
	
	if(m_iSliderPos != iIndex)
		LoadSeries(iIndex);

	Layout(m_iBeginIndex);
}

void CPatientInfoMrgDlg::Layout(int iImage/*=0*/)
{
	// layout images
	int iGap=2;
	int iDisplayWidth;
	int iDisplayHeight;	
	int nLeft=0;
	int nTop=0;	
	int imgIndex = 0;
	int iIndex = iImage;
	int nTmpLeft=17;
	int nTmpTop=540;
	iDisplayWidth=iDisplayHeight=181;

	CDlgMRI2DDisplay *pDlg2D;


	int size = m_ImgArray.GetSize();
	POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
	for (;pos;)
	{		
		pDlg2D = m_lstDispMRIDlgs.GetNext(pos);
		if(pDlg2D)
		{			
			CRect rs;
			rs.left=nTmpLeft;
			rs.top = nTmpTop;
			rs.right  = rs.left + iDisplayWidth;
			rs.bottom = rs.top + iDisplayHeight;			
			pDlg2D->MoveWindow(rs,FALSE);
			if(m_pMRISeries && imgIndex<size)
			{
				pDlg2D->LoadMRIImage(m_ImgArray.GetAt(imgIndex));
				pDlg2D->LoadMRIImage();
				pDlg2D->ShowWindow(SW_SHOW);
				pDlg2D->m_bBeyondImage=FALSE;
			}else
			{
				pDlg2D->m_bBeyondImage=TRUE;
			}

			
			imgIndex++;
			iIndex ++;
			if(iIndex % m_iColumn == 0 && pos) 
			{
				nTmpLeft=nLeft;
				nTop += iDisplayHeight + iGap;
			}
			else 
			{
				nTmpLeft += iDisplayWidth + iGap;
			}
		}
	}

}

void CPatientInfoMrgDlg::SetDB(CImriDBI* pdbIMri)
{
	m_pdbImri = pdbIMri;
}

void CPatientInfoMrgDlg::OnDestroy() 
{
	CPanaseeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	if(m_pdbImri)
//	{
//		delete m_pdbImri;
//	}
}

void CPatientInfoMrgDlg::OnCheckModalityAll() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_MODALL);
	int nCk = pBtn->GetCheck();
	if (nCk == 1)
	{
		for (int i=0; i<m_listModality.GetItemCount(); i++)
		{
			m_listModality.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED);
			m_bModalityAll = true ;
		}
	}
	else
	{
		for (int i=1; i<m_listModality.GetItemCount(); i++)
		{
			m_listModality.SetItemState( i, NULL, LVIS_SELECTED);
		}
		m_listModality.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		m_bModalityAll = false ;
	}
	ModalitySelectChanged() ;
}

void CPatientInfoMrgDlg::OnCheckPatall() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_PATALL);
	int nCk = pBtn->GetCheck();
	if (nCk == 1)
	{
		for (int i=0; i<m_listPatient.GetItemCount(); i++)
		{
			m_listPatient.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED);
		}
		m_bPatientAll = true ;
	}
	else
	{
		for (int i=1; i<m_listPatient.GetItemCount(); i++)
		{
			m_listPatient.SetItemState( i, NULL, LVIS_SELECTED);
		}
		m_listPatient.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		m_bPatientAll = false ;
	}
	PatientSelectChanged();
}

int CPatientInfoMrgDlg::ModalitySelectChanged()
{
	CString    csSQL, csTemp,csTemp1;
	int i  = 0;
	int nCount = 0;
	int nC1    = 0;
	POSITION pos;

    m_listStudy.DeleteAllItems();
	m_listSeries.DeleteAllItems();
	m_listPatient.DeleteAllItems();

	if (m_bModalityAll)
	{
		nCount = m_pdbImri->OpenPatientTable(m_listPatient);
	}
	else
	{
		pos = m_listModality.GetFirstSelectedItemPosition();
		while(pos) 
		{
			i = m_listModality.GetNextSelectedItem(pos);
			csTemp  = m_listModality.GetItemText(i, 0);
			nC1 = m_pdbImri->OpenPatientTable(m_listPatient,csTemp);
			if (nC1 > 0)
			{
				nCount += nC1 ;
			}
		}
	}

	if (nCount >= 0 )
	{
		if (m_bPatientAll)
		{
			OnCheckPatall() ;
		}
		else
		{
			m_listPatient.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		}
	    nCount = PatientSelectChanged() ;
	}

	return nCount ;
}

int CPatientInfoMrgDlg::PatientSelectChanged()
{
    CString    csSQL, csTemp,csTemp1;
	int i  = 0;
	int nCount = 0;
	int nC1    = 0;
	POSITION pos ,pos1;
	m_bAsignPatient = false ;

    m_listStudy.DeleteAllItems();
	m_listSeries.DeleteAllItems();

	if (m_bPatientAll&&m_bModalityAll)
	{
		nCount = m_pdbImri->OpenStudyTable(m_listStudy);
	}
    else if (m_bModalityAll)
	{
		pos = m_listPatient.GetFirstSelectedItemPosition();
		while(pos) 
		{
			i = m_listPatient.GetNextSelectedItem(pos);
			csTemp  = m_listPatient.GetItemText(i, 0);
			nC1 = m_pdbImri->OpenStudyTable(m_listStudy,csTemp);
			if (nC1 > 0)
			{
				nCount += nC1 ;
			}
		}
	}
	else if (m_bPatientAll)
	{
		pos = m_listModality.GetFirstSelectedItemPosition();
		while(pos) 
		{
			i = m_listModality.GetNextSelectedItem(pos);
			csTemp  = m_listModality.GetItemText(i, 0);
			nC1 = m_pdbImri->OpenStudyTable(m_listStudy,"",csTemp);
			if (nC1 > 0)
			{
				nCount += nC1 ;
			}
		}
	}
	else
	{
		pos = m_listModality.GetFirstSelectedItemPosition();
		while(pos) 
		{
			i = m_listModality.GetNextSelectedItem(pos);
			csTemp  = m_listModality.GetItemText(i, 0);

			pos1 = m_listPatient.GetFirstSelectedItemPosition();
			while(pos1) 
			{
				i = m_listPatient.GetNextSelectedItem(pos1);
				csTemp1  = m_listPatient.GetItemText(i, 0);
				nC1 = m_pdbImri->OpenStudyTable(m_listStudy,csTemp1,csTemp);
				if (nC1 > 0)
				{
					nCount += nC1 ;
				}
			}
		}
	}

	if (nCount >= 0 )
	{
		m_listStudy.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		StudySelectChanged();
		CString szItemNum;
		m_nListStudyNum = nCount;
	}
	
	return nCount ;
}

int CPatientInfoMrgDlg::StudySelectChanged()
{
    CString    csSQL, csTemp, csSId, csMId, csPId;
	int  i = 0;
	int  nCount = 0;
	int  nC1    = 0;
	bool bFirst = false ;

	m_listSeries.DeleteAllItems();

	csSQL   = "SELECT * FROM SeriesTable ";
	POSITION pos = m_listStudy.GetFirstSelectedItemPosition();
	while(pos) 
	{
		i = m_listStudy.GetNextSelectedItem(pos);

		if ( m_listStudy.GetItemText(i,5) == "M" )
		{
			m_listStudy.SetItemState( i, NULL, LVIS_SELECTED );
//			PanaMessageBox( "The selected study could not been opened ! " );
			continue;
		}
		
		csTemp  = m_listStudy.GetItemText(i, 0);
	    nC1 = m_pdbImri->OpenSeriesTable(csSQL,csTemp,m_listSeries);
		if (nC1 > 0)
		{
			nCount += nC1 ;
		}

		if ((m_bAsignPatient)&&(!bFirst))
		{
			csSId  = m_listStudy.GetItemText(i,0);
			csMId  = m_listStudy.GetItemText(i,2);
			bFirst = true ;

			csPId = m_pdbImri->QueryPatientId(csSId,csMId);
			if (!csPId.IsEmpty())
			{
				FindAndSelPatient(csPId,csMId);
			}
		}
	}

	LoadThumbnail();

	return nCount ;
}

void CPatientInfoMrgDlg::OnClickListModality(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE   temp   =   (LPNMITEMACTIVATE)   pNMHDR; 
	int index = temp->iItem;

	if(m_listStudyIndex == index)
		return;
	m_listStudyIndex = index;
	// TODO: Add your control notification handler code here


	m_sLastSearch="";
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_MODALL);
	bool bAll = ((pBtn->GetCheck())>0)?true:false;
	if ( bAll == true )
	{
		pBtn->SetCheck( false );
		m_bModalityAll = FALSE;
	}
	
	int nSelRows = m_listModality.GetSelectedCount();
	if (nSelRows <= 0)
	{
		//PanaMessageBox("Please select modality(ies).");
		return;
	}
	ModalitySelectChanged();
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnClickListPatient(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE   temp   =   (LPNMITEMACTIVATE)   pNMHDR; 
	int index = temp->iItem;
	
	if(m_listPatientIndex == index)
		return;
	m_listPatientIndex = index;
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_PATALL);
	bool bAll = ((pBtn->GetCheck())>0)?true:false ;
	if ( bAll == true )
	{
		pBtn->SetCheck( false );
		m_bPatientAll = FALSE;
	}

	int nSelRows = m_listPatient.GetSelectedCount();
	if (nSelRows <= 0)
		PanaMessageBox("Please select patient(s).");	

	PatientSelectChanged();


	*pResult = 0;
}

void CPatientInfoMrgDlg::OnClickListSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE   temp   =   (LPNMITEMACTIVATE)   pNMHDR; 
	int index = temp->iItem;
	
	if(m_listSeriesIndex == index)
		return;
	m_listSeriesIndex = index;
	// TODO: Add your control notification handler code here
	LoadThumbnail();
	
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnClickListStudy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE   temp   =   (LPNMITEMACTIVATE)   pNMHDR; 
	int index = temp->iItem;
	
	if(m_listStudyIndex == index)
		return;
	m_listStudyIndex = index;
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
	StudySelectChanged();
	*pResult = 0;
}


void CPatientInfoMrgDlg::ReSortPatientItem(int nColIndex, CListCtrl *pList,bool bAsc)
{
	int nCount = pList->GetItemCount();
	if (nCount <= 0)
	{
		return ;
	}
	CString  csTemp ;
	LV_ITEM  lvItem;
	int      i ;
	CString* pstrArr ;
	pstrArr = new CString[nCount];
	int* pnSort;
	pnSort  = new int[nCount];
	for (i=0; i<nCount; i++)
	{
		 pstrArr[i] = pList->GetItemText(i,nColIndex) ;
	}
	gBubleSortEx(pnSort, nCount, pstrArr ,bAsc);
	
	for (i=0; i<nCount; i++)
	{
		lvItem.mask   = LVIF_TEXT ;
		lvItem.iItem  = i ;
		pList->SetItemText(i,nColIndex,pstrArr[i]);
	}

	//Swap ID.
	if ( nColIndex != 0 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],0);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,0,pstrArr[i]);
		}
	}
	
	//Swap Name.
	if ( nColIndex != 1 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],1);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,1,pstrArr[i]);
		}
	}

	//Swap Sex.
	if ( nColIndex != 2 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],2);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,2,pstrArr[i]);
		}
	}

	//Swap BirthDay.
	if ( nColIndex != 3 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],3);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,3,pstrArr[i]);
		}
	}

	
    //Swap MID.
	if ( nColIndex != 4 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],4);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,4,pstrArr[i]);
		}
	}

	//Swap Age.
	if ( nColIndex != 5 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],5);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,5,pstrArr[i]);
		}
	}

	//Swap Desc.
	if ( nColIndex != 6 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],6);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,6,pstrArr[i]);
		}
	}

	//Swap Weight.
	if ( nColIndex != 7 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],7);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,7,pstrArr[i]);
		}
	}

	//Swap UID.
	if ( nColIndex != 8 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],8);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,8,pstrArr[i]);
		}
	}

	POSITION pos = pList->GetFirstSelectedItemPosition();
	int nSelCount = 0 ;
	while(pos) 
	{
		i = pList->GetNextSelectedItem(pos);
	    pList->SetItemState( i, NULL, LVIS_SELECTED);
		pnSort[nSelCount++] = pnSort[i] ;
	}
	for (i=0; i<nSelCount; i++)
	{
		pList->SetItemState( pnSort[i], LVIS_SELECTED, LVIS_SELECTED);
	}
	delete[] pnSort;
	delete[] pstrArr;
}

void CPatientInfoMrgDlg::ReSortStudyItem(int nColIndex, CListCtrl *pList,bool bAsc)
{
	int nCount = pList->GetItemCount();
	if (nCount <= 0)
	{
		return;
	}
	CString  csTemp ;
	LV_ITEM  lvItem;
	int      i ;
	CString* pstrArr ;
	pstrArr = new CString[nCount];
	int*     pnSort; 
	pnSort  = new int[nCount];
	for (i=0; i<nCount; i++)
	{
		 pstrArr[i] = pList->GetItemText(i,nColIndex);
	}

	if (nColIndex == 5)
	{
	    gBubleSortDateEx(pnSort, nCount, pstrArr ,bAsc);	// sort date
	}
	else
	{
		gBubleSortEx(pnSort, nCount, pstrArr ,bAsc);
	}
	
	for (i=0; i<nCount; i++)
	{
		lvItem.mask   = LVIF_TEXT ;
		lvItem.iItem  = i ;
		pList->SetItemText(i,nColIndex,pstrArr[i]);
	}

	//Swap ID
	if ( nColIndex != 0 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],0);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,0,pstrArr[i]);
		}
	}

	if ( nColIndex != 1 )
	{
		//Swap Patient Name.
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],1);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,1,pstrArr[i]);
		}
	}							

	//Swap Mr No.
	if ( nColIndex != 2 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],2);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,2,pstrArr[i]);
		}
	}

	//Swap Body Part
	if ( nColIndex != 3 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],3);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,3,pstrArr[i]);
		}
	}

	//Swap Build Date
	if ( nColIndex != 4 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],4);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,4,pstrArr[i]);
		}
	}

	//Swap Back Up.
	if ( nColIndex != 5 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],5);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,5,pstrArr[i]);
		}
	}

	//Swap Description.
	if ( nColIndex != 6 )
	{
		for (i=0; i<nCount; i++)
		{
			pstrArr[i] = pList->GetItemText(pnSort[i],6);
		}
		for (i=0; i<nCount; i++)
		{
			pList->SetItemText(i,6,pstrArr[i]);
		}
	}

	POSITION pos = pList->GetFirstSelectedItemPosition();
	int nSelCount = 0 ;
	while(pos) 
	{
		i = pList->GetNextSelectedItem(pos);
	    pList->SetItemState( i, NULL, LVIS_SELECTED);
		pnSort[nSelCount++] = pnSort[i] ;
	}
	for (i=0; i<nSelCount; i++)
	{
		pList->SetItemState( pnSort[i], LVIS_SELECTED, LVIS_SELECTED);
	}

	delete[] pnSort;
	delete[] pstrArr;
}

void CPatientInfoMrgDlg::OnColumnclickListPatient(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nColIndex = pNMListView->iSubItem;
	if (m_nSortIndex == nColIndex)
	{
		m_bAsc = !m_bAsc ;
	}
	else
	{
		m_nSortIndex = nColIndex ;
		m_bAsc       = false ;
	}
	ReSortPatientItem(nColIndex, &m_listPatient,m_bAsc);
	PatientSelectChanged() ;
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnColumnclickListStudy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nColIndex = pNMListView->iSubItem;
	if (m_nSortIndex == nColIndex)
	{
		m_bAsc = !m_bAsc ;
	}
	else
	{
		m_nSortIndex = nColIndex ;
		m_bAsc       = false ;
	}
	ReSortStudyItem(nColIndex, &m_listStudy,m_bAsc);
    StudySelectChanged() ;

	*pResult = 0;
}

void CPatientInfoMrgDlg::OnColumnclickListSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnColumnclickListModality(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

int CPatientInfoMrgDlg::GetSelectedSeriesNum()
{
	return m_nSelSeriesNum ;
}

int CPatientInfoMrgDlg::SetSelectedSeries()
{
	int nSelCount = m_listSeries.GetSelectedCount();
	POSITION pos  = m_listSeries.GetFirstSelectedItemPosition();
	int nC = 0 ;
	CString csSeriesId ;
	int i ;
	while(pos) 
	{
		i = m_listSeries.GetNextSelectedItem(pos);
		csSeriesId    = m_listSeries.GetItemText(i,0);
		m_nSelSeriesId[nC] = atoi(csSeriesId.GetBuffer(3));
		m_csSelStudyId[nC] = m_listSeries.GetItemText(i,2);
		m_csSelMId[nC]     = m_listSeries.GetItemText(i,4);
		nC++;
		if (nC>=100)
		{
			break ;
		}
	}
	m_nSelSeriesNum = nSelCount ;
	return nSelCount ;
}

int CPatientInfoMrgDlg::GetSelectedSeries(char csStudyId[100][256],int nSeriesId[100],char csMId[100][256],int nNum)
{
	if(nNum>m_nSelSeriesNum)
	{
		nNum=m_nSelSeriesNum;
	}
	for (int i=0; i<nNum; i++)
	{
		sprintf(csStudyId[i],"%s",m_csSelStudyId[i]);
		nSeriesId[i]=m_nSelSeriesId[i];
		sprintf(csMId[i],"%s",m_csSelMId[i]);
	}
	return nNum ;
}

int CPatientInfoMrgDlg::GetSelectedSeries(CString csStudyId[],int nSeriesId[],CString csMId[],int nNum)
{
	if(nNum>m_nSelSeriesNum)
	{
		nNum=m_nSelSeriesNum;
	}
	for (int i=0; i<nNum; i++)
	{
		csStudyId[i] = m_csSelStudyId[i] ;
		nSeriesId[i] = m_nSelSeriesId[i] ;
		csMId[i]     =   m_csSelMId[i] ;
	}
	return nNum ;
}

int CPatientInfoMrgDlg::GetSelectedSeries(CString csStudyId[],int nSeriesId[],CString csMId[])
{
	for (int i=0; i<m_nSelSeriesNum; i++)
	{
		csStudyId[i] = m_csSelStudyId[i] ;
		nSeriesId[i] = m_nSelSeriesId[i] ;
		csMId[i]     =   m_csSelMId[i] ;
	}
	return m_nSelSeriesNum ;
}

void CPatientInfoMrgDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetSelectedSeries() ;
	
	if(m_pCallbackMessageWnd!=NULL&&m_OnOKButtonMessage!=0)
	{
		::PostMessage(m_pCallbackMessageWnd->GetSafeHwnd(),m_OnOKButtonMessage,0,0);
	}

	CPanaseeDialog::OnOK();
	
}

void CPatientInfoMrgDlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
	if(m_nStudyQueryId==0 ||m_nStudyQueryId ==1)
	{
		m_csStudyParam.TrimLeft();
		m_csStudyParam.TrimRight();
		if(m_csStudyParam=="" || m_csStudyParam ==m_sLastSearch)
		{
			return;
		}else
		{
			m_sLastSearch=m_csStudyParam;
		}
	}

    m_listStudy.DeleteAllItems();
	m_listSeries.DeleteAllItems();
	
	// unselect all items
	for( int i=0; i< m_listPatient.GetItemCount(); i++ )
	{
		m_listPatient.SetItemState( i, NULL, LVIS_SELECTED );
	}

	switch (m_nStudyQueryId)
	{
	case 0:    //Query by studyId
		m_nListStudyNum = m_pdbImri->QueryByStudyId(m_csStudyParam,m_listStudy);
		
		break;
	case 1:    //Query by study description.
		m_nListStudyNum = m_pdbImri->QueryByStudyDesc(m_csStudyParam,m_listStudy);
		
		break;
	case 2:    //Query by body of the study.
		m_nListStudyNum = m_pdbImri->QueryByBodyPart(m_csBodyPart, m_listStudy);
		break;
	case 3:    //Query by data of the study created.
		{
			if (m_tmFrom > m_tmTo)
			{
				COleDateTime tmTmp ;
				tmTmp    = m_tmFrom ;
				m_tmFrom = m_tmTo ;
				m_tmTo   = tmTmp ;
			}
			m_nListStudyNum = m_pdbImri->QueryByStudyTime(m_tmFrom,m_tmTo,m_listStudy) ;
		}
		break;
	default:   //Query by studyId.
		m_nListStudyNum = m_pdbImri->QueryByStudyId(m_csStudyParam,m_listStudy);
		break;
	}
    
	if ( m_listStudy.GetItemCount() > 0 )
	{
		m_listStudy.EnsureVisible(0, false);
		m_listStudy.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		m_bAsignPatient = true ;
		StudySelectChanged();
	}
	else
	{
		PanaMessageBox("Cannot find study!");
		if(m_nStudyQueryId==0 ||m_nStudyQueryId ==1)
		{
		
			m_sLastSearch=m_csStudyParam;
		
		}
	}
}

void CPatientInfoMrgDlg::OnBtnSearchp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	m_csPatientParam.TrimLeft();
	m_csPatientParam.TrimRight();
	if(m_csPatientParam=="" || m_csPatientParam ==m_sLastSearch)
	{
		return;
	}else
	{
		m_sLastSearch=m_csPatientParam;
	}

	m_listStudy.DeleteAllItems();
	m_listSeries.DeleteAllItems();
	
	// unselect all items
	for( int i=0; i< m_listPatient.GetItemCount(); i++ )
	{
		m_listPatient.SetItemState( i, NULL, LVIS_SELECTED );
	}
	switch (m_nPatQueryId)
	{
	case 0:    //Query by studyId
		m_nListStudyNum = m_pdbImri->QueryByPatientID(m_csPatientParam, m_listStudy);
		break;
	case 1:    //Query by study description.
		m_nListStudyNum = m_pdbImri->QueryByName(m_csPatientParam, m_listStudy);
		break;
	default:   //Query by studyId.
		m_nListStudyNum = m_pdbImri->QueryByPatientID(m_csPatientParam, m_listStudy);
		break;
	}
    
	if ( m_listStudy.GetItemCount() > 0 )
	{
		m_listStudy.EnsureVisible(0, false);
		m_listStudy.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		m_bAsignPatient = true ;
		StudySelectChanged();
	}
	else
	{
		PanaMessageBox("Cannot find study!");
		m_sLastSearch=m_csPatientParam;
	}
}

int CPatientInfoMrgDlg::FindAndSelPatient(CString csPId,CString csMId)
{
	CString csListPId, csListMId ;
	int     nSelCount = -1 ;
	int     nCount = m_listPatient.GetItemCount();

	for (int j=0; j<nCount; j++)
	{
		m_listPatient.SetItemState( j, NULL, LVIS_SELECTED );
	}

	for (int i=0; i<nCount; i++)
	{
		csListPId = m_listPatient.GetItemText(i,0);
		if (csPId.Compare(csListPId) == 0)
		{
			csListMId = m_listPatient.GetItemText(i,4);
			if (csMId.Compare(csListMId) == 0)
			{
				nSelCount = i ;
				m_listPatient.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED );
				m_listPatient.EnsureVisible(i, false);
				break ;
			}
		}
	}

	return nSelCount ;
}

void CPatientInfoMrgDlg::OnCheckPattest() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_PATTEST);
	int nCk = pBtn->GetCheck();
	if (nCk == 1)
	{
		m_pdbImri->SetQueryTestFlag(TRUE);
	}else
	{
		m_pdbImri->SetQueryTestFlag(FALSE);
	}

	ModalitySelectChanged();
}



void CPatientInfoMrgDlg::OnRadioDesc() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_STUDYPARAM)->SetFocus();
	
}

void CPatientInfoMrgDlg::OnRadioId() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_STUDYPARAM)->SetFocus();
}

void CPatientInfoMrgDlg::OnRadioPatid() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_PATIENTPARAM)->SetFocus();
}

void CPatientInfoMrgDlg::OnRadioPatname() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_PATIENTPARAM)->SetFocus();
}

BOOL CPatientInfoMrgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if((pMsg->wParam   ==   VK_RETURN))   
	{   
		
		if(pMsg->hwnd   ==   ((CEdit*)GetDlgItem(IDC_EDIT_PATIENTPARAM))->m_hWnd)   
		{   
			OnBtnSearchp();   

		}   
		if(pMsg->hwnd   ==   ((CEdit*)GetDlgItem(IDC_EDIT_STUDYPARAM))->m_hWnd)   
		{   
			if(m_nStudyQueryId==0 ||m_nStudyQueryId ==1)
			{
				OnBtnSearch();
			}
		}  

		return TRUE;

	}   
	
	return CPanaseeDialog::PreTranslateMessage(pMsg);
}

void CPatientInfoMrgDlg::OnSetfocusComboBody() 
{
	// TODO: Add your control notification handler code here
	m_nStudyQueryId=2;
	UpdateData(FALSE);
}

void CPatientInfoMrgDlg::OnSetfocusDatetimepickerFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_nStudyQueryId!=3)
	{
		m_nStudyQueryId=3;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnSetfocusDatetimepickerTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_nStudyQueryId!=3)
	{
		m_nStudyQueryId=3;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CPatientInfoMrgDlg::OnKillfocusEditPatientparam() 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
}

void CPatientInfoMrgDlg::OnKillfocusEditStudyparam() 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
}

void CPatientInfoMrgDlg::OnSetfocusEditStudyparam() 
{
	// TODO: Add your control notification handler code here
	if(m_nStudyQueryId!=0 && m_nStudyQueryId!=1)
	{
		m_nStudyQueryId=0;
		UpdateData(FALSE);
	}
}

void CPatientInfoMrgDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDCtl==IDC_LIST_STUDY)
		m_listStudy.MeasureItem(lpMeasureItemStruct);
	if(nIDCtl==IDC_LIST_SERIES)
		m_listSeries.MeasureItem(lpMeasureItemStruct);
	if(nIDCtl==IDC_LIST_PATIENT)
		m_listPatient.MeasureItem(lpMeasureItemStruct);
	if(nIDCtl==IDC_LIST_MODALITY)
		m_listModality.MeasureItem(lpMeasureItemStruct);
	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CPatientInfoMrgDlg::SetInitialModalityID(CString sModalityID)
{
	m_sInitialModalityID=sModalityID;
}

void CPatientInfoMrgDlg::CheckInitialModality()
{
	CString sID;
	BOOL bFind=FALSE;
	int iIndex;
	for (int i=0; i<m_listModality.GetItemCount(); i++)
	{
		sID  = m_listModality.GetItemText(i, 0);
		if(sID==m_sInitialModalityID)
		{
			bFind=TRUE;
			iIndex=i;
			break;
		}
	}
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_MODALL);
	if(bFind)
	{
		pBtn->SetCheck(0);
		for (i=0; i<m_listModality.GetItemCount(); i++)
		{
			m_listModality.SetItemState( i, NULL, LVIS_SELECTED);
		}
		m_listModality.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);
		m_bModalityAll = false ;
		
		ModalitySelectChanged() ;
	}else
	{
		pBtn->SetCheck(1);
		OnCheckModalityAll();
	}
	
	
		
}

void CPatientInfoMrgDlg::SetOnOKCallbackMessage(CWnd *pMessageWnd, UINT uMessageID)
{
	m_pCallbackMessageWnd=pMessageWnd;
	m_OnOKButtonMessage=uMessageID;
}


void CPatientInfoMrgDlg::UpdateSeries()
{
	int iIndex = 0;

	if (m_pMRISeries)
	{
		POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
		for (;pos,iIndex<4;)
		{
			CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			if(pDlg->m_bBeyondImage)
				return;
			pDlg->LoadMRIImage(m_ImgArray.GetAt(iIndex));
			iIndex++;
		}
	}
}

void CPatientInfoMrgDlg::LoadThumbnail()
{
	CString sDBName;
	m_pdbImri->GetDBPath(sDBName,m_sDBFolder);	

	for(int j=0; j<m_listPatient.GetItemCount(); j++)
	{
		if( m_listPatient.GetItemState(j, LVIS_SELECTED) == LVIS_SELECTED )
		{
			m_sPetientID = m_listPatient.GetItemText(j,0);
			break;
		}		
	}

	CString csModality = m_listSeries.GetItemText(0,4);
	CString csStudy = m_listSeries.GetItemText(0,2);
	CString csSeries;

	bool bHaveSeleced = FALSE;
	for(int i=0; i<m_listSeries.GetItemCount(); i++)
	{
		if( m_listSeries.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			csSeries = m_listSeries.GetItemText(i,0);
			bHaveSeleced = TRUE;
			break;
		}

		bHaveSeleced = FALSE;
	}

	CString csSeriesPath="";
	csSeriesPath.Format("%s\\%s\\%s\\%s",m_sDBFolder,csModality,csStudy,csSeries);
	bool bExist = (PathFileExists(csSeriesPath)==0) ? false:true;

	if(!bHaveSeleced || !bExist)
	{
		if (m_lstDispMRIDlgs.GetCount() > 0)
		{
			POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
			for (;pos;)
			{
				CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
				if (pDlg)
				{
					pDlg->m_bBeyondImage=TRUE;
				}
			}
		}
		
		m_ctrlScanIDComb.EnableWindow(FALSE);
		m_ctrlScanIDSlider.EnableWindow(FALSE);
		Invalidate(FALSE);
		return;
	}

	m_ctrlScanIDComb.ResetContent();
	AddFile(csSeriesPath);

	m_ctrlScanIDComb.SetCurSel(0);
	m_ctrlScanIDSlider.SetPos(0);
	m_iSliderPos = 0;

	CString sScanName;
	m_ctrlScanIDComb.GetWindowText(sScanName);

	if(sScanName.IsEmpty())
	{
		m_ctrlScanIDSlider.EnableWindow(FALSE);
		m_ctrlScanIDComb.EnableWindow(FALSE);
		return;
	}else
	{
		m_ctrlScanIDSlider.EnableWindow(TRUE);
		m_ctrlScanIDComb.EnableWindow(TRUE);
	}

	sScanName.Replace(".","\\");
	CString csScanPath = m_sDBFolder + "\\" + sScanName;
	CString nTemp;

	AddFile(csScanPath);
	std::sort(m_SeriesNameTitleVect.begin(), m_SeriesNameTitleVect.end(), SortbyNumASC);	
	m_SeriesNamePathVect.clear();
	for (int iii=0;iii<m_SeriesNameTitleVect.size();iii++)
	{
		int na = m_SeriesNameTitleVect.at(iii);

		nTemp.Format("%s\\%d.%s",csScanPath,na,"img");

		m_SeriesNamePathVect.push_back(nTemp);
	}

 	LoadSeries();
 	SetMRISeries(m_iSliderPos*m_iColumn);
	Invalidate(FALSE);
}

void CPatientInfoMrgDlg::AddFile(CString StrPath)
{
	CFileFind OneFile;
	CString FName,FNameTitle,DirName;
	BOOL BeWorking; 
	m_SeriesNameTitleVect.clear();
	
	
	DirName = StrPath+"\\*.*";
	BeWorking = OneFile.FindFile( DirName );
	while ( BeWorking ) 
	{  				
		BeWorking = OneFile.FindNextFile();
		
		if ( OneFile.IsDirectory() && !OneFile.IsDots() ) 
		{	
			DirName = OneFile.GetFilePath();
			
			DirName = DirName.Right(DirName.GetLength()-m_sDBFolder.GetLength()-1);
			DirName.Replace("\\",".");
			
			int nDot = 0;
			for (int i=0;i<DirName.GetLength();i++)
			{
				if(DirName.GetAt(i) == '.')
					nDot++;
			}

			if(nDot<4)
			{
				m_ctrlScanIDComb.AddString(DirName);
				AddFile(DirName);
			}
		}
		
		if ( !OneFile.IsDirectory() && !OneFile.IsDots() ) 
		{
			FName = (OneFile.GetFileName()).Right(3);
			FNameTitle = OneFile.GetFileTitle();
			
			int nTemp=atoi((LPTSTR)(LPCTSTR)FNameTitle);

			if(FName.CompareNoCase("img")==0)
				m_SeriesNameTitleVect.push_back(nTemp);
		}		
	}
	
	OneFile.Close();								
}

void CPatientInfoMrgDlg::OnSelchangeComboScanid() 
{
	// TODO: Add your control notification handler code here
	CString sScanName,nTemp;

	m_ctrlScanIDSlider.SetPos(0);
	m_iBeginIndex = 0;
	m_iSliderPos = 0;
	
	int nlndex =m_ctrlScanIDComb.GetCurSel();
	m_ctrlScanIDComb.GetLBText(nlndex,sScanName);
	
	sScanName.Replace(".","\\");
	
	CString csScanPath = m_sDBFolder + "\\" + sScanName;
	
	AddFile(csScanPath);
	std::sort(m_SeriesNameTitleVect.begin(), m_SeriesNameTitleVect.end(), SortbyNumASC);	
	m_SeriesNamePathVect.clear();
	for (int iii=0;iii<m_SeriesNameTitleVect.size();iii++)
	{
		int na = m_SeriesNameTitleVect.at(iii);
		
		nTemp.Format("%s\\%d.%s",csScanPath,na,"img");
		
		m_SeriesNamePathVect.push_back(nTemp);
	}

	LoadSeries();
	SetMRISeries(m_iSliderPos*m_iColumn);
	Invalidate(FALSE);
}

void CPatientInfoMrgDlg::Clean()
{
	if (m_lstDispMRIDlgs.GetCount() > 0)
	{
		POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
		for (;pos;)
		{
			CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			if (pDlg)
			{
				pDlg->DestroyWindow();
				delete pDlg;
			}
		}
		m_lstDispMRIDlgs.RemoveAll();
	}
	
	if (m_pMRISeries)
	{
		for(int i=0;i<m_pMRISeries->GetNumOfImages();i++)
		{
			m_pMRISeries->GetImageAt(i)->RemoveRotatedData();
		}
		
		delete m_pMRISeries;
		m_pMRISeries=NULL;
	}
}

void CPatientInfoMrgDlg::OnReleasedcaptureSliderScan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_pMRISeries==NULL)
		return;
	
	UpdateData(TRUE);
	int iPos = m_ctrlScanIDSlider.GetPos();
	if(m_iSliderPos == iPos)
		return;
	
	m_iBeginIndex=iPos*m_iColumn;
	SetMRISeries(m_iBeginIndex);
	m_iSliderPos = iPos;
	
	UpdateData(FALSE);
	
	Invalidate(FALSE);

	*pResult = 0;
}


// 升序排列
bool CPatientInfoMrgDlg::SortbyNumASC(const int& x, const int& y)
{ 
	int nLeft, nRight; 
	nLeft = x; 
	nRight = y; 
	return nLeft<nRight;
}
// 降序排列
bool CPatientInfoMrgDlg::SortbyNumDESC(const int& x, const int& y)
{ 
	int nLeft, nRight; 
	nLeft = x; 
	nRight = y; 
	return nLeft>nRight;
}


