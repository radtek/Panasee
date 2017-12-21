// ImageReslice.h: interface for the CImageReslice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGERESLICE_H__3C3647A7_670E_4990_8B49_D816A1B5A09F__INCLUDED_)
#define AFX_IMAGERESLICE_H__3C3647A7_670E_4990_8B49_D816A1B5A09F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImagePlus.h"
#include "BI_imageHeader.h"

class IMAGEPLUS_API CImageReslice  
{
public:
	char* Reslice2D();
	void SetSpaceExtend(double space[3],int extend[3]);
	void SetDestSeries(void *pSeries);
	BOOL InitSeries(void *pSeries);
	
	char* Reslice(CString sName,int iType);
	char* Reslice2D(int iType);
	void SetResliceAxis(double matrix[16]);
	
	double m_dOldImageMatrix[4][4];

	CImageReslice();
	virtual ~CImageReslice();

private:
	void *m_pVtkImageData;
	
	double m_dResliceMatrix[16];
	double m_dSpace[3];
	int m_iExtend[3];
	BOOL m_bSelfDefinedSpaceExtend;
	void *m_pDestSeries;
	int m_type;
	MR_STRUCT_IMAGE m_header;

	double m_dFusionCoord[4][4];
};

#endif // !defined(AFX_IMAGERESLICE_H__3C3647A7_670E_4990_8B49_D816A1B5A09F__INCLUDED_)
