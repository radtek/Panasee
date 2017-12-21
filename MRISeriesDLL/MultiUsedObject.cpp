// MultiUsedObject.cpp: implementation of the CMultiUsedObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiUsedObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiUsedObject::CMultiUsedObject()
{
	m_iCount=1;
	m_pDataObject=NULL;
}

CMultiUsedObject::~CMultiUsedObject()
{
	if(m_pDataObject)
	{
		delete []m_pDataObject;
		m_pDataObject=NULL;
	}
}

void CMultiUsedObject::AddOne()
{	
	m_iCount++;
}

int CMultiUsedObject::Count()
{
	return m_iCount;
}

void CMultiUsedObject::Delete()
{
	m_iCount--;
}
