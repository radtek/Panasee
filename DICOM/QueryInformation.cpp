#include "Query.h"
#include "QueryInformation.h"


CQueryInformation::CQueryInformation ()
{
}

CQueryInformation::~CQueryInformation ()
{
}
int CQueryInformation::Execute (DICOMDataObject *DDO, int nLevelNumber)
{
	MyQueryRetrievePDU PDU;

	PDU.ClearAbstractSyntaxs();
	PDU.ClearPresentationContexts();

	UID	PatientRootUID,StudyRootUID, TransferUID, Appuid("1.2.840.10008.3.1.1.1");
	PDU.SetApplicationContext(Appuid);
	PDU.SetQueryOrRetrieve(TRUE);  //TRUE means the current operation is Query

	SOPMyPatientRootQuery.GetUID(PatientRootUID);
	PDU.AddAbstractSyntax(PatientRootUID);
	SOPMyStudyRootQuery.GetUID(StudyRootUID);
	PDU.AddAbstractSyntax(StudyRootUID);

	PDU.SetLocalAddress((BYTE *)strDICOMLocalAETitle);
	PDU.SetRemoteAddress((BYTE*)strDICOMRemoteAETitle);

	//PDU Connect
	PDU.SetTimeOut(10);//10 seconds//zxh
	if(!PDU.Connect((BYTE *)strDICOMRemoteIP, (BYTE *)strRemotePort)) 
	{
		switch(PDU.PDUGetLastError()) {
		case PDU_CONNECT_NOINTEROGATEAASSOCIATEAC:
			return PDU_CONNECT_NOINTEROGATEAASSOCIATEAC;
			break;
		case PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN:
			return PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN;
			break;
		case PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED:
			return PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED:
			return PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION:
			return PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION;
			break;
		case PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED:
			return PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN:
			return PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED:
			return PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED:
			return PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION:
			return PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED:
			return PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED;
			break;
		default:
			return -1;
		}
	}

	if (PDU.IsAbstractSyntaxAccepted(PatientRootUID, TransferUID))
	{
		SOPMyPatientRootQuery.SetLevelNumber(nLevelNumber);
//		SOPMyPatientRootQuery.RemoveArray();
		if(!SOPMyPatientRootQuery.Write(&PDU, DDO))
		{
			switch(SOPMyPatientRootQuery.GetLastError()) 
			{
				case QUERY_REFUSED_OUTOFRESOURCE:
					return QUERY_REFUSED_OUTOFRESOURCE;
					break;
				case QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS:
					return QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS;
					break;
				case QUERY_FAILED_UNABLETOPROCESS:
					return QUERY_FAILED_UNABLETOPROCESS;
					break;
				case QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST:
					return QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST;
					break;
			}
		}
		m_pPatientArray = SOPMyPatientRootQuery.GetPatientArray();
	}
	else if (PDU.IsAbstractSyntaxAccepted(StudyRootUID, TransferUID))
	{
		SOPMyStudyRootQuery.SetLevelNumber(nLevelNumber);
//		SOPMyStudyRootQuery.RemoveArray();
		if(!SOPMyStudyRootQuery.Write(&PDU, DDO))
		{
			switch(SOPMyStudyRootQuery.GetLastError()) 
			{
				case QUERY_REFUSED_OUTOFRESOURCE:
					return QUERY_REFUSED_OUTOFRESOURCE;
					break;
				case QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS:
					return QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS;
					break;
				case QUERY_FAILED_UNABLETOPROCESS:
					return QUERY_FAILED_UNABLETOPROCESS;
					break;
				case QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST:
					return QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST;
					break;
			}
		}
		m_pPatientArray = SOPMyStudyRootQuery.GetPatientArray();
	}

	PDU.Close();		
	return 0;
}
int CQueryInformation::QueryModalityWorkListInformation(WorkListInfo*pWorkList)
{
	DICOMDataObject DDOWorkList;
	VR*vr;

/*	char * cSpecificCharacterSet = new char[MAX_VRCS*4];
	strcpy(cSpecificCharacterSet,pWorkList->m_strSpecificCharacterSet);
	vr = new VR(0x0008, 0x0005, strlen(cSpecificCharacterSet), (void *)cSpecificCharacterSet);
 	DDOWorkList.Push(vr);
*/
	char * cPatientName = new char[MAX_VRPN];
	memset(cPatientName,0,MAX_VRPN);
	strcpy(cPatientName, pWorkList->m_strPatientName);
	vr = new VR(0x0010, 0x0010, strlen(cPatientName), (void *)cPatientName);
   	DDOWorkList.Push(vr);

	char * cPatientID = new char[MAX_VRLO];
	memset(cPatientID,0,MAX_VRLO);
	strcpy(cPatientID, pWorkList->m_strPatientID);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDOWorkList.Push(vr);

	char * cOtherPatientID = new char[MAX_VRLO];
	memset(cOtherPatientID,0,MAX_VRLO);
	strcpy(cOtherPatientID, pWorkList->m_strOtherPatientID);
	vr = new VR(0x0010, 0x1000, strlen(cOtherPatientID), (void *)cOtherPatientID);
   	DDOWorkList.Push(vr);

	char * cStudyInstanceUID = new char[MAX_VRUI];
	memset(cStudyInstanceUID,0,MAX_VRUI);
	strcpy(cStudyInstanceUID, pWorkList->m_strStudyInstanceUID);
	vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   	DDOWorkList.Push(vr);

	char * strScheduledStationAET = new char[50];
	memset(strScheduledStationAET,0,50);
	strcpy(strScheduledStationAET, pWorkList->strScheduledStationAET);
	vr = new VR(0x0040, 0x0001, strlen(strScheduledStationAET), (void *)strScheduledStationAET);
   	DDOWorkList.Push(vr);

	char * m_timMaxScheduledStudyDate = new char[50];
	memset(m_timMaxScheduledStudyDate,0,50);
	strcpy(m_timMaxScheduledStudyDate, pWorkList->m_timMaxScheduledStudyDate);
	vr = new VR(0x0040, 0x0002, strlen(m_timMaxScheduledStudyDate), (void *)m_timMaxScheduledStudyDate);
   	DDOWorkList.Push(vr);

	char * m_timMaxScheduledStudyTime = new char[50];
	memset(m_timMaxScheduledStudyTime,0,50);
	strcpy(m_timMaxScheduledStudyTime, pWorkList->m_timMaxScheduledStudyTime);
	vr = new VR(0x0040, 0x0003, strlen(m_timMaxScheduledStudyTime), (void *)m_timMaxScheduledStudyTime);
   	DDOWorkList.Push(vr);

	char * strScheduledProcedureStepID = new char[50];
	memset(strScheduledProcedureStepID,0,50);
	strcpy(strScheduledProcedureStepID, pWorkList->strScheduledProcedureStepID);
	vr = new VR(0x0040, 0x0009, strlen(strScheduledProcedureStepID), (void *)strScheduledProcedureStepID);
   	DDOWorkList.Push(vr);

	char * strRequestedProcedureID = new char[50];
	memset(strRequestedProcedureID,0,50);
	strcpy(strRequestedProcedureID, pWorkList->strRequestedProcedureID);
	vr = new VR(0x0040, 0x1001, strlen(strRequestedProcedureID), (void *)strRequestedProcedureID);
   	DDOWorkList.Push(vr);

	char * m_strAccessionNumber = new char[MAX_VRSH];
	memset(m_strAccessionNumber,0,MAX_VRSH);
	strcpy(m_strAccessionNumber, pWorkList->m_strAccessionNumber);
	vr = new VR(0x0008, 0x0050, strlen(m_strAccessionNumber), (void *)m_strAccessionNumber);
   	DDOWorkList.Push(vr);

	char * m_strModality = new char[MAX_VRCS];
	memset(m_strModality,0,MAX_VRCS);
	strcpy(m_strModality, pWorkList->m_strModality);
	vr = new VR(0x0008, 0x0060, strlen(m_strModality), (void *)m_strModality);
   	DDOWorkList.Push(vr);

	char * m_ReferringPhysicianName = new char[MAX_VRPN];
	memset(m_ReferringPhysicianName,0,MAX_VRPN);
	strcpy(m_ReferringPhysicianName, pWorkList->m_ReferringPhysicianName);
	vr = new VR(0x0008, 0x0090, strlen(m_ReferringPhysicianName), (void *)m_ReferringPhysicianName);
   	DDOWorkList.Push(vr);

	char * m_timMaxPatientBirthDate = new char[50];
	memset(m_timMaxPatientBirthDate,0,50);
	strcpy(m_timMaxPatientBirthDate, pWorkList->m_timMaxPatientBirthDate);
	vr = new VR(0x0010, 0x0030, strlen(m_timMaxPatientBirthDate), (void *)m_timMaxPatientBirthDate);
   	DDOWorkList.Push(vr);

	char * m_PatientSex = new char[MAX_VRCS];
	memset(m_PatientSex,0,MAX_VRCS);
	strcpy(m_PatientSex, pWorkList->m_PatientSex);
	vr = new VR(0x0010, 0x0040, strlen(m_PatientSex), (void *)m_PatientSex);
   	DDOWorkList.Push(vr);

	char * strPathentWeight = new char[10];
	memset(strPathentWeight,0,10);
	strcpy(strPathentWeight, pWorkList->strPathentWeight);
	vr = new VR(0x0010, 0x1030, strlen(strPathentWeight), (void *)strPathentWeight);
   	DDOWorkList.Push(vr);

	char * strMedicalAlerts = new char[256];
	memset(strMedicalAlerts,0,256);
	strcpy(strMedicalAlerts, pWorkList->strMedicalAlerts);
	vr = new VR(0x0010, 0x2000, strlen(strMedicalAlerts), (void *)strMedicalAlerts);
   	DDOWorkList.Push(vr);

	char * strContrastAllergies = new char[256];
	memset(strContrastAllergies,0,256);
	strcpy(strContrastAllergies, pWorkList->strContrastAllergies);
	vr = new VR(0x0010, 0x2110, strlen(strContrastAllergies), (void *)strContrastAllergies);
   	DDOWorkList.Push(vr);

	char * strPregnancyStatus = new char[20];
	memset(strPregnancyStatus,0,20);
	strcpy(strPregnancyStatus, pWorkList->strPregnancyStatus);
	vr = new VR(0x0010, 0x21C0, strlen(strPregnancyStatus), (void *)strPregnancyStatus);
   	DDOWorkList.Push(vr);

	char * strRequestingPhysician = new char[MAX_VRPN];
	memset(strRequestingPhysician,0,MAX_VRPN);
	strcpy(strRequestingPhysician, pWorkList->strRequestingPhysician);
	vr = new VR(0x0032, 0x1032, strlen(strRequestingPhysician), (void *)strRequestingPhysician);
   	DDOWorkList.Push(vr);

	char * strRequestedContrastAgent = new char[50];
	memset(strRequestedContrastAgent,0,50);
	strcpy(strRequestedContrastAgent, pWorkList->strRequestedContrastAgent);
	vr = new VR(0x0032, 0x1070, strlen(strRequestedContrastAgent), (void *)strRequestedContrastAgent);
   	DDOWorkList.Push(vr);

	char * strAdmissionID = new char[20];
	memset(strAdmissionID,0,20);
	strcpy(strAdmissionID, pWorkList->strAdmissionID);
	vr = new VR(0x0038, 0x0010, strlen(strAdmissionID), (void *)strAdmissionID);
   	DDOWorkList.Push(vr);

	char * strSpecialNeeds = new char[256];
	memset(strSpecialNeeds,0,256);
	strcpy(strSpecialNeeds, pWorkList->strSpecialNeeds);
	vr = new VR(0x0038, 0x0050, strlen(strSpecialNeeds), (void *)strSpecialNeeds);
   	DDOWorkList.Push(vr);

	char * strCurrentPatientLocation = new char[50];
	memset(strCurrentPatientLocation,0,50);
	strcpy(strCurrentPatientLocation, pWorkList->strCurrentPatientLocation);
	vr = new VR(0x0038, 0x0300, strlen(strCurrentPatientLocation), (void *)strCurrentPatientLocation);
   	DDOWorkList.Push(vr);

	char * strPatientState = new char[50];
	memset(strPatientState,0,50);
	strcpy(strPatientState, pWorkList->strPatientState);
	vr = new VR(0x0038, 0x0500, strlen(strPatientState), (void *)strPatientState);
   	DDOWorkList.Push(vr);

	char * strPerformingPhysician = new char[MAX_VRPN];
	memset(strPerformingPhysician,0,MAX_VRPN);
	strcpy(strPerformingPhysician, pWorkList->strPerformingPhysician);
	vr = new VR(0x0040, 0x0006, strlen(strPerformingPhysician), (void *)strPerformingPhysician);
   	DDOWorkList.Push(vr);

	char * strScheduledStationName = new char[50];
	memset(strScheduledStationName,0,50);
	strcpy(strScheduledStationName, pWorkList->strScheduledStationName);
	vr = new VR(0x0040, 0x0010, strlen(strScheduledStationName), (void *)strScheduledStationName);
   	DDOWorkList.Push(vr);

	char * strScheduledProcedureStepLocation = new char[50];
	memset(strScheduledProcedureStepLocation,0,50);
	strcpy(strScheduledProcedureStepLocation, pWorkList->strScheduledProcedureStepLocation);
	vr = new VR(0x0040, 0x0011, strlen(strScheduledProcedureStepLocation), (void *)strScheduledProcedureStepLocation);
   	DDOWorkList.Push(vr);

	char * strPreOrderMedication = new char[50];
	memset(strPreOrderMedication,0,50);
	strcpy(strPreOrderMedication, pWorkList->strPreOrderMedication);
	vr = new VR(0x0040, 0x0012, strlen(strPreOrderMedication), (void *)strPreOrderMedication);
   	DDOWorkList.Push(vr);

	char * strRequestedProcedurePriority = new char[50];
	memset(strRequestedProcedurePriority,0,50);
	strcpy(strRequestedProcedurePriority, pWorkList->strRequestedProcedurePriority);
	vr = new VR(0x0040, 0x1003, strlen(strRequestedProcedurePriority), (void *)strRequestedProcedurePriority);
   	DDOWorkList.Push(vr);

	char * strPatientTransportArrangements = new char[50];
	memset(strPatientTransportArrangements,0,50);
	strcpy(strPatientTransportArrangements, pWorkList->strPatientTransportArrangements);
	vr = new VR(0x0040, 0x1004, strlen(strPatientTransportArrangements), (void *)strPatientTransportArrangements);
   	DDOWorkList.Push(vr);

	char * strConfidentialityConstraint = new char[50];
	memset(strConfidentialityConstraint,0,50);
	strcpy(strConfidentialityConstraint, pWorkList->strConfidentialityConstraint);
	vr = new VR(0x0040, 0x3001, strlen(strConfidentialityConstraint), (void *)strConfidentialityConstraint);
   	DDOWorkList.Push(vr);

	MyQueryRetrievePDU PDU;

	PDU.ClearAbstractSyntaxs();
	PDU.ClearPresentationContexts();

	UID	ModalityWorkListQueryUID, TransferUID, Appuid("1.2.840.10008.3.1.1.1");
	PDU.SetApplicationContext(Appuid);

	SOPMyModalityWorklistQuery.GetUID(ModalityWorkListQueryUID);
	PDU.AddAbstractSyntax(ModalityWorkListQueryUID);

	PDU.SetLocalAddress((BYTE *)strDICOMLocalAETitle);
	PDU.SetRemoteAddress((BYTE*)strDICOMRemoteAETitle);

	//PDU Connect
	PDU.SetTimeOut(10);//10 seconds//zxh
	if(!PDU.Connect((BYTE *)strDICOMRemoteIP, (BYTE *)strRemotePort)) 
	{
		switch(PDU.PDUGetLastError()) {
		case PDU_CONNECT_NOINTEROGATEAASSOCIATEAC:
			return PDU_CONNECT_NOINTEROGATEAASSOCIATEAC;
			break;
		case PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN:
			return PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN;
			break;
		case PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED:
			return PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED:
			return PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION:
			return PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION;
			break;
		case PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED:
			return PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN:
			return PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED:
			return PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED:
			return PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED:
			return PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION:
			return PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION;
			break;
		case PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED:
			return PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED;
			break;
		default:
			return -1;
		}
	}

	if (PDU.IsAbstractSyntaxAccepted(ModalityWorkListQueryUID, TransferUID))
	{
		SOPMyPatientRootQuery.RemoveArray();
		if(!SOPMyModalityWorklistQuery.Write(&PDU, &DDOWorkList))
		{
			switch(SOPMyModalityWorklistQuery.GetLastError()) 
			{
				case QUERY_REFUSED_OUTOFRESOURCE:
					return QUERY_REFUSED_OUTOFRESOURCE;
					break;
				case QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS:
					return QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS;
					break;
				case QUERY_FAILED_UNABLETOPROCESS:
					return QUERY_FAILED_UNABLETOPROCESS;
					break;
				case QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST:
					return QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST;
					break;
			}
			return -1;
		}
		PDU.Close();
		return 0;
	}
	PDU.Close();		
	return -1;
}
int CQueryInformation::QueryWithAllTerm(PatientCondition PatientData, StudyCondition StudyData, SeriesCondition SeriesData, ImageCondition ImageData,int nLevel)
{
	DICOMDataObject DDO;
	VR * vr;
	char QLevel[10];
	char strPatientName[MAX_VRPN];
	char strPatientID[MAX_VRLO];
	char strPatientBirthDate[MAX_VRDA];
	char strPatientSex[MAX_VRCS];
	mdate timMaxPatientBirthDate,timMinPatientBirthDate;
	
	timMaxPatientBirthDate=PatientData.m_timMaxPatientBirthDate;
	timMinPatientBirthDate=PatientData.m_timMinPatientBirthDate;

	strcpy(strPatientName,PatientData.m_strPatientName);
	strcpy(strPatientID,PatientData.m_strPatientID);
	
	if (timMaxPatientBirthDate != 0 && timMinPatientBirthDate != 0)
	{	
		if (timMaxPatientBirthDate == timMinPatientBirthDate)
		{
			strcpy(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
		}
		else
		{
			strcpy(strPatientBirthDate,timMinPatientBirthDate.datebuf);
			strcat(strPatientBirthDate,"-");
			strcat(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
		}
	}
	else if (timMaxPatientBirthDate != 0 && timMinPatientBirthDate == 0)
	{
		strcpy(strPatientBirthDate, "-");
		strcat(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
	}
	else if (timMaxPatientBirthDate == 0 && timMinPatientBirthDate != 0)
	{
		strcpy(strPatientBirthDate,timMinPatientBirthDate.datebuf);
		strcat(strPatientBirthDate,"-");
	}
	else if (timMaxPatientBirthDate == 0 && timMinPatientBirthDate == 0)
	{
		strcpy(strPatientBirthDate,"");
	}

	strcpy(strPatientSex,PatientData.m_PatientSex);


//push Query Level 
	if(nLevel==1)
		lstrcpy(QLevel,"PATIENT");
	else if(nLevel==2)
		lstrcpy(QLevel,"STUDY");
	else if(nLevel==3)
		lstrcpy(QLevel,"SERIES");
	else if(nLevel==4)
		lstrcpy(QLevel,"IMAGE");

	vr = new VR(0x0008, 0x0052, strlen(QLevel), (void *) QLevel, FALSE);
	DDO.Push(vr);

//Patient Name  PN
	char * cPatientName = new char[MAX_VRPN];
	strcpy(cPatientName, strPatientName);
	vr = new VR(0x0010, 0x0010, strlen(cPatientName), (void *)cPatientName);
   	DDO.Push(vr);

//Patient ID  LO
	char * cPatientID = new char[MAX_VRLO];
	strcpy(cPatientID, strPatientID);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDO.Push(vr);

//Patient Birth date   DA
	if (strlen(strPatientBirthDate)>0)
	{
		char * cPatientBirthDate = new char[MAX_VRDA];
		strcpy(cPatientBirthDate, strPatientBirthDate);
		vr = new VR(0x0010, 0x0030, strlen(cPatientBirthDate), (void *)cPatientBirthDate);
	}
	else
	{
		vr = new VR(0x0010,0x0030,0,FALSE);
	}
	DDO.Push(vr);

//Patient Sex   CS
	if (strlen(strPatientSex)>0)
	{
		char * cPatientSex = new char[MAX_VRCS];
		strcpy(cPatientSex, strPatientSex);
		vr = new VR(0x0010, 0x0040, strlen(cPatientSex), (void *)cPatientSex);
	}
	else
	{
		vr = new VR(0x0010,0x0040,0,FALSE);
	}
	DDO.Push(vr);
	
	char strStudyDate[MAX_VRDA];
	char strStudyTime[MAX_VRTM];

	char strAccessionNumber[MAX_VRSH];
	char strStudyID[MAX_VRSH];
	char strStudyInstanceUID[MAX_VRUI];

	mdate timMaxStudyDate,timMinStudyDate;
	mtime timMaxStudyTime,timMinStudyTime;
	timMaxStudyDate = StudyData.m_timMaxStudyDate;
	timMinStudyDate = StudyData.m_timMinStudyDate;
	timMaxStudyTime = StudyData.m_timMaxStudyTime;
	timMinStudyTime = StudyData.m_timMinStudyTime;
	
	if (timMaxStudyDate != 0 && timMinStudyDate != 0)
	{
		if (timMaxStudyDate == timMinStudyDate)
		{
			strcpy(strStudyDate,timMaxStudyDate.datebuf);
		}
		else
		{
			strcpy(strStudyDate,timMinStudyDate.datebuf);
			strcat(strStudyDate,"-");
			strcat(strStudyDate,timMaxStudyDate.datebuf);
		}
	}
	else if (timMaxStudyDate != 0 && timMinStudyDate == 0)
	{
		strcpy(strStudyDate,"-");
		strcat(strStudyDate,timMaxStudyDate.datebuf);
	}
	else if (timMaxStudyDate == 0 && timMinStudyDate != 0)
	{
		strcpy(strStudyDate,timMinStudyDate.datebuf);
		strcat(strStudyDate,"-");
	}
	else if (timMaxStudyDate == 0 && timMinStudyDate == 0)
	{
		strcpy(strStudyDate,"");
	}

	if (timMaxStudyTime != 0 && timMinStudyTime != 0)
	{
		if (timMaxStudyTime == timMinStudyTime)
		{
			strcpy(strStudyTime,timMaxStudyTime.timebuf);
		}
		else
		{
			strcpy(strStudyTime,timMinStudyTime.timebuf);
			strcat(strStudyTime,"-");
			strcat(strStudyTime,timMaxStudyTime.timebuf);
		}
	}
	else if (timMaxStudyTime != 0 && timMinStudyTime == 0)
	{
		strcpy(strStudyTime,"-");
		strcat(strStudyTime,timMaxStudyTime.timebuf);
	}
	else if (timMaxStudyTime == 0 && timMinStudyTime != 0)
	{
		strcpy(strStudyTime,timMinStudyTime.timebuf);
		strcat(strStudyTime,"-");
	}
	else if (timMaxStudyTime == 0 && timMinStudyTime == 0)
	{
		strcpy(strStudyTime,"");
	}

	strcpy(strAccessionNumber,StudyData.m_strAccessionNumber);
	strcpy(strStudyID,StudyData.m_strStudyID);
	strcpy(strStudyInstanceUID,StudyData.m_strStudyInstanceUID);

//Study Date  DA
	char * cStudyDate = new char[MAX_VRDA];
	strcpy(cStudyDate, strStudyDate);
	vr = new VR(0x0008, 0x0020, strlen(cStudyDate), (void *)cStudyDate);
   	DDO.Push(vr);

//Study Time  TM
	char * cStudyTime = new char[MAX_VRTM];
	strcpy(cStudyTime, strStudyTime);
	vr = new VR(0x0008, 0x0030, strlen(cStudyTime), (void *)cStudyTime);
   	DDO.Push(vr);

//Accession Number  SH
	char * cAccessionNumber = new char[MAX_VRSH];
	strcpy(cAccessionNumber, strAccessionNumber);
	vr = new VR(0x0008, 0x0050, strlen(cAccessionNumber), (void *)cAccessionNumber);
   	DDO.Push(vr);

//Study ID  SH
	char * cStudyID = new char[MAX_VRSH];
	strcpy(cStudyID, strStudyID);
	vr = new VR(0x0020, 0x0010, strlen(cStudyID), (void *)cStudyID);
   	DDO.Push(vr);

	if(nLevel>1)
	{
	//Study Instance UID  UI
		char * cStudyInstanceUID = new char[MAX_VRUI];
		strcpy(cStudyInstanceUID, strStudyInstanceUID);
		vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   		DDO.Push(vr);
	}
//Study Description
	vr = new VR(0x0008,0x1030,0,FALSE);
	DDO.Push(vr);

//Admitting Description
	vr = new VR(0x0008,0x1080,0,FALSE);
	DDO.Push(vr);

	char strSeriesNumber[MAX_VRIS];
	char strSeriesInstanceUID[MAX_VRUI];
	char strPhysicianName[MAX_VRPN];
	char strBodyPart[MAX_VRCS];
	char strManufacturer[MAX_VRLO];
	char strStationName[MAX_VRSH];
	char strModality[MAX_VRCS];
	
	strcpy(strSeriesNumber,SeriesData.m_strSeriesNumber);
	strcpy(strSeriesInstanceUID,SeriesData.m_strSeriesInstanceUID);
	strcpy(strPhysicianName,SeriesData.m_strPhysicianName);
	strcpy(strBodyPart,SeriesData.m_strBodyPart);
	strcpy(strManufacturer,SeriesData.m_strManufacturer);
	strcpy(strStationName,SeriesData.m_strStationName);
	strcpy(strModality,SeriesData.m_strModality);

//Modality  CS
	char * cModality = new char[MAX_VRCS];
	strcpy(cModality, strModality);
	vr = new VR(0x0008, 0x0060, strlen(cModality), (void *)cModality);
   	DDO.Push(vr);

//Series Number  IS
	char * cSeriesNumber = new char[MAX_VRIS];
	strcpy(cSeriesNumber, strSeriesNumber);
	vr = new VR(0x0020, 0x0011, strlen(cSeriesNumber), (void *)cSeriesNumber);
   	DDO.Push(vr);

	if(nLevel>2)
	{
	//Series Instance UID  UI
		char * cSeriesInstanceUID = new char[MAX_VRUI];
		strcpy(cSeriesInstanceUID, strSeriesInstanceUID);
		vr = new VR(0x0020, 0x000E, strlen(cSeriesInstanceUID), (void *)cSeriesInstanceUID);
   		DDO.Push(vr);
	}
//Perform Physician Name   PN
	if (strlen(strPhysicianName)>0)
	{
		char * cPhysicianName = new char[MAX_VRPN];
		strcpy(cPhysicianName, strPhysicianName);
		vr = new VR(0x0008, 0x1050, strlen(cPhysicianName), (void *)cPhysicianName);
	}
	else
	{
		vr = new VR(0x0008,0x1050,0,FALSE);
	}
	DDO.Push(vr);

//Series Description
	vr = new VR(0x0008,0x103E,0,FALSE);
	DDO.Push(vr);

//Body Part Examined  CS
	if (strlen(strBodyPart)>0)
	{
		char * cBodyPart = new char[MAX_VRCS];
		strcpy(cBodyPart, strBodyPart);
		vr = new VR(0x0018, 0x0015, strlen(cBodyPart), (void *)cBodyPart);
	}
	else
	{
		vr = new VR(0x0018,0x0015,0,FALSE);
	}
	DDO.Push(vr);

//Manufacturer  LO
	if (strlen(strManufacturer)>0)
	{
		char * cManufacturer = new char[MAX_VRLO];
		strcpy(cManufacturer, strManufacturer);
		vr = new VR(0x0008, 0x0070, strlen(cManufacturer), (void *)cManufacturer);
	}
	else
	{
		vr = new VR(0x0008,0x0070,0,FALSE);
	}
	DDO.Push(vr);

//Station Name   SH
	if (strlen(strStationName)>0)
	{
		char * cStationName = new char[MAX_VRSH];
		strcpy(cStationName, strStationName);
		vr = new VR(0x0008, 0x1010, strlen(cStationName), (void *)cStationName);
	}
	else
	{
		vr = new VR(0x0008,0x1010,0,FALSE);
	}
	DDO.Push(vr);

//Manufacturer Model Name
	vr = new VR(0x0008,0x1090,0,FALSE);
	DDO.Push(vr);

	char strImageNumber[MAX_VRIS];
	char strSOPInstanceUID[MAX_VRUI];

	strcpy(strImageNumber,ImageData.m_strImageNumber);
	strcpy(strSOPInstanceUID,ImageData.m_strSOPInstanceUID);
	
//Image Number  IS
	char * cImageNumber = new char[MAX_VRIS];
	*cImageNumber='\0';
	strcpy(cImageNumber, strImageNumber);
	vr = new VR(0x0020, 0x0013, strlen(cImageNumber), (void *)cImageNumber);
   	DDO.Push(vr);

	if(nLevel>3)
	{
	//SOP Instance UID  UI
		char * cSOPInstanceUID = new char[MAX_VRUI];
		*cSOPInstanceUID='\0';
		strcpy(cSOPInstanceUID, strSOPInstanceUID);
		vr = new VR(0x0008, 0x0018, strlen(cSOPInstanceUID), (void *)cSOPInstanceUID);
   		DDO.Push(vr);
	}
//Image Type
	vr = new VR(0x0008,0x0008,0,FALSE);
	DDO.Push(vr);

	//FileSetUID==Volume
	if (strlen(ImageData.m_Volume)>0)
	{
		char * cVolume = new char[50];
		strcpy(cVolume, ImageData.m_Volume);
		vr = new VR(0x0088, 0x0140, strlen(cVolume), (void *)cVolume);
	}
	else
		vr = new VR(0x0088, 0x0140,0,FALSE);
   	DDO.Push(vr);
	//InstanceAvailability
	if (strlen(ImageData.m_InstanceAvailability)>0)
	{
		char * cInstanceAvailability = new char[20];
		strcpy(cInstanceAvailability, ImageData.m_InstanceAvailability);
		vr = new VR(0x0008, 0x0056, strlen(cInstanceAvailability), (void *)cInstanceAvailability);
	}
	else
		vr = new VR(0x0008, 0x0056,0,FALSE);

   	DDO.Push(vr);


//Image comments
	vr = new VR(0x0020,0x4000,0,FALSE);
	DDO.Push(vr);

//Photometric Interpretation
	vr = new VR(0x0028,0x0004,0,FALSE);
	DDO.Push(vr);

//Transducer Data
	vr = new VR(0x0018,0x5010,0,FALSE);
	DDO.Push(vr);

//Transducer Type
	vr = new VR(0x0018,0x6031,0,FALSE);
	DDO.Push(vr);

//Specific Character Set
//	vr = new VR(0x0008,0x0005,0,FALSE);
//	DDOImageInfo.Push(vr);

	SOPMyPatientRootQuery.RemoveArray();
	SOPMyStudyRootQuery.RemoveArray();

	int nRet= Execute(&DDO,nLevel);
	if(nRet==0)
	{
		if (m_pPatientArray->GetSize()-1 == -1)
		{
			return -1;
		}
		pQuery->SetArray(m_pPatientArray);
	}
	return nRet;
}	
int CQueryInformation::QueryInformation (PatientCondition PatientData, StudyCondition StudyData, SeriesCondition SeriesData, ImageCondition ImageData,int nLevel)
{
	unsigned int nPatientNumber,nStudyNumber,nSeriesNumber;
	PatientInfo * pPatient;
	StudyInfo * pStudy;
	SeriesInfo * pSeries;

	SOPMyPatientRootQuery.RemoveArray();
	SOPMyStudyRootQuery.RemoveArray();
	pQuery->RemoveAllArray();
	if(nLevel<1)
		return -1;
	if(QueryPatientInfo(PatientData))
		return -1;
	if (m_pPatientArray->GetSize()-1 == -1)
	{
		return -1;
	}
	if(nLevel>1)
	{
		for (nPatientNumber = 0; nPatientNumber <= m_pPatientArray->GetSize()-1; nPatientNumber++)
		{
			pPatient=m_pPatientArray->Get(nPatientNumber);
			QueryStudyInfo(StudyData,pPatient->m_strPatientID);
			if (pPatient->m_StudyInfo.GetSize()-1 != -1)
			{
				if(nLevel<3)
					continue;
				for (nStudyNumber = 0; nStudyNumber <= m_pStudyArray->GetSize()-1; nStudyNumber++)
				{
					pStudy=pPatient->m_StudyInfo.Get(nStudyNumber);
					QuerySeriesInfo(SeriesData,pPatient->m_strPatientID,pStudy->m_strStudyInstanceUID);
					if (pStudy->m_SeriesInfo.GetSize()-1 != -1)
					{
						if(nLevel<4)
							continue;
						for (nSeriesNumber = 0; nSeriesNumber <= m_pSeriesArray->GetSize()-1; nSeriesNumber++)
						{
							int a1=m_pSeriesArray->GetSize();
							pSeries=pStudy->m_SeriesInfo.Get(nSeriesNumber);
							QueryImageInfo(ImageData,pPatient->m_strPatientID,pStudy->m_strStudyInstanceUID,pSeries->m_strSeriesInstanceUID);
						}
					}
				}
			}
		}
	}
	pQuery->SetArray(m_pPatientArray);
	return 0;
}

int CQueryInformation::QueryPatientInfo (PatientCondition PatientData)
{
	DICOMDataObject DDOPatientInfo;
	VR * vr;
	char QLevel[10];
	char strPatientName[MAX_VRPN];
	char strPatientID[MAX_VRLO];
	char strPatientBirthDate[MAX_VRDA];
	char strPatientSex[MAX_VRCS];
	mdate timMaxPatientBirthDate,timMinPatientBirthDate;
	
	timMaxPatientBirthDate=PatientData.m_timMaxPatientBirthDate;
	timMinPatientBirthDate=PatientData.m_timMinPatientBirthDate;

	strcpy(strPatientName,PatientData.m_strPatientName);
	strcpy(strPatientID,PatientData.m_strPatientID);
	
	if (timMaxPatientBirthDate != 0 && timMinPatientBirthDate != 0)
	{	
		if (timMaxPatientBirthDate == timMinPatientBirthDate)
		{
			strcpy(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
		}
		else
		{
			strcpy(strPatientBirthDate,timMinPatientBirthDate.datebuf);
			strcat(strPatientBirthDate,"-");
			strcat(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
		}
	}
	else if (timMaxPatientBirthDate != 0 && timMinPatientBirthDate == 0)
	{
		strcpy(strPatientBirthDate, "-");
		strcat(strPatientBirthDate,timMaxPatientBirthDate.datebuf);
	}
	else if (timMaxPatientBirthDate == 0 && timMinPatientBirthDate != 0)
	{
		strcpy(strPatientBirthDate,timMinPatientBirthDate.datebuf);
		strcat(strPatientBirthDate,"-");
	}
	else if (timMaxPatientBirthDate == 0 && timMinPatientBirthDate == 0)
	{
		strcpy(strPatientBirthDate,"");
	}

	strcpy(strPatientSex,PatientData.m_PatientSex);


//push Query Level 
	lstrcpy(QLevel,"PATIENT");
	vr = new VR(0x0008, 0x0052, strlen(QLevel), (void *) QLevel, FALSE);
	DDOPatientInfo.Push(vr);

//Patient Name  PN
	char * cPatientName = new char[MAX_VRPN];
	strcpy(cPatientName, strPatientName);
	vr = new VR(0x0010, 0x0010, strlen(cPatientName), (void *)cPatientName);
   	DDOPatientInfo.Push(vr);

//Patient ID  LO
	char * cPatientID = new char[MAX_VRLO];
	strcpy(cPatientID, strPatientID);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDOPatientInfo.Push(vr);

//Patient Birth date   DA
	if (strlen(strPatientBirthDate)>0)
	{
		char * cPatientBirthDate = new char[MAX_VRDA];
		strcpy(cPatientBirthDate, strPatientBirthDate);
		vr = new VR(0x0010, 0x0030, strlen(cPatientBirthDate), (void *)cPatientBirthDate);
	}
	else
	{
		vr = new VR(0x0010,0x0030,0,FALSE);
	}
	DDOPatientInfo.Push(vr);

//Patient Sex   CS
	if (strlen(strPatientSex)>0)
	{
		char * cPatientSex = new char[MAX_VRCS];
		strcpy(cPatientSex, strPatientSex);
		vr = new VR(0x0010, 0x0040, strlen(cPatientSex), (void *)cPatientSex);
	}
	else
	{
		vr = new VR(0x0010,0x0040,0,FALSE);
	}
	DDOPatientInfo.Push(vr);
	
	return Execute(&DDOPatientInfo,1);   //the number "1" equal to "PATIENT"
}

int CQueryInformation::QueryStudyInfo (StudyCondition StudyData, char* strPatientInfo)
{

	DICOMDataObject DDOStudyInfo;
	VR * vr;
	char QLevel[10];
	char strStudyDate[MAX_VRDA];
	char strStudyTime[MAX_VRTM];

	char strAccessionNumber[MAX_VRSH];
	char strStudyID[MAX_VRSH];
	char strStudyInstanceUID[MAX_VRUI];

	mdate timMaxStudyDate,timMinStudyDate;
	mtime timMaxStudyTime,timMinStudyTime;
	timMaxStudyDate = StudyData.m_timMaxStudyDate;
	timMinStudyDate = StudyData.m_timMinStudyDate;
	timMaxStudyTime = StudyData.m_timMaxStudyTime;
	timMinStudyTime = StudyData.m_timMinStudyTime;
	
	if (timMaxStudyDate != 0 && timMinStudyDate != 0)
	{
		if (timMaxStudyDate == timMinStudyDate)
		{
			strcpy(strStudyDate,timMaxStudyDate.datebuf);
		}
		else
		{
			strcpy(strStudyDate,timMinStudyDate.datebuf);
			strcat(strStudyDate,"-");
			strcat(strStudyDate,timMaxStudyDate.datebuf);
		}
	}
	else if (timMaxStudyDate != 0 && timMinStudyDate == 0)
	{
		strcpy(strStudyDate,"-");
		strcat(strStudyDate,timMaxStudyDate.datebuf);
	}
	else if (timMaxStudyDate == 0 && timMinStudyDate != 0)
	{
		strcpy(strStudyDate,timMinStudyDate.datebuf);
		strcat(strStudyDate,"-");
	}
	else if (timMaxStudyDate == 0 && timMinStudyDate == 0)
	{
		strcpy(strStudyDate,"");
	}

	if (timMaxStudyTime != 0 && timMinStudyTime != 0)
	{
		if (timMaxStudyTime == timMinStudyTime)
		{
			strcpy(strStudyTime,timMaxStudyTime.timebuf);
		}
		else
		{
			strcpy(strStudyTime,timMinStudyTime.timebuf);
			strcat(strStudyTime,"-");
			strcat(strStudyTime,timMaxStudyTime.timebuf);
		}
	}
	else if (timMaxStudyTime != 0 && timMinStudyTime == 0)
	{
		strcpy(strStudyTime,"-");
		strcat(strStudyTime,timMaxStudyTime.timebuf);
	}
	else if (timMaxStudyTime == 0 && timMinStudyTime != 0)
	{
		strcpy(strStudyTime,timMinStudyTime.timebuf);
		strcat(strStudyTime,"-");
	}
	else if (timMaxStudyTime == 0 && timMinStudyTime == 0)
	{
		strcpy(strStudyTime,"");
	}

	strcpy(strAccessionNumber,StudyData.m_strAccessionNumber);
	strcpy(strStudyID,StudyData.m_strStudyID);
	strcpy(strStudyInstanceUID,StudyData.m_strStudyInstanceUID);

//push Query Level 
	lstrcpy(QLevel,"STUDY");
	vr = new VR(0x0008, 0x0052, strlen(QLevel), (void *) QLevel, FALSE);
	DDOStudyInfo.Push(vr);

//Patient ID  LO
	char * cPatientID = new char[MAX_VRLO];
	strcpy(cPatientID, strPatientInfo);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDOStudyInfo.Push(vr);

//Study Date  DA
	char * cStudyDate = new char[MAX_VRDA];
	strcpy(cStudyDate, strStudyDate);
	vr = new VR(0x0008, 0x0020, strlen(cStudyDate), (void *)cStudyDate);
   	DDOStudyInfo.Push(vr);

//Study Time  TM
	char * cStudyTime = new char[MAX_VRTM];
	strcpy(cStudyTime, strStudyTime);
	vr = new VR(0x0008, 0x0030, strlen(cStudyTime), (void *)cStudyTime);
   	DDOStudyInfo.Push(vr);

//Accession Number  SH
	char * cAccessionNumber = new char[MAX_VRSH];
	strcpy(cAccessionNumber, strAccessionNumber);
	vr = new VR(0x0008, 0x0050, strlen(cAccessionNumber), (void *)cAccessionNumber);
   	DDOStudyInfo.Push(vr);

//Study ID  SH
	char * cStudyID = new char[MAX_VRSH];
	strcpy(cStudyID, strStudyID);
	vr = new VR(0x0020, 0x0010, strlen(cStudyID), (void *)cStudyID);
   	DDOStudyInfo.Push(vr);

//Study Instance UID  UI
	char * cStudyInstanceUID = new char[MAX_VRUI];
	strcpy(cStudyInstanceUID, strStudyInstanceUID);
	vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   	DDOStudyInfo.Push(vr);

//Study Description
	vr = new VR(0x0008,0x1030,0,FALSE);
	DDOStudyInfo.Push(vr);

//Admitting Description
	vr = new VR(0x0008,0x1080,0,FALSE);
	DDOStudyInfo.Push(vr);

	return Execute(&DDOStudyInfo,2);  //the number "2" equal to "STUDY"
}

int CQueryInformation::QuerySeriesInfo (SeriesCondition SeriesData, char* strPatientInfo, char* strStudyInfo)
{
	DICOMDataObject DDOSeriesInfo;
	VR * vr;
	char QLevel[10];
	char strSeriesNumber[MAX_VRIS];
	char strSeriesInstanceUID[MAX_VRUI];
	char strPhysicianName[MAX_VRPN];
	char strBodyPart[MAX_VRCS];
	char strManufacturer[MAX_VRLO];
	char strStationName[MAX_VRSH];
	char strModality[MAX_VRCS];
	
	strcpy(strSeriesNumber,SeriesData.m_strSeriesNumber);
	strcpy(strSeriesInstanceUID,SeriesData.m_strSeriesInstanceUID);
	strcpy(strPhysicianName,SeriesData.m_strPhysicianName);
	strcpy(strBodyPart,SeriesData.m_strBodyPart);
	strcpy(strManufacturer,SeriesData.m_strManufacturer);
	strcpy(strStationName,SeriesData.m_strStationName);
	strcpy(strModality,SeriesData.m_strModality);

//push Query Level 
	lstrcpy(QLevel,"SERIES");
	vr = new VR(0x0008, 0x0052, strlen(QLevel), (void *) QLevel, FALSE);
	DDOSeriesInfo.Push(vr);

//Patient ID  LO
	char * cPatientID = new char[MAX_VRLO];
	strcpy(cPatientID, strPatientInfo);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDOSeriesInfo.Push(vr);

//Study Instance UID  UI
	char * cStudyInstanceUID = new char[MAX_VRUI];
	strcpy(cStudyInstanceUID, strStudyInfo);
	vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   	DDOSeriesInfo.Push(vr);

//Modality  CS
	char * cModality = new char[MAX_VRCS];
	strcpy(cModality, strModality);
	vr = new VR(0x0008, 0x0060, strlen(cModality), (void *)cModality);
   	DDOSeriesInfo.Push(vr);

//Series Number  IS
	char * cSeriesNumber = new char[MAX_VRIS];
	strcpy(cSeriesNumber, strSeriesNumber);
	vr = new VR(0x0020, 0x0011, strlen(cSeriesNumber), (void *)cSeriesNumber);
   	DDOSeriesInfo.Push(vr);

//Series Instance UID  UI
	char * cSeriesInstanceUID = new char[MAX_VRUI];
	strcpy(cSeriesInstanceUID, strSeriesInstanceUID);
	vr = new VR(0x0020, 0x000E, strlen(cSeriesInstanceUID), (void *)cSeriesInstanceUID);
   	DDOSeriesInfo.Push(vr);

//Perform Physician Name   PN
	if (strlen(strPhysicianName)>0)
	{
		char * cPhysicianName = new char[MAX_VRPN];
		strcpy(cPhysicianName, strPhysicianName);
		vr = new VR(0x0008, 0x1050, strlen(cPhysicianName), (void *)cPhysicianName);
	}
	else
	{
		vr = new VR(0x0008,0x1050,0,FALSE);
	}
	DDOSeriesInfo.Push(vr);

//Series Description
	vr = new VR(0x0008,0x103E,0,FALSE);
	DDOSeriesInfo.Push(vr);

//Body Part Examined  CS
	if (strlen(strBodyPart)>0)
	{
		char * cBodyPart = new char[MAX_VRCS];
		strcpy(cBodyPart, strBodyPart);
		vr = new VR(0x0018, 0x0015, strlen(cBodyPart), (void *)cBodyPart);
	}
	else
	{
		vr = new VR(0x0018,0x0015,0,FALSE);
	}
	DDOSeriesInfo.Push(vr);

//Manufacturer  LO
	if (strlen(strManufacturer)>0)
	{
		char * cManufacturer = new char[MAX_VRLO];
		strcpy(cManufacturer, strManufacturer);
		vr = new VR(0x0008, 0x0070, strlen(cManufacturer), (void *)cManufacturer);
	}
	else
	{
		vr = new VR(0x0008,0x0070,0,FALSE);
	}
	DDOSeriesInfo.Push(vr);

//Station Name   SH
	if (strlen(strStationName)>0)
	{
		char * cStationName = new char[MAX_VRSH];
		strcpy(cStationName, strStationName);
		vr = new VR(0x0008, 0x1010, strlen(cStationName), (void *)cStationName);
	}
	else
	{
		vr = new VR(0x0008,0x1010,0,FALSE);
	}
	DDOSeriesInfo.Push(vr);

//Manufacturer Model Name
	vr = new VR(0x0008,0x1090,0,FALSE);
	DDOSeriesInfo.Push(vr);

	return Execute(&DDOSeriesInfo,3);  //the number "3" equal to "SERIES"
}

int CQueryInformation::QueryImageInfo (ImageCondition ImageData,char* strPatientInfo,char* strStudyInfo,char* strSeriesInfo)
{
	DICOMDataObject DDOImageInfo;
	VR * vr;
	char QLevel[10];
	char strImageNumber[MAX_VRIS];
	char strSOPInstanceUID[MAX_VRUI];

	strcpy(strImageNumber,ImageData.m_strImageNumber);
	strcpy(strSOPInstanceUID,ImageData.m_strSOPInstanceUID);
	
//push Query Level 
	lstrcpy(QLevel,"IMAGE");
	vr = new VR(0x0008, 0x0052, strlen(QLevel), (void *) QLevel, FALSE);
	DDOImageInfo.Push(vr);

//Patient ID  LO
	char * cPatientID = new char[MAX_VRLO];
	strcpy(cPatientID, strPatientInfo);
	vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   	DDOImageInfo.Push(vr);

//Study Instance UID  UI
	char * cStudyInstanceUID = new char[MAX_VRUI];
	strcpy(cStudyInstanceUID, strStudyInfo);
	vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   	DDOImageInfo.Push(vr);

//Series Instance UID  UI
	char * cSeriesInstanceUID = new char[MAX_VRUI];
	strcpy(cSeriesInstanceUID, strSeriesInfo);
	vr = new VR(0x0020, 0x000E, strlen(cSeriesInstanceUID), (void *)cSeriesInstanceUID);
   	DDOImageInfo.Push(vr);

//Image Number  IS
	char * cImageNumber = new char[MAX_VRIS];
	strcpy(cImageNumber, strImageNumber);
	vr = new VR(0x0020, 0x0013, strlen(cImageNumber), (void *)cImageNumber);
   	DDOImageInfo.Push(vr);

//SOP Instance UID  UI
	char * cSOPInstanceUID = new char[MAX_VRUI];
	strcpy(cSOPInstanceUID, strSOPInstanceUID);
	vr = new VR(0x0008, 0x0018, strlen(cSOPInstanceUID), (void *)cSOPInstanceUID);
   	DDOImageInfo.Push(vr);

//Image Type
	vr = new VR(0x0008,0x0008,0,FALSE);
	DDOImageInfo.Push(vr);

//Image comments
	vr = new VR(0x0020,0x4000,0,FALSE);
	DDOImageInfo.Push(vr);

//Photometric Interpretation
	vr = new VR(0x0028,0x0004,0,FALSE);
	DDOImageInfo.Push(vr);

//Transducer Data
	vr = new VR(0x0018,0x5010,0,FALSE);
	DDOImageInfo.Push(vr);

//Transducer Type
	vr = new VR(0x0018,0x6031,0,FALSE);
	DDOImageInfo.Push(vr);

//Specific Character Set
//	vr = new VR(0x0008,0x0005,0,FALSE);
//	DDOImageInfo.Push(vr);
	return Execute(&DDOImageInfo,4);
}

Array<PatientInfo*>* CQueryInformation::GetPatientArray ()
{
	return m_pPatientArray;
}

Array<StudyInfo*>* CQueryInformation::GetStudyArray ()
{
	return m_pStudyArray;
}

Array<SeriesInfo*>* CQueryInformation::GetSeriesArray ()
{
	return m_pSeriesArray;
}

Array<ImageInfo*>* CQueryInformation::GetImageArray ()
{
	return m_pImageArray;
}
Array<WorkListInfo*>* CQueryInformation::GetWorkListArray()
{
	return SOPMyModalityWorklistQuery.pGlobalWorkListArray;
}
