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

/****************     EncBWMRToDcm: ����DICOM�ļ�      ******************/
/*   ����˵����
          pHeader����:   [in]ͼ���ļ���Ϣ��
		  strOutDcm��: �����DICOM�ļ�·�����ơ�
		  return ����:�� TRUE�ɹ�,FALSEʧ�ܡ�
     ���ã�
          ��pHeader����������������strOutDcmΪ·�����Ƶ�DICOM�ļ���
************************************************************************/
//BOOL EncBWMRToDcm(BWMRSTRUCT* pHeader,LPCTSTR strOutDcm);

/***************       StoreToServer������DICOM�ļ�     *********************/
/*   ����˵����
          pServerAET :  [in] ָ�������AET��
		  pServerIP��:  [in] ָ�������IP��
		  pPort������:  [in] ָ��������ṩ����Ķ˿ڡ�
		  pDCMFile�� :  [in] DICOM�ļ�·�����ơ�
		  return     :  TRUE�ɹ�,FALSEʧ��.
     ���ã�
          ��pDCMFile�ļ����͵��Է�,�Է���AETΪpServerAET,
		  ����ipΪpServerIP,�˿�ΪpPort��
****************************************************************************/
//BOOL StoreToServer(const char* pServerAET,const char* pServerIP,const char* pPort,const char* pDCMFile);

//�����ļ���Ļص�����,����ͨ���ú������ʵ�ʴ洢���ļ�·��
typedef int (*SVRCBProc)(char*FileName);
/***************       BeginSVRSCP������DICOM�����߳�     ******************/
/*   ����˵����
          CBProc :  [in] �ص�����,�������Ҫ�ص����������Դ�NULL��ȥ��
		  pServerIP��:  [in] ָ�������IP��
		  pPort������:  [in] ָ��������ṩ����Ķ˿ڡ�
		  pDCMFile�� :  [in] DICOM�ļ�·�����ơ�
		  return     :  �߳�Handle��NULL����ʧ�ܡ�
     ���ã�
          ����DICOM������̣�����DICOM�ļ����չ��ܣ�Q/R����Verify����
****************************************************************************/
HANDLE BeginSVRSCP(BYTE *pCbi,SVRCBProc CBProc,HANDLE hwnd,BYTE* lpBuff);

/***************       TermSVRSCP����ֹDICOM�����߳�     ******************/
/*   ����˵����
          Handle :  [in] BeginSVRSCP���ص�HANDLE��
		  return     :  TRUE�ɹ�,FALSEʧ�ܡ�
     ���ã�
          ����DICOM������̣�����DICOM�ļ����չ��ܣ�Q/R����Verify����
****************************************************************************/
BOOL TermSVRSCP(HANDLE Handle);



void SetAET(char* szAETSource);

void SetPort(char* szPortSource);



void SetRootDir(char* szPortSource);



#ifdef __cplusplus
}
#endif

#endif //__DICOMAPI_H__