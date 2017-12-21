// EMHandler.cpp: implementation of the CEMHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "trackerHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackerHandler::CTrackerHandler()
{
	m_iToolCount=0;
}

CTrackerHandler::~CTrackerHandler()
{

}

BOOL CTrackerHandler::RefreshMatrix()
{
	return TRUE;
}

BOOL CTrackerHandler::InitNDI()
{
	return TRUE;
}

int CTrackerHandler::GetToolCount()
{
	return m_iToolCount;
}

BOOL CTrackerHandler::GetMatrix(int iToolIndex, Matrix &m)
{
	return FALSE;
}

BOOL CTrackerHandler::IsValidDevice()
{
	return FALSE;
}

BOOL CTrackerHandler::StartTracking()
{
	return TRUE;
}

BOOL CTrackerHandler::StopTracking()
{
	return TRUE;
}

CString CTrackerHandler::GetErrorInfo()
{
	return m_sErrorInfo;
}

BOOL CTrackerHandler::IsValidTool(int index)
{
	return FALSE;
}

int CTrackerHandler::GetDeviceType()
{
	return 0;
}