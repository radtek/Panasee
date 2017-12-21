
#ifndef MyQueryRetrieve_h
#define MyQueryRetrieve_h 1

#include "dicom.hpp"
#include "QRParam.h"

class MyPatientRootQuery : public PatientRootQuery  //## Inherits: <unnamed>%396C4C8902A1
{

  public:
      MyPatientRootQuery ();

      ~MyPatientRootQuery ();


      virtual BOOL CallBack (DICOMCommandObject* DCO, DICOMDataObject* DDO);

      Array<PatientInfo*>* GetPatientArray ();


      void RemoveArray ();

      virtual BOOL SearchOn (DICOMDataObject *DDO, Array<DICOMDataObject *> *arrayDDO);

      BOOL uGetUID (UID &uid);

      void SetLevelNumber (int nLevelNumber);

  private:
      int m_nLevelNumber;
      Array<PatientInfo*> m_PatientArray;
};


class MyStudyRootQuery : public StudyRootQuery  //## Inherits: <unnamed>%396C4EDC03DD
{

  public:
      MyStudyRootQuery ();

      ~MyStudyRootQuery ();


      virtual BOOL CallBack (DICOMCommandObject* DCO, DICOMDataObject*DDO );

      Array<PatientInfo*>* GetPatientArray ();


      void RemoveArray ();

      virtual BOOL SearchOn (DICOMDataObject *DDO, Array<DICOMDataObject *> *arrayDDO);

      BOOL uGetUID (UID &uid);

      void SetLevelNumber (int nLevelNumber);


  private:
      int m_nLevelNumber;
      Array<PatientInfo*> m_PatientArray;

};
class	MyStudyRootRetrieve	:public	StudyRootRetrieve
{
	public:
		RunTimeClassStorage RTCStorage;
	public:
		int nQueueID;
		UINT nMsg;
		MyStudyRootRetrieve();
		BOOL	SearchOn   (DICOMDataObject	*DDO,
							Array < DICOMDataObject *> *ADDO);
		BOOL	CallBack   (DICOMCommandObject	*DCO,
							DICOMDataObject	*DDO);
		BOOL	QualifyOn  (BYTE *, BYTE *, BYTE *, BYTE *);
		BOOL	RetrieveOn (DICOMDataObject *,
							DICOMDataObject **,
							StandardStorage	** );
private:
	BOOL bSwap;
};
class	MyPatientRootRetrieve : public PatientRootRetrieve
{
	public:
		RunTimeClassStorage RTCStorage;
	public:
		int nQueueID;
		UINT nMsg;
		MyPatientRootRetrieve();
		BOOL	SearchOn   (DICOMDataObject	*DDO,
							Array < DICOMDataObject *> *ADDO);
		BOOL	CallBack   (DICOMCommandObject	*DCO,
							DICOMDataObject	*DDO);
		BOOL	QualifyOn  (BYTE *, BYTE *, BYTE *, BYTE *);
		BOOL	RetrieveOn (DICOMDataObject *,
							DICOMDataObject **,
							StandardStorage	** );
private:
	BOOL bSwap;
};
class MyModalityWorklistQuery : public ModalityWorklistQuery  //## Inherits: <unnamed>%396C4C8902A1
{

  public:
	MyModalityWorklistQuery ();

	~MyModalityWorklistQuery ();


	virtual BOOL CallBack (DICOMCommandObject* DCO, DICOMDataObject* DDO);


	BOOL uGetUID (UID &uid);
	BOOL RemoveArray ();
	Array<WorkListInfo*>*pGlobalWorkListArray;
};

#endif
