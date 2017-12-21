
#ifndef __UTIL_H
#define __UTIL_H
#include "constant.h"

class DICOMDataObject;
class PDU_Service;

typedef unsigned short UINT16;
typedef unsigned char UINT8;

#define UNKNOWPATIENTID "UNKNOWPATIENTID"
#define UNKNOWPATIENTNAME "UNKNOWPATIENTNAME"
#define UNKNOWSEX "O"
#define UNKNOWMEDICALID "UNKNOWMEDICALID"
#define UNKNOWMODALITY	"CT"
#define DEFAULTSTUDYUIDBASE "1.2.840.10008.9999."
#define DEFAULTSERIESUIDBASE "1.2.840.10008.9999.9999."
#define DEFAULTIMAGEUIDBASE "1.2.840.10008.9999.9999.9999."

UINT		uniq();
UINT32		uniq32();
UINT16		uniq16();
UINT8		uniq8();
UINT8		uniq8odd();
UINT16		uniq16odd();

BOOL		ByteCopy(BYTE *, BYTE *, UINT);
BOOL		ZeroMem(BYTE *, UINT);
BOOL		SpaceMem(BYTE	*mem,UINT	Count );

UINT		ByteStrLength(BYTE *);
int			TrimMemCmp(const void *buf1, const void *buf2, UINT	count1, UINT count2); //Add by Lin
char*		GetLastNByte(char*strUID,int n);
BOOL		ToUID(char*pStr);
BOOL NewSOPInstanceUID(char*strBase,char*retval);//add by peak
BOOL MTrimRight(char*pStr);

typedef int (*SVRCBProc)(char*FileName);
typedef struct 
{
	HANDLE    hwnd;	             //Windows handle to sent message.
	SVRCBProc cbProc;            //Address  of Callback function.
	int       Sockfd;            //SockID for connecting pending.
	BOOL      bToBeTerm;
	char      RootPath[MAX_PATH];
	char      LocalAET[256];
	char      LocalPort[10];
	char      strIPOrServer[50];
	BYTE*     lpDcmSeries ;      //point to packing data of receiving series.
	int       nPackLen ;         //the package length of receiving series.
}CBI;


//store
int StandardSave(PDU_Service *pPDU, DICOMDataObject *pDDO,CBI*pCBI);

#endif

