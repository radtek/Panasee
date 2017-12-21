#	include	"dicom.hpp"


/************************************************************************
 *
 * Presentation Context Accept
 *
 ************************************************************************/
//初始化函数,无参数
PresentationContextAccept	::	PresentationContextAccept()
	{
	ItemType = 0x21;
	Reserved1 = 0;
	PresentationContextID = uniq8();
	Reserved2 = 0;
	Result = 2;
	Reserved4 = 0;
	}
//初始化函数,带参数赋值
//类似初始化后调用SetTransferSyntax
PresentationContextAccept	::	PresentationContextAccept(
	//AbstractSyntax	&Abs, 
	TransferSyntax	&Tran)
	{
	//AbsSyntax = Abs;
	TrnSyntax = Tran;
	ItemType = 0x21;
	Reserved1 = 0;
	PresentationContextID = uniq8();
	Reserved2 = 0;
	Result = 2;
	Reserved4 = 0;
	}
//析构函数
PresentationContextAccept	::	~PresentationContextAccept()
	{
	//
	}
/*
void	PresentationContextAccept	::	SetAbstractSyntax(AbstractSyntax	&Abs)
	{
	AbsSyntax = Abs;
	}
*/
//设置内部transsytax
void	PresentationContextAccept	::	SetTransferSyntax(TransferSyntax	&Tran)
	{
	TrnSyntax = Tran;
	}
//序列化PresentationContextAccept
BOOL	PresentationContextAccept	::	Write ( Buffer	&Link )
	{
	Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));	
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) &PresentationContextID, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved2, sizeof(BYTE));
	Link.Write((BYTE *) &Result, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved4, sizeof(BYTE));
//	fprintf(stderr, "Writing Presentation Contex Accept: %d bytes\n", Length);
//	AbsSyntax.Write(Link);
	TrnSyntax.Write(Link);
//	fprintf(stderr, "ABS: %d TRN: %d\n", AbsSyntax.Size(), TrnSyntax.Size());


	Link.Flush();
	return ( TRUE );
	}
//读取PresentationContextAccept
BOOL	PresentationContextAccept	::	Read (Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//按照结构读取数据
BOOL	PresentationContextAccept	::	ReadDynamic (Buffer	&Link)
	{
//	INT32			Count;
	TransferSyntax	Tran;

	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) &PresentationContextID, sizeof(BYTE));
	Link.Read((BYTE *) &Reserved2, sizeof(BYTE));
	Link.Read((BYTE *) &Result, sizeof(BYTE));
	Link.Read((BYTE *) &Reserved4, sizeof(BYTE));

//	Count = Length - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE);
//	AbsSyntax.Read(Link);
//	Count = Count - AbsSyntax.Size();
	TrnSyntax.Read(Link);
//	Count = Count - TrnSyntax.Size();

//	if ( !Count)
		return ( TRUE );
//	return ( FALSE );
	}
//动态获得结构大小
UINT32	PresentationContextAccept	::	Size()
	{
	Length = sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE);
//	Length += AbsSyntax.Size();
	Length += TrnSyntax.Size();
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
	}

/************************************************************************
 *
 * AAssociateAC Packet
 *
 ************************************************************************/
//初始化函数
AAssociateAC	::	AAssociateAC()
	{
	ItemType = 0x02;
	Reserved1 = 0;
	ProtocolVersion = 0x0001;
	Reserved2 = 0;
	SpaceMem(CalledApTitle, 16);
	CalledApTitle[16] = 0;
	SpaceMem(CallingApTitle, 16);
	CallingApTitle[16] = 0;
	ZeroMem(Reserved3, 32);
	}
//带参数初始化
AAssociateAC	::	AAssociateAC(BYTE	*CallingAp, BYTE	*CalledAp)
	{
	ItemType = 0x02;
	Reserved1 = 0;
	ProtocolVersion = 0x0001;
	Reserved2 = 0;
	SpaceMem(CalledApTitle, 16);
	CalledApTitle[16] = 0;
	SpaceMem(CallingApTitle, 16);
	CallingApTitle[16] = 0;
	ZeroMem(Reserved3, 32);
	memcpy(CallingApTitle, CallingAp, min(strlen((char *)CallingAp), 16u));
	memcpy(CalledApTitle, CalledAp, min(strlen((char *)CalledAp), 16u));
	}
//析构函数
AAssociateAC	::	~AAssociateAC()
	{
	// nothing, everything should self-destruct nicely
	}
//设置接收方AET
void	AAssociateAC	::	SetCalledApTitle(BYTE	*CalledAp)
	{
	SpaceMem(CalledApTitle, 16);
	memcpy(CalledApTitle, CalledAp, min(strlen((char *)CalledAp), 16u));
	}
//设置请求方AET
void	AAssociateAC	::	SetCallingApTitle(BYTE	*CallingAp)
	{
	SpaceMem(CallingApTitle, 16);
	memcpy(CallingApTitle, CallingAp, min(strlen((char *)CallingAp), 16u));
	}
//设置应用上下文
void	AAssociateAC	::	SetApplicationContext(ApplicationContext	&AppC)
	{
	AppContext = AppC;
	}
//添加可接收的表达上下文
void	AAssociateAC	::	AddPresentationContextAccept(PresentationContextAccept	&PresContextAccept)
	{
	PresContextAccepts.Add(PresContextAccept);
	}
//设置用户信息
void	AAssociateAC	::	SetUserInformation(UserInformation	&User)
	{
	UserInfo = User;
	}
//序列化
//将结构写出到Link
BOOL	AAssociateAC	::	Write(Buffer	&Link)
	{
	UINT	Index;

//	fprintf(stderr, "AAssociateAC :: Write ()\n");fflush(stderr);
	Size();
	Link << ItemType; //Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT32));
	Link << ProtocolVersion;	//Link.Write((BYTE *) &ProtocolVersion, sizeof(UINT16));
	Link << Reserved2;			//Link.Write((BYTE *) &Reserved2, sizeof(UINT16));
	Link.Write((BYTE *) CalledApTitle, 16);
	Link.Write((BYTE *) CallingApTitle, 16);
	Link.Write((BYTE *) Reserved3, 32);
	Link.Flush();
//	fprintf(stderr, "AAssociateAC (writting App/Pre Contexts)\n");
	AppContext.Write(Link);
	Index = 0;
	while(Index < PresContextAccepts.GetSize())
		{
		PresContextAccepts[Index].Write(Link);
		++Index;
		}
//	fprintf(stderr, "AAssociateAC ( writting User info)\n");
	UserInfo.Write(Link);
	return ( TRUE );
	}
//从link读取数据到结构
BOOL	AAssociateAC	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//读取余下数据
BOOL	AAssociateAC	::	ReadDynamic(Buffer	&Link)
	{
//	UINT	Index;
	INT	Count;
	BYTE	TempByte;
	PresentationContextAccept	PresContextAccept;

	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >> ProtocolVersion;	//Link.Read((BYTE *) &ProtocolVersion, sizeof(UINT16));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	Link.Read((BYTE *) CalledApTitle, 16);
	Link.Read((BYTE *) CallingApTitle, 16);
	Link.Read((BYTE *) Reserved3, 32);

	CalledApTitle[16] = '\0';
	CallingApTitle[16] = '\0';

	Count = Length - sizeof(UINT16) - sizeof(UINT16) - 16 - 16 - 32;
	while(Count > 0)
		{
		Link.Read((BYTE *) &TempByte, sizeof(BYTE));
		switch(TempByte)
			{
			case	0x50:	// user information
				UserInfo.ReadDynamic(Link);
				Count = Count - UserInfo.Size();
				break;
			case	0x21:
				PresContextAccept.ReadDynamic(Link);
				Count = Count - PresContextAccept.Size();
				PresContextAccepts.Add(PresContextAccept);
				break;
			case	0x10:
				AppContext.ReadDynamic(Link);
				Count = Count - AppContext.Size();
				break;
			default:
				Link.Kill(Count-1);
				Count = -1;
			}
		}
	if(!Count)
		return ( TRUE );
	return ( FALSE);
	}
//获得结构大小
UINT32	AAssociateAC	::	Size()
	{
	UINT	Index;
//额外开销
	Length = sizeof(UINT16) + sizeof(UINT16) + 16 + 16 + 32;
	Length += AppContext.Size();
	Index = 0;
//PresContextAccepts大小
	Index = 0;
	while(Index < PresContextAccepts.GetSize())
		{
		Length += PresContextAccepts[Index].Size();
		++Index;
		}
//Userinfo大小
	Length += UserInfo.Size();
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

