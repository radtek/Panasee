// TpsTool.cpp: implementation of the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TpsTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CProbe::CProbe()
{
	m_boxWidget=NULL;
}

CProbe::~CProbe()
{

}
CProbe::ToolAssembly()
{
}
CProbe::Initial()
{
	vtkProp3DCollection* pProp3DCollection = this->GetParts();
	pProp3DCollection->InitTraversal();
	for(int i=0; i<pProp3DCollection->GetNumberOfItems();i++)
	{
		pProp3DCollection->GetNextProp3D()->SetPickable(1);
	}
}
