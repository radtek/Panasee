// EMHandler.h: interface for the CEMHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKERHANDLER_H__6A370D2C_C740_4835_819C_EB05CD3810DB__INCLUDED_)
#define AFX_TRACKERHANDLER_H__6A370D2C_C740_4835_819C_EB05CD3810DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mv.h"

enum TRACK_DEVICE_TYPE
{
	TRACK_DEVICE_NDI_OPTIC=1,
	TRACK_DEVICE_NDI_EM
};

class AFX_EXT_CLASS CTrackerHandler  
{
public:
	virtual BOOL IsValidTool(int index);
	virtual CString GetErrorInfo();
	virtual BOOL StopTracking();
	virtual BOOL StartTracking();
	virtual BOOL IsValidDevice();
	virtual BOOL GetMatrix(int iToolIndex,Matrix& m);
	virtual int GetToolCount();
	virtual BOOL InitNDI();
	virtual BOOL RefreshMatrix();
	virtual int GetDeviceType();
	

	CTrackerHandler();
	virtual ~CTrackerHandler();
protected:
	int m_iToolCount;
	CString m_sErrorInfo;
};

#endif // !defined(AFX_EMHANDLER_H__6A370D2C_C740_4835_819C_EB05CD3810DB__INCLUDED_)
