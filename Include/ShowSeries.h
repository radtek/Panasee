// ShowSeries.h: interface for the CShowSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWSERIES_H__87F17D58_751C_4047_BFAB_FA3293BADA4D__INCLUDED_)
#define AFX_SHOWSERIES_H__87F17D58_751C_4047_BFAB_FA3293BADA4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_IMGDISPLAY_DLL
	#define IMGDISPLAY_API		__declspec(dllexport)
#else
	#define IMGDISPLAY_API		 __declspec(dllimport)
#endif

#include "mriSeries.h"
#include "mv.h"

class IMGDISPLAY_API CShowSeries  
{
public:
	void ChangeView2USImage();
	void SetCrossPnt(double *pnt);
	void SetWL(int width, int level);
	void SetBase(Matrix mBase);
	void ShowSeriesByCoord(CMRISeries *pSeries,Matrix mProbeInImage,BOOL bKeepView=TRUE);
	void ShowSeriesByIndex(CMRISeries* pSeries,int index,BOOL bKeepView=TRUE);
	void ShowSeries(CWnd *parentWnd,CRect rc);
	CShowSeries();
	virtual ~CShowSeries();

private:
	void Convert(US_STRUCT_IMAGE *pImage,Matrix mBase);
	LPUS_STRUCT_IMAGE GetUSImage(CMRISeries *pSeries, int index);

	CString m_sDispOri[4];
	// Characters for orientation (A, P, S, I, R, L)
	char** m_ppcOrientation;	
	void ComputeOrientChar(US_STRUCT_IMAGE *pImage);
	void CorrectImageDispOrientation();

	CDialog *m_pDlg;

	Matrix m_tScannerInBase;
};

#endif // !defined(AFX_SHOWSERIES_H__87F17D58_751C_4047_BFAB_FA3293BADA4D__INCLUDED_)
