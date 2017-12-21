#pragma once

#include "Geometry.h"

class __declspec(dllexport) Vertex3D
{
public:
	Vertex3D(void);
	virtual ~Vertex3D(void);
	Vertex3D& operator = (Vertex3D& v);
	Vector3D GetCoordinates();
	void SetCoordinates( Vector3D p );
	Vector3D GetNormal();
	void SetNormal( Vector3D n );
public:
	Vector3D p;				//顶点所在坐标
	// Vector3D footpoint;		//顶点在曲线上的footpoint
	Vector3D* n;				//法向量	
	// Vector3D n1;			//主切向量1// Commented by Wei Wei [Feb.16, 2006]
	// Vector3D n2;			//主切向量2// Commented by Wei Wei [Feb.16, 2006]
	// double dis;				//到切平面的距离	// Commented by Wei Wei [Feb.16, 2006]
	// double dis1;			//到主法平面1的距离	// Commented by Wei Wei [Feb.16, 2006]
	// double dis2;			//到主法平面2的距离	// Commented by Wei Wei [Feb.16, 2006]
	// double alfa1;			//距离平方函数的系数1	// Commented by Wei Wei [Feb.16, 2006]
	// double alfa2;			//距离平方函数的系数2 // Commented by Wei Wei [Feb.16, 2006]
	BYTE r;					// 颜色分量r
	BYTE g;					// 颜色分量g
	BYTE b;					// 颜色分量b
	void copyVertex(const Vertex3D& vertex);	
	bool isPicked;
};
