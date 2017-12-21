

#	ifdef	DEBUG_VERSION
void	*operator new(size_t);
void	operator delete(void *);
BOOL	InitMemoryWatch();
BOOL	CloseMemoryWatch();
#else
#	define	InitMemoryWatch()
#	define	CloseMemoryWatch()
#endif

class	SafeMemory
	{
	public:
		SafeMemory ()	{ InitMemoryWatch(); };
		~SafeMemory ()	{ CloseMemoryWatch(); };
	};


