#ifndef	iodbase_hpp
#define	iodbase_hpp

// F3.2.1
// Class Module:FileSetIdentificationModule
//用于DICOMDIR
class	CO_FileSetIdentificationModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_FileSetIdentificationModule(CCO_Abstract *Composite=NULL);

		VR_CS	FileSetID;
		VR_CS	FileSetDescriptorFileID;
		VR_CS	SpecificCharacterSetOfFileSetDescriptorFile;
	}; // End of class defination for: FileSetIdentificationModule

// F3.2.2
// Class Module:DirectoryInformationModule
//用于DICOMDIR
class	CO_DirectoryInformationModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_DirectoryInformationModule(CCO_Abstract *Composite=NULL);

		VR_UL	OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity;
		VR_UL	OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity;
		VR_US	FileSetConsistencyFlag;
		VR_SQ	DirectoryRecordSequence;
	}; // End of class defination for: DirectoryInformationModule

// Class Module:DirectoryRecordSequenceModule
//用于DICOMDIR
class	CO_DirectoryRecordSequenceModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_DirectoryRecordSequenceModule(CCO_Abstract *Composite=NULL);

		VR_UL	OffsetOfTheNextDirectoryRecord;
		VR_US	RecordInUseFlag;
		VR_UL	OffsetOfReferencedLowerLevelDirectoryEntity;
		VR_CS	DirectoryRecordType;
		VR_UI	PrivateRecordUID;
		VR_CS	ReferencedFileID;
		VR_UL	MRDRDirectoryRecordOffset;
		VR_UI	ReferencedSOPClassUIDInFile;
		VR_UI	ReferencedSOPInstanceUIDInFile;
		VR_UI	ReferencedTransferSyntaxUIDInFile;
	}; // End of class defination for: DirectoryRecordSequenceModule

//F5.1
// Class Module:PatientDirectoryRecordModule
//用于DICOMDIR
class	CO_PatientDirectoryRecordModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_PatientDirectoryRecordModule(CCO_Abstract *Composite=NULL);

		VR_CS	SpecificCharacterSet;
		VR_PN	PatientsName;
		VR_LO	PatientID;
		VR_DA	PatientBirthDate;
		VR_CS	PatientSex;
	}; // End of class defination for: PatientDirectoryRecordModule

//F5.2
// Class Module:StudyDirectoryRecordModule
//用于DICOMDIR
class	CO_StudyDirectoryRecordModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_StudyDirectoryRecordModule(CCO_Abstract *Composite=NULL);

		VR_CS	SpecificCharacterSet;
		VR_DA	StudyDate;
		VR_TM	StudyTime;
		VR_LO	StudyDescription;
		VR_UI	StudyInstanceUID;
		VR_SH	StudyID;
		VR_SH	AccessionNumber;
		VR_LO	AdmittingDiagnosisDescription;
	}; // End of class defination for: StudyDirectoryRecordModule

//F5.3	
// Class Module:SeriesDirectoryRecordModule
//用于DICOMDIR
class	CO_SeriesDirectoryRecordModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_SeriesDirectoryRecordModule(CCO_Abstract *Composite=NULL);

		VR_CS	SpecificCharacterSet;
		VR_CS	Modality;
		VR_UI	SeriesInstanceUID;
		VR_IS	SeriesNumber;
		VR_PN	PerformingPhysicianName;
		VR_LO	SeriesDescription;
		VR_CS	BodyPartExamined;
		VR_LO	Manufacturer;
		VR_SH	StationName;
		VR_LO	ManufacturerModelName;
		VR_SQ	IconImageSequence;
	}; // End of class defination for: SeriesDirectoryRecordModule

//F5.4
// Class Module:ImageDirectoryRecordModule
//用于DICOMDIR
class	CO_ImageDirectoryRecordModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ImageDirectoryRecordModule(CCO_Abstract *Composite=NULL);

		VR_CS	SpecificCharacterSet;
		VR_DA	ImageDate;
		VR_TM	ImageTime;
		VR_CS	ImageType;
		VR_LT	ImageComments;
		VR_CS	PhotometricInterpretation;
		VR_IS	NumberOfFrames;
		VR_LO	TransducerData;
		VR_CS	TransducerType;
		VR_IS	InstanceNumber;
		VR_SQ	IconImageSequence;
	}; // End of class defination for: ImageDirectoryRecordModule

// C.7.1
// Class Module:PatientModule
//用于MRIMAGE
class	CO_PatientModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_PatientModule(CCO_Abstract *Composite=NULL);

		VR_PN	PatientsName;
		VR_LO	PatientID;
		VR_DA	PatientsBirthDate;
		VR_CS	PatientsSex;
		VR_SQ	ReferencedPatientSequence;
		VR_TM	PatientsBirthTime;
		VR_LO	OtherPatientIDs;
		VR_PN	OtherPatientNames;
		VR_SH	EthnicGroup;
		VR_LT	PatientComments;
	}; // End of class defination for: PatientModule


// C.7.2.1
// Class Module:GeneralStudyModule
//用于MRIMAGE
class	CO_GeneralStudyModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_GeneralStudyModule(CCO_Abstract *Composite=NULL);

		VR_UI	StudyInstanceUID;
		VR_DA	StudyDate;
		VR_TM	StudyTime;
		VR_PN	ReferringPhysiciansName;
		VR_SH	StudyID;
		VR_SH	AccessionNumber;
		VR_LO	StudyDescription;
		VR_PN	PhysiciansOfRecord;
		VR_PN	NameOfPhysiciansReadingStudy;
		VR_SQ	ReferencedStudySequence;
	}; // End of class defination for: GeneralStudyModule

// Class Module:ReferencedStudySequence
//用于MRIMAGE
class	CO_ReferencedStudySequence	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ReferencedStudySequence(CCO_Abstract *Composite=NULL);

		VR_UI	ReferencedSOPClassUID;
		VR_UI	ReferencedSOPInstanceUID;
	}; // End of class defination for: ReferencedStudySequence

// C.7.2.2
// Class Module:PatientStudyModule
//用于MRIMAGE
class	CO_PatientStudyModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_PatientStudyModule(CCO_Abstract *Composite=NULL);

		VR_LO	AdmittingDiagnosesDescription;
		VR_AS	PatientsAge;
		VR_DS	PatientsSize;
		VR_DS	PatientsWeight;
		VR_SH	Occupation;
		VR_LT	AdditionalPatientsHistory;
	}; // End of class defination for: PatientStudyModule

// C.7.3.1
// Class Module:GeneralSeriesModule
//用于MRIMAGE
class	CO_GeneralSeriesModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_GeneralSeriesModule(CCO_Abstract *Composite=NULL);

		VR_CS	Modality;
		VR_UI	SeriesInstanceUID;
		VR_IS	SeriesNumber;
		VR_CS	Laterality;
		VR_DA	SeriesDate;
		VR_TM	SeriesTime;
		VR_PN	PerformingPhysiciansName;
		VR_LO	ProtocolName;
		VR_LO	SeriesDescription;
		VR_PN	OperatorsName;
		VR_SQ	ReferencedStudyComponentSequence;
		VR_CS	BodyPartExamined;
		VR_CS	PatientPosition;
		VR_XS	SmallestPixelValueInSeries;
		VR_XS	LargestPixelValueInSeries;
		VR_SQ	RequestAttributesSequence;
		VR_SH	PerformedProcedureStepID;
		VR_DA	PerformedProcedureStepStartDate;
		VR_TM	PerformedProcedureStepStartTime;
		VR_LO	PerformedProcedureStepDescription;
	}; // End of class defination for: GeneralSeriesModule

// C.7.4.1
// Class Module:FrameOfReferenceModule
//用于MRIMAGE
class	CO_FrameOfReferenceModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_FrameOfReferenceModule(CCO_Abstract *Composite=NULL);

		VR_UI	FrameOfReferenceUID;
		VR_LO	PositionReferenceIndicator;
	}; // End of class defination for: FrameOfReferenceModule

// C.7.5.1
// Class Module:GeneralEquipmentModule
//用于MRIMAGE
class	CO_GeneralEquipmentModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_GeneralEquipmentModule(CCO_Abstract *Composite=NULL);

		VR_LO	Manufacturer;
		VR_LO	InstitutionName;
		VR_ST	InstitutionAddress;
		VR_ST	StationName;
		VR_LO	InstitutionalDepartmentName;
		VR_LO	ManufacturersModelName;
		VR_LO	DeviceSerialNumber;
		VR_LO	SoftwareVersions;
		VR_DS	SpatialResolution;
		VR_DA	DateOfLastCalibration;
		VR_TM	TimeOfLastCalibration;
		VR_XS	PixelPaddingValue;
	}; // End of class defination for: GeneralEquipmentModule

// C.7.6.1
// Class Module:GeneralImageModule
//用于MRIMAGE
class	CO_GeneralImageModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_GeneralImageModule(CCO_Abstract *Composite=NULL);

		VR_IS	ImageNumber;
		VR_CS	PatientOrientation;
		VR_DA	ImageDate;
		VR_TM	ImageTime;
		VR_CS	ImageType;
		VR_IS	AcquisitionNumber;
		VR_DA	AcquisitionDate;
		VR_TM	AcquisitionTime;
		VR_SQ	ReferencedImageSequence;
		VR_ST	DerivationDescription;
		VR_SQ	SourceImageSequence;
		VR_IS	ImagesInAcquisition;
		VR_LT	ImageComments;
		VR_CS	LossyImageCompression;
	}; // End of class defination for: GeneralImageModule


// C.7.6.2
// Class Module:ImagePlaneModule
//用于MRIMAGE
class	CO_ImagePlaneModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ImagePlaneModule(CCO_Abstract *Composite=NULL);

		VR_DS	PixelSpacing;
		VR_DS	ImageOrientationPatient;
		VR_DS	ImagePositionPatient;
		VR_DS	SliceThickness;
		VR_DS	SliceLocation;
	}; // End of class defination for: ImagePlaneModule

// C.7.6.3
// Class Module:ImagePixelModule
//用于MRIMAGE
class	CO_ImagePixelModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ImagePixelModule(CCO_Abstract *Composite=NULL);

		VR_US	SamplesPerPixel;
		VR_CS	PhotometricInterpretation;
		VR_US	Rows;
		VR_US	Columns;
		VR_US	BitsAllocated;
		VR_US	BitsStored;
		VR_US	HighBit;
		VR_US	PixelRepresentation;
		VR_OX	PixelData;
		VR_US	PlanarConfiguration;
		VR_IS	PixelAspectRatio;
		VR_XS	SmallestImagePixelValue;
		VR_XS	LargestImagePixelValue;
		VR_XS	RedPaletteColorLookupTableDescriptor;
		VR_XS	GreenPaletteColorLookupTableDescriptor;
		VR_XS	BluePaletteColorLookupTableDescriptor;
		VR_XS	RedPaletteColorLookupTableData;
		VR_XS	GreenPaletteColorLookupTableData;
		VR_XS	BluePaletteColorLookupTableData;
	}; // End of class defination for: ImagePixelModule

// C.7.6.4
// Class Module:ContrastBolusModule
//用于MRIMAGE
class	CO_ContrastBolusModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ContrastBolusModule(CCO_Abstract *Composite=NULL);

		VR_LO	ContrastBolusAgent;
		VR_SQ	ContrastBolusAgentSequence;
		VR_LO	ContrastBolusRoute;
		VR_SQ	ContrastBolusAdministrationRouteSequence;
		VR_DS	ContrastBolusVolume;
		VR_TM	ContrastBolusStartTime;
		VR_TM	ContrastBolusStopTime;
		VR_DS	ContrastBolusTotalDose;
		VR_DS	ContrastFlowRates;
		VR_DS	ContrastFlowDurations;
		VR_CS	ContrastBolusIngredient;
		VR_DS	ContrastBolusIngredientConcentration;
	}; // End of class defination for: ContrastBolusModule

// Class Module:ReferencedImageSequence
//用于MRIMAGE
class	CO_ReferencedImageSequence	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_ReferencedImageSequence(CCO_Abstract *Composite=NULL);

		VR_UI	ReferencedSOPClassUID;
		VR_UI	ReferenceSOPInstanceUID;
		VR_AE	RetrieveApplicationEntityTitle;
		VR_SH	StorageMediaFileSetID;
		VR_UI	StorageMediaFileSetUID;
	}; // End of class defination for: ReferencedImageSequence

// C.8.3.1
// Class Module:MRImageModule
//用于MRIMAGE
class	CO_MRImageModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_MRImageModule(CCO_Abstract *Composite=NULL);

		VR_CS	ImageType;
		VR_US	SamplesPerPixel;
		VR_CS	PhotometricInterpretation;
		VR_US	BitsAllocated;
		VR_SQ	ScanningSequence;
		VR_CS	DequenceVariant;
		VR_CS	ScanOptions;
		VR_CS	MRAcquisitionType;
		VR_DS	RepetitionTime;
		VR_DS	EchoTime;
		VR_IS	EchoTrainLength;
		VR_DS	InversionTime;
		VR_DS	TriggerTime;
		VR_SH	SequenceName;
		VR_CS	AngioFlag;
		VR_DS	NumberOfAverages;
		VR_DS	ImagingFrequency;
		VR_SH	ImagedNucleus;
		VR_IS	EchoNumber;
		VR_DS	MagneticFieldStrength;
		VR_DS	SpacingBetweenSlices;
		VR_IS	NumberOfPhaseEncodingSteps;
		VR_DS	PercentSampling;
		VR_DS	PercentPhaseFieldOfView;
		VR_DS	PixelBandwidth;
		VR_IS	NominalInterval;
		VR_CS	BeatRejectionFlag;
		VR_IS	LowRRValue;
		VR_IS	IntervalsAcquired;
		VR_IS	InvervalsRejected;
		VR_LO	PVCRejection;
		VR_IS	SkipBeats;
		VR_IS	HeartRate;
		VR_IS	CardiacNumberOfImages;
		VR_IS	TriggerWindow;
		VR_DS	ReconstructionDiameter;
		VR_SH	ReceivingCoil;
		VR_SH	TransmittingCoil;
		VR_US	AcquisitionMatrix;
		VR_CS	PhaseEncodingDirection;
		VR_DS	FlipAngle;
		VR_DS	SAR;
		VR_CS	VariableFlipAngleFlag;
		VR_DS	dBdt;
		VR_IS	TemporalPositionIdentifier;
		VR_IS	NumberOfTemporalPositions;
		VR_DS	TemporalResolution;
	}; // End of class defination for: MRImageModule
// C.9.2
// Class Module:OverlayPlaneModule
//用于MRIMAGE
class	CO_OverlayPlaneModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_OverlayPlaneModule(CCO_Abstract *Composite=NULL);

		VR_US	Rows;
		VR_US	Columns;
		VR_CS	OverlayType;
		VR_SS	Origin;
		VR_US	BitsAllocated;
		VR_US	BitPosition;
		VR_OW	OverlayData;
		VR_IS	ROIArea;
		VR_DS	ROIMean;
		VR_DS	ROIStandardDeviation;
		VR_US	OverlayDescriptorGray;
		VR_US	OverlayDescriptorRed;
		VR_US	OverlayDescriptorGreen;
		VR_US	OverlayDescriptorBlue;
		VR_US	OverlaysGray;
		VR_US	OverlaysRed;
		VR_US	OverlaysGreen;
		VR_US	OverlaysBlue;
	}; // End of class defination for: OverlayPlaneModule

// C.11.2
// Class Module:VOILUTModule
//用于MRIMAGE
class	CO_VOILUTModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_VOILUTModule(CCO_Abstract *Composite=NULL);

		VR_SQ	VOILUTSequence;
		VR_DS	WindowCenter;
		VR_DS	WindowWidth;
		VR_LO	WindowCenterAndWidthExplanation;
	}; // End of class defination for: VOILUTModule
// C.12.1	
// Class Module:SOPCommonModule
//用于MRIMAGE
class	CO_SOPCommonModule	:
	virtual	public	CO_Abstract
	{
	public:
		BOOL	SetIf ( VR * );
		BOOL	Morph ( DICOMObject * );
		BOOL	Reset ();

		// Constuctor (to set VR group/element tags
		CO_SOPCommonModule(CCO_Abstract *Composite=NULL);

		VR_UI	SOPClassUID;
		VR_UI	SOPInstanceUID;
		VR_CS	SpecificCharacterSet;
		VR_DA	InstanceCreationDate;
		VR_TM	InstanceCreationTime;
		VR_UI	InstanceCreatorUID;
	}; // End of class defination for: SOPCommonModule
#endif
