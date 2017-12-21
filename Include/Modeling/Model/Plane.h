// Plane.h: interface for the CPlane class.
// Created by Wei Wei [Oct.13, 2005]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANE_H__629E1D78_FC19_4CF1_B3D2_8E0AAEEA5655__INCLUDED_)
#define AFX_PLANE_H__629E1D78_FC19_4CF1_B3D2_8E0AAEEA5655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"

class __declspec(dllexport) CPlane  
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	// Plane's equation: vNormal*X = dSignedDistance
	// X is a point on the plane
	// vNormal is the normal to the plane
	// dSignedDistance is the signed distance
	// Sign of dSignedDistance:
	// + ~ If the vector from the origin to the plane has the same orientation as the plane's normal
	// - ~ If the vector from the origin to the plane has the opposite orientation to the plane's normal
	CPlane();
	CPlane( Vector3D vNormal, double dSignedDistance );
	virtual ~CPlane();
	
	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get the normal
	Vector3D	GetNormal();

	// Set the normal
	void SetNormal( Vector3D vNormal );

	// Get the signed distance from the origin to the plane
	// Sign of dSignedDistance:
	// + ~ If the vector from the origin to the plane has the same orientation as the plane's normal
	// - ~ If the vector from the origin to the plane has the opposite orientation to the plane's normal
	double		GetSignedDistance();

	// Set the signed distance
	void SetSignedDistance( double dSignedDistance );
		
	// Return the distance from the vPoint to the plane
	double		GetPlaneValue( Vector3D vPoint );
	
	// Get the projected point from a point to the plane along a projecting direction
	// Return value:
	// true ~ The projected point exist
	// false ~ No projected point because the projecting direction is parallel to the plane
	bool		PointProjection( Vector3D vPoint, Vector3D vProjectingDirection, Vector3D& projectedPoint );
	
	// bool		IntersectedBySegment( CSegment segment, Vector3D& intersectionPoint );	// 线段和平面相交
	// Members
private:
	Vector3D	m_vNormal;			// Plane's normal
	double		m_dSignedDistance;	// Signed distance from the origin to the plane
	
};

#endif // !defined(AFX_PLANE_H__629E1D78_FC19_4CF1_B3D2_8E0AAEEA5655__INCLUDED_)
