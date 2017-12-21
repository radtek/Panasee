#	include	"dicom.hpp"

#include "dicomdir.h"
#include "iod.hpp"
#include "comp.hpp"
#include <sys/timeb.h>
#include <sys/stat.h>
#include <FCNTL.H>
#include <io.h>
#include <time.h>
#include <math.h>
#include <process.h>
#include <sys/stat.h>
#include <shlwapi.h>
BOOL	ByteCopy(BYTE	*s1, BYTE	*s2, UINT	Count)
	{
	memcpy((void *) s1, (void *) s2, Count);
	return ( TRUE );
	}

BOOL	ZeroMem(BYTE	*mem, UINT	Count)
	{
	memset((void *) mem, 0, Count);
	return ( TRUE );
	}
BOOL
SpaceMem(
	BYTE	*mem,
	UINT	Count )
	{
	memset((void *) mem, ' ', Count);
	return ( TRUE );
	}
UINT	ByteStrLength(BYTE	*s)
	{
	return ( strlen((char *) s) );
	}
BOOL	ShiftLeft(char*pStr)
{
	int nlen=strlen(pStr);
	if(nlen>256||nlen<=0)
		return FALSE;
	char stmp[256];
	strcpy(stmp,pStr+1);
	strcpy(pStr,stmp);
	return TRUE;
}
BOOL ToUID(char*pStr)
{
	for(int i=0;pStr[i]!='\0';)
	{
		if((pStr[i]>'9'||pStr[i]<'0')&&pStr[i]!='.')
		{
			if(ShiftLeft(pStr)==FALSE)
				return FALSE;
		}
		else
			i++;
	}
	return TRUE;
}
//Add by Lin
int TrimMemCmp(const void *buf1, const void *buf2, UINT	count1, UINT count2 )
	{
	
	char * str1 = new char[count1 + 1];
	memset((void *) str1, 0, count1 + 1);
    char * str2 = new char[count2 + 1];
	memset((void *) str2, 0, count2 + 1);

    UINT nStart;
    UINT nEnd;
    UINT nActualLen;
	unsigned char * pBuf1 = (unsigned char *) buf1; 
	unsigned char * pBuf2 = (unsigned char *) buf2; 

#ifdef _DEBUG	//zxh
	if(strlen((char *)buf1) < count1)
		nActualLen = strlen((char *)buf1);
	else
#endif
		nActualLen = count1;
	for(nStart = 0; nStart < nActualLen; nStart++) 
		{
        if(!isspace( pBuf1[nStart])&&pBuf1[nStart]!='\0')
 //       if(!isspace( pBuf1[nStart]))//peak
            break;
		}

    for(nEnd = nActualLen - 1; nEnd >=0 ; nEnd--) 
		{
        if(!isspace( pBuf1[nEnd] )&&pBuf1[nEnd]!='\0')
 //       if(!isspace( pBuf1[nEnd] ))
            break;
		}

    UINT nLength1 = nEnd - nStart + 1;
	char * pStr1;
    pStr1 = (char *)buf1 + nStart;
    memcpy( (void *) str1, (void *) pStr1, nEnd - nStart + 1);

#ifdef _DEBUG	//zxh
	if(strlen((char *)buf2) < count2)
		nActualLen = strlen((char *)buf2);
	else
#endif
		nActualLen = count2;
    for(nStart = 0; nStart < nActualLen; nStart++)
		{
        if(!isspace( pBuf2[nStart] )&&pBuf2[nStart]!='\0')
 //       if(!isspace( pBuf2[nStart] ))
            break;
		}

    for(nEnd = nActualLen - 1; nEnd >=0 ; nEnd--)
		{
        if(!isspace( pBuf2[nEnd] )&&pBuf2[nEnd]!='\0')
 //       if(!isspace( pBuf2[nEnd] ))
            break;
		}

    UINT nLength2 = nEnd - nStart + 1;
    char * pStr2;
    pStr2 = (char *)buf2 + nStart;
    memcpy( (void *) str2, (void *) pStr2, nEnd - nStart + 1);

    if(nLength1 != nLength2) {
	    delete [] str1;
		delete [] str2;
		return nLength1 - nLength2;
	}

	int nResult = memcmp((void *)str1, (void *)str2, nLength1);

    delete [] str1;
    delete [] str2;

    return nResult;
	}
/*
UINT16	NativeEndian(UINT16	uint16, UINT	Endian)
	{
	UINT16	ruint16;

	if(Endian==NATIVE_ENDIAN)
		return(uint16);
	
	ruint16 = 0;
	ruint16 = (uint16 << 8)&0xff00;
	ruint16 = ruint16 | ((tuint16 >> 8) & 0x00ff);
	return(ruint16);
	}
*/
char* GetLastNByte(char*strUID,int n)
{
	char temp[MAX_PATH];
	memset(temp,0,MAX_PATH);
	StrTrim(strUID," ");
	int len=strlen(strUID);
	int i,j;
	j=0;
	for(i=0;i<len;i++)
	{
		if(strUID[i]=='.'||strUID[i]==':')//zxh for time
			continue;
		temp[j]=strUID[i];
		j++;
	}
	strcpy(strUID,temp);
	len=strlen(strUID);
	if(len<=n)
		return strUID;
	return strUID+len-n;
}
BOOL NewSOPInstanceUID(char*strBase,char*retval)
{
	int len,len1;
	len=strlen(strBase);
	if(len>=64)return FALSE;
	char temp[66];
	char*ptmp;
	ptmp=temp;
	memset(temp,0,66);

	struct _timeb tb;
	struct tm* ptm;
	_ftime( &tb );
	ptm=gmtime(&(tb.time));

	sprintf(temp,"%d%d%d.%d%d%d.%d.%d.%d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,\
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec,tb.millitm,clock(),rand());
	len1=strlen(temp);
	if(len1+len>62)
	{
		ptmp+=(len1+len-62);
	}
	strcpy(retval,strBase);
	strcat(retval,ptmp);
	
	return TRUE;
}


BOOL MTrimRight(char*pStr)
{
	for(int i=strlen(pStr)-1;i>=0;i--)
	{
		if(pStr[i]=='\0'||pStr[i]==' ')
			pStr[i]='\0';
		else
			break;
	}
	return TRUE;
}

BOOL GetSOPInstanceUID(char*strSOPInstanceUID,DICOMDataObject*pDDO)
{
	VR*vr=pDDO->GetVR(0x0008,0x0018);
	if(vr==NULL||vr->Length<=0)
		return FALSE;
	strncpy(strSOPInstanceUID,(char*)vr->Data,vr->Length);
	strSOPInstanceUID[vr->Length]='\0';
	MTrimRight(strSOPInstanceUID);
	return TRUE;
}
static int fileNum=0;
int StandardSave(PDU_Service *pPDU, DICOMDataObject *pDDO,CBI*pCBI)
{
    //get the image path
	TCHAR strPathName[MAX_PATH];

	try
	{
		

		//此处得到路径
	 	strcpy(strPathName,"c:\\1.dcm");
		if(pCBI->cbProc!=NULL)
		{
			pCBI->cbProc(strPathName);   //得到保存路径
		}
		int nImgNo ;
		int nImgNum  = 0 ;
		int nDataLen = 0 ;
		BYTE* lpTemp = pCBI->lpDcmSeries + pCBI->nPackLen + 4;
		nImgNum = pPDU->SaveDICOMDataObject(strPathName, 
			                                DICOM_MEDIA_IMPLICITLITTLEENDIAN,
											pDDO,
											&nImgNo,
											lpTemp,
											nDataLen) ;
		if(nImgNum < 0)
		{
			return 0;
		}

		lpTemp = pCBI->lpDcmSeries + pCBI->nPackLen ;
		memcpy(lpTemp, &nDataLen, 4);
		pCBI->nPackLen += nDataLen + 4 ;

		return 1 ;
	}
	catch(...)
	{

		return 0;
	}
}
