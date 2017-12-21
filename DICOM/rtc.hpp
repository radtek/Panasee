

// Run-Time-Classing of VR's

BOOL	Pack(BYTE, BYTE, UINT16 &);
BOOL	Pack(UINT16, BYTE &, BYTE &);


int MkBinaryRtc(char* pszTextIn, char* pszBinaryOut, BOOL bCarryDescriptions);


#	define	MAKEUINT32(xxx, yyy)	((UINT32) (((UINT32) xxx << 16) | yyy))
typedef	struct	_RTCElement
	{
	UINT16	Group;
	UINT16	Element;
	UINT16	TypeCode;
	char	*Description;
	}	RTCElement;
	
	
class	RTC
	{
	public:
		FixedArray < UINT32, RTCElement >		*TypeCodes;
		BOOL		CarryDescriptions;
	public:
					RTC (BOOL	CD = TRUE);
					~RTC ();
		BOOL		AttachMemRTC();
		BOOL		DetachRTC ();
		BOOL		RunTimeClass ( DICOMObject * );
		BOOL		RunTimeClass ( VR * );
		UINT16		RunTimeClass ( UINT16, UINT16, char * =NULL);
		BOOL		GetGroupElement(RTCElement* pEntry);
	};



