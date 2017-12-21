// ImageAnalysis.h: interface for the CImageAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEANALYSIS_H__582604A3_760A_4870_AA61_7C38EBDE3769__INCLUDED_)
#define AFX_IMAGEANALYSIS_H__582604A3_760A_4870_AA61_7C38EBDE3769__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_IMAGEANALYSIS_DLL
	#define IMAGEANALYSIS_API		__declspec(dllexport)
#else
	#define IMAGEANALYSIS_API		 __declspec(dllimport)
#endif

class IMAGEANALYSIS_API CImageAnalysis  
{
public:
	int CircleDetection2(BYTE *src, int iWidth, int iHeight, int &iSphereNum
		, double **dbSphereCenter, float **rad, float **err
		, int iMinEdgePntCount=200, int iMaxEdgePntCount=800);
	CImageAnalysis();
	virtual ~CImageAnalysis();

	/* Auto detect circle center in BMP image
	INPUT   src:  image data, one pixel one byte value range 0 - 255, store along rows
			iWidth:	 image width, pixel number of row
			iHeigh:	 image height, pixel number of colume
	OUTPUT	iSphereNum:		detected circle number
			dbSphereCenter:	detected circle center coordinates stored as X1, Y1, X2, Y2, ...
	CONTEXT
			dbSphereCenter is allocated memory inside and need to be released outside
	*/
	int CircleDetection(BYTE *src, int iWidth, int iHeight, int &iSphereNum, double **dbSphereCenter);

	int CalulateCirclePixelArea(BYTE *src, int iImgWidth, int iImgHeight, int iCenterX, int iCenterY, int iPixelRadius);
	
	void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge);

private:

};

#endif // !defined(AFX_IMAGEANALYSIS_H__582604A3_760A_4870_AA61_7C38EBDE3769__INCLUDED_)
