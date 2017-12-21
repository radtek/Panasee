// ProtocolManager.h: interface for the CProtocolManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLMANAGER_H__6D4F9446_5D86_4695_830A_82F43FFD3EEE__INCLUDED_)
#define AFX_PROTOCOLMANAGER_H__6D4F9446_5D86_4695_830A_82F43FFD3EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "header.h"
#include "GlobalFuncs.h"

class CProtocolManager  
{
public:
	CProtocolManager();
	virtual ~CProtocolManager();

	IMRI_PROTOCOL_GROUP* GetBodyPartProtocols(CString sBodyPrtDesc);
	void CleanBdyPrtLst();
	void AddBodyPartProtocols(IMRI_PROTOCOL_GROUP* pBdyPrtProtocol);	
	void* SetCurrentProtocol(CString sBodyPrtDesc, CString sProtocolDesc);
	void* GetCurrentProtocol();
	CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*>* GetBdyPrtLst();

private:
	CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*> m_lstBdyProtocols;
	void* m_pCurrentProtocol;
};

#endif // !defined(AFX_PROTOCOLMANAGER_H__6D4F9446_5D86_4695_830A_82F43FFD3EEE__INCLUDED_)
