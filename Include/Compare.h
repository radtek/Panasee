// Compare.h: interface for the CCompare class.
// Created by Wei Wei [Nov.25, 2005]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPARE_H__4C0D8EEE_8F7D_4A26_87EA_B4AAC00DAA2C__INCLUDED_)
#define AFX_COMPARE_H__4C0D8EEE_8F7D_4A26_87EA_B4AAC00DAA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Statistics.h"
#include <afxtempl.h>

class Vector3D;
class PointCloud;
class CMesh;
class CPlane;
class CBoundingBox;
class CPolyline;
class CSegment;

class __declspec(dllexport) CCompare  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CCompare();
	virtual ~CCompare();
	
	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// 3D Compare between point cloud and mesh
	int			Compare3D( PointCloud* pPointCloud, CMesh* pMesh, CStatistics& sStatistics );
	
	// 2D Compare between point cloud and mesh (close) on a plane
	int			Compare2D( PointCloud* pPointCloud, CMesh* pMesh, CPlane* pPlane, double dThreshold, 
							PointCloud*& pPointsFromPC, CList<CSegment*,CSegment*>* plSegments,
							PointCloud*& pFootPointsOnSegments, CStatistics& sStatistics );

	// 2D Compare between point cloud and mesh (close) on a plane
	int			Compare2D( PointCloud* pPointCloud, CMesh* pMesh, CPlane* pPlane, double dThreshold, 
							PointCloud*& pPointsFromPC, CPolyline*& pPolylineFromMesh, 
							PointCloud*& pFootPointsOnPolyline, CStatistics& sStatistics );

	// 2D Compare between points and polyline
	int			Compare2D( PointCloud* pPoints, CPolyline* pPolyline, 
							PointCloud* pFootPoints, CStatistics& sStatistics );

	// 2D Compare between points and a list of segments
	int			Compare2D(PointCloud* pPoints, CList<CSegment*,CSegment*>* plSegments,
							PointCloud* pFootPoints, CStatistics& sStatistics );

};

#endif // !defined(AFX_COMPARE_H__4C0D8EEE_8F7D_4A26_87EA_B4AAC00DAA2C__INCLUDED_)
