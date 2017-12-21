// DragData.cpp: implementation of the CDragData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "DragData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDragData::CDragData()
{

}

CDragData::~CDragData()
{

}

BOOL CDragData::OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile)
{
	if (lpFormatEtc->cfFormat == CF_TEXT)
	{
		pFile->Write(m_sImgName, m_sImgName.GetLength());
		return TRUE;
	}

	COleDataSource::OnRenderFileData(lpFormatEtc, pFile);

	return FALSE;
}