// Intersection.h: interface for the CIntersection class.
// Created by Wei Wei [Nov.25, 2005]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERSECTION_H__A3881D71_73BB_4480_AC54_ADC22A11CD74__INCLUDED_)
#define AFX_INTERSECTION_H__A3881D71_73BB_4480_AC54_ADC22A11CD74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <afxtempl.h>
#include "IntersectionObj.h"

class Vector3D;
class PointCloud;
class CMesh;
class CPlane;
class CBoundingBox;
class CPolyline;
class Line3D;
class CRectangle;
class CCylinder;
class CSegment;
class CTriangle;
class Vertex3D;

class __declspec(dllexport) CIntersection  
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CIntersection();
	virtual ~CIntersection();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// ------------------------------------------------------------------------------------------------------------//
	// Basic Models

	// Intersection of two lines
	int			Intersect( Line3D* pLineA, Line3D* pLineB, Vector3D& vIntersectionPoint );

	// Intersection of a line and a plane
	int			Intersect( Line3D* pLine, CPlane* pPlane, Vector3D& vIntersectionPoint );

	// Intersection of a line and a rectangle
	int			Intersect( Line3D* pLine, CRectangle* pRectangle, Vector3D& vIntersectionPoint );

	// Intersection of a line and a rectangle
	int			Intersect( Line3D* pLine, CRectangle* pRectangle, Vector3D& vIntersectionPoint, double& dbIntersectAngle );

	// Intersection of a line and a triangle
	int			Intersect( Line3D* pLine, CTriangle* pTriangle, Vector3D& vIntersectionPoint );

	// Intersection of a triangle and a plane
	void		Intersect( CTriangle* pTriangle, CPlane* pPlane, CIntersectionObj* pIntersectionObj );

	// Intersection of a cylinder and a rectangle
	// Use a rectangle to approximate the intersection area
	int			Intersect( CCylinder* pCylinder, CRectangle* pRectangle, CRectangle* pIntersectionArea );

	// Intersection of a cylinder and a rectangle  Added by Huagen 2006.10.09
	// Return position of intersection    
	int			InterSect( CCylinder* pCyliner, CRectangle* pRectangle, Vector3D& vInterSecPos);

	// ------------------------------------------------------------------------------------------------------------//
	// Advanced Models

	// Intersection of point cloud and plane
	int			Intersect( PointCloud* pPointCloud, 
										CPlane* pPlane, double dThreshold, 
										std::list<Vector3D>& lvIntersectedPoints );

	// Intersection of point cloud and plane
	int			Intersect( PointCloud* pPointCloud, 
										CPlane* pPlane, double dThreshold,
										PointCloud*& pPointsFromIntersection );

	// Intersection of mesh and plane
	// Only can handle closed mesh
	int			Intersect( CMesh* pMesh, CPlane* pPlane, bool bOpen, 
										CPolyline*& pPolylineFromMesh );

	// Intersection of mesh and plane
	// Can handle both open and closed mesh
	int			Intersect( CMesh* pMesh, CPlane* pPlane, 
										CList<CPolyline*,CPolyline*>* plPolylineFromMesh );

	// Intersection of mesh and plane
	int			Intersect( CMesh* pMesh, CPlane* pPlane, 
										CList<CSegment*,CSegment*>* plSegmentsFromMesh );

	// Intersection of line and mesh
	// vVertex: the first intersection point
	int			Intersect( CMesh* pMesh, Line3D* pLine, Vertex3D& vVertex );

	// Intersection of bounding box and plane
	// true~intersect; false~no intersection
	bool		IsIntersectant( CBoundingBox* pBoundingBox, CPlane* pPlane );

	// Intersection of bounding box and bounding box
	// true~intersect; false~no intersection
	bool		IsIntersectant( CBoundingBox* pBoundingBox_A, CBoundingBox* pBoundingBox_B );

	// ------------------------------------------------------------------------------------------------------------//

private:
	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Intersection of mesh and plane in closed mode
	int			IntersectionOfMeshAndPlaneClosedMode( CMesh* pMesh, CPlane* pPlane, 
										CPolyline*& pPolylineFromMesh );

	// Intersection of mesh and plane in open mode
	int			IntersectionOfMeshAndPlaneOpenMode( CMesh* pMesh, CPlane* pPlane, 
										CList<CSegment*,CSegment*>* plSegmentsFromMesh );
};

#endif // !defined(AFX_INTERSECTION_H__A3881D71_73BB_4480_AC54_ADC22A11CD74__INCLUDED_)
