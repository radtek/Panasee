// IGSCPManager.cpp: implementation of the CIGSCPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "IGSCPManager.h"
#include "CTCalibration.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGSCPManager::CIGSCPManager()
{
	m_pGlobalSeries=NULL;
	m_tranGlobalSeriesScanner2PT.Identity();
	m_bCalibrated=FALSE;
}

CIGSCPManager::~CIGSCPManager()
{

}


BOOL CIGSCPManager::IsCalibrated()
{
	return m_bCalibrated;
}

void CIGSCPManager::SetGlobalSeriesCoord(Matrix *m)
{
	if(m)
	{
		m_tranGlobalSeriesScanner2PT=*m;
		m_bCalibrated=TRUE;
	}else
	{
		m_tranGlobalSeriesScanner2PT.Identity();
		m_bCalibrated=FALSE;
	}
}

Matrix CIGSCPManager::GetGlobalSeriesCoord()
{
	return m_tranGlobalSeriesScanner2PT;
}

void CIGSCPManager::SetGlobalSeries(CMRISeries *pSeries)
{
	m_pGlobalSeries=pSeries;
}

CMRISeries* CIGSCPManager::GetGlobalSeries()
{
	return m_pGlobalSeries;
}

BOOL CIGSCPManager::Calibrate(CMRISeries *pSeries, Matrix &m)
{
	if(!pSeries)
		return FALSE;

	
		

	CCTCalibration cat;
	double mat[16];
	Matrix mtranMarker2Scanner(4,4);
	try
	{
		if(m_deviceType==1 && cat.GetOpticsCoord(pSeries,mat))
		{
			mtranMarker2Scanner.Identity();
			mtranMarker2Scanner.value[0][0]=mat[0];
			mtranMarker2Scanner.value[1][0]=mat[1];
			mtranMarker2Scanner.value[2][0]=mat[2];
			mtranMarker2Scanner.value[0][1]=mat[3];
			mtranMarker2Scanner.value[1][1]=mat[4];
			mtranMarker2Scanner.value[2][1]=mat[5];
			mtranMarker2Scanner.value[0][2]=mat[6];
			mtranMarker2Scanner.value[1][2]=mat[7];
			mtranMarker2Scanner.value[2][2]=mat[8];
			mtranMarker2Scanner.value[0][3]=mat[9];
			mtranMarker2Scanner.value[1][3]=mat[10];
			mtranMarker2Scanner.value[2][3]=mat[11];
			
			m = mtranMarker2Scanner.Inverse();
			return TRUE;
		}else if(m_deviceType==2 && cat.GetEMCoord(pSeries,mat))
		{
			mtranMarker2Scanner.Identity();
			mtranMarker2Scanner.value[0][0]=mat[0];
			mtranMarker2Scanner.value[1][0]=mat[1];
			mtranMarker2Scanner.value[2][0]=mat[2];
			mtranMarker2Scanner.value[0][1]=mat[3];
			mtranMarker2Scanner.value[1][1]=mat[4];
			mtranMarker2Scanner.value[2][1]=mat[5];
			mtranMarker2Scanner.value[0][2]=mat[6];
			mtranMarker2Scanner.value[1][2]=mat[7];
			mtranMarker2Scanner.value[2][2]=mat[8];
			mtranMarker2Scanner.value[0][3]=mat[9];
			mtranMarker2Scanner.value[1][3]=mat[10];
			mtranMarker2Scanner.value[2][3]=mat[11];
			
			m = mtranMarker2Scanner.Inverse();
			return TRUE;
		}

	}catch(...)
	{
		//
	}
	return FALSE;
}	

void CIGSCPManager::SetDeviceType(int i)
{
	if(i==1)
	{
		m_deviceType=1;
	}else
	{
		m_deviceType=2;
	}

}
