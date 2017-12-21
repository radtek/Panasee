// "Presentation Context" is a way of saying,
// "I'm can transfer this type of high level object,
// using any of these low-level transfer syntaxs."  
//(A typical message is: "I'm going to send CT images using implicit Little Endian transfer syntax"). 
// A PresentationContext contains the high level object UID (AbstractSyntax), 
//and a number of propesed TransferSyntax UIDs.


//类说明
//该类用于操作接受的表示上下文
//ItemType是类型
//PresentationContextID是表示上下文的id,此id用于通讯时候用
//其他一些都是输入输出以及设置内部变量函数		
class	PresentationContextAccept
	{
	private:
		BYTE							ItemType;	// 0x21
		BYTE							Reserved1;
		UINT16							Length;
	public:
		BYTE							PresentationContextID;
	private:
		BYTE							Reserved2;
	public:
		BYTE							Result;
	private:
		BYTE							Reserved4;
	public:
//		AbstractSyntax				AbsSyntax;
		TransferSyntax				TrnSyntax;
					PresentationContextAccept();
					PresentationContextAccept(/*AbstractSyntax &, */TransferSyntax &);
					~PresentationContextAccept();
//		void		SetAbstractSyntax(AbstractSyntax	&);
		void		SetTransferSyntax(TransferSyntax	&);
		void		SetResult(BYTE	Res) { Result = Res; };
		BYTE		GetResult()	{ return ( Result ); };
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32	Size();
		
	};
//连接接受类
//其中ItemType都是固定的
//CalledApTitle是被叫方AET
//CallingApTitle是呼叫方AET
//都是16个字节,后面用空格补齐
//UserInformation位于连接协议后面
//先协商AET,然后是表示上下文,最后才是用户信息
//read,write用于获取结构信息和将结构信息写入缓存
//Size用于获取整个结构大小.
class	AAssociateAC
	{
	private:
		BYTE		ItemType;		// 0x02
		BYTE		Reserved1;
		UINT32	Length;
		UINT16	ProtocolVersion;	// 0x01
		UINT16	Reserved2;
	public:
		BYTE		CalledApTitle[17];	// 16 bytes transfered
		BYTE		CallingApTitle[17];	// 16 bytes transfered
		BYTE		Reserved3[32];
		ApplicationContext					AppContext;
		Array<PresentationContextAccept>	PresContextAccepts;
		UserInformation						UserInfo;
	public:
					AAssociateAC();
					AAssociateAC(BYTE	*, BYTE	*);
		virtual			~AAssociateAC();
		void		SetCalledApTitle(BYTE	*);
		void		SetCallingApTitle(BYTE	*);
		void		SetApplicationContext(ApplicationContext &);
		void		AddPresentationContextAccept(PresentationContextAccept &);
		void		SetUserInformation(UserInformation &);
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			


