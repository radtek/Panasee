#	include	"dicom.hpp"
#	include	"iod.hpp"

VRReferentialIntegrityDatabaseElement	::
	VRReferentialIntegrityDatabaseElement ()
	{
	Group = 0xffff;
	Element = 0xffff;
	MasterVR = NULL;
	VRReferences = NULL;
	}

VRReferentialIntegrityDatabaseElement	::
	~VRReferentialIntegrityDatabaseElement ()
	{
	if ( VRReferences )
		delete VRReferences;
	}

BOOL
VRReferentialIntegrityDatabaseElement	::	SetMaster (
	VR	*vr,
	VRReferentialIntegrityDatabase	*VRED)
	{
//	UINT	Index;

	//printf("**SET MASTER %x %4.4x, %4.4x, %d\n",
	//	vr, vr->Group, vr->Element, vr->Length);fflush(stdout);

	if(vr)
		{
		Group = vr->Group;
		Element = vr->Element;
		vr->ReferentialIntegrityDatabase = VRED;
		}
	MasterVR = vr;
	Sync ();
	return ( TRUE );
	}

BOOL
VRReferentialIntegrityDatabaseElement	::	AddReference (
	VR	*vr,
	VRReferentialIntegrityDatabase	*VRED)
	{
	if (! VRReferences )
		VRReferences = new Array < VR * >;

	VRReferences->Add ( vr );
	vr->ReferentialIntegrityDatabase = VRED;
	if ( MasterVR )
		{
		vr->Data = MasterVR->Data;
		vr->Length = MasterVR->Length;
		vr->ReleaseMemory = FALSE;
		//vr->ReferentialIntegrityDatabase = VRED;
		}
	return ( TRUE );
	}

BOOL
VRReferentialIntegrityDatabaseElement	::	Sync ()
	{
	UINT	Index;
	VR		*vr;

	//printf("*** SYNC: %4.4x/%4.4x\n", Group, Element);fflush(stdout);
	
	if ( ! VRReferences )
		return ( TRUE );

	Index = 0;
	while ( Index < VRReferences->GetSize() )
		{
		vr = VRReferences->Get ( Index );
		//printf("\tReference: %d / %x\n", Index, vr);fflush(stdout);
		if ( MasterVR )
			{
			vr->Data = MasterVR->Data;
			vr->Length = MasterVR->Length;
			vr->ReleaseMemory = FALSE;
			//vr->ReferentialIntegrityDatabase = this;
			}
		else
			{
			vr->Data = NULL;
			vr->Length = 0;
			vr->ReleaseMemory = FALSE;
			}
		++Index;
		}
	return ( TRUE );
	}

/************ Referential Integrity Database *********/

VRReferentialIntegrityDatabase	::	VRReferentialIntegrityDatabase ()
	{
	AmBuilding = TRUE;
	VRArray = NULL;
	}

VRReferentialIntegrityDatabase	::	~VRReferentialIntegrityDatabase ()
	{
	UnBuild ();
	while ( VRBuildArray.GetSize() )
		{
		delete VRBuildArray.Get(0);
		VRBuildArray.RemoveAt(0);
		}
	}

VRReferentialIntegrityDatabaseElement	*
VRReferentialIntegrityDatabase	::	Addframe (
	UINT16		Group,
	UINT16		Element)
	{
	UINT		Index;
	VRReferentialIntegrityDatabaseElement	*VREF;

	//printf("***Addframe ( %4.4x, %4.4x )\n", Group, Element);fflush(stdout);
	UnBuild ();
	// Make sure the master isn't already in the database
	

	Index = 0;
	while ( Index < VRBuildArray.GetSize() )
		{
		VREF = VRBuildArray.Get(Index);
		if (VREF->Group == Group)
		if (VREF->Element == Element)
			return ( VREF );
		++Index;
		}
	VRReferentialIntegrityDatabaseElement	*VRE =
		new	VRReferentialIntegrityDatabaseElement;
	VRE->Group = Group;
	VRE->Element = Element;
	VRE->MasterVR = NULL;
	VRBuildArray.Add(VRE);
	return ( VRE );
	}

BOOL	VRReferentialIntegrityDatabase	::	AddReference (
	VR		*vr)
	{
	VRReferentialIntegrityDatabaseElement	*VRE;	
	// Add/Find the frame
	VRE = Addframe(vr->Group, vr->Element);
	if ( ! VRE )
		return ( FALSE );

	VRE->AddReference( vr, this );
	return ( TRUE );
	}

BOOL	VRReferentialIntegrityDatabase	::	Sync ()
	{
	UINT		Index;

	// We hardly ever should sync the entire database.. but just
	// Incase
	Build ();
	if ( ! VRArray )
		return ( FALSE );
	Index = 0;
	while ( Index < VRArray->GetSize() )
		{
		VRArray->Get(Index)->Sync ();
		++Index;
		}
	return ( TRUE );
	}

BOOL	VRReferentialIntegrityDatabase	::	Sync (
	UINT16	Group,
	UINT16	Element)
	{
	VRReferentialIntegrityDatabaseElement	*VREF;
	INT		Index;
	UINT32	Temp;

	Build ();

	if(!VRArray)
		return ( FALSE );

	// Note this Temp = / etc. is for gcc which complains needlessly ;-(
	Temp = MAKEUINT32 ( Group, Element );
	Index = VRArray->IndexOf ( Temp );

	if ( Index == (-1) )
		return ( FALSE );	// Not to be synced!!

	VREF = VRArray->Get(Index);

	if(!VREF)
		return ( FALSE );

	return(VREF->Sync());
	}

BOOL	VRReferentialIntegrityDatabase	::	Build ()
	{
	if (!AmBuilding)
		return ( TRUE );	// Already built

	//printf("***BUILDING VRRID\n\n");fflush(stdout);
	// take each VREF and move it into a fixed array.

	if ( ! VRBuildArray.GetSize() )
		return ( FALSE );	// Not anything to build
	VRArray = new FixedArray < UINT32,
					VRReferentialIntegrityDatabaseElement * >
					(VRBuildArray.GetSize(), TRUE);

	while ( VRBuildArray.GetSize() )
		{
		UINT32		Temp;

		Temp = MAKEUINT32 ( VRBuildArray.Get(0)->Group,
							VRBuildArray.Get(0)->Element );
		VRArray->Add( Temp,
					VRBuildArray.Get(0));
		//printf("Building: %4.4x, %4.4x\n", VRBuildArray.Get(0)->Group,
		//	VRBuildArray.Get(0)->Element);fflush(stdout);
		VRBuildArray.RemoveAt(0);
		}
	//printf("***SORTING VRRID\n\n");fflush(stdout);
	VRArray->Sort ();	// Lock
/*	UINT Index = 0;
	while ( Index < VRArray->Top )
		{
		printf("[%8.8x]\n", VRArray->KeyTable[Index]);
		++Index;
		}	*/
	AmBuilding = FALSE;
	//printf("***BUILD COMPLETE VRRID\n\n");fflush(stdout);
	return ( TRUE );
	}
	

BOOL	VRReferentialIntegrityDatabase	::	UnBuild ()
	{
	UINT		Index;
	if(AmBuilding)
		return ( TRUE );	// Already unbuilt

	if ( ! VRArray )
		return ( TRUE );	// Nothing to unbuild

	Index = 0;
	while ( Index < VRArray->GetSize())
		{
		VRBuildArray.Add(VRArray->Get(Index));
		++Index;
		}
	delete VRArray;
	VRArray = NULL;
	AmBuilding = TRUE;
	return ( TRUE );
	}


BOOL	VRReferentialIntegrityDatabase	::	SetMaster (
	UINT16		Group,
	UINT16		Element,
	void		*Data,
	UINT32		Length)
	{
	VRReferentialIntegrityDatabaseElement	*VREF;
	INT		Index;
	UINT32	Temp;

	Build ();

	if(!VRArray)
		return ( FALSE );

	Temp = MAKEUINT32 ( Group, Element );
	Index = VRArray->IndexOf ( Temp );

	if ( Index == (-1) )
		return ( FALSE );	// Not to be synced!!

	VREF = VRArray->Get(Index);

	if(!VREF)
		return ( FALSE );

	if(!VREF->MasterVR)
		{
		if(!LinkUp)
			return ( FALSE );
		VR	*vr = new VR ( Group, Element, (UINT32) 1, (BOOL) TRUE );
		delete vr->Data;
		vr->Data = Data;
		vr->Length = Length;

		LinkUp->Push(vr);
		return ( TRUE );
		}
	if(VREF->MasterVR->ReleaseMemory)
		delete VREF->MasterVR->Data;
	VREF->MasterVR->Data = Data;
	VREF->MasterVR->Length = Length;
	VREF->MasterVR->ReleaseMemory = TRUE;
	VREF->Sync ();
	return ( TRUE );
	}

BOOL	VRReferentialIntegrityDatabase	::	SetMaster (
	VR	*vr)
	{
	VRReferentialIntegrityDatabaseElement	*VREF;
	INT		Index;
	UINT32	Temp;

	if ( ! vr )
		return ( FALSE );


	Build ();

	if(!VRArray)
		return ( FALSE );

	Temp = MAKEUINT32 ( vr->Group, vr->Element );
	Index = VRArray->IndexOf ( Temp );

	//printf("SetMaster ( vr ) : %4.4x, %4.4x, %d\n",
	//	vr->Group, vr->Element, Index);fflush(stdout);
	if ( Index == (-1) )
		return ( FALSE );	// Not to be synced!!

	VREF = VRArray->Get(Index);

	if(!VREF)
		return ( FALSE );

	if(VREF->MasterVR)
		{
		delete VREF->MasterVR;
		// Note fall through
		}
	vr->ReferentialIntegrityDatabase = this;
	VREF->MasterVR = vr;
	VREF->Sync ();
	//vr->ReferentialIntegrityDatabase = this;
	return ( TRUE );
	}

BOOL	VRReferentialIntegrityDatabase	::	ClearMaster (
	UINT16		Group,
	UINT16		Element)
	{
	VRReferentialIntegrityDatabaseElement	*VREF = GetVREF(Group, Element);

	if ( ! VREF )
		return ( FALSE );

	//if ( VREF->MasterVR )
	//	delete VREF->MasterVR;

	VREF->MasterVR = NULL;
	VREF->Sync ();
	return ( TRUE );
	}

VRReferentialIntegrityDatabaseElement	*
VRReferentialIntegrityDatabase	::	GetVREF (
	UINT16		Group,
	UINT16		Element)
	{
	VRReferentialIntegrityDatabaseElement	*VREF;
	INT		Index;
	UINT32	Temp;

	Build ();

	if(!VRArray)
		return ( FALSE );

	Temp = MAKEUINT32 ( Group, Element );
	Index = VRArray->IndexOf ( Temp );

//	printf("IndexOf(%4.4x, %4.4x, =%d)\n", Group, Element, Index);fflush(stdout);
	if ( Index == (-1) )
		return ( FALSE );	// Not to be synced!!

	VREF = VRArray->Get(Index);

	return ( VREF );
	}

	

