#pragma once

#include "../Common/Geometry.h"
#include "./mesh.h"
#include "./PointCloud.h"

class NurbsVolume
{
public:
	Vector3D* ctrlPointArr;
	double* weights;	// �������Ƶ�Ķ�
	int l;	// u������ƶ�����Ϊl
	int m;	// v������ƶ�����Ϊm
	int n;	// n������ƶ�����Ϊn
	int p;	// u����B�����Ľ���  (����=����-1)
	int q;	// v����B�����Ľ���
	int r;	// n����B�����Ľ���
	double* knotU;	// u����Ľڵ�����
	double* knotV;	// v����Ľڵ�����
	double* knotW;	// w����Ľڵ�����
	Vector3D vMin;		// u=v=w=0�ĵ�
	Vector3D vMax;		// u=v=w=1�ĵ�
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
	double GetCtrlPointWeight( int i, int j, int k, double u, double v, double w );	// �õ����Ƶ��Ȩֵ
	double GetCtrlPointWeightU1( int i, int j, int k, double u, double v, double w );	// �õ���u��ƫ��ʱ�Ŀ��Ƶ�Ȩֵ
	double GetCtrlPointWeightV1( int i, int j, int k, double u, double v, double w );	// �õ���v��ƫ��ʱ�Ŀ��Ƶ�Ȩֵ
	double GetCtrlPointWeightW1( int i, int j, int k, double u, double v, double w );	// �õ���w��ƫ��ʱ�Ŀ��Ƶ�Ȩֵ
	Vector3D PartialDerivativeU1( double u, double v, double w );			// u�����һ�׵���
	Vector3D PartialDerivativeV1( double u, double v, double w );			// v�����һ�׵���
	Vector3D PartialDerivativeW1( double u, double v, double w );			// w�����һ�׵���
	void PartialDerivative( double u, double v, double w, Vector3D& deriU, Vector3D& deriV, Vector3D& deriW ); // ���������һ�׵���һ�����
	int ReverseParametric( Vector3D p, double initU, double initV, double initW, double& u, double& v, double& w );
};