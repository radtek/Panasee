
#include "cctypes.h"	// Added by ClassView
#define	DEFAULT_ACCEPTANCE	TRUE


// These are Read/Write file types
#	define	ACRNEMA_VR_DUMP				1
#	define	DICOM_CHAPTER_10_IMPLICIT	2
#	define	DICOM_CHAPTER_10_EXPLICIT	3		//DICOM_MEDIA_IMPLICITLITTLEENDIAN
#	define	DICOM_MEDIA_IMPLICITLITTLEENDIAN	3		
#	define	DICOM_MEDIA_EXPLICITLITTLEENDIAN	4
#	define	DICOM_MEDIA_EXPLICITBIGENDIAN	5
#	define	DICOM_MEDIA_RLELOSSLESS	6
#	define	DICOM_MEDIA_JPEGLOSSLESSDEFAULT	70
#	define	DICOM_MEDIA_JPEGLOSSLESS	80
#	define	DICOM_MEDIA_JPEGLS	81
#	define	DICOM_MEDIA_JPEG2K	90


#	define	_ImplicitLittleEndianUID	"1.2.840.10008.1.2"
#	define	_ExplicitLittleEndianUID	"1.2.840.10008.1.2.1"
#	define	_ExplicitBigEndianUID		"1.2.840.10008.1.2.2"
#	define	_RleLosslessUID				"1.2.840.10008.1.2.5"
#	define	_JpgLosslessDefaultUID		"1.2.840.10008.1.2.4.70"
#	define	_JpgLosslessUID				"1.2.840.10008.1.2.4.80"
#	define	_JpgLSUID					"1.2.840.10008.1.2.4.81"
#	define	_Jpg2KUID					"1.2.840.10008.1.2.4.90"

//////////////////////////////////////////////////////////////////////////////////
#	define	PDU_SUCCESS															0
#	define	PDU_SAVE_NOFREEDISKSPACE											1

#	define	PDU_CONNECT_NOINTEROGATEAASSOCIATEAC								2

#	define	PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN							3
#	define	PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED		4
#	define	PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED				5
#	define	PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED				6
#	define	PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED				7
#	define	PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION						8
#	define	PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED						9

#	define	PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN							10
#	define	PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED		11
#	define	PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED				12
#	define	PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED				13
#	define	PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED				14
#	define	PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION						15
#	define	PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED						16

#	define	PDU_CONNECT_CORRUPT													17
//////////////////////////////////////////////////////////////////////////////////

// These are "Transfer Syntax Alias" types.
enum
	{
	TSA_IMPLICIT_LITTLE_ENDIAN = 100,
	TSA_EXPLICIT_LITTLE_ENDIAN,
	TSA_EXPLICIT_BIG_ENDIAN
	};

class	FileBuffer	:
	public	LinkedBuffer
	{
	public:
		FILE	*fp;
		BOOL	bMustClose;
		FileBuffer()
		{
			bMustClose=TRUE;
			fp=NULL;
		}
		~FileBuffer()
		{
			if(fp!=NULL&&bMustClose)
				fclose(fp);
			fp=NULL;
		}
		BOOL	OpenForRead ( char	*filename );
		BOOL	OpenForWrite ( char *filename );
		BOOL	Close ();
		INT		ReadBinary(BYTE *Data, UINT Count);
		BOOL	SendBinary(BYTE *Data, UINT Count);
	};

class	PDU_Service	:
	public	Buffer,			// Transport Buffer
	public	AAssociateRQ,	// Associate Request
	public	AAssociateAC,	// Associate Accept
	public	AAssociateRJ,	// Associate Reject
	public	AReleaseRQ,		// Associate Release
	public	AReleaseRP,		// Associate Release Response
	public	AAbortRQ,		// Associate Abort Request
	public	PDataTF			// P-DATA Service
	{
	protected:
		Array<AbstractSyntax>				ProposedAbstractSyntaxs;
		Array<PresentationContextAccept>	AcceptedPresentationContexts;
		UINT32								SizeCap;
		UID					UsedTransferSyntaxUID;
	protected:
		void			PDUSetLastError(UINT32 nLastError);
		BOOL			InterogateAAssociateRQ();
		BOOL			InterogateAAssociateAC();
		virtual BOOL	GetFreeDiskSpace(char * filename, UINT32 & upper, UINT32 & lower) 
			{upper = 1; lower = 0; return (TRUE); };
		virtual	BOOL	CanYouHandleTransferSyntax(TransferSyntax &);
		virtual	BOOL	ShouldIAcceptRemoteApTitle(BYTE	*)
			{ return ( DEFAULT_ACCEPTANCE ); };
		virtual	BOOL	ShouldIAcceptLocalApTitle(BYTE *)
			{ return ( DEFAULT_ACCEPTANCE ); };
		virtual	BOOL	ShouldIAcceptApplicationContext(ApplicationContext &)
			{ return ( DEFAULT_ACCEPTANCE ); };
		virtual	BOOL	ShouldIAcceptPresentationContext(
							PresentationContext &,
							PresentationContextAccept &)
			{ return ( DEFAULT_ACCEPTANCE ); };
		virtual	BOOL	ShouldIAcceptAbstractSyntax(AbstractSyntax & abslocal)
		{
			return ( DEFAULT_ACCEPTANCE );
		};
		virtual	BOOL	AddTransferSyntaxs(PresentationContext &);
		virtual	BOOL	GetImplementationClass(ImplementationClass &);
		virtual	BOOL	GetImplementationVersion(ImplementationVersion &);

		// Entry Point Functions
		virtual	BOOL	ParseRawVRIntoDCM(BYTE, LinkedBuffer &, DICOMObject *);
		virtual	BYTE	ParseDCMIntoRawVR(DICOMObject *, LinkedBuffer &);
//		virtual	BYTE	ParseDCMIntoRawVR(DICOMObject *, LinkedBuffer &, UID &);
		virtual	BYTE	ParseDCMIntoRawVR(DICOMObject *, LinkedBuffer &, UID &, BOOL bIsDataObject = TRUE);

		// Implicit Encoders
		virtual	BOOL	Implicit_ParseRawVRIntoDCM(
							LinkedBuffer &, DICOMObject *);
		virtual	BOOL	Implicit_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &);
		virtual	BOOL	ImplicitLittleEndian_ParseRawVRIntoDCM(
							LinkedBuffer &, DICOMObject *);
		virtual	BOOL	ImplicitLittleEndian_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &);

		// Explicit Decoders
		virtual	BOOL	Explicit_ParseRawVRIntoDCM(
							LinkedBuffer &, DICOMObject *);
		virtual	BOOL	Explicit_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &);
		virtual	BOOL	ExplicitLittleEndian_ParseRawVRIntoDCM(
							LinkedBuffer &, DICOMObject *);
		virtual	BOOL	ExplicitLittleEndian_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &);
		virtual	BOOL	ExplicitBigEndian_ParseRawVRIntoDCM(
							LinkedBuffer &, DICOMObject *);
		virtual	BOOL	ExplicitBigEndian_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &);

		// Used to Read DICOM Files (all types)
		virtual	BOOL	Dynamic_ParseRawVRIntoDCM
							(LinkedBuffer &, DICOMObject *, UINT StartMode);

		// Used to write Chapter 10 Explcit (new) files
		virtual	int 	Dynamic_ParseDCMIntoRawVR
							(DICOMObject *, LinkedBuffer &, UINT StartMode,int *pnImgNo);
 
		// Image Pixel Data encapsulation routines

		virtual	BOOL	ParseImagePixelDataDCMToRaw
							(Array<DICOMDataObject*>	*, LinkedBuffer &);
		virtual	BOOL	ParseImagePixelDataRawToDCM
							(LinkedBuffer	&, DICOMObject	*);

		// Support Routines for Transfer Syntax Encoding/Decoding
	public:	// The following routine is usefull to check for a valid
			// Accepted Presentation Context UID
				BYTE	GetAcceptedPCIDForTransferSyntax(UID &TrnUID);
				BOOL	GetTransferSyntaxUID(BYTE, UID &TrnUID);
				char*	GetTransferSyntaxUID(UINT);
				BYTE	GetPresentationContextID(UID &AbsUID);
				BYTE	GetPresentationContextID(UID &AbsUID, UID &TrnUID);
				BOOL	IsAbstractSyntaxAccepted(UID &uid);
				void	GetUsedTransferSyntax(UID &uid){ uid = UsedTransferSyntaxUID;};
	public:
		BOOL SwapPixelDataByte(VR*vr);
		//edit by peak
//		static BOOL ParseIVEL(BYTE*pData,int length,Array<DICOMDataObject*>*pDDOArray);
//		static BOOL ParseRawVRToSQArray(VR*pVR);
		//edit
		BOOL bSwap;
		BOOL IsAbstractSyntaxAccepted(UID &ASuid, UID &TSuid);
		UINT32 PDUGetLastError();
		Socket	Link;

		UINT16	RunTimeClass(UINT16 Group,UINT16 Element);
		// Wrap data over from the Associate/PData Classes over to the buffer
				BOOL	Read(BYTE	*Data, UINT	Count)
			{	return ( Buffer :: Read ( Data, Count ) ); };
				BOOL	Write(BYTE	*Data, UINT	Count)
			{	return ( Buffer :: Write ( Data, Count ) ); };
				BOOL	Kill(UINT	Count)
			{	return ( Buffer :: Kill ( Count ) ); };
				BOOL	Flush()
			{	return ( Buffer :: Flush () ); };
		// Wrap data over from the buffer over to the socket
				INT	ReadBinary(BYTE *Data, UINT Count)
			{	return ( Link.ReadBinary(Data, Count) ); };
				BOOL	SendBinary(BYTE *Data, UINT Count)
			{	return ( Link.SendBinary(Data, Count) ); };
		virtual	BOOL	MakeFileMeta(DICOMDataObject *, UINT);

		DICOMDataObject	*LoadDICOMDataObject ( char	* );
		int  	SaveDICOMDataObject ( char	*, UINT, DICOMDataObject * ,int *,BYTE*, int&);
		BOOL	SaveDICOMDataObject ( char	*, UINT, DICOMDataObject* );
		BOOL	ClearAbstractSyntaxs();
		virtual	BOOL	AddAbstractSyntax(UID &);
		virtual	BOOL	AddAbstractSyntaxAlias(UID &Source, UID &Alias);
		virtual	BOOL	SetApplicationContext(UID &);
		virtual	BOOL	SetLocalAddress(BYTE *);
		virtual	BOOL	SetRemoteAddress(BYTE *);
		virtual	BOOL	GetLocalAddress(BYTE *);
		virtual	BOOL	GetRemoteAddress(BYTE *);
		BOOL	Connect(BYTE *, BYTE *);
		
		BOOL	Close();

		BOOL	Listen(BYTE *);

		BOOL	Read(DICOMObject	*);
		BOOL	Write(DICOMCommandObject	*);
		BOOL	Write(DICOMDataObject	*);
		BOOL	Write(DICOMCommandObject	*, UID	&);
		BOOL	Write(DICOMDataObject	*, UID	&);
		BOOL	Multiplex(int);

		int		Multiplex(BYTE *);
		int		Listen(int);
		BOOL	SetTimeOut(int TimeOut)
			{ return ( Link.SetTimeOut(TimeOut) ); };
				PDU_Service();
		virtual	~PDU_Service();
	private:
		UINT32 LastError;
	};
