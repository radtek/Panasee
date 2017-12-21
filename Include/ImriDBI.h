// ImriDBI.h: interface for the CImriDBI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMRIDBI_H__11079513_44BE_4731_81EB_86B1FFB8216E__INCLUDED_)
#define AFX_IMRIDBI_H__11079513_44BE_4731_81EB_86B1FFB8216E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)
#include <map>


//
int   gBubleSortEx(int naSort[], int nCount, CString csString[], bool bAsc);
int   gBubleSortDateEx(int naSort[], int nCount, CString csString[], bool bAsc );
int gDateToInt(CString csDate) ;
int   gCheckDateFormat(CString csDate, int &nY, int &nM, int &nD) ;

#import "c:\program files\common files\system\ado\msado15.dll" \
  no_namespace \
  rename ("EOF", "EndOfFile")

#include "IGTImageHeader.h"

#define MAX_SEL_SERIES      100


class  AFX_EXT_CLASS  CImriDBI
{
public:
	CImriDBI();
	virtual ~CImriDBI();
public:
	void SetQueryTestFlag(BOOL bQueryTest);
	BOOL GetRecordSet(CString sSql,_RecordsetPtr &pRst);
	BOOL ExecuateSQL(CString sSql,_RecordsetPtr &pRst);
	int ExistStudy(CString csSId ,CString csMId);
	//delete all the records in tables
	void ClearDatabase();

	//Get database folder from "HKEY_LOCAL_MACHINE\\SOFTWARE\\XinaoMDT\\Panasee\\Database"
	CString GetDatabaseFolder();
	//save database file and folder into "HKEY_LOCAL_MACHINE\\SOFTWARE\\XinaoMDT\\Panasee\\Database"
	BOOL SaveDBPath(CString sFile,CString sFolder);
	//get database file and folder from "HKEY_LOCAL_MACHINE\\SOFTWARE\\XinaoMDT\\Panasee\\Database"
	void GetDBPath(CString &sFile,CString &sFolder);
	//Check if database file and folder are valid by specified sFile and sFolder
	BOOL Validate(CString sFile,CString sFolder);
	//check if database file and folder are valid by getting file and folder from registry.
	BOOL Validate();

	bool    ConnectDB();
	void    DisconnectDB();
	void    SetDBPathName(CString csDBPathName);
	int     GetErrCode();
	CString GetErrMsg() ;

	int     OpenSiteTable(CListCtrl &listCtrl) ;
	int     OpenPatientTable(CListCtrl& listCtrl,CString csModalityID = "") ;
	int     OpenPatientTableByName(CListCtrl* pListCtrl,CString csName);
	int     OpenStudyTable(CListCtrl &listCtrl, CString csPatientID = "", CString csModalityID = "");
	int     OpenSeriesTable(CString csSQL, CString csStudyID, CListCtrl &listCtrl) ;

	int   AddSeries(IGT_SeriesField seriesParams, int dwStudyUID);
	int   AddStudy(IGT_StudyField studyParams,int dwPatientUID,CString csId,CString csMId);
	int   AddPatient(CString csId, CString csName, CString csBirth, CString csAge,int nSex, 
					  CString csWeight, CString csDesc, CString csMID);
	int   UpdatePatiet(CString csId, CString csName, CString csBirth, CString csAge, int nSex, 
					     CString csWeight, CString csDesc, CString csMID);
	int   AddModality(CString csId, CString csName,CString csType,CString csDesc);
    int     QueryByStudyId(CString csId, CListCtrl &listCtrl);
	int     QueryByStudyDesc(CString csDesc, CListCtrl &listCtrl);
	int     QueryByBodyPart(CString csBodyPart, CListCtrl &listCtrl);
	int     QueryByStudyTime(COleDateTime tmFrom,COleDateTime tmTo, CListCtrl &listCtrl);
	int     QueryByPatientID(CString csPId, CListCtrl &listCtrl) ;
	int     QueryByName(CString csName, CListCtrl &listCtrl);
	CString QueryPatientId(CString csSId, CString csMId);

	int   ExistModality(CString csMId);
	int   ExistPatient(CString csPId,CString csMId);
	int   ExistSeries(int csSeriesID ,int dwStudyID);

	void    StringToDate(CString csDate, int nDate[]);
protected:
	inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

protected:
	_ConnectionPtr m_pConnection ;
	CString        m_csDbPathName ;
	int            m_nErrCode ;
	CString        m_csErrMsg ;

    std::map<CString,CString>	m_mapPatientName ;
	std::map<CString,CString>	m_mapModalityType ;
private:
	int OpenStudyTableBySql(CListCtrl &listCtrl, CString sSql);
	int OpenTableTest();
	BOOL m_bQueryTest;
	int   ExistPatient(int dwPatientUID);
	int   ExistStudy(int dwStudyUID);
	
	
};

#endif // !defined(AFX_IMRIDBI_H__11079513_44BE_4731_81EB_86B1FFB8216E__INCLUDED_)
