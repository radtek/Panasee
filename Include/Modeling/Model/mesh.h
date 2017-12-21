///////////////////////class CMesh/////////////////////////////////////
//		可以处理任意拓扑，任意多边形的二流型网格（2-manifode mesh)	     //
//		可以读取一般的obj,smf,wrl等文件类型							 //
//		根据点表和面表生成完整的连接关系							     //
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
// 顶点类

class CBoundingBox;

class __declspec(dllexport) CVertex
{
public:
	Vector3D m_vPosition;		// 顶点坐标
	UINT* m_piEdge;             // 从该点出发的halfedge,根据点的度数动态创建
	_UINTLIST m_lEdgeList;      // 用来构造m_piEdge的临时链表
	short m_nValence;           // 点的度数
	bool m_bIsBoundary;         // 是否为边界点
	Vector3D m_vNormal;         // 法向量
	Vector3D m_vMeanCurvature;  // Mean curvature
	bool m_bColapsed;			// 是否已经被删掉
	short m_nCut;               // 该点与几条cut path相连
	// Octree
	QuadCell3D* m_pQuadCell;	// 该点的二次型结构
	// NurbsVolume
	double u;					// 在NurbsVolume中的参数				
	double v;
	double w;
	bool m_bIsPicked;			// 原来是否被选中
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
	UINT	m_iVertex[2];//边的两端点，Vertex0－>Vertex1
	UINT	m_iWedge[2];//该边指向的两个Wedge,其实有这个就可以不用m_iVertex[2]了

	UINT	m_iTwinEdge;//与该边方向相反的另一条边，如果为-1则该边为边界
	UINT	m_iNextEdge;//沿逆时针方向的下一条边
//	UINT	m_iPreEdge;
	UINT	m_iFace;//该边所属的面，应该在它的左边

	//for progressive mesh
	double	m_dPriority;//该边化简时的优先值
//	double	m_dQEM;//该边化简时的优先值
	bool	m_bColapsed;//是否已经被删掉
	bool	m_bLegal;//删除该边是否合法，3条准则
	bool	m_bCut;//是否为剪裁边
	UINT	m_iIndex;//在EdgeArray中的Index值
	// Octree
	QuadCell3D* m_pQuadCell;	// 该边的二次型结构
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
	short	m_nType;//几边形
	UINT*	m_piVertex;//所有点
	UINT*	m_piEdge;//所有边
	Vector3D m_vNormal;//法向
	Vector3D m_vMassPoint;//中心点
	double	m_dArea;//面积

	//for progressive mesh
	bool	m_bColapsed;
	// WORD	m_color;//面的颜色// Commented by Wei Wei [Aug.6, 2006]
	COLORREF m_color;// Added by Wei Wei [Aug.6, 2006]
	// double	m_dSumArea;//压缩过程中包含的所有面的面积和
	// double	m_dStretch;//Geometry Stretch
	// Octree
	QuadCell3D* m_pQuadCell;	// 该面的二次型结构
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
	UINT		m_nVertex;				//点数
	CVertex*	m_pVertex;				//点表
	UINT		m_nEdge;				//边数
	CEdge*		m_pEdge; 				//边表
	UINT		m_nFace;	 			//面数
	CFace*		m_pFace;				//面表
	UINT		m_nWedge;				//Wedge数
	CWedge*		m_pWedge;				//Wedge表

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
	Vector3D vMax;						//所有vertex的最大坐标值
	Vector3D vMin;						//所有vertex的最小坐标值
	double dis_max;						// 包围盒的最大距离
	CBoundingBox* m_pbBoundingBox;	// Bounding box
	// Octree
	UINT	m_nQuadCell;				// QuadCell的ID计数值
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