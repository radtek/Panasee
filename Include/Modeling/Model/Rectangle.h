// Rectangle.h: interface for the CRectangle class.
// Created by Wei Wei [Apr.6, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTANGLE_H__763E046F_855E_4B32_B668_511121233D4C__INCLUDED_)
#define AFX_RECTANGLE_H__763E046F_855E_4B32_B668_511121233D4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "ModelObject.h"
//#include "../Common/Texture2D.h"

//class CBoundingBox;

// Rectangle ABCD in 3D space
class __declspec(dllexport) CRectangle : public CModelObject
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CRectangle();

	// Construct a rectangle ABCD using three points as vertex A, B and D
	// A - vVertex[0], B - vVertex[1], D - vVertex[2]
	CRectangle( Vector3D vVertex[3] );

	// Construct a rectangle using the center and two vectors as vector AB and vector AD
	// Vector AB is perpendicular to Vector AD
	CRectangle( Vector3D vCenter, Vector3D vAB, Vector3D vAD );
	
	virtual ~CRectangle();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Set rectangle ABCD using three points as vertex A, B and D
	// A - vVertex[0], B - vVertex[1], D - vVertex[2]
	int SetRectangle( Vector3D vVertex[3] );

	// Construct a rectangle using the center and two vectors as vector AB and vector AD
	// Vector AB is perpendicular to Vector AD
	int SetRectangle( Vector3D vCenter, Vector3D vAB, Vector3D vAD );

	CRectangle* clone();
	
	// Get the four vertices A, B, C and D
	Vector3D* GetVertices();

	// Get vertex A
	Vector3D GetVertexA();

	// Get vertex B
	Vector3D GetVertexB();

	// Get vertex C
	Vector3D GetVertexC();

	// Get vertex D
	Vector3D GetVertexD();

	// Get center
	Vector3D GetCenter();

	// Get vector AB
	Vector3D GetVectorAB();

	// Get vector CD
	Vector3D GetVectorAD();

	// Get normal
	Vector3D GetNormal();

	// Get width
	// |AB|
	double GetWidth();

	// Get width
	// |BC|
	double GetHeight();

	// Get area
	double GetArea();

	// Transform by 4*4 matrix
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Transform to a new position and a new orientation
	// The orientation needn't be normalized
	void Transformation( Vector3D vNewCenter, Vector3D vNewABOrientation, Vector3D vNewADOrientation );

	// Translation by 3*1 vector
	void Translation( Vector3D vTranslation );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( Vector3D vAxis, double dAngle );

	// Scale from a point
	void Scale( Vector3D vScaleCenter, double dRatio );

	// Set texture coordinates
	void SetTexCoor( Vector2D* pvTexCoor );

	// Get the flag of rendering border
	bool GetRenderingBorder();

	// Set the flag of rendering border
	void SetRenderingBorder( bool bRenderingBorder );

	// Get color of borer
	CColorF* GetColorOfBorder();

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// The four vertices
	//		Vertex A - m_vVertex[0];
	//		Vertex B - m_vVertex[1];
	//		Vertex C - m_vVertex[2];
	//		Vertex D - m_vVertex[3];
	Vector3D m_vVertex[4];

private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Initialize internal variables. This function should be called only by constructors.
    void Initialize();
	
	// Update bounding box
	void UpdateBoundingBox();

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Color of border
	// R, G, B, A: 0~255
	CColorF m_ColorOfBorder;

	// Flag of rendering border
	bool m_bRenderingBorder;

};

#endif // !defined(AFX_RECTANGLE_H__763E046F_855E_4B32_B668_511121233D4C__INCLUDED_)
