#ifndef __DICOMMESSAGE_H__
#define __DICOMMESSAGE_H__

/*enum SURGERY_DCM_MESSAGE
{
	SURGERY_DCM_START_DICOM,
	SURGERY_DCM_STOP_DICOM,
	SURGERY_DCM_RECEIVE_SERIES,
	SURGERY_DCM_LOG_MESSAGE
};*/

enum STORAGE_DCM_MESSAGE
{
	STORAGE_FAULT_MULTIPLEX,     //"DICOM ������","multiplex����"
	STORAGE_FAULT_PDUREAD,       //"DICOM ������","pduread����"
	STORAGE_FAULT_NEW,           //"DICOM ������","new����"
	STORAGE_FAULT_CT_SAVE,       //"DICOM ������","���ر���CT Dicomͼ��ʧ��"
	STORAGE_FAULT_MR_SAVE,       //"DICOM ������","���ر���MR Dicomͼ��ʧ��"
	STORAGE_FAULT_STORAGEREAD,   //"DICOM ������","storage read����"
	STORAGE_FAULT_DELETECLOSE,   //"DICOM ������","delete close����"
	STORAGE_FAULT_RESETDELETE,   //"DICOM ������","reset delete����"
	STORAGE_FAULT_LISTEN,        //"DICOM ������","�����˿ڴ���,�����˳�"
	STORAGE_FAULT_RESTARTLISTEN, //"DICOM ������","�����˿ڴ���,������������"
	STORAGE_FAULT_MAINTHREAD,    //"DICOM ������","���̴߳���"
	STORAGE_FAULT_CLOSEMAINSOCKET, //"DICOM ������","�ر���SOCKET����"
	STORAGE_SERVICE_START,        //"DICOM ������","��������"
	STORAGE_SERVICE_STOP,        //"DICOM ������","��������ֹͣ"
	STORAGE_RECEIVE_CTSERIES,      //"received CT image number is %d.\r\n"
	STORAGE_RECEIVE_MRSERIES,       //"received MRI image number is %d.\r\n"
	//  [9/5/2011 Administrator]
	STORAGE_FAULT_INITIALIZENETWORK,			//"ASC_initializeNetwork����"
	STORAGE_FAULT_CREATEASSOCIATIONPARAMETERS, //"ASC_createAssociationParameters����"
	STORAGE_FAULT_SETTRANSPORTLAYERTYPE  //"ASC_setTransportLayerType����"
	//  [9/5/2011 Administrator]

	
	
};
#endif //__DICOMMESSAGE_H__