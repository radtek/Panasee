// DataProcessor.cpp: implementation of the CDataProcessor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataProcessor.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataProcessor::CDataProcessor()
{

}

CDataProcessor::~CDataProcessor()
{

}

void CDataProcessor::Rotate(IGT_IMG_DATA_TYPE *pdata_dst, 
							int nx_dst, 
							int ny_dst,
							IGT_IMG_DATA_TYPE *pdata_src, 
							int nx_src, 
							int ny_src,
							double rotateangle)
{
    rotateangle = -rotateangle;
	memset(pdata_dst, 0, sizeof (short) * nx_dst * ny_dst);
	// 源图像的宽度和高度
	LONG	lWidth  = nx_src;
	LONG	lHeight = ny_src;
	
	// 旋转后图像的宽度和高度
	LONG	lNewWidth  = nx_dst;
	LONG	lNewHeight = ny_dst;
	
	// 循环变量（象素在新DIB中的坐标）
	LONG	i;
	LONG	j;
	
	// 象素在源DIB中的坐标
	LONG	i0;
	LONG	j0;
	
	// 旋转角度（弧度）
	double	fRotateAngle;
	// 旋转角度的正弦和余弦
	double	fSina, fCosa;
	// 两个中间常量
	double	f1,f2;
	// 源图四个角的坐标（以图像中心为坐标系原点）
	double	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	double	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;

	// 将旋转角度从度转换到弧度
	fRotateAngle = rotateangle;
	
	// 计算旋转角度的正弦
	fSina = (double) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	fCosa = (double) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (double) (- (lWidth  - 1) / 2);
	fSrcY1 = (double) (  (lHeight - 1) / 2);

	fSrcX2 = (double) (  (lWidth  - 1) / 2);
	fSrcY2 = (double) (  (lHeight - 1) / 2);
	
    fSrcX3 = (double) (- (lWidth  - 1) / 2);
	fSrcY3 = (double) (- (lHeight - 1) / 2);
	
    fSrcX4 = (double) (  (lWidth  - 1) / 2);
	fSrcY4 = (double) (- (lHeight - 1) / 2);
	
	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;

	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	
    fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	
    fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// 两个常数，这样不用以后每次都计算了
	f1 = (double) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina + 0.5 * (lWidth  - 1));
	f2 = (double) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa + 0.5 * (lHeight - 1));
	
	// 针对图像每行进行操作
	for(i = 0; i < lNewHeight; i++)
	{
		// 针对图像每列进行操作
		for(j = 0; j < lNewWidth; j++)
		{

			// 计算该象素在源DIB中的坐标
			i0 = (LONG) (-((double) j) * fSina + ((double) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((double) j) * fCosa + ((double) i) * fSina + f1 + 0.5);
			
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				pdata_dst[i * lNewWidth + j] = pdata_src[i0 * lWidth+ j0];
			}
		}
	}
}

//! Flip the image.
/*!
    \param pData Pointer to the data buffer of short.
    \param sizeBuffer Size of the image.
    \param bHorizontalFlip \a true to horizontally flip the image.
    \param bVerticalFlip \a true to vertically flip the image.
    This function performs in-place flip of the image data.
*/
void CDataProcessor::Flip(IGT_IMG_DATA_TYPE* pData, 
                          CSize sizeBuffer, 
                          BOOL bHorizontalFlip, 
                          BOOL bVerticalFlip)
{
    IGT_IMG_DATA_TYPE *p1, *p2;
    short usTemp;
    
    if (bHorizontalFlip && bVerticalFlip)
    {
        // This is a fast flipping algorthm. Think of the data buffer used to 
        // store the image data, flipping both vertically and horizontally 
        // results in a reversed buffer.
        p1 = pData;
        p2 = pData + sizeBuffer.cx * sizeBuffer.cy - 1;
        for (; p1 < p2; ++p1, --p2)
        {
            usTemp = *p1;
            *p1 = *p2;
            *p2 = usTemp;
        }
    }
    else if (bHorizontalFlip)
    {

        for (int i = 0; i < sizeBuffer.cy; ++i)
        {
            p1 = pData + i * sizeBuffer.cx;
            p2 = p1 + sizeBuffer.cx - 1;
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
        for (int i = 0; i < sizeBuffer.cy / 2; i++)
        {
            p1 = pData + i * sizeBuffer.cx;
            p2 = pData + (sizeBuffer.cy - i - 1) * sizeBuffer.cx;

            for (int j = 0; j < sizeBuffer.cx; ++j, ++p1, ++p2)
            {
                usTemp = *p1;
                *p1 = * p2;
                *p2 = usTemp;
            }
        }
    }
}
