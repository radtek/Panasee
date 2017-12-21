#ifndef	_DIMSEN_HPP
#	define	_DIMSEN_HPP

#	define	N_EVENT_REPORT_RQ	0x0100
#	define	N_EVENT_REPORT_RSP	0x8100
#	define	N_GET_RQ			0x0110
#	define	N_GET_RSP			0x8110
#	define	N_SET_RQ			0x0120
#	define	N_SET_RSP			0x8120
#	define	N_ACTION_RQ			0x0130
#	define	N_ACTION_RSP		0x8130
#	define	N_CREATE_RQ			0x0140
#	define	N_CREATE_RSP		0x8140
#	define	N_DELETE_RQ			0x0150
#	define	N_DELETE_RSP		0x8150

VR		*TranslateUIDToVR ( UID *, UINT16, UINT16 );
//对于DICOM N命令有如下几种:
//N-EVENTREPORT
//N-GET
//N-SET
//N-ACTION
//N-CREATE
//N-DELETE
//所有DICOM命令都是由A命令(用于连接),N命令和C命令构成
//因此,这些命令很关键

//NEventReportRQ主要用在打印中的信息反馈
class	NEventReportRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
					PDU_Service *PDU=NULL,
					DICOMDataObject *DDO=NULL );
		BOOL	Write ( PDU_Service *,
						DICOMDataObject *DDO,
						UID		*AffectedSOPInstanceUID,
						UINT16	EventID);
		BOOL	Write ( PDU_Service *PDU,
						UID		*AffectedSOPInstanceUID,
						UINT16	EventID);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NEventReportRSP
	{
	public:

		BOOL	Read ( DICOMCommandObject *,
						PDU_Service	*PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *,
						DICOMCommandObject *,
						UID			*AffectedSOPInstanceUID,
						UINT16		Status,
						UINT16		EventID,
						DICOMDataObject *DDO = NULL);
		BOOL	Write ( PDU_Service *,
						DICOMCommandObject *,
						UID			*AffectedSOPInstanceUID,
						UINT16		EventID,
						DICOMDataObject *DDO = NULL);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};
//用于查询
class	NGetRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service	*PDU = NULL,
						DICOMDataObject	*DDO = NULL);
		BOOL	Write ( PDU_Service	*,
						DICOMDataObject *DDO,
						UID			*RequestedSOPInstanceUID);
		BOOL	Write ( PDU_Service *PDU,
						UID			*RequestedSOPInstnaceUID );
		
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NGetRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL);
		BOOL	Write ( PDU_Service *PDU,
						DICOMCommandObject *,
						UID		*AffectedSOPInstanceUID,
						UINT16	Status = 0,
						DICOMDataObject	*DDO = NULL);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};
//用于设置参数
class	NSetRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *PDU,
						DICOMDataObject *DDO,
						UID		*RequestedSOPInstanceUID );
		BOOL	Write ( PDU_Service *PDU,
						UID			*RequestedSOPInstnaceUID );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NSetRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *PDU,
						DICOMCommandObject *,
						UID		*AffectedSOPInstanceUID,
						UINT16	Status = 0,
						DICOMDataObject	*DDO = NULL);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};
//用于打印中触发打印
class	NActionRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
					PDU_Service *PDU=NULL,
					DICOMDataObject *DDO=NULL );
		BOOL	Write ( PDU_Service *,
						DICOMDataObject *,
						UID		*RequestedSOPInstanceUID,
						UINT16	ActionTypeID);
		BOOL	Write ( PDU_Service *,
						UID		*RequestedSOPInstanceUID,
						UINT16	ActionTypeID);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NActionRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *PDU,
						DICOMCommandObject *,
						UID		*AffectedSOPInstanceUID,
						UINT16	ActionTypeID,
						UINT16	Status = 0,
						DICOMDataObject	*DDO = NULL);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

//创建对象
class	NCreateRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *PDU,
						DICOMDataObject *DDO,
						UID		*AffectedSOPInstanceUID );
		BOOL	Write ( PDU_Service *PDU,
						UID		*AffectedSOPInstanceUID );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NCreateRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL,
						DICOMDataObject *DDO = NULL );
		BOOL	Write ( PDU_Service *PDU,
						DICOMCommandObject *,
						UID		*AffectedSOPInstanceUID,
						UINT16	Status = 0,
						DICOMDataObject	*DDO = NULL);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};
//删除对象
class	NDeleteRQ
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL);
		BOOL	Write ( PDU_Service *PDU,
						UID		*RequestedSOPInstanceUID );
		virtual	BOOL	GetUID ( UID & ) = 0;
	};

class	NDeleteRSP
	{
	public:
		BOOL	Read ( DICOMCommandObject *,
						PDU_Service *PDU = NULL);
		BOOL	Write ( PDU_Service *PDU,
						DICOMCommandObject *,
						UID		*AffectedSOPInstanceUID,
						UINT16	Status = 0);
		virtual	BOOL	GetUID ( UID & ) = 0;
	};


#endif

