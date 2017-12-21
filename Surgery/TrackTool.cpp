// TrackTool.cpp: implementation of the CTrackTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "TrackTool.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackTool::CTrackTool()
{
	m_pAttachedProbe = NULL;
	m_iNDIPort = -1;
	m_pAttachedProbe = NULL;
	m_TrackType = TT_EMPTY;
}

CTrackTool::~CTrackTool()
{

}

void CTrackTool::AttachTo(CProbe *pProbe)
{
	if (pProbe->m_pTrackTool)
	{
		pProbe->m_pTrackTool->Detach();
	}
	Detach();
	m_pAttachedProbe = pProbe;
	pProbe->m_pTrackTool = this;
	m_TrackType = TT_NEEDLE;
}

void CTrackTool::Detach(bool bSaveToINI /*= true*/)
{
	if (m_pAttachedProbe)
	{
		if (m_pAttachedProbe->m_pTrackTool)
		{
			CString szPortHandle;
			m_TrackType = TT_EMPTY;
		}
		m_pAttachedProbe->m_pTrackTool = NULL;
	}
	m_pAttachedProbe = NULL;
}