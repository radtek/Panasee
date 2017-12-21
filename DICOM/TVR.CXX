#	include	"dicom.hpp"
#	include	"iod.hpp"

UINT16
MAKEUINT16 (
	BYTE	x1,
	BYTE	x2)
	{
	UINT16	u1;
	
	Pack (x1, x2, u1);
	return ( u1 );
	}
	
UINT
VR_XXMULTISTRING	::	VMCount ()
	{
	UINT	Count;
	UINT	Index;
	char	*cData;

	if ( ! Data )
		return ( 0 );
	
	Count = 1;
	Index = 0;
	cData = (char*) Data;
	while ( Index < Length )
		{
		if ( (*cData) == '\\' )
			++Count;
		++cData;
		++Index;
		}
	return ( Count );
	}

BOOL
VR_XXMULTISTRING	::	VMGet (
	UINT	Index,
	char	*string)
	{
	UINT	Count, cIndex, sLength;
	char	*cData;
	char	*ostring;

	++Index;
	ostring = string;

	if ( ! Data )
		return ( FALSE );

	cData = (char*) Data;
	Count = 1;
	cIndex = 0;
	while ( Count < Index )
		{
		if(cIndex >= Length)
			return ( FALSE );
		if((*cData)=='\\')
			++Count;
		++cIndex;
		++cData;
		}
	sLength = 0;
	while ( cIndex < Length )
		{
		if((*cData)=='\\')
			break;
		(*string) = (*cData);
		++string;
		++cData;
		++cIndex;
		++sLength;
		}
	(*string) = '\0';
	PostProcGetData(sLength, ostring);
	return ( TRUE );
	}

BOOL
VR_XXMULTISTRING	::	VMSet(
	UINT	Index,
	char	*string)
	{
	UINT	VMC, FLength, BLength, iLength;
	UINT	iStringLength;
	UINT	stringLength;
	char	*iString;
//	char	*oString;
	char	*cData;
	char	*c1Data;
	void	*vData;

	++Index;
	stringLength = strlen(string);
	iStringLength = GetSetSize();
	if ( !iStringLength)
		iStringLength = stringLength;
	iString = new char[iStringLength];
	memset((void*)iString, ' ', iStringLength);
	if ( iStringLength < stringLength )
		memcpy((void*)iString, (void*)string, iStringLength);
	else
		memcpy((void*)iString, (void*)string, stringLength);
	PostProcSetData(iStringLength, iString);
	// Now, we must insert this into the VM.  First thing
	// is locate the slot1\slot2\...\Index\... insertion
	// point, and the corresponding finishing point.

	VMC = VMCount ();
	if ( Index > VMC)
		{
		if(!VMC)
			{
			VMC = 1;
			FLength = iStringLength + Index - 1;
			}
		else
			{
			FLength = iStringLength + Length + (Index - VMC - 1);
			++VMC;
			}
		vData = new char [ FLength ];
		if ( Data )
			memcpy ( vData, Data, Length );
		cData = (char*) vData;
		while((VMC<Index)&&(Index!=1))
			{
			cData [ Length ] = '\\';
			++VMC;
			++Length;
			}
		//printf("%s\n", cData);
		memcpy(&cData[Length], iString, iStringLength);
		//printf("%s\n", cData);
		Length = FLength;
		if ( ReleaseMemory )
			delete Data;
		//ReleaseMemory = TRUE;
		Data = vData;
		delete iString;
		if(ReferentialIntegrityDatabase)
			((VRReferentialIntegrityDatabase*)ReferentialIntegrityDatabase)
				->SetMaster(Group, Element, Data, Length);
		else
			ReleaseMemory = TRUE;
		return ( TRUE );
		}
	
	// Ok, no, this is not an append.  We are replacing something in
	// the middle.  In this situation we need to figure out the length
	// of the beginning, length of the end, and add the middle piece

	cData = (char*) Data;
	VMC = 1;
	BLength = 0;
	FLength = 0;
	iLength = 0;
	while ( VMC < Index )
		{
		if ((*cData) == '\\')
			++VMC;
		++BLength;
		++cData;
		++iLength;
		}
	while ( VMC == Index )
		{
		if ( iLength >= Length )
			break;
		if((*cData) == '\\')
			++VMC;
		++iLength;
		++cData;
		}
	FLength = Length - iLength;
	if(FLength)
		++FLength;
	vData = new char [ BLength + FLength + iStringLength];
	c1Data = (char*) vData;
	memcpy(vData, Data, BLength);
	memcpy((void*)&c1Data[BLength], iString, iStringLength);
	if ( FLength)
		{
		c1Data [ BLength + iStringLength ] = '\\';
		memcpy((void*)&c1Data [ BLength + iStringLength + 1],
						cData, FLength - 1);
		}
	Length = BLength + FLength + iStringLength;
	if ( ReleaseMemory )
		delete Data;

	delete iString;
	Data = vData;
	if(ReferentialIntegrityDatabase)
		((VRReferentialIntegrityDatabase*)ReferentialIntegrityDatabase)
			->SetMaster(Group, Element, Data, Length);
	else
		ReleaseMemory = TRUE;
	return ( TRUE );
	}


BOOL
VR_XXSTRING	::	Set(
	char	*string)
	{
	return(VMSet(0, string));
	}

BOOL
VR_XXSTRING	::	Get(
	char	*string)
	{
	return(VMGet(0, string));
	}

BOOL
VR_XXSTRING	::	PostProcSetData(
	UINT	Length,
	void	*vstring)
	{
	// Post-Process data we are setting into the VR
	UINT	Index;
	char	*string = (char*) vstring;
	
	if ( ! Length )
		return ( FALSE );	
	Index = Length;
	while ( Index )
		{
		--Index;
		if(string[Index]=='\0')
			string[Index]=' ';
		if(string[Index]==' ')
			continue;
		break;
		}
	return ( TRUE );
	}

BOOL
VR_XXSTRING	::	PostProcGetData(
	UINT	Length,
	void	*vstring)
	{
	UINT	Index;
	char	*string = (char*) vstring;

	if ( ! Length )
		return ( FALSE );

	Index = Length;
	while ( Index )
		{
		--Index;
		if(string[Index]!=' ')
			break;
		}
	string[Index+1] = '\0';
	return ( TRUE );
	}

UINT
VR_XXMULTIBINARY	::	VMCount ()
	{
	if ( ! GetSetSize () )
		return ( 0 );
	return ( Length / GetSetSize() );
	}

BOOL
VR_XXMULTIBINARY	::	VMGet(
	UINT	Index,
	void	*BData)
	{
	char	*cData;

	if ( Index > VMCount() )
		return ( FALSE );

	cData = (char*)Data;
	memcpy (BData, (void*)&cData[Index * GetSetSize()], GetSetSize());
	PostProcGetData(GetSetSize(), BData);
	return ( TRUE );
	}

BOOL
VR_XXMULTIBINARY	::	VMSet(
	UINT	Index,
	void	*BData)
	{
	char	*cData;
	void	*vData;

//	if ( Index > VMCount() ) changed by [peak]
	if ( Index > VMCount()||VMCount()==0 )
		{
		vData = new char[(Index + 1)* GetSetSize()];
		memcpy(vData, Data, Length);
		Length = (Index + 1) * GetSetSize();
//		if(ReleaseMemory) changed by [peak]
		if(ReleaseMemory&&Data)
			delete Data;
		Data = vData;
		if(ReferentialIntegrityDatabase)
			((VRReferentialIntegrityDatabase*)ReferentialIntegrityDatabase)
				->SetMaster(Group, Element, Data, Length);
		else
			ReleaseMemory = TRUE;
		}
	cData = (char*)Data;
	memcpy((void*)&cData[Index * GetSetSize()], BData, GetSetSize() );
	PostProcSetData(GetSetSize(), (void*)&cData[Index*GetSetSize()]);
	return ( TRUE );
	}

BOOL
VR_XXBINARY	::	Set(
	void	*vData)
	{
	return ( VMSet(0, vData) );
	}

BOOL
VR_XXBINARY	::	Get(
	void	*vData)
	{
	return ( VMGet(0, vData) );
	}

BOOL
VR_XXBINARY	::	PostProcSetData(
	UINT	Length,
	void	*vData)
	{
#if	NATIVE_ENDIAN	==	LITTLE_ENDIAN
	return ( TRUE );
#else
	return ( EndianSwap(Length, vData) );
#endif
	}

BOOL
VR_XXBINARY	::	PostProcGetData(
	UINT	Length,
	void	*vData)
	{
#if	NATIVE_ENDIAN	==	LITTLE_ENDIAN
	return ( TRUE );
#else
	return ( EndianSwap(Length, vData) );
#endif
	}

BOOL
VR_XXBINARY	::	EndianSwap(
	UINT	Length,
	void	*vData)
	{
	BYTE	*cData = (BYTE*)vData;
	BYTE	tData[16];

	if(Length==1)
		return ( TRUE );

	memcpy((void*)tData, vData, Length>>1);
	memcpy((void*)vData, (void*) &cData[Length>>1], Length>>1);
	memcpy((void*)&cData[Length>>1], (void*)tData, Length>>1);

	EndianSwap(Length>>1, vData);
	EndianSwap(Length>>1, (void*)&cData[Length>>1]);
	return ( TRUE );
	}

/***********************************************************************
 *
 * XX Constructors.  Rather pointless thunks.  They are here only to
 * compile under brain-dead C++ compilers (like g++).
 *
 ***********************************************************************/

VR_XXTYPED	::	VR_XXTYPED ()
	:	VR ()
	{
	}

VR_XXTYPED	::	VR_XXTYPED (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR ( g, e, l, a)
	{
	}

VR_XXTYPED	::	VR_XXTYPED (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR ( g, e, l, d)
	{
	}

VR_XXTYPED	::	VR_XXTYPED (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR ( g, e, l, d, a)
	{
	}


VR_XXMULTISTRING	::	VR_XXMULTISTRING ()
	:	VR_XXTYPED ()
	{
	}

VR_XXMULTISTRING	::	VR_XXMULTISTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, a)
	{
	}

VR_XXMULTISTRING	::	VR_XXMULTISTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXTYPED ( g, e, l, d)
	{
	}

VR_XXMULTISTRING	::	VR_XXMULTISTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, d, a)
	{
	}

VR_XXMULTIBINARY	::	VR_XXMULTIBINARY ()
	:	VR_XXTYPED ()
	{
	}

VR_XXMULTIBINARY	::	VR_XXMULTIBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, a)
	{
	}

VR_XXMULTIBINARY	::	VR_XXMULTIBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXTYPED ( g, e, l, d)
	{
	}

VR_XXMULTIBINARY	::	VR_XXMULTIBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, d, a)
	{
	}

VR_XXSTRING	::	VR_XXSTRING ()
	:	VR_XXMULTISTRING ()
	{
	}

VR_XXSTRING	::	VR_XXSTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXMULTISTRING ( g, e, l, a)
	{
	}

VR_XXSTRING	::	VR_XXSTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXMULTISTRING ( g, e, l, d)
	{
	}

VR_XXSTRING	::	VR_XXSTRING (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXMULTISTRING ( g, e, l, d, a)
	{
	}

VR_XXBINARY	::	VR_XXBINARY ()
	:	VR_XXMULTIBINARY ()
	{
	}

VR_XXBINARY	::	VR_XXBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXMULTIBINARY ( g, e, l, a)
	{
	}

VR_XXBINARY	::	VR_XXBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXMULTIBINARY ( g, e, l, d)
	{
	}

VR_XXBINARY	::	VR_XXBINARY (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXMULTIBINARY ( g, e, l, d, a)
	{
	}


/*****************
 *****************  VR_AE
 *****************/

VR_AE	::	VR_AE ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_AE	::	VR_AE (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_AE	::	VR_AE (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_AE	::	VR_AE (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_AE	::	GetSetSize ()
	{
	return ( 16 );
	}

UINT16
VR_AE	::	GetTypeCode ()
	{
	return (MAKEUINT16('A', 'E'));
	}

/*****************
 *****************  VR_AS
 *****************/

VR_AS	::	VR_AS ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_AS	::	VR_AS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_AS	::	VR_AS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_AS	::	VR_AS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_AS	::	GetSetSize ()
	{
	return ( 4 );
	}

UINT16
VR_AS	::	GetTypeCode ()
	{
	return (MAKEUINT16('A', 'S'));
	}


/*****************
 *****************  VR_AT
 *****************/

VR_AT	::	VR_AT ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_AT	::	VR_AT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_AT	::	VR_AT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_AT	::	VR_AT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_AT	::	GetSetSize ()
	{
	return ( 4 );
	}

UINT16
VR_AT	::	GetTypeCode ()
	{
	return (MAKEUINT16('A', 'T'));
	}

// Special handling.  AT codes are big-endian encoded.

BOOL
VR_AT	::	PostProcSetData(
	UINT	Length,
	void	*vData)
	{
#if	NATIVE_ENDIAN	==	BIG_ENDIAN
	return ( TRUE );
#else
	return ( EndianSwap(Length, vData) );
#endif
	}

BOOL
VR_AT	::	PostProcGetData(
	UINT	Length,
	void	*vData)
	{
#if	NATIVE_ENDIAN	==	BIG_ENDIAN
	return ( TRUE );
#else
	return ( EndianSwap(Length, vData) );
#endif
	}



/*****************
 *****************  VR_CS
 *****************/

VR_CS	::	VR_CS ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_CS	::	VR_CS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_CS	::	VR_CS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_CS	::	VR_CS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_CS	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_CS	::	GetTypeCode ()
	{
	return (MAKEUINT16('C', 'S'));
	}

/*****************
 *****************  VR_DA
 *****************/

VR_DA	::	VR_DA ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_DA	::	VR_DA (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_DA	::	VR_DA (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_DA	::	VR_DA (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_DA	::	GetSetSize ()
	{
	return ( 8 );	// fixed
	}

UINT16
VR_DA	::	GetTypeCode ()
	{
	return (MAKEUINT16('D', 'A'));
	}

BOOL
VR_DA	::	VMGet(
	UINT	Index,
	char	*Data)
	{
	char	sIndex;
	char	*cData;
	char	string[11];

	if (VR_XXMULTISTRING :: VMGet(Index, string))
		{
		// Post Process into an 8 byte string if need be.
		if (strlen(string)==8)
			{
			strcpy((char*)Data, string);
			return ( TRUE );
			}
		cData = (char*)Data;
		sIndex = 0;
		while ( string [ sIndex ] )
			{
			if(string [ sIndex ] != '.')
				{
				(*cData) = string [ sIndex ];
				++cData;
				}
			++sIndex;
			}
		(*cData) = '\0';
		return ( TRUE );
		}
	return ( FALSE );
	}

BOOL
VR_DA	::	VMSet(
	UINT	Index,
	char	*Data)
	{
	char	sIndex;
	char	*cData;
	char	string[11];

	cData = (char*)Data;
	sIndex = 0;
	while ( *cData )
		{
		if((*cData)!='.')
			{
			string [ sIndex ] = (*cData);
			++sIndex;
			}
		++cData;
		if(sIndex>=10)
			break;
		}
	string[sIndex] = '\0';
	return(VR_XXMULTISTRING :: VMSet ( Index, string ));
	}


/*****************
 *****************  VR_DS
 *****************/

VR_DS	::	VR_DS ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_DS	::	VR_DS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_DS	::	VR_DS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_DS	::	VR_DS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_DS	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_DS	::	GetTypeCode ()
	{
	return (MAKEUINT16('D', 'S'));
	}

/*****************
 *****************  VR_DT
 *****************/

VR_DT	::	VR_DT ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_DT	::	VR_DT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_DT	::	VR_DT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_DT	::	VR_DT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_DT	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_DT	::	GetTypeCode ()
	{
	return (MAKEUINT16('D', 'T'));
	}

/*****************
 *****************  VR_FL
 *****************/

VR_FL	::	VR_FL ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_FL	::	VR_FL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_FL	::	VR_FL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_FL	::	VR_FL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_FL	::	GetSetSize ()
	{
	return ( 4 );
	}

UINT16
VR_FL	::	GetTypeCode ()
	{
	return (MAKEUINT16('F', 'L'));
	}

/*****************
 *****************  VR_FD
 *****************/

VR_FD	::	VR_FD ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_FD	::	VR_FD (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_FD	::	VR_FD (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_FD	::	VR_FD (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_FD	::	GetSetSize ()
	{
	return ( 8 );
	}

UINT16
VR_FD	::	GetTypeCode ()
	{
	return (MAKEUINT16('F', 'D'));
	}

/*****************
 *****************  VR_IS
 *****************/

VR_IS	::	VR_IS ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_IS	::	VR_IS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_IS	::	VR_IS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_IS	::	VR_IS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_IS	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_IS	::	GetTypeCode ()
	{
	return (MAKEUINT16('I', 'S'));
	}

/*****************
 *****************  VR_LO
 *****************/

VR_LO	::	VR_LO ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_LO	::	VR_LO (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_LO	::	VR_LO (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_LO	::	VR_LO (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_LO	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_LO	::	GetTypeCode ()
	{
	return (MAKEUINT16('L', 'O'));
	}

/*****************
 *****************  VR_LT
 *****************/

VR_LT	::	VR_LT ()
	:	VR_XXTYPED ()
	{
	TypeCode = GetTypeCode ();
	}

VR_LT	::	VR_LT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_LT	::	VR_LT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXTYPED ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_LT	::	VR_LT (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_LT	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_LT	::	GetTypeCode ()
	{
	return (MAKEUINT16('L', 'T'));
	}
BOOL
VR_LT	::	PostProcSetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}

BOOL
VR_LT	::	PostProcGetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}

BOOL
VR_LT	::	Set(
	char	*string)
	{
	UINT	iLength;

	if ( Data )
		{
		if ( ReleaseMemory )
			delete Data;
		}
	iLength = (UINT) strlen(string);
	Data = new char [ iLength + 1 ];
	strcpy((char*)Data, string);
	Length = (UINT32) (iLength);
	if(ReferentialIntegrityDatabase)
		((VRReferentialIntegrityDatabase*)ReferentialIntegrityDatabase)
			->SetMaster(Group, Element, Data, Length);
	else
		ReleaseMemory = TRUE;
	return ( TRUE );
	}

BOOL
VR_LT	::	Get(
	char	*string)
	{
	if ( ! Data )
		return ( FALSE );

	memcpy((void*)string, Data, Length);
	string [ Length ] = '\0';
	return ( TRUE );
	}

/*****************
 *****************  VR_OB
 *****************/

VR_OB	::	VR_OB ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_OB	::	VR_OB (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_OB	::	VR_OB (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_OB	::	VR_OB (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_OB	::	GetSetSize ()
	{
	return ( 1 );
	}

UINT16
VR_OB	::	GetTypeCode ()
	{
	return (MAKEUINT16('O', 'B'));
	}

/*****************
 *****************  VR_OW
 *****************/

VR_OW	::	VR_OW ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_OW	::	VR_OW (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_OW	::	VR_OW (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_OW	::	VR_OW (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_OW	::	GetSetSize ()
	{
	return ( 1 );
	}

UINT16
VR_OW	::	GetTypeCode ()
	{
	return (MAKEUINT16('O', 'W'));
	}

/*****************
 *****************  VR_PN
 *****************/

VR_PN	::	VR_PN ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_PN	::	VR_PN (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_PN	::	VR_PN (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_PN	::	VR_PN (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_PN	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_PN	::	GetTypeCode ()
	{
	return (MAKEUINT16('P', 'N'));
	}


/******
 *
 * My Apologies for this partiuclar section of code.  Significant
 * amount of confusion exists here because we want a *very* flexiable
 * parser.  We must handle proper Ansi Names:
 *
 *		Prefix. First Middle. Last,Postfix
 *
 * and not so ANSI names like:
 *
 * 		John Doe
 *		John M. Doe
 *		Doe, John M.
 *
 ******/

BOOL
VR_PN	::	RealToANSI(
	char	*Data,
	char	*AName)
	{
	UINT	cIndex;
	char	*cData;
	char	Prefix [ 12 ];
	char	Postfix [ 32 ];
	char	First [ 64 ];
	char	Last [ 64 ];
	char	Middle [ 24 ];

	cData = (char*)Data; 
	Prefix [ 0 ] =
	Postfix [ 0 ] =
	First [ 0 ] =
	Last [ 0 ] =
	Middle [ 0 ] = '\0';

	cIndex = 0;
	while(*cData)
		{
		if((*cData) == ' ')
			{
			++cData;
			Prefix [ cIndex ] = '\0';
			strcpy(First, Prefix);
			Prefix [ 0 ] = '\0';
			break;
			}
		if((*cData) == ',')
			{
			++cData;
			Prefix [ cIndex ] = '\0';
			strcpy(Last, Prefix);
			Prefix [ 0 ] = '\0';
			break;
			}

		Prefix [ cIndex ] = (*cData);
		if ((*cData) == '.')
			{
			++cIndex;
			++cData;
			Prefix [ cIndex ] = '\0';
			break;
			}
		++cData;
		++cIndex;
		}
	Prefix [ cIndex ] = '\0';
	if (!(*cData))
		{
		strcpy(Last, Prefix);
		Prefix [ 0 ] = '\0';
		goto ConstructName;
		}
//	fprintf(stderr, "Prefix: (%s) First: (%s) Last: (%s)\n",
//		Prefix, First, Last);
	// Ok, Either the First, Last, or  Prefix field is valid
	if ( Prefix [ 0 ] )
		{
		goto ExtractFirstLastPost;
		}
	if ( Last [ 0 ] )
		{
		goto ExtractFirstMiddlePost;
		}
	if (First[0])
		{
		First[cIndex] = ' ';
		++cIndex;
		while(*cData)
			{
			if((*cData)==',')
				{
				++cData;
				break;
				}
			First[cIndex]=(*cData);
			++cIndex;
			++cData;
			}
		First[cIndex]='\0';
		
		goto ExtractMiddleLastPost;
		}

ExtractFirstLastPost:
//	fprintf(stderr, "ExtractFirstLastPost:\n");
	// Prefix, so, assume this is an ANSI Name
	while ( (*cData) == ' ')
		++cData;
	if(!(*cData))
		return ( FALSE );
	cIndex = 0;
	while (*cData)
		{
		if ((*cData) == ',')
			{
			++cData;
			First [ cIndex ] = '\0';
			break;
			}
		if(!(*cData))
			{
			First [ cIndex ] = '\0';
			break;
			}				
		First [ cIndex ] = (*cData);
		++cIndex;
		++cData;
		}
	First [ cIndex ] = '\0';
//	fprintf(stderr, "Prefix: (%s) First: (%s) Last: (%s)\n",
//		Prefix, First, Last);
ExtractMiddleLastPost:
//	fprintf(stderr, "ExtractMiddleLastPost\n");
	if(!cIndex)
		return ( FALSE );
	// Ok, now we have first name, need to back track to extract
	// last name from it.
	while ( cIndex )
		{
		--cIndex;
		if(First[cIndex]==' ')
			break;
		}
	if(cIndex)
		{
		First[cIndex] = '\0';
		strcpy(Last, &First[cIndex+1]);
		}
	else
		{
		strcpy(Last, First);
		First [ 0 ] = '\0';
		}
	// Ok, got Prefix/First/Last, now it's time to extract
	// middle (if possiable)
//	fprintf(stderr, "Prefix: (%s) First: (%s) Last: (%s)\n",
//		Prefix, First, Last);
ExtractMiddleFromFirst:
//	fprintf(stderr, "ExtractMiddleFromLast\n");

	if(cIndex)
		{
		while(cIndex)
			{
			--cIndex;
			if(First[cIndex]=='.')
				{
				while(cIndex)
					{
					--cIndex;
					if(First[cIndex]==' ')
						break;
					}
				if(cIndex)
					{
					strcpy(Middle, &First[cIndex+1]);
					First[cIndex] = '\0';
					break;
					}
				else
					{
					strcpy(Middle, First);
					First [ 0 ] = '\0';
					break;
					}
				}
			}
		}
	// Got Pre/First/Middle/Last time to extract last part
	strcpy(Postfix, cData);

	goto ConstructName;

ExtractFirstMiddlePost:
	while ( (*cData) == ' ')
		++cData;
	if(!(*cData))
		return ( FALSE );
	cIndex = 0;
	while (*cData)
		{
		if ((*cData) == ',')
			{
			First [ cIndex ] = '\0';
			break;
			}
		if(!(*cData))
			{
			First [ cIndex ] = '\0';
			break;
			}				
		First [ cIndex ] = (*cData);
		++cIndex;
		++cData;
		}
	First [ cIndex ] = '\0';
	if(!cIndex)
		return ( FALSE );
	// Ok, got Prefix/First/Last, now it's time to extract
	// middle (if possiable)
	goto ExtractMiddleFromFirst;




ConstructName:
	// woa.. we made it..

	UINT	biLast, biFirst, biMiddle, biPrefix, biPostfix;

	biLast = biFirst = biMiddle = biPrefix = biPostfix = 0;

	while(Last[biLast]==' ')
		++biLast;
	while(First[biFirst]==' ')
		++biFirst;
	while(Middle[biMiddle]==' ')
		++biMiddle;
	while(Prefix[biPrefix]==' ')
		++biPrefix;
	while(Postfix[biPostfix]==' ')
		++biPostfix;
 
	strcpy(AName, &Last[biLast]);
	strcat(AName, "^");
	strcat(AName, &First[biFirst]);
	strcat(AName, "^");
	strcat(AName, &Middle[biMiddle]);
	strcat(AName, "^");
	strcat(AName, &Prefix[biPrefix]);
	strcat(AName, "^");
	strcat(AName, &Postfix[biPostfix]);

	cIndex = strlen(AName);
	while(cIndex)
		{
		--cIndex;
		if(AName[cIndex]!='^')
			break;
		AName[cIndex]='\0';
		}

	return ( TRUE );
	}

BOOL
VR_PN	::	ANSIToReal(
	char	*AName,
	char	*vData)
	{
	UINT	cIndex;
	char	*cData, *Data;
	char	First [ 32 ];
	char	Last [ 64 ];
	char	Middle [ 20 ];
	char	Postfix [ 32 ];
	char	Prefix[20];

	cData = (char*)AName;
	Data = (char*)vData; 
	Prefix [ 0 ] =
	Postfix [ 0 ] =
	First [ 0 ] =
	Last [ 0 ] =
	Middle [ 0 ] = '\0';

	cIndex = 0;
	while(*cData)
		{
		if((*cData)=='^')
			{
			++cData;
			break;
			}
		Last[cIndex]=(*cData);
		++cIndex;
		++cData;
		}
	Last[cIndex] = '\0';
	if(!(*cData))
		goto MakeRealName;

	cIndex = 0;
	while(*cData)
		{
		if((*cData)=='^')
			{
			++cData;
			break;
			}
		First[cIndex]=(*cData);
		++cIndex;
		++cData;
		}
	First[cIndex] = '\0';
	if(!(*cData))
		goto MakeRealName;

	cIndex = 0;
	while(*cData)
		{
		if((*cData)=='^')
			{
			++cData;
			break;
			}
		Middle[cIndex]=(*cData);
		++cIndex;
		++cData;
		}
	Middle[cIndex] = '\0';
	if(!(*cData))
		goto MakeRealName;

	cIndex = 0;
	while(*cData)
		{
		if((*cData)=='^')
			{
			++cData;
			break;
			}
		Prefix[cIndex]=(*cData);
		++cIndex;
		++cData;
		}
	Prefix[cIndex] = '\0';
	if(!(*cData))
		goto MakeRealName;

	cIndex = 0;
	while(*cData)
		{
		if((*cData)=='^')
			{
			++cData;
			break;
			}
		Postfix[cIndex]=(*cData);
		++cIndex;
		++cData;
		}
	Postfix[cIndex] = '\0';
	
MakeRealName:
	strcpy(Data, "");
	if (Prefix[0])
		{
		strcat(Data, Prefix);strcat(Data, " ");
		}
	if(First[0])
		{
		strcat(Data, First);strcat(Data, " ");
		}
	if(Middle[0])
		{
		strcat(Data, Middle);strcat(Data, " ");
		}
	if(Last[0])
		{
		strcat(Data, Last);
		}
	if(Postfix[0])
		{
		strcat(Data, ", ");
		strcat(Data, Postfix);
		}
	return ( TRUE );
	}

BOOL
VR_PN	::	VMSet(
	UINT	Index,
	char	*vData)
	{
	char	*cData;
	char	AName[ 65 ];

	cData = (char*) vData;
	while(*cData)
		{
		if((*cData)=='^')
			{
			// Assume user has formatted the data
			return(VR_XXMULTISTRING::VMSet(Index, vData));
			}
		++cData;
		}
	if(!RealToANSI(vData, AName))
		return(FALSE);
	return ( VR_XXMULTISTRING::VMSet(Index, AName) );
	}

BOOL
VR_PN	::	VMGet(
	UINT	Index,
	char	*vData)
	{
	char	*cData;
	char	AName[65];

	if(VR_XXMULTISTRING :: VMGet(Index, AName) )
		{
		cData = &AName[0];
		while(*cData)
			{
			if((*cData)=='^')
				{
				// OK, This is a correctly formatted patient name
				return(ANSIToReal(AName, vData));
				}
			++cData;
			}
		// NO, This name is NOT ansi
		strcpy(vData, AName);
		return ( TRUE );
		}
	return ( FALSE );
	}

/*****************
 *****************  VR_SH
 *****************/

VR_SH	::	VR_SH ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_SH	::	VR_SH (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_SH	::	VR_SH (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_SH	::	VR_SH (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_SH	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_SH	::	GetTypeCode ()
	{
	return (MAKEUINT16('S', 'H'));
	}

/*****************
 *****************  VR_SL
 *****************/

VR_SL	::	VR_SL ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_SL	::	VR_SL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_SL	::	VR_SL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_SL	::	VR_SL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_SL	::	GetSetSize ()
	{
	return ( 4 );
	}

UINT16
VR_SL	::	GetTypeCode ()
	{
	return (MAKEUINT16('S', 'L'));
	}

/*****************
 *****************  VR_SS
 *****************/

VR_SS	::	VR_SS ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_SS	::	VR_SS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_SS	::	VR_SS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_SS	::	VR_SS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_SS	::	GetSetSize ()
	{
	return ( 2 );
	}

UINT16
VR_SS	::	GetTypeCode ()
	{
	return (MAKEUINT16('S', 'S'));
	}

/*****************
 *****************  VR_ST
 *****************/

VR_ST	::	VR_ST ()
	:	VR_XXTYPED ()
	{
	TypeCode = GetTypeCode ();
	}

VR_ST	::	VR_ST (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_ST	::	VR_ST (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXTYPED ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_ST	::	VR_ST (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_ST	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_ST	::	GetTypeCode ()
	{
	return (MAKEUINT16('S', 'T'));
	}

BOOL
VR_ST	::	PostProcSetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}

BOOL
VR_ST	::	PostProcGetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}

BOOL
VR_ST	::	Set(
	char	*string)
	{
	UINT	iLength;

	if ( Data )
		{
		if ( ReleaseMemory )
			delete Data;
		}
	iLength = (UINT) strlen(string);
	Data = new char [ iLength + 1 ];
	strcpy((char*)Data, string);
	Length = (UINT32) (iLength);
	if(ReferentialIntegrityDatabase)
		((VRReferentialIntegrityDatabase*)ReferentialIntegrityDatabase)
			->SetMaster(Group, Element, Data, Length);
	else
		ReleaseMemory = TRUE;
	return ( TRUE );
	}

BOOL
VR_ST	::	Get(
	char	*string)
	{
	if ( ! Data )
		return ( FALSE );

	memcpy((void*)string, Data, Length);
	string [ Length ] = '\0';
	return ( TRUE );
	}

/*****************
 *****************  VR_TM
 *****************/

VR_TM	::	VR_TM ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_TM	::	VR_TM (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_TM	::	VR_TM (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_TM	::	VR_TM (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_TM	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_TM	::	GetTypeCode ()
	{
	return (MAKEUINT16('T', 'M'));
	}

/*****************
 *****************  VR_UI
 *****************/

VR_UI	::	VR_UI ()
	:	VR_XXSTRING ()
	{
	TypeCode = GetTypeCode ();
	}

VR_UI	::	VR_UI (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_UI	::	VR_UI (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXSTRING ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_UI	::	VR_UI (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXSTRING ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_UI	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_UI	::	GetTypeCode ()
	{
	return (MAKEUINT16('U', 'I'));
	}

// UI's are unique in that they are appended with NULL bytes and not
// spaces.
BOOL
VR_UI	::	PostProcSetData(
	UINT	Length,
	void	*vstring)
	{
	// Post-Process data we are setting into the VR
	UINT	Index;
	char	*string = (char*) vstring;
	
	if ( ! Length )
		return ( FALSE );	
	Index = Length;
	while ( Index )
		{
		--Index;
		if(string[Index]==' ')
			{
			string[Index] = '\0';
			continue;
			}
		break;
		}
	return ( TRUE );
	}

/*****************
 *****************  VR_UL
 *****************/

VR_UL	::	VR_UL ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_UL	::	VR_UL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_UL	::	VR_UL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_UL	::	VR_UL (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_UL	::	GetSetSize ()
	{
	return ( 4 );
	}

UINT16
VR_UL	::	GetTypeCode ()
	{
	return (MAKEUINT16('U', 'L'));
	}

/*****************
 *****************  VR_US
 *****************/

VR_US	::	VR_US ()
	:	VR_XXBINARY ()
	{
	TypeCode = GetTypeCode ();
	}

VR_US	::	VR_US (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_US	::	VR_US (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_US	::	VR_US (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_US	::	GetSetSize ()
	{
	return ( 2 );
	}

UINT16
VR_US	::	GetTypeCode ()
	{
	return (MAKEUINT16('U', 'S'));
	}


/*****************
 *****************  VR_XS
 *****************/

VR_XS	::	VR_XS ()
	:	VR_XXBINARY ()
	{
	RTCTypeCode = MAKEUINT16('U', 'S');
	TypeCode = GetTypeCode ();
	}

VR_XS	::	VR_XS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	RTCTypeCode = MAKEUINT16('U', 'S');
	TypeCode = GetTypeCode ();
	}

VR_XS	::	VR_XS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	RTCTypeCode = MAKEUINT16('U', 'S');
	TypeCode = GetTypeCode ();
	}

VR_XS	::	VR_XS (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	RTCTypeCode = MAKEUINT16('U', 'S');
	TypeCode = GetTypeCode ();
	}

UINT
VR_XS	::	GetSetSize ()
	{
	return ( 2 );
	}

UINT16
VR_XS	::	GetTypeCode ()
	{
	return (RTCTypeCode);
	}

/*****************
 *****************  VR_XS
 *****************/

VR_OX	::	VR_OX ()
	:	VR_XXBINARY ()
	{
	RTCTypeCode = MAKEUINT16('O', 'W');
	TypeCode = GetTypeCode ();
	}

VR_OX	::	VR_OX (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, a)
	{
	RTCTypeCode = MAKEUINT16('O', 'W');
	TypeCode = GetTypeCode ();
	}

VR_OX	::	VR_OX (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXBINARY ( g, e, l, d)
	{
	RTCTypeCode = MAKEUINT16('O', 'W');
	TypeCode = GetTypeCode ();
	}

VR_OX	::	VR_OX (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXBINARY ( g, e, l, d, a)
	{
	RTCTypeCode = MAKEUINT16('O', 'W');
	TypeCode = GetTypeCode ();
	}

UINT
VR_OX	::	GetSetSize ()
	{
	if ( RTCTypeCode == MAKEUINT16('O', 'W'))
		return ( 2 );
	
	return ( 1 );
	}

UINT16
VR_OX	::	GetTypeCode ()
	{
	return (RTCTypeCode);
	}

BOOL
VR_OX	::	SetTypeCode (
	UINT16		NewTypeCode)
	{
	RTCTypeCode = NewTypeCode;
	TypeCode = GetTypeCode ();
	return ( TRUE );
	}


/*****************
 *****************  VR_SQ
 *****************/

VR_SQ	::	VR_SQ ()
	:	VR_XXTYPED ()
	{
	TypeCode = GetTypeCode ();
	}

VR_SQ	::	VR_SQ (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, a)
	{
	TypeCode = GetTypeCode ();
	}

VR_SQ	::	VR_SQ (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d)
	:	VR_XXTYPED ( g, e, l, d)
	{
	TypeCode = GetTypeCode ();
	}

VR_SQ	::	VR_SQ (
	UINT16	g,
	UINT16	e,
	UINT32	l,
	void	*d,
	BOOL	a)
	:	VR_XXTYPED ( g, e, l, d, a)
	{
	TypeCode = GetTypeCode ();
	}

UINT
VR_SQ	::	GetSetSize ()
	{
	return ( 0 );	// variable
	}

UINT16
VR_SQ	::	GetTypeCode ()
	{
	return (MAKEUINT16('S', 'Q'));
	}

BOOL
VR_SQ	::	PostProcSetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}

BOOL
VR_SQ	::	PostProcGetData(
	UINT	iLength,
	void	*vData)
	{
	return ( TRUE );
	}


