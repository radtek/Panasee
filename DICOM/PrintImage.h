#ifndef PrintImage_h
#define PrintImage_h 1

#include "dicom.hpp"
#include "PrnParam.h"

#pragma warning(disable : 4786) // Identifier was truncated to '255' characters in the debug information

class MyGrayscalePrintManagement : public BasicGrayscalePrintManagementMeta
{
  protected:
      virtual BOOL ActionPrint (AbstractFilmSession *AFS, DICOMDataObject *DDO);

      virtual BOOL ActionPrint (AbstractFilmBox *AFB, DICOMDataObject *  DDO);

      virtual void HandlePrinterEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg);

      virtual void HandlePrintJobEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg);

      virtual int HandleError (const ErrorDescription &theError) const;

};

class MyColorPrintManagement : public BasicColorPrintManagementMeta
{

  public:
      virtual int HandleError (const ErrorDescription &theError) const;

  private:

      virtual BOOL ActionPrint (AbstractFilmSession *AFS, DICOMDataObject *DDO);

      virtual BOOL ActionPrint (AbstractFilmBox *AFB, DICOMDataObject *  DDO);

      virtual void HandlePrinterEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg);

      virtual void HandlePrintJobEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg);

};


#endif
