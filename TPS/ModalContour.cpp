// ModalContour.cpp: implementation of the CModalContour class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModalContour.h"
#include "TPS2DView.h"
#include "TPS3DView.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModalContour::CModalContour()
{
	m_actor=NULL;
	m_zType=ORGAN;
	m_modalType=ModalContour;
	m_iImageIndex=0;
}

CModalContour::~CModalContour()
{
	clear();
	
}



void CModalContour::clear()
{
	m_actor=NULL;
	m_iImageIndex=0;
}


BOOL CModalContour::save(FILE *file)
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
	
	fwrite(&m_options,sizeof(MESH_OPTIONS),1,file);
		
	return TRUE;
}


BOOL CModalContour::readContent(FILE *file)
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

	
	iReadCount=fread(&m_options,sizeof(MESH_OPTIONS),1,file);
	if(iReadCount!=1)
	{				
		return FALSE;
	}

	vtkImageData *pImage;
	if(imageIndex==1)
	{
		pImage=CGlobalManager::Manager()->m_2DView1->GetImage();
	}else if(imageIndex==2)
	{
		pImage=CGlobalManager::Manager()->m_2DView2->GetImage();
	}else if(imageIndex==3)
	{
		pImage=CGlobalManager::Manager()->m_2DView3->GetImage();
	}else
	{
		return FALSE;
	}

	if(!pImage)
		return FALSE;

	try
	{
		m_actor=GenerateMesh(pImage,m_options);
	}catch(...)
	{
		return FALSE;
	}
	if(!m_actor)
		return FALSE;

	
	m_iImageIndex = imageIndex;

	double fColor[3];
	fColor[0] = (float )GetRValue(m_color)/255.0;
	fColor[1] = (float )GetGValue(m_color)/255.0;
	fColor[2] = (float )GetBValue(m_color)/255.0;
	m_actor->GetProperty()->SetColor(fColor);
	m_actor->GetProperty()->SetOpacity(m_opacity/100.0);
	
	vtkMatrix4x4 *pImage2Scanner=vtkMatrix4x4::New();
	if(imageIndex==1)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView1->m_imageInfo.imageInScanner);
	}else if(imageIndex==2)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView2->m_imageInfo.imageInScanner);
	}else if(imageIndex==3)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView3->m_imageInfo.imageInScanner);
	}else
	{
		pImage2Scanner->Identity();
	}

	vtkTransform* pTransform = vtkTransform::New();
	pTransform->SetMatrix(pImage2Scanner);
	m_actor->SetUserTransform(pTransform);
	pTransform->Delete();
	pImage2Scanner->Delete();

	CGlobalManager::Manager()->m_3DView->AddActor(this,m_actor);
	
	m_actor->Delete();
	return TRUE;
}



void CModalContour::AddActor(int imageIndex,vtkActor *pActor,IMAGE_CONTOUR_INFO* info)
{
	for(int i=0;i<6;i++)
	{
		m_options.VOI[i]=info->VOI[i];
	}
	m_options.label=info->contourValue;
	m_options.bLargest=info->bLargest;

	m_actor=pActor;
	
	if(!m_actor)
		return ;

	m_name=GetValidName();

	m_iImageIndex=imageIndex;
	m_color=info->color;
	
	CGlobalManager::Manager()->m_3DView->AddActor(this,m_actor);

}

CString CModalContour::GetValidName()
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
