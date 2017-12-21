// MRIRectangle.cpp: implementation of the CMRIRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "MRIRectangle.h"
#include "Texture2D.h"
#include "MRISeries.h"
#include "MRIRectSeries.h"
#include "Sphere.h"
//#include "./LookupTable/LUT.h"
#include "ConfigMgr.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern CLUT32 m_oClut32;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRIRectangle::CMRIRectangle()
{
	Init();
}


CMRIRectangle::CMRIRectangle(CMRISeries *pMRISeries, int iImgIndex)
{
	Init();
	if (iImgIndex >= 0 && iImgIndex < pMRISeries->GetNumOfImages())
	{
		m_pMRIImg = pMRISeries->GetImageAt(iImgIndex);
		m_iIndex = iImgIndex;
	}
}

CMRIRectangle::CMRIRectangle(MRIRectSeries *pMRIRectSeries, int iIndex)
{
	Init();
	if (iIndex >= 0 && iIndex < pMRIRectSeries->GetMRISeries()->GetNumOfImages())
	{
		m_pMRIImg = pMRIRectSeries->GetMRISeries()->GetImageAt(iIndex);
		m_iIndex = iIndex;
	}
	m_pMRIRectSeries = pMRIRectSeries;
	SetName();
}

void CMRIRectangle::Init()
{
	m_pRectangle = NULL;
	

	m_pMRIImg = NULL;
	m_sName = "";
	m_bIsVisible = FALSE;
	m_bIsVisible3D = FALSE;
	m_bIn3DWindows = FALSE;
	m_iIndex = -1;

	m_pMRIRectSeries = NULL;
}

CMRIRectangle::~CMRIRectangle()
{
	Clean();
}

void CMRIRectangle::Clean()
{
	if ( NULL != m_pRectangle )
	{
		delete m_pRectangle;
		m_pRectangle = NULL;
	}
	
	Init();
}

void CMRIRectangle::UpdatePos()
{
	if (m_pMRIImg == NULL)
		return;

	double *pdbSliceDir = m_pMRIImg->GetSliceDirection();
	double *pdbXDir = m_pMRIImg->GetFreqDirection();
	double *pdbYDir = m_pMRIImg->GetPhaseDirection();
	double *pdbCenter = m_pMRIImg->GetCenter();

	double dbFOVX = m_pMRIImg->GetFOVX();
	double dbFOVY = m_pMRIImg->GetFOVY();

	Vector3D vNormal( pdbSliceDir[0], pdbSliceDir[1], pdbSliceDir[2] );
	Vector3D vXDirection( pdbXDir[0], pdbXDir[1], pdbXDir[2] );
	Vector3D vYDirection( pdbYDir[0], pdbYDir[1], pdbYDir[2] );
	Vector3D vCenter( pdbCenter[0], pdbCenter[1], pdbCenter[2] );

	// Field of view
	vXDirection.SetUnit();
	vXDirection *= dbFOVX;
	//vXDirection *= m_pMRIImg->GetResolutionX();
	vYDirection.SetUnit();
	//vYDirection *= m_pMRIImg->GetResolutionY();
	vYDirection *= dbFOVY;

	if (m_pRectangle) 
	{
		m_pRectangle->SetRectangle(vCenter, vXDirection, vYDirection);
	}
}

BOOL CMRIRectangle::CreateMRIRect()
{
	if (m_pMRIImg == NULL)
		return FALSE;

	double *pdbSliceDir = m_pMRIImg->GetSliceDirection();
	double *pdbXDir = m_pMRIImg->GetFreqDirection();
	double *pdbYDir = m_pMRIImg->GetPhaseDirection();
	double *pdbCenter = m_pMRIImg->GetCenter();

	double dbFOVX = m_pMRIImg->GetFOVX();
	double dbFOVY = m_pMRIImg->GetFOVY();

	Vector3D vNormal( pdbSliceDir[0], pdbSliceDir[1], pdbSliceDir[2] );
	Vector3D vXDirection( pdbXDir[0], pdbXDir[1], pdbXDir[2] );
	Vector3D vYDirection( pdbYDir[0], pdbYDir[1], pdbYDir[2] );
	Vector3D vCenter( pdbCenter[0], pdbCenter[1], pdbCenter[2] );

	// Field of view
	vXDirection.SetUnit();
	vXDirection *= dbFOVX;
	//vXDirection *= m_pMRIImg->GetResolutionX();
	vYDirection.SetUnit();
	//vYDirection *= m_pMRIImg->GetResolutionY();
	vYDirection *= dbFOVY;

	if (m_pRectangle) 
	{
		m_pRectangle->SetRectangle(vCenter, vXDirection, vYDirection);
	}
	else
	{
		m_pRectangle = new CRectangle( vCenter, vXDirection, vYDirection );
		m_pRectangle->SetName(m_sName);
				
	} 

	CreateTexture();

	return TRUE;
}

BOOL CMRIRectangle::SetMRIImg(CMRIImage *pMRIImg, bool bUpdateTexture /*= false*/)
{
	m_pMRIImg = pMRIImg;
	if (m_bIsVisible || bUpdateTexture) 
	{
		CreateMRIRect();
	}
	return TRUE;
}


BOOL CMRIRectangle::SetName()
{
	if (m_pMRIRectSeries == NULL)
		return FALSE;

	m_sName.Format("%s-%02d", m_pMRIRectSeries->GetName(), m_iIndex);
	
	return TRUE;
}

BOOL CMRIRectangle::CreateTexture()
{
	// Set color
	CColorF color( 1.0f, 1.0f, 1.0f, 0.5f );
	m_pRectangle->SetColor( &color );
	
	int iNumOfPts = m_pMRIImg->GetNumOfPts();
	if(m_pMRIImg->GetImage()==NULL)
		return FALSE;
	
	CConfigMgr conf;
	IMG_DATA_TYPE* pCorlorOfRawData = m_pMRIImg->GetColorOfRawData();
	if ( NULL == pCorlorOfRawData )
	{
		return FALSE;
	}
	
	
	//ningsj 2009.2.10
	STRUCT_COLORF* pImageData = new STRUCT_COLORF[iNumOfPts];
	float fTemp = 1.0f/0xffffff;
	for ( int i = 0; i < iNumOfPts; i++ )
	{
		COLORREF color=m_pMRIImg->m_pSeries->m_oClut32[pCorlorOfRawData[i]];
		//float fVal = m_pMRIImg->m_pSeries->m_oClut32[pCorlorOfRawData[i]] * fTemp;
		//pImageData[i].R = fVal ;
		//pImageData[i].G = fVal ;
		//pImageData[i].B = fVal ;
		pImageData[i].A = 1.0f ;
		pImageData[i].R=  (GetBValue(color))/255.0;
		pImageData[i].G=  (GetGValue(color))/255.0;
		pImageData[i].B=  (GetRValue(color))/255.0;
	}

	unsigned short usResolutionX = m_pMRIImg->GetResolutionX();
	unsigned short usResolutionY = m_pMRIImg->GetResolutionY();
	
	if (m_pRectangle->GetTexture() == NULL)
	{
		m_pRectangle->SetTexture(pImageData, usResolutionX, usResolutionY);
	}
	else
	{
		m_pRectangle->GetTexture()->SetImageData(pImageData, usResolutionX, usResolutionY); //SetTexture(pTexture);
		m_pRectangle->SetUpdatingSubTexture(true);
	}

	Vector2D vTexCoor[4];
	vTexCoor[0].x = 0.0;
	vTexCoor[0].y = 0.0;
	vTexCoor[1].x = 1.0;
	vTexCoor[1].y = 0.0;
	vTexCoor[2].x = 1.0;
	vTexCoor[2].y = 1.0;
	vTexCoor[3].x = 0.0;
	vTexCoor[3].y = 1.0;
	m_pRectangle->SetTexCoor( vTexCoor );
	
	
	delete []pImageData;
	

	m_pRectangle->SetUsingTextureMapping(TRUE);

	return TRUE;
}

CMRIImage*  CMRIRectangle::GetMRIImg()
{
	return m_pMRIImg;
}

void CMRIRectangle::SetVisible3D(BOOL nCmdShow)
{
	m_bIsVisible3D = nCmdShow;
	if(m_pRectangle!=NULL)
	{
		m_pRectangle->SetVisible(m_bIsVisible3D);
	}

}

BOOL CMRIRectangle::IsVisible3D()
{
	return m_bIsVisible3D;
}

void CMRIRectangle::RemoveMRIRect()
{
	if ( NULL != m_pRectangle )
	{
		delete m_pRectangle;
		m_pRectangle = NULL;
	}
}
