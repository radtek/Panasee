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
	Vector3D p;				//������������
	// Vector3D footpoint;		//�����������ϵ�footpoint
	Vector3D* n;				//������	
	// Vector3D n1;			//��������1// Commented by Wei Wei [Feb.16, 2006]
	// Vector3D n2;			//��������2// Commented by Wei Wei [Feb.16, 2006]
	// double dis;				//����ƽ��ľ���	// Commented by Wei Wei [Feb.16, 2006]
	// double dis1;			//������ƽ��1�ľ���	// Commented by Wei Wei [Feb.16, 2006]
	// double dis2;			//������ƽ��2�ľ���	// Commented by Wei Wei [Feb.16, 2006]
	// double alfa1;			//����ƽ��������ϵ��1	// Commented by Wei Wei [Feb.16, 2006]
	// double alfa2;			//����ƽ��������ϵ��2 // Commented by Wei Wei [Feb.16, 2006]
	BYTE r;					// ��ɫ����r
	BYTE g;					// ��ɫ����g
	BYTE b;					// ��ɫ����b
	void copyVertex(const Vertex3D& vertex);	
	bool isPicked;
};
