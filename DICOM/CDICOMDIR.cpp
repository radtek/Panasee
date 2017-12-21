// DICOMDIR.cpp: implementation of the DICOMDIR class.
//
//////////////////////////////////////////////////////////////////////

#	include	"dicom.hpp"
#	include	"iod.hpp"
#	include	"iodbase.hpp"
#	include	"comp.hpp"
#	include "CDICOMDIR.hpp"

#include <shlwapi.h>
#include <direct.h>
#include <time.h>
#include <sys/timeb.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//构造函数
DICOMDIR::DICOMDIR()
{

}
//析构函数
DICOMDIR::~DICOMDIR()
{

}
//得到uid
BOOL DICOMDIR::GetUID(UID &uid)
{
	uid.Set("1.2.840.10008.1.3.10");
	return ( TRUE );
}
//创建DICOMDIR
BOOL DICOMDIR::CreateDICOMDIR(
	DICOMDataObject *pDDO,
	char *strDICOMDIRRoot,
	char *strReferencedFileID,
	char *strFileSetID,
	char *strFileSetUID,
	UINT nTransferSyntax)
{
	FileBuffer		IOBuffer;
	FILE			*fp;
	char			s[264];

	DICOMDataObject DDO;
	DICOMDataObject DDOMeta;
	DICOMDataObject DDOTemp;

	BasicDirectoryIODModule BasicDirectoryIOD;
	
	UID	uid;

	GetUID(uid);
//memory leak	VR vrSOPClassUID(0x0008, 0x0016, uid.GetSize(), uid.GetBuffer(0), FALSE);
	VR *vrSOPClassUID=new VR(0x0008, 0x0016, uid.GetSize(),TRUE);
	memset(vrSOPClassUID->Data, 0, vrSOPClassUID->Length);
	memcpy(vrSOPClassUID->Data, (void*)uid.GetBuffer(0), vrSOPClassUID->Length);
	DDOMeta.Push(vrSOPClassUID);
	

//	VR vrSOPClassInstanceUID(0x0008, 0x0018, lstrlen(strFileSetUID), strFileSetUID, FALSE);
	VR *vrSOPClassInstanceUID=new VR(0x0008, 0x0018, lstrlen(strFileSetUID),TRUE);
	memset(vrSOPClassInstanceUID->Data, 0, vrSOPClassInstanceUID->Length);
	memcpy(vrSOPClassInstanceUID->Data, (void*)strFileSetUID, vrSOPClassInstanceUID->Length);
	DDOMeta.Push(vrSOPClassInstanceUID);

	PDU_Service::MakeFileMeta(&DDOMeta,nTransferSyntax);

	VR * vr;
	// Strip away the group 0008
	while(vr = DDOMeta.Pop())
		{
		if(vr->Group!=0x0008)
			DDOTemp.Push(vr);
		else
			delete vr;
		}
	while(vr = DDOTemp.Pop())
		DDOMeta.Push(vr);

	unsigned long lMetaLength = DDOMeta.Length;
	switch(nTransferSyntax) 
	{
		case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			if(!Explicit_ParseDCMIntoRawVR(&DDOMeta, IOBuffer))
				return FALSE;
			break;
		case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			{
				DICOMDataObject tmpddo;
				VR*vrt;
				while(vrt=DDOMeta.Pop())
				{
					vrt->TypeCode=0x0000;
					tmpddo.Push(vrt);
				}
				if(!Implicit_ParseDCMIntoRawVR(&tmpddo, IOBuffer))
					return FALSE;
				while(vrt=tmpddo.Pop())
				{
					vrt->TypeCode=0x0000;
					DDOMeta.Push(vrt);
				}				
			}
			break;
		default:
			return FALSE;
	}

//	if(!Explicit_ParseDCMIntoRawVR(&DDOMeta, IOBuffer))
//		return FALSE;

//File-set ID	(0004,1130)	2	User or implementation specific Identifier (up to 16 characters)
	VR vrFileSetID(0x0004, 0x1130, strlen(strFileSetID), strFileSetID, FALSE);
	BasicDirectoryIOD.FileSetIdentificationModule.SetIf(&vrFileSetID);
	BasicDirectoryIOD.FileSetIdentificationModule.Morph(&DDO);
//Offset of the First Directory Record of Root Directory Entity	(0004,1200)	1	
	unsigned long lOffsetofFirstRecordofRootDirectoryEntity;
	VR vrOffsetofFirstRecordofRootDirectoryEntity(0x0004, 0x1200, sizeof(unsigned long), &lOffsetofFirstRecordofRootDirectoryEntity, FALSE);
	BasicDirectoryIOD.DirectoryInformationModule.SetIf(&vrOffsetofFirstRecordofRootDirectoryEntity);
//Offset of the Last Directory Record of Root Directory Entity	(0004,1202)	1	
	unsigned long lOffsetofLastRecordofRootDirectoryEntity;
	VR vrOffsetofLastRecordofRootDirectoryEntity(0x0004, 0x1202, sizeof(unsigned long), &lOffsetofLastRecordofRootDirectoryEntity, FALSE);
	BasicDirectoryIOD.DirectoryInformationModule.SetIf(&vrOffsetofLastRecordofRootDirectoryEntity);

//File-set Consistency Flag	(0004,1212)	1	0000H  = No known inconsistencies FFFFH = The FSR or FSU shall assume that inconsistencies are present
	unsigned short sFileSetConsistencyFlag;
	sFileSetConsistencyFlag = 0x0000;
	VR vrFileSetConsistencyFlag(0x0004, 0x1212, sizeof(unsigned short), &sFileSetConsistencyFlag, FALSE);
	BasicDirectoryIOD.DirectoryInformationModule.SetIf(&vrFileSetConsistencyFlag);

	BasicDirectoryIOD.DirectoryInformationModule.Morph(&DDO);

//Directory Record Sequence	(0004,1220)	2	
	VR vrDirectoryRecordSequence(0x0004,0x1220, 0, FALSE);
	vrDirectoryRecordSequence.Valid = TRUE;
	BasicDirectoryIOD.DirectoryInformationModule.SetIf(&vrDirectoryRecordSequence);
	BasicDirectoryIOD.DirectoryInformationModule.Morph(&DDO);

	switch(nTransferSyntax) 
	{
	case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
		lOffsetofFirstRecordofRootDirectoryEntity = DDO.Length + lMetaLength + 128 + 4 + 8; //For Explicit +8 /Implicit +4
		break;
	case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
		lOffsetofFirstRecordofRootDirectoryEntity = DDO.Length + lMetaLength + 128 + 4 + 4; //For Explicit +8 /Implicit +4
		break;
	case DICOM_MEDIA_EXPLICITBIGENDIAN:
		lOffsetofFirstRecordofRootDirectoryEntity = DDO.Length + lMetaLength + 128 + 4 + 8; //For Explicit +8 /Implicit +4
		break;
	default:
		return FALSE;
	}
	lOffsetofLastRecordofRootDirectoryEntity = lOffsetofFirstRecordofRootDirectoryEntity;

////////////////////////////////////////////////////////////////////////////////////////////
//	Patient
	DICOMDataObject * pDDOPatientDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forPatient = new unsigned long;
	unsigned long *pOffset2forPatient = new unsigned long;
	int nMaxReferenceFileIDLenth = MAX_VRCS;
	nMaxReferenceFileIDLenth = nMaxReferenceFileIDLenth*16;
	TCHAR * strVRReferenceFileIDforPatient = new TCHAR[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforPatient, strReferencedFileID);

	#ifdef _UNICODE
	// Code converting unicode to multi-byte character should be add here.
	#endif 

	if(!CollectPatientDirectoryRecord (pDDOPatientDirectoryRecord, pDDO, pOffset1forPatient, \
		pOffset2forPatient, strVRReferenceFileIDforPatient))
		return FALSE;
	*pOffset1forPatient = 0;
	*pOffset2forPatient = lOffsetofFirstRecordofRootDirectoryEntity + pDDOPatientDirectoryRecord->Length + 8 + 8;
	DDO.Push(pDDOPatientDirectoryRecord);
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//	Study
	DICOMDataObject * pDDOStudyDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forStudy = new unsigned long;
	unsigned long *pOffset2forStudy = new unsigned long;

	TCHAR * strVRReferenceFileIDforStudy = new TCHAR[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforStudy, strReferencedFileID);

	if(!CollectStudyDirectoryRecord (pDDOStudyDirectoryRecord, pDDO, pOffset1forStudy, \
		pOffset2forStudy, strVRReferenceFileIDforStudy))
		return FALSE;
	
	*pOffset1forStudy = 0;
	*pOffset2forStudy = *pOffset2forPatient + pDDOStudyDirectoryRecord->Length + 8 + 8;
	DDO.Push(pDDOStudyDirectoryRecord);
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//	Series
	DICOMDataObject * pDDOSeriesDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forSeries = new unsigned long;
	unsigned long *pOffset2forSeries = new unsigned long;

	TCHAR * strVRReferenceFileIDforSeries = new TCHAR[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforSeries, strReferencedFileID);

	if(!CollectSeriesDirectoryRecord (pDDOSeriesDirectoryRecord, pDDO, pOffset1forSeries, \
		pOffset2forSeries, strVRReferenceFileIDforSeries))
		return FALSE;
	
	*pOffset1forSeries = 0;
	*pOffset2forSeries = *pOffset2forStudy + pDDOSeriesDirectoryRecord->Length + 8 + 8;
	DDO.Push(pDDOSeriesDirectoryRecord);
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//  Image
	DICOMDataObject * pDDOImageDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forImage = new unsigned long;
	unsigned long *pOffset2forImage = new unsigned long;

	TCHAR * strVRReferenceFileIDforImage = new TCHAR[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforImage, strReferencedFileID);

	if(!CollectImageDirectoryRecord (pDDOImageDirectoryRecord, pDDO, pOffset1forImage, \
		pOffset2forImage, strVRReferenceFileIDforImage))
		return FALSE;
	
	*pOffset1forImage = 0;
	*pOffset2forImage = 0;
	DDO.Push(pDDOImageDirectoryRecord);

////////////////////////////////////////////////////////////////////////////////////////////
	switch(nTransferSyntax) 
	{
		case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			if(!Explicit_ParseDCMIntoRawVR(&DDO, IOBuffer))
				return FALSE;
			break;
		case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			{
				DICOMDataObject tmpddo;
				VR*vrt;
				while(vrt=DDO.Pop())
				{
					vrt->TypeCode=0x0000;
					tmpddo.Push(vrt);
				}
				if(!Implicit_ParseDCMIntoRawVR(&tmpddo, IOBuffer))
					return FALSE;
			}
			break;
		default:
			return FALSE;
	}

	UINT32 upper, lower;

	if(!GetFreeDiskSpace(strDICOMDIRRoot, upper, lower))
		return FALSE;

	UINT nFileSize;

	nFileSize = IOBuffer.GetOutgoingSize();

	nFileSize = nFileSize + 128 + 4;

	if( upper == 0)
		if(lower <= nFileSize) {
			PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
			return FALSE;
		}

	fp = fopen(strDICOMDIRRoot, "wb");
	if(!fp)
		return ( FALSE );

	memset((void*)s, 0, 256);
	strcpy(&s[128], "DICM");
	fwrite(s, 1, 128 + 4, fp);


	IOBuffer.fp = fp;
	IOBuffer.Buffer::Flush();
	IOBuffer.Close();
	return ( TRUE );
}
//收集目录信息
BOOL DICOMDIR::CollectDirectorRecord(DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID, char *pDirectoryRecordType)
{
	VR * vr;
	CO_DirectoryRecordSequenceModule DirectoryRecordSequence;

//> Offset of Next Directory Record	(0004,1400)	1C
	VR vrOffsetofNextDirectoryRecord(0x0004,0x1400, sizeof(unsigned long), pOffsetofNextDirectoryRecord);
	DirectoryRecordSequence.SetIf(&vrOffsetofNextDirectoryRecord);

//> Record In-use Flag	(0004,1410)	1C	FFFFH = Record is in use 0000H  = Record is inactive
	unsigned short * pRecordInuseFlag = new unsigned short;
	*pRecordInuseFlag = 0xFFFF;
	VR vrRecordInuseFlag(0x0004, 0x1410, sizeof(unsigned short), pRecordInuseFlag);
	DirectoryRecordSequence.SetIf(&vrRecordInuseFlag);
//> Offset of Ref. Lower-Level Directory Entity	(0004,1420)	1C	
	VR vrOffsetofReferencedLowerLevelDirectoryEntity(0x0004, 0x1420, sizeof(unsigned long), pOffsetofReferenceLowerLevelDirectoryEntity);
	DirectoryRecordSequence.SetIf(&vrOffsetofReferencedLowerLevelDirectoryEntity);
//> Directory Record Type	(0004,1430)	1C	Appl. value: PATIENT, STUDY, SERIES or IMAGE 
	VR vrDirectoryRecordType(0x0004, 0x1430, strlen(pDirectoryRecordType), pDirectoryRecordType);
	DirectoryRecordSequence.SetIf(&vrDirectoryRecordType);
//> Ref. File ID	(0004,1500)	2C	("Path name" concatenated with "file name")
	if(pReferencedFileID) {
		VR vrReferenceFileID(0x0004, 0x1500, strlen(pReferencedFileID), pReferencedFileID);
		DirectoryRecordSequence.SetIf(&vrReferenceFileID);
	}
//> Ref. SOP Class UID in File	(0004,1510)	1C	1.2.840.10008.5.1.4.1.1.6.1 (US) or 1.2.840.10008.5.1.4.1.1.3.1 (US-mf)
	vr = pSourceDDO->GetVR(0x0008, 0x0016);
	if(vr == NULL)
		return FALSE; //EXCEPTION;
	char * strReferenceSOPClassUIDinFile = new char[MAX_VRUI];
	memset((void *) strReferenceSOPClassUIDinFile, 0, MAX_VRUI);
	memcpy(strReferenceSOPClassUIDinFile, (const char *)vr->Data,vr->Length);
	VR vrReferenceSOPClassUIDinFile(0x0004, 0x1510, vr->Length, strReferenceSOPClassUIDinFile);
	DirectoryRecordSequence.SetIf(&vrReferenceSOPClassUIDinFile);
//> Ref. SOP Instance UID in File	(0004,1511)	1C	
	vr = pSourceDDO->GetVR(0x0008, 0x0018);
	if(vr == NULL)
		return FALSE; //EXCEPTION;
	char * strReferenceSOPInstanceUIDinFile = new char[MAX_VRUI];
	memset((void *) strReferenceSOPInstanceUIDinFile, 0, MAX_VRUI);
	memcpy(strReferenceSOPInstanceUIDinFile, (const char *)vr->Data,vr->Length);
	VR vrReferenceSOPInstanceUIDinFile(0x0004, 0x1511, vr->Length, strReferenceSOPInstanceUIDinFile);
	DirectoryRecordSequence.SetIf(&vrReferenceSOPInstanceUIDinFile);
//> Ref. Transfer Syntax UID in File	(0004,1512)	1C	1.2.840.10008.1.2.1 (Exp.VR L.End.) 1.2.840.10008.1.2.5 (RLE Lossless C.)
	vr = pSourceDDO->GetVR(0x0002, 0x0010);
	if(vr == NULL)
		return FALSE; //EXCEPTION;
	char * strReferenceTransferSyntaxUIDinFile = new char[MAX_VRUI];
	memset((void *) strReferenceTransferSyntaxUIDinFile, 0, MAX_VRUI);
	memcpy(strReferenceTransferSyntaxUIDinFile, (const char *)vr->Data,vr->Length);
	VR vrReferenceTransferSyntaxUIDinFile(0x0004, 0x1512, vr->Length, strReferenceTransferSyntaxUIDinFile);
	DirectoryRecordSequence.SetIf(&vrReferenceTransferSyntaxUIDinFile);

	DirectoryRecordSequence.Morph(pTargetDDO);

	return TRUE;

}
//收集病人信息
BOOL DICOMDIR::CollectPatientDirectoryRecord(DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID)
{
	VR * vr;

	char * strDirectoryRecordType = new char[MAX_VRCS];
	strcpy(strDirectoryRecordType, "PATIENT");
	if(!CollectDirectorRecord (pTargetDDO, pSourceDDO, pOffsetofNextDirectoryRecord, \
		pOffsetofReferenceLowerLevelDirectoryEntity, pReferencedFileID, strDirectoryRecordType))
		return FALSE;

	CO_PatientDirectoryRecordModule PatientDirectoryRecord;

//>Specific Character Set	(0008,0005)	1C	IR 100 (Latin Alphabet No. 1)
	vr = pSourceDDO->GetVR(0x0008, 0x0005);
	if(vr) {
		unsigned char * strSpecificCharacterSet = new unsigned char[MAX_VRCS];
		memset( (void *) strSpecificCharacterSet, 0x0000, MAX_VRCS );
		memcpy( (void *) strSpecificCharacterSet, (void *) vr->Data, vr->Length );
		VR vrSpecificCharacterSet(0x0008, 0x0005, vr->Length, strSpecificCharacterSet);
		PatientDirectoryRecord.SetIf(&vrSpecificCharacterSet);
	}

//>Patient's Name	(0010,0010)	2	Received from RIS or entered by user	
	vr = pSourceDDO->GetVR(0x0010, 0x0010);
	if(vr) {
		unsigned char * strPatientsName = new unsigned char[vr->Length+1];
		memset( (void *) strPatientsName, 0x0000, vr->Length+1 );
		memcpy( (void *) strPatientsName, (void *) vr->Data, vr->Length );
		VR vrPatientsName(0x0010, 0x0010, vr->Length, strPatientsName);
		vrPatientsName.Valid = TRUE;
		PatientDirectoryRecord.SetIf(&vrPatientsName);
	} else {
		char * strPatientsName = new char[20];
		memset( (void *) strPatientsName, 0x0000, 20 );
		strcpy(strPatientsName,"NONAME");
		VR vrPatientsName(0x0010, 0x0010, strlen(strPatientsName)+1, strPatientsName);
		vrPatientsName.Valid = TRUE;
		PatientDirectoryRecord.SetIf(&vrPatientsName);

	/*	VR vrPatientsName(0x0010, 0x0010, 0, FALSE);
		vrPatientsName.Valid = TRUE;
		PatientDirectoryRecord.SetIf(&vrPatientsName);
	*/
	}

//>Patient ID	(0010,0020)	1	Received from RIS or entered by user	
	vr = pSourceDDO->GetVR(0x0010, 0x0020);
	if(!vr || vr->Length == 0) 
	{
		char * strPatientID = new char[20];
		memset( (void *) strPatientID, 0x0000, 20 );
		strcpy(strPatientID,"NOPATIENTID");
		VR vrPatientID(0x0010, 0x0020,strlen(strPatientID)+1, strPatientID);
		PatientDirectoryRecord.SetIf(&vrPatientID);
	}
	else
	{
		unsigned char * strPatientID = new unsigned char[MAX_VRLO];
		memset( (void *) strPatientID, 0x0000, MAX_VRLO );
		memcpy( (void *) strPatientID, (void *) vr->Data, vr->Length );
		VR vrPatientID(0x0010, 0x0020, vr->Length, strPatientID);
		PatientDirectoryRecord.SetIf(&vrPatientID);
	}
//>Patient's Birth Date	(0010,0030)	3	Received from RIS or entered by user
	vr = pSourceDDO->GetVR(0x0010, 0x0030);
	if(vr && vr->Length > 0) {
		unsigned char * strPatientsBirthDate = new unsigned char[MAX_VRDA];
		memset( (void *) strPatientsBirthDate, 0x0000, MAX_VRDA );
		memcpy( (void *) strPatientsBirthDate, (void *) vr->Data, vr->Length );
		VR vrPatientsBirthDate(0x0010, 0x0030, vr->Length, strPatientsBirthDate);
		PatientDirectoryRecord.SetIf(&vrPatientsBirthDate);
	}
//>Patient's Sex	(0010,0040)	3	Received from RIS or entered by user
	vr = pSourceDDO->GetVR(0x0010, 0x0040);
	if(vr && vr->Length > 0) {
		unsigned char * strPatientsSex = new unsigned char[MAX_VRCS];
		memset( (void *) strPatientsSex, 0x0000, MAX_VRCS );
		memcpy( (void *) strPatientsSex, (void *) vr->Data, vr->Length );
		VR vrPatientsSex(0x0010, 0x0040, vr->Length, strPatientsSex);
		PatientDirectoryRecord.SetIf(&vrPatientsSex);
	}

	PatientDirectoryRecord.Morph(pTargetDDO);

	return TRUE;

}
//收集病历信息
BOOL DICOMDIR::CollectStudyDirectoryRecord(DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID)
{
	VR * vr;
	char * strDirectoryRecordType = new char[MAX_VRCS];
	strcpy(strDirectoryRecordType, "STUDY");
		if(!CollectDirectorRecord (pTargetDDO, pSourceDDO, pOffsetofNextDirectoryRecord, \
		pOffsetofReferenceLowerLevelDirectoryEntity, pReferencedFileID, strDirectoryRecordType))
		return FALSE;

	CO_StudyDirectoryRecordModule StudyDirectoryRecord;

//>Specific Character Set	(0008,0005)	1C	IR 100 (Latin Alphabet No. 1)
	vr = pSourceDDO->GetVR(0x0008, 0x0005);
	if(vr) {
		unsigned char * strSpecificCharacterSet = new unsigned char[MAX_VRCS];
		memset( (void *) strSpecificCharacterSet, 0x0000,MAX_VRCS);
		memcpy( (void *) strSpecificCharacterSet,(void *) vr->Data, vr->Length);
		VR vrSpecificCharacterSet(0x0008, 0x0005, vr->Length, strSpecificCharacterSet);
		StudyDirectoryRecord.SetIf(&vrSpecificCharacterSet);
	}

//>Study Date	(0008,0020)	1		
	vr = pSourceDDO->GetVR(0x0008, 0x0020);
	if(vr && vr->Length > 0) 
	{
		unsigned char * strStudyDate = new unsigned char[MAX_VRDA];
		memset( (void *) strStudyDate, 0x0000,MAX_VRDA);
		memcpy( (void *) strStudyDate,(void *) vr->Data, vr->Length);
		VR vrStudyDate(0x0008, 0x0020, vr->Length, strStudyDate);
		vrStudyDate.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyDate);
	}
	else
	{
		struct _timeb tb;
		struct tm* ptm;
		_ftime( &tb );
		ptm=gmtime(&(tb.time));
		
		char*str=new char[20];
		sprintf(str,"%04d%02d%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);

		char * strStudyDate = new char[10];
		memset( (void *) strStudyDate, 0x0000,10);
		strcpy(strStudyDate,str);
		VR vrStudyDate(0x0008, 0x0020, strlen(str)+1, strStudyDate);
		vrStudyDate.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyDate);
	}
//>Study Time	(0008,0030)	1
	vr = pSourceDDO->GetVR(0x0008, 0x0030);
	if(vr && vr->Length > 0) {
		unsigned char * strStudyTime = new unsigned char[MAX_VRTM];
		memset( (void *) strStudyTime, 0x0000,MAX_VRTM);
		memcpy( (void *) strStudyTime,(void *) vr->Data, vr->Length);
		VR vrStudyTime(0x0008, 0x0030, vr->Length, strStudyTime);
		vrStudyTime.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyTime);
	} 
	else
	{
		char *temp=new char[17];//for time
		memset(temp,0,17);
		strcpy(temp,"000000.0000");
		VR vr1(0x0008, 0x0030, strlen(temp),temp, TRUE);
		StudyDirectoryRecord.SetIf(&vr1);
	}
//>Study Description	(0008,1030)	2	Zero length value if not entered by user
	vr = pSourceDDO->GetVR(0x0008, 0x1030);
	if(vr) {
		unsigned char * strStudyDescription = new unsigned char[MAX_VRLO];
		memset( (void *) strStudyDescription, 0x0000,MAX_VRLO);
		memcpy( (void *) strStudyDescription,(void *) vr->Data, vr->Length);
		VR vrStudyDescription(0x0008, 0x1030, vr->Length, strStudyDescription);
		vrStudyDescription.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyDescription);
	}
	else 
	{
		VR vrStudyDescription(0x0008, 0x1030, 0, FALSE);
		vrStudyDescription.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyDescription);
	}

//>	Study Instance UID	(0020,000D)	1C	Generated at creation of the Study or received from RIS
	vr = pSourceDDO->GetVR(0x0020, 0x000D);
	if(vr && vr->Length > 0) 
	{
		unsigned char * strStudyInstanceUID = new unsigned char[MAX_VRUI];
		memset( (void *) strStudyInstanceUID, 0x0000,MAX_VRUI);
		memcpy( (void *) strStudyInstanceUID,(void *) vr->Data, vr->Length);
		VR vrStudyInstanceUID(0x0020, 0x000D, vr->Length, strStudyInstanceUID);
		vrStudyInstanceUID.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyInstanceUID);
	} 
	else
	{
		char * strStudyInstanceUID = new char[MAX_VRUI];
		memset( (void *) strStudyInstanceUID, 0x0000,MAX_VRUI);
		NewSOPInstanceUID(DEFAULTSTUDYUIDBASE,strStudyInstanceUID);
		VR vrStudyInstanceUID(0x0020, 0x000D,strlen(strStudyInstanceUID)+1, strStudyInstanceUID);
		vrStudyInstanceUID.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyInstanceUID);
	}
//> Study ID	(0020,0010)	1	Generated at creation of the Study (by the scanner) or entered by user	
	vr = pSourceDDO->GetVR(0x0020, 0x0010);
	if(vr && vr->Length > 0) 
	{
		unsigned char * strStudyID = new unsigned char[MAX_VRSH];
		memset( (void *) strStudyID, 0x0000,MAX_VRSH);
		memcpy( (void *) strStudyID,(void *) vr->Data, vr->Length);
		VR vrStudyID(0x0020, 0x0010, vr->Length, strStudyID);
		vrStudyID.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyID);
	} 
	else
	{
		unsigned char * strStudyID = new unsigned char[MAX_VRSH];
		memset( (void *) strStudyID, 0x0000,MAX_VRSH);
		strcpy((char*)strStudyID,"9999");
		VR vrStudyID(0x0020, 0x0010, 5, strStudyID);
		vrStudyID.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrStudyID);
	}
//> Accession Number	(0008,0050)	2	Zero length value if not received from RIS	
	vr = pSourceDDO->GetVR(0x0008, 0x0050);
	if(vr) {
		unsigned char * strAccessionNumber = new unsigned char[MAX_VRSH];
		memset( (void *) strAccessionNumber, 0x0000,MAX_VRSH);
		memcpy( (void *) strAccessionNumber,(void *) vr->Data, vr->Length);
		VR vrAccessionNumber(0x0008, 0x0050, vr->Length, strAccessionNumber);
		vrAccessionNumber.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrAccessionNumber);
	} 
	else 
	{
		VR vrAccessionNumber(0x0008, 0x0050, 0, FALSE);
		vrAccessionNumber.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrAccessionNumber);
	}

//> Admitting Diagnosis Description	(0008,1080)	3	Not present if not received from RIS
	
	vr = pSourceDDO->GetVR(0x0008, 0x1080);
	if(vr) {
		unsigned char * strAdmittingDiagnosisDescription = new unsigned char[MAX_VRLO];
		memset( (void *) strAdmittingDiagnosisDescription, 0x0000,MAX_VRLO);
		memcpy( (void *) strAdmittingDiagnosisDescription,(void *) vr->Data, vr->Length);
		VR vrAdmittingDiagnosisDescription(0x0008, 0x1080, vr->Length, strAdmittingDiagnosisDescription);
		vrAdmittingDiagnosisDescription.Valid = TRUE;
		StudyDirectoryRecord.SetIf(&vrAdmittingDiagnosisDescription);
	} 
	
	StudyDirectoryRecord.Morph(pTargetDDO);

	return TRUE;

}
//收集序列信息
BOOL DICOMDIR::CollectSeriesDirectoryRecord(DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID)
{
	VR * vr;
	char * strDirectoryRecordType = new char[MAX_VRCS];
	strcpy(strDirectoryRecordType, "SERIES");
	if(!CollectDirectorRecord (pTargetDDO, pSourceDDO, pOffsetofNextDirectoryRecord, \
		pOffsetofReferenceLowerLevelDirectoryEntity, pReferencedFileID, strDirectoryRecordType))
		return FALSE;

	CO_SeriesDirectoryRecordModule SeriesDirectoryRecord;

//>Specific Character Set	(0008,0005)	1C	IR 100 (Latin Alphabet No. 1)
	vr = pSourceDDO->GetVR(0x0008, 0x0005);
	if(vr) 
	{
		unsigned char * strSpecificCharacterSet = new unsigned char[MAX_VRCS];
		memset( (void *) strSpecificCharacterSet, 0x0000,MAX_VRCS);
		memcpy( (void *) strSpecificCharacterSet,(void *) vr->Data, vr->Length);
		VR vrSpecificCharacterSet(0x0008, 0x0005, vr->Length, strSpecificCharacterSet);
		SeriesDirectoryRecord.SetIf(&vrSpecificCharacterSet);
	}
//> Modality	(0008,0060)	1	US		
	vr = pSourceDDO->GetVR(0x0008, 0x0060);
	if(vr) 
	{
		unsigned char * strModality = new unsigned char[MAX_VRCS];
		memset( (void *) strModality, 0x0000,MAX_VRCS);
		memcpy( (void *) strModality,(void *) vr->Data, vr->Length);
		VR vrModality(0x0008, 0x0060, vr->Length, strModality);
		vrModality.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrModality);
	} 
	else
	{
		char * strModality = new char[MAX_VRCS];
		memset( (void *) strModality, 0x0000,MAX_VRCS);
		strcpy(strModality,"SC");
		VR vrModality(0x0008, 0x0060,3, strModality);
		vrModality.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrModality);
	}
//>	Series Instance UID	(0020,000E)	1	Generated at creation of the Series
	vr = pSourceDDO->GetVR(0x0020, 0x000E);
	if(vr) 
	{
		unsigned char * strSeriesInstanceUID = new unsigned char[MAX_VRUI];
		memset( (void *) strSeriesInstanceUID, 0x0000,MAX_VRUI);
		memcpy( (void *) strSeriesInstanceUID,(void *) vr->Data, vr->Length);
		VR vrSeriesInstanceUID(0x0020, 0x000E, vr->Length, strSeriesInstanceUID);
		vrSeriesInstanceUID.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrSeriesInstanceUID);
	} 
	else 
	{
		char * strSeriesInstanceUID = new char[MAX_VRUI];
		memset( (void *) strSeriesInstanceUID, 0x0000,MAX_VRUI);
		NewSOPInstanceUID(DEFAULTSERIESUIDBASE,strSeriesInstanceUID);
		VR vrSeriesInstanceUID(0x0020, 0x000E,strlen(strSeriesInstanceUID), strSeriesInstanceUID);
		vrSeriesInstanceUID.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrSeriesInstanceUID);
	}
//> Series Number	(0020,0011)	1
	vr = pSourceDDO->GetVR(0x0020, 0x0011);
	if(vr && vr->Length > 0) 
	{
		unsigned char * strSeriesNumber = new unsigned char[MAX_VRIS];
		memset( (void *) strSeriesNumber, 0x0000,MAX_VRIS);
		memcpy( (void *) strSeriesNumber,(void *) vr->Data, vr->Length);
		VR vrSeriesNumber(0x0020, 0x0011, vr->Length, strSeriesNumber);
		vrSeriesNumber.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrSeriesNumber);
	} 
	else 
	{
		char * strSeriesNumber = new char[MAX_VRIS];
		memset( (void *) strSeriesNumber, 0x0000,MAX_VRIS);
		strcpy(strSeriesNumber,"999");
		VR vrSeriesNumber(0x0020, 0x0011,strlen(strSeriesNumber)+1, strSeriesNumber);
		vrSeriesNumber.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrSeriesNumber);
	}
	
//>Performing Physician's Name	(0008,1050)	3	Not present if neither received from RIS nor entered by user
	vr = pSourceDDO->GetVR(0x0008, 0x1050);
	if(vr) 
	{
		unsigned char * strPerformingPhysiciansName = new unsigned char[MAX_VRPN];
		memset( (void *) strPerformingPhysiciansName, 0x0000,MAX_VRPN);
		memcpy( (void *) strPerformingPhysiciansName,(void *) vr->Data, vr->Length);
		VR vrPerformingPhysiciansName(0x0008, 0x1050, vr->Length, strPerformingPhysiciansName);
		vrPerformingPhysiciansName.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrPerformingPhysiciansName);
	}

//>Series Description	(0008,103E)	3	Not present if not entered by user
	vr = pSourceDDO->GetVR(0x0008, 0x103E);
	if(vr) 
	{
		unsigned char * strSeriesDescription = new unsigned char[MAX_VRLO];
		memset( (void *) strSeriesDescription, 0x0000,MAX_VRLO);
		memcpy( (void *) strSeriesDescription,(void *) vr->Data, vr->Length);
		VR vrSeriesDescription(0x0008, 0x103E, vr->Length, strSeriesDescription);
		vrSeriesDescription.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrSeriesDescription);
	} 
//> Body Part Examined	(0018,0015)	3	Not present if not entered by user.    See defined terms (Table 14.1-5)
	vr = pSourceDDO->GetVR(0x0018, 0x0015);
	if(vr) 
	{
		unsigned char * strBodyPartExamined = new unsigned char[MAX_VRCS];
		memset( (void *) strBodyPartExamined, 0x0000,MAX_VRCS);
		memcpy( (void *) strBodyPartExamined,(void *) vr->Data, vr->Length);
		VR vrBodyPartExamined(0x0018, 0x0015, vr->Length, strBodyPartExamined);
		vrBodyPartExamined.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrBodyPartExamined);
	} 

//> Manufacturer	(0008,0070)	3
	vr = pSourceDDO->GetVR(0x0008, 0x0070);
	if(vr) 
	{
		unsigned char * strManufacturer = new unsigned char[MAX_VRLO];
		memset( (void *) strManufacturer, 0x0000,MAX_VRLO);
		memcpy( (void *) strManufacturer,(void *) vr->Data, vr->Length);
		VR vrManufacturer(0x0008, 0x0070, vr->Length, strManufacturer);
		vrManufacturer.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrManufacturer);
	}
	else
	{
		char * strManufacturer = new char[MAX_VRLO];
		memset( (void *) strManufacturer, 0x0000,MAX_VRLO);
		strcpy(strManufacturer,"HOPESOFT Encode");
		VR vrManufacturer(0x0008, 0x0070, strlen(strManufacturer), strManufacturer);
		vrManufacturer.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrManufacturer);
	}
//> Station Name	(0008,1010)	3	Local AE Title (entered during installation)
	vr = pSourceDDO->GetVR(0x0008, 0x1010);
	if(vr) 
	{
		unsigned char * strStationName = new unsigned char[MAX_VRSH];
		memset( (void *) strStationName, 0x0000,MAX_VRSH);
		memcpy( (void *) strStationName,(void *) vr->Data, vr->Length);
		VR vrStationName(0x0008, 0x1010, vr->Length, strStationName);
		vrStationName.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrStationName);
	} 
//> Manufacturer's Model Name	(0008,1090)	3	BKMED Type 2400 Rev.xx
	vr = pSourceDDO->GetVR(0x0008, 0x1090);
	if(vr) 
	{
		unsigned char * strManufacturersModelName = new unsigned char[MAX_VRLO];
		memset( (void *) strManufacturersModelName, 0x0000,MAX_VRLO);
		memcpy( (void *) strManufacturersModelName,(void *) vr->Data, vr->Length);
		VR vrManufacturersModelName(0x0008, 0x1090, vr->Length, strManufacturersModelName);
		vrManufacturersModelName.Valid = TRUE;
		SeriesDirectoryRecord.SetIf(&vrManufacturersModelName);
	} 
	SeriesDirectoryRecord.Morph(pTargetDDO);
	return TRUE;

}
//收集图象信息
BOOL DICOMDIR::CollectImageDirectoryRecord(DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID)
{
	VR * vr;
	char * strDirectoryRecordType = new char[MAX_VRCS];
	strcpy(strDirectoryRecordType, "IMAGE");
	if(!CollectDirectorRecord (pTargetDDO, pSourceDDO, pOffsetofNextDirectoryRecord, \
		pOffsetofReferenceLowerLevelDirectoryEntity, pReferencedFileID, strDirectoryRecordType))
		return FALSE;

	CO_ImageDirectoryRecordModule ImageDirectoryRecord;

//>Specific Character Set	(0008,0005)	1C	IR 100 (Latin Alphabet No. 1)
	vr = pSourceDDO->GetVR(0x0008, 0x0005);
	if(vr) 
	{
		unsigned char * strSpecificCharacterSet = new unsigned char[MAX_VRCS];
		memset( (void *)  strSpecificCharacterSet, 0x0000, MAX_VRCS );
		memcpy( (void *)  strSpecificCharacterSet, (const unsigned char *)vr->Data, vr->Length );
		VR vrSpecificCharacterSet(0x0008, 0x0005, vr->Length, strSpecificCharacterSet);
		ImageDirectoryRecord.SetIf(&vrSpecificCharacterSet);
	}

//> Instance Number	(0020,0013)	1
//edit by peak(also we think Instance Number can be ignored)
	vr = pSourceDDO->GetVR(0x0020, 0x0013);
	if(vr && vr->Length > 0) 
	{
		unsigned char * strInstanceNumber = new unsigned char[MAX_VRIS];
		memset( (void *) strInstanceNumber, 0x0000,MAX_VRIS);
		memcpy( (void *) strInstanceNumber,(void *) vr->Data, vr->Length);
		VR vrInstanceNumber(0x0020, 0x0013, vr->Length, strInstanceNumber);
		vrInstanceNumber.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrInstanceNumber);
	}
	else
	{
		char * strInstanceNumber = new char[MAX_VRIS];
		memset( (void *) strInstanceNumber, 0x0000,MAX_VRIS);
		strcpy(strInstanceNumber,"999");
		VR vrInstanceNumber(0x0020, 0x0013,strlen(strInstanceNumber)+1, strInstanceNumber);
		vrInstanceNumber.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrInstanceNumber);
	}
	
//> Image Date	(0008,0023)	3
	vr = pSourceDDO->GetVR(0x0008, 0x0023);
	if(vr) 
	{
		unsigned char * strImageDate = new unsigned char[MAX_VRDA];
		memset( (void *) strImageDate, 0x0000,MAX_VRDA);
		memcpy( (void *) strImageDate,(void *) vr->Data, vr->Length);
		VR vrImageDate(0x0008, 0x0023, vr->Length, strImageDate);
		vrImageDate.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrImageDate);
	} 

//> Image Time	(0008,0033)	3	
	vr = pSourceDDO->GetVR(0x0008, 0x0033);
	if(vr) 
	{
		unsigned char * strImageTime = new unsigned char[MAX_VRTM];
		memset( (void *) strImageTime, 0x0000,MAX_VRTM);
		memcpy( (void *) strImageTime,(void *) vr->Data, vr->Length);
		VR vrImageTime(0x0008, 0x0033, vr->Length, strImageTime);
		vrImageTime.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrImageTime);
	}

//> Image Type	(0008,0008)	3	ORIGINAL, PRIMARY, Value 3, Value 4 (see def. terms: Table 14.1-11)
	vr = pSourceDDO->GetVR(0x0008, 0x0008);
	if(vr) 
	{
		int nImageTypeLength = MAX_VRCS;
		nImageTypeLength = nImageTypeLength * 4;
		unsigned char * strImageType = new unsigned char[nImageTypeLength];
		memset( (void *) strImageType, 0x0000,nImageTypeLength);
		memcpy( (void *) strImageType,(void *) vr->Data, vr->Length);
		VR vrImageType(0x0008, 0x0008, vr->Length, strImageType);
		vrImageType.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrImageType);
	}

//> Image Comments	(0020,4000)	3	Not present if neither entered by user nor any annotations are present
	vr = pSourceDDO->GetVR(0x0020, 0x4000);
	if(vr) 
	{
		unsigned char * strImageComments = new unsigned char[MAX_VRLT];
		memset( (void *) strImageComments, 0x0000,MAX_VRLT);
		memcpy( (void *) strImageComments,(void *) vr->Data, vr->Length);
		VR vrImageComments(0x0020, 0x4000, vr->Length, strImageComments);
		vrImageComments.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrImageComments);
	}

//> Photometric Interpretation	(0028,0004)	3	RGB
	vr = pSourceDDO->GetVR(0x0028, 0x0004);
	if(vr) 
	{
		unsigned char * strPhotometricInterpretation = new unsigned char[MAX_VRCS];
		memset( (void *) strPhotometricInterpretation, 0x0000,MAX_VRCS);
		memcpy( (void *) strPhotometricInterpretation,(void *) vr->Data, vr->Length);
		VR vrPhotometricInterpretation(0x0028, 0x0004, vr->Length, strPhotometricInterpretation);
		vrPhotometricInterpretation.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrPhotometricInterpretation);
	}

//> Number of Frames	(0028,0008)	3	Only used for US_mf Image IOD 
	vr = pSourceDDO->GetVR(0x0028, 0x0008);
	if(vr) 
	{
		unsigned char * strNumberOfFrames = new unsigned char[MAX_VRIS];
		memset( (void *) strNumberOfFrames, 0x0000,MAX_VRIS);
		memcpy( (void *) strNumberOfFrames,(void *) vr->Data, vr->Length);
		VR vrNumberOfFrames(0x0028, 0x0008, vr->Length, strNumberOfFrames);
		vrNumberOfFrames.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrNumberOfFrames);
	}

//> Transducer Data	(0018,5010)	3	Transducer Code: 88xx
	vr = pSourceDDO->GetVR(0x0018, 0x5010);
	if(vr) 
	{
		unsigned char * strTransducerData = new unsigned char[MAX_VRLO];
		memset( (void *) strTransducerData, 0x0000,MAX_VRLO);
		memcpy( (void *) strTransducerData,(void *) vr->Data, vr->Length);
		VR vrTransducerData(0x0018, 0x5010, vr->Length, strTransducerData);
		vrTransducerData.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrTransducerData);
	}

//> Transducer Type	(0018,6031)	3	See defined terms (Table 14.1-11)

	vr = pSourceDDO->GetVR(0x0018, 0x6031);
	if(vr) 
	{
		unsigned char * strTransducerType = new unsigned char[MAX_VRCS];
		memset( (void *) strTransducerType, 0x0000,MAX_VRCS);
		memcpy( (void *) strTransducerType,(void *) vr->Data, vr->Length);
		VR vrTransducerType(0x0018, 0x6031, vr->Length, strTransducerType);
		vrTransducerType.Valid = TRUE;
		ImageDirectoryRecord.SetIf(&vrTransducerType);
	}	

	ImageDirectoryRecord.Morph(pTargetDDO);
	
	return TRUE;

}
//添加DDO到DICOMDIR
BOOL DICOMDIR::Add(DICOMDataObject *pDDO, char *strDICOMDIRRoot, char *strReferencedFileID)
{
	FileBuffer		IOBuffer;
	FILE			*fp;
	char			s[264];
	char			strTransferSyntax[MAX_VRUI];
	UINT			nTransferSyntax;
	VR *vr;
	
	memset( (void *) strTransferSyntax, 0, MAX_VRUI);
	DICOMDataObject * pDDODICOMDIR;

//载入DICOMDIR
	pDDODICOMDIR = PDU_Service::LoadDICOMDataObject(strDICOMDIRRoot);
	if(pDDODICOMDIR) 
	{
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0002);
		if(!vr || vr->Length < 20) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		if(TrimMemCmp( vr->Data, (void *) "1.2.840.10008.1.3.10", vr->Length, 20)) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0003);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0001);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0010);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		} else {
			memcpy((void *)strTransferSyntax,vr->Data, vr->Length);
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1130);
		if(!vr) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1200);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1202);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1212);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return FALSE;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1220);
		if(!vr) {
			delete pDDODICOMDIR;
			return FALSE;
		}
	} else
		return FALSE;

	if(strcmp(strTransferSyntax, _ImplicitLittleEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_IMPLICITLITTLEENDIAN;
	else if(strcmp(strTransferSyntax, _ExplicitLittleEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_EXPLICITLITTLEENDIAN;
	else if(strcmp(strTransferSyntax, _ExplicitBigEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_EXPLICITBIGENDIAN;
	else {
		delete pDDODICOMDIR;
		return FALSE;
	}

	DICOMDataObject DDOMeta;
	DICOMDataObject DDOBasicDirectory;
	unsigned long *pOffsetofFirstRecordofRootDirectoryEntity;
	unsigned long *pOffsetofLastRecordofRootDirectoryEntity;
	VR * vrDirectoryRecordSequence;

	while(vr = pDDODICOMDIR->Pop())
	{
		if(vr->Group==0x0002)
			DDOMeta.Push(vr);
		else {
			
			if(vr->Group==0x0004 && vr->Element==0x1200)
				pOffsetofFirstRecordofRootDirectoryEntity = (unsigned long *) vr->Data;

			if(vr->Group==0x0004 && vr->Element==0x1202)
				pOffsetofLastRecordofRootDirectoryEntity = (unsigned long *) vr->Data;

			if(vr->Group==0x0004 && vr->Element==0x1220) {
				vrDirectoryRecordSequence = vr;
				continue;
			}
			DDOBasicDirectory.Push(vr);
		}
	}
	delete	pDDODICOMDIR;
//根据数据格式选择不同的编码
	unsigned long lMetaLength = DDOMeta.Length;
	switch(nTransferSyntax) 
	{
		case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			if(!Explicit_ParseDCMIntoRawVR(&DDOMeta, IOBuffer))
				return FALSE;
			break;
		case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			{
				DICOMDataObject tmpddo;
				VR*vrt;
				while(vrt=DDOMeta.Pop())
				{
					vrt->TypeCode=0x0000;
					tmpddo.Push(vrt);
				}
				if(!Implicit_ParseDCMIntoRawVR(&tmpddo, IOBuffer))
					return FALSE;
			}
			break;
		default:
			return FALSE;
	}
//	if(!Explicit_ParseDCMIntoRawVR(&DDOMeta, IOBuffer))
//		return FALSE;

	Array < DICOMObject *> 		*ArrayPtr = NULL;
	if(vrDirectoryRecordSequence->SQObjectArray == NULL) {
		if(vrDirectoryRecordSequence->Length != 0) {
			delete vrDirectoryRecordSequence;
			return FALSE;
		}
	} else {
		ArrayPtr = (Array<DICOMObject *> *)	vrDirectoryRecordSequence->SQObjectArray;
		vrDirectoryRecordSequence->SQObjectArray = NULL;
		if(vrDirectoryRecordSequence->Length > 0 && vrDirectoryRecordSequence->Data) {
			delete vrDirectoryRecordSequence->Data;
			vrDirectoryRecordSequence->Data = NULL;
		}
		vrDirectoryRecordSequence->Length =0;
		vrDirectoryRecordSequence->ReleaseMemory = FALSE;
	}
	DDOBasicDirectory.Push(vrDirectoryRecordSequence);
	switch(nTransferSyntax) {
	case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
		*pOffsetofFirstRecordofRootDirectoryEntity = DDOBasicDirectory.Length + lMetaLength + 128 + 4 + 8; //For Explicit +8 /Implicit +4
		break;
	case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
		*pOffsetofFirstRecordofRootDirectoryEntity = DDOBasicDirectory.Length + lMetaLength + 128 + 4 + 4; //For Explicit +8 /Implicit +4
		break;
	case DICOM_MEDIA_EXPLICITBIGENDIAN:
		*pOffsetofFirstRecordofRootDirectoryEntity = DDOBasicDirectory.Length + lMetaLength + 128 + 4 + 8; //For Explicit +8 /Implicit +4
		break;
	default:
		return FALSE;
	}
////////////////////////////////////////////////////////////////////////////////////////////
//	Patient
	DICOMDataObject * pDDOPatientDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forPatient = new unsigned long;
	unsigned long *pOffset2forPatient = new unsigned long;

	int nMaxReferenceFileIDLenth = MAX_VRCS;
	nMaxReferenceFileIDLenth = nMaxReferenceFileIDLenth*16;
	char * strVRReferenceFileIDforPatient = new char[nMaxReferenceFileIDLenth];
	//pOffset1forPatient、pOffset2forPatient、strVRReferenceFileIDforPatient都随着pDDOPatientDirectoryRecord析构
	strcpy(strVRReferenceFileIDforPatient, strReferencedFileID);

	if(!CollectPatientDirectoryRecord (pDDOPatientDirectoryRecord, pDDO, pOffset1forPatient, \
		pOffset2forPatient, strVRReferenceFileIDforPatient))
		return FALSE;

////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//	Study
	DICOMDataObject * pDDOStudyDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forStudy = new unsigned long;
	unsigned long *pOffset2forStudy = new unsigned long;

	char * strVRReferenceFileIDforStudy = new char[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforStudy, strReferencedFileID);

	if(!CollectStudyDirectoryRecord (pDDOStudyDirectoryRecord, pDDO, pOffset1forStudy, \
		pOffset2forStudy, strVRReferenceFileIDforStudy))
		return FALSE;
	
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//	Series
	DICOMDataObject * pDDOSeriesDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forSeries = new unsigned long;
	unsigned long *pOffset2forSeries = new unsigned long;

	char * strVRReferenceFileIDforSeries = new char[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforSeries, strReferencedFileID);

	if(!CollectSeriesDirectoryRecord (pDDOSeriesDirectoryRecord, pDDO, pOffset1forSeries, \
		pOffset2forSeries, strVRReferenceFileIDforSeries))
		return FALSE;
	
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//  Image
	DICOMDataObject * pDDOImageDirectoryRecord = new DICOMDataObject;
	unsigned long *pOffset1forImage = new unsigned long;
	unsigned long *pOffset2forImage = new unsigned long;

	char * strVRReferenceFileIDforImage = new char[nMaxReferenceFileIDLenth];
	lstrcpy(strVRReferenceFileIDforImage, strReferencedFileID);

	if(!CollectImageDirectoryRecord (pDDOImageDirectoryRecord, pDDO, pOffset1forImage, \
		pOffset2forImage, strVRReferenceFileIDforImage))
		return FALSE;
	
	*pOffset1forImage = 0;
	*pOffset2forImage = 0;

/*	delete pDDOImageDirectoryRecord;
	delete pDDOPatientDirectoryRecord;
	delete pDDOStudyDirectoryRecord;
	delete pDDOSeriesDirectoryRecord;
	if(ArrayPtr)
	{
		while(ArrayPtr->GetSize()) {
			delete ArrayPtr->Get(0);
			ArrayPtr->RemoveAt(0);
		}
		delete ArrayPtr;
		ArrayPtr=NULL;
	}
	return TRUE;//zxh
*/
////////////////////////////////////////////////////////////////////////////////////////////
	if(ArrayPtr == NULL) {
		*pOffset1forPatient = 0;
		*pOffset2forPatient = *pOffsetofFirstRecordofRootDirectoryEntity + pDDOPatientDirectoryRecord->Length + 8 + 8;
		*pOffset1forStudy = 0;
		*pOffset2forStudy = *pOffset2forPatient + pDDOStudyDirectoryRecord->Length + 8 + 8;
		*pOffset1forSeries = 0;
		*pOffset2forSeries = *pOffset2forStudy + pDDOSeriesDirectoryRecord->Length + 8 + 8;
		*pOffset1forImage = 0;
		*pOffset2forImage = 0;
		DDOBasicDirectory.Push(pDDOPatientDirectoryRecord);
		DDOBasicDirectory.Push(pDDOStudyDirectoryRecord);
		DDOBasicDirectory.Push(pDDOSeriesDirectoryRecord);
		DDOBasicDirectory.Push(pDDOImageDirectoryRecord);
	} else {
		BOOL bLowerConsist;
		BOOL bNewAdded = FALSE;
		int nDeleteFlag=4;
		unsigned long lLastOffsite = *pOffsetofFirstRecordofRootDirectoryEntity;

		try
		{
		if(!WalkThroughBasicDirectory(ArrayPtr, &DDOBasicDirectory,\
									lLastOffsite, TRUE, bLowerConsist,\
									pDDOPatientDirectoryRecord, pDDOStudyDirectoryRecord,\
									pDDOSeriesDirectoryRecord, pDDOImageDirectoryRecord, bNewAdded,\
									*pOffsetofLastRecordofRootDirectoryEntity, nTransferSyntax,nDeleteFlag))
			return FALSE;
		}
		catch(char*str)
		{
			str;
		}

		if(nDeleteFlag>=1)
			delete pDDOPatientDirectoryRecord;
		if(nDeleteFlag>=2)
			delete pDDOStudyDirectoryRecord;
		if(nDeleteFlag>=3)
			delete pDDOSeriesDirectoryRecord;
		if(nDeleteFlag>=4)
			delete pDDOImageDirectoryRecord;

		if(!bNewAdded) {
			return FALSE;
		}

	}
//////////////////////////////////////////////////////////////////////////////////////////
	switch(nTransferSyntax) 
	{
		case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			if(!Explicit_ParseDCMIntoRawVR(&DDOBasicDirectory, IOBuffer))
				return FALSE;
			break;
		case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			{
				DICOMDataObject tmpddo;
				VR*vrt;
				while(vrt=DDOBasicDirectory.Pop())
				{
					vrt->TypeCode=0x0000;
					tmpddo.Push(vrt);
				}
				if(!Implicit_ParseDCMIntoRawVR(&tmpddo, IOBuffer))
					return FALSE;
			}
			break;
		default:
			return FALSE;
	}

//	switch(nTransferSyntax) {
//	case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
//		if(!Explicit_ParseDCMIntoRawVR(&DDOBasicDirectory, IOBuffer))
//			return FALSE;
//		break;
//	default:
//		return FALSE;
//	}

	UINT32 upper, lower;

	if(!GetFreeDiskSpace(strDICOMDIRRoot, upper, lower))
		return FALSE;

	UINT nFileSize;

	nFileSize = IOBuffer.GetOutgoingSize();

	nFileSize = nFileSize + 128 + 4;

	if( upper == 0)
		if(lower <= nFileSize) {
			PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
			return FALSE;
		}

	fp = fopen(strDICOMDIRRoot, "wb");
	if(!fp)
		return ( FALSE );

	memset((void*)s, 0, 256);
	strcpy(&s[128], "DICM");
	fwrite(s, 1, 128 + 4, fp);


	IOBuffer.fp = fp;
	IOBuffer.Buffer::Flush();
	IOBuffer.Close();

	delete ArrayPtr;
	
	return ( TRUE );
}
//得到层信息
BOOL DICOMDIR::GetRecordInformation(DICOMObject * pDDODirectoryRecord, unsigned long * & pOffset1, unsigned long * & pOffset2, RecordType &eRecordType)
{
	VR * vr;
	vr = pDDODirectoryRecord->GetVR(0x0004, 0x1400);
	if(!vr || vr->Length != 0x0004)
		return FALSE;
	pOffset1 = (unsigned long *) vr->Data;

	vr = pDDODirectoryRecord->GetVR(0x0004, 0x1420);
	if(!vr || vr->Length != 0x0004)
		return FALSE;
	pOffset2 = (unsigned long *) vr->Data;

	vr = pDDODirectoryRecord->GetVR(0x0004, 0x1430);
	if(!vr || vr->Length < 1)
		return FALSE;

	char strtmp[20];
	memset(strtmp,0,20);
	strncpy(strtmp,(char*)vr->Data,vr->Length);
	sprintf(strtmp,"len:%d,%0x",vr->Length,*((char*)vr->Data+7));
	if(TrimMemCmp( vr->Data,  (void *) "PATIENT", vr->Length, 7) == 0) 
		eRecordType = PATIENT;
	else if(TrimMemCmp( vr->Data, (void *) "STUDY", vr->Length, 5) == 0) 
		eRecordType = STUDY;
	else if(TrimMemCmp( vr->Data, (void *) "SERIES", vr->Length, 6) == 0) 
		eRecordType = SERIES;
	else if(TrimMemCmp( vr->Data, (void *) "IMAGE", vr->Length, 5) == 0) 
		eRecordType = IMAGE;
	else if (TrimMemCmp(vr->Data,(void *) "OVERLAY", vr->Length, 7) == 0)
		eRecordType = OVERLAY;
	else if (TrimMemCmp(vr->Data,(void *) "MODALITY LUT", vr->Length, 12) == 0)
		eRecordType = MODALITYLUT;
	else if (TrimMemCmp(vr->Data,(void *) "VOI LUT", vr->Length, 7) == 0)
		eRecordType = VOILUT;
	else if (TrimMemCmp(vr->Data,(void *) "CURVE", vr->Length, 5) == 0)
		eRecordType = CURVE;
	else if (TrimMemCmp(vr->Data,(void *) "STORED PRINT", vr->Length, 12) == 0)
		eRecordType = STOREDPRINT;
	else if (TrimMemCmp(vr->Data,(void *) "RT DOSE", vr->Length, 6) == 0)
		eRecordType = RTDOSE;
	else if (TrimMemCmp(vr->Data,(void *) "RT STRUCTURE SET", vr->Length, 16) == 0)
		eRecordType = RTSTRUCTURESET;
	else if (TrimMemCmp(vr->Data,(void *) "RT PLAN", vr->Length, 7) == 0)
		eRecordType = RTPLAN;
	else if (TrimMemCmp(vr->Data,(void *) "RT TREAT RECORD", vr->Length, 15) == 0)
		eRecordType = RTTREATRECORD;
	else if (TrimMemCmp(vr->Data,(void *) "TOPIC", vr->Length, 5) == 0)
		eRecordType = TOPIC;
	else if (TrimMemCmp(vr->Data,(void *) "VISIT", vr->Length, 5) == 0)
		eRecordType = VISIT;
	else if (TrimMemCmp(vr->Data,(void *) "RESULTS", vr->Length, 7) == 0)
		eRecordType = RESULTS;
	else if (TrimMemCmp(vr->Data,(void *) "INTERPRETATION", vr->Length, 14) == 0)
		eRecordType = INTERPRETATION;
	else if (TrimMemCmp(vr->Data,(void *) "STUDY COMPONENT", vr->Length, 15) == 0)
		eRecordType = STUDYCOMPONENT;
	else if (TrimMemCmp(vr->Data,(void *) "PRIVATE", vr->Length, 7) == 0)
		eRecordType = PRIVATE;
	else if (TrimMemCmp(vr->Data,(void *) "MRDR", vr->Length, 4) == 0)
		eRecordType = MRDR;
	else
		return FALSE;
	return TRUE;
}
//一致性检查
BOOL DICOMDIR::CheckPatientRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget)
{
	VR * vrSource;
	VR * vrTarget;

	vrSource = pDDOSource->GetVR(0x0004, 0x1430);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data, (void *) "PATIENT", vrSource->Length, 7))
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0004, 0x1430);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrTarget->Data, (void *) "PATIENT", vrTarget->Length, 7))
		return FALSE;

//>Patient ID	(0010,0020)	1	Received from RIS or entered by user
//zxh The patient name is necessary for uniquely identifing a patient.
	vrSource = pDDOSource->GetVR(0x0010, 0x0020);
	if(!vrSource || vrSource->Length == 0) {
		return FALSE;
	}

	vrTarget = pDDOTarget->GetVR(0x0010, 0x0020);
	if(!vrTarget || vrTarget->Length == 0) {
		return FALSE;
	}

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;

	return TRUE;
}
//查询整个DIR
BOOL DICOMDIR::WalkThroughBasicDirectory(Array < DICOMObject *> *ArrayPtr,
										 DICOMObject * pDDOBasicDirectory,
										 unsigned long & lLastOffsite,
										 BOOL bUpperConsist,
										 BOOL & bCurConsist,
										 DICOMObject * pDDOPatientDirectoryRecord,
										 DICOMObject * pDDOStudyDirectoryRecord,
										 DICOMObject * pDDOSeriesDirectoryRecord,
										 DICOMObject * pDDOImageDirectoryRecord,
										 BOOL &bNewAdded,
										 unsigned long & lRootLastRecordOffsite,
										 UINT nTransferSyntax,
										 int &nDeleteFlag)
{
	if(ArrayPtr->GetSize() < 1) 
		return TRUE;
	
	DICOMObject * pDDODirectoryRecord = ArrayPtr->Get(0);

	unsigned long * pOffset1;
	unsigned long * pOffset2;
	RecordType eRecordType;
	RecordType eRecordTypeTemp;
	
	if(!GetRecordInformation(pDDODirectoryRecord, pOffset1, pOffset2, eRecordType)) {
		while(ArrayPtr->GetSize()) {
			delete ArrayPtr->Get(0);
			ArrayPtr->RemoveAt(0);
		}

		return FALSE;
	}

	switch(eRecordType) {
	case PATIENT:
	case TOPIC:
		lRootLastRecordOffsite = lLastOffsite;
		break;
	}
	pDDOBasicDirectory->Push(pDDODirectoryRecord);
	ArrayPtr->RemoveAt(0);

	unsigned long lDDOLength;
	lDDOLength = pDDODirectoryRecord->Length;

	if(!CheckDICOMObjectLength(pDDODirectoryRecord, nTransferSyntax, lDDOLength))
		return FALSE;
	lLastOffsite = lLastOffsite + lDDOLength + 8 + 8;

	if(bUpperConsist) {
		switch(eRecordType) {
		case PATIENT:
			bCurConsist = CheckPatientRecordConsistent(pDDODirectoryRecord, pDDOPatientDirectoryRecord);
			break;
		case STUDY:
			bCurConsist = CheckStudyRecordConsistent(pDDODirectoryRecord, pDDOStudyDirectoryRecord);
			break;
		case SERIES:
			bCurConsist = CheckSeriesRecordConsistent(pDDODirectoryRecord, pDDOSeriesDirectoryRecord);
			break;
		case IMAGE:

			bCurConsist = CheckImageRecordConsistent(pDDODirectoryRecord, pDDOImageDirectoryRecord);
			break;
		default:
			bCurConsist = FALSE;
		}
	} else
		bCurConsist = FALSE;

	BOOL bLowerConsist;
	BOOL bNextConsist;
	unsigned long * pAddedOffset1;
	unsigned long * pAddedOffset2;

	if (*pOffset2) {
		*pOffset2 = lLastOffsite;
		
		if(!WalkThroughBasicDirectory(ArrayPtr, pDDOBasicDirectory, lLastOffsite, bCurConsist,\
			bLowerConsist, pDDOPatientDirectoryRecord, pDDOStudyDirectoryRecord,\
			pDDOSeriesDirectoryRecord, pDDOImageDirectoryRecord, bNewAdded, lRootLastRecordOffsite, nTransferSyntax,nDeleteFlag))
			return FALSE;
	} else {
		if(bCurConsist && !bNewAdded) {
			switch(eRecordType) {
			case PATIENT:
				*pOffset2 = lLastOffsite;
				 nDeleteFlag=1;

				if(!GetRecordInformation(pDDOStudyDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOStudyDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOStudyDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 

				if(!GetRecordInformation(pDDOSeriesDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOSeriesDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOSeriesDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 
				
				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case STUDY:
				*pOffset2 = lLastOffsite;
				 nDeleteFlag=2;

				if(!GetRecordInformation(pDDOSeriesDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOSeriesDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOSeriesDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 
				
				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case SERIES:
				*pOffset2 = lLastOffsite;
				 nDeleteFlag=3;

				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case IMAGE:
				 nDeleteFlag=4;
				bNewAdded = TRUE;
				break;
			default:
				break;
			}
		}
	}
	if (*pOffset1) {
		*pOffset1 = lLastOffsite;

		if(!WalkThroughBasicDirectory(ArrayPtr, pDDOBasicDirectory, lLastOffsite, bUpperConsist,\
			bNextConsist, pDDOPatientDirectoryRecord, pDDOStudyDirectoryRecord,\
			pDDOSeriesDirectoryRecord, pDDOImageDirectoryRecord, bNewAdded, lRootLastRecordOffsite, nTransferSyntax,nDeleteFlag))
			return FALSE;
	} else {
		if(bUpperConsist && !bNewAdded) {
			switch(eRecordType) {
			case PATIENT: //and equallevel
			case TOPIC:
				*pOffset1 = lLastOffsite;
				nDeleteFlag=0;

				lRootLastRecordOffsite = lLastOffsite;

				if(!GetRecordInformation(pDDOPatientDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOPatientDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOPatientDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2  = lLastOffsite; 

				if(!GetRecordInformation(pDDOStudyDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOStudyDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOStudyDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 

				if(!GetRecordInformation(pDDOSeriesDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOSeriesDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOSeriesDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 
				
				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case STUDY:
				*pOffset1 = lLastOffsite;
				 nDeleteFlag=1;

				if(!GetRecordInformation(pDDOStudyDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOStudyDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOStudyDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 

				if(!GetRecordInformation(pDDOSeriesDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOSeriesDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOSeriesDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 
				
				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case SERIES:
			case VISIT:
			case RESULTS:
			case STUDYCOMPONENT:
				*pOffset1 = lLastOffsite;
				 nDeleteFlag=2;

				if(!GetRecordInformation(pDDOSeriesDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOSeriesDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOSeriesDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = lLastOffsite; 

				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			case IMAGE:
			case OVERLAY:
			case MODALITYLUT:
			case VOILUT:
			case CURVE:
			case STOREDPRINT:
			case RTDOSE:
			case RTSTRUCTURESET:
			case RTPLAN:
			case RTTREATRECORD:

				*pOffset1 = lLastOffsite;
				 nDeleteFlag=3;

				if(!GetRecordInformation(pDDOImageDirectoryRecord, pAddedOffset1, pAddedOffset2, eRecordTypeTemp)) 
					return FALSE;
				pDDOBasicDirectory->Push(pDDOImageDirectoryRecord);
				*pAddedOffset1 = 0;
				lLastOffsite = lLastOffsite + pDDOImageDirectoryRecord->Length + 8 + 8;
				*pAddedOffset2 = 0; 

				bNewAdded = TRUE;
				break;
			default:
				break;
			}
		}

	}
//	if(*pOffset1 == 0 && *pOffset1 == 0)
	return TRUE;

}
//检查study一致性
BOOL DICOMDIR::CheckStudyRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget)
{
	VR * vrSource;
	VR * vrTarget;

	vrSource = pDDOSource->GetVR(0x0004, 0x1430);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data, (void *) "STUDY",vrSource->Length, 5))
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0004, 0x1430);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrTarget->Data, (void *) "STUDY",vrTarget->Length, 5))
		return FALSE;
/*//zxh
//>Study Date	(0008,0020)	1		
	vrSource = pDDOSource->GetVR(0x0008, 0x0020);
	if(!vrSource || vrSource->Length == 0) {
		return FALSE;
	}

	vrTarget = pDDOTarget->GetVR(0x0008, 0x0020);
	if(!vrTarget || vrTarget->Length == 0) {
		return FALSE;
	}

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;

//>Study Time	(0008,0030)	1
	vrSource = pDDOSource->GetVR(0x0008, 0x0030);
	if(!vrSource || vrSource->Length == 0) {
		return FALSE;
	}

	vrTarget = pDDOTarget->GetVR(0x0008, 0x0030);
	if(!vrTarget || vrTarget->Length == 0) {
		return FALSE;
	}

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;

//> Study ID	(0020,0010)	1	Generated at creation of the Study (by the scanner) or entered by user	
	vrSource = pDDOSource->GetVR(0x0020, 0x0010);
	if(!vrSource || vrSource->Length == 0) {
		return FALSE;
	}

	vrTarget = pDDOTarget->GetVR(0x0020, 0x0010);
	if(!vrTarget || vrTarget->Length == 0) {
		return FALSE;
	}

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;
*/
//> Study Instance UID	(0020,000D)	1 Unique identifier for the Study.
//zxh
	vrSource = pDDOSource->GetVR(0x0020, 0x000D);
	if(!vrSource || vrSource->Length == 0) {
		return FALSE;
	}

	vrTarget = pDDOTarget->GetVR(0x0020, 0x000D);
	if(!vrTarget || vrTarget->Length == 0) {
		return FALSE;
	}

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;
	return TRUE;
}
//检查序列一致性
BOOL DICOMDIR::CheckSeriesRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget)
{
	VR * vrSource;
	VR * vrTarget;

	vrSource = pDDOSource->GetVR(0x0004, 0x1430);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data, (void *) "SERIES",vrSource->Length, 6))
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0004, 0x1430);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrTarget->Data, (void *) "SERIES",vrTarget->Length, 6))
		return FALSE;

//>	Series Instance UID	(0020,000E)	1	Generated at creation of the Series
	vrSource = pDDOSource->GetVR(0x0020, 0x000E);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0020, 0x000E);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;
	
	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;

	return TRUE;

}
//检查图象一致性
BOOL DICOMDIR::CheckImageRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget)
{
	VR * vrSource;
	VR * vrTarget;

	vrSource = pDDOSource->GetVR(0x0004, 0x1430);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data, (void *) "IMAGE",vrSource->Length, 5))
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0004, 0x1430);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrTarget->Data, (void *) "IMAGE",vrTarget->Length, 5))
		return FALSE;

/*//zxh
//> Instance Number	(0020,0013)	1
	vrSource = pDDOSource->GetVR(0x0020, 0x0013);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

	vrTarget = pDDOTarget->GetVR(0x0020, 0x0013);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;
*/
//> SOP Instance UID	(0008,0018)	1
//zxh
//	vrSource = pDDOSource->GetVR(0x0008, 0x0018);
//	if(!vrSource || vrSource->Length < 1)
		vrSource = pDDOSource->GetVR(0x0004, 0x1511);
	if(!vrSource || vrSource->Length < 1)
		return FALSE;

//	vrTarget = pDDOTarget->GetVR(0x0008, 0x0018);
//	if(!vrTarget || vrTarget->Length < 1)
		vrTarget = pDDOTarget->GetVR(0x0004, 0x1511);
	if(!vrTarget || vrTarget->Length < 1)
		return FALSE;

	if(TrimMemCmp(vrSource->Data,vrTarget->Data, vrSource->Length, vrTarget->Length))
		return FALSE;

	return TRUE;

}
//保持DICOMDIR
BOOL DICOMDIR::Save(DICOMDataObject *pDDO, char *strDICOMDIRRoot, char *strReferencedFileID, char *strFileSetID, char *strFileSetUID, UINT nTransferSyntax)
{
	if(!Add(pDDO, strDICOMDIRRoot, strReferencedFileID))
		return CreateDICOMDIR(pDDO, strDICOMDIRRoot, strReferencedFileID,\
		strFileSetID, strFileSetUID, nTransferSyntax);
	else
		return TRUE;
}
//检查长度
BOOL DICOMDIR::CheckDICOMObjectLength(DICOMObject * pDDO, UINT nTransferSyntax, unsigned long &lLength)
{
	DICOMObject TempDDO;
	VR * vr;
	UINT nIndex;
	Array < DICOMObject *> 		*ArrayPtr = NULL;
	while(vr = pDDO->Pop()) {
		if(!vr->TypeCode)
			return FALSE;
		else if(vr->TypeCode == 'SQ' || vr->TypeCode == 'OW' ||\
			vr->TypeCode == 'OB' || vr->TypeCode == 'UN') {

			switch(nTransferSyntax) {
			case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
				lLength = lLength + 4;
				break;
			case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
				break;
			case DICOM_MEDIA_EXPLICITBIGENDIAN:
				lLength = lLength + 4;
				break;
			default:
				return FALSE;
			}

			if(vr->SQObjectArray) {
				ArrayPtr = (Array < DICOMObject *> *) vr->SQObjectArray;
				for(nIndex = 0; nIndex < ArrayPtr->GetSize(); nIndex++) {
					if(!CheckDICOMObjectLength(ArrayPtr->Get(nIndex), nTransferSyntax, lLength))
						return FALSE;
					lLength = lLength + 8 + 8;
				}
			}
		}
		TempDDO.Push(vr);
	}

	while(vr = TempDDO.Pop()) {
		pDDO->Push(vr);
	}
	return TRUE;
}
//加载DICOMDIR
DICOMObject * DICOMDIR::LoadDICOMDIR(char * strFileName)
{
	char			strTransferSyntax[MAX_VRUI];
	VR				*vr;
	Array < DICOMObject *> * ArrayPtr;
	UINT nTransferSyntax;
	
	memset( (void *) strTransferSyntax, 0, MAX_VRUI);
	DICOMDataObject * pDDODICOMDIR;
	pDDODICOMDIR = PDU_Service::LoadDICOMDataObject(strFileName);
	if(pDDODICOMDIR) {
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0002);
		if(!vr || vr->Length < 20) {
			delete pDDODICOMDIR;
			return NULL;
		}
		if(TrimMemCmp( vr->Data, (void *) "1.2.840.10008.1.3.10", vr->Length, 20)) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0003);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0001);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0002, 0x0010);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		} else {
			memcpy((void *)strTransferSyntax,vr->Data, vr->Length);
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1130);
		if(!vr) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1200);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1202);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1212);
		if(!vr || vr->Length < 1) {
			delete pDDODICOMDIR;
			return NULL;
		}
		vr = pDDODICOMDIR->GetVR(0x0004, 0x1220);
		if(!vr) {
			delete pDDODICOMDIR;
			return NULL;
		}
	} else
		return NULL;

	if(strcmp(strTransferSyntax, _ImplicitLittleEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_IMPLICITLITTLEENDIAN;
	else if(strcmp(strTransferSyntax, _ExplicitLittleEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_EXPLICITLITTLEENDIAN;
	else if(strcmp(strTransferSyntax, _ExplicitBigEndianUID) == 0)
		nTransferSyntax = DICOM_MEDIA_EXPLICITBIGENDIAN;
	else {
		delete pDDODICOMDIR;
		return NULL;
	}

	VR * vrDirectoryRecordSequence;
	unsigned long lOffsetofFirstRecordofRootDirectoryEntity;
	unsigned long lOffsetofLastRecordofRootDirectoryEntity;

	while(vr = pDDODICOMDIR->Pop())
	{
		if(vr->Group==0x0002)
			delete vr;
		else {
			
			if(vr->Group==0x0004 && vr->Element==0x1200) 
				lOffsetofFirstRecordofRootDirectoryEntity = *((unsigned long *) vr->Data);

			if(vr->Group==0x0004 && vr->Element==0x1202)
				lOffsetofLastRecordofRootDirectoryEntity = *((unsigned long *) vr->Data);

			if(vr->Group==0x0004 && vr->Element==0x1220) {
				vrDirectoryRecordSequence = vr;
				continue;
			}
			delete vr;
		}
	}
	delete	pDDODICOMDIR;


	if(vrDirectoryRecordSequence->SQObjectArray == NULL) {
//zxh此条件？？		if(vrDirectoryRecordSequence->Length != 0) 
		{
			delete vrDirectoryRecordSequence;
			return NULL;
		}
	} else {
		ArrayPtr = (Array<DICOMObject *> *)	vrDirectoryRecordSequence->SQObjectArray;
		vrDirectoryRecordSequence->SQObjectArray = NULL;
		if(vrDirectoryRecordSequence->Length > 0 && vrDirectoryRecordSequence->Data) {
			delete vrDirectoryRecordSequence->Data;
			vrDirectoryRecordSequence->Data = NULL;
		}
		vrDirectoryRecordSequence->Length =0;
		vrDirectoryRecordSequence->ReleaseMemory = FALSE;
		delete vrDirectoryRecordSequence;
	}
	
	DICOMObject * pDO = new DICOMObject();
	
	char * strTOP = new char[4];
	memset(strTOP, 0, 4);
	strcpy(strTOP, "TOP");
	vr = new VR(0x0004, 0x1430, strlen(strTOP), strTOP);
	pDO->Push(vr);
	
	vr = new VR(0xffff, 0xffff, 0, FALSE);
	pDO->Push(vr);

	if(!BuildDirTree( pDO, ArrayPtr)){
		delete pDO;
		while(ArrayPtr->GetSize()) {
			delete ArrayPtr->Get(0);
			ArrayPtr->RemoveAt(0);
		}
		delete ArrayPtr;
		return NULL;
	}

	while(ArrayPtr->GetSize()) {
		delete ArrayPtr->Get(0);
		ArrayPtr->RemoveAt(0);
	}
	delete ArrayPtr;
	
	return pDO;
}
//建立整个dir树
BOOL DICOMDIR::BuildDirTree( DICOMObject * pDDODirTree, Array <DICOMObject *> * ArrayPtr)
{
	
	if(ArrayPtr->GetSize() < 1) 
		return TRUE;
	
	VR * vr;

	DICOMObject * pDDODirectoryRecord = ArrayPtr->Get(0);

	unsigned long * pOffset1;
	unsigned long * pOffset2;
	unsigned long lOffset1;
	unsigned long lOffset2;

	RecordType eRecordType;

	if(!GetRecordInformation(pDDODirectoryRecord, pOffset1, pOffset2, eRecordType)) {
		while(ArrayPtr->GetSize()) {
			delete ArrayPtr->Get(0);
			ArrayPtr->RemoveAt(0);
		}
		return FALSE;
	}
	
	lOffset1 = *pOffset1;
	lOffset2 = *pOffset2;

	vr = pDDODirTree->GetVR(0x0004, 0x1430);
	if(!vr || vr->Length < 1) {
		while(ArrayPtr->GetSize()) {
			delete ArrayPtr->Get(0);
			ArrayPtr->RemoveAt(0);
		}
		return FALSE;
	}
	if(TrimMemCmp( vr->Data,  (void *) "TOP", vr->Length, 3) == 0) { 
		switch(eRecordType) {
		case PATIENT:
		case TOPIC:
			break;
		default:
			while(ArrayPtr->GetSize()) {
				delete ArrayPtr->Get(0);
				ArrayPtr->RemoveAt(0);
			}
			return FALSE;
		}
	} else if((TrimMemCmp( vr->Data,  (void *) "PATIENT", vr->Length, 7) == 0) ||
		      (TrimMemCmp( vr->Data,  (void *) "TOPIC", vr->Length, 5) == 0)) {
		switch(eRecordType) {
		case STUDY:
			break;
		default:
			while(ArrayPtr->GetSize()) {
				delete ArrayPtr->Get(0);
				ArrayPtr->RemoveAt(0);
			}
			return FALSE;
		}

	} else if( TrimMemCmp( vr->Data,  (void *) "STUDY", vr->Length, 5) == 0 ) {
		switch(eRecordType) {
		case SERIES:
		case VISIT:
		case RESULTS:
		case STUDYCOMPONENT:
			break;
		default:
			while(ArrayPtr->GetSize()) {
				delete ArrayPtr->Get(0);
				ArrayPtr->RemoveAt(0);
			}
			return FALSE;
		}

	} else if((TrimMemCmp( vr->Data,  (void *) "SERIES", vr->Length, 6) == 0) ||
		      (TrimMemCmp( vr->Data,  (void *) "VISIT", vr->Length, 5) == 0) ||
			  (TrimMemCmp( vr->Data,  (void *) "RESULTS", vr->Length, 7) == 0) ||
			  (TrimMemCmp( vr->Data,  (void *) "STUDY COMPONENT", vr->Length, 15) == 0)) {
		switch(eRecordType) {
		case IMAGE:
		case OVERLAY:
		case MODALITYLUT:
		case VOILUT:
		case CURVE:
		case STOREDPRINT:
		case RTDOSE:
		case RTSTRUCTURESET:
		case RTPLAN:
		case RTTREATRECORD:
			break;
		default:
			while(ArrayPtr->GetSize()) {
				delete ArrayPtr->Get(0);
				ArrayPtr->RemoveAt(0);
			}
			return FALSE;
		}

	} else {
			while(ArrayPtr->GetSize()) {
				delete ArrayPtr->Get(0);
				ArrayPtr->RemoveAt(0);
			}
			return FALSE;
	
	}

	DICOMObject * pDDO = new DICOMObject;
	while(vr = pDDODirectoryRecord->Pop()) {
		if(vr->Group == 0x0004 && vr->Element != 0x1430 && vr->Element != 0x1500 && vr->Element != 0x1511 && vr->Element != 0x1510)//zxh add 1511 and 1510
			delete vr;
		else
			pDDO->Push(vr);
	}

	//Data Element (ffff, ffff) is used to connect leaves of DIR tree
	vr = new VR(0xffff, 0xffff, 0, FALSE);			
	pDDO->Push(vr);

	delete pDDODirectoryRecord;
	ArrayPtr->RemoveAt(0);

	pDDODirTree->Push(pDDO);

	if (lOffset2) {
		if(!BuildDirTree( pDDO, ArrayPtr))
			return FALSE;
	} 
	
	if (lOffset1) {
		if(!BuildDirTree( pDDODirTree, ArrayPtr))
			return FALSE;
	} 

	return TRUE;
}
