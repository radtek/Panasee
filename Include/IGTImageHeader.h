/********************************************************************
File Name:	ImageHeader.h
Written by: 	Ningsj
通用导航图像数据结构头
********************************************************************/
#ifndef IGT_IMAGEHEADER_H
#define IGT_IMAGEHEADER_H

#define IGT_U1 unsigned char
#define IGT_U2 unsigned short
#define IGT_S2 short
#define IGT_U4 unsigned int

#define IGT_I1 char
#define IGT_I2 short
#define IGT_I4 int
#define IGT_I8 long

#define IGT_F4 float
#define IGT_F8 double

#define IGT_IMG_DATA_TYPE	IGT_U2


struct IGT_DateTime
{
	IGT_U2	year;		//!< Four-digit year.
	IGT_U1	month;		//!< From 1 to 12.
	IGT_U1	day;		//!< Day of the month.
	IGT_U1 	hour;		//!< From 0 to 23.
	IGT_U1	minute;		//!< From 0 to 59.
	IGT_U1	second;		//!< From 0 to 59.
};

struct IGT_SeriesField
{
	IGT_U1	Id;					/* Series ID: == ui_seriesID. Series id is				
		     				   a sequential number assigned to every 
							   series in a study.						*/
	IGT_I1	Desc[128];			/* Series description: 
							   == ui_seriesDescription					*/
    IGT_I1	Protocol[32];       /* Name of series protocol: 
							   == ui_seriesProtocolName					*/
};

struct IGT_PatientField
{
	IGT_I1	Id[16];				/* Patient ID.								*/
    IGT_I1	Name[32];			/* Patient name, "family_name, first_name".	*/
	IGT_DateTime	BirthDate;	/* Patient birth date.				    	*/
	IGT_U1	Age;				/* Patient age.								*/
    IGT_I1	Sex;				/* Patient Sex: 
							   'M' : MALE , 'F': FEMALE, or 'U': UNKNOWn*/
    IGT_U2	Weight;				/* Weight of patient in kilogram.			*/
	IGT_I1	Desc[128];			/* Description of patient background, 
							   history and other info					*/
};

struct IGT_StudyField
{
	IGT_I1	Id[16];				/* Study ID	== ui_studyID, it's a unique id					
							   in the range of a hospital.				*/
	IGT_I1  BodyPart[32];		// {ZLX,2006.01.06}
	IGT_I1	Desc[128];			/* Description of study	
								== ui_studyDescription					*/
	IGT_DateTime	Time;		/* the date and time of study*/
};

#endif