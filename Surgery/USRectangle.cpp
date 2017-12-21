// USRectangle.cpp: implementation of the CUSRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USRectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSRectangle::CUSRectangle()
{
	m_pRectangle=NULL;
	m_sName="UltraSonic";

}

CUSRectangle::~CUSRectangle()
{
	Clean();
}

void CUSRectangle::Clean()
{
	if ( NULL != m_pRectangle )
	{
		delete m_pRectangle;
		m_pRectangle = NULL;
	}
	
}

void CUSRectangle::SetImg(LPUS_STRUCT_IMAGE m_pImage)
{
		
	double pdbXDir[3],pdbYDir[3],pdbCenter[3];

	pdbXDir[0]=m_pImage->oHeader.scanField.FreqDirX;
	pdbXDir[1]=m_pImage->oHeader.scanField.FreqDirY;
	pdbXDir[2]=m_pImage->oHeader.scanField.FreqDirZ;
	pdbYDir[0]=m_pImage->oHeader.scanField.PhaseDirX;
	pdbYDir[1]=m_pImage->oHeader.scanField.PhaseDirY;
	pdbYDir[2]=m_pImage->oHeader.scanField.PhaseDirZ;

	pdbCenter[0]=m_pImage->oHeader.imageField.fCenterPointX;
	pdbCenter[1]=m_pImage->oHeader.imageField.fCenterPointY;
	pdbCenter[2]=m_pImage->oHeader.imageField.fCenterPointZ;

	double dbFOVX = m_pImage->oHeader.seriesField.FovX;
	double dbFOVY = m_pImage->oHeader.seriesField.FovY;
	int ResolutionX= m_pImage->oHeader.seriesField.ResolutionX;
	int ResolutionY= m_pImage->oHeader.seriesField.ResolutionY;

	

	Vector3D vXDirection( pdbXDir[0], pdbXDir[1], pdbXDir[2] );
	Vector3D vYDirection( pdbYDir[0], pdbYDir[1], pdbYDir[2] );
	Vector3D vCenter( pdbCenter[0], pdbCenter[1], pdbCenter[2] );
	Vector3D vNormal = vXDirection ^ vYDirection;

	// Field of view
	vXDirection.SetUnit();
	vXDirection *= dbFOVX;
	
	vYDirection.SetUnit();	
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

	m_oClut32.SetWidth(m_pImage->oHeader.imageField.WindowWidth);
	m_oClut32.SetLevel(m_pImage->oHeader.imageField.WindowLevel);
	m_oClut32.Update();
	CreateTexture(m_pImage);

	return;
}

void CUSRectangle::Flip(short* pData, 
                          int cx,
						  int cy, 
                          BOOL bHorizontalFlip, 
                          BOOL bVerticalFlip)
{
    short *p1, *p2;
    short usTemp;
    
    if (bHorizontalFlip && bVerticalFlip)
    {
        // This is a fast flipping algorthm. Think of the data buffer used to 
        // store the image data, flipping both vertically and horizontally 
        // results in a reversed buffer.
        p1 = pData;
        p2 = pData + cx * cy - 1;
        for (; p1 < p2; ++p1, --p2)
        {
            usTemp = *p1;
            *p1 = *p2;
            *p2 = usTemp;
        }
    }
    else if (bHorizontalFlip)
    {

        for (int i = 0; i < cy; ++i)
        {
            p1 = pData + i * cx;
            p2 = p1 + cx - 1;
            for (; p1 < p2; ++p1, --p2)
            {
                usTemp = *p1;
                *p1 = *p2;
                *p2 = usTemp;
            }
        }
    }
    else if (bVerticalFlip)
    {
        for (int i = 0; i < cy / 2; i++)
        {
            p1 = pData + i * cx;
            p2 = pData + (cy - i - 1) * cx;

            for (int j = 0; j < cx; ++j, ++p1, ++p2)
            {
                usTemp = *p1;
                *p1 = * p2;
                *p2 = usTemp;
            }
        }
    }
}


BOOL CUSRectangle::CreateTexture(LPUS_STRUCT_IMAGE m_pImage)
{
	// Set color
	CColorF color( 1.0f, 1.0f, 1.0f, 0.5f );
	m_pRectangle->SetColor( &color );
	
	int iNumOfPts = m_pImage->oHeader.seriesField.ResolutionX * m_pImage->oHeader.seriesField.ResolutionY;
	
	
	IMG_DATA_TYPE* pCorlorOfRawData = m_pImage->pData;
	if ( NULL == pCorlorOfRawData )
	{
		return FALSE;
	}
	
	
	//ningsj 2009.2.10
	STRUCT_COLORF* pImageData = new STRUCT_COLORF[iNumOfPts];
	float fTemp = 1.0f/0xffffff;
	for ( int i = 0; i < iNumOfPts; i++ )
	{
		COLORREF color=m_oClut32[pCorlorOfRawData[i]];
		//float fVal = m_pMRIImg->m_pSeries->m_oClut32[pCorlorOfRawData[i]] * fTemp;
		//pImageData[i].R = fVal ;
		//pImageData[i].G = fVal ;
		//pImageData[i].B = fVal ;
		pImageData[i].A = 1.0f ;
		pImageData[i].R=  (GetBValue(color))/255.0;
		pImageData[i].G=  (GetGValue(color))/255.0;
		pImageData[i].B=  (GetRValue(color))/255.0;
	}

	unsigned short usResolutionX = m_pImage->oHeader.seriesField.ResolutionX;
	unsigned short usResolutionY = m_pImage->oHeader.seriesField.ResolutionY;
	
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