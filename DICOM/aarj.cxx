#	include	"dicom.hpp"


/************************************************************************
 *
 * AAssociateRJ Packet
 *
 ************************************************************************/
//初始化函数,无参数

AAssociateRJ	::	AAssociateRJ()
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Result = 1;
	Source = 3;
	Reason = 1;
	}
//初始化函数,带参数赋值
//说明拒绝原因,源,代码
AAssociateRJ	::	AAssociateRJ(BYTE Res, BYTE S, BYTE Re)
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Result = Res;
	Source = S;
	Reason = Re;
	}
//析构函数
AAssociateRJ	::	~AAssociateRJ()
	{
	// nothing, everything should self-destruct nicely
	}

//序列化结构
BOOL	AAssociateRJ	::	Write(Buffer	&Link)
	{
	Size();
	Link << ItemType; //Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT32));
	Link << Reserved2;			//Link.Write((BYTE *) &Reserved2, sizeof(UINT16));
	Link << Result;
	Link << Source;
	Link << Reason;
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	AAssociateRJ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//读取余下结构
BOOL	AAssociateRJ	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	Link >> Result;
	Link >> Source;
	Link >> Reason;

	return (TRUE);
	}
//获得结构大小
UINT32	AAssociateRJ	::	Size()
	{
	Length = 4;
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}


/************************************************************************
 *
 * AReleaseRQ Packet
 *
 ************************************************************************/
//构造函数
AReleaseRQ	::	AReleaseRQ()
	{
	ItemType = 0x05;
	Reserved1 = 0;
	Reserved2 = 0;
	}
//析构函数
AReleaseRQ	::	~AReleaseRQ()
	{
	// nothing, everything should self-destruct nicely
	}

//序列化
BOOL	AReleaseRQ	::	Write(Buffer	&Link)
	{
	Size();
	Link << ItemType; //Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT32));
	Link << Reserved2;			//Link.Write((BYTE *) &Reserved2, sizeof(UINT16));
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	AReleaseRQ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//读取余下结构
BOOL	AReleaseRQ	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	return (TRUE);
	}
//得到结构大小
UINT32	AReleaseRQ	::	Size()
	{
	Length = 4;
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

/************************************************************************
 *
 * AReleaseRP Packet
 *
 ************************************************************************/
//构造函数
AReleaseRP	::	AReleaseRP()
	{
	ItemType = 0x06;
	Reserved1 = 0;
	Reserved2 = 0;
	}
//析构函数
AReleaseRP	::	~AReleaseRP()
	{
	// nothing, everything should self-destruct nicely
	}

//序列化
BOOL	AReleaseRP	::	Write(Buffer	&Link)
	{
	Size();
	Link << ItemType; //Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT32));
	Link << Reserved2;			//Link.Write((BYTE *) &Reserved2, sizeof(UINT16));
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	AReleaseRP	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//读取余下结构
BOOL	AReleaseRP	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	return (TRUE);
	}
//计算结构大小
UINT32	AReleaseRP	::	Size()
	{
	Length = 4;
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

/************************************************************************
 *
 * AAbortRQ Packet
 *
 ************************************************************************/
//构造函数
AAbortRQ	::	AAbortRQ()
	{
	ItemType = 0x07;
	Reserved1 = 0;
	Reserved2 = 0;
	Reserved3 = 0;
	Source = 3;
	Reason = 1;
	}
//构造函数,设置中止原因
AAbortRQ	::	AAbortRQ(BYTE S, BYTE Re)
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Reserved3 = 0;
	Source = S;
	Reason = Re;
	}
//析构函数
AAbortRQ	::	~AAbortRQ()
	{
	// nothing, everything should self-destruct nicely
	}

//序列化
BOOL	AAbortRQ	::	Write(Buffer	&Link)
	{
	Size();
	Link << ItemType; //Link.Write((BYTE *) &ItemType, sizeof(BYTE));
	Link.Write((BYTE *) &Reserved1, sizeof(BYTE));
	Link << Length;	//Link.Write((BYTE *) &Length, sizeof(UINT32));
	Link << Reserved2;			//Link.Write((BYTE *) &Reserved2, sizeof(UINT16));
	Link << Reserved3;
	Link << Source;
	Link << Reason;
	Link.Flush();
	return ( TRUE );
	}
//读取结构
BOOL	AAbortRQ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//读取余下信息
BOOL	AAbortRQ	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	Link >> Reserved3;
	Link >> Source;
	Link >> Reason;

	return ( TRUE );
	}
//得到结构大小
UINT32	AAbortRQ	::	Size()
	{
	Length = 4;
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

