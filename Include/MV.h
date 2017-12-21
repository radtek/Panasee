
#ifndef _MV_H
#define _MV_H

#include "afx.h"
#include <stdarg.h>

#ifndef PI
#define PI	3.14159265358979
#endif 
#ifndef DELTA
#define	DELTA	0.0005
#endif 

#define VERSION		"V1.0.0.2 "

class RPY; 

class  Vector  
{
public:
	Vector(int size = 3);
	Vector(double x, double y, double z);
	Vector(const Vector& v);
	virtual ~Vector();
	
	Vector operator=(const Vector& v);    // assign
	Vector operator+=(const Vector& v1);  // add
	Vector operator-=(const Vector& v);   // subtract

	int	length() const { return size; };   
	double Magnitude();
	Vector Normalize();

	bool SetValue(int valNum, ...);
	bool SetSize(int s);
	CString toString();
	
	double *value;
private:
	int		size;
};

class lPoint : public Vector
{
public:
	double &x, &y, &z;
	lPoint(double x, double y, double z);
	lPoint();
	
	lPoint operator=(const Vector& v);    // assign
};

class  Matrix
{
public:
	Matrix(int row = 3, int col = 3);
	Matrix(const Matrix& m);
	virtual ~Matrix();
	
	int row() const { return iRow;}
	int col() const { return iCol;};
	
	Matrix operator=(const Matrix& m);  // assign
	Matrix Inverse();					// Inverse
	Matrix Transpose();					// Transpose
	
	Matrix RotateX(double angle);		// Rotate around X axis, only apply to 4X4 matrix, unit radian	
	Matrix RotateY(double angle);		// Rotate around Y axis, only apply to 4X4 matrix, unit radian
	Matrix RotateZ(double angle);		// Rotate around Z axis, only apply to 4X4 matrix, unit radian
	double Trace();
	bool Zero();

	Matrix GetSubMatrix(int nRow, int nCol); // Get upperleft submatrix of nRow x nCol;

	bool Identity();
	bool SetDimension(int row, int col);
	bool SetRow(int row, Vector v);
	bool SetCol(int col, Vector v);
	bool SetRow(int row, ...);
	bool SetCol(int col, ...);
	
	//get the eigenVector, only calculate left-upper 3x3 matrix
	int GetEigenVector(double* eigenVector); 
	//get the eigenvalue and min max eigenvector, only calculate left-upper 3x3 matrix
	int GetEigenValueAndVector(double* eigenValue,double* MinEigenvector,double* MaxEigenvetor);
	//to get the multi eigenmatrix eigenvalue and eigenvector
	int GetEigenValueAndVector(double* eigenValue,double** Eigenvector);
	
	
	CString toString();
	void SaveToFile(CString sFile);
	bool LoadFromFile(CString sFile);
	void Serialize(CArchive& ar);	

	double **value;
private:
	int iRow;
	int iCol;
}; 

class  Transform : public Matrix
{
public:
	Transform(Vector vx, Vector vy, Vector vz, Vector pos);
	Transform();
	
	bool SetValue(Vector vx, Vector vy, Vector vz, Vector pos);
	bool SetVx(Vector vx);
	bool SetVy(Vector vy);
	bool SetVz(Vector vz);
	bool SetPos(Vector pos);
};

// Vector operations
 Vector operator+(const Vector &v1, const Vector &v2);   // add
 Vector operator-(const Vector &v1, const Vector &v2);   // subtract
 Vector operator*(const Vector &v1, const Vector &v2);   // cross product
 double operator^(const Vector &v1, const Vector &v2);   // dot product


// Matrix operations
 Matrix operator+(const Matrix &m1, const Matrix &m2);  // add
 Matrix operator-(const Matrix &m1, const Matrix &m2);  // subtract
 Matrix operator*(const Matrix &m1, const Matrix &m2); // multiply

 Matrix operator*(const Matrix &m1, const double d2); // multiply
 Matrix operator/(const Matrix &m1, const double d2); // divide

 Vector operator*(const Matrix &m, const Vector &v);

 lPoint operator*(const Transform &m, const lPoint &v);


 
 class  QUAT  
 {
 public:
	 QUAT(double x = 0, double y = 0, double z = 0, 
		 double q1 = 1, double q2 = 0, double q3 = 0, double q4 = 0);
	 virtual ~QUAT();
	 
	 Matrix toMatrix();
	 RPY toRPY();
	 CString toString();
	 
	 double x, y, z, q1, q2, q3, q4;
 };
 
 class  RPY  
 {
 public:
	 RPY(double x = 0, double y = 0, double z = 0, 
		 double Rx = 0, double Ry = 0, double Rz = 0);
	 virtual ~RPY();
	 
	 CString toString();
	 QUAT toQUAT();
	 Matrix toMatrix();
	 
	 double x, y, z;
	 union {
		 double Rx;
		 double Yaw;
	 };
	 union {
		 double Ry;
		 double Pitch;
	 };
	 union {
		 double Rz;
		 double Roll;
	 };
	 // Units of Rx,Ry,Rz are degree
	 /****************************************************
	 (Rz)	Roll  Represent Rotation around Z 
	 (Ry)	Pitch Represent Rotation around Y 
	 (Rx)	Yaw   Represent Rotation around X 
	 *****************************************************/
	 
 };
 
 
 
 class  Convertor  
 {
 public:
	 Convertor();
	 virtual ~Convertor();
	 
	 RPY Matrix2RPY(Matrix m);
	 QUAT Matrix2QUAT(Matrix m);
	 RPY QUAT2RPY(QUAT q);
 };


#endif 
