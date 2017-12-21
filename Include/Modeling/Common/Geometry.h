// Vector2D.h: interface for the Vector2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR2D_H__A44A61DD_F250_4836_8BE4_62A5220C460F__INCLUDED_)
#define AFX_VECTOR2D_H__A44A61DD_F250_4836_8BE4_62A5220C460F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
#include <math.h>
//#include "../Model/mesh.h"
class CVertex;
class CEdge;
class CFace;
//#include "common.h"
#define DOUBLE_EPS 1e-8
// double zero eps
#define	EQUALZERO(x)	(fabs((x)) < DOUBLE_EPS)
#ifndef PI
#define PI acos(-1.0)
#endif
#define TOLER		1e-6
#define MAX_DOUBLE	1e+30
#define MIN_DOUBLE	-1e+30




/////////////////////////////////////////////////////////////
// Vector2D : 2D vector
/////////////////////////////////////////////////////////////
class __declspec(dllexport) Vector2D  
{
public :
	double x,y;

public:
	Vector2D(){	x = 0;	y = 0;}
	// constructions
	Vector2D(double xx,double yy)	{	x = xx;	y = yy;}
	Vector2D(const Vector2D& v)	{	x = v.x;	y = v.y;}

  
	// operator
	double	  length()		{	return sqrt(x*x + y*y);	}
	double	  length2()		{	return x*x + y*y;	}
	double	  normalize()	{	double len = length();	if (!EQUALZERO(len)) {x/= len;y/=len;}	return len;	}
	Vector2D& operator=(const Vector2D& v);
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(double u);
	Vector2D& operator/=(double u);
//	Vector2D& operator^=(const Vector2D& v);

	bool	Intersect(Vector2D v1,Vector2D v2,Vector2D v3,Vector2D v4);
	bool	Intersect(Vector2D v1,Vector2D v2);

	__declspec(dllexport) friend Vector2D operator+(const Vector2D& lv, const Vector2D& rv);
	__declspec(dllexport) friend Vector2D operator-(const Vector2D& lv, const Vector2D& rv);
	__declspec(dllexport) friend Vector2D operator*(const double u, const Vector2D& rv);
	__declspec(dllexport) friend Vector2D operator*(const Vector2D& lv, const double u);
	__declspec(dllexport) friend Vector2D operator/(const Vector2D& lv, const double u);
	__declspec(dllexport) friend double   operator*(const Vector2D& lv, const Vector2D& rv);
//	friend Vector2D operator^(const Vector2D& lv, const Vector2D& rv);

	short	AtWhere(Vector2D v0,Vector2D v1);
	bool	AtRight(Vector2D v0,Vector2D v1);
	bool	AtLeft(Vector2D v0,Vector2D v1);
	bool	OnLine(Vector2D v0,Vector2D v1);
	double	GetArea(Vector2D v);
	

};

/////////////////////////////////////////////////////////////
// Vector3D : 3D vector
/////////////////////////////////////////////////////////////
class __declspec(dllexport) Vector3D 
{
public :
	double x,y,z;

	// constructions
	Vector3D()	{	x = 0;	y = 0;	z = 0;	}
	Vector3D(double xx,double yy,double zz)	{	x = xx;	y = yy;	z = zz;	}
	Vector3D(const Vector3D& v)	{	x = v.x;	y = v.y;	z = v.z;	}

	// operator
	double	  length()		{	return sqrt(x*x + y*y + z*z);	}
	double	  length2()		{	return x*x + y*y + z*z;	}
	double	  normalize()	{	double len = length();	if (!EQUALZERO(len)) {x/= len;y/=len;z/=len;}	return len;	}
	double	  DisFrom( Vector3D v )  { return sqrt( (x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z) ); }
	void	  Reverse() { x = -x; y = -y; z = -z; }
	Vector3D& operator=(const Vector3D& v);
	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator*=(double u);
	Vector3D& operator/=(double u);
	Vector3D& operator^=(const Vector3D& v);
	//取元素
	double& operator [] (int i)
	{
		switch (i) 
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
		}
	}

	__declspec(dllexport) friend Vector3D operator+(const Vector3D& lv, const Vector3D& rv);
	__declspec(dllexport) friend Vector3D operator-(const Vector3D& lv, const Vector3D& rv);
	__declspec(dllexport) friend Vector3D operator*(const double u, const Vector3D& rv);
	__declspec(dllexport) friend Vector3D operator*(const Vector3D& lv, const double u);
	__declspec(dllexport) friend Vector3D operator/(const Vector3D& lv, const double u);
	__declspec(dllexport) friend double   operator*(const Vector3D& lv, const Vector3D& rv);
	__declspec(dllexport) friend Vector3D operator^(const Vector3D& lv, const Vector3D& rv);

	//取长度
	double GetLen() const
	{
		return sqrt(x*x+y*y+z*z);
	}
	//单位化
	void SetUnit () {
		double len=GetLen();
		if (len<TOLER) {
			return;
		}
		x/=len;y/=len;z/=len;
	}
	double disFrom(const Vector3D& vec) const;
	//单目减
	Vector3D operator - () const
	{ return Vector3D (-x, -y, -z); }
	// 绕固定轴旋转
	void GetRotMatrix(double* out, double radians) const;
	void GetRotMatrix(double** ppdRotMatrix, double radians ) const;
	void Rotate(Vector3D& target, double radians) const;
	// Transformation according to the matrix
	void Transformation( double** ppdTransformation );	// ppdTransformation: 4*4
	void Transformation( double** ppdRotation, double* pdTrans );	// ppdRotation: 3*3; pdTrans: 3*1
	void Rotate( double** ppdRotation );	// ppdRotation: 3*3
	void Translation( double* pdTrans );	// pdTrans: 3*1

};

enum GENERATOR_TYPE{
	GENERATOR_POINT,
	GENERATOR_EDGE,
	GENERATOR_FACE
};

enum POLYGON_RASTERIZATION_MODE{
	MODELING_GL_POINT,
	MODELING_GL_LINE,
	MODELING_GL_FILL
};

class QuadCell3D 
{
public:
	QuadCell3D() { a=b=c=d=e=f=g=h=i=j=0; ID=-1; pGenVertex=NULL; pGenEdge=NULL; pGenFace=NULL;}
	~QuadCell3D() {}
public:
	// axx + byy + czz + dxy + eyz + fzx + gx + hy + iz + j;
	int ID;
	GENERATOR_TYPE gen_Type;
	CVertex* pGenVertex;
	CEdge* pGenEdge;
	CFace* pGenFace;
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double h;
	double i;
	double j;
};



#endif // !defined(AFX_VECTOR2D_H__A44A61DD_F250_4836_8BE4_62A5220C460F__INCLUDED_)
