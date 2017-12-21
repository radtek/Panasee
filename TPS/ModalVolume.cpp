// ModalVolume.cpp: implementation of the CModalVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModalVolume.h"
#include "Volume.h"
#include "TPS2DView.h"
#include "TPS3DView.h"
#include "GlobalManager.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModalVolume::CModalVolume()
{
	m_pVolume=NULL;
	m_actor=NULL;
	m_zType=ORGAN;
	m_modalType=ModalVolume;
	
	m_pRenderWindow=NULL;
	m_iImageIndex=0;

	m_pImage=NULL;

	CConfigMgr conf;
	int iMinSize=128*1024*1024;
	m_iGPUMemorySize = conf.Read(GPU, GPU_MEMORY_SIZE, iMinSize);
	if(m_iGPUMemorySize<=iMinSize)
	{
		m_iGPUMemorySize=iMinSize;
	}
}

CModalVolume::~CModalVolume()
{
	clear();
}

BOOL CModalVolume::AddVolume(int iIndex, VOLRENDER_CONFIG config,BOOL bResetName)
{
	vtkImageData *pImage=NULL;

	if(iIndex==1)
	{
		pImage=CGlobalManager::Manager()->m_2DView1->GetImage();
	}else if(iIndex==2)
	{
		pImage=CGlobalManager::Manager()->m_2DView2->GetImage();
	}else if(iIndex==3)
	{
		pImage=CGlobalManager::Manager()->m_2DView3->GetImage();
	}else
	{
		return FALSE;
	}

	if(!pImage)
		return FALSE;

	if(bResetName)
	{
		m_name=GetValidName();
	}
	m_iImageIndex=iIndex;

	m_config=config;
	m_config.bCrop=config.bCrop;
	m_config.cropMode=config.cropMode;
	for (int k=0;k<6;k++)
	{
		m_config.dCropPlane[k]=config.dCropPlane[k];
	}

	CVolume vol;
	vol.SetGPUMemorySize(m_iGPUMemorySize);
	vol.m_pRenderWindow=m_pRenderWindow;
	vol.SetImage(pImage);
	vol.SetVolumeInfo(&m_config);
	
	m_pVolume=vol.GetVolume();
	m_pImage=pImage;

	vtkMatrix4x4 *pImage2Scanner=vtkMatrix4x4::New();
	if(iIndex==1)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView1->m_imageInfo.imageInScanner);
	}else if(iIndex==2)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView2->m_imageInfo.imageInScanner);
	}else if(iIndex==3)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView3->m_imageInfo.imageInScanner);
	}else
	{
		pImage2Scanner->Identity();
	}

	vtkTransform* pTransform = vtkTransform::New();
	pTransform->SetMatrix(pImage2Scanner);
	m_pVolume->SetUserTransform(pTransform);
	pTransform->Delete();
	pImage2Scanner->Delete();
	
	CGlobalManager::Manager()->m_3DView->AddVolume(this,m_pVolume);

	return TRUE;
}

BOOL CModalVolume::save(FILE *file)
{
	if(!CModal::save(file))
		return FALSE;

	if(m_iImageIndex<=0 || m_iImageIndex>3)
		return FALSE;

	
	MODAL_INFO modalGroup;
	modalGroup.color=m_color;
	sprintf(modalGroup.name,"%s",m_name);
	modalGroup.Opacity=m_opacity;
	modalGroup.treeItem=m_treeItem;
	modalGroup.volume=m_volume;
	modalGroup.zType=m_zType;
	fwrite(&modalGroup,sizeof(MODAL_INFO),1,file);

	fwrite(&m_iImageIndex,sizeof(int),1,file);
	
	fwrite(&m_config,sizeof(VOLRENDER_CONFIG),1,file);
		
	return TRUE;
}

BOOL CModalVolume::readContent(FILE *file)
{
	
	MODAL_INFO modalGroup;
	int iReadCount=fread(&modalGroup,sizeof(MODAL_INFO),1,file);
	if(iReadCount!=1)
	{		
		return FALSE;
	}
	
	m_color=modalGroup.color;
	m_name.Format("%s",modalGroup.name);
	m_opacity=modalGroup.Opacity;
	m_treeItem=modalGroup.treeItem;
	m_volume=modalGroup.volume;
	m_zType=modalGroup.zType;

	int imageIndex;
	iReadCount=fread(&imageIndex,sizeof(int),1,file);
	if(iReadCount!=1)
	{				
		return FALSE;
	}
	m_iImageIndex=imageIndex;

	VOLRENDER_CONFIG config;
	iReadCount=fread(&config,sizeof(VOLRENDER_CONFIG),1,file);
	if(iReadCount!=1)
	{				
		return FALSE;
	}
		
	AddVolume(m_iImageIndex,config,FALSE);
		
	return TRUE;
}

void CModalVolume::clear()
{
	m_pVolume=NULL;
	m_actor=NULL;
	m_iImageIndex=0;
	m_pImage=NULL;
}


CString CModalVolume::GetValidName()
{
	CString sName;
	CModal* tmpModalInfo;
	int j=1;
	CString sTmp="Modal_1";
	POSITION pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		tmpModalInfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",tmpModalInfo->m_name);
		if(sTmp == sName )
		{
			pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
			j++;
			sTmp.Format("Modal_%i",j);
			i=-1;
		}
			
		
	}
	return sTmp;
}
