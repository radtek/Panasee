// MRIBuffer.cpp: implementation of the CMRIBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "MRIBuffer.h"
#include "MRISeries.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRIBuffer::CMRIBuffer()
{
	m_iDispIndex = -1;
	m_iSaveIndex = 0;
	for (int i=0; i<IMG_BUFFER_SIZE; i++)
	{
		m_ppMRIBuffer[i] = NULL;
	}
}

CMRIBuffer::~CMRIBuffer()
{
	for (int i=0; i<IMG_BUFFER_SIZE; i++)
	{
		if (m_ppMRIBuffer[i])
		{
			
			delete m_ppMRIBuffer[i];
			m_ppMRIBuffer[i] = NULL;
		}
	}
}

BOOL CMRIBuffer::Push(CMRISeries* pMRISeries)
{
	//Added by Wang fengliang 070808;
	//避免重复导入同一序列
	//--begin
	for(int i=0;i<IMG_BUFFER_SIZE;i++)
	{
		if(m_ppMRIBuffer[i]&&m_ppMRIBuffer[i]->GetName()==pMRISeries->GetName())
			return FALSE;
	}
	//--end
	while (m_ppMRIBuffer[m_iSaveIndex] && m_ppMRIBuffer[m_iSaveIndex]->IsVisible())
	{
		m_iSaveIndex = NextSlot(m_iSaveIndex);
	}

	if (m_ppMRIBuffer[m_iSaveIndex])
	{
		delete m_ppMRIBuffer[m_iSaveIndex];
	}
	m_ppMRIBuffer[m_iSaveIndex] = pMRISeries;
	m_iDispIndex = m_iSaveIndex;                 //最后导入序列...
	m_iSaveIndex = NextSlot(m_iSaveIndex);
	return TRUE;
}
//Added by Wang fengliang 070809;
//删除一个序列
//参数:strName 学列名称
BOOL CMRIBuffer::Delete(CString strName)
{
	if(strName=="")
		return FALSE;
	for(int i=0;i<IMG_BUFFER_SIZE;i++)
	{
		if(m_ppMRIBuffer[i]&&m_ppMRIBuffer[i]->GetName()==strName)
		{
			delete m_ppMRIBuffer[i];
			m_ppMRIBuffer[i]=NULL;
			return TRUE;
		}
	}
	return FALSE;
}
// Added by Wang fengliang 070809;
// 设置当前显示的序列
BOOL CMRIBuffer::SetDisp(CMRISeries* pMRISeries)
{
	if(pMRISeries == NULL)
		return FALSE ;
	for(int i=0;i<IMG_BUFFER_SIZE;i++)
	{
		if(m_ppMRIBuffer[i]&&m_ppMRIBuffer[i] == pMRISeries)
		{
			m_iDispIndex = i ;
			return TRUE;
		}
	}
	return FALSE;
}
CMRISeries* CMRIBuffer::GetDisp()
{
	if (m_iDispIndex >= 0)
	{
		return m_ppMRIBuffer[m_iDispIndex];
	}
	else
		return NULL;
}

CMRISeries* CMRIBuffer::GetSeries(CString sStudyID, int nSeriesID)
{
	if (m_iDispIndex == -1)//没有导入序列,则直接返回
	{
		return NULL;
	}
	
//	int iStop = NextSlot(m_iDispIndex);  //Commented by Wang fengliang 070809;
	for (int i=0; i<IMG_BUFFER_SIZE; i++)
	{
		if (m_ppMRIBuffer[i] &&
			m_ppMRIBuffer[i]->GetSeriesID() == nSeriesID &&
			m_ppMRIBuffer[i]->GetStudyID() == sStudyID)
		{
			return m_ppMRIBuffer[i];
		}
	}
	return NULL;
}

CMRISeries* CMRIBuffer::GetSeries(CString sName)
{
	if (m_iDispIndex == -1)
	{
		return NULL;
	}
	
	for (int i=0; i<IMG_BUFFER_SIZE; i++)
	{
		if (m_ppMRIBuffer[i] &&
			m_ppMRIBuffer[i]->GetName() == sName)
		{
			return m_ppMRIBuffer[i];
		}
	}
	return NULL;
}

CMRISeries* CMRIBuffer::GetSeriesAt(int iIndex)
{
	if (iIndex < 0 || iIndex >= IMG_BUFFER_SIZE)
		return NULL;

	return m_ppMRIBuffer[iIndex];
}

int CMRIBuffer::NextSlot(int iIndex)
{
	if (iIndex == IMG_BUFFER_SIZE - 1)
	{
		return 0;
	}
	else
	{
		return iIndex + 1;
	}
}

int CMRIBuffer::PreSlot(int iIndex)
{
	if (iIndex == 0)
	{
		return IMG_BUFFER_SIZE - 1;
	}
	else
	{
		return iIndex - 1;
	}
}