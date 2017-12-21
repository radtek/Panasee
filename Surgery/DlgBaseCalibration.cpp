// DlgBaseCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgBaseCalibration.h"
#include "ConfigMgr.h" //Add by Dai Liang 2007.6.5
#include "CryotherapyDlg.h"//Add by Dai Liang 2007.6.5
#include "CommandHandling.h"//Add by Dai Liang 2007.6.5
#include "ErrorCodes.h"
#include "DlgWizard.h"
#include "DlgManualBaseCali.h"
#include "Config.h"
#include "header.h"
#include "DlgMRIWrap.h"
#include "DlgMain.h"
#include "Log.h"
#include "ShowImage.h"
#include "globalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBaseCalibration dialog


CDlgBaseCalibration::CDlgBaseCalibration(CWnd* pParent /*=NULL*/)
: CPanaseeDialog(CDlgBaseCalibration::IDD, pParent)
{
	m_iStep=1;
	m_iNumOfImage = 0;		//Add by Dai Liang 2007.6.5
	
	CConfigMgr conf;
	m_dbCaliThreshold = conf.Read(CALI_SET, CALI_THRESHOLD, 1.5);
	
	m_pCryoDlg = NULL;
}


void CDlgBaseCalibration::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBaseCalibration)
	DDX_Control(pDX, IDC_BTN_PHANTOM_INDICATOR, m_btnPhantom);
	DDX_Control(pDX, IDC_BTN_BASE_INDICATOR, m_btnBase);
	DDX_Control(pDX, IDC_PROGRESS_CALI_SCAN, m_ctlProgressCaliScan);
	DDX_Control(pDX, IDC_PROGRESS_CALI, m_ctlProGressCalibrate);
	DDX_Control(pDX, ID_CALI_START, m_btnStart);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBaseCalibration, CPanaseeDialog)
//{{AFX_MSG_MAP(CDlgBaseCalibration)
ON_WM_CLOSE()
ON_MESSAGE(WM_NEW_SERIES, OnMsgNewSeries)
ON_BN_CLICKED(ID_CALI_START, OnCaliStart)
ON_WM_SHOWWINDOW()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define PROGRESS_MAX 500

/////////////////////////////////////////////////////////////////////////////
// CDlgBaseCalibration message handlers

BOOL CDlgBaseCalibration::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	
	EnableSeprateLine(TRUE);
	
	CPanaseeDialog::OnInitDialog();
	
	//设置UI
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnStart);
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	
	
	CRect rcTmp,rcClient;
	GetClientRect(&rcClient);
	m_ctlProGressCalibrate.GetWindowRect(&rcTmp);
	ScreenToClient(&rcTmp);
	rcTmp.left=15;
	rcTmp.right=rcClient.right-15;
	rcTmp.bottom=rcTmp.top+27;
	m_ctlProGressCalibrate.MoveWindow(&rcTmp);
	
	m_ctlProgressCaliScan.GetWindowRect(&rcTmp);
	ScreenToClient(&rcTmp);
	rcTmp.left=15;
	rcTmp.right=rcClient.right-15;
	rcTmp.bottom=rcTmp.top+27;
	m_ctlProgressCaliScan.MoveWindow(&rcTmp);
	
	
	if(m_iStep==1)
	{
		GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NOTE2)->ShowWindow(SW_HIDE);
	}
	
	//m_btnOK.SetThemeHelper(&m_ThemeHelper);
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	//m_btnStart.SetThemeHelper(&m_ThemeHelper);
	
	m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnBase.SizeToContent();
	m_btnPhantom.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnPhantom.SizeToContent();
	
	m_iScanTime = 78; // scanning time is 78s.
	
	//	m_ctlProGressCalibrate.ModifyStyle(NULL, PBS_SMOOTH);
	m_ctlProGressCalibrate.SetRange(0, PROGRESS_MAX);
	
	//	m_ctlProgressCaliScan.ModifyStyle(NULL, PBS_SMOOTH);
	m_ctlProgressCaliScan.SetRange(0, PROGRESS_MAX);
	m_iScanedTime = 0;
	
	m_pMRIWrap = &(g_DlgMain->m_MRIWrap);
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBaseCalibration::OnOK() 
{
	CDlgMain* pDlgMain = g_DlgMain;

	if (pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER &&
		pDlgMain->m_dlgCryotherapy.m_bMarkerVisible == FALSE) //NAVI MODE
	{
		PanaMessageBox("Patient Marker is not visible. Please reposition camera.");
		return;
	}

	CCryotherapyDlg* pCryDlg = &(pDlgMain->m_dlgCryotherapy);
	CConfigMgr conf;
	CString sFilePath;
	int nBaseIndex = conf.Read(CONF_BASE,CONF_BASE_INDEX,0);
	switch(nBaseIndex) 
	{
	case 0:
		//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE1,"0");
		sFilePath = CONFIG_FILES[CF_BASE1];
		break;
	case 1:
		//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE2,"0");
		sFilePath = CONFIG_FILES[CF_BASE2];
		break;
	case 2:
		//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE3,"0");
		sFilePath = CONFIG_FILES[CF_BASE3];
		break;
	case 3:
		//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE4,"0");
		sFilePath = CONFIG_FILES[CF_BASE4];
		break;			
	}	
	
	pDlgMain->m_dlgCryotherapy.m_tranScanner2Base = m_mBaseINScan.Inverse();
	pCryDlg->SaveTransformation(sFilePath,pCryDlg->m_tranScanner2Base.value);
	if (pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER) //NAVI MODE
	{
		Matrix tTrack2Marker = pDlgMain->m_dlgCryotherapy.m_tMarker.Inverse();
		pDlgMain->m_dlgCryotherapy.m_tranScanner2Marker = tTrack2Marker * pDlgMain->m_dlgCryotherapy.m_tBase * pDlgMain->m_dlgCryotherapy.m_tranScanner2Base;
	}
	
	pCryDlg->CreateTransform();
	pCryDlg->InitFrameBoxPosition();
	pCryDlg->TransformAllMRISeries();
	
	CString sLog;
	sLog.Format("标定结果保存至Base %d 对应文件:%s",nBaseIndex+1,sFilePath);				
	CLog::Log(sLog);
	
	CDialog::OnOK();
}

void CDlgBaseCalibration::OnCancel() 
{
	if (m_iStep > 1)
	{
		m_pMRIWrap->SendIMRICommand(COMMAND_STOPSCAN);
		
		
		CLog::Log("标定扫描过程未完成，原因：用户终止扫描");
		ShowMsg("Make sure that the phantom has been placed in MRI.\n\nPress \'Start\' to start scanning.\nPress \'Cancel\' to return the previous step.");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(ID_CALI_START)->EnableWindow(TRUE);
		m_iStep = 1;
		
		m_ctlProGressCalibrate.SetPos(0);
		m_ctlProgressCaliScan.SetPos(0);
		SetDlgItemText(IDC_LABL_CALI_TIME, "");
		m_iScanedTime = 0;
		
		if (!m_pCryoDlg->m_bTracking)
		{
			m_pCryoDlg->StartTracking();
			
		}
		
		return;
	}
	if (!m_pCryoDlg->m_bTracking)
	{
		m_pCryoDlg->StartTracking();
		
	}
	CDialog::OnCancel();
}

void CDlgBaseCalibration::OnClose() //Modified by Dai Liang 2007.6.5
{
	CDialog::OnClose();
}

LRESULT CDlgBaseCalibration::OnMsgNewSeries(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		CMRISeries* pMRISeries = (CMRISeries*)lParam;
		if (pMRISeries)
		{
			m_iNumOfImage ++;
			if (m_iNumOfImage < 3) 
			{
				m_pMRISeries[m_iNumOfImage-1] = pMRISeries;
				
				CConfigMgr con;
				CString sPart;
				CString sProtocal;
												
				void* pCurMRIProtocol;
				if (m_iNumOfImage==1)
				{
					sPart = con.Read(CALI_SET,CONF_PROTOCAL_COR_PART,"");
					sProtocal = con.Read(CALI_SET,CONF_PROTOCAL_COR_PROTOCAL,"");
					pCurMRIProtocol = m_pCryoDlg->m_ProtMng.SetCurrentProtocol(sPart,sProtocal);
					ShowMsg("Start scanning coronal, please wait...");
				}
				if (m_iNumOfImage==2)
				{
					sPart = con.Read(CALI_SET,CONF_PROTOCAL_SAG_PART,"");
					sProtocal = con.Read(CALI_SET,CONF_PROTOCAL_SAG_PROTOCAL,"");
					pCurMRIProtocol = m_pCryoDlg->m_ProtMng.SetCurrentProtocol(sPart,sProtocal);
					ShowMsg("Start scanning sagital, please wait...");
				}
				
				if (pCurMRIProtocol)
				{
					// ningsj 2009/3/1
					/*开始扫描关闭相机*/
					if(m_pCryoDlg->m_bTracking)
					{
						m_pCryoDlg->StopTracking();
						
					}
					m_pMRIWrap->SendIMRICommand(COMMAND_CREATESERIES,  (IMRI_PROTOCOL*)pCurMRIProtocol);
					
					CString sMsg;
					sMsg.Format("已向MRI发送标定命令，使用序列：部位 %s， 协议 %s",sPart,sProtocal);
					CLog::Log(sMsg);
				}
			} 
			else if (m_iNumOfImage == 3)
			{
				m_pMRISeries[m_iNumOfImage-1] = pMRISeries;
				OnScanFinished();
			}
			else if(m_iNumOfImage > 3)
			{
				CString sMsg;
				sMsg.Format("标定错误：扫描序列数量大于3，数量为%d\n", m_iNumOfImage);
				CLog::Log(sMsg);
				PanaMessageBox("Scanning error!");
			}
		}
	}
	
	return 0;
}

// This if for MRI communication protocol V2
void CDlgBaseCalibration::RespondMRIStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
										   unsigned short nSeriesStatus /*= 0*/, int nOperResult /*= 0*/)
{
	switch (nSysStatus)
	{
	case MSYS_IDLE:
		{
			switch (nSeriesStatus)
			{
			case 0x0000:
				break;
			case 0x0002:
				break;
			case 0x0004:
				break;
			case 0x0006:
				break;
			case 0x0008:	// 已经设置好待扫描序列的参数 后面紧接操作结果
				{
				}
				break;
			case 0x0009:	// 已经设置好待扫描序列的参数 接扫描时间预计
				{
					m_iEstScanTime = nOperResult * 1000;
					m_ctlProgressCaliScan.SetPos(0);
					m_iScanedTime = 0;
				}
				break;
			case 0x000A:	//序列处于等待扫描列队等待 后面紧接操作结果
				{
					if (m_pMRIWrap->m_pImriSrv->CheckOperResult(nOperResult))
					{
						m_pMRIWrap->SendIMRICommand(COMMAND_STARTPRESCANSCAN);
					}
					else
					{
						PanaMessageBox("Load series error, please check MRI scanner");
					}
				}
				break;
			case 0x000B:	// 序列处于等待扫描列队等待 接扫描时间预计
				{
					m_iEstScanTime = nOperResult * 1000;
				}
				break;
			case 0x000C:
				break;
			default:
				break;
			}
		}
		break;
	case MSYS_IDLE_TO_SCAN:
		{
			if (nSeriesStatus == 0x0000 || nSeriesStatus == 0x0001)
			{
				// Start prescan
				m_iScanedTime = 0;
			}
			else if (nSeriesStatus == 0x0002 || nSeriesStatus == 0x0003)
			{			
				m_iScanedTime = 0;
			}
		}
		break;
	case MSYS_SCAN_TO_PAUSE:
		{
		}
		break;
	case MSYS_PAUSE_TO_SCAN:
		{
		}
		break;
	case MSYS_SCANNING:
		{
			switch (nSeriesStatus)
			{
			case 0x0000:	// Prescanning
				break;
			case 0x0001:	// Prescanning
				break;
			case 0x0002:	// Scanning
				break;
			case 0x0003:	// Scanning
				{
					int iRcvedScanTime;
					if (nOperResult >= m_iScanedTime)
					{
						m_iScanedTime = nOperResult;
						iRcvedScanTime = nOperResult;
					}
					else
					{
						iRcvedScanTime = m_iScanedTime + nOperResult;
					}
					
					float fRate = iRcvedScanTime >= m_iEstScanTime ? 0.99 : ((float)iRcvedScanTime) / m_iEstScanTime;
					m_ctlProgressCaliScan.SetPos(PROGRESS_MAX * fRate);
					
					float fRate2 = (float)(m_iNumOfImage * m_iEstScanTime + iRcvedScanTime) / (m_iEstScanTime * 3.2);
					m_ctlProGressCalibrate.SetPos(PROGRESS_MAX * fRate2);
					
					CString sTimeLeft;
					int iTime = (int)(m_iEstScanTime * 3.2 - (m_iNumOfImage * m_iEstScanTime + iRcvedScanTime)) / 1000;
					if (iTime > 60)
					{
						sTimeLeft.Format("%d minutes left", iTime / 60 + (iTime % 60 > 0 ? 1 : 0));
					}
					else 
					{
						sTimeLeft.Format("%d seconds left", iTime);
					}
					SetDlgItemText(IDC_LABL_CALI_TIME, sTimeLeft);
				}
				break;
			default:
				break;
			}
		}
		break;
	case MSYS_SCAN_TO_IDLE:
		switch (nSeriesStatus)
		{
		case 0x0000:	// Finish prescan
			{
			}
			break;
		case 0x0002:	// Finish scan
			{
			}
			break;
		case 0x0004:	// Stop prescan normally
			{
			}
			break;
		case 0x0006:	// Stop scan normally
			{
			}
			break;
		case 0x0008:	// Stop prescan abnormally
			{
			}
			break;
		case 0x000A:	// Stop scan abnormally
			{
			}
			break;
		default:
			break;
		}
		break;
		default:
			break;
	}
}


void CDlgBaseCalibration::OnScanFinished()			
{
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &(g_DlgMain->m_dlgCryotherapy);
	// Huagen 2008/12/24
	//	if (pCryDlg->m_bTracking)
	//	{
	//		pCryDlg->StopTracking();
	//		
	//	}
	
	ShowMsg("Scanning Finished! Calculating ...");
	m_ctlProgressCaliScan.SetPos(PROGRESS_MAX);
	
	CDlgMain* pDlgMain;
	pDlgMain = g_DlgMain;
	
	CConfigMgr conf;
	//	CString sFilePath = conf.Read(CONF_PHANTOM, CONF_PHANTOM_GEOFILE, "0");
	CString sFilePath = CONFIG_FILES[CF_PHANTOM];
	m_mPhatomGeo.SetDimension(4,4);
	//	CCryotherapyDlg *pCryDlg;
	
	//	if (!pCryDlg->m_bTracking)
	//	{
	//		pCryDlg->StartTracking();
	//		
	//	}
	if (pCryDlg->LoadTransformation(sFilePath,m_mPhatomGeo.value))
	{
		CString sMsg;
		sMsg.Format("装载水模几何文件成功，文件路径：%s\n", sFilePath);
		CLog::Log(sMsg);
	}
	else
	{	
		CString sMsg;
		sMsg.Format("标定失败，原因：装载水模几何文件失败，水模文件路径：%s\n", sFilePath);
		CLog::Log(sMsg);
		g_DlgMain->EndWaitCursor();
		return;
	}
	
	m_mBaseINScan.SetDimension(4,4);
	
	Matrix mPhatomRefINTracker = pDlgMain->m_dlgCryotherapy.m_tPhatom;
	Matrix mBaseINTracker = pDlgMain->m_dlgCryotherapy.m_tBase;
	
	CString s;
	CLog::Log("PhatomRefINTracker:");	
	for(int i=0; i<4; i++)
	{
		s.Format("%f %f %f %f", mPhatomRefINTracker.value[i][0], mPhatomRefINTracker.value[i][1], mPhatomRefINTracker.value[i][2], mPhatomRefINTracker.value[i][3]);
		CLog::Log(s);
	}
	CLog::Log("BaseINTracker:");
	for(i=0; i<4; i++)
	{
		s.Format("%f %f %f %f", mBaseINTracker.value[i][0], mBaseINTracker.value[i][1], mBaseINTracker.value[i][2], mBaseINTracker.value[i][3]);
		CLog::Log(s);
	}
	
	// Set window width and level
	for (i=0; i<3; i++)
	{
		CMRISeries* pMRISeries = m_pMRISeries[i];
		MR_ImageHeader* pImageHeader = pMRISeries->GetImageAt(0)->GetImageHeader();
		CString sDesc = pImageHeader->seriesField.Desc;
		int iWidth = 4096;
		int iLevel = 2048;
		if (sDesc.Find("Axi") >= 0)
		{
			CConfigMgr conf;
			iWidth = conf.Read(CALI_SET, CALI_AXI_WIDTH, 4096);
			iLevel = conf.Read(CALI_SET, CALI_AXI_LEVEL, 2048);
		}
		else if (sDesc.Find("Cor") >= 0)
		{
			iWidth = conf.Read(CALI_SET, CALI_COR_WIDTH, 4096);
			iLevel = conf.Read(CALI_SET, CALI_COR_LEVEL, 2048);
		}
		else if (sDesc.Find("Sag") >= 0)
		{
			iWidth = conf.Read(CALI_SET, CALI_SAG_WIDTH, 4096);
			iLevel = conf.Read(CALI_SET, CALI_SAG_LEVEL, 2048);
		}
		pMRISeries->SetWindowWidthAndLevel(iWidth, iLevel);
	}
	
	double* pderr;											//Add by Dai Liang 2007.6.5
	CList<POINT,POINT>* lpCircle;							//Add by Dai Liang 2007.6.5
	CALI_ERR_RESULT* pCaliErr;
	pderr = new double[17];
	lpCircle = new CList<POINT,POINT>[3];
	pCaliErr = new CALI_ERR_RESULT [3];
	
	/* 修改人：宁随军 从参数文件中读取标定参数
	   修改时间：2009.2.10
	*/
	int iMinCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MIN, 200);
	int iMaxCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MAX, 800);
	
	CMriCalibration mMriCalibration;
	ERR_CODE eCalErr = (ERR_CODE)mMriCalibration.CalMRIBase(m_pMRISeries,4,m_mPhatomGeo.value,mPhatomRefINTracker.value,
		mBaseINTracker.value, m_mBaseINScan.value, 
		pderr, lpCircle, pCaliErr, iMinCirclePnts, iMaxCirclePnts, 1);
	
	LogCaliResult(eCalErr, pderr, m_mBaseINScan, pCaliErr, m_pMRISeries);
	
	m_ctlProGressCalibrate.SetPos(PROGRESS_MAX);
	SetDlgItemText(IDC_LABL_CALI_TIME, "");
	
	g_DlgMain->EndWaitCursor();
	
	//	oFile = fopen("Circle.txt","w");
	//	
	//	for(int j=0;j<3;j++)
	//	{
	//		int nCount = lpCircle[j].GetCount();
	//		POSITION pos = lpCircle[j].GetHeadPosition();
	//		for(i=0;i<nCount;i++)
	//		{	
	//			POINT point = lpCircle[j].GetNext(pos);
	//			fprintf(oFile,"%ld %ld\n",point.x, point.y);
	//		}
	//		fprintf(oFile,"\n");
	//	}
	//	fclose(oFile);
	
	//	int nBaseIndex = conf.Read(CONF_BASE,CONF_BASE_INDEX,0);
	//	switch(nBaseIndex) 
	//	{
	//		case 0:
	//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE1,"0");
	//			break;
	//		case 1:
	//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE2,"0");
	//			break;
	//		case 2:
	//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE3,"0");
	//			break;
	//		case 3:
	//			sFilePath = conf.Read(CONF_BASE,CONF_BASE_CALIFILE4,"0");
	//			break;			
	//	}	
	
	CString msg;
	
	if (eCalErr != ERR_OK)
	{
		msg.Format("Calibration failed!\n  Error code %d.", eCalErr);
		CLog::Log(msg);
		PanaMessageBox("Calibration failed!\n Please check if four circles can be detected.");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else 
	{
		if (pderr[12]>0 && pderr[12]<=m_dbCaliThreshold)
		{
			msg.Format("Calibration is successful. Calibration accuracy is %.4f mm.",pderr[12]);
			CString sLog;
			sLog.Format("标定误差小于%.1f，标定成功", m_dbCaliThreshold);
			CLog::Log(sLog);
			
			m_iStep = 1;
			pDlgMain->m_dlgCryotherapy.m_bPreSurgeryCaliDone = 1;
			ShowMsg(msg);
			GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
		else
		{
			msg.Format("Calibration Failed. Calibration accuracy is %.4f mm.", pderr[12]);
			CLog::Log(msg);
			
			pDlgMain->m_dlgCryotherapy.m_bPreSurgeryCaliDone = 1;
			m_iStep = 1;
			ShowMsg(msg);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
	}
	GetDlgItem(ID_CALI_START)->EnableWindow(TRUE);
	
	delete []pderr;
	delete []lpCircle;
	delete []pCaliErr;
	
	UpdateData(FALSE);
	
}

void CDlgBaseCalibration::LogCaliResult(ERR_CODE eCalErr,double* pderr, Matrix m_mBaseINScan, 
										CALI_ERR_RESULT* pCaliErr, CMRISeries* pMRISeries[3])
{
	CLog::Log("标定结果与误差:");
	CString sLogMsg;
	sLogMsg.Format("注册误差：%.4f",pderr[12]);
	CLog::Log(sLogMsg);
	sLogMsg.Format("标志点注册后误差：%.4f,%.4f,%.4f,%.4f",pderr[13],pderr[14],pderr[15],pderr[16]);
	CLog::Log(sLogMsg);
	sLogMsg.Empty();
	
	CLog::Log("标定结果(Scan2Base Transform)");
	for(int i=0;i<4;i++)
	{
		sLogMsg.Format("%.4f %.4f %.4f %.4f",m_mBaseINScan.Inverse().value[i][0],
			m_mBaseINScan.Inverse().value[i][1],m_mBaseINScan.Inverse().value[i][2],
			m_mBaseINScan.Inverse().value[i][3]);
		CLog::Log(sLogMsg);
		sLogMsg.Empty();
	}
	CLog::Log("圆拟和标准差和最大误差:");
	for(i=0;i<3;i++)
	{
		if (pMRISeries != NULL)
		{
			MR_ImageHeader* pImageHeader = pMRISeries[i]->GetImageAt(0)->GetImageHeader();
			CString sLog;
			sLog.Format("Series: %s - %d/%d", pImageHeader->seriesField.Desc, 
				pMRISeries[i]->GetWindowWidth(), pMRISeries[i]->GetWindowLevel());
			CLog::Log(sLog);
		}	
		
		for (int j=0;j<4;j++)
		{
			
			CString sMothod;
			if (pCaliErr[i].iGetCenterMethod[j] == 0)
			{
				sMothod = "fitting algorithm";
			}
			else if (pCaliErr[i].iGetCenterMethod[j] == 1)
			{
				sMothod = "gravity algorithm";
			}
			sLogMsg.Format("%.4f %.4f %s",pCaliErr[i].fFittingstd[j], pCaliErr[i].fFittingMaxerr[j], sMothod);
			CLog::Log(sLogMsg);
			sLogMsg.Empty();
		}
	}
	CLog::Log("垂线相交误差:");
	for(i=0;i<12;i++)
	{
		sLogMsg.Format("%.4f ",pderr[i]);
		CLog::Log(sLogMsg);
		sLogMsg.Empty();
	}
	CLog::Log("圆半径:");
	for(i=0;i<3;i++)
	{
		sLogMsg.Format("%.4f %.4f %.4f %.4f",pCaliErr[i].fRadius[0],pCaliErr[i].fRadius[1],pCaliErr[i].fRadius[2],pCaliErr[i].fRadius[3]);
		CLog::Log(sLogMsg);
		sLogMsg.Empty();
	}
	if(!(eCalErr == ERR_OK))
	{
		CString sMsg;	
		switch(eCalErr) {
		case ERR_INVALID_MRISERIES:
			sMsg.Format("标定失败，原因：无效的MRI图像指针");
			break;
		case ERR_FAIL_TO_DETECT_CIRCLE:
			sMsg.Format("标定失败，原因：圆心探测失败");
			break;
		case ERR_NOT_ENOUGH_POINTS:
			sMsg.Format("标定失败，原因：边缘探测到的点过少");
		default:
			sMsg.Format("标定失败，原因：未知");
		}
		
		CLog::Log(sMsg);
		return;
	}
}

void CDlgBaseCalibration::ShowMsg(CString sMsg)
{
	SetDlgItemText(IDC_STATIC_NOTE2, sMsg);
	GetDlgItem(IDC_STATIC_NOTE2)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(FALSE);
}

void CDlgBaseCalibration::OnCaliStart() 
{
	// TODO: Add your control notification handler code here
	g_DlgMain->BeginWaitCursor();
	{
		CConfigMgr conf;
		
		if(m_iStep==1)
		{
			CLog::Log("用户选择现在标定，并在提示窗口中点击OK");
			//扫描3个相互垂直的Series
			CConfigMgr con;
			CString sPart;
			CString sProtocal;
			sPart = con.Read(CALI_SET,CONF_PROTOCAL_AXI_PART,"");
			sProtocal = con.Read(CALI_SET,CONF_PROTOCAL_AXI_PROTOCAL,"");
			
			CLog::Log("程序进入标定模式");
			
			// ningsj 2009/3/1
			/*开始扫描关闭相机*/
			if(m_pCryoDlg->m_bTracking)
			{
				m_pCryoDlg->StopTracking();
				
			}
			m_iNumOfImage = 0;
			
				IMRI_PROTOCOL* pCurMRIProtocol;
				pCurMRIProtocol = (IMRI_PROTOCOL*)(m_pCryoDlg->m_ProtMng.SetCurrentProtocol(sPart,sProtocal));
				if (pCurMRIProtocol)
				{
					m_pMRIWrap->SendIMRICommand(COMMAND_CREATESERIES, pCurMRIProtocol);
				}
				else
				{
					g_DlgMain->EndWaitCursor();
					return;
				}
			
			CString sMsg;
			sMsg.Format("已向MRI发送标定命令，使用序列：部位 %s， 协议 %s",sPart,sProtocal);
			CLog::Log(sMsg);
			
			ShowMsg("Start scanning axial, please wait...");
			
			GetDlgItem(ID_CALI_START)->EnableWindow(FALSE);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
			
			m_iStep ++;
		}
	}
	g_DlgMain->EndWaitCursor();
}

void CDlgBaseCalibration::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		if (m_pCryoDlg)
		{
			if (!m_pCryoDlg->m_bTracking)
			{
				m_pCryoDlg->StartTracking();
				
			}
			SetTimer(INDICATOR_UPDATE_TIMER1,MY_ELAPSE200,NULL);
		}
	}
	else
	{
		KillTimer(INDICATOR_UPDATE_TIMER1);
	}
	
}

void CDlgBaseCalibration::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == INDICATOR_UPDATE_TIMER1)
	{
		if (m_pCryoDlg)
		{
			if (m_pCryoDlg->m_bTracking && m_pCryoDlg->m_bPhantomVisible)
			{
				m_btnPhantom.SetIcon(IDI_LEDON, IDI_LEDON);
			}
			else
			{
				m_btnPhantom.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
			}
			
			if (m_pCryoDlg->m_bTracking && m_pCryoDlg->m_bBaseVisible)
			{
				m_btnBase.SetIcon(IDI_LEDON, IDI_LEDON);
			}
			else
			{
				m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
			}
			GetDlgItem(ID_CALI_START)->EnableWindow(m_pCryoDlg->m_bPhantomVisible 
				&& m_pCryoDlg->m_bBaseVisible
				&& m_iStep == 1);
			
			Invalidate(FALSE);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}




