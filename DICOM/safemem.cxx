
#	include	"dicom.hpp"

#ifdef	DEBUG_VERSION

SafeMemory	SF;	// For the init / de-init
#	include	<stddef.h>
#	include	<stdlib.h>

void	*MemorySegments [ MAX_SEGMENTS ];
UINT	MemorySizes [ MAX_SEGMENTS ];

BOOL	InitMemoryWatch()
	{
	UINT	Index;

	Index = 0;
	while ( Index < MAX_SEGMENTS )
		{
		MemorySegments [ Index ] = NULL;
		MemorySizes [ Index ] = 0;
		++Index;
		}
	return ( TRUE );
	}

BOOL
PrintUnfreed(void	*data, UINT	Size)
	{
	UINT32	*p32;
	UINT16	*p16;
	UINT	Index;
	char	*pc;

	switch ( Size )
		{
		case	2:	p16 = (UINT16*) data;
					fprintf(stderr, "%4.4 / %d ",*p16,*p16);
					break;
		case	4:	p32 = (UINT32*) data;
					fprintf(stderr, "%8.8x / %d ",*p32,*p32);
					break;
		}
	Index = 0;
	pc = (char*)data;
	while ( Index < Size )
		{
		if ( Index > 50 )
			break;
		fprintf(stderr, "%2.2x(%c),", *pc, *pc);
		++pc;
		++Index;
		}
	fprintf(stderr, "\n");
	return ( TRUE );
	}

UINT	CloseMemoryWatch()
	{
	UINT	Index;
	UINT	Count;

	Index = 0; Count = 0;

	while ( Index < MAX_SEGMENTS )
		{
		if ( MemorySegments [ Index ] )
			{
			fprintf (stderr, "Memory : %8.8x Size : %8.8x : %d Not Freed\n",
				MemorySegments [ Index ], MemorySizes [ Index ],
				MemorySizes [ Index ]);
			fflush ( stderr );
			PrintUnfreed(MemorySegments [ Index ], MemorySizes [ Index ]);
			free( MemorySegments [ Index ] );
			++Count;
			}
		++Index;
		}
	
	return ( Count );	
	}

BOOL
RegisterMemory (size_t	Size, void	*vptr)
	{
	UINT	Index;

	Index = 0;
	while ( Index < MAX_SEGMENTS )
		{
		if ( ! MemorySegments [ Index ] )
			{
			MemorySegments [ Index ] = vptr;
			MemorySizes [ Index ] = Size;
			return ( TRUE );
			}
		++Index;
		}
	fprintf(stderr, "Error: Increase MAX_SEGMENTS and recompile\n");
	return ( FALSE );
	}

BOOL
UnRegisterMemory ( void *vptr )
	{
	UINT	Index;

	Index = 0;
	while ( Index < MAX_SEGMENTS )
		{
		if ( MemorySegments [ Index ] == vptr )
			{
			MemorySegments [ Index ] = NULL;
			MemorySegments [ Index ] = 0;
			return ( TRUE );
			}
		++Index;
		}
	return ( FALSE );
	}

void	* operator	new(size_t	x)
	{
	void	*vptr;
	
	if ( ! x )
		return ( NULL );

	vptr = malloc(x);
	RegisterMemory (x, vptr);
	return ( vptr );
	}

void	 operator	delete	( void	*vptr )
	{
	if ( ! vptr )
		return;
	if (!UnRegisterMemory(vptr))
		{
		fprintf(stdout, "possible duplicate delete of: %8.8x\n", vptr);
		char	*s = ((char*)vptr);
		int		Index = 0;
		while ( Index < 10 )
			{
			fprintf(stdout, "[%c,%d,%x], ", (*s),(*s),(*s));
			++Index;++s;
			}
		fprintf(stdout, "\n");
		fflush(stdout);
		}
	free(vptr);
	}

#endif

