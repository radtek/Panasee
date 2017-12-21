// TargetPool.cpp: implementation of the CTargetPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "TargetPool.h"
#include "VirtualTarget.h"
#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"
#include "DisplayDlg.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTargetPool::CTargetPool()
{
	m_iNumVirtualTarget = 0;
	m_pCryoDlg = NULL;
}

CTargetPool::~CTargetPool()
{
	if (m_lstVirtualTargets.GetCount() > 0)
	{
		POSITION pos = m_lstVirtualTargets.GetHeadPosition();
		CVirtualTarget* pVTarget;
		for (;pos;)
		{
			pVTarget = m_lstVirtualTargets.GetNext(pos);
			delete pVTarget;
		}
		m_lstVirtualTargets.RemoveAll();
		m_iNumVirtualTarget = 0;
	}
}

void CTargetPool::CreateATarget(Vector3D vPos, BOOL bCreateVirtualProbe /* = FALSE */)
{
	CVirtualTarget *pVTarget = new CVirtualTarget(vPos);

	pVTarget->SetIndex(++m_iNumVirtualTarget);
	
	m_lstVirtualTargets.AddTail(pVTarget);

	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.AddVirtualTarget(pVTarget);
	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene();
	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.UpdateAllProjectVTarget();
	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.UpdateAllProjectVTarget();
	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.UpdateAllProjectVTarget();
	m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.UpdateAllProjectVTarget();
	
}

void CTargetPool::DeleteATarget(CVirtualTarget* pTarget)
{
	if (pTarget)
	{
		if(PanaMessageBox("Are you sure to delete the target " + pTarget->GetSphere()->GetName() + "?",MB_OKCANCEL) != IDOK)
			return;

		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.RemoveVirtualTarget(pTarget);
		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene(); 

		m_lstVirtualTargets.RemoveAt(m_lstVirtualTargets.Find(pTarget));
		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryoDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.UpdateAllProjectVTarget();
//		m_pCurVirtualTarget = NULL;
		
		CString sLog;
		sLog.Format("Delete target %d", pTarget->GetSphere()->GetName());
		CLog::Log(sLog);

		delete pTarget;
	}
}

void CTargetPool::TransformAllVTarget(double** ppdtran)
{
	if (m_lstVirtualTargets.GetCount() > 0)
	{
		POSITION pos = m_lstVirtualTargets.GetHeadPosition();
		for (;pos;)
		{ 
			CVirtualTarget* pVTarget = m_lstVirtualTargets.GetNext(pos);
			pVTarget->TransformM(ppdtran);
		}
	}
}

void CTargetPool::TranslateAllVTarget(Vector3D vMov)
{
	if (m_lstVirtualTargets.GetCount() > 0)
	{
		POSITION pos = m_lstVirtualTargets.GetHeadPosition();
		for (;pos;)
		{ 
			CVirtualTarget* pVTarget = m_lstVirtualTargets.GetNext(pos);
			pVTarget->Translate(vMov);
		}
	}
}

void CTargetPool::TranslateAllVTarget(double dCurPTPos,BOOL bInverse)
{
	Vector3D vMov;
	if (m_lstVirtualTargets.GetCount() > 0)
	{
		POSITION pos = m_lstVirtualTargets.GetHeadPosition();
		for (;pos;)
		{ 
			CVirtualTarget* pVTarget = m_lstVirtualTargets.GetNext(pos);
			vMov = m_pCryoDlg->GetPTMoveVectorInTracker(dCurPTPos);
			if(bInverse)
			{
				vMov = -vMov;
			}
			pVTarget->Translate(vMov);
		}
	}
}
