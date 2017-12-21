// Cylinder.h: interface for the CCylinder class.
// Created by Wei Wei [Mar.19, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYLINDER_H__9271A00A_7A7F_48B2_8E4F_8139F6DE4EE4__INCLUDED_)
#define AFX_CYLINDER_H__9271A00A_7A7F_48B2_8E4F_8139F6DE4EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"

class CBoundingBox;

class __declspec(dllexport) CCylinder  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CCylinder();
	CCylinder( Vector3D vCentroid, Vector3D vAxis, double dRadius, double dHalfHeight );
	virtual ~CCylinder();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get centroid
	Vector3D GetCentroid();

	// Set centroid
	void SetCentroid( Vector3D vCentroid );

	// Get axis
	Vector3D GetAxis();

	// Set axis
	void SetAxis( Vector3D vAxis );

	// Get radius
	double GetRadius();

	// Set radius
	void SetRadius( double dRadius );

	// Get half height
	double GetHalfHeight();

	// Set half height
	void SetHalfHeight( double dHalfHeight );

	// Get display color
	COLORREF GetColorRef();

	// Set display color
	void SetColorRef(COLORREF colorRef);

	// Set visible
	void SetVisible(BOOL bVisible);

	// Get visible
	BOOL IsVisible();

	/*3D绘制时是否禁用深度缓存*/
	BOOL m_bDisableDepthTest; 
private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Update bounding box
	void UpdateBoundingBox();

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Centroid
	Vector3D m_vCentroid;

	// Axis
	Vector3D m_vAxis;

	// Radius
	double m_dRadius;

	// Half height
	double m_dHalfHeight;

	// Color
	COLORREF m_crColor;

	// Bounding box
	CBoundingBox* m_pBoundingBox;

	// visible, default is TRUE;
	BOOL m_bVisible;
};

#endif // !defined(AFX_CYLINDER_H__9271A00A_7A7F_48B2_8E4F_8139F6DE4EE4__INCLUDED_)
