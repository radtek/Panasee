

// The library used to link with the DICOM Lib has a naming conflict with
// socket.  Therefor we simply remap our socket class into MacSocket.
 
#ifdef	MAC
#	define	Socket	MacSocket
#endif

class	Socket
	{
	int					Error;
	unsigned	long	tulong,*tulongptr;
	public:
		int					Socketfd;
		int					ListenSocketfd;
		BOOL				UDP;
		BOOL				Connected;
		BOOL				Listened;
		struct	sockaddr_in	sa;
	private:
	struct	hostent		hes;
	struct	servent		servs;

		int					TimeOut;					
	public:
		virtual	BOOL	SendBinary(BYTE *, UINT);
		virtual	INT		ReadBinary(BYTE *, UINT);
		struct	hostent	*Gethostbyname(char	*);
		struct	servent	*Getservbyname(char	*, char *);
		int		GetLinkError();
				Socket();
		virtual	~Socket();		
		virtual	BOOL	Open ( char *ip, char *port );
		BOOL	Close ();
		int		GetLastError()	{ return ( Error ); };
		BOOL	Listen (char *port);
		BOOL	Accept ();
		BOOL	SetTimeOut(int);
		
		// UDP Extensions
		BOOL	BindUDPServer(char *port);
		BOOL	BindUDPClient(char *host, char *port);

	};

