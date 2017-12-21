// ImriDefine.h : 
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(__IMRIDEFINE_H__)
#define __IMRIDEFINE_H__

#include <afxtempl.h>



enum ENUM_MRI_SYS_STATUS
{
	MSYS_IDLE			= 0x0000,
	MSYS_IDLE_TO_SCAN	= 0x0001,
	MSYS_SCAN_TO_PAUSE	= 0x0002,
	MSYS_PAUSE_TO_SCAN	= 0x0003,
	MSYS_SCANNING		= 0x0004,
	MSYS_SCAN_TO_IDLE	= 0x0005
};

enum ENUM_STATUS_TYPE
{
	INFO_STATUS		= 0x0001,
};

// 提示消息类别
enum ENUM_READY_MODE
{
	PACKET_READY_MODE_PROSPECT				= 1,	// 在PROSPECT端操作ReadyForScan按钮
	PACKET_READY_MODE_NAVIGATION			= 2,	// 在NAVIGATION端远程操作ReadyForScan按钮
}; 

enum ENUM_INFOR_TYPE
{
	INFO_PROTOCOLS	= 0x0002,
	INFO_MODALITY	= 0x0003,
	INFO_PATIENT	= 0x0004,
	INFO_STUDY		= 0x0005,
	INFO_PROTOCAL_FAMILY = 0x0006,
	INFO_SHIM_SNR   = 0x0007
};

enum ENUM_COMMANDS
{
	COMMAND_CREATESERIES	    = 0x0001,
	COMMAND_STARTPRESCAN	    = 0x0002,
	COMMAND_STARTPRESCANSCAN	= 0x0003,
	COMMAND_STARTSCAN	    	= 0x0004,
	COMMAND_STOPSCAN	    	= 0x0005,
	COMMAND_PAUSESCAN	    	= 0x0006,
	COMMAND_RESUMESCAN	    	= 0x0007,
	COMMAND_CREATESTUDY      	= 0x0008,
	COMMAND_STARTLOOPSCAN    	= 0x0009,
	COMMAND_CREATEPATIENT       = 0x0010,
	COMMAND_SETCURPATIENT       = 0x0011,
	COMMAND_CHANGEPROTOCOL      = 0x0012,
	COMMAND_STARTSHIM           = 0x0013,
	COMMAND_STARTTUNE           = 0x0014
};

enum ENUM_INFOR_RESULT
{
	IR_OK	= 0x0000,
	IR_NO_PROTOCOL = 0x0100,
	IR_NO_MODALITY = 0x0200,
	IR_NO_PATIENT  = 0x0300,
	IR_NO_STUDY    = 0x0400,
};


/************************************************************************/
/*
	时间数据 
*/
/************************************************************************/
typedef struct DataType_DateTime
{
	unsigned short	year;		//!< Four-digit year.
	unsigned char	month;		//!< From 1 to 12.
	unsigned char	day;		//!< Day of the month.
	unsigned char 	hour;		//!< From 0 to 23.
	unsigned char	minute;		//!< From 0 to 59.
	unsigned char	second;		//!< From 0 to 59.
} DATATYPE_DATETIME;



struct IMRI_SHIM_SNR_TUNING_INFO
{
	int Version;
	unsigned short Size;
	float	       ShimRst;	 
	DATATYPE_DATETIME	   ShimTime;
	BYTE        ShimCoilCode;
	BYTE        ShimPhantom;    
	float	       SNR;	 
	DATATYPE_DATETIME	   SNRTime;	   
	BYTE        SNRCoilCode;
	BYTE        SNRPhantom;
	float	       TuningRst;
	DATATYPE_DATETIME	   TuningTime;
	BYTE        TuneCoilCode;
	BYTE        TunePhantom;
	unsigned short  Voltage1;
	unsigned short  Voltage2;
	char Reserved[50];  

	IMRI_SHIM_SNR_TUNING_INFO()
	{
		memset(this,0,sizeof(IMRI_SHIM_SNR_TUNING_INFO));
	}
};

struct IMRI_PROTOCOL_FAMILY
{
	unsigned int	nID;			// 协议族ID
	unsigned char	sDesc[64];		// 协议族描述
	
	IMRI_PROTOCOL_FAMILY()
	{
		nID = 0;
	}


};

struct IMRI_PROTOCOL_GROUP
{
	unsigned int	nID;			// 部位ID
	unsigned char	sDesc[64];					// 部位描述
	unsigned int	nProtocolSum;			// number of protocols
	unsigned int	nProtocolType;		// 1: IMRIACP V1, 2: IMRIACP V2
	void			**pProtocols;

	IMRI_PROTOCOL_GROUP()
	{
		pProtocols = NULL;
		nProtocolSum = 0;
		nID = 0;
	}


};

struct IMRI_PROTOCOLGROUP_DESC
{
	unsigned short  nProtocolGroupNo;
	unsigned char   cProtocolGroupDesc[64];
};

struct IMRI_PROTOCOL_DESC
{
	unsigned short  nProtocolNo;
	unsigned char   cProtocolDesc[64];
	float fThickness;
	float fGap;
	unsigned short iSlice;
	unsigned short iNex;
	unsigned short Reserved[1];  
	unsigned short iPlane; //0:axi 1:sag 2:cor

};

struct IMRI_PROTOCOL
{
	IMRI_PROTOCOL_DESC oProtDesc;
	IMRI_PROTOCOL_GROUP* pBelongGrp;

	IMRI_PROTOCOL()
	{
		pBelongGrp = NULL;
	}
};

struct IMRI_COMMAND_LOADPROTOCOL
{
	unsigned short  nReadyMode;		// enum ENUM_LOADPROTOCOL_TYPE
	IMRI_PROTOCOLGROUP_DESC	protocolGroup;
	IMRI_PROTOCOL_DESC			protocol;
};



struct IMRI_LOCALIZATION
{
	float          fCenterX, fCenterY, fCenterZ;	// SLAB的中心点的物理坐标(mm)
	float          fFreqX, fFreqY, fFreqZ;		    // SLAB的频率编码方向
	float          fPhaseX, fPhaseY, fPhaseZ;	    // SLAB的相位编码方向
	unsigned short nFovX, nFovY;		            // 扫描视野(mm)
	unsigned short nResolutionZ;		            // SLAB的扫描层数

	unsigned short nPlane;                          // 扫描平面 0:Obl 1: probe tip Axis 2:probe tip Cor 3:probe tip Sag  4:mri center axi 5:mri center cor 6:mri center sag 
	float fThickness;
	float fGap;
	unsigned short iSlice;
	unsigned short iNex;

};


struct IMRI_MODALITY
{
	char			cId[16] ;
	char			cName[32] ;
	char			cType[32] ;
	char			cDesc[100] ;
};

struct IMRI_PATIENT
{
    int				Version;			/* Version of this field. Format:
											 (major_version << 16) + lower_version.	*/
    unsigned short	Size;				/* Size of this field.                      */	
    char			Id[16];				/* Patient ID.								*/
    char			Name[32];			/* Patient name, "family_name, first_name".	*/
    DATATYPE_DATETIME	BirthDate;		/* Patient birth date.				    	*/
    unsigned char	Age;				/* Patient age.								*/
    char			Sex;				/* Patient Sex: 
											 'M' : MALE , 'F': FEMALE, or 'U': UNKNOWn*/
    unsigned short	Weight;				/* Weight of patient in kilogram.			*/
    unsigned char	Desc[128];			/* Description of patient background, 
											history and other info					*/
    unsigned char   Reserved[128];      /* Reserved space for this field.           */
};

struct IMRI_STUDY
{
    int				Version;			/* Version of this field. Format:
										   (major_version << 16) + lower_version.	*/
    unsigned short	Size;				/* Size of this field.                      */	
    char			Id[16];				/* Study ID	== ui_studyID, it's a unique id					
										   in the range of a hospital.				*/
    char			Desc[128];			/* Description of study	
											== ui_studyDescription					*/
    char			Protocol[32];		/* Name of protocol: == ui_clinicalProtocol */
    unsigned short	ProtocolVersion;	/* Version of protocol used.				*/
    DATATYPE_DATETIME	Time;			/* the date and time of study*/
    char			OperatorID[16];		/* ID of the operator: == ui_operatorID		*/
    char			Radiologist[32];	/* Name of the radologist/specialist: 
											 == ui_radiologistName					*/
    char			RefPhy[32];			/* Name of referring physician: 
										   == ui_referalPhysician					*/
    unsigned char	PatPosition;        /* Position of the patient: ==ui_lyingPos.   
											See enum BODYPOS.						*/
    unsigned char	PatEntry;           /* Patient entry direction:
											== ui_bodyEntry
											enum BODYENTRY {HEAD_FIRST, FOOT_FIRST}	*/
    unsigned char	PatBodyPart;        /* Part of body examined: ==ui_bodyPart
										   See enum BODYPART.						*/
    unsigned char	LandmarkPos;        /* Scan landmark position reference: 
							   ==ui_landmarkPos. See enum LandPARKPOS.  */
    char			GradientId[32];		/* ID of Gradient coil.						*/
	char			MrNo[32];			// {ZLX,2006.01.06}
	char			BodyPart[32];		// {ZLX,2006.01.06}
	
    char			Reserved[64];	    /* Reserved space for this field.           */
};


#endif