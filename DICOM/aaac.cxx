#	include	"dicom.hpp"


/************************************************************************
 *
 * Presentation Context Accept
 *
 ************************************************************************/
//��ʼ������,�޲���
PresentationContextAccept	::	PresentationContextAccept()
	{
	ItemType = 0x21;
	Reserved1 = 0;
	PresentationContextID = uniq8();
	Reserved2 = 0;
	Result = 2;
	Reserved4 = 0;
	}
//��ʼ������,��������ֵ
//���Ƴ�ʼ�������SetTransferSyntax
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
//��������
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
//�����ڲ�transsytax
void	PresentationContextAccept	::	SetTransferSyntax(TransferSyntax	&Tran)
	{
	TrnSyntax = Tran;
	}
//���л�PresentationContextAccept
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
//��ȡPresentationContextAccept
BOOL	PresentationContextAccept	::	Read (Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return ( this->ReadDynamic(Link) );
	}
//���սṹ��ȡ����
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
//��̬��ýṹ��С
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
//��ʼ������
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
//��������ʼ��
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
//��������
AAssociateAC	::	~AAssociateAC()
	{
	// nothing, everything should self-destruct nicely
	}
//���ý��շ�AET
void	AAssociateAC	::	SetCalledApTitle(BYTE	*CalledAp)
	{
	SpaceMem(CalledApTitle, 16);
	memcpy(CalledApTitle, CalledAp, min(strlen((char *)CalledAp), 16u));
	}
//��������AET
void	AAssociateAC	::	SetCallingApTitle(BYTE	*CallingAp)
	{
	SpaceMem(CallingApTitle, 16);
	memcpy(CallingApTitle, CallingAp, min(strlen((char *)CallingAp), 16u));
	}
//����Ӧ��������
void	AAssociateAC	::	SetApplicationContext(ApplicationContext	&AppC)
	{
	AppContext = AppC;
	}
//��ӿɽ��յı��������
void	AAssociateAC	::	AddPresentationContextAccept(PresentationContextAccept	&PresContextAccept)
	{
	PresContextAccepts.Add(PresContextAccept);
	}
//�����û���Ϣ
void	AAssociateAC	::	SetUserInformation(UserInformation	&User)
	{
	UserInfo = User;
	}
//���л�
//���ṹд����Link
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
//��link��ȡ���ݵ��ṹ
BOOL	AAssociateAC	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//��ȡ��������
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
//��ýṹ��С
UINT32	AAssociateAC	::	Size()
	{
	UINT	Index;
//���⿪��
	Length = sizeof(UINT16) + sizeof(UINT16) + 16 + 16 + 32;
	Length += AppContext.Size();
	Index = 0;
//PresContextAccepts��С
	Index = 0;
	while(Index < PresContextAccepts.GetSize())
		{
		Length += PresContextAccepts[Index].Size();
		++Index;
		}
//Userinfo��С
	Length += UserInfo.Size();
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

