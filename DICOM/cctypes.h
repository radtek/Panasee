//本文件定义一些基本数据类型
//

typedef	signed		int		INT;
typedef	signed		char	INT8;
typedef	signed		short	INT16;

typedef unsigned	int		UINT;
typedef unsigned	char	UINT8;
typedef unsigned	short	UINT16;



#ifndef	TRUE
#define	TRUE	((UINT) 1)
#endif
#ifndef	FALSE
#define	FALSE	((UINT) 0)
#endif


#ifdef	LITTLE_ENDIAN
#undef	LITTLE_ENDIAN
#endif
#ifdef	BIG_ENDIAN
#undef	BIG_ENDIAN
#endif

#	define	LITTLE_ENDIAN	1
#	define	BIG_ENDIAN		2
#ifndef	NATIVE_ENDIAN
#	define	NATIVE_ENDIAN	LITTLE_ENDIAN
#endif
        
