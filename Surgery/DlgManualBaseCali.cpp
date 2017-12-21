// DlgManualBaseCali.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgManualBaseCali.h"
#include "CryotherapyDlg.h"
#include "ConfigMgr.h"
#include "ErrorCodes.h"
#include "DlgBaseCalibration.h"
#include "DlgMRIBrowse.h"
#include "Config.h"
#include "DlgMain.h"
#include "Log.h"
#include "globalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgManualBaseCali dialog


CDlgManualBaseCali::CDlgManualBaseCali(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgManualBaseCali::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgManualBaseCali)
	//}}AFX_DATA_INIT
	m_mPhatomRefINTracker.SetDimension(4,4);	
	m_mBaseINTracker.SetDimension(4,4);	
	m_nImgNums = 0;			

	CConfigMgr conf;
	m_dbCaliThreshold = conf.Read(CALI_SET, CALI_THRESHOLD, 1.5);

	m_pCryoDlg = NULL;
}


void CDlgManualBaseCali::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgManualBaseCali)
	DDX_Control(pDX, IDC_BTN_PHANTOM_INDICATOR, m_btnPhantom);
	DDX_Control(pDX, IDC_BTN_BASE_INDICATOR, m_btnBase);
	DDX_Control(pDX, ID_MANUAL_CALI_CALI, m_btnCali);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, ID_MANUAL_CALI_LOAD_DB, m_btnLoadDB);
	DDX_Control(pDX, ID_MANUAL_CALI_LOAD_IMG, m_btnLoadImg);
	DDX_Control(pDX, ID_MANUAL_CALI_LOAD_PHANTOMREF, m_btnLoadPhantom);
	DDX_Control(pDX, ID_MANUAL_CALI_LOAD_BASEINTRACKER, m_btnLoadBase);
	DDX_Control(pDX, ID_MANUAL_CALI, m_btnManualCali);
	DDX_Control(pDX, ID_MANUAL_CALI_S3, m_btnCir3);
	DDX_Control(pDX, ID_MANUAL_CALI_S2, m_btnCir2);
	DDX_Control(pDX, ID_MANUAL_CALI_S1, m_btnCir1);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO3, m_comboSeries3);
	DDX_Control(pDX, IDC_COMBO2, m_comboSeries2);
	DDX_Control(pDX, IDC_COMBO1, m_comboSeries1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgManualBaseCali, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgManualBaseCali)
	ON_BN_CLICKED(ID_MANUAL_CALI_S1, OnManualCaliS1)
	ON_BN_CLICKED(ID_MANUAL_CALI, OnManualCali)
	ON_BN_CLICKED(ID_MANUAL_CALI_LOAD_BASEINTRACKER, OnManualCaliLoadBaseintracker)
	ON_BN_CLICKED(ID_MANUAL_CALI_LOAD_PHANTOMREF, OnManualCaliLoadPhantomref)
	ON_BN_CLICKED(ID_MANUAL_CALI_S2, OnManualCaliS2)
	ON_BN_CLICKED(ID_MANUAL_CALI_S3, OnManualCaliS3)
	ON_MESSAGE(WM_NEW_SERIES, OnMsgNewSeries)
	ON_BN_CLICKED(ID_MANUAL_CALI_LOAD_IMG, OnManualCaliLoadImg)
	ON_BN_CLICKED(ID_MANUAL_CALI_LOAD_DB, OnManualCaliLoadDb)
	ON_BN_CLICKED(ID_MANUAL_CALI_CALI, OnManualCaliCali)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgManualBaseCali message handlers

BOOL CDlgManualBaseCali::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);
	EnableSeprateLine(TRUE);
	CPanaseeDialog::OnInitDialog();

	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);

	CStringArray saMRIs;
	pCryDlg->m_dlgSeries.GetMRISeriesName(saMRIs);

	m_nImgNums = saMRIs.GetSize();
	for( int i=0; i<saMRIs.GetSize(); i++ )
	{
		m_comboSeries1.AddString(saMRIs[i]);
		m_comboSeries2.AddString(saMRIs[i]);
		m_comboSeries3.AddString(saMRIs[i]);
	}
	if ( m_comboSeries1.GetCount()>2 )
	{
		m_comboSeries1.SetCurSel(0);
		m_comboSeries2.SetCurSel(1);
		m_comboSeries3.SetCurSel(2);
	}

	if (m_dlgMRIBrowse.GetSafeHwnd() == NULL)
	{
		m_dlgMRIBrowse.Create(IDD_DLG_MRI_BROWSE, this);
		m_dlgMRIBrowse.m_pCryDlg = pCryDlg;
		m_dlgMRIBrowse.m_bDisplayPolylineTool=TRUE;
	}

#ifdef   _DEBUG   
#else
	CConfigMgr conf;
	BOOL bManualCali = conf.Read(SYSINFO, SYS_MANUAL_CALI, 0);
	if (!bManualCali)
	{
		GetDlgItem(IDC_EDIT_BASE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_PHANTOM_REF)->ShowWindow(FALSE);
		GetDlgItem(ID_MANUAL_CALI_LOAD_BASEINTRACKER)->ShowWindow(FALSE);
		GetDlgItem(ID_MANUAL_CALI_LOAD_PHANTOMREF)->ShowWindow(FALSE);
		GetDlgItem(ID_MANUAL_CALI)->ShowWindow(FALSE);
		m_btnLoadDB.ShowWindow(FALSE);
		m_btnLoadImg.ShowWindow(FALSE);
	}
#endif

	/*m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	m_btnOK.SetThemeHelper(&m_ThemeHelper);
	m_btnManualCali.SetThemeHelper(&m_ThemeHelper);
	m_btnCir1.SetThemeHelper(&m_ThemeHelper);
	m_btnCir2.SetThemeHelper(&m_ThemeHelper);
	m_btnCir3.SetThemeHelper(&m_ThemeHelper);
	m_btnLoadBase.SetThemeHelper(&m_ThemeHelper);
	m_btnLoadPhantom.SetThemeHelper(&m_ThemeHelper);
	m_btnCali.SetThemeHelper(&m_ThemeHelper);
	m_btnLoadDB.SetThemeHelper(&m_ThemeHelper);
	m_btnLoadImg.SetThemeHelper(&m_ThemeHelper);*/

	m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnBase.SizeToContent();
	m_btnPhantom.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnPhantom.SizeToContent();

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	//设置UI界面
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnManualCali);
	btnList.AddTail(&m_btnCali);
	btnList.AddTail(&m_btnOK);
	btnList.AddTail(&m_btnCancel);
	SetBottomBtnList(&btnList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CDlgManualBaseCali::ManualCaliBase()
{
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);

	// Load images
	CString sName;
	int nCurSel = m_comboSeries1.GetCurSel();
	if (nCurSel < 0)
		return false;
	int nLength = m_comboSeries1.GetLBTextLen(nCurSel);
	m_comboSeries1.GetLBText(nCurSel, sName.GetBuffer(nLength));
	m_pMRISeries[0] = pCryDlg->GetMRISeries(sName);

	nCurSel = m_comboSeries2.GetCurSel();
	if (nCurSel < 0)
		return false;
	nLength = m_comboSeries2.GetLBTextLen(nCurSel);
	m_comboSeries2.GetLBText(nCurSel, sName.GetBuffer(nLength));
	m_pMRISeries[1] = pCryDlg->GetMRISeries(sName);

	nCurSel = m_comboSeries3.GetCurSel();
	if (nCurSel < 0)
		return false;
	nLength = m_comboSeries3.GetLBTextLen(nCurSel);
	m_comboSeries3.GetLBText(nCurSel, sName.GetBuffer(nLength));
	m_pMRISeries[2] = pCryDlg->GetMRISeries(sName);
	
	CLog::Log("Start manual calibration ...");
	if ( m_pMRISeries[0] == m_pMRISeries[1] && m_pMRISeries[0] == m_pMRISeries[2] && m_pMRISeries[2] == m_pMRISeries[1])
	{
		PanaMessageBox("Invalid MRI images!");
		return false;
	}

	// Load phantom 
//	CConfigMgr conf;
//	CString sFilePath = conf.Read(CONF_PHANTOM, CONF_PHANTOM_GEOFILE, "0");
	CString sFilePath = CONFIG_FILES[CF_PHANTOM];
	m_mPhatomGeo.SetDimension(4,4);
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
		PanaMessageBox(sMsg);
		return false;
	}

	m_mBaseINScan.SetDimension(4,4);

	CString s;
	CLog::Log("PhatomRefINTracker");
	for(int i=0; i<4; i++)
	{
		s.Format("%f %f %f %f", m_mPhatomRefINTracker.value[i][0], m_mPhatomRefINTracker.value[i][1], m_mPhatomRefINTracker.value[i][2], m_mPhatomRefINTracker.value[i][3]);
		CLog::Log(s);
	}
	CLog::Log("BaseINTracker");
	for(i=0; i<4; i++)
	{
		s.Format("%f %f %f %f", m_mBaseINTracker.value[i][0], m_mBaseINTracker.value[i][1], m_mBaseINTracker.value[i][2], m_mBaseINTracker.value[i][3]);
		CLog::Log(s);
	}

	// Begin calibration
	double* pderr;													//Add by Dai Liang 2007.6.5
	CList<POINT,POINT>* lpCircle;									//Add by Dai Liang 2007.6.5
	CALI_ERR_RESULT* pCaliErr;
	pderr = new double[17];
	memset(pderr, 0, sizeof(double) * 17);
	lpCircle = new CList<POINT,POINT>[3];
	pCaliErr = new CALI_ERR_RESULT [3];

	/* 修改人：宁随军 从参数文件中读取标定参数
	   修改时间：2009.2.10
	*/
	CConfigMgr conf;
	int iMinCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MIN, 200);
	int iMaxCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MAX, 800);
	
	CMriCalibration mMriCalibration;
	ERR_CODE eCalErr = (ERR_CODE)mMriCalibration.CalMRIBase(m_pMRISeries, 4,
		m_mPhatomGeo.value, m_mPhatomRefINTracker.value, m_mBaseINTracker.value,
		m_mBaseINScan.value, pderr, lpCircle, pCaliErr, iMinCirclePnts, iMaxCirclePnts, 1);

	// Log result
	CDlgBaseCalibration dlgBaseCali;
	dlgBaseCali.LogCaliResult(eCalErr, pderr, m_mBaseINScan, pCaliErr, m_pMRISeries);
	
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
			CString sLog;
			sLog.Format("标定误差小于%.1f，标定成功", m_dbCaliThreshold);
			CLog::Log(sLog);

			pCryDlg->m_bPreSurgeryCaliDone = 1;
			msg.Format("Calibration is successful. Calibration accuracy is %.4f mm.",pderr[12]);
			PanaMessageBox(msg);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
		else
		{
			msg.Format("Calibration failed.  Calibration accuracy is %.4f mm.",pderr[12]);
			CLog::Log(msg);
			pCryDlg->m_bPreSurgeryCaliDone = 1;
			PanaMessageBox(msg);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
	}

	delete []pderr;
	delete []lpCircle;
	delete []pCaliErr;

	return true;
}

void CDlgManualBaseCali::OnOK() 
{
	CDlgMain* pDlgMain=g_DlgMain;
	if (pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_WITH_MARKER &&
		pDlgMain->m_dlgCryotherapy.m_bMarkerVisible == FALSE) //NAVI MODE
	{
		PanaMessageBox("Patient Marker is not visible. Please reposition camera.");
		return;
	}

	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);
	CString sFilePath;
	// Save calibration result
	CConfigMgr conf;
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
	pCryDlg->m_tranScanner2Base = m_mBaseINScan.Inverse();
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
	sLog.Format("标定结果保存至Base %d 对应文件:%s",nBaseIndex+1,sFilePath);					//Modified by Dai Liang 2007.12.24
	CLog::Log(sLog);

	CDialog::OnOK();
}

void CDlgManualBaseCali::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgManualBaseCali::OnManualCaliLoadBaseintracker() 
{
	// TODO: Add your control notification handler code here
	CFileDialog LoadFileDlg(TRUE,		
							"*.mtx",
							"*.mtx",
							OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
							"Matrix Files (*.mtx)|*.mtx;*.MTX||");
	if(LoadFileDlg.DoModal()==IDCANCEL)
		return;
	SetDlgItemText(IDC_EDIT_BASE, LoadFileDlg.GetPathName());
}

void CDlgManualBaseCali::OnManualCaliLoadPhantomref() 
{
	// TODO: Add your control notification handler code here
	CFileDialog LoadFileDlg(TRUE,		
							"*.mtx",
							"*.mtx",
							OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
							"Matrix Files (*.mtx)|*.mtx;*.MTX||");
	if(LoadFileDlg.DoModal()==IDCANCEL)
		return;
	SetDlgItemText(IDC_EDIT_PHANTOM_REF, LoadFileDlg.GetPathName());
}

void CDlgManualBaseCali::OnManualCali() 
{
	// TODO: Add your control notification handler code here
	CString sFile;
	GetDlgItemText(IDC_EDIT_PHANTOM_REF, sFile);
	sFile.TrimLeft();
	sFile.TrimRight();
	if(sFile=="")
	{
		PanaMessageBox("Please select a valid Phantom MTX file.");
		return;
	}
	sFile.MakeUpper();
	if(sFile.GetLength()<=4 || sFile.Right(4)!=".MTX")
	{
		CString sTmp;
		sTmp.Format("%s is not a valid MTX file.",sFile);
		PanaMessageBox(sTmp);
		return;
	}
	if (!m_mPhatomRefINTracker.LoadFromFile(sFile))
		return;

	GetDlgItemText(IDC_EDIT_BASE, sFile);
	sFile.TrimLeft();
	sFile.TrimRight();
	if(sFile=="")
	{
		PanaMessageBox("Please select a valid Base MTX file.");
		return;
	}
	sFile.MakeUpper();
	if(sFile.GetLength()<=4 || sFile.Right(4)!=".MTX")
	{
		CString sTmp;
		sTmp.Format("%s is not a valid MTX file.",sFile);
		PanaMessageBox(sTmp);
		return;
	}
	if (!m_mBaseINTracker.LoadFromFile(sFile))
		return;

	ManualCaliBase();	
}

void CDlgManualBaseCali::OnManualCaliS1() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	CMRISeries *pMRISeries;
	int nCurSel = m_comboSeries1.GetCurSel();
	if (nCurSel < 0)
		return;

	int nLength = m_comboSeries1.GetLBTextLen(nCurSel);
	m_comboSeries1.GetLBText(nCurSel, sName.GetBuffer(nLength));
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);
	pMRISeries = pCryDlg->GetMRISeries(sName);
	
	
	//检查序列是否合格
	if(pMRISeries==NULL)
		return;
	CMRIImage *pMRIImage = pMRISeries->GetImageAt(0);
	float fFOVX = pMRIImage->GetFOVX();
	float fFOVY = pMRIImage->GetFOVY();
	int iWidth  = pMRIImage->GetResolutionX();
	int iHeight = pMRIImage->GetResolutionY();
	if(fFOVX!=fFOVY || iWidth!=iHeight)
	{
		PanaMessageBox("Incorrect images. \r\nFovX of image must be equal to FovY.");
		return;
	}

	if (pMRISeries != NULL)
	{
		double* pdCenterPosition  = new double[3*4];
		double* pdCircleDirection = new double[3];
		double* pdFiterr = new double[4];
		CMriCalibration mMriCalibration;
		CList<POINT,POINT> lpCircle;		
		CALI_ERR_RESULT pCaliErr;
		memset((void*)&pCaliErr, 0, sizeof(CALI_ERR_RESULT));

		/* 修改人：宁随军 从参数文件中读取标定参数
		   修改时间：2009.2.10
		*/
		CConfigMgr conf;
		int iMinCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MIN, 200);
		int iMaxCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MAX, 800);

		ERR_CODE eCalErr = (ERR_CODE)mMriCalibration.GetProjCenterOfSphere
			(pMRISeries, 20, pdCircleDirection, pdCenterPosition, pdFiterr,
			&lpCircle, &pCaliErr, iMinCirclePnts, iMaxCirclePnts, 1);

		delete pdCenterPosition;
		delete pdCircleDirection;
		delete pdFiterr;

		m_dlgMRIBrowse.SetMRISeries(pMRISeries);
		int iDlgNum = m_dlgMRIBrowse.GetDlgNum();
		for (int i=0; i<iDlgNum; i++)
		{
			m_dlgMRIBrowse.GetDispDlgAt(i)->SetCaliResult(&lpCircle, pCaliErr);
		}
		m_dlgMRIBrowse.ShowWindow(TRUE);
		lpCircle.RemoveAll();
	}
}

void CDlgManualBaseCali::OnManualCaliS2() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	CMRISeries *pMRISeries;
	int nCurSel = m_comboSeries2.GetCurSel();
	if (nCurSel < 0)
		return;

	int nLength = m_comboSeries2.GetLBTextLen(nCurSel);
	m_comboSeries2.GetLBText(nCurSel, sName.GetBuffer(nLength));
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);
	pMRISeries = pCryDlg->GetMRISeries(sName);
	
	//检查序列是否合格
	if(pMRISeries==NULL)
		return;
	CMRIImage *pMRIImage = pMRISeries->GetImageAt(0);
	float fFOVX = pMRIImage->GetFOVX();
	float fFOVY = pMRIImage->GetFOVY();
	int iWidth  = pMRIImage->GetResolutionX();
	int iHeight = pMRIImage->GetResolutionY();
	if(fFOVX!=fFOVY || iWidth!=iHeight)
	{
		PanaMessageBox("Incorrect images. \r\nFovX of image must be equal to FovY.");
		return;
	}

	if (pMRISeries != NULL)
	{
		double* pdCenterPosition = new double[3*4];
		double* pdCircleDirection = new double[3];
		double* pdFiterr = new double[4];
		CMriCalibration mMriCalibration;
		CList<POINT,POINT> lpCircle;		
		CALI_ERR_RESULT pCaliErr;
		memset((void*)&pCaliErr, 0, sizeof(CALI_ERR_RESULT));
		
		/* 修改人：宁随军 从参数文件中读取标定参数
		   修改时间：2009.2.10
		*/
		CConfigMgr conf;
		int iMinCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MIN, 200);
		int iMaxCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MAX, 800);

		ERR_CODE eCalErr = (ERR_CODE)mMriCalibration.GetProjCenterOfSphere
			(pMRISeries, 20, pdCircleDirection, pdCenterPosition, pdFiterr,
			&lpCircle, &pCaliErr, iMinCirclePnts, iMaxCirclePnts, 1);

		delete pdCenterPosition;
		delete pdCircleDirection;
		delete pdFiterr;

		m_dlgMRIBrowse.SetMRISeries(pMRISeries);
		int iDlgNum = m_dlgMRIBrowse.GetDlgNum();
		for (int i=0; i<iDlgNum; i++)
		{
			m_dlgMRIBrowse.GetDispDlgAt(i)->SetCaliResult(&lpCircle, pCaliErr);
		}
		m_dlgMRIBrowse.ShowWindow(TRUE);
	}
}

void CDlgManualBaseCali::OnManualCaliS3() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	CMRISeries *pMRISeries;
	int nCurSel = m_comboSeries3.GetCurSel();
	if (nCurSel < 0)
		return;

	int nLength = m_comboSeries3.GetLBTextLen(nCurSel);
	m_comboSeries3.GetLBText(nCurSel, sName.GetBuffer(nLength));
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);
	pMRISeries = pCryDlg->GetMRISeries(sName);
	
	//检查序列是否合格
	if(pMRISeries==NULL)
		return;
	CMRIImage *pMRIImage = pMRISeries->GetImageAt(0);
	float fFOVX = pMRIImage->GetFOVX();
	float fFOVY = pMRIImage->GetFOVY();
	int iWidth  = pMRIImage->GetResolutionX();
	int iHeight = pMRIImage->GetResolutionY();
	if(fFOVX!=fFOVY || iWidth!=iHeight)
	{
		PanaMessageBox("Incorrect images. \r\nFovX of image must be equal to FovY.");
		return;
	}

	if (pMRISeries != NULL)
	{
		double* pdCenterPosition = new double[3*4];
		double* pdCircleDirection = new double[3];
		double* pdFiterr = new double[4];
		CMriCalibration mMriCalibration;
		CList<POINT,POINT> lpCircle;		
		CALI_ERR_RESULT pCaliErr;
		memset((void*)&pCaliErr, 0, sizeof(CALI_ERR_RESULT));
		
		/* 修改人：宁随军 从参数文件中读取标定参数
		   修改时间：2009.2.10
		*/
		CConfigMgr conf;
		int iMinCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MIN, 200);
		int iMaxCirclePnts = conf.Read(CALI_SET, CALI_CIRCLE_POINTS_MAX, 800);

		ERR_CODE eCalErr = (ERR_CODE)mMriCalibration.GetProjCenterOfSphere
			(pMRISeries, 20, pdCircleDirection, pdCenterPosition, pdFiterr,
			&lpCircle, &pCaliErr, iMinCirclePnts, iMaxCirclePnts, 1);

		delete pdCenterPosition;
		delete pdCircleDirection;
		delete pdFiterr;

		m_dlgMRIBrowse.SetMRISeries(pMRISeries);
		int iDlgNum = m_dlgMRIBrowse.GetDlgNum();
		for (int i=0; i<iDlgNum; i++)
		{
			m_dlgMRIBrowse.GetDispDlgAt(i)->SetCaliResult(&lpCircle, pCaliErr);
		}
		m_dlgMRIBrowse.ShowWindow(TRUE);
	}
}

LRESULT CDlgManualBaseCali::OnMsgNewSeries(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		CMRISeries* pMRISeries = (CMRISeries*)lParam;
		if (pMRISeries)
		{
			m_comboSeries1.AddString(pMRISeries->GetName());
			m_comboSeries2.AddString(pMRISeries->GetName());
			m_comboSeries3.AddString(pMRISeries->GetName());

			m_nImgNums ++;

			GetDlgItem(ID_MANUAL_CALI_CALI)->EnableWindow(m_pCryoDlg->m_bPhantomVisible 
				&& m_pCryoDlg->m_bBaseVisible
				&& m_nImgNums >= 3
				&& CGlobalFuncs::IsFileExist(CONFIG_FILES[CF_PHANTOM]));
		}
	}

	return 0;
}

void CDlgManualBaseCali::OnManualCaliLoadImg() 
{
	// TODO: Add your control notification handler code here
	m_pCryoDlg->m_dlgCmdBar.SysImportSeries();	
}

void CDlgManualBaseCali::OnManualCaliLoadDb() 
{
	// TODO: Add your control notification handler code here
	m_pCryoDlg->m_dlgCmdBar.OpenDatabase();
}

void CDlgManualBaseCali::OnManualCaliCali() 
{
	// TODO: Add your control notification handler code here
	CCryotherapyDlg *pCryDlg;
	pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);
	if (pCryDlg->m_bTracking)
	{
		pCryDlg->StopTracking();
		
	}

//	if ( !pCryDlg->m_bBaseVisible )
//	{
//		CLog::Log("Track base failed!");
//		AfxMessageBox("Track base failed!");
//		return;
//	}

	m_mPhatomRefINTracker = pCryDlg->m_tPhatom;			
	m_mBaseINTracker = pCryDlg->m_tBase;				
	
	ManualCaliBase();	
}

void CDlgManualBaseCali::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{

		if (m_pCryoDlg)
		{
			if (!m_pCryoDlg->m_bTracking)
			{
				m_pCryoDlg->StartTracking();
				
			}
			SetTimer(INDICATOR_UPDATE_TIMER4,MY_ELAPSE200,NULL);
		}
	}
	else
	{
		KillTimer(INDICATOR_UPDATE_TIMER4);
	}

}

void CDlgManualBaseCali::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == INDICATOR_UPDATE_TIMER4)
	{
		if (m_pCryoDlg)
		{
			if (m_pCryoDlg->m_bPhantomVisible)
			{
				m_btnPhantom.SetIcon(IDI_LEDON, IDI_LEDON);
			}
			else
			{
				m_btnPhantom.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
			}

			if (m_pCryoDlg->m_bBaseVisible)
			{
				m_btnBase.SetIcon(IDI_LEDON, IDI_LEDON);
			}
			else
			{
				m_btnBase.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
			}
			GetDlgItem(ID_MANUAL_CALI_CALI)->EnableWindow(m_pCryoDlg->m_bPhantomVisible 
				&& m_pCryoDlg->m_bBaseVisible
				&& m_nImgNums >= 3
				&& CGlobalFuncs::IsFileExist(CONFIG_FILES[CF_PHANTOM]));

			Invalidate(FALSE);
		}
	}
	
	CPanaseeDialog::OnTimer(nIDEvent);
}
