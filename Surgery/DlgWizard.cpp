// DlgWizard.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgWizard.h"
#include "ConfigMgr.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CImriDBI m_DBmri;
/////////////////////////////////////////////////////////////////////////////
// CDlgWizard dialog


CDlgWizard::CDlgWizard(CWnd* pParent /*=NULL*/)
	: CDlgWizardSkin(CDlgWizard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizard)
	//}}AFX_DATA_INIT

	m_bExitWholeProgram=TRUE;
}


void CDlgWizard::DoDataExchange(CDataExchange* pDX)
{
	CDlgWizardSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizard)
	DDX_Control(pDX, IDC_CMD_REDO, m_btnRedo);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizard, CDlgWizardSkin)
	//{{AFX_MSG_MAP(CDlgWizard)
	ON_MESSAGE(WM_WIZARD_STEP, OnWizardStep)
	ON_MESSAGE(WM_DIALOG_PATINFO_MSG, OnDLGPatientInfo)
	ON_MESSAGE(WM_MRI_PATIENT_INFO, OnMRIPatientInfo)
	ON_BN_CLICKED(IDC_CMD_REDO, OnCmdRedo)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)	
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizard message handlers


LONG CDlgWizard::OnInitDialog(UINT wParam, LONG lParam) 
{
	CDlgMain* pDlgMain =g_DlgMain;
	if(pDlgMain->m_DeviceType==DEVICE_IGS_CP)
	{
		SetUIMode(2);
	}else
	{
		SetUIMode(1);
	}

	SetQuitBtn(&m_btnExit);
	SetPreBtn(&m_btnRedo);

	CDlgWizardSkin::OnInitDialog();

	m_btnExit.SetWindowText("");
	m_btnRedo.SetWindowText("");
	
	//m_btnRedo.EnableWindow(FALSE);

	GetDlgItem(IDC_FRM_DLG)->ShowWindow(FALSE);


	// Init m_dlgBaseSel
	if (m_dlgBaseSel.GetSafeHwnd() == NULL)
	{
		m_dlgBaseSel.Create(IDD_DLG_BASESELECTION, this);
	}
	m_dlgBaseSel.ShowWindow(FALSE);

	// Init m_dlgCaliModeSel
	if (m_dlgCaliModeSel.GetSafeHwnd() == NULL)
	{
		m_dlgCaliModeSel.Create(IDD_DLG_CALI_MODE_SEL, this);
	}
	m_dlgCaliModeSel.ShowWindow(FALSE);

	// Init m_dlgPatInfo
	m_dlgPatInfo.SetDB(&m_DBmri);
	CConfigMgr conf;
	CString sCurModalityID = conf.Read(SYSINFO, SYS_LAST_MODALITY, "001");
	m_dlgPatInfo.SetMID(sCurModalityID);
	if (m_dlgPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgPatInfo.Create(CPatientInfoDlg::IDD, this);
	}
	m_dlgPatInfo.ShowWindow(FALSE);
	m_dlgPatInfo.SetMsgRcvr(m_hWnd);

	// Init m_dlgMRIPatInfo
	m_dlgMRIPatInfo.SetDataBase(&m_DBmri);
	if (m_dlgMRIPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgMRIPatInfo.Create(CDlgPatientInfo::IDD, this);
	}
	m_dlgMRIPatInfo.ShowWindow(FALSE);

	if(m_dlgImportPatient.GetSafeHwnd() == NULL)
	{
		m_dlgImportPatient.Create(CDlgImportPatient::IDD, this);
	}
	m_dlgImportPatient.ShowWindow(FALSE);

	if(m_dlgManualCP.GetSafeHwnd() == NULL)
	{
		m_dlgManualCP.Create(CDlgManualCalibrationCP::IDD, this);
	}
	m_dlgManualCP.ShowWindow(FALSE);

	{
		CRect rect;
		CRect dlgRect;
		GetWindowRect(&rect);
		GetDlgItem(IDC_FRM_DLG)->GetWindowRect(&dlgRect);
		GetDlgItem(IDC_FRM_DLG)->MoveWindow(250,120,rect.Width()-250,rect.Height()-120); //rect.Width() * 2/3, rect.Height() * 5/9, dlgRect.Width(), dlgRect.Height());
		GetDlgItem(IDC_FRM_DLG)->GetWindowRect(&rect);

		m_dlgBaseSel.GetWindowRect(&dlgRect);
		m_dlgBaseSel.MoveWindow(rect.left + (rect.Width() - dlgRect.Width()) / 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

		m_dlgCaliModeSel.GetWindowRect(&dlgRect);
		m_dlgCaliModeSel.MoveWindow(rect.left + (rect.Width() - dlgRect.Width()) / 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

		m_dlgPatInfo.GetWindowRect(&dlgRect);
		m_dlgPatInfo.MoveWindow(rect.left + (rect.Width() - dlgRect.Width())/ 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

		m_dlgMRIPatInfo.GetWindowRect(&dlgRect);
		m_dlgMRIPatInfo.MoveWindow(rect.left + (rect.Width() - dlgRect.Width())/ 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

		m_dlgImportPatient.GetWindowRect(&dlgRect);
		m_dlgImportPatient.MoveWindow(rect.left + (rect.Width() - dlgRect.Width())/ 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

		m_dlgManualCP.GetWindowRect(&dlgRect);
		m_dlgManualCP.MoveWindow(rect.left + (rect.Width() - dlgRect.Width())/ 2, rect.top + (rect.Height() - dlgRect.Height()) / 3, 
			dlgRect.Width(), dlgRect.Height());

	}

	SetWindowText("Panasee - Wizard");


	return 0; 
}

LRESULT CDlgWizard::OnWizardStep(WPARAM wParam, LPARAM lParam)
{
	SetStep((WIZARD_STEP)wParam, (WIZARD_STEP_RESULT)lParam);

	return 0;
}

void CDlgWizard::SetStep(WIZARD_STEP nStep, WIZARD_STEP_RESULT nRet)
{
	if (nStep < 1 || nStep > 6)
		return;


	//m_btnRedo.EnableWindow(nRet == WSRET_ERR);

	CDlgMain* pDlgMain = g_DlgMain;
	m_iStep = nStep;
	
	if(pDlgMain->m_DeviceType == DEVICE_IGS_MF)
	{
		switch (nStep)
		{
		case WSTEP_CAM:
			{
				
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_RUNNING);
				
				Invalidate(FALSE);
				UpdateWindow();
				
				CConfigMgr conf;
				BOOL bDebugNoCamera = pDlgMain->m_bDebugNoCamera;
				if (!bDebugNoCamera)
				{
					pDlgMain->m_dlgWizard.SetStatusMsg("Connecting camera...");
					if (!pDlgMain->m_trackerManager.GetHandler()->InitNDI())
					{
						
						pDlgMain->m_dlgWizard.SetStep(WSTEP_MRI, WSRET_ERR);
						PanaMessageBox(pDlgMain->m_trackerManager.GetHandler()->GetErrorInfo());
						return;
					}
					
					int iRadioBase = conf.Read(CONF_BASE, CONF_BASE_INDEX, 0);
					pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(iRadioBase);
					pDlgMain->m_dlgCryotherapy.InitTrackingTool();
					pDlgMain->m_dlgCryotherapy.StartTracking();
				}
				pDlgMain->m_dlgWizard.SetStep(WSTEP_MRI, WSRET_OK);
			}
			break;
		case WSTEP_MRI:
			m_dlgMRIPatInfo.ShowWindow(FALSE);
			m_dlgPatInfo.ShowWindow(FALSE);
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_MRI, PASK_WSRET_RUNNING);
				
				
				Invalidate(FALSE);
				UpdateWindow();
				SetStatusMsg("Connecting MRI...");
				
				m_pDlgMain->m_MRIWrap.InitMRI();
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_ERR);
				
			}
			break;
		case WSTEP_PAT:
			m_dlgBaseSel.ShowWindow(FALSE);
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_MRI, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_RUNNING);
				
				m_dlgMRIPatInfo.m_bEnableEdit = FALSE;
				m_dlgMRIPatInfo.m_bSendCreateStudyCommand=FALSE;
				
				m_bEnableMRICtrl = pDlgMain->m_bEnableMRICtrl;
				if (!m_bEnableMRICtrl)
				{
					ManualInputPatientInfo();
				}
				else
				{
					
					
					SetStatusMsg("Accepting patient information...");
					m_pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_PATIENT);
					
					
				}
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_MRI, PASK_WSRET_ERR);
				
			}
			break;
		case WSTEP_BAS:
			m_dlgCaliModeSel.ShowWindow(FALSE);
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_BAS, PASK_WSRET_RUNNING);
				
				
				m_dlgBaseSel.ShowWindow(TRUE);
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_ERR);
				
			}
			break;
		case WSTEP_CAL:
			if (nRet == WSRET_OK)
			{
				m_dlgBaseSel.ShowWindow(FALSE);
				
				SetWizardStatus(PASK_WSTEP_BAS, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_RUNNING);
				
				
				m_bEnableMRICtrl = pDlgMain->m_bEnableMRICtrl;
				if (m_bEnableMRICtrl && !m_pDlgMain->m_MRIWrap.m_bMRIConneted)
				{
					// If redo calibration, Prospect might restart to change protocols
					if (m_pDlgMain->m_MRIWrap.ConnectMRI())
					{
						
						m_pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_PROTOCOLS);
						
					}
				}
				
				m_dlgCaliModeSel.ShowWindow(TRUE);
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_BAS, PASK_WSRET_ERR);
				//			m_pbmpDispSteps[3] = &m_bmpSteps[3][3];
			}
			break;
		case WSTEP_END:
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_OK);
				
				m_pDlgMain->EnterSurgery();
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_ERR);
			}
			break;
		default:
			break;
		}
	}else //IGS-CP型产品
	{
		switch (nStep)
		{
		case WSTEP_CAM:
			{
				m_dlgImportPatient.ShowWindow(FALSE);
				pDlgMain->RemoveAMsgRcvWnd(&pDlgMain->m_dlgWizard.m_dlgImportPatient);
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_RUNNING);
				
				Invalidate(FALSE);
				UpdateWindow();
				
				CConfigMgr conf;
				BOOL bDebugNoCamera = pDlgMain->m_bDebugNoCamera;
				if (!bDebugNoCamera)
				{
					pDlgMain->m_dlgWizard.SetStatusMsg("Connecting camera...");
					if (!pDlgMain->m_trackerManager.GetHandler()->InitNDI())
					{
						
						pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_ERR);
						PanaMessageBox(pDlgMain->m_trackerManager.GetHandler()->GetErrorInfo());
						return;
					}
					
					int iRadioBase = conf.Read(CONF_BASE, CONF_BASE_INDEX, 0);
					pDlgMain->m_dlgCryotherapy.SetCurBaseIndex(iRadioBase);
					pDlgMain->m_dlgCryotherapy.InitTrackingTool();
					pDlgMain->m_dlgCryotherapy.StartTracking();
				}
				pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_OK);
			}
			break;
		case WSTEP_MRI:
			m_dlgImportPatient.ShowWindow(FALSE);
			pDlgMain->RemoveAMsgRcvWnd(&pDlgMain->m_dlgWizard.m_dlgImportPatient);
			if(nRet == WSRET_OK)
			{
				Redo();
			}
			break;
		case WSTEP_PAT:
			m_dlgManualCP.ShowWindow(FALSE);
			SetStatusMsg("Prepare data...");
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_RUNNING);
				
				m_dlgImportPatient.ShowWindow(TRUE);
				pDlgMain->AddAMsgRcvWnd(&pDlgMain->m_dlgWizard.m_dlgImportPatient);
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_CAM, PASK_WSRET_ERR);
				
			}
			break;
		case WSTEP_BAS:
			m_dlgManualCP.ShowWindow(FALSE);
			if(nRet == WSRET_OK)
			{
				Redo();
			}
			break;
		case WSTEP_CAL:
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_OK);
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_RUNNING);
				
				m_dlgManualCP.ShowWindow(TRUE);
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_PAT, PASK_WSRET_ERR);
			}
			break;
		case WSTEP_END:
			if (nRet == WSRET_OK)
			{
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_OK);
				
				m_pDlgMain->EnterSurgery();
			}
			else
			{
				SetWizardStatus(PASK_WSTEP_CAL, PASK_WSRET_ERR);
			}
			break;
		default:
			break;
		}
	}
	Invalidate(FALSE);
	UpdateWindow();
}


void CDlgWizard::SetOperType(OPERATIONTYPE nOperType)
{
	if (nOperType > OPERATION_TYPE_NONE && nOperType < OPERATION_TYPE_TPS )
	{
		switch (nOperType)
		{
		case OPERATION_TYPE_NONE:
			SetOperation(PASK_OPERATION_TYPE_NONE);
			break;
		case OPERATION_TYPE_BIOPSY:

			SetOperation(PASK_OPERATION_TYPE_BIOPSY);
			break;
		case OPERATION_TYPE_CRYOTHERAPY:

			SetOperation(PASK_OPERATION_TYPE_CRYOTHERAPY);
			break;
		case OPERARION_TYPE_BRACHYTHERAPY:

			break;
		case OPERATION_TYPE_OZONETHERAPY:

			break;
		case OPERATION_TYPE_REVIEW:

			SetOperation(PASK_OPERATION_TYPE_REVIEW);
			break;
		default:
			SetOperation(PASK_OPERATION_TYPE_NONE);
			break;
		}
	}
}

void CDlgWizard::ShowMRIPatientConfirmDlg(IGT_PatientField *patInfor,IMRI_STUDY *pStudy, CString sModalityID)
{
	
	m_dlgMRIPatInfo.SetModalityID(sModalityID);
	m_dlgMRIPatInfo.SetDataBase(&m_DBmri);
	m_dlgMRIPatInfo.SetPatientInfo(patInfor);
	
	m_dlgMRIPatInfo.SetMsgRcvr(m_hWnd);
	if (m_dlgMRIPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgMRIPatInfo.Create(CDlgPatientInfo::IDD, this);
	}

	
	m_dlgMRIPatInfo.ShowWindow(TRUE);
	m_dlgMRIPatInfo.SetStudyInfo(pStudy);
}

BOOL CDlgWizard::ManualInputPatientInfo()
{
	
	if (m_dlgPatInfo.GetSafeHwnd() == NULL)
	{
		m_dlgPatInfo.Create(CPatientInfoDlg::IDD, this);
	}
	m_dlgPatInfo.ShowWindow(TRUE);
	return TRUE;
}

LRESULT CDlgWizard::OnDLGPatientInfo(WPARAM wParam, LPARAM lParam)
{
	int iRet = (int)wParam;
	if (iRet == 1)
	{
		m_dlgPatInfo.GetSelectedPUID(m_pDlgMain->m_MRIWrap.m_iCurPatientUID);
		m_dlgPatInfo.GetSelectedPatientInfo(m_pDlgMain->m_MRIWrap.m_CurPatient);
		SetStep(WSTEP_BAS, WSRET_OK);
		return 0;
	}
	else
	{
		SetStep(WSTEP_BAS, WSRET_ERR);
		return 1;
	}
}

LRESULT CDlgWizard::OnMRIPatientInfo(WPARAM wParam, LPARAM lParam)
{
	int iRet = (int)wParam;
	if (iRet == 1)
	{
		m_dlgMRIPatInfo.GetPatientInfo(&(m_pDlgMain->m_MRIWrap.m_CurPatient));
		m_dlgMRIPatInfo.GetStudyInfo(&(m_pDlgMain->m_MRIWrap.m_CurStudy));
		m_dlgMRIPatInfo.GetNewPatientUID(m_pDlgMain->m_MRIWrap.m_iCurPatientUID);
            m_pDlgMain->m_dlgCryotherapy.m_dlgPatient.Update(); //med联调 nsj 2011.07.08
		
			SetStatusMsg("Accepting protocol information...");
			m_pDlgMain->m_MRIWrap.m_pImriSrv->SendInfoRequest(INFO_PROTOCOLS);
		
		//此时不是在wizard过程中因此不需要进入到下一步 nsj 2011.07.08
		if(this->IsWindowVisible())
		{
			SetStep(WSTEP_BAS, WSRET_OK);
		}
		
		return 0;
	}
	else
	{
		SetStep(WSTEP_BAS, WSRET_ERR);
		return 1;
	}
}

void CDlgWizard::Redo() 
{
	// TODO: Add your control notification handler code here
	SetWizardStatus(enum PASK_WIZARD_STEP(m_iStep), PASK_WSRET_WAIT);
	SetStep(enum WIZARD_STEP(m_iStep - 1), WSRET_OK);
}

void CDlgWizard::OnCmdRedo()
{
	Redo();
}

void CDlgWizard::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if(m_bExitWholeProgram)
	{
		::SendMessage(g_DlgMain->m_hWnd,WM_CLOSE,0,0);
	}else
	{
		CDlgMain* pDlgMain = g_DlgMain;
		//pDlgMain->m_dlgCryotherapy.m_dlgPatient.Update();
		pDlgMain->m_dlgWizard.ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.m_dlgBaseSel.ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.m_dlgCaliModeSel.ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.m_dlgPatInfo.ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.ShowWindow(FALSE);
		pDlgMain->m_dlgWizard.m_dlgImportPatient.ShowWindow(FALSE);
		pDlgMain->RemoveAMsgRcvWnd(&pDlgMain->m_dlgWizard.m_dlgImportPatient);
		pDlgMain->m_dlgWizard.m_dlgManualCP.ShowWindow(FALSE);
		pDlgMain->m_dlgCryotherapy.ShowWindow(TRUE);
		pDlgMain->m_dlgCryotherapy.RestoreTabDisplay();
		pDlgMain->m_display4Dlg.ShowWindow(TRUE);
	}
//	m_pDlgMain->Close();
}
