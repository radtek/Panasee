#if !defined _DATAMANAGER_H
#define _DATAMANAGER_H

#include "MyGeometry.h"

/*typedef enum DATA_TYPE {
	DATATYPE_POINTCLOUD,
	DATATYPE_MESH
} DATATYPE;*/

class _declspec(dllexport) CData : public CObject
{
public:
	CData();
	CData(CString sPathFileName, VOID *pData, MY_GEO_DATATYPE iType);
	~CData();

	VOID* GetData();
	MY_GEO_DATATYPE GetType();
	CString GetFilePathName();
	CString GetFileName();
	CString GetExtension();

private:
	CString sPathFileName, sFileName, sExt;
	VOID* m_pData;
};

class _declspec(dllexport) CDataManager : public CObject
{
public:
	CDataManager();
	~CDataManager();

	CString Add(VOID *pData, CString sName, MY_GEO_DATATYPE iType);
	int AddWithReplace(VOID *pData, CString sName, MY_GEO_DATATYPE iType);	
	VOID* Get(CString sName, MY_GEO_DATATYPE& iType);
	
	CStringArray * GetPointCloudListName();
	CStringArray * GetMeshListName();

	int GetPointCloudAll(CPtrArray *ppaPontCloud);
	int GetMeshAll(CPtrArray *ppaMesh);

	int ReName(CString sOldName, CString sNewName);
	
	int Remove(CString sName);
	int ClearAll();
	
private:
	CPtrArray m_paMeshs;
	CPtrArray m_paPointCloud;
	
	CStringArray m_saMeshs;
	CStringArray m_saPointCloud;

};
#endif
