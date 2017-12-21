// DCMINT.cpp : Defines the entry point for the DLL application.
//

#include "Global.h"
#include "dcmtk.h"

extern MR_STRUCT_IMAGE *g_MRImgData;
extern CT_STRUCT_IMAGE *g_CTImgData;

//服务线程	
void _cdecl MainSVRThread(LPVOID Param)
{
	FCBI *pCBI = (FCBI*)Param;
	opt_port = atoi(pCBI->LocalPort);

	T_ASC_Network *net;
	DcmAssociationConfiguration asccfg;

#ifdef HAVE_WINSOCK_H
	WSAData winSockData;
	/* we need at least version 1.1 */
	WORD winSockVersionNeeded = MAKEWORD( 1, 1 );
	WSAStartup(winSockVersionNeeded, &winSockData);
#endif

beginsvrthread:
	/* initialize network, i.e. create an instance of T_ASC_Network*. */
	OFCondition cond = ASC_initializeNetwork(NET_ACCEPTOR, OFstatic_cast(int, opt_port), opt_acse_timeout, &net);
	if (cond.bad())
	{
		::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,STORAGE_FAULT_INITIALIZENETWORK,NULL);
		bTerminated = TRUE;		
		
		DimseCondition::dump(cond);
		goto beginsvrthread;		
	}


	::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,STORAGE_SERVICE_START,NULL);



	while(cond.good())
	{
		/* receive an association and acknowledge or reject it. If the association was */
		/* acknowledged, offer corresponding services and invoke one or more if required. */
		cond = acceptAssociation(pCBI,net, asccfg);
		
		/* remove zombie child processes */
		cleanChildren(-1, OFFalse);
#ifdef WITH_OPENSSL
		/* since storescp is usually terminated with SIGTERM or the like,
		* we write back an updated random seed after every association handled.
		*/
		if (tLayer && opt_writeSeedFile)
		{
			if (tLayer->canWriteRandomSeed())
			{
				if (!tLayer->writeRandomSeed(opt_writeSeedFile))
				{
					CERR << "Error while writing random seed file '" << opt_writeSeedFile << "', ignoring." << endl;
				}
			}
			else
			{
				CERR << "Warning: cannot write random seed, ignoring." << endl;
			}
		}
#endif
		// if running in inetd mode, we always terminate after one association
		if (dcmExternalSocketHandle.get() >= 0) break/*goto RetryStopService*/;
		
		// if running in multi-process mode, always terminate child after one association
		if (DUL_processIsForkedChild()) break/*goto RetryStopService*/;
		
	}

	/* drop the network, i.e. free memory of T_ASC_Network* structure. This call */
	/* is the counterpart of ASC_initializeNetwork(...) which was called above. */
	cond = ASC_dropNetwork(&net);
	if (cond.bad())
	{
		DimseCondition::dump(cond);
		return;
	}

#ifdef HAVE_WINSOCK_H
	WSACleanup();
#endif

#ifdef WITH_OPENSSL
	delete tLayer;
#endif	

	if(g_MRImgData)
	{
		delete g_MRImgData->pData;
		delete g_MRImgData;
		g_MRImgData = NULL;
	}

	if(g_CTImgData)
	{
		delete g_CTImgData->pData;
		delete g_CTImgData;
		g_CTImgData = NULL;
	}

	bTerminated = TRUE;
	_endthread();

}


//开始DICOM服务进程，包括DICOM文件接收功能，Q/R服务，Verify服务
//如果不需要回调函数，可以传NULL进去
HANDLE BeginServer(BYTE *pCbi,SVRProc CBProc,LPVOID pWnd,HANDLE hwnd)
{
	FCBI* pCBI=(FCBI*)pCbi;
	
	pCBI->pDlgDICOMSrv    = pWnd;
	pCBI->hwnd            = hwnd ;
	pCBI->bToBeTerm       = FALSE;
	bTerminated			  = FALSE;
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
BOOL TermServer(HANDLE Handle)
{
	if(Handle == NULL)
	{
		return TRUE;
	}
	FCBI* pCBI = (FCBI*)Handle;
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

void SetSvrAET(char* szAETSource)
{
	strcpy(szAET,szAETSource);
}

void SetSvrPort(char* szPortSource)
{
	strcpy(szPort,szPortSource);
}


void SetSvrRootDir(char* szPortSource)
{
	strcpy(szRootDir,szPortSource);
}

