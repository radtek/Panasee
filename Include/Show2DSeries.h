// Show2DSeries.h: interface for the CShow2DSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOW2DSERIES_H__D5432BAE_A770_4AC8_A8B7_EBC09ABCDC24__INCLUDED_)
#define AFX_SHOW2DSERIES_H__D5432BAE_A770_4AC8_A8B7_EBC09ABCDC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRISeries.h"

#ifdef _BUILD_IMGFUSION_DLL
	#define IMGFUSION_API		__declspec(dllexport)
#else
	#define IMGFUSION_API		 __declspec(dllimport)
#endif

class IMGFUSION_API CShow2DSeries  
{
public:
	void SetCrossLine(double *dPos);
	void SetSeries(CMRISeries *pSeries);
	void Show(CWnd *pParentWnd,CRect rc);
	void Hide();
	CShow2DSeries();
	virtual ~CShow2DSeries();

private:
	CWnd *m_pDlg;

};

#endif // !defined(AFX_SHOW2DSERIES_H__D5432BAE_A770_4AC8_A8B7_EBC09ABCDC24__INCLUDED_)
