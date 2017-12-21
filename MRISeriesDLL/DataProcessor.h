// DataProcessor.h: interface for the CDataProcessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPROCESSOR_H__C4B58880_B5C8_4275_87C7_FE546293F4DC__INCLUDED_)
#define AFX_DATAPROCESSOR_H__C4B58880_B5C8_4275_87C7_FE546293F4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IGTImageHeader.h"

class CDataProcessor  
{
public:
	CDataProcessor();
	virtual ~CDataProcessor();

    //  Rotate the image.
	static void Rotate(IGT_IMG_DATA_TYPE *pdata_dst, 
						int nx_dst, 
						int ny_dst,
						IGT_IMG_DATA_TYPE *pdata_src, 
						int nx_src, 
						int ny_src,
						double rotateangle);
	//  Flipping the image.
    static void Flip(IGT_IMG_DATA_TYPE* pData, CSize sizeBuffer, 
        BOOL bHorizontalFlip, BOOL bVerticalFlip);

};

#endif // !defined(AFX_DATAPROCESSOR_H__C4B58880_B5C8_4275_87C7_FE546293F4DC__INCLUDED_)
