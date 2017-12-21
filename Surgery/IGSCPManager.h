// IGSCPManager.h: interface for the CIGSCPManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGSCPMANAGER_H__479AD1C5_A68D_4A84_8523_FCF1B60BAC27__INCLUDED_)
#define AFX_IGSCPMANAGER_H__479AD1C5_A68D_4A84_8523_FCF1B60BAC27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRISeries.h"
#include "mv.h"
//管理IGS-CP导航设备的全局数据
class CIGSCPManager  
{
public:
	void SetDeviceType(int i);
	BOOL Calibrate(CMRISeries *pSeries,Matrix& m);
	CMRISeries* GetGlobalSeries();
	void SetGlobalSeries(CMRISeries *pSeries);

	void SetGlobalSeriesCoord(Matrix *m);
	Matrix GetGlobalSeriesCoord();
	BOOL IsCalibrated();

	CIGSCPManager();
	virtual ~CIGSCPManager();

	
private:
	BOOL m_bCalibrated;
	CMRISeries *m_pGlobalSeries;
	Matrix     m_tranGlobalSeriesScanner2PT;
	int m_deviceType;
};

#endif // !defined(AFX_IGSCPMANAGER_H__479AD1C5_A68D_4A84_8523_FCF1B60BAC27__INCLUDED_)
