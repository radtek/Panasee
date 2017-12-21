// PatientInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatientInfoDlg.h"
#include "resource.h"
#include "PatInfoEditorDlg.h"
#include "PanaMetrics.h"
#include "PanaGlobal.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatientInfoDlg dialog


CPatientInfoDlg::CPatientInfoDlg(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPatientInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatientInfoDlg)
	m_csPID          = _T("");
	m_csPatName      = _T("");
	m_otimeBirthFrom = COleDateTime::GetCurrentTime();
	m_otimeBirthTo   = COleDateTime::GetCurrentTime();
	m_csAge = _T("");
	m_nSearchMode = -1;
	//}}AFX_DATA_INIT
	m_pdbImri     = NULL;
	m_csMID       = "";
	m_dwSelPID    = 0 ;
	m_dwMaxPID    = 0 ;
	m_nSearchMode = 0 ;
	m_nSortIndex  = 0 ;
	m_bAsc        = false ;
	m_hwndMsgRcvr = NULL;
	m_sLastSearch="";
	m_bMustRequery=FALSE;
}

void CPatientInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatientInfoDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SEARCHPAT, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_LIST_PATINFO, m_listPatInfo);
	DDX_Text(pDX, IDC_EDIT_PID, m_csPID);
	DDX_Text(pDX, IDC_EDIT_PNAME, m_csPatName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PATFROM, m_otimeBirthFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PATTO, m_otimeBirthTo);
	DDX_Text(pDX, IDC_EDIT_PATAGE, m_csAge);
	DDX_Radio(pDX, IDC_RADIO_ID, m_nSearchMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatientInfoDlg, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPatientInfoDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PATINFO, OnClickListPatinfo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PATINFO, OnDblclkListPatinfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PATINFO, OnColumnclickListPatinfo)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_NEW, OnBtnNew)
	ON_BN_CLICKED(IDC_BTN_SEARCHPAT, OnBtnSearchpat)
	ON_BN_CLICKED(IDC_CHECK_PATTEST, OnCheckPattest)
	ON_BN_CLICKED(IDC_RADIO_AGE, OnRadioAge)
	ON_BN_CLICKED(IDC_RADIO_ID, OnRadioId)
	ON_BN_CLICKED(IDC_RADIO_NAME, OnRadioName)
	ON_EN_SETFOCUS(IDC_EDIT_PATAGE, OnSetfocusEditPatage)
	ON_EN_SETFOCUS(IDC_EDIT_PID, OnSetfocusEditPid)
	ON_EN_SETFOCUS(IDC_EDIT_PNAME, OnSetfocusEditPname)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER_PATFROM, OnSetfocusDatetimepickerPatfrom)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER_PATTO, OnSetfocusDatetimepickerPatto)
	ON_EN_KILLFOCUS(IDC_EDIT_PATAGE, OnKillfocusEditPatage)
	ON_EN_KILLFOCUS(IDC_EDIT_PID, OnKillfocusEditPid)
	ON_EN_KILLFOCUS(IDC_EDIT_PNAME, OnKillfocusEditPname)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatientInfoDlg message handlers

void  CPatientInfoDlg::SetDB(CImriDBI* pdbImri)
{
	m_pdbImri = pdbImri;
}

void  CPatientInfoDlg::SetMID(CString csMID)
{
	m_csMID = csMID ;
}

void CPatientInfoDlg::SetMsgRcvr(HWND hwnd)
{
	m_hwndMsgRcvr = hwnd;
}

bool  CPatientInfoDlg::GetSelectedPID(int& dwPID)
{
	dwPID = m_dwSelPID ;
	return true ;
}

bool  CPatientInfoDlg::GetSelectedPUID(int& dwPUID)
{
	dwPUID = m_dwSelPUID ;
	return true ;
}

bool CPatientInfoDlg::GetSelectedPatientInfo(struct IGT_PatientField &patField)
{
	memcpy(&patField, &m_selPatField, sizeof(struct IGT_PatientField));
	return true;
}

BOOL CPatientInfoDlg::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	EnableSeprateLine(TRUE);
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_listPatInfo.SetExtendedStyle(dwExStyle) ;
	m_listPatInfo.InsertColumn(0, "ID",LVCFMT_CENTER, 70);
	m_listPatInfo.InsertColumn(1, "Name", LVCFMT_CENTER, 150);
	m_listPatInfo.InsertColumn(2, "Sex", LVCFMT_CENTER,  50);
	m_listPatInfo.InsertColumn(3, "Birthday", LVCFMT_CENTER, 150);
	m_listPatInfo.InsertColumn(4, "MId", LVCFMT_CENTER, 60);
	m_listPatInfo.InsertColumn(5, "Age", LVCFMT_CENTER, 0);
	m_listPatInfo.InsertColumn(6, "Desc.", LVCFMT_CENTER, 0);
	m_listPatInfo.InsertColumn(7, "Weight", LVCFMT_CENTER, 0);
	m_listPatInfo.InsertColumn(8, "PUID", LVCFMT_CENTER, 0);
	
	m_listPatInfo.InitHeader();
	
	m_pdbImri->SetQueryTestFlag(FALSE);

	if (m_pdbImri != NULL)
	{
		m_pdbImri->OpenPatientTable(m_listPatInfo,m_csMID);
		m_dwMaxPID = GetMaxPatId();
	}
	
	
	m_nSearchMode=0;
	UpdateData(FALSE);

	//设置UI界面
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnOK);
	btnList.AddTail(&m_btnCancel);
	SetBottomBtnList(&btnList);

	CRect rcTmp,rcClient;
	GetClientRect(&rcClient);

	GetDlgItem(IDC_STATIC_QUERY_PATIENT)->GetWindowRect(&rcTmp);
	ScreenToClient(&rcTmp);
	rcTmp.right=rcClient.right-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_RIGHT)+5);
	GetDlgItem(IDC_STATIC_QUERY_PATIENT)->MoveWindow(rcTmp);

	m_listPatInfo.GetWindowRect(&rcTmp);
	ScreenToClient(&rcTmp);
	rcTmp.left=CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_LEFT)+5;
	m_listPatInfo.MoveWindow(rcTmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatientInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_listPatInfo.GetSelectedCount() > 0)
	{
		int     i ;
		CString csPID ,csUID;
		POSITION pos = m_listPatInfo.GetFirstSelectedItemPosition();
		i = m_listPatInfo.GetNextSelectedItem(pos);
		csPID  = m_listPatInfo.GetItemText(i,0);
		m_dwSelPID = atoi(csPID.GetBuffer(16));
		csUID  = m_listPatInfo.GetItemText(i,8);
		m_dwSelPUID = atoi(csUID.GetBuffer(16));

		strcpy(m_selPatField.Id, m_listPatInfo.GetItemText(i,0));
		strcpy(m_selPatField.Name, m_listPatInfo.GetItemText(i,1));
		strcpy(m_selPatField.Desc, m_listPatInfo.GetItemText(i,6));
		m_selPatField.Sex = m_listPatInfo.GetItemText(i,2)[0];
		m_selPatField.Age = atoi(m_listPatInfo.GetItemText(i,5).GetBuffer(2));
		m_selPatField.Weight = atof(m_listPatInfo.GetItemText(i,7).GetBuffer(2));
		CString sBirth = m_listPatInfo.GetItemText(i,3);
		int iBirth[3];
		m_pdbImri->StringToDate(sBirth, iBirth);
		m_selPatField.BirthDate.year = iBirth[0];
		m_selPatField.BirthDate.month = iBirth[1];
		m_selPatField.BirthDate.day = iBirth[2];
	}
	else
	{
		m_dwSelPID = 0;
		PanaMessageBox("No patient is selected! Please select one.");
		return ;
	}
	if (m_hwndMsgRcvr)
	{
		::SendMessage(m_hwndMsgRcvr, WM_DIALOG_PATINFO_MSG, 1, 0);
	}
	CDialog::OnOK();
}

void CPatientInfoDlg::OnClickListPatinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
	*pResult = 0;
}

void CPatientInfoDlg::OnDblclkListPatinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPatInfoEditorDlg    dlg ;

	if (m_listPatInfo.GetSelectedCount() > 0)
	{
		int     i ;
		CString csPID ;
		POSITION pos = m_listPatInfo.GetFirstSelectedItemPosition();
		i = m_listPatInfo.GetNextSelectedItem(pos);
		dlg.SetEditMode(0);
		dlg.SetValue(m_listPatInfo.GetItemText(i,0),m_listPatInfo.GetItemText(i,1),
			         m_listPatInfo.GetItemText(i,3),m_listPatInfo.GetItemText(i,5),
					 m_listPatInfo.GetItemText(i,7),m_listPatInfo.GetItemText(i,2),
					 m_listPatInfo.GetItemText(i,6) );
		if (dlg.DoModal() == IDOK)
		{
		}
	}
	
	*pResult = 0;
}


void CPatientInfoDlg::OnBtnEdit() 
{
	// TODO: Add your control notification handler code here
	if (m_listPatInfo.GetSelectedCount() > 0)
	{
		CPatInfoEditorDlg  dlg ;
		int                i ;
		CString            csPID ;

		POSITION pos = m_listPatInfo.GetFirstSelectedItemPosition();
		i = m_listPatInfo.GetNextSelectedItem(pos);
		dlg.SetEditMode(1);
		dlg.SetValue(m_listPatInfo.GetItemText(i,0),m_listPatInfo.GetItemText(i,1),
			         m_listPatInfo.GetItemText(i,3),m_listPatInfo.GetItemText(i,5),
					 m_listPatInfo.GetItemText(i,7),m_listPatInfo.GetItemText(i,2),
					 m_listPatInfo.GetItemText(i,6) );
		if (dlg.DoModal() == IDOK)
		{
			int nSex = 2;
			CString csId, csName,csBirth, csAge, csSex, csWeight,csDesc;
			dlg.GetValue(csId,csName,csBirth,csAge,csWeight,csSex,csDesc);
			if (csSex.Compare("M") == 0)
			{
				nSex = 0 ;
			}
			else if (csSex.Compare("F") == 0)
			{
				nSex = 1 ;
			}
			if (m_pdbImri->UpdatePatiet(csId,csName,csBirth,csAge,nSex,csWeight,csDesc,m_csMID) > 0)
			{
				m_listPatInfo.SetItemText(i,1,(char*)(LPCTSTR)csName);
				m_listPatInfo.SetItemText(i,2,csSex);
				m_listPatInfo.SetItemText(i,3,csBirth);
				m_listPatInfo.SetItemText(i,5,csAge);
				m_listPatInfo.SetItemText(i,6,csDesc);
				m_listPatInfo.SetItemText(i,7,csWeight);

				m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
			}
		}
	}
}

void CPatientInfoDlg::OnBtnNew() 
{
	// TODO: Add your control notification handler code here
	CString csPID ;
	UINT    dwPID;
	dwPID= m_dwMaxPID + 1;
	csPID.Format("%d", dwPID);
	while(m_pdbImri->ExistPatient(csPID,m_csMID)>=0)
	{
		m_dwMaxPID++;
		dwPID= m_dwMaxPID + 1;
		csPID.Format("%d", dwPID);
	}

	CPatInfoEditorDlg    dlg ;
	dlg.SetEditMode(1)  ;
	dlg.SetValue(csPID,"","","0","0","M","" );
	if (dlg.DoModal() == IDOK)
	{
		int nSex = 2;
		CString csId, csName,csBirth, csAge, csSex, csWeight,csDesc;
		dlg.GetValue(csId,csName,csBirth,csAge,csWeight,csSex,csDesc);
		if (csSex.Compare("M") == 0)
		{
			nSex = 0 ;
		}
		else if (csSex.Compare("F") == 0)
		{
			nSex = 1 ;
		}
		int iUID;
		if ((iUID = m_pdbImri->AddPatient(csId,csName,csBirth,csAge,nSex,csWeight,csDesc,m_csMID)) > 0)
		{
			CString sUID;
			sUID.Format("%d", iUID);

			m_dwMaxPID ++ ;

			LV_ITEM    lvitem ;
			lvitem.mask     = LVIF_TEXT ;
			lvitem.iItem    = 0 ;
			lvitem.iSubItem = 0 ;
			lvitem.pszText  = (char*)(LPCTSTR)csId ;
			m_listPatInfo.InsertItem(&lvitem);
			m_listPatInfo.SetItemText(0,1,(char*)(LPCTSTR)csName);
			m_listPatInfo.SetItemText(0,2,csSex);
			m_listPatInfo.SetItemText(0,3,csBirth);
			m_listPatInfo.SetItemText(0,4,m_csMID);
			m_listPatInfo.SetItemText(0,5,csAge);
			m_listPatInfo.SetItemText(0,6,csDesc);
			m_listPatInfo.SetItemText(0,7,csWeight);
			m_listPatInfo.SetItemText(0,8,sUID);

			ClearAllSelMark();
			m_listPatInfo.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
			//自动滚动到第一行
			CRect rcItem;
			m_listPatInfo.GetItemRect(0,rcItem,LVIR_BOUNDS);
			CSize scrSize;
			scrSize.cy=0-m_listPatInfo.GetTopIndex() * rcItem.Height();
			m_listPatInfo.Scroll(scrSize);
		}else
		{
			CString sTmp=m_pdbImri->GetErrMsg();
			PanaMessageBox(sTmp);
		}
	}
}

void CPatientInfoDlg::OnColumnclickListPatinfo(NMHDR* pNMHDR, LRESULT* pResult) 
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
	ReSortPatientItem(nColIndex, &m_listPatInfo,m_bAsc);
	*pResult = 0;
}

void CPatientInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_dwSelPID = 0 ;
	if (m_hwndMsgRcvr)
	{
		::SendMessage(m_hwndMsgRcvr, WM_DIALOG_PATINFO_MSG, 0, 0);
	}
	CDialog::OnCancel();
}

UINT CPatientInfoDlg::GetMaxPatId()
{
	UINT dwPID = 0 ,dwPIDTmp;
	CString csPID ;
	int nItemCount = m_listPatInfo.GetItemCount() ;
	for (int i=0; i<nItemCount; i++)
	{
		csPID = m_listPatInfo.GetItemText(i,0);
		dwPIDTmp = atoi(csPID.GetBuffer(16)) ;
		if (dwPIDTmp > dwPID)
		{
			dwPID = dwPIDTmp ;
		}
	}
	return dwPID ;
}


int CPatientInfoDlg::SearchPatient(CString csConditions)
{
	int      nCnt = m_listPatInfo.GetItemCount();
	CString  csItemText ,csAge;
	int      nIndex ,nCount = 0;
	int      nCompMode = 0; // 0: =; 1: <=; 2: <; 3: >=; 4: >.
	int      nAge = 0 ;

	ClearAllSelMark();

	switch (m_nSearchMode)
	{
	case 0:
		nIndex = 0 ;
		break;
	case 1:
		nIndex = 1 ;
		break;
	case 2:
		{
			nIndex = 5 ;
        
			if (csConditions.GetAt(0) == 0x3C)
			{
				if (csConditions.GetAt(1) == 0x3D)
				{
					nCompMode = 1 ;
					csAge = csConditions.Right(csConditions.GetLength() - 2);
					nAge  = atoi(csAge.GetBuffer(4));
				}
				else
				{
					nCompMode = 2 ;
					csAge = csConditions.Right(csConditions.GetLength() - 1);
					nAge  = atoi(csAge.GetBuffer(4));
				}
			}
			else if (csConditions.GetAt(0) == 0x3E)
			{
				if (csConditions.GetAt(1) == 0x3D)
				{
					nCompMode = 3 ;
					csAge = csConditions.Right(csConditions.GetLength() - 2);
					nAge  = atoi(csAge.GetBuffer(4));
				}
				else
				{
					nCompMode = 4 ;
					csAge = csConditions.Right(csConditions.GetLength() - 1);
					nAge  = atoi(csAge.GetBuffer(4));
				}
			}
			else
			{
				nCompMode = 0 ;
				nAge      = atoi(csConditions.GetBuffer(4));
			}
		}
		break;
	default:
		nIndex = 0 ;
		break;
	}
	for (int i=0; i<nCnt; i++)
	{
		csItemText = m_listPatInfo.GetItemText(i,nIndex);
		if (m_nSearchMode != 2)
		{
			if (csItemText.Compare(csConditions) == 0)
			{
				m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
				m_listPatInfo.EnsureVisible(i, false);
				nCount++ ;
				break ;
			}
		}
		else
		{
			int  nCondAge = atoi(csItemText.GetBuffer(4));
			switch (nCompMode)
			{
			
			case 1:
				if (nCondAge <= nAge)
				{
					m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_listPatInfo.EnsureVisible(i, false);
					nCount++ ;
				}
				break;
			case 2:
				if (nCondAge < nAge)
				{
					m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_listPatInfo.EnsureVisible(i, false);
					nCount++ ;
				}
				break;
			case 3:
				if (nCondAge >= nAge)
				{
					m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_listPatInfo.EnsureVisible(i, false);
					nCount++ ;
				}
				break;
			case 4:
				if (nCondAge > nAge)
				{
					m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_listPatInfo.EnsureVisible(i, false);
					nCount++ ;
				}
				break;
			case 0:
			default:
				if (nCondAge == nAge)
				{
					m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_listPatInfo.EnsureVisible(i, false);
					nCount++ ;
				}
				break;
			}
		}
	}
	return nCount ;
}

int CPatientInfoDlg::SearchPatient(COleDateTime timeFrom, COleDateTime timeTo)
{
	int          nCnt = m_listPatInfo.GetItemCount();
	CString      csItemText ;
	COleDateTime timeBirth ,timeTmp;
	int          nCount = 0 ;

	ClearAllSelMark();

	if (timeFrom > timeTo)
	{
		timeTmp  = timeFrom ;
		timeFrom = timeTo ;
		timeTo   = timeTmp ;
	}
	timeFrom.SetDateTime(timeFrom.GetYear(),timeFrom.GetMonth(),timeFrom.GetDay(),0,0,0);
    timeTo.SetDateTime(timeTo.GetYear(),timeTo.GetMonth(),timeTo.GetDay(),23,59,59);
	
	for (int i=0; i<nCnt; i++)
	{
		int nYear,nMonth,nDay ;
		csItemText = m_listPatInfo.GetItemText(i,3);
		if (gCheckDateFormat(csItemText,nYear,nMonth,nDay) < 0)
		{
			return -1 ;
		}
		timeBirth.SetDateTime(nYear,nMonth,nDay,0,0,0);
		if ((timeBirth >= timeFrom) && (timeBirth <= timeTo))
		{
			m_listPatInfo.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
			nCount ++ ;
		}
	}
	return i ;
}

void CPatientInfoDlg::OnBtnSearchpat() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true) ;
	int iCount=-1;
	CString csSearchCond;

	if (m_nSearchMode != 3)
	{
		
		if (m_nSearchMode == 0)
		{
			csSearchCond = m_csPID ;
		}
		else if (m_nSearchMode == 1)
		{
			csSearchCond = m_csPatName ;
		}
		else
		{
			
			csSearchCond = m_csAge ;
		}
		csSearchCond.TrimLeft();
		csSearchCond.TrimRight();

		if(csSearchCond!=""&&csSearchCond!=m_sLastSearch)
		{
			iCount=SearchPatient(csSearchCond);
			m_sLastSearch=csSearchCond;
		}
	}
	else
	{
		iCount=SearchPatient(m_otimeBirthFrom,m_otimeBirthTo) ;
	}

	if(iCount==0)
	{
		PanaMessageBox("Cannot find patient!");
		if (m_nSearchMode != 3)
			m_sLastSearch=csSearchCond;
	}
}

void  CPatientInfoDlg::ClearAllSelMark()
{
	int nCnt = m_listPatInfo.GetItemCount() ;
	for (int i=0; i<nCnt; i++)
	{
		m_listPatInfo.SetItemState(i, NULL, LVIS_SELECTED );
	}
}


void CPatientInfoDlg::ReSortPatientItem(int nColIndex, CListCtrl *pList,bool bAsc)
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
	int*     pnSort;

	pstrArr = new CString[nCount];
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

	delete[] pnSort;
	delete[] pstrArr;
}

void CPatientInfoDlg::OnCheckPattest() 
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

	m_listPatInfo.DeleteAllItems();
	if (m_pdbImri != NULL)
	{
		m_pdbImri->OpenPatientTable(m_listPatInfo,m_csMID);
		m_dwMaxPID = GetMaxPatId();
	}
}

void CPatientInfoDlg::OnRadioAge() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_PATAGE)->SetFocus();
}

void CPatientInfoDlg::OnRadioId() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_PID)->SetFocus();
}

void CPatientInfoDlg::OnRadioName() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_PNAME)->SetFocus();
}

BOOL CPatientInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->wParam   ==   VK_RETURN))   
	{   
		
		if(pMsg->hwnd   ==   ((CEdit*)GetDlgItem(IDC_EDIT_PID))->m_hWnd ||
			pMsg->hwnd   ==   ((CEdit*)GetDlgItem(IDC_EDIT_PNAME))->m_hWnd ||
			pMsg->hwnd   ==   ((CEdit*)GetDlgItem(IDC_EDIT_PATAGE))->m_hWnd)   
		{   
			OnBtnSearchpat();   
			return TRUE;
		}   
		
		

	}   
	return CDialog::PreTranslateMessage(pMsg);
}

void CPatientInfoDlg::OnSetfocusEditPatage() 
{
	// TODO: Add your control notification handler code here
	m_nSearchMode=2;
	UpdateData(FALSE);
}

void CPatientInfoDlg::OnSetfocusEditPid() 
{
	// TODO: Add your control notification handler code here
	m_nSearchMode=0;
	UpdateData(FALSE);
}

void CPatientInfoDlg::OnSetfocusEditPname() 
{
	// TODO: Add your control notification handler code here
	m_nSearchMode=1;
	UpdateData(FALSE);
}

void CPatientInfoDlg::OnSetfocusDatetimepickerPatfrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_nSearchMode!=3)
	{
		m_nSearchMode=3;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CPatientInfoDlg::OnSetfocusDatetimepickerPatto(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_nSearchMode!=3)
	{
		m_nSearchMode=3;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CPatientInfoDlg::OnKillfocusEditPatage() 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
}

void CPatientInfoDlg::OnKillfocusEditPid() 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
}

void CPatientInfoDlg::OnKillfocusEditPname() 
{
	// TODO: Add your control notification handler code here
	m_sLastSearch="";
}

void CPatientInfoDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LIST_PATINFO)
		m_listPatInfo.MeasureItem(lpMeasureItemStruct);
	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CPatientInfoDlg::EnableEdit(BOOL bEdit)
{
	if(bEdit)
	{
		m_btnNew.ShowWindow(SW_SHOW);
		m_btnEdit.ShowWindow(SW_SHOW);
	}else
	{
		m_btnNew.ShowWindow(SW_HIDE);
		m_btnEdit.ShowWindow(SW_HIDE);
	}
}

void CPatientInfoDlg::QueryPatientByName(CString sName)
{
	
	m_listPatInfo.DeleteAllItems();
	if (m_pdbImri != NULL)
	{
		m_pdbImri->OpenPatientTableByName(&m_listPatInfo,sName);
		m_dwMaxPID = GetMaxPatId();
		m_bMustRequery=TRUE;
	}
}

void CPatientInfoDlg::Requery()
{
	if(!m_bMustRequery)
		return;

	if(m_bMustRequery)
	{
		OnCheckPattest();
		m_bMustRequery=FALSE;
	}
}
