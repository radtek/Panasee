// ProtocolManager.cpp: implementation of the CProtocolManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "ProtocolManager.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProtocolManager::CProtocolManager()
{
	m_pCurrentProtocol = NULL;
}

CProtocolManager::~CProtocolManager()
{
	CleanBdyPrtLst();
}

void CProtocolManager::AddBodyPartProtocols(IMRI_PROTOCOL_GROUP* pBdyPrtProtocol)
{
	m_lstBdyProtocols.AddTail(pBdyPrtProtocol);
}

CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* CProtocolManager::GetBdyPrtLst()
{
	return &m_lstBdyProtocols;
}


IMRI_PROTOCOL_GROUP* CProtocolManager::GetBodyPartProtocols(CString sBodyPrtDesc)
{
	IMRI_PROTOCOL_GROUP* pBdyPrtPro = NULL;
	if (m_lstBdyProtocols.GetCount() > 0)
	{
		POSITION pos = m_lstBdyProtocols.GetHeadPosition();
		IMRI_PROTOCOL_GROUP* pSearchPro;
		for (;pos;)
		{
			pSearchPro = m_lstBdyProtocols.GetNext(pos);
			if (pSearchPro && pSearchPro->sDesc == sBodyPrtDesc)
			{
				pBdyPrtPro = pSearchPro;
				break;
			}
		}
	}
	return pBdyPrtPro;
}

void* CProtocolManager::SetCurrentProtocol(CString sBodyPrtDesc, CString sProtocolDesc)
{
	CConfigMgr conf;
	
	if (m_lstBdyProtocols.GetCount() > 0)
	{
		POSITION pos = m_lstBdyProtocols.GetHeadPosition();
		IMRI_PROTOCOL_GROUP* pSearchBdyPro;
		for (;pos;)
		{
			pSearchBdyPro = m_lstBdyProtocols.GetNext(pos);
			if (pSearchBdyPro && pSearchBdyPro->sDesc == sBodyPrtDesc)
			{
				for (int i=0; i<int(pSearchBdyPro->nProtocolSum); i++)
				{
					
						IMRI_PROTOCOL* pSearchPro1 = NULL;
						pSearchPro1 = (IMRI_PROTOCOL*)pSearchBdyPro->pProtocols[i];
						if (pSearchPro1 && pSearchPro1->oProtDesc.cProtocolDesc == sProtocolDesc)
						{
							m_pCurrentProtocol = (void*)pSearchPro1;
							break;
						}
					
				}
				break;
			}
		}
	}

	if (m_pCurrentProtocol == NULL)
	{
		CString sMsg;
		sMsg.Format("Can't find protocol \"%s:%s\". ", sBodyPrtDesc, sProtocolDesc);
		PanaMessageBox(sMsg);
	}
	return m_pCurrentProtocol;
}

void* CProtocolManager::GetCurrentProtocol()
{
	return m_pCurrentProtocol;
}

void CProtocolManager::CleanBdyPrtLst()
{
	CConfigMgr conf;
	
	if (m_lstBdyProtocols.GetCount() > 0)
	{
		POSITION pos = m_lstBdyProtocols.GetHeadPosition();
		IMRI_PROTOCOL_GROUP* pSearchBdyPro;
		for (;pos;)
		{
			pSearchBdyPro = m_lstBdyProtocols.GetNext(pos);
			if (pSearchBdyPro)
			{
				
				void *pSearchProtocol;
				for (int i=0; i<(int)(pSearchBdyPro->nProtocolSum); i++)
				{
					pSearchProtocol = pSearchBdyPro->pProtocols[i];
					if (pSearchProtocol)
					{
							delete (IMRI_PROTOCOL*)pSearchProtocol;
						
						pSearchProtocol = NULL;
					}
				}
				if (pSearchBdyPro->pProtocols)
				{
					delete pSearchBdyPro->pProtocols;
				}
				delete pSearchBdyPro;
			}
		}
		m_lstBdyProtocols.RemoveAll();
	}
}

