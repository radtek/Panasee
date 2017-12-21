// Matrix.h: interface for the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__CB836413_59C8_4220_9593_60F5E5CB34E4__INCLUDED_)
#define AFX_MATRIX_H__CB836413_59C8_4220_9593_60F5E5CB34E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "math.h"
#include "assert.h"
#include "stdio.h"
#include "VectorM.h"

class __declspec(dllexport) MatrixND  
{
public:
	MatrixND();
	MatrixND( int r);
	MatrixND( int r,  int c);
	MatrixND(const MatrixND& a);
	virtual ~MatrixND();
	
	MatrixND& operator=(const MatrixND& a);
	
	// Initialization functions
	void clear();
	void create( int r,  int c);
	void create( int r);
	void identity(  );
	void setval(double val);
	// Statistics 
	VectorM  getRow(const  int r);
	VectorM getMeanRow();
	VectorM getStdRow();
	VectorM getVarRow();
	VectorM getColumn(const  int c);
	void setRow(const  int r, VectorM& vec);
	void setColumn(const  int r, VectorM& vec);
	
	int getRows();
	int getColumns();

	MatrixND operator*(const MatrixND& matrix);
	MatrixND operator*(const double val);
	MatrixND operator+(const MatrixND& matrix);
	MatrixND& operator+=(const MatrixND& matrix);
	MatrixND operator-(const MatrixND& matrix);
	MatrixND& operator-=(const MatrixND& matrix);
	MatrixND& operator/=(const double val);
	MatrixND& operator*=(const double val);
	MatrixND GetTransposeMatrix(  );
	MatrixND GetInverseMatrix(  );
	
	double& operator()(const long r, const long c);
	double operator()(const long r, const long c) const;
	
	void Print( FILE* f );
	
protected:
	double *data;
	int size;
	int rows;
	int columns;
	
	
	VectorM row;
	VectorM column;
	VectorM meanRow;
	VectorM stdRow;
	VectorM varRow;
	//bool printHeader_;
	
 };

#endif // !defined(AFX_MATRIX_H__CB836413_59C8_4220_9593_60F5E5CB34E4__INCLUDED_)
