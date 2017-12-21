// Modal.cpp: implementation of the CModal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Modal.h"
#include "vtkMarchingCubes.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageThreshold.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkStripper.h"
#include "vtkImageMarchingCubes.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkDecimatePro.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkExtractVOI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModal::CModal()
{
	m_pVolume=NULL;
	m_volume=0;
	m_opacity=100;
	m_color=RGB(200,200,200);
	m_name="default";
	m_actor=NULL;
	m_zType=ORGAN;
	m_modalType=ModalDrag;
}

CModal::~CModal()
{
	clear();
}

void CModal::clear()
{

}

BOOL CModal::save(FILE *file)
{
	if(!saveHeader(file))
		return FALSE;

	return TRUE;
}

BOOL CModal::readHeader(FILE *file,MODALTYPE &modalType)
{
	int type=0;
	int iReadCount=fread(&type,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		return FALSE;
	}
	if(type==100)
	{
		modalType=ModalDrag;
	}else if(type==200)
	{
		modalType=ModalSeg;
	}else if(type==300)
	{
		modalType=ModalContour;
	}else if(type==400)
	{
		modalType=ModalVolume;
	}else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CModal::readContent(FILE *file)
{
	return TRUE;
}

BOOL CModal::saveHeader(FILE *file)
{
	int type;
	if(m_modalType==ModalDrag)
	{
		type=100;
	}else if(m_modalType==ModalSeg)
	{
		type=200;
	}else if(m_modalType==ModalContour)
	{
		type=300;
	}else if(m_modalType==ModalVolume)
	{
		type=400;
	}else
	{
		return FALSE;
	}
	fwrite(&type,sizeof(int),1,file);
	return TRUE;
}

vtkActor* CModal::GenerateActor(vtkPolyData *polyData, MESH_OPTIONS option)
{
	COLORREF color=option.color;
	float fOpacity=option.opacity;

	vtkPolyDataMapper *isoMapper=vtkPolyDataMapper::New();
	isoMapper->SetInput(polyData); 
	isoMapper->ScalarVisibilityOff();

	vtkActor *isoActor=vtkActor::New();
	isoActor->SetMapper(isoMapper);

	isoActor->GetProperty()->SetDiffuseColor(1, .49, .25);
	isoActor->GetProperty()->SetSpecular(.3);
	isoActor->GetProperty()->SetSpecularPower(20);
	isoActor->GetProperty()->SetOpacity(fOpacity);
	isoActor->GetProperty()->SetColor(GetRValue(color)/255.0,GetGValue(color)/255.0,GetBValue(color)/255.0);
	isoActor->GetProperty()->SetShading(1);

	isoMapper->Delete();

	return isoActor;
}

vtkActor* CModal::GenerateMesh(vtkImageData *pImage,MESH_OPTIONS option)
{
	float fContour=option.label-0.5;
	COLORREF color=option.color;
	float fOpacity=option.opacity;
	
	int select=1;
	if(select==0)
	{
		vtkExtractVOI* extract = vtkExtractVOI::New();
		extract->SetVOI(option.VOI);
		extract->SetSampleRate(1, 1, 1);
		extract->SetInput(pImage);
		extract->ReleaseDataFlagOff();

		vtkImageMarchingCubes *pMarchFilter=vtkImageMarchingCubes::New();
		pMarchFilter->SetInput(extract->GetOutput());
		pMarchFilter->ReleaseDataFlagOn();
		pMarchFilter->ComputeNormalsOff();
		pMarchFilter->ComputeScalarsOff();
		pMarchFilter->ComputeGradientsOff();
		//pMarchFilter->UseScalarTreeOn();
		pMarchFilter->SetNumberOfContours(1);
		pMarchFilter->SetValue(0,fContour);

		vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();
		skinMapper->ScalarVisibilityOff();
		if(option.bLargest)
		{
			vtkPolyDataConnectivityFilter *Largestfilter=vtkPolyDataConnectivityFilter::New();
			Largestfilter->SetInput(pMarchFilter->GetOutput());
			Largestfilter->SetExtractionModeToLargestRegion();
			
			skinMapper->SetInputConnection(Largestfilter->GetOutputPort());

			Largestfilter->Delete();
		}else
		{
			skinMapper->SetInputConnection(pMarchFilter->GetOutputPort());
		}
		
		
		vtkActor *pActor=vtkActor::New();
		pActor->SetMapper(skinMapper);
		pActor->GetProperty()->SetDiffuseColor(1, .49, .25);
		pActor->GetProperty()->SetSpecular(.3);
		pActor->GetProperty()->SetSpecularPower(20);
		pActor->GetProperty()->SetOpacity(fOpacity);
		pActor->GetProperty()->SetShading(1);
		pActor->GetProperty()->SetColor(GetRValue(color)/255.0,GetGValue(color)/255.0,GetBValue(color)/255.0);
		
		extract->Delete();
		pMarchFilter->Delete();
		skinMapper->Delete();
		
		return pActor;
	}

	// VTK Gaussian (because we care about the speed and not so much about
	// precision)
	vtkImageGaussianSmooth *m_VTKGaussianFilter;
	
	// The polygon smoothing filter
	vtkSmoothPolyDataFilter *m_PolygonSmoothingFilter;
	
	// Triangle stripper
	vtkStripper *m_StripperFilter;  
	
	// Marching cubes filter
	vtkImageMarchingCubes *     m_MarchingCubesFilter;
	
	
	// A filter that computes normals
	vtkPolyDataNormals *        m_NormalsFilter;
	
	// The triangle decimation driver
	vtkDecimatePro *            m_DecimateProFilter;

	// Initialize the Gaussian filter
	m_VTKGaussianFilter = vtkImageGaussianSmooth::New();
	m_VTKGaussianFilter->ReleaseDataFlagOn();
  
	// Create and configure a filter for polygon smoothing
	m_PolygonSmoothingFilter = vtkSmoothPolyDataFilter::New();
	m_PolygonSmoothingFilter->ReleaseDataFlagOn();

	// Create and configure a filter for triangle strip generation
	m_StripperFilter = vtkStripper::New();
	m_StripperFilter->ReleaseDataFlagOn();

	// Create and configure the marching cubes filter
	m_MarchingCubesFilter = vtkImageMarchingCubes::New();
	m_MarchingCubesFilter->ReleaseDataFlagOn();
	m_MarchingCubesFilter->ComputeScalarsOff();
	m_MarchingCubesFilter->ComputeGradientsOff();
	m_MarchingCubesFilter->SetNumberOfContours(1);
	m_MarchingCubesFilter->SetValue(0,fContour);

		
	// Create and configure the normal computer
	m_NormalsFilter = vtkPolyDataNormals::New();
	m_NormalsFilter->SplittingOff();
	m_NormalsFilter->ConsistencyOff();
	m_NormalsFilter->ReleaseDataFlagOn();

	// Create and configure a filter for triangle decimation
	m_DecimateProFilter = vtkDecimatePro::New();
	m_DecimateProFilter->ReleaseDataFlagOn();

	vtkPolyDataConnectivityFilter *Largestfilter=vtkPolyDataConnectivityFilter::New();

	vtkExtractVOI* extract = vtkExtractVOI::New();
	extract->SetVOI(option.VOI);
	extract->SetSampleRate(1, 1, 1);
	extract->SetInput(pImage);
	extract->ReleaseDataFlagOff();
	
	//配置三角化参数
	// Define the current pipeline end-point
	vtkImageData *pipeImageTail = extract->GetOutput();
	vtkPolyData *pipePolyTail = NULL;

	// Route the pipeline according to the settings
	// 1. Check if Gaussian smoothing will be used
	if(option.bUseGaussianSmoothing)
    {    
		// The Gaussian filter is enabled
		m_VTKGaussianFilter->SetInput(pipeImageTail);
		pipeImageTail = m_VTKGaussianFilter->GetOutput();

		// Apply parameters to the Gaussian filter
		float sigma = option.fGaussianStandardDeviation;

		// Sigma is in millimeters
		const double *spacing = pImage->GetSpacing();
		m_VTKGaussianFilter->SetStandardDeviation(
			sigma / spacing[0], sigma / spacing[1], sigma / spacing[2]);
		m_VTKGaussianFilter->SetRadiusFactors(
			3 * sigma / spacing[0], 3 * sigma / spacing[1], 3 * sigma / spacing[2]);
    }

	m_MarchingCubesFilter->SetInput(pipeImageTail);
		pipePolyTail = m_MarchingCubesFilter->GetOutput();
	

	if(option.bLargest)
	{
		
		Largestfilter->SetInput(pipePolyTail);
		Largestfilter->SetExtractionModeToLargestRegion();
		pipePolyTail = Largestfilter->GetOutput();
	}
	
	// 3. Check if decimation is required
	if(option.bUseDecimation)
    {
		
		
		// Decimate Pro filter gets the pipe tail
		m_DecimateProFilter->SetInput(pipePolyTail);
		pipePolyTail = m_DecimateProFilter->GetOutput();
		
		// Apply parameters to the decimation filter
		m_DecimateProFilter->SetTargetReduction(
			option.fDecimateTargetReduction);
		
		m_DecimateProFilter->SetPreserveTopology(
			option.bDecimatePreserveTopology);
		
		
	}
	
	// 4. Compute the normals (non-patented only)
	m_NormalsFilter->SetInput(pipePolyTail);
	pipePolyTail = m_NormalsFilter->GetOutput();
	
	

	// 5. Include/exclude mesh smoothing filter
	if(option.bUseMeshSmoothing)
    {
		// Pipe smoothed output into the pipeline
		m_PolygonSmoothingFilter->SetInput(pipePolyTail);
		pipePolyTail = m_PolygonSmoothingFilter->GetOutput();

		// Apply parameters to the mesh smoothing filter
		m_PolygonSmoothingFilter->SetNumberOfIterations(
			option.iMeshSmoothingIterations);

		m_PolygonSmoothingFilter->SetRelaxationFactor(
			option.fMeshSmoothingRelaxationFactor);

		m_PolygonSmoothingFilter->SetFeatureAngle(
			option.fMeshSmoothingFeatureAngle);

		m_PolygonSmoothingFilter->SetFeatureEdgeSmoothing(
			option.bMeshSmoothingFeatureEdgeSmoothing);

		m_PolygonSmoothingFilter->SetBoundarySmoothing(
			option.bMeshSmoothingBoundarySmoothing);

		m_PolygonSmoothingFilter->SetConvergence(
			option.fMeshSmoothingConvergence);
    }

	// 6. Pipe in the final output into the stripper
	//m_StripperFilter->SetInput(pipePolyTail);

	pipePolyTail->Update();
	vtkPolyData *polyData=vtkPolyData::New();
	polyData->DeepCopy(pipePolyTail);
		
	extract->Delete();	
	m_VTKGaussianFilter->Delete();	
	m_MarchingCubesFilter->Delete();	
	Largestfilter->Delete();
	m_DecimateProFilter->Delete();
	m_NormalsFilter->Delete();
	m_PolygonSmoothingFilter->Delete();
	m_StripperFilter->Delete();
	
	vtkActor* isoActor=GenerateActor(polyData,option);
	polyData->Delete();

	return isoActor;
}