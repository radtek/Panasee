// DataPool.cpp: implementation of the CDataPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "DataPool.h"
#include "Config.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataPool::CDataPool()
{
	LoadProbePar();
}

CDataPool::~CDataPool()
{
	CleanProbePar();
}

BOOL CDataPool::LoadProbePar()
{
	// Load cryotherapy probe data
	CString sFilePath = CONFIG_FILES[CF_PROBE_CRYOTYPE];
	FILE *fp;
	fp = fopen(sFilePath, "r");
	if (!fp)
	{
		CLog::Log("Cryotherapy probe data file doesn't exit.");
	}
	else 
	{
		char sType[40];
		while (!feof(fp))
		{
			struct PROBE_PAR_CRY* pParCry = new struct PROBE_PAR_CRY;
			fscanf(fp, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n", 
				sType,
				&pParCry->dbLength,
				&pParCry->dbRadius,
				&pParCry->dbLAxis0,
				&pParCry->dbSAxis0,
				&pParCry->dbOffset0,
				&pParCry->dbLAxis20,
				&pParCry->dbSAxis20,
				&pParCry->dbOffset20,
				&pParCry->dbLAxis40,
				&pParCry->dbSAxis40,
				&pParCry->dbOffset40);
			CString sTmp(sType);
			sTmp.Replace("_", " ");
			pParCry->sName = sTmp;
			pParCry->dbRadius = pParCry->dbRadius / 2;
			arrProbeParCrys.Add(pParCry);
		}
		fclose(fp);
	}

	// Load biopsy probe data
	sFilePath = CONFIG_FILES[CF_PROBE_BIOTYPE];
	fp = fopen(sFilePath, "r");
	if (!fp)
	{
		CLog::Log("Biopsy probe data file doesn't exit.");
	}
	else 
	{
		char sType[40];
		while (!feof(fp))
		{
			struct PROBE_PAR_BIO* pParBio = new struct PROBE_PAR_BIO;
			fscanf(fp, "%s %lf\n", 
				sType,
				&pParBio->dbRadius);
			CString sTmp(sType);
			sTmp.Replace("_", " ");
			pParBio->sName = sTmp;
			pParBio->dbRadius = pParBio->dbRadius / 2;
			arrProbeParBios.Add(pParBio);
		}
		fclose(fp);
	}
	return TRUE;
}

void CDataPool::CleanProbePar()
{
	for (int i=0; i<arrProbeParCrys.GetSize(); i++)
	{
		delete arrProbeParCrys[i];
		arrProbeParCrys[i] = NULL;
	}
	arrProbeParCrys.RemoveAll();

	for (i=0; i<arrProbeParBios.GetSize(); i++)
	{
		delete arrProbeParBios[i];
		arrProbeParBios[i] = NULL;
	}
	arrProbeParBios.RemoveAll();
}

CArray<struct PROBE_PAR_CRY*, struct PROBE_PAR_CRY*>* CDataPool::GetProbeParCry()
{
	return &arrProbeParCrys;
}

struct PROBE_PAR_CRY* CDataPool::GetProbeParCryByName(CString sName)
{
	for (int i=0; i<arrProbeParCrys.GetSize(); i++)
	{
		if (arrProbeParCrys[i]->sName == sName)
			return arrProbeParCrys[i];
	}
	return NULL;
}

CArray<struct PROBE_PAR_BIO*, struct PROBE_PAR_BIO*>* CDataPool::GetProbeParBio()
{
	return &arrProbeParBios;
}

struct PROBE_PAR_BIO* CDataPool::GetProbeParBioByName(CString sName)
{
	for (int i=0; i<arrProbeParBios.GetSize(); i++)
	{
		if (arrProbeParBios[i]->sName == sName)
			return arrProbeParBios[i];
	}
	return NULL;
}

