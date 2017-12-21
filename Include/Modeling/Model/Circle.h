// Circle.h: interface for the CCircle class.
// Created by Wei Wei [Mar.31, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLE_H__2ECA000A_4FDF_4110_A77D_1A4E3CD1ED14__INCLUDED_)
#define AFX_CIRCLE_H__2ECA000A_4FDF_4110_A77D_1A4E3CD1ED14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"

class CBoundingBox;

class __declspec(dllexport) CCircle  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CCircle();
	CCircle( Vector3D vCenter, double dRadius, Vector3D vNormal );
	virtual ~CCircle();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Get center
	Vector3D GetCenter();

	// Set center
	void SetCenter( Vector3D vCenter );

	// Get radius
	double GetRadius();

	// Set radius
	void SetRadius( double dRadius );

	// Get normal
	Vector3D GetNormal();

	// Set normal
	void SetNormal( Vector3D vNormal );

	// Get bounding box
	CBoundingBox* GetBoundingBox();

	// Get diameter
	double GetDiameter();

	// Get circumference
	double GetCircumference();

	// Get area
	double GetArea();
	
private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Update bounding box
	void UpdateBoundingBox();

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Center
	Vector3D m_vCenter;

	// Radius
	double m_dRadius;

	// Normal
	Vector3D m_vNormal;

	// Bounding box
	CBoundingBox* m_pBoundingBox;
	

};

#endif // !defined(AFX_CIRCLE_H__2ECA000A_4FDF_4110_A77D_1A4E3CD1ED14__INCLUDED_)
