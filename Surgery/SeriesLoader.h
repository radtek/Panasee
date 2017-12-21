// SeriesLoader.h: interface for the CSeriesLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIESLOADER_H__83B1BB94_C1E9_4203_828E_70AC5667B51E__INCLUDED_)
#define AFX_SERIESLOADER_H__83B1BB94_C1E9_4203_828E_70AC5667B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRISeries.h"

class CSeriesLoader  
{
public:
	CMRISeries* GetAt(int index);
	int GetCount();
	void LoadSeries(CString sPath);
	CSeriesLoader();
	virtual ~CSeriesLoader();
private:
	CList<CMRISeries*,CMRISeries*> m_seriesList;
};

#endif // !defined(AFX_SERIESLOADER_H__83B1BB94_C1E9_4203_828E_70AC5667B51E__INCLUDED_)
