///////////////////////class CMesh/////////////////////////////////////
//		���Դ����������ˣ��������εĶ���������2-manifode mesh)	     //
//		���Զ�ȡһ���obj,smf,wrl���ļ�����							 //
//		���ݵ�������������������ӹ�ϵ							     //
//																    //
//////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "model.h"
#include <afx.h>
#include <list>
#include "../ANN/ANN.h"
//#include "PointCloud.h"
#include "ModelObject.h"

using namespace std;
typedef list<Vector3D>	_VECTORLIST;
typedef list<UINT>		_UINTLIST;

#define MAX_VERTEX_PER_FACE 20
// ������

class CBoundingBox;

class __declspec(dllexport) CVertex
{
public:
	Vector3D m_vPosition;		// ��������
	UINT* m_piEdge;             // �Ӹõ������halfedge,���ݵ�Ķ�����̬����
	_UINTLIST m_lEdgeList;      // ��������m_piEdge����ʱ����
	short m_nValence;           // ��Ķ���
	bool m_bIsBoundary;         // �Ƿ�Ϊ�߽��
	Vector3D m_vNormal;         // ������
	Vector3D m_vMeanCurvature;  // Mean curvature
	bool m_bColapsed;			// �Ƿ��Ѿ���ɾ��
	short m_nCut;               // �õ��뼸��cut path����
	// Octree
	QuadCell3D* m_pQuadCell;	// �õ�Ķ����ͽṹ
	// NurbsVolume
	double u;					// ��NurbsVolume�еĲ���				
	double v;
	double w;
	bool m_bIsPicked;			// ԭ���Ƿ�ѡ��
	COLORREF m_color;
public:
	//constructions
	CVertex() { m_piEdge=NULL; m_nValence=0; m_bIsBoundary=false;m_bColapsed=false;m_nCut=0;m_pQuadCell=NULL;u=v=w=0;m_bIsPicked=false;}
	CVertex(double x,double y,double z) {m_vPosition=Vector3D(x,y,z);m_piEdge=NULL; m_nValence=0;m_bIsBoundary=false;m_bColapsed=false;m_nCut=0;m_pQuadCell=NULL;u=v=w=0;m_bIsPicked=false;}
	CVertex(Vector3D vec) {m_vPosition=vec;m_piEdge=NULL; m_nValence=0;m_bIsBoundary=false;m_bColapsed=false;m_nCut=0;m_pQuadCell=NULL;u=v=w=0;m_bIsPicked=false;}
	virtual ~CVertex();
	//operations
	CVertex& operator = (CVertex& vertex);

};



class __declspec(dllexport) CWedge
{
public:
	Vector2D	m_vParameter;
	UINT	m_iVertex;
public:
	CWedge()	{m_iVertex=-1;}
	CWedge(UINT iVertex)	{m_iVertex=iVertex;}
	virtual ~CWedge();
};

class __declspec(dllexport) CEdge
{
public:
	UINT	m_iVertex[2];//�ߵ����˵㣬Vertex0��>Vertex1
	UINT	m_iWedge[2];//�ñ�ָ�������Wedge,��ʵ������Ϳ��Բ���m_iVertex[2]��

	UINT	m_iTwinEdge;//��ñ߷����෴����һ���ߣ����Ϊ-1��ñ�Ϊ�߽�
	UINT	m_iNextEdge;//����ʱ�뷽�����һ����
//	UINT	m_iPreEdge;
	UINT	m_iFace;//�ñ��������棬Ӧ�����������

	//for progressive mesh
	double	m_dPriority;//�ñ߻���ʱ������ֵ
//	double	m_dQEM;//�ñ߻���ʱ������ֵ
	bool	m_bColapsed;//�Ƿ��Ѿ���ɾ��
	bool	m_bLegal;//ɾ���ñ��Ƿ�Ϸ���3��׼��
	bool	m_bCut;//�Ƿ�Ϊ���ñ�
	UINT	m_iIndex;//��EdgeArray�е�Indexֵ
	// Octree
	QuadCell3D* m_pQuadCell;	// �ñߵĶ����ͽṹ
public:
	//constructions
	CEdge() {m_iVertex[0]=m_iVertex[1]=m_iWedge[0]=m_iWedge[1]=m_iTwinEdge=m_iNextEdge=m_iFace=-1;m_bColapsed=false;m_bLegal=true;m_bCut=false;m_pQuadCell=NULL;}
	CEdge(UINT iV0, UINT iV1) { m_iVertex[0]=iV0; m_iVertex[1]=iV1; m_iWedge[0]=m_iWedge[1]=-1;m_iTwinEdge=m_iNextEdge=m_iFace=-1;m_bColapsed=false;m_bLegal=true;m_bCut=false;m_pQuadCell=NULL;}
	virtual ~CEdge();

	//operations
	CEdge& operator = (const CEdge& e);
	
	
};

class __declspec(dllexport) CFace
{
public:
	short	m_nType;//������
	UINT*	m_piVertex;//���е�
	UINT*	m_piEdge;//���б�
	Vector3D m_vNormal;//����
	Vector3D m_vMassPoint;//���ĵ�
	double	m_dArea;//���

	//for progressive mesh
	bool	m_bColapsed;
	// WORD	m_color;//�����ɫ// Commented by Wei Wei [Aug.6, 2006]
	COLORREF m_color;// Added by Wei Wei [Aug.6, 2006]
	// double	m_dSumArea;//ѹ�������а�����������������
	// double	m_dStretch;//Geometry Stretch
	// Octree
	QuadCell3D* m_pQuadCell;	// ����Ķ����ͽṹ
public:
	//constructions
	CFace();
	CFace(short s);
	virtual ~CFace();

	//operations
	void Create(short s);
	CFace& operator = (const CFace& f);


};

class __declspec(dllexport) CEdgeArray//for progressmesh
{
public:
	CEdge*	m_pEdge;
	UINT*	m_aEdge;//array
	UINT	m_nValidEdge;//left edge number
	UINT	m_nTotalEdge;
public:
	CEdgeArray() {m_pEdge=NULL;m_aEdge=NULL;m_nValidEdge=0;}
	virtual ~CEdgeArray();
	void Create(CEdge* pEdge,UINT nValidEdge);
	UINT GetNextEdge();
	void RemoveEdge(UINT iEdge);
	void RestoreEdge(UINT iEdge);

	CEdgeArray& operator = (CEdgeArray& ea);

};


class __declspec(dllexport) CMesh :public CModel
{
public:
	UINT		m_nVertex;				//����
	CVertex*	m_pVertex;				//���
	UINT		m_nEdge;				//����
	CEdge*		m_pEdge; 				//�߱�
	UINT		m_nFace;	 			//����
	CFace*		m_pFace;				//���
	UINT		m_nWedge;				//Wedge��
	CWedge*		m_pWedge;				//Wedge��

	//temp
//	_UINTLIST m_lPickedFace;
	_UINTLIST m_lFocusEdge;
	_UINTLIST m_lFocusVertex;
	_UINTLIST m_lFocusFace;
//	_UINTLIST m_lPickedVertex;
	UINT	m_iPickedFace;
	UINT	m_iPickedEdge;
	UINT	m_iPickedVertex;
	_UINTLIST m_pickedIndexList;
//	_VECTORLIST m_pickedVertexList;
	bool	m_bClosed;
	Vector3D vMax;						//����vertex���������ֵ
	Vector3D vMin;						//����vertex����С����ֵ
	double dis_max;						// ��Χ�е�������
	CBoundingBox* m_pbBoundingBox;	// Bounding box
	// Octree
	UINT	m_nQuadCell;				// QuadCell��ID����ֵ
	bool	m_bQuadCell;				// Flag that indicating if the quadcells have been built
	bool m_bANNtree;
	ANNpointArray	data_pts;
	ANNpoint     query_pt_3D;
	ANNidxArray  nn_idx;
	ANNdistArray dists;
	ANNkd_tree			*ANN3DTree;
	Vector3D baryCenter;
	MATERIAL_TYPE m_mMaterial;
	bool	m_bUsingMaterial;
	float    m_fMeshTransparency;
	bool	m_bUsingTransparency;
public:
	//constructions
	CMesh();
	CMesh(CMesh* pMesh);
	virtual ~CMesh();

	//operations
public:
	CString GetName();
	void SetName(CString sName);
	void    GetDistanceOfPointToMesh( Vector3D v, Vector3D& footpoint, double& distance );
	BOOL	Load(CString sFileName,double* dbInitTxyz=NULL,double* dbInitRxyz=NULL);	// load from file
	BOOL	Save(CString sFileName);	// save to file
	double	GetEdgeLen(UINT iEdge);
	// double  GetEdgeWeight(UINT iEdge);
	BOOL	construct();// construct connectivity
	BOOL	construct( Vector3D* pvVertex, int nNumOfVertices, int** ppnFace, int nNumOfFaces );
	// Normal
	void	CalFaceAndVertexNormal(  );
	void	FlipNormals();	// Flip the normal direction of the mesh model
	// Octree
	void BuildQuadCell();
	void PrintQuadCell( FILE* f );
	Vector3D GetFootpoint( Vector3D v );
	double GetApproximateSignedDistanceToPoint( Vector3D v );	// Compute the approximate signed distance from a point to the mesh
	double GetExactSignedDistanceToPoint( Vector3D v );	// Compute the exact signed distance from a point to the mesh
	Vector3D GetNearestVertex( Vector3D vec, double& u, double& v, double& w );
	QuadCell3D* GetFootpointAndQuadCell( Vector3D v, Vector3D& footpoint, double& dis  );
	QuadCell3D* GetFootPointInfoAndQuadCell( Vector3D v, Vector3D& footpoint, double& dis, Vector3D& n, Vector3D& n1, Vector3D& n2 );
	void BuildANNkdTree(  );
	void DeleteANNkdTree(  );
	UINT GetNearestVertexIndexFromANNkdTree( Vector3D v );
	// Registration
	// void ComputeVertexArr( PointCloud* pPointCloud );
	// Save Parameter
	BOOL	loadFromSMFWithParam( CString sFileName );
	BOOL    saveToSMFWithParam( CString sFileName ); 
	// Save FeaturePoints
	void	saveFeaturePoints( CString sFileName );
	void	loadFeaturePoints( CString sFileName );
	// Pick Vertex
	void	PushPickedVertex( Vector3D p, UINT id );
	void	PopPickedVertex(  );
	void	ClearPickedVertex(  );
	void	PrintPickedVertex( FILE* f );
	// Povray
	void	SaveFaces(CString filename);
	void	SaveSmoothFaces( CString filename );
	void	SaveEdges( CString filename );
	void	SaveVertices( CString filename );
	void	SavePovrayFileFeaturePoints( CString filename );
	// For display
	void	SetFaceColorByDepth( Vector3D vOrientation );
	void	SetVertexColorByDepth( Vector3D vOrientation );
	MATERIAL_TYPE GetMaterial();
	void    SetMaterial( MATERIAL_TYPE material);
	bool	IsUsingMaterial();
	void	SetUsingMaterial( bool bUsingMaterial );
	float   GetTransparency();
	void    SetTransperancy(float transperancy);
	bool	IsUsingTransparency();
	void	SetUsingTransparency( bool bUsingTransparency );
private:
	CString m_sName;
	void	clear();
	BOOL	reConstruct();// construct connectivity from current mesh
	BOOL	loadFromSMF(CString sFileName,double* dbInitTxyz=NULL,double* dbInitRxyz=NULL);
	BOOL	loadFromWRL(CString sFileName);
	BOOL	loadFromTMP(CString sFileName);
	BOOL	loadFromStlBin(CString sFileName,double* dbInitTxyz=NULL,double* dbInitRxyz=NULL);
	BOOL	loadFromVTKBin(CString sFileName, double *dbInitTxyz=NULL, double *dbInitRxyz=NULL);
	BOOL	loadFromGTS(CString sFileName);
	BOOL	saveToSMF(CString sFileName);
	BOOL	saveToStlBin(CString sFileName);
	void	calFaceNormal(UINT i);
	void	calVertexNormal(UINT i);
	void	calVertexMeanCurvature(UINT i);
	// Octree
	QuadCell3D* GetFootpointAndQuadCellFromFace( Vector3D v, Vector3D& footpoint, double& dis, UINT faceIdx );
	QuadCell3D* GetFootpointAndQuadCellFromEdge( Vector3D v, Vector3D& footpoint, double& dis, UINT edgeIdx );
	QuadCell3D* GetFootpointAndQuadCellFromVertex( Vector3D v, Vector3D& footpoint, double& dis, UINT vertexIdx );
	bool	InFace( Vector3D v, UINT faceIdx );
	bool	InEdge( Vector3D v, UINT edgeIdx );
};

#endif