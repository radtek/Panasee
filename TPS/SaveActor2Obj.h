// SaveActor2Obj.h: interface for the CSaveActor2Obj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEACTOR2OBJ_H__0E86015F_EC6E_49E2_B9AF_B763BE6FBA03__INCLUDED_)
#define AFX_SAVEACTOR2OBJ_H__0E86015F_EC6E_49E2_B9AF_B763BE6FBA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vtkActor.h"

class CSaveActor2Obj  
{
public:
	void Save();
	CSaveActor2Obj();
	virtual ~CSaveActor2Obj();

	vtkActor *m_pActor;
	CString m_sObjFile;
};

UINT SaveActor2Obj(LPVOID pSaveActor2Obj);

#endif // !defined(AFX_SAVEACTOR2OBJ_H__0E86015F_EC6E_49E2_B9AF_B763BE6FBA03__INCLUDED_)
