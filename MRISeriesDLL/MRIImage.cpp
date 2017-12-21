// MRIImage.cpp: implementation of the CMRIImage class.
// Created by Wei Wei [Mar.13, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRIImage.h"
#include "ErrorCodes.h"
#include "Geometry.h"
#include "Rectangle.h"
#include "Texture2D.h"
#include "MRISeries.h"
#include "DataProcessor.h"

#pragma warning (disable:4786)
#include <fstream>
#include <exception>
#include <iostream>
#include <cassert>
#include <strstrea.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRIImage::CMRIImage()
{
	
	m_bFreeMemoryOnDestroy=TRUE;

	m_sImgName = "";
	m_pImage = new MR_STRUCT_IMAGE();
	m_pImage->pData = NULL;

	m_ppdOrientCharPos = new double*[4];
	for (int i = 0; i < 4; i++ )
	{
		m_ppdOrientCharPos[i] = new double[3];
	}
	m_ppcOrientation = new char*[4];
	for (i = 0; i < 4; i++)
	{
		m_ppcOrientation[i] = new char[10];
		m_sDispOri[i] = CString("");
	}
	
	m_pRotatedData = NULL;

	m_eImgSource = IMS_DEFAULT;
	m_bLeftRight=fdLeftToRight;

	
	m_dFreqDirectionInScanner[0]=m_dPhaseDirectionInScanner[0]=m_dSliceDirectionInScanner[0]=1;
	m_dFreqDirectionInScanner[1]=m_dPhaseDirectionInScanner[1]=m_dSliceDirectionInScanner[1]=0;
	m_dFreqDirectionInScanner[2]=m_dPhaseDirectionInScanner[2]=m_dSliceDirectionInScanner[2]=0;
	m_dCenterInScanner[0]=m_dCenterInScanner[1]=m_dCenterInScanner[2]=0;

}

CMRIImage::~CMRIImage()
{

	if(m_bFreeMemoryOnDestroy)
	{
		if ( NULL != m_pImage->pData )
		{
			delete m_pImage->pData;
			m_pImage->pData = NULL;
		}
	}else
	{
		m_pImage->pData=NULL;
	}

	if ( NULL != m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	if ( NULL != m_ppdOrientCharPos )
	{
		for ( int i = 0; i <4; i++ )
		{
			delete []m_ppdOrientCharPos[i];
			m_ppdOrientCharPos[i] = NULL;
		}
		delete []m_ppdOrientCharPos;
		m_ppdOrientCharPos = NULL;
	}
	if ( NULL != m_ppcOrientation )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( NULL != m_ppcOrientation[i] )
			{
				delete []m_ppcOrientation[i];
				m_ppcOrientation[i] = NULL;
			}	
		}
		delete [] m_ppcOrientation;
		m_ppcOrientation = NULL;
	}
	
	if (m_pRotatedData)
	{
		delete m_pRotatedData;
		m_pRotatedData = NULL;
	}

	
}


//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////

int CMRIImage::GetImageID()
{
	return m_nImageID;
}


int CMRIImage::GetSeriesID()
{
	if(m_eImgSource == IMS_DCMCT)
		return ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Id;
	else if(m_eImgSource == IMS_DCMUS)
		return ((US_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Id;
	else
		return m_pImage->oHeader.seriesField.Id;
}


int CMRIImage::GetScanID()
{
	return m_nScanID;
}

CString CMRIImage::GetStudyID()
{
	return m_sStudyID;
}

int CMRIImage::GetNumOfPts()
{
	return m_nNumOfPts;
}

double CMRIImage::GetRotatedFOVX()
{
	return m_dRotatedFOV[0];
}

double CMRIImage::GetRotatedFOVY()
{
	return m_dRotatedFOV[1];
}

double CMRIImage::GetFOVX()
{
	return m_dFOV[0];
}

double CMRIImage::GetFOVY()
{
	return m_dFOV[1];
}

double CMRIImage::GetFOVZ()
{
	return m_dFOV[2];
}

unsigned short CMRIImage::GetResolutionX()
{
	return m_usResolution[0];
}

unsigned short CMRIImage::GetResolutionY()
{
	return m_usResolution[1];
}

unsigned short CMRIImage::GetRotatedResolutionX()
{
	return m_usRotatedResolution[0];
}

unsigned short CMRIImage::GetRotatedResolutionY()
{
	return m_usRotatedResolution[1];
}

unsigned short CMRIImage::GetResolutionZ()
{
	return m_usResolution[2];
}

double* CMRIImage::GetCenter()
{
	return m_dCenter;
}



double* CMRIImage::GetCenterInScanner()
{
	return m_dCenterInScanner;
}

double* CMRIImage::GetFreqDirectionInScanner()
{
	return m_dFreqDirectionInScanner;
}

double* CMRIImage::GetPhaseDirectionInScanner()
{
	return m_dPhaseDirectionInScanner;
}

double* CMRIImage::GetSliceDirectionInScanner()
{
	return m_dSliceDirectionInScanner;
}

double* CMRIImage::GetFreqDirRotInScanner()
{
	return m_dFreqDirRotInScanner;
}

double* CMRIImage::GetPhaseDirRotInScanner()
{	
	return m_dPhaseDirRotInScanner;
}

double* CMRIImage::GetSliceDirRotInScanner()
{
	return m_dSliceDirRotInScanner;
}

double* CMRIImage::GetSliceDirection()
{
	return m_dSliceDirection;
}

double* CMRIImage::GetPhaseDirection()
{
	return m_dPhaseDirection;
}

double* CMRIImage::GetFreqDirection()
{
	return m_dFreqDirection;
}


double CMRIImage::GetThickness()
{
	return m_dThickness;
}



double CMRIImage::GetGap()
{
	return m_dGap;
}


int CMRIImage::GetWindowWidth()
{
	return m_pSeries->GetWindowWidth();
}

int CMRIImage::GetWindowLevel()
{
	return m_pSeries->GetWindowLevel();
}

int CMRIImage::GetInterWindowWidth()
{
	return m_nWindowWidth;
}

int CMRIImage::GetInterWindowLevel()
{
	return m_nWindowLevel;
}



IMG_DATA_TYPE* CMRIImage::GetColorOfRawData()
{
	
	return m_pImage->pData;
}

void CMRIImage::RemoveRotatedData()
{
	if (m_pRotatedData)
	{
		delete m_pRotatedData;
		m_pRotatedData=NULL;
	}
}

IMG_DATA_TYPE* CMRIImage::GetRotatedData()
{
	if(!m_pRotatedData)
	{
		RotateAndFlip();
	}
	return m_pRotatedData;
}



MR_STRUCT_IMAGE* CMRIImage::GetImage()
{
	return m_pImage;
}

void CMRIImage::GetImageDataBYTE( BYTE* pbyColor )
{
	if(!m_pRotatedData)
	{
		RotateAndFlip();
	}
	float fTemp = 1.0f/0xffffff;
	for ( int i = 0; i < m_nNumOfPts; i++ )
	{
		pbyColor[i] = (BYTE) ( 255.0f * m_pSeries->m_oClut32[m_pRotatedData[i]] * fTemp );
	}
}



void CMRIImage::Transformation( double** ppdTrans )
{
	int i, j;

	// Construct a rotation matrix
	double** ppdRot = new double*[3];
	for ( i = 0; i < 3; i++ )
	{
		ppdRot[i] = new double[3];
		for ( j = 0; j < 3; j++ )
		{
			ppdRot[i][j] = ppdTrans[i][j];
		}
	}

	// Center
	Vector3D vCenter( m_dCenterInScanner[0], m_dCenterInScanner[1], m_dCenterInScanner[2] );
	vCenter.Transformation( ppdTrans );
	m_dCenter[0] = vCenter.x;
	m_dCenter[1] = vCenter.y;
	m_dCenter[2] = vCenter.z;

	// Slice direction
	Vector3D vSliceDirection( m_dSliceDirectionInScanner[0], m_dSliceDirectionInScanner[1], m_dSliceDirectionInScanner[2] );
	vSliceDirection.Rotate( ppdRot );
	m_dSliceDirection[0] = vSliceDirection.x;
	m_dSliceDirection[1] = vSliceDirection.y;
	m_dSliceDirection[2] = vSliceDirection.z;
	
	// Phase direction
	Vector3D vPhaseDirection( m_dPhaseDirectionInScanner[0], m_dPhaseDirectionInScanner[1], m_dPhaseDirectionInScanner[2] );
	vPhaseDirection.Rotate( ppdRot );
	m_dPhaseDirection[0] = vPhaseDirection.x;
	m_dPhaseDirection[1] = vPhaseDirection.y;
	m_dPhaseDirection[2] = vPhaseDirection.z;

	// Freq direction
	Vector3D vFreqDirection( m_dFreqDirectionInScanner[0], m_dFreqDirectionInScanner[1], m_dFreqDirectionInScanner[2] );
	vFreqDirection.Rotate( ppdRot );
	m_dFreqDirection[0] = vFreqDirection.x;
	m_dFreqDirection[1] = vFreqDirection.y;
	m_dFreqDirection[2] = vFreqDirection.z;
	
	// Release memory
	for ( i = 0; i < 3; i++ )
		delete []ppdRot[i];
	delete []ppdRot;

	ComputeOrientCharPosition();	// Added by Wei Wei [Sep.8, 2006]
}


void CMRIImage::Translation( double dTranslation[3] )
{
	m_dCenter[0] += dTranslation[0];
	m_dCenter[1] += dTranslation[1];
	m_dCenter[2] += dTranslation[2];
	Vector3D vTanslation( dTranslation[0], dTranslation[1], dTranslation[2] );
	

	ComputeOrientCharPosition();	// Added by Wei Wei [Sep.8, 2006]
}

void CMRIImage::Rotation( double** ppdRot )
{
	Vector3D vTmp;
	// Center
	vTmp.x = m_dCenter[0];
	vTmp.y = m_dCenter[1];
	vTmp.z = m_dCenter[2];
	vTmp.Rotate( ppdRot );
	m_dCenter[0] = vTmp.x;
	m_dCenter[1] = vTmp.y;
	m_dCenter[2] = vTmp.z;
	// Phase direction
	vTmp.x = m_dPhaseDirection[0];
	vTmp.y = m_dPhaseDirection[1];
	vTmp.z = m_dPhaseDirection[2];
	vTmp.Rotate( ppdRot );
	m_dPhaseDirection[0] = vTmp.x;
	m_dPhaseDirection[1] = vTmp.y;
	m_dPhaseDirection[2] = vTmp.z;
	// Freq direction
	vTmp.x = m_dFreqDirection[0];
	vTmp.y = m_dFreqDirection[1];
	vTmp.z = m_dFreqDirection[2];
	vTmp.Rotate( ppdRot );
	m_dFreqDirection[0] = vTmp.x;
	m_dFreqDirection[1] = vTmp.y;
	m_dFreqDirection[2] = vTmp.z;
	// Slice direction
	vTmp.x = m_dSliceDirection[0];
	vTmp.y = m_dSliceDirection[1];
	vTmp.z = m_dSliceDirection[2];
	vTmp.Rotate( ppdRot );
	m_dSliceDirection[0] = vTmp.x;
	m_dSliceDirection[1] = vTmp.y;
	m_dSliceDirection[2] = vTmp.z;
	
	ComputeOrientCharPosition();	// Added by Wei Wei [Sep.8, 2006]
}

void CMRIImage::Rotation( double dAxis[3], double dAngle )
{
	Vector3D vAxis( dAxis[0], dAxis[1], dAxis[2] );
	Vector3D vTmp;
	double dRadians = dAngle * 180 / PI;
	// Center
	vTmp.x = m_dCenter[0];
	vTmp.y = m_dCenter[1];
	vTmp.z = m_dCenter[2];
	vAxis.Rotate( vTmp, dRadians );
	m_dCenter[0] = vTmp.x;
	m_dCenter[1] = vTmp.y;
	m_dCenter[2] = vTmp.z;
	// Phase direction
	vTmp.x = m_dPhaseDirection[0];
	vTmp.y = m_dPhaseDirection[1];
	vTmp.z = m_dPhaseDirection[2];
	vAxis.Rotate( vTmp, dRadians );
	m_dPhaseDirection[0] = vTmp.x;
	m_dPhaseDirection[1] = vTmp.y;
	m_dPhaseDirection[2] = vTmp.z;
	// Freq direction
	vTmp.x = m_dFreqDirection[0];
	vTmp.y = m_dFreqDirection[1];
	vTmp.z = m_dFreqDirection[2];
	vAxis.Rotate( vTmp, dRadians );
	m_dFreqDirection[0] = vTmp.x;
	m_dFreqDirection[1] = vTmp.y;
	m_dFreqDirection[2] = vTmp.z;
	// Slice direction
	vTmp.x = m_dSliceDirection[0];
	vTmp.y = m_dSliceDirection[1];
	vTmp.z = m_dSliceDirection[2];
	vAxis.Rotate( vTmp, dRadians );
	m_dSliceDirection[0] = vTmp.x;
	m_dSliceDirection[1] = vTmp.y;
	m_dSliceDirection[2] = vTmp.z;
	
	ComputeOrientCharPosition();	// Added by Wei Wei [Sep.8, 2006]

}

void CMRIImage::InitImage(MR_STRUCT_IMAGE *pImage)
{
		
	// Determine version
	m_nVersion = pImage->oHeader.headInfo.Version;

	// ID
	m_nImageID = (int) pImage->oHeader.imageField.Id;
	m_nSeriesID = (int) pImage->oHeader.seriesField.Id;

	int a = IMG_VER_HEADER_1_2;
	if (m_nVersion == a)
	{
		m_nScanID = (int) pImage->oHeader.scanField._v1.nNumOfScans;
	}
	else 
	{
		m_nScanID = (int) pImage->oHeader.scanField._v0.nNumOfScans;

	}
	if(m_nScanID<0)
	{
		m_nScanID=0;
	}
	m_sStudyID = pImage->oHeader.studyField.Id;

	// Freq direction
	Vector3D vFreq((double) pImage->oHeader.scanField.FreqDirX,(double) pImage->oHeader.scanField.FreqDirY,(double) pImage->oHeader.scanField.FreqDirZ);
	vFreq.SetUnit();
	m_dFreqDirection[0] = m_dFreqDirectionInScanner[0] = vFreq.x;
	m_dFreqDirection[1] = m_dFreqDirectionInScanner[1] = vFreq.y;
	m_dFreqDirection[2] = m_dFreqDirectionInScanner[2] = vFreq.z;
	
	// Phase direction
	Vector3D vPhase((double) pImage->oHeader.scanField.PhaseDirX,
		(double) pImage->oHeader.scanField.PhaseDirY,
		(double) pImage->oHeader.scanField.PhaseDirZ);
	vPhase.SetUnit();
	m_dPhaseDirection[0] = m_dPhaseDirectionInScanner[0] = vPhase.x;
	m_dPhaseDirection[1] = m_dPhaseDirectionInScanner[1] = vPhase.y;
	m_dPhaseDirection[2] = m_dPhaseDirectionInScanner[2] = vPhase.z;
	
	// Slice direction
	Vector3D vSlice = vFreq ^ vPhase;
	vSlice.SetUnit();
	m_dSliceDirection[0] = m_dSliceDirectionInScanner[0] = vSlice.x;
	m_dSliceDirection[1] =  m_dSliceDirectionInScanner[1] = vSlice.y;
	m_dSliceDirection[2] =  m_dSliceDirectionInScanner[2] = vSlice.z;
	// Center
	m_dCenter[0] = m_dCenterInScanner[0] = (double) pImage->oHeader.imageField.fCenterPointX;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[1] = m_dCenterInScanner[1] = (double) pImage->oHeader.imageField.fCenterPointY;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[2] = m_dCenterInScanner[2] = (double) pImage->oHeader.imageField.fCenterPointZ;	// Added by Wei Wei [Aug.9, 2006]
	// Thickness
	m_dThickness = (double) pImage->oHeader.seriesField.SliceThickness;
	
	if(m_eImgSource == IMS_XINAO)
	{
		if(m_pImage->oHeader.seriesField._v1.bDefaultSliceOrder)
		{
			m_dGap = (double) m_pImage->oHeader.seriesField.SliceGap;
		}else
		{
			m_dGap = -m_pImage->oHeader.seriesField.SliceGap-2*m_pImage->oHeader.seriesField.SliceThickness;
		}
	}else
	{
		m_dGap = (double) m_pImage->oHeader.seriesField.SliceGap;
	}
	
	// Field of view
	m_dFOV[0] = (double) pImage->oHeader.seriesField.FovX;
	m_dFOV[1] = (double) pImage->oHeader.seriesField.FovY;
	m_dFOV[2] = (double) pImage->oHeader.seriesField.FovZ;
	//vXDirection *= m_dFOV[0];
	//vYDirection *= m_dFOV[1];
	//SetRectangle( vCenter, vXDirection, vYDirection );
	// Resolution
	m_usResolution[0] = (unsigned short) pImage->oHeader.seriesField.ResolutionX;
	m_usResolution[1] = (unsigned short) pImage->oHeader.seriesField.ResolutionY;
	m_usResolution[2] = (unsigned short) pImage->oHeader.seriesField.ResolutionZ;
	// Window width
	m_nWindowWidth = (int) pImage->oHeader.imageField.WindowWidth;
	// Window level
	m_nWindowLevel = (int) pImage->oHeader.imageField.WindowLevel;

	if (m_eImgSource == IMS_XINAO)
	{
		m_sImgName.Format( "%s.%d.%d.%d", m_sStudyID, m_nSeriesID, m_nScanID, m_nImageID+1 );
	}
	else
	{
		m_sImgName.Format( "%s.%d.%d", m_sStudyID, m_nSeriesID, m_nImageID );
	}
	
	ComputeOrientChar(pImage->oHeader.studyField.PatEntry,
		pImage->oHeader.studyField.PatPosition);	
	ComputeOrientCharPosition();
	CorrectImageDispOrientation();
}

void CMRIImage::InitImage(CT_STRUCT_IMAGE *pImage)
{
	
	// Determine version
	m_nVersion = pImage->oHeader.headInfo.Version;

	// ID
	m_nImageID = (int) pImage->oHeader.imageField.Id;
	m_nSeriesID = (int) pImage->oHeader.seriesField.Id;

	m_nScanID = 1;

	m_sStudyID = pImage->oHeader.studyField.Id;

	// Freq direction
	Vector3D vFreq((double) pImage->oHeader.scanField.FreqDirX,
		(double) pImage->oHeader.scanField.FreqDirY,
		(double) pImage->oHeader.scanField.FreqDirZ);
	vFreq.SetUnit();

	m_dFreqDirection[0] = m_dFreqDirectionInScanner[0] = vFreq.x;
	m_dFreqDirection[1] = m_dFreqDirectionInScanner[1] = vFreq.y;
	m_dFreqDirection[2] = m_dFreqDirectionInScanner[2] = vFreq.z;
	

	// Phase direction
	Vector3D vPhase((double) pImage->oHeader.scanField.PhaseDirX,
		(double) pImage->oHeader.scanField.PhaseDirY,
		(double) pImage->oHeader.scanField.PhaseDirZ);
	vPhase.SetUnit();
	m_dPhaseDirection[0] = m_dPhaseDirectionInScanner[0] = vPhase.x;
	m_dPhaseDirection[1] = m_dPhaseDirectionInScanner[1] = vPhase.y;
	m_dPhaseDirection[2] = m_dPhaseDirectionInScanner[2] = vPhase.z;
	
	// Slice direction
	Vector3D vSlice = vFreq ^ vPhase;
	vSlice.SetUnit();
	m_dSliceDirection[0] = m_dSliceDirectionInScanner[0] = vSlice.x;
	m_dSliceDirection[1] =  m_dSliceDirectionInScanner[1] = vSlice.y;
	m_dSliceDirection[2] =  m_dSliceDirectionInScanner[2] = vSlice.z;

	// Center
	m_dCenter[0] = m_dCenterInScanner[0] = (double) pImage->oHeader.imageField.fCenterPointX;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[1] = m_dCenterInScanner[1] = (double) pImage->oHeader.imageField.fCenterPointY;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[2] = m_dCenterInScanner[2] = (double) pImage->oHeader.imageField.fCenterPointZ;	// Added by Wei Wei [Aug.9, 2006]
	
	// Thickness
	m_dThickness = (double) pImage->oHeader.seriesField.SliceThickness;
	m_dGap = (double) pImage->oHeader.seriesField.SliceGap;
	
	// Field of view
	m_dFOV[0] = (double) pImage->oHeader.seriesField.FovX;
	m_dFOV[1] = (double) pImage->oHeader.seriesField.FovY;
	m_dFOV[2] = (double) pImage->oHeader.seriesField.FovZ;

	
	// Resolution
	m_usResolution[0] = (unsigned short) pImage->oHeader.seriesField.ResolutionX;
	m_usResolution[1] = (unsigned short) pImage->oHeader.seriesField.ResolutionY;
	m_usResolution[2] = (unsigned short) pImage->oHeader.seriesField.ResolutionZ;
	// Window width
	m_nWindowWidth = (int) pImage->oHeader.imageField.WindowWidth;
	// Window level
	m_nWindowLevel = (int) pImage->oHeader.imageField.WindowLevel;

	m_sImgName.Format( "%s.%d.%d", m_sStudyID, m_nSeriesID, m_nImageID );
	
	
	ComputeOrientChar(pImage->oHeader.studyField.PatEntry,
		pImage->oHeader.studyField.PatPosition);	
	ComputeOrientCharPosition();
	CorrectImageDispOrientation();
}

void CMRIImage::InitImage(US_STRUCT_IMAGE *pImage)
{
	/*判断study版本号*/
		
	// Determine version
	m_nVersion = pImage->oHeader.headInfo.Version;

	// ID
	m_nImageID = (int) pImage->oHeader.imageField.Id;
	m_nSeriesID = (int) pImage->oHeader.seriesField.Id;

	m_nScanID = 1;

	m_sStudyID = pImage->oHeader.studyField.Id;

	// Freq direction
	Vector3D vFreq((double) pImage->oHeader.scanField.FreqDirX,
		(double) pImage->oHeader.scanField.FreqDirY,
		(double) pImage->oHeader.scanField.FreqDirZ);
	vFreq.SetUnit();

	m_dFreqDirection[0] = m_dFreqDirectionInScanner[0] = vFreq.x;
	m_dFreqDirection[1] = m_dFreqDirectionInScanner[1] = vFreq.y;
	m_dFreqDirection[2] = m_dFreqDirectionInScanner[2] = vFreq.z;
	
	// Phase direction
	Vector3D vPhase((double) pImage->oHeader.scanField.PhaseDirX,
		(double) pImage->oHeader.scanField.PhaseDirY,
		(double) pImage->oHeader.scanField.PhaseDirZ);
	vPhase.SetUnit();
	m_dPhaseDirection[0] = m_dPhaseDirectionInScanner[0] = vPhase.x;
	m_dPhaseDirection[1] = m_dPhaseDirectionInScanner[1] = vPhase.y;
	m_dPhaseDirection[2] = m_dPhaseDirectionInScanner[2] = vPhase.z;
	
	// Slice direction
	Vector3D vSlice = vFreq ^ vPhase;
	vSlice.SetUnit();
	m_dSliceDirection[0] = m_dSliceDirectionInScanner[0] = vSlice.x;
	m_dSliceDirection[1] =  m_dSliceDirectionInScanner[1] = vSlice.y;
	m_dSliceDirection[2] =  m_dSliceDirectionInScanner[2] = vSlice.z;

	// Center
	m_dCenter[0] = m_dCenterInScanner[0] = (double) pImage->oHeader.imageField.fCenterPointX;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[1] = m_dCenterInScanner[1] = (double) pImage->oHeader.imageField.fCenterPointY;	// Added by Wei Wei [Aug.9, 2006]
	m_dCenter[2] = m_dCenterInScanner[2] = (double) pImage->oHeader.imageField.fCenterPointZ;	// Added by Wei Wei [Aug.9, 2006]
	
	// Thickness
	m_dThickness = (double) pImage->oHeader.seriesField.SliceThickness;
	m_dGap = (double) pImage->oHeader.seriesField.SliceGap;
	
	// Field of view
	m_dFOV[0] = (double) pImage->oHeader.seriesField.FovX;
	m_dFOV[1] = (double) pImage->oHeader.seriesField.FovY;
	m_dFOV[2] = (double) pImage->oHeader.seriesField.FovZ;

	
	// Resolution
	m_usResolution[0] = (unsigned short) pImage->oHeader.seriesField.ResolutionX;
	m_usResolution[1] = (unsigned short) pImage->oHeader.seriesField.ResolutionY;
	m_usResolution[2] = (unsigned short) pImage->oHeader.seriesField.ResolutionZ;
	// Window width
	m_nWindowWidth = (int) pImage->oHeader.imageField.WindowWidth;
	// Window level
	m_nWindowLevel = (int) pImage->oHeader.imageField.WindowLevel;

	m_sImgName.Format( "%s.%d.%d", m_sStudyID, m_nSeriesID, m_nImageID );
	
	
	ComputeOrientChar(pImage->oHeader.studyField.PatEntry,
		pImage->oHeader.studyField.PatPosition);	
	ComputeOrientCharPosition();
	CorrectImageDispOrientation();
}
// Used currently
int CMRIImage::LoadFromIMGFile( MR_ImageHeader* pImageHeader, IMG_DATA_TYPE* pData )
{
	// read whole header
	
	if(sizeof(US_ImageHeader)!=sizeof(CT_ImageHeader) ||
		sizeof(US_HEADER_INFO)!=sizeof(CT_HEADER_INFO) ||
		sizeof(US_SITE_FIELD)!=sizeof(CT_SITE_FIELD) ||
		sizeof(US_PATIENT_FIELD)!=sizeof(CT_PATIENT_FIELD) ||
		sizeof(US_STUDY_FIELD)!=sizeof(CT_STUDY_FIELD) ||
		sizeof(US_SERIES_FIELD)!=sizeof(CT_SERIES_FIELD) ||
		sizeof(US_SCAN_FIELD)!=sizeof(CT_SCAN_FIELD) ||
		sizeof(US_IMAGE_FIELD)!=sizeof(CT_IMAGE_FIELD) )
	{
		//软件文件头不正确
		BOOL i=sizeof(US_HEADER_INFO)-sizeof(CT_HEADER_INFO);
		i=sizeof(US_SITE_FIELD)-sizeof(CT_SITE_FIELD);
		i=sizeof(US_PATIENT_FIELD)-sizeof(CT_PATIENT_FIELD);
		i=sizeof(US_STUDY_FIELD)-sizeof(CT_STUDY_FIELD) ;
		i=sizeof(US_SERIES_FIELD)-sizeof(CT_SERIES_FIELD) ;
		i=sizeof(US_SCAN_FIELD)-sizeof(CT_SCAN_FIELD) ;
		i=sizeof(US_IMAGE_FIELD)-sizeof(CT_IMAGE_FIELD) ;
		
		AfxMessageBox("File Header Defination is not correct.");
		return -1;
	}
	memcpy( &(m_pImage->oHeader.headInfo), &pImageHeader->headInfo, sizeof(m_pImage->oHeader.headInfo) );				// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.siteField), &pImageHeader->siteField, sizeof(m_pImage->oHeader.siteField) );				// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.patientField), &pImageHeader->patientField, sizeof(m_pImage->oHeader.patientField) );	// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.studyField), &pImageHeader->studyField, sizeof(m_pImage->oHeader.studyField) );		// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.seriesField), &pImageHeader->seriesField, sizeof(m_pImage->oHeader.seriesField) );		// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.scanField), &pImageHeader->scanField, sizeof(m_pImage->oHeader.scanField) );			// Added by Wei Wei [Aug.18, 2006]
	memcpy( &(m_pImage->oHeader.imageField), &pImageHeader->imageField, sizeof(m_pImage->oHeader.imageField) );	// Added by Wei Wei [Aug.18, 2006]
	
	
	if (strncmp(m_pImage->oHeader.headInfo.BITitle, "BIMRI", 5) == 0)
	{
		m_eImgSource = IMS_XINAO;
	}
	else if(strncmp(m_pImage->oHeader.headInfo.BITitle, "DCMCT", 5) == 0)
	{
		
		m_eImgSource = IMS_DCMCT;
	}else if(strncmp(m_pImage->oHeader.headInfo.BITitle, "DCMMR", 5) == 0)
	{
		m_eImgSource = IMS_DCMMR;
	}else if(strncmp(m_pImage->oHeader.headInfo.BITitle, "DCMUS", 5) == 0)
	{
		m_eImgSource = IMS_DCMUS;
	}else
	{
		AfxMessageBox("Not support this file.");
		return -1;
	}

	if(m_eImgSource == IMS_DCMCT)
	{
		m_nNumOfPts = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.ResolutionX 
			* ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.ResolutionY;
	}else if(m_eImgSource == IMS_DCMUS)
	{
		m_nNumOfPts = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.ResolutionX 
			* ((US_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.ResolutionY;
	}else
	{
		m_nNumOfPts = m_pImage->oHeader.seriesField.ResolutionX * m_pImage->oHeader.seriesField.ResolutionY;
		
	}
	
	if ( NULL != m_pImage->pData )
	{
		delete m_pImage->pData;
		m_pImage->pData = NULL;
	}
	m_pImage->pData = new IMG_DATA_TYPE[m_nNumOfPts];
	memcpy(m_pImage->pData,pData,sizeof(short) * m_nNumOfPts);

	for (int i=0; i<m_nNumOfPts; i++)
	{
		if (m_pImage->pData[i] > 32767)
		{
			m_pImage->pData[i] = 0;
		}
	}

	if(m_eImgSource == IMS_DCMCT)
	{
		InitImage((CT_STRUCT_IMAGE *)m_pImage);
	}else if(m_eImgSource == IMS_DCMUS)
	{
		InitImage((US_STRUCT_IMAGE *)m_pImage);
	}else
	{
		InitImage(m_pImage);
		
	}
	
	return ERR_OK;
}


void CMRIImage::GetColorValueOfRawData( int nWidth, int nLevel, int nNumOfRawData, short* pRawData, short* pColorOfRawData )
{
	int* pColorTable = new int[4096]; 
	//GetColorTable( nWidth, nLevel, pColorTable );
	int i;
	int nMaxValue = 255, nMinValue = 0;
	int nMinKey = 0, m_nMaxKey = 4095;

	int nLow = nLevel - (nWidth - 1) / 2;
	int nHigh = nLevel + nWidth / 2;

	if (nLow >= 0)
	{
		memset( pColorTable, 0, sizeof (int) * (nLow + 1) );
	}

	COLORREF clrWhite = RGB(255, 255, 255);
	for ( i = nHigh; i <= m_nMaxKey; i++)
	{
		pColorTable[i] = clrWhite;
	}

	double a, b;
	a = (nMaxValue - nMinValue)/static_cast<double>(nHigh - nLow);
	b = (nHigh * static_cast<double>(nMinValue) - nLow * static_cast<double>(nMaxValue))/static_cast<double>(nHigh - nLow);

	int nStart = max (nLow, 0);
	int nEnd = min (nHigh, m_nMaxKey);

	unsigned char* puc = 
	reinterpret_cast<unsigned char*>(pColorTable + nStart);

	for (i = nStart; i < nEnd; i++)
	{
		*(puc+2) = *(puc+1) = *puc = static_cast<unsigned char>(a * i + b);
		puc += 4;
	}

	for (  i = 0; i < nNumOfRawData; i++ )
	{
		if ( pRawData[i] < 0 || pRawData[i] >= 4096 )
		{
			pColorOfRawData[i] = (short) RGB(255,255,255);
		}
		else
		{
			pColorOfRawData[i] =  pColorTable[ pRawData[i] ];
		}
	}

	delete []pColorTable;
	pColorTable = NULL;
}

void CMRIImage::GetColorTable( int nWidth, int nLevel, int* pColorTable )
{
	int i;
	int nMaxValue = 255, nMinValue = 0;
	int nMinKey = 0, m_nMaxKey = 4095;

	int nLow = nLevel - (nWidth - 1) / 2;
	int nHigh = nLevel + nWidth / 2;

	if (nLow >= 0)
	{
		memset( pColorTable, 0, sizeof (int) * (nLow + 1) );
	}

	COLORREF clrWhite = RGB(255, 255, 255);
	for ( i = nHigh; i <= m_nMaxKey; i++)
	{
		pColorTable[i] = clrWhite;
	}

	double a, b;
	a = (nMaxValue - nMinValue)/static_cast<double>(nHigh - nLow);
	b = (nHigh * static_cast<double>(nMinValue) - nLow * static_cast<double>(nMaxValue))/static_cast<double>(nHigh - nLow);

	int nStart = max (nLow, 0);
	int nEnd = min (nHigh, m_nMaxKey);

	unsigned char* puc = 
	reinterpret_cast<unsigned char*>(pColorTable + nStart);

	for (i = nStart; i < nEnd; i++)
	{
		*(puc+2) = *(puc+1) = *puc = static_cast<unsigned char>(a * i + b);
		puc += 4;
	}
}


bool CMRIImage::IsInCircle( int nPointX, int nPointY, int nCircleCenterX, int nCircleCenterY, int nCircleRadius )
{
	double dDistanceX = (double) nPointX - (double) nCircleCenterX;
	double dDistanceY = (double) nPointY - (double) nCircleCenterY;
	double dSquaredDistance = dDistanceX*dDistanceX+dDistanceY*dDistanceY;
	double dSquaredRadius = (double) nCircleRadius * (double) nCircleRadius;

	return ( dSquaredDistance <= dSquaredRadius );
}



CString CMRIImage::GetImageName()
{
	return m_sImgName;
}

void CMRIImage::SetImageName(CString sName)
{
	m_sImgName=sName;
}

char** CMRIImage::GetOrientChar()
{
	return m_ppcOrientation;
}

CString* CMRIImage::GetDispOrientChar()
{
	return m_sDispOri;
}

void CMRIImage::ComputeOrientChar(unsigned char PatEntry,unsigned char PatPosition)
{
	char sOrient[6];
	if (m_pSeries==NULL || NULL == m_pSeries->GetNewBoxOrientChar() )
	{
		
		if ( beHeadFirst == PatEntry )
		{
			switch( PatPosition )
			{
			case bpSupine: //仰卧
				{
					sOrient[0] = 'L';  //+x
					sOrient[1] = 'R';  //-x
					sOrient[2] = 'S';  //+y
					sOrient[3] = 'I';  //-y
					sOrient[4] = 'A';  //+z
					sOrient[5] = 'P';  //-z
				}
				break;
			case bpProne:
				{
					sOrient[0] = 'R';
					sOrient[1] = 'L';
					sOrient[2] = 'S';
					sOrient[3] = 'I';
					sOrient[4] = 'P';
					sOrient[5] = 'A';
				}
				break;
			case bpDecubitusRight:
				{
					sOrient[0] = 'P';
					sOrient[1] = 'A';
					sOrient[2] = 'S';
					sOrient[3] = 'I';
					sOrient[4] = 'L';
					sOrient[5] = 'R';
				}
			    break;
			case bpDecubitusLeft:
				{
					sOrient[0] = 'A';
					sOrient[1] = 'P';
					sOrient[2] = 'S';
					sOrient[3] = 'I';
					sOrient[4] = 'R';
					sOrient[5] = 'L';
				}
			    break;
			default:
			    break;
			}
		} 
		else
		{
			switch( PatPosition )
			{
			case bpSupine:
				{
					sOrient[0] = 'R';
					sOrient[1] = 'L';
					sOrient[2] = 'I';
					sOrient[3] = 'S';
					sOrient[4] = 'A';
					sOrient[5] = 'P';
				}
				break;
			case bpProne:
				{
					sOrient[0] = 'L';
					sOrient[1] = 'R';
					sOrient[2] = 'I';
					sOrient[3] = 'S';
					sOrient[4] = 'P';
					sOrient[5] = 'A';
				}
				break;
			case bpDecubitusRight:
				{
					sOrient[0] = 'A';
					sOrient[1] = 'P';
					sOrient[2] = 'I';
					sOrient[3] = 'S';
					sOrient[4] = 'L';
					sOrient[5] = 'R';
				}
				break;
			case bpDecubitusLeft:
				{
					sOrient[0] = 'P';
					sOrient[1] = 'A';
					sOrient[2] = 'I';
					sOrient[3] = 'S';
					sOrient[4] = 'R';
					sOrient[5] = 'L';
				}
				break;
			default:
				break;
			}
		}
		if(m_pSeries)
		{
			m_pSeries->SetNewBoxOrientChar( sOrient );
		}
	}
	// End of compute box orient char

	// Calculate the char corresponding to the edge
	Matrix mFreq(4,4);
	mFreq.Identity();
	mFreq.value[0][0]=m_dFreqDirectionInScanner[0];
	mFreq.value[1][0]=m_dFreqDirectionInScanner[1];
	mFreq.value[2][0]=m_dFreqDirectionInScanner[2];
	
	Matrix mPhase(4,4);
	mPhase.Identity();
	mPhase.value[0][0]=m_dPhaseDirectionInScanner[0];
	mPhase.value[1][0]=m_dPhaseDirectionInScanner[1];
	mPhase.value[2][0]=m_dPhaseDirectionInScanner[2];
	
	Vector3D vAB( mFreq.value[0][0], mFreq.value[1][0], mFreq.value[2][0] );
	Vector3D vAD( mPhase.value[0][0], mPhase.value[1][0], mPhase.value[2][0] );
	
	// Edge 0, 2
	Vector3D vEdgeDir = -vAD;
	double dFactor[3], dAbsFactor[3], dTmpFactor;
	int nAxis[3] = { 0, 1, 2 }, nTmpAxis;
	dFactor[0] = vEdgeDir.x;// * vXAxis;
	if (fabs(dFactor[0]) < TOLER)
	{
		dFactor[0] = 0.0;
	}
	dFactor[1] = vEdgeDir.y;// * vYAxis;
	if (fabs(dFactor[1]) < TOLER)
	{
		dFactor[1] = 0.0;
	}
	dFactor[2] = vEdgeDir.z;// * vZAxis;
	if (fabs(dFactor[2]) < TOLER)
	{
		dFactor[2] = 0.0;
	}
	dAbsFactor[0] = fabs(dFactor[0]);
	dAbsFactor[1] = fabs(dFactor[1]);
	dAbsFactor[2] = fabs(dFactor[2]);

	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	} 
	if (dAbsFactor[1] < dAbsFactor[2])
	{
		dTmpFactor = dAbsFactor[1];
		dAbsFactor[1] = dAbsFactor[2];
		dAbsFactor[2] = dTmpFactor;
		nTmpAxis = nAxis[1];
		nAxis[1] = nAxis[2];
		nAxis[2] = nTmpAxis;
	}
	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	}

	char* pcBoxChar = NULL;
	if(m_pSeries)
	{
		pcBoxChar=m_pSeries->GetNewBoxOrientChar();
	}else
	{
		pcBoxChar=sOrient;
	}
	// char cOrient[4][10];
	if (dFactor[nAxis[0]] > 0)
	{
		sprintf( m_ppcOrientation[0], "%c", pcBoxChar[2*nAxis[0]] );
		sprintf( m_ppcOrientation[2], "%c", pcBoxChar[2*nAxis[0]+1] );
	} 
	else if (dFactor[nAxis[0]] < 0)
	{
		sprintf( m_ppcOrientation[0], "%c", pcBoxChar[2*nAxis[0]+1] );
		sprintf( m_ppcOrientation[2], "%c", pcBoxChar[2*nAxis[0]] );
	}
	if (dFactor[nAxis[1]] > 0)
	{
		sprintf( &m_ppcOrientation[0][1], "%c", pcBoxChar[2*nAxis[1]] );
		sprintf( &m_ppcOrientation[2][1], "%c", pcBoxChar[2*nAxis[1]+1] );
	} 
	else if (dFactor[nAxis[1]] < 0)
	{
		sprintf( &m_ppcOrientation[0][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		sprintf( &m_ppcOrientation[2][1], "%c", pcBoxChar[2*nAxis[1]] );
	}
	if (dFactor[nAxis[2]] > 0)
	{
		sprintf( &m_ppcOrientation[0][2], "%c", pcBoxChar[2*nAxis[2]] );
		sprintf( &m_ppcOrientation[2][2], "%c", pcBoxChar[2*nAxis[2]+1] );
	} 
	else if (dFactor[nAxis[2]] < 0)
	{
		sprintf( &m_ppcOrientation[0][2], "%c", pcBoxChar[2*nAxis[2]+1] );
		sprintf( &m_ppcOrientation[2][2], "%c", pcBoxChar[2*nAxis[2]] );
	}

	// Edge 1,3
	nAxis[0] = 0;
	nAxis[1] = 1;
	nAxis[2] = 2;
	vEdgeDir = vAB;
	dFactor[0] = vEdgeDir.x;// * vXAxis;
	if (fabs(dFactor[0]) < TOLER)
	{
		dFactor[0] = 0.0;
	}
	dFactor[1] = vEdgeDir.y;// * vYAxis;
	if (fabs(dFactor[1]) < TOLER)
	{
		dFactor[1] = 0.0;
	}
	dFactor[2] = vEdgeDir.z;// * vZAxis;
	if (fabs(dFactor[2]) < TOLER)
	{
		dFactor[2] = 0.0;
	}
	dAbsFactor[0] = fabs(dFactor[0]);
	dAbsFactor[1] = fabs(dFactor[1]);
	dAbsFactor[2] = fabs(dFactor[2]);

	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	} 
	if (dAbsFactor[1] < dAbsFactor[2])
	{
		dTmpFactor = dAbsFactor[1];
		dAbsFactor[1] = dAbsFactor[2];
		dAbsFactor[2] = dTmpFactor;
		nTmpAxis = nAxis[1];
		nAxis[1] = nAxis[2];
		nAxis[2] = nTmpAxis;
	}
	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	}
	
	if (dFactor[nAxis[0]] > 0)
	{
		sprintf( m_ppcOrientation[1], "%c", pcBoxChar[2*nAxis[0]] );
		sprintf( m_ppcOrientation[3], "%c", pcBoxChar[2*nAxis[0]+1] );
	} 
	else if (dFactor[nAxis[0]] < 0)
	{
		sprintf( m_ppcOrientation[1], "%c", pcBoxChar[2*nAxis[0]+1] );
		sprintf( m_ppcOrientation[3], "%c", pcBoxChar[2*nAxis[0]] );
	}
	if (dFactor[nAxis[1]] > 0)
	{
		sprintf( &m_ppcOrientation[1][1], "%c", pcBoxChar[2*nAxis[1]] );
		sprintf( &m_ppcOrientation[3][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		if ( fabs(fabs(dFactor[nAxis[0]])-fabs(dFactor[nAxis[1]])) < TOLER )
		{
			if ( m_ppcOrientation[1][0] == m_ppcOrientation[0][0] 
				|| m_ppcOrientation[1][0] == m_ppcOrientation[2][0] )
			{
				char cTmp;
				cTmp = m_ppcOrientation[1][0];
				m_ppcOrientation[1][0] = m_ppcOrientation[1][1];
				m_ppcOrientation[1][1] = cTmp;
				cTmp = m_ppcOrientation[3][0];
				m_ppcOrientation[3][0] = m_ppcOrientation[3][1];
				m_ppcOrientation[3][1] = cTmp;
			}
		}
	} 
	else if (dFactor[nAxis[1]] < 0)
	{
		sprintf( &m_ppcOrientation[1][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		sprintf( &m_ppcOrientation[3][1], "%c", pcBoxChar[2*nAxis[1]] );
		if ( fabs(fabs(dFactor[nAxis[0]])-fabs(dFactor[nAxis[1]])) < TOLER )
		{
			if ( m_ppcOrientation[1][0] == m_ppcOrientation[0][0] 
				|| m_ppcOrientation[1][0] == m_ppcOrientation[2][0] )
			{
				char cTmp;
				cTmp = m_ppcOrientation[1][0];
				m_ppcOrientation[1][0] = m_ppcOrientation[1][1];
				m_ppcOrientation[1][1] = cTmp;
				cTmp = m_ppcOrientation[3][0];
				m_ppcOrientation[3][0] = m_ppcOrientation[3][1];
				m_ppcOrientation[3][1] = cTmp;
			}
		}
	}
	if (dFactor[nAxis[2]] > 0)
	{
		sprintf( &m_ppcOrientation[1][2], "%c", pcBoxChar[2*nAxis[2]] );
		sprintf( &m_ppcOrientation[3][2], "%c", pcBoxChar[2*nAxis[2]+1] );
	} 
	else if (dFactor[nAxis[2]] < 0)
	{
		sprintf( &m_ppcOrientation[1][2], "%c", pcBoxChar[2*nAxis[2]+1] );
		sprintf( &m_ppcOrientation[3][2], "%c", pcBoxChar[2*nAxis[2]] );
	}
}

double** CMRIImage::GetOrientCharPos()
{
	return m_ppdOrientCharPos;
}

void CMRIImage::ComputeOrientCharPosition()
{
	Vector3D vCenter( m_dCenter[0], m_dCenter[1], m_dCenter[2] );
	Vector3D vXVector( m_dFreqDirection[0]*m_dFOV[0], m_dFreqDirection[1]*m_dFOV[0], m_dFreqDirection[2]*m_dFOV[0] );
	Vector3D vYVector( m_dPhaseDirection[0]*m_dFOV[1], m_dPhaseDirection[1]*m_dFOV[1], m_dPhaseDirection[2]*m_dFOV[1] );
	Vector3D vPosition;

	// Char 0
	vPosition = vCenter - 0.5 * vYVector;
	m_ppdOrientCharPos[0][0] = vPosition.x;
	m_ppdOrientCharPos[0][1] = vPosition.y;
	m_ppdOrientCharPos[0][2] = vPosition.z;
	// Char 1
	vPosition = vCenter + 0.5 * vXVector;
	m_ppdOrientCharPos[1][0] = vPosition.x;
	m_ppdOrientCharPos[1][1] = vPosition.y;
	m_ppdOrientCharPos[1][2] = vPosition.z;
	// Char 2
	vPosition = vCenter + 0.5 * vYVector;
	m_ppdOrientCharPos[2][0] = vPosition.x;
	m_ppdOrientCharPos[2][1] = vPosition.y;
	m_ppdOrientCharPos[2][2] = vPosition.z;
	// Char 3
	vPosition = vCenter - 0.5 * vXVector;
	m_ppdOrientCharPos[3][0] = vPosition.x;
	m_ppdOrientCharPos[3][1] = vPosition.y;
	m_ppdOrientCharPos[3][2] = vPosition.z;
}
 

//计算在2D显示时图像应该如何旋转和反转
//才能满足以下显示条件
/*
  A         S         S
R   L     R   L     A   P
  P         I         I
*/
void CMRIImage::CorrectImageDispOrientation()
{
	char** sOrientChar;
	sOrientChar = GetOrientChar();
	BOOL bA = FALSE;
	BOOL bS = FALSE;
	BOOL bR = FALSE;
	int iSPos, iIPos, iLPos, iRPos, iAPos, iPPos;
	for ( int j = 0; j < 4; j++ )
	{
		if (sOrientChar[j][0] == 'A')
		{
			bA = TRUE;
			iAPos = j;
		}
		if (sOrientChar[j][0] == 'P')
		{
			iPPos = j;
		}
		if (sOrientChar[j][0] == 'S')
		{
			bS = TRUE;
			iSPos = j;
		}
		if (sOrientChar[j][0] == 'I')
		{
			iIPos = j;
		}
		if (sOrientChar[j][0] == 'R')
		{
			bR = TRUE;
			iRPos = j;
		}
		if (sOrientChar[j][0] == 'L')
		{
			iLPos = j;
		}
	}

	BOOL bHFlip = FALSE;
	BOOL bVFlip = FALSE;
	double dbAngle = 0;
	if (bR && bS)
	{
		if ( (iRPos + 1)%4 != iSPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iLPos - 1) * (bHFlip ? -1 : 1);
	}
	if (bA && bR)
	{
		if ( (iRPos + 1)%4 != iAPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iLPos - 1) * (bHFlip ? -1 : 1);
	}
	else if (bA && bS)
	{
		if ( (iAPos + 1)%4 != iSPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iPPos - 1) * (bHFlip ? -1 : 1);
	}
	RotateAndFlip(dbAngle, bHFlip, bVFlip);

	/*计算频率编码方向
	  缺省编码方向是从左到右
	
	fdLeftToRight,		// →
	fdTopToBottom,		// ↓
	fdRightToLeft,		// ←
	fdBottomToTop,		// ↑
	*/
	m_bLeftRight=fdLeftToRight;

	// change orientation char
	for (int i=0; i<4; i++)
	{
		m_sDispOri[i] = CString(m_ppcOrientation[i]);
	}
	CString sTmp;
	/*两个double型变量做比较，不能直接使用==操作符*/
	if (fabs(dbAngle-PI/2)<TOLER || fabs(dbAngle+PI*3/2)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[2];
		m_sDispOri[2] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		m_bLeftRight=fdBottomToTop;
		
	}
	else if (fabs(dbAngle-PI)<TOLER || fabs(dbAngle+PI)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[2];
		m_sDispOri[2] = sTmp;
		sTmp = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		m_bLeftRight=fdRightToLeft;
	}
	else if (fabs(dbAngle-PI*3/2)<TOLER || fabs(dbAngle+PI/2)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[3];
		m_sDispOri[3] = m_sDispOri[2];
		m_sDispOri[2] = m_sDispOri[1];
		m_sDispOri[1] = sTmp;
		m_bLeftRight=fdTopToBottom;
	}

	if (bHFlip)
	{
		CString sTmp;
		sTmp = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		if(m_bLeftRight == fdRightToLeft)
		{
			m_bLeftRight = fdLeftToRight;
		}else if(m_bLeftRight == fdLeftToRight)
		{
			m_bLeftRight = fdRightToLeft;
		}
	}
	if (bVFlip)
	{
		CString sTmp;
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[2];
		m_sDispOri[2] = sTmp;
		if(m_bLeftRight == fdBottomToTop)
		{
			m_bLeftRight = fdTopToBottom;
		}else if(m_bLeftRight == fdTopToBottom)
		{
			m_bLeftRight = fdBottomToTop;
		}
	}
}

void CMRIImage::RotateAndFlip()
{
	int iOriginWidth = GetResolutionX();
	int iOriginHeight = GetResolutionY();
	
	int iRotateWidth,iRotateHeight; 
	/*m_dAngle=1.0*PI;
	iRotateWidth = fabs(cos(m_dAngle)) * iOriginWidth + fabs(sin(m_dAngle)) * iOriginHeight + 0.5;

	if (iRotateWidth % 4 != 0)
		iRotateWidth += (4 - iRotateWidth % 4);
	iRotateHeight = iRotateWidth; // only square image are processed*/
	if (fabs(fabs(m_dAngle)-PI*0.5)<TOLER || fabs(fabs(m_dAngle)-PI*1.5)<TOLER)
	{
		iRotateHeight=iOriginWidth;
		iRotateWidth=iOriginHeight;
		m_dRotatedFOV[0]=m_dFOV[1];
		m_dRotatedFOV[1]=m_dFOV[0];
	}else
	{
		iRotateWidth=iOriginWidth;
		iRotateHeight=iOriginHeight;
		m_dRotatedFOV[0]=m_dFOV[0];
		m_dRotatedFOV[1]=m_dFOV[1];
	}

	m_usRotatedResolution[0]=iRotateWidth;
	m_usRotatedResolution[1]=iRotateHeight;

	if (m_pRotatedData)
	{
		delete m_pRotatedData;
	}
	m_pRotatedData = new IMG_DATA_TYPE[iRotateWidth * iRotateHeight]; // * sizeof(IMG_DATA_TYPE)];

	// rotate
    if (fabs(m_dAngle) < 1.0e-6)
    {
        
		memcpy(m_pRotatedData, m_pImage->pData, iOriginWidth * iOriginHeight * sizeof(IMG_DATA_TYPE));
    }
    else
    {
		CDataProcessor::Rotate(m_pRotatedData, iRotateWidth, iRotateHeight,
		    m_pImage->pData, iOriginWidth, iOriginHeight, m_dAngle);
    }


    // flip in place
	CSize rotateSize;
	rotateSize.cx = iRotateWidth;
	rotateSize.cy = iRotateHeight;
    CDataProcessor::Flip(m_pRotatedData, rotateSize, m_bHFlip, m_bVFlip); 
	
}

void CMRIImage::RotateAndFlip(double dAngle, BOOL bHFlip, BOOL bVFlip)
{
	m_dAngle=dAngle;
	m_bHFlip=bHFlip;
	m_bVFlip=bVFlip;

	int iOriginWidth = GetResolutionX();
	int iOriginHeight = GetResolutionY();

	
	for (int i=0; i<3; i++)
	{
		m_dFreqDirRotInScanner[i] = m_dFreqDirectionInScanner[i];
		m_dPhaseDirRotInScanner[i] = m_dPhaseDirectionInScanner[i];
		m_dSliceDirRotInScanner[i] = m_dSliceDirectionInScanner[i];
	}

    // rotate
    if (fabs(dAngle) < 1.0e-6)
    {
        
		//memcpy(m_pRotatedData, m_pImage->pData, iOriginWidth * iOriginHeight * sizeof(IMG_DATA_TYPE));
    }
    else
    {
		//CDataProcessor::Rotate(m_pRotatedData, iRotateWidth, iRotateHeight,
		    //m_pImage->pData, iOriginWidth, iOriginHeight, dAngle);

		Vector3D vAxis( m_dSliceDirection[0], m_dSliceDirection[1], m_dSliceDirection[2] );
		Vector3D vTmp;
		double dRadians = dAngle;
		// Center
		vTmp.x = m_dCenter[0];
		vTmp.y = m_dCenter[1];
		vTmp.z = m_dCenter[2];
		vAxis.Rotate( vTmp, dRadians );
		m_dCenter[0] = vTmp.x;
		m_dCenter[1] = vTmp.y;
		m_dCenter[2] = vTmp.z;
		// Phase direction
		vTmp.x = m_dPhaseDirRotInScanner[0];
		vTmp.y = m_dPhaseDirRotInScanner[1];
		vTmp.z = m_dPhaseDirRotInScanner[2];
		vAxis.Rotate( vTmp, dRadians );
		m_dPhaseDirRotInScanner[0] = vTmp.x;
		m_dPhaseDirRotInScanner[1] = vTmp.y;
		m_dPhaseDirRotInScanner[2] = vTmp.z;
		// Freq direction
		vTmp.x = m_dFreqDirRotInScanner[0];
		vTmp.y = m_dFreqDirRotInScanner[1];
		vTmp.z = m_dFreqDirRotInScanner[2];
		vAxis.Rotate( vTmp, dRadians );
		m_dFreqDirRotInScanner[0] = vTmp.x;
		m_dFreqDirRotInScanner[1] = vTmp.y;
		m_dFreqDirRotInScanner[2] = vTmp.z;   
    }

    // flip in place
	//CSize rotateSize;
	//rotateSize.cx = iRotateWidth;
	//rotateSize.cy = iRotateHeight;
    //CDataProcessor::Flip(m_pRotatedData, rotateSize, bHFlip, bVFlip); 

	if (bVFlip)
	{
		Vector3D vAxis( m_dFreqDirRotInScanner[0], m_dFreqDirRotInScanner[1], m_dFreqDirRotInScanner[2] );
		Vector3D vTmp;
		double dRadians = PI;
		// Phase direction
		vTmp.x = m_dPhaseDirRotInScanner[0];
		vTmp.y = m_dPhaseDirRotInScanner[1];
		vTmp.z = m_dPhaseDirRotInScanner[2];
		vAxis.Rotate( vTmp, dRadians );
		m_dPhaseDirRotInScanner[0] = vTmp.x;
		m_dPhaseDirRotInScanner[1] = vTmp.y;
		m_dPhaseDirRotInScanner[2] = vTmp.z; 
		
		m_dSliceDirRotInScanner[0] = -m_dSliceDirRotInScanner[0];
		m_dSliceDirRotInScanner[1] = -m_dSliceDirRotInScanner[1];
		m_dSliceDirRotInScanner[2] = -m_dSliceDirRotInScanner[2];
	} 
	if (bHFlip)
	{
		Vector3D vAxis( m_dPhaseDirRotInScanner[0], m_dPhaseDirRotInScanner[1], m_dPhaseDirRotInScanner[2] );
		Vector3D vTmp;
		double dRadians = PI;
		// Phase direction
		vTmp.x = m_dFreqDirRotInScanner[0];
		vTmp.y = m_dFreqDirRotInScanner[1];
		vTmp.z = m_dFreqDirRotInScanner[2];
		vAxis.Rotate( vTmp, dRadians );
		m_dFreqDirRotInScanner[0] = vTmp.x;
		m_dFreqDirRotInScanner[1] = vTmp.y;
		m_dFreqDirRotInScanner[2] = vTmp.z; 
		
		m_dSliceDirRotInScanner[0] = -m_dSliceDirRotInScanner[0];
		m_dSliceDirRotInScanner[1] = -m_dSliceDirRotInScanner[1];
		m_dSliceDirRotInScanner[2] = -m_dSliceDirRotInScanner[2];
	}  
}



MR_ImageHeader* CMRIImage::GetImageHeader()
{
	return &(m_pImage->oHeader);
}

IGT_StudyField CMRIImage::GetIGTStudyField()
{
	IGT_StudyField studyField;

	memset(&studyField,0,sizeof(IGT_StudyField));

	if(m_pImage==NULL || m_pImage->pData==NULL)
		return studyField;

	if(m_eImgSource == IMS_DCMCT)
	{
		
		sprintf(studyField.Id, "%s", ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Id);
		sprintf(studyField.BodyPart, "%s", ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.BodyPart);
		sprintf(studyField.Desc, "%s", ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Desc);
		studyField.Time.year = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.year;
		studyField.Time.month = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.month;
		studyField.Time.day = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.day;
		studyField.Time.hour = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.hour;
		studyField.Time.minute = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.minute;
		studyField.Time.second = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.second;
	}else if(m_eImgSource == IMS_DCMUS)
	{
		
		sprintf(studyField.Id, "%s", ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Id);
		sprintf(studyField.BodyPart, "%s", ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.BodyPart);
		sprintf(studyField.Desc, "%s", ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Desc);
		studyField.Time.year = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.year;
		studyField.Time.month = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.month;
		studyField.Time.day = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.day;
		studyField.Time.hour = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.hour;
		studyField.Time.minute = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.minute;
		studyField.Time.second = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.studyField.Time.second;
	}else
	{
		sprintf(studyField.Id, "%s", m_pImage->oHeader.studyField.Id);
		sprintf(studyField.BodyPart, "%s", m_pImage->oHeader.studyField.BodyPart);
		sprintf(studyField.Desc, "%s", m_pImage->oHeader.studyField.Desc);
		studyField.Time.year = m_pImage->oHeader.studyField.Time.year;
		studyField.Time.month = m_pImage->oHeader.studyField.Time.month;
		studyField.Time.day = m_pImage->oHeader.studyField.Time.day;
		studyField.Time.hour = m_pImage->oHeader.studyField.Time.hour;
		studyField.Time.minute = m_pImage->oHeader.studyField.Time.minute;
		studyField.Time.second = m_pImage->oHeader.studyField.Time.second;
	}

	return studyField;


}

IGT_SeriesField CMRIImage::GetIGTSeriesField()
{
	IGT_SeriesField seriesField;
	memset(&seriesField,0,sizeof(IGT_SeriesField));

	if(m_pImage==NULL || m_pImage->pData==NULL)
		return seriesField;

	
	if(m_eImgSource == IMS_DCMCT)
	{
		
		seriesField.Id = ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Id;
		sprintf(seriesField.Desc, "%s", ((CT_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Desc);
		sprintf(seriesField.Protocol, "");
	}else if(m_eImgSource == IMS_DCMUS)
	{
		
		seriesField.Id = ((US_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Id;
		sprintf(seriesField.Desc, "%s", ((US_STRUCT_IMAGE *)m_pImage)->oHeader.seriesField.Desc);
		sprintf(seriesField.Protocol, "");
	}else
	{
		seriesField.Id = m_pImage->oHeader.seriesField.Id;
		sprintf(seriesField.Desc, "%s", m_pImage->oHeader.seriesField.Desc);
		sprintf(seriesField.Protocol, "%s", m_pImage->oHeader.seriesField.Protocol);
	}


	return seriesField;
}

ENUM_CODING_DIR_ON_2D_IMAGE CMRIImage::GetFreqFlag()
{
	return m_bLeftRight;
}

void CMRIImage::DisableFreeMemoryOnDestroy()
{
	m_bFreeMemoryOnDestroy=FALSE;
}


