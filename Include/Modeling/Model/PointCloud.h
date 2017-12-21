#pragma once

#include "../Common/Geometry.h"
#include "../Common/Vertex3D.h"
#include "../ANN/ANN.h"
#include <assert.h>
#include <list>

#include <afxTempl.h>

class CBoundingBox;

class __declspec(dllexport) CLine
{
public:
	CLine();
	~CLine();

	int m_numPnts;
	Vertex3D* m_vertexArr;
};

class __declspec(dllexport) CPatch
{
public:
	CPatch();
	~CPatch();

	int m_numLines;
	CLine* m_lineArr;
};

class __declspec(dllexport) PointCloud
{
public:
	PointCloud();
	PointCloud(int n);
	~PointCloud(void);

public:
	int num;
	Vertex3D* vertexArr;	// 需要在初始化点云的时候初始化
	Vector3D vMax;
	Vector3D vMin;
	CBoundingBox* m_pbBoundingBox;	// Bounding box
	int pickedNum;			// 被选中的点序号
	std::list<int> pickedIndexList;
	double rigidRotateMatrix[3][3];
	double rigidTranslate[3];
	float	m_color[3];		// Showing color in Scene
	
	int m_numPatch;
	CPatch* m_patchArr;
	
	double dis_max;
	Vector3D baryCenter;

	// ANN Tree for searching the nearest point set to a query point
	bool			m_bANNkdTree;			// Flag that indicating whether the ANN tree is built
	ANNpointArray	m_ann_data_pts;			// The set of points used to build ANN tree
	ANNpoint		m_ann_query_pt_3D;		// Query point
	ANNidxArray		m_ann_nn_idx;			// The set of the nearest points to the query point
	ANNdistArray	m_ann_dists;			// The set of the distances from the nearest points to the query point
	ANNkd_tree*		m_ann_ANN3DTree;		// ANN 3D Tree

public:
	void GetTransMatrix(double** dbTransMatrix);
	int GetNumOfPoints() {return num;}
	int GetNumofPatchs() {return m_numPatch;}
	Vertex3D GetVertex( int i ) { assert(i>=0&&i<num); return vertexArr[i]; }
	int SetPoint(int i, Vertex3D* p);
	int LoadFile(LPCSTR filename);
	int SaveFile(LPCSTR filename);
	void SavePovrayFile( LPCSTR filename );
	void SavePovrayFileFeaturePoints( LPCSTR filename );
	void SaveFeaturePoints( LPCSTR filename );
	void LoadFeaturePoints( LPCSTR filename );
	PointCloud* clone(  );
	void CreateZeroPointCloud( int n );
	bool construct();
	// operators
	void VertexInitTransAndRotate( double transOfX, double transOfY, double transOfZ, double rotateOfX, double rotateOfY, double rotateOfZ );
	void RotateAboutFixedAxies( Vector3D& vec, Vector3D axis, double theta );
	void VertexRigidBodyMotionOnVelocityField( Vector3D origin, Vector3D axis, double theta, double pitch );
	void VertexEulerMotion( double** rotate, double* translate );	
	void VertexEulerMotion(double** matrix);
	void VertexEulerTranslation( Vector3D vec );
	void VertexEulerRotation( Vector3D axis, double delta );
	void VertexScale( double ratio, bool isZoomIn );	// ratio:放大或缩小的倍数(always more than 1) | isZoomIn true:放大 false:缩小
	// pick
	void PushPickedIndex( int index );
	// Print
	void PrintRigid( FILE* file );

	// Sampling //
	// Grid sample
	int GridSample( double dSpacing, PointCloud*& pPointCloud );

	/***********  Point Cloud edit **********/
	//Decimate Point cloud (iDecimateType==0:by line;   ==1,by point)
	PointCloud* Decimate(int iDecimateType, int iTimes = 1);
	// Remove noise
	PointCloud* RemoveNoise(double dbThreshold = 0.003, int iRemoveEdge = 0);
	// Merge point cloud into this point cloud
	void MergePointClouds(PointCloud* pPointCloud);
	PointCloud* CutSelected(BOOL bCrop = FALSE);

	// Smoothing by average
	int SmoothByAverage( int nNeighbors, PointCloud*& pSmoothedPointCloud );
	
	void SetPointCloud(CList<float,float>* lstSelectedPoint);
	void SetPointCloudD(CList<double,double>* lstSelectedPoint);
	void SetPointCloud(CList<float,float>* lstSelectedPoint,CList<float,float>* lstSelectedNormal);
	void SetPointCloudD(CList<double,double>* lstSelectedPoint,CList<double,double>* lstSelectedNormal);
	void SetPointCloud(CList<float,float>* plstPnts, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void SetPointCloudD(CList<double,double>* plstPnts, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void SetPointCloud(CList<float,float>* plstPnts, CList<float,float>* plstNormals, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void SetPointCloudD(CList<double,double>* plstPnts, CList<double,double>* plstNormals, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void GetPoints(CList<float,float>* plstPnts, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void GetPoints(CList<float,float>* plstPnts, CList<float,float>* plstNormal, CList<int,int>* plstPntsPerLine, CList<int,int>* plstLinesPerPatch);
	void GetPoints(float** ppPoints);
	void GetPoints(double** ppPoints);
	void SetColor(float r, float g, float b);

	float GetPointSize();
	void SetPointSize(float fSize);

	// Normal
	void SetNormalsInitialized( bool bIsNormalsInitialized );
	bool IsNormalsInitialized();
	void SetNormalsComputed( bool bIsNormalsComputed );	// Set the flag that indicates whether the normals have been computed
	bool isNormalsComputed();	// Whether the normals have been computed
	void ComputeNormals( int nNumOfNeighbours );	// Compute the normals of all vertices
	void SetNormalsOriented( bool bIsNormalsOriented );	// Set the flag that indicates whether the normals have been oriented consistently
	bool isNormalsOriented();	// Whether the normals have been oriented consistently
	void OrientNormals( int nNumOfNeighbours );		// Orient all the normals consistently
	void FlipNormals();	// Flip the normal direction of the point cloud
	
	// ANN Tree
	void BuildANNkdTree(  );	// Build ANN Tree
	void DeleteANNkdTree(  );	// Delete ANN Tree
	
	// Nearest neighbors
	void GetNearestVertices( Vector3D vecQueryPoint, int nNumOfVertices, int*& pnIndexOfVertex );

private:
	// Operations

	// Normal
	double Weight( double d, double h );						// Compute the weight for the neighboring points
	void CoVariance( double* eigenVector, double** matrix );	// Compute the eigen vector for the CoVariance matrix

	// Members

	// Normal
	bool bIsNormalsInitialized;
	bool bIsNormalsComputed;	// Flag indicating whether the normals have been computed
	bool bIsNormalsOriented;	// Flag indicating whether the normals have been oriented consistently

	// File IO
	int LoadFromTrvFile(LPCSTR fileName);
	int SaveToTrvFile(LPCSTR fileName);		
	int LoadFromPCFile( LPCSTR fileName );	// Load a point cloud from a PC file
	int SaveToPCFile( LPCSTR fileName );	// Save a point cloud to a PC file
	int LoadFromPCNFile( LPCSTR fileName );	// Save a point cloud to a PC file
	int SaveToPCNFile( LPCSTR fileName );	// Save a point cloud to a PCN file

	float m_fPixelSize;  // pixel size for display
};
