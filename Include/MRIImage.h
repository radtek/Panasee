// MRIImage.h: interface for the CMRIImage class.
// Created by Wei Wei [Mar.13, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRIIMAGE_H__8134F777_5E69_4160_A986_0A1568888C85__INCLUDED_)
#define AFX_MRIIMAGE_H__8134F777_5E69_4160_A986_0A1568888C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BI_ImageHeader.h"
#include "CT_ImageHeader.h"
#include "US_ImageHeader.h"
#include "IGTImageHeader.h"


class istrstream;
class CColorF;
class CMRISeries;

enum ENUM_IMG_SOURCE
{
	IMS_DEFAULT,
	IMS_XINAO,
	IMS_DCMMR,
	IMS_DCMCT,
	IMS_DCMUS
};

enum ENUM_CODING_DIR_ON_2D_IMAGE
{
    fdLeftToRight,		// →
	fdTopToBottom,		// ↓
	fdRightToLeft,		// ←
	fdBottomToTop,		// ↑
};
 
#ifdef _BUILD_MRISERIES_DLL
	#define MRISERIES_API		__declspec(dllexport)
#else
	#define MRISERIES_API		__declspec(dllimport)
#endif

class MRISERIES_API CMRIImage
{
public:
	void DisableFreeMemoryOnDestroy();
	ENUM_CODING_DIR_ON_2D_IMAGE GetFreqFlag();

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CMRIImage();
	virtual ~CMRIImage();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	CString GetImageName();
	void SetImageName(CString sName);

	// Get image ID
	int GetImageID();
	// Get series ID
	int GetSeriesID();
	// Get Scan ID
	int GetScanID();
	// Get Study ID
	CString GetStudyID();

	// Get the number of the points
	int GetNumOfPts();

	// Get the filed of view of the image
	double GetFOVX();
	double GetFOVY();
	double GetFOVZ();

	double GetRotatedFOVX();
	double GetRotatedFOVY();

	// Get the resolution of the image in pixel
	unsigned short GetResolutionX();
	unsigned short GetResolutionY();
	unsigned short GetResolutionZ();

	unsigned short GetRotatedResolutionX();
	unsigned short GetRotatedResolutionY();

	// Get center
	double* GetCenter();
	// Set offset to series center

	// Get slice direction
	double* GetSliceDirection();
	// Get phase direction
	double* GetPhaseDirection();
	// Get freq direction
	double* GetFreqDirection();

	// Get center position in scanner coordinates 
	double* GetCenterInScanner();

	// Get direction in scanner coordinates
	double* GetFreqDirectionInScanner();

	double* GetPhaseDirectionInScanner();

	double* GetSliceDirectionInScanner();

	// Get direct after rotation
	double* GetFreqDirRotInScanner();
	double* GetPhaseDirRotInScanner();
	double* GetSliceDirRotInScanner();

	// Get thickness
	double GetThickness();
	// Get gap
	double GetGap();
	// Get window width
	int GetWindowWidth();
	// Get window level
	int GetWindowLevel();

	// Get window width
	int GetInterWindowWidth();
	// Get window level
	int GetInterWindowLevel();

	// Get image handle
	MR_STRUCT_IMAGE* GetImage();
	
	// Get the pointer to the colors of each points
	IMG_DATA_TYPE* GetColorOfRawData();

	// Get the pointer to the rotated data
	IMG_DATA_TYPE* GetRotatedData();

	void RemoveRotatedData();

	
	// Get color data in BYTE
	// The memory allocating operation should be done ahead
	void GetImageDataBYTE( BYTE* pbyColor );

	
	// Transformation
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Transform to a new position and a new orientation
	// The orientation needn't be normalized

	// Translation by 3*1 vector
	void Translation( double dTranslation[3] );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( double dAxis[3], double dAngle );

	int LoadFromIMGFile( MR_ImageHeader* pImageHeader, IMG_DATA_TYPE* pData );
	

	// Get image orientation characters
	char** GetOrientChar();

	// get image display orientation after rectification 
	CString* GetDispOrientChar();

	// Get orientation character positions
	double** GetOrientCharPos();

	MR_ImageHeader* GetImageHeader();
	
	ENUM_IMG_SOURCE GetImageType(){ return m_eImgSource;} ;
	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	CMRISeries *m_pSeries;

	IGT_StudyField GetIGTStudyField();
	IGT_SeriesField GetIGTSeriesField();

private:
	
		//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	void InitImage(CT_STRUCT_IMAGE *pImage);
	void InitImage(US_STRUCT_IMAGE *pImage);
	void InitImage(MR_STRUCT_IMAGE *pImage);

	// Get color
	void GetColorValueOfRawData( int nWidth/*4096 - 1024*/, int nLevel/*1024*/, int nNumOfRawData, short* pRawData, short* pColorOfRawData );
	
	// Get color table
	void GetColorTable( int nWidth, int nLevel, int* pColorTable );

	// Decide whether a point is in the circle
	bool IsInCircle( int nPointX, int nPointY, int nCircleCenterX, int nCircleCenterY, int nCircleRadius );

	// Compute the orientation characters
	void ComputeOrientChar(unsigned char PatEntry,unsigned char PatPosition);
	// Compute the orientation characters with new method

	// Compute the positions of the characters
	void ComputeOrientCharPosition();

	// Checked the orientation

					
	// Rotate or flip source image to standard image display orientation
	//   rotate image stored in m_pRotatedData
	void CorrectImageDispOrientation();

	// Rotate source image by dbAngle angle and flip images
	//   rotated and flipped image stored in m_pRotatedData
	void RotateAndFlip(double dbAngle, BOOL bHFlip, BOOL bVFlip);
	void RotateAndFlip();

	double m_dAngle;
	BOOL m_bHFlip;
	BOOL m_bVFlip;
	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////

	CString m_sImgName;

	// Image ID, i.e., sequential number in series
	int m_nImageID;

	// Series ID
	int m_nSeriesID;

	// Scan ID
	int m_nScanID;

	// Study ID
	CString m_sStudyID;

	// Number of the points
	int m_nNumOfPts;

	// Field of view of the image
	double m_dFOV[3];
	double m_dRotatedFOV[2];

	// Resolution of the image in pixel
	unsigned short m_usResolution[3];
	unsigned short m_usRotatedResolution[2];

	// Center
	double m_dCenter[3];

	
	// Slice direction
	double m_dSliceDirection[3];

	// Phase direction
	double m_dPhaseDirection[3];

	// Frequency direction
	double m_dFreqDirection[3];

	// Following used to store original position in scanner coordinates
	double m_dCenterInScanner[3];
	double m_dSliceDirectionInScanner[3];
	double m_dPhaseDirectionInScanner[3];
	double m_dFreqDirectionInScanner[3];

	// Following used to store direction after rotation in scanner coordinates
	double m_dSliceDirRotInScanner[3];
	double m_dPhaseDirRotInScanner[3];
	double m_dFreqDirRotInScanner[3];

	// Thickness
	double m_dThickness;

	// Gap
	double m_dGap;

	// Display window level, determined from the mean signal I4ersities of all the images in the scan.
	int	m_nWindowLevel;

	// Display window width, determined from the standard deviation of the pixel signal I4ensities of all the images in the scan.
    int	m_nWindowWidth;

	
	// MRI image
	MR_STRUCT_IMAGE* m_pImage;

	// Characters for orientation (A, P, S, I, R, L)
	char** m_ppcOrientation;

	// string for corrected display orientation (A, P, S, I, R, L)
	CString m_sDispOri[4];

	// Positions of the orientation characters
	double** m_ppdOrientCharPos;
	

	IMG_DATA_TYPE* m_pRotatedData;

	ENUM_IMG_SOURCE m_eImgSource;

	int m_nVersion;

	ENUM_CODING_DIR_ON_2D_IMAGE m_bLeftRight; 

	//是否在删除本对象时,释放图像占用的内存。图像内存可能由CMRISeries负责释放.
	//默认是TRUE
	BOOL m_bFreeMemoryOnDestroy; 

	
};

#endif // !defined(AFX_MRIIMAGE_H__8134F777_5E69_4160_A986_0A1568888C85__INCLUDED_)
