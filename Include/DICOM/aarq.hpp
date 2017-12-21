//下面这个表很重要
//他描述了整个连接过程的一些参数和结构
//M表示必须的
//C表示有条件必须
//U表示可选的
//=表示可以和原来的一样
//KEY A-ASSOCIATE SERVICE PARAMETERS
//A-ASSOCIATE parameter	name		Request	Indication	Response	Confirmation
//application context name			M		M(=)		M			M(=)
//calling AE title					M		M(=)		M			M(=)
//called AE title					M		M(=)		M			M(=)
//user information					M		M(=)		M			M(=)
//result												M			M(=)
//result source														M
//diagnostic											U			C(=)
//calling presentation address							M			M(=)
//called presentation address							M			M(=)
//presentation context definition list					M			M(=)
//presentation context definition list result			M			M(=)

//在所有的DICOM类中,最基本的几个函数是read,write,setif,morph,reset,pop,push
//这几个函数尽管在每个实现类中功能各不相同,但是其实质是都是一样的
//很多都是用虚函数的方式实现

//UID是个全球唯一id
//其大小最大位64字节
//用\0结束
//这些函数是辅助UID的操作
//其内部成员主要有2个
//内存uid和长度
//包括得到和设置数据,得到和设置长度,赋值
//以及=判断等
class	UID
	{
	UINT	Length;
	BYTE	uid[65];
	
	public:
			UID()	{ ZeroMem(uid, 64); Length = 0; };
			UID(BYTE *s) { Set ( s ); };
			UID(char *s) { Set ( (BYTE*)s ); };
			void	Set(BYTE *s)
				{
				if(!s)
					return;
				ZeroMem(uid, 64);
				strcpy((char *) uid, (char *) s);
				Length = strlen((char*) uid);
				};
			void	Set(UID	&u) { (*this) = u; };
			void	Set(char *s) { this->Set((BYTE *) s); };
			BYTE	*GetBuffer(UINT Min) { return(&uid[0]); };
			void	SetLength(UINT	L)	{ Length = L; while (L < 65) uid[L++] = '\0'; };
			UINT	GetSize()	{ return ( Length ); };
			BOOL	operator	==	(UID &ud)
				{ 	//if(GetSize()!=ud.GetSize()) return(FALSE);
					if(!strcmp((char*) GetBuffer(1), (char*) ud.GetBuffer(1)/*, (int) GetSize()*/))
						return(TRUE);
					return(FALSE); };
			BOOL	operator	!=	(UID &ud) { return (!((*this)==ud));
};
			BOOL	operator	=	(UID &ud)
				{	memcpy(uid, ud.GetBuffer(1), 64);
					SetLength(ud.GetSize());
					
					return(TRUE);
				}
	};
//该类是运用上下文
//itemtype固定是0x10
//ApplicationContextName其实就是个uid,只是应用到协议里面充当
//应用上下文角色,表达该应用自身的属性.各个厂家的上下文不一样
class	ApplicationContext
	{
	private:
		BYTE		ItemType;		// 0x10
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			ApplicationContextName;

					ApplicationContext();
					ApplicationContext(UID	&);
					ApplicationContext(BYTE *);
					~ApplicationContext();
		ApplicationContext&	operator = (ApplicationContext &AC) 
		{ 
			ApplicationContextName=AC.ApplicationContextName;
			Length=ApplicationContextName.GetSize();
			return *this;
		}
		void		Set(UID	&);
		void		Set(BYTE *);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();
	};
//
class	AbstractSyntax
	{
	private:
		BYTE		ItemType;	// 0x30
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			AbstractSyntaxName;
					AbstractSyntax();
					AbstractSyntax(BYTE *);
					AbstractSyntax(UID &);
					~AbstractSyntax();
		AbstractSyntax&	operator = (AbstractSyntax &AC) 
		{ 
			AbstractSyntaxName=AC.AbstractSyntaxName;
			Length=AbstractSyntaxName.GetSize();
			return *this;
		}


		void		Set(UID	&);
		void		Set(BYTE *);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();

	};
//传输语法
//用于确定传输方式
class	TransferSyntax
	{
	private:
		BYTE		ItemType;	// 0x40
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			TransferSyntaxName;
		UINT		EndianType;
					TransferSyntax();
					TransferSyntax(BYTE *);
					TransferSyntax(UID &);
					~TransferSyntax();
		TransferSyntax&	operator = (TransferSyntax &AC) 
		{ 
			TransferSyntaxName=AC.TransferSyntaxName;
			Length=TransferSyntaxName.GetSize();
			EndianType=AC.EndianType;
			return *this;
		}

		void		Set(UID	&);
		void		Set(BYTE *);
		void		SetType(UINT	T) { EndianType = T; };
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();

	};
//实现信息类
class	ImplementationClass
	{
	private:
		BYTE		ItemType;	// 0x52
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			ImplementationName;
		UINT		EndianType;
					ImplementationClass();
					ImplementationClass(BYTE *);
					ImplementationClass(UID &);
					~ImplementationClass();
		ImplementationClass&	operator = (ImplementationClass &AC) 
		{ 
			ImplementationName=AC.ImplementationName;
			Length=ImplementationName.GetSize();
			EndianType=AC.EndianType;
			return *this;
		}


		void		Set(UID	&);
		void		Set(BYTE *);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();

	};

//实现版本类
//说明与版本有关的内容
class	ImplementationVersion
	{
	private:
		BYTE		ItemType;	// 0x55
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			Version;
		UINT		EndianType;
					ImplementationVersion();
					ImplementationVersion(BYTE *);
					ImplementationVersion(UID &);
					~ImplementationVersion();
		ImplementationVersion&	operator = (ImplementationVersion &AC) 
		{ 
			Version=AC.Version;
			Length=Version.GetSize();
			EndianType=AC.EndianType;
			return *this;
		}
		void		Set(UID	&);
		void		Set(BYTE *);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();

	};

//在原来的DICOM中,可以选择服务类提供者和服务类使用者互换
//现在基本都没有了,该功能实际上已经被废弃
class	SCPSCURoleSelect
	{
	private:
		BYTE		ItemType;	// 0x54
		BYTE		Reserved1;
		UINT16		Length;
	public:
		UID			uid;
		BYTE		SCURole;
		BYTE		SCPRole;
					SCPSCURoleSelect();
					~SCPSCURoleSelect();
		SCPSCURoleSelect&	operator = (SCPSCURoleSelect &AC) 
		{ 
			uid=AC.uid;
			Length=uid.GetSize();
			SCURole=AC.SCURole;
			SCPRole=AC.SCPRole;
			return *this;
		}
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();

	};

//表达上下文	
class	PresentationContext
	{
	private:
		BYTE							ItemType;	// 0x20
		BYTE							Reserved1;
		UINT16							Length;
	public:
		BYTE							PresentationContextID;
	private:
		BYTE							Reserved2;
		BYTE							Reserved3;
		BYTE							Reserved4;
	public:
		AbstractSyntax				AbsSyntax;
		Array<TransferSyntax>		TrnSyntax;
					PresentationContext();
					PresentationContext(AbstractSyntax &, TransferSyntax &);
					~PresentationContext();
///add by lin
		PresentationContext&	operator = (PresentationContext &AC) 
		{ 
			PresentationContextID=AC.PresentationContextID;
			AbsSyntax=AC.AbsSyntax;
			TrnSyntax.RemoveAll();
			for(unsigned int i=0;i<AC.TrnSyntax.GetSize();i++)
			{
				TrnSyntax.Add(AC.TrnSyntax.Get(i));
			}
			Size();
			return *this;
		}
		void		Reset();
////////
		void		SetAbstractSyntax(AbstractSyntax	&);
		void		AddTransferSyntax(TransferSyntax	&);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();
		
	};
//最大长度类,用于协商每个pdv的最大数据长度
//如果该长度太大,将会使得对方无法接收
class	MaximumSubLength
	{
	private:
		BYTE						ItemType;	// 0x51
		BYTE						Reserved1;
		UINT16						Length;
		UINT32						MaximumLength;
	public:
					MaximumSubLength();
					MaximumSubLength(UINT32);
					~MaximumSubLength();
		void		Set(UINT32);
		UINT32		Get();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();
	};
//用户信息
class	UserInformation
	{
	private:
		BYTE						ItemType;	// 0x50
		BYTE						Reserved1;
		UINT16						Length;
	public:
		UINT32						UserInfoBaggage;
		MaximumSubLength			MaxSubLength;
		ImplementationClass			ImpClass;
		ImplementationVersion		ImpVersion;
		SCPSCURoleSelect			SCPSCURole;
	public:
					UserInformation();
					~UserInformation();
		UserInformation&	operator = (UserInformation &AC) 
		{ 
			UserInfoBaggage=AC.UserInfoBaggage;
			MaxSubLength=AC.MaxSubLength;
			ImpClass=AC.ImpClass;
			ImpVersion=AC.ImpVersion;
			ImpVersion=AC.ImpVersion;
			SCPSCURole=AC.SCPSCURole;
			Size();
			return *this;
		}
		void		SetMax(MaximumSubLength	&);
		UINT32		GetMax();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};
//A-ASSOC请求
class	AAssociateRQ
	{
	private:
		BYTE		ItemType;		// 0x01
		BYTE		Reserved1;
		UINT32	Length;
		UINT16	ProtocolVersion;	// 0x01
		UINT16	Reserved2;
	public:
		BYTE		CalledApTitle[17];	// 16 bytes transfered
		BYTE		CallingApTitle[17];	// 16 bytes transfered
		BYTE		Reserved3[32];

		ApplicationContext			AppContext;
		Array<PresentationContext>	PresContexts;
		UserInformation				UserInfo;
	public:
					AAssociateRQ();
					AAssociateRQ(BYTE	*, BYTE	*);
		virtual		~AAssociateRQ();
		void		SetCalledApTitle(BYTE	*);
		void		SetCallingApTitle(BYTE	*);
		void		SetApplicationContext(ApplicationContext &);
		void		SetApplicationContext(UID &);
		void		AddPresentationContext(PresentationContext &);
		void		ClearPresentationContexts();
		void		SetUserInformation(UserInformation &);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			


