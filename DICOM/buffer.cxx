/************************************************************
 * Buffer Class
 *
 ************************************************************/

#	include	"dicom.hpp"

#	define	DEFAULT_BREAK_SIZE	32600
//构造函数,同时分配内存
BufferSpace	::	BufferSpace(UINT	Alloc)
	{
	if ( Alloc )
		Data = new BYTE [ Alloc ];
	else
		Data = NULL;
	Index = 0;
	BufferSize = Alloc;
	isTemp = FALSE;
	}
//构造函数
BufferSpace	::	BufferSpace()
	{
	Data = NULL;
	Index = 0;
	BufferSize = 0;
	isTemp = TRUE;
	}
//析构函数
BufferSpace	::	~BufferSpace()
	{
	if ( !isTemp )
		if ( Data )
			delete Data;
	}
//构造函数
Buffer	::	Buffer()
	{
	BreakSize = DEFAULT_BREAK_SIZE;
	InEndian = NATIVE_ENDIAN;
	OutEndian = NATIVE_ENDIAN;
	InSize = 0;
	OutSize = 0;
	}
//析构函数,释放内存
Buffer	::	~Buffer()
	{
	
	while ( Incoming.GetSize() )
		{
//		if ( Incoming[0].Data )
//			delete Incoming[0].Data;
//		Incoming[0].Data = NULL;
		
		delete Incoming.Get(0);
		Incoming.RemoveAt( 0 );
		}
	while ( Outgoing.GetSize() )
		{
//		if ( Outgoing[0].Data )
//			delete Outgoing[0].Data;
//		Outgoing[0].Data = NULL;
		
		delete Outgoing.Get(0);
		Outgoing.RemoveAt ( 0 );
		}
	Incoming.ClearType = 0;
	Outgoing.ClearType = 0;
//	Incoming.dt.Data = NULL;
//	Outgoing.dt.Data = NULL;
	InEndian = NATIVE_ENDIAN;
	}
//设置进来的数据格式
BOOL	Buffer	::	SetIncomingEndian(UINT	Endian)
	{
	InEndian = Endian;
	return ( TRUE );
	}
//设置送出的数据格式
BOOL	Buffer	::	SetOutgoingEndian(UINT	Endian)
	{
	OutEndian = Endian;
	return ( TRUE );
	}
//设置内存块大小
BOOL	Buffer	::	SetBreakSize(UINT	ToSize)
	{
	BreakSize = ToSize;
	return ( TRUE );
	}
//送出数据8位
Buffer	&	Buffer	::	operator	>>	(BYTE	&x)
	{
	BYTE	s[2];

	Read(s, 1);
	x = s[0];
//	fprintf(stderr, "%2.2x ", x);
	return (*this);
	}

/*************************************************
	BufferSpace	BS;

	if (InSize <= 0)
		{
		ReadBlock();
		}
	if (InSize <= 0)
		{
		x = 0;
		return ( * this );
		}
	BS = Incoming.Get ( 0 );
	if ( ! BS.Data )
		{
		InSize -= BS.Size;
		Incoming.RemoveAt ( 0 );
		x = 0;
		return ( * this );
		}
	x = BS.Data[BS.Index];
	++BS.Index;
	--InSize;
	if(BS.Index == BS.Size)
		Incoming.RemoveAt ( 0 );
	else
		Incoming.Get ( 0 ) = BS;
	BS.isTemp = TRUE;
	return ( * this );
	}
***************************************************/
//送出数据16位
Buffer	&	Buffer	::	operator	>>	(UINT16	&x)
	{
	BYTE	b1, b2;
	BYTE	*op;

	(*this) >> b1;
	(*this) >> b2;

	if(InEndian == NATIVE_ENDIAN)
		{
		op = (BYTE*) &x;
		(*op) = b1;++op;
		(*op) = b2;	
		}
	else
		{
		op = (BYTE*) &x;
		(*op) = b2;++op;
		(*op) = b1;	
		}

	return ( * this );
	}
/*
static	void	Dump(void	*vp, int	size)
	{
	unsigned	char	*cvp;

	cvp = (unsigned char *) vp;
	while(size)
		{
		fprintf(stderr, "%2.2x ", *cvp);
		++cvp;
		--size;
		}
	}
*/
//送出数据32位
Buffer	&	Buffer	::	operator	>>	(UINT32	&x)
	{
	UINT16	w1, w2;
	BYTE	*op, *ip;

	(*this) >> w1;
	(*this) >> w2;

	if(InEndian == NATIVE_ENDIAN)
		{
		op = (BYTE *) &x;
		ip = (BYTE *) &w1;
		(*op) = (*ip);++op;++ip;
		(*op) = (*ip);++op;++ip;
		ip = (BYTE *) &w2;
		(*op) = (*ip);++op;++ip;
		(*op) = (*ip);
		}
	else
		{
		op = (BYTE *) &x;
		ip = (BYTE *) &w2;
		(*op) = (*ip);++op;++ip;
		(*op) = (*ip);++op;++ip;
		ip = (BYTE *) &w1;
		(*op) = (*ip);++op;++ip;
		(*op) = (*ip);
		}
	
	return ( * this );
	}
//读取数据8位
Buffer	&	Buffer	::	operator	<<	(BYTE	&x)
	{
	BYTE	s[2];
	
//	fprintf(stderr, "Buffer :: << Byte\n");fflush(stderr);
	s[0] = x;
	Write(s, 1);
//	fprintf(stderr, "returning\n");fflush(stderr);
	return (*this);
	}

/************************************************
	BufferSpace	BS;

	if ( ! Outgoing.GetSize() )
		{
		BufferSpace	ABS( BreakSize );
		Outgoing.Add ( ABS );
		ABS.Data = NULL;
		}
	BS = Outgoing.Get ( Outgoing.GetSize() - 1 );
	if(BS.Index == BS.Size)
		{
		BufferSpace	ABS( BreakSize );
		Outgoing.Add ( ABS );
		ABS.Data = NULL;
		BS = Outgoing.Get ( Outgoing.GetSize() - 1);
		}
	BS.Data[BS.Index] = x;
	++BS.Index;
	Outgoing.Get ( Outgoing.GetSize() - 1 ) = BS;
	BS.isTemp = TRUE;
	++OutSize;
	return (*this );
	}
*************************************************/
//读取数据16位	
Buffer	&	Buffer	::	operator	<<	(UINT16	&x)
	{
	BYTE	b1, b2;
	BYTE	*ip;

	ip = (BYTE*)&x;
	b1 = (*ip);++ip;
	b2 = (*ip);
	if(OutEndian == NATIVE_ENDIAN)
		{
		(*this) << b1;
		(*this) << b2;
		}
	else
		{
		(*this) << b2;
		(*this) << b1;
		}
	return ( * this );
	}
//读取数据32位
Buffer	&	Buffer	::	operator	<<	(UINT32	&x)
	{
	UINT16	w1, w2;
	UINT16	*ip;

	ip = (UINT16*)&x;
	w1 = (*ip);++ip;
	w2 = (*ip);
	if(OutEndian == NATIVE_ENDIAN)
		{
		(*this) << w1;
		(*this) << w2;
		}
	else
		{
		(*this) << w2;
		(*this) << w1;
		}
	return ( * this );
	}
//将数据全部送出
BOOL	Buffer	::	Flush()
	{
	BufferSpace	*BS;

	
	//fprintf(stderr, "Buffer :: Flush() %d buffers\n", Outgoing.GetSize());fflush(stderr);
	while ( Outgoing.GetSize() )
		{
		BS = Outgoing.Get ( 0 );
		if(BS->Index)
			SendBinary(BS->Data, BS->Index);
		
		delete Outgoing.Get(0);
		Outgoing.RemoveAt ( 0 );
		}
	OutSize = 0;
//	fprintf(stderr, "returning TRUE\n");fflush(stderr);
	return ( TRUE );
	}

BOOL    Buffer	::	Flush(BYTE* lpDcm,int& nLength)
{
	BufferSpace	*BS;
	BYTE* lpTem ;
	lpTem = lpDcm ;
	nLength = 0 ;
	while ( Outgoing.GetSize() )
	{
		BS = Outgoing.Get ( 0 );
		if(BS->Index)
		{
			memcpy(lpTem,BS->Data,BS->Index);
			lpTem   += BS->Index ;
			nLength += BS->Index ;
		}
		delete Outgoing.Get(0);
		Outgoing.RemoveAt ( 0 );
	}
	OutSize = 0;
	return ( TRUE );
}
//送出Bytes个字节
BOOL	Buffer	::	Flush(UINT	Bytes)
	{
	BufferSpace	*BS;

	//fprintf(stderr, "FLUSH: OutSize=%d, Bytes=%d\n", OutSize,Bytes);

	if(Bytes > OutSize)
		return(Flush());

	BYTE*pByte=new BYTE[Bytes];
	int nCur=0;
	
	OutSize -= Bytes;

	while(Bytes)
		{
		if(!Outgoing.GetSize())
			{
			//fprintf(stderr, "Attempting to flush more bytes than input, OutSize=%d Bytes=%d\n", OutSize,Bytes);
			OutSize = 0;
			return ( FALSE );
			}

		BS = Outgoing.Get( 0 );
		if(BS->Index > Bytes)
			{
			memcpy(pByte+nCur,BS->Data,Bytes);
			nCur+=Bytes;
			SendBinary(pByte, nCur);
			delete pByte;
			//peak change for memcpy may cause undefined error if memory overlap
			//orginal as 
			//memcpy(BS->Data, &BS->Data[Bytes], BS->Index - Bytes);
			BYTE*pTmp=new BYTE[BS->Index - Bytes];
			memcpy(pTmp, &BS->Data[Bytes], BS->Index - Bytes);
			memcpy(BS->Data,pTmp, BS->Index - Bytes);
			delete pTmp;

			BS->Index = BS->Index - Bytes;
			Bytes = 0;
			return ( TRUE );
			}
		else
			{
			if(BS->Index)
				{
				memcpy(pByte+nCur,BS->Data,BS->Index);
				nCur+=BS->Index;
			//	SendBinary(BS->Data, BS->Index);
				Bytes -= BS->Index;
				delete Outgoing.Get(0);
				Outgoing.RemoveAt ( 0 );
				}
			else
				{
				delete Outgoing.Get(0);
				Outgoing.RemoveAt ( 0 );
				}
			}
		}
	if(nCur>0)
	{
		SendBinary(pByte, nCur);
	}
	delete pByte;
	return ( TRUE );
	}

//跳过Bytes个字节				
BOOL	Buffer	::	Kill(UINT	Bytes)
	{
	BOOL	Ret;
	BYTE	*Data = new BYTE[Bytes+1];

	Ret = Read(Data, Bytes);
	delete Data;
	return ( Ret );
	}
	
/*
	BufferSpace	*BS;
	InSize -= Bytes;
	while ( Incoming.GetSize() )
		{
		BS = Incoming.Get ( 0 );
		if ( (BS->Index + Bytes) > BS->BufferSize)
			Bytes -= (BS->BufferSize - BS->Index);
		else
			{
			// NOTE, Used to be other way around...
			BS->Index += Bytes;
			Bytes = 0;
			return ( TRUE );
			}
		delete Incoming.Get(0);
		Incoming.RemoveAt ( 0 );
		}
	
	return ( FALSE );	// not got that many bytes yet, but go negative
	}
*/
//读取bytes个字节
BOOL	Buffer	::	Fill(UINT	Bytes)
	{
	UINT	ECount;

	SetBreakSize(Bytes);
	
	ECount = InSize + Bytes;
	while(InSize < ECount)
		{
		//fprintf(stderr, "InSize: %d\n", InSize);
		//printf("Fill %d, %d\n", ECount, Bytes);
		SetBreakSize(ECount - InSize);

		if(!ReadBlock())
			return ( FALSE );
		//fprintf(stderr, "InSize: %d\n", InSize);
		}
	SetBreakSize(DEFAULT_BREAK_SIZE);
	return ( TRUE );
	}

//读取数据		
BOOL	Buffer	::	Read(BYTE	*Data, UINT	Bytes)
	{
	BufferSpace	*BS;

	SetBreakSize(Bytes);

	while(InSize < Bytes)
		if(!ReadBlock())
			return ( FALSE );

	while ( Bytes )
		{
		BS = Incoming.Get ( 0 );
		if(BS->Index == BS->BufferSize)
			{
			delete BS;
			Incoming.RemoveAt( 0 );
			}
		else
			{
			if((BS->Index + Bytes) <= BS->BufferSize)
				{
				memcpy(Data, &BS->Data[BS->Index], Bytes);
				BS->Index += Bytes;
				InSize -= Bytes;
				return ( TRUE );
				}
			else
				{
				memcpy(Data, &BS->Data[BS->Index], BS->BufferSize - BS->Index);
				Bytes -= (BS->BufferSize - BS->Index);
				Data += (BS->BufferSize - BS->Index);
				InSize -= (BS->BufferSize - BS->Index);
				delete BS;
				Incoming.RemoveAt ( 0 );
				}
			}
		}
 	
	return ( FALSE );	// should not get here
	}
//写入数据,实际上就是生成Outgoing buffer链
BOOL	Buffer	::	Write(BYTE	*Data, UINT	Bytes)
	{
	BufferSpace	*BS;
	UINT		Count;

	
	if ( !Outgoing.GetSize() )
		{
		BufferSpace	*ABS	=	new BufferSpace(BreakSize);
		Outgoing.Add ( ABS );
		}
	while(Bytes)
		{
		BS = Outgoing.Get ( Outgoing.GetSize() - 1);
		Count = BS->BufferSize - BS->Index;
		if ( Count )
			{
			if (Count <= Bytes )
				{
				memcpy(&BS->Data[BS->Index], Data, Count);
				Bytes -= Count;
				Data += Count;
				BS->Index += Count;
				
				
				OutSize += Count;
				BufferSpace	*ABS	=	new BufferSpace(BreakSize);
				Outgoing.Add ( ABS );
				
				}
			else
				{
				memcpy(&BS->Data[BS->Index], Data, Bytes);
				BS->Index += Bytes;
				OutSize += Bytes;
				Bytes = 0;
				return ( TRUE );
				}
			}
		else
			{
			//BufferSpace	ABS(BreakSize);
			BufferSpace	*ABS	=	new BufferSpace(DEFAULT_BREAK_SIZE);
			Outgoing.Add( ABS );
			
			}
		}
	return ( TRUE );
	}
//读取数据块
BOOL	Buffer	::	ReadBlock()
	{
	int		Length, ILength;
	BYTE	s[1024];

	if(BreakSize<1024)
		ILength = BreakSize;
	else
		ILength = 1024;
	
	//printf("ReadBlock(%d)\n", Length);
	
	while(!(Length = ReadBinary((BYTE*)s, ILength)))
		;
	
	//fprintf(stderr, "Block Size = %d\n", ABS.Size);
	if(Length == (-1))
		{
		//fprintf(stderr, "Error from ReadBinary()\n");
		//printf("ReadBinary:-1\n");
		return (FALSE);	// Error from ReadBinary()
		}
	
	BufferSpace	*ABS	=	new BufferSpace(Length);
	//fprintf(stdout, "Length=%d\n", Length);
	memcpy((void*)ABS->Data, (void*)s, Length);

	InSize += ABS->BufferSize;
	Incoming.Add(ABS);
	
	return ( TRUE );
	}
//已经废弃,用于测试是否有数据可以读取
BOOL	Buffer	::	Poll()
	{
	/*
	BufferSpace	ABS(BreakSize);

	ABS.BufferSize = ReadBinary(ABS.Data, BreakSize);
	if(!ABS.BufferSize)
		return ( TRUE );	// no data available
	if(ABS.BufferSize == (-1))
		return (FALSE);	// Error from ReadBinary()
	InSize += ABS.BufferSize;
	Incoming.Add(ABS);
	ABS.Data = NULL;
	*/
	return ( TRUE );
	}
/*
class	TestBuffer	:	public	Buffer
	{

	BOOL	SendBinary(BYTE	*Data, UINT	Count)
		{
		UINT	Index;
		fprintf(stderr, "Sending: %d Bytes\n", Count);
		Index = 0;
		while ( Index < Count )
			{
			fprintf(stderr, "%2.2x ", Data[Index]);
			++Index;
			if(!(Index%16))
				fprintf(stderr, "\n");
			}
		fprintf(stderr, "\n");
		fflush(stderr);
		return ( TRUE );
		}
		
	INT	ReadBinary(BYTE	*Data, UINT	Count)
		{
		fgets((char *) Data, Count, stdin);
		return ( strlen((char*) Data));
		}
	};

*/
