// DlgImriImpl.cpp : implementation file
//

#include "stdafx.h"
#include "DlgImriImpl.h"
#include "ImriUlSrv.h"
#include "ImriDefine.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImriImpl dialog

static CDlgImriImpl	s_DlgImriImpl;

int ULServerParser(char* lpULPack, int &nPackLen);


CDlgImriImpl::CDlgImriImpl(CWnd* pParent /*=NULL*/)
: CDialog(CDlgImriImpl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImriImpl)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_wLoVer         = 3;
	m_wHiVer         = 2 ;
	m_wLoVerUsed     = m_wLoVer ;
	m_wHiVerUsed     = m_wHiVer ;
	
	m_nClientCnt	 = 0;
	
    m_pvImriUlServerApp = NULL ;	
	m_wndMsgHandle		= NULL;
	m_bSrvStarted		= FALSE;
	m_bSrvConnected		= FALSE;
	m_bAsServer			= TRUE;

}

CDlgImriImpl::~CDlgImriImpl()
{
	
}

void CDlgImriImpl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImriImpl)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImriImpl, CDialog)
//{{AFX_MSG_MAP(CDlgImriImpl)
// NOTE: the ClassWizard will add message map macros here
ON_MESSAGE(WM_IMRINET_MSG, OnImriNetMsg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImriImpl message handlers

void CDlgImriImpl::OnImriNetMsg(UINT wParam,LONG lParam)
{
	CString sMsg;
	switch (wParam)
	{
	case 0:
		{
			switch (lParam)  //Process error message.
			{
			case -1:
				break;
			case -2:
				break;
			default:
				break;
			}
		}
		break;
	case 10:     //peer socket has closed.
		sMsg = "peer socket has closed! " ;
		if (m_bAsServer && m_nClientCnt > 0)
		{
			m_nClientCnt --;
		}
		if (!m_bAsServer && m_bSrvConnected)
		{
			m_bSrvConnected = FALSE;
		}
		if (m_wndMsgHandle)
		{
			::PostMessage(m_wndMsgHandle, WM_IMRIUL_MSG, 10, 1);
		}
		break;
	case 20:    //A user has connected to server.
		sMsg = "A user has connected to server! " ;
		if (m_bAsServer)
		{
			m_nClientCnt ++;
		}
		if (m_wndMsgHandle)
		{
			::PostMessage(m_wndMsgHandle, WM_IMRIUL_MSG, 20, 1);
		}
		break ;
	}
	TRACE(sMsg);
}

CDlgImriImpl* CDlgImriImpl::GetHandle()			// Get Dlg handle
{
	return &s_DlgImriImpl;
}

WORD CDlgImriImpl::GetIMRIACPHiVer()
{
	return m_wHiVerUsed;
}

WORD CDlgImriImpl::GetIMRIACPLoVer()
{
	return m_wLoVerUsed;
}

int CDlgImriImpl::StartServer(short sPort, HWND hMsgWnd)
{
	if (!m_bAsServer && m_bSrvConnected)
	{
		DisconnectServer();
	}
	
	int nRetVal = 0 ;
	if (!m_bSrvStarted)
	{
		m_bAsServer = TRUE;

		
		nRetVal = m_llcpCommObj.StartImriServer(sPort, m_hWnd, ULServerParser);
		if (nRetVal == 0)
		{
			m_nClientCnt = 0;
			m_bSrvStarted = TRUE;
		}
		m_wndMsgHandle = hMsgWnd;
		
	}
	return nRetVal ;
}

void CDlgImriImpl::StopServer()
{
	m_llcpCommObj.StopImriServer();
	m_nClientCnt = 0;
	m_bSrvStarted = FALSE;
}

int CDlgImriImpl::ConnectServer(short nPort, char* cIPAddr, HWND hMsgWnd)
{
	if (m_bAsServer && m_bSrvStarted)
	{
		StopServer();
	}
	
	int nRetVal = 0;
	if (!m_bSrvConnected)
	{
		
		m_bAsServer = FALSE;

		
		nRetVal = m_llcpCommObj.ConnectImriServer(cIPAddr, nPort, m_hWnd, ULServerParser);
		if (nRetVal == 0)
		{
			m_bSrvConnected = TRUE; 
			SendULPack(PACKTYPE_NEGOTIATE, NULL, 0);
		}
		m_wndMsgHandle = hMsgWnd;
		
	}
	return nRetVal;
}

int CDlgImriImpl::DisconnectServer()
{
	if (m_bSrvConnected)
	{
		m_llcpCommObj.DisconnectImriServer();
		m_bSrvConnected = FALSE;
	}
	return 0;
}

BOOL CDlgImriImpl::IsMRIConnected()
{
	return m_bSrvStarted && (m_nClientCnt > 0);	
}

void CDlgImriImpl::SetAppImp(CVImriUlServerApp *pulInterfaceImp)
{
	m_pvImriUlServerApp = pulInterfaceImp;
}

int CDlgImriImpl::CheckIllegal(unsigned char* lpULPack, int nPackLen)
{
    WORD  wVer ;
	int   nLen , nModalityType;
	int   nCommand = 0 ;
	unsigned char *lpTemp = lpULPack ;
    //检查协议版本
	if (nPackLen == ULPACKHEADER_LEN ) //协议版本协商
	{
		lpTemp +=4;
	}else if( nPackLen > ULPACKHEADER_LEN )
	{
		memcpy(&wVer, lpTemp, 2);
		lpTemp += 2 ;
		if (wVer != m_wLoVerUsed)
		{
			return -1 ;
		}
		memcpy(&wVer, lpTemp, 2);
		lpTemp += 2 ;
		if (wVer != m_wHiVerUsed)
		{
			return -2 ;
		}
	}else
	{
		return -5;
	}
	
	//检查包长度
	memcpy(&nLen, lpTemp, 4);
	lpTemp += 4 ;
	nLen += 80 ;
	if (nLen != nPackLen)
	{
		return -3 ;
	}
	
	//检查设备类型
    lpTemp += 64 ;
	memcpy(&nModalityType, lpTemp, 4);
	lpTemp += 4 ;
	if (nModalityType != DEVICETYPE_MRI)   //MRI: 3
	{
		return -4 ;
	}
	
	memcpy(&nCommand, lpTemp, 4);
	return  nCommand ;
}

int CDlgImriImpl::SendULPack(ENUM_PACK_TYPE ePackType, unsigned char* pPackData, int nDataLen)
{
	int nRetVal = 0 ;
	
	IMRI_PACKHEADER *pPacket = (IMRI_PACKHEADER *)malloc(sizeof(IMRI_PACKHEADER) + nDataLen);
	if (pPacket)
	{
		pPacket->nVer          = m_wLoVerUsed + (m_wHiVerUsed << 16) ;
		pPacket->nPackDataLen  = nDataLen ;
		pPacket->nModalityType = 3 ;
		pPacket->nCommand      = ePackType ;
		if (nDataLen > 0 && pPackData)
		{
			memcpy(pPacket->cPackData, pPackData, nDataLen);
		}

		//Attention:
		//sizeof(IMRI_PACKHEADER)-4 == 80 . This is the size we want.
		nRetVal = m_llcpCommObj.SendImriULData((char*)pPacket, sizeof(IMRI_PACKHEADER)-4+nDataLen);

		delete pPacket;
	}
	return nRetVal;
}

int CDlgImriImpl::DealNegotiatePkg(unsigned char* lpULPack, int &nPackLen)
{
	// Negotiate protocol version
	IMRI_PACKHEADER* pPkg = (IMRI_PACKHEADER*)lpULPack;
	if (GET_VERSION_HI(pPkg->nVer) != m_wHiVer)
	{
	}
	else
	{
		m_wHiVerUsed = m_wHiVer;
		m_wLoVerUsed = min(GET_VERSION_LO(pPkg->nVer), m_wLoVer);
	}
	if(m_bAsServer)
	{
		return SendULPack(PACKTYPE_NEGOTIATE, NULL, 0);
	}else
	{
		return -1;
	}
}

int CDlgImriImpl::DealImagePkg(unsigned char* lpULPackData, int &nPackDataLen)
{
	if (m_pvImriUlServerApp)
	{
		return m_pvImriUlServerApp->Imri_DecodeImages(lpULPackData, nPackDataLen);
	}
	return -1;
}

int CDlgImriImpl::DealMessagePkg(unsigned char* lpULPackData, int &nPackDataLen)
{
	if (m_pvImriUlServerApp)
	{
		IMRI_MESSAGE* pPkgMsg = (IMRI_MESSAGE*)lpULPackData;
		unsigned char* sMsg = NULL;
		if (pPkgMsg->nMsgLength > 0)
		{
			sMsg = new unsigned char[pPkgMsg->nMsgLength + 1];
			memcpy(sMsg, pPkgMsg->cMsg, pPkgMsg->nMsgLength);
			sMsg[pPkgMsg->nMsgLength] = '\0';
		}
		switch (pPkgMsg->nMsgType)
		{
		case MSGTYPE_CONFIRM:
			m_pvImriUlServerApp->Imri_DealConfirmMsg(sMsg);
			break;
		case MSGTYPE_WARNING:
			m_pvImriUlServerApp->Imri_DealWarningMsg(sMsg);
			break;
		case MSGTYPE_ERROR:
			m_pvImriUlServerApp->Imri_DealErrorMsg(sMsg);
			break;
		default:
			break;
		}
		if (sMsg)
		{
			delete sMsg;
		}
	}
	return 0;
}

int CDlgImriImpl::DealStatusPkg(unsigned char* lpULPackData, int &nPackDataLen)
{
	if (m_pvImriUlServerApp)
	{
		IMRI_STATUS_HEADER* pPkgInfo = (IMRI_STATUS_HEADER*)lpULPackData;
		switch (pPkgInfo->nOperMode)
		{
		case INFOOPER_REQUEST:
			break;
		case INFOOPER_ANSWER:
			{
				switch (pPkgInfo->nSatusType)
				{
				case INFO_STATUS:
					{
						IMRI_STATUS_SYSTEM* pPkgStatus = (IMRI_STATUS_SYSTEM*)(pPkgInfo->cStatusData);
						ENUM_MRI_SYS_STATUS nSysStatus = (ENUM_MRI_SYS_STATUS)(pPkgStatus->nSysStatus);
						unsigned short nSeriesStatus = pPkgStatus->nSeriesStatus;
						int nOperResult = pPkgStatus->nOperResult;
						m_pvImriUlServerApp->Imri_DealStatus(nSysStatus, nSeriesStatus, nOperResult);
						break;
					}
				default:
					break;
				}
			}
		default:
			break;
		}
	}
	return 0;
}


int CDlgImriImpl::DealInfoPkg(unsigned char* lpULPackData, int &nPackDataLen)
{
	if (m_pvImriUlServerApp)
	{
		IMRI_INFOR_HEADER* pPkgInfo = (IMRI_INFOR_HEADER*)lpULPackData;
		switch (pPkgInfo->nOperMode)
		{
		case INFOOPER_REQUEST:
			break;
		case INFOOPER_ANSWER:
			{
				switch (pPkgInfo->nInfoType)
				{
				case INFO_SHIM_SNR:
					{
						IMRI_SHIM_SNR_TUNING_INFO* pPkgHeader = (IMRI_SHIM_SNR_TUNING_INFO*)(pPkgInfo->cInfoData);
						
						m_pvImriUlServerApp->Imri_DealShimSNR((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, pPkgHeader);
						break;
					}
				case INFO_PROTOCAL_FAMILY:
					{
						CList<IMRI_PROTOCOL_FAMILY, IMRI_PROTOCOL_FAMILY> lstprotocolFamily;
						if (pPkgInfo->nResult == IR_OK)
						{
							IMRI_INFO_PROTOCOLFAMILY_HEADER* pPkgHeader = (IMRI_INFO_PROTOCOLFAMILY_HEADER*)(pPkgInfo->cInfoData);
							int nFamilyNum = pPkgHeader->nProtocolFamilyNum;
							unsigned char* pTmp = (unsigned char*)pPkgHeader->cProtocolFamilyData;
							for (int i=0; i<nFamilyNum; i++)
							{
								IMRI_INFO_PROTOCOLFAMILY* pProFamily = (IMRI_INFO_PROTOCOLFAMILY*)pTmp;
								IMRI_PROTOCOL_FAMILY protocolFamily;
								protocolFamily.nID = pProFamily->protocolFamily.nProtocolGroupNo;
								memcpy((char*)protocolFamily.sDesc, pProFamily->protocolFamily.cProtocolGroupDesc, 64);
								lstprotocolFamily.AddTail(protocolFamily);
								pTmp+=sizeof(IMRI_INFO_PROTOCOLFAMILY);
							}
							
						}
						m_pvImriUlServerApp->Imri_DealProtocolFamily((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, lstprotocolFamily);
						break;
					}
				case INFO_PROTOCOLS:
					{
						CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*> lstprotocolGrp;
						if (pPkgInfo->nResult == IR_OK)
						{
							IMRI_INFO_PROTOCOLGROUP_HEADER* pPkgHeader = (IMRI_INFO_PROTOCOLGROUP_HEADER*)(pPkgInfo->cInfoData);
							int nGourpNum = pPkgHeader->nProtocolGroupNum;
							unsigned char* pTmp = (unsigned char*)pPkgHeader->cProtocolGroupData;
							for (int i=0; i<nGourpNum; i++)
							{
								IMRI_INFO_PROTOCOLGROUP* pProGrp = (IMRI_INFO_PROTOCOLGROUP*)pTmp;
								IMRI_PROTOCOL_GROUP *protocolGrp = new IMRI_PROTOCOL_GROUP;
								protocolGrp->nID = pProGrp->protocolGroup.nProtocolGroupNo;
								memcpy((char*)protocolGrp->sDesc, pProGrp->protocolGroup.cProtocolGroupDesc, 64);
								protocolGrp->nProtocolSum = pProGrp->nProtocolNum;
								protocolGrp->nProtocolType = m_wHiVer;
								//								protocolGrp->pProtocols = (void **)(new IMRI_PROTOCOL_DESC*[protocolGrp->nProtocolSum]);
								protocolGrp->pProtocols = (void **)(new IMRI_PROTOCOL*[protocolGrp->nProtocolSum]);
								
								pTmp = pProGrp->cProtocolData;
								for (int j=0; j<(int)(protocolGrp->nProtocolSum); j++)
								{
									IMRI_PROTOCOL_DESC* pPro = (IMRI_PROTOCOL_DESC*)pTmp;
									protocolGrp->pProtocols[j] = (void *)(new IMRI_PROTOCOL);
									//									memcpy(protocolGrp->pProtocols[j], pPro, sizeof(IMRI_PROTOCOL_DESC));
									memcpy(&(((IMRI_PROTOCOL*)(protocolGrp->pProtocols[j]))->oProtDesc), pPro, sizeof(IMRI_PROTOCOL_DESC));
									((IMRI_PROTOCOL*)(protocolGrp->pProtocols[j]))->pBelongGrp = protocolGrp;
									pTmp += sizeof(IMRI_PROTOCOL_DESC);
								}
								lstprotocolGrp.AddTail(protocolGrp);
							}
						}
						m_pvImriUlServerApp->Imri_DealProtocolGrps((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, lstprotocolGrp);
						
						
						break;
					}
				case INFO_MODALITY:
					{
						IMRI_MODALITY modality;
						if (pPkgInfo->nResult == IR_OK)
						{
							memcpy(&modality, &(pPkgInfo->cInfoData), sizeof(IMRI_MODALITY));
						}
						m_pvImriUlServerApp->Imri_DealModality((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, modality);
						break;
					}
				case INFO_PATIENT:
					{
						IMRI_PATIENT patient;
						if (pPkgInfo->nResult == IR_OK)
						{
							memcpy(&patient, &(pPkgInfo->cInfoData), sizeof(IMRI_PATIENT));
						}
						m_pvImriUlServerApp->Imri_DealPatient((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, patient);
						break;
					}
				case INFO_STUDY:
					{
						IMRI_STUDY study;
						if (pPkgInfo->nResult == IR_OK)
						{
							memcpy(&study, &(pPkgInfo->cInfoData), sizeof(IMRI_STUDY));
						}
						m_pvImriUlServerApp->Imri_DealStudy((enum ENUM_INFOR_RESULT)pPkgInfo->nResult, study);
						break;
					}
				default:
					{
						//
						break;
					}
				}
				break;
			}
		default:
			break;
		}
	}
	return 0;
}

int CDlgImriImpl::DealCriticalMsgPkg(unsigned char* lpULPackData, int &nPackDataLen)
{
	if (m_pvImriUlServerApp)
	{
		IMRI_CRITICAL_MSG* pPkgMsg = (IMRI_CRITICAL_MSG*)lpULPackData;
		unsigned char* sMsg = NULL;
		if (pPkgMsg->nMsgLength > 0)
		{
			sMsg = new unsigned char[pPkgMsg->nMsgLength + 1];
			memcpy(sMsg, pPkgMsg->cMsg, pPkgMsg->nMsgLength);
			sMsg[pPkgMsg->nMsgLength] = '\0';
		}
		
		// Do opeartion
		m_pvImriUlServerApp->Imri_DealCriticalMsg(sMsg);
		
		if (sMsg)
		{
			delete sMsg;
		}
	}
	return 0;
}

int ULServerParser(char* lpULPack, int &nPackLen)
{
	int nCommandCode = s_DlgImriImpl.CheckIllegal((unsigned char*)lpULPack,nPackLen) ;
	if ( nCommandCode < 0 )
	{
		return -1 ;				//Illegal pack
	}
	
	//	void *pRcvData = malloc(nPackLen);
	//	memcpy(pRcvData, (void*)lpULPack, nPackLen);
	IMRI_PACKHEADER* pPkg = (IMRI_PACKHEADER*)lpULPack;
	
	switch (nCommandCode)
	{
	case PACKTYPE_NEGOTIATE:				//协议协商
		return  s_DlgImriImpl.DealNegotiatePkg((unsigned char*)lpULPack, nPackLen);
		break ;
	case PACKTYPE_IMAGE:					//传送1个序列的图像
		return s_DlgImriImpl.DealImagePkg((unsigned char *)pPkg->cPackData, pPkg->nPackDataLen);
		break ;
	case PACKTYPE_LOCALIZATION:				//接收定位信息
		break ;
	case PACKTYPE_COMMAND:					//对MRI进行操作的命令
		break ;
	case PACKTYPE_MESSAGE:					//提示消息
		return s_DlgImriImpl.DealMessagePkg((unsigned char *)pPkg->cPackData, pPkg->nPackDataLen);
		break ;
	case PACKTYPE_STATUS:					// 状态
		return s_DlgImriImpl.DealStatusPkg((unsigned char *)pPkg->cPackData, pPkg->nPackDataLen);
		break;
	case PACKTYPE_INFOR:					//信息
		return s_DlgImriImpl.DealInfoPkg((unsigned char *)pPkg->cPackData, pPkg->nPackDataLen);
		break ;
	case PACKTYPE_CRITICAL_MSG:				//警告消息
		return s_DlgImriImpl.DealCriticalMsgPkg((unsigned char *)pPkg->cPackData, pPkg->nPackDataLen);
		break ;
	default:
		break ;
	}
	
	//	pPkg = NULL;
	//	free(pRcvData);
	
	return 0;
}

int CDlgImriImpl::SendCommand(ENUM_COMMANDS eCommand, void *pBuffer /* = NULL */, int nDataLen /* = 0 */)
{
	if (eCommand < COMMAND_CREATESERIES || eCommand > COMMAND_STARTTUNE)
		return 1;
	
	int nRetVal = 1;
	IMRI_COMMAND* pPkgCom = (IMRI_COMMAND*)malloc(sizeof(IMRI_COMMAND) + nDataLen);
	if (pPkgCom)
	{
		pPkgCom->nCmd = eCommand;
		if (pBuffer && nDataLen)
		{
			memcpy(pPkgCom->cCmdData, pBuffer, nDataLen);
		}
		nRetVal = SendULPack(PACKTYPE_COMMAND, (unsigned char*)pPkgCom, sizeof(IMRI_COMMAND)+nDataLen);
		delete pPkgCom;
	}
	return nRetVal;
}

int CDlgImriImpl::SendStatusRequest(ENUM_STATUS_TYPE eRequest)
{
	if (eRequest != INFO_STATUS )
		return 1;
	
	int nRetVal = 1;
	IMRI_STATUS_HEADER* pPkgReq = (IMRI_STATUS_HEADER*)malloc(sizeof(IMRI_STATUS_HEADER));
	if (pPkgReq)
	{
		pPkgReq->nOperMode = INFOOPER_REQUEST;
		pPkgReq->nSatusType = eRequest;
		nRetVal = SendULPack(PACKTYPE_STATUS, (unsigned char*)pPkgReq, sizeof(IMRI_STATUS_HEADER));
		delete pPkgReq;
	}
	return nRetVal;
}

int CDlgImriImpl::SendInfoRequest(ENUM_INFOR_TYPE eRequest)
{
	if (eRequest < INFO_PROTOCOLS || eRequest > INFO_SHIM_SNR)
		return 1;
	
	int nRetVal = 1;
	IMRI_INFOR_HEADER* pPkgReq = (IMRI_INFOR_HEADER*)malloc(sizeof(IMRI_INFOR_HEADER));
	if (pPkgReq)
	{
		pPkgReq->nOperMode = INFOOPER_REQUEST;
		pPkgReq->nInfoType = eRequest;
		pPkgReq->nResult = 0;
		nRetVal = SendULPack(PACKTYPE_INFOR, (unsigned char*)pPkgReq, sizeof(IMRI_INFOR_HEADER));
		delete pPkgReq;
	}
	return nRetVal;
}

int	CDlgImriImpl::SendLocalization(CList<IMRI_LOCALIZATION*, IMRI_LOCALIZATION*>& lststcrLocalization)
{
	int nCnt = lststcrLocalization.GetCount();
	if (nCnt <= 0)
		return 1;
	
	int nRetVal = 1;
	int LocaDataSize = sizeof(IMRI_LOCALIZATION);
	IMRI_LOCALIZATION_HEADER* pPkgLoc = (IMRI_LOCALIZATION_HEADER*)malloc(sizeof(IMRI_LOCALIZATION_HEADER) + nCnt * LocaDataSize);
	if (pPkgLoc)
	{
		pPkgLoc->nSlabNum = nCnt;
		POSITION pos = lststcrLocalization.GetHeadPosition();
		unsigned char* pTmp = pPkgLoc->cSlabData;
		for (;pos;)
		{
			IMRI_LOCALIZATION* pPkgLocData = (IMRI_LOCALIZATION*)pTmp;
			IMRI_LOCALIZATION* pData = lststcrLocalization.GetNext(pos);
			memcpy(pPkgLocData, pData, LocaDataSize);
			pTmp += LocaDataSize;
		}
		nRetVal = SendULPack(PACKTYPE_LOCALIZATION, (unsigned char*)pPkgLoc, sizeof(IMRI_LOCALIZATION_HEADER) + nCnt * LocaDataSize);
		delete pPkgLoc;
	}
	return nRetVal;
}

BOOL CDlgImriImpl::CheckOperResult(int nOperResult)
{
	if (nOperResult == 0)
		return TRUE;
	
	return FALSE;
}
