#ifndef QRGLOBAL_H
#define QRGLOBAL_H
#include "dicom.hpp"
void trimcpy(char* str,void*pData,int len);
class mdate
{
	public:
		char datebuf[9];//yyyymmdd0
		void SetDate(char*pstr)
		{
			int j=0;
			for(int i=0;pstr[i]!='\0'&&j<8;i++)
			{
				if(pstr[i]>='0'&&pstr[i]<='9')
				{
					datebuf[j]=pstr[i];
					j++;
					continue;
				}
				else if(pstr[i]=='.')
					continue;
				else 
				{
					memset(datebuf,0,9);
					return;
				}
			}
			if(j!=8)
			{
				memset(datebuf,0,9);
				return;
			}
			datebuf[j]='\0';
		}
		void SetDate(VR* vr)
		{
			memset(datebuf,0,9);
			if(vr==NULL)
				return;
			char newbuf[20];
			memset(newbuf,0,20);
			if(vr->Length>=20)
				return;
			memcpy(newbuf,vr->Data,vr->Length);
			newbuf[19]='\0';
			for(int i=18;i>=0;i--)
			{
				if(newbuf[i]==' ')
				{
					newbuf[i]='\0';
					continue;
				}
				if(newbuf[i]=='\0')
					continue;
				else
					break;
			}
			SetDate(newbuf);
		}
	public:
		mdate(){memset(datebuf,0,9);}
		mdate(char*pstr)
		{
			SetDate(pstr);
		}
		mdate(VR* vr)
		{
			SetDate(vr);
		}
		mdate& operator =(char* strdate){SetDate(strdate);return *this;}
		mdate& operator =(VR* vr){SetDate(vr);return *this;}
		static void splite(mdate& mindate,mdate& maxdate,char*pstr)
		{
			int len=strlen(pstr);
			if(len<8)
				return;
			char*ps=strchr(pstr,'-');
			if(ps==NULL)
			{
				mindate=pstr;
				maxdate=pstr;
				return;
			}
			if(ps!=pstr&&ps!=pstr+8&&ps!=pstr+10)
				return;
			*ps='\0';
			mindate=pstr;
			maxdate=ps+1;
		}
		static void splite(mdate& mindate,mdate& maxdate,VR*vr)
		{
			char pstr[64];
			memset(pstr,0,64);
			if(vr->Length>63)return;
			memcpy(pstr,vr->Data,vr->Length);
			for(int len=63;len>=0;len--)
			{
				if(pstr[len]==' ')
					pstr[len]='\0';
				if(pstr[len]!='\0')
					break;
			}
			splite(mindate,maxdate,pstr);
		}

		BOOL operator !=(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)==0)return FALSE;return TRUE;}
		BOOL operator ==(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)==0)return TRUE;return FALSE;}
		BOOL operator !=(int val){if(datebuf[0]=='\0')return FALSE;return TRUE;}
		BOOL operator ==(int val){if(datebuf[0]=='\0')return TRUE;return FALSE;}
		BOOL operator <(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)<0)return TRUE;return FALSE;}
		BOOL operator >(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)>0)return TRUE;return FALSE;}
		BOOL operator <=(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)<=0)return TRUE;return FALSE;}
		BOOL operator >=(mdate otherdate){if(memcmp(datebuf,otherdate.datebuf,8)>=0)return TRUE;return FALSE;}

		BOOL Empty(){memset(datebuf,0,9);}
		BOOL IsEmpty(){if(datebuf[0]=='\0')return TRUE;return FALSE;}
		BOOL IsNotEmpty(){return !IsEmpty();}
};
class mtime
{
	public:
		char timebuf[16];//hhmmss.ffffff0
		void SetTime(char*pstr)
		{
			int j=0;
			for(int i=0;pstr[i]!='\0'&&j<13;i++)
			{
				if((pstr[i]>='0'&&pstr[i]<='9')||pstr[i]=='.')
				{
					timebuf[j]=pstr[i];
					j++;
					continue;
				}
				else if(pstr[i]==':')
					continue;
				else 
				{
					memset(timebuf,0,16);
					return;
				}
			}
			timebuf[j]='\0';
		}
		void SetTime(VR* vr)
		{
			char newbuf[20];
			memset(newbuf,0,20);
			memset(timebuf,0,16);
			if(vr==NULL||vr->Length==0)
				return;
			if(vr->Length>=20)
				return;
			memcpy(newbuf,vr->Data,vr->Length);
			newbuf[19]='\0';
			for(int i=18;i>=0;i--)
			{
				if(newbuf[i]==' ')
				{
					newbuf[i]='\0';
					continue;
				}
				if(newbuf[i]=='\0')
					continue;
				else
					break;
			}
			SetTime(newbuf);
		}

	public:
		mtime(){memset(timebuf,0,16);}
		mtime(char*pstr){SetTime(pstr);}
		mtime(VR*vr){SetTime(vr);}

		static void splite(mtime& mintime,mtime& maxtime,char*pstr)
		{
			int len=strlen(pstr);
			if(len<6)return;
			char*ps=strchr(pstr,'-');
			if(ps==NULL)
			{
				mintime=pstr;
				maxtime=pstr;
				return;
			}
			*ps='\0';
			mintime=pstr;
			maxtime=ps+1;
		}
		static void splite(mtime& mintime,mtime& maxtime,VR*vr)
		{
			char pstr[64];
			memset(pstr,0,64);
			if(vr->Length>63)return;
			memcpy(pstr,vr->Data,vr->Length);
			for(int i=63;i>=0;i--)
			{
				if(pstr[i]==' ')
					pstr[i]='\0';
				if(pstr[i]!='\0')
					break;
			}
			splite(mintime,maxtime,pstr);
		}
		mtime& operator =(char* strtime){SetTime(strtime);return *this;}
		mtime& operator =(VR* vr){SetTime(vr);return *this;}

		BOOL operator !=(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)==0)return FALSE;return TRUE;}
		BOOL operator ==(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)==0)return TRUE;return FALSE;}
		BOOL operator !=(int val){if(timebuf[0]=='\0')return FALSE;return TRUE;}
		BOOL operator ==(int val){if(timebuf[0]=='\0')return TRUE;return FALSE;}
		BOOL operator >(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)>0)return TRUE;return FALSE;}
		BOOL operator <(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)<0)return TRUE;return FALSE;}
		BOOL operator >=(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)>=0)return TRUE;return FALSE;}
		BOOL operator <=(mtime othertime){if(memcmp(timebuf,othertime.timebuf,13)<=0)return TRUE;return FALSE;}

		BOOL IsEmpty(){if(timebuf[0]=='\0')return TRUE;return FALSE;}
		BOOL IsNotEmpty(){return !IsEmpty();}

};
class mdatetime
{
public:
	mdate md;
	mtime mt;
};
typedef struct  
{

  public:
      char m_strImageNumber[MAX_VRIS];
      char m_strSOPInstanceUID[MAX_VRUI];
      char m_strImageType[4*MAX_VRCS];
      char m_strImageComment[MAX_VRLT];
      char m_strPhotometric[MAX_VRCS];
      char m_strTransducerData[MAX_VRLO];
      char m_strTransducerType[MAX_VRCS];
      char m_strSpecificCharacterSet[MAX_VRCS*8];
	  char m_Volume[50];
	  char m_InstanceAvailability[20];
}ImageCondition;

typedef struct  
{

  public:
      char m_strModality[MAX_VRCS];
      char m_strSeriesNumber[MAX_VRIS];
      char m_strSeriesInstanceUID[MAX_VRUI];
      char m_strPhysicianName[MAX_VRPN];
      char m_strSeriesDescription[MAX_VRLO];
      char m_strBodyPart[MAX_VRCS];
      char m_strManufacturer[MAX_VRLO];
      char m_strStationName[MAX_VRSH];
      char m_strManufacturerModelName[MAX_VRLO];

}SeriesCondition;


typedef struct  
{

  public:
      mdate m_timMaxStudyDate;
      mtime m_timMaxStudyTime;
      char m_strAccessionNumber[MAX_VRSH];
      char m_strStudyID[MAX_VRSH];
      char m_strStudyInstanceUID[MAX_VRUI];
      char m_strStudyDescription[MAX_VRLO];
      char m_strAdmitDescription[MAX_VRLO];
      mdate m_timMinStudyDate;
      mtime m_timMinStudyTime;
	  char strHospitalNO[20];
}StudyCondition;



typedef struct  
{

  public:
      char m_strPatientName[MAX_VRPN];
      char m_strPatientID[MAX_VRLO];
      char m_PatientSex[MAX_VRCS];
      mdate m_timMinPatientBirthDate;
      mdate m_timMaxPatientBirthDate;
}PatientCondition;

class SeriesInfo;
class StudyInfo;
class PatientInfo;
class ImageInfo 
{

  public:
      void SetIsNeedRetrieve (BOOL bIsNeedRetrieve);
	  ImageInfo()
	  {
			memset((void*)this,0,sizeof(ImageInfo));
			m_bIsNeedRetrieve=FALSE;
	  }
		ImageInfo( const ImageInfo& Info);
		ImageInfo& operator=( const ImageInfo& Info);

		BOOL operator==( const ImageInfo& Info);

      BOOL GetIsNeedRetrieve ();

      void SetImageFileName (char* strFileName);

      char* GetImageFileName ();

	  int nImageID;
      char m_strImageNumber[MAX_VRIS];
	  char m_strSOPClassUID[MAX_VRUI];
      char m_strSOPInstanceUID[MAX_VRUI];
      char m_strImageType[4*MAX_VRCS];
      char m_strImageComment[MAX_VRLT];
      char m_strPhotometric[MAX_VRCS];
      char m_strTransducerData[MAX_VRLO];
      char m_strTransducerType[MAX_VRCS];
      char m_strSpecificCharacterSet[MAX_VRCS*8];
      char m_strFileName[MAX_PATH];
	  char m_Volume[50];
	  char m_InstanceAvailability[20];
      mdate m_timImageDate;
      mtime m_timImageTime;
      char m_strFrameNumber[MAX_VRIS];

	  char m_strPixelSpacing[MAX_VRDS];
	  char m_strImageOrientation[MAX_VRDS*6];//(patient)
	  char m_strImagePosition[MAX_VRDS*6];//(patient)
	  char m_strSliceThickness[MAX_VRDS*2];


	  short m_iImageWidth;
	  short m_iImageHeight;
	  short m_iImageBites;
	  short m_iBitsAllocated;
	  short m_iHighBits;
	  short m_iWinCenter;
	  short m_iWinWidth;
	  BOOL m_bIsNeedRetrieve;

	  SeriesInfo *pParent;
};

class SeriesInfo
{

  public:
	  ImageInfo* GetChild(unsigned int index);
	  ImageInfo* HasThisChild(ImageInfo *pInfo);
	  void Sort();
	  SeriesInfo();
	  ~SeriesInfo()
	  {
		  RemoveArray();
	  }
		SeriesInfo( const SeriesInfo& Info);
		SeriesInfo& operator=( const SeriesInfo& Info);

		BOOL Compare(const SeriesInfo& Info,BOOL bCompareChild=FALSE);
		int GetPosition();
      void SetImageArray (Array<ImageInfo*>* pImageArray);

      void RemoveArray ();
	  
	  int nSeriesID;
      char m_strReferenceFileID[MAX_PATH];
      char m_strModality[MAX_VRCS];
      char m_strSeriesNumber[MAX_VRIS];
      char m_strSeriesInstanceUID[MAX_VRUI];
      char m_strPhysicianName[MAX_VRPN];
      char m_strSeriesDescription[MAX_VRLO];
      char m_strBodyPart[MAX_VRCS];
      char m_strSpecificCharacterSet[MAX_VRCS*8];

	  char m_strFrameofReferenceUID[MAX_VRUI];
	  char m_PositionReferenceIndicator[MAX_VRLO];
   
	  char m_strManufacturer[MAX_VRLO];
      char m_strStationName[MAX_VRSH];
      char m_strManufacturerModelName[MAX_VRLO];

      Array<ImageInfo *> m_ImageInfo;
 
	  StudyInfo *pParent;
};
class StudyInfo
{

  public:
	  SeriesInfo* GetChild(unsigned int index);
	  void Sort();
	  StudyInfo();
	  ~StudyInfo()
	  {
		  RemoveArray();
	  }
		StudyInfo( const StudyInfo& Info);
		StudyInfo& operator=( const StudyInfo& Info);

	  SeriesInfo* HasThisChild(SeriesInfo *pInfo);
		BOOL Compare(const StudyInfo& Info,BOOL bCompareChild=FALSE);
      void SetSeriesArray (Array<SeriesInfo*>* pSeriesArray);

      void RemoveArray ();

      void RemoveSeriesPtr (int nWhichSeries);

      int nStudyID;
	  mdate m_timStudyDate;
      mtime m_timStudyTime;
      char m_strAccessionNumber[MAX_VRSH];
      char m_strReferenceFileID[MAX_PATH];
	  char m_ReferringPhysicianName[MAX_VRPN];
      char m_strStudyID[MAX_VRSH];
      char m_strStudyInstanceUID[MAX_VRUI];
      char m_strStudyDescription[MAX_VRLO];
      char m_strAdmitDescription[MAX_VRLO];
      char m_strSpecificCharacterSet[MAX_VRCS*8];
	  int nStatus;//是否成文
	  char strStudyAge[11];//检查年龄
	  char strHospitalNO[20];
      Array<SeriesInfo *> m_SeriesInfo;

	  PatientInfo *pParent;
};

class PatientInfo
{
  public:
	  StudyInfo* GetChild(unsigned int index);
	  BOOL HasStudyNotWriteReport();
	  void Sort();
	  PatientInfo();
	  ~PatientInfo()
	  {
		  RemoveArray();
	  }
		PatientInfo( const PatientInfo& Info);
		PatientInfo& operator=( const PatientInfo& Info);

	  StudyInfo* HasThisChild(StudyInfo *pInfo);
		BOOL Compare(const PatientInfo& Info,BOOL bCompareChild=FALSE);
      void SetStudyArray (Array<StudyInfo*>* pStudyArray);

      void RemoveArray ();

      void RemoveStudyPtr (int nWhichStudy);

	  char m_strPatientInstanceUID[MAX_VRUI];
      char m_strPatientName[MAX_VRPN];
      char m_strPatientID[MAX_VRLO];
      char m_strReferenceFileID[MAX_PATH];
      mdate m_timPatientBirthDate;
      char m_PatientSex[MAX_VRCS];
      char m_strSpecificCharacterSet[MAX_VRCS*8];
      Array<StudyInfo *> m_StudyInfo;

};
typedef struct
{
      char m_strSpecificCharacterSet[MAX_VRCS*8];
      char m_strPatientName[MAX_VRPN];
      char m_strPatientID[MAX_VRLO];
	  char m_strOtherPatientID[MAX_VRLO];//add for bowei
      char m_strStudyInstanceUID[MAX_VRUI];
	  char strScheduledStationAET[50];
	  char m_timMinScheduledStudyDate[20];
	  char m_timMaxScheduledStudyDate[20];
      char m_timMinScheduledStudyTime[20];
      char m_timMaxScheduledStudyTime[20];
	  char strScheduledProcedureStepID[50];
	  char strRequestedProcedureID[50];

      char m_strAccessionNumber[MAX_VRSH];
      char m_strModality[MAX_VRCS];
	  char m_ReferringPhysicianName[MAX_VRPN];
      char m_timMinPatientBirthDate[20];
      char m_timMaxPatientBirthDate[20];
      char m_PatientSex[MAX_VRCS];
	  char strPathentWeight[10];
	  char strMedicalAlerts[256];
	  char strContrastAllergies[256];
	  char strPregnancyStatus[20];
	  char strRequestingPhysician[MAX_VRPN];
	  char strRequestedContrastAgent[50];
	  char strAdmissionID[20];
	  char strSpecialNeeds[256];
	  char strCurrentPatientLocation[50];
	  char strPatientState[20];
	  char strPerformingPhysician[MAX_VRPN];
	  char strScheduledStationName[50];
	  char strScheduledProcedureStepLocation[50];
	  char strPreOrderMedication[50];
	  char strRequestedProcedurePriority[50];
	  char strPatientTransportArrangements[50];
	  char strConfidentialityConstraint[50];		
}WorkListInfo;
#endif 
