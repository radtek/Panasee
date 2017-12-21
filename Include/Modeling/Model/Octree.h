#pragma once

#include "cube.h"
#include "mesh.h"

class Octree
{
public:
	Cube * root;
	int endlevel;
	double length;
	Vector3D lllpoint;
	int numOfLeaf;			// 叶子节点个数
	int numOfVertexQuadCell;	// 顶点二次型个数
	int numOfEdgeQuadCell;		// 边二次型个数
	int numOfFaceQuadCell;		// 面二次型个数
public:
	Octree(void);
	virtual ~Octree(void);
	void CreateOctreeForMesh( CMesh* pMesh, int el );
	// Poly QuadOctree
	void ComputeFootpointsForMesh( CMesh* pMesh );
	Cube* LocatePoint( double x, double y, double z );
	Cube* LocateCube( unsigned int xCode, unsigned int yCode, unsigned int zCode );
	void GetCubeNeighbors( Cube* pCube );
	void GetAllCubeNeighbors(  );
	void PrintAllCubeNeigbors( FILE* f );
	void Sweeping( CMesh* pMesh );
	void GetNumOfLeaf(  );
private:
	void RecCreateOctreeForMesh( Cube* rt, CMesh* pMesh, int el );
	void RecComputeFootpointsForMesh( Cube* rt, CMesh* pMesh);//, FILE* f );
	void RecGetNumOfLeaf( Cube* rt );
	void OctreeTraverse( Cube*& pCube, int nextlevel, unsigned int xLocCode, unsigned int yLocCode, unsigned int zLocCode );
	// Get Neighbors for a Cube
	void RecGetCubeNeighbors( Cube* rt);
	void RecPrintCubeNeighbors( Cube* rt, FILE* f );
	Cube* GetCubeRightNeighbors( Cube* pCube );
	Cube* GetCubeLeftNeighbors( Cube* pCube );
	Cube* GetCubeUpperNeighbors( Cube* pCube );
	Cube* GetCubeBottomNeighbors( Cube* pCube );
	Cube* GetCubeForwordNeighbors( Cube* pCube );
	Cube* GetCubeBackwordNeighbors( Cube* pCube );
	Cube* GetCubeFRBNeighbors( Cube* pCube );
	Cube* GetCubeFLBNeighbors( Cube* pCube );
	Cube* GetCubeFRUNeighbors( Cube* pCube );
	Cube* GetCubeFLUNeighbors( Cube* pCube );
	Cube* GetCubeBRBNeighbors( Cube* pCube );
	Cube* GetCubeBLBNeighbors( Cube* pCube );
	Cube* GetCubeBRUNeighbors( Cube* pCube );
	Cube* GetCubeBLUNeighbors( Cube* pCube );
	bool LocateInOctree( unsigned int xLocCode, unsigned int yLocCode, unsigned int zLocCode );
	// Sweeping Update Function
	void Update( Cube* pCube, int flag );		// flag标志着sweeping的方向 1->2->3->4...->8
};
