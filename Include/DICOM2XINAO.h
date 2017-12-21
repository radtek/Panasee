// DICOM2XINAO.h: interface for the CDICOM2XINAO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICOM2XINAO_H__3EA299EF_E376_4906_98F0_2EA3C7EA8945__INCLUDED_)
#define AFX_DICOM2XINAO_H__3EA299EF_E376_4906_98F0_2EA3C7EA8945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BI_ImageHeader.h"
#include "CT_ImageHeader.h"

class __declspec(dllexport) CDICOM2XINAO  
{
public:
	CDICOM2XINAO();
	virtual ~CDICOM2XINAO();

	int  ConvertMRDcm2XB(MR_ImageHeader &MR_ImageHeader,char* lpDcm,int nDcmSize, WORD* &pBIImageData);
	int  ConvertMRDcm2XB(char* lpDcm, int nDcmSize, LPMR_STRUCT_IMAGE& pXinAoImg);
	int  ConvertCTDcm2XB(CT_ImageHeader &ct_imageHeader,char* lpDcm,int nDcmSize, WORD* &pBIImageData);
	int  ConvertCTDcm2XB(char* lpDcm, int nDcmSize, LPCT_STRUCT_IMAGE& pXinAoImg);

	void DCMTKConvertMRDcm2XB(void** dcmDataset,LPMR_STRUCT_IMAGE& pXinAoImg);
	void DCMTKConvertCTDcm2XB(void** dcmDataset,LPCT_STRUCT_IMAGE& pXinAoImg);

	//针对东芝CT病床设定的调节参数
	BOOL  m_bToshibaCTTable;
private:
	void TimeConvert(MR_BIDateTime datetime,char date[8],char time[6]);
	void TimeConvert2(MR_BIDateTime &datetime,const char *p,int flag);
	void TimeConvert2(CT_BIDateTime &datetime,const char *p,int flag);
	void NameConvert2(char BIname[], char DIname[], int vlength);
	unsigned char AgeConvert2(char DIage[]);
	unsigned short SwapByte(unsigned short sPoint);
	int   GetSeriesOption(char szSeriesName[]);
	void  GetDSVM(char* lpTemp, int nVLength, double dVal[], int& nVM);
	void  CalcBIImgDirec(double dDirCos[6], double dTopLeft[3] ,BYTE iPatPos, BYTE iPatEntry, 
		                 unsigned short sFovX, unsigned short sFovY,double dBIDirec[]) ;
};

#endif // !defined(AFX_DICOM2XINAO_H__3EA299EF_E376_4906_98F0_2EA3C7EA8945__INCLUDED_)
