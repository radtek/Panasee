// Sphere.h: interface for the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHERE_H__51E9664D_3502_4E38_9B06_162FB3AC6131__INCLUDED_)
#define AFX_SPHERE_H__51E9664D_3502_4E38_9B06_162FB3AC6131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Geometry.h"
#include "ModelObject.h"

class __declspec(dllexport) CSphere : public CModelObject
{
public:
	CSphere();
	CSphere( Vector3D vCenter, double dRadius );
	virtual ~CSphere();

	void SetRadius( double dbRadius );
	double GetRadius();

	void SetCenter( Vector3D vCenter );
	Vector3D GetCenter();

	// Set texture coordinate
	void SetTexCoor( Vector2D* pvTexCoor );

	// Transform by 4*4 matrix
	// ppdTrans: 4*4
	void Transformation( double** ppdTrans );

	// Translation by 3*1 vector
	void Translation( Vector3D vTranslation );

	// Rotate by 3*3 matrix
	void Rotation( double** ppdRot );

	// Rotate dAngle degree about an axis
	void Rotation( Vector3D vAxis, double dAngle );

	// Scale from a point
	void Scale( Vector3D vScaleCenter, double dRatio );

private:

	double m_dbRadius;
	Vector3D m_vCenter;
};

#endif // !defined(AFX_SPHERE_H__51E9664D_3502_4E38_9B06_162FB3AC6131__INCLUDED_)
