// Segment.h: interface for the CSegment class.
// Created by Wei Wei [Dec.6, 2005]
// InterSmart Robotic Systems Co. Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENT_H__6A09EB89_D55F_4048_B51D_1B15226C5AB4__INCLUDED_)
#define AFX_SEGMENT_H__6A09EB89_D55F_4048_B51D_1B15226C5AB4__INCLUDED_

//#include "COMMON\Vertex3D.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "../Common/Vertex3D.h"
#include "../Common/ColorF.h"

class __declspec(dllexport) CSegment  
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CSegment();
	CSegment( Vector3D vP1, Vector3D vP2 );
	CSegment( Vector3D vP1, Vector3D vP2, Vector3D vecNormal );
	virtual ~CSegment();
	
	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Show or hide segment
	void SetVisible(BOOL bShow);
	BOOL IsVisible();

	// Get the first endpoint
	Vector3D	GetFirstEndpoint();

	// Get the second endpoint
	Vector3D	GetSecondEndpoint();

	// Get the segment's vector that from the first endpoint to the second
	Vector3D	GetVector();

	// Get the segment's normal
	Vector3D	GetNormal();

	// Get the length of the segment
	double		GetLength();

	// Construct the segment
	int			Construct( Vector3D vP1, Vector3D vP2 );

	// Construct the segment
	int			Construct( Vector3D vP1, Vector3D vP2, Vector3D vecNormal );

	// Construct the segment
	int			Construct( Vector3D vP1, Vector3D vOri, double dbLength);
	
	// Compute the distance to a point
	double		DistanceToPoint( Vector3D vecQueryPoint, Vector3D& projectedPoint );

	// Compute the squared distance to a point
	double		SquaredDistanceToPoint( Vector3D vecQueryPoint, Vector3D& projectedPoint );

	// Get the nearest point on the segment to a query point
	Vector3D	GetNearestPoint( Vector3D vecQueryPoint, double& dDist, 
									UINT& uGeoID, UINT& uType );

	// Compare with the other segment
	bool		Compare( CSegment* pSeg );

	// Get width of segment
	float GetWidth();

	// Set width of segment
	void SetWidth(float fWidth);

	// Get color
	CColorF* GetColor();

	// Set color 
	void SetColor(CColorF* pColor );
	
	/*3D绘制时是否禁用深度缓存*/
	BOOL m_bDisableDepthTest; 

	//TRUE:实线 FALSE:虚线
	BOOL m_bRealLine;
private:
	void Init();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////
	
	Vector3D	m_vP1;			// First endpoint
	Vector3D	m_vP2;			// Second endpoint
	// Vector3D	m_vecP1ToP2;	// Vector from the first endpoint to the second endpoint
	Vector3D	m_vecNormal;	// Normal of the segment
	
	float		m_fSegWidth;	// Width of segment in display
	double		m_dbSegLength;	// Length of segment 
	// Color
	// R, G, B, A: 0~255
	CColorF m_Color;

	BOOL m_bVisible;
};

#endif // !defined(AFX_SEGMENT_H__6A09EB89_D55F_4048_B51D_1B15226C5AB4__INCLUDED_)
