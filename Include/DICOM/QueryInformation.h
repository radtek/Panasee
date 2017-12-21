
#ifndef QueryInformation_h
#define QueryInformation_h 1

#include "dicom.hpp"
#include "MyQueryRetrieve.h"
#include "MyQueryRetrievePDU.h"
#include "QRParam.h"
#include "Query.h"
class CQueryInformation 
{

  public:
      CQueryInformation ();

      ~CQueryInformation ();

	  int CQueryInformation::QueryWithAllTerm(PatientCondition PatientData, StudyCondition StudyData, SeriesCondition SeriesData, ImageCondition ImageData,int nLevel);
      int QueryInformation (PatientCondition PatientData, StudyCondition StudyData, SeriesCondition SeriesData, ImageCondition ImageData,int nLevel);
     
	  int QueryModalityWorkListInformation (WorkListInfo*pWorkList);

      Array<PatientInfo*>* GetPatientArray ();

      Array<StudyInfo*>* GetStudyArray ();

      Array<SeriesInfo*>* GetSeriesArray ();

      Array<ImageInfo*>* GetImageArray ();
	
	  Query*pQuery;
	  char* strDICOMLocalAETitle;
	  char* strDICOMRemoteAETitle;
	  char* strDICOMRemoteIP;
	  char* strRemotePort;

	  Array<WorkListInfo*>* GetWorkListArray();

  protected:

      int Execute (DICOMDataObject *DDO, int nLevelNumber);

      int QueryPatientInfo (PatientCondition PatientData);

      int QueryStudyInfo (StudyCondition StudyData,char* strPatientInfo);

      int QuerySeriesInfo (SeriesCondition SeriesData,char* strPatientInfo,char* strStudyInfo);

      int QueryImageInfo (ImageCondition ImageData,char* strPatientInfo,char* strStudyInfo,char* strSeriesInfo);


  private:
	
	  MyModalityWorklistQuery SOPMyModalityWorklistQuery;

	  MyPatientRootQuery SOPMyPatientRootQuery;
	  MyStudyRootQuery SOPMyStudyRootQuery;
	  
      int m_nLevelNumber;
      Array<PatientInfo*>* m_pPatientArray;
      Array<StudyInfo*>* m_pStudyArray;
      Array<SeriesInfo*>* m_pSeriesArray;
      Array<ImageInfo*>* m_pImageArray;

};
#endif
