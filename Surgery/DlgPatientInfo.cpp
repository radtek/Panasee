// DlgPatientInfo.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgPatientInfo.h"
#include "PatInfoEditorDlg.h"
#include "Log.h"
#include "GlobalFuncs.h"
#include "DlgMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatientInfo dialog


CDlgPatientInfo::CDlgPatientInfo(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgPatientInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPatientInfo)
	m_sPatient = _T("None");
	m_sMRNo = _T("");
	m_sOperator = _T("");
	m_sTime = _T("");
	m_sID = _T("");
	m_sDesc = _T("");
	//}}AFX_DATA_INIT

	m_bInsertDatabase = FALSE;
	m_pDBmri = NULL;
	m_sCurModalityID = "";
	
	m_bEnableEdit = FALSE;
	m_hwndMsgRcvr = NULL;
	m_bSendCreateStudyCommand=FALSE;
}


void CDlgPatientInfo::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatientInfo)
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_COMBO_BODY_ENTRY, m_cboBodyEntry);
	DDX_Control(pDX, IDC_COMBO_BODYPART, m_cboBodyPart);
	DDX_Control(pDX, IDC_COMBO_LANDMARK_POS, m_cboLandmarkPos);
	DDX_Control(pDX, IDC_COMBO_LYING_POS, m_cboLyingPos);
	DDX_Control(pDX, IDC_COMBO_RADIOLOGIST, m_cboRadiologist);
	DDX_Control(pDX, IDC_COMBO_REF_PHY, m_cboRefPhy);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_STATIC_PATIENT, m_sPatient);
	DDX_Text(pDX, IDC_EDIT_MR_NO, m_sMRNo);
	DDX_Text(pDX, IDC_EDIT_OPERATOR, m_sOperator);
	DDX_Text(pDX, IDC_EDIT_STUDY_TIME, m_sTime);
	DDX_Text(pDX, IDC_EDIT_STUDY_ID, m_sID);
	DDX_Text(pDX, IDC_EDIT_STUDY_DESCRIPTION, m_sDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPatientInfo, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgPatientInfo)
	ON_BN_CLICKED(IDC_BTN_SELECT_PATIENT_LOCAL, OnBtnSelectPatientLocal)
	ON_BN_CLICKED(IDC_BTN_REGISTER_PATIENT, OnBtnRegisterPatient)
	ON_MESSAGE(WM_DIALOG_PATINFO_MSG, OnDLGPatientInfo)
	ON_BN_CLICKED(IDC_BTN_NEW, OnBtnNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatientInfo message handlers

BOOL CDlgPatientInfo::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnNew);
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	
	// TODO: Add extra initialization here
	m_cboBodyEntry.AddString("Head First");
	m_cboBodyEntry.AddString("Foot First");

	m_cboBodyPart.AddString("Head");
	m_cboBodyPart.AddString("Neck");
	m_cboBodyPart.AddString("Shoulder");
	m_cboBodyPart.AddString("Cervical Spine");
	m_cboBodyPart.AddString("Thoracic Spine");
	m_cboBodyPart.AddString("Lumber Spine");
	m_cboBodyPart.AddString("Spine");
	m_cboBodyPart.AddString("Chest");
	m_cboBodyPart.AddString("Breast");
	m_cboBodyPart.AddString("Abdomen");
	m_cboBodyPart.AddString("Pelvis");
	m_cboBodyPart.AddString("Hip");
	m_cboBodyPart.AddString("Elbow");
	m_cboBodyPart.AddString("Wrist");
	m_cboBodyPart.AddString("Arm");
	m_cboBodyPart.AddString("Hand");
	m_cboBodyPart.AddString("Knee");
	m_cboBodyPart.AddString("Leg");
	m_cboBodyPart.AddString("Ankle");
	m_cboBodyPart.AddString("Foot");
	m_cboBodyPart.AddString("Heart");
	m_cboBodyPart.AddString("Vascular Head");
	m_cboBodyPart.AddString("Vascular Neck");
	m_cboBodyPart.AddString("Vascular Body");
	m_cboBodyPart.AddString("Vascular Peripheral");
	m_cboBodyPart.AddString("Lung");
	m_cboBodyPart.AddString("Liver");
	m_cboBodyPart.AddString("Kieney");

	m_cboLandmarkPos.AddString("Nasion");
	m_cboLandmarkPos.AddString("Sternal");
	m_cboLandmarkPos.AddString("Notch");
	m_cboLandmarkPos.AddString("Xyphoid");
	m_cboLandmarkPos.AddString("Illiac Crest");
	m_cboLandmarkPos.AddString("Symph Publis");

	m_cboLyingPos.AddString("Supine");
	m_cboLyingPos.AddString("Prone");
	m_cboLyingPos.AddString("Decubitus Right");
	m_cboLyingPos.AddString("Decubitus Left");

	
	if (m_dlgPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgPatInfo.Create(CPatientInfoDlg::IDD, this);
	}
	m_dlgPatInfo.ShowWindow(FALSE);
	m_dlgPatInfo.SetMsgRcvr(m_hWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatientInfo::SetDataBase(CImriDBI* pDBmri)
{
	m_pDBmri = pDBmri;
	m_dlgPatInfo.SetDB(m_pDBmri);
}

void CDlgPatientInfo::SetModalityID(CString sModalityID)
{
	m_sCurModalityID = sModalityID;
	m_dlgPatInfo.SetMID(m_sCurModalityID);
}

void CDlgPatientInfo::SetPatientInfo(IGT_PatientField* pPatientField)
{
	
	if(!pPatientField)
	{
		memset(&m_patInfo,0,sizeof(IGT_PatientField));
		m_sPatient="None";
		if (m_hWnd)
		{
			UpdateData(FALSE);
		}
		return;
	}


	CString sID;
	sID.Format("%s", pPatientField->Id);
	CString sName;
	sName.Format("%s", pPatientField->Name);
	CString sBirth; 
	sBirth.Format("%4d-%02d-%02d", pPatientField->BirthDate.year, pPatientField->BirthDate.month, pPatientField->BirthDate.day);
	CString sWeight;
	sWeight.Format("%d", pPatientField->Weight);
	CString sDesc;
	sDesc.Format("%s", pPatientField->Desc);
	CString sAge;
	sAge.Format("%d", pPatientField->Age);

	int iUID = m_pDBmri->ExistPatient(sID, m_sCurModalityID);
	m_bInsertDatabase = TRUE;
	if (iUID <= 0)
	{
		iUID = m_pDBmri->AddPatient(sID, sName, sBirth, sAge, (pPatientField->Sex == 'M' ? 0 : (pPatientField->Sex == 'F' ? 1 : 2)), sWeight, sDesc, m_sCurModalityID);
		m_dlgPatInfo.EnableRequery();
	}
	if (iUID <= 0)
	{
		PanaMessageBox("Error in inserting patient data to database");
		m_bInsertDatabase = FALSE;
		return;
	}
	m_iCurPatientUID = iUID;

	CString sLog;
	sLog.Format("Current patient ID: %s, Name: %s", sID, sName);
	
	CLog::Log(sLog);

	memcpy(&m_patInfo, pPatientField, sizeof(IGT_PatientField));
	m_sPatient.Format(" [Patient ID]  %s\r\n [       Name]  %s\r\n [   Birthday]  %d-%02d-%-2d\r\n [          Sex]  %s\r\n [     Weight]  %.0d\r\n [         Note]  %s",
		              pPatientField->Id,pPatientField->Name,
					  pPatientField->BirthDate.year, pPatientField->BirthDate.month, pPatientField->BirthDate.day,
					  pPatientField->Sex=='M' ? "Male" : (pPatientField->Sex=='F' ? "Female" : "Unknown"),
					  pPatientField->Weight,pPatientField->Desc);

	if (m_hWnd)
	{
		UpdateData(FALSE);
	}
	
}

void CDlgPatientInfo::SetMsgRcvr(HWND hwnd)
{
	m_hwndMsgRcvr = hwnd;
}

void CDlgPatientInfo::GetPatientInfo(IGT_PatientField* pPatientField)
{
	memcpy(pPatientField, &m_patInfo, sizeof(IGT_PatientField));
}

void CDlgPatientInfo::GetStudyInfo(IMRI_STUDY *pStudy)
{
	memcpy(pStudy,&m_studyInfo,sizeof(IMRI_STUDY));
}

BOOL CDlgPatientInfo::GetNewPatientUID(int& iPatientUID)
{
	if (m_bInsertDatabase)
	{
		iPatientUID = m_iCurPatientUID;
	}
	return m_bInsertDatabase;
}

LRESULT CDlgPatientInfo::OnDLGPatientInfo(WPARAM wParam, LPARAM lParam)
{
	int iRet = (int)wParam;
	if (iRet == 1)
	{
		IGT_PatientField igtPatInfo;
		IMRI_PATIENT patInfo;
		m_dlgPatInfo.GetSelectedPUID(m_iCurPatientUID);
		m_dlgPatInfo.GetSelectedPatientInfo(igtPatInfo);

		patInfo.Age = igtPatInfo.Age;
		patInfo.BirthDate.year = igtPatInfo.BirthDate.year;
		patInfo.BirthDate.month = igtPatInfo.BirthDate.month;
		patInfo.BirthDate.day = igtPatInfo.BirthDate.day;
		sprintf((char *)patInfo.Desc,"%s",igtPatInfo.Desc);
		sprintf(patInfo.Id,"%s",igtPatInfo.Id);
		sprintf(patInfo.Name,"%s",igtPatInfo.Name);
		patInfo.Sex = igtPatInfo.Sex;
		patInfo.Weight = igtPatInfo.Weight;
		patInfo.Size=sizeof(IMRI_PATIENT);

		CDlgMain* pDlgMain = g_DlgMain;
		pDlgMain->m_dlgWizard.SetStatusMsg("Sending select patient command...");
		pDlgMain->m_MRIWrap.m_pImriSrv->SendCommand(COMMAND_SETCURPATIENT,&patInfo,patInfo.Size);

		//SetPatientInfo(&m_patInfo);
		return 0;
	}
	else
	{
		
		return 1;
	}
}

void CDlgPatientInfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_sPatient=="None")
	{
		PanaMessageBox("Please select patient first.");
		return;
	}

	m_sMRNo.TrimLeft();
	m_sMRNo.TrimRight();
	if(m_sMRNo.GetLength()<=0)
	{
		//PanaMessageBox("Please input MR No.");
		//return;
	}
	if(m_cboBodyEntry.GetCurSel()==CB_ERR)
	{
		PanaMessageBox("Please select body entry.");
		return;
	}
	if(m_cboBodyPart.GetCurSel()==CB_ERR)
	{
		PanaMessageBox("Please select body part.");
		return;
	}

	if(m_cboLyingPos.GetCurSel()==CB_ERR)
	{
		PanaMessageBox("Please select lying position.");
		return;
	}

	CString sTmp;
	m_studyInfo.PatBodyPart=m_cboBodyPart.GetCurSel();
	m_studyInfo.PatEntry=m_cboBodyEntry.GetCurSel();
	m_studyInfo.PatPosition=m_cboLyingPos.GetCurSel();
	m_studyInfo.LandmarkPos=m_cboLandmarkPos.GetCurSel();
	m_cboRadiologist.GetWindowText(sTmp);
	sprintf(m_studyInfo.Radiologist,"%s",sTmp);
	m_cboRefPhy.GetWindowText(sTmp);
	sprintf(m_studyInfo.RefPhy,"%s",sTmp);
	sprintf(m_studyInfo.Desc,"%s",m_sDesc);
	if(m_sID=="")
	{
		CString csStudyID ;
		int iTmp=GetMaxStudyID();
		if(iTmp<=0)
		{
			return;
		}
		UINT    dwStudyID=(UINT)iTmp;
		csStudyID.Format("%d", dwStudyID);
		sprintf(m_studyInfo.Id,"%s",csStudyID);
	}else
	{
		sprintf(m_studyInfo.Id,"%s",m_sID);
	}
	

	sprintf(m_studyInfo.MrNo,"%s",m_sMRNo);
	sprintf(m_studyInfo.OperatorID,"%s",m_sOperator);
	CTime time=CTime::GetCurrentTime(); 
	m_studyInfo.Time.year=time.GetYear();
	m_studyInfo.Time.month=time.GetMonth();
	m_studyInfo.Time.day=time.GetDay();
	m_studyInfo.Time.hour=time.GetHour();
	m_studyInfo.Time.minute=time.GetMinute();
	m_studyInfo.Time.second=time.GetSecond();
	m_studyInfo.Size=sizeof(IMRI_STUDY);

	
	if(m_sID=="")
	{
		CDlgMain* pDlgMain = g_DlgMain;
		pDlgMain->m_dlgWizard.SetStatusMsg("Sending create study command...");
		pDlgMain->m_MRIWrap.m_pImriSrv->SendCommand(COMMAND_CREATESTUDY,&m_studyInfo,m_studyInfo.Size);
		m_bSendCreateStudyCommand=TRUE;
		return;
	}

	if (m_hwndMsgRcvr)
	{
		::SendMessage(m_hwndMsgRcvr, WM_MRI_PATIENT_INFO, 1, 0);
	}
	CPanaseeDialog::OnOK();
	
}

void CDlgPatientInfo::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_hwndMsgRcvr)
	{
		::SendMessage(m_hwndMsgRcvr, WM_MRI_PATIENT_INFO, 0, 0);
	}
	
	CPanaseeDialog::OnCancel();
}

void CDlgPatientInfo::SetStudyInfo(IMRI_STUDY *pStudy)
{
	if(m_bEnableEdit && !m_bSendCreateStudyCommand)
	{
		pStudy=NULL;
	}

	int nIndex;
	CString sTmp;
	if(pStudy)
	{
		memcpy(&m_studyInfo, pStudy, sizeof(IMRI_STUDY));

		m_cboBodyEntry.SetCurSel(int(pStudy->PatEntry));
		m_cboBodyPart.SetCurSel(int(pStudy->PatBodyPart));
		m_cboLyingPos.SetCurSel(int(pStudy->PatPosition));
		m_cboLandmarkPos.SetCurSel(int(pStudy->LandmarkPos));
		
	
		sTmp.Format("%s",pStudy->Radiologist);
		if((nIndex=m_cboRadiologist.FindString(0, sTmp)) != CB_ERR)
		{
			m_cboRadiologist.SetCurSel(nIndex);
		}else
		{
			nIndex = m_cboRadiologist.AddString(sTmp);
			m_cboRadiologist.SetCurSel(nIndex);
		}

		sTmp.Format("%s",pStudy->RefPhy);
		if((nIndex=m_cboRefPhy.FindString(0, sTmp)) != CB_ERR)
		{
			m_cboRefPhy.SetCurSel(nIndex);
		}else
		{
			nIndex = m_cboRefPhy.AddString(sTmp);
			m_cboRefPhy.SetCurSel(nIndex);
		}

		m_sDesc.Format("%s",pStudy->Desc);
		m_sID.Format("%s",pStudy->Id);
		while(m_sID.GetLength()<6)
		{
			m_sID="0"+m_sID;
		}
		m_sMRNo.Format("%s",pStudy->MrNo);
		m_sOperator.Format("%s",pStudy->OperatorID);
		m_sTime.Format("%4d-%02d-%02d %02d:%02d:%02d",pStudy->Time.year,
			pStudy->Time.month,pStudy->Time.day,pStudy->Time.hour,
			pStudy->Time.minute,pStudy->Time.second);
		
		if(!m_bEnableEdit)
		{
			GetDlgItem(IDC_BTN_SELECT_PATIENT_LOCAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_REGISTER_PATIENT)->EnableWindow(FALSE);
		
			GetDlgItem(IDC_COMBO_BODYPART)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_MR_NO)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_LYING_POS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_LANDMARK_POS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_BODY_ENTRY)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_REF_PHY)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_RADIOLOGIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OPERATOR)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STUDY_DESCRIPTION)->EnableWindow(FALSE);
		}

	}else
	{
		memset(&m_studyInfo, 0, sizeof(IMRI_STUDY));
		m_sDesc="";
		m_sID="";
		m_sMRNo="";
		m_sOperator="";
		CTime time=CTime::GetCurrentTime(); 
		m_sTime.Format("%4d-%02d-%02d %02d:%02d:%02d",time.GetYear(),
			time.GetMonth(),time.GetDay(),time.GetHour(),
			time.GetMinute(),time.GetSecond());

		GetDlgItem(IDC_BTN_SELECT_PATIENT_LOCAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REGISTER_PATIENT)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_BODYPART)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MR_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LYING_POS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LANDMARK_POS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BODY_ENTRY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_REF_PHY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RADIOLOGIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPERATOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STUDY_DESCRIPTION)->EnableWindow(TRUE);
	}

	if (m_hWnd)
	{
		UpdateData(FALSE);
	}

	if(pStudy)
	{
		if(m_bSendCreateStudyCommand)
		{
			m_bSendCreateStudyCommand=FALSE;
			if (m_hwndMsgRcvr)
			{
				::SendMessage(m_hwndMsgRcvr, WM_MRI_PATIENT_INFO, 1, 0);
			}
			CPanaseeDialog::OnOK();

			
		}
	}else
	{
		m_bSendCreateStudyCommand=FALSE;
	}

}

void CDlgPatientInfo::OnBtnSelectPatientLocal() 
{
	// TODO: Add your control notification handler code here
	
	if (m_dlgPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgPatInfo.Create(CPatientInfoDlg::IDD, this);
	}
	m_dlgPatInfo.EnableEdit(FALSE);
	CRect rect;
	CRect dlgRect;
	this->GetWindowRect(&rect);
	m_dlgPatInfo.GetWindowRect(&dlgRect);
	m_dlgPatInfo.MoveWindow(rect.left + (rect.Width()-dlgRect.Width())/2,rect.top,
			dlgRect.Width(),dlgRect.Height());
	
	m_dlgPatInfo.Requery();
	m_dlgPatInfo.ShowWindow(TRUE);
}

void CDlgPatientInfo::OnBtnRegisterPatient() 
{
	// TODO: Add your control notification handler code here
	CPatInfoEditorDlg  dlg ;
	dlg.SetEditMode(2)  ;
	dlg.SetValue("","","","0","0","M","" );
	if (dlg.DoModal() == IDOK)
	{
		int nSex = 2;
		IMRI_PATIENT patInfo;
		CString csId, csName,csBirth, csAge, csSex, csWeight,csDesc;
		dlg.GetValue(csId,csName,csBirth,csAge,csWeight,csSex,csDesc);
		
		/*取得可用的最大ID*/
		CString csPID ;
		UINT    dwPID;
		dwPID= m_dlgPatInfo.GetMaxPatId() + 1;
		csPID.Format("%d", dwPID);
		while(m_pDBmri->ExistPatient(csPID,m_sCurModalityID)>=0)
		{
			dwPID++;
			csPID.Format("%d", dwPID);
		}

		int iBirth[3];
		m_pDBmri->StringToDate(csBirth, iBirth);
		patInfo.BirthDate.year = iBirth[0];
		patInfo.BirthDate.month = iBirth[1];
		patInfo.BirthDate.day = iBirth[2];
		sprintf((char*)patInfo.Desc,"%s",csDesc);
		sprintf(patInfo.Id,"%s",csPID);
		sprintf(patInfo.Name,"%s",csName);
		patInfo.Weight = atoi(csWeight);
		if(csSex=="M")
		{
			patInfo.Sex='M';
		}else if(csSex=="F")
		{
			patInfo.Sex='F';
		}else
		{
			patInfo.Sex='U';
		}

		patInfo.Age=atoi(csAge);
		
		patInfo.Size = sizeof(IMRI_PATIENT);
		
		CDlgMain* pDlgMain = g_DlgMain;
		if(m_pDBmri->OpenPatientTableByName(NULL,csName)>0)
		{
			if(PanaMessageBox("There are patients with same name in database. \nDo you want to check it?",MB_OKCANCEL)==IDOK)
			{
				m_dlgPatInfo.EnableEdit(FALSE);
				CRect rect;
				CRect dlgRect;
				this->GetWindowRect(&rect);
				m_dlgPatInfo.GetWindowRect(&dlgRect);
				m_dlgPatInfo.MoveWindow(rect.left + (rect.Width()-dlgRect.Width())/2,rect.top,
						dlgRect.Width(),dlgRect.Height());
	
				m_dlgPatInfo.QueryPatientByName(csName);
				m_dlgPatInfo.ShowWindow(TRUE);
				
			}else
			{
				pDlgMain->m_dlgWizard.SetStatusMsg("Sending create patient command...");
				pDlgMain->m_MRIWrap.m_pImriSrv->SendCommand(COMMAND_CREATEPATIENT,&patInfo,patInfo.Size);
			}
		}else
		{
			pDlgMain->m_dlgWizard.SetStatusMsg("Sending create patient command...");
			pDlgMain->m_MRIWrap.m_pImriSrv->SendCommand(COMMAND_CREATEPATIENT,&patInfo,patInfo.Size);
		}

		//SetPatientInfo(&m_patInfo);
	}
}

void CDlgPatientInfo::RespondMRIStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
										   unsigned short nSeriesStatus /*= 0*/, int nOperResult /*= 0*/)
{
	CString sTmp="";
	switch (nOperResult)
	{
	case 0x00010000:
		sTmp="新建或更换病人:未知错误";
		break;
	case 0x00010001:
		sTmp="新建或更换病人:ID编码错";
		break;
	case 0x00010002:
		sTmp="新建或更换病人:姓名不合法";
		break;
	case 0x00010003:
		sTmp="新建或更换病人:生日不合理";
		break;
	case 0x00010004:
		sTmp="新建或更换病人:重量不合理";
		break;
	case 0x00010005:
		sTmp="新建或更换病人:性别不合法";
		break;
	case 0x00020000:
		sTmp="新建study:未知错误";
		break;
	case 0x00020001:
		sTmp="新建study:ID编码错";
		break;
	case 0x00020002:
		sTmp="新建study:时间不合理";
		break;
	case 0x00020003:
		sTmp="新建study:方向编码错";
		break;
	case 0x00020004:
		sTmp="新建study:方位编码错";
		break;
	case 0x00020005:
		sTmp="新建study:部位编码不合法";
		break;
	case 0x00020006:
		sTmp="新建study:LandmarkPos编码错";
		break;
	case 0x00020007:
		sTmp="新建study:OperatorID编码错";
		break;
	case 0x00020008:
		sTmp="新建study:Radiologist姓名不合法";
		break;
	case 0x00020009:
		sTmp="新建study:RefPhy姓名不合法";
		break;
	}

	if(sTmp!="")
	{
		PanaMessageBox(sTmp);
	}
}

int CDlgPatientInfo::GetMaxStudyID()
{
	CString sSql;
	sSql.Format("Select ID from StudyTable where ModalityId='%s'",m_sCurModalityID);
	_RecordsetPtr pResult;
	BOOL bReturn;
	bReturn=m_pDBmri->ExecuateSQL(sSql,pResult);
	if(!bReturn)
	{
		PanaMessageBox("Failed to query database.");
		return -1;
	}

	if(pResult==NULL)
	{
		PanaMessageBox("Failed to query database.");
		return -1;
	}
	
	int iID=1;
	CString sID;
	int iTmp;
	try
	{
		while (!(pResult->EndOfFile))
		{
			_variant_t theValue;

			
			theValue = pResult->Fields->GetItem("ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
				sID = (char*)_bstr_t(theValue);
				
			
			}else
			{
				pResult->MoveNext();
				continue;
			}

			iTmp=atoi(sID);
			if(iID<iTmp)
				iID=iTmp;
		
			pResult->MoveNext();
				
		}
				
		pResult->Close();
	}
	catch(...)
	{
		PanaMessageBox("Failed to query database.");
		return -1;
	}

	iID++;
	return iID;
}



void CDlgPatientInfo::OnBtnNew() 
{
	// TODO: Add your control notification handler code here
	m_bEnableEdit=TRUE;
	
	SetPatientInfo(NULL);
	SetStudyInfo(NULL);
}
