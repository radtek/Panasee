#	include	"dicom.hpp"

// Rather useful routine.  Sets up a uid from a vr containing the vr

BOOL	SetUID ( UID	&uid, VR	*vr)
	{
	if(!vr)
		return ( FALSE );
	if(!vr->Data)
		return ( FALSE );
	if(vr->Length > 64 )
		return ( FALSE );
	memcpy((void*)uid.GetBuffer(vr->Length), (void*)vr->Data, vr->Length);

	// This ensures that rounded up lengths don't pass garbage in that
	// excess byte.  They shouldn't be doing it, but if they do...
	uid.SetLength(vr->Length);
	if(vr->Length==0)
		return TRUE;
	switch (uid.GetBuffer(1) [ vr->Length - 1 ] )
		{
		case	'0':
		case	'1':
		case	'2':
		case	'3':
		case	'4':
		case	'5':
		case	'6':
		case	'7':
		case	'8':
		case	'9':
		case	'.':
			return ( TRUE );
		}
	uid.GetBuffer(1) [ vr->Length - 1 ] = '\0';
	uid.SetLength (vr->Length - 1);
	return ( TRUE );
	}

BOOL	Verification :: GetUID ( UID &uid )
	{
	uid.Set("1.2.840.10008.1.1");
	return ( TRUE );
	}

BOOL	Verification :: Read ( PDU_Service *PDU, DICOMCommandObject *DCO)
	{
	VR	*vr;
	UID	MyUID, uid;

	GetUID(MyUID);	// always try and use GetUID to obtain my own uid

	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );

	vr = DCO->GetVR ( 0x0000, 0x0002 );
	if ( ! vr )
		return ( FALSE );

	if ( !SetUID(uid, vr) )
		return ( FALSE );

	if ( MyUID != uid )
		return ( FALSE );

	if ( CEchoRQ :: Read ( DCO ) )
		{
		return ( CEchoRSP :: Write ( PDU, DCO ) );
		}
	if ( CEchoRSP :: Read ( DCO ) )
		{
		// No worries..
		return ( TRUE );
		}
	return ( FALSE );	// woa.. corrupted message
	}

BOOL	Verification :: Write ( PDU_Service *PDU )
	{
	DICOMCommandObject	DCO;

	if ( ! PDU )
		return ( FALSE );

	if ( ! CEchoRQ :: Write ( PDU ) )
		return ( FALSE );

	if(!PDU->Read ( & DCO ))
		return (FALSE);

	if ( ! CEchoRSP :: Read ( &DCO ) )
		return ( FALSE );
	return ( TRUE );
	}

		
