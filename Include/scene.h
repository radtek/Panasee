///////////////////////class CScene///////////////////////////////////
//						场景的组织，绘制，交互						//
//////////////////////////////////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_

#include "mesh.h"
#include "PointCloud.h"
#include "Cylinder.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Polyline.h"
#include "display.h"
#include "MyGeometry.h"
#include "Ellipsoid.h"
#include "Box.h"
#include "Line3D.h"
#include "Segment.h"
#include "Ruler.h"

//class Octree;
//class NurbsVolume;
//class Cube;

#define MAX_MODEL_NUM 0xffff

//场景的渲染方式
#define RENDER_INVISIBLE		0	//不可见
#define RENDER_WIREFRAME		1	//线框
#define RENDER_HIDENLINE		2	//隐藏线
#define RENDER_FLATSHADE		3	//flat shade
#define RENDER_FLATSHADE_LINE	4	//flat shade with line
#define RENDER_SMOOTHSHADE		5	//smooth shade
#define RENDER_FOCUS			6	//focus edge
#define RENDER_TEXTURE			7	//with texture
#define RENDER_FACE_TEXTURE		8	//Render face texture
#define RENDER_MESH_IN_FACE_COLOR		9	// Render mesh colorful
#define RENDER_MESH_IN_VERTEX_COLOR		10	// Render mesh colorful

//拾取状态
#define PICK_NONE	0
#define PICK_VERTEX	1
#define PICK_EDGE	2
#define PICK_FACE	3
#define PICK_NURBSVOLUME 4
#define PICK_DEFORMVERTEX 5
#define PICK_POINTCLOUD 6
#define	PICK_POINTCLOUD_RECTANGLE 7

class __declspec(dllexport) CScene
{
public:
	CScene();
	virtual ~CScene();
	BOOL	Create(HDC hDC);
	BOOL	Create(HWND hWnd);
	void	DrawScene();
	void	SetDrawColorBox(double dbMaxMiss);
	void    ShowPointsDiff(BOOL bShow);
	void	DrawCoordAxis(BOOL bShow);

	int		GetSceneID();

	int		GetMaterialColor(COLORREF *pColorArray);
	int     GetMaterailNum();

	void	SetEyePoint(double x, double y, double z);
	void	SetGazePoint(double x, double y, double z);
	void	SetUp(double x, double y, double z);
	void	SetFront(double x, double y, double z);
	void	SetDistance(double dbDistance);
	Vector3D GetEyePoint() { return m_vEyePoint;};
	void	GetLookAtPar( Vector3D& vEyePoint, Vector3D& vGazePoint, Vector3D& vUp, Vector3D& vFront, double& dbDistance );
	void	SetLookAtPar( Vector3D vEyePoint, Vector3D vGazePoint, Vector3D vUp, Vector3D vFront, double dbDistance );
	
	BOOL	AddMesh(CMesh* pNewMesh, CString sName);
	BOOL    AddPointCloud( PointCloud* pNewPointCloud, CString sName );
	BOOL	AddRectangle( CRectangle* pNewRectangle, CString sName );
	BOOL	AddData(VOID* pData, CString sName, MY_GEO_DATATYPE iType);
	BOOL	AddCylinder(CCylinder *pCylinder, CString sName);
	BOOL	AddSphere(CSphere* pNewSphere, CString sName );
	BOOL	AddPolyline(CPolyline* pPolyline, CString sName );
	BOOL	AddEllipsoid( CEllipsoid* pEllipsoid, CString sName );
	BOOL	AddBox( CBox* pBox, CString sName );
	BOOL	AddSegment( CSegment* pSeg, CString sName );

	// Add a CString object and its position, and return its index in the CString list
//	int		AddText( CString sText, Vector3D vPosition, BYTE byColor[4] );
	int		AddText(ST_TEXT* sText);
	int		Add2DText(ST_TEXT* sText);
	
	int     RemoveCylinder(CCylinder *pCylinder);
	int		RemoveSegment(CSegment* pSeg);

	int		Remove(VOID* pData);
//	int		Remove(CString sName, MY_GEO_DATATYPE iType);
	int		RemoveAll();
	int		RemoveAllCylinder();
	int		RemoveAllPointCloud();
	int		RemoveAllMesh();
	int		RemoveAllRectangle();
	int		RemoveAllSphere();
	int		RemoveAllPolyline();
	int		RemoveAllEllipsoids();
	int		RemoveAllBoxes();
	int		RemoveAllSegments();
	int		RemoveAllText();

//	int		RemoveText( CString sText, Vector3D vPosition, BYTE byColor[4] );
	int		RemoveTextAt( int nIndex );
	int		RemoveText(ST_TEXT* sText);
	int		Remove2DText(ST_TEXT* sText);

	void	SetPointCloudColor(float r, float g, float b);
	void	SetUsingPCColor(BOOL bUsing);	// Set whether using color information from point cloud or using scene defined color
	void	SetPointCloudPixelSize(int iSize);

	void	SetIsTextDrawn( BOOL bIsTextDrawn );	// Whether to draw text items

//	BOOL	AddNurbsVolume( NurbsVolume* pNurbsVolume );
	//视角变换
	void	Move(int x,int y);
	void	Rotate(double a,double b);
	void	Scale(int d);
	void	MouseMove(UINT nFlags, int cx, int cy);//鼠标移动时变换视角
	void	Reshape(int cx,int cy);//改变viewport的大小
	//改变场景的渲染方式,整个场景用一样的渲染方式
	void	SetRenderMode(BYTE mode);
	//改变材质
	void	SetMeterial(BYTE meterial);
	//拾取
	void	SetPickMode(BYTE mode);//设置拾取对象
	//void	Pick(int x, int y);//完成拾取操作
	Vector3D* Pick(int x, int y);
	int		SelectRectangle( PointCloud* pPointCloud, int nX1, int nY1, int nX2, int nY2 );	// Use Rectangle to select a region of a point cloud

	// Select sphere by clicking
	int SelectSphereByClicking(CList<CSphere*,CSphere*>* lstSphere, int nX, int nY);

	void    ClearPick();
	//	bool	PickDeform( int x, int y ); // 完成拾取NurbsVolume控制顶点的操作 返回值: true 选中 false 没选中
	// 得到象素坐标和体坐标的比率
	double GetScaleRatio( Vector3D p );
	// Octree
/*	void CreateOctreeForMesh( UINT level );
	bool IsOctreeCreate() {return m_bOctree;}
	void GenOctreeCubeNeighbor();
	void PrintOctreeCubeNeighbor(FILE* f);
	void GetCubeAndNeighbors( unsigned int x, unsigned int y, unsigned int z );
	void DrawCubeAndNeighbors(); */
	void CreateQuadCellForMesh();
	void PrintQuadCell(FILE* f);
/*	void OctreeProjection();
	// Render set
	void SetMeshRender() { if(m_bMesh) m_bMeshRender = !m_bMeshRender; }
	void SetPointCloudRender() { if(m_bPointCloud) m_bPointCloudRender = !m_bPointCloudRender; }
	void SetNurbsVolumeRender() { if(m_bNurbsVolume) m_bNurbsVolumeRender = !m_bNurbsVolumeRender; }
	void SetOctreeRender() { if(m_bOctree) m_bOctreeRender = !m_bOctreeRender; }
	void SetCubeRender()	{ if(m_bCube) m_bCubeRender = !m_bCubeRender; } */
	Vector3D PhysicalToLogical( Vector3D vec );
	double PhysicalToLogical(double dVal);

private:
	void	DrawMesh(CMesh *pMesh, bool bFlat);
	void	DrawPMesh(CMesh *pMesh, bool bFlat, bool bColor, bool bTexture);//Progressive Mesh
	void	DrawMeshInFaceColor( CMesh* pMesh, bool bFlat );
	void	DrawMeshInVertexColor( CMesh* pMesh, bool bFlat );
	void	DrawFace(CMesh *pMesh, bool bPick);
	void	DrawEdge(CMesh *pMesh, bool bPick);
	void	DrawVertex(CMesh *pMesh, bool bPick);
	void	DrawVertexRed( CMesh *pMesh );
	void	DrawPickPointCloud( PointCloud* pPointCloud , bool bPick );
//	void	DrawNurbsVolumeCtrlPoints( NurbsVolume* pNurbsVolume, bool bPick );		// bPick: true-只在背后缓冲区画ctrlPoints, false-画场景
	void	DrawFocus(CMesh *pMesh);
	void	DrawPointCloud( PointCloud* pPointCloud );
	void	DrawRectangle( CRectangle* pRectangle );
	void	DrawRectangleWithTexture( CRectangle* pRectangle );
	void	DrawSphere( CSphere* pSphere );
	void	DrawPolyline( CPolyline* pPolyline );
	void	DrawEllipsoid( CEllipsoid* pEllipsoid );
	void	DrawBox( CBox *pBox );
	void	DrawSegment( CSegment *pSeg );
//	void	DrawNurbsVolume( NurbsVolume* pNurbsVolume );
//	void	DrawPartNurbsVolume( NurbsVolume* pNurbsVolume );
//	void	DrawOctree( Octree* pOctree );
//	void	DrawCube( Cube* pCube );
//	void	RecDrawOctree( Cube* pCube );
	void	PickFace(CMesh *pMesh, int n,UINT *pLong);
	void	PickEdge(CMesh *pMesh, int n,UINT *pLong);
	int	PickVertex(CMesh *pMesh, int n,UINT *pLong);
	void ClearPickVertex(CMesh *pMesh);
//	bool	PickNurbsVolumeCtrlPoints(NurbsVolume* pNurbsVolume, int n,UINT *pLong);
	int	PickPointCloud( PointCloud* pPointCloud, int n, UINT* pLong );
	void ClearPickPointCloud(PointCloud* pPointCloud);
	void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);
	void DrawText(float x, float y, float z, char *str);
	void DrawAllText();
	void DrawAll2DText();

	double m_dbColorBoxMax;
	bool   m_bDrawColorBox;
	BOOL   m_bShowPointsDiff;
	
	
public:
	CDisplay m_Display;//显示接口,可以选择用什么方式显示,OPENGL or DIRECT3D
	CPtrArray m_MeshPointers;
	CPtrArray m_PointCloudPointers;
	CPtrArray m_RectanglePointers;
	CPtrArray m_SpherePointers;
	CPtrArray m_PolylinePointers;
	CPtrArray m_EllipsoidPointers;
	CPtrArray m_BoxPointers;
	CPtrArray m_SegmentPointers;
	CPtrArray m_CylinderPointers;
	CStringArray m_saMesh;
	CStringArray m_saPointCloud;

	CRuler m_ruler;
	//CMesh*	m_MeshPointers[MAX_MODEL_NUM];//所有模型的指针
	//PointCloud* m_PointCloudPointers[MAX_MODEL_NUM]; // 所有点云的指针
public:
//	BYTE	m_MeshNum;//场景中模型的个数
//	BYTE    m_PointCloudNum; // 场景中的点云个数
//	BYTE	m_RectangleNum; // Number of rectangles in the scene
//	BYTE	m_SphereNum;	// Number of spheres in the scene
//	BYTE	m_PolylineNum;	// Number of polylines in  the scene
//	BYTE	m_EllipsoidNum;	// Number of ellipsoids in the scene
//	BYTE	m_BoxNum;			// Number of boxes in the scene
//	BYTE	m_SegmentNum;	// Number of segments in the scene
	BYTE	m_RenderMode;//场景的渲染方式,整个场景用一样的渲染方式
	BYTE	m_RectangleRenderMode;	// Render mode of rectangle
	CMesh*	m_pCurrentMesh;//当前选中的模型
	PointCloud* m_pCurrentPointCloud;// 当前的点云
	CRectangle* m_pCurrentRectangle;
	CSphere* m_pCurrentSphere;
	CPolyline* m_pCurrentPolyline;
	CEllipsoid* m_pCurrentEllipsoid;
	CBox*	m_pCurrentBox;
	CSegment* m_pCurrentSegment;
//	NurbsVolume* m_pCurrentNurbsVolume;	// 当前的Nurbs体

	//视锥
	Vector3D m_vEyePoint;
	Vector3D m_vGazePoint;
	Vector3D m_vUp;
	Vector3D m_vFront;
	double	m_dDistance;
	//viewport
	int		m_nWidth;
	int		m_nHeight;
	//光源，目前规定只有一个光源
	Vector3D m_vLight;
	//拾取状态
	BYTE	m_PickMode;//拾取对象
	//Octree
/*	Octree* m_pOctree;
	Cube*	m_pCube;
	Vector3D nearestpoint; */
	bool	m_bMesh;			// mesh是否已经导入
	bool	m_bPointCloud;		// PointCloud是否已经导入
	bool	m_bRectangle;	// Whether a rectangle is imported
	bool	m_bSphere;	// Whether a sphere is imported
	bool	m_bPolyline; // Whether a polyline is imported
	bool	m_bEllipsoid;// Whether a ellipsoid is imported
	bool	m_bBox;// Whether a box is imported
	bool	m_bSegment;// Whether a segment is imported
/*	bool	m_bOctree;			// Octree是否已经生成
	bool	m_bCube;			// 特定的Cube指针是否已经得到
*/
	bool	m_bMeshRender;		// Mesh是否进行绘制
	bool	m_bPointCloudRender;	// PointCloud是否进行绘制
	bool	m_bRectangleRender;	// Whether to render the rectangle
	bool	m_bSphereRender;	// Whether to render the sphere
	bool	m_bPolylineRender;	// Whether to render the polyline
	bool	m_bEllipsoidRender; // Whether to render the ellipsoid
	bool	m_bBoxRender; // Whether to render the box
	bool	m_bSegmentRender;	// Whether to render the segment
	bool    m_bCylinderRender;  // Whether to render cylinder
/*	bool    m_bOctreeRender;	// Octree是否进行绘制
	bool	m_bCubeRender;		// 特定的Cube是否进行绘制
	bool	m_bNurbsVolume;		// NurbsVolume是否已经根据mesh构造
	bool	m_bNurbsVolumeRender;	// NurbsVolume是否需要渲染
	bool	m_bCtrlPointPicked;	// Nurbs体的控制点是否被选中
	int picked_i;	// 被选中的Nurbs控制点
	int picked_j;
	int picked_k; */
	int picked_index;	// face被选中的变形点
	Vector3D baryCenter;
	double dis_max;

  // Texture
public:
	int  Click(int mouse_x, int mouse_y, Line3D *pLine);
	void SetMaterial(MATERIAL_TYPE byMaterial,float fTransparency);
//	CDib m_dibFile;
//	void CreateTexture(CDib*);
//	void DrawFaceMesh(CMesh *pMesh);

private:
	void DrawRuler();
	float	m_PCcolor[3];
	BOOL	m_bUsingPCColor;
	int		m_iPCPixelSize;

	//CCylinder* m_ppCylinder[MAX_MODEL_NUM];
	//CString	   m_sCylinder[MAX_MODEL_NUM];
	//int		m_iNumCylinder;

	int		m_iSceneID;
	
	BOOL m_bIsTextDrawn;	// Flag to indicate whether to draw the text items
//	CList<CString,CString>* m_plsTextItems;
//	CList<Vector3D,Vector3D>* m_plvTextPositions;
	CList<ST_TEXT*,ST_TEXT*>* m_plText;
	CList<ST_TEXT*, ST_TEXT*>* m_pl2DText;

};

#endif