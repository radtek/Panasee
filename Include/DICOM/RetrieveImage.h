#ifndef RetrieveImage_h
#define RetrieveImage_h 1
#include "Query.h"
class CRetrieveImage 
{

  public:
      CRetrieveImage ();
      ~CRetrieveImage ();

	  int Retrieve (char* strDICOMLocalAETitle,char*strDICOMRemoteAETitle,\
					 char*strDICOMRemoteIP,char*strRemotePort/*void(*pFun)(int,int)*/);
      void SetRetrieveImage (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, BOOL bIsNeedRetrieve);
      void GetTotalImageNumber ();
      void SetRetrieveImageFileName (int nPatientNumber, int nStudyNumber, int nSeriesNumber, int nImageNumber, char* strFileName);

	  Query*pQuery;
  private:
      int m_nTotalImageNumber;
      int m_nCurrentImageNumber;

};
#endif
