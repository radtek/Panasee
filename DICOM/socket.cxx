
/*******************************************************************************
 *
 * socket.C
 *
 *******************************************************************************/

#	include	"dicom.hpp"
#	include	<stdlib.h>


#ifdef	WINDOWS
#define SD_BOTH         0x02		/* From winsock2.h */
static	WORD	WinSockUsed	=	0;
static	WSADATA	wsaData;



Socket	::	Socket ()
	{
	WORD		VersionNeeded;
//	FARPROC		FarProc;
#ifdef DEBUG
	TimeOut=0;
#else
	TimeOut = 60 * 5;	// 5 minutes	
#endif
	UDP = 0;
	if(! WinSockUsed )
		{
		/* WinSock.dll not WSAStartup(), so call it and init the library */
		VersionNeeded = 0x0101;
		
		Error = WSAStartup( VersionNeeded, &wsaData);
		if( Error )
			{
			/* no socket library */
			return;
			}
		/*FarProc = (FARPROC) SocketBlockingHook;
		if(!WSASetBlockingHook(FarProc))
			{
			OutputDebugString("Failed to Install Blocking Hook\n");
			}*/
		}
	++WinSockUsed;
	Socketfd = 0;
	ListenSocketfd = 0;
	Connected = 0;
	Listened = 0;
	}

Socket	::	~Socket ()
	{

	if( ! WinSockUsed )
		return;	// must have failed installation
	
	if ( Socketfd )
		{
		shutdown(Socketfd, SD_BOTH); 
		closesocket(Socketfd);
		}
	if ( ListenSocketfd )
		{
		shutdown(ListenSocketfd, SD_BOTH); 
		closesocket(ListenSocketfd);
		}
	
	--WinSockUsed;
	if(! WinSockUsed )
		{
		//WSAUnhookBlockingHook();
		WSACleanup();
		}
	
	}
#else

#	define	closesocket(xxx)	close(xxx)

/*
int	closesocket(int	sock)
	{
	int	flags;
	struct	linger	sl;

	sl.l_onoff=1;
	sl.l_linger=30;
	errno=0;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, &sl, sizeof(linger));
	fprintf(stderr, "%d\n", errno);

	errno = 0;
	fprintf(stderr, "CloseSocket(%d) : %d ", sock, close(sock));
	fprintf(stderr, "%d\n", errno);
	return(0);
	}
*/

Socket	::	Socket()
	{
	Socketfd = 0;
	Connected = 0;
	Listened = 0;
	ListenSocketfd = 0;
#ifdef DEBUG
	TimeOut=0;
#else
	TimeOut = 60 * 5;	// 5 minutes	
#endif
	UDP = 0;
	}

Socket	::	~Socket()
	{
	if ( Socketfd )
		{
		closesocket(Socketfd);
		}
	if ( ListenSocketfd )
		{
		closesocket(ListenSocketfd);
		}
	}

#endif
	

struct	hostent	*	Socket	::	Gethostbyname(char	*name1)
	{
	struct	hostent		*he;
	unsigned	long	ip;
//	unsigned	long	i1,i2,i3,i4;
//	char		*dot2, *dot3, *dot4;
	char		*name;
	char		ndat[128];
	
	strcpy(ndat, name1);
	name = &ndat[0];
	
	if(!atoi(name1))
		he = gethostbyname(name);
	else
		he = NULL;
	if(he)
		{
		memcpy ((void *) &hes, (void *) he, sizeof(struct hostent));
		return(&hes);
		}
#ifdef	MAC
	struct	in_addr 	ipa	= inet_addr(name);
	ip = ipa.s_addr;
#else
	ip = inet_addr(name);
#endif
/******************************
	dot2 = strchr(name, '.');
	if(!dot2)
		return ( NULL );
	(*dot2) = '\0';
	++dot2;
	dot3 = strchr(dot2, '.');
	if(!dot3)
		return ( NULL );
	(*dot3) = '\0';
	++dot3;
	dot4 = strchr(dot3, '.');
	if(!dot4)
		return ( NULL );
	(*dot4) = '\0';
	++dot4;
	i1 = atoi(name);
	i2 = atoi(dot2);
	i3 = atoi(dot3);
	i4 = atoi(dot4);
	
#if	NATIVE_ENDIAN == BIG_ENDIAN
	i1 = i4 << 24;
	i2 = i3 << 16;
	i3 = i2 << 8;
#else	
	i4 = i4 << 24;
	i3 = i3 << 16;
	i2 = i2 << 8;
#endif
	
	ip = i1 | i2 | i3 | i4;
********************************/
	hes.h_addr_list = (char **) &tulongptr;
	tulongptr = &tulong;
	tulong = ip;
	return ( &hes );	
	}
	
struct	servent	*	Socket	::	Getservbyname(char	*name, char	*prot)
	{
	unsigned	short	s;
//	unsigned	short	s1;
	struct	servent		*se;
	
	s = atoi(name);
	if(!s)
		{
		se = getservbyname(name, prot);
		if(!se)
			return(se);
		memcpy ((void *) &servs, (void *) se, sizeof(struct servent));
		return(&servs);
		}
#ifdef	MAC
	servs.s_port = s;
#else
	servs.s_port = htons(s);
#endif
/****************************
	s1 = s & 0x00ff;
	s1 = s1 << 8;
	s = s >> 8;
	s1 = s1 | s;
	servs.s_port = s1;
*******************************/	
//	fprintf(stderr, "returning: %d\n", servs.s_port);
	return ( &servs );
	}

BOOL	Socket	::	Listen ( char 	*port )
	{
	UINT32				tuint32;
//	struct	hostent		*he;
//	struct	hostent		hecopy;
	struct	servent		*se;
	struct	servent		secopy;
	struct	linger		Linger;

	Connected = 0;
	Listened = 0;

	if ( Socketfd )
		closesocket(Socketfd);

//	fprintf(stderr, "[Listen: %s]\n", port); fflush(stderr);
	/*he = Gethostbyname("hampson");
	
	if ( !he )
		{
		return ( FALSE );	// could not resolve host name
		}
	*/   
	//memcpy ((void *) &hecopy, (void *) he, sizeof(struct hostent));
	
	se = Getservbyname(port, NULL);
	
	if ( !se )
		return ( FALSE );	// could not resolv port
	memcpy ((void *) &secopy, (void *) se, sizeof(struct servent));

	Socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(!Socketfd)
		return ( FALSE );	// could not create socket
	Linger.l_onoff  = 1;
	Linger.l_linger = 0;
	setsockopt(Socketfd, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(struct linger));
	//memcpy((void *) &sa.sin_addr.S_un.S_addr , (void *) hecopy.h_addr_list[0], 4);	// adddress
	// should bind to the default address
	tuint32 = 0;
#ifdef	SYSV_SOCKET
	sa.sin_addr.s_addr = tuint32;
#else
	memcpy((void *) &sa.sin_addr.S_un.S_addr, (void *) &tuint32, 4);
#endif
	memcpy((void *) &sa.sin_port , (void *) &secopy.s_port, 2);			// port
	sa.sin_port = secopy.s_port;
	sa.sin_family = AF_INET;

	if(bind(Socketfd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)))
		{
//		fprintf(stderr, "Erro (bind) : %d\n", GetLinkError());
#ifdef	FORCEDIO
/*		while(errno==125)
			{
			errno=0;
			bind(Socketfd, (struct sockaddr *) &sa, sizeof(struct
				sockaddr_in));
			}*/
#endif
		if(GetLinkError())
			{
			closesocket(Socketfd);
			Error = GetLinkError();
			Socketfd = 0;
			return ( FALSE );
			}
		}
	if(listen(Socketfd, 5))
		{
		//fprintf(stderr, "Erro (bind): %d\n", GetLinkError());fflush(stderr);
		closesocket(Socketfd);
		Socketfd = 0;
		return ( FALSE );
		}

	Listened = 1;
	ListenSocketfd = Socketfd;
	Socketfd = 0;
	Connected = 0;
	return ( TRUE );
	}

BOOL	Socket	::	Accept()
	{
	int		Size;

	//fprintf(stderr, "Accept()\n");fflush(stderr);
	if(!Listened)
		return ( FALSE );
	if(!ListenSocketfd)
		return ( FALSE );

	Size = sizeof(struct sockaddr_in);
	Socketfd = accept(ListenSocketfd, (struct sockaddr *) &sa, &Size);
	if(Socketfd>0)
		{
		Connected = TRUE;
		return ( TRUE );
		}
	//fprintf(stderr, "Error (accept) : %d\n", errno);
	closesocket(ListenSocketfd);
	Listened = 0;
	ListenSocketfd = 0;
	Socketfd = 0;
	Connected = FALSE;
	return ( FALSE );
	}
	

BOOL	Socket	::	Open ( char	*ip, char	*port)
	{
	struct	hostent		*he;
	struct	hostent		hecopy;
	struct	servent		*se;
	struct	servent		secopy;
	struct	linger		Linger;

	if ( Socketfd )
		closesocket(Socketfd);

	Connected = 0;

	he = Gethostbyname(ip);
	
	if ( !he )
		{
	//	fprintf(stderr, "Could not resolve host\n");
		return ( FALSE );	// could not resolve host name
		}
	   
	memcpy ((void *) &hecopy, (void *) he, sizeof(struct hostent));
	
	se = Getservbyname(port, NULL);
	
	if ( !se )
		{
//		fprintf(stderr, "Could not resolve port\n");
		return ( FALSE );	// could not resolv port
		}
	
	memcpy ((void *) &secopy, (void *) se, sizeof(struct servent));
	if(!hecopy.h_addr_list)
		return ( FALSE );
	
	Socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(Socketfd<0)
		return ( FALSE );	// could not create socket
	Linger.l_onoff  = 1;
	Linger.l_linger = 0;
	setsockopt(Socketfd, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(struct linger));
		
#ifdef	SYSV_SOCKET
	memcpy((void *) &sa.sin_addr.s_addr , (void *) hecopy.h_addr_list[0], 4);	// adddress
#else
	memcpy((void *) &sa.sin_addr.S_un.S_addr , (void *) hecopy.h_addr_list[0], 4);	// adddress
#endif
	memcpy((void *) &sa.sin_port , (void *) &secopy.s_port, 2);			// port
	sa.sin_family = AF_INET;
	sa.sin_port = secopy.s_port;

	Error = connect(Socketfd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in));
	if( ! Error )
		{
		Connected = TRUE;
		return ( TRUE );
		}

	closesocket(Socketfd);
	Socketfd = 0;
	return ( FALSE );
	}

BOOL	Socket	::	Close()
	{


	if(ListenSocketfd)
		{
		closesocket(ListenSocketfd);
		}
	if(Socketfd)
		{
		closesocket(Socketfd);
		}
	ListenSocketfd = 0;
	Socketfd = 0;
	Connected = 0;
	Listened = 0;
	UDP = 0;
	return ( TRUE );
	}
BOOL		Socket	::	SendBinary(BYTE *s, UINT	count)
	{
	if ( !Connected)
		return ( 0 );
//////////////////////////////////////////////////////////
//	Add by Lin
//	FILE *stream;
//	if( ( stream  = fopen( "c:\\dataout.tst", "ab" )) == NULL )
//		printf( "The file 'data' was not opened\n" );
//	else
//		printf( "The file 'data' was opened\n" );
//	fwrite("Send:", sizeof( BYTE ), 5, stream );
//	size_t  numwritten = fwrite( s, sizeof( BYTE ), count, stream );
//	fclose(stream);
////////////////////////////////////////////////////////////		
	if(send ( Socketfd, (char *) s, count, 0)<0)
		{
		fprintf(stderr, "SOCKET END ERROR: %d\n", GetLastError());
		}
	return ( TRUE );
	}
	
INT	Socket	::	ReadBinary(BYTE *s, UINT	count)
	{
	struct	fd_set	fds;
	struct	timeval	tv	= {TimeOut, 0};	// poll

	if ( ! Connected )
		return ( -1 );
#ifdef	MAC
	memset((void*)&fds, 0, sizeof(struct fd_set));
#else
	FD_ZERO(&fds);
#endif
	FD_SET(Socketfd, &fds);
	if(select(Socketfd + 1, &fds, NULL, NULL, &tv)==1)
		{
		memset(s,0,count);
		if((count = recv(Socketfd, (char *) s, count, 0))==0)
			{
//If the connection has been gracefully closed, the return value is zero. 
//so,please return -1 to indicate peer close the socket
/*#ifdef	WINDOWS
			if(WSAGetLastError()==WSAEDISCON)
				return(-1);
			else
				return(0);
#else
			return ( -1 );
#endif
*/
			Close ();			
			return ( -1 );
			}
//////////////////////////////////////////////////////////
//	Add by Lin
//	FILE *stream;
//	if( ( stream  = fopen( "c:\\dataout.tst", "ab" )) == NULL )
//		printf( "The file 'data' was not opened\n" );
//	else
//		printf( "The file 'data' was opened\n" );
//	fwrite("Send:", sizeof( BYTE ), 5, stream );
//	size_t  numwritten = fwrite( s, sizeof( BYTE ), count, stream );
//	fclose(stream);
////////////////////////////////////////////////////////////	
		return ( count );
		}
	if ( TimeOut )
		return ( -1 );
	return ( 0 );
	}
	
int		Socket	::	GetLinkError()
	{
#ifdef	WINDOWS
	return ( WSAGetLastError() );
#else
	return ( errno );
#endif
	}

BOOL	Socket	::	SetTimeOut(int	TimeOut)
	{
	this->TimeOut = TimeOut;
	return ( TRUE );
	}	

BOOL
Socket	::	BindUDPServer (
	char	*port)
	{
	Close();
	struct	servent		*se;
	struct	servent		secopy;
	se = Getservbyname(port, NULL);
	
	if ( !se )
		{
		return ( FALSE );	// could not resolv port
		}
	
	memcpy ((void *) &secopy, (void *) se, sizeof(struct servent));
	Socketfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(Socketfd<0)
		return ( FALSE );	// could not create socket
		
#ifdef	SYSV_SOCKET
	memset((void *) &sa.sin_addr.s_addr , 0, 4);
#else
	memset((void *) &sa.sin_addr.S_un.S_addr , 0, 4);
#endif
	memcpy((void *) &sa.sin_port , (void *) &secopy.s_port, 2);			// port
	sa.sin_family = AF_INET;
	sa.sin_port = secopy.s_port;
	
	if(bind(Socketfd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)))
		{
		closesocket(Socketfd);
		Error = GetLinkError();
		Socketfd = 0;
		return ( FALSE );
		}
	Connected = 1;
	UDP = 1;
	return ( TRUE );
	}

BOOL	
Socket	::	 BindUDPClient (
	char	*ip,
	char	*port)
	{
	struct	hostent		*he;
	struct	hostent		hecopy;
	struct	servent		*se;
	struct	servent		secopy;
	

	if ( Socketfd )
		closesocket(Socketfd);
	if ( ListenSocketfd )
		closesocket(ListenSocketfd);
	
	Listened = 0;
	Connected = 0;
	Socketfd = 0;
	ListenSocketfd = 0;
	
	he = Gethostbyname(ip);
	
	if ( !he )
		{
		fprintf(stderr, "Could not resolve host\n");
		return ( FALSE );	// could not resolve host name
		}
	   
	memcpy ((void *) &hecopy, (void *) he, sizeof(struct hostent));
	
	se = Getservbyname(port, NULL);
	
	if ( !se )
		{
		fprintf(stderr, "Could not resolve port\n");
		return ( FALSE );	// could not resolv port
		}
	
	memcpy ((void *) &secopy, (void *) se, sizeof(struct servent));
	if(!hecopy.h_addr_list)
		return ( FALSE );
	
	Socketfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(Socketfd<0)
		return ( FALSE );	// could not create socket
		
#ifdef	SYSV_SOCKET
	memcpy((void *) &sa.sin_addr.s_addr , (void *) hecopy.h_addr_list[0], 4);	// adddress
#else
	memcpy((void *) &sa.sin_addr.S_un.S_addr , (void *) hecopy.h_addr_list[0], 4);	// adddress
#endif
	memcpy((void *) &sa.sin_port , (void *) &secopy.s_port, 2);			// port
	sa.sin_family = AF_INET;
	sa.sin_port = secopy.s_port;
	Error = connect(Socketfd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in));
	if( ! Error )
		{
		Connected = TRUE;
		UDP = 1;
		return ( TRUE );
		}
	closesocket(Socketfd);
	Socketfd = 0;
	return ( FALSE );
	}
	
/*#endif*/

