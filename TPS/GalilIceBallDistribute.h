// GalilIceBallDistribute.h: interface for the CBallIceGalil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GALILICEBALLDISTRIBUTE_H__1125083F_C9D6_4CA8_A782_0750B830AF29__INCLUDED_)
#define AFX_GALILICEBALLDISTRIBUTE_H__1125083F_C9D6_4CA8_A782_0750B830AF29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TpsGlobalConst.h"
#include "IceBallDistribute.h"
class CProbeCryo;

class CBallIceGalil : public CBallIce  
{
public:
	void GetBounds(double *bound);
	/*static CBallIceGalil * New ()
	{
		return new CBallIceGalil;
	}*/
	
	CBallIceGalil();
	virtual ~CBallIceGalil();

	InitialBall(NEEDLE_INFO eNeedleType);

	CreateDose(CProbeCryo* pTool);

	void CreateShell();
	void SetShellProperty(BALL_SHELL ball_shell);

	CProbeCryo* m_pToolAttach;
	CList<INTERSECTLINE_PROPERTY,INTERSECTLINE_PROPERTY&> m_arrIntersectionInfo;
	vtkProp3DCollection* m_pIntersections;
	vtkProp3DCollection* m_pShell;
	vtkPolyDataCollection* m_pShellData;
	vtkImplicitFunctionCollection * m_pQuadrics;
	vtkMatrix4x4* m_pTransform;
	int GetImageIntersection(vtkPlaneCollection* pPlanes,int imageIndex);
	int ShowIntersections(BOOL bShow);
	void SetIntersectionProperty(INTERSECTLINE_PROPERTY intersection_property);

	BOOL m_bDisplayInterLine;
protected:
	
};

#endif // !defined(AFX_GALILICEBALLDISTRIBUTE_H__1125083F_C9D6_4CA8_A782_0750B830AF29__INCLUDED_)
