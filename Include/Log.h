// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__0AB856F0_5587_4A3C_81D3_80485B580BD7__INCLUDED_)
#define AFX_LOG_H__0AB856F0_5587_4A3C_81D3_80485B580BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class __declspec(dllexport) CLog  
{
public:
	CLog();
	virtual ~CLog();

	static int StartLog(CString sPathFile);
	static int Log(CString sLog);
	static int Log(char *pLog);
	static int StopLog();
	
	static CString GetErrorDesc(int iErrCode);
};

#endif // !defined(AFX_LOG_H__0AB856F0_5587_4A3C_81D3_80485B580BD7__INCLUDED_)
