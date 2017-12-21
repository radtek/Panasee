


class	LinkedBuffer	:
	public	Buffer
	{
		Buffer	*LinkedTo;
	public:
			LinkedBuffer()	{ LinkedTo = NULL; };
			~LinkedBuffer()	{ LinkedTo = NULL; };
		BOOL	Fill(Buffer	&Link, UINT	Count);
		BOOL	Flush(Buffer &Link, UINT	Count);
		UINT	GetOutgoingSize();
		UINT	GetIncomingSize();
		virtual	INT		ReadBinary(BYTE	*Data, UINT	Count);
		virtual	BOOL	SendBinary(BYTE *Data, UINT Count);
	};

class	MemoryBuffer	:
	public	LinkedBuffer
	{
	private:
		BYTE		*Data;
		UINT		Length;
		UINT		Index;
		BOOL		DestructFlag;
	public:
		INT		ReadBinary(BYTE	*Data, UINT	Count);
		BOOL	SendBinary(BYTE *Data, UINT Count);
				MemoryBuffer ( BYTE *Data, UINT Length, BOOL Destruct,
								UINT Endian);
				~MemoryBuffer ();
	};


class	PDV
	{
	public:
		UINT32	Length;
		BYTE	PresentationContextID;
		BYTE	MsgHeader;
	};

class	PDataTF
	{
	private:
		BYTE			ItemType;	// 0x04
		BYTE			Reserved1;
	public:
		UINT32			Length;
	public:
		LinkedBuffer	VRBuffer;

	public:
		INT		MsgStatus;
		UINT	Endian;
		PDV		pdv;
		BYTE	PresentationContextID;
		BYTE	MsgHeader;

		BOOL	ReadDynamic(Buffer	&Link);
		BOOL	Write(Buffer	&Link);

				PDataTF();
		virtual		~PDataTF();
	};
