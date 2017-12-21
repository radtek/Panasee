// ImriUlSrv.cpp: implementation of the CImriUlSrv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImriUlSrv.h"
#include "DlgImriImpl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CImriUlSrv  s_ulSrvObj ;

CDlgImriImpl*	m_pDlgImriImpl;


//int ULServerPaser(char* lpULPack, int &nPackLen)
//{
//	return m_DlgImriImpl.ULServerPaser((unsigned char *)lpULPack, nPackLen);
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImriUlSrv::CImriUlSrv()
{

}

CImriUlSrv::~CImriUlSrv()
{
}

int CImriUlSrv::StartServer(short sPort, HWND hMsgWnd)
{
	m_pDlgImriImpl = CDlgImriImpl::GetHandle();
	if (m_pDlgImriImpl->GetSafeHwnd() == NULL)
	{
		m_pDlgImriImpl->Create(IDD_DLG_IMRI, NULL);
	}
	m_pDlgImriImpl->ShowWindow(FALSE);

	return m_pDlgImriImpl->StartServer(sPort, hMsgWnd);
}

void CImriUlSrv::StopServer()
{
	m_pDlgImriImpl->StopServer();
}

int CImriUlSrv::ConnectServer(char* szIP, short sPort, HWND hMsgWnd)
{
	m_pDlgImriImpl = CDlgImriImpl::GetHandle();
	if (m_pDlgImriImpl->GetSafeHwnd() == NULL)
	{
		m_pDlgImriImpl->Create(IDD_DLG_IMRI, NULL);
	}
	m_pDlgImriImpl->ShowWindow(FALSE);

	return m_pDlgImriImpl->ConnectServer(sPort, szIP, hMsgWnd);
}

int CImriUlSrv::DisconnectServer()
{
	return m_pDlgImriImpl->DisconnectServer();
}

CImriUlSrv* CImriUlSrv::GetHandle()
{
	return &s_ulSrvObj;
}

WORD CImriUlSrv::GetIMRIACPHiVer()
{
	return m_pDlgImriImpl->GetIMRIACPHiVer();
}

WORD CImriUlSrv::GetIMRIACPLoVer()
{
	return m_pDlgImriImpl->GetIMRIACPLoVer();
}

void CImriUlSrv::SetAppImp(CVImriUlServerApp *pulInterfaceImp)
{
	m_pDlgImriImpl->SetAppImp(pulInterfaceImp);
}

BOOL CImriUlSrv::IsMRIClientConnected()
{
	return m_pDlgImriImpl->IsMRIConnected();
}

int CImriUlSrv::SendCommand(ENUM_COMMANDS eCommand, void *pBuffer /* = NULL */, int nDataLen /* = 0 */)
{
	return m_pDlgImriImpl->SendCommand(eCommand, pBuffer, nDataLen);
}

int CImriUlSrv::SendStatusRequest(ENUM_STATUS_TYPE eRequest)
{
	return m_pDlgImriImpl->SendStatusRequest(eRequest);
}

int CImriUlSrv::SendInfoRequest(ENUM_INFOR_TYPE eRequest)
{
	return m_pDlgImriImpl->SendInfoRequest(eRequest);
}


int	CImriUlSrv::SendLocalization(CList<IMRI_LOCALIZATION*, IMRI_LOCALIZATION*>& lststcrLocalization)
{
	return m_pDlgImriImpl->SendLocalization(lststcrLocalization);
}

BOOL CImriUlSrv::CheckOperResult(int nOperResult)
{
	return m_pDlgImriImpl->CheckOperResult(nOperResult);
}

void  CImriUlSrv::AddClientIP(char* szIP)
{
	m_pDlgImriImpl->m_llcpCommObj.SetAcceptIP(szIP);
}


