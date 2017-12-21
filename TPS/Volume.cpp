// Volume.cpp: implementation of the CVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Volume.h"
#include "vtkHeader.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkOpenGLVolumeTextureMapper3D.h"
#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkGPUVolumeRayCastMapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVolume::CVolume()
{
	m_volume=NULL;
	m_pImage=NULL;
	m_pRenderWindow=NULL;

	m_iGPUMemorySize=128*1024*1024;
}

CVolume::~CVolume()
{
	if(m_volume)
	{
		m_volume->Delete();
		m_volume=NULL;
	}
}

void CVolume::SetImage(vtkImageData *pImage)
{
	m_pImage=pImage;
	if(!m_pImage)
	{
		if(m_volume)
		{
			m_volume->Delete();
			m_volume=NULL;
		}
		return;
	}
	if(m_volume)
	{
		m_volume->Delete();
		m_volume=NULL;
	}
	
	m_config.bCrop=FALSE;
	m_config.sampleDistance=1;
	m_config.LOD=1.0;
	m_config.lowResLODFactor=1.5;
	m_config.lodRange[0]=1.0;
	m_config.lodRange[1]=4.0;

	m_config.renderMode=VOLRENDER_VOLUME;
	m_config.bGradient=FALSE;
	m_config.bShade=TRUE;

	/*设置默认颜色和透明度*/
	double dData[2];
	dData[0]=0;
	dData[1]=4096;
	
	IA_PIXEL_ITEM item;
	m_config.opacityListCount=0;
	float rate=4;

	item.pixel=dData[0];
	item.opacity=0;
	item.color=RGB(0.3*255,0.3*255,255);
	m_config.opacityColorList[m_config.opacityListCount]=item;
	m_config.opacityListCount++;

	item.pixel=153*rate;
	item.opacity=0;
	item.color=RGB(0.3*255,0.3*255,255);
	m_config.opacityColorList[m_config.opacityListCount]=item;
	m_config.opacityListCount++;

	item.pixel=279*rate;
	item.opacity=1.0;
	item.color=RGB(0.3*255,255,0.3*255);
	m_config.opacityColorList[m_config.opacityListCount]=item;
	m_config.opacityListCount++;

	item.pixel=366*rate;
	item.opacity=0.2;
	item.color=RGB(1.0*255,0.3*255,0.3*255);
	m_config.opacityColorList[m_config.opacityListCount]=item;
	m_config.opacityListCount++;

	item.pixel=dData[1];
	item.opacity=0.2;
	item.color=RGB(1.0*255,0.3*255,0.3*255);
	m_config.opacityColorList[m_config.opacityListCount]=item;
	m_config.opacityListCount++;

	m_config.gradListCount=0;
	item.pixel=0;
	item.opacity=0;
	m_config.gradList[m_config.gradListCount]=item;
	m_config.gradListCount++;

	item.pixel=500;
	item.opacity=1;
	m_config.gradList[m_config.gradListCount]=item;
	m_config.gradListCount++;

	Update();
		
}

vtkVolume* CVolume::GetVolume()
{
	if(m_pImage==NULL)
		return NULL;

	return m_volume;
}

void CVolume::SetVolumeInfo(VOLRENDER_CONFIG *pConfig)
{
	m_config.bCrop=pConfig->bCrop;
	m_config.cropMode=pConfig->cropMode;
	m_config.dCropPlane[0]=pConfig->dCropPlane[0];
	m_config.dCropPlane[1]=pConfig->dCropPlane[1];
	m_config.dCropPlane[2]=pConfig->dCropPlane[2];
	m_config.dCropPlane[3]=pConfig->dCropPlane[3];
	m_config.dCropPlane[4]=pConfig->dCropPlane[4];
	m_config.dCropPlane[5]=pConfig->dCropPlane[5];

	m_config.bShade=pConfig->bShade;
	m_config.bGradient=pConfig->bGradient;
	m_config.renderMode=pConfig->renderMode;
	
	m_config.LOD=pConfig->LOD;
	m_config.sampleDistance=pConfig->sampleDistance;
	m_config.lowResLODFactor=pConfig->lowResLODFactor;
	m_config.lodRange[0]=pConfig->lodRange[0];
	m_config.lodRange[1]=pConfig->lodRange[1];

	m_config.gradListCount=0;
	m_config.opacityListCount=0;

	
	for(int i=0;i<pConfig->opacityListCount;i++)
	{
		m_config.opacityColorList[i]=pConfig->opacityColorList[i];
	}
	m_config.opacityListCount=pConfig->opacityListCount;

	for(i=0;i<pConfig->gradListCount;i++)
	{
		m_config.gradList[i]=pConfig->gradList[i];
	}
	m_config.gradListCount=pConfig->gradListCount;

	Update();
}

void CVolume::Update()
{
	if(!m_pImage)
	{
		return;
	}

	if(m_volume)
	{
		m_volume->Delete();
		m_volume=NULL;
	}
	
	m_volume=vtkVolume::New();

	//Update Color and Opacity list
	vtkColorTransferFunction* colorTransferFunction=vtkColorTransferFunction::New();
	vtkPiecewiseFunction*     opacityTransferFunction=vtkPiecewiseFunction::New();;
	IA_PIXEL_ITEM item;
	colorTransferFunction->RemoveAllPoints();
	opacityTransferFunction->RemoveAllPoints();

	for(int i=0;i<m_config.opacityListCount;i++)
	{
		item=m_config.opacityColorList[i];
		colorTransferFunction->AddRGBPoint(item.pixel,GetRValue(item.color)/255.0,
			GetGValue(item.color)/255.0,GetBValue(item.color)/255.0);
		opacityTransferFunction->AddPoint(item.pixel,item.opacity);
	}
	
	vtkVolumeProperty* volumeProperty=vtkVolumeProperty::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	colorTransferFunction->Delete();
	opacityTransferFunction->Delete();

	//update gradient list
	if(m_config.bGradient)
	{
		vtkPiecewiseFunction* gradOpacityTransferFunction=vtkPiecewiseFunction::New();
		gradOpacityTransferFunction->RemoveAllPoints();
		
		IA_PIXEL_ITEM item;		
		for(int i=0;i<m_config.gradListCount;i++)
		{
			item=m_config.gradList[i];
			gradOpacityTransferFunction->AddPoint(item.pixel,item.opacity);
		}
		volumeProperty->SetGradientOpacity(gradOpacityTransferFunction);
		gradOpacityTransferFunction->Delete();
	}else
	{
		volumeProperty->SetGradientOpacity(NULL);
	}

	if(m_config.bShade)
	{
		volumeProperty->ShadeOn();
	}else
	{
		volumeProperty->ShadeOff();
	}
	
	volumeProperty->SetDiffuse(1.0);
	volumeProperty->SetAmbient(.20);
    volumeProperty->SetSpecular(0);
    volumeProperty->SetSpecularPower(1);
	volumeProperty->SetInterpolationTypeToLinear();

	
	//set volume mapper
	float fLod=0.9; //优化显示参数，阴影时有效
	vtkVolumeMapper* volMapper=NULL;
	vtkFixedPointVolumeRayCastMapper *fixVolMapper=vtkFixedPointVolumeRayCastMapper::New();
	vtkGPUVolumeRayCastMapper *gpuVolMapper=vtkGPUVolumeRayCastMapper::New();
	if(gpuVolMapper->IsRenderSupported(m_pRenderWindow,volumeProperty))
	{
		gpuVolMapper->SetAutoAdjustSampleDistances( 1);
		gpuVolMapper->SetMinimumImageSampleDistance(m_config.LOD);
		gpuVolMapper->SetSampleDistance(m_config.LOD*m_config.sampleDistance);
		gpuVolMapper->SetMaximumImageSampleDistance(m_config.LOD*m_config.lowResLODFactor);
		gpuVolMapper->SetMaxMemoryInBytes(m_iGPUMemorySize);
		if(m_config.bShade)
		{
			gpuVolMapper->SetFinalColorWindow(fLod);
			gpuVolMapper->SetFinalColorLevel(fLod/2);
		}
		volMapper=gpuVolMapper;
	}else
	{ 
		fixVolMapper->SetAutoAdjustSampleDistances( 1);
		fixVolMapper->SetMinimumImageSampleDistance(m_config.LOD);
		fixVolMapper->SetSampleDistance(m_config.LOD*m_config.sampleDistance);
		fixVolMapper->SetMaximumImageSampleDistance(m_config.LOD*m_config.lowResLODFactor);
		if(m_config.bShade)
		{
			fixVolMapper->SetFinalColorWindow(fLod);
			fixVolMapper->SetFinalColorLevel(fLod/2);
		}
		volMapper=fixVolMapper;
	}
	
	
	volMapper->SetInput(m_pImage);
	m_volume->SetMapper(volMapper);
	//volMapper->SetInteractiveUpdateRate(0.001);
	
	if(m_config.renderMode==VOLRENDER_VOLUME)
	{
		volMapper->SetBlendModeToComposite();
	}else if(m_config.renderMode==VOLRENDER_MIP)
	{
		volMapper->SetBlendModeToMaximumIntensity();
	}else
	{
		volMapper->SetBlendModeToMinimumIntensity();
	}
	
	
	if(m_config.bCrop)
	{
		volMapper->SetCropping(1);
		switch(m_config.cropMode)
		{
		case ENUM_VOLRENDER_CROP_SUBVOLUME:
			volMapper->SetCroppingRegionFlagsToSubVolume();
			break;
		case ENUM_VOLRENDER_CROP_FENCE:
			volMapper->SetCroppingRegionFlagsToFence();
			break;
		case ENUM_VOLRENDER_CROP_INVERTED_FENCE:
			volMapper->SetCroppingRegionFlagsToInvertedFence();
			break;
		case ENUM_VOLRENDER_CROP_CROSS:
			volMapper->SetCroppingRegionFlagsToCross();
			break;
		case ENUM_VOLRENDER_CROP_INVERTED_CROSS:
			volMapper->SetCroppingRegionFlagsToInvertedCross();
			break;
		}
		
		volMapper->SetCroppingRegionPlanes(m_config.dCropPlane);
	}else
	{
		volMapper->SetCropping(0);
	}

	fixVolMapper->Delete();
	gpuVolMapper->Delete();

	m_volume->SetProperty(volumeProperty);
	volumeProperty->Delete();

	m_volume->SetPickable(1);
}

void CVolume::SetGPUMemorySize(int memSize)
{
	int iMinSize=128*1024*1024;
	if(memSize>=iMinSize)
	{
		m_iGPUMemorySize=memSize;
	}
}
