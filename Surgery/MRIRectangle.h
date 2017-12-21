// MRIRectangle.h: interface for the CMRIRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRIRECTANGLE_H__7BB17F93_D782_4625_B06A_6F2E091745B5__INCLUDED_)
#define AFX_MRIRECTANGLE_H__7BB17F93_D782_4625_B06A_6F2E091745B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rectangle.h"
#include "MRIImage.h"

#define MAX_HANDLES				20
#define MAX_VTARGET				20

class MRIRectSeries;
class CSphere;

class CMRIRectangle  
{
public:
	void RemoveMRIRect();
	CMRIRectangle();
	CMRIRectangle(CMRISeries *pMRISeries, int iImgIndex);
	CMRIRectangle(MRIRectSeries *pMRIRectSeries, int iIndex);
	virtual ~CMRIRectangle();
	

	void Clean();
	BOOL CreateMRIRect();
	BOOL SetName();
	BOOL SetMRIImg(CMRIImage *pMRIImg, bool bUpdateTexture = false);   // Set MRI image and update texture
	
	void SetVisible3D(BOOL nCmdShow);
	BOOL IsVisible3D();

	void UpdatePos();

	// Image rectangle
	CRectangle* m_pRectangle;

	
	MRIRectSeries* m_pMRIRectSeries;  // point to MRIseries belongs to

	CMRIImage* GetMRIImg();

	CString m_sName;
	int   m_iIndex;  // Index in the MRIRectSeries
	BOOL  m_bIsVisible;
	BOOL  m_bIn3DWindows;

private:

	CMRIImage *m_pMRIImg;
	void Init();
	BOOL CreateTexture();
	
	BOOL  m_bIsVisible3D; // Specify whether displayed in 3D window
};

#endif // !defined(AFX_MRIRECTANGLE_H__7BB17F93_D782_4625_B06A_6F2E091745B5__INCLUDED_)
