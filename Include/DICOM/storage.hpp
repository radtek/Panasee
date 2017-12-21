

// Storage classes

// Note there are many "ungood" things about the class hierachys in
// this file.  Notable things should be protected rather than public,
// there should be no inline "wrapper"'s..  Both of these are done
// to be compatible with gcc.

// Base Storage class

#include "cctypes.h"	// Added by ClassView

#define STORE_SUCCESS								0
#define STORE_REFUSED_OUTOFRESOURCE					1
#define STORE_ERROR_DATASETDOESNOTMATCHSOPCLASS		2
#define STORE_ERROR_CANNOTUNDERSTAND				3
#define STORE_WARNING_COERCIONOFDATAELEMENTS		4
#define STORE_WARNING_DATASETDOESNOTMATCHSOPCLASS	5
#define STORE_WARNING_ELEMENTDISCARDED				6


class	StandardStorage	:
	public	CStoreRQ,
	public	CStoreRSP
	{
	public:
		UINT32	GetLastError();
		BOOL	GetUID ( UID &uid) { return (uGetUID(uid)); };
		virtual	BOOL	uGetUID ( UID &uid ) = 0;
		virtual	UINT16	CheckObject(DICOMDataObject * ) { return ( 0 ); };
		BOOL	Read (	PDU_Service *,
						DICOMCommandObject *, 
						DICOMDataObject * );
		BOOL	Write (	PDU_Service	*, DICOMDataObject	*);
	private:
		UINT32 LastError;
	protected:
		void SetLastError(UINT32 nLastError);
	};

class	UnknownStorage	:
	public	CStoreRQ,
	public	CStoreRSP
	{
	public:
		BOOL	GetUID(UID &);
		virtual	UINT16	CheckObject(DICOMDataObject * ) { return ( 0 ); };

		BOOL	Read (	PDU_Service	*,
					DICOMCommandObject	*,
					DICOMDataObject	* );
	};
class	RunTimeClassStorage	: public	StandardStorage
{
		UID		MyUID;
	public:
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
		BOOL	GetUID(UID	&);
		BOOL	SetUID(UID	&);
		BOOL	SetUID(VR	*);
		BOOL	SetUID(DICOMDataObject	*);

};

class	CRStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	CTStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO) ); };
	};

class	USMultiframeStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	MRStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	NMStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	USStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	SCStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	StandaloneOverlayStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	StandaloneCurveStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	StandaloneModalityLUTStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	StandaloneVOILUTStorage	:
	public	StandardStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
	};

class	GEStorage:
	public StandardStorage
	{
	public:
		BOOL	GEToStandard(DICOMDataObject *DDO);
		inline	Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{
				BOOL ret=StandardStorage::Read(PDU,DCO,DDO);
				if(ret)
					GEToStandard(DDO);
				return ret;
			};
		inline	Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };

	};
class	GEMRStorage	:
	public	GEStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
	};


class	GECTStorage	:
	public	GEStorage
	{
	public:
		BOOL	GetUID(UID &);
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
	};


