// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include <afxdllx.h>
#include "configmgr.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char
	pszStringValue[256];
char
	pszMainFileName[MAX_PATH]; //文件全路径
char
	pszActFileName[MAX_PATH];

void GetINIFileName(LPCTSTR pFileName);


/*
static AFX_EXTENSION_MODULE ConfigMgrDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CONFIGMGR.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ConfigMgrDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(ConfigMgrDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CONFIGMGR.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ConfigMgrDLL);
	}
	return 1;   // ok
}
*/
CConfigMgr::CConfigMgr()
{
}

////////////////////////////////////////////////////////////////////////////////
// Write()
// Description: Helper to write an integer data
//
// Return: void 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    int nValue
////////////////////////////////////////////////////////////////////////////////
void CConfigMgr::Write(LPCTSTR pszSession, LPCTSTR pszEntry, int nValue, LPCTSTR pszFileName /* = NULL*/ )
{
//	CWinApp* pApp = AfxGetApp();
//	int status;
//	status = pApp->WriteProfileInt(pszSession, pszEntry, nValue);

	GetINIFileName(pszFileName);
	sprintf( pszStringValue, "%d", nValue );
	WritePrivateProfileString(pszSession, pszEntry, pszStringValue, pszActFileName);
}

////////////////////////////////////////////////////////////////////////////////
// Write()
// Description: Helper to write a float data
//
// Return: void 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    float fValue
////////////////////////////////////////////////////////////////////////////////
void CConfigMgr::Write(LPCTSTR pszSession, LPCTSTR pszEntry, float fValue, LPCTSTR pszFileName /* = NULL*/ )
{
//	CString szValue;
//	szValue.Format(_T("%0.7f"), fValue);
//	Write(pszSession, pszEntry, szValue);

	GetINIFileName(pszFileName);
	sprintf( pszStringValue, "%0.7f", fValue );
	WritePrivateProfileString(pszSession, pszEntry, pszStringValue, pszActFileName);
}

////////////////////////////////////////////////////////////////////////////////
// Write()
// Description: Helper to write a double data
//
// Return: void 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    double dbValue
////////////////////////////////////////////////////////////////////////////////
void CConfigMgr::Write(LPCTSTR pszSession, LPCTSTR pszEntry, double dbValue, LPCTSTR pszFileName /* = NULL*/ )
{
//	CString szValue;
//	szValue.Format(_T("%0.7f"), dbValue);
//	Write(pszSession, pszEntry, szValue);

	GetINIFileName(pszFileName);
	sprintf( pszStringValue, "%0.7f", dbValue );
	WritePrivateProfileString(pszSession, pszEntry, pszStringValue, pszActFileName);
}


////////////////////////////////////////////////////////////////////////////////
// Write()
// Description: Helper to write a string data
//
// Return: void 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    LPCTSTR pszValue 
////////////////////////////////////////////////////////////////////////////////
void CConfigMgr::Write(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszValue, LPCTSTR pszFileName /* = NULL*/ )
{
//	CWinApp* pApp = AfxGetApp();
//	int status;
//	status = pApp->WriteProfileString(pszSession, pszEntry, pszValue);

	GetINIFileName(pszFileName);
	WritePrivateProfileString(pszSession, pszEntry, pszValue, pszActFileName);
}
 

////////////////////////////////////////////////////////////////////////////////
// Read()
// Description: Helper to read int data
//
// Return: int 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    int nDefault
////////////////////////////////////////////////////////////////////////////////
int CConfigMgr::Read(LPCTSTR pszSession, LPCTSTR pszEntry, int nDefault, LPCTSTR pszFileName /* = NULL*/ )
{
//	CWinApp* pApp = AfxGetApp();
//	return (int)pApp->GetProfileInt(pszSession, pszEntry, nDefault);

	CString szValue;
	szValue.Format(_T("%d"), nDefault);
	char pszValue[256];
	GetINIFileName(pszFileName);
	GetPrivateProfileString( pszSession, pszEntry, szValue, pszValue, 
							 sizeof(pszValue), pszActFileName);
	return atoi(pszValue);
}

////////////////////////////////////////////////////////////////////////////////
// Read()
// Description: Helper to read float data
//
// Return: float 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    int nDefault
////////////////////////////////////////////////////////////////////////////////
float CConfigMgr::Read(LPCTSTR pszSession, LPCTSTR pszEntry, float fDefault, LPCTSTR pszFileName /* = NULL*/ )
{
	CString szValue;
	szValue.Format(_T("%0.7f"), fDefault);
//	szValue = Read(pszSession, pszEntry, szValue);
//	return (float)atof(szValue);

	GetINIFileName(pszFileName);
	char pszValue[256];
	GetPrivateProfileString( pszSession, pszEntry, szValue, pszValue, 
							 sizeof(pszValue), pszActFileName);
	return (float)atof(pszValue);
}

////////////////////////////////////////////////////////////////////////////////
// Read()
// Description: Helper to read a string data
//
// Return: CString 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    LPCTSTR pszDefault
////////////////////////////////////////////////////////////////////////////////
CString CConfigMgr::Read(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault, LPCTSTR pszFileName /* = NULL*/ )
{
//	CWinApp* pApp = AfxGetApp();
//	return pApp->GetProfileString(pszSession, pszEntry, pszDefault);
	GetINIFileName(pszFileName);
	char pszValue[256];
	GetPrivateProfileString( pszSession, pszEntry, pszDefault, pszValue, 
							 sizeof(pszValue), pszActFileName);
	return pszValue;
}

////////////////////////////////////////////////////////////////////////////////
// Read()
// Description: Helper to read double data
//
// Return: double 
// Parameters: 
//    LPCTSTR pszSession
//    LPCTSTR pszEntry
//    double nDefault
////////////////////////////////////////////////////////////////////////////////
double CConfigMgr::Read(LPCTSTR pszSession, LPCTSTR pszEntry, double dbDefault, LPCTSTR pszFileName /* = NULL*/ )
{
	CString szValue;
	szValue.Format(_T("%0.7f"), dbDefault);
//	szValue = Read(pszSession, pszEntry, szValue);
//	return (double)atof(szValue);

	GetINIFileName(pszFileName);
	char pszValue[256];
	GetPrivateProfileString( pszSession, pszEntry, szValue, pszValue, 
							 sizeof(pszValue), pszActFileName);
	return (double)atof(pszValue);
}

CString CConfigMgr::ReadREG(HKEY hKey, LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault /*=NULL*/)
{
	HKEY hRetKey;
	
	if (RegOpenKeyEx(hKey,  _T(pszSession),
                     0, KEY_QUERY_VALUE, &hRetKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hRetKey, _T(pszEntry), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
			RegCloseKey(hRetKey);
			return szData;
	    }
		else
		{
			RegCloseKey(hRetKey);
			return pszDefault;
		}
    }
	else
	{
		return pszDefault;
	}
}


void SetINIFileName(char* pFileName)//设置INI文件名,路径为当前或Windows目录
{
	if(pFileName==NULL) return ;
	strcpy(pszMainFileName,pFileName);
}

void GetINIFileName(LPCTSTR pFileName)
{
	if(pFileName==NULL)
		strcpy(pszActFileName, pszMainFileName);
	else
		strcpy(pszActFileName, pFileName);
}

