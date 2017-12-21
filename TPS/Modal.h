// Modal.h: interface for the CModal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODAL_H__6CD9FCB8_068F_4BEA_BB44_C5DD2D726CF6__INCLUDED_)
#define AFX_MODAL_H__6CD9FCB8_068F_4BEA_BB44_C5DD2D726CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TpsGlobalConst.h"
#include "vtkActor.h"
#include "vtkVolume.h"
#include "globalDef.h"

class CModal  
{
public:
	
	CModal();
	virtual ~CModal();

	virtual void clear();
	virtual BOOL readContent(FILE* file);
	virtual BOOL save(FILE* file);
	BOOL readHeader(FILE* file,MODALTYPE &modalType);
	

	CString m_name;
	HTREEITEM m_treeItem;
	COLORREF m_color;
	ZONETYPE m_zType;
	double m_volume;
	int m_opacity;
	vtkActor *m_actor;
	
	vtkVolume* m_pVolume;
	MODALTYPE m_modalType;

protected:
	vtkActor* GenerateMesh(vtkImageData *pImage, MESH_OPTIONS option);
private:
	vtkActor* GenerateActor(vtkPolyData *polyData, MESH_OPTIONS option);
	BOOL saveHeader(FILE* file);
};

#endif // !defined(AFX_MODAL_H__6CD9FCB8_068F_4BEA_BB44_C5DD2D726CF6__INCLUDED_)
