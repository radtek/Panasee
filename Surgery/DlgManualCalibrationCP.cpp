// DlgManualCalibrationCP.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgManualCalibrationCP.h"
#include "dlgMain.h"
#include "config.h"
#include "pgMath.h"
#include "ConfigMgr.h"
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgManualCalibrationCP dialog


CDlgManualCalibrationCP::CDlgManualCalibrationCP(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgManualCalibrationCP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgManualCalibrationCP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bShowCalibrateTool=FALSE;

	m_preImageTip[0]=m_preImageTip[1]=m_preImageTip[2]=0;

	
}


void CDlgManualCalibrationCP::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgManualCalibrationCP)
	DDX_Control(pDX, IDC_LIST_PNT, m_pntList);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_btnQuit);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_btnRecord);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATE, m_btnCalibrate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgManualCalibrationCP, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgManualCalibrationCP)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, OnButtonCalibrate)
	ON_MESSAGE(WM_FUSION_POINT_POS,OnMarkerResult)
	ON_WM_SHOWWINDOW()
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_CLICK, IDC_LIST_PNT, OnClickListPnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgManualCalibrationCP message handlers

double CDlgManualCalibrationCP::GetAngle(Vector v1, Vector v2)
{
	v1=v1.Normalize();
	v2=v2.Normalize();
	//得到两向量的点积 
	double dotProduct = v1 ^ v2;

	//得到两向量长度的乘积 
	double vectorsMagnitude = 1; //v1.Magnitude() * v2.Magnitude(); 

	//得到两向量夹角 
	double angle = acos( dotProduct / vectorsMagnitude ); 

	//返回角度值 
	angle=(angle/PI)*180;
	return( angle ); 
}

BOOL CDlgManualCalibrationCP::FindCircleIndex(double pdCenter[3][4], int &index1, int &index2, int &index3)
{
	double dFlagValue1=20;
	double dFlagValue2=25;
	double dFlagValue3=30;
	double dThreld=300;

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(j==i)
				continue;
			for(int k=0;k<3;k++)
			{
				if(k==i)
					continue;
				if(k==j)
					continue;
				
				double dis1=Dis(pdCenter[i],pdCenter[j]);
				double dis2=Dis(pdCenter[j],pdCenter[k]);
				double dis3=Dis(pdCenter[k],pdCenter[i]);
				if(dis1<dis2 && dis2<dis3)
				{
					if((fabs(dis1-dFlagValue1)+
						fabs(dis2-dFlagValue2)+
						fabs(dis3-dFlagValue3))<dThreld)
					{
						index1=i;
						index2=j;
						index3=k;
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}
		}
	}
	return FALSE;
}

double CDlgManualCalibrationCP::Dis(double *p1, double *p2)
{
	double *center1=p1;
	double *center2=p2;
	double dResult = sqrt((center1[0]-center2[0])*(center1[0]-center2[0])+
		(center1[1]-center2[1])*(center1[1]-center2[1])+
		(center1[2]-center2[2])*(center1[2]-center2[2]));

	return dResult;
}

LRESULT CDlgManualCalibrationCP::OnMarkerResult(WPARAM wParam, LPARAM lParam)
{
	double *pData=(double*)lParam;
	
	int index=-1;
	POSITION pos = m_pntList.GetFirstSelectedItemPosition();
	if(pos) 
	{
		index = m_pntList.GetNextSelectedItem(pos);		
	}
	if(index>=0 && index<=2)
	{
		m_dFlagPoint[index][0]=pData[0];
		m_dFlagPoint[index][1]=pData[1];
		m_dFlagPoint[index][2]=pData[2];
		m_dFlagPoint[index][3]=1;	
		
		CString sTmp;
		sTmp.Format("     %i *",index+1);
		m_pntList.SetItemText(index, 0,sTmp);

		m_pntList.SetItemText(0, 1, "");
		m_pntList.SetItemText(1, 1, "");
		m_pntList.SetItemText(2, 1, "");
		if(m_dFlagPoint[0][3] && m_dFlagPoint[1][3] && m_dFlagPoint[2][3])
		{
			int index1,index2,index3;
			double dFlagValue1=20;
			double dFlagValue2=25;
			double dFlagValue3=30;
			double dError;
			if(FindCircleIndex(m_dFlagPoint,index1,index2,index3))
			{
				dError=fabs(Dis(m_dFlagPoint[index1],m_dFlagPoint[index2])-dFlagValue1)+
					fabs(Dis(m_dFlagPoint[index1],m_dFlagPoint[index3])-dFlagValue3);
				sTmp.Format("%.2f",dError);
				m_pntList.SetItemText(index1,1,sTmp);

				dError=fabs(Dis(m_dFlagPoint[index2],m_dFlagPoint[index1])-dFlagValue1)+
					fabs(Dis(m_dFlagPoint[index2],m_dFlagPoint[index3])-dFlagValue2);
				sTmp.Format("%.2f",dError);
				m_pntList.SetItemText(index2,1,sTmp);

				dError=fabs(Dis(m_dFlagPoint[index3],m_dFlagPoint[index2])-dFlagValue2)+
					fabs(Dis(m_dFlagPoint[index3],m_dFlagPoint[index1])-dFlagValue3);
				sTmp.Format("%.2f",dError);
				m_pntList.SetItemText(index3,1,sTmp);

			}
		}
	}
	
	return 0;
}

void CDlgManualCalibrationCP::OnButtonQuit() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pDlgMain = g_DlgMain;

	if(pDlgMain->m_CPManager.GetGlobalSeries()==NULL)
	{
		return;
	}

	if(m_dFlagPoint[0][3] && m_dFlagPoint[1][3] && m_dFlagPoint[2][3])
	{
		int index1,index2,index3;
		if(FindCircleIndex(m_dFlagPoint,index1,index2,index3))
		{
			double dTip[3][3];
			dTip[0][0]=m_dFlagPoint[index1][0];
			dTip[0][1]=m_dFlagPoint[index1][1];
			dTip[0][2]=m_dFlagPoint[index1][2];
			dTip[1][0]=m_dFlagPoint[index2][0];
			dTip[1][1]=m_dFlagPoint[index2][1];
			dTip[1][2]=m_dFlagPoint[index2][2];
			dTip[2][0]=m_dFlagPoint[index3][0];
			dTip[2][1]=m_dFlagPoint[index3][1];
			dTip[2][2]=m_dFlagPoint[index3][2];

			Matrix mScanner2Marker(4,4);
			double matrix[4][4];	
			GetMatrix(dTip,matrix);
			for(int i=0;i<4;i++)
			{
				for(int j=0;j<4;j++)
				{
					mScanner2Marker.value[i][j] = matrix[i][j];
				}
			} 
			mScanner2Marker = mScanner2Marker.Inverse();
			
			Matrix mMarker2PT(4,4);
			for(i=0;i<4;i++)
			{
				for(int j=0;j<4;j++)
				{
					mMarker2PT.value[i][j]=m_dMarker2PT[i][j];
				}
			}
			
			Matrix mScanner2PT = mMarker2PT * mScanner2Marker;
			pDlgMain->m_CPManager.SetGlobalSeriesCoord(&mScanner2PT);
		}else
		{
			return;
		}
	}else
	{
		return;
	}
	
	ShowWindow(FALSE);
	pDlgMain->m_dlgCryotherapy.m_tranScanner2Marker = pDlgMain->m_CPManager.GetGlobalSeriesCoord();
	pDlgMain->m_dlgCryotherapy.CreateTransform();
	pDlgMain->m_dlgCryotherapy.InitFrameBoxPosition();
	pDlgMain->m_dlgCryotherapy.TransformAllMRISeries();
	
	::SendMessage(pDlgMain->m_dlgWizard.m_hWnd, WM_WIZARD_STEP, WSTEP_END, WSRET_OK);
}

BOOL CDlgManualCalibrationCP::OnInitDialog() 
{
	SetTitleBarVisable(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iIndex=0;
	GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("Record 1");
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(i==j)
			{
				m_Scanner2Marker[i][j]=1;
			}else
			{
				m_Scanner2Marker[i][j]=0;
			}
		}
	}

	//获取预扫描针的参数
	double dTip[3],dOri[3];
	double dLength,dDiameter;
	CConfigMgr con;
	CString sFilePath = CONFIG_FILES[CF_PROBE_PREIMAGE];
	if( CGlobalFuncs::LoadProbeFile(sFilePath, dOri, dTip, dLength, dDiameter) )
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s successfully. ", sFilePath);
		CLog::Log(sLog);

		m_preImageTip[0]=dTip[0];
		m_preImageTip[1]=dTip[1];
		m_preImageTip[2]=dTip[2];
	}
	else
	{
		m_preImageTip[0]=0;
		m_preImageTip[1]=0;
		m_preImageTip[2]=0;
		CString sLog;
		sLog.Format("Load pre-scan probe from %s fail. ", sFilePath);
		CLog::Log(sLog);
	}

	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_pntList.SetExtendedStyle(dwExStyle);
	m_pntList.InsertColumn( 0, "Index", LVCFMT_CENTER, 40);
	m_pntList.InsertColumn( 1, "Accuracy", LVCFMT_CENTER, 70);
	m_pntList.InitHeader();

	int nItem = m_pntList.InsertItem(0, "     1");
	m_pntList.SetItemText(nItem, 1, "");
	nItem = m_pntList.InsertItem(1, "     2");
	m_pntList.SetItemText(nItem, 1, "");
	nItem = m_pntList.InsertItem(2, "     3");
	m_pntList.SetItemText(nItem, 1, "");


	sFilePath = CONFIG_FILES[CF_MARKER2PT];
	double **pMarker;
	pMarker=new double*[4];
	for(i=0;i<4;i++)
	{
		pMarker[i]=new double [4];
	}
	CDlgMain* pDlgMain = g_DlgMain;
	if(!pDlgMain->m_dlgCryotherapy.LoadTransformation(sFilePath,pMarker))
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_dMarker2PT[i][j]=0;
			}
		}

		m_dMarker2PT[0][0]=1;
		m_dMarker2PT[1][1]=1;
		m_dMarker2PT[2][2]=1;
		m_dMarker2PT[3][3]=1;
	}else
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_dMarker2PT[i][j]=pMarker[i][j];
			}
		}
	}
	for(i=0;i<4;i++)
	{
		delete pMarker[i];
	}
	delete pMarker;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgManualCalibrationCP::OnButtonRecord() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pDlgMain = g_DlgMain;
	if(pDlgMain->m_dlgCryotherapy.m_bProbeVisible==FALSE)
	{
		PanaMessageBox("Probe is not visible.");
		return;
	}
	Vector vTip(4);
	vTip.value[0]=m_preImageTip[0];
	vTip.value[1]=m_preImageTip[1];
	vTip.value[2]=m_preImageTip[2];
	vTip.value[3]=1;
	vTip=pDlgMain->m_dlgCryotherapy.m_tProbe * vTip;

	m_dTip[m_iIndex][0] = vTip.value[0];
	m_dTip[m_iIndex][1] = vTip.value[1];
	m_dTip[m_iIndex][2] = vTip.value[2];

	m_iIndex++;
	if(m_iIndex>2)
		m_iIndex=0;
	if(m_iIndex==0)
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("Record 1");
	else if(m_iIndex==1)
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("Record 2");
	else
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("Record 3");
	
}

void CDlgManualCalibrationCP::OnButtonCalibrate() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pDlgMain = g_DlgMain;
	
	if(pDlgMain->m_dlgCryotherapy.m_bMarkerVisible==FALSE)
	{
		PanaMessageBox("Marker is not visible.");
		return;
	}

	//get marker2PT
	Matrix mMarker2EM(4,4);
	double matrix[4][4];
	GetMatrix(m_dTip,matrix);

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mMarker2EM.value[i][j] = matrix[i][j];
		}
	}

	Matrix mMarker2PT(4,4);
	Matrix mEM2PT = pDlgMain->m_dlgCryotherapy.m_tMarker.Inverse();
	mMarker2PT = mEM2PT * mMarker2EM;
	
	CString sFilePath = CONFIG_FILES[CF_MARKER2PT];
	pDlgMain->m_dlgCryotherapy.SaveTransformation(sFilePath,mMarker2PT.value);
	MessageBox("OK");
	
}


void CDlgManualCalibrationCP::GetMatrix(double dTip[][3],double matrix[][4])
{
	Vector vX(3);
	vX.value[0]=dTip[1][0]-dTip[0][0];
	vX.value[1]=dTip[1][1]-dTip[0][1];
	vX.value[2]=dTip[1][2]-dTip[0][2];
	vX=vX.Normalize();

	Vector vY(3);
	vY.value[0]=dTip[2][0]-dTip[0][0];
	vY.value[1]=dTip[2][1]-dTip[0][1];
	vY.value[2]=dTip[2][2]-dTip[0][2];
	vY=vY.Normalize();

	Vector vZ(3);
	vZ = vX * vY;
	vZ=vZ.Normalize();	
	vY = vZ * vX;
	vY=vY.Normalize();
	


	matrix[0][0]=vX.value[0];
	matrix[1][0]=vX.value[1];
	matrix[2][0]=vX.value[2];
	matrix[3][0]=0;
	matrix[0][1]=vY.value[0];
	matrix[1][1]=vY.value[1];
	matrix[2][1]=vY.value[2];
	matrix[3][1]=0;
	matrix[0][2]=vZ.value[0];
	matrix[1][2]=vZ.value[1];
	matrix[2][2]=vZ.value[2];
	matrix[3][2]=0;
	matrix[0][3]=dTip[0][0];
	matrix[1][3]=dTip[0][1];
	matrix[2][3]=dTip[0][2];
	matrix[3][3]=1;

}


void CDlgManualCalibrationCP::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPanaseeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(m_bShowCalibrateTool)
	{
		m_btnCalibrate.ShowWindow(TRUE);
		m_btnRecord.ShowWindow(TRUE);
		m_pntList.ShowWindow(FALSE);
	}else
	{
		m_btnCalibrate.ShowWindow(FALSE);
		m_btnRecord.ShowWindow(FALSE);

		CDlgMain* pDlgMain = g_DlgMain;
		
		if(pDlgMain->m_CPManager.GetGlobalSeries()==NULL)
		{
			m_showSeries.Hide();
			return;
		}
		int x=GetSystemMetrics(SM_CXSCREEN);
		int y=GetSystemMetrics(SM_CYSCREEN);
		int listWidth=125;
		int listHeight=(x-listWidth-10)/3;
		MoveWindow(5,(y-listHeight)/2,x-10,listHeight);
		
		CRect rc;
		GetClientRect(&rc);
		rc.right-=listWidth;
		m_showSeries.Show(this,rc);
		m_showSeries.SetSeries(pDlgMain->m_CPManager.GetGlobalSeries());

		rc.right+=listWidth;
		m_btnQuit.MoveWindow(rc.right-70,rc.Height()-25,65,20);
		m_pntList.ShowWindow(TRUE);
		m_pntList.MoveWindow(rc.right-listWidth+5,5,listWidth-10,listHeight-80);

		m_pntList.SetItemText(0, 0, "     1");
		m_pntList.SetItemText(1, 0, "     2");
		m_pntList.SetItemText(2, 0, "     3");
		m_pntList.SetItemText(0, 1, "");
		m_pntList.SetItemText(1, 1, "");
		m_pntList.SetItemText(2, 1, "");

		m_dFlagPoint[0][3]=0;
		m_dFlagPoint[1][3]=0;
		m_dFlagPoint[2][3]=0;
		
	}
}

void CDlgManualCalibrationCP::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LIST_PNT)
		m_pntList.MeasureItem(lpMeasureItemStruct);

	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CDlgManualCalibrationCP::OnClickListPnt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index=-1;
	POSITION pos = m_pntList.GetFirstSelectedItemPosition();
	if(pos) 
	{
		index = m_pntList.GetNextSelectedItem(pos);		
	}
	if(index>=0 && index<=2)
	{
		double pnt[3];
		pnt[0]=m_dFlagPoint[index][0];
		pnt[1]=m_dFlagPoint[index][1];
		pnt[2]=m_dFlagPoint[index][2];
		int flag=m_dFlagPoint[index][3];
		if(flag==1) //有效的点
		{
			m_showSeries.SetCrossLine(pnt);
		}
		
	}
	*pResult = 0;
}
