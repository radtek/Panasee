// SaveActor2Obj.cpp: implementation of the CSaveActor2Obj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveActor2Obj.h"
#include "vtkPolyDataMapper.h"
#include "MeshWriter.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveActor2Obj::CSaveActor2Obj()
{
	m_pActor=NULL;
	m_sObjFile="";
}

CSaveActor2Obj::~CSaveActor2Obj()
{

}



void CSaveActor2Obj::Save()
{
	if(!m_pActor)
		return;
	if(m_sObjFile=="")
		return;

	CMeshWriter *write=CMeshWriter::New();
		
	try
	{
		vtkPolyData *pPolyData=((vtkPolyDataMapper*)m_pActor->GetMapper())->GetInput();
		
		int iFind=m_sObjFile.Find("\\",0);
		if(iFind>=0)
		{
			int iResult=iFind;			
			while(iFind>=0)
			{
				iFind=m_sObjFile.Find("\\",iFind+1);
				if(iFind>0)
				{
					iResult=iFind;
				}else
				{
					break;
				}
			}
			if(iResult>0)
			{
				CString sTmp=m_sObjFile.Left(iResult);
				::CreateDirectory( sTmp, NULL );
			}
		}
						
		write->SetFileName(m_sObjFile);
		write->Writer(pPolyData);
		
		write->Delete();
	}catch(...)
	{
		PanaMessageBox("Failed to save segmented result.");
		return;
	}

}


UINT SaveActor2Obj(LPVOID pSaveActor2Obj)
{
	CSaveActor2Obj* pClass=(CSaveActor2Obj*)pSaveActor2Obj;

	try
	{
		pClass->Save();
		
		AfxEndThread(0,TRUE);
		return 0;
	}catch(...)
	{
		
		AfxEndThread(-1,TRUE);
		return -1;
	}
	AfxEndThread(0,TRUE);
	return 0;

}