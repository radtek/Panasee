// IceBallDistribute.h: interface for the CBallIce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICEBALLDISTRIBUTE_H__A09DC223_D727_4694_836F_4D763ED2330A__INCLUDED_)
#define AFX_ICEBALLDISTRIBUTE_H__A09DC223_D727_4694_836F_4D763ED2330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DoseDistribute.h"
#include "TpsGlobalConst.h"

class CBallIce : public CBall  
{
public:
	/*static CBallIce * New ()
	{
		return new CBallIce;
	}*/
	
	CreateDose(vtkMatrix4x4* pTransform);
	CArray<BALL_SHELL,BALL_SHELL> m_lstBallShell;
	BOOL m_ballVisible;
	
protected:
	CBallIce();
	virtual ~CBallIce();

};

#endif // !defined(AFX_ICEBALLDISTRIBUTE_H__A09DC223_D727_4694_836F_4D763ED2330A__INCLUDED_)
