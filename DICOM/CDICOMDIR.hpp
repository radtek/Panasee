// DICOMDIR.h: interface for the DICOMDIR class.
//
//////////////////////////////////////////////////////////////////////
//本文件定义DICOMDIR类,
//该类包括创建,添加,加载,保存DICOMDIR
//每个DICOMDIR实际上分为4层:病人层,检查层,序列层,图象层
//这些函数其实就是填充这些层的数据
//以及遍历这些层的数据
//函数具体说明见实现文件
//
#if !defined(AFX_DICOMDIR_H__C6389813_0242_11D4_A5D4_0000E86E1969__INCLUDED_)
#define AFX_DICOMDIR_H__C6389813_0242_11D4_A5D4_0000E86E1969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "pdu.hpp"
#include "cctypes.h"	// Added by ClassView

enum RecordType {PATIENT, STUDY, SERIES, IMAGE, OVERLAY, MODALITYLUT, VOILUT, CURVE, STOREDPRINT, RTDOSE, RTSTRUCTURESET, RTPLAN, RTTREATRECORD, TOPIC, VISIT, RESULTS, INTERPRETATION, STUDYCOMPONENT, PRIVATE, MRDR};

class DICOMDIR : public PDU_Service  
{
public:
	BOOL Save(DICOMDataObject *pDDO, char *strDICOMDIRRoot, char *strReferencedFileID, char *strFileSetID, char *strFileSetUID, UINT nTransferSyntax);
//zxh	BOOL Add(DICOMDataObject *pDDO, char *strDICOMDIRRoot, char *strReferencedFileID);
	BOOL CreateDICOMDIR(DICOMDataObject *pDDO, char *strDICOMDIRRoot,
		char *strReferencedFileID, char *strFileSetID, 
		char *strFileSetUID, UINT nTransferSyntax);
	DICOMObject * LoadDICOMDIR(char * strFileName);
	BOOL GetUID ( UID	&uid );
	DICOMDIR();
	virtual ~DICOMDIR();

protected:
	BOOL Add(DICOMDataObject *pDDO, char *strDICOMDIRRoot, char *strReferencedFileID);
	BOOL BuildDirTree( DICOMObject * pDDODirTree, Array <DICOMObject *> * SourceArrayPtr);
	BOOL CheckDICOMObjectLength(DICOMObject * pDDO, UINT nTransferSyntax, unsigned long &lLength);
	BOOL CheckImageRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget);
	BOOL CheckSeriesRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget);
	BOOL CheckStudyRecordConsistent(DICOMObject *pDDOSource, DICOMObject *pDDOTarget);
	BOOL WalkThroughBasicDirectory(Array < DICOMObject *> *ArrayPtr, DICOMObject * pDDOBasicDirectory, unsigned long & lLastOffsite, BOOL bUpperConsist, BOOL & bLowerConsist, DICOMObject * pDDOPatientDirectoryRecord, DICOMObject * pDDOStudyDirectoryRecord, DICOMObject * pDDOSeriesDirectoryRecord, DICOMObject * pDDOImageDirectoryRecord, BOOL &bNewAdded, unsigned long & lRootLastRecordOffsite, UINT nTransferSyntax,int &nDeleteFlag);
	BOOL CheckPatientRecordConsistent(DICOMObject * pDDOSource, DICOMObject * pDDOTarget);
	BOOL GetRecordInformation(DICOMObject * pDDODirectoryRecord, unsigned long * & pOffset1, unsigned long * & pOffset2, RecordType &eRecordType);
	BOOL DeleteNodeAccordFileID(Array < DICOMObject *> *ArrayPtr,char*pFileID,char* pDICOMDIR);//zxh
	BOOL CollectImageDirectoryRecord (DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID);
	BOOL CollectSeriesDirectoryRecord (DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID);
	BOOL CollectStudyDirectoryRecord (DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID);
	BOOL CollectPatientDirectoryRecord (DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID);
	BOOL CollectDirectorRecord (DICOMDataObject *pTargetDDO, DICOMDataObject *pSourceDDO, unsigned long *pOffsetofNextDirectoryRecord, unsigned long *pOffsetofReferenceLowerLevelDirectoryEntity, char *pReferencedFileID, char *pDirectoryRecordType);
};

#endif // !defined(AFX_DICOMDIR_H__C6389813_0242_11D4_A5D4_0000E86E1969__INCLUDED_)
