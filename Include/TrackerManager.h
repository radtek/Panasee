// EMManager.h: interface for the CEMManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKERMANAGER_H__92F77555_2B9F_4EA1_AE34_BB4249DEF44D__INCLUDED_)
#define AFX_TRACKERMANAGER_H__92F77555_2B9F_4EA1_AE34_BB4249DEF44D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "trackerHandler.h"

class AFX_EXT_CLASS CTrackerManager  
{
public:
	CTrackerHandler* GetHandler();

	CTrackerManager();
	virtual ~CTrackerManager();

private:
	CString GetStartupPath();
	CString m_sStartupPath;
	CTrackerHandler *m_pTrackerHandler;
	TRACK_DEVICE_TYPE m_trackerType;
};

#endif // !defined(AFX_EMMANAGER_H__92F77555_2B9F_4EA1_AE34_BB4249DEF44D__INCLUDED_)
