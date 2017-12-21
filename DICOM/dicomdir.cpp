#include "dicomdir.h"
#include <io.h>
#include <Shlwapi.h>
//添加文件到dir
BOOL AddToDir(char*DcmDirPath,char*DcmFilePath)
{
	char Path[MAX_PATH],strRefID[MAX_PATH];
	strcpy(Path,DcmDirPath);
	::PathRemoveFileSpec(Path);
	if(Path[strlen(Path)-1]!='\\')
		strcat(Path,"\\");
	if(strstr(DcmFilePath,Path)!=DcmFilePath)
		return FALSE;//必须在同一个目录下
	strcpy(strRefID,DcmFilePath+strlen(Path));

	DICOMDIR clsDICOMDIR;
	clsDICOMDIR.SetLocalAddress((BYTE*)"DCMPACS_V0100");
	PDU_Service pdu;
//	pdu.AttachRTC();
	DICOMDataObject *pDDO;
	if((pDDO=pdu.LoadDICOMDataObject(DcmFilePath))==0)
		return FALSE;

	if(!clsDICOMDIR.Save(pDDO, DcmDirPath,strRefID, "DCMPACS_V0100", "106.3453.78.678", DICOM_MEDIA_EXPLICITLITTLEENDIAN)) 
//	if(!clsDICOMDIR.Save(pDDO, DcmDirPath,strRefID, "DCMPACS_V0100", "106.3453.78.678", DICOM_MEDIA_IMPLICITLITTLEENDIAN)) 
	{
		pDDO->Reset();
		delete pDDO;
		return FALSE;
	}
	pDDO->Reset();
	delete pDDO;
	return TRUE;
}

