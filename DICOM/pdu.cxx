
#	include	"dicom.hpp"
extern RTC GlobalRTC;
//construction
PDU_Service	::	PDU_Service()
	{
// PDU Level services are always big endian architecture

//Endian Incoming 
	Buffer :: SetIncomingEndian(BIG_ENDIAN);
//Endian Outgoing 
	Buffer :: SetOutgoingEndian(BIG_ENDIAN);
	LastError = PDU_SUCCESS;
	bSwap=FALSE;
 }

//Destruction
PDU_Service	::	~PDU_Service()
	{
	if(Link.Connected)
		Close();
//clear AbstractSyntaxs
	ClearAbstractSyntaxs();

//Clear PresentationContexts
	while ( AcceptedPresentationContexts.GetSize())
		AcceptedPresentationContexts.RemoveAt ( 0 );

}

UINT16 
PDU_Service::RunTimeClass(UINT16 Group,UINT16 Element)
{	
	UINT16 tc=0;
//Query Data dictionary
	tc=GlobalRTC.RunTimeClass(Group,Element);
	if(tc==0)
	{
/*
The VR for Data Elements, Item (FFFE,E000), Item Delimitation Item (FFFE,E00D), 
and Sequence Delimitation Item (FFFE,E0DD) do not exist.  
See Part 5 of the DICOM Standard for explanation
*/
		if(Group==0xFFFE)
			tc=0;
//Unknown
		else
			tc='UN';
	}
	return tc;
}
BOOL	PDU_Service	::	Read(DICOMObject	*DCMObject)
	{
	BYTE		ItemType;

	if (!Link.Connected)
		return ( FALSE );
	PDataTF :: MsgStatus = 0;

	if((PDataTF :: Length)!=0)	// for bad GCC v2.6.0
		{
		PDataTF :: ReadDynamic( *this );

		if (PDataTF :: MsgStatus > 0)
			{
			SizeCap = 0;
			if(!ParseRawVRIntoDCM(PDataTF :: PresentationContextID,
				PDataTF::VRBuffer, DCMObject))
				{
				AAbortRQ :: Write(*this);
				Link.Close();
				return ( FALSE );
				}
			return(TRUE);
			}
		}
	
	while ( TRUE )
		{
		(*this) >> ItemType;
		switch ( ItemType )
			{
			case	0x01:	// A-ASSOCIATE-RQ Should not get this here
				AAssociateRQ :: Read(*this);
				AAbortRQ :: Write ( *this );
				Link.Close();

				return ( FALSE );
			case	0x02:	// A-ASSOCIATE-AC Should not get this here
				AAssociateAC :: Read(*this);
				AAbortRQ :: Write ( *this );
				Link.Close();

				return ( FALSE );
			case	0x03:	// A-ASSOCIATE-RJ Should not get this here
				AAbortRQ :: Write ( *this );
				Link.Close();

				return ( FALSE );
			case	0x04:	// P-DATA-TF
				try
				{
//				fprintf(stderr, "start: P-DATA-TF Read\n");
				PDataTF :: ReadDynamic( *this );
//				fprintf(stderr, "end: P-DATA-TF Read : %d, Buffer Size %d\n",
//					PDataTF :: MsgStatus, Buffer :: GetSize());
				}
				catch(...)
				{	
					AAbortRQ :: Write(*this);
					Link.Close();
					return ( FALSE );
				}

				try
				{
				if (PDataTF :: MsgStatus > 0)
					{
					SizeCap = 0;
					if(!ParseRawVRIntoDCM(PDataTF :: PresentationContextID,
						PDataTF::VRBuffer, DCMObject))
						{
						AAbortRQ :: Write(*this);
						Link.Close();
						return ( FALSE );
						}
					return(TRUE);
					}
				}
				catch(...)
				{	
					AAbortRQ :: Write(*this);
					Link.Close();
					return ( FALSE );
				}

				break;
			case	0x05:	// A-RELEASE-RQ
				AReleaseRQ :: ReadDynamic(*this);
				// also drop
				AReleaseRP :: Write(*this);

				return ( FALSE );
			case	0x06:	// A-RELEASE-RP
				Link.Close();
				return ( FALSE );
			case	0x07:	// A-ABORT-RQ
				Link.Close();
				return ( FALSE );
			default:
				// Protocol Error
				//[peak
			//	continue;
			/*	AReleaseRQ :: Write(*this);
				// also drop
				AReleaseRP :: Read(*this);
				return ( FALSE );
				//]
			*/	AAbortRQ :: Write ( *this );
				Link.Close();

				return ( FALSE );
			}
		}
	return ( FALSE );
	}

// These routines are used seldom
BOOL	PDU_Service	::	Write(DICOMCommandObject	*DCMObject)
	{
	BYTE	TempByte;

	TempByte = ParseDCMIntoRawVR(DCMObject, PDataTF :: VRBuffer);
	if ( ! TempByte )
		return ( FALSE );
	//[peak
//	PDataTF :: PresentationContextID = TempByte;//here a error?
	//]
	PDataTF :: MsgHeader = 0x01;
	return(PDataTF :: Write ( *this ));
	}

BOOL	PDU_Service	::	Write(DICOMDataObject	*DCMObject)
	{
	BYTE	TempByte;

	TempByte = ParseDCMIntoRawVR(DCMObject, PDataTF :: VRBuffer);
	if ( ! TempByte )
		return ( FALSE );
	//[peak
	//PDataTF :: PresentationContextID = TempByte;
	//]
	PDataTF :: MsgHeader = 0x00;

	return(PDataTF :: Write ( *this ));
	}

// These routines are the recommended method of sending a DICOM Object

BOOL	PDU_Service	::	Write(DICOMCommandObject	*DCMObject, UID	&uid)
	{
	BYTE	TempByte, TempByte1;

	TempByte1 = GetPresentationContextID(uid);
	if(!TempByte1)
		return ( FALSE );

	TempByte = ParseDCMIntoRawVR(DCMObject, PDataTF :: VRBuffer, uid,FALSE);
	if ( ! TempByte )
		return ( FALSE );
	//[peak
	//PDataTF :: PresentationContextID = TempByte1;
	//]
	//[peak
	PDataTF :: PresentationContextID = TempByte;
	//]
	PDataTF :: MsgHeader = 0x01;
	return(PDataTF :: Write ( *this ));
	}

BOOL	PDU_Service	::	Write(DICOMDataObject	*DCMObject, UID	&uid)
	{
	BYTE	TempByte, TempByte1;

	TempByte1 = GetPresentationContextID(uid);
	if(!TempByte1)
		return ( FALSE );

	TempByte = ParseDCMIntoRawVR(DCMObject, PDataTF :: VRBuffer, uid,TRUE);
	if ( ! TempByte )
		return ( FALSE );
	//[peak
	//PDataTF :: PresentationContextID = TempByte1;
	//]
	//[peak
	PDataTF :: PresentationContextID = TempByte;
	//]
	PDataTF :: MsgHeader = 0x00;

	return(PDataTF :: Write ( *this ));
	}


BOOL	PDU_Service	::	InterogateAAssociateRQ()
	{
	UINT	Index, IndexTrn,ValidPresContexts;
	TransferSyntax				TrnSyntax;
	PresentationContext			PresContext;
	PresentationContextAccept	PresContextAccept;
	UserInformation				UserInfo;
	MaximumSubLength			MaxSubLength;
	Array<PresentationContext>	PCArray ( FALSE );


	if(!ShouldIAcceptRemoteApTitle(AAssociateRQ :: CallingApTitle))
		{
		AAssociateRJ :: Reason = 3;	// Calling party not rec.
		AAssociateRJ :: Source = 1;	// PDU
		AAssociateRJ :: Result = 1;	// Permanent
		AAssociateRJ :: Write ( *this );
		return ( FALSE );
		}
	if(!ShouldIAcceptLocalApTitle(AAssociateRQ :: CalledApTitle))
		{
		AAssociateRJ :: Reason = 7;	// Called Party Not Rec.
		AAssociateRJ :: Source = 1;	// PDU
		AAssociateRJ :: Result = 1;	// Permanent
		AAssociateRJ :: Write ( *this );
		return ( FALSE );
		}
	if(!ShouldIAcceptApplicationContext(AAssociateRQ :: AppContext))
		{
		AAssociateRJ :: Reason = 2;	// AppContext Not-Supported
		AAssociateRJ :: Source = 1;	// PDU
		AAssociateRJ :: Result = 1;	// Permanent
		AAssociateRJ :: Write ( *this );
		return ( FALSE );
		}
	// Transfer the information over to the A-ASSOCIATE-AC Class
	AAssociateAC :: SetCalledApTitle(AAssociateRQ :: CalledApTitle);
	AAssociateAC :: SetCallingApTitle(AAssociateRQ :: CallingApTitle);
	AAssociateAC :: AppContext = AAssociateRQ :: AppContext;
	AAssociateAC :: UserInfo = AAssociateRQ :: UserInfo;

	// Ok, now, we need to find an acceptable presentation context
	Index = 0;
	ValidPresContexts = 0;
	PCArray = (AAssociateRQ :: PresContexts);

	while ( AcceptedPresentationContexts.GetSize())
		AcceptedPresentationContexts.RemoveAt ( 0 );
	/* Check all Presentation Contexts proposed by the other side, e.g.:
		CTStorage			1.2.840.10008.5.1.4.1.1.2	sop
		USMultiframeStorage		1.2.840.10008.5.1.4.1.1.3.1	sop
		MRStorage			1.2.840.10008.5.1.4.1.1.4	sop
		USStorage			1.2.840.10008.5.1.4.1.1.6.1	sop
		SCStorage			1.2.840.10008.5.1.4.1.1.7	sop
	*/
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		IndexTrn = 0;
		PresContextAccept.PresentationContextID =
			PresContext.PresentationContextID;
		/* Fix: default to (Transfer Syntaxes Not Supported) */
		PresContextAccept.Result = 4;
		/* For each Presentation Context, multiple TransferTyntaxes may be proposed, e.g:
			LittleEndianImplicit		1.2.840.10008.1.2
			LittleEndianExplicit		1.2.840.10008.1.2.1
			BigEndianExplicit		1.2.840.10008.1.2.2
			JPEGLossless			1.2.840.10008.1.2.4.70
			RLELossless			1.2.840.10008.1.2.5
			JPEG2000			1.2.840.10008.1.2.4.91
		*/
		while ( IndexTrn < PresContext.TrnSyntax.GetSize())
			{
			TrnSyntax = PresContext.TrnSyntax.Get(IndexTrn);
			/* Following routine could better be named 'CanI...' */
			if(CanYouHandleTransferSyntax(TrnSyntax))
				{
				PresContextAccept.TrnSyntax.TransferSyntaxName =
					TrnSyntax.TransferSyntaxName;
				/* OK: Accept the Transfer Syntax */
				PresContextAccept.Result = 0;
				AcceptedPresentationContexts.Add ( PresContextAccept );
				++ValidPresContexts;
				break;
				}
			++IndexTrn;
			}
		/* Do we support the offered Presentation Context? */
		if(!ShouldIAcceptAbstractSyntax(PresContext.AbsSyntax))
			PresContextAccept.Result = 3;
		AAssociateAC :: AddPresentationContextAccept(PresContextAccept);
		++Index;
		}
	if(!ValidPresContexts)
		{
		AAssociateAC :: Write ( *this );
		return ( FALSE );
		}
	MaxSubLength.Set(16384);	// we can do all DICOM can handle
	GetImplementationClass(UserInfo.ImpClass);
	GetImplementationVersion(UserInfo.ImpVersion);
	UserInfo.SetMax(MaxSubLength);

	AAssociateAC :: SetUserInformation ( UserInfo );
	AAssociateAC :: Write ( *this );

	return ( TRUE );	// yes, the communication should work out
	}

BOOL	PDU_Service	::	InterogateAAssociateAC()
	{
	UINT	Index/*, IndexTrn*/,ValidPresContexts;
//	BOOL	Good;
//	TransferSyntax				TrnSyntax;
//	PresentationContext			PresContext;
	PresentationContextAccept	PresContextAccept;
	Array<PresentationContextAccept>	PCArray ( FALSE );

	Index = 0;
	ValidPresContexts = 0;
	PCArray = (AAssociateAC :: PresContextAccepts);

	while ( AcceptedPresentationContexts.GetSize())
		AcceptedPresentationContexts.RemoveAt ( 0 );

	while ( Index < PCArray.GetSize())
		{
		PresContextAccept = PCArray.Get ( Index );
		if(!PresContextAccept.Result)
			{
//			fprintf(stderr, "Remote Accepting Abstract Class\n");
			AcceptedPresentationContexts.Add ( PresContextAccept );
			}
//		else
//			fprintf(stderr, "Remote Rejecting Abstract Class [%d]\n",
//				PresContextAccept.Result);

		++Index;
		}
	if(!AcceptedPresentationContexts.GetSize())
		return ( FALSE );	// no, this association should be terminated
	//[peak add
	//PDataTF::PresentationContextID=AcceptedPresentationContexts.Get(0).PresentationContextID;
	//]
	return ( TRUE );	// yes, the communication should work out
	}


// Checks the Known Class list for the UID and further verifies that
// it's a big/little Endian.  If it is, then it sets the Transfer type
// in the TransferSyntax for further reference
BOOL	PDU_Service	::	CanYouHandleTransferSyntax(TransferSyntax &TrnSyntax)
	{
	/*
	KnownClass	KC;

	if(KnownUID(TrnSyntax.TransferSyntaxName, KC))
		return ( FALSE );
	
	if(KC.Type != "transfer")
		return ( FALSE );

	if(KC.Comment != "LittleEndian")
		{
		if(KC.Command != "BigEndian")
			return ( FALSE );
		else
			TrnSyntax.SetType ( BIG_ENDIAN );
		}
	else
			TrnSyntax.SetType ( LITTLE_ENDIAN );
	*/
	if(!strcmp((char*) TrnSyntax.TransferSyntaxName.GetBuffer(1), "1.2.840.10008.1.2"))
		{
		TrnSyntax.SetType ( LITTLE_ENDIAN );
		return ( TRUE );
		}
/*	else if(!strcmp((char*) TrnSyntax.TransferSyntaxName.GetBuffer(1), "1.2.840.10008.1.2.1"))
		{
		TrnSyntax.SetType ( LITTLE_ENDIAN );
		return ( TRUE );
		}
*/	return ( FALSE );
	}

BOOL	PDU_Service	::	Listen(BYTE	*port)
	{
	PDataTF :: Length = 0;
	
	if (Link.Connected)
		Link.Close();

	if(!Link.Listen((char *) port))
		return ( FALSE );

	if(!Link.Accept())
		return ( FALSE );

//	fprintf(stderr, "Waiting for AAssociateRQ\n");
	AAssociateRQ :: Read( *this );
//	fprintf(stderr, "Interogating AAssociateRQ\n");
	if(!InterogateAAssociateRQ())
		{
//		fprintf(stderr, "failed to connect\n");
		Link.Close();
		return ( FALSE );
		}
//	fprintf(stderr, "connected, but not disconnecting (missing P-DATA service)\n");
//	AAbortRQ :: Write ( *this );

//	return ( FALSE );
	return ( TRUE );
	}

int	PDU_Service	::	Multiplex(BYTE	*port)
	{
	int	TempSocket;
	
	PDataTF :: Length = 0;

	if(!Link.Listen((char*)port))
		return(-1);

	TempSocket = Link.ListenSocketfd;
	Link.ListenSocketfd = 0;
	Link.Listened = FALSE;
	return (Listen(TempSocket));
	}

int	PDU_Service	::	Listen(int	sock)
	{
	int	TempSocket;

	if(Link.Connected)
		Link.Close();
	Link.ListenSocketfd = sock;
	Link.Listened =  TRUE;

	while (!Link.Connected)
		{	
		if(!Link.Accept())
			return ( FALSE );
	
		AAssociateRQ :: Read( *this );
		if(!InterogateAAssociateRQ())
			{
			TempSocket = Link.ListenSocketfd;
			Link.Listened = FALSE;
			Link.Close();
			Link.Listened = TRUE;
			Link.ListenSocketfd = TempSocket;
			}
		}
	TempSocket = Link.ListenSocketfd;
	Link.ListenSocketfd = 0;
	Link.Listened = 0;
	return ( TempSocket );	
	}

BOOL	PDU_Service	::	Multiplex(int	Socketfd)
	{
	if(Link.Connected)
		Link.Close();
	Link.Connected = TRUE;
	Link.Socketfd = Socketfd;

	AAssociateRQ :: Read( *this );
	if(!InterogateAAssociateRQ())
		{
		Link.Close();
		return(FALSE);
		}
	return (TRUE);
	}	

BOOL	PDU_Service	::	Close()
	{
	PDataTF :: Length = 0;
	if ( Link.Connected )
		{
		AReleaseRQ :: Write ( *this );
		AReleaseRP :: Read ( *this );
		}
	Link.Close();
	return ( TRUE );
	}

// Support routine to get Presentation Context ID from UID

BYTE	PDU_Service	::	GetAcceptedPCIDForTransferSyntax(UID	&uid)
	{
	UINT	Index;
	PresentationContextAccept	PCA;

	Index = 0;
	while (Index < AcceptedPresentationContexts.GetSize() )
		{
		PCA = AcceptedPresentationContexts.Get ( Index );
/*		fprintf(stderr, "Comparing: %s with %s [%d:%d]\n",
			PCA.TrnSyntax.TransferSyntaxName.GetBuffer(1),
			uid.GetBuffer(1),
			PCA.TrnSyntax.TransferSyntaxName.GetSize(),
			uid.GetSize());*/
		if(PCA.TrnSyntax.TransferSyntaxName == uid)
			return ( PCA.PresentationContextID );	
		++Index;
		}
	return ( 0 );
	}

BYTE	PDU_Service	::	GetPresentationContextID(UID	&uid)
	{
	UINT	Index;
	Array <PresentationContext>	PCArray ( FALSE );
	PresentationContext		PresContext;

	PCArray = (AAssociateRQ :: PresContexts);

	Index = 0;
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		if(PresContext.AbsSyntax.AbstractSyntaxName == uid)
			return ( PresContext.PresentationContextID );
		++Index;
		}

	return ( 0 );
	}

// Get the PCID for a given AbsUID and TrnUID
/* below is lin's code,now commented
BYTE	PDU_Service	::	GetPresentationContextID(UID	&AbsUID, UID	&TrnUID)
	{
	UINT	Index;
	Array <PresentationContext>	PCArray ( FALSE );
	PresentationContext			PresContext;

	PCArray = (AAssociateRQ :: PresContexts);

	Index = 0;
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		if(PresContext.AbsSyntax.AbstractSyntaxName == AbsUID)
			{
			UINT	Index;
			PresentationContextAccept	PCA;
			Index = 0;
			while (Index < AcceptedPresentationContexts.GetSize() )
				{
				PCA = AcceptedPresentationContexts.Get ( Index );
				if(PCA.TrnSyntax.TransferSyntaxName == TrnUID)
					return ( PCA.PresentationContextID );
				++Index;
				}
			return ( 0 );
			}
		++Index;
		}

	return ( 0 );
	}
*/
BYTE	PDU_Service	::	GetPresentationContextID(UID	&AbsUID, UID	&TrnUID)
	{
	UINT	Index;
	Array <PresentationContext>	PCArray ( FALSE );
	PresentationContext			PresContext;

	PCArray = (AAssociateRQ :: PresContexts);

	Index = 0;
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		if(PresContext.AbsSyntax.AbstractSyntaxName == AbsUID)
			{
			UINT	Index;
			PresentationContextAccept	PCA;
			Index = 0;
			while (Index < AcceptedPresentationContexts.GetSize() )
				{
				PCA = AcceptedPresentationContexts.Get ( Index );
				if((PCA.TrnSyntax.TransferSyntaxName == TrnUID) &&
					(PCA.PresentationContextID == PresContext.PresentationContextID))
					{
					return ( PCA.PresentationContextID );
					}
				++Index;
				}
			}
		++Index;
		}

	return ( 0 );
	}
BOOL	PDU_Service	::	GetTransferSyntaxUID(BYTE	PCID, UID	&uid)
	{
	UINT	Index;
	PresentationContextAccept	PCA;

	Index = 0;
	while (Index < AcceptedPresentationContexts.GetSize() )
		{
		PCA = AcceptedPresentationContexts.Get ( Index );
		if(PCA.PresentationContextID == PCID)
			{
			uid = PCA.TrnSyntax.TransferSyntaxName;
			return ( TRUE );
			}
		++Index;
		}
	return ( FALSE );
	}
BOOL	PDU_Service	::	IsAbstractSyntaxAccepted(UID &uid)
{
	BYTE	id;
	UINT	Index;
	PresentationContextAccept	PCA;


	id = GetPresentationContextID(uid);
	if (!id)
		return FALSE;
	Index = 0;
	while (Index < AcceptedPresentationContexts.GetSize() )
		{
		PCA = AcceptedPresentationContexts.Get ( Index );
/*		fprintf(stderr, "Comparing: %s with %s [%d:%d]\n",
			PCA.TrnSyntax.TransferSyntaxName.GetBuffer(1),
			uid.GetBuffer(1),
			PCA.TrnSyntax.TransferSyntaxName.GetSize(),
			uid.GetSize());
*/
		if(PCA.PresentationContextID == id)
			return TRUE;
		++Index;
		}
	return FALSE;
}

BOOL	PDU_Service	::	ClearAbstractSyntaxs ()
	{
	while (ProposedAbstractSyntaxs.GetSize())
		ProposedAbstractSyntaxs.RemoveAt ( 0 );
	return ( TRUE );
	}

BOOL	PDU_Service	::	AddAbstractSyntax(UID &uid)
	{
	UINT			Index;
	AbstractSyntax	AbsSyntax;

	Index = 0;
	while ( Index < ProposedAbstractSyntaxs.GetSize() )
		{
		if ( uid ==
			ProposedAbstractSyntaxs.Get ( Index ).AbstractSyntaxName)
			return ( TRUE );	// already in array
		++Index;
		}
	AbsSyntax.Set(uid);
	ProposedAbstractSyntaxs.Add ( AbsSyntax );
	return ( TRUE );
	}
/*below is lin's code
BOOL	PDU_Service	::	AddAbstractSyntaxAlias (
	UID		&UIDSource,
	UID		&UIDAlias)
	{
	UINT	Index;
	Array <PresentationContext>	PCArray ( FALSE );
	PresentationContext			PresContext;

	// First check to see if it's already aliased
	if ( GetPresentationContextID ( UIDAlias ) )
		return ( TRUE );

	PCArray = (AAssociateRQ :: PresContexts);

	Index = 0;
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		if(PresContext.AbsSyntax.AbstractSyntaxName == UIDSource)
			{
			PresContext.AbsSyntax.AbstractSyntaxName = UIDAlias;
			AAssociateRQ :: AddPresentationContext(PresContext);
			return ( TRUE );
			}
		++Index;
		}

	// The source was not proposed

	return ( FALSE );
	}
*/
BOOL	PDU_Service	::	AddAbstractSyntaxAlias (
	UID		&UIDSource,
	UID		&UIDAlias)
	{
	UINT	Index;
	Array <PresentationContext>	PCArray ( FALSE );
	PresentationContext			PresContext;

	// First check to see if it's already aliased
	if ( GetPresentationContextID ( UIDAlias ) )
		return ( TRUE );

	PCArray = (AAssociateRQ :: PresContexts);

	Index = 0;
	while ( Index < PCArray.GetSize())
		{
		PresContext = PCArray.Get ( Index );
		PresContext.TrnSyntax.ClearType = FALSE;
		if(PresContext.AbsSyntax.AbstractSyntaxName == UIDSource)
			{
			PresentationContext			vPresContext = PresContext;
		
			vPresContext.TrnSyntax.ClearType = FALSE;
				
			PresContext.TrnSyntax.Clear();
			UINT	SubIndex = 0;
			while (SubIndex < vPresContext.TrnSyntax.GetSize() )
				{
				PresContext.TrnSyntax.Add(vPresContext.TrnSyntax.Get(SubIndex));
				++SubIndex;
				}
			PresContext.TrnSyntax.ClearType = TRUE;
			PresContext.AbsSyntax.AbstractSyntaxName = UIDAlias;
			AAssociateRQ :: AddPresentationContext(PresContext);
			PresContext.TrnSyntax.ClearType = FALSE;
			return ( TRUE );
			}
		++Index;
		}

	// The source was not proposed

	return ( FALSE );
	}
BOOL	PDU_Service	::	SetApplicationContext(UID &uid)
	{
	AAssociateRQ :: SetApplicationContext(uid);
	return ( TRUE );
	}

BOOL	PDU_Service	::	SetLocalAddress(BYTE *address)
	{
	AAssociateRQ :: SetCallingApTitle(address);
	return ( TRUE );
	}

BOOL	PDU_Service	::	SetRemoteAddress(BYTE *address)
	{
	AAssociateRQ :: SetCalledApTitle(address);
	return ( TRUE );
	}
BOOL	PDU_Service	::	GetLocalAddress(BYTE *address)
	{
	memset(address, 0, 17); memcpy(address, AAssociateAC :: CallingApTitle, 16);
	return ( TRUE );
	}

BOOL	PDU_Service	::	GetRemoteAddress(BYTE *address)
	{
	memset(address, 0, 17); memcpy(address, AAssociateAC :: CalledApTitle, 16);
	return ( TRUE );
	}

BOOL	PDU_Service	::	AddTransferSyntaxs(PresentationContext &PresContext)
	{
	UID		uid;
	TransferSyntax	TrnSyntax;
//	char		szAbstractSyntax[100];

/*
	uid = PresContext.AbsSyntax.AbstractSyntaxName;
	strncpy(szAbstractSyntax, (char*)uid.GetBuffer(0), uid.GetSize());
	szAbstractSyntax[uid.GetSize()] = 0;
	if ((strcmp(szAbstractSyntax, "1.2.840.10008.1.1") != 0) &&	// c-echo
	    (strstr(szAbstractSyntax, ".5.1.4.1.2.") == 0))		// c-find
	{
		uid.Set("1.2.840.10008.1.2.4.70");	// LJ LosslessJPEG
		TrnSyntax.Set(uid);
		PresContext.AddTransferSyntax(TrnSyntax);
	}
*/
	uid.Set("1.2.840.10008.1.2");		// LJ ImplicitLittleEndian
	TrnSyntax.Set(uid);
	PresContext.AddTransferSyntax(TrnSyntax);
//	uid.Set("1.2.840.10008.1.2.1");		// LJ ExplicitLittleEndian
//	TrnSyntax.Set(uid);
//	PresContext.AddTransferSyntax(TrnSyntax);
	return ( TRUE );
	}


BOOL	PDU_Service	::	GetImplementationClass(ImplementationClass &ImpClass)
	{
	ImpClass.Set((BYTE*) IMPLEMENTATION_CLASS_STRING);
	return(TRUE);
	}

BOOL	PDU_Service	::	GetImplementationVersion(ImplementationVersion &ImpVersion)
	{
	ImpVersion.Set((BYTE*) IMPLEMENTATION_VERSION_STRING);
	return ( TRUE );
	}

BOOL	PDU_Service	::	Connect ( BYTE *ip, BYTE *port)
	{
	UINT					Index;
	PresentationContext		PresContext;
	UserInformation			UserInfo;
	MaximumSubLength		MaxSubLength;
	BYTE					ItemType;
	PDataTF :: Length = 0;

	if (!ProposedAbstractSyntaxs.GetSize () )
		return ( FALSE );		// must proposed atleast one SOP class
	AAssociateRQ :: ClearPresentationContexts();
	Index = 0;
	//AddTransferSyntaxs(PresContext);
	while ( Index < ProposedAbstractSyntaxs.GetSize())
		{
/*		//origninal code
		//PresContext.TrnSyntax.Clear();
		//changed to following by Lin
		PresContext.Reset();
*/
//		PresContext.TrnSyntax.Clear();
		PresContext.TrnSyntax.RemoveAll();//[PEAK]
		PresContext.SetAbstractSyntax ( ProposedAbstractSyntaxs.Get ( Index ) );
		AddTransferSyntaxs(PresContext);
		PresContext.PresentationContextID = uniq8odd();//[PEAK] 
		AAssociateRQ :: AddPresentationContext ( PresContext );
		++Index;
		}
	PresContext.TrnSyntax.ClearType = FALSE;
	MaxSubLength.Set(16384);	// we can do all DICOM can handle
	GetImplementationClass(UserInfo.ImpClass);
	GetImplementationVersion(UserInfo.ImpVersion);
	UserInfo.SetMax(MaxSubLength);

	AAssociateRQ :: SetUserInformation ( UserInfo );

	// ok, every thing proposed, now try and contact the remote end
	if ( ! Link.Open ( (char*) ip, (char*)port ) )
		return ( FALSE );	// Socket level Error

	AAssociateRQ :: Size();
	AAssociateRQ :: Write ( *this );
	
	(*this) >> ItemType;
//	fprintf(stderr, "Got Response Type: %d\n", ItemType);
//[peak
//	char temp[256];
//]
	switch ( ItemType )
		{
		case	0x02:
			AAssociateAC :: ReadDynamic ( * this );
			if(!InterogateAAssociateAC())
				{
				Link.Close();
				PDUSetLastError(PDU_CONNECT_NOINTEROGATEAASSOCIATEAC);
				return ( FALSE );
				}
			return ( TRUE );	// this is what we want to happen
		case	0x03:
			AAssociateRJ :: ReadDynamic ( * this );
			Link.Close();
			switch(AAssociateRJ::Result) {
			case 0x01:
				switch(AAssociateRJ::Source) {
				case 0x01:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_APPLICATIONCONTEXNAMENOTSUPPORTED);
						break;
					case 0x03:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_CALLINGAETITLENOTRECOGNIZED);
						break;
					case 0x07:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_CALLEDAETITLENOTRECOGNIZED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				case 0x02:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_NOREASONGIVEN);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_PROTOCOLVERSIONNOTSUPPORTED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				case 0x03:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_TEMPORARYCONGESTION);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTPERMANENT_LOCALLIMITEXCEED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
					PDUSetLastError(PDU_CONNECT_CORRUPT);
					break;
				}
				break;
			case 0x02:
				switch(AAssociateRJ::Source) {
				case 0x01:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_APPLICATIONCONTEXNAMENOTSUPPORTED);
						break;
					case 0x03:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_CALLINGAETITLENOTRECOGNIZED);
						break;
					case 0x07:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_CALLEDAETITLENOTRECOGNIZED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				case 0x02:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_NOREASONGIVEN);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_PROTOCOLVERSIONNOTSUPPORTED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				case 0x03:
					switch(AAssociateRJ::Reason) {
					case 0x01:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_TEMPORARYCONGESTION);
						break;
					case 0x02:
						PDUSetLastError(PDU_CONNECT_REJECTTRANSIENT_LOCALLIMITEXCEED);
						break;
					default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
						PDUSetLastError(PDU_CONNECT_CORRUPT);
						break;
					}
					break;
				default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
					PDUSetLastError(PDU_CONNECT_CORRUPT);
					break;
				}
				break;
			default:
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
				PDUSetLastError(PDU_CONNECT_CORRUPT);
				break;
			}
			return ( FALSE );
		default:
			// corrupt transmission
			Link.Close();
			PDUSetLastError(PDU_CONNECT_CORRUPT);
						//[peak
						//	sprintf(temp,"%d;%d;%d",AAssociateRJ::Result,AAssociateRJ::Source,AAssociateRJ::Reason);
						//	MessageBox(NULL,temp,"DICOM ERROR",MB_OK);
						//]
			return ( FALSE );
		}
	
	return ( FALSE );
	}

void PDU_Service::PDUSetLastError(UINT32 nLastError)
{
	LastError = nLastError;
}

UINT32 PDU_Service::PDUGetLastError()
{
	UINT32 nLastError = LastError;
	LastError = PDU_SUCCESS;
	return nLastError;
}
/*
BOOL PDU_Service::ParseIVEL(BYTE*pData,int length,Array<DICOMDataObject*>*pDDOArray)
{
	int *pInt;
	WORD*pWd;
	BYTE*pByte=pData;
	int tmp;
	while((pByte-pData)<length)
	{
		pInt=(int*)pByte;
		if(*pInt==0xE000FFFE)
		{
			DICOMDataObject* pDDO=new DICOMDataObject;
			pDDOArray->Add(pDDO);
			pInt++;
			ParseIVEL(pByte+8,*pInt,pDDOArray);
			pByte+=*pInt+8;
		}
		else
		{
			tmp=pDDOArray->GetSize();
			if(tmp==0)
				return FALSE;
			pWd=(WORD*)pInt;
			tmp=*(pInt+1);
			BYTE*pBuf=new BYTE[tmp];
			memcpy(pBuf,pByte+8,tmp);
			VR*pVR=new VR(*pWd,*(pWd+1),tmp,pBuf);
			pDDOArray->Get(pDDOArray->GetSize()-1)->Push(pVR);
			pByte+=tmp+8;
		}
	}
	return TRUE;
}
		
BOOL PDU_Service::ParseRawVRToSQArray(VR*pVR)//Implicit VR Explicit Length
{
	if(pVR->SQObjectArray!=NULL)
		return TRUE;
	if(pVR->Data==NULL)
		return FALSE;
	pVR->TypeCode='SQ';
	Array<DICOMDataObject*>*pArray=new Array<DICOMDataObject*>;
	pVR->SQObjectArray=pArray;
	ParseIVEL((BYTE*)pVR->Data,pVR->Length,pArray);
	if(pVR->ReleaseMemory)
	{
		delete pVR->Data;
	}
	pVR->Data=NULL;
	return TRUE;
}
*/
BOOL PDU_Service::IsAbstractSyntaxAccepted(UID &ASuid, UID &TSuid)
{
	UINT	Index;
	PresentationContextAccept	PCA;

	BYTE btPCID = GetPresentationContextID(ASuid);

	Index = 0;
	while (Index < AcceptedPresentationContexts.GetSize() )
		{
		PCA = AcceptedPresentationContexts.Get ( Index );
		if(PCA.PresentationContextID == btPCID) {
			TSuid = PCA.TrnSyntax.TransferSyntaxName;
			return ( TRUE );
		}	
		++Index;
		}
	return ( FALSE );
}
