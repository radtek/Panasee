//此类用于拒绝连接
//ItemType固定是0x03
//Reason是拒绝原因,source是拒绝对象,result就是结果
//读写和size基本都是dicom里面通用的函数
//就是将结构序列化和从序列化的数据中获得结构信息.
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

