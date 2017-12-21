#ifndef __REGISTRATION_H
#define __REGISTRATION_H

#include <afxtempl.h>

class PointCloud;
class CMesh;

class __declspec(dllexport) CRegistration
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CRegistration();
	virtual ~CRegistration();
 
	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Multi-pointcloud registration
	// Automatic mode
	// The number of the steps (iterations) is determined by the program automatically.
	// Each matrix is a 4x4 dimension array.
	int		MultiPointCloud_VelocityField( CArray<PointCloud*,PointCloud*>* pAllPC, 
				CArray<int*,int*>* pPair, 
				int iMaxIteration, double dInitialErr, 
				double dStopRatio, double dSampling, 
				CArray<double**,double**>* pTransMatrix, double& dStatDist );

	// Multi-pointcloud registration
	// Manual mode
	// The number of the steps (iterations) is one.
	// Each matrix is a 4x4 dimension array.
	int		MultiPointCloud_VelocityField_Step( CArray<PointCloud*,PointCloud*>* pAllPC, 
				CArray<int*,int*>* pPair, 
				double dInitialErr, double dSampling, 
				CArray<double**,double**>* pTransMatrix, double& dStatDist );

	// Two point clouds registration
	// Automatic mode
	// The number of the steps (iterations) is determined by the program automatically.
	// ppdTransMatrix is a 4x4 dimension array.
	int		PointCloud2PointCloud_VelocityField( PointCloud* pFixedPC, PointCloud* pFloatingPC,
				int iMaxIteration, double dInitialErr, 
				double dStopRatio, double dSampling, 
				double** ppdTransMatrix, double& dStatDist );

	// Two point clouds registration
	// Manual mode
	// The number of the step (iteration) is one.
	// ppdTransMatrix is a 4x4 dimension array.
	int		PointCloud2PointCloud_VelocityField_Step( PointCloud* pFixedPC, PointCloud* pFloatingPC,
				double dInitialErr, double dSampling, 
				double** ppdTransMatrix, double& dStatDist );

	// Point cloud - mesh registration
	// Automatic mode
	// The number of the steps (iterations) is determined by the program automatically.
	// dbTransMatrix is a 4x4 dimension array.
	int		PointCloud2Mesh_VelocityField(PointCloud* pPointCloud, CMesh* pMesh, int iMaxIteration, double dbStopRatio,
				double** dbTransMatrix, double& dbStatDist);

	// Point cloud - mesh registration
	// Manual mode
	// The number of the step (iteration) is one.
	// dbTransMatrix is a 4x4 dimension array.
	int		PointCloud2Mesh_VelocityField_Step(PointCloud* pPointCloud, CMesh* pMesh, double** dbTransMatrix, double& dbStatDist);

	// Get the iteration times
	int		GetIterationTimes();

	// Three-vertex-pair alignment
	// pFixedVertex and pFloatingVertex are both 3x2 array
	// ppdTransMatrix is a 4x4 dimension array
	int		ThreeVertexPairAlignment(double** pFixedVertex, double** pFloatingVertex, 
				double** ppdTransMatrix );

private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Decide convergence by the mean distance
	bool	IsConvergentByMeanDist( double dPreMeanDist, double dCurMeanDist, 
				double dStopRatio );

	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////

	// Iteration times
	int		m_nIterationTimes;

};

#endif