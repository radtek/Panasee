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
	STORAGE_FAULT_MULTIPLEX,     //"DICOM 服务器","multiplex错误"
	STORAGE_FAULT_PDUREAD,       //"DICOM 服务器","pduread错误"
	STORAGE_FAULT_NEW,           //"DICOM 服务器","new错误"
	STORAGE_FAULT_CT_SAVE,       //"DICOM 服务器","本地保存CT Dicom图像失败"
	STORAGE_FAULT_MR_SAVE,       //"DICOM 服务器","本地保存MR Dicom图像失败"
	STORAGE_FAULT_STORAGEREAD,   //"DICOM 服务器","storage read错误"
	STORAGE_FAULT_DELETECLOSE,   //"DICOM 服务器","delete close错误"
	STORAGE_FAULT_RESETDELETE,   //"DICOM 服务器","reset delete错误"
	STORAGE_FAULT_LISTEN,        //"DICOM 服务器","监听端口错误,服务退出"
	STORAGE_FAULT_RESTARTLISTEN, //"DICOM 服务器","监听端口错误,重新启动服务"
	STORAGE_FAULT_MAINTHREAD,    //"DICOM 服务器","主线程错误"
	STORAGE_FAULT_CLOSEMAINSOCKET, //"DICOM 服务器","关闭主SOCKET错误"
	STORAGE_SERVICE_START,        //"DICOM 服务器","服务启动"
	STORAGE_SERVICE_STOP,        //"DICOM 服务器","服务被正常停止"
	STORAGE_RECEIVE_CTSERIES,      //"received CT image number is %d.\r\n"
	STORAGE_RECEIVE_MRSERIES,       //"received MRI image number is %d.\r\n"
	//  [9/5/2011 Administrator]
	STORAGE_FAULT_INITIALIZENETWORK,			//"ASC_initializeNetwork错误"
	STORAGE_FAULT_CREATEASSOCIATIONPARAMETERS, //"ASC_createAssociationParameters错误"
	STORAGE_FAULT_SETTRANSPORTLAYERTYPE  //"ASC_setTransportLayerType错误"
	//  [9/5/2011 Administrator]

	
	
};
#endif //__DICOMMESSAGE_H__