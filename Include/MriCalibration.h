// MriCalibration.h: interface for the CMriCalibration class.
// Created by Wei Wei [Mar.28, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRICALIBRATION_H__51D37A32_D15B_4151_A813_6B9A2D4A37B6__INCLUDED_)
#define AFX_MRICALIBRATION_H__51D37A32_D15B_4151_A813_6B9A2D4A37B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_MRICALIBRATION_DLL
	#define MRICALIBRATION_API		__declspec(dllexport)
#else
	#define MRICALIBRATION_API		 __declspec(dllimport)
#endif

#include <afxtempl.h>
#include "MRISeries.h" 
#define MRI_CAL_INVALID_VALUE -999999
#define PHATOM_SPHERE_NUM 4


typedef struct STRUCT_CALI_ERR_RESULT
{
	float fFittingstd[PHATOM_SPHERE_NUM]; //in mm
	float fFittingMaxerr[PHATOM_SPHERE_NUM]; //in mm
	float fCircleCenter[PHATOM_SPHERE_NUM][2]; // in Pixel
	float fRadius[PHATOM_SPHERE_NUM];// in mm
	int iGetCenterMethod[PHATOM_SPHERE_NUM];	// TO sign get center mothod; 0 - fitting cicirle algorithm; 1-gravity algorithm
}CALI_ERR_RESULT;

class MRICALIBRATION_API CMriCalibration  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CMriCalibration();
	virtual ~CMriCalibration();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// GetProjCenterOfSphere 用于计算某序列内所有球心在该序列所在平面内的投影在MRI坐标系内的坐标
	/* 
	[Input] 
	pMRISeries: 某一MRISeries的指针
	dSphereRadius: 球的半径（mm）
	[Output]
	pdDirectionOfCenter：球在MRIseries平面内投影的法向在MRI坐标系下的表达，也即序列所在平面的法向，为double 指针数组
	pdPositionOfCenter：球心在MRIseries平面内投影在MRI坐标系下的表达，
	pdPositionOfCenter[0+i*3]为第i个球的x坐标。
	pdPositionOfCenter[1+i*3]为第i个球的y坐标。
	pdPositionOfCenter[2+i*3]为第i个球的z坐标。
	pdFiterr：球的拟合误差，尺寸为4，pdFiterr[i]第i个球的拟合误差
	lpCircle：CList<POINT,POINT>指针，序列pMRISeries内所有圆的边界点的图像坐标
	pCaliErr：CALI_ERR_RESULT 指针，保存该序列内4个球球心的计算误差，参考CALI_ERR_RESULT结构
	iCirDectMethod: 0: old method, 1: new method
	[Return]
	根据错误类型返回Error
	*/ 
	int GetProjCenterOfSphere(CMRISeries* pMRISeries,double dSphereRadius,
								double* pdDirectionOfCenter, 
								double* pdPositionOfCenter, double* pdFiterr, 
								CList<POINT,POINT>* lpCircle, CALI_ERR_RESULT* pCaliErr,
								int iMinEdgePntCount, int iMaxEdgePntCount,
								int iCirDectMethod = 0);

	int CalToolTipOrientation(double dHolderAngle,double dHolderOri[3], double dSampleOri[3], double dSampleTip[3], 
										   double dSampleLength, double dSampleDiameter,
										   double dLength, double dDiameter, double dTip[3],double dOrientation[3]);

	// 目前仅用到以下两个函数
	/*
	All the coordinates are Matrix(4,4) defined in double**.
	CalMRIBase 用于计算Base坐标系在MRI坐标系（scan）内的坐标，即ppdBaseINScan矩阵。计算的误差是Base坐标系到MRI坐标系的注册误差。
	[Input]
	pMRISeries: Pointer array in size of 3.
	nNumOfSeries: Number of MRISeries.Default is 3.
	ppdSphereINRef:Spheres's coordinate in Reference(Marker) coordinate system.
	ppdRefINTracker:Reference coordinate in Tracker system.
	ppdBaseINTracker:Base coordinate in Tracker system.
	[OutPut]
	ppdBaseINScan:Base coordinate in MRI (scan) system.
	
	pdRegerr: The error of registeration and intersection, size is 13. pdRegerr[12] is the registeration error and 
	pdRegerr[0]~pdReger[3] are four intersection errors of each sphere in first series; 
	pdRegerr[4]~pdReger[7] are four intersection errors of each sphere in second series;
	pdRegerr[8]~pdReger[11] are four intersection errors of each sphere in third series;
	lpCircle: three lists of image coordinate of circle edge, size is 3. lpCircle[i] is the list to pMRISeries[i]
	pCaliErr: the error of circle fitting and fitting result, size is 3. Refer to the CALI_ERR_RESULT structure
	iCirDectMethod: 0: old method, 1: new method	
	[return]
	ERR_OK: success.
	0: failure.
	*/
	int CalMRIBase(CMRISeries* pMRISeries[3], int nNumOfSeries,
								double** ppdSphereINRef,double** ppdRefINTracker,
								double** ppdBaseINTracker, double** ppdBaseINScan, 
								double* pdRegerr,CList<POINT, POINT>* lpCircle, CALI_ERR_RESULT* pCaliErr,
								int iMinEdgePntCount, int iMaxEdgePntCount,
								int iCirDectMethod = 0);
				
								
	
private:

	// Calibration of the coordinates of the tool tip in the coordinate system of Tool System
	int CalToolTip( CList<double,double>* ldQuat, double dTip[3], double& dStdDev, double& dMaxErr );
	
	// Calibration of the orientation of the tool in the coordinate system of Tool System
	int CalToolOrientation( CList<double,double>* ldqPin, CList<double,double>* ldqBody, double dTip[3],
							double dOrientation[3], double& dStdDev, double& dMaxErr );

	// Just for test
	// Calibration of the orientation of the tool in the coordinate system of Tool System
	// Using line fitting
	// ldPin: coordinates in the coordinate system of Optical Tracking System
	int CalToolOrientation( CList<double,double>* ldPin, CList<double,double>* ldQuaTool, double dOrientation[3], double& dStdDev, double& dMaxErr );

	// Calibration of the rectangle in the coordinate system of base system, i.e., Optical Tracking System or Robot System
	// Vertex A - dRectangle[0]
	// Vertex B - dRectangle[1]
	// Vertex D - dRectangle[2]
	int CalRectangle( CList<double,double>* ldqPoint0, CList<double,double>* ldqPoint1, 
							CList<double,double>* ldqPoint2, CList<double,double>* ldqPoint3, 
							double dTip[3], 
							double dRectangle[3][3], double& dStdDev, double& dMaxErr );

	// Calibration of the image in the coordinate system of base system, i.e., Optical Tracking System or Robot System
	// ppdTrans is the transition matrix (4*4) from base system to the rectangle system. It should be initialized before 
	// invoking the function
	int CalRectangle( CList<double,double>* ldqPoint0, CList<double,double>* ldqPoint1, 
							CList<double,double>* ldqPoint2, CList<double,double>* ldqPoint3, 
							double dTip[3], 
							double** ppdTrans, double& dStdDev, double& dMaxErr );

	// Output dbRectangle
	//  Vertex center - dbRectangle[0]
	//  vAB - dbRectangle[1]
	//  vAD - dbRectangle[2]
	// Output ppdTrans
	//  ppdTrans is the transition matrix (4*4) from base system to the rectangle system. It should be initialize before 
	//  invoking the function
	int CalRectangle(CList<double,double>* ldqCenter, CList<double,double>* ldqXAxis, double dTip[3], double dTipOri[3],
							double dbWidth, double dbHeight, double dbRectangle[3][3], double** ppdTrans);

	// Calibration of Patient Table and Optical Tracking System
	// ppdTrans denotes the transition matrix from Optical Tracking System to Patient Table. It should be initialized before 
	// invoking the function
	int CalPtAndTracking( CList<double,double>* ldPointSetA,
							CList<double,double>* ldPointSetB, double** ppdTrans, double& dError );

	// Calibration of Temporary System and Optical Tracking System
	// ppdTrans denotes the transition matrix from Optical Tracking System to Temporary System. It should be initialized before 
	// invoking the function
	int CalTmpAndTracking( CList<double,double>* ldPointSetA,
							CList<double,double>* ldPointSetB, double** ppdTrans, double& dError );

	// Calibration of Patient Table and MRI System
	// ppdTrans denotes the transition matrix from MRI System to Temporary System. It should be initialized before 
	// invoking the function

	// Calibration of Patient Table and MRI System
	// ppdTrans denotes the transition matrix from MRI System to Temporary System. It should be initialized before 
	// invoking the function

	// Calibration of Patient Table and MRI System
	// ppdTrans denotes the transition matrix from MRI System to Patient Table. It should be initialized before 
	// invoking the function
	int CalTmpAndMRI( CList<double,double>* ldPointSetA,
							CList<double,double>* ldPointSetB, double** ppdTrans, double& dError );

	// Calibration of standard base and new base

	// Get the coordinates of a point in the coordinate system of Optical Tracking System. It should be initialized before 
	// invoking the function
	int GetCoordinate( CList<double,double>* ldQuat, double dTip[3], double dPointInTracking[3] );

	// Calibration of phantom
	int CalPhantom( CList<double,double>* ldPlanePoint, CList<double,double>* ldFeaturePoint, double dSphereCenter, 
							double* pdPhantomOffsets, double& dError );

	// Get the center of the sphere

	int GetCenterOfSphereByFittingSphere( CList<double,double>* ldCentersOfCircles, CList<double,double>* ldAreasOfCircles, CList<double,double>* ldNormalOfCircles,
							CList<int,int>* lnNumOfSlices, double dInitialRadius,
							CList<double,double>* ldCentersOfSpheres, CList<double,double>* ldRadiusesOfSpheres, CList<double,double>* ldError );
	
	

	int MRIImageNINONE(CMRISeries* pMRISeries, BYTE *imgData,
		               int iMinEdgePntCount,int iMaxEdgePntCount,
					   int iCirDectMethod = 0);		//Add by Dai Liang 2008.1.7
	bool IsInCircle( int nPointX, int nPointY, int nCircleCenterX, int nCircleCenterY, int nCircleRadius );
};

#endif // !defined(AFX_MRICALIBRATION_H__51D37A32_D15B_4151_A813_6B9A2D4A37B6__INCLUDED_)
