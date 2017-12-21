#	include	"dicom.hpp"


/************************************************************************
 *
 * AAssociateRJ Packet
 *
 ************************************************************************/
//��ʼ������,�޲���

AAssociateRJ	::	AAssociateRJ()
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Result = 1;
	Source = 3;
	Reason = 1;
	}
//��ʼ������,��������ֵ
//˵���ܾ�ԭ��,Դ,����
AAssociateRJ	::	AAssociateRJ(BYTE Res, BYTE S, BYTE Re)
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Result = Res;
	Source = S;
	Reason = Re;
	}
//��������
AAssociateRJ	::	~AAssociateRJ()
	{
	// nothing, everything should self-destruct nicely
	}

//���л��ṹ
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
//��ȡ�ṹ
BOOL	AAssociateRJ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//��ȡ���½ṹ
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
//��ýṹ��С
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
//���캯��
AReleaseRQ	::	AReleaseRQ()
	{
	ItemType = 0x05;
	Reserved1 = 0;
	Reserved2 = 0;
	}
//��������
AReleaseRQ	::	~AReleaseRQ()
	{
	// nothing, everything should self-destruct nicely
	}

//���л�
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
//��ȡ�ṹ
BOOL	AReleaseRQ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//��ȡ���½ṹ
BOOL	AReleaseRQ	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	return (TRUE);
	}
//�õ��ṹ��С
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
//���캯��
AReleaseRP	::	AReleaseRP()
	{
	ItemType = 0x06;
	Reserved1 = 0;
	Reserved2 = 0;
	}
//��������
AReleaseRP	::	~AReleaseRP()
	{
	// nothing, everything should self-destruct nicely
	}

//���л�
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
//��ȡ�ṹ
BOOL	AReleaseRP	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//��ȡ���½ṹ
BOOL	AReleaseRP	::	ReadDynamic(Buffer	&Link)
	{
	Link.Read((BYTE *) &Reserved1, sizeof(BYTE));
	Link >> Length;	//Link.Read((BYTE *) &Length, sizeof(UINT32));
	Link >>	Reserved2;			//Link.Read((BYTE *) &Reserved2, sizeof(UINT16));
	return (TRUE);
	}
//����ṹ��С
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
//���캯��
AAbortRQ	::	AAbortRQ()
	{
	ItemType = 0x07;
	Reserved1 = 0;
	Reserved2 = 0;
	Reserved3 = 0;
	Source = 3;
	Reason = 1;
	}
//���캯��,������ֹԭ��
AAbortRQ	::	AAbortRQ(BYTE S, BYTE Re)
	{
	ItemType = 0x03;
	Reserved1 = 0;
	Reserved2 = 0;
	Reserved3 = 0;
	Source = S;
	Reason = Re;
	}
//��������
AAbortRQ	::	~AAbortRQ()
	{
	// nothing, everything should self-destruct nicely
	}

//���л�
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
//��ȡ�ṹ
BOOL	AAbortRQ	::	Read(Buffer	&Link)
	{
	Link.Read((BYTE *) &ItemType, sizeof(BYTE));
	return(this->ReadDynamic(Link));
	}
//��ȡ������Ϣ
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
//�õ��ṹ��С
UINT32	AAbortRQ	::	Size()
	{
	Length = 4;
	return ( Length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
	}

