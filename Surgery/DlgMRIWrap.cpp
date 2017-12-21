// DlgMRIWrap.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMRIWrap.h"
#include "Log.h"
#include "ConfigMgr.h"
#include "DlgMain.h"
#include "DlgWizard.h"
#include <winsock2.h>	
#include <ws2tcpip.h>
#include "TCPCom.h"
#include "MRISeries.h"


CImriDBI m_DBmri;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMRIWrap dialog

CDlgMRIWrap::CDlgMRIWrap(CWnd* pParent /*=NULL*/)
: CDialog(CDlgMRIWrap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMRIWrap)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlgMain = NULL;
	
	m_bEnableGradCorrect=FALSE;
	m_sGradFile="";

	m_pImriSrv = NULL;
	m_iScanCount=0;
	
}

CDlgMRIWrap::~CDlgMRIWrap()
{
	
	
	
}


void CDlgMRIWrap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMRIWrap)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMRIWrap, CDialog)
//{{AFX_MSG_MAP(CDlgMRIWrap)
ON_MESSAGE(WM_IMRIUL_MSG, OnImriULMsg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMRIWrap message handlers

BOOL CDlgMRIWrap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pImriSrv=CImriUlSrv::GetHandle();
	InitWrap();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgMRIWrap::InitWrap()
{
	CConfigMgr conf;
	
	m_bEnableMRICtrl = m_pDlgMain->m_bEnableMRICtrl;
	m_sCurModalityID = conf.Read(SYSINFO, SYS_LAST_MODALITY, "001");
	
	m_bMRIConneted = FALSE;
	m_bProtocolsReceived = FALSE;
	
	m_iCurPatientUID = -1;
	m_iCurStudyUID = -1;
	
		
	memset((void*)&m_CurPatient, 0, sizeof(IGT_PatientField));
	memset((void*)&m_CurStudy, 0, sizeof(IMRI_STUDY));
	
	m_bEnableGradCorrect = conf.Read(GRADIENT, GRADIENT_ENABLE, 0);
	m_sGradFile = conf.Read(GRADIENT,GRADIENT_REFDATA,"");
	if(m_sGradFile=="")
	{
		m_bEnableGradCorrect=FALSE;
	}
	if(m_bEnableGradCorrect)
	{		
		m_bEnableGradCorrect=m_GradCorrecter.Init(m_sGradFile);
	}

	return TRUE;
}

BOOL CDlgMRIWrap::InitMRI()
{
	CDlgWizard *pDlgWizard = &m_pDlgMain->m_dlgWizard;
	
	if (m_bEnableMRICtrl)
	{
		
		m_bInitMRIConnection = TRUE;
		pDlgWizard->SetStatusMsg("Connecting MRI...");
		if (!ConnectMRI())
		{
			pDlgWizard->SetStep(WSTEP_PAT, WSRET_ERR);
			return FALSE;
		}
			
		
		pDlgWizard->SetStatusMsg("Accepting modality information...");
		m_pImriSrv->SendInfoRequest(INFO_MODALITY);
		CLog::Log("请求Modality信息");
		pDlgWizard->SetStatusMsg("Accepting protocol family information...");
		m_pImriSrv->SendInfoRequest(INFO_PROTOCAL_FAMILY);
		CLog::Log("请求协议族信息");
		pDlgWizard->SetStatusMsg("Accepting shimming information...");
		m_pImriSrv->SendInfoRequest(INFO_SHIM_SNR);
		CLog::Log("请求匀场信噪比信息");
		
		
	}
	else
	{
		pDlgWizard->SetStep(WSTEP_PAT, WSRET_OK);
	}
	return TRUE;
}

BOOL CDlgMRIWrap::ConnectMRI()
{
	if(m_bMRIConneted)
		return m_bMRIConneted;

	m_bMRIConneted = FALSE;
	
	CConfigMgr conf;
	BYTE byIP[4];
	byIP[0] = conf.Read(SYSINFO, SERVER_IP_1, 127);
	byIP[1] = conf.Read(SYSINFO, SERVER_IP_2, 0);
	byIP[2] = conf.Read(SYSINFO, SERVER_IP_3, 0);
	byIP[3] = conf.Read(SYSINFO, SERVER_IP_4, 1);
	CString sIPAddr;
	sIPAddr.Format("%d.%d.%d.%d", byIP[0], byIP[1], byIP[2], byIP[3]);
	
	int iPort = conf.Read(SYSINFO, SERVER_PORT, 8000);
	
	
	// Connect to the MRI server
	CString sLog;
	sLog.Format("Connecting MRI at %s:%d", sIPAddr, iPort);
	CLog::Log(sLog);
	
	g_DlgMain->BeginWaitCursor();
	CString sMsg;
	
		int nRetVal = m_pImriSrv->ConnectServer(sIPAddr.GetBuffer(1), iPort, m_hWnd);
		m_pImriSrv->SetAppImp(this);
		switch( nRetVal ) {
		case -1:
			sMsg = "Proper library of socket 2.0 is not installed.";
			break;
		case -2:
			sMsg = "Failed to create socket.";
			break;
		case -4:
			sMsg = "Failed to connect to MRI server.";
			break;
		case -21:
			sMsg = "Failed to send negoaite data.";
		case -31:
			sMsg = "Failed to allocate buffer.";
			break;
		default:
			sMsg = "Connect MRI OK.";
			m_bMRIConneted = TRUE;
			break;
		}
	
	g_DlgMain->EndWaitCursor();
	CLog::Log(sMsg);
	if (!m_bMRIConneted)
	{
		PanaMessageBox("Fail to connect MRI scanner. Please check hardware.\n\nError description: " + sMsg);
	}
		
	return m_bMRIConneted;
}

//从设备坐标系转换到病人坐标系
/*
Input:vCenter vFreqDir vPhaseDir 位于设备坐标系
output:vCenter vFreqDir vPhaseDir 位于病人坐标系
*/
void CDlgMRIWrap::ConvertCoordFromDevice2Patient(BYTE iPatPos, BYTE iPatEntry,double *vCenter, double  *vFreqDir, double *vPhaseDir)
{
	
	double dImgCenterX,dImgCenterY,dImgCenterZ;
	double dBIDirec[9],dX[3],dY[3];
		
	dImgCenterX=vCenter[0];
	dImgCenterY=vCenter[1];
	dImgCenterZ=vCenter[2];
	dX[0]=vFreqDir[0];
	dX[1]=vFreqDir[1];
	dX[2]=vFreqDir[2];
	dY[0]=vPhaseDir[0];
	dY[1]=vPhaseDir[1];
	dY[2]=vPhaseDir[2];

	
	
	//
	if (iPatEntry == beHeadFirst)
	{
		switch (iPatPos)
		{
		case bpProne:			
			dBIDirec[0] = -dImgCenterX ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] = -dImgCenterZ ;
			dBIDirec[3] = -dX[0];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] = -dX[2];
			dBIDirec[6] = -dY[0];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] = -dY[2];
			break ;
		case bpDecubitusRight:
			dBIDirec[0] =  dImgCenterZ ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] = -dImgCenterX ;
			dBIDirec[3] =  dX[2];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] = -dX[0];
			dBIDirec[6] =  dY[2];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] = -dY[0];
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] = -dImgCenterZ ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] =  dImgCenterX ;
			dBIDirec[3] = -dX[2];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] =  dX[0];
			dBIDirec[6] = -dY[2];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] =  dY[0];
			break ;
		case bpSupine:
		default:
			dBIDirec[0] =  dImgCenterX ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] =  dImgCenterZ ;
			dBIDirec[3] = dX[0];
			dBIDirec[4] = dX[1];
			dBIDirec[5] = dX[2];
			dBIDirec[6] = dY[0];
			dBIDirec[7] = dY[1];
			dBIDirec[8] = dY[2];
			break ;
		}
	}
	else
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] =  dImgCenterX ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] = -dImgCenterZ ;
			dBIDirec[3] =  dX[0];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] = -dX[2];
			dBIDirec[6] =  dY[0];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] = -dY[2];
			break ;
		case bpDecubitusRight:
			dBIDirec[0] =  dImgCenterZ ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] =  dImgCenterX ;
			dBIDirec[3] =  dX[2];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] =  dX[0];
			dBIDirec[6] =  dY[2];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] =  dY[0];
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] = -dImgCenterZ ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] = -dImgCenterX ;
			dBIDirec[3] = -dX[2];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] = -dX[0];
			dBIDirec[6] = -dY[2];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] = -dY[0];
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = -dImgCenterX ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] =  dImgCenterZ ;
			dBIDirec[3] = -dX[0];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] =  dX[2];
			dBIDirec[6] = -dY[0];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] =  dY[2];
			break ;
			
			
		}
	}
	vCenter[0]=dBIDirec[0];
	vCenter[1]=dBIDirec[1];
	vCenter[2]=dBIDirec[2];
	vFreqDir[0]=dBIDirec[3];
	vFreqDir[1]=dBIDirec[4];
	vFreqDir[2]=dBIDirec[5];
	vPhaseDir[0]=dBIDirec[6];
	vPhaseDir[1]=dBIDirec[7];
	vPhaseDir[2]=dBIDirec[8];
}
void CDlgMRIWrap::SendMRIScannerLocalization(Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir)
{
	//需要传送给prospect基于病人坐标系的方位
	//之前版本传送的是基于磁共振坐标系的方位
	if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
	{
		double dCenter[3];
		double dFreq[3],dPhase[3];
		dCenter[0]=vCenter.x;
		dCenter[1]=vCenter.y;
		dCenter[2]=vCenter.z;
		dFreq[0]=vFreqDir.x;
		dFreq[1]=vFreqDir.y;
		dFreq[2]=vFreqDir.z;
		dPhase[0]=vPhaseDir.x;
		dPhase[1]=vPhaseDir.y;
		dPhase[2]=vPhaseDir.z;
		ConvertCoordFromDevice2Patient(m_CurStudy.PatPosition,m_CurStudy.PatEntry,dCenter,dFreq,dPhase);
		vCenter.x=dCenter[0];
		vCenter.y=dCenter[1];
		vCenter.z=dCenter[2];
		vFreqDir.x=dFreq[0];
		vFreqDir.y=dFreq[1];
		vFreqDir.z=dFreq[2];
		vPhaseDir.x=dPhase[0];
		vPhaseDir.y=dPhase[1];
		vPhaseDir.z=dPhase[2];
	}
	vCenter.x=30;
	vCenter.y=20;
	vCenter.z=10;
	vPhaseDir.x=-1;
	vPhaseDir.y=-1;
	vPhaseDir.z=1;
	vFreqDir.x=1;
	vFreqDir.y=1;
	vFreqDir.z=2;
/*
	vCenter.x=1;
	vCenter.y=0;
	vCenter.z=0;
	vPhaseDir.x=0;
	vPhaseDir.y=1;
	vPhaseDir.z=0;
	vFreqDir.x=0;
	vFreqDir.y=0;
	vFreqDir.z=1;

*/
	vPhaseDir.SetUnit();
	vFreqDir.SetUnit();
		
	
		IMRI_LOCALIZATION *pckLoc2 = new IMRI_LOCALIZATION;
		MRIScanOptions *pScanOption=&m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_ScanOption;


		if(pScanOption->iOrient!=3 && pScanOption->iReferPos==1)
		{
			vCenter.x=0;
			vCenter.y=0;
			vCenter.z=0;
		}
		
		
		if(pScanOption->iReferPos==0 && pScanOption->iOrient==0)
		{
		pckLoc2->nPlane=2; //cor
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=1;
			vFreqDir.y=0;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=1;
			vPhaseDir.z=0;
		}
		}else if(pScanOption->iReferPos==0 && pScanOption->iOrient==1)
		{
		pckLoc2->nPlane=3; //sag
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=0;
			vFreqDir.y=1;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=0;
			vPhaseDir.z=1;
		}
		}else if(pScanOption->iReferPos==0 && pScanOption->iOrient==2)
		{
		pckLoc2->nPlane=1; //axi
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=1;
			vFreqDir.y=0;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=0;
			vPhaseDir.z=1;
		}
		}else if(pScanOption->iReferPos==1 && pScanOption->iOrient==0)
		{
		pckLoc2->nPlane=5;//cor
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=1;
			vFreqDir.y=0;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=1;
			vPhaseDir.z=0;
		}
		}else if(pScanOption->iReferPos==1 && pScanOption->iOrient==1)
		{
		pckLoc2->nPlane=6; //sag
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=0;
			vFreqDir.y=1;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=0;
			vPhaseDir.z=1;
		}
		}else if(pScanOption->iReferPos==1 && pScanOption->iOrient==2)
		{
		pckLoc2->nPlane=4; //axi
		if(m_pImriSrv->GetIMRIACPHiVer()==2 && m_pImriSrv->GetIMRIACPLoVer()==3)
		{
			pckLoc2->nPlane=0;
			vFreqDir.x=1;
			vFreqDir.y=0;
			vFreqDir.z=0;
			vPhaseDir.x=0;
			vPhaseDir.y=0;
			vPhaseDir.z=1;
		}
		}else
		{
			pckLoc2->nPlane=0;
		}

	pckLoc2->fCenterX = (float) vCenter.x;
	pckLoc2->fCenterY = (float) vCenter.y;
	pckLoc2->fCenterZ = (float) vCenter.z;
	pckLoc2->fPhaseX = (float) vPhaseDir.x;
	pckLoc2->fPhaseY = (float) vPhaseDir.y;
	pckLoc2->fPhaseZ = (float) vPhaseDir.z;
	pckLoc2->fFreqX = (float) vFreqDir.x;
	pckLoc2->fFreqY = (float) vFreqDir.y;
	pckLoc2->fFreqZ = (float) vFreqDir.z; 
	pckLoc2->nFovY = pckLoc2->nFovX = pckLoc2->nResolutionZ = 0;
	
		pckLoc2->fThickness=pScanOption->fThickness;
		pckLoc2->fGap=pScanOption->fGap;
		pckLoc2->iSlice=pScanOption->iSlice;
		pckLoc2->iNex=pScanOption->iNex;

	pckLoc2->nFovX=200;
	pckLoc2->nFovY=200;
	
		CList<IMRI_LOCALIZATION*, IMRI_LOCALIZATION*> lstLocs;
		lstLocs.AddTail(pckLoc2);
		m_pImriSrv->SendLocalization(lstLocs);
		delete pckLoc2;
	
}


BOOL CDlgMRIWrap::InsertImgToDB(CMRISeries *pMRISeries)
{
	if (pMRISeries)
	{
		CString sLog;
		sLog.Format("Receive MRI series: %s", pMRISeries->GetName());
		CLog::Log(sLog);
		
		if (m_iCurPatientUID <= 0)
		{
			PanaMessageBox("Warning: Patient information is not defined. MR image will not be inserted to database.");
			return FALSE;
		}
		
		// Add MRI study to database if it is not inside
		CString sStudyID = pMRISeries->GetStudyID();		
		int iUID = m_DBmri.ExistStudy(sStudyID, m_sCurModalityID);
		if (iUID <= 0)
		{
			iUID = m_DBmri.AddStudy(pMRISeries->GetImageAt(0)->GetIGTStudyField(), m_iCurPatientUID, sStudyID, m_sCurModalityID);
		}
		if (iUID <= 0)
		{
			PanaMessageBox("Error in inserting study data to database");
			return FALSE;
		}
		m_iCurStudyUID = iUID;
		
		// Add MRI series to database
		if (m_DBmri.ExistSeries(pMRISeries->GetSeriesID(), m_iCurStudyUID) <= 0)
		{
			int iUSID = m_DBmri.AddSeries(pMRISeries->GetImageAt(0)->GetIGTSeriesField(), m_iCurStudyUID);
			if (iUSID <= 0)
			{
				sLog.Format("Insert %s to database Error", pMRISeries->GetName());
				CLog::Log(sLog);
				PanaMessageBox(sLog);
			}
			else
			{
				sLog.Format("Insert %s to database OK", pMRISeries->GetName());
				CLog::Log(sLog);
			}
		}
	}
	return TRUE;
}


LRESULT CDlgMRIWrap::OnImriULMsg(UINT wParam,LONG lParam)
{
	switch (wParam)
	{
	case 0:
		break;
	case 10:     //peer socket has closed.
		{
			m_bMRIConneted = FALSE;
			CString csMsg = "Connection to MRI scanner lost! Please check it.";
			CLog::Log(csMsg);
			m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(csMsg);
			//PanaMessageBox("Connection to MRI scanner lost! Please check it.");
			m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.UpdateSelectedProtocol(NULL, NULL);
			m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.StopScanProgress();
		}
		break;
	case 20:    //A user has connected to server.
		break ;
	}
	return 0;
}

int CDlgMRIWrap::Imri_DecodeImages(LPBYTE lpBuff, int nDataLen)
{
	
	m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.StopScanProgress();
	CMRISeries *pMRISeries = new CMRISeries;
	if ( ERR_OK != pMRISeries->Load( (char*)lpBuff, nDataLen, m_pImriSrv->GetIMRIACPHiVer(), m_pImriSrv->GetIMRIACPLoVer() ) )
	{
		delete pMRISeries;
		pMRISeries = NULL;
		return 1;
	}

	if(pMRISeries->GetNumOfImages()<=0)
	{
		delete pMRISeries;
		return 0;
	}
	
	if(pMRISeries->GetImageAt(0)->GetImageType()==IMS_XINAO && m_bEnableGradCorrect==TRUE)
	{
		try
		{
			m_GradCorrecter.GradientCorrection(pMRISeries);
		}catch(...)
		{
			//
		}
	}
		
	m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_iCntCycleScan ++;
	InsertImgToDB(pMRISeries);
	m_pDlgMain->m_dlgCryotherapy.LoadMRISeries(pMRISeries);
	
	return 0;
}

int CDlgMRIWrap::Imri_DealCriticalMsg(LPBYTE lpBuff)
{
	CString sMsg = lpBuff;
	CLog::Log(sMsg);
	PanaMessageBox(sMsg);
	return 0;
}

int CDlgMRIWrap::Imri_DealConfirmMsg(LPBYTE lpBuff)
{
	CString sMsg = lpBuff;
	CLog::Log(sMsg);
	
	m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(sMsg);
	
	
	return 0;
}

int CDlgMRIWrap::Imri_DealWarningMsg(LPBYTE lpBuff) 
{
	CString sMsg = lpBuff;
	CLog::Log(sMsg);
	m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(sMsg);
	return 0;
}

int CDlgMRIWrap::Imri_DealErrorMsg(LPBYTE lpBuff) 
{
	CString sMsg = lpBuff;
	CString sTmp;
	sTmp.Format("Message from MRI:\n%s",sMsg);
	sMsg=sTmp;

	CLog::Log(sMsg);
	m_pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage(sMsg);
	PanaMessageBox(sMsg);
	return 0;
}

int CDlgMRIWrap::Imri_DealStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
								 unsigned short nSeriesStatus, int nOperResult) 
{
	m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.UpdateMRIScannerControlUI2(nSysStatus, nSeriesStatus, nOperResult);
	
	if (m_pDlgMain->m_pBaseCaliDlg 
		&& m_pDlgMain->m_pBaseCaliDlg->GetSafeHwnd()
		&& m_pDlgMain->m_pBaseCaliDlg->IsWindowVisible())
	{
		m_pDlgMain->m_pBaseCaliDlg->RespondMRIStatus(nSysStatus, nSeriesStatus, nOperResult);
	}

	if (m_pDlgMain->m_dlgWizard.m_dlgMRIPatInfo 
		&& m_pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.GetSafeHwnd()
		&& m_pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.IsWindowVisible())
	{
		m_pDlgMain->m_dlgWizard.m_dlgMRIPatInfo.RespondMRIStatus(nSysStatus, nSeriesStatus, nOperResult);
	}
	return 0;
}

int CDlgMRIWrap::Imri_DealShimSNR(ENUM_INFOR_RESULT eRet, IMRI_SHIM_SNR_TUNING_INFO *pShimSNR)
{
	return 0;
}

int  CDlgMRIWrap::Imri_DealProtocolFamily(ENUM_INFOR_RESULT eRet, CList<IMRI_PROTOCOL_FAMILY, IMRI_PROTOCOL_FAMILY> &lstprotocolGrp)
{
	if (eRet != IR_OK)
	{
		PanaMessageBox("Can't find protocol family information, please contact supplier.");
		m_pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_ERR);
		return 1;
	}

	m_lstProtocolFamily.RemoveAll();
	POSITION pos=lstprotocolGrp.GetHeadPosition();
	while(pos)
	{
		m_lstProtocolFamily.AddTail(lstprotocolGrp.GetNext(pos));
	}
	return 0;
}
int CDlgMRIWrap::Imri_DealProtocolGrps(enum ENUM_INFOR_RESULT eResult, CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*> &lstprotocolGrp)
{
	CDlgWizard *pDlgWizard = &m_pDlgMain->m_dlgWizard;

	pDlgWizard->SetStatusMsg("");
	if (eResult != IR_OK)
	{
		PanaMessageBox("Can't load protocols, please contact supplier.");
		return 1;
	}
	
	
	CLog::Log("Receive protocols");
	
	m_pDlgMain->m_dlgCryotherapy.m_ProtMng.CleanBdyPrtLst();
	POSITION pos = lstprotocolGrp.GetHeadPosition();
	for (;pos;)
	{
		IMRI_PROTOCOL_GROUP *pProtocolGrp = lstprotocolGrp.GetNext(pos);
		m_pDlgMain->m_dlgCryotherapy.m_ProtMng.AddBodyPartProtocols((IMRI_PROTOCOL_GROUP*)pProtocolGrp);
	}
	
	if (m_pDlgMain->m_dlgCryotherapy.IsWindowVisible())
	{
		m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.CreateProtocolMenu(m_pDlgMain->m_dlgCryotherapy.m_ProtMng.GetBdyPrtLst());
	}
	else
	{
		m_bProtocolsReceived = TRUE;
	}
	
	m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.SetMRIScannerControlUI(US_READY_TO_CHOOSE_PROTOCOL);
	
	if (m_bInitMRIConnection)
	{
		m_bInitMRIConnection = FALSE;
	}
	
	ShowWaiting(-1,""); //关闭提示框
	
	return 0;
}

int CDlgMRIWrap::Imri_DealModality(enum ENUM_INFOR_RESULT eResult, IMRI_MODALITY &modality) 
{
	if (eResult != IR_OK)
	{
		PanaMessageBox("Don't find modality information, please contact supplier.");
		m_pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_ERR);
		return 1;
	}
	
	
	m_sCurModalityID = modality.cId;
	
	CString sID;
	sID.Format("%s", modality.cId);
	CString sName;
	sName.Format("%s", modality.cName);
	CString sType;
	sType.Format("%s", modality.cType);
	CString sDesc;
	sDesc.Format("%s", modality.cDesc);
	int iMUID = m_DBmri.ExistModality(sID);
	if (iMUID <= 0)
	{
		iMUID = m_DBmri.AddModality(sID, sName, sType, sDesc);
	}
	if (iMUID <= 0)
	{
		PanaMessageBox("Error in inserting modality data to database");
		return 1;
	}
	
	CConfigMgr conf;
	conf.Write(SYSINFO, SYS_LAST_MODALITY, m_sCurModalityID);
	
	CString sLog;
	sLog.Format("MRI modality ID: %s", modality.cId);
	
	CLog::Log(sLog);
	
	m_pDlgMain->m_dlgWizard.SetStep(WSTEP_PAT, WSRET_OK); 
	
	return 0;
}

int CDlgMRIWrap::Imri_DealPatient(enum ENUM_INFOR_RESULT eResult, IMRI_PATIENT &patient) 
{
	CDlgWizard *pDlgWizard = &m_pDlgMain->m_dlgWizard;

	pDlgWizard->SetStatusMsg("");
	if (eResult == IR_NO_PATIENT)
	{
		memset(&m_CurPatient,0,sizeof(IGT_PatientField));
		if (m_pDlgMain->m_dlgWizard.GetSafeHwnd())
		{
			m_pDlgMain->m_dlgWizard.ShowMRIPatientConfirmDlg(NULL, NULL,m_sCurModalityID);
		}
		return 0;
	}
	
	MR_PATIENT_FIELD patInfor;
	memcpy(&patInfor, (void*)&patient, sizeof(MR_PATIENT_FIELD));
		 
	/*转换成IGT格式 ningsj 2009.3.4*/
	IGT_PatientField IGTPatientInfo;
	IGTPatientInfo.Age = patInfor.Age;
	IGTPatientInfo.Sex = patInfor.Sex;
	IGTPatientInfo.Weight = patInfor.Weight;
	IGTPatientInfo.BirthDate.year = patInfor.BirthDate.year;
	IGTPatientInfo.BirthDate.month = patInfor.BirthDate.month;
	IGTPatientInfo.BirthDate.day = patInfor.BirthDate.day;
	IGTPatientInfo.BirthDate.hour = patInfor.BirthDate.hour;
	IGTPatientInfo.BirthDate.minute = patInfor.BirthDate.minute;
	IGTPatientInfo.BirthDate.second = patInfor.BirthDate.second;
	sprintf(IGTPatientInfo.Id , "%s" , patInfor.Id);
	sprintf(IGTPatientInfo.Name , "%s" , patInfor.Name);
	sprintf(IGTPatientInfo.Desc , "%s" , patInfor.Desc);

	memcpy(&m_CurPatient,&IGTPatientInfo,sizeof(IGT_PatientField));

	/*接受到Study信息后,把Patient and study信息一起显示*/
	pDlgWizard->SetStatusMsg("Accepting study information...");
	m_pImriSrv->SendInfoRequest(INFO_STUDY);
	CLog::Log("请求Study信息");
	return 0;
}

int CDlgMRIWrap::Imri_DealStudy(enum ENUM_INFOR_RESULT eResult, IMRI_STUDY &study) 
{
	CDlgWizard *pDlgWizard = &m_pDlgMain->m_dlgWizard;

	pDlgWizard->SetStatusMsg("");

	if (eResult == IR_NO_STUDY)
	{
		memset(&m_CurStudy,0,sizeof(IMRI_STUDY));
		if (m_pDlgMain->m_dlgWizard.GetSafeHwnd())
		{
			m_pDlgMain->m_dlgWizard.ShowMRIPatientConfirmDlg(&m_CurPatient, NULL,m_sCurModalityID);
		}
		return 0;
	}
	
	
	memcpy(&m_CurStudy,&study,sizeof(IMRI_STUDY));

	/*接受到Study信息后,把Patient and study信息一起显示*/
	m_pDlgMain->m_dlgWizard.ShowMRIPatientConfirmDlg(&m_CurPatient, &m_CurStudy,m_sCurModalityID);
	return 0;
}

int CDlgMRIWrap::SendIMRICommand(ENUM_COMMANDS eCommand, IMRI_PROTOCOL *pProtocol /* = NULL */)
{
	if(!m_bMRIConneted)
		return 1;

	if (eCommand == COMMAND_CREATESERIES)
	{
		IMRI_COMMAND_LOADPROTOCOL par;
		par.nReadyMode = (unsigned short)PACKET_READY_MODE_NAVIGATION;
		if (pProtocol->pBelongGrp)
		{
			par.protocolGroup.nProtocolGroupNo = pProtocol->pBelongGrp->nID;
			memcpy(par.protocolGroup.cProtocolGroupDesc, pProtocol->pBelongGrp->sDesc, 64);
		}
		memcpy(&(par.protocol), &(pProtocol->oProtDesc), sizeof(IMRI_PROTOCOL_DESC));
		return m_pImriSrv->SendCommand(eCommand, (void*)&par, sizeof(par));
	}
	else if (eCommand == COMMAND_STARTLOOPSCAN)
	{
		unsigned short siCycleScanGap = m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.m_ScanOption.iInterval;
		return m_pImriSrv->SendCommand(eCommand, (void*)&siCycleScanGap, 2);
	}
	else
	{
		if(eCommand==COMMAND_STARTPRESCANSCAN || eCommand==COMMAND_STARTPRESCAN)
		{
			//临时测试使用 nsj 2011.5.20
			Vector3D vCenter, vPhaseDir, vFreqDir;
			SendMRIScannerLocalization(vCenter, vPhaseDir, vFreqDir);
			//结束

			if(m_pDlgMain->m_dlgCryotherapy.m_dlgCmdBar.m_bTuneInNextScan) //判断是否需要进行调谐
			{
				m_pImriSrv->SendCommand(COMMAND_STARTTUNE);
				m_pDlgMain->m_dlgCryotherapy.m_dlgCmdBar.m_bTuneInNextScan=FALSE;
			}
		}

		
		return m_pImriSrv->SendCommand(eCommand);
		
		
	}
}


void CDlgMRIWrap::ChangeProtocolFamily(IMRI_PROTOCOLGROUP_DESC proFamily)
{
	//避免改变协议族的时协议没有及时更新
	m_pDlgMain->m_dlgCryotherapy.m_dlgScannerControl.SetMRIScannerControlUI(US_NO_MRI);

	m_pImriSrv->SendCommand(COMMAND_CHANGEPROTOCOL, (void*)&proFamily, sizeof(IMRI_PROTOCOLGROUP_DESC));


}

void CDlgMRIWrap::CalcDirection(MRIScanOptions *pScanOption, Vector3D &vPhaseDir, Vector3D &vFreqDir)
{
	if(pScanOption->iOrient==3)
	{
		if(!pScanOption->bNewOrient)
			return;

		Vector3D v1(1,0,0);
		Vector3D v2(0,1,0);
		Vector3D v3(0,0,1);
		Vector3D vUse(0,0,1);
		Vector3D vTmp(0,0,1);

		//计算向量夹角
		double d1=GetAngle(vPhaseDir,v1);
		double d2=GetAngle(vPhaseDir,v2);
		double d3=GetAngle(vPhaseDir,v3);
		if(d1>90)
		{
			d1=180-d1;
		}

		if(d2>90)
		{
			d2=180-d2;
		}

		if(d3>90)
		{
			d3=180-d3;
		}

		if(d1<=d2 && d1<=d3)
		{
			vUse.x=v1.x; vUse.y=v1.y; vUse.z=v1.z;
		}else if(d2<=d1 && d2<=d3)
		{
			vUse.x=v2.x; vUse.y=v2.y; vUse.z=v2.z;
		}
		if(d3<=d1 && d3<=d2)
		{
			vUse.x=v3.x; vUse.y=v3.y; vUse.z=v3.z;
		}
		Vector3D vSlice= vPhaseDir ^ vFreqDir;
		vTmp= vSlice ^ vUse;  //临时vFreqDir
		if(GetAngle(vTmp,vFreqDir)<90)
		{
			vFreqDir.x=vTmp.x;
			vFreqDir.y=vTmp.y;
			vFreqDir.z=vTmp.z;
		}else
		{
			vFreqDir.x=-vTmp.x;
			vFreqDir.y=-vTmp.y;
			vFreqDir.z=-vTmp.z;
		}

		vTmp=vFreqDir ^ vSlice;  //临时vPhaseDir
		if(GetAngle(vTmp,vPhaseDir)<90)
		{
			vPhaseDir.x=vTmp.x;
			vPhaseDir.y=vTmp.y;
			vPhaseDir.z=vTmp.z;
		}
		return;
	}
	
	//if ( beHeadFirst == m_studyInfo.PatEntry )
	{
		switch( m_CurStudy.PatPosition )
		{
		case bpSupine: //仰卧
		case bpProne:  //俯卧
			{
				if(pScanOption->iOrient==0)  //cor
				{
					vPhaseDir.x=1;
					vPhaseDir.y=0;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=1;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==1)  //sag
				{
					vPhaseDir.x=0;
					vPhaseDir.y=0;
					vPhaseDir.z=1;
					vFreqDir.x=0;
					vFreqDir.y=1;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==2)  //axi
				{
					vPhaseDir.x=0;
					vPhaseDir.y=0;
					vPhaseDir.z=1;
					vFreqDir.x=1;
					vFreqDir.y=0;
					vFreqDir.z=0;
				}
			}
			break;
		case bpDecubitusRight:
		case bpDecubitusLeft:
			{
				if(pScanOption->iOrient==0)  //cor
				{
					vPhaseDir.x=0;
					vPhaseDir.y=0;
					vPhaseDir.z=1;
					vFreqDir.x=0;
					vFreqDir.y=1;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==1)  //sag
				{
					vPhaseDir.x=1;
					vPhaseDir.y=0;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=1;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==2)  //axi
				{
					vPhaseDir.x=1;
					vPhaseDir.y=0;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=0;
					vFreqDir.z=1;
				}
			}
			break;
		default:
			break;
		}
	} 
	/*else
	{
		switch( m_studyInfo.PatPosition )
		{
		case bpSupine: //仰卧
		case bpProne:  //俯卧
			{
				if(pScanOption->iOrient==0)  //cor
				{
					vPhaseDir.x=0;
					vPhaseDir.y=1;
					vPhaseDir.z=0;
					vFreqDir.x=1;
					vFreqDir.y=0;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==1)  //sag
				{
					vPhaseDir.x=0;
					vPhaseDir.y=1;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=0;
					vFreqDir.z=1;
				}
				
				if(pScanOption->iOrient==2)  //axi
				{
					vPhaseDir.x=0;
					vPhaseDir.y=0;
					vPhaseDir.z=1;
					vFreqDir.x=1;
					vFreqDir.y=0;
					vFreqDir.z=0;
				}
			}
			break;
		case bpDecubitusRight:
		case bpDecubitusLeft:
			{
				if(pScanOption->iOrient==0)  //cor
				{
					vPhaseDir.x=0;
					vPhaseDir.y=1;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=0;
					vFreqDir.z=1;
				}
				
				if(pScanOption->iOrient==1)  //sag
				{
					vPhaseDir.x=0;
					vPhaseDir.y=1;
					vPhaseDir.z=0;
					vFreqDir.x=1;
					vFreqDir.y=0;
					vFreqDir.z=0;
				}
				
				if(pScanOption->iOrient==2)  //axi
				{
					vPhaseDir.x=1;
					vPhaseDir.y=0;
					vPhaseDir.z=0;
					vFreqDir.x=0;
					vFreqDir.y=0;
					vFreqDir.z=1;
				}
			}
			break;
		default:
			break;
		}
	}*/

	if(pScanOption->bSwap)
	{
		float x=vPhaseDir.x;
		float y=vPhaseDir.y;
		float z=vPhaseDir.z;
		vPhaseDir.x=vFreqDir.x;
		vPhaseDir.y=vFreqDir.y;
		vPhaseDir.z=vFreqDir.z;

		vFreqDir.x=x;
		vFreqDir.y=y;
		vFreqDir.z=z;
	}

}

double CDlgMRIWrap::GetAngle(Vector3D &v1, Vector3D &v2)
{
	//得到两向量的点积 
	double dotProduct = v1 * v2;

	//得到两向量长度的乘积 
	double vectorsMagnitude = v1.GetLen() * v2.GetLen(); 

	//得到两向量夹角 
	double angle = acos( dotProduct / vectorsMagnitude ); 

	//返回角度值 
	angle=(angle/PI)*180;
	return( angle ); 
}

