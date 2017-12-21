// ShowImage.cpp: implementation of the CShowImage class.
//
//////////////////////////////////////////////////////////////////////
#include "stdAfx.h"
#include "ShowImage.h"
#include "ImageManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern AFX_EXTENSION_MODULE PanaSkinDLLModule;

CShowImage::CShowImage()
{
	
	m_transparentColor=RGB(255,255,255);
	SetShowRegion(0,0,10,10);
	SetSizingMargin(0,0,0,0);
	m_bTransparent=FALSE;
	m_bAlpha=FALSE;
}

/*********** ningsj *********************
 说明:设置贴图图像ID,该图像中包含几个子图像--count
      子图像4角的大小
	  如:距离左边left个像素,顶部top个像素的区域为左上角
*****************************************/
void CShowImage::SetSizingMargin( int Left,int Top, 
					   int Right,int Bottom)
{


	m_iMarginLeft=Left;
	m_iMarginTop=Top;
	m_iMarginRight=Right;
	m_iMarginBottom=Bottom;
}


CShowImage::~CShowImage()
{
	
}



/*********** ningsj *********************
 说明:指定贴图位置
*****************************************/
void CShowImage::SetShowRegion(int iLeft, int iTop, int iWidth, int iHeight)
{
	m_iShowRegionLeft=iLeft;
	m_iShowRegionTop=iTop;
	m_iShowRegionWidth=iWidth;
	m_iShowRegionHeight=iHeight;
}

/*********** ningsj *********************
 说明:指定贴图位置
*****************************************/
void CShowImage::SetShowRegion(CRect rc)
{
	m_iShowRegionLeft=rc.left;
	m_iShowRegionTop=rc.top;
	m_iShowRegionWidth=rc.Width();
	m_iShowRegionHeight=rc.Height();
}

/*********** ningsj *********************
 说明:向指定pDC中贴上第iImageIndex幅图像
      贴图位置由SetShowRegion决定
*****************************************/
BOOL CShowImage::DrawImage(CDC *pDC, int iImageIndex)
{
	if(iImageIndex>m_iImageCount-1)
		return FALSE;

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap *pOldImage;
	pOldImage=srcDC.SelectObject(m_pImage);	

	CRect rc(m_iShowRegionLeft,m_iShowRegionTop,
		m_iShowRegionLeft+m_iShowRegionWidth,m_iShowRegionTop+m_iShowRegionHeight);
    
	CRect rcImage;
	rcImage.left=iImageIndex * m_iImageWidth;
	rcImage.right=rcImage.left+m_iImageWidth;
	rcImage.top=0;
	rcImage.bottom=m_iImageHeight;
	
	
	// lefttop,leftbottom,righttop,rightbottom
	DrawImagePart(pDC,rc.left,rc.top,rc.left+m_iMarginLeft,rc.top+m_iMarginTop,
		&srcDC,rcImage.left,rcImage.top,rcImage.left+m_iMarginLeft,rcImage.top+m_iMarginTop);

	DrawImagePart(pDC,rc.left,rc.bottom-m_iMarginBottom,rc.left+m_iMarginLeft,rc.bottom,
		&srcDC,rcImage.left,rcImage.bottom-m_iMarginBottom,rcImage.left+m_iMarginLeft,rcImage.bottom);

	DrawImagePart(pDC,rc.right-m_iMarginRight,rc.top,rc.right,rc.top+m_iMarginTop,
		&srcDC,rcImage.right-m_iMarginRight,rcImage.top,rcImage.right,rcImage.top+m_iMarginTop);

	DrawImagePart(pDC,rc.right-m_iMarginRight,rc.bottom-m_iMarginBottom,rc.right,rc.bottom,
		&srcDC,rcImage.right-m_iMarginRight,rcImage.bottom-m_iMarginBottom,rcImage.right,rcImage.bottom);
	
    
    // entireimage
	DrawImagePart(pDC,rc.left+m_iMarginLeft,rc.top+m_iMarginTop,rc.right-m_iMarginRight,rc.bottom-m_iMarginBottom,
		&srcDC,rcImage.left+m_iMarginLeft,rcImage.top+m_iMarginTop,rcImage.right-m_iMarginRight,rcImage.bottom-m_iMarginBottom);

    
    
    // topbar,bottombar
	DrawImagePart(pDC,rc.left+m_iMarginLeft,rc.top,rc.right-m_iMarginRight,rc.top+m_iMarginTop,
		&srcDC,rcImage.left+m_iMarginLeft,rcImage.top,rcImage.right-m_iMarginRight,rcImage.top+m_iMarginTop);

	DrawImagePart(pDC,rc.left+m_iMarginLeft,rc.bottom-m_iMarginBottom,rc.right-m_iMarginRight,rc.bottom,
		&srcDC,rcImage.left+m_iMarginLeft,rcImage.bottom-m_iMarginBottom,rcImage.right-m_iMarginRight,rcImage.bottom);
    
    // sidebar
	DrawImagePart(pDC,rc.left,rc.top+m_iMarginTop,rc.left+m_iMarginLeft,rc.bottom-m_iMarginBottom,
		&srcDC,rcImage.left,rcImage.top+m_iMarginTop,rcImage.left+m_iMarginLeft,rcImage.bottom-m_iMarginBottom);

	DrawImagePart(pDC,rc.right-m_iMarginRight,rc.top+m_iMarginTop,rc.right,rc.bottom-m_iMarginBottom,
		&srcDC,rcImage.right-m_iMarginRight,rcImage.top+m_iMarginTop,rcImage.right,rcImage.bottom-m_iMarginBottom);
     
	
	srcDC.SelectObject(pOldImage);
	srcDC.DeleteDC();

	return TRUE;

}
/*BOOL CShowImage::DrawImage(CDC *pDC, int iImageIndex)
{
	if(iImageIndex>m_iImageCount-1)
		return FALSE;

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap memImage,*pMemImage;
	memImage.CreateCompatibleBitmap(pDC,m_iImageWidth,m_iImageHeight);
	pMemImage=srcDC.SelectObject(&memImage);

	InitImage(&srcDC,iImageIndex);
	

	CRect rc(m_iShowRegionLeft,m_iShowRegionTop,
		m_iShowRegionLeft+m_iShowRegionWidth,m_iShowRegionTop+m_iShowRegionHeight);
    CMemDC memDC;
	memDC.LoadDC(pDC,rc,TRUE,m_transparentColor);

	int orix=0 ;//iImageIndex * m_iImageWidth;
	
	
	// lefttop,leftbottom,righttop,rightbottom
    if( !memDC.BitBlt(0,0,m_iMarginLeft, m_iMarginTop, &srcDC,orix,0,SRCCOPY) ) return false;
    

    if( !memDC.BitBlt(0,rc.Height() - m_iMarginBottom,m_iMarginLeft, m_iMarginBottom, 
                &srcDC,orix,m_iImageHeight - m_iMarginBottom,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.Width() - m_iMarginRight, 0 ,m_iMarginRight, m_iMarginTop, 
                 &srcDC,orix + m_iImageWidth - m_iMarginRight,0,SRCCOPY) ) return false;
	

    if( !memDC.BitBlt(rc.Width() - m_iMarginRight,rc.Height() - m_iMarginBottom,m_iMarginRight,m_iMarginBottom,
                 &srcDC,orix + m_iImageWidth - m_iMarginRight,m_iImageHeight - m_iMarginBottom,SRCCOPY) ) return false;
		
    // entireimage
    memDC.StretchBlt(m_iMarginLeft,
                     m_iMarginTop,
                     rc.Width()  - ( m_iMarginLeft + m_iMarginRight ) ,
                     rc.Height() -  ( m_iMarginTop + m_iMarginBottom) ,
                     &srcDC,
                     orix + m_iMarginLeft,
                     m_iMarginTop,
                     m_iImageWidth - ( m_iMarginLeft + m_iMarginRight ) ,
                     m_iImageHeight - ( m_iMarginTop + m_iMarginBottom ) ,SRCCOPY);

    
    // topbar,bottombar( Stretch )
    memDC.StretchBlt(m_iMarginLeft,0, rc.Width() - (m_iMarginLeft + m_iMarginRight), m_iMarginTop,
                     &srcDC,orix + m_iMarginLeft, 0, m_iImageWidth - ( m_iMarginLeft + m_iMarginRight) , m_iMarginTop,SRCCOPY);

    memDC.StretchBlt(m_iMarginLeft, rc.Height() - m_iMarginBottom, rc.Width() - ( m_iMarginLeft + m_iMarginRight), m_iMarginBottom,
                     &srcDC,orix + m_iMarginLeft,m_iImageHeight - m_iMarginBottom,m_iImageWidth - ( m_iMarginLeft + m_iMarginRight) , m_iMarginBottom,SRCCOPY );
	

    // sidebar ( STretch? )
    memDC.StretchBlt(0,m_iMarginTop,m_iMarginLeft,rc.Height() - m_iMarginTop - m_iMarginBottom ,
                     &srcDC, orix,m_iMarginTop, m_iMarginLeft, m_iImageHeight - ( m_iMarginTop + m_iMarginBottom ) ,SRCCOPY);
	
    memDC.StretchBlt(rc.Width() - m_iMarginRight ,m_iMarginTop,m_iMarginRight,rc.Height() - m_iMarginTop - m_iMarginBottom ,
                     &srcDC, orix +  m_iImageWidth - m_iMarginRight,m_iMarginTop, m_iMarginRight, m_iImageHeight - m_iMarginTop - m_iMarginBottom ,SRCCOPY);
    
	
	srcDC.SelectObject(pMemImage);
	memImage.DeleteObject();
	srcDC.DeleteDC();

	return TRUE;

}*/

BOOL CShowImage::DrawImagePart(CDC* pDCDest, const CRect& rcDest,const CRect& rcSrc)
{
	CDC srcDC;
	srcDC.CreateCompatibleDC(pDCDest);
	CBitmap *pOldImage;
	pOldImage=srcDC.SelectObject(m_pImage);	

	BOOL bResult=DrawImagePart(pDCDest,rcDest,&srcDC,rcSrc);
	
	srcDC.SelectObject(pOldImage);
	srcDC.DeleteDC();

	return bResult;
}
BOOL CShowImage::DrawImagePart(CDC* pDCDest, int DestLeft,int DestTop,int DestRight,int DestBottom,
							   CDC* pDCSrc, int SrcLeft,int SrcTop,int SrcRight,int SrcBottom)
{
	CRect rcDest,rcSrc;
	rcDest.left=DestLeft;
	rcDest.top=DestTop;
	rcDest.right=DestRight;
	rcDest.bottom=DestBottom;

	rcSrc.left=SrcLeft;
	rcSrc.right=SrcRight;
	rcSrc.top=SrcTop;
	rcSrc.bottom=SrcBottom;
	return DrawImagePart(pDCDest,rcDest,pDCSrc,rcSrc);
}

BOOL CShowImage::DrawImagePart(CDC* pDCDest, const CRect& rcDest, CDC* pDCSrc, const CRect& rcSrc)
{
	if (rcDest.Width() <= 0 || rcDest.Height() <= 0 || rcSrc.Width() <= 0 || rcSrc.Height() <= 0)
		return TRUE;

	BOOL bResult = TRUE;

	if (m_bAlpha)
	{
		BLENDFUNCTION bf;
		ZeroMemory(&bf, sizeof(BLENDFUNCTION));

		bf.AlphaFormat  = 0x01;
		bf.SourceConstantAlpha =255;
		bf.BlendOp = AC_SRC_OVER;
		bResult=::AlphaBlend(pDCDest->GetSafeHdc(),rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				pDCSrc->GetSafeHdc(), rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),bf);
	}
	else if(m_bTransparent)
	{
		bResult=TransparentBlt(pDCDest->GetSafeHdc(),rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				pDCSrc->GetSafeHdc(), rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),m_transparentColor);
	}else
	{
		if((rcSrc.Width() == rcDest.Width()) && (rcSrc.Height() == rcDest.Height()))
		{
			bResult = pDCDest->BitBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
		}
		else
		{
			pDCDest->SetStretchBltMode(COLORONCOLOR);
			bResult = pDCDest->StretchBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				pDCSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), SRCCOPY);
		}
	}

	return bResult;
}



void CShowImage::SetTransparentColor( COLORREF color )
{ 
	if(color!=-1)
	{
		m_transparentColor = color;
		m_bTransparent=TRUE;
	}else
	{
		m_bTransparent=FALSE;
	}
}

BOOL CShowImage::SetImage(UINT IDBitmap,int Count)
{
	return SetImage(PanaSkinDLLModule.hModule,IDBitmap,Count);
}

BOOL CShowImage::SetImage(HMODULE hModule,UINT IDBitmap,int Count)
{
	m_bAlpha = FALSE;

	m_pImage=CImageManager::GetImageManager()->LoadImage(hModule,IDBitmap,m_bAlpha);
	
	if(m_pImage==NULL)
		return FALSE;

	BITMAP bmpinfo;
    m_pImage->GetBitmap(&bmpinfo);
    int iWidth= bmpinfo.bmWidth / Count;
    int iHeight= bmpinfo.bmHeight;

	m_iImageCount=Count;
	m_iImageWidth=iWidth;
	m_iImageHeight=iHeight;
	
	return TRUE;
}







void CShowImage::GetBitmap(BITMAP *bmpinfo)
{
	if(m_pImage)
		m_pImage->GetBitmap(bmpinfo);
}

BOOL CShowImage::LoadBitmap(UINT IDBitmap)
{
	return SetImage(IDBitmap);
}

BOOL CShowImage::SetImage(CString sFilePath, int Count)
{
	m_bAlpha = FALSE;

	m_pImage=CImageManager::GetImageManager()->LoadImage(sFilePath,m_bAlpha);
	
	if(m_pImage==NULL)
		return FALSE;

	BITMAP bmpinfo;
    m_pImage->GetBitmap(&bmpinfo);
    int iWidth= bmpinfo.bmWidth / Count;
    int iHeight= bmpinfo.bmHeight;

	m_iImageCount=Count;
	m_iImageWidth=iWidth;
	m_iImageHeight=iHeight;
	
	return TRUE;
}
