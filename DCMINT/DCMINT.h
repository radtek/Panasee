#ifndef __DICOMAPI_H__
#define __DICOMAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include "DICOMEnums.h"
#include "GlobalDef.h"


typedef struct  
{
	int           nDatalen;
	int           nImgNum;
	unsigned char data[1];
} DCM_SERIES_HEADER;

typedef struct 
{
	int             nImgDatalen;
	unsigned char	data[1];
} DCM_IMG_HEADER;

/****************     EncBWMRToDcm: 生成DICOM文件      ******************/
/*   参数说明：
          pHeader　　:   [in]图像文件信息。
		  strOutDcm　: 　输出DICOM文件路径名称。
		  return 　　:　 TRUE成功,FALSE失败。
     作用：
          将pHeader描述的数据生成以strOutDcm为路径名称的DICOM文件。
************************************************************************/
//BOOL EncBWMRToDcm(BWMRSTRUCT* pHeader,LPCTSTR strOutDcm);

/***************       StoreToServer：发送DICOM文件     *********************/
/*   参数说明：
          pServerAET :  [in] 指向服务器AET。
		  pServerIP　:  [in] 指向服务器IP。
		  pPort　　　:  [in] 指向服务器提供服务的端口。
		  pDCMFile　 :  [in] DICOM文件路径名称。
		  return     :  TRUE成功,FALSE失败.
     作用：
          将pDCMFile文件发送到对方,对方的AET为pServerAET,
		  服务ip为pServerIP,端口为pPort。
****************************************************************************/
//BOOL StoreToServer(const char* pServerAET,const char* pServerIP,const char* pPort,const char* pDCMFile);

//接收文件后的回调函数,可以通过该函数获得实际存储的文件路径
typedef int (*SVRCBProc)(char*FileName);
/***************       BeginSVRSCP：启动DICOM服务线程     ******************/
/*   参数说明：
          CBProc :  [in] 回调函数,如果不需要回调函数，可以传NULL进去。
		  pServerIP　:  [in] 指向服务器IP。
		  pPort　　　:  [in] 指向服务器提供服务的端口。
		  pDCMFile　 :  [in] DICOM文件路径名称。
		  return     :  线程Handle。NULL表明失败。
     作用：
          启动DICOM服务进程，包括DICOM文件接收功能，Q/R服务，Verify服务。
****************************************************************************/
HANDLE BeginSVRSCP(BYTE *pCbi,SVRCBProc CBProc,HANDLE hwnd,BYTE* lpBuff);

/***************       TermSVRSCP：终止DICOM服务线程     ******************/
/*   参数说明：
          Handle :  [in] BeginSVRSCP返回的HANDLE。
		  return     :  TRUE成功,FALSE失败。
     作用：
          启动DICOM服务进程，包括DICOM文件接收功能，Q/R服务，Verify服务。
****************************************************************************/
BOOL TermSVRSCP(HANDLE Handle);



void SetAET(char* szAETSource);

void SetPort(char* szPortSource);



void SetRootDir(char* szPortSource);



#ifdef __cplusplus
}
#endif

#endif //__DICOMAPI_H__