
#ifndef Query_h
#define Query_h 1

#include "QrParam.h"
class Query 
{

  public:
	  void SetArray(Array<PatientInfo*>* pPatientArray);

      void GetQueryArray (int nWhichPatient, int nWhichStudy, int nWhichSeries, Array<ImageInfo*>* pImageArray);

      Array<PatientInfo*> * GetQueryArray ();


      void SetRetrieveImage (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, BOOL bIsNeedRetrieve);

      void SetImageArray (int nWhichPatient, int nWhichStudy, int nWhichSeries, Array<ImageInfo*>* pImageArray);

      void RemoveAllArray ();

      void SetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, char* strFileName);

      char* GetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber);

      Array<PatientInfo *> m_PatientInfo;


      Query ();

      ~Query ();
  private:
//      static HANDLE s_hQuery;
};
#endif
