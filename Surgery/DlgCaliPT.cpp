// DlgCaliPT.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgCaliPT.h"
#include "CryotherapyDlg.h"
#include "Pgmath.h"
#include "GlobalFuncs.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliPT dialog


CDlgCaliPT::CDlgCaliPT(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgCaliPT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliPT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCryoDlg = NULL;
	m_bInCollecting = FALSE;
}

CDlgCaliPT::~CDlgCaliPT()
{
}

void CDlgCaliPT::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliPT)
	DDX_Control(pDX, IDC_BTN_PHANTOM_INDICATOR, m_btnIndicator);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_READ_POS, m_btnReadPos);
	DDX_Control(pDX, ID_CALI_PT, m_btnCaliPT);
	DDX_Control(pDX, ID_CALI_CLEAN, m_btnClean);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliPT, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgCaliPT)
	ON_BN_CLICKED(ID_READ_POS, OnReadPos)
	ON_BN_CLICKED(ID_CALI_PT, OnCaliPt)
	ON_BN_CLICKED(ID_CALI_CLEAN, OnCaliClean)
	ON_MESSAGE(WM_COLLECTION_DONE, OnCollectionDone)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliPT message handlers

BOOL CDlgCaliPT::OnInitDialog() 
{
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	

	
	// TODO: Add extra initialization here
	//m_btnClean.SetThemeHelper(&m_ThemeHelper);
	//m_btnCaliPT.SetThemeHelper(&m_ThemeHelper);
	//m_btnReadPos.SetThemeHelper(&m_ThemeHelper);
//	//m_btnOK.SetThemeHelper(&m_ThemeHelper);
	//m_btnCancel.SetThemeHelper(&m_ThemeHelper);

	m_btnIndicator.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
	m_btnIndicator.SizeToContent();
	CString sTip = "Phantom";
	m_btnIndicator.SetTooltipText(sTip);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCaliPT::OnReadPos() 
{
	// TODO: Add your control notification handler code here
	if (m_pCryoDlg)
	{
		GetDlgItem(ID_READ_POS)->EnableWindow(FALSE);
		m_bInCollecting = TRUE;
		m_pCryoDlg->StartCollectTrackingData(m_pCryoDlg->m_iPhatomHandle, this->m_hWnd);
	}
}

LONG CDlgCaliPT::OnCollectionDone( UINT wParam, LONG lParam )
{
	if (m_pCryoDlg)
	{
		m_pCryoDlg->StopCollectTrackingData();
		
		POSITION pos = m_pCryoDlg->m_lstTrackingData.GetHeadPosition();
		int nCnt = 0;
		double dbX = 0;
		double dbY = 0; 
		double dbZ = 0;
		for (;pos;)
		{
			dbX += m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			dbY += m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			dbZ += m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			m_pCryoDlg->m_lstTrackingData.GetNext(pos);
			m_pCryoDlg->m_lstTrackingData.GetNext(pos);	
			nCnt ++;
		}
		Vector3D vPos;
		vPos.x = dbX / nCnt;
		vPos.y = dbY / nCnt;
		vPos.z = dbZ / nCnt;

		/*判断和上次比较病床位置是否有移动。
		  至少移动100mm以上
		*/
		if(m_arrVPos.GetSize()>0)
		{
			dbX=m_arrVPos.GetAt(m_arrVPos.GetSize()-1).x;
			dbY=m_arrVPos.GetAt(m_arrVPos.GetSize()-1).y;
			dbZ=m_arrVPos.GetAt(m_arrVPos.GetSize()-1).z;

			double dDis;
			dDis=sqrt((dbX-vPos.x)*(dbX-vPos.x)+(dbY-vPos.y)*(dbY-vPos.y)+
				      (dbZ-vPos.z)*(dbZ-vPos.z));
			if(dDis<50)
			{
				PanaMessageBox("Patient table must move forward more than 50mm than last position.");
				
			}else
			{
				m_arrVPos.Add(vPos);
			}
		}else
		{
			m_arrVPos.Add(vPos);
		}

		GetDlgItem(ID_READ_POS)->EnableWindow(TRUE);
		m_bInCollecting = FALSE;

		if (m_arrVPos.GetSize() >= 5)
		{
			GetDlgItem(ID_CALI_PT)->EnableWindow(TRUE);
		}
	}
	return 0;
}

void CDlgCaliPT::OnCaliPt() 
{
	// TODO: Add your control notification handler code here
	CString sMsg;
	int iNum = m_arrVPos.GetSize();
	if (iNum < 5)
	{
		//2009.2.19 ningsj
		sMsg.Format("Please read five positions at least.");
		PanaMessageBox(sMsg);
		return;
	}

	double **dbPos;
	dbPos = new double*[iNum];
	for (int i=0; i<iNum; i++)
	{
		dbPos[i] = new double[3];
		dbPos[i][0] = m_arrVPos.GetAt(i).x;
		dbPos[i][1] = m_arrVPos.GetAt(i).y;
		dbPos[i][2] = m_arrVPos.GetAt(i).z;
	}
	CPGMath pgMath;

	double kx2, ky2, kz2, x2, y2, z2, dbStd2, maxErr2;
	pgMath.LineFitting3D(iNum, dbPos, &kx2, &ky2, &kz2, &x2, &y2, &z2, &dbStd2, &maxErr2);

	Vector3D vDir(kx2, ky2, kz2);

	Matrix mTracker2Scan = m_pCryoDlg->m_tranScanner2Tracker.Inverse();
	Matrix mRotateM = mTracker2Scan.GetSubMatrix(3,3);
	vDir.Rotate(mRotateM.value);
	vDir.normalize();

	vDir.normalize();

	/*将病床标定方向设置为从扫描位置指向手术位置*/
	Vector3D vScanToSurgery;
	vScanToSurgery.x = dbPos[iNum-1][0]-dbPos[0][0];
	vScanToSurgery.y = dbPos[iNum-1][1]-dbPos[0][1];
	vScanToSurgery.z = dbPos[iNum-1][2]-dbPos[0][2];
	vScanToSurgery.normalize();
	//得到两向量的点积 
	double dotProduct = vScanToSurgery * vDir; 
	//得到两向量长度的乘积 
	double vectorsMagnitude = vScanToSurgery.GetLen() * vDir.GetLen() ; 
	//得到两向量夹角 
	double angle = acos( dotProduct / vectorsMagnitude ); 
	//返回角度值 
	angle=(angle/3.1415926)*180;
	if(angle>90.0) //线的方向需要旋转180度
	{
		vDir.x = 0-vDir.x;
		vDir.y = 0-vDir.y;
		vDir.z = 0-vDir.z;
	}

	for (i=0; i<iNum; i++)
	{
		delete dbPos[i];
	}
	delete dbPos;

	sMsg.Format("Calibration result:\n Patient table direction: (%.3f, %.3f, %.3f)\n Standard deviation: %.3f\n Maximum error: %0.3f", vDir.x, vDir.y, vDir.z, dbStd2, maxErr2);
	CLog::Log(sMsg);

	if(fabs(dbStd2)>0.5 || fabs(maxErr2)>1.5)
	{
		sMsg.Format("Calibration failed.\nCalibration accuracy is %.4f mm.", dbStd2);
		CLog::Log(sMsg);
		PanaMessageBox(sMsg);
	}else
	{
		sMsg.Format("Calibration is successful.\nCalibration accuracy is %.4f mm.", dbStd2);
		CLog::Log(sMsg);
		PanaMessageBox(sMsg);

		m_vPTDirInScan = vDir;


		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
}

void CDlgCaliPT::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CDlgCaliPT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgCaliPT::OnCaliClean() 
{
	// TODO: Add your control notification handler code here
	m_arrVPos.RemoveAll();
	GetDlgItem(ID_CALI_PT)->EnableWindow(FALSE);
//	GetDlgItem(ID_READ_POS)->EnableWindow(TRUE);
	m_bInCollecting = FALSE;
}

void CDlgCaliPT::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == INDICATOR_UPDATE_TIMER3)
	{
		if (m_pCryoDlg)
		{
			if (m_pCryoDlg->m_bPhantomVisible)
			{
				m_btnIndicator.SetIcon(IDI_LEDON, IDI_LEDON);
			}
			else
			{
				m_btnIndicator.SetIcon(IDI_LEDOFF, IDI_LEDOFF);
			}
			GetDlgItem(ID_READ_POS)->EnableWindow(m_pCryoDlg->m_bPhantomVisible && !m_bInCollecting);
			//Invalidate(FALSE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgCaliPT::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		SetTimer(INDICATOR_UPDATE_TIMER3,MY_ELAPSE200,NULL);
	}
	else
	{
		KillTimer(INDICATOR_UPDATE_TIMER3);
	}

}

BOOL CDlgCaliPT::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(INDICATOR_UPDATE_TIMER3);

	return CPanaseeDialog::DestroyWindow();
}
