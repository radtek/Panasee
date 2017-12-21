
#include "StdAfx.h"
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TCPCOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TCPCOM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef TCPCOM_EXPORTS
#define TCPCOM_API __declspec(dllexport)
#else
#define TCPCOM_API __declspec(dllimport)
#endif

// This class is exported from the TCPCom.dll
class TCPCOM_API CTCPCom {
public:
	CTCPCom(void);
	// TODO: add your methods here.
};

//definition Export Functions
TCPCOM_API int StartListen(short nPort, HWND hWnd);
TCPCOM_API int ConnectServer(short nPort, char* cIPAddr, HWND hWnd);
TCPCOM_API int AcceptConn(HWND hWnd);
TCPCOM_API int SendData(char* pBuff, DWORD dwLen, bool bClient);
TCPCOM_API int RecvData(char* pBuff, DWORD dwLen, bool bClient);

