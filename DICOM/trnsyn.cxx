/*
19980409   ljz   Fixed possible transition between presence and absence
                 of TypeCodes in 'Dynamic_ParseRawVRIntoDCM'
19990904   ljz   Treat type UN same as OB,OW and SQ when reading Explicit files.
20001106   mvh   Replaced == operator by strcmp
20010905   ljz   Several fixes: UCDMC should NOW be able to read ExplicitBigEndian files
20011220   ljz   Fixes in Sequence and Item delimiters
20030122   ljz   Fixed small leak in Dynamic_ParseRawVRIntoDCM
20030205   ljz   Support new datatype 'UT' (unlimited text). Thanx to William Peterson
20030424   ljz   Changedprototype of ParseDCMIntoRawVR
		 Fixed huge bug in sending of ExplicitLittleEndian sequences
20030627   mvh   Fix for MSC 4.2
*/

#	include	"dicom.hpp"


// A special thanks to Terry Rosenbaum at MSU for the countless bug fixes
// in the sequence stuff.


// These two routines ParseRawVRIntoDCM and ParseDCMIntoRawVR represent the support
// side of the Transfer Syntax negiotian.  If you plan on supporting anything beyond
// base level DICOM Little Endian, then you need to override these routines, and 
// provide the appropiate parseing mechanisms.

// This should only be called if the transfer syntax is the default DICOM transfer
// syntax. 1.2.840.10008.1.2

// Note that this routine is recursive in nature.

typedef	struct	_TransferSyntaxAlias
	{
	char	*TransferSyntaxUID;
	int		Alias;
	}	TransferSyntaxAlias;

// Transfer Syntax Aliases.  Currently, we don't try and figure out how
// to decode the JPEG compressed transfer syntaxes.  We can read/write them
// assuming that someother process can encode/decode the JPEG sections.
//
// The order these are placed in the table is very important.  The ones
// at the top are tryed first.  Ie, the syntax encoder will default to
// sending the top most transfer synax.  In this case, that is Implicit
// Little Endian.
TransferSyntaxAlias	TransferSyntaxAliases[] =
	{
		{ "1.2.840.10008.1.2", TSA_IMPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.1", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.2", TSA_EXPLICIT_BIG_ENDIAN },
		{ "1.2.840.10008.1.2.5", TSA_EXPLICIT_LITTLE_ENDIAN },//RLE
		{ "1.2.840.10008.1.2.4.50", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.51", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.52", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.53", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.54", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.55", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.56", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.57", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.58", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.59", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.60", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.61", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.62", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.63", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.64", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.65", TSA_EXPLICIT_LITTLE_ENDIAN },
//		{ "1.2.840.10008.1.2.4.66", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.70", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.80", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.81", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.90", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ "1.2.840.10008.1.2.4.91", TSA_EXPLICIT_LITTLE_ENDIAN },
		{ NULL, 0 }
	};


// Decapsulate image pixel data into an encapsulted format.
BOOL
PDU_Service	::	ParseImagePixelDataRawToDCM(
	LinkedBuffer	&VRBuffer,
	DICOMObject		*DCMObject)
	{

	while ( VRBuffer.GetIncomingSize() )
		{
		VR	*vr	= new VR;

		VRBuffer >> vr->Group;
		VRBuffer >> vr->Element;
		VRBuffer >> vr->Length;
	
		if ( (vr->Group == 0xfffe) &&
			 (vr->Element == 0xe0dd))
			{
			delete vr;
			return ( TRUE );	// the end
			}
		if ( (vr->Group == 0xfffe) &&
			 (vr->Element == 0xe000))	// hopefully...
			{
			DICOMDataObject		*DDO = new DICOMDataObject;
			vr->Data = new BYTE[vr->Length];

			VRBuffer.Read((BYTE*)vr->Data, vr->Length);
			DDO->Push(vr);
			DCMObject->Push(DDO);
			continue;
			}
		// protocol error? or maybe something encoded before an image start..
		// whatever it is.. we dont' care.. do our best..
		vr->Data = new BYTE[vr->Length];
		VRBuffer.Read((BYTE*)vr->Data, vr->Length);
		delete vr;		
		}	


	return ( FALSE );	// mis-encoded
	}

// Encapsulate image pixel data into an encapsulated format.
BOOL
PDU_Service	::	ParseImagePixelDataDCMToRaw(
	Array < DICOMDataObject *>	*ADDO,
	LinkedBuffer				&VRBuffer)
	{
	VR		*vr;
	UINT	Index = 0;
	DICOMDataObject	*DDO;
	
	while ( Index < ADDO->GetSize() )
		{
		DDO = ADDO->Get(Index);
		++Index;
		while(vr = DDO->Pop())
			{
			if ((vr->Group == 0xfffe) &&
				(vr->Element == 0xe000))
				break;
			delete vr;
			}
		if(!vr)
			{
			// messed up encapuslated
			continue;
			}
		VRBuffer << vr->Group;
		VRBuffer << vr->Element;
		VRBuffer << vr->Length;
		if(vr->Length)
			VRBuffer.Write((BYTE*)vr->Data, vr->Length);
		delete vr;
		}
	VR	vrs;

//	vrs.Group = 0xffe0;
	vrs.Group = 0xfffe;
	vrs.Element = 0xe0dd;
	vrs.Length = 0;
//	VRBuffer << vr->Group;		//ljz
//	VRBuffer << vr->Element;
//	VRBuffer << vr->Length;
	VRBuffer << vrs.Group;
	VRBuffer << vrs.Element;
	VRBuffer << vrs.Length;
	return ( TRUE );	
	}


BOOL	PDU_Service	::	ParseRawVRIntoDCM(BYTE	PCID, LinkedBuffer	&VRBuffer, DICOMObject	*DCMObject)
	{
	UID	uid;
	UINT	Index;
	VR*	pVR;

	if(!GetTransferSyntaxUID(PCID, uid))
		return ( FALSE );	// Error no UID associated with PCID

	Index = 0;
	while ( TransferSyntaxAliases[Index].TransferSyntaxUID )
		{
		if(strcmp((char *)(uid.GetBuffer(0)), TransferSyntaxAliases[Index].TransferSyntaxUID)==0)
			{
			switch (TransferSyntaxAliases[Index].Alias)
				{
				case	TSA_IMPLICIT_LITTLE_ENDIAN:
					if(!ImplicitLittleEndian_ParseRawVRIntoDCM(
						VRBuffer,DCMObject))
						return ( FALSE );
					break;
				case	TSA_EXPLICIT_LITTLE_ENDIAN:
					/* LJ: Store the used TransferSyntax for later use (in database
					   and) when we want to write a Chapter10 file.
					*/
					pVR = DCMObject->GetVR(0x0002, 0x0010);
					if (pVR)
						{
						delete[] pVR->Data;
						pVR->Data   = new BYTE[strlen((char*)uid.GetBuffer(0)) + 1];
						strcpy((char*)pVR->Data, (char*)uid.GetBuffer(0));
						pVR->Length = strlen((char*)uid.GetBuffer(0));
						if (pVR->Length & 0x01)
							pVR->Length++;
						}
					else
						{
						pVR = new VR(0x0002, 0x0010, strlen((char*)uid.GetBuffer(0)),
							strdup((char*)uid.GetBuffer(0)), TRUE); 
						}
					DCMObject->Push(pVR);
					if(!ExplicitLittleEndian_ParseRawVRIntoDCM(
						VRBuffer,DCMObject))
						return ( FALSE );
					break;
				case	TSA_EXPLICIT_BIG_ENDIAN:
					if(!ExplicitBigEndian_ParseRawVRIntoDCM(
						VRBuffer,DCMObject))
						return ( FALSE );
					break;
				default:
					return ( FALSE );	// internal error
				}
			return ( TRUE );
			}
		++Index;
		}
	return ( FALSE );	// Cannot Decode
	}

// Routine looks in the accepted transfer syntax table for a transfer
// syntax to use.
BYTE	PDU_Service	::	ParseDCMIntoRawVR(DICOMObject	*DCMObject, LinkedBuffer	&VRBuffer)
	{
//	VR		*vr;
	UID		uid;
	BYTE	PCID;
	UINT					Index;

	if ( ! DCMObject )
		return ( FALSE );

	Index = 0;
	while (TransferSyntaxAliases[Index].TransferSyntaxUID)
		{
		uid.Set(TransferSyntaxAliases[Index].TransferSyntaxUID);
		if((PCID=GetAcceptedPCIDForTransferSyntax(uid)))
			{
			switch (TransferSyntaxAliases[Index].Alias)
				{
				case	TSA_IMPLICIT_LITTLE_ENDIAN:
					if(!ImplicitLittleEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				case	TSA_EXPLICIT_LITTLE_ENDIAN:
					if(!ExplicitLittleEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				case	TSA_EXPLICIT_BIG_ENDIAN:
					if(!ExplicitBigEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				default:
					return ( 0 );	// internal error
				}
			return ( PCID );
			}
		++Index;
		}
	return ( 0 );
	}

BYTE	PDU_Service	::	ParseDCMIntoRawVR(DICOMObject	*DCMObject, LinkedBuffer	&VRBuffer, UID	&AbsUID, BOOL bIsDataObject)
	{
//	VR		*vr;
	UID		uid;
	BYTE	PCID;
	UINT					Index;

	if ( ! DCMObject )
		return ( FALSE );

	Index = 0;
	while (TransferSyntaxAliases[Index].TransferSyntaxUID)
		{
		uid.Set(TransferSyntaxAliases[Index].TransferSyntaxUID);
		if((PCID=GetPresentationContextID(AbsUID, uid)))
			{
			if (!bIsDataObject)
				{	// CommandObject always ImplicitLittleEndian
				if(!ImplicitLittleEndian_ParseDCMIntoRawVR(
					DCMObject,VRBuffer))
				return ( 0 );	// internal error
				}
			else
				switch (TransferSyntaxAliases[Index].Alias)
				{
				case	TSA_IMPLICIT_LITTLE_ENDIAN:
					if(!ImplicitLittleEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				case	TSA_EXPLICIT_LITTLE_ENDIAN:
					if(!ExplicitLittleEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				case	TSA_EXPLICIT_BIG_ENDIAN:
					if(!ExplicitBigEndian_ParseDCMIntoRawVR(
						DCMObject,VRBuffer))
						return ( 0 );
					break;
				default:
					return ( 0 );	// internal error
				}
			return ( PCID );
			}
		++Index;
		}
	return ( 0 );
	}




BOOL	PDU_Service	::	ImplicitLittleEndian_ParseRawVRIntoDCM(
	LinkedBuffer	&VRBuffer,
	DICOMObject	*DCMObject)
	{
	VRBuffer.SetIncomingEndian(LITTLE_ENDIAN);
	return ( Implicit_ParseRawVRIntoDCM(VRBuffer, DCMObject) );
	}

BOOL	PDU_Service	::	ImplicitLittleEndian_ParseDCMIntoRawVR (
	DICOMObject	*DCMObject,
	LinkedBuffer	&VRBuffer)
	{
	VRBuffer.SetOutgoingEndian ( LITTLE_ENDIAN );
	return ( Implicit_ParseDCMIntoRawVR(DCMObject, VRBuffer) );
	}

BOOL	PDU_Service	::	Implicit_ParseRawVRIntoDCM(LinkedBuffer	&VRBuffer, DICOMObject	*DCMObject)
	{
	VR	*vr = NULL;
//	UINT32	tuint32, tuint32_2;
//	DICOMObject	*EBO;		// Embedded sequence Object
	
//	fprintf(stderr, "ParseRawVRInfoDCM(%d, VRBuffer, %x)\n",
//		PCID, DCMObject);
	if ( ! DCMObject )
		return ( FALSE );	// passed a NULL object pointer

	while ( VRBuffer.GetIncomingSize())
		{
		vr = new VR;
		if ( ! vr )
			return ( FALSE );	// memory error
		
		VRBuffer >> vr->Group;
		VRBuffer >> vr->Element;
		//printf("[%4.4x, %4.4x]\n", vr->Group, vr->Element);
		if ( vr->Group == 0xfffe )
			{
			// Ok, this is a deliminator type of item.
			// handled special.  Always implicit
			VRBuffer >> vr->Length;
			if ( (vr->Element == 0xe0dd)||
				 (vr->Element == 0xe00d) )	// implicit terminmator
				{
				delete vr;
				return ( TRUE );
				}
			if ( vr->Length == 0xffffffff)
				{
				// Implicit length.. go until deliminator
				vr->Length = 0;delete vr;
				
				DICOMDataObject *DDO = new DICOMDataObject;
				if ( ! Implicit_ParseRawVRIntoDCM ( VRBuffer, DDO ) )
					{
					delete DDO;
					return ( FALSE );
					}
				DCMObject->Push(DDO);
				continue;
				}
			// Ok, an explicit length.. sequence item begin?
			if ( vr->Element == 0xe000 )
				{
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					return ( FALSE );	// memory error
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

				MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
							TRUE, VRBuffer.GetIncomingEndian() );
				vr->Length = 0;
				//delete vr->Data;
				vr->Data = NULL;
				delete vr;
				DICOMDataObject *DDO = new DICOMDataObject;
				if ( ! Implicit_ParseRawVRIntoDCM ( MB, DDO ) )
					{
					delete DDO;
					return ( FALSE );
					}
				DCMObject->Push(DDO);
				continue;
				}
			// Not quite sure what to say here...
			return ( FALSE );
			}
		VRBuffer >> vr->Length;
		// Type this element
		vr->TypeCode=RunTimeClass(vr->Group,vr->Element);


		if ( vr->Length == 0xffffffff )
			{
			// variable length item [sequence, etc]
			vr->Length = 0;
			DCMObject->Push(vr);
			if ( (vr->Group == 0x7fe0) &&
				 (vr->Element == 0x0010))
				{
				// Image Pixel data.. encapsulated.. ugly..

				if(!ParseImagePixelDataRawToDCM(
					VRBuffer,
					DCMObject))
					return(FALSE);
				continue;
				}
			if(!Implicit_ParseRawVRIntoDCM(VRBuffer, DCMObject))
				return(FALSE);
			continue;
			}

		if ( vr->TypeCode == 'SQ' )
			{
			// explicit length sequence
			// in this case.. pull the data into a memory buffer
			// and parse that.
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

			MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
			vr->Length = 0;
			//delete vr->Data;
			vr->Data = NULL;
			DCMObject->Push(vr);
			//DICOMDataObject *DDO = new DICOMDataObject;
			if ( ! Implicit_ParseRawVRIntoDCM ( MB, DCMObject ) )
				{
				//delete DDO;
				return ( FALSE );
				}
			//DCMObject->Push(DDO);
			continue;			
			}
		// Explicit Length ( normal)
		if ( vr->Length )
			{
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
			}
		else
			vr->Data = NULL;

		DCMObject->Push(vr);
		}
				
	return ( TRUE );
	}

BOOL	PDU_Service	::	Implicit_ParseDCMIntoRawVR (
	DICOMObject	*DCMObject,
	LinkedBuffer	&VRBuffer)
	{
	VR		*vr;
//	Array<DICOMObject *>	*ArrayPtr;
	UINT					Index;
	UINT					A1Flag;
	UINT32					tuint32;
	DCMObject->ReGenerateGroupLength(FALSE);
	while ( vr = DCMObject->Pop() )
		{
	//	if (!vr->Element)
	//	{
	//		delete vr;
	//		continue;
	//	}
		VRBuffer << vr->Group;
		VRBuffer << vr->Element;
		if(vr->Length&0x01)
			{
			A1Flag = 1;
			++vr->Length;
			}
		else
			A1Flag = 0;
		if(!vr->SQObjectArray)
			{
			VRBuffer << vr->Length;
			if(vr->Length)
				VRBuffer.Write ( (BYTE *) vr->Data, vr->Length-A1Flag );
			if(A1Flag)
				{
				VRBuffer.Write((BYTE*) "\0", 1);
				--vr->Length;
				A1Flag = 0;
				}
			}
		else
			{
			tuint32 = 0xffffffff;
			VRBuffer << tuint32;
			if(A1Flag)
				{
				// (intentionally commented out)VRBuffer.Write((BYTE*) "\0", 1);
				--vr->Length;
				A1Flag = 0;
				}
			}
		if(vr->SQObjectArray)
			{
			Array < DICOMDataObject	*>	*ArrayPtr =
				(Array<DICOMDataObject *> *) vr->SQObjectArray;
			Index = 0;
			if ( (vr->Group == 0x7fe0 )&&
				 (vr->Element == 0x0010 ))
				{
				ParseImagePixelDataDCMToRaw(ArrayPtr, VRBuffer);
				delete vr;
				continue;
				}
			while ( Index < ArrayPtr->GetSize() )
				{
				vr->Group = 0xfffe;
				vr->Element = 0xe000;
				vr->Length = 0xffffffff;
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;
				Implicit_ParseDCMIntoRawVR(
					ArrayPtr->Get(Index), VRBuffer);
				vr->Group = 0xfffe;
				vr->Element = 0xe00d;
				vr->Length = 0x00000000;
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;
				++Index;
				}
			vr->Group = 0xfffe;
			vr->Element = 0xe0dd;
			vr->Length = 0x00000000;
			VRBuffer << vr->Group;
			VRBuffer << vr->Element;
			VRBuffer << vr->Length;
			}
			
		delete vr;
		}
	return ( TRUE );
	}


BOOL	PDU_Service	::	ExplicitLittleEndian_ParseRawVRIntoDCM(
	LinkedBuffer	&VRBuffer,
	DICOMObject	*DCMObject)
	{
	VRBuffer.SetIncomingEndian(LITTLE_ENDIAN);
	return ( Explicit_ParseRawVRIntoDCM(VRBuffer, DCMObject) );
	}

BOOL	PDU_Service	::	ExplicitLittleEndian_ParseDCMIntoRawVR (
	DICOMObject	*DCMObject,
	LinkedBuffer	&VRBuffer)
	{
	VRBuffer.SetOutgoingEndian ( LITTLE_ENDIAN );
	return ( Explicit_ParseDCMIntoRawVR(DCMObject, VRBuffer) );
	}

BOOL	PDU_Service	::	ExplicitBigEndian_ParseRawVRIntoDCM(
	LinkedBuffer	&VRBuffer,
	DICOMObject	*DCMObject)
	{
	VRBuffer.SetIncomingEndian(BIG_ENDIAN);
	return ( Explicit_ParseRawVRIntoDCM(VRBuffer, DCMObject) );
	}

BOOL	PDU_Service	::	ExplicitBigEndian_ParseDCMIntoRawVR (
	DICOMObject	*DCMObject,
	LinkedBuffer	&VRBuffer)
	{
	VRBuffer.SetOutgoingEndian ( BIG_ENDIAN );
	return ( Explicit_ParseDCMIntoRawVR(DCMObject, VRBuffer) );
	}

BOOL	PDU_Service	::	Explicit_ParseRawVRIntoDCM(LinkedBuffer	&VRBuffer, DICOMObject	*DCMObject)
	{
	VR	*vr;
//	UINT32	tuint32, tuint32_2;
	UINT16	Length16;
//	DICOMObject	*EBO;		// Embedded sequence Object
	BYTE		s1[2];

	if ( ! DCMObject )
		return ( FALSE );	// passed a NULL object pointer

	while ( VRBuffer.GetIncomingSize())
		{
		vr = new VR;
		if ( ! vr )
			return ( FALSE );	// memory error

		VRBuffer >> vr->Group;
		VRBuffer >> vr->Element;

		if (vr->Group == 0x0000)//[peak]this is a command
			{
			VRBuffer >> vr->Length;
			if (vr->Length)
				{
				// Explicit Length ( normal)
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					return ( FALSE );	// memory error
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
				}
			else
				vr->Data = NULL;
			DCMObject->Push(vr);
			continue;
			}

		if ( vr->Group == 0xfffe )
			{
			// Ok, this is a deliminator type of item.
			// handled special.  Always implicit
			VRBuffer >> vr->Length;
			if ( (vr->Element == 0xe0dd)||
				 (vr->Element == 0xe00d) )	// implicit terminmator
				{
				delete vr;
				return ( TRUE );
				}
			if ( vr->Length == 0xffffffff)
				{
				// Implicit length.. go until deliminator
				vr->Length = 0;delete vr;
				
				DICOMDataObject *DDO = new DICOMDataObject;
				if ( ! Explicit_ParseRawVRIntoDCM ( VRBuffer, DDO ) )
//				if ( ! Implicit_ParseRawVRIntoDCM ( VRBuffer, DDO ) )
					{
					delete DDO;
					return ( FALSE );
					}
				DCMObject->Push(DDO);
				continue;
				}
			// Ok, an explicit length.. sequence item begin?
			if ( vr->Element == 0xe000 )
				{
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					return ( FALSE );	// memory error
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

				MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
				vr->Length = 0;
				//delete vr->Data;
				vr->Data = NULL;
				delete vr;
				DICOMDataObject *DDO = new DICOMDataObject;
				if ( ! Explicit_ParseRawVRIntoDCM ( MB, DDO ) )
					{
					delete DDO;
					return ( FALSE );
					}
				DCMObject->Push(DDO);
				continue;
				}
			// Not quite sure what to say here...
			return ( FALSE );
			}

		VRBuffer >> s1[0];VRBuffer>>s1[1];
		Pack(s1[0],s1[1],vr->TypeCode);

		if(	(vr->TypeCode=='OB')||
			(vr->TypeCode=='OW')||
			(vr->TypeCode=='OF')||//[peak]
			(vr->TypeCode=='UT')||//[peak]
			(vr->TypeCode=='UN')||	/* LJ */
			(vr->TypeCode=='SQ'))
			{
			VRBuffer >> Length16;	// Toss away 16 bits
			VRBuffer >> vr->Length;	// 32 bit Length
			}
		else
			{
			VRBuffer >> Length16;
			vr->Length = (UINT32) Length16;
			}


		if ( vr->Length == 0xffffffff )
			{
			// variable length item [sequence, etc]
			vr->Length = 0;
			DCMObject->Push(vr);
			if ( (vr->Group == 0x7fe0) &&
				 (vr->Element == 0x0010))
				{
				// Image Pixel data.. encapsulated.. ugly..

				if(!ParseImagePixelDataRawToDCM(
					VRBuffer,
					DCMObject))
					return(FALSE);
				continue;
				}
			// Recurse
			if(!Explicit_ParseRawVRIntoDCM(VRBuffer, DCMObject))
				return(FALSE);
			continue;
			}

		if ( vr->TypeCode == 'SQ' )
			{
			// explicit length sequence
			// in this case.. pull the data into a memory buffer
			// and parse that.
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

			MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
			vr->Length = 0;
			//delete vr->Data;
			vr->Data = NULL;
			DCMObject->Push(vr);
			//DICOMDataObject *DDO = new DICOMDataObject;
			if ( ! Explicit_ParseRawVRIntoDCM ( MB, DCMObject ) )
				{
				//delete DDO;
				return ( FALSE );
				}
			//DCMObject->Push(vr);
			continue;			
			}
		if (vr->Length)
			{
			// Explicit Length ( normal)
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
			}
		else
			vr->Data = NULL;
		DCMObject->Push(vr);
		}

/*

		vr = new VR;
		if ( ! vr )
			return ( FALSE );	// memory error
		
		VRBuffer >> vr->Group;
		VRBuffer >> vr->Element;

		VRBuffer >> s1[0];VRBuffer>>s1[1];

		Pack(s1[0],s1[1],vr->TypeCode);

//		vr->TypeCode = (((UINT16)s1[0])<<8)+((UINT16)s1[1]);


		if(	(vr->TypeCode=='OB')||
			(vr->TypeCode=='OW')||
			(vr->TypeCode=='SQ'))
			{
			VRBuffer >> Length16;	// Toss away 16 bits
			VRBuffer >> vr->Length;	// 32 bit Length
			}
		else
			{
			VRBuffer >> Length16;
			vr->Length = (UINT32) Length16;
			}
		if(vr->Group == 0xFFFE)
			{
			tuint32 = vr->Length;
			delete vr;
			if(vr->Element == 0xE00D)
				{

				return ( TRUE );	// end of sequence
				}
			else	
			if(vr->Element == 0xE000)
				{
				if(tuint32 == 0xffffffff)	// not specifed length
					{
					EBO = new DICOMObject;
					if(!EBO)
						return ( FALSE );
					if(!Explicit_ParseRawVRIntoDCM(VRBuffer, EBO))
						return ( FALSE ); // error
					DCMObject->Push(EBO);
					if(SizeCap)
						{
						SizeCap -= (EBO->Length + sizeof(UINT16) +
							sizeof(UINT16) + sizeof(UINT32) +
							sizeof(UINT16) + sizeof(UINT16) +
							sizeof(UINT32));
						if(!SizeCap)
							return ( TRUE );
						}
					}
				else
					{
					if(tuint32)	// save
						{
						tuint32_2 = SizeCap;
						SizeCap = tuint32;
						EBO = new DICOMObject;
						if(!EBO)
							return(FALSE);
						if(!Explicit_ParseRawVRIntoDCM(VRBuffer, EBO))
							return ( FALSE );
						DCMObject->Push(EBO);
						SizeCap = tuint32_2;
						if(SizeCap)
							{
							SizeCap -= (EBO->Length + sizeof(UINT16) +
									sizeof(UINT16) +sizeof(UINT32));
							if(!SizeCap)
								return ( TRUE );
							}
						}
					else
						{
						// zero length sequence (ignored)
						if(SizeCap)
							{
							SizeCap -= (sizeof(UINT16) + sizeof(UINT16) +
									sizeof(UINT32));
							if(!SizeCap)
								return ( TRUE );
							}
						}
					}
				}
			}
		else
			{
			if(vr->Length == 0xffffffff )
				vr->Length = 0;
			if(vr->Length)
				{
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					return ( FALSE );	// memory error
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
				}
			else
				vr->Data = NULL;

			if(vr->TypeCode == 'SQ')
				{
				tuint32 = vr->Length;
				vr->Length = 0;
				}
			DCMObject->Push ( vr );
			if ( vr->TypeCode == 'SQ' )
				{
				MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
				//if(vr->Data)
				//	delete vr->Data;
				vr->Data = NULL;
			
				if(!Explicit_ParseRawVRIntoDCM(MB, EBO))
					return ( FALSE );
				}
			if(SizeCap)
				{
				SizeCap -= (vr->Length + sizeof(UINT16) +
					sizeof(UINT16) + sizeof(UINT32));
				if(!SizeCap)
					return ( TRUE );
				}
			}
		}
*/
	return ( TRUE );
	}


BOOL	PDU_Service	::	Explicit_ParseDCMIntoRawVR (
	DICOMObject	*DCMObject,
	LinkedBuffer	&VRBuffer)
	{
	VR		*vr;
//	Array<DICOMObject *>	*ArrayPtr;
	UINT					Index;
	UINT					A1Flag;
	UINT16					Length16;
 	BYTE					s1[2];
	UINT32					tuint32;

	while ( vr = DCMObject->Pop() )
		{
//		if((!vr->Element) && (vr->Group != 0x0000)&&(vr->Group != 0x0002)&&(vr->Group != 0x0004)&&(vr->Group != 0x0006))
//			{
//					// LJ maybe send group-length of group 0x0000
//			delete vr;	// 4/16/96 - no longer sender group length under
//					// explicit.  It wasn't correct anyway.
//			continue;
//			}
		VRBuffer << vr->Group;
		VRBuffer << vr->Element;
		if(vr->TypeCode==0)
		{
			vr->TypeCode=RunTimeClass(vr->Group,vr->Element);
		}

		Pack(vr->TypeCode, s1[0], s1[1]);
		VRBuffer << s1[0];VRBuffer<<s1[1];
	
		if(vr->Length&0x01)
			{
			A1Flag = 1;
			++vr->Length;
			}
		else
			A1Flag = 0;
		if(	(vr->TypeCode=='OB')||
			(vr->TypeCode=='OW')||
			(vr->TypeCode=='OF')||
			(vr->TypeCode=='UN')||
			(vr->TypeCode=='UT')||
			(vr->TypeCode=='SQ'))
			{
			Length16 = 0;
			VRBuffer << Length16;	// Toss in 16 bits
//			if(vr->TypeCode == 'SQ')	// LJ !!!!!!!!
			if(vr->SQObjectArray)
				{
				if(vr->SQObjectArray)
					tuint32 = 0xffffffff;
				else
					tuint32 = 0;
				VRBuffer << tuint32;
				}
			else
				VRBuffer << vr->Length;	// 32 bit Length
			}
		else
			{
			Length16 = (UINT16) vr->Length;
			VRBuffer << Length16;
			}
		if(vr->TypeCode!='SQ')
			if(vr->Length)
				VRBuffer.Write ( (BYTE *) vr->Data, vr->Length-A1Flag );
		if(A1Flag)
			{
			if(vr->TypeCode!='SQ')
				VRBuffer.Write((BYTE*) "\0", 1);
			--vr->Length;
			A1Flag = 0;
			}

		if(vr->SQObjectArray)
			{
			Array<DICOMDataObject*>	*ArrayPtr =
				(Array<DICOMDataObject *> *) vr->SQObjectArray;
			Index = 0;
			if ( (vr->Group == 0x7fe0 )&&
				 (vr->Element == 0x0010 ))
				{
				ParseImagePixelDataDCMToRaw(ArrayPtr, VRBuffer);
				delete vr;
				continue;
				}
			while ( Index < ArrayPtr->GetSize() )
				{
				vr->Group = 0xfffe;
				vr->Element = 0xe000;
				vr->Length = 0xffffffff;
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;

				Explicit_ParseDCMIntoRawVR(
					ArrayPtr->Get(Index), VRBuffer);
//				++Index; //removed by lin; there should be only one Index
//				++Index;
//				vr->Group = 0xffe0;
				vr->Group = 0xfffe;	// (0xfffe,0xe00d) is a Item Delimiter
				vr->Element = 0xe00d;
				vr->Length = 0;		
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;
				++Index;
				}
//			vr->Group = 0xffe0;
			vr->Group = 0xfffe;	// (0xfffe,0xe0dd) is a Sequence Delimiter
			vr->Element = 0xe0dd;
			vr->Length = 0;		
			VRBuffer << vr->Group;
			VRBuffer << vr->Element;
			VRBuffer << vr->Length;

			}
			
		delete vr;
		}
	return ( TRUE );
	}


UINT
static
EndianOf(
	UINT	Alias)
	{
	if((Alias==TSA_EXPLICIT_LITTLE_ENDIAN)||
		(Alias==TSA_IMPLICIT_LITTLE_ENDIAN))
		return ( LITTLE_ENDIAN );
	return ( BIG_ENDIAN );
	}

void SwapBigEndianData(VR* pVR)
{ char		cSave;
  int		i, iNbValues;
  char*		pcData;

  switch (pVR->TypeCode)
  { case 'FL':	// float
    case 'SL':	// signed long
    case 'UL':	// unsigned long
    pcData = (char*)pVR->Data;
    iNbValues = pVR->Length / 4;
    for (i=0; i<iNbValues; i++)
    { cSave = pcData[0];
      pcData[0] = pcData[3];
      pcData[3] = cSave;
      cSave = pcData[1];
      pcData[1] = pcData[2];
      pcData[2] = cSave;
      pcData += 4;
    }
    break;

    case 'OW':	// int16 array
    case 'SS':	// signed short
    case 'US':	// unsigned short
    pcData = (char*)pVR->Data;
    iNbValues = pVR->Length / 2;
    for (i=0; i<iNbValues; i++)
    { cSave = pcData[0];
      pcData[0] = pcData[1];
      pcData[1] = cSave;
      pcData += 2;
    }
    break;

    case 'FD':	// double
    pcData = (char*)pVR->Data;
    iNbValues = pVR->Length / 8;
    for (i=0; i<iNbValues; i++)
    { cSave = pcData[0];
      pcData[0] = pcData[7];
      pcData[7] = cSave;
      cSave = pcData[1];
      pcData[1] = pcData[6];
      pcData[6] = cSave;
      cSave = pcData[1];
      pcData[2] = pcData[5];
      pcData[5] = cSave;
      cSave = pcData[1];
      pcData[3] = pcData[4];
      pcData[4] = cSave;
      pcData += 8;
    }
    break;
  }
}


BOOL	PDU_Service	::	Dynamic_ParseRawVRIntoDCM(
	LinkedBuffer	&VRBuffer,
	DICOMObject		*DCMObject,
	UINT			Mode
	)
	{
	VR	*vr;
	VR	*vrTRN;	//0x0002,0x0010
//	UINT32	tuint32, tuint32_2;
	UINT16	Length16;
//	DICOMObject	*EBO;		// Embedded sequence Object
	UINT		NewMode;
	BYTE		s1[2];
	char		s[128];
	int			iOrgMode;	// LJ + MVH: Added to save the original mode
	
	iOrgMode = Mode;		// LJ + MVH
	if ( ! DCMObject )
		return ( FALSE );	// passed a NULL object pointer

	if((Mode==TSA_EXPLICIT_LITTLE_ENDIAN)||
		(Mode==TSA_IMPLICIT_LITTLE_ENDIAN))
		VRBuffer.SetIncomingEndian(LITTLE_ENDIAN);
	else
		VRBuffer.SetIncomingEndian(BIG_ENDIAN);
		
	vrTRN = NULL;

	while ( VRBuffer.GetIncomingSize())
		{
		vr = new VR;
		if ( ! vr )
			return ( FALSE );	// memory error
		
		VRBuffer >> vr->Group;
		if(vr->Group > 0x0002)
			{
			// Key on the vrTRN
			UINT		Index,sLen;
			if(vrTRN)
				{
				memset((void*)s, 0,128);
				memcpy((void*)s,vrTRN->Data, vrTRN->Length%64);
				sLen = strlen(s);
				if(sLen)
					{
					if(s[sLen-1]==' ')
						s[sLen-1]='\0';
					}
				Index = 0;
				while(TransferSyntaxAliases[Index].TransferSyntaxUID)
					{
					if(!strcmp(TransferSyntaxAliases[Index].TransferSyntaxUID,s))
						{
						NewMode = TransferSyntaxAliases[Index].Alias;
						if(NewMode==Mode)
							break;
						if(EndianOf(NewMode)!=EndianOf(Mode))
							{
							// Darn
							vr->Group = SwitchEndian(vr->Group);
							VRBuffer.SetIncomingEndian(EndianOf(NewMode));
							}
						Mode = NewMode;		// LJ + MVH: Maybe disollow this later...
						break;
						}
					++Index;
					}
				if(!TransferSyntaxAliases[Index].TransferSyntaxUID)
					{
					//fprintf(stderr, "Unknown UID (%s)\n", s);
					return ( FALSE );	// woa, cannot decode remainder of data set
					}
//				delete vrTRN;	// LJ: small leak
				vrTRN = NULL;
				}
			}
		VRBuffer >> vr->Element;

		//printf("\t\t[%4.4x, %4.4x]\n", vr->Group, vr->Element);
		// Regardless of mode.. 0xfffe are encoded implicit
		if ( vr->Group == 0xfffe )
			{
			// Ok, this is a deliminator type of item.
			// handled special.  Always implicit
			VRBuffer >> vr->Length;
			if ( (vr->Element == 0xe0dd)||
				 (vr->Element == 0xe00d) )	// implicit terminmator
				{
				delete vr;
				return ( TRUE );
				}
			if ( vr->Length == 0xffffffff)
				{
				// Implicit length.. go until deliminator
				vr->Length = 0;delete vr;
				
				DICOMDataObject *DDO = new DICOMDataObject;
				if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
					(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
					{
					if ( ! Explicit_ParseRawVRIntoDCM ( VRBuffer, DDO ) )
						{
						delete DDO;
						return ( FALSE );
						}
					}
				else
					{
					if ( ! Implicit_ParseRawVRIntoDCM ( VRBuffer, DDO ) )
						{
						delete DDO;
						return ( FALSE );
						}
					}	
				DCMObject->Push(DDO);
				continue;
				}
			// Ok, an explicit length.. sequence item begin?
			if ( vr->Element == 0xe000 )
				{
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					{
					delete vr;
					return ( FALSE );	// memory error
					}
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

				MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
				vr->Length = 0;
				//delete vr->Data;
				vr->Data = NULL;
				delete vr;
				DICOMDataObject *DDO = new DICOMDataObject;
				if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
					(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
					{
					if ( ! Explicit_ParseRawVRIntoDCM ( MB, DDO ) )
						{
						delete DDO;
						return ( FALSE );
						}
					}
				else
					{
					if ( ! Implicit_ParseRawVRIntoDCM ( MB, DDO ) )
						{
						delete DDO;
						return ( FALSE );
						}
					}
				DCMObject->Push(DDO);
				continue;
				}
			// Not quite sure what to say here...
			delete vr;
			return ( FALSE );
			}


		if (iOrgMode && (vr->Group > 0x0002))
		{
		/* LJ + MVH: iOrgMode will be set to one when leaving this 'if', and set
		   to zero in the following if.
		   So, this if is entered only when processing this first GroupElement NOT
		   belonging to the metaheader...
		   This code check whether TypeCodes are present. In the Davis software,
		   the presence of TypeCodes is called 'explicit' !
		   The same check is done in 'filepdu.cxx': PDU_Service::LoadDICOMDataObject()
		   In the Davis software, IMPLICIT_BIG_ENDIAN is not known.
		 */
			VRBuffer >> s1[0];
			VRBuffer >> s1[1];
			if (s1[1] > 10)		// Explicit
			{
//				if (Mode == TSA_IMPLICIT_BIG_ENDIAN)
//					Mode = TSA_EXPLICIT_BIG_ENDIAN;
//				else
				if (Mode == TSA_IMPLICIT_LITTLE_ENDIAN)
					Mode = TSA_EXPLICIT_LITTLE_ENDIAN;
			}
			else
			{
//				if (Mode == TSA_EXPLICIT_BIG_ENDIAN)
//					Mode = TSA_IMPLICIT_BIG_ENDIAN;
//				else
				if (Mode == TSA_EXPLICIT_LITTLE_ENDIAN)
					Mode = TSA_IMPLICIT_LITTLE_ENDIAN;
			}
			iOrgMode = 1;
		}
		
		if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
			(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
			{
			if (iOrgMode != 1)
			{
				VRBuffer >> s1[0];
				VRBuffer >> s1[1];
			}
			else
				iOrgMode = 0;		// LJ + MVH: allready read two bytes
			Pack(s1[0], s1[1], vr->TypeCode);


			if(	(vr->TypeCode=='OB')||
				(vr->TypeCode=='OW')||
				(vr->TypeCode=='OF')||
				(vr->TypeCode=='UN')||
				(vr->TypeCode=='UT')||
				(vr->TypeCode=='SQ'))
				{
				VRBuffer >> Length16;	// Toss away 16 bits
				VRBuffer >> vr->Length;	// 32 bit Length
				}
			else
				{
				VRBuffer >> Length16;
				vr->Length = (UINT32) Length16;
				}
			}
		else
			{
			if (iOrgMode != 1)
				VRBuffer >> vr->Length;
			else
			{					// LJ + MVH: allready read two bytes
				if (Mode==TSA_EXPLICIT_BIG_ENDIAN)
				{
					*((char*)(&vr->Length) + 3) = s1[0];
					*((char*)(&vr->Length) + 2) = s1[1];
					VRBuffer >> ((char*)(&vr->Length))[1];
					VRBuffer >> ((char*)(&vr->Length))[0];
				}
				else
				{
					*((char*)(&vr->Length) + 0) = s1[0];
					*((char*)(&vr->Length) + 1) = s1[1];
					VRBuffer >> ((char*)(&vr->Length))[2];
					VRBuffer >> ((char*)(&vr->Length))[3];
				}
				iOrgMode = 0;
			}
				vr->TypeCode=RunTimeClass (vr->Group,vr->Element);
			}
			
		if(vr->Group == 0x0002)
			{
			if(vr->Element==0x0010)
				vrTRN = vr;
			}


		// Ok, at this point it's not a 0xfffe element, and it's
		// and the group, element, length (and possibly typecode)
		// has been decoded.
		// must now 


		if ( vr->Length == 0xffffffff )
			{
			// variable length item [sequence, etc]
			vr->Length = 0;
			DCMObject->Push(vr);
			if ( (vr->Group == 0x7fe0) &&
				 (vr->Element == 0x0010))
				{
				// Image Pixel data.. encapsulated.. ugly..

				if(!ParseImagePixelDataRawToDCM(
					VRBuffer,
					DCMObject))
					return(FALSE);
				continue;
				}
			// Recurse
			if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
				(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
				{
				if ( ! Explicit_ParseRawVRIntoDCM ( VRBuffer, DCMObject ) )
					return(0);
				}
			else
				{
				if ( ! Implicit_ParseRawVRIntoDCM ( VRBuffer, DCMObject ) )
					{
					return(0);
					}
				}

			continue;
			}

		if ( vr->TypeCode == 'SQ' )
			{
			// explicit length sequence
			// in this case.. pull the data into a memory buffer
			// and parse that.
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);

			MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
			vr->Length = 0;
			//delete vr->Data;
			vr->Data = NULL;
			DCMObject->Push(vr);
			//DICOMDataObject *DDO = new DICOMDataObject;
			if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
				(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
				{
				if ( ! Explicit_ParseRawVRIntoDCM ( MB, DCMObject ) )
					{
					//delete DDO;
					return ( FALSE );
					}
				}
			else
				{
				if ( ! Implicit_ParseRawVRIntoDCM ( MB, DCMObject ) )
					{
					//delete DDO;
					return ( FALSE );
					}
				}
			//DCMObject->Push(DDO);
			continue;			
			}
		// Explicit Length ( normal)
		if ( vr->Length )
			{
			vr->Data =  new BYTE [ vr->Length ];
			if ( !vr->Data )
				return ( FALSE );	// memory error
			VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
			vr->ReleaseMemory=TRUE;//peak add
			if (Mode==TSA_EXPLICIT_BIG_ENDIAN)
				SwapBigEndianData(vr);
			}
		else
			vr->Data = NULL;

		/* LJ */
//		if (vr != vrTRN)
		  DCMObject->Push(vr);
		}

/*

		if(vr->Group == 0xFFFE)
			{
			tuint32 = vr->Length;
			delete vr;
			if(vr->Element == 0xE00D)
				{

				return ( TRUE );	// end of sequence
				}
			else	
			if(vr->Element == 0xE000)
				{
				if(tuint32 == 0xffffffff)	// not specifed length
					{
					EBO = new DICOMObject;
					if(!EBO)
						return ( FALSE );
					if(!Implicit_ParseRawVRIntoDCM(VRBuffer, EBO))
						return ( FALSE ); // error
					DCMObject->Push(EBO);
					if(SizeCap)
						{
						SizeCap -= (EBO->Length + sizeof(UINT16) +
							sizeof(UINT16) + sizeof(UINT32) +
							sizeof(UINT16) + sizeof(UINT16) +
							sizeof(UINT32));
						if(!SizeCap)
							return ( TRUE );
						}
					}
				else
					{
					if(tuint32)	// save
						{
						tuint32_2 = SizeCap;
						SizeCap = tuint32;
						EBO = new DICOMObject;
						if(!EBO)
							return(FALSE);
						if(!Implicit_ParseRawVRIntoDCM(VRBuffer, EBO))
							return ( FALSE );
						DCMObject->Push(EBO);
						SizeCap = tuint32_2;
						if(SizeCap)
							{
							SizeCap -= (EBO->Length + sizeof(UINT16) +
									sizeof(UINT16) +sizeof(UINT32));
							if(!SizeCap)
								return ( TRUE );
							}
						}
					else
						{
						// zero length sequence (ignored)
						if(SizeCap)
							{
							SizeCap -= (sizeof(UINT16) + sizeof(UINT16) +
									sizeof(UINT32));
							if(!SizeCap)
								return ( TRUE );
							}
						}
					}
				}
			}
		else
			{
			if(vr->Length == 0xffffffff)
				vr->Length = 0;
			if(vr->Length)
				{
				vr->Data =  new BYTE [ vr->Length ];
				if ( !vr->Data )
					return ( FALSE );	// memory error
				VRBuffer.Read ( (BYTE *) vr->Data, vr->Length);
				}
			else
				vr->Data = NULL;
			if (vr->TypeCode == 'SQ')
				{
				if(vr->Length)
					{
					// Explicit length 'SQ'.. hack it apart and dump it into
					// SQObjectArray.
					MemoryBuffer 	MB( (BYTE*)vr->Data, vr->Length,
								TRUE, VRBuffer.GetIncomingEndian() );
					//if(vr->Data)
					//	delete vr->Data;
					vr->Data = NULL;
					vr->Length = 0;	
					DCMObject->Push(vr);
					if(!Implicit_ParseRawVRIntoDCM(MB, DCMObject))
						{
						return ( FALSE );
						}
					}
				else
					DCMObject->Push(vr);
				}
			else
				DCMObject->Push ( vr );
			if(SizeCap)
				{
				SizeCap -= (vr->Length + sizeof(UINT16) +
					sizeof(UINT16) + sizeof(UINT32));
				if(!SizeCap)
					return ( TRUE );
				}
			}
		}
*/
	return ( TRUE );
	}

int	PDU_Service	::	Dynamic_ParseDCMIntoRawVR(
	DICOMObject	*DCMObject,
	LinkedBuffer &VRBuffer,
	UINT Mode, int* pnImgNo)
	{

	VR		*vr,*vrTRN;
//	Array<DICOMObject *>	*ArrayPtr;
	UINT					Index;
	UINT					A1Flag;
	UINT16					Length16;
	UINT					NewMode;
 	BYTE					s1[2];
	char					s[128];
	UINT32					tuint32;
	int                     nImgNumOfSeries =  0;

	if((Mode==TSA_EXPLICIT_LITTLE_ENDIAN)||
		(Mode==TSA_IMPLICIT_LITTLE_ENDIAN))
		VRBuffer.SetIncomingEndian(LITTLE_ENDIAN);
	else
		VRBuffer.SetIncomingEndian(BIG_ENDIAN);
		

	vrTRN = NULL;
	while ( vr = DCMObject->Pop() )
		{
//peak comment
//		if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
//			(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
//			if(!vr->Element)
//				{
//				if  (vr->Group == 0x0000||vr->Group==0x0002||vr->Group==0x0004||vr->Group==0x0006)
//					{
//						/* LJ: Keep grouplength of Chapter10, but fix the value so that
//						   the 'DICM' tag is included
//						*/
////					*((int*)vr->Data) = *((int*)vr->Data) + strlen("DICM");
//					}
//				else
//					{
//						delete vr;	// 4-16-96 No longer send group length in
//							// explicit mode.  --wasn't correct anyway.
//						continue;
//					}
//				}

		if(vr->Group>0x0002)
			{
			// Key on the vrTRN
			UINT		Index,sLen;
			if(vrTRN)
				{
				memset((void*)s, 0,128);
				memcpy((void*)s,vrTRN->Data, vrTRN->Length%64);
				sLen = strlen(s);
				if(sLen)
					{
					if(s[sLen-1]==' ')
						s[sLen-1]='\0';
					}
				Index = 0;
				while(TransferSyntaxAliases[Index].TransferSyntaxUID)
					{
					if(!strcmp(TransferSyntaxAliases[Index].TransferSyntaxUID,s))
						{
						NewMode = TransferSyntaxAliases[Index].Alias;
						if(NewMode==Mode)
							break;
						VRBuffer.SetIncomingEndian(EndianOf(NewMode));
						Mode = NewMode;
						break;
						}
					++Index;
					}
				if(!TransferSyntaxAliases[Index].TransferSyntaxUID)
					return ( FALSE );	// woa, cannot encode remainder of data set
				delete vrTRN;
				vrTRN = NULL;
				}
			}
		

		VRBuffer << vr->Group;
		VRBuffer << vr->Element;

		int nLen;
		char szImgNum[10] ;
		if ((vr->Group == 0x20)&&(vr->Element == 0x13))
		{
			nLen = vr->Length;
			memcpy(szImgNum , vr->Data, nLen);
			if (szImgNum[nLen-1] == 32)
			{
				szImgNum[nLen-1] = 0 ;
			}
			else
			{
				szImgNum[nLen] = 0 ;
			}
			*pnImgNo = atoi(szImgNum);
		}
		if ((vr->Group == 0x20)&&(vr->Element == 0x1002))
		{
			nLen = vr->Length;
			memcpy(szImgNum , vr->Data, nLen);
			if (szImgNum[nLen-1] == 32)
			{
				szImgNum[nLen-1] = 0 ;
			}
			else
			{
				szImgNum[nLen] = 0 ;
			}
			nImgNumOfSeries = atoi(szImgNum);
		}


		if(vr->Length&0x01)
			{
			A1Flag = 1;
			++vr->Length;
			}
		else
			A1Flag = 0;


		if((Mode==TSA_EXPLICIT_BIG_ENDIAN)||
			(Mode==TSA_EXPLICIT_LITTLE_ENDIAN))
			{
			
			if(vr->TypeCode==0)
			{
				vr->TypeCode=RunTimeClass(vr->Group,vr->Element);
			}
			Pack(vr->TypeCode, s1[0], s1[1]);
			VRBuffer << s1[0];VRBuffer<<s1[1];
			
			if(	(vr->TypeCode=='OB')||
				(vr->TypeCode=='OW')||
				(vr->TypeCode=='OF')||
				(vr->TypeCode=='UN')||
				(vr->TypeCode=='UT')||
				(vr->TypeCode=='SQ'))
				{
				Length16 = 0;
				VRBuffer << Length16;	// Toss in 16 bits
				if(vr->SQObjectArray)
					{
					tuint32 = 0xffffffff;
					VRBuffer << tuint32;
					}
				else
					VRBuffer << vr->Length;	// 32 bit Length
				}
			else
				{
				Length16 = (UINT16) vr->Length;
				VRBuffer << Length16;
				}
			}
		else
			{
			if(vr->SQObjectArray)
				{
				tuint32 = 0xffffffff;
				VRBuffer << tuint32;
				}
			else
				VRBuffer << vr->Length;
			}
	

		if(vr->Length&&vr->Data)
			VRBuffer.Write ( (BYTE *) vr->Data, vr->Length-A1Flag );

		
		if(A1Flag)
			{
			VRBuffer.Write((BYTE*) "\0", 1);
			--vr->Length;
			A1Flag = 0;
			}

		if(vr->SQObjectArray)
			{
			Array<DICOMDataObject*>	*ArrayPtr =
				(Array<DICOMDataObject *> *) vr->SQObjectArray;

			if ( (vr->Group == 0x7fe0 )&&
				 (vr->Element == 0x0010 ))
				{
				ParseImagePixelDataDCMToRaw(ArrayPtr, VRBuffer);
				delete vr;
				continue;
				}

			Index = 0;
			while ( Index < ArrayPtr->GetSize() )
				{
				vr->Group = 0xfffe;
				vr->Element = 0xe000;
				vr->Length = 0xffffffff;
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;

				if((Mode==TSA_EXPLICIT_LITTLE_ENDIAN)||
					(Mode==TSA_EXPLICIT_BIG_ENDIAN))
					{
					Explicit_ParseDCMIntoRawVR(
						ArrayPtr->Get(Index), VRBuffer);
					}
				else
					{
					Implicit_ParseDCMIntoRawVR(
						ArrayPtr->Get(Index), VRBuffer);
					}
				vr->Group = 0xfffe;
				vr->Element = 0xe00d;
				vr->Length = 0;
				VRBuffer << vr->Group;
				VRBuffer << vr->Element;
				VRBuffer << vr->Length;
				++Index;
				}
			vr->Group = 0xfffe;
			vr->Element = 0xe0dd;
			vr->Length = 0;
			VRBuffer << vr->Group;
			VRBuffer << vr->Element;
			VRBuffer << vr->Length;
			}

		if((vr->Group == 0x0002)&&
			(vr->Element==0x0010))
				vrTRN = vr;
		else			
			delete vr;
		}
	if(vrTRN)
		delete	vrTRN;

	return nImgNumOfSeries;
	}


	
char* PDU_Service :: GetTransferSyntaxUID (UINT TransferSyntaxMode)
{
	
	UINT			Index;
	char			*TransferSyntax;

	Index = 0;
	while(TransferSyntaxAliases[Index].TransferSyntaxUID)
	{
		if(TransferSyntaxMode == (UINT)TransferSyntaxAliases[Index].Alias)
		{
			TransferSyntax = (char *)malloc(64+1);
			memset(TransferSyntax, 0, (64+1));
			strcpy(TransferSyntax, TransferSyntaxAliases[Index].TransferSyntaxUID);
			break;
		}
		++Index;
	}
	return (TransferSyntax);
}
