// VirtualTarget.cpp: implementation of the CVirtualTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "VirtualTarget.h"
#include "GlobalColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualTarget::CVirtualTarget(Vector3D vPos)
{

	m_Color=*(CGlobalColor::GetColor()->GetVTColor());

	m_pSphereTarget = new CSphere(vPos, 4.0);

	// Iceball related
	m_pSphereInside = new CEllipsoid(Vector3D(0,0,0), Vector3D(1,0,0), 4.0, 12.0);
	m_pSphereInside->SetVisible(TRUE);
	m_pSphereInside->SetColor(&m_Color);

	
	m_bSelected = FALSE;
	m_iRelativePos = 0;
	//
	
	m_pSphereTarget->SetColor(&m_Color);
	m_stVTLabel.cfColor = *(CGlobalColor::GetColor()->GetVTDefaultLabel());

	m_stVTLabel.sText="";
	m_stVTLabel.bShow=FALSE;

	SetPos(vPos);

	
}

CVirtualTarget::CVirtualTarget(Vector3D vPos,CColorF color,double *pDirection)
{
	m_Color=color;

	m_pSphereTarget = new CSphere(vPos, 4.0);

	// Iceball related
	m_pSphereInside = new CEllipsoid(Vector3D(0,0,0), 
		                        Vector3D(pDirection[0],pDirection[1],pDirection[2]), 
								4.0, 12.0);
	m_pSphereInside->SetVisible(TRUE);
	m_pSphereInside->SetColor(&m_Color);

	
	m_bSelected = FALSE;
	m_iRelativePos = 0;
	//
	
	m_pSphereTarget->SetColor(&m_Color);
	m_stVTLabel.cfColor = *(CGlobalColor::GetColor()->GetVTDefaultLabel());

	m_stVTLabel.sText="";
	m_stVTLabel.bShow=FALSE;

	SetPos(vPos);

	
}

CVirtualTarget::~CVirtualTarget()
{
	if (m_pSphereTarget)
	{
		delete m_pSphereTarget;
	}

	if (m_pSphereInside)
	{
		delete m_pSphereInside;
		m_pSphereInside = NULL;
	}
}

void CVirtualTarget::TransformM(double** ppdtran)
{
	m_pSphereTarget->Transformation(ppdtran);
	m_stVTLabel.vPosition.Transformation(ppdtran);
	m_pSphereInside->Transformation(ppdtran);
}

void CVirtualTarget::Translate(Vector3D vMov)
{
	m_pSphereTarget->Translation(vMov);
	m_stVTLabel.vPosition = m_stVTLabel.vPosition + vMov;
	m_pSphereInside->Translation(vMov);
}


void CVirtualTarget::SetIndex(int iIndex)
{
	CString sName;
	m_iIndex = iIndex;
	sName.Format("VTarget%02d", m_iIndex);
	m_pSphereTarget->SetName(sName);
	
	sName.Format("InsideSphere%02d-1", m_iIndex);
	m_pSphereInside->SetName(sName);
}

BOOL CVirtualTarget::IsSelected()
{
	return m_bSelected;
}

void CVirtualTarget::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
	if (m_bSelected)
	{
		m_pSphereTarget->SetColor(CGlobalColor::GetColor()->GetVTSelected());
		m_pSphereInside->SetColor(CGlobalColor::GetColor()->GetVTSelected());
	}
	else
	{
		SetRelativePosition(m_iRelativePos);
	}
}


void CVirtualTarget::SetRelativePosition(int iPos)
{

	m_iRelativePos = iPos;
	if (!m_bSelected)
	{
		if (iPos == 0)
		{
			m_pSphereTarget->SetColor(&m_Color);
			m_pSphereInside->SetColor(&m_Color);
		}
		else if (iPos == 1)
		{
			m_pSphereTarget->SetColor(&m_Color);
			m_pSphereInside->SetColor(CGlobalColor::GetColor()->GetVTBefore());
		}
		else if (iPos == 2)
		{
			m_pSphereTarget->SetColor(&m_Color);
			m_pSphereInside->SetColor(CGlobalColor::GetColor()->GetVTAfter());
		}
	}
}

Vector3D CVirtualTarget::GetPos()
{
	return m_pSphereTarget->GetCenter();
}

void CVirtualTarget::SetPos(Vector3D vPos)
{
	m_pSphereTarget->SetCenter(vPos);	
	m_pSphereInside->SetCenter(vPos);
}


void CVirtualTarget::SetLabelText(CString text,Vector3D vPos)
{
	m_stVTLabel.sText=text;
	m_stVTLabel.bShow=TRUE;
	m_stVTLabel.vPosition = vPos;
}
