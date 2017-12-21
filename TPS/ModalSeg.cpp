// ModalSeg.cpp: implementation of the CModalSeg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModalSeg.h"
#include "TPS2DView.h"
#include "TPS3DView.h"
#include "GlobalManager.h"
#include "vtkOBJReader.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModalSeg::CModalSeg()
{
	m_actor=NULL;
	m_zType=ORGAN;
	m_modalType=ModalSeg;

	m_iImageIndex=0;
	m_sObjFileName="";
}

CModalSeg::~CModalSeg()
{
	clear();
}


void CModalSeg::clear()
{
	m_actor=NULL;
	m_iImageIndex=0;
}


BOOL CModalSeg::save(FILE *file,CString sTpsFile)
{
	if(!CModal::save(file))
		return FALSE;
	
	if(m_iImageIndex<=0 || m_iImageIndex>3)
		return FALSE;

	if(m_sObjFileName=="")
		return FALSE;

	CString sTpsModalFolder=GetValidObjPath(sTpsFile);
	CString sCurFolder="";
	int iFind=m_sObjFileName.ReverseFind('\\');
	if(iFind<2)
	{
		return FALSE;
	}
	sCurFolder=m_sObjFileName.Left(iFind+1);
	if(sCurFolder!=sTpsModalFolder)
	{
		//get valid file name;
		int i=0;
		CFileFind finder;
		CString sTmp;
		
		sTmp.Format("%s%i.obj",sTpsModalFolder,i);
		BOOL bFind = finder.FindFile(sTmp);
		while(bFind)
		{
			i++;
			sTmp.Format("%s%i.obj",sTpsModalFolder,i);
			bFind = finder.FindFile(sTmp);
		}

		CopyFile(m_sObjFileName,sTmp,FALSE);

		m_sObjFileName=sTmp;
	}


	MODAL_INFO modalGroup;
	modalGroup.color=m_color;
	sprintf(modalGroup.name,"%s",m_name);
	modalGroup.Opacity=m_opacity;
	modalGroup.treeItem=m_treeItem;
	modalGroup.volume=m_volume;
	modalGroup.zType=m_zType;
	fwrite(&modalGroup,sizeof(MODAL_INFO),1,file);

	fwrite(&m_iImageIndex,sizeof(int),1,file);
	
	iFind=m_sObjFileName.ReverseFind('\\');
	CString sTmp;
	sTmp = m_sObjFileName.Right(m_sObjFileName.GetLength()-iFind-1);
	char objFile[256];
	sprintf(objFile,"%s",sTmp);
	fwrite(objFile,sizeof(char),256,file);
	
		
	return TRUE;
}


BOOL CModalSeg::readContent(FILE *file,CString sTpsFile)
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

	char objFile[256];
	iReadCount=fread(objFile,sizeof(char),256,file);
	if(iReadCount!=256)
	{				
		return FALSE;
	}
	m_sObjFileName.Format("%s",objFile);
	CString sTpsModalFolder=GetValidObjPath(sTpsFile);
	m_sObjFileName = sTpsModalFolder + m_sObjFileName;

	MESH_OPTIONS option;
	option.color=m_color;
	option.opacity=m_opacity/100.0;
	
	m_actor=ReadObjFile(m_sObjFileName);
	if(m_actor==NULL)
		return FALSE;

	// 设置坐标系
	vtkMatrix4x4 *pImage2Scanner=vtkMatrix4x4::New();
	if(m_iImageIndex==1)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView1->m_imageInfo.imageInScanner);
	}else if(m_iImageIndex==2)
	{
		vtkMatrix4x4::DeepCopy(*pImage2Scanner->Element,CGlobalManager::Manager()->m_2DView2->m_imageInfo.imageInScanner);
	}else if(m_iImageIndex==3)
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

	AddActor(m_iImageIndex,m_actor,option,FALSE,"");
	m_actor->Delete();
	
	return TRUE;
}

void CModalSeg::AddActor(int imageIndex, vtkActor *pActor, MESH_OPTIONS meshOption,
						 BOOL bSaveObjFile,CString sTpsFile)
{
	if(bSaveObjFile)
	{
		m_color=meshOption.color;
		m_opacity=meshOption.opacity*100;
		m_iImageIndex=imageIndex;

		m_actor=pActor;
		m_sObjFileName=SaveObjFile(pActor,sTpsFile);

		CString sTmp;
		sTmp.Format("%s",meshOption.name);
		m_name=GetValidName(sTmp);
	}	
		
	CGlobalManager::Manager()->m_3DView->AddActor(this,m_actor);
	
}

CString CModalSeg::GetValidName(CString sFirstName)
{
	CString sName;
	CModal* tmpModalInfo;
	int j=0;
	CString sTmp=sFirstName;
	POSITION pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		tmpModalInfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",tmpModalInfo->m_name);
		if(sTmp == sName )
		{
			pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
			j++;
			sTmp.Format("%s_%i",sFirstName,j);
			i=-1;
		}
			
		
	}
	return sTmp;
}

CString CModalSeg::SaveObjFile(vtkActor *pActor,CString sTpsFile)
{
	CString sFolder = GetValidObjPath(sTpsFile);

	//get valid file name;
	int i=0;
	CFileFind finder;
	CString sTmp;

	sTmp.Format("%s%i.obj",sFolder,i);
	BOOL bFind = finder.FindFile(sTmp);
	while(bFind)
	{
		i++;
		sTmp.Format("%s%i.obj",sFolder,i);
		bFind = finder.FindFile(sTmp);
	}

	m_saveObj.m_sObjFile=sTmp;
	m_saveObj.m_pActor=pActor;

	AfxBeginThread(SaveActor2Obj,(LPVOID)(&m_saveObj),THREAD_PRIORITY_LOWEST,0,0,NULL);
	return sTmp;
}

vtkActor* CModalSeg::ReadObjFile(CString sFile)
{
	try
	{
		vtkOBJReader *reader=vtkOBJReader::New();
		reader->SetFileName(sFile);
		
		
		vtkPolyDataNormals* m_NormalsFilter = vtkPolyDataNormals::New();
		m_NormalsFilter->SplittingOff();
		m_NormalsFilter->ConsistencyOff();
		m_NormalsFilter->ReleaseDataFlagOn();
		m_NormalsFilter->SetInput(reader->GetOutput());
		reader->Delete();
		
		vtkPolyDataMapper *isoMapper=vtkPolyDataMapper::New();
		isoMapper->SetInput(m_NormalsFilter->GetOutput());
		isoMapper->ScalarVisibilityOff();
		m_NormalsFilter->Delete();
		
		isoMapper->Update();
		
		m_actor=vtkActor::New();
		m_actor->SetMapper(isoMapper);
		isoMapper->Delete();
		
		m_actor->GetProperty()->SetDiffuseColor(1, .49, .25);
		m_actor->GetProperty()->SetSpecular(.3);
		m_actor->GetProperty()->SetSpecularPower(20);
		m_actor->GetProperty()->SetOpacity(m_opacity);
		m_actor->GetProperty()->SetColor(GetRValue(m_color)/255.0,
			GetGValue(m_color)/255.0,
			GetBValue(m_color)/255.0);
		m_actor->GetProperty()->SetShading(1);
	}catch(...)
	{
		return NULL;
	}
	
	return m_actor;
}

CString CModalSeg::GetValidObjPath(CString sTpsFile)
{
	CString sFolder="";
	if(sTpsFile=="")
	{
		TCHAR szStylesPath[_MAX_PATH];
		VERIFY(::GetModuleFileName(
			AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
		
		CString strPath = szStylesPath;
		int nIndex  = strPath.ReverseFind(_T('\\'));
		if (nIndex > 0) {
			strPath = strPath.Left(nIndex);
		}
		else {
			strPath.Empty();
		}
		sFolder=strPath+"\\TpsData\\";
		
		
		CConfigMgr conf;
		CString sPath = conf.ReadREG(HKEY_LOCAL_MACHINE, "SOFTWARE\\XinaoMDT\\Panasee\\Database", "Folder", "C:\\TPSData");
		if(sPath!="C:\\TPSData")
		{
			if(sPath.Right(1)=="\\")
			{
				sPath = sPath.Left(sPath.GetLength()-1);
			}
			int iFind = sPath.ReverseFind('\\');
			if(iFind>=0)
			{
				sPath = sPath.Left(iFind +1);
			}
			sFolder = sPath + "TpsData\\";
		}

		CreateDirectory(sFolder, 0);
		
	}else
	{
		CString fileName=sTpsFile;
		//分离出tps文件所在的目录
		fileName.MakeLower();		
		int iFind=fileName.ReverseFind('\\');		
		CString sTpsFolder=fileName.Left(iFind);
		CString sTpsFilename=fileName.Mid(iFind+1,fileName.GetLength()-iFind-5);
		CString sTmp;
		sTmp.Format("%s\\%s",sTpsFolder,sTpsFilename);
		sTpsFolder=sTmp;
		
		//创建临时目录
		CFileFind finder;
		if(!finder.FindFile(sTpsFolder))
		{
			if(!::CreateDirectory( sTpsFolder, NULL ))
			{			
				return "";
			}
		}
		sTmp.Format("%s\\Modal",sTpsFolder);
		if(!finder.FindFile(sTmp))
		{
			if(!::CreateDirectory( sTmp, NULL ))
			{			
				return "";
			}
		}
		sFolder=sTmp+"\\";
	}

	return sFolder;
}
