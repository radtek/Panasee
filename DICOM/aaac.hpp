// "Presentation Context" is a way of saying,
// "I'm can transfer this type of high level object,
// using any of these low-level transfer syntaxs."  
//(A typical message is: "I'm going to send CT images using implicit Little Endian transfer syntax"). 
// A PresentationContext contains the high level object UID (AbstractSyntax), 
//and a number of propesed TransferSyntax UIDs.


//��˵��
//�������ڲ������ܵı�ʾ������
//ItemType������
//PresentationContextID�Ǳ�ʾ�����ĵ�id,��id����ͨѶʱ����
//����һЩ������������Լ������ڲ���������		
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
//���ӽ�����
//����ItemType���ǹ̶���
//CalledApTitle�Ǳ��з�AET
//CallingApTitle�Ǻ��з�AET
//����16���ֽ�,�����ÿո���
//UserInformationλ������Э�����
//��Э��AET,Ȼ���Ǳ�ʾ������,�������û���Ϣ
//read,write���ڻ�ȡ�ṹ��Ϣ�ͽ��ṹ��Ϣд�뻺��
//Size���ڻ�ȡ�����ṹ��С.
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


