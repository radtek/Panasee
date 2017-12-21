
#	include	"dicom.hpp"
BOOL	StandardQuery	::	Read (
	PDU_Service			*PDU,
	DICOMCommandObject	*DCO)
	{
	UID	MyUID, uid;
	VR	*vr;
	DICOMDataObject	DDO;
	Array < DICOMDataObject *>	ADDO;
	UINT		Index;

	GetUID(MyUID);

	if( ! PDU )
		return ( FALSE );

	if( ! DCO )
		return ( FALSE );

	vr = DCO->GetVR(0x0000, 0x0002);
	SetUID(uid, vr);
	if (!(MyUID == uid))
		return ( FALSE );
	if (! CFindRQ :: Read (DCO, PDU, &DDO) )
		return ( FALSE ); // my SOP, but wrong command
    
	if (! SearchOn (&DDO, &ADDO) )
		{
		CFindRSP :: Write (PDU, DCO, 0xc001, NULL);
	//	delete DCO;
		return ( TRUE );
		}
	Index = 0;
	while ( Index < ADDO.GetSize() )
		{
		CFindRSP :: Write (PDU, DCO, ADDO.Get ( Index ) );
		delete ADDO.Get ( Index );
		++Index;
		}
	CFindRSP :: Write ( PDU, DCO, NULL );
//	delete DCO;
	
	return ( TRUE );
	}

BOOL	StandardQuery	::	Write (
	PDU_Service		*PDU,
	DICOMDataObject	*DDO)
	{
	DICOMCommandObject	*DCO;
	DICOMDataObject		*RDDO;

	if ( ! PDU )
		return ( FALSE );

	if ( ! CFindRQ :: Write ( PDU, DDO ) )
		return ( FALSE );

	CallBack ( NULL, DDO );

	DCO = new DICOMCommandObject;

	while ( PDU->Read ( DCO ) )
		{
		RDDO = new DICOMDataObject;

		if (! (CFindRSP :: Read ( DCO, PDU, RDDO) ) )
			{
			return ( FALSE );
			}
		if ( DCO->GetUINT16(0x0000, 0x0800) == 0x0101)
			{
			CallBack ( DCO, NULL );
			delete RDDO;
			delete DCO;
			return ( TRUE );
			}
		CallBack ( DCO, RDDO );

/*********************ADD BY JWANG**************************/
		UINT16 Status;
		Status = DCO->GetUINT16(0x0000, 0x0900);

		delete RDDO;
		delete DCO;

		if(Status == 0xa700) {
			SetLastError(QUERY_REFUSED_OUTOFRESOURCE);
			return ( FALSE );
		} else if(Status == 0xa900) {
			SetLastError(QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS);
			return ( FALSE );
		} else if((Status&0xf000) == 0xc000) {
			SetLastError(QUERY_FAILED_UNABLETOPROCESS);
			return ( FALSE );
		} else if(Status == 0xfe00) {
			SetLastError(QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST);
			return ( FALSE );
		} else if(Status == 0xff00) {
			SetLastError(QUERY_PENDING_ALLOPTIONKEYSWERESUPPORTED);
		} else if(Status == 0xff01) {
			SetLastError(QUERY_PENDING_SOMEOPTIONKEYSWERENOTSUPPORTED);
		}
/******************************************************************/

		DCO = new DICOMCommandObject;
		}

	delete DCO;
	return ( FALSE );
	}
/********************ADD BY JWANG**********************/
void StandardQuery::SetLastError(UINT32 nLastError)
{
	 LastError = nLastError;
}

UINT32 StandardQuery::GetLastError()
{
	UINT32 nLastError = LastError;
	LastError = QUERY_SUCCESS;
	return nLastError;
	
}
/*********************************************************/

BOOL	StandardRetrieve	::	Read (
	PDU_Service			*PDU,
	DICOMCommandObject	*DCO)
	{
	UID	MyUID, uid, iUID, AppUID ("1.2.840.10008.3.1.1.1");
	VR	*vr;
	DICOMDataObject	DDO;
	Array < DICOMDataObject *>	ADDO;
	UINT		Index;
	BYTE		IP [ 64 ], Port [ 64 ], ACRNema [ 17 ], MyACR[17];
	StandardStorage *SS;
	DICOMDataObject	*iDDO;
	PDU_Service	NewPDU;
//	NewPDU.AttachRTC();
	UINT16		Failed;

	GetUID(MyUID);

	if( ! PDU )
		return ( FALSE );

	if( ! DCO )
		return ( FALSE );

	vr = DCO->GetVR(0x0000, 0x0002);
	SetUID(uid, vr);
	if (!(MyUID == uid))
		return ( FALSE );

	if (! CMoveRQ :: Read (DCO, PDU, &DDO) )
		return ( FALSE ); // my SOP, but wrong command


	vr = DCO->GetVR(0x0000, 0x0600);
	if(!vr)
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc001 , 0, 0, 0, 0, NULL );
//		delete DCO;
		return ( TRUE );
		}

	memset((void*)ACRNema, 0, 17);
	if(vr->Length > 16)
		vr->Length = 16;
	memcpy((void*)ACRNema, vr->Data, (int) vr->Length);
	if(!vr->Length)
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc002 , 0, 0, 0, 0, NULL );
//		delete DCO;
		return ( TRUE );
		}
	if(ACRNema[vr->Length-1]==' ')
		ACRNema[vr->Length-1] = '\0';
	MTrimRight((char*)ACRNema);
	if(!QualifyOn(ACRNema, MyACR, IP, Port))
		{
		struct   sockaddr_in  peer;
		int nLen=sizeof(peer);
		getpeername(PDU->Link.Socketfd,(struct sockaddr*)&peer,&nLen);
		strcpy((char*)MyACR,"COOLPACS");
		strcpy((char*)IP,inet_ntoa(peer.sin_addr));
		strcpy((char*)Port,"104");
/*
		CMoveRSP :: Write ( PDU, DCO, 0xc003 , 0, 0, 0, 0, NULL );
//		delete DCO;
		return ( TRUE );
*/		}


	if (! SearchOn (&DDO, &ADDO) )
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc004 , 0, 0, 0, 0, NULL );
//		delete DCO;
		return ( TRUE );
		}

	NewPDU.SetApplicationContext ( AppUID );
	NewPDU.SetLocalAddress ( MyACR );
	NewPDU.SetRemoteAddress ( ACRNema );
	
	// Add all the Abstract Syntaxs we need

	Index = 0;
	while ( Index < ADDO.GetSize() )
		{
		vr = ADDO.Get ( Index ) -> GetVR(0x0008, 0x0016);
		if(!vr)
			{
			delete ADDO.Get ( Index );
			ADDO.RemoveAt ( Index );
			}
		else
			{
			SetUID ( iUID, vr );
			NewPDU.AddAbstractSyntax ( iUID );
			++Index;
			}
		}

	if (!NewPDU.Connect (IP, Port))
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc005 , 0, 0, 0, 0, NULL );
//		delete DCO;
		return ( TRUE );
		}


	Index = 0;
	Failed = 0;
	while ( Index < ADDO.GetSize() )
	{
		vr = ADDO.Get ( Index ) -> GetVR(0x0008, 0x0016);
		SetUID ( iUID, vr );
		if ( !NewPDU.GetPresentationContextID(iUID) )
		{
			++Failed;
			// Remote end did not accept this UID
		}
		else
		{
			if ( !RetrieveOn (ADDO.Get(Index), &iDDO, &SS))
			{
				++Failed;
				vr=ADDO.Get(Index)->GetVR(0x0004,0x1500);
				if(vr)
				{
					char cFileName[MAX_PATH];
					memset(cFileName,0,MAX_PATH);
					memcpy(cFileName,vr->Data,vr->Length);
					if(cFileName[vr->Length-1]==' ')
						cFileName[vr->Length-1]='\0';
					MTrimRight(cFileName);
				}
			}
			else
			{
				try
				{
					if(!SS->Write(&NewPDU, iDDO))
						{
						//++Failed;
						// Remote end should accept this image.  if it did
						// not, then just bail out.  Probably means the
						// TCP/IP link has been dropped.
						Failed += (ADDO.GetSize() - Index);
						break;
						}
				}
				catch(...)
				{
					Failed ++;
				}
				delete iDDO;
			}
		}
		CMoveRSP :: Write ( PDU,
							DCO,
							0xff00, ADDO.GetSize() - Index - 1,
							(UINT16) Index, Failed, 0,
							ADDO.Get ( Index ));
		delete ADDO.Get ( Index );
		++Index;
	}
	CMoveRSP :: Write ( PDU, DCO, 0, 0, (UINT16) Index, Failed, 0, NULL );
	// Incase we broke out from above..
	while ( Index < ADDO.GetSize () )
	{
		delete ADDO.Get(Index);
		++Index;
	}
//	delete DCO;
	
	return ( TRUE );
	}

//BOOL	DumpVR (VR	*);

BOOL	StandardRetrieve	::	Write (
	PDU_Service		*PDU,
	DICOMDataObject	*DDO,
	BYTE			*ACRNema )
	{
	DICOMCommandObject	*DCO;
	DICOMDataObject		*RDDO;

	if ( ! PDU )
		return ( FALSE );

	if ( ! CMoveRQ :: Write ( PDU, DDO, ACRNema ) )
		return ( FALSE );

	CallBack ( NULL, DDO );

	DCO = new DICOMCommandObject;

	while ( PDU->Read ( DCO ) )
		{
		RDDO = new DICOMDataObject;

		if (! (CMoveRSP :: Read ( DCO, PDU, RDDO) ) )
			{
			return ( FALSE );
			}
		if ( DCO->GetUINT16(0x0000, 0x0800) == 0x0101)
			{
			CallBack ( DCO, NULL );
			delete RDDO;
			if ( DCO->GetUINT16(0x0000, 0x0900) != 0x0000)
				{
			//	VR *vr;
			//	while (vr = DCO->Pop())
			//		{
			//		//DumpVR(vr);
			//		delete vr;
			//		}
				delete DCO;
				return ( FALSE );
				}
			delete DCO;
			return ( TRUE );
			}
		CallBack ( DCO, RDDO );
/*********************ADD BY JWANG**************************/
		UINT16 Status;
		Status = DCO->GetUINT16(0x0000, 0x0900);

		if(Status == 0xa701) 
		{
			SetLastError(RETRIEVE_REFUSED_UNABLETOCALCULATENUMBEROFMATCHS);
			return ( FALSE );
		}
		else if(Status == 0xa702) 
		{
			SetLastError(RETRIEVE_REFUSED_UNABLETOPERFORMSUBOPERATIONS);
			return ( FALSE );
		}
		else if(Status == 0xa801) 
		{
			SetLastError(RETRIEVE_REFUSED_MOVEDESTINATIONUNKNOWN);
			return ( FALSE );
		}
		else if(Status == 0xa900) 
		{
			SetLastError(RETRIEVE_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS);
			return ( FALSE );
		} 
		else if((Status&0xf000) == 0xc000) 
		{
			SetLastError(RETRIEVE_FAILED_UNABLETOPROCESS);
			return ( FALSE );
		} 
		else if(Status == 0xfe00) 
		{
			SetLastError(RETRIEVE_CANCEL_SUBOPERATIONSTERMINATED);
			return ( FALSE );
		} 
		else if(Status == 0xb000) 
		{
			SetLastError(RETRIEVE_WARNING_SUBOPERATIONCOMPLETE);
		} 
		else if(Status == 0xff00) 
		{
			SetLastError(RETRIEVE_PENDING_SUBOPERATIONSARECONTINUING);
		}
/******************************************************************/

		delete RDDO;
		delete DCO;
		DCO = new DICOMCommandObject;
		}

	delete DCO;
	return ( FALSE );
	}
/********************ADD BY JWANG**********************/
void StandardRetrieve::SetLastError(UINT32 nLastError)
{
	 LastError = nLastError;
}

UINT32 StandardRetrieve::GetLastError()
{
	UINT32 nLastError = LastError;
	LastError = RETRIEVE_SUCCESS;
	return nLastError;
	
}
/*********************************************************/

BOOL	PatientRootQuery	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.1.1" );
	return ( TRUE );
	}

BOOL	PatientRootRetrieve	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.1.2" );
	return ( TRUE );
	}

BOOL	StudyRootQuery	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.2.1" );
	return ( TRUE );
	}

BOOL	StudyRootRetrieve	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.2.2" );
	return ( TRUE );
	}

BOOL	PatientStudyOnlyQuery	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.3.1" );
	return ( TRUE );
	}

BOOL	PatientStudyOnlyRetrieve	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.3.2" );
	return ( TRUE );
	}
BOOL	ModalityWorklistQuery	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.31" );
	return ( TRUE );
	}
