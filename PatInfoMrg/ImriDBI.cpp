// ImriDBI.cpp: implementation of the CImriDBI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImriDBI.h"

#include "Registry.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/************************************************************************************************/
//           GetErrCode() 返回错误码含义（2位）:如果操作成功，为0，如果出错，包含两位数字负数。
//           第一位数字表示出错的表：1-image；2-scan；3-series；4-study；5-patient；6-modality。
//           第二位数字表示的操作：  1-连接；2-查询；3-更新；4-插入新纪录。
/************************************************************************************************/
CImriDBI::CImriDBI()
{
	m_nErrCode     = 0  ;
	m_csErrMsg     = "" ;
    m_csDbPathName = "" ;
	m_pConnection = NULL;
	m_bQueryTest=FALSE;
	::CoInitialize(NULL);
	
}

CImriDBI::~CImriDBI()
{
	DisconnectDB(); //added by nsj
    ::CoUninitialize();
	
}

void CImriDBI::SetDBPathName(CString csDBPathName)
{
	m_csDbPathName = csDBPathName ;
	DisconnectDB();
	ConnectDB();
}

int CImriDBI::GetErrCode()
{
	return m_nErrCode ;
}

CString CImriDBI::GetErrMsg()
{
	return m_csErrMsg ;
}

bool CImriDBI::ConnectDB()
{
	m_nErrCode = 0;

	if ( m_pConnection )
	{
		return true;
	}

	if(m_csDbPathName=="")
	{
		CString sFolder;
		GetDBPath(m_csDbPathName,sFolder);
	}

	_bstr_t    bstrCnn;
	CString    csConn;
	csConn = "Provider=Microsoft.Jet.OLEDB.4.0; User ID=Admin; Data Source= "
		     + m_csDbPathName + ";" 
			 + "Mode=Share Deny None; Extended Properties=\"\"; Jet OLEDB:System database=\"\";"
             + "Jet OLEDB:Registry Path=\"\"; Jet OLEDB:Database Password=\"\";" 
             + "Jet OLEDB:Engine Type=5; Jet OLEDB:Database Locking Mode=1;" 
             + "Jet OLEDB:Global Partial Bulk Ops=2; Jet OLEDB:Global Bulk Transactions=1;" 
             + "Jet OLEDB:New Database Password=\"\"; Jet OLEDB:Create System Database=False;" 
             + "Jet OLEDB:Encrypt Database=False; Jet OLEDB:Don't Copy Locale on Compact=False; "
             + "Jet OLEDB:Compact Without Replica Repair=False; Jet OLEDB:SFP=False" ;

	bstrCnn = csConn.AllocSysString();
	try
	{
		TESTHR(m_pConnection.CreateInstance((__uuidof(Connection))));
		m_pConnection->Open(bstrCnn,"","",NULL);
	}
	catch (_com_error e)
	{

		DisconnectDB();
		m_pConnection = NULL;
		m_nErrCode = -1  ;
	    m_csErrMsg.Format("Cann't connect to database %s.",m_csDbPathName);
		return false ;
	}
	return true ;
}

void CImriDBI::DisconnectDB()
{
    try
	{
		if (m_pConnection != NULL)
		{
			if(m_pConnection->GetState()==adStateOpen)
			{
				m_pConnection->Close();
			}
			m_pConnection = NULL;
		}
	}
	catch (_com_error e)
	{
		//
	}
	
}

//return items for quury conditions.
//-1 if failed to connect the DB.
//-2 if open site(modality) table failed.
//-3 if open patient table failed.
int CImriDBI::OpenPatientTableByName(CListCtrl* pListCtrl,CString sName)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	// Define ADO object pointers.
     _RecordsetPtr    pRstPatient = NULL;
   
    // Define string variables.
	CString    csSQL ;
	_bstr_t    bstrSQL;
	CString    csUnique_Id, csId,csName,csSex,csBirthday,csMId,csAge,csWeight,csDesc;
	int        i = -1;
	int        nIndex = 0;
	LV_ITEM    lvitem ;
   
	
		csSQL.Format("SELECT * FROM PatientTable Where Name='%s' ORDER BY Id DESC",sName); 

		bstrSQL = csSQL.AllocSysString() ;
		try
		{
			TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
			pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
							  adLockOptimistic, adCmdText);

			while (!(pRstPatient->EndOfFile))
			{
				_variant_t theValue;

				//Get Unique_Id
				theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csUnique_Id = (char*)_bstr_t(theValue);
				}else
				{
					csUnique_Id="-1";
				}

				//Get Id
				theValue = pRstPatient->Fields->GetItem("Id")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csId = (char*)_bstr_t(theValue);
				}else
				{
					csId="-1";
				}

				//Get Name
				theValue = pRstPatient->Fields->GetItem("Name")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csName = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				theValue = pRstPatient->Fields->GetItem("Sex")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csSex = (char*)_bstr_t(theValue);
				}
				else
				{
					csSex = "U" ;
				}

				theValue = pRstPatient->Fields->GetItem("BirthDate")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csBirthday = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				
				theValue = pRstPatient->Fields->GetItem("Weight")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csWeight = (char*)_bstr_t(theValue);
				}
				else
				{
					csWeight = "0" ;
				}

				theValue = pRstPatient->Fields->GetItem("Modality_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csMId = (char*)_bstr_t(theValue);
				}else
				{
					csMId="";
				}

				theValue = pRstPatient->Fields->GetItem("Age")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csAge = (char*)_bstr_t(theValue);
				}else
				{
					csAge="0";
				}

				theValue = pRstPatient->Fields->GetItem("Desc")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csDesc = (char*)_bstr_t(theValue);
				}
				else
				{
					csDesc = "";
				}

				i++ ;
				pRstPatient->MoveNext();
				
				if(pListCtrl)
				{
					nIndex = pListCtrl->GetItemCount();
					lvitem.mask     = LVIF_TEXT ;
					lvitem.iItem    = nIndex ;
					lvitem.iSubItem = 0 ;
					lvitem.pszText  = (char*)(LPCTSTR)csId ;
					pListCtrl->InsertItem(&lvitem);
					pListCtrl->SetItemText(nIndex,1,(char*)(LPCTSTR)csName);
					pListCtrl->SetItemText(nIndex,2,csSex);
					pListCtrl->SetItemText(nIndex,3,csBirthday);
					pListCtrl->SetItemText(nIndex,4,csMId);
					pListCtrl->SetItemText(nIndex,5,csAge);
					pListCtrl->SetItemText(nIndex,6,csDesc);
					pListCtrl->SetItemText(nIndex,7,csWeight);
					pListCtrl->SetItemText(nIndex,8,csUnique_Id);
				}
			}
			i++ ;
			pRstPatient->Close();
		}
		catch (_com_error e)
		{
			DisconnectDB();
			m_pConnection = NULL;
			m_nErrCode = -52  ;
			CString  csMsg("Cannot open id = ");
			csMsg += csId;
			csMsg += " item in Patient table!";
			m_csErrMsg = csMsg ;
			return -3 ;
		}
	
    return i ;
}

//return items for quury conditions.
//-1 if failed to connect the DB.
//-2 if open site(modality) table failed.
//-3 if open patient table failed.
int CImriDBI::OpenPatientTable(CListCtrl& listCtrl,CString csModalityID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	// Define ADO object pointers.
     _RecordsetPtr    pRstPatient = NULL;
   
    // Define string variables.
	CString    csSQL ;
	_bstr_t    bstrSQL;
	CString    csUnique_Id, csId,csName,csSex,csBirthday,csMId,csAge,csWeight,csDesc;
	int        i = -1;
	int        nIndex = 0;
	LV_ITEM    lvitem ;
   
	if (csModalityID.IsEmpty())
	{
		if(m_bQueryTest)
			csSQL = "SELECT * FROM PatientTable Where TestFlag=1 ORDER BY Id DESC"; 
		else
			csSQL = "SELECT * FROM PatientTable Where TestFlag=0 ORDER BY Id DESC"; 

		bstrSQL = csSQL.AllocSysString() ;
		try
		{
			TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
			pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
							  adLockOptimistic, adCmdText);

			while (!(pRstPatient->EndOfFile))
			{
				_variant_t theValue;

				//Get Unique_Id
				theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csUnique_Id = (char*)_bstr_t(theValue);
				}else
				{
					csUnique_Id="-1";
				}

				//Get Id
				theValue = pRstPatient->Fields->GetItem("Id")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csId = (char*)_bstr_t(theValue);
				}else
				{
					csId="-1";
				}

				//Get Name
				theValue = pRstPatient->Fields->GetItem("Name")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csName = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				theValue = pRstPatient->Fields->GetItem("Sex")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csSex = (char*)_bstr_t(theValue);
				}
				else
				{
					csSex = "U" ;
				}

				theValue = pRstPatient->Fields->GetItem("BirthDate")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csBirthday = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				
				theValue = pRstPatient->Fields->GetItem("Weight")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csWeight = (char*)_bstr_t(theValue);
				}
				else
				{
					csWeight = "0" ;
				}

				theValue = pRstPatient->Fields->GetItem("Modality_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csMId = (char*)_bstr_t(theValue);
				}else
				{
					csMId="";
				}

				theValue = pRstPatient->Fields->GetItem("Age")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csAge = (char*)_bstr_t(theValue);
				}else
				{
					csAge="0";
				}

				theValue = pRstPatient->Fields->GetItem("Desc")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csDesc = (char*)_bstr_t(theValue);
				}
				else
				{
					csDesc = "";
				}

				i++ ;
				pRstPatient->MoveNext();

				nIndex = listCtrl.GetItemCount();
				lvitem.mask     = LVIF_TEXT ;
				lvitem.iItem    = nIndex ;
				lvitem.iSubItem = 0 ;
				lvitem.pszText  = (char*)(LPCTSTR)csId ;
				listCtrl.InsertItem(&lvitem);
				listCtrl.SetItemText(nIndex,1,(char*)(LPCTSTR)csName);
				listCtrl.SetItemText(nIndex,2,csSex);
				listCtrl.SetItemText(nIndex,3,csBirthday);
				listCtrl.SetItemText(nIndex,4,csMId);
				listCtrl.SetItemText(nIndex,5,csAge);
				listCtrl.SetItemText(nIndex,6,csDesc);
				listCtrl.SetItemText(nIndex,7,csWeight);
				listCtrl.SetItemText(nIndex,8,csUnique_Id);
			}
			i++ ;
			pRstPatient->Close();
		}
		catch (_com_error e)
		{
			DisconnectDB();
			m_pConnection = NULL;
			m_nErrCode = -52  ;
			CString  csMsg("Cannot open id = ");
			csMsg += csId;
			csMsg += " item in Patient table!";
			m_csErrMsg = csMsg ;
			return -3 ;
		}
	} //end if (csModalityID.IsEmpty())
	else
	{
		if(m_bQueryTest)
			csSQL.Format("SELECT * FROM PatientTable WHERE TestFlag=1 and Modality_ID='%s' ORDER BY Id DESC",
			         csModalityID); 
		else
			csSQL.Format("SELECT * FROM PatientTable WHERE TestFlag=0 and Modality_ID='%s' ORDER BY Id DESC",
			         csModalityID); 

		bstrSQL = csSQL.AllocSysString() ;
		try
		{
			TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
			pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
							  adLockOptimistic, adCmdText);

			while (!(pRstPatient->EndOfFile))
			{
				_variant_t theValue;

				//Get Unique_Id
				theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csUnique_Id = (char*)_bstr_t(theValue);
				}else
				{
					csUnique_Id="-1";
				}

				//Get Id
				theValue = pRstPatient->Fields->GetItem("Id")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csId = (char*)_bstr_t(theValue);
				}else
				{
					csId="-1";
				}

				//Get Name
				theValue = pRstPatient->Fields->GetItem("Name")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csName = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				theValue = pRstPatient->Fields->GetItem("Sex")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csSex = (char*)_bstr_t(theValue);
				}
				else
				{
					csSex = "U" ;
				}

				theValue = pRstPatient->Fields->GetItem("BirthDate")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csBirthday = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				
				theValue = pRstPatient->Fields->GetItem("Weight")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csWeight = (char*)_bstr_t(theValue);
				}
				else
				{
					csWeight = "0" ;
				}

				theValue = pRstPatient->Fields->GetItem("Modality_ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csMId = (char*)_bstr_t(theValue);
				}else
				{
					csMId="";
				}

				theValue = pRstPatient->Fields->GetItem("Age")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csAge = (char*)_bstr_t(theValue);
				}else
				{
					csAge="0";
				}

				theValue = pRstPatient->Fields->GetItem("Desc")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csDesc = (char*)_bstr_t(theValue);
				}
				else
				{
					csDesc = "";
				}

				i++ ;
				pRstPatient->MoveNext();

				nIndex = listCtrl.GetItemCount();
				lvitem.mask     = LVIF_TEXT ;
				lvitem.iItem    = nIndex ;
				lvitem.iSubItem = 0 ;
				lvitem.pszText  = (char*)(LPCTSTR)csId ;
				listCtrl.InsertItem(&lvitem);
				listCtrl.SetItemText(nIndex,1,(char*)(LPCTSTR)csName);
				listCtrl.SetItemText(nIndex,2,csSex);
				listCtrl.SetItemText(nIndex,3,csBirthday);
				listCtrl.SetItemText(nIndex,4,csMId);
				listCtrl.SetItemText(nIndex,5,csAge);
				listCtrl.SetItemText(nIndex,6,csDesc);
				listCtrl.SetItemText(nIndex,7,csWeight);
				listCtrl.SetItemText(nIndex,8,csUnique_Id);
			}
			i++ ;
			pRstPatient->Close();
		}
		catch (_com_error e)
		{
			DisconnectDB();
			m_pConnection = NULL;
			m_nErrCode = -52  ;
			CString  csMsg("Cannot open id = ");
			csMsg += csId;
			csMsg += " item in Patient table!";
			m_csErrMsg = csMsg ;
			return -3 ;
		}
	}
    return i ;
}

/**********************************************************************************/
//return items for query conditions.
//-1 if failed to connect the DB.
//-2 if open patient table failed.
//-3 if open study table failed.

//Input Parameters
//1. csPatientID: filter string. if = "", will list studies include all patients.
//2. csModalityID:filter string. if = "", will list studies include all modalities.
/**********************************************************************************/
int CImriDBI::OpenStudyTable(CListCtrl &listCtrl, CString csPatientID, CString csModalityID)
{
    CString    csSQL;
	int        i      = -1 ;
	
	if (csPatientID.IsEmpty()&&csModalityID.IsEmpty())  //List all studies in DB.
	{
		if(m_bQueryTest)
		{
			csSQL    = "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			csSQL   += "Where PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}else
		{
			csSQL    = "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			csSQL   += "Where PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}
	}

	if (!csPatientID.IsEmpty()&&csModalityID.IsEmpty())  //List all studies in DB.
	{
		if(m_bQueryTest)
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where PatientTable.ID='%s' and ",sTmp,csPatientID);
			csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}else
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where PatientTable.ID='%s' and ",sTmp,csPatientID);
			csSQL.Format("%s PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC",sTmp1);
		}
	}

	if (csPatientID.IsEmpty()&&!csModalityID.IsEmpty())  //List all studies in DB.
	{
		if(m_bQueryTest)
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where SiteTable.ID='%s' and ",sTmp,csModalityID);
			csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}else
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where SiteTable.ID='%s' and ",sTmp,csModalityID);
			csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}
	}

	if (!csPatientID.IsEmpty()&&!csModalityID.IsEmpty())  //List all studies in DB.
	{
		if(m_bQueryTest)
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where PatientTable.ID='%s' and SiteTable.ID='%s' and ",sTmp,csPatientID,csModalityID);
			csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}else
		{
			CString sTmp,sTmp1;
			sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
			sTmp1.Format("%s Where PatientTable.ID='%s' and SiteTable.ID='%s' and ",sTmp,csPatientID,csModalityID);
			csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
		}
	}

	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}

//return items for quury conditions.
//-1 if failed to connect the DB.
//-2 if open study  table failed.
//-3 if open series table failed.
int CImriDBI::OpenSeriesTable(CString csSQL, CString csStudyID, CListCtrl &listCtrl)
{
    // Define ADO object pointers.
    // Initialize pointers on define.
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstStudy   = NULL;
	 _RecordsetPtr    pRstSeries  = NULL;

    // Define string variables.
	_bstr_t    bstrCnn, bstrSQL;
	CString    csConn, csId, csProtocol, csSequenceName, csDesc, csTemp;
	CString    csSeriesUID,csMId;
	int        i = -1;
	int        nIndex ;
	int        nLastId = -1 ;
	LV_ITEM    lvitem ;

	try
	{
		//Get Study Unique ID
		_bstr_t bstrSQL1; 
		CString csSQL1 = "SELECT Study_Unique_ID,ModalityId FROM StudyTable WHERE Id = ";
		csSQL1        += "'"+csStudyID+"'" ;
        bstrSQL1       = csSQL1.AllocSysString();

		TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
        pRstStudy->Open(bstrSQL1, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                        adLockOptimistic, adCmdText);

        _variant_t theValue;
		//Get Study_Unique_Id
		long lUID;
		theValue = pRstStudy->Fields->GetItem("Study_Unique_ID")->Value ;
		if (theValue.vt != VT_NULL)
		{
			lUID = theValue.lVal;
		}

		theValue = pRstStudy->Fields->GetItem("ModalityId")->Value ;
		if (theValue.vt != VT_NULL)
		{
			csMId = (char*)_bstr_t(theValue);
		}
        pRstStudy->Close();

		csTemp.Format("WHERE Study_Unique_ID = %d ORDER BY Id",lUID);
		csSQL1 = csSQL + csTemp;
		bstrSQL = csSQL1.AllocSysString() ;
		TESTHR(pRstSeries.CreateInstance(__uuidof(Recordset)));
		pRstSeries->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                         adLockOptimistic, adCmdText);
		while (!(pRstSeries->EndOfFile))
		{
            //Get Id
			theValue = pRstSeries->Fields->GetItem("Id")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csId.Format("%d",theValue.iVal);
				
			}
			//Get Protocol Name
			theValue = pRstSeries->Fields->GetItem("Protocol")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csProtocol = (char*)_bstr_t(theValue);
			}
			
			//Get Description
			theValue = pRstSeries->Fields->GetItem("Desc")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csDesc = (char*)_bstr_t(theValue);
			}
			//Get UID
			theValue = pRstSeries->Fields->GetItem("Series_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csSeriesUID.Format("%d",theValue.lVal);
			}

			i++ ;
			pRstSeries->MoveNext();

			nIndex = listCtrl.GetItemCount();
            lvitem.mask     = LVIF_TEXT ;
			lvitem.iItem    = nIndex ;
			lvitem.iSubItem = 0 ;
			lvitem.pszText  = (char*)(LPCTSTR)csId ;
			
			listCtrl.InsertItem(&lvitem);
			listCtrl.SetItemText(nIndex,1,csProtocol);
			listCtrl.SetItemText(nIndex,2,csStudyID);
			listCtrl.SetItemText(nIndex,3,csDesc);
			listCtrl.SetItemText(nIndex,4,csMId);
			//listCtrl.SetItemState( nIndex, LVIS_SELECTED, LVIS_SELECTED);
		}

		listCtrl.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);

        pRstSeries->Close();
		i++ ;
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -32;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
		return -3 ;
	}
    
	return i ;
}



//return items for quury conditions.
//-1 if failed to connect the DB.
//-2 if open site  table failed.
int CImriDBI::OpenSiteTable(CListCtrl &listCtrl)
{
    // Define ADO object pointers.
    // Initialize pointers on define.
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstSite  = NULL;

    // Define string variables.
	_bstr_t    bstrCnn, bstrSQL;
	CString    csConn, csID, csModalityName, csType,csModalityDesc;
	int        i = -1;
	int        nIndex ;
	LV_ITEM    lvitem ;

	try
	{
		//Get Series Unique ID 
		CString csSQL = "SELECT * FROM SiteTable ORDER BY Id ASC";
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstSite.CreateInstance(__uuidof(Recordset)));
        pRstSite->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);

        _variant_t theValue;
		while (!(pRstSite->EndOfFile))
		{
            //Get Id
			theValue = pRstSite->Fields->GetItem("Id")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csID = (char*)_bstr_t(theValue);
			}
			//Get Modality Name
			theValue = pRstSite->Fields->GetItem("InstName")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csModalityName = (char*)_bstr_t(theValue);
			}
			//Get Modality Type
			theValue = pRstSite->Fields->GetItem("type")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csType = (char*)_bstr_t(theValue);
			}

			theValue = pRstSite->Fields->GetItem("Desc")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csModalityDesc = (char*)_bstr_t(theValue);
			}
			
			i++ ;
			pRstSite->MoveNext();

			nIndex = listCtrl.GetItemCount();
            lvitem.mask     = LVIF_TEXT ;
			lvitem.iItem    = nIndex ;
			lvitem.iSubItem = 0 ;
			lvitem.pszText  = (char*)(LPCTSTR)csID ;
			listCtrl.InsertItem(&lvitem);
			listCtrl.SetItemText(nIndex,1,csType);
			listCtrl.SetItemText(nIndex,2,csModalityName);
			listCtrl.SetItemText(nIndex,3,csModalityDesc);
		}
        i++ ;
        pRstSite->Close();		
	}
	catch (_com_error e)
	{
		DisconnectDB();
		 i = -1;
		m_nErrCode = -62  ;
		CString csMsg("Cannot open id = ");
		csMsg += csID;
		csMsg += " item in Site table!";
	    m_csErrMsg = csMsg ;
	}
    
	return i ;
}


/********************************Return Value************************************/
//       > 0                          Success
//       <=0                          failed
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************/
int CImriDBI::AddSeries(IGT_SeriesField seriesParams, int dwStudyUID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	seriesParams.Protocol[31]='\0';  //避免协议字符串大于32

	//判断外键是否有效
	int iReturn=ExistStudy(dwStudyUID);
	if(iReturn<=0)
	{
		m_csErrMsg.Format("Study UID %i is not valid.",dwStudyUID);
		return -1;
	}

	iReturn=ExistSeries(seriesParams.Id,dwStudyUID);
	if(iReturn>=0)
		return iReturn;

	_RecordsetPtr    pRstSeries = NULL;

    // Define string variables.
	_bstr_t       bstrSQL;
	_variant_t    theValue    ;
	COleDateTime  oleDateTime(1980,1,1,0,0,0) ;
	int         dwUID = -1;

	try
	{
        bstrSQL = "SELECT * FROM SeriesTable";

		TESTHR(pRstSeries.CreateInstance(__uuidof(Recordset)));
        pRstSeries->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
//		if (!(pRstSrudy->EndOfFile))
		{
			pRstSeries->AddNew();

			
			theValue.vt    = VT_I4;
			theValue.lVal = dwStudyUID;
			pRstSeries->Fields->GetItem("Study_Unique_ID")->Value =  theValue;

            theValue.vt    = VT_I2;
			theValue.iVal  = seriesParams.Id;
			pRstSeries->Fields->GetItem("Id")->Value =  theValue;

			pRstSeries->Fields->GetItem("Desc")->Value         =  _bstr_t(seriesParams.Desc) ;
			pRstSeries->Fields->GetItem("Protocol")->Value     =  _bstr_t(seriesParams.Protocol) ;
			

			pRstSeries->Update();
			theValue = pRstSeries->Fields->GetItem("Series_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwUID = theValue.ulVal ;
			}else
			{
				dwUID=-1;
				m_nErrCode=-34;
				m_csErrMsg="Insert record successfully, but can't return Series_Unique_ID";
			}
		}
		pRstSeries->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -34 ;
		dwUID      = -1  ;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwUID;
}

/********************************Return Value************************************/
//       > 0                          Success
//       <=0                          failed
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************/
int CImriDBI::AddStudy(IGT_StudyField studyParams,int dwPatientUID,CString csId,CString csMId)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	CString sStudyID;
	sStudyID.Format("%s",studyParams.Id);

	if(sStudyID!=csId)
	{
		m_nErrCode=-44;
		m_csErrMsg="Study ID from input parameters is different with ID from StudyField.";
		return m_nErrCode;
	}

	//判断外键是否有效
	int iReturn=ExistPatient(dwPatientUID);
	if(iReturn<=0)
	{
		m_csErrMsg.Format("Patient UID %i is not valid.",dwPatientUID);
		return -1;
	}

	iReturn=ExistStudy(sStudyID,csMId);
	if(iReturn>=0)
		return iReturn;

	CString csStudyUniqueID;
	csStudyUniqueID.Format("%s", studyParams.Id );
	
	_RecordsetPtr    pRstStudy = NULL;

    // Define string variables.
	_bstr_t       bstrSQL ;
	CString       csSql ;
	_variant_t    theValue;
	int         dwUID = -1;
	COleDateTime  oleDateTime(1900,1,1,0,0,0) ;

	try
	{
        bstrSQL = "SELECT * FROM StudyTable";

		TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
        pRstStudy->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
//		if (!(pRstSrudy->EndOfFile))
		{
			pRstStudy->AddNew();
			
			
			pRstStudy->Fields->GetItem("Id")->Value                =  _bstr_t(studyParams.Id);

			theValue.vt    = VT_UI4;
			theValue.ulVal = dwPatientUID;
			pRstStudy->Fields->GetItem("Patient_Unique_ID")->Value =  theValue;

			
			if ( strcmp( studyParams.BodyPart,"") != 0 )
			{
				pRstStudy->Fields->GetItem("BodyPart")->Value    =  _bstr_t( studyParams.BodyPart);
			}
			
			pRstStudy->Fields->GetItem("Desc")->Value        =  _bstr_t(studyParams.Desc);
			
			oleDateTime.SetDateTime(studyParams.Time.year,
				                    studyParams.Time.month,
									studyParams.Time.day,
									studyParams.Time.hour,
									studyParams.Time.minute,
									studyParams.Time.second);
			theValue.vt     = VT_DATE ;
			theValue.date   = oleDateTime.m_dt;
			pRstStudy->Fields->GetItem("Time")->Value        =  theValue ;
			pRstStudy->Fields->GetItem("ModalityId")->Value  =  _bstr_t(csMId);

			pRstStudy->Update();

			theValue = pRstStudy->Fields->GetItem("Study_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwUID = theValue.ulVal ;
			}else
			{
				dwUID=-1;
				m_nErrCode=-44;
				m_csErrMsg="Insert record successfully, but can't return Study_Unique_ID";
			}
			
		}
		pRstStudy->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -44 ;
		dwUID      = -1  ;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}

	return dwUID;
}

/********************************Return Value************************************/
//       > 0                          Success
//       <=0                          failed
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************/
int CImriDBI::AddPatient(CString csId, CString csName, CString csBirth, CString csAge,int nSex, 
								 CString csWeight, CString csDesc, CString csMID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	//判断外键是否有效
	int iReturn=ExistModality(csMID);
	if(iReturn<=0)
	{
		m_csErrMsg.Format("SiteID %s is not valid.",csMID);
		return -1;
	}

	//判断主键是否重复
	iReturn=ExistPatient(csId,csMID);
	if(iReturn>=0)
		return iReturn;

	CString patientName=csName;
	int iTestPatientFlag=0;
	patientName.MakeUpper();
	if(patientName.Find("TEST")>=0)
		iTestPatientFlag=1;
	
     _RecordsetPtr    pRstPatient = NULL;

    // Define string variables.
	int         dwUID = -1;
	_bstr_t       bstrSQL;
	CString       csSex;
	_variant_t    theValue;
	COleDateTime  oleDateTime(1980,1,1,0,0,0) ;
	SYSTEMTIME    currentTime ;
	int           nWeight = 100;
	nWeight = atoi(csWeight.GetBuffer(3));

	try
	{
        bstrSQL = "SELECT * FROM PatientTable";
		TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
        pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
//		if (!(pRstPatient->EndOfFile))
		{
			pRstPatient->AddNew();
			
			pRstPatient->Fields->GetItem("Id")->Value     =  _bstr_t(csId) ;
			pRstPatient->Fields->GetItem("Name")->Value   =  _bstr_t(csName) ;
			pRstPatient->Fields->GetItem("Desc")->Value   =  _bstr_t(csDesc) ;
			pRstPatient->Fields->GetItem("Modality_ID")->Value =  _bstr_t(csMID);
			pRstPatient->Fields->GetItem("Age")->Value =  _bstr_t(csAge);
			
			theValue.vt   = VT_I2 ;
			theValue.iVal = (short)nWeight;
			pRstPatient->Fields->GetItem("Weight")->Value = theValue ;

			theValue.vt   = VT_I2 ;
			theValue.iVal = (short)iTestPatientFlag;
			pRstPatient->Fields->GetItem("TestFlag")->Value = theValue ;

			//Set Sex
			if (nSex == 0)
			{
				csSex = "M" ;
			}
			else if (nSex == 1)
			{
				csSex = "F" ;
			}
			else
			{
				csSex = "U" ;
			}
			pRstPatient->Fields->GetItem("Sex")->Value   =  _bstr_t(csSex) ;
			
			//Set Date
			int nDate[3] ;
			StringToDate(csBirth, nDate);
			oleDateTime.SetDateTime(nDate[0],nDate[1],nDate[2],0,0,0);
			theValue.vt     = VT_DATE ;
			theValue.date   = oleDateTime.m_dt;
			pRstPatient->Fields->GetItem("BirthDate")->Value = theValue ;

			GetSystemTime(&currentTime);
				
			
			pRstPatient->Update();
			theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwUID = theValue.ulVal ;
			}else
			{
				dwUID=-1;
				m_nErrCode=-54;
				m_csErrMsg="Insert record successfully, but can't return Patient_Unique_ID";
			}
		}
		pRstPatient->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -54 ;
		dwUID      = -1  ;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return dwUID;
}

/********************************Return Value************************************/
//       > 0                          Success
//       <=0                          failed
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************/
int CImriDBI::UpdatePatiet(CString csId, CString csName, CString csBirth,CString csAge, int nSex, 
							 CString csWeight, CString csDesc, CString csMID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return 0;
		}
	}
	
     _RecordsetPtr    pRstPatient = NULL;

    // Define string variables.
	int         dwUID = -1;
	_bstr_t       bstrSQL;
	CString       csSex, csSQL;
	_variant_t    theValue;
	COleDateTime  oleDateTime(1980,1,1,0,0,0) ;
	int           nWeight = 100;
	nWeight = atoi(csWeight.GetBuffer(3));

	try
	{
		csSQL.Format("SELECT * FROM PatientTable WHERE Id = '%s' AND Modality_ID = '%s'",csId,csMID);
        bstrSQL = csSQL.AllocSysString() ;
		TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
        pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if (!(pRstPatient->EndOfFile))
		{
//			pRstPatient->Fields->GetItem("Id")->Value     =  _bstr_t(csId) ;
			pRstPatient->Fields->GetItem("Name")->Value   =  _bstr_t(csName) ;
			pRstPatient->Fields->GetItem("Desc")->Value   =  _bstr_t(csDesc) ;
			pRstPatient->Fields->GetItem("Age")->Value   =  _bstr_t(csAge) ;
//			pRstPatient->Fields->GetItem("Modality_ID")->Value =  _bstr_t(csMID);
			//Set weight
			theValue.vt   = VT_I2 ;
			theValue.iVal = (short)nWeight;
			pRstPatient->Fields->GetItem("Weight")->Value = theValue ;
			//Set Sex
			if (nSex == 0)
			{
				csSex = "M" ;
			}
			else if (nSex == 1)
			{
				csSex = "F" ;
			}
			else
			{
				csSex = "U" ;
			}
			pRstPatient->Fields->GetItem("Sex")->Value   =  _bstr_t(csSex) ;
			//Set Date
			int nDate[3] ;
			StringToDate(csBirth, nDate);
			oleDateTime.SetDateTime(nDate[0],nDate[1],nDate[2],0,0,0);
			theValue.vt     = VT_DATE ;
			theValue.date   = oleDateTime.m_dt;
			pRstPatient->Fields->GetItem("BirthDate")->Value = theValue ;

			
			pRstPatient->Update();
			theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwUID = theValue.ulVal ;
			}
		}
		pRstPatient->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -54 ;
		dwUID      = -1  ;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return dwUID;
}

/********************************Return Value************************************/
//       > 0                          Success
//       <=0                          failed
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************/
int CImriDBI::AddModality(CString csId, CString csName,CString csType,CString csDesc)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

	if(csId=="")
	{
		m_nErrCode=-64;
		m_csErrMsg="Site ID can not be null string";
		return m_nErrCode;
	}

	int iReturn=ExistModality(csId);
	if(iReturn>=0)
		return iReturn;
	
	
     _RecordsetPtr    pRstModality = NULL;

    // Define string variables.
	int         dwUID = -1;
	_bstr_t       bstrSQL;
	_variant_t    theValue;

	try
	{
        bstrSQL = "SELECT * FROM SiteTable";

		TESTHR(pRstModality.CreateInstance(__uuidof(Recordset)));
        pRstModality->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
//		if (!(pRstPatient->EndOfFile))
		{
			pRstModality->AddNew();
			//pRstModality->Fields->GetItem("Site_Unique_ID")->Value = _bstr_t("0");
			pRstModality->Fields->GetItem("Id")->Value   =  _bstr_t(csId);
			pRstModality->Fields->GetItem("InstName")->Value =  _bstr_t(csName) ;
			pRstModality->Fields->GetItem("type")->Value =  _bstr_t(csType) ;
			pRstModality->Fields->GetItem("Desc")->Value =  _bstr_t(csDesc) ;
			pRstModality->Update();
			theValue = pRstModality->Fields->GetItem("Site_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwUID = theValue.ulVal ;
			}else
			{
				dwUID  = -1 ;
				m_nErrCode=-64;
				m_csErrMsg="Insert record successfully, but can't return Site_Unique_ID";
			}
		}
		pRstModality->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -64 ;
		dwUID      = -1 ;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return dwUID;
}

void CImriDBI::StringToDate(CString csDate, int nDate[])
{
	CString csTep, csLeft;
	csTep = csDate ;
    int nPos = csTep.Find('-',0);
	int nLen = csTep.GetLength();
	if (nPos > 0)
	{
		csLeft   = csTep.Left(nPos);
		nDate[0] = atoi(csLeft.GetBuffer(4));
		csLeft   = csTep.Right(nLen-nPos-1);
	}
	else
	{
		nDate[0] = 1980 ;
	}

	csTep = csLeft ;
	nPos = csTep.Find('-',0);
	nLen = csTep.GetLength();
	if (nPos > 0)
	{
		csLeft   = csTep.Left(nPos);
		nDate[1] = atoi(csLeft.GetBuffer(2));
		csLeft   = csTep.Right(nLen-nPos-1);
	}
	else
	{
		nDate[1] = 1 ;
	}

	nDate[2] = atoi(csLeft.GetBuffer(2));
}

int CImriDBI::QueryByStudyId(CString csId, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.ID='%s' and ",sTmp,csId);
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.ID='%s' and ",sTmp,csId);
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}

int CImriDBI::QueryByStudyDesc(CString csDesc, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.Desc='%s' and ",sTmp,csDesc);
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.Desc='%s' and ",sTmp,csDesc);
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}

int CImriDBI::QueryByBodyPart(CString csBodyPart, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.BodyPart='%s' and ",sTmp,csBodyPart);
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where StudyTable.BodyPart='%s' and ",sTmp,csBodyPart);
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}

int CImriDBI::QueryByStudyTime(COleDateTime tmFrom,COleDateTime tmTo, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;

	COleDateTime tmTemp(tmFrom.GetYear(),tmFrom.GetMonth(),tmFrom.GetDay(), 0,0,0);
	COleDateTime tmTemp1(tmTo.GetYear(),tmTo.GetMonth(),tmTo.GetDay(), 23,59,59);
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where Time >= #%s# AND Time <= #%s# and ",sTmp,tmTemp.Format( "%Y-%m-%d %H:%M:%S" ), tmTemp1.Format( "%Y-%m-%d %H:%M:%S" ) );
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where Time >= #%s# AND Time <= #%s# and ",sTmp,tmTemp.Format( "%Y-%m-%d %H:%M:%S" ), tmTemp1.Format( "%Y-%m-%d %H:%M:%S" ) );
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;

}

int CImriDBI::QueryByName(CString csName, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where PatientTable.Name='%s' and ",sTmp,csName);
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where PatientTable.Name='%s' and ",sTmp,csName);
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}

int CImriDBI::QueryByPatientID(CString csPId, CListCtrl &listCtrl)
{
	CString    csSQL;
	int        i      = -1 ;
	
	if(m_bQueryTest)
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where PatientTable.ID='%s' and ",sTmp,csPId);
		csSQL=sTmp1 + " PatientTable.TestFlag=1 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}else
	{
		CString sTmp,sTmp1;
		sTmp= "SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable ";
		sTmp1.Format("%s Where PatientTable.ID='%s' and ",sTmp,csPId);
		csSQL=sTmp1 + " PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC" ;
	}
	

	
	i=OpenStudyTableBySql(listCtrl,csSQL);
	return i ;
}


CString CImriDBI::QueryPatientId(CString csSId, CString csMId)
{
	CString    csPId("") ;
	CString    csSQL ;
	_bstr_t	   bstrSQL;
	_variant_t theValue;
	long       lUID = 0;

    if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}

    _RecordsetPtr    pRstStudy    = NULL;
    _RecordsetPtr    pRstPatient  = NULL;

	csSQL.Format("SELECT Patient_Unique_ID FROM StudyTable WHERE Id='%s' AND ModalityId='%s'",
		         csSId, csMId);

	bstrSQL = csSQL.AllocSysString() ;
	TESTHR( pRstStudy.CreateInstance(__uuidof(Recordset)) );
	pRstStudy->Open( bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
					 adLockOptimistic, adCmdText);
	try
	{
		if (!(pRstStudy->EndOfFile))
		{
			theValue = pRstStudy->Fields->GetItem("Patient_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
				lUID = theValue.lVal;
				csSQL.Format("SELECT Id FROM PatientTable WHERE Patient_Unique_ID=%d",lUID);
				bstrSQL = csSQL.AllocSysString() ;
				TESTHR( pRstPatient.CreateInstance(__uuidof(Recordset)) );
				pRstPatient->Open( bstrSQL, _variant_t((IDispatch *)m_pConnection,true),
								   adOpenDynamic,adLockOptimistic, adCmdText);
				if (!(pRstPatient->EndOfFile))
				{
					theValue = pRstPatient->Fields->GetItem("Id")->Value ;
					if (theValue.vt != VT_NULL)
					{
						csPId = (char*)_bstr_t(theValue);
					}
				}
				pRstPatient->Close();
			}
		}
		pRstStudy->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		m_nErrCode    = -62   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
		PanaMessageBox(m_csErrMsg);
	}
	return csPId ;
}
/********************************Return Value************************************************/
//       =0  : nErrCode == 0---------Success: there is no item in the table. 
//       =0  : nErrCode <  0---------Failed :                             
//       >0                          Success: there is item in the table. the return is UID.
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************************/
int CImriDBI::ExistModality(CString csMId)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstSite  = NULL;

    // Define string variables.
	int       dwMUId = -1 ;
	_bstr_t     bstrSQL;
	CString     csSQL ;
	_variant_t  theValue;

	try
	{
		//Get Series Unique ID 
		csSQL.Format("SELECT * FROM SiteTable WHERE Id='%s'",csMId);
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstSite.CreateInstance(__uuidof(Recordset)));
        pRstSite->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if(!(pRstSite->EndOfFile))
		{
			theValue = pRstSite->Fields->GetItem("Site_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwMUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwMUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstSite->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwMUId        = -1 ;
		m_nErrCode    = -62   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}

	return dwMUId ;
}

/********************************Return Value************************************************/
//       =0  : nErrCode == 0---------Success: there is no item in the table. 
//       =0  : nErrCode <  0---------Failed :                             
//       >0                          Success: there is item in the table. the return is UID.
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************************/
int CImriDBI::ExistPatient(CString csPId,CString csMId)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";
	
     _RecordsetPtr    pRstPatient = NULL;

    // Define string variables.
	int         dwPUId = -1;
	_bstr_t       bstrSQL;
	CString       csSql;
	_variant_t    theValue;

    try
	{
		csSql.Format("SELECT * FROM PatientTable WHERE Id='%s' AND Modality_ID='%s'",csPId,csMId);
		bstrSQL = csSql.AllocSysString() ;
		TESTHR(pRstPatient.CreateInstance(__uuidof(Recordset)));
        pRstPatient->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);

		if(!(pRstPatient->EndOfFile))
		{
			theValue = pRstPatient->Fields->GetItem("Patient_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwPUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwPUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstPatient->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwPUId        = -1 ;
		m_nErrCode    = -52   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwPUId ;
}

/********************************Return Value************************************************/
//       =0  : nErrCode == 0---------Success: there is no item in the table. 
//       =0  : nErrCode <  0---------Failed :                             
//       >0                          Success: there is item in the table. the return is UID.
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************************/
int CImriDBI::ExistStudy(int dwStudyUID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstStudy  = NULL;

    // Define string variables.
	int       dwSUId = -1 ;
	_bstr_t     bstrSQL;
	CString     csSQL ;
	_variant_t  theValue;

	try
	{
		//Get Series Unique ID 
		csSQL.Format("SELECT * FROM StudyTable WHERE Study_Unique_ID=%d",dwStudyUID);
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
        pRstStudy->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if(!(pRstStudy->EndOfFile))
		{
			theValue = pRstStudy->Fields->GetItem("Study_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwSUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwSUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstStudy->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwSUId        = -1 ;
		m_nErrCode    = -42   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwSUId ;
}

/********************************Return Value************************************************/
//       =0  : nErrCode == 0---------Success: there is no item in the table. 
//       =0  : nErrCode <  0---------Failed :                             
//       >0                          Success: there is item in the table. the return is UID.
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************************/
int CImriDBI::ExistPatient(int dwPatientUID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstStudy  = NULL;

    // Define string variables.
	int       dwSUId = -1 ;
	_bstr_t     bstrSQL;
	CString     csSQL ;
	_variant_t  theValue;

	try
	{
		//Get Series Unique ID 
		csSQL.Format("SELECT * FROM PatientTable WHERE Patient_Unique_ID=%d",dwPatientUID);
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
        pRstStudy->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if(!(pRstStudy->EndOfFile))
		{
			theValue = pRstStudy->Fields->GetItem("Patient_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwSUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwSUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstStudy->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwSUId        = -1 ;
		m_nErrCode    = -42   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwSUId ;
}

/********************************Return Value************************************************/
//       =0  : nErrCode == 0---------Success: there is no item in the table. 
//       =0  : nErrCode <  0---------Failed :                             
//       >0                          Success: there is item in the table. the return is UID.
//       if failed,call GetErrMsg() to get error message. 
/********************************************************************************************/
int CImriDBI::ExistSeries(int csSeriesID ,int dwStudyID)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstSeries  = NULL;

    // Define string variables.
	int       dwSUId = -1 ;
	_bstr_t     bstrSQL;
	CString     csSQL ;
	_variant_t  theValue;

	try
	{
		//Get Series Unique ID 
		csSQL.Format("SELECT * FROM SeriesTable WHERE Id=%d AND Study_Unique_ID=%d",csSeriesID,dwStudyID);
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstSeries.CreateInstance(__uuidof(Recordset)));
        pRstSeries->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if(!(pRstSeries->EndOfFile))
		{
			theValue = pRstSeries->Fields->GetItem("Series_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwSUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwSUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstSeries->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwSUId        = -1 ;
		m_nErrCode    = -32   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwSUId ;
}


//Globals
int gBubleSortEx(int naSort[], int nCount, CString csString[], bool bAsc )
{
	int i, nPass, nTemp;
	CString csTemp;
    for (i=0; i<nCount; i++)
	{
		naSort[i] = i ;
	}

	if (bAsc)
	{
		for (nPass=0; nPass<nCount-1; nPass++)
		{
			for (i=nPass; i<nCount-1; i++)
			{
				if(csString[nPass].Compare(csString[i+1]) > 0)
				{
					csTemp          = csString[nPass] ;
					csString[nPass] = csString[i+1];
					csString[i+1]   = csTemp ;
					nTemp           = naSort[nPass] ;
					naSort[nPass]   = naSort[i+1] ;
					naSort[i+1]     = nTemp ;
				}
			}
		}
	}
	else
	{
		for (nPass=0; nPass<nCount-1; nPass++)
		{
			for (i=nPass; i<nCount-1; i++)
			{
				if(csString[nPass].Compare(csString[i+1]) < 0)
				{
					csTemp          = csString[nPass] ;
					csString[nPass] = csString[i+1];
					csString[i+1]   = csTemp ;
					nTemp           = naSort[nPass] ;
					naSort[nPass]   = naSort[i+1] ;
					naSort[i+1]     = nTemp ;
				}
			}
		}
	}
	return nPass ;
}

int gBubleSortDateEx(int naSort[], int nCount, CString csString[], bool bAsc )
{
	int i, nPass, nTemp;
	CString csTemp ;
	DWORD  dwTemp;
	DWORD* pDate ;
	pDate = new DWORD[nCount];
    for (i=0; i<nCount; i++)
	{
		naSort[i] = i ;
		pDate[i]  = gDateToInt(csString[i]);
	}

	if (bAsc)
	{
		for (nPass=0; nPass<nCount-1; nPass++)
		{
			for (i=nPass; i<nCount-1; i++)
			{
				if(pDate[nPass] > pDate[i+1])
				{
					dwTemp       = pDate[nPass] ;
					pDate[nPass] = pDate[i+1];
					pDate[i+1]   = dwTemp ;
					nTemp           = naSort[nPass] ;
					naSort[nPass]   = naSort[i+1] ;
					naSort[i+1]     = nTemp ;
					csTemp          = csString[nPass] ;
					csString[nPass] = csString[i+1];
					csString[i+1]   = csTemp ;
				}
			}
		}
	}
	else
	{
		for (nPass=0; nPass<nCount-1; nPass++)
		{
			for (i=nPass; i<nCount-1; i++)
			{
				if(pDate[nPass] < pDate[i+1])
				{
					dwTemp       = pDate[nPass] ;
					pDate[nPass] = pDate[i+1];
					pDate[i+1]   = dwTemp ;
					nTemp           = naSort[nPass] ;
					naSort[nPass]   = naSort[i+1] ;
					naSort[i+1]     = nTemp ;
					csTemp          = csString[nPass] ;
					csString[nPass] = csString[i+1];
					csString[i+1]   = csTemp ;
				}
			}
		}
	}
	delete[] pDate;

	return nPass ;
}

int gDateToInt(CString csDate)
{
	CString csTemp;
	csTemp = csDate;
    CString csLeft;
	int nYear  = 1885;
	int nMonth = 1 ;
	int nDay   = 1 ;
	int nHour  = 0 ;
	int nMin   = 0 ;

	int nLen = csTemp.GetLength();
	int nPos = csTemp.Find('-',0);
	
	if (nPos == 4)
	{
		csLeft = csTemp.Left(nPos);
		nYear = atoi(csLeft.GetBuffer(4));
		csLeft = csTemp.Right(nLen-nPos-1);
	}

	csTemp = csLeft ;
	nPos = csTemp.Find('-',0);
	nLen = csTemp.GetLength();
	if ((nPos == 1)||(nPos == 2))
	{
		csLeft = csTemp.Left(nPos);
		nMonth = atoi(csLeft.GetBuffer(2));
		csLeft = csTemp.Right(nLen-nPos-1);
	}

	csTemp = csLeft ;
	nPos = csTemp.Find(' ',0);
	nLen = csTemp.GetLength();
	if ((nPos == 1)||(nPos == 2))
	{
		csLeft = csTemp.Left(nPos);
		nDay = atoi(csLeft.GetBuffer(2));
		csLeft = csTemp.Right(nLen-nPos-1);
	}

    csTemp.TrimLeft();
	csTemp = csLeft ;
	nPos = csTemp.Find(':',0);
	nLen = csTemp.GetLength();
	if ((nPos == 1)||(nPos == 2))
	{
		csLeft = csTemp.Left(nPos);
		nHour = atoi(csLeft.GetBuffer(2));
		csLeft = csTemp.Right(nLen-nPos-1);
	}

	nMin = atoi(csLeft.GetBuffer(2));
	
	return (nYear-1800)*535680 + nMonth*44640 + nDay*1440 + nHour*60 + nMin;
}

int gCheckDateFormat(CString csDate, int &nY, int &nM, int &nD)
{
	int nPos1, nPos2,nLen;
	int nFormat = 0 ;
//	int nY, nM, nD ;
	CString csY, csM, csD, csTemp, csTemp1;

    //Is dot format?
	nLen = csDate.GetLength();
	nPos1 = csDate.Find(".",0);
	if (nPos1 > 0)
	{
		csY     = csDate.Left(nPos1);
		csTemp  = csDate.Right(nLen - nPos1 - 1);
		nLen    = csTemp.GetLength();
		nPos2   = csTemp.Find(".",0);
		csM     = csTemp.Left(nPos2);
		csD     = csDate.Right(nLen - nPos2 - 1);

		nFormat = 1 ;
	}
	//Is - format
	nPos1 = csDate.Find("-",0);
	if (nPos1 > 0)
	{
		csY     = csDate.Left(nPos1);
		csTemp  = csDate.Right(nLen - nPos1 - 1);
		nLen    = csTemp.GetLength();
		nPos2   = csTemp.Find("-",0);
		csM     = csTemp.Left(nPos2);
		csD     = csDate.Right(nLen - nPos2 - 1);
		
		nFormat = 2 ;
	}

	//Is other format
	if (nFormat == 0)
	{
		if ( nLen != 8 )
		{
			return -1;
		}
		else
		{
			csY     = csDate.Left(4);
			csTemp  = csDate.Right(nLen - 4);
			csM     = csTemp.Left(2);
			csD     = csTemp.Right(2);
		}
	}

	nLen = csY.GetLength() ;
	nY   = atoi(csY.GetBuffer(nLen));
	if ((nY < 1900)||(nY > 2020))
	{
		return -1;
	}

	nLen = csM.GetLength() ;
	nM   = atoi(csM.GetBuffer(nLen));
	if ((nM < 1)||(nM > 12))
	{
		return -1;
	}

	nLen = csD.GetLength() ;
	nD   = atoi(csD.GetBuffer(nLen));
	if ((nM == 1)||(nM == 3)||(nM == 5)||(nM == 7)||(nM == 8)||(nM == 10)||(nM == 12))
	{
		if ((nD < 1)||(nD > 31))
		{
			return -1;
		}
	}
	else if ((nM == 4)||(nM == 6)||(nM == 9)||(nM == 10))
	{
		if ((nD < 1)||(nD > 30))
		{
			return -1;
		}
	}
	else
	{
		if (nY%4 == 0)
		{
			if ((nD < 1)||(nD > 29))
			{
				return -1;
			}
		}
		else
		{
			if ((nD < 1)||(nD > 28))
			{
				return -1;
			}
		}
	}
	return nFormat;
}

BOOL CImriDBI::Validate()
{
	
	
    CString sFolder,sFile;
    HKEY hKey;

	// get database folder from registry
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,  _T("SOFTWARE\\XinaoMDT\\Panasee\\Database"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("Folder"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            sFolder.Format("%s",szData);
        }
		else
		{
			RegCloseKey(hKey);
			return FALSE;
		}
        RegCloseKey(hKey);
    }
	else
	{
		return FALSE;
	}

	//get database file folder from registry
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,  _T("SOFTWARE\\XinaoMDT\\Panasee\\Database"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("File"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            sFile.Format("%s",szData);
        }
		else
		{
			RegCloseKey(hKey);
			return FALSE;
		}
        RegCloseKey(hKey);
    }
	else
	{
		return FALSE;
	}

	//check if database folder exists.
	WIN32_FIND_DATA wfd;
    BOOL rValue = FALSE;
	BOOL bFind=FALSE;
    HANDLE hFind = FindFirstFile(sFolder, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE) && 
         (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
    {
        bFind = TRUE;
    }
    FindClose(hFind);
    if(!bFind)
		return FALSE;
	
	SetDBPathName(sFile);

	if(ConnectDB())
		if(OpenTableTest()>0)
			return TRUE;
		else
		{
			DisconnectDB();
			return FALSE;
		}
	else
	{
		return FALSE;
	}
	
    return FALSE;

}

int CImriDBI::OpenTableTest()
{
	return 1; //don't try to open sitetable.

	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
     _RecordsetPtr    pRstSite  = NULL;

    // Define string variables.
	_bstr_t    bstrCnn, bstrSQL;
	CString    csConn, csID, csModalityName, csType,csModalityDesc;
	int        i = -1;
	
	

	try
	{
		//Get Series Unique ID 
		CString csSQL = "SELECT * FROM SiteTable ORDER BY Id ASC";
        bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstSite.CreateInstance(__uuidof(Recordset)));
        pRstSite->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);

        _variant_t theValue;
		while (!(pRstSite->EndOfFile))
		{
            //Get Id
			theValue = pRstSite->Fields->GetItem("Id")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csID = (char*)_bstr_t(theValue);
			}
			//Get Modality Name
			theValue = pRstSite->Fields->GetItem("InstName")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csModalityName = (char*)_bstr_t(theValue);
			}
			//Get Modality Type
			theValue = pRstSite->Fields->GetItem("type")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csType = (char*)_bstr_t(theValue);
			}

			theValue = pRstSite->Fields->GetItem("Desc")->Value ;
			if (theValue.vt != VT_NULL)
			{
				csModalityDesc = (char*)_bstr_t(theValue);
			}
			
			i++ ;
			pRstSite->MoveNext();

			
		}
        i++ ;
        pRstSite->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		i = -1;
		m_nErrCode = -62  ;
		CString csMsg("Cannot open id = ");
		csMsg += csID;
		csMsg += " item in Site table!";
	    m_csErrMsg = csMsg ;
	}
    
	return i ;
}

BOOL CImriDBI::Validate(CString sFile,CString sFolder)
{
	//check if database folder exists.
	WIN32_FIND_DATA wfd;
    BOOL rValue = FALSE;
	BOOL bFind=FALSE;
    HANDLE hFind = FindFirstFile(sFolder, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE) && 
         (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
    {
        bFind = TRUE;
    }
    FindClose(hFind);
    if(!bFind)
		return FALSE;
	
	SetDBPathName(sFile);

	if(ConnectDB())
		if(OpenTableTest()>0)
			return TRUE;
		else
		{
			DisconnectDB();
			return FALSE;
		}
	else
	{
		return FALSE;
	}
	
    return FALSE;
}

void CImriDBI::GetDBPath(CString &sFile, CString &sFolder)
{
	HKEY hKey;
	sFile="";
	sFolder="";
	// get database folder from registry
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,  _T("SOFTWARE\\XinaoMDT\\Panasee\\Database"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("Folder"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            sFolder.Format("%s",szData);
        }
		
        RegCloseKey(hKey);
    }
	

	//get database file folder from registry
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,  _T("SOFTWARE\\XinaoMDT\\Panasee\\Database"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("File"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            sFile.Format("%s",szData);
        }
		
        RegCloseKey(hKey);
    }
	
}

BOOL CImriDBI::SaveDBPath(CString sFile, CString sFolder)
{
	
	
    CRegistry reg;
	if(!reg.CreateKey(_T("SOFTWARE\\XinaoMDT\\Panasee\\Database")))
	{
		PanaMessageBox("Can't open registry table");
		return FALSE;
	}
	if(!reg.Write(_T("File"),sFile))
	{
		PanaMessageBox("Can't open registry table");
		return FALSE;
	}
	if(!reg.Write(_T("Folder"),sFolder))
	{
		PanaMessageBox("Can't open registry table");
		return FALSE;
	}
	return TRUE;
	
}

CString CImriDBI::GetDatabaseFolder()
{
	CString sFolder="";
    HKEY hKey;

	// get database folder from registry
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,  _T("SOFTWARE\\XinaoMDT\\Panasee\\Database"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("Folder"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            sFolder.Format("%s",szData);
        }
		
        RegCloseKey(hKey);
    }
	
	return sFolder;
}

void CImriDBI::ClearDatabase()
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return;
		}
	}

	m_nErrCode=0;
	m_csErrMsg="";

     _CommandPtr    pCommand = NULL;

    // Define string variables.
	

	try
	{
        
		TESTHR(pCommand.CreateInstance(__uuidof(Command)));
		pCommand->ActiveConnection=m_pConnection;
		
		
		pCommand->CommandText="Delete * FROM SiteTable";
		pCommand->Execute(NULL,NULL,adCmdText);

		pCommand->CommandText="Delete * FROM PatientTable";
		pCommand->Execute(NULL,NULL,adCmdText);

		
		pCommand->CommandText="Delete * FROM SeriesTable";
		pCommand->Execute(NULL,NULL,adCmdText);

		pCommand->CommandText="Delete * FROM StudyTable";
		pCommand->Execute(NULL,NULL,adCmdText);
		    
		
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -14 ;
		
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return;
}

int CImriDBI::ExistStudy(CString csSId, CString csMId)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstStudy  = NULL;

    // Define string variables.
	int       dwSUId = -1 ;
	_bstr_t     bstrSQL;
	CString     csSQL ;
	_variant_t  theValue;

	try
	{
		csSQL.Format("SELECT * FROM StudyTable WHERE Id='%s' AND ModalityId='%s'",csSId,csMId);
		bstrSQL = csSQL.AllocSysString() ;

		//csSQL.Format("SELECT * FROM StudyTable WHERE Id='%s' and ModalityId='%s'",csSId,csMId);
       // bstrSQL        = csSQL.AllocSysString();

		TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
        pRstStudy->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		if(!(pRstStudy->EndOfFile))
		{
			theValue = pRstStudy->Fields->GetItem("Study_Unique_ID")->Value ;
			if (theValue.vt != VT_NULL)
			{
			    dwSUId = theValue.ulVal ;
			}
			m_nErrCode = 0 ;
		}
		else
		{
			dwSUId      = -1  ;
			m_nErrCode  = 0  ;
		}
        pRstStudy->Close();
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_pConnection = NULL;
		dwSUId        = -1 ;
		m_nErrCode    = -42   ;
		CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	return dwSUId ;
}

BOOL CImriDBI::ExecuateSQL(CString sSql,_RecordsetPtr  &pRst)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return FALSE;
		}
	}

	m_nErrCode=0;
	m_csErrMsg="";
	
     //_RecordsetPtr    pRst ;
	 _CommandPtr	  pCommand;

    // Define string variables.
	BOOL         bReturn=FALSE;
	_bstr_t       bstrSQL;
	_variant_t    theValue;

	try
	{
        
		TESTHR(pCommand.CreateInstance(__uuidof(Command)));
		// 将库连接赋于它
		pCommand->ActiveConnection = m_pConnection;  
		// SQL语句
		pCommand->CommandText = _bstr_t(sSql);
		// 执行SQL语句，返回记录集
		pRst = pCommand->Execute(NULL, NULL,adCmdText); 
		bReturn=TRUE;
		
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -64 ;
		bReturn=FALSE;
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return bReturn;
}

BOOL CImriDBI::GetRecordSet(CString sSql, _RecordsetPtr &pRst)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return FALSE;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";
	
    
	try
	{
        TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
        pRst->Open(_bstr_t(sSql), _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
                          adLockOptimistic, adCmdText);
		
		return TRUE;
	}
	catch (_com_error e)
	{
		DisconnectDB();
		m_nErrCode = -64 ;
		
        CString mes1,mes2,mes3,mes4;
        mes1.Format("Error:%08lx.\n", e.Error());
        mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
        mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
        mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
        m_csErrMsg = mes1 + mes2 + mes3 +mes4 ;
	}
	
	return FALSE;
}

void CImriDBI::SetQueryTestFlag(BOOL bQueryTest)
{
	m_bQueryTest=bQueryTest;
}

int CImriDBI::OpenStudyTableBySql(CListCtrl &listCtrl, CString sSql)
{
	if (m_pConnection == NULL)
	{
		if (!ConnectDB())
		{
		    return -1;
		}
	}
	m_nErrCode=0;
	m_csErrMsg="";

     _RecordsetPtr    pRstStudy    = NULL;

    // Define string variables.
	_bstr_t    bstrCnn, bstrSQL;
	CString    csId,csName,csTime,csDesc,csTemp, csBodyPart, csModalityNo;
	CString    csSQL ,csPatient_Unique_ID, csType;
	int        i      = -1 ;
	int        nIndex = 0;
	LV_ITEM    lvitem ;
	_variant_t theValue;

	csSQL=sSql;
	//csSQL="SELECT StudyTable.Id,PatientTable.Name,StudyTable.Desc,StudyTable.Time,StudyTable.BodyPart,SiteTable.ID,SiteTable.type FROM StudyTable,PatientTable,SiteTable Where PatientTable.ID='1098' and PatientTable.TestFlag=0 and StudyTable.Patient_Unique_ID=PatientTable.Patient_Unique_ID and StudyTable.ModalityId=SiteTable.ID ORDER BY StudyTable.Id DESC";

		try
		{
			
			//Get All Study
			bstrSQL  = csSQL.AllocSysString();
			TESTHR(pRstStudy.CreateInstance(__uuidof(Recordset)));
			pRstStudy->Open(bstrSQL, _variant_t((IDispatch *)m_pConnection,true), adOpenDynamic,
							adLockOptimistic, adCmdText);
			while (!(pRstStudy->EndOfFile))
			{
				//Get Id
				theValue = pRstStudy->Fields->GetItem("StudyTable.Id")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csId = (char*)_bstr_t(theValue);
				}else
				{
					csId="";
				}
				
				//Get Patient_Unique_ID
				theValue = pRstStudy->Fields->GetItem("Name")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csName = (char*)_bstr_t(theValue);
				}else
				{
					csName="";
				}

				theValue = pRstStudy->Fields->GetItem("SiteTable.ID")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csModalityNo = (char*)_bstr_t(theValue);
				}else
				{
					csModalityNo="";
				}

				theValue = pRstStudy->Fields->GetItem("Type")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csType = (char*)_bstr_t(theValue);
				}else
				{
					csType="";
				}
                
				//Get Body Part
				theValue = pRstStudy->Fields->GetItem("BodyPart")->Value ;
				if (theValue.vt != VT_NULL)
				{
					if ( theValue.vt != 1 )
						csBodyPart = (char*)_bstr_t(theValue);
					else
						csBodyPart = "";
				}else
				{
					csBodyPart="";
				}

				//Get Date and Time
				theValue = pRstStudy->Fields->GetItem("Time")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csTime = (char*)_bstr_t(theValue);

					struct tm tttt;
					sscanf( csTime.GetBuffer(255), "%d-%d-%d", // %d:%d:%d
						&tttt.tm_year, &tttt.tm_mon, &tttt.tm_mday/*, &tttt.tm_hour, &tttt.tm_min, &tttt.tm_sec*/ );
								
					tttt.tm_year -= 1900;
					tttt.tm_mon -= 1;
					tttt.tm_hour = 0;
					tttt.tm_min = 0;
					tttt.tm_sec = 0;
					time_t tm = mktime( &tttt );
					CTime oTime( tm/*tttt.tm_year, tttt.tm_mon, tttt.tm_mday, tttt.tm_hour, tttt.tm_min, tttt.tm_sec*/ );
					CString sDate;
					sDate = oTime.Format( "%Y-%m-%d %H:%M:%S" );
					sDate = sDate;
				}else
				{
					csTime="";
				}

				//Get Description
				theValue = pRstStudy->Fields->GetItem("Desc")->Value ;
				if (theValue.vt != VT_NULL)
				{
					csDesc = (char*)_bstr_t(theValue);
				}else
				{
					csDesc="";
				}
				
				
				i++ ;
				pRstStudy->MoveNext();
				
				nIndex = listCtrl.GetItemCount();
				lvitem.mask     = LVIF_TEXT ;
				lvitem.iItem    = nIndex ;
				lvitem.iSubItem = 0 ;
				lvitem.pszText  = (char*)(LPCTSTR)csId ;
				listCtrl.InsertItem(&lvitem);
				listCtrl.SetItemText(nIndex,1,(char*)(LPCTSTR)csType);
				listCtrl.SetItemText(nIndex,2,(char*)(LPCTSTR)csModalityNo);
				listCtrl.SetItemText(nIndex,3,(char*)(LPCTSTR)csName);			
				listCtrl.SetItemText(nIndex,4,csBodyPart);
				listCtrl.SetItemText(nIndex,5,csTime);
				listCtrl.SetItemText(nIndex,6,csDesc);			
			}  //while (!(pRstStudy->EndOfFile))
			i++ ;
			pRstStudy->Close();
		}  //end try .
		catch (_com_error e)
		{
			DisconnectDB();
			m_nErrCode = -42  ;
            CString mes1,mes2,mes3,mes4;
            mes1.Format("Error:%08lx.\n", e.Error());
            mes2.Format("ErrorMessage:%s.\n", e.ErrorMessage());
            mes3.Format("Source:%s.\n", (LPCTSTR) _bstr_t(e.Source()));
            mes4.Format("Description:%s.\n", (LPCTSTR) _bstr_t(e.Description()));
            m_csErrMsg = mes1 + mes2 + mes3 + mes4;
			return -3 ;
		}
	

	return i ;
}
