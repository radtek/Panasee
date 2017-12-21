#pragma once

#include "../Common/Geometry.h"
#include "./mesh.h"
#include "./PointCloud.h"

class NurbsVolume
{
public:
	Vector3D* ctrlPointArr;
	double* weights;	// 各个控制点的度
	int l;	// u方向控制顶点数为l
	int m;	// v方向控制顶点数为m
	int n;	// n方向控制顶点数为n
	int p;	// u方向B样条的阶数  (次数=阶数-1)
	int q;	// v方向B样条的阶数
	int r;	// n方向B样条的阶数
	double* knotU;	// u方向的节点向量
	double* knotV;	// v方向的节点向量
	double* knotW;	// w方向的节点向量
	Vector3D vMin;		// u=v=w=0的点
	Vector3D vMax;		// u=v=w=1的点
public:
	NurbsVolume(  );
	NurbsVolume( int ll, int mm, int nn, int pp, int qq, int rr );
	~NurbsVolume(  );
	Vector3D GetCtrlPoints( int i, int j, int k );
	void BuildNurbsVolume( Vector3D min, Vector3D max );
	void BuildNurbsVolumeWithoutSetParamForMesh( CMesh* pMesh );
	void BuildNurbsVolumeAndSetParamForMesh( CMesh*& pMesh );
	Vector3D GetVolumePointFromParam( double u, double v, double w );
	void DeformationOnMesh( CMesh* pMesh );
	void ComputeDeformation( CMesh* pMesh, PointCloud* pPointcloud );
	void ComputeDeformationByFeaturePoints( CMesh* pMesh, PointCloud* pPointcloud );
	// Povray
	void SaveToPovray( CString str );
private:
	int IndexMap( int i, int j, int k );
	double BaseU( double u, int i, int order );			// The B-Spline Base Function on U
	double BaseV( double v, int j, int order );			// The B-Spline Base Function on V
	double BaseW( double w, int k, int order );			// The B-Spline Base Function on W
	double BaseU1( double u, int i, int order );		// The B-Spline Base Function Derivative 1 order on U
	double BaseV1( double v, int j, int order );		// The B-Spline Base Function Derivative 1 order on V	
	double BaseW1( double w, int k, int order );		// The B-Spline Base Function Derivative 1 order on W	
	double GetCtrlPointWeight( int i, int j, int k, double u, double v, double w );	// 得到控制点的权值
	double GetCtrlPointWeightU1( int i, int j, int k, double u, double v, double w );	// 得到对u求偏导时的控制点权值
	double GetCtrlPointWeightV1( int i, int j, int k, double u, double v, double w );	// 得到对v求偏导时的控制点权值
	double GetCtrlPointWeightW1( int i, int j, int k, double u, double v, double w );	// 得到对w求偏导时的控制点权值
	Vector3D PartialDerivativeU1( double u, double v, double w );			// u方向的一阶倒数
	Vector3D PartialDerivativeV1( double u, double v, double w );			// v方向的一阶倒数
	Vector3D PartialDerivativeW1( double u, double v, double w );			// w方向的一阶倒数
	void PartialDerivative( double u, double v, double w, Vector3D& deriU, Vector3D& deriV, Vector3D& deriW ); // 三个方向的一阶倒数一起计算
	int ReverseParametric( Vector3D p, double initU, double initV, double initW, double& u, double& v, double& w );
};