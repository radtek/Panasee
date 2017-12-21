// Ellipsoid.h: interface for the CEllipsoid class.
// Created by Wei Wei [Aug.29, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSOID_H__F118D4B5_28AB_44C0_A6C8_625D1F928C93__INCLUDED_)
#define AFX_ELLIPSOID_H__F118D4B5_28AB_44C0_A6C8_625D1F928C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "ModelObject.h"

class __declspec(dllexport) CEllipsoid : public CModelObject  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	// Default construction
	CEllipsoid();

	// Construct a common ellipsoid with three different semi-axes a, b, c
	CEllipsoid( Vector3D vCenter, Vector3D vSemiAxes[3] );

	// Construct a ellipsoid with semi-axes a, b, c (|a|=|b|)
	CEllipsoid( Vector3D vCenter, Vector3D vOrientation, double dDiameter, double dHeight );
	
	virtual ~CEllipsoid();

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////////////////

	// Get center
	Vector3D GetCenter();

	// Set center
	void SetCenter( Vector3D vCenter );

	// Get semi-axis a
	Vector3D GetSemiAxisA();

	// Set semi-axis a
	void SetSemiAxisA( Vector3D vSemiAxisA );

	// Get semi-axis b
	Vector3D GetSemiAxisB();

	// Set semi-axis b
	void SetSemiAxisB( Vector3D vSemiAxisB );

	// Get semi-axis c
	Vector3D GetSemiAxisC();

	// Set semi-axis c
	void SetSemiAxisC( Vector3D vSemiAxisC );

	// Set diameter, just for the ellipsoid with semi-axes a, b, c (|a|=|b|)
	void SetDiameter( double dDiameter );

	// Set height, just for the ellipsoid with semi-axes a, b, c (|a|=|b|)
	void SetHeight( double dHeight );

	// Get transformation from standard pose
	double** GetTransFromStdPose();

	// Set texture coordinate
	void SetTexCoor( Vector2D* pvTexCoor );

	// Transform by 4*4 matrix
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Transformation
	void Transformation( Vector3D vNewCenter, Vector3D vNewOrientation );

	// Translation by 3*1 vector
	void Translation( Vector3D vTranslation );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( Vector3D vAxis, double dAngle );

	// Scale from a point
	void Scale( Vector3D vScaleCenter, double dRatio );

private:

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////////////////
	
	// Update bounding box
	void UpdateBoundingBox();

	// Update transformation from standard pose
	void UpdateTransFromStdPose();

	// Reset the semi-axes
	void ResetSemiAxes();

	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////
	
	// Center
	Vector3D m_vCenter;
	
	// Semi-axes
	// a - m_vSemiAxis[0]
	// b - m_vSemiAxis[1]
	// c - m_vSemiAxis[2]
	Vector3D m_vSemiAxis[3];

	// Transformation from standard pose
	double** m_ppdTransFromStdPose;
};

#endif // !defined(AFX_ELLIPSOID_H__F118D4B5_28AB_44C0_A6C8_625D1F928C93__INCLUDED_)
