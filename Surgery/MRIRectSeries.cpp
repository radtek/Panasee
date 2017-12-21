// MRIRectSeries.cpp: implementation of the MRIRectSeries class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "MRIRectSeries.h"
//#include "./LookupTable/LUT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CLUT32 m_oClut32;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MRIRectSeries::MRIRectSeries()
{
	m_ppMRIRects = NULL;
	m_pMRISeries = NULL;
	m_iRectNum = 0;
	m_idispMRIRectIndex = 0;
	m_sName = "";
	m_dispMRIRect.m_pMRIRectSeries = this;
	m_iResolutionX = 0;
	m_iResolutionY = 0;
}

MRIRectSeries::~MRIRectSeries()
{
	if (m_ppMRIRects)
	{
		for (int i=0; i<m_iRectNum; i++)
		{
			if (m_ppMRIRects[i])
			{
				delete m_ppMRIRects[i];
				m_ppMRIRects[i] = NULL;
			}
		}
		m_iRectNum = 0;
		delete[] m_ppMRIRects;
		m_ppMRIRects = NULL;
	}
}

void MRIRectSeries::Clean()
{
	if (m_ppMRIRects)
	{
		for (int i=0; i<m_iRectNum; i++)
		{
			if (m_ppMRIRects[i])
			{
				delete m_ppMRIRects[i];
				m_ppMRIRects[i] = NULL;
			}
		}
		m_iRectNum = 0;
		delete[] m_ppMRIRects;
		m_ppMRIRects = NULL;
	}
	m_pMRISeries = NULL;
}

BOOL MRIRectSeries::Init(CMRISeries *pMRISeries)
{
	if (pMRISeries == NULL)
		return FALSE;

//	m_pMRISeries = pMRISeries;

	int iRectNum = pMRISeries->GetNumOfImages();

	if (iRectNum != m_iRectNum 
		|| pMRISeries->GetImageAt(0)->GetResolutionX() != m_iResolutionX
		|| pMRISeries->GetImageAt(0)->GetResolutionY() != m_iResolutionY)
	{
		Clean();
		m_pMRISeries = pMRISeries;
		m_iRectNum = iRectNum;
		m_iResolutionX = pMRISeries->GetImageAt(0)->GetResolutionX();
		m_iResolutionY = pMRISeries->GetImageAt(0)->GetResolutionY();
		m_ppMRIRects = new CMRIRectangle*[m_iRectNum];
		for (int i=0; i<m_iRectNum; i++)
		{
			m_ppMRIRects[i] = new CMRIRectangle(this, i);
		}
	}
	else
	{
		m_pMRISeries = pMRISeries;
		for (int i=0; i<m_iRectNum; i++)
		{
			m_ppMRIRects[i]->SetMRIImg(pMRISeries->GetImageAt(i));//直接改变图象
		}
	}

	// Commented by Huagen 2007/08/28, this is redundant 
/*	if (m_idispMRIRectIndex >= 0  && m_idispMRIRectIndex < m_iRectNum)
	{
		m_dispMRIRect.SetMRIImg(pMRISeries->GetImageAt(m_idispMRIRectIndex));
	} */
	
	return TRUE;
}

CMRIRectangle* MRIRectSeries::GetMRIRectAt(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_iRectNum)
		return NULL;

	return m_ppMRIRects[iIndex];
}

CMRIRectangle* MRIRectSeries::GetDispMRIRect()
{
	return &m_dispMRIRect;
}

void MRIRectSeries::UpdateTexture(BOOL bUpdateAllImage)//更新纹理
{
	if(bUpdateAllImage)
	{
		for (int i=0; i<m_iRectNum; i++)
		{
			if (m_ppMRIRects[i]->m_bIsVisible)
			{
				m_ppMRIRects[i]->CreateMRIRect();//包括创建Rect和Texture
			}
		}
	}

	if (m_dispMRIRect.m_bIsVisible)
	{
		m_dispMRIRect.CreateMRIRect();
	}
}

void MRIRectSeries::UpdatePos()
{
	for (int i=0; i<m_iRectNum; i++)
	{
		m_ppMRIRects[i]->UpdatePos();
	}
}

void MRIRectSeries::SetDispIndex(int iIndex)
{
	if (iIndex >= 0  && iIndex < m_iRectNum)
	{
		
		m_idispMRIRectIndex = iIndex;
		m_dispMRIRect.SetMRIImg(m_ppMRIRects[m_idispMRIRectIndex]->GetMRIImg(), true);
		
	}
}

int MRIRectSeries::GetDispIndex()
{
	return m_idispMRIRectIndex;
}

CMRISeries* MRIRectSeries::GetMRISeries()
{
	return m_pMRISeries;
}

void MRIRectSeries::SetName(CString sName)
{
	m_sName = sName;
}

CString MRIRectSeries::GetName()
{
	return m_sName;
}

int MRIRectSeries::GetRectNum()
{
	if (m_pMRISeries)
	{
		return m_pMRISeries->GetNumOfImages();
	}
	else
	{
		return 0;
	}
}