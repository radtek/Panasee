// Reconstruction.h: interface for the CReconstruction class.
// Created by Wei Wei [Dec.20, 2005]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECONSTRUCTION_H__55B3D56A_1B0F_4052_86C3_1334402D09F5__INCLUDED_)
#define AFX_RECONSTRUCTION_H__55B3D56A_1B0F_4052_86C3_1334402D09F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointCloud;
class CMesh;
class CPlane;
class Line3D;
class Vector3D;
class CCylinder;

class __declspec(dllexport) CReconstruction  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CReconstruction();
	virtual ~CReconstruction();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Reconstruction of Advanced Model //

	// Construct mesh from point cloud
	int			Reconstruct( PointCloud* pPointCloud, CMesh*& pMesh );

	// Reconstruction of Basic Model //

	// Fit a line to 3D points
	int			Fit( Vector3D* pvPointSet, int nSizeOfPointSet, Line3D*& pLine, double& dStdDev, double& dMaxErr );

	// Fit a plane to 3D points
	int			Fit( Vector3D* pvPointSet, int nSizeOfPointSet, CPlane*& pPlane, double& dStdDev, double& dMaxErr );

	// Fit a cylinder to 3D points
	int			Fit( Vector3D* pvPointSet, int nSizeOfPointSet, CCylinder*& pCylinder, double& dStdDev, double& dMaxErr );

};

#endif // !defined(AFX_RECONSTRUCTION_H__55B3D56A_1B0F_4052_86C3_1334402D09F5__INCLUDED_)
