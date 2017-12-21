// Box.h: interface for the CBox class.
// Created by Wei Wei [Sep.14, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOX_H__25019CB9_A3E4_4D59_9868_82A21D26A7B3__INCLUDED_)
#define AFX_BOX_H__25019CB9_A3E4_4D59_9868_82A21D26A7B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "ModelObject.h"

class __declspec(dllexport) CBox : public CModelObject   
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CBox();
	CBox( Vector3D vVertex[8] );
	CBox( Vector3D vCenter, Vector3D vDirection[2], double dLengthOfEdge[3] );
	virtual ~CBox();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get vertices
	Vector3D* GetVertices();

	// Set vertices
	void SetVertices( Vector3D vVertices[8] );

	void GetVertices(Vector3D vVertices[8]);

	// Transform by 4*4 matrix
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Transformation
	void Transformation( Vector3D vNewCenter, Vector3D vNewAB, Vector3D vNewAD );

	// Translation by 3*1 vector
	void Translation( Vector3D vTranslation );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( Vector3D vAxis, double dAngle );

	// Scale from a point
	void Scale( Vector3D vScaleCenter, double dRatio );

	// Set texture coordinate
	void SetTexCoor( Vector2D* pvTexCoor );

	Vector3D GetCenter();

private:

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////////////////
	
	// Update bounding box
	void UpdateBoundingBox();

	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////
	
	// Vertices
	Vector3D m_vVertex[8];
};

#endif // !defined(AFX_BOX_H__25019CB9_A3E4_4D59_9868_82A21D26A7B3__INCLUDED_)
