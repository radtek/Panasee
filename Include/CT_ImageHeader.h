
/********************************************************************
File Name:	CTImageHeader.h
Written by: Zhang Xiaochun    2008.11.10

  Copyright(C) XinAoMDT, 2001 - 2020
  
	Purpose:
	This file defines CT image structs.
	
	  History:
	  2008.11.10	Created. version is 1.0
*********************************************************************/
#ifndef CT_IMAGEHEADER_H
#define CT_IMAGEHEADER_H

#ifndef U1
#define U1 unsigned char
#endif
#ifndef U2
#define U2 unsigned short
#endif
#ifndef U4
#define U4 unsigned int
#endif
#ifndef I1
#define I1 char
#endif
#ifndef I2
#define I2 short
#endif
#ifndef I4
#define I4 int
#endif
#ifndef I8
#define I8 long
#endif
#ifndef F4
#define F4 float
#endif
#ifndef F8
#define F8 double
#endif
#ifndef IMG_DATA_TYPE
#define IMG_DATA_TYPE	U2
#endif


/************************************************************************/
/*
日期和时间数据结构。 
*/
/************************************************************************/
struct CT_BIDateTime
{
	U2	year;		//!< Four-digit year.
	U1	month;		//!< From 1 to 12.
	U1	day;		//!< Day of the month.
	U1 	hour;		//!< From 0 to 23.
	U1	minute;		//!< From 0 to 59.
	U1	second;		//!< From 0 to 59.
};

/************************************************************************/
/*
图像头文件； size is 152 
*/
/************************************************************************/
struct CT_HeaderInfo
{
	//必备字段
	I1	BITitle[5];			/* Storing Modality Type and Image type    */
	I4	Version;			/* Header file version, format:	major_version=1	minor_version=0						
							(major_version << 16) + minor_version.	*/
	
	
	//可选字段
	U1  Reserved[140];      /* Reserved space for this field.           */
};   //sizeof(IMG_HeaderInfo) = 152

/************************************************************************/
/*
设备参数结构； size is 308
*/
/************************************************************************/
struct CT_SiteField
{
	//必备字段
	I1			Id[16];				/* machine's ID								*/
	I1          Manufacturer[64];   /* Manufacturer of the equipment            */
	I1			InstName[32];		/* name of this site institution, for example
									CHN_BJ_TIANTAN001						    */
	I1			InstDeptName[32];	/* name of department						*/
	I1			StationName[32];	/* name of machine							*/
	I1	        BISerialNum[32];	/* SN of the machine, assigned by BI		*/

	//可选字段
	CT_BIDateTime  LastCalib;			/* date and time of last calibration		*/
	I1          SoftVer[32]	;		/*Software Version that Produced the image. */
	U1          Reserved[60];		/* Reserved space for this field.           */
};   //sizeof(IMG_SiteField) = 308

/************************************************************************/
/*
病人结构信息； size if 324
*/
/************************************************************************/
struct CT_PatientField
{
	//必备字段
	I1			Id[16];				/* Patient ID.								*/
	I1			Name[32];			/* Patient name, "family_name, first_name".	*/
	
	U1			Age;				/* Patient age.								*/
	I1			Sex;				/* Patient Sex: 
									'M' : MALE , 'F': FEMALE, or 'U': UNKNOWn	*/
	//可选字段
	CT_BIDateTime	BirthDate;			/* Patient birth date.				    */
	U2			Weight;				/* Weight of patient in kilogram.			*/
	I1			Desc[128];			/* Description of patient background, 
									history and other info						*/
	U1			Reserved[136];      /* Reserved space for this field.           */
};   //sizeof(IMG_PatientField) = 324

/************************************************************************/
/*
Study信息； size is 436
*/
/************************************************************************/
struct CT_StudyField
{
	//必备字段
	I1	Id[16];				/* Study ID	== ui_studyID, it's a unique id					
							in the range of a hospital.				*/
	U1	PatPosition;        /* Position of the patient: ==ui_lyingPos.   
							See enum BODYPOS.						*/
	U1	PatEntry;           /* Patient entry direction:
							== ui_bodyEntry
							enum BODYENTRY {HEAD_FIRST, FOOT_FIRST}	*/
	I1  BodyPart[32];		// {ZLX,2006.01.06}
	I1	Desc[128];			/* Description of study	
							== ui_studyDescription					*/
	CT_BIDateTime Time; /* the date and time of created the study   */
	
	//可选字段																	
	U1  Reserved[250];       /* Reserved space for this field.           */
};   //sizeof(IMG_StudyField) = 436


/*********************************************************
定义序列的参数设定；size is 856
**********************************************************/
struct CT_SeriesField
{
	//必备字段
	U2	Id;					/* Series ID: == ui_seriesID. Series id is				
							a sequential number assigned to every 
							series in a study.						*/
	F4	SliceThickness;		/* Slice thickness in mm,  ==ui_sliceThk;   */
	F4	SliceGap;			/* Gap between two adjacent slices in mm. 
							== ui_sliceGap							*/
	U2	FovX;				/* Display field-of-view along X (mm).
							== ui_imageFOV							*/
	U2	FovY;				/* Diplay field-of-view along Y (mm).
							== ui_imageFOV * ui_phaseFOVRatio		*/
	U2	FovZ;				/* Display field-of-view along Z (mm). only 
							usable in 3D, leave it for now			*/
	U2	ResolutionX;		/* The number of data poI4s along read 
							direction  (number of read poI4s).
							== ui_readPnts							*/
	U2	ResolutionY;		/* The number of data poI4s along phase 
							direction (number of phase encoding)
							== ui_phasePnts							*/
	U2	ResolutionZ;		/* The number of data poI4s along second 
							phase direction in 3D imaging.
							== ui_phaseZPnts							*/
	I1	Desc[128];			/* Series description: 
							== ui_seriesDescription					*/
	I1	Protocol[32];       /* Name of series protocol: 
							== ui_seriesProtocolName					*/
	U2  KVP;                /* Peak kilo voltage of X-ray generator */
	U2  TubeCurrent ;       /* Tube Current of X-ray in mA */
	U2  DetectorTilt;       /* Gantry/Detector Tilt in degree */

	//可选字段
	CT_BIDateTime DateTime;		/* Series Date & Time	*/
	U1  RotateDir;          /* Rotate direction of the source.0: unkown;1:clockwise;2:counter clockwise */
	I2  TableHeight;        /* Table Height in mm .Below the center is positive */
	U2  CollectDiameter;    /* Data collected diameter */
	U2  DataDiameter;       /* Data reconstructed diameter */
	U2  Exposure ;          /* Exposure expressed in uAs. */
	I1  Reserved[648];
};  // sizeof(SeriesField) = 856

/*****************************************
	扫描序列信息； size is 340
*****************************************/
struct CT_ScanField
{
	//必备字段
	I4	Id;			        /* Scan ID, sequential number in series.	*/
	F4   FreqDirX;			
	F4   FreqDirY;
	F4   FreqDirZ;          /* Frequence encoding direction			*/
	
    F4   PhaseDirX;
	F4   PhaseDirY;
	F4   PhaseDirZ;			/* Phase encoding direction                 */
	
	F4	SliceDirX;
	F4	SliceDirY;
	F4	SliceDirZ;

	//可选字段
	I1  Reserved[300];
};   //sizeof(IMG_ScanField) = 340

/************************************************************************/
/*
图像序列信息； size is 160
*/
/************************************************************************/
struct CT_ImageField
{
	//必备字段
	U2		Id;					/* Image ID, sequential number in scan.		*/
	
	F4 fCenterPointX;
	F4 fCenterPointY;
	F4 fCenterPointZ;			// coordinates of slice's center point
	
	I2		WindowLevel;        /* Display window level, which is determined
								by the mean I4ensity of the image		*/
	I2		WindowWidth;        /* Display window width, which is determined 
														by the standard deviation of the signal 
								I4ersities of the image.				*/
	//可选字段	
	CT_BIDateTime DateTime;		/* Image Date & Time          */
	U1 Reserved[132];  /* Reserved space for this field.           */
};   //sizeof(IMG_ImageField) = 160

typedef CT_HeaderInfo		CT_HEADER_INFO;
typedef	CT_SiteField		CT_SITE_FIELD;
typedef CT_PatientField	    CT_PATIENT_FIELD;
typedef CT_StudyField		CT_STUDY_FIELD;
typedef CT_SeriesField		CT_SERIES_FIELD;
typedef CT_ScanField		CT_SCAN_FIELD;
typedef CT_ImageField		CT_IMAGE_FIELD;

/************************************************************************/
/*
BIImage Header Information  
*/
/************************************************************************/
struct CT_ImageHeader
{
    CT_HEADER_INFO		headInfo;
    CT_SITE_FIELD		siteField;
    CT_PATIENT_FIELD	patientField;
    CT_STUDY_FIELD		studyField;
    CT_SERIES_FIELD	    seriesField;
    CT_SCAN_FIELD		scanField;
    CT_IMAGE_FIELD		imageField;
	
	CT_ImageHeader()
	{
		memset( &headInfo, 0, sizeof(CT_HEADER_INFO) );
		memset( &siteField, 0, sizeof(CT_SITE_FIELD) );
		memset( &patientField, 0, sizeof(CT_PATIENT_FIELD) );
		memset( &studyField, 0, sizeof(CT_STUDY_FIELD) );
		memset( &seriesField, 0, sizeof(CT_SERIES_FIELD) );
		memset( &scanField, 0, sizeof(CT_SCAN_FIELD) );
		memset( &imageField, 0, sizeof(CT_IMAGE_FIELD) );
	}
};

struct CT_STRUCT_IMAGE
{
	CT_ImageHeader	oHeader;
	IMG_DATA_TYPE*	pData;
	
	CT_STRUCT_IMAGE()
	{
		memset( &oHeader, 0, sizeof(CT_ImageHeader) );
		pData = NULL;
	}
};

typedef CT_STRUCT_IMAGE* LPCT_STRUCT_IMAGE;


#endif