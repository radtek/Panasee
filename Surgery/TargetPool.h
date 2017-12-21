// TargetPool.h: interface for the CTargetPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGETPOOL_H__6F639E86_DA94_4776_91CC_362D072366A6__INCLUDED_)
#define AFX_TARGETPOOL_H__6F639E86_DA94_4776_91CC_362D072366A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Geometry.h"


class CVirtualTarget;
class CCryotherapyDlg;


class CTargetPool  
{
public:
	CTargetPool();
	virtual ~CTargetPool();

	void CreateATarget(Vector3D vPos, BOOL bCreateVirtualProbe = FALSE);

	void DeleteATarget(CVirtualTarget* pTarget);

	void TransformAllVTarget(double** ppdtran);
	void TranslateAllVTarget(Vector3D vMov);
	/*根据每个target创建时保留的病床原始位置计算病床位移*/
	void TranslateAllVTarget(double dCurPTPos,BOOL bInverse);

	CCryotherapyDlg *m_pCryoDlg;
	CList<CVirtualTarget*, CVirtualTarget*> m_lstVirtualTargets;

private:
	int m_iNumVirtualTarget;

};

#endif // !defined(AFX_TARGETPOOL_H__6F639E86_DA94_4776_91CC_362D072366A6__INCLUDED_)
