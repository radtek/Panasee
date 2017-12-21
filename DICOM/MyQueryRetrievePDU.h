#ifndef MyQueryRetrievePDU_h
#define MyQueryRetrievePDU_h 1

#include "dicom.hpp"

class MyQueryRetrievePDU : public PDU_Service  //## Inherits: <unnamed>%396C277D027C
{

  public:

      BOOL GetAcceptedTransferSyntax (UID &uid);

      void SetQueryOrRetrieve (BOOL bIsQuery);

  protected:

      BOOL AddTransferSyntaxs (PresentationContext& PresContext);


  private:
      BOOL m_bIsQuery;

};
#endif
