// ProbePool.h: interface for the CProbePool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBEPOOL_H__521406F4_6732_40BA_B287_1984971DF509__INCLUDED_)
#define AFX_PROBEPOOL_H__521406F4_6732_40BA_B287_1984971DF509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Probe.h"

class CProbePool  
{
public:
	CProbePool();
	virtual ~CProbePool();

	void InitPreimageProbe();
	void AddNewProbe(CProbe *pProbe);
	int CProbePool::DeleteProbe(CProbe *pProbe);
	CProbe*	GetProbeByIndex(int iIndex);  // Get by probe index
	CProbe*	GetProbeByName(CString sName);
	CProbe* GetProbeByArrIndex(int iIndex);	// Get by array index
	CProbe* GetPreimageProbe() {return &m_preImgProbe;};
	CProbe* GetLastProbe();
	CArray<CProbe*, CProbe*>* GetProbePool();
	int	 GetNextNewProbeIndex();
	void SetNextNewProbeIndex(int iIndex);

private:
	CArray<CProbe*, CProbe*> m_arrpProbePool;
	CProbe	m_preImgProbe;
	int		m_iProbeNameIndex;		// Use for remember largest probe name
};

#endif // !defined(AFX_PROBEPOOL_H__521406F4_6732_40BA_B287_1984971DF509__INCLUDED_)
