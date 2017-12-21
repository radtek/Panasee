// ImriULProtocol.h : 
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(__IMRIULPROTOCOL_H__)
#define __IMRIULPROTOCOL_H__

#include "ImriDefine.h"
 
#define ULPACKHEADER_LEN             80
#define MAX_ULPACKLEN                16*1024*1024

#define GET_VERSION_HI(x) ((x) >> 16)
#define GET_VERSION_LO(x) ((x) & 0xffff)


/************************************************************************/
/*
	IMRIACP Upper Protocol 
	Version 2.1
	2008.2
*/
/***********************************************************************/
enum ENUM_DEVICE_TYPE
{
	DEVICETYPE_UNKNOWN = 0x0000,
	DEVICETYPE_CT      = 0x0001,
	DEVICETYPE_US      = 0x0002,
	DEVICETYPE_MRI     = 0x0003,
	DEVICETYPE_XRAY    = 0x0004
};

enum ENUM_PACK_TYPE
{
	PACKTYPE_NEGOTIATE	    = 0x0000,
	PACKTYPE_IMAGE		    = 0x0001,
	PACKTYPE_LOCALIZATION	= 0x0002,
	PACKTYPE_COMMAND   	    = 0x0003,
	PACKTYPE_MESSAGE	    = 0x0004,
	PACKTYPE_STATUS		    = 0x0005,
	PACKTYPE_INFOR		    = 0x0006,
	PACKTYPE_CRITICAL_MSG	= 0x0014,
};

enum ENUM_IMAGE_TYPE
{
	IMGTYPE_XINAO		= 0x0001,
	IMGTYPE_DICOM3		= 0x0002,
	IMGTYPE_BMP         = 0x0003,
	IMGTYPE_JPG         = 0x0004,
	IMGTYPE_OTHER         = 0x0080
};

enum ENUM_LOADPROTOCOL_TYPE
{
	LOADPROTOCOL_MANUAL		= 0x0001,
	LOADPROTOCOL_AUTO		= 0x0002,
};

enum ENUM_MESSAGE_TYPE
{
	MSGTYPE_CONFIRM	= 0x0001,
	MSGTYPE_WARNING	= 0x0002,
	MSGTYPE_ERROR	= 0x0003,
};

enum ENUM_INFO_OPER_TYPE
{
	INFOOPER_REQUEST = 0x0001,
	INFOOPER_ANSWER	 = 0x0002,
};




//定义需要的数据结构
//1、上层协议的包头定义
struct IMRI_PACKHEADER
{
	unsigned int nVer     ;
	int          nPackDataLen ;
	char         cUnUsed[64];
	int          nModalityType;
	int          nCommand     ;			// enum ENUM_PACK_TYPE
	unsigned char *cPackData[1];
} ;

struct IMRI_IMAGEPKG_HEADER
{
	unsigned short	nImgType;			// enum ENUM_IMAGE_TYPE
	unsigned short  nSeriesNo;
	unsigned short  nScanNo;
	unsigned short  nImgNum;
	char			cReserved[24];
} ;

struct IMRI_IMAGEPKG
{
	int				nImgLength;
	unsigned char	cImgData[1];
} ;

struct IMRI_LOCALIZATION_HEADER
{
	unsigned short  nSlabNum;
	unsigned char   cSlabData[1];
} ;

struct IMRI_COMMAND
{
	unsigned short  nCmd;			// enum ENUM_COMMANDS
	unsigned char   cCmdData[1];
} ;

struct IMRI_MESSAGE
{
	unsigned short  nMsgType;		// enum ENUM_MESSAGE_TYPE
	unsigned short  nMsgLength;
	unsigned char   cMsg[1];
} ;

struct IMRI_STATUS_HEADER
{
	unsigned short  nOperMode;		// enum ENUM_INFO_OPER_TYPE
	unsigned short  nSatusType;		// enum ENUM_STATUS_TYPE
	unsigned char	cStatusData[1];
};

struct IMRI_INFOR_HEADER 
{
	unsigned short nOperMode;		// enum ENUM_INFO_OPER_TYPE
	unsigned short nInfoType;		// enum ENUM_INFOR_TYPE
	unsigned short nResult;			// enum ENUM_INFO_RESULT
	unsigned char  cInfoData[1];

	IMRI_INFOR_HEADER() {
		nOperMode = 0;
		nInfoType = 0;
		nResult = 0;
	}
};

struct IMRI_STATUS_SYSTEM
{
	unsigned short  nSysStatus;
	unsigned short  nSeriesStatus;
	int				nOperResult;
} ;

struct IMRI_INFO_PROTOCOLGROUP_HEADER
{
	int				nProtocolGroupNum;
	unsigned char   cProtocolGroupData[1];
} ;

struct IMRI_INFO_PROTOCOLGROUP
{
	IMRI_PROTOCOLGROUP_DESC	protocolGroup;	
	unsigned short			nProtocolNum ;
	unsigned char			cProtocolData[1] ;
} ;

struct IMRI_INFO_PROTOCOLFAMILY_HEADER
{
	int				nProtocolFamilyNum;
	unsigned char   cProtocolFamilyData[1];
} ;

struct IMRI_INFO_PROTOCOLFAMILY
{
	IMRI_PROTOCOLGROUP_DESC	protocolFamily;
} ;

struct IMRI_CRITICAL_MSG
{
	unsigned short	nMsgLength;
	unsigned char	cMsg[1];
} ;



#endif