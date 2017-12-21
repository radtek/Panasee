// ServiceQueue.h: interface for the CServiceQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICEQUEUE_H__631A7E61_4C55_4E26_9CC6_F30DB05F7A45__INCLUDED_)
#define AFX_SERVICEQUEUE_H__631A7E61_4C55_4E26_9CC6_F30DB05F7A45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dicom.hpp"
typedef struct
{
	int nCmd;//0:��������1��ʧ�ܣ�2���ȴ���Ӧ��3��������
	int nType;//1:STORE;2:PRINT;3:RETRIEVE;4:CDRECORD;5:MESSAGE     STORE,PRINT��CDRECORD��DCMCLIENTSVR�д���RETRIEVE��CPVIEWER�д���
	char* strServerName;
	char* strFileList;
	char* strWinLevels;
	char* strFormat;//����RETRIEVE,���������Ϊ��"10,2,28",��ʾ�ɹ�10��ʧ��2��ʣ��28��
	int nCopy;
	int nID;
	char strStartTime[30];
}SERVICEINFO;
class CServiceInfo
{
public:
	int nCmd;//0:��������1��ʧ�ܣ�2���ȴ���Ӧ��3��������
	int nType;//1:STORE;2:PRINT;3:RETRIEVE;4:CDRECORD;5:MESSAGE     STORE,PRINT��CDRECORD��DCMCLIENTSVR�д���RETRIEVE��CPVIEWER�д���
	char* strServerName;
	char* strFileList;
	char* strWinLevels;
	char* strFormat;//����RETRIEVE,���������Ϊ��"10,2,28",��ʾ�ɹ�10��ʧ��2��ʣ��28��
	int nCopy;
	int nID;
	int nFail;
	char strStartTime[30];
public:
	CServiceInfo()
	{
		nCmd=0;
		nType=3;
		strServerName=NULL;
		strFileList=NULL;
		strWinLevels=NULL;
		strFormat=NULL;
		nCopy=1;
		nID=0;
		nFail=0;
		strStartTime[0]='\0';
	}
	~CServiceInfo()
	{
		if(strServerName)
			delete strServerName;
		if(strFileList)
			delete strFileList;
		if(strWinLevels)
			delete strWinLevels;
		if(strFormat)
			delete strFormat;
	}
};
class CServiceQueue  
{
public:
	BOOL CleanInfo(SERVICEINFO&Info);
	BOOL RemoveAll();
	BOOL GetInfo(int nIndex,SERVICEINFO*&pInfo);
	int Add(SERVICEINFO* pInfo);
	int nMaxID;
	int GetMaxID();
	BOOL GenerateLine(char*&pString,SERVICEINFO*pInfo);
	BOOL ParseLine(char*pString,SERVICEINFO*pInfo);
	Array<SERVICEINFO*>info;
	BOOL DeleteAt(int nID);
	BOOL GetAt(int nID,SERVICEINFO*&pInfo);
	BOOL SetAt(int nID,SERVICEINFO*pInfo);
	int GetCount();
	BOOL Save(char*QueuePath);
	BOOL Load(char*QueuePath);
	CServiceQueue();
	virtual ~CServiceQueue();

private:
	BOOL ReadLine(FILE*pF,char*&OutString);
	BOOL WriteLine(FILE*pF,char*&InString);
};

#endif // !defined(AFX_SERVICEQUEUE_H__631A7E61_4C55_4E26_9CC6_F30DB05F7A45__INCLUDED_)
