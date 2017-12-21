// TrackTool.h: interface for the CTrackTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKTOOL_H__5A2CFE76_20E2_40ED_BA50_AB281771E5A1__INCLUDED_)
#define AFX_TRACKTOOL_H__5A2CFE76_20E2_40ED_BA50_AB281771E5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Probe.h"

enum Track_Type {
	TT_BASE,
	TT_PHATOM,
	TT_NEEDLE,
	TT_EMPTY
};

class CTrackTool  
{
public:
	CTrackTool();
	virtual ~CTrackTool();

	void AttachTo(CProbe *pProbe);
	void Detach(bool bSaveToINI = true);

	CProbe *m_pAttachedProbe;
	int    m_iNDIPort;
	enum Track_Type  m_TrackType;
};

#endif // !defined(AFX_TRACKTOOL_H__5A2CFE76_20E2_40ED_BA50_AB281771E5A1__INCLUDED_)
