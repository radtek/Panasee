
#	include	"dicom.hpp"

/************************* C-Echo *******************************/
//ECHOÇëÇó¶Á
BOOL	CEchoRQ :: Read ( DICOMCommandObject *DCO )
	{
	if (!DCO)
		return ( FALSE );

	if (DCO->GetUINT16(0x0000, 0x0100)!=0x0030)
		return ( FALSE );	// not a C-Echo-RQ

	return ( TRUE );
	}
//ECHOÇëÇóÐ´
BOOL	CEchoRQ :: Write ( PDU_Service *PDU )
	{
	DICOMCommandObject	DCO;
	VR					*vr;
	UINT16				Temp;
	LE_UINT16			leCommand, leDSType, leMessageID;
	UID					uid;

	if (!PDU)
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );
	vr = new VR (0x0000, 0x0002, uid.GetSize(),
			(void*)uid.GetBuffer(1), FALSE );
	DCO.Push ( vr );
	
	leCommand = 0x0030;
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand,
			FALSE);
	DCO.Push ( vr );

	leDSType = 0x0101;
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType,
			FALSE);
	DCO.Push ( vr );

	Temp = uniq16odd();
	leMessageID = Temp;
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID,
			FALSE);
	DCO.Push ( vr );

	if (!PDU->Write(&DCO, uid))
		return ( FALSE );

	return ( TRUE );
	}
//ECHO·´Ó³¶Á
BOOL	CEchoRSP :: Read ( DICOMCommandObject *DCO )
	{
	if ( ! DCO )
		return ( FALSE );

	if (DCO->GetUINT16(0x0000, 0x0100)!=0x8030)
		return ( FALSE );

	return ( TRUE );
	}
//ECHO·´Ó³Ð´

BOOL	CEchoRSP :: Write ( PDU_Service *PDU, DICOMCommandObject *DCO,
	UINT16	ErrorCode )
	{
	DICOMCommandObject	DCOR;
	LE_UINT16	leCommand, leError;
	VR			*vr;
	UID			uid;

	if ( ! PDU )
		return ( FALSE );
	if ( ! DCO )
		return ( FALSE );
	
	if (!GetUID(uid))
		return ( FALSE );

	while ( vr = DCO->Pop() )
		{
		switch (vr->Element)
			{
			case	0x0002:
			case	0x0800:
				DCOR.Push ( vr );
				break;
			case	0x0110:
				vr->Element = 0x0120;
				DCOR.Push ( vr );
				break;
			default:
				delete vr;
			}
		}
	leCommand = 0x8030;
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), &leCommand, FALSE );
	DCOR.Push ( vr );

	leError = ErrorCode;
	vr = new VR (0x0000, 0x0900, sizeof(UINT16), &leError, FALSE );
	DCOR.Push ( vr );

	if (!PDU->Write(&DCOR, uid))
		return ( FALSE );

	return ( TRUE );
	}
//ECHO·´Ó³Ð´

BOOL	CEchoRSP :: Write (PDU_Service *PDU, DICOMCommandObject *DCO)
	{
	return ( Write ( PDU, DCO, 0x0000 ) );	// Write Success
	}


/************************** C-Store ***************************/
//CStoreRQ¶Á
BOOL	CStoreRQ :: Read (
	DICOMCommandObject	*DCO,
	PDU_Service			*PDU,
	DICOMDataObject *DDO )
	{
	if ( ! DCO )
		return ( FALSE );
	
	if ( DCO->GetUINT16(0x0000, 0x0100) != 0x0001 )
		return ( FALSE ); // not C-Store

	if ( DCO->GetUINT16(0x0000, 0x0800) != 0x0101 )
		{
		if ( PDU )
			{
			if ( DDO )
				{
				return ( PDU->Read( DDO ) );
				}
			}
		return ( TRUE );
		}
	// C-Store request, but no data section.. -error
	return ( FALSE );
	}
//CStoreRQ¶Á

BOOL	CStoreRQ	::	Read ( DICOMCommandObject	*DCO )
	{
	return ( Read ( DCO, NULL, NULL ) );
	}
//CStoreRQÐ´

BOOL	CStoreRQ	::	Write (
	PDU_Service	*PDU,
	DICOMDataObject *DDO)
	{
	DICOMCommandObject	DCO;
	VR					*vr;
	UINT16				Temp;
	LE_UINT16			leCommand, leDSType, leMessageID, lePriority;
	UID					uid;

	if (!PDU)
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );
	
	vr = new VR (0x0000, 0x0002, uid.GetSize(),
			(void*)uid.GetBuffer(1), FALSE );
	DCO.Push ( vr );
	
	leCommand = 0x0001;
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand,
			FALSE);
	DCO.Push ( vr );

	leDSType = 0x0102;
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType,
			FALSE);
	DCO.Push ( vr );

	Temp = uniq16odd();
	leMessageID = Temp;

	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID,
			FALSE);
	DCO.Push ( vr );

	lePriority = 0;
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority,
			FALSE);
	DCO.Push ( vr );

	vr = DDO->GetVR(0x0008, 0x0018);	// Instance
	if(vr)
		{
		VR	*vr2;

		vr2 = new VR(0x0000, 0x1000, vr->Length, (BOOL)TRUE);
		memcpy(vr2->Data, vr->Data, vr->Length);
		DCO.Push(vr2);
		}
	if (!PDU->Write(&DCO, uid))
		return ( FALSE );
	if ( DDO )
		return ( PDU->Write ( DDO, uid ) );
	return ( TRUE );
	}

//CStoreRSP¶Á

BOOL	CStoreRSP	::	Read ( DICOMCommandObject	*DCO )
	{
	if ( ! DCO )
		return ( FALSE );

	if (DCO->GetUINT16(0x0000, 0x0100) != 0x8001)
		return ( FALSE ); // not a C-Store-RSP

	return ( TRUE );
	}
//CStoreRSPÐ´

BOOL	CStoreRSP	::	Write (
	PDU_Service			*PDU,
	DICOMCommandObject 	*DCO,
	UINT16				ErrorCode)	
	{
	VR			*vr;
	LE_UINT16	leCommand, leMessageID, leRQMessageID, leDSType,
		leErrorCode, lePriority;
	DICOMCommandObject	DCOR;
	UID					uid;


	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );
	
	if (!GetUID(uid))
		{
		// Try and grab the uid from the DCO
		VR		*vru = DCO->GetVR(0x0000, 0x0002);
		if ( ! vru )
			return ( FALSE );
		SetUID(uid, vru);
		}

	lePriority = 0;
	if(DCO->GetVR(0x0000, 0x0700))
		lePriority = DCO->GetUINT16( 0x0000, 0x0700 );
	leRQMessageID = DCO->GetUINT16 ( 0x0000, 0x0110 );
	leMessageID = leRQMessageID;
	leDSType = 0x0101;
	leCommand = 0x8001;
	leErrorCode = ErrorCode;
	
	while ( vr = DCO->Pop() )
		{
		switch ( vr->Element )
			{
			case	0x0002:
				DCOR.Push ( vr );
				break;
			case	0x0200:
				vr->Element = 0x0300;
				DCOR.Push ( vr );
				break;
			case	0x0300:
				vr->Element = 0x0200;
				DCOR.Push ( vr );
				break;
			case	0x1000:
				DCOR.Push ( vr );
				break;
			default:
				delete vr;
			}
		}	
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand, FALSE);
		DCOR.Push ( vr );
		//peak
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0120, sizeof(UINT16), (void*)&leRQMessageID, FALSE);
		DCOR.Push ( vr );
		//peak
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0900, sizeof(UINT16), (void*)&leErrorCode, FALSE);
		DCOR.Push ( vr );

	PDU->Write ( &DCOR, uid );
	return ( TRUE );
	}
//CStoreRSPÐ´

BOOL	CStoreRSP :: Write ( PDU_Service *PDU, DICOMCommandObject *DCO)
	{
	return ( Write ( PDU, DCO, 0x0000 ) );
	}

/************************** C-Find ***************************/
//CFindRQ¶Á

BOOL	CFindRQ :: Read (
	DICOMCommandObject	*DCO,
	PDU_Service			*PDU,
	DICOMDataObject *DDO )
	{
	if ( ! DCO )
		return ( FALSE );
	
	if ( DCO->GetUINT16(0x0000, 0x0100) != 0x0020 )
		return ( FALSE ); // not C-Find

	if ( DCO->GetUINT16(0x0000, 0x0800) != 0x0101 )
		{
		if ( PDU )
			{
			if ( DDO )
				{
				return ( PDU->Read( DDO ) );
				}
			}
		return ( TRUE );
		}
	// C-Find request, but no data section.. -error
	return ( FALSE );
	}
//CFindRQ¶Á

BOOL	CFindRQ	::	Read ( DICOMCommandObject	*DCO )
	{
	return ( Read ( DCO, NULL, NULL ) );
	}
//CFindRQÐ´

BOOL	CFindRQ	::	Write (
	PDU_Service	*PDU,
	DICOMDataObject *DDO)
	{
	DICOMCommandObject	DCO;
	VR					*vr;
	UINT16				Temp;
	LE_UINT16			leCommand, leDSType, leMessageID, lePriority;
	UID					uid;

	if (!PDU)
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );
	vr = new VR (0x0000, 0x0002, uid.GetSize(),
			(void*)uid.GetBuffer(1), FALSE );
	DCO.Push ( vr );
	
	leCommand = 0x0020;
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand,
			FALSE);
	DCO.Push ( vr );

	leDSType = 0x0102;
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType,
			FALSE);
	DCO.Push ( vr );

	Temp = uniq16odd();
	leMessageID = Temp;
	
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID,
			FALSE);
	DCO.Push ( vr );

	lePriority = 0;
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority,
			FALSE);
	DCO.Push ( vr );

	if (!PDU->Write(&DCO, uid))
		return ( FALSE );
	if ( DDO )
		return ( PDU->Write ( DDO, uid ) );
	return ( TRUE );
	}

//CFindRSP¶Á

BOOL	CFindRSP	::	Read (
	DICOMCommandObject	*DCO,
	PDU_Service		*PDU,
	DICOMDataObject	*DDO )
	{
	if ( ! DCO )
		return ( FALSE );

	if (DCO->GetUINT16(0x0000, 0x0100) != 0x8020)
		return ( FALSE ); // not a C-Find-RSP

	if (DDO)
		{
		if (DCO->GetUINT16(0x0000, 0x0800) != 0x0101)
			{
			return ( PDU->Read ( DDO ) );
			}
		}	
	return ( TRUE );
	}
//CFindRSP¶Á
BOOL	CFindRSP	::	Read ( DICOMCommandObject	*DCO )
	{
	return ( Read ( DCO, NULL, NULL ) );
	}
//CFindRSPÐ´

BOOL	CFindRSP	::	Write (
	PDU_Service			*PDU,
	DICOMCommandObject 	*DCO,
	UINT16				ErrorCode,
	DICOMDataObject		*DDO)	
	{
	VR			*vr, *vr2;
	LE_UINT16	leCommand, leMessageID, leRQMessageID, leDSType,
		leErrorCode, lePriority;
	DICOMCommandObject	DCOR;
	UID					uid;

	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );

	lePriority = 0;
	if(DCO->GetVR(0x0000, 0x0700))
		lePriority = DCO->GetUINT16( 0x0000, 0x0700 );
	leRQMessageID = DCO->GetUINT16 ( 0x0000, 0x0110 );
	leMessageID = leRQMessageID;
	if(DDO)
		leDSType = 0x0102;
	else
		leDSType = 0x0101;
	leCommand = 0x8020;
	leErrorCode = ErrorCode;

	// Rather than destory the original command object, just make
	// copies of the elements we need.

	if(vr = DCO->GetVR ( 0x0000, 0x0002 ) )
		{
		vr2 = new VR ( 0x0000, 0x0002, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	if(vr = DCO->GetVR ( 0x0000, 0x0300 ) )
		{
		vr2 = new VR ( 0x0000, 0x0300, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	if(vr = DCO->GetVR ( 0x0000, 0x0200 ) )
		{
		vr2 = new VR ( 0x0000, 0x0200, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0120, sizeof(UINT16), (void*)&leRQMessageID, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0900, sizeof(UINT16), (void*)&leErrorCode, FALSE);
		DCOR.Push ( vr );

	PDU->Write ( &DCOR, uid );
	if(DDO)
		PDU->Write ( DDO, uid );
	return ( TRUE );
	}
//CFindRSPÐ´

BOOL	CFindRSP :: Write (
	PDU_Service *PDU,
	DICOMCommandObject *DCO,
	DICOMDataObject	*DDO)
	{
	if ( DDO )
		return ( Write ( PDU, DCO, 0xff00, DDO ) );
	
	return ( Write ( PDU, DCO, 0x0000, NULL ) );
	}

/************************** C-Move ***************************/
//CMoveRQ¶Á

BOOL	CMoveRQ :: Read (
	DICOMCommandObject	*DCO,
	PDU_Service			*PDU,
	DICOMDataObject *DDO )
	{
	if ( ! DCO )
		return ( FALSE );
	
	if ( DCO->GetUINT16(0x0000, 0x0100) != 0x0021 )
		return ( FALSE ); // not C-Move

	if ( DCO->GetUINT16(0x0000, 0x0800) != 0x0101 )
		{
		if ( PDU )
			{
			if ( DDO )
				{
				return ( PDU->Read( DDO ) );
				}
			}
		return ( TRUE );
		}
	// C-Move request, but no data section.. -error
	return ( FALSE );
	}
//CMoveRQ¶Á

BOOL	CMoveRQ	::	Read ( DICOMCommandObject	*DCO )
	{
	return ( Read ( DCO, NULL, NULL ) );
	}
//CMoveRQÐ´

BOOL	CMoveRQ	::	Write (
	PDU_Service	*PDU,
	DICOMDataObject *DDO,
	BYTE		*Where)
	{
	DICOMCommandObject	DCO;
	VR					*vr;
	UINT16				Temp;
	LE_UINT16			leCommand, leDSType, leMessageID, lePriority;
	BYTE				WhereACRNema [ 17 ];
	UID					uid;

	if (!PDU)
		return ( FALSE );

	if (!Where )
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );
	vr = new VR (0x0000, 0x0002, uid.GetSize(),
			(void*)uid.GetBuffer(1), FALSE );
	DCO.Push ( vr );
	
	leCommand = 0x0021;
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand,
			FALSE);
	DCO.Push ( vr );

	leDSType = 0x0102;
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType,
			FALSE);
	DCO.Push ( vr );

	Temp = uniq16odd();
	leMessageID = Temp;
	
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID,
			FALSE);
	DCO.Push ( vr );

	lePriority = 0;
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority,
			FALSE);
	DCO.Push ( vr );

	strcpy((char*) WhereACRNema, "                ");
	memcpy(	(void*) &WhereACRNema[0],
			(void*) Where,
			strlen((char*) Where) );
	vr = new VR (0x0000, 0x0600, 16, (void*) &WhereACRNema[0],
			FALSE);
	DCO.Push ( vr );

	if (!PDU->Write(&DCO, uid))
		return ( FALSE );
	if ( DDO )
		return ( PDU->Write ( DDO, uid ) );
	return ( TRUE );
	}

//CMoveRSP¶Á

BOOL	CMoveRSP	::	Read (
	DICOMCommandObject	*DCO,
	PDU_Service			*PDU,
	DICOMDataObject		*DDO )
	{
	if ( ! DCO )
		return ( FALSE );

	if (DCO->GetUINT16(0x0000, 0x0100) != 0x8021)
		return ( FALSE ); // not a C-Move-RSP

	if (DDO)
		{
		if (DCO->GetUINT16(0x0000, 0x0800) != 0x0101)
			{
			if ( PDU )
				return ( PDU->Read ( DDO ) );
			}
		}	
	return ( TRUE );
	}
//CMoveRSP¶Á

BOOL	CMoveRSP	::	Read ( DICOMCommandObject	*DCO )
	{
	return ( Read ( DCO, NULL, NULL ) );
	}
//CMoveRSPÐ´

BOOL	CMoveRSP	::	Write (
	PDU_Service			*PDU,
	DICOMCommandObject 	*DCO,
	UINT16				ErrorCode,
	UINT16				NumberRemaining,
	UINT16				NumberComplete,
	UINT16				NumberFailed,
	UINT16				NumberWarning,
	DICOMDataObject		*DDO)	
	{
	VR			*vr, *vr2;
	LE_UINT16	leCommand, leMessageID, leRQMessageID, leDSType,
		leErrorCode, lePriority;
	LE_UINT16	leNumberRemaining, leNumberComplete,
				leNumberFailed, leNumberWarning;
	DICOMCommandObject	DCOR;
	UID					uid;

	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );

	if (!GetUID(uid))
		return ( FALSE );

	lePriority = 0;
	if(DCO->GetVR(0x0000, 0x0700))
		lePriority = DCO->GetUINT16( 0x0000, 0x0700 );
	leRQMessageID = DCO->GetUINT16 ( 0x0000, 0x0110 );
	leMessageID = leRQMessageID;
	leNumberRemaining = NumberRemaining;
	leNumberComplete = NumberComplete;
	leNumberFailed = NumberFailed;
	leNumberWarning = NumberWarning;
	if(DDO)
		leDSType = 0x0102;
	else
		leDSType = 0x0101;
	leCommand = 0x8021;
	leErrorCode = ErrorCode;
	
	if(vr = DCO->GetVR ( 0x0000, 0x0002 ) )
		{
		vr2 = new VR ( 0x0000, 0x0002, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	if(vr = DCO->GetVR ( 0x0000, 0x0300 ) )
		{
		vr2 = new VR ( 0x0000, 0x0300, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	if(vr = DCO->GetVR ( 0x0000, 0x0200 ) )
		{
		vr2 = new VR ( 0x0000, 0x0200, vr->Length, TRUE );
		memcpy(vr2->Data, vr->Data, (int)vr->Length);
		DCOR.Push ( vr2 );
		}
	
	vr = new VR (0x0000, 0x0100, sizeof(UINT16), (void*)&leCommand, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0110, sizeof(UINT16), (void*)&leMessageID, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0120, sizeof(UINT16), (void*)&leRQMessageID, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0700, sizeof(UINT16), (void*)&lePriority, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0800, sizeof(UINT16), (void*)&leDSType, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x0900, sizeof(UINT16), (void*)&leErrorCode, FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x1020, sizeof(UINT16), (void*)&leNumberRemaining,
		FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x1021, sizeof(UINT16), (void*)&leNumberComplete,
		FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x1022, sizeof(UINT16), (void*)&leNumberFailed,
		FALSE);
		DCOR.Push ( vr );
	vr = new VR (0x0000, 0x1023, sizeof(UINT16), (void*)&leNumberWarning,
		FALSE);
		DCOR.Push ( vr );

	PDU->Write ( &DCOR, uid );
	if(DDO)
		PDU->Write ( DDO, uid );
	return ( TRUE );
	}
