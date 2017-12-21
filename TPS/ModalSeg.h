// ModalSeg.h: interface for the CModalSeg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODALSEG_H__DF2BB635_EE23_4B30_A613_016F3A6C6101__INCLUDED_)
#define AFX_MODALSEG_H__DF2BB635_EE23_4B30_A613_016F3A6C6101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "modal.h"
#include "SaveActor2Obj.h"

class CModalSeg  :public CModal
{
public:
	CModalSeg();
	virtual ~CModalSeg();


	void AddActor(int imageIndex,vtkActor *pActor,MESH_OPTIONS meshOption,BOOL bSaveObjFile,CString sTpsFile);
	
	virtual void clear();
	virtual BOOL readContent(FILE* file,CString sTpsFile);
	virtual BOOL save(FILE *file,CString sTpsFile);

private:
	CString GetValidObjPath(CString sTpsFile);
	vtkActor* ReadObjFile(CString sFile);
	CString SaveObjFile(vtkActor *pActor,CString sTpsFile);
	int m_iImageIndex;
	CString m_sObjFileName;

	CString GetValidName(CString sFirstName);
	
	CSaveActor2Obj m_saveObj;
};

#endif // !defined(AFX_MODALSEG_H__DF2BB635_EE23_4B30_A613_016F3A6C6101__INCLUDED_)
