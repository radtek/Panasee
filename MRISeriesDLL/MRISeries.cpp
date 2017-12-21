// MRISeries.cpp: implementation of the CMRISeries class.
// Created by Wei Wei [Mar.13, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRISeries.h"
#include "Geometry.h"
#include "MV.h"
#include "ErrorCodes.h"
#pragma warning (disable:4786)
#include <fstream>
#include <exception>
#include <iostream>
#include <cassert>
#include <strstrea.h>
#include "DataAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRISeries::CMRISeries()
{
	m_nNumOfImages = 0;
	m_pMRIImage = NULL;
	m_iVisibleCnt = 0;
	m_psNewBoxOrientation = NULL;
	m_nWindowWidth = 4096;
	m_nWindowLevel = 2048;

	m_nIMRIACPHiVer = 0;
	m_nIMRIACPLoVer = 0;
	m_sFolder="";
	
	m_pImagesMemoryObject=NULL;
	m_imageInScanner.SetDimension(4,4);
	m_dPixelSpacing[0]=1;
	m_dPixelSpacing[1]=1;
	m_dPixelSpacing[2]=1;

	//单位化融合矩阵
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(i==j)
			{
				m_dFusionCoord[i][j]=1;
			}else
			{
				m_dFusionCoord[i][j]=0;
			}
		}
	}

	m_dFreqDirectionInScanner[0]=m_dPhaseDirectionInScanner[0]=m_dSliceDirectionInScanner[0]=1;
	m_dFreqDirectionInScanner[1]=m_dPhaseDirectionInScanner[1]=m_dSliceDirectionInScanner[1]=0;
	m_dFreqDirectionInScanner[2]=m_dPhaseDirectionInScanner[2]=m_dSliceDirectionInScanner[2]=0;
	m_dCenterInScanner[0]=m_dCenterInScanner[1]=m_dCenterInScanner[2]=0;

}

CMRISeries::~CMRISeries()
{	

	if(m_pImagesMemoryObject!=NULL)
	{
		if(m_pImagesMemoryObject->Count()==1)
		{
			delete m_pImagesMemoryObject;

		}else
		{
			m_pImagesMemoryObject->Delete();
		}
	}

	if ( NULL != m_pMRIImage )
	{
		for(int i=0;i<m_nNumOfImages;i++)
			delete m_pMRIImage[i];
		delete []m_pMRIImage;
		m_pMRIImage = NULL;
	}
	if ( NULL != m_psNewBoxOrientation )
	{
		delete []m_psNewBoxOrientation;
		m_psNewBoxOrientation = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////

int CMRISeries::GetSeriesID()
{
	return m_nSeriesID;
}

int CMRISeries::GetScanID()
{
	return m_nScanID;
}

CString CMRISeries::GetStudyID()
{
	return m_sStudyID;
}


CString CMRISeries::GetName()
{
	return m_sName;
}

void CMRISeries::SetName(CString sName)
{
	m_sName=sName;
}

double* CMRISeries::GetSliceDirection()
{
	return m_dSliceDirection;
}

double* CMRISeries::GetPhaseDirection()
{
	return m_dPhaseDirection;
}

double* CMRISeries::GetFreqDirection()
{
	return m_dFreqDirection;
}


void CMRISeries::Transformation( double** ppdTrans )
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

	// Transformation of each image
	for ( i = 0; i < m_nNumOfImages; i++ )
		m_pMRIImage[i]->Transformation( ppdTrans );

}

void CMRISeries::Translation( double dTranslation[3] )
{
	Vector3D vTmp(dTranslation[0], dTranslation[1], dTranslation[2]);

	// Center
	m_dCenter[0] += dTranslation[0];
	m_dCenter[1] += dTranslation[1];
	m_dCenter[2] += dTranslation[2];

	
	// Translation of each image
	for ( int i = 0; i < m_nNumOfImages; i++ )
	{
		m_pMRIImage[i]->Translation( dTranslation );
	}

}

void CMRISeries::Rotation( double** ppdRot )
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
	// Images
	for ( int i = 0; i < m_nNumOfImages; i++ )
		m_pMRIImage[i]->Rotation( ppdRot );

}

void CMRISeries::Rotation( double dAxis[3], double dAngle )
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
	// Images
	for ( int i = 0; i < m_nNumOfImages; i++ )
		m_pMRIImage[i]->Rotation( dAxis, dAngle );

}


void CMRISeries::ChangeWindowWidthAndLevel( int nWidthShift, int nLevelShift )
{
	UpdateOClut(m_nWindowWidth + nWidthShift, m_nWindowLevel + nLevelShift);
		
}

int CMRISeries::GetNumOfImages()
{
	return m_nNumOfImages;
}


CMRIImage* CMRISeries::GetImageAt( int nIndex )
{
	if ( 0 <= nIndex && nIndex < m_nNumOfImages)
		return (m_pMRIImage[nIndex]);
	else
		return NULL;
}


int CMRISeries::Load( CString sFilePath )
{
	
	CDataAccess myDataAccess;
	vImagesContainer myImagesContainer;
	myDataAccess.ReadImagesDataOfScan( &myImagesContainer, sFilePath );
	int nNumOfImages = (int) myImagesContainer.size();
	
	if ( 0 >= nNumOfImages )
		return ERR_FAIL_TO_LOAD_IMAGE;

	if ( NULL != m_pMRIImage )
	{
		for(int i=0;i<m_nNumOfImages;i++)
			delete m_pMRIImage[i];
		delete []m_pMRIImage;
		m_pMRIImage = NULL;
	}

	m_nNumOfImages=nNumOfImages;
	m_pMRIImage = new CMRIImage*[m_nNumOfImages];

	vImagesContainer::iterator it;
	MR_STRUCT_IMAGE image;
	int i;
	for ( i = 0, it = myImagesContainer.begin(); it != myImagesContainer.end(); it++, i++ )
	{
		image = *it;
		m_pMRIImage[i]=new CMRIImage();
		m_pMRIImage[i]->m_pSeries = this;
		m_pMRIImage[i]->LoadFromIMGFile( &image.oHeader, image.pData );
		delete image.pData;
	}

	InitSeries();
	CorrectSliceDirection();
	
	m_sFolder = sFilePath;

	return ERR_OK;
}

int CMRISeries::LoadImg(CArray<CMRIImage*, CMRIImage*> &caImgs,CString sFilePath)
{
	m_nNumOfImages = 0;
	MR_STRUCT_IMAGE image;
	CDataAccess myDataAccess;
	CString FileName[4];

	if ( NULL != m_pMRIImage )
	{
		for(int i=0;i<m_nNumOfImages;i++)
			delete m_pMRIImage[i];
		delete []m_pMRIImage;
		m_pMRIImage = NULL;
	}
	
	for (int i=0;i<sFilePath.GetLength();i++)
	{
		if (sFilePath.GetAt(i) == '|')
		{
			FileName[m_nNumOfImages] = sFilePath.Left(i);
			sFilePath = sFilePath.Right(sFilePath.GetLength()-i-1);
			m_nNumOfImages++;
			i=0;
		}
	}

	if(m_nNumOfImages == 0 && !sFilePath.IsEmpty())
	{
		m_nNumOfImages = 1;
		FileName[0] = sFilePath;
	}

	m_pMRIImage = new CMRIImage*[m_nNumOfImages];
	for (int j=0;j<m_nNumOfImages;j++)
	{
		myDataAccess.ReadImageData(image,FileName[j]);
		m_pMRIImage[j]=new CMRIImage();
		m_pMRIImage[j]->m_pSeries = this;
		m_pMRIImage[j]->LoadFromIMGFile( &image.oHeader, image.pData );
		
		caImgs.Add(m_pMRIImage[j]);
		delete image.pData;

	}

	InitSeries();
	CorrectSliceDirection();
	
	return ERR_OK;
}

//从病人坐标系转换到设备坐标系
void CMRISeries::ConvertCoordFromPatient2Device(MR_STRUCT_IMAGE *pMRHeader)
{
	//return;
	double dImgCenterX,dImgCenterY,dImgCenterZ;
	BYTE iPatPos, iPatEntry;
	double dBIDirec[9],dX[3],dY[3],dZ[3];
	
	iPatPos=pMRHeader->oHeader.studyField.PatPosition;
	iPatEntry=pMRHeader->oHeader.studyField.PatEntry;
	
	dImgCenterX=pMRHeader->oHeader.imageField.fCenterPointX;
	dImgCenterY=pMRHeader->oHeader.imageField.fCenterPointY;
	dImgCenterZ=pMRHeader->oHeader.imageField.fCenterPointZ;
	dX[0]=pMRHeader->oHeader.imageField.fFreqDirX;
	dX[1]=pMRHeader->oHeader.imageField.fFreqDirY;
	dX[2]=pMRHeader->oHeader.imageField.fFreqDirZ;
	dY[0]=pMRHeader->oHeader.imageField.fPhaseDirX;
	dY[1]=pMRHeader->oHeader.imageField.fPhaseDirY;
	dY[2]=pMRHeader->oHeader.imageField.fPhaseDirZ;

	Vector vx(dX[0],dX[1],dX[2]);
	Vector vy(dY[0],dY[1],dY[2]);
	Vector vz;
	vz = vx * vy;
	dZ[0]=vz.value[0];
	dZ[1]=vz.value[1];
	dZ[2]=vz.value[2];
	
	
	//
	if (iPatEntry == beHeadFirst)
	{
		switch (iPatPos)
		{
		case bpProne:			
			dBIDirec[0] = -dImgCenterX ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] = -dImgCenterZ ;
			dBIDirec[3] = -dX[0];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] = -dX[2];
			dBIDirec[6] = -dY[0];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] = -dY[2];
			break ;
		case bpDecubitusRight:
			dBIDirec[0] = -dImgCenterZ ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] =  dImgCenterX ;
			dBIDirec[3] = -dX[2];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] =  dX[0];
			dBIDirec[6] = -dY[2];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] =  dY[0];
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] =  dImgCenterZ ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] = -dImgCenterX ;
			dBIDirec[3] =  dX[2];
			dBIDirec[4] =  dX[1];
			dBIDirec[5] = -dX[0];
			dBIDirec[6] =  dY[2];
			dBIDirec[7] =  dY[1];
			dBIDirec[8] = -dY[0];
			break ;
		case bpSupine:
		default:
			dBIDirec[0] =  dImgCenterX ;
			dBIDirec[1] =  dImgCenterY ;
			dBIDirec[2] =  dImgCenterZ ;
			dBIDirec[3] = dX[0];
			dBIDirec[4] = dX[1];
			dBIDirec[5] = dX[2];
			dBIDirec[6] = dY[0];
			dBIDirec[7] = dY[1];
			dBIDirec[8] = dY[2];
			break ;
		}
	}
	else
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] =  dImgCenterX ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] = -dImgCenterZ ;
			dBIDirec[3] =  dX[0];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] = -dX[2];
			dBIDirec[6] =  dY[0];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] = -dY[2];
			break ;
		case bpDecubitusRight:
			dBIDirec[0] =  dImgCenterZ ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] =  dImgCenterX ;
			dBIDirec[3] =  dX[2];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] =  dX[0];
			dBIDirec[6] =  dY[2];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] =  dY[0];
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] = -dImgCenterZ ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] = -dImgCenterX ;
			dBIDirec[3] = -dX[2];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] = -dX[0];
			dBIDirec[6] = -dY[2];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] = -dY[0];
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = -dImgCenterX ;
			dBIDirec[1] = -dImgCenterY ;
			dBIDirec[2] =  dImgCenterZ ;
			dBIDirec[3] = -dX[0];
			dBIDirec[4] = -dX[1];
			dBIDirec[5] =  dX[2];
			dBIDirec[6] = -dY[0];
			dBIDirec[7] = -dY[1];
			dBIDirec[8] =  dY[2];
			break ;
			
			
		}
	}
	vx.value[0]=dBIDirec[3];
	vx.value[1]=dBIDirec[4];
	vx.value[2]=dBIDirec[5];
	vy.value[0]=dBIDirec[6];
	vy.value[1]=dBIDirec[7];
	vy.value[2]=dBIDirec[8];
	vz = vx * vy;

	pMRHeader->oHeader.scanField.FreqDirX = vx.value[0];
	pMRHeader->oHeader.scanField.FreqDirY = vx.value[1];
	pMRHeader->oHeader.scanField.FreqDirZ = vx.value[2];
	pMRHeader->oHeader.scanField.PhaseDirX = vy.value[0];
	pMRHeader->oHeader.scanField.PhaseDirY = vy.value[1];
	pMRHeader->oHeader.scanField.PhaseDirZ = vy.value[2];
	pMRHeader->oHeader.scanField.SliceDirX = vz.value[0];
	pMRHeader->oHeader.scanField.SliceDirY = vz.value[1];
	pMRHeader->oHeader.scanField.SliceDirZ = vz.value[2];
	pMRHeader->oHeader.imageField.fCenterPointX = (float)dBIDirec[0] ;
	pMRHeader->oHeader.imageField.fCenterPointY = (float)dBIDirec[1] ;
	pMRHeader->oHeader.imageField.fCenterPointZ = (float)dBIDirec[2] ;
	
	
	
}

//此函数被Xinao核磁通讯协议使用
int CMRISeries::Load( char* pBuffer, int nLength, WORD nIMRIACPHiVer, WORD nIMRIACPLoVer  )
{
	m_nIMRIACPHiVer = nIMRIACPHiVer;
	m_nIMRIACPLoVer = nIMRIACPLoVer;

	// Added by Wei Wei [Aug.9, 2006]
	// - Begin
	char* pTempBuffer = pBuffer;
	short nIdOfSeries, nIdOfScan, nNumOfSlices;
	//memcpy( &m_nImageType, pTempBuffer, sizeof(short) );
	pTempBuffer += sizeof(short);
	memcpy( &nIdOfSeries, pTempBuffer, sizeof(short) );
	pTempBuffer += sizeof(short);
	memcpy( &nIdOfScan, pTempBuffer, sizeof(short) );
	pTempBuffer += sizeof(short);
	memcpy( &nNumOfSlices, pTempBuffer, sizeof(short) );
	pTempBuffer += sizeof(short) + 24;

	m_nSeriesID = (int) nIdOfSeries;
	
	if ( 0 >= nNumOfSlices )
		return ERR_FAIL_TO_LOAD_IMAGE;
	
	if ( NULL != m_pMRIImage )
	{
		for(int i=0;i<m_nNumOfImages;i++)
			delete m_pMRIImage[i];
		delete []m_pMRIImage;
		m_pMRIImage = NULL;
	}

	m_nNumOfImages = (int) nNumOfSlices;
	m_pMRIImage = new CMRIImage*[m_nNumOfImages];

	for ( int i = 0; i < m_nNumOfImages; i++ )
	{
		m_pMRIImage[i]=new CMRIImage();
		m_pMRIImage[i]->m_pSeries = this;
		
		if (m_nIMRIACPHiVer == 2)
		{
			pTempBuffer = pTempBuffer + 4;
		}
		MR_STRUCT_IMAGE* pImg = (MR_STRUCT_IMAGE*)pTempBuffer;
		int nLen = sizeof(MR_STRUCT_IMAGE);
		pTempBuffer += sizeof(MR_STRUCT_IMAGE) - 4;
		pImg->pData = (IMG_DATA_TYPE*)pTempBuffer;
		pImg->oHeader.headInfo.Version = IMG_VER_HEADER_1_2;
		if (strncmp(pImg->oHeader.headInfo.BITitle, "BIMRI", 5) == 0)
		{
			if(m_nIMRIACPHiVer==2 && m_nIMRIACPLoVer==3) //如果是2.3版MR通讯协议，则需要从imageField中获取图像方向
			{
				//FovY存放的是实际指定的Fov,
				//新的扫描图像要求Fov必须是正方形，所以以FovX为准 
				//来自王义槐
				pImg->oHeader.seriesField.FovY = pImg->oHeader.seriesField.FovX;
								
				ConvertCoordFromPatient2Device(pImg); //从病人坐标系转换到设备坐标系 
			}

		}
		m_pMRIImage[i]->LoadFromIMGFile(&(pImg->oHeader), pImg->pData);
		if(m_pMRIImage[i]->GetImageType() == IMS_DCMCT)
		{
			
			CT_STRUCT_IMAGE* pTmpImg = (CT_STRUCT_IMAGE*)pImg;
			pTempBuffer += sizeof(IMG_DATA_TYPE) * pTmpImg->oHeader.seriesField.ResolutionX * 
				           pTmpImg->oHeader.seriesField.ResolutionY;
		}else if(m_pMRIImage[i]->GetImageType() == IMS_DCMUS)
		{
			
			US_STRUCT_IMAGE* pTmpImg = (US_STRUCT_IMAGE*)pImg;
			pTempBuffer += sizeof(IMG_DATA_TYPE) * pTmpImg->oHeader.seriesField.ResolutionX * 
				           pTmpImg->oHeader.seriesField.ResolutionY;
		}else
		{
			pTempBuffer += sizeof(IMG_DATA_TYPE) * pImg->oHeader.seriesField.ResolutionX * pImg->oHeader.seriesField.ResolutionY;
		}
	}

	InitSeries();
	CorrectSliceDirection();
	
	return ERR_OK;
	// - End
} 

int CMRISeries::Load(CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> &caImgs)
{
	
	if(caImgs.GetSize()<=0)
		return -1;

	// Sort images
	CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> caTemp;
	int iMin = 1000;
	int iMinIdx;
	while (caImgs.GetSize() > 0) 
	{
		iMin = 100;
		iMinIdx = 0;
		for (int i=0; i<caImgs.GetSize(); i++)
		{
			MR_STRUCT_IMAGE* pImage=caImgs[i];
			int iID=0;
			int eImgSource;
			if (strncmp(pImage->oHeader.headInfo.BITitle, "BIMRI", 5) == 0)
			{
				eImgSource = 0;
			}
			else if(strncmp(pImage->oHeader.headInfo.BITitle, "DCMCT", 5) == 0)
			{
		
				eImgSource = 1;
			}else if(strncmp(pImage->oHeader.headInfo.BITitle, "DCMMR", 5) == 0)
			{
				eImgSource = 2;
			}else if(strncmp(pImage->oHeader.headInfo.BITitle, "DCMUS", 5) == 0)
			{
				eImgSource = 3;
			}else
			{
		
				eImgSource = 0;
			}
			if(eImgSource == 1)
			{
				
				CT_STRUCT_IMAGE* pCTImage = (CT_STRUCT_IMAGE*)pImage;
				iID = pCTImage->oHeader.imageField.Id;
			}else if(eImgSource == 3)
			{
				
				US_STRUCT_IMAGE* pUSImage = (US_STRUCT_IMAGE*)pImage;
				iID = pUSImage->oHeader.imageField.Id;
			}else
			{
				iID = pImage->oHeader.imageField.Id;
			}

			if (iID < iMin)
			{
				iMin = iID;
				iMinIdx = i;
			}
		}
		caTemp.Add(caImgs[iMinIdx]);
		caImgs.RemoveAt(iMinIdx);
	}
	caImgs.Append(caTemp);
	// end sort 

	if ( NULL != m_pMRIImage )
	{
		for(int i=0;i<m_nNumOfImages;i++)
			delete m_pMRIImage[i];
		delete []m_pMRIImage;
		m_pMRIImage = NULL;
	}
	m_nNumOfImages=caImgs.GetSize();
	m_pMRIImage = new CMRIImage*[m_nNumOfImages];

	for (int i=0; i<m_nNumOfImages; i++)
	{
		m_pMRIImage[i]=new CMRIImage();
		
		m_pMRIImage[i]->m_pSeries = this;
		m_pMRIImage[i]->LoadFromIMGFile(&(caImgs[i]->oHeader), caImgs[i]->pData);
	}

	InitSeries();
	CorrectSliceDirection();

	return ERR_OK;
} 

void CMRISeries::InitSeries()
{
	// Series ID
	m_nSeriesID = m_pMRIImage[0]->GetSeriesID();
	m_nScanID = m_pMRIImage[0]->GetScanID();
	m_sStudyID = m_pMRIImage[0]->GetStudyID();

	CString sImgName = m_pMRIImage[0]->GetImageName();
	m_sName = sImgName.Left(sImgName.ReverseFind('.'));

	// Geometry info
	
	// Slice direction
	double* pdDirection;
	pdDirection = m_pMRIImage[0]->GetSliceDirectionInScanner();
	m_dSliceDirection[0] = m_dSliceDirectionInScanner[0] = pdDirection[0];
	m_dSliceDirection[1] = m_dSliceDirectionInScanner[1] = pdDirection[1];
	m_dSliceDirection[2] = m_dSliceDirectionInScanner[2] = pdDirection[2];

	// Center
	double* pdImageCenter = m_pMRIImage[0]->GetCenterInScanner();
	Vector3D vImageCenter( pdImageCenter[0], pdImageCenter[1], pdImageCenter[2] );
	m_dCenter[0] = m_dCenterInScanner[0] = vImageCenter.x;
	m_dCenter[1] = m_dCenterInScanner[1] = vImageCenter.y;
	m_dCenter[2] = m_dCenterInScanner[2] = vImageCenter.z;

	// Phase direction
	pdDirection = m_pMRIImage[0]->GetPhaseDirectionInScanner();
	m_dPhaseDirection[0] = m_dPhaseDirectionInScanner[0] = pdDirection[0];
	m_dPhaseDirection[1] = m_dPhaseDirectionInScanner[1] = pdDirection[1];
	m_dPhaseDirection[2] = m_dPhaseDirectionInScanner[2] = pdDirection[2];

	// Freq direction
	pdDirection = m_pMRIImage[0]->GetFreqDirectionInScanner();
	m_dFreqDirection[0] = m_dFreqDirectionInScanner[0] = pdDirection[0];
	m_dFreqDirection[1] = m_dFreqDirectionInScanner[1] = pdDirection[1];
	m_dFreqDirection[2] = m_dFreqDirectionInScanner[2] = pdDirection[2];
		

	m_nWindowWidth = m_pMRIImage[0]->GetInterWindowWidth();
	m_nWindowLevel = m_pMRIImage[0]->GetInterWindowLevel();
	
	m_oClut32.SetWidth( m_nWindowWidth );
	m_oClut32.SetLevel( m_nWindowLevel );
	m_oClut32.Update();
	m_nWindowWidth = m_oClut32.GetWidth();
	m_nWindowLevel = m_oClut32.GetLevel();

	//calculate spacing
	m_dPixelSpacing[0] = m_pMRIImage[0]->GetFOVX()/m_pMRIImage[0]->GetResolutionX();
	m_dPixelSpacing[1] = m_pMRIImage[0]->GetFOVY()/m_pMRIImage[0]->GetResolutionY();
	if(m_nNumOfImages<=1)
	{
		m_dPixelSpacing[2] = m_pMRIImage[0]->GetThickness()+m_pMRIImage[0]->GetGap();
	}else
	{
		double *center1=m_pMRIImage[0]->GetCenterInScanner();
		double *center2=m_pMRIImage[1]->GetCenterInScanner();
		
		m_dPixelSpacing[2]=sqrt((center1[0]-center2[0])*(center1[0]-center2[0])+
			(center1[1]-center2[1])*(center1[1]-center2[1])+
			(center1[2]-center2[2])*(center1[2]-center2[2]));
	}
	
				
	//设置基座标矩阵
	double fEdgePosition[3];
	fEdgePosition[0] = m_dCenterInScanner[0]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[0]-m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[0];
	fEdgePosition[1] = m_dCenterInScanner[1]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[1]-m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[1];
	fEdgePosition[2] = m_dCenterInScanner[2]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[2]-m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[2];
	m_imageInScanner.Identity();
	m_imageInScanner.value[0][0]=m_dFreqDirectionInScanner[0];
	m_imageInScanner.value[1][0]=m_dFreqDirectionInScanner[1];
	m_imageInScanner.value[2][0]=m_dFreqDirectionInScanner[2];
	m_imageInScanner.value[0][1]=m_dPhaseDirectionInScanner[0];
	m_imageInScanner.value[1][1]=m_dPhaseDirectionInScanner[1];
	m_imageInScanner.value[2][1]=m_dPhaseDirectionInScanner[2];
	m_imageInScanner.value[0][2]=m_dSliceDirectionInScanner[0];
	m_imageInScanner.value[1][2]=m_dSliceDirectionInScanner[1];
	m_imageInScanner.value[2][2]=m_dSliceDirectionInScanner[2];
	m_imageInScanner.value[0][3]=fEdgePosition[0];
	m_imageInScanner.value[1][3]=fEdgePosition[1];
	m_imageInScanner.value[2][3]=fEdgePosition[2];
}

int CMRISeries::Save( CString sFolder )
{
	CDataAccess myDataAccess;
	vImagesContainer myImagesContainer;
	for ( int i =0; i < m_nNumOfImages; i++ )
	{
		myImagesContainer.push_back( *(m_pMRIImage[i]->GetImage()) );
	}
	myDataAccess.WriteImagesDataOfScan( &myImagesContainer, sFolder );
	
	m_sFolder = sFolder;
	return ERR_OK;
}


void CMRISeries::SetWindowWidthAndLevel(int nWindowWidth, int nWindowLevel)
{
	UpdateOClut(nWindowWidth, nWindowLevel);
}

void CMRISeries::UpdateOClut(int nWindowWidth, int nWindowLevel)
{
	if (m_nWindowLevel == nWindowLevel && m_nWindowWidth == nWindowWidth)
		return;

	
	m_nWindowWidth = nWindowWidth;
	m_nWindowLevel = nWindowLevel;
	
	if ( m_nWindowWidth != m_oClut32.GetWidth() || m_nWindowLevel != m_oClut32.GetLevel() )
	{
		m_oClut32.SetWidth( m_nWindowWidth );
		m_oClut32.SetLevel( m_nWindowLevel );
		m_oClut32.Update();
		m_nWindowWidth = m_oClut32.GetWidth();
		m_nWindowLevel = m_oClut32.GetLevel();
	}

}

int CMRISeries::GetWindowLevel()
{
	return m_nWindowLevel;
}

int CMRISeries::GetWindowWidth()
{
	return m_nWindowWidth;
}

char* CMRISeries::GetNewBoxOrientChar()
{
	return m_psNewBoxOrientation;
}

void CMRISeries::SetNewBoxOrientChar(char sOrient[6] )
{
	if ( NULL == m_psNewBoxOrientation )
	{
		m_psNewBoxOrientation = new char[6];
	}

	m_psNewBoxOrientation[0] = sOrient[0];
	m_psNewBoxOrientation[1] = sOrient[1];
	m_psNewBoxOrientation[2] = sOrient[2];
	m_psNewBoxOrientation[3] = sOrient[3];
	m_psNewBoxOrientation[4] = sOrient[4];
	m_psNewBoxOrientation[5] = sOrient[5];
}




bool CMRISeries::IsVisible()
{
	return (m_iVisibleCnt > 0);
}
void CMRISeries::SetVisible(bool bVisible)
{
	if (bVisible)
	{
		m_iVisibleCnt ++;
	}
	else 
	{
		m_iVisibleCnt --;
		if (m_iVisibleCnt < 0)
		{
			m_iVisibleCnt = 0;
		}
	}
}

int CMRISeries::Save()
{
	if(m_sFolder!="")
	{
		return Save(m_sFolder);
	}else
	{
		return -1;
	}
}

void CMRISeries::SetNumberOfImages(int number)
{
	m_nNumOfImages=number;
}

void CMRISeries::InsertImage(CMRIImage *pImage)
{
	//判断有没有image id重复的。
	for(int i=0;i<m_nNumOfImages;i++)
	{
		if(m_pMRIImage[i]->GetImageID()==pImage->GetImageID())
			return;
	}

	if(m_nNumOfImages==0)
	{
		m_pMRIImage=new CMRIImage*[1];
		m_nNumOfImages=1;
		m_pMRIImage[0]=pImage;
		InitSeries();

	}else
	{
		//按照image ID从小到大排列顺序将图像插入到序列中
		int iInsertPos;
		BOOL bFind=FALSE;
		for(int i=0;i<m_nNumOfImages;i++)
		{
			if(m_pMRIImage[i]->GetImageID()>pImage->GetImageID())
			{
				iInsertPos=i+1;
				bFind=TRUE;
				break;
			}else
			{
				continue;
			}
		}
		
		
		if(!bFind)  //没有找到则放到最后一个
		{
			iInsertPos=m_nNumOfImages+1;
		}

		CMRIImage** ppImage=new CMRIImage*[m_nNumOfImages+1];
		for(i=0;i<iInsertPos-1;i++)
		{
			ppImage[i]=m_pMRIImage[i];
		}
		ppImage[iInsertPos-1]=pImage;
		for(i=iInsertPos-1;i<m_nNumOfImages;i++)
		{
			ppImage[i+1]=m_pMRIImage[i];
		}

		m_nNumOfImages++;
		delete []m_pMRIImage;
		
		m_pMRIImage=ppImage;
		if(iInsertPos==1)
		{
			InitSeries();
		}

	}
}

//在某些图像序列中，slice方向和实际切片slice方向相反，
//这种情况下，需要将图像序列排序反转，使得slice方向和实际切片slice方向一致
void CMRISeries::CorrectSliceDirection()
{
	if(m_nNumOfImages<=1)
		return;

	// Center 1
	double* pdImageCenter1 = m_pMRIImage[0]->GetCenterInScanner();
	Vector3D vImageCenter1( pdImageCenter1[0], pdImageCenter1[1], pdImageCenter1[2] );

	// Center n
	double* pdImageCenterN = m_pMRIImage[m_nNumOfImages-1]->GetCenterInScanner();
	Vector3D vImageCenterN( pdImageCenterN[0], pdImageCenterN[1], pdImageCenterN[2] );

	//实际slice方向
	Vector3D vRealSlice=vImageCenterN-vImageCenter1;
	vRealSlice.normalize();

	Vector3D vSlice(m_dSliceDirectionInScanner[0],m_dSliceDirectionInScanner[1],m_dSliceDirectionInScanner[2]);
	//判断两个方向是否一致
	double dotProduct=vSlice * vRealSlice;
	double angle=180.0*acos(dotProduct)/PI; //角度值

	/*Matrix m(4,4);
	double fPosition[3];
	Convertor cn;
	RPY rpy;

	m.Identity();
	m.value[0][0]=m_dFreqDirectionInScanner[0];
	m.value[1][0]=m_dFreqDirectionInScanner[1];
	m.value[2][0]=m_dFreqDirectionInScanner[2];
	m.value[0][1]=m_dPhaseDirectionInScanner[0];
	m.value[1][1]=m_dPhaseDirectionInScanner[1];
	m.value[2][1]=m_dPhaseDirectionInScanner[2];
	m.value[0][2]=m_dSliceDirectionInScanner[0];
	m.value[1][2]=m_dSliceDirectionInScanner[1];
	m.value[2][2]=m_dSliceDirectionInScanner[2];
		
	
	fPosition[0] = m_dCenterInScanner[0]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[0]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[0];
	fPosition[1] = m_dCenterInScanner[1]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[1]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[1];
	fPosition[2] = m_dCenterInScanner[2]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[2]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[2];

	m.value[0][3]=fPosition[0];
	m.value[1][3]=fPosition[1];
	m.value[2][3]=fPosition[2];

	
	rpy=cn.Matrix2RPY(m);*/

	if(angle<1.0)
		return;
	
	//

	CMRIImage **pImages=new CMRIImage*[m_nNumOfImages];
	for(int i=0;i<m_nNumOfImages;i++)
	{
		pImages[i]=m_pMRIImage[i];
	}
	for(i=0;i<m_nNumOfImages;i++)
	{
		m_pMRIImage[i]=pImages[m_nNumOfImages-1-i];
	}

	delete []pImages;

	InitSeries();
	
	
	/*m.Identity();
	m.value[0][0]=m_dFreqDirectionInScanner[0];
	m.value[1][0]=m_dFreqDirectionInScanner[1];
	m.value[2][0]=m_dFreqDirectionInScanner[2];
	m.value[0][1]=m_dPhaseDirectionInScanner[0];
	m.value[1][1]=m_dPhaseDirectionInScanner[1];
	m.value[2][1]=m_dPhaseDirectionInScanner[2];
	m.value[0][2]=m_dSliceDirectionInScanner[0];
	m.value[1][2]=m_dSliceDirectionInScanner[1];
	m.value[2][2]=m_dSliceDirectionInScanner[2];
		
	
	fPosition[0] = m_dCenterInScanner[0]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[0]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[0];
	fPosition[1] = m_dCenterInScanner[1]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[1]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[1];
	fPosition[2] = m_dCenterInScanner[2]-m_pMRIImage[0]->GetFOVX()/2*m_dFreqDirectionInScanner[2]-
		m_pMRIImage[0]->GetFOVY()/2*m_dPhaseDirectionInScanner[2];

	m.value[0][3]=fPosition[0];
	m.value[1][3]=fPosition[1];
	m.value[2][3]=fPosition[2];

	
	rpy=cn.Matrix2RPY(m);
	*/

}

CMultiUsedObject* CMRISeries::GetImagesMemory()
{
	if(m_nNumOfImages<=0)
		return NULL;

	if(m_pImagesMemoryObject)
	{		
		return m_pImagesMemoryObject;
	}

	int iWidth=m_pMRIImage[0]->GetResolutionX();
	int iHeight=m_pMRIImage[0]->GetResolutionY();
	unsigned short * pImagesMemory = NULL;
	try
	{
		pImagesMemory = new unsigned short [iWidth*iHeight*m_nNumOfImages];
	}catch(...)
	{
		AfxMessageBox("Memory is not enough.");
		return NULL;
	}
	
	
	for(int nIndex=0;nIndex<m_nNumOfImages;nIndex++)
	{		
		IMG_DATA_TYPE* p = &pImagesMemory[nIndex*iWidth*iHeight];
		memcpy(p,m_pMRIImage[nIndex]->GetColorOfRawData(),sizeof(unsigned short)*iWidth*iHeight);
		//释放避免内存浪费，将series中原来的内存删掉，并更改内存指针。
		IMG_DATA_TYPE *pOldData=m_pMRIImage[nIndex]->GetColorOfRawData();
		delete []pOldData;
		m_pMRIImage[nIndex]->GetImage()->pData=(IMG_DATA_TYPE*)p;

		m_pMRIImage[nIndex]->DisableFreeMemoryOnDestroy();
	}

	m_pImagesMemoryObject=new CMultiUsedObject();
	m_pImagesMemoryObject->m_pDataObject=pImagesMemory;

	return m_pImagesMemoryObject;
}

double* CMRISeries::GetSpacing()
{
	return m_dPixelSpacing;
}

Matrix CMRISeries::GetImageInScanner()
{
	return m_imageInScanner;
}

void CMRISeries::SetFusionCoord(double **ppdTrans)
{
	int i, j;
	//保存融合矩阵
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			m_dFusionCoord[i][j]=ppdTrans[i][j];
		}
	}

	
}

Matrix CMRISeries::GetFusionCoord()
{
	Matrix m(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m.value[i][j]=m_dFusionCoord[i][j];
		}
	}
	return m;
}

void CMRISeries::SaveFusionCoord(CString sFolder)
{
	if(sFolder=="")
	{
		m_sFolder.TrimLeft();
		m_sFolder.TrimRight();	
		if(m_sFolder=="")
			return;
		sFolder=m_sFolder;
	}
	
	CFileFind finder;
	if(!finder.FindFile(sFolder))
	{
		return;
	}

	CString sFile;
	if(sFolder.Right(1)=="\\")
	{
		sFile.Format("%sfusion_%s.mtx",sFolder,m_sName);
	}else
	{
		sFile.Format("%s\\fusion_%s.mtx",sFolder,m_sName);
	}
	FILE *fp;
	try
	{
		fp = fopen(sFile, "w");
		if (fp)
		{
			for (int i=0; i<4; i++)	
			{
				fprintf(fp, "%.6f  %.6f  %.6f  %.6f\n", m_dFusionCoord[i][0], m_dFusionCoord[i][1], m_dFusionCoord[i][2], m_dFusionCoord[i][3]);
			}
			fclose(fp);
		}else
		{
			CString sTmp;
			sTmp.Format("Can't open file %s",sFile);
			AfxMessageBox(sTmp);
			return;
		}
	}catch(...)
	{
		CString sTmp;
		sTmp.Format("Can't open file %s",sFile);
		AfxMessageBox(sTmp);
		return;
	}
}

BOOL CMRISeries::LoadFusionCoord(CString sFolder,Matrix &mFusion)
{
	sFolder.TrimLeft();
	sFolder.TrimRight();	
	if(sFolder=="")
		return FALSE;

	CFileFind finder;
	if(!finder.FindFile(sFolder))
	{
		return FALSE;
	}

	CString sFile;
	if(sFolder.Right(1)=="\\")
	{
		sFile.Format("%sfusion_%s.mtx",sFolder,m_sName);
	}else
	{
		sFile.Format("%s\\fusion_%s.mtx",sFolder,m_sName);
	}

	if(!finder.FindFile(sFile))
	{
		return FALSE;
	}

	FILE *fp;
	try
	{
		
		fp = fopen(sFile, "r");
		if (!fp)
		{
			CString sTmp;
			sTmp.Format("Can't open file %s",sFile);
			AfxMessageBox(sTmp);
			return FALSE;
		}
		float db1, db2, db3, db4;
		double dCoord[4][4];
		for (int i=0; i<4; i++)	
		{
			fscanf(fp, "%f  %f  %f  %f\n", &db1, &db2, &db3, &db4);
			dCoord[i][0] = db1;
			dCoord[i][1] = db2;
			dCoord[i][2] = db3;
			dCoord[i][3] = db4;
		}
		fclose(fp);
		for(i=0;i<4;i++)
		{
			mFusion.value[i][0]=dCoord[i][0];
			mFusion.value[i][1]=dCoord[i][1];
			mFusion.value[i][2]=dCoord[i][2];
			mFusion.value[i][3]=dCoord[i][3];
		}
		return TRUE;
	}catch(...)
	{
		CString sTmp;
		sTmp.Format("Can't open file %s",sFile);
		AfxMessageBox(sTmp);
		return FALSE;
	}
}
