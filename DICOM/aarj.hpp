//�������ھܾ�����
//ItemType�̶���0x03
//Reason�Ǿܾ�ԭ��,source�Ǿܾ�����,result���ǽ��
//��д��size��������dicom����ͨ�õĺ���
//���ǽ��ṹ���л��ʹ����л��������л�ýṹ��Ϣ.
class	AAssociateRJ
	{
	private:
		BYTE		ItemType;		// 0x03
		BYTE		Reserved1;
		UINT32		Length;
		BYTE		Reserved2;
	public:
		BYTE		Result;
		BYTE		Source;
		BYTE		Reason;

					AAssociateRJ();
					AAssociateRJ(BYTE, BYTE, BYTE);
		virtual		~AAssociateRJ();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			


class	AReleaseRQ
	{
	private:
		BYTE		ItemType;		// 0x04
		BYTE		Reserved1;
		UINT32		Length;
		UINT32		Reserved2;
	public:
					AReleaseRQ();
		virtual		~AReleaseRQ();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			

class	AReleaseRP
	{
	private:
		BYTE		ItemType;		// 0x06
		BYTE		Reserved1;
		UINT32		Length;
		UINT32		Reserved2;
	public:
					AReleaseRP();
		virtual		~AReleaseRP();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			

class	AAbortRQ
	{
	private:
		BYTE		ItemType;		// 0x07
		BYTE		Reserved1;
		UINT32		Length;
		BYTE		Reserved2;
		BYTE		Reserved3;
	public:
		BYTE		Source;
		BYTE		Reason;

					AAbortRQ();
					AAbortRQ(BYTE, BYTE);
		virtual		~AAbortRQ();
		BOOL		Write(Buffer &);
		BOOL		Read(Buffer &);
		BOOL		ReadDynamic(Buffer	&);
		UINT32		Size();
	};			

