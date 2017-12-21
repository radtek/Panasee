// MRIRectSeries.h: interface for the MRIRectSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRIRECTSERIES_H__A5D1214E_C276_4D14_AF57_B2462288649F__INCLUDED_)
#define AFX_MRIRECTSERIES_H__A5D1214E_C276_4D14_AF57_B2462288649F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRIRectangle.h"
#include "MRISeries.h"


class MRIRectSeries  
{
public:
	MRIRectSeries();
	virtual ~MRIRectSeries();

	BOOL  Init(CMRISeries *pMRISeries);
	void Clean();
	void  UpdateTexture(BOOL bUpdateAllImage);
	void  UpdatePos();
//	BOOL  CreateMRIRectAt(int iIndex, CMRIImage* pMRIImg);
	CMRIRectangle* GetMRIRectAt(int iIndex);
	
	// Display MRI rect functions
	CMRIRectangle* GetDispMRIRect();
	void SetDispIndex(int iIndex);
	int  GetDispIndex();

	CMRISeries* GetMRISeries();

	CString GetName();
	void SetName(CString sName);

	int  GetRectNum();

	int m_iRectNum;
	int m_iResolutionX, m_iResolutionY;
	
private:
	
	CMRIRectangle** m_ppMRIRects;
	CMRIRectangle m_dispMRIRect;  // MRIRect only used for display
	int m_idispMRIRectIndex;  // index of m_dispMRIRect

	CMRISeries *m_pMRISeries;

	CString m_sName;
};

#endif // !defined(AFX_MRIRECTSERIES_H__A5D1214E_C276_4D14_AF57_B2462288649F__INCLUDED_)
