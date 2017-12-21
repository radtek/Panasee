

// Q/R SOP Class

// Abstract Base Class for Q/R

/*******************ADD BY JWANG******************************/
#define QUERY_SUCCESS										0
#define QUERY_REFUSED_OUTOFRESOURCE							1
#define QUERY_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS			2
#define QUERY_FAILED_UNABLETOPROCESS						3
#define QUERY_CANCEL_MATCHINGTERMINATEDDUETOCANCELREQUEST	4
#define QUERY_PENDING_ALLOPTIONKEYSWERESUPPORTED			5
#define QUERY_PENDING_SOMEOPTIONKEYSWERENOTSUPPORTED		6

#define RETRIEVE_SUCCESS									10
#define RETRIEVE_REFUSED_UNABLETOCALCULATENUMBEROFMATCHS	11
#define RETRIEVE_REFUSED_UNABLETOPERFORMSUBOPERATIONS		12
#define RETRIEVE_REFUSED_MOVEDESTINATIONUNKNOWN				13
#define RETRIEVE_FAILED_IDENTIFIERDOESNOTMATCHSOPCLASS		14
#define RETRIEVE_FAILED_UNABLETOPROCESS						15
#define RETRIEVE_CANCEL_SUBOPERATIONSTERMINATED				16
#define RETRIEVE_WARNING_SUBOPERATIONCOMPLETE				17
#define RETRIEVE_PENDING_SUBOPERATIONSARECONTINUING			18
/*************************************************************/

class	StandardQuery	:
	public	CFindRQ,
	public	CFindRSP
	{
	public:
		UINT32	GetLastError();  //ADD BY JWANG
		virtual	BOOL	GetUID (UID	&uid) { return (uGetUID(uid)); };
		virtual	BOOL	uGetUID ( UID &) = 0;
		virtual	BOOL	SearchOn (	DICOMDataObject	*,
									Array < DICOMDataObject  *> 	*) = 0;
		virtual	BOOL	CallBack ( 	DICOMCommandObject	*,
									DICOMDataObject * ) = 0;
		BOOL	Read (	PDU_Service	*,
						DICOMCommandObject	*);
		BOOL	Write (	PDU_Service	*,
						DICOMDataObject	*);
	private:
		UINT32 LastError;   //ADD BY JWANG
	protected:
		void SetLastError(UINT32 nLastError);  //ADD BY JWANG
						
	};

class	StandardRetrieve	:
	public	CMoveRQ,
	public	CMoveRSP
	{
	public:
		UINT32	GetLastError();  //ADD BY JWANG
		virtual	BOOL	GetUID (UID	&uid) { return (uGetUID(uid)); };
		virtual	BOOL	uGetUID ( UID &) = 0;
		virtual	BOOL	SearchOn ( DICOMDataObject	*,
								Array < DICOMDataObject *> *) = 0;
		virtual	BOOL	RetrieveOn (	DICOMDataObject *,
										DICOMDataObject **,
										StandardStorage	**) = 0;
		virtual	BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * ) = 0;
		virtual	BOOL	CallBack (	DICOMCommandObject	*,
									DICOMDataObject	* ) = 0;
		BOOL	Read (	PDU_Service *, DICOMCommandObject	* );
		BOOL	Write ( PDU_Service	*, DICOMDataObject	*, BYTE	*);
	private:
		UINT32 LastError;   //ADD BY JWANG
	protected:
		void SetLastError(UINT32 nLastError);  //ADD BY JWANG
	};

class	PatientRootQuery	:
			public	StandardQuery
	{
	public:
		BOOL	GetUID ( UID &);
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardQuery :: Read ( PDU, DCO ) ); };
		inline	BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO)
			{ return ( StandardQuery :: Write ( PDU, DDO ) ); };
	};

class	PatientRootRetrieve	:
			public	StandardRetrieve
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };

		virtual	BOOL	SearchOn ( DICOMDataObject	*,
			Array < DICOMDataObject *> *){return TRUE;};
		virtual	BOOL	RetrieveOn (	DICOMDataObject *,
										DICOMDataObject **,
										StandardStorage	**){return TRUE;};
		virtual	BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * ){return TRUE;};
		virtual	BOOL	CallBack (	DICOMCommandObject	*,
			DICOMDataObject	* ){return TRUE;};

		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardRetrieve :: Read ( PDU, DCO ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieve :: Write ( PDU,
						DDO, ACRNema )); };
	};

class	StudyRootQuery	:
			public	StandardQuery
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardQuery :: Read ( PDU, DCO ) ); };
		inline	BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO)
			{ return ( StandardQuery :: Write ( PDU, DDO ) ); };
	};

class	StudyRootRetrieve	:
			public	StandardRetrieve
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		virtual	BOOL	SearchOn ( DICOMDataObject	*,
			Array < DICOMDataObject *> *){return TRUE;};
		virtual	BOOL	RetrieveOn (	DICOMDataObject *,
										DICOMDataObject **,
										StandardStorage	**){return TRUE;};
		virtual	BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * ){return TRUE;};
		virtual	BOOL	CallBack (	DICOMCommandObject	*,
			DICOMDataObject	* ){return TRUE;};

		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardRetrieve :: Read ( PDU, DCO ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieve :: Write ( PDU,
						DDO, ACRNema )); };
	};

class	PatientStudyOnlyQuery	:
			public	StandardQuery
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardQuery :: Read ( PDU, DCO ) ); };
		inline	BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO)
			{ return ( StandardQuery :: Write ( PDU, DDO ) ); };
	};

class	PatientStudyOnlyRetrieve	:
			public	StandardRetrieve
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardRetrieve :: Read ( PDU, DCO ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieve :: Write ( PDU,
						DDO, ACRNema )); };
	};

//add by TLin
class ModalityWorklistQuery : public StandardQuery  
{
public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		virtual	BOOL	SearchOn (	DICOMDataObject	*,
			Array < DICOMDataObject  *> 	*){return TRUE;};
		virtual	BOOL	CallBack ( 	DICOMCommandObject	*,
			DICOMDataObject * ){return TRUE;};
		inline	BOOL	Read (	PDU_Service	*PDU,
								DICOMCommandObject	*DCO )
			{ return ( StandardQuery :: Read ( PDU, DCO ) ); };
		inline	BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO)
			{ return ( StandardQuery :: Write ( PDU, DDO ) ); };

};


