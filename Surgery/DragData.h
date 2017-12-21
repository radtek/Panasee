// DragData.h: interface for the CDragData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGDATA_H__29CD170A_D6EC_42CC_AE06_5112A984CAE4__INCLUDED_)
#define AFX_DRAGDATA_H__29CD170A_D6EC_42CC_AE06_5112A984CAE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>

class CDragData : public COleDataSource
{
public:
	CDragData();
	virtual ~CDragData();

	virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);

	CString m_sImgName;

};

#endif // !defined(AFX_DRAGDATA_H__29CD170A_D6EC_42CC_AE06_5112A984CAE4__INCLUDED_)
