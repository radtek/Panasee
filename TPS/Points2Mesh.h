// Points2Mesh.h: interface for the CPoints2Mesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTS2MESH_H__613D06D0_0B66_4BAA_AEDD_FE93E873073F__INCLUDED_)
#define AFX_POINTS2MESH_H__613D06D0_0B66_4BAA_AEDD_FE93E873073F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VtkHeader.h"
struct PNT3D
{
	float x,y,z;
};

enum MESH_METHOD
{
	DELAUNAY,
	POWERSUFR,
	VTKSURFACE
};

// 使用POWERSUFR方法时，存在内存泄漏问题 //
class CPoints2Mesh  
{
public:
	vtkActor* GetActor();
	CPoints2Mesh();
	virtual ~CPoints2Mesh();

	PNT3D *m_pPnts;
	int m_iCount;

	MESH_METHOD m_meshType;

	CString m_sTextureFile;

};

#endif // !defined(AFX_POINTS2MESH_H__613D06D0_0B66_4BAA_AEDD_FE93E873073F__INCLUDED_)
