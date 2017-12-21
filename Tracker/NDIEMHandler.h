// NDIHandler.h: interface for the CNDIHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDIEMHANDLER_H__1CD936B0_9303_4216_AC97_C84D6FC3E8CE__INCLUDED_)
#define AFX_NDIEMHANDLER_H__1CD936B0_9303_4216_AC97_C84D6FC3E8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandHandling.h"
#include "TrackerHandler.h"

class CNDIEMHandler :public CTrackerHandler
{
public:
	
	CNDIEMHandler();
	virtual ~CNDIEMHandler();

	virtual BOOL IsValidTool(int index);
	virtual BOOL RefreshMatrix();
	virtual BOOL StopTracking();
	virtual BOOL StartTracking();
	virtual BOOL GetMatrix(int iToolIndex,Matrix& m);
	virtual int GetToolCount();
	virtual BOOL InitNDI();				// Execute ConnectNDI and then ActiveTool
	virtual BOOL IsValidDevice();
	virtual int GetDeviceType();
	
private:
	BOOL ActiveTool();
	BOOL ConnectNDI();
	
	int m_iMinValidHandle;
	BOOL	m_bNDISystemConnected;
	BOOL	m_bPortsActivated;
	CCommandHandling *m_pNDICommandHandler;
	int m_minIndex;
};

#endif // !defined(AFX_NDIHANDLER_H__1CD936B0_9303_4216_AC97_C84D6FC3E8CE__INCLUDED_)
