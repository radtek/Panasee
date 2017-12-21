// DoseDistribute.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoseDistribute.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall()
{

}

CBall::~CBall()
{

}
CBall::CreateDose(vtkMatrix4x4* pTransform)
{
	
}


float CBall::GetGrowthRatio()
{
	return m_fGrowthRatio;
}
void CBall::SetGrowthRatio(float fGrowthRatio)
{
	m_fGrowthRatio = fGrowthRatio;
}