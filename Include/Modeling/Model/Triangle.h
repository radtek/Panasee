// Triangle.h: interface for the CTriangle class.
// Created by Wei Wei [Dec.6, 2005]
// InterSmart Robotic Systems Co. Ltd.
//////////////////////////////////////////////////////////////////////

#include "../Common/Geometry.h"
#include "../Common/Vertex3D.h"

#if !defined(AFX_TRIANGLE_H__774006FB_1B6E_417B_9CB5_9689F41F7375__INCLUDED_)
#define AFX_TRIANGLE_H__774006FB_1B6E_417B_9CB5_9689F41F7375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __declspec(dllexport) CTriangle  
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CTriangle();
	CTriangle( Vector3D vVertex_0, Vector3D vVertex_1, Vector3D vVertex_2, 
				Vector3D vecNormal );
	CTriangle( Vector3D vVertex[3], Vector3D vecNormal );
	virtual ~CTriangle();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Set
	void SetTriangle( Vector3D vVertex_0, Vector3D vVertex_1, Vector3D vVertex_2, 
				Vector3D vecNormal );
	void SetTriangle( Vector3D vVertex[3], Vector3D vecNormal );

	// Get vertex A
	Vector3D GetVertexA();

	// Get vertex A
	void GetVertexA( Vertex3D& vVertex );

	// Get vertex B
	Vector3D GetVertexB();

	// Get vertex B
	void GetVertexB( Vertex3D& vVertex );

	// Get vertex C
	Vector3D GetVertexC();

	// Get vertex C
	void GetVertexC( Vertex3D& vVertex );

	// Get normal
	Vector3D GetNormal();

	// Get vector AB
	Vector3D GetVectorAB();

	// Get vector BC
	Vector3D GetVectorBC();

	// Get vector CA
	Vector3D GetVectorCA();

	// Get the area of the triangle
	double		Area();

	// Get the signed distance from the triangle to a point
	double		SignedDistanceFromTriangleToPoint( Vector3D vecPoint );

	// Get the signed distance from the plane where the triangle lies to a point
	double		SignedDistanceFromTrianglePlaneToPoint( Vector3D vecPoint );

	// Determine whether a point is inside the triangle
	bool		IsInsideTriangle( Vector3D vecPoint );

	// Get the footpoint of a query point
	// The nearest point is inside the triangle
	Vector3D	GetNearestPoint( Vector3D vecQueryPoint, double& dSignedDist, 
								UINT& uGeoID, UINT& uType );

	// Get the footpoint of a query point
	// The footpoint is on the plane where the triangle lies and may be outside the triangle
	Vector3D	GetFootPointOnTrianglePlane( Vector3D vecQueryPoint, double& dSignedDist );
	
	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////


protected:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////


private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////
	// Members
	//////////////////////////////////////////////////////////////////////////
	Vector3D	m_vVertex[3];	// Vertex
	Vector3D	m_vNormal;		// Normal
	
};

#endif // !defined(AFX_TRIANGLE_H__774006FB_1B6E_417B_9CB5_9689F41F7375__INCLUDED_)
