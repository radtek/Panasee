// ModalContour.h: interface for the CModalContour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODALCONTOUR_H__2E3BE00D_39A3_4B9E_BF0A_9065576D158E__INCLUDED_)
#define AFX_MODALCONTOUR_H__2E3BE00D_39A3_4B9E_BF0A_9065576D158E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "modal.h"
#include "vtkImageData.h"
#include "globalDef.h"

class CModalContour :public CModal 
{
public:
	CModalContour();
	virtual ~CModalContour();

	void AddActor(int imageIndex,vtkActor *pActor,IMAGE_CONTOUR_INFO* info);

	virtual void clear();
	virtual BOOL readContent(FILE* file);
	virtual BOOL save(FILE *file);

	

private:	
	int m_iImageIndex;

	CString GetValidName();

	MESH_OPTIONS m_options;
};

#endif // !defined(AFX_MODALCONTOUR_H__2E3BE00D_39A3_4B9E_BF0A_9065576D158E__INCLUDED_)
