// MultiUsedObject.h: interface for the CMultiUsedObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIUSEDOBJECT_H__FBB1A7DE_FBD7_420D_B85E_AEC997DA6886__INCLUDED_)
#define AFX_MULTIUSEDOBJECT_H__FBB1A7DE_FBD7_420D_B85E_AEC997DA6886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_MRISERIES_DLL
	#define MRISERIES_API		__declspec(dllexport)
#else
	#define MRISERIES_API		__declspec(dllimport)
#endif

class MRISERIES_API CMultiUsedObject
{
public:
	void Delete();
	int Count();
	void AddOne();
	
	unsigned short *m_pDataObject;

	CMultiUsedObject();
	virtual ~CMultiUsedObject();

private:
	
	int m_iCount; //使用计数	

};

#endif // !defined(AFX_MULTIUSEDOBJECT_H__FBB1A7DE_FBD7_420D_B85E_AEC997DA6886__INCLUDED_)
