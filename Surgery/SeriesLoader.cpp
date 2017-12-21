// SeriesLoader.cpp: implementation of the CSeriesLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "SeriesLoader.h"
#include "imageConvertor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeriesLoader::CSeriesLoader()
{

}

CSeriesLoader::~CSeriesLoader()
{

}

void CSeriesLoader::LoadSeries(CString sPath)
{
	m_seriesList.RemoveAll();

	CMRISeries *pMRISeries=NULL;	
	//查找目录下的.vti文件,如果没有,则按照.img的方式打开
	CFileFind finder;
	CString sTmp;
	sTmp.Format("%s\\*.vti",sPath);
	BOOL bFindVTI = finder.FindFile(sTmp);
	finder.Close();

	sTmp.Format("%s\\*.img",sPath);
	BOOL bFindIMG = finder.FindFile(sTmp);
	finder.Close();

	try
	{
		if (bFindVTI)
		{
			sTmp.Format("%s\\*.vti",sPath);
			finder.FindFile(sTmp);
			finder.FindNextFile();		
			CString sFile = finder.GetFilePath();
			finder.Close();
			CImageConvertor imageConvertor;
			pMRISeries=(CMRISeries*)imageConvertor.Vti2MRISeries(sFile);
			if(!pMRISeries)
			{
				PanaMessageBox("Failed to load series. Continual memory is not enough.");
				return;
			}
			
			//加载融合坐标
			pMRISeries->m_sFolder=sPath;
			Matrix m(4,4);
			if(pMRISeries->LoadFusionCoord(sPath,m))
			{
				pMRISeries->SetFusionCoord(m.value);
			}
			
			m_seriesList.AddTail(pMRISeries);
			return;
		}else if(bFindIMG)
		{		
			pMRISeries = new CMRISeries;
			if (pMRISeries->Load( sPath ))
			{
				delete pMRISeries;
				pMRISeries = NULL;
				PanaMessageBox("Failed to load series.\n  Series is not available at " + sPath + ".", MB_ICONWARNING);
				return ;
			}
			
			//加载融合坐标
			Matrix m(4,4);
			if(pMRISeries->LoadFusionCoord(sPath,m))
			{
				pMRISeries->SetFusionCoord(m.value);
			}
			
			m_seriesList.AddTail(pMRISeries);
			return;
		}else
		{
			CImageConvertor plus;
			plus.ReadDCMFolder(sPath);
			if(plus.GetSeriesCount()>0)
			{
				for(int index=0;index<plus.GetSeriesCount();index++)
				{
					pMRISeries = (CMRISeries*)plus.GetMRISeriesAt(index);

					//加载融合坐标
					pMRISeries->m_sFolder=sPath;
					Matrix m(4,4);
					if(pMRISeries->LoadFusionCoord(sPath,m))
					{
						pMRISeries->SetFusionCoord(m.value);
					}
					
					m_seriesList.AddTail(pMRISeries);
				}
			}else
			{
				PanaMessageBox("Failed to load series.\n  Series is not available at " + sPath + ".", MB_ICONWARNING);
			}
			return;
		}
	}catch(...)
	{
		return;
	}
	
	
}

int CSeriesLoader::GetCount()
{
	return m_seriesList.GetCount();
}

CMRISeries* CSeriesLoader::GetAt(int index)
{
	if(index>=0 && index<m_seriesList.GetCount())
	{
		CMRISeries *pMRISeries=m_seriesList.GetAt(m_seriesList.FindIndex(index));		
		return pMRISeries;
	}
	return NULL;
}
