

// Compiler / Architecture Dependent.
typedef	float	IEEE_FL;
typedef	double	IEEE_FD;

// Corresponding "C" Datatypes.  Note There are many instances where
// One should *not* declare and use these data types (particularly
// with ones that are more expensive in memory costs).  However, they
// are complete; simply for the sake of completeness.

typedef	char	DCM_C_AE [ 17 ];
typedef	char	DCM_C_AS [ 5 ];
typedef	UINT16	DCM_C_AT [ 2 ];
typedef	char	DCM_C_DA [ 9 ];
typedef	char	DCM_C_DS [ 17 ];
typedef	char	DCM_C_DT [ 27 ];
typedef	IEEE_FL	DCM_C_FL;
typedef	IEEE_FD	DCM_C_FD;
typedef	char	DCM_C_IS [ 13 ];
typedef	char	DCM_C_LO [ 65 ];
typedef	char	DCM_C_LT [ 10241 ];
typedef	BYTE	DCM_C_OB;
typedef	UINT16	DCM_C_OW;
typedef	char	DCM_C_PN [ 65 ];
typedef	char	DCM_C_SH [ 17 ];
typedef	UINT32	DCM_C_SL;
typedef	UINT16	DCM_C_SS;
typedef	char	DCM_C_ST [ 1025 ];
typedef	char	DCM_C_TM [ 17 ];
typedef	char	DCM_C_UI [ 65 ];
typedef	UINT32	DCM_C_UL;
typedef	UINT16	DCM_C_US;

typedef	char	*DCM_C_AE_PTR;
typedef	char	*DCM_C_AS_PTR;
typedef	UINT16	*DCM_C_AT_PTR;
typedef	char	*DCM_C_DA_PTR;
typedef	char	*DCM_C_DS_PTR;
typedef	char	*DCM_C_DT_PTR;
typedef	IEEE_FL	*DCM_C_FL_PTR;
typedef	IEEE_FD	*DCM_C_FD_PTR;
typedef	char	*DCM_C_IS_PTR;
typedef	char	*DCM_C_LO_PTR;
typedef	char	*DCM_C_LT_PTR;
typedef	BYTE	*DCM_C_OB_PTR;
typedef	UINT16	*DCM_C_OW_PTR;
typedef	char	*DCM_C_PN_PTR;
typedef	char	*DCM_C_SH_PTR;
typedef	UINT32	*DCM_C_SL_PTR;
typedef	UINT16	*DCM_C_SS_PTR;
typedef	char	*DCM_C_ST_PTR;
typedef	char	*DCM_C_TM_PTR;
typedef	char	*DCM_C_UI_PTR;
typedef	UINT32	*DCM_C_UL_PTR;
typedef	UINT16	*DCM_C_US_PTR;


/*
#ifndef	MAKEUINT16
#	define	MAKEUINT16(xxx, yyy)	\
	((((UINT16)xxx)<<8)+((UINT16)yyy))
#endif
*/
// VR_XXTYPED Provides the base-class VDT for defined VR type

class	VR_XXTYPED	:
	public	VR
	{
	public:
		VR_XXTYPED ();
		VR_XXTYPED (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XXTYPED (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XXTYPED (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		// GetSetSize is used when a VR is about to set with the
		// a new value entry.  Some VR types are fixed in length,
		// and that needs to be reflected here
		virtual	UINT	GetSetSize () = 0;
		// PostProcSetData is used to post-process the data in the
		// VR.Data field member.  Typically one would pad things out
		// or verify it's correctness
		virtual	BOOL	PostProcSetData (UINT, void *) = 0;
		// PostProcGetData is used once data is copied out of a VR
		// and into user space.  Typical applications involve
		// stripping of leading/trailing white space, and on string
		// constants, an appending of a new-line
		virtual	BOOL	PostProcGetData (UINT, void *) = 0;
		// GetTypeCode ()
		virtual	UINT16	GetTypeCode() = 0;
	};

// XXMULTI is for Value-multiplicity elements.  It adds features such
// as VMCount(), VMGet(UINT, char *), VMSet(UINT, char *)

class	VR_XXMULTISTRING	:
	public	VR_XXTYPED
	{
	public:
		VR_XXMULTISTRING ();
		VR_XXMULTISTRING (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XXMULTISTRING (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XXMULTISTRING (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);

		UINT	VMCount ();
		virtual	BOOL	VMGet(UINT, char *);
		virtual	BOOL	VMSet(UINT, char *);
	};

class	VR_XXMULTIBINARY	:
	public	VR_XXTYPED
	{
	public:
		VR_XXMULTIBINARY ();
		VR_XXMULTIBINARY (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XXMULTIBINARY (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XXMULTIBINARY (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		
		UINT	VMCount ();
		virtual	BOOL	VMGet(UINT, void *);
		virtual	BOOL	VMSet(UINT, void *);
	};

// XXSTRING is a generic "string" type VR.  All text based VR's
// share this classes operators (makes life easier).
class	VR_XXSTRING	:
	public	VR_XXMULTISTRING
	{
	public:
		VR_XXSTRING ();
		VR_XXSTRING (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XXSTRING (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XXSTRING (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);

		virtual	BOOL	Set(char *);
		virtual	BOOL	Get(char *);
		BOOL	PostProcSetData (UINT, void *);
		BOOL	PostProcGetData (UINT, void *);
	};

class	VR_XXBINARY	:
	public	VR_XXMULTIBINARY
	{
	protected:
		BOOL	EndianSwap(UINT, void *);
	public:
		VR_XXBINARY ();
		VR_XXBINARY (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XXBINARY (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XXBINARY (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);

		virtual	BOOL	Set(void *);
		virtual	BOOL	Get(void *);
		BOOL	PostProcSetData(UINT, void *);
		BOOL	PostProcGetData(UINT, void *);
	};

class	VR_AE	:
	public	VR_XXSTRING
	{
	public:
		VR_AE ();
		VR_AE (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_AE (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_AE (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_AS	:
	public	VR_XXSTRING
	{
	public:
		VR_AS ();
		VR_AS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_AS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_AS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_AT	:
	public	VR_XXBINARY
	{
	public:
		VR_AT ();
		VR_AT (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_AT (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_AT (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		// AT Elements are always encoded big-endian, no matter the
		// transfer syntax.  Makes life interesting..
		BOOL	PostProcSetData(UINT, void *);
		BOOL	PostProcGetData(UINT, void *);
	};

class	VR_CS	:
	public	VR_XXSTRING
	{
	public:
		VR_CS ();
		VR_CS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_CS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_CS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};


// DA Elements include a "backward" compatibility feature.  Data
// in the form yyyy.mm.dd is converted into DICOM v3 format yyyymmdd.
// Certain older DICOM implementations (those based from ACR-NEMA 300)
// will have this old-style date.
//
// If you want this old style, you will either have to set it
// yourself, (maniuplating the 'Data' member), or explicitly call
// VR_XXMUTLISTRING :: VMSet().
//
class	VR_DA	:
	public	VR_XXSTRING
	{
	public:
		VR_DA ();
		VR_DA (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_DA (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_DA (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		virtual	BOOL	VMGet(UINT, char *);
		virtual	BOOL	VMSet(UINT, char *);
		//virtual	BOOL	Set(void *);
		//virtual	BOOL	Get(void *);
	};

class	VR_DS	:
	public	VR_XXSTRING
	{
	public:
		VR_DS ();
		VR_DS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_DS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_DS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_DT	:
	public	VR_XXSTRING
	{
	public:
		VR_DT ();
		VR_DT (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_DT (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_DT (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_FL	:
	public	VR_XXBINARY
	{
	public:
		VR_FL ();
		VR_FL (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_FL (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_FL (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_FD	:
	public	VR_XXBINARY
	{
	public:
		VR_FD ();
		VR_FD (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_FD (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_FD (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_IS	:
	public	VR_XXSTRING
	{
	public:
		VR_IS ();
		VR_IS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_IS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_IS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_LO	:
	public	VR_XXSTRING
	{
	public:
		VR_LO ();
		VR_LO (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_LO (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_LO (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

// LT Items have the distinction of *not* being VM > 1 possiable.
// Because of this we hack out the MULTI base class
class	VR_LT	:
	public	VR_XXTYPED
	{
	public:
		VR_LT ();
		VR_LT (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_LT (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_LT (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		BOOL	PostProcSetData(UINT, void *);
		BOOL	PostProcGetData(UINT, void *);
		BOOL	Set(char *);
		BOOL	Get(char *);
	};

class	VR_OB	:
	public	VR_XXBINARY
	{
	public:
		VR_OB ();
		VR_OB (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_OB (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_OB (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_OW	:
	public	VR_XXBINARY
	{
	public:
		VR_OW ();
		VR_OW (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_OW (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_OW (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};


// Note: Data *IS* formatted In/Out of this VR.  You *can*
// format data into this VR yourself (do so by simply adding the ^,
// if the symbol is caught in the string, then it is assumed you
// know what your doing...).  As for how the internal parser
// works, refer to DICOM v3.0 Part 5 the PN section (around pg 18
// in the 1993 August draft).  The basic idea is you have
//
// Prefix. First Names middle-initial. Lastname, Postfix
//
// That is what is "nice".  However, to allow for a great deal of
// flexiability, there are several things looked for and caught
// and hopefully parsed correctly.  Things like the following will
// be correctly parsed:
//
//	John Doe	-> Doe^John
//	John M. Doe	-> Doe^John^M.
//	Doe,John	-> Doe^John
//	Doe, John M.	-> Doe^John^M.
//
// Note on Get() routines, strings are "unparsed" and converted back
// into normal form.  Hopefully (the ideal is) you wont have to worry
// about how DICOM encodes names.  You can just write software that
// wants a patient name.. (or so the thoery goes...)
//
class	VR_PN	:
	public	VR_XXSTRING
	{
	public:
		VR_PN ();
		VR_PN (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_PN (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_PN (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		virtual	BOOL	RealToANSI(char *, char *);
		virtual	BOOL	ANSIToReal(char *, char *);
		virtual	BOOL	VMGet(UINT, char *);
		virtual	BOOL	VMSet(UINT, char *);
		//virtual	BOOL	Set(void *);
		//virtual	BOOL	Get(void *);
	};

class	VR_SH	:
	public	VR_XXSTRING
	{
	public:
		VR_SH ();
		VR_SH (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_SH (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_SH (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_SL	:
	public	VR_XXBINARY
	{
	public:
		VR_SL ();
		VR_SL (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_SL (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_SL (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_SS	:
	public	VR_XXBINARY
	{
	public:
		VR_SS ();
		VR_SS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_SS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_SS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};


// ST Items have the distinction of *not* being VM > 1 possiable.
// Because of this we hack out the MULTI base class
class	VR_ST	:
	public	VR_XXTYPED
	{
	public:
		VR_ST ();
		VR_ST (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_ST (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_ST (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		BOOL	PostProcSetData(UINT, void *);
		BOOL	PostProcGetData(UINT, void *);
		BOOL	Set(char *);
		BOOL	Get(char *);
	};

class	VR_TM	:
	public	VR_XXSTRING
	{
	public:
		VR_TM ();
		VR_TM (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_TM (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_TM (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_UI	:
	public	VR_XXSTRING
	{
	public:
		VR_UI ();
		VR_UI (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_UI (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_UI (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		BOOL	PostProcSetData(UINT, void *);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_UL	:
	public	VR_XXBINARY
	{
	public:
		VR_UL ();
		VR_UL (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_UL (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_UL (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_US	:
	public	VR_XXBINARY
	{
	public:
		VR_US ();
		VR_US (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_US (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_US (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
	};

class	VR_XS	:
	public	VR_XXBINARY
	{
		UINT16	RTCTypeCode;
	public:
		VR_XS ();
		VR_XS (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_XS (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_XS (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		BOOL	SetTypeCode ( UINT16 );	
	};

class	VR_OX	:
	public	VR_XXBINARY
	{
		UINT16	RTCTypeCode;
	public:
		VR_OX ();
		VR_OX (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_OX (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_OX (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		BOOL	SetTypeCode ( UINT16 );	
	};


// VR_SQ's are special

class	VR_SQ	:
	public	VR_XXTYPED
	{
	public:
		VR_SQ ();
		VR_SQ (UINT16 g, UINT16 e, UINT32 l, BOOL a);
		VR_SQ (UINT16 g, UINT16 e, UINT32 l, void *d);
		VR_SQ (UINT16 g, UINT16 e, UINT32 l, void *d, BOOL a);
		UINT	GetSetSize ();
		UINT16	GetTypeCode ();
		BOOL	PostProcSetData(UINT, void *);
		BOOL	PostProcGetData(UINT, void *);
	};

