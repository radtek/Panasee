#include "QRParam.h"

//截断尾部空格
void trimcpy(char* str,void*pData,int len)
{
	char*pBuf=new char[len+1];
	memset(pBuf,0,len+1);
	memcpy(pBuf,pData,len);
	for(int i=len-1;i>=0;i--)
	{
		if(pBuf[i]=='\0'||pBuf[i]==' ')
			pBuf[i]='\0';
		else
			break;
	}
	strcpy(str,pBuf);
	delete pBuf;
}

PatientInfo::PatientInfo( const PatientInfo& Info)
{
}
StudyInfo::StudyInfo( const StudyInfo& Info)
{
}
SeriesInfo::SeriesInfo( const SeriesInfo& Info)
{
}
ImageInfo::ImageInfo( const ImageInfo& Info)
{
}

//Copy function
PatientInfo& PatientInfo::operator=( const PatientInfo& Info)
{
//Copy Member variable
	strcpy(m_strPatientInstanceUID,Info.m_strPatientInstanceUID);
	strcpy(m_strPatientName,Info.m_strPatientName);
	strcpy(m_strPatientID,Info.m_strPatientID);
	strcpy(m_strReferenceFileID,Info.m_strReferenceFileID);
	strcpy(m_PatientSex,Info.m_PatientSex);
	strcpy(m_strSpecificCharacterSet,Info.m_strSpecificCharacterSet);
	m_timPatientBirthDate=Info.m_timPatientBirthDate;
//调用了RemoveArray(),会清除原用的内存
	RemoveArray();
	StudyInfo *pStudyInfo;
//Copy StudyInfoArray
	for(unsigned int i=0;i<((Array<class StudyInfo *> &)Info.m_StudyInfo).GetSize();i++)
	{
		pStudyInfo=new StudyInfo;
		*pStudyInfo=*((Array<class StudyInfo *> &)Info.m_StudyInfo).Get(i);
		//Add to this patientinfo
		pStudyInfo->pParent=this;
		this->m_StudyInfo.Add(pStudyInfo);
	}
	return *this;
}

//查询ptientinfo中是否有此studyinfo
//by StudyInstanceUID
StudyInfo* PatientInfo::HasThisChild(StudyInfo *pInfo)
{
	for(unsigned int i=0;i<m_StudyInfo.GetSize();i++)
		if(!strcmp(m_StudyInfo.Get(i)->m_strStudyInstanceUID,pInfo->m_strStudyInstanceUID))
			return m_StudyInfo.Get(i);
	return NULL;
}

BOOL PatientInfo::Compare(const PatientInfo& Info,BOOL bCompareChild)
{
	if(!strlen(Info.m_strPatientID)||!strlen(this->m_strPatientID)||strcmp(Info.m_strPatientID,this->m_strPatientID))
		return FALSE;
	if(bCompareChild)
	{
		if(((Array<class StudyInfo *> &)Info.m_StudyInfo).GetSize()!=m_StudyInfo.GetSize())
			return FALSE;
		StudyInfo *pStudyInfo;
		for(unsigned int i=0;i<((Array<class StudyInfo *> &)Info.m_StudyInfo).GetSize();i++)
		{
			pStudyInfo=HasThisChild(((Array<class StudyInfo *> &)Info.m_StudyInfo).Get(i));
			if(pStudyInfo==NULL)
				return FALSE;
			if(!pStudyInfo->Compare( *(((Array<class StudyInfo *> &)Info.m_StudyInfo).Get(i)) ,TRUE) )
				return FALSE;
		}
	}
	return TRUE;
}

//获得PatientInfo使用时
//在合适的地方排序！！！
void PatientInfo::Sort()
{
	StudyInfo* pStudyInfo;
	int nIndex;

	//冒泡法排序
	for(int i=0;i<(int)this->m_StudyInfo.GetSize()-1;i++)
	{
		nIndex=i;
		for(int j=i+1;j<(int)this->m_StudyInfo.GetSize();j++)
			if(this->m_StudyInfo.Get(nIndex)->m_strStudyID&&this->m_StudyInfo.Get(j)->m_strStudyID)
				if(strcmp(this->m_StudyInfo.Get(nIndex)->m_strStudyID,this->m_StudyInfo.Get(j)->m_strStudyID)>0)
					nIndex=j;
		if(nIndex!=i)
		{
			pStudyInfo=this->m_StudyInfo.Get(i);
			this->m_StudyInfo[i]=this->m_StudyInfo[nIndex];
			this->m_StudyInfo[nIndex]=pStudyInfo;
		}
	}
	for(i=0;i<(int)this->m_StudyInfo.GetSize();i++)
		this->m_StudyInfo[i]->Sort();
}

void PatientInfo::SetStudyArray (Array<StudyInfo*>* pStudyArray)
{
	unsigned int i;
	StudyInfo * pStudy;
	for (i =0 ; i <= pStudyArray->GetSize()-1 ; i++)
	{
		pStudy = new StudyInfo;
		strcpy(pStudy->m_strAccessionNumber,pStudyArray->Get(i)->m_strAccessionNumber);
		strcpy(pStudy->m_strAdmitDescription,pStudyArray->Get(i)->m_strAdmitDescription);
		pStudy->m_timStudyDate=pStudyArray->Get(i)->m_timStudyDate;
		strcpy(pStudy->m_strStudyDescription,pStudyArray->Get(i)->m_strStudyDescription);
		strcpy(pStudy->m_strStudyID,pStudyArray->Get(i)->m_strStudyID);
		strcpy(pStudy->m_strStudyInstanceUID,pStudyArray->Get(i)->m_strStudyInstanceUID);
		pStudy->m_timStudyTime=pStudyArray->Get(i)->m_timStudyTime;
		pStudy->nStatus=pStudyArray->Get(i)->nStatus;
		strcpy(pStudy->strStudyAge,pStudyArray->Get(i)->strStudyAge);
		m_StudyInfo.Add(pStudy);
	}
}

void PatientInfo::RemoveArray ()
{
	while(m_StudyInfo.GetSize()-1 != -1)
	{
		m_StudyInfo.Get(m_StudyInfo.GetSize()-1)->RemoveArray();
		delete m_StudyInfo.Get(m_StudyInfo.GetSize()-1);
		m_StudyInfo.RemoveAt(m_StudyInfo.GetSize()-1);
	}
}

void PatientInfo::RemoveStudyPtr (int nWhichStudy)
{
	delete m_StudyInfo.Get(nWhichStudy);
	m_StudyInfo.RemoveAt(nWhichStudy);
}

StudyInfo& StudyInfo::operator=( const StudyInfo& Info)
{
	m_timStudyDate=Info.m_timStudyDate;
	m_timStudyTime=Info.m_timStudyTime;
	strcpy(m_strAccessionNumber,Info.m_strAccessionNumber);
	strcpy(m_strReferenceFileID,Info.m_strReferenceFileID);
	strcpy(m_ReferringPhysicianName,Info.m_ReferringPhysicianName);
	strcpy(m_strStudyID,Info.m_strStudyID);
	strcpy(m_strStudyInstanceUID,Info.m_strStudyInstanceUID);
	strcpy(m_strStudyDescription,Info.m_strStudyDescription);
	strcpy(m_strAdmitDescription,Info.m_strAdmitDescription);
	strcpy(m_strSpecificCharacterSet,Info.m_strSpecificCharacterSet);
	nStudyID=Info.nStudyID;
	nStatus=Info.nStatus;
	strcpy(strStudyAge,Info.strStudyAge);
	strcpy(strHospitalNO,Info.strHospitalNO);
	RemoveArray();
	SeriesInfo *pSeriesInfo;
	for(unsigned int i=0;i<((Array<class SeriesInfo *> &)Info.m_SeriesInfo).GetSize();i++)
	{
		pSeriesInfo=new SeriesInfo;
		*pSeriesInfo=*((Array<class SeriesInfo *> &)Info.m_SeriesInfo).Get(i);
		pSeriesInfo->pParent=this;
		this->m_SeriesInfo.Add(pSeriesInfo);
	}
	return *this;
}

SeriesInfo* StudyInfo::HasThisChild(SeriesInfo *pInfo)
{
	for(unsigned int i=0;i<m_SeriesInfo.GetSize();i++)
		if(!strcmp(m_SeriesInfo.Get(i)->m_strSeriesInstanceUID,pInfo->m_strSeriesInstanceUID))
			return m_SeriesInfo.Get(i);
	return NULL;
}

BOOL StudyInfo::Compare(const StudyInfo& Info,BOOL bCompareChild)
{
	if(!strlen(Info.m_strStudyInstanceUID)||!strlen(this->m_strStudyInstanceUID)||strcmp(Info.m_strStudyInstanceUID,this->m_strStudyInstanceUID))
		return FALSE;
	if(bCompareChild)
	{
		if(((Array<class SeriesInfo *> &)Info.m_SeriesInfo).GetSize()!=m_SeriesInfo.GetSize())
			return FALSE;
		SeriesInfo *pSeriesInfo;
		for(unsigned int i=0;i<((Array<class SeriesInfo *> &)Info.m_SeriesInfo).GetSize();i++)
		{
			pSeriesInfo=HasThisChild(((Array<class SeriesInfo *> &)Info.m_SeriesInfo).Get(i));
			if(pSeriesInfo==NULL)
				return FALSE;
			if(!pSeriesInfo->Compare( *(((Array<class SeriesInfo *> &)Info.m_SeriesInfo).Get(i)) ,TRUE) )
				return FALSE;
		}
	}
	return TRUE;
}

void StudyInfo::Sort()
{
	SeriesInfo* pSeriesInfo;
	int nIndex;

	//冒泡法排序
	for(int i=0;i<(int)this->m_SeriesInfo.GetSize()-1;i++)
	{
		nIndex=i;
		for(int j=i+1;j<(int)this->m_SeriesInfo.GetSize();j++)
			if(this->m_SeriesInfo.Get(nIndex)->m_strSeriesNumber&&this->m_SeriesInfo.Get(j)->m_strSeriesNumber)
				if(atoi(this->m_SeriesInfo.Get(nIndex)->m_strSeriesNumber)>atoi(this->m_SeriesInfo.Get(j)->m_strSeriesNumber))
					nIndex=j;
		if(nIndex!=i)
		{
			pSeriesInfo=this->m_SeriesInfo.Get(i);
			this->m_SeriesInfo[i]=this->m_SeriesInfo[nIndex];
			this->m_SeriesInfo[nIndex]=pSeriesInfo;
		}
	}
	for(i=0;i<(int)this->m_SeriesInfo.GetSize();i++)
		this->m_SeriesInfo[i]->Sort();
}

void StudyInfo::SetSeriesArray (Array<SeriesInfo*>* pSeriesArray)
{
	unsigned int i;
	SeriesInfo * pSeries;
	for (i = 0; i <= pSeriesArray->GetSize()-1 ; i++)
	{
		pSeries = new SeriesInfo;
		strcpy(pSeries->m_strBodyPart,pSeriesArray->Get(i)->m_strBodyPart);
		strcpy(pSeries->m_strManufacturer,pSeriesArray->Get(i)->m_strManufacturer);
		strcpy(pSeries->m_strManufacturerModelName,pSeriesArray->Get(i)->m_strManufacturerModelName);
		strcpy(pSeries->m_strModality,pSeriesArray->Get(i)->m_strModality);
		strcpy(pSeries->m_strPhysicianName,pSeriesArray->Get(i)->m_strPhysicianName);
		strcpy(pSeries->m_strSeriesDescription,pSeriesArray->Get(i)->m_strSeriesDescription);
		strcpy(pSeries->m_strSeriesInstanceUID,pSeriesArray->Get(i)->m_strSeriesInstanceUID);
		strcpy(pSeries->m_strSeriesNumber,pSeriesArray->Get(i)->m_strSeriesNumber);
		strcpy(pSeries->m_strStationName,pSeriesArray->Get(i)->m_strStationName);
		m_SeriesInfo.Add(pSeries);
	}
}

void StudyInfo::RemoveArray ()
{
	while(m_SeriesInfo.GetSize()-1 != -1)
	{
		m_SeriesInfo.Get(m_SeriesInfo.GetSize()-1)->RemoveArray();
		delete m_SeriesInfo.Get(m_SeriesInfo.GetSize()-1);
		m_SeriesInfo.RemoveAt(m_SeriesInfo.GetSize()-1);
	}
}

void StudyInfo::RemoveSeriesPtr (int nWhichSeries)
{
	delete m_SeriesInfo.Get(nWhichSeries);
	m_SeriesInfo.RemoveAt(nWhichSeries);
}

SeriesInfo& SeriesInfo::operator=( const SeriesInfo& Info)
{
	strcpy(m_strReferenceFileID,Info.m_strReferenceFileID);
	strcpy(m_strModality,Info.m_strModality);
	strcpy(m_strSeriesNumber,Info.m_strSeriesNumber);
	strcpy(m_strSeriesInstanceUID,Info.m_strSeriesInstanceUID);
	strcpy(m_strPhysicianName,Info.m_strPhysicianName);
	strcpy(m_strSeriesDescription,Info.m_strSeriesDescription);
	strcpy(m_strBodyPart,Info.m_strBodyPart);
	strcpy(m_strSpecificCharacterSet,Info.m_strSpecificCharacterSet);

	strcpy(m_strFrameofReferenceUID,Info.m_strFrameofReferenceUID);
	strcpy(m_PositionReferenceIndicator,Info.m_PositionReferenceIndicator);

	strcpy(m_strManufacturer,Info.m_strManufacturer);
	strcpy(m_strStationName,Info.m_strStationName);
	strcpy(m_strManufacturerModelName,Info.m_strManufacturerModelName);
	nSeriesID=Info.nSeriesID;

	RemoveArray();
	ImageInfo *pImageInfo;
	for(unsigned int i=0;i<((Array<class ImageInfo *> &)Info.m_ImageInfo).GetSize();i++)
	{
		pImageInfo=new ImageInfo;
		*pImageInfo=*((Array<class ImageInfo *> &)Info.m_ImageInfo).Get(i);
		this->m_ImageInfo.Add(pImageInfo);
		pImageInfo->pParent=this;
	}
	return *this;
}

ImageInfo* SeriesInfo::HasThisChild(ImageInfo *pInfo)
{
	for(unsigned int i=0;i<m_ImageInfo.GetSize();i++)
		if(!strcmp(m_ImageInfo.Get(i)->m_strSOPInstanceUID,pInfo->m_strSOPInstanceUID))
			return m_ImageInfo.Get(i);
	return NULL;
}

BOOL SeriesInfo::Compare(const SeriesInfo& Info,BOOL bCompareChild)
{
	if(!strlen(Info.m_strSeriesInstanceUID)||!strlen(this->m_strSeriesInstanceUID)||strcmp(Info.m_strSeriesInstanceUID,this->m_strSeriesInstanceUID))
		return FALSE;
	if(bCompareChild)
	{
		if(((Array<class ImageInfo *> &)Info.m_ImageInfo).GetSize()!=m_ImageInfo.GetSize())
			return FALSE;
		ImageInfo *pImageInfo;
		for(unsigned int i=0;i<((Array<class ImageInfo *> &)Info.m_ImageInfo).GetSize();i++)
		{
			pImageInfo=HasThisChild(((Array<class ImageInfo *> &)Info.m_ImageInfo).Get(i));
			if(pImageInfo==NULL)
				return FALSE;
			if(!(*pImageInfo==*((Array<class ImageInfo *> &)Info.m_ImageInfo).Get(i)))
				return FALSE;
		}
	}
	return TRUE;
}

int SeriesInfo::GetPosition()
{
	if(pParent)
	{
		for(int i=0;i<(int)pParent->m_SeriesInfo.GetSize();i++)
		{
			if(this->Compare(*(pParent->m_SeriesInfo.Get(i))))
				return i;
		}
	}
	return -1;
}

void SeriesInfo::Sort()
{
	ImageInfo* pImageInfo;
	int nIndex;

	//冒泡法排序
	for(int i=0;i<(int)this->m_ImageInfo.GetSize()-1;i++)
	{
		nIndex=i;
		for(int j=i+1;j<(int)this->m_ImageInfo.GetSize();j++)
			if(this->m_ImageInfo.Get(nIndex)->m_strImageNumber&&this->m_ImageInfo.Get(j)->m_strImageNumber)
				if(atoi(this->m_ImageInfo.Get(nIndex)->m_strImageNumber)>atoi(this->m_ImageInfo.Get(j)->m_strImageNumber))
					nIndex=j;
		if(nIndex!=i)
		{
			pImageInfo=this->m_ImageInfo.Get(i);
			this->m_ImageInfo[i]=this->m_ImageInfo[nIndex];
			this->m_ImageInfo[nIndex]=pImageInfo;
		}
	}
}

void SeriesInfo::SetImageArray (Array<ImageInfo*>* pImageArray)
{
	unsigned int i;
	ImageInfo * pImage;
	for (i = 0 ; i <= pImageArray->GetSize()-1; i++)
	{
		pImage = new ImageInfo;
		*pImage=*(pImageArray->Get(i));
		m_ImageInfo.Add(pImage);
	}
}

void SeriesInfo::RemoveArray ()
{
	while(m_ImageInfo.GetSize()-1 != -1)
	{
		delete m_ImageInfo.Get(m_ImageInfo.GetSize()-1);
		m_ImageInfo.RemoveAt(m_ImageInfo.GetSize()-1);
	}
}


ImageInfo& ImageInfo::operator=( const ImageInfo& Info)
{
      strcpy(m_strImageNumber,Info.m_strImageNumber);
	  strcpy(m_strSOPClassUID,Info.m_strSOPClassUID);
      strcpy(m_strSOPInstanceUID,Info.m_strSOPInstanceUID);
      strcpy(m_strImageType,Info.m_strImageType);
      strcpy(m_strImageComment,Info.m_strImageComment);
      strcpy(m_strPhotometric,Info.m_strPhotometric);
      strcpy(m_strTransducerData,Info.m_strTransducerData);
      strcpy(m_strTransducerType,Info.m_strTransducerType);
      strcpy(m_strSpecificCharacterSet,Info.m_strSpecificCharacterSet);
      strcpy(m_strFileName,Info.m_strFileName);
      strcpy(m_Volume,Info.m_Volume);
      strcpy(m_InstanceAvailability,Info.m_InstanceAvailability);
      m_timImageDate=Info.m_timImageDate;
      m_timImageTime=Info.m_timImageTime;
      strcpy(m_strFrameNumber,Info.m_strFrameNumber);
	  strcpy(m_strPixelSpacing,Info.m_strPixelSpacing);
	  strcpy(m_strImageOrientation,Info.m_strImageOrientation);
	  strcpy(m_strImagePosition,Info.m_strImagePosition);
	  strcpy(m_strSliceThickness,Info.m_strSliceThickness);
	  m_iImageWidth=Info.m_iImageWidth;
	  m_iImageHeight=Info.m_iImageHeight;
	  m_iImageBites=Info.m_iImageBites;
	  m_iBitsAllocated=Info.m_iBitsAllocated;
	  m_iHighBits=Info.m_iHighBits;
	  m_iWinCenter=Info.m_iWinCenter;
	  m_iWinWidth=Info.m_iWinWidth;
	  m_bIsNeedRetrieve=Info.m_bIsNeedRetrieve;
	  nImageID=Info.nImageID;
	  return *this;
}

BOOL ImageInfo::operator==( const ImageInfo& Info)
{
	if(strcmp(Info.m_strSOPInstanceUID,this->m_strSOPInstanceUID))
		return FALSE;
	return TRUE;
}

void ImageInfo::SetIsNeedRetrieve (BOOL bIsNeedRetrieve)
{
	m_bIsNeedRetrieve = bIsNeedRetrieve;
}

BOOL ImageInfo::GetIsNeedRetrieve ()
{
	return m_bIsNeedRetrieve;
}

void ImageInfo::SetImageFileName (char* strFileName)
{
	strcpy(m_strFileName,strFileName);
}

char* ImageInfo::GetImageFileName ()
{
	return m_strFileName;
}

PatientInfo::PatientInfo()
{
	memset((void*)this,0,sizeof(PatientInfo));
}

StudyInfo::StudyInfo()
{
	memset((void*)this,0,sizeof(StudyInfo));
	nStatus=1;//假定都是成文的，不用特别标出。。。
}

SeriesInfo::SeriesInfo()
{
	memset((void*)this,0,sizeof(SeriesInfo));
}

StudyInfo* PatientInfo::GetChild(unsigned int index)
{
	if(m_StudyInfo.GetSize()<=index)
		return NULL;
	else
		return m_StudyInfo.Get(index);
}

SeriesInfo* StudyInfo::GetChild(unsigned int index)
{
	if(m_SeriesInfo.GetSize()<=index)
		return NULL;
	else
		return m_SeriesInfo.Get(index);
}

ImageInfo* SeriesInfo::GetChild(unsigned int index)
{
	if(m_ImageInfo.GetSize()<=index)
		return NULL;
	else
		return m_ImageInfo.Get(index);
}

BOOL PatientInfo::HasStudyNotWriteReport()
{
	for(unsigned int i=0;i<m_StudyInfo.GetSize();i++)
		if(m_StudyInfo.Get(i)->nStatus==0)
			return TRUE;
	return FALSE;
}