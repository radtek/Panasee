// DoseDistribute.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOSEDISTRIBUTE_H__58FA13C9_D9E5_461F_855A_EF50F8BE1363__INCLUDED_)
#define AFX_DOSEDISTRIBUTE_H__58FA13C9_D9E5_461F_855A_EF50F8BE1363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VtkHeader.h"



class CBall  //: public vtkObject  
{
public:
	/*static CBall * New ()
	{
		return new CBall;
	}*/
	
	virtual CreateDose(vtkMatrix4x4* pTransform);
	float GetGrowthRatio();
	void SetGrowthRatio(float fGrowthRatio);
	float m_fDoseOffSet[3][6]; // x y z,Rx Ry Rz
	float m_fGrowthRatio;
protected:
	CBall();
	virtual ~CBall();
	
};

#endif // !defined(AFX_DOSEDISTRIBUTE_H__58FA13C9_D9E5_461F_855A_EF50F8BE1363__INCLUDED_)
