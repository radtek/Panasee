#include "dicom.hpp"
#include "query.h"
#include "RetrieveImage.h"
#include "MyQueryRetrievePDU.h"
#define ABNORMALLOG(s)
CRetrieveImage::CRetrieveImage ()
{
	m_nTotalImageNumber = 0;
	m_nCurrentImageNumber = 0;
}


CRetrieveImage::~CRetrieveImage ()
{
}

int CRetrieveImage::Retrieve (char* strDICOMLocalAETitle,char*strDICOMRemoteAETitle,
							   char*strDICOMRemoteIP,char*strRemotePort/*void(*pFun)(int,int)*/)
{
	int nUseWhichSOP;
	MyQueryRetrievePDU PDU;
	PatientRootRetrieve SOPMyPatientRootRetrieve;
	StudyRootRetrieve SOPMyStudyRootRetrieve;

	Array<PatientInfo *> * pPatientInfoArray = pQuery->GetQueryArray();

	UID	PatientRootUID,StudyRootUID, TransferUID, Appuid("1.2.840.10008.3.1.1.1");
	PDU.SetApplicationContext(Appuid);
	PDU.SetQueryOrRetrieve(FALSE);  //FALSE means the current operation is Retrieve

	SOPMyPatientRootRetrieve.GetUID(PatientRootUID);
	PDU.AddAbstractSyntax(PatientRootUID);
	SOPMyStudyRootRetrieve.GetUID(StudyRootUID);
	PDU.AddAbstractSyntax(StudyRootUID);
	PDU.AddAbstractSyntax(UID("1.2.840.10008.5.1.4.1.1.3.1"));
	PDU.AddAbstractSyntax(UID("1.2.840.10008.5.1.4.1.1.6.1"));


	PDU.SetRemoteAddress((BYTE*)strDICOMRemoteAETitle);
	PDU.SetLocalAddress((BYTE *)strDICOMLocalAETitle);

	//PDU Connect
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
		nUseWhichSOP = 1;   //use PatientRootRetrieve
	}
	else if (PDU.IsAbstractSyntaxAccepted(StudyRootUID, TransferUID))
	{
		nUseWhichSOP = 2;   //use StudyRootRetrieve
	}
	
	DICOMDataObject DDOImageLevel;
	VR * vr;
	char * cImageNumber;
	char * cSOPInstanceUID;
	char strPatientLevel[MAX_VRUI];
	char strStudyLevel[MAX_VRUI];
	char strSeriesLevel[MAX_VRUI];
	char strImageNumber[MAX_VRUI];
	char strSOPInstanceUID[MAX_VRUI];
	int nErrorNumber = 0;

	GetTotalImageNumber();

	for (unsigned int nPatientNumber = 0; nPatientNumber <= pPatientInfoArray->GetSize()-1; nPatientNumber++)
	{
		for (unsigned int nStudyNumber = 0; nStudyNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.GetSize()-1; nStudyNumber++)
		{
			for (unsigned int nSeriesNumber = 0; nSeriesNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.GetSize()-1; nSeriesNumber++)
			{
				for (unsigned int nImageNumber = 0; nImageNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.GetSize()-1; nImageNumber++)
				{
					if (pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->GetIsNeedRetrieve())
					{
						strcpy(strPatientLevel,pPatientInfoArray->Get(nPatientNumber)->m_strPatientID);
						strcpy(strStudyLevel, pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_strStudyInstanceUID);
						strcpy(strSeriesLevel,pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_strSeriesInstanceUID);
						strcpy(strImageNumber,pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->m_strImageNumber);
						strcpy(strSOPInstanceUID,pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->m_strSOPInstanceUID);
						//push Query Level 
							vr = new VR(0x0008, 0x0052, 6, "IMAGE", FALSE);
							DDOImageLevel.Push(vr);

						//Patient ID  LO
							char * cPatientID = new char[MAX_VRLO];
							strcpy(cPatientID, strPatientLevel);
							vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   							DDOImageLevel.Push(vr);

						//Study Instance UID  UI
							char * cStudyInstanceUID = new char[MAX_VRUI];
							strcpy(cStudyInstanceUID, strStudyLevel);
							vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   							DDOImageLevel.Push(vr);

						//Series Instance UID  UI
							char * cSeriesInstanceUID = new char[MAX_VRUI];
							strcpy(cSeriesInstanceUID, strSeriesLevel);
							vr = new VR(0x0020, 0x000E, strlen(cSeriesInstanceUID), (void *)cSeriesInstanceUID);
   							DDOImageLevel.Push(vr);

						//Image Number  IS
							cImageNumber = new char[MAX_VRIS];
							strcpy(cImageNumber, strImageNumber);
							vr = new VR(0x0020, 0x0013, strlen(cImageNumber), (void *)cImageNumber);
   							DDOImageLevel.Push(vr);

						//SOP Instance UID  UI
							cSOPInstanceUID = new char[MAX_VRUI];
							strcpy(cSOPInstanceUID, strSOPInstanceUID);
							vr = new VR(0x0008, 0x0018, strlen(cSOPInstanceUID), (void *)cSOPInstanceUID);
   							DDOImageLevel.Push(vr);

						if (nUseWhichSOP == 1)    //use PatientRootRetrieve
						{
							if(!SOPMyPatientRootRetrieve.Write(&PDU, &DDOImageLevel , (BYTE*)(LPCTSTR)strDICOMLocalAETitle))
							{
								switch(SOPMyPatientRootRetrieve.GetLastError()) 
								{
									case RETRIEVE_REFUSED_UNABLETOCALCULATENUMBEROFMATCHS:
										ABNORMALLOG("Retrieve refused, out of resource, unable to calculate the number of matchs!");
										break;
									case RETRIEVE_REFUSED_UNABLETOPERFORMSUBOPERATIONS:
										ABNORMALLOG("Retrieve refused, out of resource, unable to perform sub_operations!");
										break;
									case RETRIEVE_REFUSED_MOVEDESTINATIONUNKNOWN:
										ABNORMALLOG("Retrieve refused, move destination unknown!");
										break;
									case RETRIEVE_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS:
										ABNORMALLOG("Retrieve failed, identifier does not match SOP class!");
										break;
									case RETRIEVE_CANCEL_SUBOPERATIONSTERMINATED:
										ABNORMALLOG("Retrieve cancel, sub_operations terminated!");
										break;
									case RETRIEVE_FAILED_UNABLETOPROCESS:
										ABNORMALLOG("Retrieve failed, unable to process!");
										break;
								}
								nErrorNumber = nErrorNumber + 1;
							}
							else
							{
								switch(SOPMyPatientRootRetrieve.GetLastError()) 
								{
									case RETRIEVE_WARNING_SUBOPERATIONCOMPLETE:
										ABNORMALLOG("Retrieve warning, sub_operation complete-one or more failures!");
										break;
									case RETRIEVE_PENDING_SUBOPERATIONSARECONTINUING:
										ABNORMALLOG("Retrieve pending, sub_operation are continuing!");
										break;
								}
							}
						}
						else if (nUseWhichSOP == 2)   //use StudyRootQuery
						{
							if(!SOPMyStudyRootRetrieve.Write(&PDU, &DDOImageLevel , (BYTE*)strDICOMLocalAETitle))
							{
								switch(SOPMyStudyRootRetrieve.GetLastError()) 
								{
									case RETRIEVE_REFUSED_UNABLETOCALCULATENUMBEROFMATCHS:
										ABNORMALLOG("Retrieve refused, out of resource, unable to calculate the number of matchs!");
										break;
									case RETRIEVE_REFUSED_UNABLETOPERFORMSUBOPERATIONS:
										ABNORMALLOG("Retrieve refused, out of resource, unable to perform sub_operations!");
										break;
									case RETRIEVE_REFUSED_MOVEDESTINATIONUNKNOWN:
										ABNORMALLOG("Retrieve refused, move destination unknown!");
										break;
									case RETRIEVE_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS:
										ABNORMALLOG("Retrieve failed, identifier does not match SOP class!");
										break;
									case RETRIEVE_CANCEL_SUBOPERATIONSTERMINATED:
										ABNORMALLOG("Retrieve cancel, sub_operations terminated!");
										break;
									case RETRIEVE_FAILED_UNABLETOPROCESS:
										ABNORMALLOG("Retrieve failed, unable to process!");
										break;
								}
								nErrorNumber = nErrorNumber + 1;
							}
							else
							{
								switch(SOPMyPatientRootRetrieve.GetLastError()) 
								{
									case RETRIEVE_WARNING_SUBOPERATIONCOMPLETE:
										ABNORMALLOG("Retrieve warning, sub_operation complete-one or more failures!");
										break;
									case RETRIEVE_PENDING_SUBOPERATIONSARECONTINUING:
										ABNORMALLOG("Retrieve pending, sub_operation are continuing!");
										break;
								}
							}
						}
						m_nCurrentImageNumber = m_nCurrentImageNumber + 1;
						//pFun(m_nTotalImageNumber,m_nCurrentImageNumber);
					}
				}
			}
		}
	}
	
	PDU.Close();
	return nErrorNumber;
}

void CRetrieveImage::SetRetrieveImage (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, BOOL bIsNeedRetrieve)
{
	pQuery->SetRetrieveImage(nPatientNumber,nStudyNumber,nSeriesNumber,nImageNumber,bIsNeedRetrieve);
}

void CRetrieveImage::GetTotalImageNumber ()
{
	Array<PatientInfo *> * pPatientInfoArray = pQuery->GetQueryArray();

	for (unsigned int nPatientNumber = 0; nPatientNumber <= pPatientInfoArray->GetSize()-1; nPatientNumber++)
	{
		for (unsigned int nStudyNumber = 0; nStudyNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.GetSize()-1; nStudyNumber++)
		{
			for (unsigned int nSeriesNumber = 0; nSeriesNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.GetSize()-1; nSeriesNumber++)
			{
				for (unsigned int nImageNumber = 0; nImageNumber <= pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.GetSize()-1; nImageNumber++)
				{
					if (pPatientInfoArray->Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->GetIsNeedRetrieve())
					{
						m_nTotalImageNumber = m_nTotalImageNumber + 1;
					}
				}
			}
		}
	}
}

void CRetrieveImage::SetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, char* strFileName)
{
	pQuery->SetRetrieveImageFileName(nPatientNumber,nStudyNumber,nSeriesNumber,nImageNumber,strFileName);
}