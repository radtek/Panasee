#	include	"iod.hpp"
#	include	"iodbase.hpp"

//构造 重置 设置 拷贝 for: FileSetIdentificationModule
//构造
CO_FileSetIdentificationModule :: CO_FileSetIdentificationModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&FileSetID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&FileSetDescriptorFileID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSetOfFileSetDescriptorFile);
		}
	}

//重置
BOOL
CO_FileSetIdentificationModule ::  Reset()
	{
	FileSetID.Reset();
	FileSetID.Group = 0x0004;
	FileSetID.Element = 0x1130;
	FileSetDescriptorFileID.Reset();
	FileSetDescriptorFileID.Group = 0x0004;
	FileSetDescriptorFileID.Element = 0x1141;
	SpecificCharacterSetOfFileSetDescriptorFile.Reset();
	SpecificCharacterSetOfFileSetDescriptorFile.Group = 0x0004;
	SpecificCharacterSetOfFileSetDescriptorFile.Element = 0x1142;

	return ( TRUE );
	}

//设置
BOOL
CO_FileSetIdentificationModule :: SetIf	( VR *vr )
	{
	if ( FileSetID.SetIf ( vr ) )
		return ( TRUE );
	if ( FileSetDescriptorFileID.SetIf ( vr ) )
		return ( TRUE );
	if ( SpecificCharacterSetOfFileSetDescriptorFile.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_FileSetIdentificationModule :: Morph	( DICOMObject *DO )
	{
	FileSetID.Morph ( DO );
	FileSetDescriptorFileID.Morph ( DO );
	SpecificCharacterSetOfFileSetDescriptorFile.Morph ( DO );


	return ( FALSE );
	}

//构造 重置 设置 拷贝 for: DirectoryInformationModule
//构造
CO_DirectoryInformationModule :: CO_DirectoryInformationModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&FileSetConsistencyFlag);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DirectoryRecordSequence);
		}
	}

//重置
BOOL
CO_DirectoryInformationModule ::  Reset()
	{
	OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity.Reset();
	OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity.Group = 0x0004;
	OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity.Element = 0x1200;
	OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity.Reset();
	OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity.Group = 0x0004;
	OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity.Element = 0x1202;
	FileSetConsistencyFlag.Reset();
	FileSetConsistencyFlag.Group = 0x0004;
	FileSetConsistencyFlag.Element = 0x1212;
	DirectoryRecordSequence.Reset();
	DirectoryRecordSequence.Group = 0x0004;
	DirectoryRecordSequence.Element = 0x1220;

	return ( TRUE );
	}

//设置
BOOL
CO_DirectoryInformationModule :: SetIf	( VR *vr )
	{
	if ( OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity.SetIf ( vr ) )
		return ( TRUE );
	if ( OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity.SetIf ( vr ) )
		return ( TRUE );
	if ( FileSetConsistencyFlag.SetIf ( vr ) )
		return ( TRUE );
	if ( DirectoryRecordSequence.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_DirectoryInformationModule :: Morph	( DICOMObject *DO )
	{
	OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity.Morph ( DO );
	OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity.Morph ( DO );
	FileSetConsistencyFlag.Morph ( DO );
	DirectoryRecordSequence.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: DirectoryRecordSequenceModule
//构造
CO_DirectoryRecordSequenceModule :: CO_DirectoryRecordSequenceModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OffsetOfTheNextDirectoryRecord);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&RecordInUseFlag);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OffsetOfReferencedLowerLevelDirectoryEntity);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DirectoryRecordType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PrivateRecordUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedFileID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&MRDRDirectoryRecordOffset);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedSOPClassUIDInFile);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedSOPInstanceUIDInFile);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedTransferSyntaxUIDInFile);
		}
	}

//重置
BOOL
CO_DirectoryRecordSequenceModule ::  Reset()
	{
	OffsetOfTheNextDirectoryRecord.Reset();
	OffsetOfTheNextDirectoryRecord.Group = 0x0004;
	OffsetOfTheNextDirectoryRecord.Element = 0x1400;
	RecordInUseFlag.Reset();
	RecordInUseFlag.Group = 0x0004;
	RecordInUseFlag.Element = 0x1410;
	OffsetOfReferencedLowerLevelDirectoryEntity.Reset();
	OffsetOfReferencedLowerLevelDirectoryEntity.Group = 0x0004;
	OffsetOfReferencedLowerLevelDirectoryEntity.Element = 0x1420;
	DirectoryRecordType.Reset();
	DirectoryRecordType.Group = 0x0004;
	DirectoryRecordType.Element = 0x1430;
	PrivateRecordUID.Reset();
	PrivateRecordUID.Group = 0x0004;
	PrivateRecordUID.Element = 0x1432;
	ReferencedFileID.Reset();
	ReferencedFileID.Group = 0x0004;
	ReferencedFileID.Element = 0x1500;
	MRDRDirectoryRecordOffset.Reset();
	MRDRDirectoryRecordOffset.Group = 0x0004;
	MRDRDirectoryRecordOffset.Element = 0x1504;
	ReferencedSOPClassUIDInFile.Reset();
	ReferencedSOPClassUIDInFile.Group = 0x0004;
	ReferencedSOPClassUIDInFile.Element = 0x1510;
	ReferencedSOPInstanceUIDInFile.Reset();
	ReferencedSOPInstanceUIDInFile.Group = 0x0004;
	ReferencedSOPInstanceUIDInFile.Element = 0x1511;
	ReferencedTransferSyntaxUIDInFile.Reset();
	ReferencedTransferSyntaxUIDInFile.Group = 0x0004;
	ReferencedTransferSyntaxUIDInFile.Element = 0x1512;

	return ( TRUE );
	}

//设置
BOOL
CO_DirectoryRecordSequenceModule :: SetIf	( VR *vr )
	{
	if ( OffsetOfTheNextDirectoryRecord.SetIf ( vr ) )
		return ( TRUE );
	if ( RecordInUseFlag.SetIf ( vr ) )
		return ( TRUE );
	if ( OffsetOfReferencedLowerLevelDirectoryEntity.SetIf ( vr ) )
		return ( TRUE );
	if ( DirectoryRecordType.SetIf ( vr ) )
		return ( TRUE );
	if ( PrivateRecordUID.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedFileID.SetIf ( vr ) )
		return ( TRUE );
	if ( MRDRDirectoryRecordOffset.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedSOPClassUIDInFile.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedSOPInstanceUIDInFile.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedTransferSyntaxUIDInFile.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_DirectoryRecordSequenceModule :: Morph	( DICOMObject *DO )
	{
	OffsetOfTheNextDirectoryRecord.Morph ( DO );
	RecordInUseFlag.Morph ( DO );
	OffsetOfReferencedLowerLevelDirectoryEntity.Morph ( DO );
	DirectoryRecordType.Morph ( DO );
	PrivateRecordUID.Morph ( DO );
	ReferencedFileID.Morph ( DO );
	MRDRDirectoryRecordOffset.Morph ( DO );
	ReferencedSOPClassUIDInFile.Morph ( DO );
	ReferencedSOPInstanceUIDInFile.Morph ( DO );
	ReferencedTransferSyntaxUIDInFile.Morph ( DO );


	return ( FALSE );
	}

//构造 重置 设置 拷贝 for: PatientDirectoryRecordModule
//构造
CO_PatientDirectoryRecordModule :: CO_PatientDirectoryRecordModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSet);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientBirthDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientSex);
		}
	}

//重置
BOOL
CO_PatientDirectoryRecordModule ::  Reset()
	{
	SpecificCharacterSet.Reset();
	SpecificCharacterSet.Group = 0x0008;
	SpecificCharacterSet.Element = 0x0005;
	PatientsName.Reset();
	PatientsName.Group = 0x0010;
	PatientsName.Element = 0x0010;
	PatientID.Reset();
	PatientID.Group = 0x0010;
	PatientID.Element = 0x0020;
	PatientBirthDate.Reset();
	PatientBirthDate.Group = 0x0010;
	PatientBirthDate.Element = 0x0030;
	PatientSex.Reset();
	PatientSex.Group = 0x0010;
	PatientSex.Element = 0x0040;

	return ( TRUE );
	}

//设置
BOOL
CO_PatientDirectoryRecordModule :: SetIf	( VR *vr )
	{
	if ( SpecificCharacterSet.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsName.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientID.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientBirthDate.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientSex.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_PatientDirectoryRecordModule :: Morph	( DICOMObject *DO )
	{
	SpecificCharacterSet.Morph ( DO );
	PatientsName.Morph ( DO );
	PatientID.Morph ( DO );
	PatientBirthDate.Morph ( DO );
	PatientSex.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: StudyDirectoryRecordModule
//构造
CO_StudyDirectoryRecordModule :: CO_StudyDirectoryRecordModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSet);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyInstanceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AccessionNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AdmittingDiagnosisDescription);
		}
	}

//重置
BOOL
CO_StudyDirectoryRecordModule ::  Reset()
	{
	SpecificCharacterSet.Reset();
	SpecificCharacterSet.Group = 0x0008;
	SpecificCharacterSet.Element = 0x0005;
	StudyDate.Reset();
	StudyDate.Group = 0x0008;
	StudyDate.Element = 0x0020;
	StudyTime.Reset();
	StudyTime.Group = 0x0008;
	StudyTime.Element = 0x0030;
	StudyDescription.Reset();
	StudyDescription.Group = 0x0008;
	StudyDescription.Element = 0x1030;
	StudyInstanceUID.Reset();
	StudyInstanceUID.Group = 0x0020;
	StudyInstanceUID.Element = 0x000D;
	StudyID.Reset();
	StudyID.Group = 0x0020;
	StudyID.Element = 0x0010;
	AccessionNumber.Reset();
	AccessionNumber.Group = 0x0008;
	AccessionNumber.Element = 0x0050;
	AdmittingDiagnosisDescription.Reset();
	AdmittingDiagnosisDescription.Group = 0x0008;
	AdmittingDiagnosisDescription.Element = 0x1080;

	return ( TRUE );
	}

//设置
BOOL
CO_StudyDirectoryRecordModule :: SetIf	( VR *vr )
	{
	if ( SpecificCharacterSet.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyDate.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyTime.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyInstanceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyID.SetIf ( vr ) )
		return ( TRUE );
	if ( AccessionNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( AdmittingDiagnosisDescription.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_StudyDirectoryRecordModule :: Morph	( DICOMObject *DO )
	{
	SpecificCharacterSet.Morph ( DO );
	StudyDate.Morph ( DO );
	StudyTime.Morph ( DO );
	StudyDescription.Morph ( DO );
	StudyInstanceUID.Morph ( DO );
	StudyID.Morph ( DO );
	AccessionNumber.Morph ( DO );
	AdmittingDiagnosisDescription.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: SeriesDirectoryRecordModule
//构造
CO_SeriesDirectoryRecordModule :: CO_SeriesDirectoryRecordModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSet);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Modality);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesInstanceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformingPhysicianName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BodyPartExamined);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Manufacturer);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StationName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ManufacturerModelName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&IconImageSequence);
		}
	}

//重置
BOOL
CO_SeriesDirectoryRecordModule ::  Reset()
	{
	SpecificCharacterSet.Reset();
	SpecificCharacterSet.Group = 0x0008;
	SpecificCharacterSet.Element = 0x0005;
	Modality.Reset();
	Modality.Group = 0x0008;
	Modality.Element = 0x0060;
	SeriesInstanceUID.Reset();
	SeriesInstanceUID.Group = 0x0020;
	SeriesInstanceUID.Element = 0x000E;
	SeriesNumber.Reset();
	SeriesNumber.Group = 0x0020;
	SeriesNumber.Element = 0x0011;
	PerformingPhysicianName.Reset();
	PerformingPhysicianName.Group = 0x0008;
	PerformingPhysicianName.Element = 0x1050;
	SeriesDescription.Reset();
	SeriesDescription.Group = 0x0008;
	SeriesDescription.Element = 0x103E;
	BodyPartExamined.Reset();
	BodyPartExamined.Group = 0x0018;
	BodyPartExamined.Element = 0x0015;
	Manufacturer.Reset();
	Manufacturer.Group = 0x0008;
	Manufacturer.Element = 0x0070;
	StationName.Reset();
	StationName.Group = 0x0008;
	StationName.Element = 0x1010;
	ManufacturerModelName.Reset();
	ManufacturerModelName.Group = 0x0008;
	ManufacturerModelName.Element = 0x1090;
	IconImageSequence.Reset();
	IconImageSequence.Group = 0x0088;
	IconImageSequence.Element = 0x0200;

	return ( TRUE );
	}

//设置
BOOL
CO_SeriesDirectoryRecordModule :: SetIf	( VR *vr )
	{
	if ( SpecificCharacterSet.SetIf ( vr ) )
		return ( TRUE );
	if ( Modality.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesInstanceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformingPhysicianName.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( BodyPartExamined.SetIf ( vr ) )
		return ( TRUE );
	if ( Manufacturer.SetIf ( vr ) )
		return ( TRUE );
	if ( StationName.SetIf ( vr ) )
		return ( TRUE );
	if ( ManufacturerModelName.SetIf ( vr ) )
		return ( TRUE );
	if ( IconImageSequence.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_SeriesDirectoryRecordModule :: Morph	( DICOMObject *DO )
	{
	SpecificCharacterSet.Morph ( DO );
	Modality.Morph ( DO );
	SeriesInstanceUID.Morph ( DO );
	SeriesNumber.Morph ( DO );
	PerformingPhysicianName.Morph ( DO );
	SeriesDescription.Morph ( DO );
	BodyPartExamined.Morph ( DO );
	Manufacturer.Morph ( DO );
	StationName.Morph ( DO );
	ManufacturerModelName.Morph ( DO );
	IconImageSequence.Morph ( DO );


	return ( FALSE );
	}

//构造 重置 设置 拷贝 for: ImageDirectoryRecordModule
//构造
CO_ImageDirectoryRecordModule :: CO_ImageDirectoryRecordModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSet);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageComments);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PhotometricInterpretation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NumberOfFrames);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TransducerData);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TransducerType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstanceNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&IconImageSequence);
		}
	}

//重置
BOOL
CO_ImageDirectoryRecordModule ::  Reset()
	{
	SpecificCharacterSet.Reset();
	SpecificCharacterSet.Group = 0x0008;
	SpecificCharacterSet.Element = 0x0005;
	ImageDate.Reset();
	ImageDate.Group = 0x0008;
	ImageDate.Element = 0x0023;
	ImageTime.Reset();
	ImageTime.Group = 0x0008;
	ImageTime.Element = 0x0033;
	ImageType.Reset();
	ImageType.Group = 0x0008;
	ImageType.Element = 0x0008;
	ImageComments.Reset();
	ImageComments.Group = 0x0020;
	ImageComments.Element = 0x4000;
	PhotometricInterpretation.Reset();
	PhotometricInterpretation.Group = 0x0028;
	PhotometricInterpretation.Element = 0x0004;
	NumberOfFrames.Reset();
	NumberOfFrames.Group = 0x0028;
	NumberOfFrames.Element = 0x0008;
	TransducerData.Reset();
	TransducerData.Group = 0x0018;
	TransducerData.Element = 0x5010;
	TransducerType.Reset();
	TransducerType.Group = 0x0018;
	TransducerType.Element = 0x6031;
	InstanceNumber.Reset();
	InstanceNumber.Group = 0x0020;
	InstanceNumber.Element = 0x0013;
	IconImageSequence.Reset();
	IconImageSequence.Group = 0x0088;
	IconImageSequence.Element = 0x0200;

	return ( TRUE );
	}

//设置
BOOL
CO_ImageDirectoryRecordModule :: SetIf	( VR *vr )
	{
	if ( SpecificCharacterSet.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageDate.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageType.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageComments.SetIf ( vr ) )
		return ( TRUE );
	if ( PhotometricInterpretation.SetIf ( vr ) )
		return ( TRUE );
	if ( NumberOfFrames.SetIf ( vr ) )
		return ( TRUE );
	if ( TransducerData.SetIf ( vr ) )
		return ( TRUE );
	if ( TransducerType.SetIf ( vr ) )
		return ( TRUE );
	if ( InstanceNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( IconImageSequence.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}

//拷贝
BOOL
CO_ImageDirectoryRecordModule :: Morph	( DICOMObject *DO )
	{
	SpecificCharacterSet.Morph ( DO );
	ImageDate.Morph ( DO );
	ImageTime.Morph ( DO );
	ImageType.Morph ( DO );
	ImageComments.Morph ( DO );
	PhotometricInterpretation.Morph ( DO );
	NumberOfFrames.Morph ( DO );
	TransducerData.Morph ( DO );
	TransducerType.Morph ( DO );
	InstanceNumber.Morph ( DO );
	IconImageSequence.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: PatientModule

CO_PatientModule :: CO_PatientModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsBirthDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsSex);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedPatientSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsBirthTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OtherPatientIDs);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OtherPatientNames);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&EthnicGroup);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientComments);
		}
	}


/*重置*/ BOOL
CO_PatientModule ::  Reset()
	{
	PatientsName.Reset();
	PatientsName.Group = 0x0010;
	PatientsName.Element = 0x0010;
	PatientID.Reset();
	PatientID.Group = 0x0010;
	PatientID.Element = 0x0020;
	PatientsBirthDate.Reset();
	PatientsBirthDate.Group = 0x0010;
	PatientsBirthDate.Element = 0x0030;
	PatientsSex.Reset();
	PatientsSex.Group = 0x0010;
	PatientsSex.Element = 0x0040;
	ReferencedPatientSequence.Reset();
	ReferencedPatientSequence.Group = 0x0008;
	ReferencedPatientSequence.Element = 0x1120;
	PatientsBirthTime.Reset();
	PatientsBirthTime.Group = 0x0010;
	PatientsBirthTime.Element = 0x0032;
	OtherPatientIDs.Reset();
	OtherPatientIDs.Group = 0x0010;
	OtherPatientIDs.Element = 0x1000;
	OtherPatientNames.Reset();
	OtherPatientNames.Group = 0x0010;
	OtherPatientNames.Element = 0x1001;
	EthnicGroup.Reset();
	EthnicGroup.Group = 0x0010;
	EthnicGroup.Element = 0x2160;
	PatientComments.Reset();
	PatientComments.Group = 0x0010;
	PatientComments.Element = 0x4000;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_PatientModule :: SetIf	( VR *vr )
	{
	if ( PatientsName.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientID.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsBirthDate.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsSex.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedPatientSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsBirthTime.SetIf ( vr ) )
		return ( TRUE );
	if ( OtherPatientIDs.SetIf ( vr ) )
		return ( TRUE );
	if ( OtherPatientNames.SetIf ( vr ) )
		return ( TRUE );
	if ( EthnicGroup.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientComments.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_PatientModule :: Morph	( DICOMObject *DO )
	{
	PatientsName.Morph ( DO );
	PatientID.Morph ( DO );
	PatientsBirthDate.Morph ( DO );
	PatientsSex.Morph ( DO );
	ReferencedPatientSequence.Morph ( DO );
	PatientsBirthTime.Morph ( DO );
	OtherPatientIDs.Morph ( DO );
	OtherPatientNames.Morph ( DO );
	EthnicGroup.Morph ( DO );
	PatientComments.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: GeneralStudyModule

CO_GeneralStudyModule :: CO_GeneralStudyModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyInstanceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferringPhysiciansName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AccessionNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StudyDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PhysiciansOfRecord);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NameOfPhysiciansReadingStudy);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedStudySequence);
		}
	}


/*重置*/ BOOL
CO_GeneralStudyModule ::  Reset()
	{
	StudyInstanceUID.Reset();
	StudyInstanceUID.Group = 0x0020;
	StudyInstanceUID.Element = 0x000D;
	StudyDate.Reset();
	StudyDate.Group = 0x0008;
	StudyDate.Element = 0x0020;
	StudyTime.Reset();
	StudyTime.Group = 0x0008;
	StudyTime.Element = 0x0030;
	ReferringPhysiciansName.Reset();
	ReferringPhysiciansName.Group = 0x0008;
	ReferringPhysiciansName.Element = 0x0090;
	StudyID.Reset();
	StudyID.Group = 0x0020;
	StudyID.Element = 0x0010;
	AccessionNumber.Reset();
	AccessionNumber.Group = 0x0008;
	AccessionNumber.Element = 0x0050;
	StudyDescription.Reset();
	StudyDescription.Group = 0x0008;
	StudyDescription.Element = 0x1030;
	PhysiciansOfRecord.Reset();
	PhysiciansOfRecord.Group = 0x0008;
	PhysiciansOfRecord.Element = 0x1048;
	NameOfPhysiciansReadingStudy.Reset();
	NameOfPhysiciansReadingStudy.Group = 0x0008;
	NameOfPhysiciansReadingStudy.Element = 0x1060;
	ReferencedStudySequence.Reset();
	ReferencedStudySequence.Group = 0x0008;
	ReferencedStudySequence.Element = 0x1110;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_GeneralStudyModule :: SetIf	( VR *vr )
	{
	if ( StudyInstanceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyDate.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferringPhysiciansName.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyID.SetIf ( vr ) )
		return ( TRUE );
	if ( AccessionNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( StudyDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( PhysiciansOfRecord.SetIf ( vr ) )
		return ( TRUE );
	if ( NameOfPhysiciansReadingStudy.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedStudySequence.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_GeneralStudyModule :: Morph	( DICOMObject *DO )
	{
	StudyInstanceUID.Morph ( DO );
	StudyDate.Morph ( DO );
	StudyTime.Morph ( DO );
	ReferringPhysiciansName.Morph ( DO );
	StudyID.Morph ( DO );
	AccessionNumber.Morph ( DO );
	StudyDescription.Morph ( DO );
	PhysiciansOfRecord.Morph ( DO );
	NameOfPhysiciansReadingStudy.Morph ( DO );
	ReferencedStudySequence.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: PatientStudyModule

CO_PatientStudyModule :: CO_PatientStudyModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AdmittingDiagnosesDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsAge);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsSize);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientsWeight);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Occupation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AdditionalPatientsHistory);
		}
	}


/*重置*/ BOOL
CO_PatientStudyModule ::  Reset()
	{
	AdmittingDiagnosesDescription.Reset();
	AdmittingDiagnosesDescription.Group = 0x0008;
	AdmittingDiagnosesDescription.Element = 0x1080;
	PatientsAge.Reset();
	PatientsAge.Group = 0x0010;
	PatientsAge.Element = 0x1010;
	PatientsSize.Reset();
	PatientsSize.Group = 0x0010;
	PatientsSize.Element = 0x1020;
	PatientsWeight.Reset();
	PatientsWeight.Group = 0x0010;
	PatientsWeight.Element = 0x1030;
	Occupation.Reset();
	Occupation.Group = 0x0010;
	Occupation.Element = 0x2180;
	AdditionalPatientsHistory.Reset();
	AdditionalPatientsHistory.Group = 0x0010;
	AdditionalPatientsHistory.Element = 0x21B0;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_PatientStudyModule :: SetIf	( VR *vr )
	{
	if ( AdmittingDiagnosesDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsAge.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsSize.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientsWeight.SetIf ( vr ) )
		return ( TRUE );
	if ( Occupation.SetIf ( vr ) )
		return ( TRUE );
	if ( AdditionalPatientsHistory.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_PatientStudyModule :: Morph	( DICOMObject *DO )
	{
	AdmittingDiagnosesDescription.Morph ( DO );
	PatientsAge.Morph ( DO );
	PatientsSize.Morph ( DO );
	PatientsWeight.Morph ( DO );
	Occupation.Morph ( DO );
	AdditionalPatientsHistory.Morph ( DO );


	return ( FALSE );
	}

//构造 重置 设置 拷贝 for: GeneralSeriesModule

CO_GeneralSeriesModule :: CO_GeneralSeriesModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Modality);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesInstanceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Laterality);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformingPhysiciansName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ProtocolName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SeriesDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OperatorsName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedStudyComponentSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BodyPartExamined);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientPosition);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SmallestPixelValueInSeries);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&LargestPixelValueInSeries);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&RequestAttributesSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformedProcedureStepID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformedProcedureStepStartDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformedProcedureStepStartTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PerformedProcedureStepDescription);
		}
	}


/*重置*/ BOOL
CO_GeneralSeriesModule ::  Reset()
	{
	Modality.Reset();
	Modality.Group = 0x0008;
	Modality.Element = 0x0060;
	SeriesInstanceUID.Reset();
	SeriesInstanceUID.Group = 0x0020;
	SeriesInstanceUID.Element = 0x000E;
	SeriesNumber.Reset();
	SeriesNumber.Group = 0x0020;
	SeriesNumber.Element = 0x0011;
	Laterality.Reset();
	Laterality.Group = 0x0020;
	Laterality.Element = 0x0060;
	SeriesDate.Reset();
	SeriesDate.Group = 0x0008;
	SeriesDate.Element = 0x0021;
	SeriesTime.Reset();
	SeriesTime.Group = 0x0008;
	SeriesTime.Element = 0x0031;
	PerformingPhysiciansName.Reset();
	PerformingPhysiciansName.Group = 0x0008;
	PerformingPhysiciansName.Element = 0x1050;
	ProtocolName.Reset();
	ProtocolName.Group = 0x0018;
	ProtocolName.Element = 0x1030;
	SeriesDescription.Reset();
	SeriesDescription.Group = 0x0008;
	SeriesDescription.Element = 0x103E;
	OperatorsName.Reset();
	OperatorsName.Group = 0x0008;
	OperatorsName.Element = 0x1070;
	ReferencedStudyComponentSequence.Reset();
	ReferencedStudyComponentSequence.Group = 0x0008;
	ReferencedStudyComponentSequence.Element = 0x1111;
	BodyPartExamined.Reset();
	BodyPartExamined.Group = 0x0018;
	BodyPartExamined.Element = 0x0015;
	PatientPosition.Reset();
	PatientPosition.Group = 0x0018;
	PatientPosition.Element = 0x5100;
	SmallestPixelValueInSeries.Reset();
	SmallestPixelValueInSeries.Group = 0x0028;
	SmallestPixelValueInSeries.Element = 0x0108;
	LargestPixelValueInSeries.Reset();
	LargestPixelValueInSeries.Group = 0x0028;
	LargestPixelValueInSeries.Element = 0x0109;
	RequestAttributesSequence.Reset();
	RequestAttributesSequence.Group = 0x004;
	RequestAttributesSequence.Element = 0x00275;
	PerformedProcedureStepID.Reset();
	PerformedProcedureStepID.Group = 0x004;
	PerformedProcedureStepID.Element = 0x00253;
	PerformedProcedureStepStartDate.Reset();
	PerformedProcedureStepStartDate.Group = 0x0040;
	PerformedProcedureStepStartDate.Element = 0x0244;
	PerformedProcedureStepStartTime.Reset();
	PerformedProcedureStepStartTime.Group = 0x0040;
	PerformedProcedureStepStartTime.Element = 0x0245;
	PerformedProcedureStepDescription.Reset();
	PerformedProcedureStepDescription.Group = 0x0040;
	PerformedProcedureStepDescription.Element = 0x0254;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_GeneralSeriesModule :: SetIf	( VR *vr )
	{
	if ( Modality.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesInstanceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( Laterality.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesDate.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesTime.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformingPhysiciansName.SetIf ( vr ) )
		return ( TRUE );
	if ( ProtocolName.SetIf ( vr ) )
		return ( TRUE );
	if ( SeriesDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( OperatorsName.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedStudyComponentSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( BodyPartExamined.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientPosition.SetIf ( vr ) )
		return ( TRUE );
	if ( SmallestPixelValueInSeries.SetIf ( vr ) )
		return ( TRUE );
	if ( LargestPixelValueInSeries.SetIf ( vr ) )
		return ( TRUE );
	if ( RequestAttributesSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformedProcedureStepID.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformedProcedureStepStartDate.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformedProcedureStepStartTime.SetIf ( vr ) )
		return ( TRUE );
	if ( PerformedProcedureStepDescription.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_GeneralSeriesModule :: Morph	( DICOMObject *DO )
	{
	Modality.Morph ( DO );
	SeriesInstanceUID.Morph ( DO );
	SeriesNumber.Morph ( DO );
	Laterality.Morph ( DO );
	SeriesDate.Morph ( DO );
	SeriesTime.Morph ( DO );
	PerformingPhysiciansName.Morph ( DO );
	ProtocolName.Morph ( DO );
	SeriesDescription.Morph ( DO );
	OperatorsName.Morph ( DO );
	ReferencedStudyComponentSequence.Morph ( DO );
	BodyPartExamined.Morph ( DO );
	PatientPosition.Morph ( DO );
	SmallestPixelValueInSeries.Morph ( DO );
	LargestPixelValueInSeries.Morph ( DO );
	RequestAttributesSequence.Morph ( DO );
	PerformedProcedureStepID.Morph ( DO );
	PerformedProcedureStepStartDate.Morph ( DO );
	PerformedProcedureStepStartTime.Morph ( DO );
	PerformedProcedureStepDescription.Morph ( DO );


	return ( FALSE );
	}

//构造 重置 设置 拷贝 for: FrameOfReferenceModule

CO_FrameOfReferenceModule :: CO_FrameOfReferenceModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&FrameOfReferenceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PositionReferenceIndicator);
		}
	}


/*重置*/ BOOL
CO_FrameOfReferenceModule ::  Reset()
	{
	FrameOfReferenceUID.Reset();
	FrameOfReferenceUID.Group = 0x0020;
	FrameOfReferenceUID.Element = 0x0052;
	PositionReferenceIndicator.Reset();
	PositionReferenceIndicator.Group = 0x0020;
	PositionReferenceIndicator.Element = 0x1040;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_FrameOfReferenceModule :: SetIf	( VR *vr )
	{
	if ( FrameOfReferenceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( PositionReferenceIndicator.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_FrameOfReferenceModule :: Morph	( DICOMObject *DO )
	{
	FrameOfReferenceUID.Morph ( DO );
	PositionReferenceIndicator.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: GeneralEquipmentModule

CO_GeneralEquipmentModule :: CO_GeneralEquipmentModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Manufacturer);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstitutionName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstitutionAddress);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&StationName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstitutionalDepartmentName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ManufacturersModelName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DeviceSerialNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SoftwareVersions);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpatialResolution);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DateOfLastCalibration);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TimeOfLastCalibration);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelPaddingValue);
		}
	}


/*重置*/ BOOL
CO_GeneralEquipmentModule ::  Reset()
	{
	Manufacturer.Reset();
	Manufacturer.Group = 0x0008;
	Manufacturer.Element = 0x0070;
	InstitutionName.Reset();
	InstitutionName.Group = 0x0008;
	InstitutionName.Element = 0x0080;
	InstitutionAddress.Reset();
	InstitutionAddress.Group = 0x0008;
	InstitutionAddress.Element = 0x0081;
	StationName.Reset();
	StationName.Group = 0x0008;
	StationName.Element = 0x1010;
	InstitutionalDepartmentName.Reset();
	InstitutionalDepartmentName.Group = 0x0008;
	InstitutionalDepartmentName.Element = 0x1040;
	ManufacturersModelName.Reset();
	ManufacturersModelName.Group = 0x0008;
	ManufacturersModelName.Element = 0x1090;
	DeviceSerialNumber.Reset();
	DeviceSerialNumber.Group = 0x0018;
	DeviceSerialNumber.Element = 0x1000;
	SoftwareVersions.Reset();
	SoftwareVersions.Group = 0x0018;
	SoftwareVersions.Element = 0x1020;
	SpatialResolution.Reset();
	SpatialResolution.Group = 0x0018;
	SpatialResolution.Element = 0x1050;
	DateOfLastCalibration.Reset();
	DateOfLastCalibration.Group = 0x0018;
	DateOfLastCalibration.Element = 0x1200;
	TimeOfLastCalibration.Reset();
	TimeOfLastCalibration.Group = 0x0018;
	TimeOfLastCalibration.Element = 0x1201;
	PixelPaddingValue.Reset();
	PixelPaddingValue.Group = 0x0028;
	PixelPaddingValue.Element = 0x0120;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_GeneralEquipmentModule :: SetIf	( VR *vr )
	{
	if ( Manufacturer.SetIf ( vr ) )
		return ( TRUE );
	if ( InstitutionName.SetIf ( vr ) )
		return ( TRUE );
	if ( InstitutionAddress.SetIf ( vr ) )
		return ( TRUE );
	if ( StationName.SetIf ( vr ) )
		return ( TRUE );
	if ( InstitutionalDepartmentName.SetIf ( vr ) )
		return ( TRUE );
	if ( ManufacturersModelName.SetIf ( vr ) )
		return ( TRUE );
	if ( DeviceSerialNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( SoftwareVersions.SetIf ( vr ) )
		return ( TRUE );
	if ( SpatialResolution.SetIf ( vr ) )
		return ( TRUE );
	if ( DateOfLastCalibration.SetIf ( vr ) )
		return ( TRUE );
	if ( TimeOfLastCalibration.SetIf ( vr ) )
		return ( TRUE );
	if ( PixelPaddingValue.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_GeneralEquipmentModule :: Morph	( DICOMObject *DO )
	{
	Manufacturer.Morph ( DO );
	InstitutionName.Morph ( DO );
	InstitutionAddress.Morph ( DO );
	StationName.Morph ( DO );
	InstitutionalDepartmentName.Morph ( DO );
	ManufacturersModelName.Morph ( DO );
	DeviceSerialNumber.Morph ( DO );
	SoftwareVersions.Morph ( DO );
	SpatialResolution.Morph ( DO );
	DateOfLastCalibration.Morph ( DO );
	TimeOfLastCalibration.Morph ( DO );
	PixelPaddingValue.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: GeneralImageModule

CO_GeneralImageModule :: CO_GeneralImageModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PatientOrientation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AcquisitionNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AcquisitionDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AcquisitionTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReferencedImageSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DerivationDescription);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SourceImageSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImagesInAcquisition);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageComments);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&LossyImageCompression);
		}
	}


/*重置*/ BOOL
CO_GeneralImageModule ::  Reset()
	{
	ImageNumber.Reset();
	ImageNumber.Group = 0x0020;
	ImageNumber.Element = 0x0013;
	PatientOrientation.Reset();
	PatientOrientation.Group = 0x0020;
	PatientOrientation.Element = 0x0020;
	ImageDate.Reset();
	ImageDate.Group = 0x0008;
	ImageDate.Element = 0x0023;
	ImageTime.Reset();
	ImageTime.Group = 0x0008;
	ImageTime.Element = 0x0033;
	ImageType.Reset();
	ImageType.Group = 0x0008;
	ImageType.Element = 0x0008;
	AcquisitionNumber.Reset();
	AcquisitionNumber.Group = 0x0020;
	AcquisitionNumber.Element = 0x0012;
	AcquisitionDate.Reset();
	AcquisitionDate.Group = 0x0008;
	AcquisitionDate.Element = 0x0022;
	AcquisitionTime.Reset();
	AcquisitionTime.Group = 0x0008;
	AcquisitionTime.Element = 0x0032;
	ReferencedImageSequence.Reset();
	ReferencedImageSequence.Group = 0x0008;
	ReferencedImageSequence.Element = 0x1140;
	DerivationDescription.Reset();
	DerivationDescription.Group = 0x0008;
	DerivationDescription.Element = 0x2111;
	SourceImageSequence.Reset();
	SourceImageSequence.Group = 0x0008;
	SourceImageSequence.Element = 0x2112;
	ImagesInAcquisition.Reset();
	ImagesInAcquisition.Group = 0x0020;
	ImagesInAcquisition.Element = 0x1002;
	ImageComments.Reset();
	ImageComments.Group = 0x0020;
	ImageComments.Element = 0x4000;
	LossyImageCompression.Reset();
	LossyImageCompression.Group = 0x0028;
	LossyImageCompression.Element = 0x2110;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_GeneralImageModule :: SetIf	( VR *vr )
	{
	if ( ImageNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( PatientOrientation.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageDate.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageType.SetIf ( vr ) )
		return ( TRUE );
	if ( AcquisitionNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( AcquisitionDate.SetIf ( vr ) )
		return ( TRUE );
	if ( AcquisitionTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ReferencedImageSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( DerivationDescription.SetIf ( vr ) )
		return ( TRUE );
	if ( SourceImageSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( ImagesInAcquisition.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageComments.SetIf ( vr ) )
		return ( TRUE );
	if ( LossyImageCompression.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_GeneralImageModule :: Morph	( DICOMObject *DO )
	{
	ImageNumber.Morph ( DO );
	PatientOrientation.Morph ( DO );
	ImageDate.Morph ( DO );
	ImageTime.Morph ( DO );
	ImageType.Morph ( DO );
	AcquisitionNumber.Morph ( DO );
	AcquisitionDate.Morph ( DO );
	AcquisitionTime.Morph ( DO );
	ReferencedImageSequence.Morph ( DO );
	DerivationDescription.Morph ( DO );
	SourceImageSequence.Morph ( DO );
	ImagesInAcquisition.Morph ( DO );
	ImageComments.Morph ( DO );
	LossyImageCompression.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: ImagePlaneModule

CO_ImagePlaneModule :: CO_ImagePlaneModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelSpacing);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageOrientationPatient);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImagePositionPatient);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SliceThickness);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SliceLocation);
		}
	}


/*重置*/ BOOL
CO_ImagePlaneModule ::  Reset()
	{
	PixelSpacing.Reset();
	PixelSpacing.Group = 0x0028;
	PixelSpacing.Element = 0x0030;
	ImageOrientationPatient.Reset();
	ImageOrientationPatient.Group = 0x0020;
	ImageOrientationPatient.Element = 0x0037;
	ImagePositionPatient.Reset();
	ImagePositionPatient.Group = 0x0020;
	ImagePositionPatient.Element = 0x0032;
	SliceThickness.Reset();
	SliceThickness.Group = 0x0018;
	SliceThickness.Element = 0x0050;
	SliceLocation.Reset();
	SliceLocation.Group = 0x0020;
	SliceLocation.Element = 0x1041;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_ImagePlaneModule :: SetIf	( VR *vr )
	{
	if ( PixelSpacing.SetIf ( vr ) )
		return ( TRUE );
	if ( ImageOrientationPatient.SetIf ( vr ) )
		return ( TRUE );
	if ( ImagePositionPatient.SetIf ( vr ) )
		return ( TRUE );
	if ( SliceThickness.SetIf ( vr ) )
		return ( TRUE );
	if ( SliceLocation.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_ImagePlaneModule :: Morph	( DICOMObject *DO )
	{
	PixelSpacing.Morph ( DO );
	ImageOrientationPatient.Morph ( DO );
	ImagePositionPatient.Morph ( DO );
	SliceThickness.Morph ( DO );
	SliceLocation.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: ImagePixelModule

CO_ImagePixelModule :: CO_ImagePixelModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SamplesPerPixel);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PhotometricInterpretation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Rows);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Columns);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BitsAllocated);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BitsStored);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&HighBit);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelRepresentation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelData);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PlanarConfiguration);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelAspectRatio);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SmallestImagePixelValue);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&LargestImagePixelValue);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&RedPaletteColorLookupTableDescriptor);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&GreenPaletteColorLookupTableDescriptor);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BluePaletteColorLookupTableDescriptor);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&RedPaletteColorLookupTableData);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&GreenPaletteColorLookupTableData);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BluePaletteColorLookupTableData);
		}
	}


/*重置*/ BOOL
CO_ImagePixelModule ::  Reset()
	{
	SamplesPerPixel.Reset();
	SamplesPerPixel.Group = 0x0028;
	SamplesPerPixel.Element = 0x0002;
	PhotometricInterpretation.Reset();
	PhotometricInterpretation.Group = 0x0028;
	PhotometricInterpretation.Element = 0x0004;
	Rows.Reset();
	Rows.Group = 0x0028;
	Rows.Element = 0x0010;
	Columns.Reset();
	Columns.Group = 0x0028;
	Columns.Element = 0x0011;
	BitsAllocated.Reset();
	BitsAllocated.Group = 0x0028;
	BitsAllocated.Element = 0x0100;
	BitsStored.Reset();
	BitsStored.Group = 0x0028;
	BitsStored.Element = 0x0101;
	HighBit.Reset();
	HighBit.Group = 0x0028;
	HighBit.Element = 0x0102;
	PixelRepresentation.Reset();
	PixelRepresentation.Group = 0x0028;
	PixelRepresentation.Element = 0x0103;
	PixelData.Reset();
	PixelData.Group = 0x7FE0;
	PixelData.Element = 0x0010;
	PlanarConfiguration.Reset();
	PlanarConfiguration.Group = 0x0028;
	PlanarConfiguration.Element = 0x0006;
	PixelAspectRatio.Reset();
	PixelAspectRatio.Group = 0x0028;
	PixelAspectRatio.Element = 0x0034;
	SmallestImagePixelValue.Reset();
	SmallestImagePixelValue.Group = 0x0028;
	SmallestImagePixelValue.Element = 0x0106;
	LargestImagePixelValue.Reset();
	LargestImagePixelValue.Group = 0x0028;
	LargestImagePixelValue.Element = 0x0107;
	RedPaletteColorLookupTableDescriptor.Reset();
	RedPaletteColorLookupTableDescriptor.Group = 0x0028;
	RedPaletteColorLookupTableDescriptor.Element = 0x1101;
	GreenPaletteColorLookupTableDescriptor.Reset();
	GreenPaletteColorLookupTableDescriptor.Group = 0x0028;
	GreenPaletteColorLookupTableDescriptor.Element = 0x1102;
	BluePaletteColorLookupTableDescriptor.Reset();
	BluePaletteColorLookupTableDescriptor.Group = 0x0028;
	BluePaletteColorLookupTableDescriptor.Element = 0x1103;
	RedPaletteColorLookupTableData.Reset();
	RedPaletteColorLookupTableData.Group = 0x0028;
	RedPaletteColorLookupTableData.Element = 0x1201;
	GreenPaletteColorLookupTableData.Reset();
	GreenPaletteColorLookupTableData.Group = 0x0028;
	GreenPaletteColorLookupTableData.Element = 0x1202;
	BluePaletteColorLookupTableData.Reset();
	BluePaletteColorLookupTableData.Group = 0x0028;
	BluePaletteColorLookupTableData.Element = 0x1203;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_ImagePixelModule :: SetIf	( VR *vr )
	{
	if ( SamplesPerPixel.SetIf ( vr ) )
		return ( TRUE );
	if ( PhotometricInterpretation.SetIf ( vr ) )
		return ( TRUE );
	if ( Rows.SetIf ( vr ) )
		return ( TRUE );
	if ( Columns.SetIf ( vr ) )
		return ( TRUE );
	if ( BitsAllocated.SetIf ( vr ) )
		return ( TRUE );
	if ( BitsStored.SetIf ( vr ) )
		return ( TRUE );
	if ( HighBit.SetIf ( vr ) )
		return ( TRUE );
	if ( PixelRepresentation.SetIf ( vr ) )
		return ( TRUE );
	if ( PixelData.SetIf ( vr ) )
		return ( TRUE );
	if ( PlanarConfiguration.SetIf ( vr ) )
		return ( TRUE );
	if ( PixelAspectRatio.SetIf ( vr ) )
		return ( TRUE );
	if ( SmallestImagePixelValue.SetIf ( vr ) )
		return ( TRUE );
	if ( LargestImagePixelValue.SetIf ( vr ) )
		return ( TRUE );
	if ( RedPaletteColorLookupTableDescriptor.SetIf ( vr ) )
		return ( TRUE );
	if ( GreenPaletteColorLookupTableDescriptor.SetIf ( vr ) )
		return ( TRUE );
	if ( BluePaletteColorLookupTableDescriptor.SetIf ( vr ) )
		return ( TRUE );
	if ( RedPaletteColorLookupTableData.SetIf ( vr ) )
		return ( TRUE );
	if ( GreenPaletteColorLookupTableData.SetIf ( vr ) )
		return ( TRUE );
	if ( BluePaletteColorLookupTableData.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_ImagePixelModule :: Morph	( DICOMObject *DO )
	{
	SamplesPerPixel.Morph ( DO );
	PhotometricInterpretation.Morph ( DO );
	Rows.Morph ( DO );
	Columns.Morph ( DO );
	BitsAllocated.Morph ( DO );
	BitsStored.Morph ( DO );
	HighBit.Morph ( DO );
	PixelRepresentation.Morph ( DO );
	PixelData.Morph ( DO );
	PlanarConfiguration.Morph ( DO );
	PixelAspectRatio.Morph ( DO );
	SmallestImagePixelValue.Morph ( DO );
	LargestImagePixelValue.Morph ( DO );
	RedPaletteColorLookupTableDescriptor.Morph ( DO );
	GreenPaletteColorLookupTableDescriptor.Morph ( DO );
	BluePaletteColorLookupTableDescriptor.Morph ( DO );
	RedPaletteColorLookupTableData.Morph ( DO );
	GreenPaletteColorLookupTableData.Morph ( DO );
	BluePaletteColorLookupTableData.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: ContrastBolusModule

CO_ContrastBolusModule :: CO_ContrastBolusModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusAgent);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusAgentSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusRoute);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusAdministrationRouteSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusVolume);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusStartTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusStopTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusTotalDose);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastFlowRates);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastFlowDurations);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusIngredient);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ContrastBolusIngredientConcentration);
		}
	}


/*重置*/ BOOL
CO_ContrastBolusModule ::  Reset()
	{
	ContrastBolusAgent.Reset();
	ContrastBolusAgent.Group = 0x0018;
	ContrastBolusAgent.Element = 0x0010;
	ContrastBolusAgentSequence.Reset();
	ContrastBolusAgentSequence.Group = 0x0018;
	ContrastBolusAgentSequence.Element = 0x0012;
	ContrastBolusRoute.Reset();
	ContrastBolusRoute.Group = 0x0018;
	ContrastBolusRoute.Element = 0x1040;
	ContrastBolusAdministrationRouteSequence.Reset();
	ContrastBolusAdministrationRouteSequence.Group = 0x0018;
	ContrastBolusAdministrationRouteSequence.Element = 0x0014;
	ContrastBolusVolume.Reset();
	ContrastBolusVolume.Group = 0x0018;
	ContrastBolusVolume.Element = 0x1041;
	ContrastBolusStartTime.Reset();
	ContrastBolusStartTime.Group = 0x0018;
	ContrastBolusStartTime.Element = 0x1042;
	ContrastBolusStopTime.Reset();
	ContrastBolusStopTime.Group = 0x0018;
	ContrastBolusStopTime.Element = 0x1043;
	ContrastBolusTotalDose.Reset();
	ContrastBolusTotalDose.Group = 0x0018;
	ContrastBolusTotalDose.Element = 0x1044;
	ContrastFlowRates.Reset();
	ContrastFlowRates.Group = 0x0018;
	ContrastFlowRates.Element = 0x1046;
	ContrastFlowDurations.Reset();
	ContrastFlowDurations.Group = 0x0018;
	ContrastFlowDurations.Element = 0x1047;
	ContrastBolusIngredient.Reset();
	ContrastBolusIngredient.Group = 0x0018;
	ContrastBolusIngredient.Element = 0x1048;
	ContrastBolusIngredientConcentration.Reset();
	ContrastBolusIngredientConcentration.Group = 0x0018;
	ContrastBolusIngredientConcentration.Element = 0x1049;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_ContrastBolusModule :: SetIf	( VR *vr )
	{
	if ( ContrastBolusAgent.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusAgentSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusRoute.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusAdministrationRouteSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusVolume.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusStartTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusStopTime.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusTotalDose.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastFlowRates.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastFlowDurations.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusIngredient.SetIf ( vr ) )
		return ( TRUE );
	if ( ContrastBolusIngredientConcentration.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_ContrastBolusModule :: Morph	( DICOMObject *DO )
	{
	ContrastBolusAgent.Morph ( DO );
	ContrastBolusAgentSequence.Morph ( DO );
	ContrastBolusRoute.Morph ( DO );
	ContrastBolusAdministrationRouteSequence.Morph ( DO );
	ContrastBolusVolume.Morph ( DO );
	ContrastBolusStartTime.Morph ( DO );
	ContrastBolusStopTime.Morph ( DO );
	ContrastBolusTotalDose.Morph ( DO );
	ContrastFlowRates.Morph ( DO );
	ContrastFlowDurations.Morph ( DO );
	ContrastBolusIngredient.Morph ( DO );
	ContrastBolusIngredientConcentration.Morph ( DO );


	return ( FALSE );
	}




//构造 重置 设置 拷贝 for: MRImageModule

CO_MRImageModule :: CO_MRImageModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImageType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SamplesPerPixel);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PhotometricInterpretation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BitsAllocated);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ScanningSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&DequenceVariant);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ScanOptions);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&MRAcquisitionType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&RepetitionTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&EchoTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&EchoTrainLength);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InversionTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TriggerTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SequenceName);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AngioFlag);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NumberOfAverages);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImagingFrequency);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ImagedNucleus);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&EchoNumber);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&MagneticFieldStrength);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpacingBetweenSlices);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NumberOfPhaseEncodingSteps);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PercentSampling);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PercentPhaseFieldOfView);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PixelBandwidth);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NominalInterval);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BeatRejectionFlag);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&LowRRValue);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&IntervalsAcquired);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InvervalsRejected);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PVCRejection);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SkipBeats);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&HeartRate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&CardiacNumberOfImages);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TriggerWindow);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReconstructionDiameter);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ReceivingCoil);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TransmittingCoil);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&AcquisitionMatrix);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&PhaseEncodingDirection);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&FlipAngle);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SAR);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&VariableFlipAngleFlag);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&dBdt);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TemporalPositionIdentifier);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&NumberOfTemporalPositions);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&TemporalResolution);
		}
	}


/*重置*/ BOOL
CO_MRImageModule ::  Reset()
	{
	ImageType.Reset();
	ImageType.Group = 0x0008;
	ImageType.Element = 0x0008;
	SamplesPerPixel.Reset();
	SamplesPerPixel.Group = 0x0028;
	SamplesPerPixel.Element = 0x0002;
	PhotometricInterpretation.Reset();
	PhotometricInterpretation.Group = 0x0028;
	PhotometricInterpretation.Element = 0x0004;
	BitsAllocated.Reset();
	BitsAllocated.Group = 0x0028;
	BitsAllocated.Element = 0x0100;
	ScanningSequence.Reset();
	ScanningSequence.Group = 0x0018;
	ScanningSequence.Element = 0x0020;
	DequenceVariant.Reset();
	DequenceVariant.Group = 0x0018;
	DequenceVariant.Element = 0x0021;
	ScanOptions.Reset();
	ScanOptions.Group = 0x0018;
	ScanOptions.Element = 0x0022;
	MRAcquisitionType.Reset();
	MRAcquisitionType.Group = 0x0018;
	MRAcquisitionType.Element = 0x0023;
	RepetitionTime.Reset();
	RepetitionTime.Group = 0x0018;
	RepetitionTime.Element = 0x0080;
	EchoTime.Reset();
	EchoTime.Group = 0x0018;
	EchoTime.Element = 0x0081;
	EchoTrainLength.Reset();
	EchoTrainLength.Group = 0x0018;
	EchoTrainLength.Element = 0x0091;
	InversionTime.Reset();
	InversionTime.Group = 0x0018;
	InversionTime.Element = 0x0082;
	TriggerTime.Reset();
	TriggerTime.Group = 0x0018;
	TriggerTime.Element = 0x1060;
	SequenceName.Reset();
	SequenceName.Group = 0x0018;
	SequenceName.Element = 0x0024;
	AngioFlag.Reset();
	AngioFlag.Group = 0x0018;
	AngioFlag.Element = 0x0025;
	NumberOfAverages.Reset();
	NumberOfAverages.Group = 0x0018;
	NumberOfAverages.Element = 0x0083;
	ImagingFrequency.Reset();
	ImagingFrequency.Group = 0x0018;
	ImagingFrequency.Element = 0x0084;
	ImagedNucleus.Reset();
	ImagedNucleus.Group = 0x0018;
	ImagedNucleus.Element = 0x0085;
	EchoNumber.Reset();
	EchoNumber.Group = 0x0018;
	EchoNumber.Element = 0x0086;
	MagneticFieldStrength.Reset();
	MagneticFieldStrength.Group = 0x0018;
	MagneticFieldStrength.Element = 0x0087;
	SpacingBetweenSlices.Reset();
	SpacingBetweenSlices.Group = 0x0018;
	SpacingBetweenSlices.Element = 0x0088;
	NumberOfPhaseEncodingSteps.Reset();
	NumberOfPhaseEncodingSteps.Group = 0x0018;
	NumberOfPhaseEncodingSteps.Element = 0x0089;
	PercentSampling.Reset();
	PercentSampling.Group = 0x0018;
	PercentSampling.Element = 0x0093;
	PercentPhaseFieldOfView.Reset();
	PercentPhaseFieldOfView.Group = 0x0018;
	PercentPhaseFieldOfView.Element = 0x0094;
	PixelBandwidth.Reset();
	PixelBandwidth.Group = 0x0018;
	PixelBandwidth.Element = 0x0095;
	NominalInterval.Reset();
	NominalInterval.Group = 0x0018;
	NominalInterval.Element = 0x1062;
	BeatRejectionFlag.Reset();
	BeatRejectionFlag.Group = 0x0018;
	BeatRejectionFlag.Element = 0x1080;
	LowRRValue.Reset();
	LowRRValue.Group = 0x0018;
	LowRRValue.Element = 0x1081;
	IntervalsAcquired.Reset();
	IntervalsAcquired.Group = 0x0018;
	IntervalsAcquired.Element = 0x1083;
	InvervalsRejected.Reset();
	InvervalsRejected.Group = 0x0018;
	InvervalsRejected.Element = 0x1084;
	PVCRejection.Reset();
	PVCRejection.Group = 0x0018;
	PVCRejection.Element = 0x1085;
	SkipBeats.Reset();
	SkipBeats.Group = 0x0018;
	SkipBeats.Element = 0x1086;
	HeartRate.Reset();
	HeartRate.Group = 0x0018;
	HeartRate.Element = 0x1088;
	CardiacNumberOfImages.Reset();
	CardiacNumberOfImages.Group = 0x0018;
	CardiacNumberOfImages.Element = 0x1090;
	TriggerWindow.Reset();
	TriggerWindow.Group = 0x0018;
	TriggerWindow.Element = 0x1094;
	ReconstructionDiameter.Reset();
	ReconstructionDiameter.Group = 0x0018;
	ReconstructionDiameter.Element = 0x1100;
	ReceivingCoil.Reset();
	ReceivingCoil.Group = 0x0018;
	ReceivingCoil.Element = 0x1250;
	TransmittingCoil.Reset();
	TransmittingCoil.Group = 0x0018;
	TransmittingCoil.Element = 0x1251;
	AcquisitionMatrix.Reset();
	AcquisitionMatrix.Group = 0x0018;
	AcquisitionMatrix.Element = 0x1310;
	PhaseEncodingDirection.Reset();
	PhaseEncodingDirection.Group = 0x0018;
	PhaseEncodingDirection.Element = 0x1312;
	FlipAngle.Reset();
	FlipAngle.Group = 0x0018;
	FlipAngle.Element = 0x1312;
	SAR.Reset();
	SAR.Group = 0x0018;
	SAR.Element = 0x1316;
	VariableFlipAngleFlag.Reset();
	VariableFlipAngleFlag.Group = 0x0018;
	VariableFlipAngleFlag.Element = 0x1315;
	dBdt.Reset();
	dBdt.Group = 0x0018;
	dBdt.Element = 0x1318;
	TemporalPositionIdentifier.Reset();
	TemporalPositionIdentifier.Group = 0x0020;
	TemporalPositionIdentifier.Element = 0x0100;
	NumberOfTemporalPositions.Reset();
	NumberOfTemporalPositions.Group = 0x0020;
	NumberOfTemporalPositions.Element = 0x0105;
	TemporalResolution.Reset();
	TemporalResolution.Group = 0x0020;
	TemporalResolution.Element = 0x0110;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_MRImageModule :: SetIf	( VR *vr )
	{
	if ( ImageType.SetIf ( vr ) )
		return ( TRUE );
	if ( SamplesPerPixel.SetIf ( vr ) )
		return ( TRUE );
	if ( PhotometricInterpretation.SetIf ( vr ) )
		return ( TRUE );
	if ( BitsAllocated.SetIf ( vr ) )
		return ( TRUE );
	if ( ScanningSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( DequenceVariant.SetIf ( vr ) )
		return ( TRUE );
	if ( ScanOptions.SetIf ( vr ) )
		return ( TRUE );
	if ( MRAcquisitionType.SetIf ( vr ) )
		return ( TRUE );
	if ( RepetitionTime.SetIf ( vr ) )
		return ( TRUE );
	if ( EchoTime.SetIf ( vr ) )
		return ( TRUE );
	if ( EchoTrainLength.SetIf ( vr ) )
		return ( TRUE );
	if ( InversionTime.SetIf ( vr ) )
		return ( TRUE );
	if ( TriggerTime.SetIf ( vr ) )
		return ( TRUE );
	if ( SequenceName.SetIf ( vr ) )
		return ( TRUE );
	if ( AngioFlag.SetIf ( vr ) )
		return ( TRUE );
	if ( NumberOfAverages.SetIf ( vr ) )
		return ( TRUE );
	if ( ImagingFrequency.SetIf ( vr ) )
		return ( TRUE );
	if ( ImagedNucleus.SetIf ( vr ) )
		return ( TRUE );
	if ( EchoNumber.SetIf ( vr ) )
		return ( TRUE );
	if ( MagneticFieldStrength.SetIf ( vr ) )
		return ( TRUE );
	if ( SpacingBetweenSlices.SetIf ( vr ) )
		return ( TRUE );
	if ( NumberOfPhaseEncodingSteps.SetIf ( vr ) )
		return ( TRUE );
	if ( PercentSampling.SetIf ( vr ) )
		return ( TRUE );
	if ( PercentPhaseFieldOfView.SetIf ( vr ) )
		return ( TRUE );
	if ( PixelBandwidth.SetIf ( vr ) )
		return ( TRUE );
	if ( NominalInterval.SetIf ( vr ) )
		return ( TRUE );
	if ( BeatRejectionFlag.SetIf ( vr ) )
		return ( TRUE );
	if ( LowRRValue.SetIf ( vr ) )
		return ( TRUE );
	if ( IntervalsAcquired.SetIf ( vr ) )
		return ( TRUE );
	if ( InvervalsRejected.SetIf ( vr ) )
		return ( TRUE );
	if ( PVCRejection.SetIf ( vr ) )
		return ( TRUE );
	if ( SkipBeats.SetIf ( vr ) )
		return ( TRUE );
	if ( HeartRate.SetIf ( vr ) )
		return ( TRUE );
	if ( CardiacNumberOfImages.SetIf ( vr ) )
		return ( TRUE );
	if ( TriggerWindow.SetIf ( vr ) )
		return ( TRUE );
	if ( ReconstructionDiameter.SetIf ( vr ) )
		return ( TRUE );
	if ( ReceivingCoil.SetIf ( vr ) )
		return ( TRUE );
	if ( TransmittingCoil.SetIf ( vr ) )
		return ( TRUE );
	if ( AcquisitionMatrix.SetIf ( vr ) )
		return ( TRUE );
	if ( PhaseEncodingDirection.SetIf ( vr ) )
		return ( TRUE );
	if ( FlipAngle.SetIf ( vr ) )
		return ( TRUE );
	if ( SAR.SetIf ( vr ) )
		return ( TRUE );
	if ( VariableFlipAngleFlag.SetIf ( vr ) )
		return ( TRUE );
	if ( dBdt.SetIf ( vr ) )
		return ( TRUE );
	if ( TemporalPositionIdentifier.SetIf ( vr ) )
		return ( TRUE );
	if ( NumberOfTemporalPositions.SetIf ( vr ) )
		return ( TRUE );
	if ( TemporalResolution.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_MRImageModule :: Morph	( DICOMObject *DO )
	{
	ImageType.Morph ( DO );
	SamplesPerPixel.Morph ( DO );
	PhotometricInterpretation.Morph ( DO );
	BitsAllocated.Morph ( DO );
	ScanningSequence.Morph ( DO );
	DequenceVariant.Morph ( DO );
	ScanOptions.Morph ( DO );
	MRAcquisitionType.Morph ( DO );
	RepetitionTime.Morph ( DO );
	EchoTime.Morph ( DO );
	EchoTrainLength.Morph ( DO );
	InversionTime.Morph ( DO );
	TriggerTime.Morph ( DO );
	SequenceName.Morph ( DO );
	AngioFlag.Morph ( DO );
	NumberOfAverages.Morph ( DO );
	ImagingFrequency.Morph ( DO );
	ImagedNucleus.Morph ( DO );
	EchoNumber.Morph ( DO );
	MagneticFieldStrength.Morph ( DO );
	SpacingBetweenSlices.Morph ( DO );
	NumberOfPhaseEncodingSteps.Morph ( DO );
	PercentSampling.Morph ( DO );
	PercentPhaseFieldOfView.Morph ( DO );
	PixelBandwidth.Morph ( DO );
	NominalInterval.Morph ( DO );
	BeatRejectionFlag.Morph ( DO );
	LowRRValue.Morph ( DO );
	IntervalsAcquired.Morph ( DO );
	InvervalsRejected.Morph ( DO );
	PVCRejection.Morph ( DO );
	SkipBeats.Morph ( DO );
	HeartRate.Morph ( DO );
	CardiacNumberOfImages.Morph ( DO );
	TriggerWindow.Morph ( DO );
	ReconstructionDiameter.Morph ( DO );
	ReceivingCoil.Morph ( DO );
	TransmittingCoil.Morph ( DO );
	AcquisitionMatrix.Morph ( DO );
	PhaseEncodingDirection.Morph ( DO );
	FlipAngle.Morph ( DO );
	SAR.Morph ( DO );
	VariableFlipAngleFlag.Morph ( DO );
	dBdt.Morph ( DO );
	TemporalPositionIdentifier.Morph ( DO );
	NumberOfTemporalPositions.Morph ( DO );
	TemporalResolution.Morph ( DO );


	return ( FALSE );
	}



//构造 重置 设置 拷贝 for: OverlayPlaneModule

CO_OverlayPlaneModule :: CO_OverlayPlaneModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Rows);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Columns);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayType);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&Origin);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BitsAllocated);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&BitPosition);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayData);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ROIArea);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ROIMean);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&ROIStandardDeviation);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayDescriptorGray);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayDescriptorRed);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayDescriptorGreen);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlayDescriptorBlue);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlaysGray);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlaysRed);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlaysGreen);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&OverlaysBlue);
		}
	}


/*重置*/ BOOL
CO_OverlayPlaneModule ::  Reset()
	{
	Rows.Reset();
	Rows.Group = 0x6001;
	Rows.Element = 0x0010;
	Columns.Reset();
	Columns.Group = 0x6001;
	Columns.Element = 0x0011;
	OverlayType.Reset();
	OverlayType.Group = 0x6001;
	OverlayType.Element = 0x0040;
	Origin.Reset();
	Origin.Group = 0x6001;
	Origin.Element = 0x0050;
	BitsAllocated.Reset();
	BitsAllocated.Group = 0x6001;
	BitsAllocated.Element = 0x0100;
	BitPosition.Reset();
	BitPosition.Group = 0x6001;
	BitPosition.Element = 0x0102;
	OverlayData.Reset();
	OverlayData.Group = 0x6001;
	OverlayData.Element = 0x3000;
	ROIArea.Reset();
	ROIArea.Group = 0x6001;
	ROIArea.Element = 0x1301;
	ROIMean.Reset();
	ROIMean.Group = 0x6001;
	ROIMean.Element = 0x1302;
	ROIStandardDeviation.Reset();
	ROIStandardDeviation.Group = 0x6001;
	ROIStandardDeviation.Element = 0x1303;
	OverlayDescriptorGray.Reset();
	OverlayDescriptorGray.Group = 0x6001;
	OverlayDescriptorGray.Element = 0x1100;
	OverlayDescriptorRed.Reset();
	OverlayDescriptorRed.Group = 0x6001;
	OverlayDescriptorRed.Element = 0x1101;
	OverlayDescriptorGreen.Reset();
	OverlayDescriptorGreen.Group = 0x6001;
	OverlayDescriptorGreen.Element = 0x1102;
	OverlayDescriptorBlue.Reset();
	OverlayDescriptorBlue.Group = 0x6001;
	OverlayDescriptorBlue.Element = 0x1103;
	OverlaysGray.Reset();
	OverlaysGray.Group = 0x6001;
	OverlaysGray.Element = 0x1200;
	OverlaysRed.Reset();
	OverlaysRed.Group = 0x6001;
	OverlaysRed.Element = 0x1201;
	OverlaysGreen.Reset();
	OverlaysGreen.Group = 0x6001;
	OverlaysGreen.Element = 0x1202;
	OverlaysBlue.Reset();
	OverlaysBlue.Group = 0x6001;
	OverlaysBlue.Element = 0x1203;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_OverlayPlaneModule :: SetIf	( VR *vr )
	{
	if ( Rows.SetIf ( vr ) )
		return ( TRUE );
	if ( Columns.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayType.SetIf ( vr ) )
		return ( TRUE );
	if ( Origin.SetIf ( vr ) )
		return ( TRUE );
	if ( BitsAllocated.SetIf ( vr ) )
		return ( TRUE );
	if ( BitPosition.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayData.SetIf ( vr ) )
		return ( TRUE );
	if ( ROIArea.SetIf ( vr ) )
		return ( TRUE );
	if ( ROIMean.SetIf ( vr ) )
		return ( TRUE );
	if ( ROIStandardDeviation.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayDescriptorGray.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayDescriptorRed.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayDescriptorGreen.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlayDescriptorBlue.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlaysGray.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlaysRed.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlaysGreen.SetIf ( vr ) )
		return ( TRUE );
	if ( OverlaysBlue.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_OverlayPlaneModule :: Morph	( DICOMObject *DO )
	{
	Rows.Morph ( DO );
	Columns.Morph ( DO );
	OverlayType.Morph ( DO );
	Origin.Morph ( DO );
	BitsAllocated.Morph ( DO );
	BitPosition.Morph ( DO );
	OverlayData.Morph ( DO );
	ROIArea.Morph ( DO );
	ROIMean.Morph ( DO );
	ROIStandardDeviation.Morph ( DO );
	OverlayDescriptorGray.Morph ( DO );
	OverlayDescriptorRed.Morph ( DO );
	OverlayDescriptorGreen.Morph ( DO );
	OverlayDescriptorBlue.Morph ( DO );
	OverlaysGray.Morph ( DO );
	OverlaysRed.Morph ( DO );
	OverlaysGreen.Morph ( DO );
	OverlaysBlue.Morph ( DO );


	return ( FALSE );
	}


//构造 重置 设置 拷贝 for: VOILUTModule

CO_VOILUTModule :: CO_VOILUTModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&VOILUTSequence);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&WindowCenter);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&WindowWidth);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&WindowCenterAndWidthExplanation);
		}
	}


/*重置*/ BOOL
CO_VOILUTModule ::  Reset()
	{
	VOILUTSequence.Reset();
	VOILUTSequence.Group = 0x0028;
	VOILUTSequence.Element = 0x3010;
	WindowCenter.Reset();
	WindowCenter.Group = 0x0028;
	WindowCenter.Element = 0x1050;
	WindowWidth.Reset();
	WindowWidth.Group = 0x0028;
	WindowWidth.Element = 0x1051;
	WindowCenterAndWidthExplanation.Reset();
	WindowCenterAndWidthExplanation.Group = 0x0028;
	WindowCenterAndWidthExplanation.Element = 0x1055;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_VOILUTModule :: SetIf	( VR *vr )
	{
	if ( VOILUTSequence.SetIf ( vr ) )
		return ( TRUE );
	if ( WindowCenter.SetIf ( vr ) )
		return ( TRUE );
	if ( WindowWidth.SetIf ( vr ) )
		return ( TRUE );
	if ( WindowCenterAndWidthExplanation.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_VOILUTModule :: Morph	( DICOMObject *DO )
	{
	VOILUTSequence.Morph ( DO );
	WindowCenter.Morph ( DO );
	WindowWidth.Morph ( DO );
	WindowCenterAndWidthExplanation.Morph ( DO );


	return ( FALSE );
	}



//构造 重置 设置 拷贝 for: SOPCommonModule

CO_SOPCommonModule :: CO_SOPCommonModule(
	CCO_Abstract *LinkUp)
	{
	Reset();
	if ( LinkUp )
		{
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SOPClassUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SOPInstanceUID);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&SpecificCharacterSet);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstanceCreationDate);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstanceCreationTime);
		LinkUp->ReferentialIntegrityDatabase.AddReference(
			&InstanceCreatorUID);
		}
	}


/*重置*/ BOOL
CO_SOPCommonModule ::  Reset()
	{
	SOPClassUID.Reset();
	SOPClassUID.Group = 0x0008;
	SOPClassUID.Element = 0x0016;
	SOPInstanceUID.Reset();
	SOPInstanceUID.Group = 0x0008;
	SOPInstanceUID.Element = 0x0018;
	SpecificCharacterSet.Reset();
	SpecificCharacterSet.Group = 0x0008;
	SpecificCharacterSet.Element = 0x0005;
	InstanceCreationDate.Reset();
	InstanceCreationDate.Group = 0x0008;
	InstanceCreationDate.Element = 0x0012;
	InstanceCreationTime.Reset();
	InstanceCreationTime.Group = 0x0008;
	InstanceCreationTime.Element = 0x0013;
	InstanceCreatorUID.Reset();
	InstanceCreatorUID.Group = 0x0008;
	InstanceCreatorUID.Element = 0x0014;

	return ( TRUE );
	}


/*设置*/ BOOL
CO_SOPCommonModule :: SetIf	( VR *vr )
	{
	if ( SOPClassUID.SetIf ( vr ) )
		return ( TRUE );
	if ( SOPInstanceUID.SetIf ( vr ) )
		return ( TRUE );
	if ( SpecificCharacterSet.SetIf ( vr ) )
		return ( TRUE );
	if ( InstanceCreationDate.SetIf ( vr ) )
		return ( TRUE );
	if ( InstanceCreationTime.SetIf ( vr ) )
		return ( TRUE );
	if ( InstanceCreatorUID.SetIf ( vr ) )
		return ( TRUE );

	return ( FALSE );
	}


/*拷贝*/ BOOL
CO_SOPCommonModule :: Morph	( DICOMObject *DO )
	{
	SOPClassUID.Morph ( DO );
	SOPInstanceUID.Morph ( DO );
	SpecificCharacterSet.Morph ( DO );
	InstanceCreationDate.Morph ( DO );
	InstanceCreationTime.Morph ( DO );
	InstanceCreatorUID.Morph ( DO );


	return ( FALSE );
	}


