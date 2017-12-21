// USRectangle.h: interface for the CUSRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USRECTANGLE_H__BB030D15_2029_4842_93AB_307D4947947E__INCLUDED_)
#define AFX_USRECTANGLE_H__BB030D15_2029_4842_93AB_307D4947947E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "US_ImageHeader.h"
#include "Rectangle.h"
#include "LUT.h"

class CUSRectangle  
{
public:
	void SetImg(LPUS_STRUCT_IMAGE m_pImage);
	CUSRectangle();
	virtual ~CUSRectangle();

	// Image rectangle
	CRectangle* m_pRectangle;
	double m_ppdOrientCharPos[4][3];
	char m_ppcOrientation[4][10];

private:
	void Flip(short* pData,int cx,int cy,BOOL bHorizontalFlip,BOOL bVerticalFlip);
	void Clean();
	BOOL CreateTexture(LPUS_STRUCT_IMAGE m_pImage);

	//CMRIImage *m_pMRIImg;
	CString m_sName;
	CLUT32 m_oClut32;
};

#endif // !defined(AFX_USRECTANGLE_H__BB030D15_2029_4842_93AB_307D4947947E__INCLUDED_)
