
#	include	"dicom.hpp"
//转换编码UINT16位
UINT16	SwitchEndian(UINT16	x)
	{
	return ( ((x >> 8) & 0x00ff) | ((x << 8) & 0xff00) );
	}
//转换编码UINT3232位

UINT32	SwitchEndian(UINT32	x)
	{
	return ( ((UINT32) SwitchEndian ((UINT16) (x >> 16))) |
		((UINT32) (SwitchEndian ((UINT16) (x & 0x0000ffff)) << 16) ));
	}
//转换编码INT16位

INT16	SwitchEndian(INT16	x)
	{
	return ((INT16) SwitchEndian((UINT16) x) );
	}
//转换编码INT32位

INT32	SwitchEndian(INT32	x)
	{
	return ((INT32) SwitchEndian((UINT32) x) );
	}


#	if	NATIVE_ENDIAN	==	LITTLE_ENDIAN

#	define	LE_UINT16	UINT16
#	define	LE_UINT32	UINT32
#	define	LE_INT16	INT16
#	define	LE_INT32	INT32

#	define	XE_UINTX	BE_UINT16
#	define	UINTX		UINT16
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_UINT32
#	define	UINTX		UINT32
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT16
#	define	UINTX		INT16
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT32
#	define	UINTX		INT32
#	include	"endian.cpd"

#undef	XE_UINTX
#undef	UINTX

#else
#	define	BE_UINT16	UINT16
#	define	BE_UINT32	UINT32
#	define	BE_INT16	INT16
#	define	BE_INT32	INT32

#	define	XE_UINTX	LE_UINT16
#	define	UINTX		UINT16
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_UINT32
#	define	UINTX		UINT32
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT16
#	define	UINTX		INT16
#	include	"endian.cpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT32
#	define	UINTX		INT32
#	include	"endian.cpd"

#undef	XE_UINTX
#undef	UINTX

#endif




