// DCMINT.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
//#include "io.h"
#include "DCMINT.H"
#include "DICOM.hpp"
#include "dicomdir.h"
#include "iod.hpp"
//#include "comp.hpp"
#include "QueryInformation.h"
//#include "MyQueryRetrieve.h"
#include "DcmMessage.h"
#include <mmsystem.h>
#include <process.h>

char szAET[16]="COOLPACS";
char szPort[16]="104";
char szRootDir[256]="d:\\dicom\\imagedir\\";

BOOL bTerminated=TRUE;

//DICOM服务

void _cdecl AcceptSVR(LPVOID Param)
{
	CBI *pCBI       = (CBI*)Param;
	int m_nSocketfd = pCBI->Sockfd;
	int nRcvFlag    = 0 ;
	pCBI->nPackLen=0;
	
	try
	{
		SVRCBProc           cbp = pCBI->cbProc;
		DICOMCommandObject	DCO;
		
		DICOMDataObject     DDO;
		PDU_Service         PDU;
		MRStorage           SOPMRStorage;
		CTStorage           SOPCTStorage;
		Verification        SOPVerification;
		
		MyPatientRootQuery    PRQuery;
		MyPatientRootRetrieve PRRetrieve;
		MyStudyRootQuery      SRQuery;
		MyStudyRootRetrieve   SRRetrieve;
		
				
		try
		{
			if(!PDU.Multiplex (m_nSocketfd))
			{
				_endthread();
			}
		}
		catch(...)
		{
			nRcvFlag = STORAGE_FAULT_MULTIPLEX;
			::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
			_endthread();
		}
		
		while(TRUE)
		{
			try
			{
				DCO.Reset();
				if (!PDU.Read(&DCO))
					break;
			}
			catch(...)
			{
				nRcvFlag = STORAGE_FAULT_PDUREAD;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
				
				_endthread();
			}
			
			try
			{
				
			}
			catch(...)
			{
				nRcvFlag = STORAGE_FAULT_NEW;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
				
				_endthread();
			}
			
			try
			{			
				if(SOPMRStorage.Read(&PDU, &DCO, &DDO))
				{
					
					if(!StandardSave(&PDU,&DDO,pCBI))
					{
						//Send a message that show a error happens
						nRcvFlag = STORAGE_FAULT_MR_SAVE;
						::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
						
					}
					else
					{
						nRcvFlag = STORAGE_RECEIVE_MRSERIES;
						//Send a message that show received image number
						::SendMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
					}
					
					
					DDO.Reset();
					
					continue;
				}
				else if(SOPCTStorage.Read(&PDU, &DCO, &DDO))
				{
													
					if(!StandardSave(&PDU,&DDO,pCBI))
					{
						nRcvFlag = STORAGE_FAULT_CT_SAVE;
						::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
						
					}
					else
					{
						nRcvFlag = STORAGE_RECEIVE_CTSERIES;
						//Send a message that show received image number
						::SendMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
					}
					
					DDO.Reset();
					
					continue;
				}
				else if(PRQuery.Read(&PDU, &DCO))
				{
					continue;
				}
				else if(SRQuery.Read(&PDU, &DCO))
				{
					continue;
				}
				else if(PRRetrieve.Read(&PDU, &DCO))
				{
					continue;
				}
				else if(SRRetrieve.Read(&PDU, &DCO))
				{
					continue;
				}
				else if (SOPVerification.Read(&PDU, &DCO))
				{
					continue;
				}
			}
			catch(...)
			{
				nRcvFlag = STORAGE_FAULT_STORAGEREAD;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
				
								
				PDU.Close();  //added by nsj
				
				_endthread();
			}

			try
			{
				
				PDU.Close();
			}
			catch(...)
			{
				nRcvFlag = STORAGE_FAULT_DELETECLOSE;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
			}
			
			
			_endthread();
		}

		try
		{
			DCO.Reset();
			
			PDU.Close();
		}
		catch(...)
		{
			nRcvFlag = STORAGE_FAULT_RESETDELETE;
			::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
		}
	

	}
	
	catch(...)
	{
		char strTmp[1000];
		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf(strTmp,"接收文件出错:%04d-%02d-%02d %02d:%02d:%02d 来源:%s",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,pCBI->strIPOrServer);
		
	}
		
	return;
}
//主服务线程	
void _cdecl MainSVRThread(LPVOID Param)
{
	Socket           m_MSocket;
	CBI*             pCBI = (CBI*)Param;
	fd_set           readfds;
	struct timeval   timeout;
	unsigned long    itmp;
	char             strTmp[MAX_PATH];
	int nRcvFlag=0;
	timeout.tv_usec = 500;
	timeout.tv_sec  = 0;
	
beginsvrthread:
	if (!m_MSocket.Listen(pCBI->LocalPort))
	{
		m_MSocket.Close();
		nRcvFlag = STORAGE_FAULT_LISTEN;
		::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
		bTerminated = TRUE;
		
		_endthread();
	}
	
	nRcvFlag = STORAGE_SERVICE_START;
	::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);

	try
	{
		while (TRUE)
		{
			
			itmp=1;
			itmp=ioctlsocket(m_MSocket.ListenSocketfd,FIONBIO,&itmp);
			if(itmp==SOCKET_ERROR)
			{
				m_MSocket.Close();
				sprintf(strTmp,"Socket Error:%x,重新启动服务", WSAGetLastError());
				nRcvFlag = STORAGE_FAULT_RESTARTLISTEN;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);

				goto beginsvrthread;
			}
			FD_ZERO(&readfds);
			FD_SET(m_MSocket.ListenSocketfd,&readfds);
			itmp=select(1,&readfds,0,0,&timeout);
			switch(itmp)
			{
			case 0://timeout
				if(pCBI->bToBeTerm)
				{
					m_MSocket.Close();
					nRcvFlag = STORAGE_SERVICE_STOP;
					::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
					bTerminated = TRUE;
					_endthread();
				}
				break;
			case SOCKET_ERROR://error
				itmp=GetLastError();
				m_MSocket.Close();
				sprintf(strTmp,"Socket Error:%x", WSAGetLastError());
				nRcvFlag = STORAGE_FAULT_RESTARTLISTEN;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
				goto beginsvrthread;
			default:
				if(!FD_ISSET(m_MSocket.ListenSocketfd,&readfds))break;
				itmp=0;
				ioctlsocket(m_MSocket.ListenSocketfd,FIONBIO,&itmp);
				m_MSocket.Accept();
				pCBI->Sockfd=m_MSocket.Socketfd;
				strcpy(pCBI->strIPOrServer,inet_ntoa(m_MSocket.sa.sin_addr));
				unsigned long iHandle=_beginthread(AcceptSVR,0,Param);
				SetThreadPriority((HANDLE)iHandle,THREAD_PRIORITY_LOWEST);
				m_MSocket.Socketfd = 0;
				m_MSocket.Connected = FALSE;
				break;
			}
		}
	}
	catch(...)
	{
		nRcvFlag = STORAGE_FAULT_MAINTHREAD;
		::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
		try
		{
			m_MSocket.Close();
		}
		catch(...)
		{
			nRcvFlag = STORAGE_FAULT_CLOSEMAINSOCKET;
			::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
		}
		
		nRcvFlag = STORAGE_FAULT_RESTARTLISTEN;
		::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,nRcvFlag,NULL);
		goto beginsvrthread;
	}
	m_MSocket.Close();
	
	
	bTerminated = TRUE;
	_endthread();
}


//开始DICOM服务进程，包括DICOM文件接收功能，Q/R服务，Verify服务
//如果不需要回调函数，可以传NULL进去
HANDLE BeginSVRSCP(BYTE *pCbi,SVRCBProc CBProc,HANDLE hwnd,BYTE* lpBuff)
{
	CBI* pCBI=(CBI*)pCbi;
    pCBI->lpDcmSeries = lpBuff;
	pCBI->nPackLen    = 0 ;
	if (!pCBI->lpDcmSeries)
	{
		return NULL ;
	}
	
	pCBI->hwnd            = hwnd ;
	pCBI->bToBeTerm       = FALSE;
	bTerminated     = FALSE;
	pCBI->cbProc          = CBProc;
	
		
	strcpy(pCBI->RootPath,szRootDir);
	strcpy(pCBI->LocalAET,  szAET);
	strcpy(pCBI->LocalPort, szPort);
	
	unsigned long iHandle=_beginthread(MainSVRThread,0,(LPVOID)pCBI);
	if(iHandle==-1)
	{
		return NULL;
	}
	
	BOOL bReturn=SetThreadPriority((HANDLE)iHandle,THREAD_PRIORITY_LOWEST);
	return (HANDLE)pCBI;
}

//结束服务
BOOL TermSVRSCP(HANDLE Handle)
{
	if(Handle == NULL)
	{
		return TRUE;
	}
	CBI* pCBI = (CBI*)Handle;
	if(bTerminated)
		return TRUE;

	pCBI->bToBeTerm = TRUE;
	
	while(bTerminated!=TRUE)
	{
		Sleep(2000);
	}
	
	Sleep(2000);  //等待两秒，确保所有的子线程也都退出。
	
	return TRUE;
}

void SetAET(char* szAETSource)
{
	strcpy(szAET,szAETSource);
}

void SetPort(char* szPortSource)
{
	strcpy(szPort,szPortSource);
}


void SetRootDir(char* szPortSource)
{
	strcpy(szRootDir,szPortSource);
}

