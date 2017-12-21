

// Architectually Neutral Endian types.
//
// Note these are not done with C++ templates.  This is a design choice.
UINT16	SwitchEndian(UINT16	x);
UINT32	SwitchEndian(UINT32	x);
INT16	SwitchEndian(INT16	x);
INT32	SwitchEndian(INT32	x);


#	if	NATIVE_ENDIAN	==	LITTLE_ENDIAN

#	define	LE_UINT16	UINT16
#	define	LE_UINT32	UINT32
#	define	LE_INT16	INT16
#	define	LE_INT32	INT32

#	define	XE_UINTX	BE_UINT16
#	define	UINTX		UINT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_UINT32
#	define	UINTX		UINT32
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT16
#	define	UINTX		INT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT32
#	define	UINTX		INT32
#	include	"endian.hpd"

#undef	XE_UINTX
#undef	UINTX

#else
#	define	BE_UINT16	UINT16
#	define	BE_UINT32	UINT32
#	define	BE_INT16	INT16
#	define	BE_INT32	INT32

#	define	XE_UINTX	LE_UINT16
#	define	UINTX		UINT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_UINT32
#	define	UINTX		UINT32
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT16
#	define	UINTX		INT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT32
#	define	UINTX		INT32
#	include	"endian.hpd"

#undef	XE_UINTX
#undef	UINTX

#endif




