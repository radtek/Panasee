// EMManager.cpp: implementation of the CEMManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "trackerManager.h"
#include "ConfigMgr.h"
#include "NDIEMHandler.h"
#include "NDIOpticHandler.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackerManager::CTrackerManager()
{
	m_sStartupPath=GetStartupPath();

	char pszFileName[MAX_PATH];
	sprintf( pszFileName, "%s\\Surgery.INI", m_sStartupPath);
	SetINIFileName(pszFileName);

	m_trackerType=TRACK_DEVICE_NDI_OPTIC;
	m_pTrackerHandler=NULL;

	CConfigMgr conf;
	int iType = conf.Read(TRACK_DEVICE,TRACK_DEVICE_TYPE, 1);
	if(iType==1) 
	{
		m_trackerType = TRACK_DEVICE_NDI_OPTIC;
	}else if(iType==2)
	{
		m_trackerType = TRACK_DEVICE_NDI_EM;
	} 
}

CTrackerManager::~CTrackerManager()
{
	if(m_pTrackerHandler)
	{
		delete m_pTrackerHandler;
	}
}

CString CTrackerManager::GetStartupPath()
{
	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	
	CString m_strStylesPath = szStylesPath;
	int nIndex  = m_strStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		m_strStylesPath = m_strStylesPath.Left(nIndex);
	}
	else {
		m_strStylesPath.Empty();
	}
	m_strStylesPath += _T("\\");

	CString sProgramPath;
	sProgramPath.Format("%s",m_strStylesPath);

	return sProgramPath;
}


CTrackerHandler* CTrackerManager::GetHandler()
{
	if(m_pTrackerHandler)
		return m_pTrackerHandler;
	
	switch(m_trackerType)
	{
	case TRACK_DEVICE_NDI_EM:
		{
			m_pTrackerHandler=new CNDIEMHandler();
			break;
		}
	case TRACK_DEVICE_NDI_OPTIC:
		{
			m_pTrackerHandler=new CNDIOpticHandler();
			break;
		}
	default:
		{
			m_pTrackerHandler=new CNDIOpticHandler();
			break;
		}
	}
	return m_pTrackerHandler;
}
