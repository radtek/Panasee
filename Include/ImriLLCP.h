// ImriLLCP.h: interface for the CImriLLCP class.
//
//////////////////////////////////////////////////////////////////////
/********************     error code   ********************************
      0 :   No error.
	  -1:   Not install the correct libary of SOCKET 2.0.
	  -2:   Failure to create SOCKET.
	  -3:   Failure to Bind the socket.
	  -4:   Connect server failed.

      -11:  Failed to accept connect because
	        the socket has been connected to server.
	  -12:  Failure to accept.
	  -13:  The server refuse to connect .

      -21:  failed to send data.
      -22:  failed to receive data.

      -31:  malloc buffer failed.

      -51:  Communication object has not been created.
	  -61:  DataPacket is too big .

      -101 : PackingULDataToSend Method not supports the Version.
	  -102 : illegal packet.
***********************************************************************/

#if !defined(AFX_IMRILLCP_H__375E8D90_56AF_4F23_B5A5_71ECC570D71C__INCLUDED_)
#define AFX_IMRILLCP_H__375E8D90_56AF_4F23_B5A5_71ECC570D71C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum ENUM_OF_SOCK_TYPE
{
	SOCK_TYPE_CLIENT = 0,
	SOCK_TYPE_SERVER = 1,
	SOCK_TYPE_UNKOWN = 2,
};

typedef int (*PFN_ULCPPaser) (char* lpULPack, int &nPackLen);

class CNetMsgDlg;

class AFX_EXT_CLASS CImriLLCP  
{
public:
	int  SendImriULData(char *lpDataBuf, int nDataLen);
	int  ConnectImriServer(char* szIPAddr,short sPort, HWND hMsgWnd, PFN_ULCPPaser pfnULParser);
	void DisconnectImriServer();
	int  StartImriServer(short sPort, HWND hMsgWnd, PFN_ULCPPaser pfnULParser);
	void StopImriServer();
	void GetVersion(char szVer[]);
	void SetAcceptIP(char* szIP);
	int  GetNegoPackLength();
	int  GetNegoLLCPVersion();
	CImriLLCP();
	virtual ~CImriLLCP();
protected:
    void*  m_pLLCPObj ;
	char   m_szVer[20];
};

#endif // !defined(AFX_IMRILLCP_H__375E8D90_56AF_4F23_B5A5_71ECC570D71C__INCLUDED_)
