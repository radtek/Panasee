#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <io.h>
#include "MyQueryRetrieve.h"
#include "ServiceQueue.h"
#include "QRParam.h"
#define ABNORMALLOG(s)
#include "dicomdir.h"

//在Array<PatientInfo*>中查找某PatientInfo
//关键字PatientID
PatientInfo* IsPatientExistDCM(Array<PatientInfo*>*pAP,const char* strPatientID)
{
	for(int i=0;i<(int)pAP->GetSize();i++)
	{
		PatientInfo*ppi=pAP->Get(i);
		if(strcmp(ppi->m_strPatientID,strPatientID)==0)
			return ppi;
	}
	return NULL;
}

//在Array<StudyInfo*>中查找某StudyInfo
//关键字StudyInstanceUID
StudyInfo* IsStudyExistDCM(Array<StudyInfo*>*pAS,const char*strStudyInstanceUID)
{
	for(int i=0;i<(int)pAS->GetSize();i++)
	{
		StudyInfo*psi=pAS->Get(i);
		if(strcmp(psi->m_strStudyInstanceUID,strStudyInstanceUID)==0)
			return psi;
	}
	return NULL;
}

//在Array<SeriesInfo*>中查找某SeriesInfo
//关键字SeriesInstanceUID
SeriesInfo* IsSeriesExistDCM(Array<SeriesInfo*>*pAS,const char*strSeriesInstanceUID)
{
	for(int i=0;i<(int)pAS->GetSize();i++)
	{
		SeriesInfo*psi=pAS->Get(i);
		if(strcmp(psi->m_strSeriesInstanceUID,strSeriesInstanceUID)==0)
			return psi;
	}
	return NULL;
}

//在Array<SeriesInfo*>中查找某ImageInfo
//关键字SOPInstanceUID
ImageInfo* IsImageExistDCM(Array<ImageInfo*>*pAI,const char*strSOPInstanceUID)
{
	for(int i=0;i<(int)pAI->GetSize();i++)
	{
		ImageInfo*pii=pAI->Get(i);
		if(strcmp(pii->m_strSOPInstanceUID,strSOPInstanceUID)==0)
			return pii;
	}
	return NULL;
}

BOOL StandardQRCallBack(int m_nLevelNumber,Array<PatientInfo*>& m_PatientArray,DICOMCommandObject* DCO, DICOMDataObject* DDO)
{
// Initial call back (start)
	VR	*vr;
	char TempBuffer[10240];

	PatientInfo * pPatient,*pTmpPatient;
	StudyInfo * pStudy,*pTmpStudy;
	SeriesInfo * pSeries,*pTmpSeries;
	ImageInfo * pImage,*pTmpImage;

	char strPatientID[MAX_VRUI];
	char strStudyInstanceUID[MAX_VRUI];
	char strSeriesInstanceUID[MAX_VRUI];
	char strSOPInstanceUID[MAX_VRUI];

	if (! DCO )
		return TRUE;	
	if (! DDO )
		return TRUE;	

	memset((void*) TempBuffer, 0, 10240);

	if (m_nLevelNumber >= 1)   //Patient Level
	{
		//Patient ID
		vr = DDO->GetVR(0x0010, 0x0020);
		if(vr && vr->Length > 0)
		{
			trimcpy(strPatientID,vr->Data,vr->Length);
		}
		else
		{
			ABNORMALLOG("Exception occurs when querying patient,this patient has no Patient ID!");
			return FALSE;
		}
//查询PatientInfo
		pTmpPatient=IsPatientExistDCM(&m_PatientArray,strPatientID);
//有PatientInfo
		if(pTmpPatient!=NULL)
		{
			pPatient=pTmpPatient;
//下一步查询StudyInfo.
			goto HavePatient;
		}
//没有PatientInfo
//从DDO中提取信息产生PatientInfo.
		pPatient = new PatientInfo;
		if(pPatient==NULL)
			return FALSE;

		strcpy(pPatient->m_strPatientID,strPatientID);
		//Patient Name
		vr = DDO->GetVR(0x0010, 0x0010);
		if(vr && vr->Length > 0)
		{
			trimcpy(pPatient->m_strPatientName,vr->Data,vr->Length);
		}

		//Patient Birth Date
		vr = DDO->GetVR(0x0010, 0x0030);
		if(vr && vr->Length > 0)
		{
			pPatient->m_timPatientBirthDate=vr;
		}

		//Patient Sex
		vr = DDO->GetVR(0x0010, 0x0040);
		if(vr && vr->Length > 0)
		{
			trimcpy(pPatient->m_PatientSex,vr->Data,vr->Length);
		}

		m_PatientArray.Add(pPatient);
	}
HavePatient:
	if (m_nLevelNumber >= 2)    //Study Level
	{
		//Study Instance UID
		vr = DDO->GetVR(0x0020, 0x000D);
		if(vr && vr->Length > 0)
		{
			trimcpy(strStudyInstanceUID,vr->Data,vr->Length);
		}
		else
		{
			ABNORMALLOG("Exception occurs when querying study,this study has no Study Instance UID!");
			return FALSE;
		}
//查询StudyInfo
		pTmpStudy=IsStudyExistDCM(&pPatient->m_StudyInfo,strStudyInstanceUID);
//有StudyInfo
		if(pTmpStudy!=NULL)
		{
			pStudy=pTmpStudy;
//下一步查询SeriesInfo.
			goto HaveStudy;
		}

//没有StudyInfo
//从DDO中提取信息产生StudyInfo.
		pStudy = new StudyInfo;
		if(pStudy==NULL)
			return FALSE;

		strcpy(pStudy->m_strStudyInstanceUID,strStudyInstanceUID);

		//Study Date
		vr = DDO->GetVR(0x0008, 0x0020);
		if(vr && vr->Length > 0)
		{
			pStudy->m_timStudyDate=vr;
		}

		//Study Time
		vr = DDO->GetVR(0x0008, 0x0030);
		if(vr && vr->Length > 0)
		{
			pStudy->m_timStudyTime=vr;
		}

		//Accession Number
		vr = DDO->GetVR(0x0008, 0x0050);
		if(vr && vr->Length > 0)
		{
			trimcpy(pStudy->m_strAccessionNumber,vr->Data,vr->Length);
		}

		//Study ID
		vr = DDO->GetVR(0x0020, 0x0010);
		if(vr && vr->Length > 0)
		{
			trimcpy(pStudy->m_strStudyID,vr->Data,vr->Length);
		}


		//Study Description
		vr = DDO->GetVR(0x0008, 0x1030);
		if(vr && vr->Length > 0)
		{
			trimcpy(pStudy->m_strStudyDescription,vr->Data,vr->Length);
		}

		//Admitting Description
		vr = DDO->GetVR(0x0008, 0x1080);
		if(vr && vr->Length > 0)
		{
			trimcpy(pStudy->m_strAdmitDescription,vr->Data,vr->Length);
		}

		pPatient->m_StudyInfo.Add(pStudy);
		pStudy->pParent=pPatient;
	}
HaveStudy:
	if (m_nLevelNumber >= 3)     //Series Level
	{
		//Series Instance UID
		vr = DDO->GetVR(0x0020, 0x000E);
		if(vr && vr->Length > 0)
		{
			trimcpy(strSeriesInstanceUID,vr->Data,vr->Length);
		}
		else
		{						
			ABNORMALLOG("Exception occurs when querying series,this series has no Series Instance UID!");
			return FALSE;
		}
//查询SeriesInfo
//有SeriesInfo
		pTmpSeries=IsSeriesExistDCM(&pStudy->m_SeriesInfo,strSeriesInstanceUID);
		if(pTmpSeries!=NULL)
		{
			pSeries=pTmpSeries;
//下一步查询ImageInfo
			goto HaveSeries;
		}


//没有SeriesInfo
//从DDO中提取信息产生SeriesInfo.
		pSeries = new SeriesInfo;
		if(pSeries==NULL)
			return FALSE;

		strcpy(pSeries->m_strSeriesInstanceUID,strSeriesInstanceUID);

		//Modality
		vr = DDO->GetVR(0x0008, 0x0060);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strModality,vr->Data,vr->Length);
		}

		//Series Number
		vr = DDO->GetVR(0x0020, 0x0011);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strSeriesNumber,vr->Data,vr->Length);
		}


		//Performing Physician Name
		vr = DDO->GetVR(0x0008, 0x1050);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strPhysicianName,vr->Data,vr->Length);
		}

		//Series Description
		vr = DDO->GetVR(0x0008, 0x103E);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strSeriesDescription,vr->Data,vr->Length);
		}

		//Body Part Examined
		vr = DDO->GetVR(0x0018, 0x0015);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strBodyPart,vr->Data,vr->Length);
		}

		//Manufacturer
		vr = DDO->GetVR(0x0008, 0x0070);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strManufacturer,vr->Data,vr->Length);
		}

		//Station Name
		vr = DDO->GetVR(0x0008, 0x1010);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strStationName,vr->Data,vr->Length);
		}

		//Manufacturer Model Name
		vr = DDO->GetVR(0x0008, 0x1090);
		if(vr && vr->Length > 0)
		{
			trimcpy(pSeries->m_strManufacturerModelName,vr->Data,vr->Length);
		}

		pStudy->m_SeriesInfo.Add(pSeries);
		pSeries->pParent=pStudy;
	}
HaveSeries:
	if (m_nLevelNumber >= 4)     //Image Level
	{
		//SOP Instance UID
		vr = DDO->GetVR(0x0008, 0x0018);
		if(vr && vr->Length > 0)
		{
			trimcpy(strSOPInstanceUID,vr->Data,vr->Length);
		}
		else
		{						
			ABNORMALLOG("Exception occurs when querying Image,this Image has no SOP Instance UID!");
			return FALSE;
		}

		pTmpImage=IsImageExistDCM(&pSeries->m_ImageInfo,strSOPInstanceUID);
		if(pTmpImage!=NULL)
		{
			pImage=pTmpImage;
			goto HaveImage;
		}


//没有ImageInfo
//从DDO中提取信息产生ImageInfo.
		pImage = new ImageInfo;
		if(pImage==NULL)
			return FALSE;

		strcpy(pImage->m_strSOPInstanceUID,strSOPInstanceUID);

		//Image Number
		vr = DDO->GetVR(0x0020, 0x0013);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strImageNumber,vr->Data,vr->Length);
		}


		//Image Type
		vr = DDO->GetVR(0x0008, 0x0008);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strImageType,vr->Data,vr->Length);
		}

		//FileSetUID==Volume
		vr = DDO->GetVR(0x0088, 0x0140);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_Volume,vr->Data,vr->Length);
		}

		//FileSetUID==Volume
		vr = DDO->GetVR(0x0088, 0x0140);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_Volume,vr->Data,vr->Length);
		}
		//InstanceAvailability
		vr = DDO->GetVR(0x0008, 0x0056);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_InstanceAvailability,vr->Data,vr->Length);
		}
		
		//Image Comments
		vr = DDO->GetVR(0x0020, 0x4000);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strImageComment,vr->Data,vr->Length);
		}

		//Photometric Interpretation
		vr = DDO->GetVR(0x0028, 0x0004);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strPhotometric,vr->Data,vr->Length);
		}

		//Transducer Data
		vr = DDO->GetVR(0x0018, 0x5010);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strTransducerData,vr->Data,vr->Length);
		}

		//Transducer Type
		vr = DDO->GetVR(0x0018, 0x6031);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strTransducerType,vr->Data,vr->Length);
		}

		//Specific Character Set
		vr = DDO->GetVR(0x0008, 0x0005);
		if(vr && vr->Length > 0)
		{
			trimcpy(pImage->m_strSpecificCharacterSet,vr->Data,vr->Length);
		}

		pSeries->m_ImageInfo.Add(pImage);
		pImage->pParent=pSeries;

	}
//	delete vr;
HaveImage:
	return TRUE;

	// final call back (end)
}
BOOL StandardQualifyOn (BYTE*ACRNema,BYTE*MyACRNema,BYTE*RemoteIP,BYTE*RemotePort,BOOL&bSwap)
{
	//此处必须返回ip,端口等数据
	return FALSE;
}
BOOL StandardRetrieveOn(	DICOMDataObject	*DDO,
	DICOMDataObject	**pRDDO,
	StandardStorage	**SOPClass,BOOL& bSwap,RunTimeClassStorage& RTCStorage)
{
	//此处返回RunTimeClassStorage
	return FALSE;
}

//填充DICOMDataObject
int FillDDO(Array<PatientInfo*> * pPatientList,int nPatient,int nStudy,int nSeries,int nImage,DICOMDataObject*pDO,DICOMDataObject*pDOOut,int nLevel,BOOL bRetrieveSearch)
{
	PatientInfo* ppc;
	StudyInfo* psc;
	SeriesInfo* pec;
	ImageInfo* pic;
	ppc=pPatientList->Get(nPatient);
	VR* vr;
	
	//must
//(0008,0054)	Retrieve AE Title
	char * AET = new char[MAX_VRAE];
	strcpy(AET,"COOLPACS");
	vr =new VR(0x0008,0x0054,strlen(AET),(void*)AET);
	pDOOut->Push(vr);
	
//(0008,0052)	Query/Retrieve Level
	vr = pDO->GetVR(0x0008,0x0052);
	if(vr)
	{
		char * LEVEL = new char[MAX_VRCS*4];
		memset(LEVEL,0,MAX_VRCS*4);
		strncpy(LEVEL,(char*)vr->Data,vr->Length);
		vr =new VR(0x0008,0x0052,strlen(LEVEL),(void*)LEVEL);
		pDOOut->Push(vr);
	}

	//patient

//PatientName
	vr = pDO->GetVR(0x0010, 0x0010);
	if(vr)
	{
		char * cPatientName = new char[MAX_VRPN];
		strcpy(cPatientName, ppc->m_strPatientName);
		vr = new VR(0x0010, 0x0010, strlen(cPatientName), (void *)cPatientName);
   		pDOOut->Push(vr);
	}

//PatientID
//	vr = pDO->GetVR(0x0010, 0x0020);
//	if(vr)
//	{
		char * cPatientID = new char[MAX_VRLO];
		strcpy(cPatientID, ppc->m_strPatientID);
		vr = new VR(0x0010, 0x0020, strlen(cPatientID), (void *)cPatientID);
   		pDOOut->Push(vr);
//	}
//PatientBirthDate
	vr = pDO->GetVR(0x0010, 0x0030);
	if(vr)
	{
		char * cPatientBirthDate = new char[MAX_VRDA];
		strcpy(cPatientBirthDate, ppc->m_timPatientBirthDate.datebuf);
		vr = new VR(0x0010, 0x0030, strlen(cPatientBirthDate), (void *)cPatientBirthDate);
   		pDOOut->Push(vr);
	}

//Sex
	vr = pDO->GetVR(0x0010, 0x0040);
	if(vr)
	{
		char * cPatientSex = new char[MAX_VRCS*4];
		strcpy(cPatientSex, ppc->m_PatientSex);
		vr = new VR(0x0010, 0x0040, strlen(cPatientSex), (void *)cPatientSex);
   		pDOOut->Push(vr);

	}

	//study
	if(nLevel>1)
	{
		psc=ppc->m_StudyInfo.Get(nStudy);
//cStudy Date
		vr = pDO->GetVR(0x0008, 0x0020);
		if(vr)
		{
			char * cStudyDate = new char[MAX_VRDA];
			strcpy(cStudyDate, psc->m_timStudyDate.datebuf);
			vr = new VR(0x0008, 0x0020, strlen(cStudyDate), (void *)cStudyDate);
   			pDOOut->Push(vr);
		}
//Study Time
		vr = pDO->GetVR(0x0008, 0x0030);
		if(vr)
		{
			char * cStudyTime = new char[MAX_VRTM];
			strcpy(cStudyTime, psc->m_timStudyTime.timebuf);
			vr = new VR(0x0008, 0x0030, strlen(cStudyTime), (void *)cStudyTime);
   			pDOOut->Push(vr);
		}
//Accession Number
		vr = pDO->GetVR(0x0008, 0x0050);
		if(vr)
		{
			char * cAccessionNumber = new char[MAX_VRSH];
			strcpy(cAccessionNumber, psc->m_strAccessionNumber);
			vr = new VR(0x0008, 0x0050, strlen(cAccessionNumber), (void *)cAccessionNumber);
   			pDOOut->Push(vr);
		}
//StudyID
		vr = pDO->GetVR(0x0020, 0x0010);
		if(vr)
		{
			char * cStudyID = new char[MAX_VRSH];
			strcpy(cStudyID, psc->m_strStudyID);
			vr = new VR(0x0020, 0x0010, strlen(cStudyID), (void *)cStudyID);
   			pDOOut->Push(vr);
		}
//Study InstanceUID
	//	vr = pDO->GetVR(0x0020, 0x000D);
	//	if(vr&&vr->Length>0)
	//	{
			char * cStudyInstanceUID = new char[MAX_VRUI];
			strcpy(cStudyInstanceUID, psc->m_strStudyInstanceUID);
			vr = new VR(0x0020, 0x000D, strlen(cStudyInstanceUID), (void *)cStudyInstanceUID);
   			pDOOut->Push(vr);
	//	}

//Study Description
		vr = pDO->GetVR(0x0008, 0x1030);
		if(vr)
		{
			char * cStudyDescription = new char[MAX_VRLO];
			strcpy(cStudyDescription, psc->m_strStudyDescription);
			vr = new VR(0x0008, 0x1030, strlen(cStudyDescription), (void *)cStudyDescription);
   			pDOOut->Push(vr);
		}

//Admit Description
		vr = pDO->GetVR(0x0008, 0x1080);
		if(vr)
		{
			char * cAdmitDescription = new char[MAX_VRLO];
			strcpy(cAdmitDescription, psc->m_strAdmitDescription);
			vr = new VR(0x0008, 0x1080, strlen(cAdmitDescription), (void *)cAdmitDescription);
   			pDOOut->Push(vr);
		}
	}
	//series
	if(nLevel>2)
	{
		pec=psc->m_SeriesInfo.Get(nSeries);

//Modality
		vr = pDO->GetVR(0x0008, 0x0060);
		if(vr||bRetrieveSearch)
		{
			char * cModality = new char[MAX_VRCS*4];
			strcpy(cModality, pec->m_strModality);
			vr = new VR(0x0008, 0x0060, strlen(cModality), (void *)cModality);
   			pDOOut->Push(vr);
		}
//Series Number
		vr = pDO->GetVR(0x0020, 0x0011);
		if(vr)
		{
			char * cSeriesNumber = new char[MAX_VRIS];
			strcpy(cSeriesNumber, pec->m_strSeriesNumber);
			vr = new VR(0x0020, 0x0011, strlen(cSeriesNumber), (void *)cSeriesNumber);
   			pDOOut->Push(vr);
		}
//Series InstanceUID
	//	vr = pDO->GetVR(0x0020, 0x000E);
	//	if(vr)
	//	{
			char * cSeriesInstanceUID = new char[MAX_VRUI];
			strcpy(cSeriesInstanceUID, pec->m_strSeriesInstanceUID);
			vr = new VR(0x0020, 0x000E, strlen(cSeriesInstanceUID), (void *)cSeriesInstanceUID);
   			pDOOut->Push(vr);
	//	}

//Physician Name
		vr = pDO->GetVR(0x0008, 0x1050);
		if(vr)
		{
			char * cPhysicianName = new char[MAX_VRPN];
			strcpy(cPhysicianName, pec->m_strPhysicianName);
			vr = new VR(0x0008, 0x1050, strlen(cPhysicianName), (void *)cPhysicianName);
   			pDOOut->Push(vr);
		}
//Series Description
		vr = pDO->GetVR(0x0008, 0x103E);
		if(vr)
		{
			char * cSeriesDescription = new char[MAX_VRLO];
			strcpy(cSeriesDescription, pec->m_strSeriesDescription);
			vr = new VR(0x0008, 0x1080, strlen(cSeriesDescription), (void *)cSeriesDescription);
   			pDOOut->Push(vr);
		}
//BodyPart
		vr = pDO->GetVR(0x0018, 0x0015);
		if(vr)
		{
			char * cBodyPart = new char[MAX_VRCS*4];
			strcpy(cBodyPart, pec->m_strBodyPart);
			vr = new VR(0x0018, 0x0015, strlen(cBodyPart), (void *)cBodyPart);
   			pDOOut->Push(vr);
		}
//Manufacturer
		vr = pDO->GetVR(0x0008, 0x0070);
		if(vr)
		{
			char * cManufacturer = new char[MAX_VRLO];
			strcpy(cManufacturer, pec->m_strManufacturer);
			vr = new VR(0x0008, 0x0070, strlen(cManufacturer), (void *)cManufacturer);
   			pDOOut->Push(vr);
		}
//Station Name
		vr = pDO->GetVR(0x0008, 0x1010);
		if(vr)
		{
			char * cStationName = new char[MAX_VRSH];
			strcpy(cStationName, pec->m_strStationName);
			vr = new VR(0x0008, 0x1010, strlen(cStationName), (void *)cStationName);
   			pDOOut->Push(vr);
		}
//Manufacturer Model Name
		vr = pDO->GetVR(0x0008, 0x1090);
		if(vr)
		{
			char * cManufacturerModelName = new char[MAX_VRLO];
			strcpy(cManufacturerModelName, pec->m_strManufacturerModelName);
			vr = new VR(0x0008, 0x1090, strlen(cManufacturerModelName), (void *)cManufacturerModelName);
   			pDOOut->Push(vr);
		}
	}
	//Image
	if(nLevel>3)
	{
		pic=pec->m_ImageInfo.Get(nImage);
//Image Number
		vr = pDO->GetVR(0x0020, 0x0013);
		if(vr)
		{
			char * cImageNumber = new char[MAX_VRIS];
			strcpy(cImageNumber, pic->m_strImageNumber);
			vr = new VR(0x0020, 0x0013, strlen(cImageNumber), (void *)cImageNumber);
   			pDOOut->Push(vr);
		}
//SOPClass UID
//		vr = pDO->GetVR(0x0008, 0x0016);
//		if(vr&&vr->Length>0)
//		{
			char * cSOPClassUID = new char[MAX_VRUI];
			strcpy(cSOPClassUID, pic->m_strSOPClassUID);
			vr = new VR(0x0008, 0x0016, strlen(cSOPClassUID), (void *)cSOPClassUID);
   			pDOOut->Push(vr);
//		}

//SOPInstance UID
	//	vr = pDO->GetVR(0x0008, 0x0018);
	//	if(vr)
	//	{
			char * cSOPInstanceUID = new char[MAX_VRUI];
			strcpy(cSOPInstanceUID, pic->m_strSOPInstanceUID);
			vr = new VR(0x0008, 0x0018, strlen(cSOPInstanceUID), (void *)cSOPInstanceUID);
   			pDOOut->Push(vr);
	//	}

//Image Type
		vr = pDO->GetVR(0x0008, 0x0008);
		if(vr)//ZXH
		{
			char * cImageType = new char[MAX_VRCS*8];
			strcpy(cImageType, pic->m_strImageType);
			vr = new VR(0x0008, 0x0008, strlen(cImageType), (void *)cImageType);
   			pDOOut->Push(vr);
		}

//Volume
		//FileSetUID==Volume
		vr = pDO->GetVR(0x0088, 0x0140);
		if(vr)
		{
			char * cVolume = new char[50];
			strcpy(cVolume, pic->m_Volume);
			vr = new VR(0x0088, 0x0140, strlen(cVolume), (void *)cVolume);
   			pDOOut->Push(vr);
		}

//InstanceAvailability
		vr = pDO->GetVR(0x0008, 0x0056);
		if(vr)
		{
			char * cInstanceAvailability = new char[20];
			strcpy(cInstanceAvailability, pic->m_InstanceAvailability);
			vr = new VR(0x0008, 0x0056, strlen(cInstanceAvailability), (void *)cInstanceAvailability);
   			pDOOut->Push(vr);
		}

//ImageComment
		vr = pDO->GetVR(0x0020, 0x4000);
		if(vr)
		{
			char * cImageComment = new char[MAX_VRLO];
			strcpy(cImageComment, pic->m_strImageComment);
			vr = new VR(0x0020, 0x4000, strlen(cImageComment), (void *)cImageComment);
   			pDOOut->Push(vr);
		}

//Photometric
		vr = pDO->GetVR(0x0028, 0x0004);
		if(vr)
		{
			char * cPhotometric = new char[MAX_VRCS*4];
			strcpy(cPhotometric, pic->m_strPhotometric);
			vr = new VR(0x0028, 0x0004, strlen(cPhotometric), (void *)cPhotometric);
   			pDOOut->Push(vr);
		}

//Transducer Data
		vr = pDO->GetVR(0x0018, 0x5010);
		if(vr)
		{
			char * cTransducerData = new char[MAX_VRLO];
			strcpy(cTransducerData, pic->m_strTransducerData);
			vr = new VR(0x0018, 0x5010, strlen(cTransducerData), (void *)cTransducerData);
   			pDOOut->Push(vr);
		}

//Transducer Type
		vr = pDO->GetVR(0x0018, 0x6031);
		if(vr)
		{
			char * cTransducerType = new char[MAX_VRCS*4];
			strcpy(cTransducerType, pic->m_strTransducerType);
			vr = new VR(0x0018, 0x6031, strlen(cTransducerType), (void *)cTransducerType);
   			pDOOut->Push(vr);
		}
//		vr = pDO->GetVR(0x0008, 0x0005);
//		if(vr)
//		{
//			char * cSpecificCharacterSet = new char[MAX_VRCS*4];
//			strcpy(cSpecificCharacterSet, pic->m_strSpecificCharacterSet);
//			vr = new VR(0x0008, 0x0005, strlen(cSpecificCharacterSet), (void *)cSpecificCharacterSet);
// 			pDOOut->Push(vr);
//		}

//File Name
		vr = pDO->GetVR(0x0004, 0x1500);
		if(bRetrieveSearch||vr)
		{
			char * cFileName = new char[MAX_VRCS*8];
			strcpy(cFileName, pic->m_strFileName);
			vr = new VR(0x0004, 0x1500, strlen(cFileName), (void *)cFileName);
   			pDOOut->Push(vr);
		}
	}
	return 0;
}
int	FillDDOArray(Array<PatientInfo*> * pPatientList,DICOMDataObject *pDO,Array<DICOMDataObject *> *arrayDDO,BOOL bRetrieveSearch)
{
	int nLevel;
	DICOMDataObject*pDOOut;
//(0008,0052)	Query/Retrieve Level
	VR* vr = pDO->GetVR(0x0008, 0x0052);
	if(strnicmp((char*)vr->Data,"PATIENT",7)==0)nLevel=1;
	else if(strnicmp((char*)vr->Data,"STUDY",5)==0)nLevel=2;
	else if(strnicmp((char*)vr->Data,"SERIES",6)==0)nLevel=3;
	else if(strnicmp((char*)vr->Data,"IMAGE",5)==0)nLevel=4;
	else return -1;
	//[add by peak.when i test it in chaoyang hospital i find that it can not return the image when it retrieve a whole series
	//so i add these two line below
	if(bRetrieveSearch)
		nLevel=4;
	//add]
	int nPatient=pPatientList->GetSize();
	for(int i=0;i<nPatient;i++)
	{
		if(nLevel==1)
//只需要返回Patient这一级
		{
			pDOOut=new DICOMDataObject;
			FillDDO(pPatientList,i,0,0,0,pDO,pDOOut,nLevel,bRetrieveSearch);
			arrayDDO->Add(pDOOut);
//只需要返回Patient这一级
			continue;
		}
		Array<StudyInfo*>*pStudyList=&(pPatientList->Get(i)->m_StudyInfo);
		int nStudy=pStudyList->GetSize();
		for(int j=0;j<nStudy;j++)
		{
			if(nLevel==2)
//只需要返回Patient/study
			{
				pDOOut=new DICOMDataObject;
				FillDDO(pPatientList,i,j,0,0,pDO,pDOOut,nLevel,bRetrieveSearch);
				arrayDDO->Add(pDOOut);
//只需要返回Patient/study
				continue;
			}

			Array<SeriesInfo*>*pSeriesList=&(pStudyList->Get(j)->m_SeriesInfo);
			int nSeries=pSeriesList->GetSize();
			for(int k=0;k<nSeries;k++)
			{
				if(nLevel==3)
//只需要返回Patient/study/series
				{
					pDOOut=new DICOMDataObject;
					FillDDO(pPatientList,i,j,k,0,pDO,pDOOut,nLevel,bRetrieveSearch);
					arrayDDO->Add(pDOOut);
//只需要返回Patient/study/series
					continue;
				}
//需要返回Patient/study/series/image
				Array<ImageInfo*>*pImageList=&(pSeriesList->Get(k)->m_ImageInfo);
				int nImage=pImageList->GetSize();
				for(int l=0;l<nImage;l++)
				{
					pDOOut=new DICOMDataObject;
					FillDDO(pPatientList,i,j,k,l,pDO,pDOOut,nLevel,bRetrieveSearch);
					arrayDDO->Add(pDOOut);
				}
			}
		}
	}
	return 0;
}

BOOL StandardQRSearchOn (DICOMDataObject *pDO, Array<DICOMDataObject *> *arrayDDO,BOOL bRetrieveSearch)
{
	VR*vr;
//查询条件变量
	PatientCondition pc;
	StudyCondition sc;
	SeriesCondition ec;
	ImageCondition ic;
	memset((void*)&pc,0,sizeof(pc));
	memset((void*)&sc,0,sizeof(sc));
	memset((void*)&ec,0,sizeof(ec));
	memset((void*)&ic,0,sizeof(ic));
//提取查询条件变量

	////设置Patient查询条件
	//Patient Name
	vr = pDO->GetVR(0x0010, 0x0010);
	if(vr&&vr->Length>0)
	{
		trimcpy(pc.m_strPatientName,vr->Data,vr->Length);
	}
	//Patient ID
	vr = pDO->GetVR(0x0010, 0x0020);
	if(vr&&vr->Length>0)
	{
		trimcpy(pc.m_strPatientID,vr->Data,vr->Length);
	}
	//Patient BirthDate
	vr = pDO->GetVR(0x0010, 0x0030);
	if(vr&&vr->Length>0)
	{
		mdate::splite(pc.m_timMinPatientBirthDate,pc.m_timMaxPatientBirthDate,vr);
	}
	//Sex
	vr = pDO->GetVR(0x0010, 0x0040);
	if(vr&&vr->Length>0)
	{
		trimcpy(pc.m_PatientSex,vr->Data,vr->Length);
	}

	////设置study查询条件
	//Study Date
	vr = pDO->GetVR(0x0008, 0x0020);
	if(vr&&vr->Length>0)
	{
		mdate::splite(sc.m_timMinStudyDate,sc.m_timMaxStudyDate,vr);
	}
	//Study Time
	vr = pDO->GetVR(0x0008, 0x0030);
	if(vr&&vr->Length>0)
	{
		mtime::splite(sc.m_timMinStudyTime,sc.m_timMaxStudyTime,vr);
	}
	//Accession Number
	vr = pDO->GetVR(0x0008, 0x0050);
	if(vr&&vr->Length>0)
	{
		trimcpy(sc.m_strAccessionNumber,vr->Data,vr->Length);
	}
	//StudyID
	vr = pDO->GetVR(0x0020, 0x0010);
	if(vr&&vr->Length>0)
	{
		trimcpy(sc.m_strStudyID,vr->Data,vr->Length);
	}
	//Study InstanceUID
	vr = pDO->GetVR(0x0020, 0x000D);
	if(vr&&vr->Length>0)
	{
		trimcpy(sc.m_strStudyInstanceUID,vr->Data,vr->Length);
	}
	//StudyDescription
	vr = pDO->GetVR(0x0008, 0x1030);
	if(vr&&vr->Length>0)
	{
		trimcpy(sc.m_strStudyDescription,vr->Data,vr->Length);
	}
	//Admit Description
	vr = pDO->GetVR(0x0008, 0x1080);
	if(vr&&vr->Length>0)
	{
		trimcpy(sc.m_strAdmitDescription,vr->Data,vr->Length);
	}

	//设置series查询条件
	//Modality
	vr = pDO->GetVR(0x0008, 0x0060);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strModality,vr->Data,vr->Length);
	}
	//Series Number
	vr = pDO->GetVR(0x0020, 0x0011);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strSeriesNumber,vr->Data,vr->Length);
	}
	//Series InstanceUID
	vr = pDO->GetVR(0x0020, 0x000E);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strSeriesInstanceUID,vr->Data,vr->Length);
	}
	//Physician Name
	vr = pDO->GetVR(0x0008, 0x1050);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strPhysicianName,vr->Data,vr->Length);
	}
	//Series Description
	vr = pDO->GetVR(0x0008, 0x103E);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strSeriesDescription,vr->Data,vr->Length);
	}
	//BodyPart
	vr = pDO->GetVR(0x0018, 0x0015);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strBodyPart,vr->Data,vr->Length);
	}
	//Manufacturer
	vr = pDO->GetVR(0x0008, 0x0070);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strManufacturer,vr->Data,vr->Length);
	}
	//StationName
	vr = pDO->GetVR(0x0008, 0x1010);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strStationName,vr->Data,vr->Length);
	}
	//ManufacturerModelName
	vr = pDO->GetVR(0x0008, 0x1090);
	if(vr&&vr->Length>0)
	{
		trimcpy(ec.m_strManufacturerModelName,vr->Data,vr->Length);
	}

	//设置Image查询条件
	//Image
	vr = pDO->GetVR(0x0020, 0x0013);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strImageNumber,vr->Data,vr->Length);
	}

	//SOPInstanceUID
	vr = pDO->GetVR(0x0008, 0x0018);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strSOPInstanceUID,vr->Data,vr->Length);
	}
	//ImageType
	vr = pDO->GetVR(0x0008, 0x0008);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strImageType,vr->Data,vr->Length);
	}

	//FileSetUID==Volume
	vr = pDO->GetVR(0x0088, 0x0140);
	if(vr && vr->Length > 0)
	{
		trimcpy(ic.m_Volume,vr->Data,vr->Length);
	}
	//InstanceAvailability
	vr = pDO->GetVR(0x0008, 0x0056);
	if(vr && vr->Length > 0)
	{
		trimcpy(ic.m_InstanceAvailability,vr->Data,vr->Length);
	}

	//Image Comment
	vr = pDO->GetVR(0x0020, 0x4000);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strImageComment,vr->Data,vr->Length);
	}
	//Photometric
	vr = pDO->GetVR(0x0028, 0x0004);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strPhotometric,vr->Data,vr->Length);
	}

	//TransducerData
	vr = pDO->GetVR(0x0018, 0x5010);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strTransducerData,vr->Data,vr->Length);
	}
	//TransducerType
	vr = pDO->GetVR(0x0018, 0x6031);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strTransducerType,vr->Data,vr->Length);
	}
	//SpecificCharacterSet
	vr = pDO->GetVR(0x0008, 0x0005);
	if(vr&&vr->Length>0)
	{
		trimcpy(ic.m_strSpecificCharacterSet,vr->Data,vr->Length);
	}
//	
//	char strGlobalDICOMDIRPath[MAX_PATH];
//	GetImageDir(strGlobalDICOMDIRPath);
//	strcat(strGlobalDICOMDIRPath,"dicomdir");
	Array<PatientInfo*> *pPatientList;
	//此处查询数据库,并将数据返回填充到pPatientList里面
	return FALSE;
	//将查询返回的PatientList填充到DDOArray
	if(FillDDOArray(pPatientList,pDO,arrayDDO,bRetrieveSearch)==-1)
	{
		return FALSE;
	}
	return TRUE;
}

MyPatientRootQuery::MyPatientRootQuery ()
{
	m_nLevelNumber = 100;
}


MyPatientRootQuery::~MyPatientRootQuery ()
{
	m_nLevelNumber = 0;  //clear all arrays
	RemoveArray();
}
BOOL MyPatientRootQuery::CallBack (DICOMCommandObject* DCO, DICOMDataObject* DDO)
{
	return StandardQRCallBack(m_nLevelNumber,m_PatientArray,DCO,DDO);
}

Array<PatientInfo*>* MyPatientRootQuery::GetPatientArray ()
{
	return &m_PatientArray;
}

void MyPatientRootQuery::RemoveArray ()
{
	while (m_PatientArray.GetSize()-1 != -1)
	{
		delete m_PatientArray.Get(m_PatientArray.GetSize()-1);
		m_PatientArray.RemoveAt(m_PatientArray.GetSize()-1);
	}
}

BOOL MyPatientRootQuery::SearchOn (DICOMDataObject *pDO, Array<DICOMDataObject *> *arrayDDO)
{
	return StandardQRSearchOn(pDO,arrayDDO,FALSE);
}
BOOL MyPatientRootQuery::uGetUID (UID &uid)
{
	return (GetUID(uid));
}

void MyPatientRootQuery::SetLevelNumber (int nLevelNumber)
{
	m_nLevelNumber = nLevelNumber;
}


MyStudyRootQuery::MyStudyRootQuery ()
{
	m_nLevelNumber = 100;
}


MyStudyRootQuery::~MyStudyRootQuery ()
{
	m_nLevelNumber = 0;  //clear all array.
	RemoveArray();
}



BOOL MyStudyRootQuery::CallBack (DICOMCommandObject* DCO, DICOMDataObject*DDO )
{
	return StandardQRCallBack(m_nLevelNumber,m_PatientArray,DCO,DDO);
}

Array<PatientInfo*>* MyStudyRootQuery::GetPatientArray ()
{
	return &m_PatientArray;
}

void MyStudyRootQuery::RemoveArray ()
{
	while (m_PatientArray.GetSize()-1 != -1)
	{
		delete m_PatientArray.Get(m_PatientArray.GetSize()-1);
		m_PatientArray.RemoveAt(m_PatientArray.GetSize()-1);
	}
}

BOOL MyStudyRootQuery::SearchOn (DICOMDataObject *DDO, Array<DICOMDataObject *> *arrayDDO)
{
	return StandardQRSearchOn(DDO,arrayDDO,FALSE);
}

BOOL MyStudyRootQuery::uGetUID (UID &uid)
{
	return GetUID(uid);
}

void MyStudyRootQuery::SetLevelNumber (int nLevelNumber)
{
	m_nLevelNumber = nLevelNumber;
}

// Call back is not required to do anything, but return TRUE, and
// not destroy/alter the DICOMObjects passed in.  The most simple
// CallBack() routine is a return ( TRUE ); statement.

MyStudyRootRetrieve::MyStudyRootRetrieve()
{
	bSwap=FALSE;
	nQueueID=-1;

}
BOOL StandardRetrieveCallBack(int nQueueID,int nMsg,
	DICOMCommandObject	*DCO,
	DICOMDataObject		*DDO )
{
	UINT16 leNumberRemaining,leNumberComplete,leNumberFailed;
	leNumberRemaining=DCO->GetUINT16(0x0000, 0x1020);
	leNumberComplete=DCO->GetUINT16(0x0000, 0x1021);
	leNumberFailed=DCO->GetUINT16(0x0000, 0x1022);

/*	char strFileName[MAX_PATH];
	GetMonitorServiceFile(2,strFileName);
	CServiceQueue sq;
	sq.Load(strFileName);
	SERVICEINFO*pInfo;
	if(sq.GetAt(nQueueID,pInfo)==TRUE)
	{
		if(pInfo->strFormat)
			delete pInfo->strFormat;
		char*str=new char[50];
		sprintf(str,"%d,%d,%d",leNumberComplete,leNumberFailed,leNumberRemaining);
		pInfo->strFormat=str;
		sq.Save(strFileName);
	}
*///zxh

	return TRUE;
}
BOOL	MyStudyRootRetrieve	::	CallBack (
	DICOMCommandObject	*DCO,
	DICOMDataObject		*DDO )
{

	if (! DCO )
	{
		nMsg=RegisterWindowMessage("RetrieveStatus");
		return ( TRUE );	// Initial call back (start)
	}
	if (! DDO )
	{
		StandardRetrieveCallBack(nQueueID,nMsg,DCO,DDO);
		return ( TRUE );	// final call back (end)
	}

	StandardRetrieveCallBack(nQueueID,nMsg,DCO,DDO);

	return ( TRUE );
}

// SearchOn presents the hook into your database.  Here you should
// perform your SQL quary, or link to any other database routines.

BOOL	MyStudyRootRetrieve	::	SearchOn (
	DICOMDataObject				*DDO,
	Array < DICOMDataObject	*>	*ADDO)
{
	
	return StandardQRSearchOn(DDO,ADDO,TRUE);	
}

// Retrieve on is like SearchOn in that it is the hook into
// the database.  However, this time you should actually retrieve
// the entire image, and not just the attributes asked for.

BOOL	MyStudyRootRetrieve	::	RetrieveOn (
	DICOMDataObject	*DDO,
	DICOMDataObject	**pRDDO,
	StandardStorage	**SOPClass)
	{
	return StandardRetrieveOn(DDO,pRDDO,SOPClass,bSwap,RTCStorage);
}
// QualifyOn is a way of looking up known ACR-NEMA addresses.
// In most cases you should check whatever table you have
// set up for storing other DICOM entities, and return the
// parameters.  The first parameter is passed in, the final
// three on actually value-result,(i.e., you fill them in).

BOOL	MyStudyRootRetrieve	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort)
{
	return StandardQualifyOn(ACRNema,MyACRNema,RemoteIP,RemotePort,bSwap);
}
// Call back is not required to do anything, but return TRUE, and
// not destroy/alter the DICOMObjects passed in.  The most simple
// CallBack() routine is a return ( TRUE ); statement.

MyPatientRootRetrieve::MyPatientRootRetrieve()
{
	nQueueID=-1;
	bSwap=FALSE;
}

BOOL	MyPatientRootRetrieve	::	CallBack (
	DICOMCommandObject	*DCO,
	DICOMDataObject		*DDO )
{

	if (! DCO )
	{
		nMsg=RegisterWindowMessage("RetrieveStatus");
		return ( TRUE );	// Initial call back (start)
	}
	if (! DDO )
	{
		StandardRetrieveCallBack(nQueueID,nMsg,DCO,DDO);
		return ( TRUE );	// final call back (end)
	}

	StandardRetrieveCallBack(nQueueID,nMsg,DCO,DDO);

	return ( TRUE );

}

// SearchOn presents the hook into your database.  Here you should
// perform your SQL quary, or link to any other database routines.

BOOL	MyPatientRootRetrieve	::	SearchOn (
	DICOMDataObject				*DDO,
	Array < DICOMDataObject	*>	*ADDO)
{
	return StandardQRSearchOn(DDO,ADDO,TRUE);	
}

// Retrieve on is like SearchOn in that it is the hook into
// the database.  However, this time you should actually retrieve
// the entire image, and not just the attributes asked for.

BOOL	MyPatientRootRetrieve	::	RetrieveOn (
	DICOMDataObject	*DDO,
	DICOMDataObject	**pRDDO,
	StandardStorage	**SOPClass)
{
	return StandardRetrieveOn(DDO,pRDDO,SOPClass,bSwap,RTCStorage);

}
// QualifyOn is a way of looking up known ACR-NEMA addresses.
// In most cases you should check whatever table you have
// set up for storing other DICOM entities, and return the
// parameters.  The first parameter is passed in, the final
// three on actually value-result,(i.e., you fill them in).
BOOL	MyPatientRootRetrieve	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort)
{
	return StandardQualifyOn(ACRNema,MyACRNema,RemoteIP,RemotePort,bSwap);
}


MyModalityWorklistQuery::MyModalityWorklistQuery ()
{
	pGlobalWorkListArray=NULL;
}


MyModalityWorklistQuery::~MyModalityWorklistQuery ()
{
	RemoveArray();
}

BOOL MyModalityWorklistQuery::CallBack (DICOMCommandObject* DCO, DICOMDataObject* DDO)
{
	VR	*vr;

	if(pGlobalWorkListArray==NULL)
		pGlobalWorkListArray=new Array<WorkListInfo*>;
	if (! DCO )
	{
		RemoveArray();
		return TRUE;	// Initial call back (start)
	}
	if (! DDO )
		return TRUE;	// final call back (end)

	WorkListInfo*pWorkList=new WorkListInfo;
	memset(pWorkList,0, sizeof(WorkListInfo));
	//Patient Name
	vr = DDO->GetVR(0x0010, 0x0010);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_strPatientName,vr->Data,vr->Length);
	}

	//Patient ID
	vr = DDO->GetVR(0x0010, 0x0020);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_strPatientID,vr->Data,vr->Length);
	}
	else
	{
		delete pWorkList;
		return FALSE;
	}

	vr = DDO->GetVR(0x0020, 0x000D);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_strStudyInstanceUID,vr->Data,vr->Length);
	}
/*	else
	{
		ABNORMALLOG("Exception occurs when querying study,this study has no Study Instance UID!");
		delete pStudy;
		return FALSE;
	}
*/
	vr = DDO->GetVR(0x0040, 0x0001);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strScheduledStationAET,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0002);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_timMaxScheduledStudyDate,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0003);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_timMaxScheduledStudyTime,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0009);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strScheduledProcedureStepID,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x1001);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strRequestedProcedureID,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0008, 0x0050);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_strAccessionNumber,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0008, 0x0060);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_strModality,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0008, 0x0090);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_ReferringPhysicianName,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x0030);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_timMaxPatientBirthDate,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x0040);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->m_PatientSex,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x1030);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPathentWeight,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x2000);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strMedicalAlerts,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x2110);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strContrastAllergies,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0010, 0x21C0);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPregnancyStatus,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0032, 0x1032);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strRequestingPhysician,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0032, 0x1070);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strRequestedContrastAgent,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0038, 0x0010);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strAdmissionID,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0038, 0x0050);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strSpecialNeeds,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0038, 0x0300);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strCurrentPatientLocation,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0038, 0x0500);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPatientState,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0006);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPerformingPhysician,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0010);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strScheduledStationName,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0011);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strScheduledProcedureStepLocation,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x0012);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPreOrderMedication,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x1003);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strRequestedProcedurePriority,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x1004);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strPatientTransportArrangements,vr->Data,vr->Length);
	}
	vr = DDO->GetVR(0x0040, 0x3001);
	if(vr && vr->Length > 0)
	{
		trimcpy(pWorkList->strConfidentialityConstraint,vr->Data,vr->Length);
	}

	pGlobalWorkListArray->Add(pWorkList);
	return TRUE;
}


BOOL MyModalityWorklistQuery::RemoveArray ()
{
	if(pGlobalWorkListArray==NULL)
		return TRUE;
	unsigned int nLen=pGlobalWorkListArray->GetSize();
	for(unsigned int i=0;i<nLen;i++)
	{
		delete pGlobalWorkListArray->Get(i);
	}
	delete pGlobalWorkListArray;
	pGlobalWorkListArray=NULL;
	return TRUE;
}	
BOOL MyModalityWorklistQuery::uGetUID (UID &uid)
{
	return (GetUID(uid));
}
