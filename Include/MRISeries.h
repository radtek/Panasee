// MRISeries.h: interface for the CMRISeries class.
// Created by Wei Wei [Mar.13, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRISERIES_H__8EF1F6DE_9D98_4CCA_A2B6_28C1F8B55773__INCLUDED_)
#define AFX_MRISERIES_H__8EF1F6DE_9D98_4CCA_A2B6_28C1F8B55773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include<vector>
#include <algorithm> 

#include "MRIImage.h"
#include <afxtempl.h>
#include <afxcoll.h>
#include "LUT.h"
#include "mv.h"
#include "MultiUsedObject.h"
  
#ifdef _BUILD_MRISERIES_DLL
	#define MRISERIES_API		__declspec(dllexport)
#else
	#define MRISERIES_API		__declspec(dllimport)
#endif

class MRISERIES_API CMRISeries  
{
public:
	//读取指定目录下的融合坐标保存到mFusion中。此操作不影响CMRIseries中的任何数据
	//如果需要使用此融合坐标，需要调用SetFusionCoord函数
	BOOL LoadFusionCoord(CString sFolder,Matrix &mFusion);
	void SaveFusionCoord(CString sFolder=""); 
	Matrix GetFusionCoord();
	void SetFusionCoord(double **ppdTrans);

	Matrix GetImageInScanner();
	double* GetSpacing();

	CMultiUsedObject* GetImagesMemory();

	void CorrectSliceDirection();
	void InsertImage(CMRIImage *pImage);
	//此函数仅供CT下自动标定函数调用
	//随意设置此参数会导致数据丢失
	void SetNumberOfImages(int number);

	int Save();

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CMRISeries();
	virtual ~CMRISeries();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Get series ID
	int GetSeriesID();
	// Get Scan ID
	int GetScanID();
	// Get Study ID
	CString GetStudyID();


	CString m_sFolder;  /*图像存放路径*/
	// Get Name
	CString GetName();
	void SetName(CString sName);

	// Get slice direction
	double* GetSliceDirection();

	// Get phase direction
	double* GetPhaseDirection();

	// Get freq direction
	double* GetFreqDirection();

	// Transformation
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Transform to new position and orientation

	// Translation by 3*1 vector
	void Translation( double dTranslation[3] );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( double dAxis[3], double dAngle );

	
	
	// Set window width
	void ChangeWindowWidthAndLevel( int nWidthShift, int nLevelShift );
	void SetWindowWidthAndLevel(int nWindowWidth, int nWindowLevel);
	
	// Get all images from MRI workstation
	int Load( CString sFolder );

	// Get all images from buffer transfered by Internet
	int Load( char* pBuffer, int nLength, WORD nIMRIACPHiVer = 1, WORD nIMRIACPLoVer = 2);
	
	int Load(CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> &caImgs);

	int LoadImg(CArray<CMRIImage*, CMRIImage*> &caImgs,CString sFilePath);

	// Save image data
	int Save( CString sFolder );

	// Get the pointer to the i th image
	CMRIImage* GetImageAt( int nIndex );

	// Get the number of the images
	int GetNumOfImages();

	bool IsVisible();
	void SetVisible(bool bVisible);

	int  GetWindowWidth();
	int  GetWindowLevel();

	
	// Get new box orientation characters
	char* GetNewBoxOrientChar();

	// Set new box orientation characters
	void SetNewBoxOrientChar( char sOrient[6] );
		
	CLUT32 m_oClut32;

private:
	void ConvertCoordFromPatient2Device(MR_STRUCT_IMAGE *pMRHeader);
	WORD GetIMRIACPVerHi() {return m_nIMRIACPHiVer;};
	WORD GetIMRIACPVerLo() {return m_nIMRIACPLoVer;};
	void UpdateOClut(int nWindowWidth, int nWindowLevel);

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	// Scan ID
	int m_nScanID;
	// Series ID
	int m_nSeriesID;
	
	// Study ID
	CString m_sStudyID;

	// Series(Scan) name 
	CString m_sName;
	
	// Center of the series
	double m_dCenter[3];

	// Normal of the series
	double m_dSliceDirection[3];

	// X direction of the image
	double m_dPhaseDirection[3];

	// Y direction of the image
	double m_dFreqDirection[3];

	// Following used to store original position in scanner coordinates
	double m_dCenterInScanner[3];
	double m_dSliceDirectionInScanner[3];
	double m_dPhaseDirectionInScanner[3];
	double m_dFreqDirectionInScanner[3];

	
	// Pointer to the images of the series
	CMRIImage** m_pMRIImage;

	// Number of the images
	int m_nNumOfImages;

	int m_nWindowWidth;
	int m_nWindowLevel;

	int m_iVisibleCnt; // To identify whether it is display or not
	
	// Characters for box orientation
	// [0]: X+
	// [1]: X-
	// [2]: Y+
	// [3]: Y-
	// [4]: Z+
	// [5]: Z-
	// char m_sBoxOrientation[6];
	char* m_psNewBoxOrientation;
	
	void InitSeries();

	WORD m_nIMRIACPHiVer;
	WORD m_nIMRIACPLoVer;


	//共享图像内存 默认是NULL 图像内存由CMRIImage负责提供
	//图像内存可能由多个对象进行共享，因此需要在合适的时候再进行内存释放
	CMultiUsedObject *m_pImagesMemoryObject; 

	Matrix m_imageInScanner;
	double m_dPixelSpacing[3];

	double m_dFusionCoord[4][4];
};

#endif // !defined(AFX_MRISERIES_H__8EF1F6DE_9D98_4CCA_A2B6_28C1F8B55773__INCLUDED_)
