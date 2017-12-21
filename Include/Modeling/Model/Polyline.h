// Polyline.h: interface for the CPolyline class.
// Created by Wei Wei [Dec.12, 2005]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINE_H__666F9644_2412_4294_AB16_90B68A082FAA__INCLUDED_)
#define AFX_POLYLINE_H__666F9644_2412_4294_AB16_90B68A082FAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "../Common/Vertex3D.h"
#include "Segment.h"
#include <list>

//using namespace std;

// A polyline consists of a finite number of line segments. Adjacent line segments 
// share an end point. A polyline may be open or closed (polygon).
class __declspec(dllexport) CPolyline  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CPolyline();
	virtual ~CPolyline();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Construct the polyline with vertex pointer

	// Construct the polyline with vertex list

	// Construct the polyline with segment list
	int			Construct( std::list<CSegment> lSegment, bool bOpen );
	
	// Clear the polyline
	int			Clear();
	
	// Get the pointer to all the vertices
	Vector3D*	GetVertices();

	// Get the number of all the vertices
	int			GetNumOfVertices();

	// Get the pointer to all the segments
	CSegment*	GetSegments();

	// Get the number of all the segments
	int			GetNumOfSegments();
	
	// Whether the polyline is open
	bool		IsOpen();

	// Compute the signed distance to a query point
	double		SignedDistanceToPoint( Vertex3D& vQueryPoint, Vertex3D& vFootPoint );

	// Save the vertices to PC file
	int			SaveToPCFile( CString sFileName );

	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////
	
	

private:
	
	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////
	
	// Pointer to the vertices
	Vector3D*	m_pVertex;

	// Number of the vertices
	int			m_nNumOfVertices;

	// Pointer to the segments
	CSegment*	m_pSegment;

	// Number of the segments
	int			m_nNumOfSegments;

	// Flag indicating whether the polyline is open
	bool		m_bOpen;

};

#endif // !defined(AFX_POLYLINE_H__666F9644_2412_4294_AB16_90B68A082FAA__INCLUDED_)
