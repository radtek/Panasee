// VirtualTarget.h: interface for the CVirtualTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALTARGET_H__0EEB7B77_41F4_4495_90FE_6AE71868F900__INCLUDED_)
#define AFX_VIRTUALTARGET_H__0EEB7B77_41F4_4495_90FE_6AE71868F900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sphere.h"
#include "Ellipsoid.h"

class CVirtualTarget  
{
public:
	void SetLabelText(CString text,Vector3D vPos);
	CVirtualTarget(Vector3D vPos);
	CVirtualTarget(Vector3D vPos,CColorF color,double *pDirection);
	virtual ~CVirtualTarget();
	
	
	void TransformM(double** ppdtran);
	void Translate(Vector3D vMov);
	
	CSphere *GetSphere() { return m_pSphereTarget;};
	CEllipsoid* GetSphereInside() { return m_pSphereInside;};
	ST_TEXT *GetLabel() { return &m_stVTLabel;};
	CColorF GetColor() { return m_Color;};

	Vector3D GetPos();
	void  SetPos(Vector3D vPos);
	
	void SetIndex(int iIndex);
	int  GetIndex() {return m_iIndex;};
	
	
	void SetSelected(BOOL bSelected);
	
	BOOL IsSelected();
	
	/*  0: exact virtual target on target layer; 
	1: projected virtual target on layer before target layer;
	2: projected virtual target on layer after target layer  */
	void SetRelativePosition(int iPos); 
		
private:
	int m_iIndex;
	
	CSphere*  m_pSphereTarget;
	ST_TEXT   m_stVTLabel;    // virtual target label
	
	CEllipsoid *m_pSphereInside;		// inside iceball
	
	
	BOOL  m_bSelected;
	int m_iRelativePos;		// to record relative position with image
	CColorF m_Color;
	
};

#endif // !defined(AFX_VIRTUALTARGET_H__0EEB7B77_41F4_4495_90FE_6AE71868F900__INCLUDED_)
