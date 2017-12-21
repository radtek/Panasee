// Projection.h: interface for the CProjection class.
// Created by Wei Wei [Mar.16, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTION_H__989FE00B_4538_4A20_8D4B_5ECD09A2E9DD__INCLUDED_)
#define AFX_PROJECTION_H__989FE00B_4538_4A20_8D4B_5ECD09A2E9DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Geometry.h"
//#include "Plane.h"
// #include "Line3D.h"
class Vector3D;
class CPlane;
class Line3D;

class __declspec(dllexport) CProjection  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CProjection();
	virtual ~CProjection();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Projection of a vector onto a plane along its normal
	int VectorOntoPlane( Vector3D* pVec, CPlane* pPlane, Vector3D* pProjectionVector );

	// Projection of a point onto a plane along its normal
	void PointOntoPlane( Vector3D* pPoint, CPlane* pPlane, Vector3D* pProjectoinPoint );
	
	// Projection of a line onto a plane along its normal
	int LineOntoPlane( Line3D* pLine, CPlane* pPlane, Line3D* pProjectionLine );

};

#endif // !defined(AFX_PROJECTION_H__989FE00B_4538_4A20_8D4B_5ECD09A2E9DD__INCLUDED_)
