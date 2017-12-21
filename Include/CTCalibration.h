// CTCalibration.h: interface for the CCTCalibration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCALIBRATION_H__49FFD7C0_C51D_44E1_A364_7373ED8E91AD__INCLUDED_)
#define AFX_CTCALIBRATION_H__49FFD7C0_C51D_44E1_A364_7373ED8E91AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_MRICALIBRATION_DLL
	#if !defined(MRICALIBRATION_API)
		#define MRICALIBRATION_API		__declspec(dllexport)
	#endif
#else
    #if !defined(MRICALIBRATION_API)
		#define MRICALIBRATION_API		 __declspec(dllimport)
	#endif
#endif

#include "MRISeries.h" 

class MRICALIBRATION_API CCTCalibration  
{
public:
	
	CCTCalibration();
	virtual ~CCTCalibration();

	BOOL GetOpticsCoord(CMRISeries *pSeries,double *pMatrix);
	BOOL GetEMCoord(CMRISeries *pSeries,double *pMatrix);

private:
	void Write(double **data,int size);
	BYTE GetPixelValue(double x,double y,double space,int pixelX,int pixelY);
	void GetIndexRange(double value,double space,int dim, int &min,int &max);
	double Dis(double *p1,double *p2);
	BOOL FindRose(double **pdCenter,int size,int &index1,int &index2,int &index3,int &index4);
	BOOL FindCircle(double **pdCenter, int size, int &index1, int &index2, int &index3);
	BOOL FindCircleIndex(double **pdCenter, int size, int &index1, int &index2, int &index3);
};

#endif // !defined(AFX_CTCALIBRATION_H__49FFD7C0_C51D_44E1_A364_7373ED8E91AD__INCLUDED_)
