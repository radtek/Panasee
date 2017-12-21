// ImriUlSrv.h: interface for the CImriUlSrv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMRIULSRV_H__F0BF8679_E6E1_4656_84F9_9B3586C592AB__INCLUDED_)
#define AFX_IMRIULSRV_H__F0BF8679_E6E1_4656_84F9_9B3586C592AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VIMRIulServerApp.h"



class AFX_EXT_CLASS CImriUlSrv  
{
public:
	CImriUlSrv();
	virtual ~CImriUlSrv();

	static CImriUlSrv* GetHandle();			// Get server handle

	WORD	GetIMRIACPHiVer();
	WORD	GetIMRIACPLoVer();

	void    AddClientIP(char* szIP);

	int		StartServer(short sPort, HWND hMsgWnd);
	void	StopServer();
	void	SetAppImp(CVImriUlServerApp *pulInterfaceImp); 

	int		ConnectServer(char* szIP, short sPort, HWND hMsgWnd);
	int		DisconnectServer();

	BOOL	IsMRIClientConnected();

	BOOL    CheckOperResult(int nOperResult);

	int		SendInfoRequest(ENUM_INFOR_TYPE eRequest);
	int		SendStatusRequest(ENUM_STATUS_TYPE eRequest);
	int		SendCommand(ENUM_COMMANDS eCommand, void *pBuffer = NULL, int nDataLen = 0);
	int		SendLocalization(CList<IMRI_LOCALIZATION*, IMRI_LOCALIZATION*>& lststcrLocalization);
private:

};

#endif // !defined(AFX_IMRIULSRV_H__F0BF8679_E6E1_4656_84F9_9B3586C592AB__INCLUDED_)
