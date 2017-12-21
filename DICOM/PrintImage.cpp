#include <time.h>
#include "PrintImage.h"

BOOL CPrintConfig::GetUID(UID& uid)
{
	uid.Set("1.2.840.10008.5.1.1.16.376");
	return TRUE;
}
BOOL CPrintConfig::GetConfiguration( PDU_Service *pPDU,DICOMCommandObject*pDCO,DICOMDataObject *pDDO)
{
	UID uid("1.2.840.10008.5.1.1.17.376");
	pDDO->Reset();
	VR*vr=new VR(0x2000,0x001E,0,"",FALSE);
	pDDO->Push(vr);
	NGetRQ::Write(pPDU,pDDO,&uid);
	if (!pPDU->Read (pDCO ) )
		return ( FALSE );
	NGetRSP::Read(pDCO,pPDU,pDDO);
	return TRUE;
}
BOOL CPrintQueue::GetUID(UID& uid)
{
	uid.Set("1.2.840.10008.5.1.1.26");
	return TRUE;
}
BOOL CPrintQueue::GetQueueInfo( PDU_Service *pPDU,DICOMCommandObject*pDCO,DICOMDataObject *pDDO)
{
	UID uid("1.2.840.10008.5.1.1.25");
	pDDO->Reset();
//	VR*vr=new VR(0x2000,0x001E,0,"",FALSE);
//	pDDO->Push(vr);
	NGetRQ::Write(pPDU,pDDO,&uid);
	if (!pPDU->Read (pDCO ) )
		return ( FALSE );
	NGetRSP::Read(pDCO,pPDU,pDDO);
//	FILE*pF=fopen("c:\\dicom.txt","wt");
//	DumpDDO(pF,pDDO,5,pPDU->AttachedRTC);
//	fclose(pF);
	return TRUE;
}

BOOL MyGrayscalePrintManagement::ActionPrint (AbstractFilmSession *AFS, DICOMDataObject *DDO)
{
	return FALSE;
}

BOOL MyGrayscalePrintManagement::ActionPrint (AbstractFilmBox *AFB, DICOMDataObject *  DDO)
{
	return FALSE;
}

void MyGrayscalePrintManagement::HandlePrinterEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg)
{
	return;
}

void MyGrayscalePrintManagement::HandlePrintJobEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg)
{
	return;
}

int MyGrayscalePrintManagement::HandleError (const ErrorDescription &theError) const
{
	string strErrorMsg;
	strErrorMsg="打印机返回错误！code:";
	char strStatus[256];
	sprintf(strStatus,"%d",theError.GetDICOMStatus());
	strErrorMsg+=strStatus;
	strErrorMsg+="msg:";
	strErrorMsg+=theError.GetMessage();

	MessageBox(NULL,strErrorMsg.c_str(),"DICOMERROR",MB_OK);
	return TRUE;
/*
#ifdef DEBUG

	strErrorMsg = "HandleError: ";
	strErrorMsg += theError.GetFile();
	strErrorMsg += ": (";
	char strline[20];
	_itoa(theError.GetLine(), strline, 10);
	strErrorMsg += strline;
	strErrorMsg += ") ";
	strErrorMsg += theError.GetClass();
	strErrorMsg += " :: ";
	strErrorMsg += theError.GetMethod();
	strErrorMsg += " Msg = ";
	strErrorMsg += theError.GetMessage();

  strErrorMsg="打印机返回错误！code:";
	char strStatus[256];
	sprintf(strStatus,"%d",theError.GetDICOMStatus());
	strErrorMsg+=strStatus;
	strErrorMsg+="msg:";
	strErrorMsg+=theError.GetMessage();

	MessageBox(NULL,strErrorMsg.c_str(),"DICOMERROR",MB_OK);
	return TRUE;
	
#endif
*/	switch(theError.GetErrorType()) {
	case ErrorDescription::InvalidArgument:
		return FALSE;
		break;

//		NoError,
//		InvalidArgument,
//		PDUReadError,
//		PDUWriteError,
//		UnexpectedDIMSENResponse,
//		DIMSENWriteError,
//		DIMSENReadError,
//		DIMSENWarningStatus,
//		DIMSENErrorStatus,
//		UnexpectedDIMSENStatus

	default:
	return FALSE;
		return TRUE;
	}
					
	return ( TRUE );
}



BOOL MyColorPrintManagement::ActionPrint (AbstractFilmSession *AFS, DICOMDataObject *DDO)
{
	return FALSE;
}

BOOL MyColorPrintManagement::ActionPrint (AbstractFilmBox *AFB, DICOMDataObject *  DDO)
{
	return FALSE;
}

void MyColorPrintManagement::HandlePrinterEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg)
{
	return;
}

int MyColorPrintManagement::HandleError (const ErrorDescription &theError) const
{
	string strErrorMsg;

#ifdef DEBUG

	strErrorMsg = "HandleError: ";
	strErrorMsg += theError.GetFile();
	strErrorMsg += ": (";
	char strline[20];
	_itoa(theError.GetLine(), strline, 10);
	strErrorMsg += strline;
	strErrorMsg += ") ";
	strErrorMsg += theError.GetClass();
	strErrorMsg += " :: ";
	strErrorMsg += theError.GetMethod();
	strErrorMsg += " Msg = ";
	strErrorMsg += theError.GetMessage();

	return FALSE;
	
#endif
	switch(theError.GetErrorType()) {
	case ErrorDescription::InvalidArgument:
		return FALSE;
		break;

//		NoError,
//		InvalidArgument,
//		PDUReadError,
//		PDUWriteError,
//		UnexpectedDIMSENResponse,
//		DIMSENWriteError,
//		DIMSENReadError,
//		DIMSENWarningStatus,
//		DIMSENErrorStatus,
//		UnexpectedDIMSENStatus

	default:
		return FALSE;
		return TRUE;
	}
					
	return ( TRUE );
}

void MyColorPrintManagement::HandlePrintJobEvent (DICOMDataObject *theDDO, UID &theUID, UINT16 theEventTypeID, void *theArg)
{
	return;
}
