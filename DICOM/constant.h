//该文件定义常数
//比如错误码,数据代码
#ifndef __CONSTANT_H
#define __CONSTANT_H
#	define	IsThisaCmdMsg(ccc)			(ccc&0x01)
#	define	IsThisaDataMsg(ccc)			(!(ccc&0x01))
#	define	IsThisTheLastFragment(ccc)	(ccc&0x02)

#	define	PDU_ASSOCIATE_RQ			0x01
#	define	PDU_ASSOCIATE_AC			0x02
#	define	PDU_ASSOCIATE_RJ			0x03
#	define	PDU_PDATA					0x04
#	define	PDU_RELEASE_RQ				0x05
#	define	PDU_RELEASE_RP				0x06
#	define	PDU_ABORT_RQ				0x07

#	define	PDU_APPLICATION_CONTEXT		0x10
#	define	PDU_PRESENTATION_CONTEXT	0x20
#	define	PDU_PRESENTATION_CONTEXT_ACCEPT	0x21
#	define	PDU_ABSTRACT_SYNTAX			0x30
#	define	PDU_TRANSFER_SYNTAX			0x40
#	define	PDU_USER_INFORMATION		0x50

#	define	NO_REASON						0x01
#	define	APPLICATION_CONTEXT_NAME_BAD	0x02
#	define	CALLING_APPICATION_BAD			0x03
#	define	CALLED_APPLICATION_BAD			0x07
#	define	PROTOCOL_VERSION_BAD			0x02
#	define	TO_MUCH_NETWORK_TRAFFIC			0x01
#	define	TIME_LIMIT						0x02

#	define	DICOM_ULS_USER					0x01
#	define	DICOM_ULS_PROVIDER_ACSE			0x02
#	define	DICOM_ULS_PROVIDER_PRESENTATION	0x03
#define MAX_HOSTNAME (15 + 1)
#define MAX_VRAE (16 + 1)
#define MAX_VRAS 4
#define MAX_VRAT 4
#define MAX_VRCS (16 + 1) 
#define MAX_VRDA (10 + 1)
#define MAX_VRDS (16 + 1)
#define MAX_VRDT (26 + 1)
#define MAX_VRFL 4
#define MAX_VRFD 8
#define MAX_VRIS (12 + 1)
#define MAX_VRLO (64 + 1)
#define MAX_VRLT (10240 + 1)
#define MAX_VRPN (320 + 1)
#define MAX_VRSH (16 + 1)
#define MAX_VRSL 4
#define MAX_VRSS 2
#define MAX_VRST (1024 + 1)
#define MAX_VRTM (16 + 1)
#define MAX_VRUI (64 + 1)
#define MAX_VRUL 4
#define MAX_VRUS 2
#endif