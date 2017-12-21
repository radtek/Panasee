//������������Ҫ
//���������������ӹ��̵�һЩ�����ͽṹ
//M��ʾ�����
//C��ʾ����������
//U��ʾ��ѡ��
//=��ʾ���Ժ�ԭ����һ��
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

//�����е�DICOM����,������ļ���������read,write,setif,morph,reset,pop,push
//�⼸������������ÿ��ʵ�����й��ܸ�����ͬ,������ʵ���Ƕ���һ����
//�ܶ඼�����麯���ķ�ʽʵ��

//UID�Ǹ�ȫ��Ψһid
//���С���λ64�ֽ�
//��\0����
//��Щ�����Ǹ���UID�Ĳ���
//���ڲ���Ա��Ҫ��2��
//�ڴ�uid�ͳ���
//�����õ�����������,�õ������ó���,��ֵ
//�Լ�=�жϵ�
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
//����������������
//itemtype�̶���0x10
//ApplicationContextName��ʵ���Ǹ�uid,ֻ��Ӧ�õ�Э������䵱
//Ӧ�������Ľ�ɫ,����Ӧ�����������.�������ҵ������Ĳ�һ��
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
//�����﷨
//����ȷ�����䷽ʽ
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
//ʵ����Ϣ��
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

//ʵ�ְ汾��
//˵����汾�йص�����
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

//��ԭ����DICOM��,����ѡ��������ṩ�ߺͷ�����ʹ���߻���
//���ڻ�����û����,�ù���ʵ�����Ѿ�������
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

//���������	
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
//��󳤶���,����Э��ÿ��pdv��������ݳ���
//����ó���̫��,����ʹ�öԷ��޷�����
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
//�û���Ϣ
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
//A-ASSOC����
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


