// IntersectionObj.h: interface for the CIntersectionObj class.
// Created by Wei Wei [Sep.12, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERSECTIONOBJ_H__A81AA433_56DB_43BE_96F7_0126C20AD76B__INCLUDED_)
#define AFX_INTERSECTIONOBJ_H__A81AA433_56DB_43BE_96F7_0126C20AD76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Vertex3D;
class CSegment;
class CTriangle;
class CPlane;

enum INTERSECTION_TYPE
{
	INTERSECTION_NO_INTERSECTION,
	INTERSECTION_POINT,
	INTERSECTION_SEGMENT,
	INTERSECTION_TRIANGLE,
	INTERSECTION_PLANE,
};

class __declspec(dllexport) CIntersectionObj  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CIntersectionObj();
	virtual ~CIntersectionObj();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Get type of intersection object
	int GetType();

	// Set type of intersection object
	void SetType( int nType );

	// Type of the intersection object
	int m_nType;

	// Set point
	void SetPoint( Vertex3D* pVertex );

	// Get point
	Vertex3D* GetPoint();

	// Clear point
	void ClearPoint();

	// Set segment
	void SetSegment( CSegment* pSegment );

	// Get segment
	CSegment* GetSegment();

	// Clear segment
	void ClearSegment();

	// Set triangle
	void SetTriangle( CTriangle* pTriangle );

	// Get triangle
	CTriangle* GetTriangle();

	// Clear triangle
	void ClearTriangle();

	// Set plane
	void SetPlane( CPlane* pPlane );

	// Get plane
	CPlane* GetPlane();

	// Clear plane
	void ClearPlane();
	
private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////

	// Point
	Vertex3D* m_pPoint;

	// Segment
	CSegment* m_pSegment;

	// Triangle
	CTriangle* m_pTriangle;

	// Plane
	CPlane* m_pPlane;

};

#endif // !defined(AFX_INTERSECTIONOBJ_H__A81AA433_56DB_43BE_96F7_0126C20AD76B__INCLUDED_)
