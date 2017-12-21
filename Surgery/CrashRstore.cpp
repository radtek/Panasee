// CrashRstore.cpp: implementation of the CCrashRstore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "CrashRstore.h"
#include "CryotherapyDlg.h"
#include "DlgSeries.h"
#include "ConfigMgr.h"
#include "Dispaly4Dlg.h"
#include "DlgMain.h"
//#include "Config.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

char g_cCrashFile[MAX_PATH];  //File to store crash restore information

BOOL g_bInRestoring;
BOOL g_bUserConfirmRestore;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrashRstore::CCrashRstore()
{
	g_bInRestoring = FALSE;
	g_bUserConfirmRestore = FALSE;
}

CCrashRstore::~CCrashRstore()
{

}

void CCrashRstore::Clean()
{
	CConfigMgr conf;
	conf.Write(SYS_RESTORE, RESTORED, 0, g_cCrashFile);
	conf.Write(RESTORE_IMG, RESTORED, 0, g_cCrashFile);
	conf.Write(RESTORE_DISP, RESTORED, 0, g_cCrashFile);
	conf.Write(RESTORE_PROBE, RESTORED, 0, g_cCrashFile);

	// Clean image list restore
//	conf.Write(RESTORE_IMG, IMG_NUM, 0, g_cCrashFile);
	SaveCrashStateImage(TRUE);

	// Clean display restore
	char pszItem[256];
	CString sName = "";
	for (int i=1; i<5; i++)
	{
		sprintf(pszItem, "%s%d", DISPLAY, i);
		conf.Write(RESTORE_DISP, pszItem, sName, g_cCrashFile);
	}

	// Clean probe restore
	SaveCrashStateProbe(TRUE);
}

BOOL CCrashRstore::IsRestoredFromCrash()
{
	CConfigMgr conf;
	BOOL bRestoredFromCrash = conf.Read(SYS_RESTORE, RESTORED, 0, g_cCrashFile);
	return bRestoredFromCrash;
}

BOOL CCrashRstore::IsUserConfirmedRestore()
{
	return g_bUserConfirmRestore;
}

void CCrashRstore::UserConfirmRestore(BOOL bRestore)
{
	g_bUserConfirmRestore = bRestore;
}

void CCrashRstore::SaveCrashState(CRASH_INFO eCurSaveType)
{
	if (g_bInRestoring)
		return;

	CConfigMgr conf;
	conf.Write(SYS_RESTORE, RESTORED, 1, g_cCrashFile);

	CDlgMain *pFrm = g_DlgMain;
	CCryotherapyDlg *pDlgCry = &pFrm->m_dlgCryotherapy;
	switch (eCurSaveType)
	{
	case CI_IMAGES:
		{
//			conf.Write(RESTORE_IMG, RESTORED, 1, g_cCrashFile);
//			CStringArray arMRIs;
//			CStringArray arProbes;
//			char pszItem[256];
//			pDlgCry->m_dlgTabCry.m_dlgSeries.GetMRISeriesName(arMRIs, &arProbes);
//			conf.Write(RESTORE_IMG, IMG_NUM, arMRIs.GetSize(), g_cCrashFile);
//			for (int i=0; i<arMRIs.GetSize(); i++)
//			{
//				sprintf( pszItem, "%s%d", IMG_NAME, i );
//				conf.Write(RESTORE_IMG, pszItem, arMRIs[i], g_cCrashFile);
//			}
			SaveCrashStateImage();
			break;
		}
	case CI_DISPLAY:
		{
			conf.Write(RESTORE_DISP, RESTORED, 1, g_cCrashFile);
			char pszItem[256];
			CString sName;
			for (int i=1; i<5; i++)
			{
				sprintf(pszItem, "%s%d", DISPLAY, i);
				if (pDlgCry->m_pDisplay4Dlg->GetDispWrap(i)->m_pDispSeries)
				{
					sName = pDlgCry->m_pDisplay4Dlg->GetDispWrap(i)->m_pDispSeries->GetMRISeries()->GetName();
				}
				else
				{
					sName = "";
				}
				conf.Write(RESTORE_DISP, pszItem, sName, g_cCrashFile);
			}
		}
		break;
	case CI_PROBES:
		{
			SaveCrashStateProbe();
		}
		break;
	case CI_SCANNER:
		break;
	default:
		break;
	}
}

void CCrashRstore::SaveCrashStateImage(BOOL bClean /* = FALSE */)
{
	CConfigMgr conf;
	if(!bClean)
	{
		conf.Write(RESTORE_IMG, RESTORED, 1, g_cCrashFile);
	}

	CDlgMain *pFrm = g_DlgMain;
	CCryotherapyDlg *pDlgCry = &pFrm->m_dlgCryotherapy;

	CFile f;
	char buf[512];
	if( !f.Open( g_ExecPath + IMAGE_RST_FILE, CFile::modeCreate | CFile::modeWrite) ) 
	{
		PanaMessageBox("Can't open file !");
		return;
	}
	CArchive ar(&f, CArchive::store, 512, buf);
	if (bClean)
	{
		ar << 0;
	}
	else
	{
		CStringArray arMRIs;
		CStringArray arProbes;
		pDlgCry->m_dlgSeries.GetMRISeriesName(arMRIs, &arProbes);

		ar << arMRIs.GetSize();
		for (int i=0; i<arMRIs.GetSize(); i++)
		{
			ar << arMRIs[i] << arProbes[i];
		}
	}
}

void CCrashRstore::SaveCrashStateProbe(BOOL bClean /* = FALSE */)
{
	CDlgMain *pFrm = g_DlgMain;
	CCryotherapyDlg *pDlgCry = &pFrm->m_dlgCryotherapy;

	CConfigMgr conf;
	if(!bClean)
	{
		conf.Write(RESTORE_PROBE, RESTORED, 1, g_cCrashFile);
	}

	CFile f;
	char buf[512];
	if( !f.Open( g_ExecPath + PROBES_RST_FILE, CFile::modeCreate | CFile::modeWrite) ) 
	{
		PanaMessageBox("Can't open file !");
		return;
	}
	CArchive ar(&f, CArchive::store, 512, buf);
	if (bClean)
	{
		ar << 0;
	}
	else
	{
		CArray<CProbe*, CProbe*>* pArrProbes;
		CProbe* pProbe;
		pArrProbes = pDlgCry->m_ProbePool.GetProbePool();

		int iProbesCnt = pArrProbes->GetSize();
		if (iProbesCnt > 0)
		{
			ar << pArrProbes->GetSize();
			ar << pDlgCry->m_ProbePool.GetNextNewProbeIndex();
			for (int i=0; i<pArrProbes->GetSize(); i++)
			{
				pProbe = pArrProbes->GetAt(i);
				if (pProbe->GetName() != PREPROBE_NAME)
				{
					pProbe->Serialize(ar);
				}
			}
			ar << pDlgCry->m_dlgScannerControl.m_pCurRealProbe->GetName();
		}
	}
}

void CCrashRstore::RestoreCrashState()
{
	CConfigMgr conf;
	CDlgMain *pFrm = g_DlgMain;
	CCryotherapyDlg *pDlgCry = &pFrm->m_dlgCryotherapy;
	char pszItem[256];

	g_bInRestoring = TRUE;

	// Restore image list
	BOOL bRestoredFromCrash = conf.Read(RESTORE_IMG, RESTORED, 0, g_cCrashFile);
	if (bRestoredFromCrash)
	{
//		CStringArray arMRIs;
//		int iImgNum = conf.Read(RESTORE_IMG, IMG_NUM, 0, g_cCrashFile);
//		for (int i=0; i<iImgNum; i++)
//		{
//			sprintf( pszItem, "%s%d", IMG_NAME, i );
//			CString sName = conf.Read(RESTORE_IMG, pszItem, "", g_cCrashFile);
//			arMRIs.Add(sName);
//		}
//		pDlgCry->m_dlgTabCry.m_dlgSeries.InsertMRIList(arMRIs);
		CFile f;
		char buf[512];
		if( f.Open( g_ExecPath + IMAGE_RST_FILE, CFile::modeRead) ) 
		{
			CArchive ar(&f, CArchive::load, 512, buf);

			int iImgCnt;
			ar >> iImgCnt;
			if (iImgCnt > 0)
			{
				CStringArray arMRIs;
				CStringArray arProbes;
				CString sMRI, sProbe;
				for (int i=0; i<iImgCnt; i++)
				{
					ar >> sMRI >> sProbe;
					arMRIs.Add(sMRI);
					arProbes.Add(sProbe);
				}
				pDlgCry->m_dlgSeries.InsertMRIList(arMRIs, &arProbes);
			}
		}
	}

	// Restore display
	bRestoredFromCrash = conf.Read(RESTORE_DISP, RESTORED, 0, g_cCrashFile);
	if (bRestoredFromCrash)
	{
		CString sName[5];
		for (int i=1; i<5; i++)
		{
			sprintf( pszItem, "%s%d", DISPLAY, i );
			sName[i] = conf.Read(RESTORE_DISP, pszItem, "", g_cCrashFile);
		}
		for (i=1; i<5; i++)
		{
			if (sName[i] != "")
			{
				pDlgCry->m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.DisplayMRIToMainDispalyWnd(sName[i]);
			}
		}
	}

	// Restore probes
	bRestoredFromCrash = conf.Read(RESTORE_PROBE, RESTORED, 0, g_cCrashFile);
	if (bRestoredFromCrash)
	{
		CFile f;
		char buf[512];
		if( f.Open( g_ExecPath + PROBES_RST_FILE, CFile::modeRead) ) 
		{
			CArchive ar(&f, CArchive::load, 512, buf);

			int iProbeCnt;
			ar >> iProbeCnt;
			if (iProbeCnt > 0)
			{
				int iProbeIndex;
				ar >> iProbeIndex;
				pDlgCry->m_ProbePool.SetNextNewProbeIndex(iProbeIndex);
				
				for (int i=0; i<iProbeCnt; i++)
				{
					CProbe* pProbe = new CProbe();
					pProbe->Serialize(ar);
					pDlgCry->m_ProbePool.AddNewProbe(pProbe);
					pDlgCry->m_dlgScannerControl.SelecteProbe(pProbe);
				}
				pDlgCry->m_dlgScannerControl.CreateProbeMenu();
				CString sCurProbeName;
				ar >> sCurProbeName;
				CProbe *pProbe = pDlgCry->m_ProbePool.GetProbeByName(sCurProbeName);
				pDlgCry->m_dlgScannerControl.SelecteProbe(pProbe);
			}
		}
	}

	g_bInRestoring = FALSE;
}