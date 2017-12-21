

// Verification SOP Service Class
BOOL	SetUID	(	UID	&,	VR	* );

class	Verification	:
	public	CEchoRQ,
	public	CEchoRSP
	{
	public:
		BOOL	GetUID ( UID & );
	public:
		BOOL	Read ( PDU_Service *, DICOMCommandObject * );
		BOOL	Write ( PDU_Service * );
	};

