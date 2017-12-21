#include "Query.h"



Query::Query ()
{
}


Query::~Query ()
{
	RemoveAllArray();
}
void Query::SetArray(Array<PatientInfo*>* pPatientArray)
{
	RemoveAllArray();

	unsigned int i;
	PatientInfo * pPatient;
	for (i = 0; i <= pPatientArray->GetSize()-1; i++)
	{
		pPatient = new PatientInfo;
		*pPatient=*pPatientArray->Get(i);
		m_PatientInfo.Add(pPatient);
	}
}

void Query::GetQueryArray (int nWhichPatient, int nWhichStudy, int nWhichSeries, Array<ImageInfo*>* pImageArray)
{
	m_PatientInfo.Get(nWhichPatient)->m_StudyInfo.Get(nWhichStudy)->m_SeriesInfo.Get(nWhichSeries)->RemoveArray();
	m_PatientInfo.Get(nWhichPatient)->m_StudyInfo.Get(nWhichStudy)->m_SeriesInfo.Get(nWhichSeries)->SetImageArray(pImageArray);
}

Array<PatientInfo*> * Query::GetQueryArray ()
{
	return &m_PatientInfo;
}


void Query::SetRetrieveImage (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, BOOL bIsNeedRetrieve)
{
	m_PatientInfo.Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->SetIsNeedRetrieve(bIsNeedRetrieve);
}

void Query::SetImageArray (int nWhichPatient, int nWhichStudy, int nWhichSeries, Array<ImageInfo*>* pImageArray)
{
	m_PatientInfo.Get(nWhichPatient)->m_StudyInfo.Get(nWhichStudy)->m_SeriesInfo.Get(nWhichSeries)->RemoveArray();
	m_PatientInfo.Get(nWhichPatient)->m_StudyInfo.Get(nWhichStudy)->m_SeriesInfo.Get(nWhichSeries)->SetImageArray(pImageArray);
}

void Query::RemoveAllArray ()
{
	while(m_PatientInfo.GetSize()-1 != -1)
	{
		m_PatientInfo.Get(m_PatientInfo.GetSize()-1)->RemoveArray();
		delete m_PatientInfo.Get(m_PatientInfo.GetSize()-1);
		m_PatientInfo.RemoveAt(m_PatientInfo.GetSize()-1);
	}
}

void Query::SetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, char* strFileName)
{
	m_PatientInfo.Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->SetImageFileName(strFileName);
}

char* Query::GetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber)
{
	return m_PatientInfo.Get(nPatientNumber)->m_StudyInfo.Get(nStudyNumber)->m_SeriesInfo.Get(nSeriesNumber)->m_ImageInfo.Get(nImageNumber)->GetImageFileName();
}

