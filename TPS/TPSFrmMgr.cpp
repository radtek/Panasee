
#include "stdafx.h"
#include "resource.h"
#include "AppDialog.h"
#include "TPSFrmMgr.h"
#include "DisplayDlg.h"
#include "ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPSFrmMgr



CTPSFrmMgr::CTPSFrmMgr()
{	
	SetType("Tps");
 	CreateContents();
	ShowContents(SW_HIDE);
}

CTPSFrmMgr::~CTPSFrmMgr()
{
	

}


////////////////////////////////////////////////////////////////////////////////
// CreateContents()
// Description: virtual function. Override to create its own frame dialogs.
//
// Return: void 
// Parameters: 
//    CWnd* pParentWnd
////////////////////////////////////////////////////////////////////////////////
void CTPSFrmMgr::CreateContents()
{
	CAppDialog *pDlg1,*pDlg2 = NULL;
	pDlg1= new CDisplayDlg(NULL);
	pDlg1->SetFrameMgr(this);
		
	Add(pDlg1);

	pDlg2= new CControlDlg(NULL);
	pDlg2->SetFrameMgr(this);
	Add(pDlg2);

	
}

void CTPSFrmMgr::UpdateProbeInfo(double *tipPos, double *ori)
{
	CDisplayDlg* pDlg=(CDisplayDlg*)GetAt(0);
	if(pDlg)
	{		
		pDlg->UpdateRealProbeInfo(tipPos,ori);
	}
}

void CTPSFrmMgr::GetProbeInfo(int &count, double *pProbeData)
{
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	count = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	if(!pProbeData)
		return; 

	int nCount=count;
	double tip[3];
	for(int i=0;i<nCount;i++)
	{
		CProbeCryo* pNeedle = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		pNeedle->GetTipPosInWorld(tip);
		pProbeData[i*3]=tip[0];
		pProbeData[i*3+1]=tip[1];
		pProbeData[i*3+2]=tip[2];
	}	
}
