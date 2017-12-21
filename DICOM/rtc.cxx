
/*
19980330    ljz    Added an RTC memberfunction for loading the dicom
                   library from memory (e.g. a resource).
19980331    ljz    Added function for writing binary RTCs (function was
                   originally implemeted in 'makebrtc.cxx'
19980414    ljz    Added RTC::GetGroupElement memberfunction, to retrieve the
                   Group and Element numbers using a library-keyword.
*/


/**********************************************************************
 *
 * Run-Time-Class support
 *
 **********************************************************************/
#	include	"dicom.hpp"

#ifdef	WINDOWS
#	include	<stdlib.h>
#endif

#ifdef	SOLARIS
#	include	<stdlib.h>
#endif

BOOL
Pack(
	BYTE	c1,
	BYTE	c2,
	UINT16	&u1)
	{

	if('OB'==20290)
		u1 = (((UINT16)c1)<<8)+((UINT16)c2);
	else
		u1 = (((UINT16)c2)<<8)+((UINT16)c1);

	return ( TRUE );
	}

BOOL
Pack(
	UINT16	u1,
	BYTE	&c1,
	BYTE	&c2)
	{
	if('OB'==20290)
		{
		c1 = (u1 >> 8);
		c2 = (u1 & 0xff);
		}
	else
		{
		c2 = (u1 >> 8);
		c1 = (u1 & 0xff);
		}
	return ( TRUE );
	}
RTC	::	RTC	(
	BOOL	CD)
	{
	TypeCodes = NULL;
	CarryDescriptions = CD;
	AttachMemRTC();
}
RTC	::	~RTC ()
	{
	DetachRTC ();
	}

BOOL
RTC	::	DetachRTC ()
	{
	if ( TypeCodes )
	{
		delete TypeCodes;
	}
	TypeCodes = NULL;
	return ( TRUE );
	}

extern RTCElement RTCMem[];
extern int nRTCMem;
BOOL RTC	::	AttachMemRTC ()
{
	DetachRTC();
	int nElem=nRTCMem;
	TypeCodes = new FixedArray < UINT32, RTCElement > (nElem, TRUE);
	if ( ! TypeCodes )
		return ( FALSE );
	UINT32	GCode;
	for(int i=0;i<nElem;i++)
	{
		GCode = MAKEUINT32(RTCMem[i].Group,RTCMem[i].Element);
		TypeCodes->Add(GCode, RTCMem[i]);
	}
	TypeCodes->Sort();
	return ( TRUE );	
}

UINT16
RTC	::	RunTimeClass (
	UINT16	Group,
	UINT16	Element,
	char	*Desc)
	{
	INT			Index;
	UINT32		GCode;
	RTCElement	VRT;

	if ( Desc )
		(*Desc) = '\0';

	if ( ! TypeCodes )
		return ( 0 );

	GCode = MAKEUINT32(Group, Element);
	Index = TypeCodes->IndexOf(GCode);
	if ( Index < 0)
		return ( 0 );

	VRT = TypeCodes->Get(Index);
	if(Desc)
		if(VRT.Description)
			strcpy(Desc, VRT.Description);

	return (VRT.TypeCode);
	}

BOOL
RTC	::	RunTimeClass(VR	*vr)
	{
	UINT16	TypeCode = RunTimeClass(vr->Group, vr->Element, NULL);

	vr->TypeCode = TypeCode;
	if ( TypeCode )
		return ( TRUE );
	return ( FALSE );
	}

BOOL
RTC	::	RunTimeClass(DICOMObject	*DO)
	{
	DICOMObject	TempObject;
	VR			*vr;

	while ( vr = DO->Pop() )
		{
		RunTimeClass(vr);
		TempObject.Push(vr);
		}
	while ( vr = TempObject.Pop() )
		{
		DO->Push(vr);
		}
	return ( TRUE );
	}

BOOL
RTC ::  GetGroupElement(RTCElement* pEntry)
{
	int			iArraySize;
	int			i;
	RTCElement	Entry;

	iArraySize = TypeCodes->GetSize();
	for (i=0; i< iArraySize; i++)
	{
		Entry = TypeCodes->Get(i);
		if (Entry.Description)
			if (strcmp(Entry.Description, pEntry->Description) == 0)
				break;
	}
	if (i == iArraySize)
		return FALSE;
	pEntry->Group = Entry.Group;
	pEntry->Element = Entry.Element;
	return TRUE;

}
