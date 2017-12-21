// DataPool.h: interface for the CDataPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPOOL_H__7FE4A480_A740_4F27_A731_3CD390ADBB26__INCLUDED_)
#define AFX_DATAPOOL_H__7FE4A480_A740_4F27_A731_3CD390ADBB26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataPool  
{
public:
	CDataPool();
	virtual ~CDataPool();

	CArray<struct PROBE_PAR_CRY*, struct PROBE_PAR_CRY*>* GetProbeParCry();
	struct PROBE_PAR_CRY* GetProbeParCryByName(CString sName);

	CArray<struct PROBE_PAR_BIO*, struct PROBE_PAR_BIO*>* GetProbeParBio();
	struct PROBE_PAR_BIO* GetProbeParBioByName(CString sName);

private:
	BOOL LoadProbePar();
	void CleanProbePar();
	CArray<struct PROBE_PAR_CRY*, struct PROBE_PAR_CRY*> arrProbeParCrys;
	CArray<struct PROBE_PAR_BIO*, struct PROBE_PAR_BIO*> arrProbeParBios;
};

#endif // !defined(AFX_DATAPOOL_H__7FE4A480_A740_4F27_A731_3CD390ADBB26__INCLUDED_)
