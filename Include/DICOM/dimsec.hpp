//定义C命令
//包括C-ECHO
//C-STORE
//C-FIND
//C-MOVE
//的所有请求,回应的实现
//具体用的时候可以字节使用read,write函数
class	CEchoRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject * );
		BOOL	Write ( PDU_Service * );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CEchoRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject * );
		BOOL	Write ( PDU_Service *, DICOMCommandObject *, UINT16 );
		BOOL	Write ( PDU_Service *, DICOMCommandObject * );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CStoreRQ
	{
	public:
		BOOL	Read (	DICOMCommandObject *,
						PDU_Service *,
						DICOMDataObject * );
		BOOL	Read (	DICOMCommandObject	* );
		BOOL	Write ( PDU_Service * , DICOMDataObject	*);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CStoreRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject * );
		BOOL	Write ( PDU_Service * , DICOMCommandObject *, UINT16 );
		BOOL	Write ( PDU_Service *, DICOMCommandObject * );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CFindRQ
	{
	public:
		BOOL	Read (	DICOMCommandObject *,
						PDU_Service *,
						DICOMDataObject * );
		BOOL	Read (	DICOMCommandObject	* );
		BOOL	Write ( PDU_Service * , DICOMDataObject	*);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CFindRSP
	{
	public:
		BOOL	Read (	DICOMCommandObject *,
						PDU_Service	*,
						DICOMDataObject	* );
		BOOL	Read (	DICOMCommandObject	* );
		BOOL	Write ( PDU_Service *,
						DICOMCommandObject *,
						UINT16,
						DICOMDataObject	*);
		BOOL	Write ( PDU_Service *,
						DICOMCommandObject *,
						DICOMDataObject * );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CMoveRQ
	{
	public:
		BOOL	Read (	DICOMCommandObject *,
						PDU_Service *,
						DICOMDataObject * );
		BOOL	Read (	DICOMCommandObject	* );
		BOOL	Write ( PDU_Service * , DICOMDataObject	*, BYTE *);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	CMoveRSP
	{
	public:
		BOOL	Read (	DICOMCommandObject *,
						PDU_Service	*,
						DICOMDataObject	* );
		BOOL	Read (	DICOMCommandObject * );
		BOOL	Write ( PDU_Service *,
						DICOMCommandObject *,
						UINT16,
						UINT16,
						UINT16,
						UINT16,
						UINT16,
						DICOMDataObject *);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

