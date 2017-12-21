// ImageConvertor.h: interface for the CImageConvertor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGECONVERTOR_H__DBFAB2F2_9284_49AE_B8F2_4EE4F822CDBD__INCLUDED_)
#define AFX_IMAGECONVERTOR_H__DBFAB2F2_9284_49AE_B8F2_4EE4F822CDBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImagePlus.h"
#include "BI_ImageHeader.h"
#include "CT_ImageHeader.h"
#include <afxtempl.h>
#include <map>

#define MAX_SIZE 100
class  IMAGEPLUS_API CImageConvertor  
{
public:	

	void* GetMRISeriesAt(int index);
	int GetSeriesCount();
	CImageConvertor();
	virtual ~CImageConvertor();

	char* Vti2MRISeries(CString sFile);
	void ReadDCMFile(CString sFile,void* pMRIImage);
	void ReadDCMFolder(CString sFolder);
	

private:
	void Clear();
	int  GetSeriesOption(char szSeriesName[]);
	void TimeConvert2(MR_BIDateTime &datetime,const char *p,int flag);
	void TimeConvert2(CT_BIDateTime &datetime,const char *p,int flag);
	void CalcBIImgDirec(double dDirCos[6], double dTopLeft[3] ,BYTE iPatPos, BYTE iPatEntry, 
		                 unsigned short sFovX, unsigned short sFovY,double dBIDirec[]) ;
	BOOL InitMR(void* pDcmReader,MR_STRUCT_IMAGE *pXinAoImg);
	BOOL InitCT(void* pDcmReader,CT_STRUCT_IMAGE *pCTXinAoImg);

	CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> m_ArrayMRImage[MAX_SIZE];	
	int m_iSeriesCount;
};

#endif // !defined(AFX_IMAGECONVERTOR_H__DBFAB2F2_9284_49AE_B8F2_4EE4F822CDBD__INCLUDED_)
