
#	include	"dicom.hpp"


/*************************************************************************
 *
 * Application Context Class
 *
 ************************************************************************/
//构造函数
ApplicationContext	::	ApplicationContext()
	{
	ItemType = 0x10;
	Reserved1 = 0;
	}
//构造函数,附加设置uid初始值
ApplicationContext	::	ApplicationContext(UID	&uid)
	{
	ItemType = 0x10;
	Reserved1 = 0;
	ApplicationContextName = uid;
	}
//构造函数,附加设置名称初始值
ApplicationContext	:: ApplicationContext(BYTE	*name)
	{
	ItemType = 0x10;
	Reserved1 = 0;
	ApplicationContextName.Set(name);
	}
//析构函数
ApplicationContext	::	~ApplicationContext()
	{
	// nothing to de-allocate specifically

	};
//设置uid值
void	ApplicationContext	::	Set(UID	&uid)
	{
	ApplicationContextName = uid;
	};
//设置名称
void	ApplicationContext	::	Set(BYTE	*name)
	{
	ApplicationContextName.Set(name);
	}
//序列化
BOOL	ApplicationContext	::	Write(Buffer	&Link)
	{
	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) ApplicationContextName.GetBuffer(Length), Length);
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	ApplicationContext	::	Read(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	ApplicationContext	::	ReadDynamic(Buffer	&Link)
	{
	Link >> Reserved1;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) ApplicationContextName.GetBuffer(Length), Length);
	ApplicationContextName.GetBuffer(Length)[Length] = '\0';
	ApplicationContextName.SetLength(Length);
	return ( TRUE );
	}
//结构大小
UINT32	ApplicationContext	::	Size()
	{
	Length = ApplicationContextName.GetSize();
	return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Length );
	}

/*************************************************************************
 *
 * Abstract Syntax Class
 *
 ************************************************************************/
//构造函数
AbstractSyntax	::	AbstractSyntax()
	{
	ItemType = 0x30;
	Reserved1 = 0;
	}
//构造函数,设置初始值
AbstractSyntax	::	AbstractSyntax(UID	&uid)
	{
	ItemType = 0x30;
	Reserved1 = 0;
	AbstractSyntaxName = uid;
	}
//构造函数,设置名称
AbstractSyntax	:: AbstractSyntax(BYTE	*name)
	{
	ItemType = 0x30;
	Reserved1 = 0;
	AbstractSyntaxName.Set(name);
	}
//析构函数
AbstractSyntax	::	~AbstractSyntax()
	{
	// nothing to de-allocate specifically

	};
//设置uid值
void	AbstractSyntax	::	Set(UID	&uid)
	{
	AbstractSyntaxName = uid;
	};
//设置名称
void	AbstractSyntax	::	Set(BYTE	*name)
	{
	AbstractSyntaxName.Set(name);
	}
//序列化
BOOL	AbstractSyntax	::	Write(Buffer	&Link)
	{
	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) AbstractSyntaxName.GetBuffer(Length), Length);
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	AbstractSyntax	::	Read(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下结构
BOOL	AbstractSyntax	::	ReadDynamic(Buffer	&Link)
	{
	Link >> Reserved1;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) AbstractSyntaxName.GetBuffer(Length), Length);
	AbstractSyntaxName.GetBuffer(Length)[Length] = '\0';
	AbstractSyntaxName.SetLength(Length);
	return ( TRUE );
	}
//得到结构大小
UINT32	AbstractSyntax	::	Size()
	{
	Length = AbstractSyntaxName.GetSize();
	return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Length );
	}

/************************************************************************
 *
 * Transfer Syntax
 *
 ************************************************************************/
//构造函数
TransferSyntax	::	TransferSyntax()
	{
	ItemType = 0x40;
	Reserved1 = 0;
	EndianType=0;
	}
//构造函数,设置uid
TransferSyntax	::	TransferSyntax(UID	&uid)
	{
	ItemType = 0x40;
	Reserved1 = 0;
	TransferSyntaxName = uid;
	}
//构造函数,设置名称
TransferSyntax	:: TransferSyntax(BYTE	*name)
	{
	ItemType = 0x40;
	Reserved1 = 0;
	TransferSyntaxName.Set(name);
	}
//析构函数
TransferSyntax	::	~TransferSyntax()
	{
	// nothing to de-allocate specifically

	};
//设置uid
void	TransferSyntax	::	Set(UID	&uid)
	{
	TransferSyntaxName = uid;
	};
//设置名称
void	TransferSyntax	::	Set(BYTE	*name)
	{
	TransferSyntaxName.Set(name);
	}
//序列化
BOOL	TransferSyntax	::	Write(Buffer	&Link)
	{
//	fprintf(stderr, "Writing Transfer Syntax: %d ItemType\n", ItemType);
//	fprintf(stderr, "Length = %d\n", Length);
//	fprintf(stderr, "UID: %s\n", TransferSyntaxName.GetBuffer(Length));

	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) TransferSyntaxName.GetBuffer(Length), Length);
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	TransferSyntax	::	Read(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	//fprintf(stderr, "Reading Transfter Syntax Magic Code: %x\n", ItemType);
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	TransferSyntax	::	ReadDynamic(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) TransferSyntaxName.GetBuffer(Length), Length);
	TransferSyntaxName.GetBuffer(Length)[Length] = '\0';
	TransferSyntaxName.SetLength(Length);
	return ( TRUE );
	}
//结构大小
UINT32	TransferSyntax	::	Size()
	{
	Length = TransferSyntaxName.GetSize();
	return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Length );
	}



/************************************************************************
 *
 *  Implementation Class
 *
 ************************************************************************/
//构造函数
ImplementationClass	::	ImplementationClass()
	{
	ItemType = 0x52;
	Reserved1 = 0;
	EndianType=0;
	}
//构造函数,设置uid
ImplementationClass	::	ImplementationClass(UID	&uid)
	{
	ItemType = 0x52;
	Reserved1 = 0;
	ImplementationName = uid;
	}
//构造函数,设置名称
ImplementationClass	:: ImplementationClass(BYTE	*name)
	{
	ItemType = 0x52;
	Reserved1 = 0;
	ImplementationName.Set(name);
	}
//析构函数
ImplementationClass	::	~ImplementationClass()
	{
	// nothing to de-allocate specifically

	};
//设置uid
void	ImplementationClass	::	Set(UID	&uid)
	{
	ImplementationName = uid;
	};
//设置名称
void	ImplementationClass	::	Set(BYTE	*name)
	{
	ImplementationName.Set(name);
	}
//序列化
BOOL	ImplementationClass	::	Write(Buffer	&Link)
	{
	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) ImplementationName.GetBuffer(Length), Length);
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	ImplementationClass	::	Read(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	ImplementationClass	::	ReadDynamic(Buffer	&Link)
	{
	Link >> Reserved1;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) ImplementationName.GetBuffer(Length), Length);
	ImplementationName.GetBuffer(Length)[Length] = '\0';
	ImplementationName.SetLength(Length);
	return ( TRUE );
	}
//得到结构大小
UINT32	ImplementationClass	::	Size()
	{
	Length = ImplementationName.GetSize();
	return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Length );
	}


/************************************************************************
 *
 *  Implementation Class
 *
 ************************************************************************/
//构造函数
ImplementationVersion	::	ImplementationVersion()
	{
	ItemType = 0x55;
	Reserved1 = 0;
	EndianType=0;
	}
//构造函数,设置uid
ImplementationVersion	::	ImplementationVersion(UID	&uid)
	{
	ItemType = 0x55;
	Reserved1 = 0;
	Version = uid;
	}
//构造函数,设置名称
ImplementationVersion	:: ImplementationVersion(BYTE	*name)
	{
	ItemType = 0x55;
	Reserved1 = 0;
	Version.Set(name);
	}
//析构函数
ImplementationVersion	::	~ImplementationVersion()
	{
	// nothing to de-allocate specifically

	};
//设置uid
void	ImplementationVersion	::	Set(UID	&uid)
	{
	Version = uid;
	};
//设置名称
void	ImplementationVersion	::	Set(BYTE	*name)
	{
	Version.Set(name);
	}
//序列化
BOOL	ImplementationVersion	::	Write(Buffer	&Link)
	{
	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) Version.GetBuffer(Length), Length);
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	ImplementationVersion	::	Read(Buffer	&Link)
	{
	Link >> ItemType;	//Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	ImplementationVersion	::	ReadDynamic(Buffer	&Link)
	{
	Link >> Reserved1;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) Version.GetBuffer(Length), Length);
	Version.GetBuffer(Length)[Length] = '\0';
	Version.SetLength(Length);
	return ( TRUE );
	}
//获得大小
UINT32	ImplementationVersion	::	Size()
	{
	Length = Version.GetSize();
	return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Length );
	}

/************************************************************************
 *
 * Presentation Context
 *
 ************************************************************************/
//构造函数
PresentationContext	::	PresentationContext()
	{
	ItemType = 0x20;
	Reserved1 = 0;
	PresentationContextID = uniq8odd();
	Reserved2 = 0;
	Reserved3 = 0;
	Reserved4 = 0;
//	fprintf(stderr, "Creating PresentationContext / %d\n",
//		TrnSyntax.ClearType);
	}
//构造函数,设置初始值
PresentationContext	::	PresentationContext(
	AbstractSyntax	&Abs, 
	TransferSyntax	&Tran)
	{
	AbsSyntax = Abs;
	TrnSyntax.Add ( Tran );
	ItemType = 0x20;
	Reserved1 = 0;
	PresentationContextID = uniq8odd();
	Reserved2 = 0;
	Reserved3 = 0;
	Reserved4 = 0;
	}
//析构函数
PresentationContext	::	~PresentationContext()
	{
//	fprintf (stderr, "~PresentationContext ClearType=%d\n", TrnSyntax.ClearType);
	//TrnSyntax.FreeData ( FALSE );
	TrnSyntax.RemoveAll();
	}
//重新初始化
void	PresentationContext::Reset()
	{
		PresentationContextID = uniq8odd();
		TrnSyntax.Clear();

	}
//设置AbstractSyntax
void	PresentationContext	::	SetAbstractSyntax(AbstractSyntax	&Abs)
	{
	AbsSyntax = Abs;
	}
//添加AbstractSyntax
void	PresentationContext	::	AddTransferSyntax(TransferSyntax	&Tran)
	{
	TrnSyntax.Add ( Tran );

	}
//序列化
BOOL	PresentationContext	::	Write ( Buffer	&Link )
	{
	UINT	Index;

	Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));	
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Write((BYTE *) &PresentationContextID, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved2, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved3, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved4, sizeof(BYTE));
	AbsSyntax.Write(Link);
	Link.Flush();
	Index = 0;
	while ( Index < TrnSyntax.GetSize() )
		{
		TrnSyntax[ Index ].Write(Link);
		++Index;
		}
	if ( Index )
		return ( TRUE );
	return ( FALSE );
	}
//读取结构
BOOL	PresentationContext	::	Read (Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	PresentationContext	::	ReadDynamic (Buffer	&Link)
	{
	INT32			Count;
	TransferSyntax	Tran;

	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link.Read((BYTE *) &PresentationContextID, sizeof(BYTE));
	Link.Read((BYTE *) &Reserved2, sizeof(BYTE));
	Link.Read((BYTE *) &Reserved3, sizeof(BYTE));
	Link.Read((BYTE *) &Reserved4, sizeof(BYTE));

	Count = Length - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE);
//	fprintf(stderr, "Pres Context Length: %d\n", Length);
//	fprintf(stderr, "Reading Abstract Syntax\n");
	AbsSyntax.Read(Link);
	Count = Count - AbsSyntax.Size();
	while ( Count > 0)
		{
//		fprintf(stderr, "Reading Transfer Syntax Count = %d\n", Count);
		Tran.Read ( Link );
		Count = Count - Tran.Size();
		TrnSyntax.Add ( Tran );
		}
//	fprintf(stderr, "Read Presentation Context, ClearType = %d\n", 
//		TrnSyntax.ClearType);
	if ( !Count)
		return ( TRUE );
	return ( FALSE );
	}
//得到结构大小
UINT32	PresentationContext	::	Size()
	{
	UINT	Index;
	Length = sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE);
	Length += AbsSyntax.Size();
	Index = 0;
	while(Index < TrnSyntax.GetSize() )
		{
		Length += TrnSyntax.Get(Index).Size();
		++Index;
		}
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
	}

/************************************************************************
 *
 * Maximum Sub Length
 *
 ************************************************************************/
//构造函数
MaximumSubLength	::	MaximumSubLength()
	{
	ItemType = 0x51;
	Reserved1 = 0;
	}
//构造函数,设置最大长度
MaximumSubLength	::	MaximumSubLength(UINT32	Max)
	{
	ItemType = 0x51;
	Reserved1 = 0;
	MaximumLength = Max;
	}
//析构函数
MaximumSubLength	::	~MaximumSubLength()
	{
	// nothing to de-allocate
	}
//设置最大长度
void	MaximumSubLength	::	Set(UINT32	Max)
	{
	MaximumLength = Max;
	}
//返回最大长度
UINT32	MaximumSubLength	::	Get()
	{
	return ( MaximumLength );
	}
//序列化
BOOL	MaximumSubLength	::	Write(Buffer	&Link)
	{
	Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link << MaximumLength; //Link.Write((BYTE *) &MaximumLength, sizeof(UINT32));
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	MaximumSubLength	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	MaximumSubLength	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link >> MaximumLength; //Link.Read((BYTE *) &MaximumLength, sizeof(UINT32));

	return ( TRUE );
	}
//获得结构大小
UINT32	MaximumSubLength	::	Size()
	{
	Length = sizeof(UINT32);
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
	}



/******** SCP / SCU Role Select ***********/

//构造函数
SCPSCURoleSelect	::	SCPSCURoleSelect()
	{
	ItemType = 0x54;
	Reserved1 = 0;
	}
//析构函数
SCPSCURoleSelect	::	~SCPSCURoleSelect()
	{
	// nothing to de-allocate
	}

//序列化
BOOL	SCPSCURoleSelect	::	Write(Buffer	&Link)
	{
	//[peak
	//storage commitment
//	if(strncmp((const char*)uid.GetBuffer(1),"1.2.840.10008.1.20.1",20)!=0)
//	{
//		return TRUE;
//	}
//	SCURole=0x0;
//	SCPRole=0x1;
	//]
	UINT16	TL;

	ItemType = 0x54;
	
	Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT16));
	TL = uid.GetSize();
	Link << TL; //Link.Write((BYTE *) &MaximumLength, sizeof(UINT32));
	Link.Write((BYTE *) uid.GetBuffer(1), TL);
	Link << SCURole;
	Link << SCPRole;
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	SCPSCURoleSelect	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	SCPSCURoleSelect	::	ReadDynamic(Buffer	&Link)
	{
	UINT16	TL;
	
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Link >> TL;
	Link.Read((BYTE*) uid.GetBuffer(TL+1), TL);
	uid.GetBuffer(Length)[Length] = '\0';
	uid.SetLength(TL);
	Link >> SCURole;
	Link >> SCPRole;
	return ( TRUE );
	}
//获得结构大小
UINT32	SCPSCURoleSelect	::	Size()
	{
	Length = sizeof(UINT16) + uid.GetSize() + sizeof(BYTE) + sizeof(BYTE);
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) );
	}

/************************************************************************
 *
 * User Information
 *
 ************************************************************************/
//用户信息构造
UserInformation	::	UserInformation()
	{
	ItemType = 0x50;
	Reserved1 = 0;
	UserInfoBaggage = 0;
	}
//析构
UserInformation	::	~UserInformation()
	{
	// nothing to de-allocate
	}
//设置最大长度
void	UserInformation	::	SetMax(MaximumSubLength	&Max)
	{
	MaxSubLength = Max;
	}
//得到最大长度
UINT32	UserInformation	::	GetMax()
	{
	return(MaxSubLength.Get());
	}
//序列化
BOOL	UserInformation	::	Write(Buffer	&Link)
	{
 	Link << ItemType;	//Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link << Reserved1;	//Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;		//Link.Write((BYTE *) &Length, sizeof(UINT16));
	Link.Flush();
	MaxSubLength.Write(Link);
	ImpClass.Write(Link);
//[peak
//	SCPSCURole.Write(Link);
//]
	ImpVersion.Write(Link);
	return ( TRUE );
	}
//读取结构
BOOL	UserInformation	::	Read(Buffer	&Link)
	{
	Link >> ItemType; //Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//读取余下信息
BOOL	UserInformation	::	ReadDynamic(Buffer	&Link)
	{
	BYTE		TempByte;
//	BYTE		TempByte2;
//	UINT16		TempLength;
	INT32		Count;

	UserInfoBaggage = 0;
	
	Link >> Reserved1;	//Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;		//Link.Read((BYTE *) &Length, sizeof(UINT16));
	Count = Length;
//	fprintf(stderr, "User Info Length: %d\n", Length);
	while(Count > 0)
		{
		Link >> TempByte;	//Link.Read((BYTE *) &TempByte, sizeof(BYTE));
		switch ( TempByte )
			{
			case	0x51:
//				fprintf(stderr, "Reading Max Sub Length Count = %d\n", Count);
				MaxSubLength.ReadDynamic(Link);
				Count = Count - MaxSubLength.Size();
				break;
			case	0x52:
//				fprintf(stderr, "Reading Implementation Class Count = %d\n", Count);
				ImpClass.ReadDynamic(Link);
				Count = Count - ImpClass.Size();
				break;
			case	0x54:	// Role selection
				SCPSCURole.ReadDynamic(Link);
				Count = Count - SCPSCURole.Size();
				UserInfoBaggage += SCPSCURole.Size();
				break;
			case	0x55:
//				fprintf(stderr, "Reading Implemenation Version Count = %d\n", Count);
				ImpVersion.ReadDynamic(Link);
				Count = Count - ImpVersion.Size();
				break;
			default:
				//fprintf(stderr, "Unknown Packet: %x, Count = %d\n", TempByte, Count);
				Link.Kill(Count-1);	// unknown user information packet
				UserInfoBaggage = Count;
				Count = -1;
			}
		}
//	fprintf(stderr, "ending User Info ReadD Count = %d\n", Count);
	if(!Count)
		return ( TRUE );
	return ( FALSE );
	}
//得到结构大小
UINT32	UserInformation	::	Size()
	{
	Length = MaxSubLength.Size();
	Length += ImpClass.Size();
	Length += ImpVersion.Size();
	return ( Length + UserInfoBaggage + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
	}

/************************************************************************
 *
 * AAssociateRQ Packet
 *
 ************************************************************************/
//构造函数
AAssociateRQ	::	AAssociateRQ()
	{
	ItemType = 0x01;
	Reserved1 = 0;
	ProtocolVersion = 0x0001;
	Reserved2 = 0;
	SpaceMem(CalledApTitle, 16);
	CalledApTitle[16]='\0';
	SpaceMem(CallingApTitle, 16);
	CallingApTitle[16]='\0';
	ZeroMem(Reserved3, 32);
	}
//构造函数,设置呼叫方和接收方AET
AAssociateRQ	::	AAssociateRQ(BYTE	*CallingAp, BYTE	*CalledAp)
	{
	ItemType = 0x01;
	Reserved1 = 0;
	ProtocolVersion = 0x0001;
	Reserved2 = 0;
	SpaceMem(CalledApTitle, 16);
	CalledApTitle[16]='\0';
	SpaceMem(CallingApTitle, 16);
	CallingApTitle[16]='\0';
	ZeroMem(Reserved3, 32);
	memcpy(CallingApTitle, CallingAp,min(strlen((char*)CallingAp),16u));
	memcpy(CalledApTitle, CalledApTitle,min(strlen((char*)CalledApTitle),16u));
	}
//析构函数
AAssociateRQ	::	~AAssociateRQ()
	{
	// nothing, everything should self-destruct nicely
	while(PresContexts.GetSize())
		{
		PresContexts.Get ( 0 ).TrnSyntax.ClearType = TRUE;
		PresContexts.RemoveAt ( 0 );
		}

	PresContexts.ClearType = TRUE;
//	fprintf(stderr, "~AAssociateRQ : PresContexts.ClearType = %d\n",
//		PresContexts.ClearType);
	}
//设置呼叫方AET
void	AAssociateRQ	::	SetCalledApTitle(BYTE	*CalledAp)
	{
	SpaceMem(CalledApTitle, 16);
	memcpy(CalledApTitle, CalledAp, min(strlen((char *)CalledAp), 16u));
	}
//设置接收方AET
void	AAssociateRQ	::	SetCallingApTitle(BYTE	*CallingAp)
	{
	SpaceMem(CallingApTitle, 16);
	memcpy(CallingApTitle, CallingAp, min(strlen((char *)CallingAp), 16u));
	}
//设置应用上下文
void	AAssociateRQ	::	SetApplicationContext(ApplicationContext	&AppC)
	{
	AppContext = AppC;
	}
//设置应用上下文uid
void	AAssociateRQ	::	SetApplicationContext(UID &uid)
	{
	AppContext.Set(uid);
	}
//添加PresentationContext
void	AAssociateRQ	::	AddPresentationContext(PresentationContext	&PresContext)
	{
	PresContexts.Add(PresContext);
	PresContexts.Get(PresContexts.GetSize()-1).TrnSyntax.ClearType = TRUE;
	}
//设置用户信息
void	AAssociateRQ	::	SetUserInformation(UserInformation	&User)
	{
	UserInfo = User;
	}
//清楚PresentationContext
void	AAssociateRQ	::	ClearPresentationContexts()
	{
	while(PresContexts.GetSize())
		PresContexts.RemoveAt ( 0 );
	return;
	}
//序列化
BOOL	AAssociateRQ	::	Write(Buffer	&Link)
	{
	UINT	Index;

	//fprintf(stderr, "AAssociateRQ :: Write ()\n");fflush(stderr);
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
	AppContext.Write(Link);
	Index = 0;
	while(Index < PresContexts.GetSize())
		{
		PresContexts[Index].Write(Link);
		++Index;
		}
	UserInfo.Write(Link);
	return ( TRUE );
	}
//读取结构
BOOL	AAssociateRQ	::	Read(Buffer	&Link)
	{
//	fprintf(stderr, "Attempting to Read AAssociateRQ PDU\n");
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
//	fprintf(stderr, "Got ItemType: %x\n", ItemType);
	return(this->ReadDynamic(Link));
	}
//读取余下信息
BOOL	AAssociateRQ	::	ReadDynamic(Buffer	&Link)
	{
//	UINT				Index;
	INT					Count;
	BYTE				TempByte;
	PresentationContext	*PresContext;

	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >> ProtocolVersion;	//Link.Read((BYTE *) &ProtocolVersion, sizeof(UINT16));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	Link.Read((BYTE *) CalledApTitle, 16);
	Link.Read((BYTE *) CallingApTitle, 16);
	Link.Read((BYTE *) Reserved3, 32);
	CalledApTitle[16] = '\0';
	CallingApTitle[16] = '\0';
	//对方送过来的AET有空格后缀，去掉。
	MTrimRight((char*)CalledApTitle);
	MTrimRight((char*)CallingApTitle);
	//
//	fprintf(stderr, "Item Length: %d\n", Length);
//	fprintf(stderr, "Protocol Version: %d\n", ProtocolVersion);
//	fprintf(stderr, "CalledApTitle: %16.16s\n", CalledApTitle);
//	fprintf(stderr, "CallingApTitle: %16.16s\n", CallingApTitle);

	Count = Length - sizeof(UINT16) - sizeof(UINT16) - 16 - 16 - 32;
	while(Count > 0)
		{
		Link.Read((BYTE *) &TempByte, sizeof(BYTE));
		switch(TempByte)
			{
			case	0x50:	// user information
//				fprintf(stderr, "Reading User Information (%d)\n", Count);
				UserInfo.ReadDynamic(Link);
				Count = Count - UserInfo.Size() - UserInfo.UserInfoBaggage;
//				fprintf(stderr, "Count = %d/UserInfo = %d\n", Count,
//					UserInfo.Size());
				break;
			case	0x20:
//				fprintf(stderr, "Reading Presentation Context (%d)\n", Count);
				PresContext = new PresentationContext;
				PresContext->TrnSyntax.ClearType = TRUE;
				PresContext->ReadDynamic(Link);
				Count = Count - PresContext->Size();
				PresContexts.Add(*PresContext);
				PresContext->TrnSyntax.ClearType = FALSE;
				delete PresContext;

				break;
			case	0x10:
//				fprintf(stderr, "Reading AppContext (%d)\n", Count);
				AppContext.ReadDynamic(Link);
				Count = Count - AppContext.Size();
				break;
			default:
				//fprintf(stderr, "unknown item: %x ending\n", TempByte);
				Link.Kill(Count-1);
				Count = -1;
			}
		}
	if(!Count)
		return ( TRUE );
	return ( FALSE);
	}
//得到结构大小
UINT32	AAssociateRQ	::	Size()
	{
	UINT	Index;

	Length = sizeof(UINT16) + sizeof(UINT16) + 16 + 16 + 32;
	Length += AppContext.Size();

	Index = 0;
	while(Index < PresContexts.GetSize())
		{
		Length += PresContexts[Index].Size();
		++Index;
		}
	Length += UserInfo.Size();
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}
