// ProbePool.cpp: implementation of the CProbePool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "ProbePool.h"
#include "ConfigMgr.h"
#include "Config.h"
#include "Log.h"
#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"
#include "CrashRstore.h"
#include "DlgMain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbePool::CProbePool()
{
	m_iProbeNameIndex = 1;
}

CProbePool::~CProbePool()
{
	for (int i=0; i<m_arrpProbePool.GetSize(); i++)
	{
		if (m_arrpProbePool[i])
		{
			delete m_arrpProbePool[i];
		}
	}
	m_arrpProbePool.RemoveAll();
}

void CProbePool::InitPreimageProbe()
{
	double dTip[3],dOri[3];
	double dLength,dDiameter;
	CConfigMgr con;
	CString sFilePath = CONFIG_FILES[CF_PROBE_PREIMAGE];
	if( CGlobalFuncs::LoadProbeFile(sFilePath, dOri, dTip, dLength, dDiameter) )
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s successfully. ", sFilePath);
		CLog::Log(sLog);
	}
	else
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s fail. ", sFilePath);
		CLog::Log(sLog);
		PanaMessageBox("Fail to load prescan probe!");
		return;
	}
	CCryotherapyDlg* pCryotherapyDlg;
	pCryotherapyDlg = &(g_DlgMain)->m_dlgCryotherapy;

	m_preImgProbe.SetTipInProbe(dTip[0], dTip[1], dTip[2]);
	m_preImgProbe.SetOrientationInProbe(dOri[0], dOri[1], dOri[2]);
	m_preImgProbe.SetName(PREPROBE_NAME);
	m_preImgProbe.UpdatePosOri();
	
	pCryotherapyDlg->m_pDisplay4Dlg->AddProbe(&m_preImgProbe);
}

void CProbePool::AddNewProbe(CProbe *pProbe)
{
	CDlgMain *pMainFrm = g_DlgMain;
	CCryotherapyDlg* pCryotherapyDlg;
	pCryotherapyDlg = &pMainFrm->m_dlgCryotherapy;

//	pProbe->SetIndex(m_iProbeNameIndex);
//	m_iProbeNameIndex ++;
	
	m_arrpProbePool.Add(pProbe);
	pCryotherapyDlg->m_pDisplay4Dlg->AddProbe(pProbe);

//	CCrashRstore::SaveCrashState(CI_PROBES);
}

int CProbePool::DeleteProbe(CProbe *pProbe)
{
	CDlgMain *pMainFrm = g_DlgMain;
	CCryotherapyDlg *pDlgCry = &pMainFrm->m_dlgCryotherapy;
	for (int i=0; i<m_arrpProbePool.GetSize(); i++)
	{
		if (m_arrpProbePool.GetAt(i) == pProbe)
		{
			pDlgCry->m_pDisplay4Dlg->RemoveProbe(pProbe);
			m_arrpProbePool.RemoveAt(i);
			delete pProbe;
			return 1;
		}
	}
	return 0;
}

CProbe* CProbePool::GetProbeByIndex(int iIndex)
{
	for (int i=0; i<m_arrpProbePool.GetSize(); i++)
	{
		if (m_arrpProbePool.GetAt(i)->GetIndex() == iIndex)
			return m_arrpProbePool.GetAt(i);
	}
	return NULL;
}

CProbe* CProbePool::GetProbeByArrIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_arrpProbePool.GetSize())
		return NULL;

	return m_arrpProbePool.GetAt(iIndex);
}

CProbe* CProbePool::GetProbeByName(CString sName)
{
	if (sName == PREPROBE_NAME)
		return &m_preImgProbe;

	for (int i=0; i<m_arrpProbePool.GetSize(); i++)
	{
		if (m_arrpProbePool.GetAt(i)->GetName() == sName)
			return m_arrpProbePool.GetAt(i);
	}
	return NULL;
}

CProbe* CProbePool::GetLastProbe()
{
	CProbe *pProbe = NULL;
	if (m_arrpProbePool.GetSize() > 0)
	{
		pProbe = m_arrpProbePool.GetAt(m_arrpProbePool.GetSize() - 1);
	}
	else
	{
		pProbe = &m_preImgProbe;
	}
	return pProbe;
}

CArray<CProbe*, CProbe*>* CProbePool::GetProbePool()
{
	return &m_arrpProbePool;
}

int CProbePool::GetNextNewProbeIndex()
{
	return m_iProbeNameIndex;
}

void CProbePool::SetNextNewProbeIndex(int iIndex)
{
	m_iProbeNameIndex = iIndex;
}
