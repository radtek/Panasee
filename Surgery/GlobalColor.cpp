// GlobalColor.cpp: implementation of the CGlobalColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "GlobalColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalColor::CGlobalColor()
{
	float bAlpha=(float)0.3;
	cVTDefaultLabel = CColorF(0.6f, 0.2f, 0.2f, bAlpha);
	cVTBefore = CColorF(1.0f, 1.0f, 1.0f, bAlpha);
	cVTAfter = CColorF(0.0f, 0.0f, 0.0, bAlpha);
	cVTSelected = CColorF(1.0f, 1.0f, 0.0f, bAlpha);

	m_iCurColorIndex=0;
	cVTColor[0] = CColorF(1.0f, 0.0f, 0.0f, bAlpha);	
	cVTColor[1] = CColorF(0.0f, 1.0f, 0.0f, bAlpha);
	cVTColor[2] = CColorF(1.0f, 0.0f, 1.0f, bAlpha);
	cVTColor[3] = CColorF(0.0f, 1.0f, 1.0f, bAlpha);
	cVTColor[4] = CColorF(0.0f, 0.0f, 1.0f, bAlpha);	
	cVTColor[5] = CColorF(0.0f, 128/255.0f, 0.0f, bAlpha);
	cVTColor[6] = CColorF(1,1,0, bAlpha);

    cProbeExt = CColorF( 1.0f, 1.0f, 0.0f, 1.0f );
	cProbeBody = CColorF( 1.0f, 0.0f, 0.0f, 1.0f );
	cProbeOffset = CColorF(0.0f, 1.0f, 0.0f, 1.0f);

}

CGlobalColor::~CGlobalColor()
{

}

CColorF* CGlobalColor::GetVTColor()
{
	CColorF *pColor=&(cVTColor[m_iCurColorIndex]);
	m_iCurColorIndex++;
	if(m_iCurColorIndex>6)
		m_iCurColorIndex=0;
	return pColor;
}

CColorF* CGlobalColor::GetVTDefaultLabel()
{
	return &cVTDefaultLabel;
}

CColorF* CGlobalColor::GetVTBefore()
{
	return &cVTBefore;
}

CColorF* CGlobalColor::GetVTAfter()
{
	return &cVTAfter;
}

CColorF* CGlobalColor::GetVTSelected()
{
	return &cVTSelected;
}

CColorF* CGlobalColor::GetProbeExt()
{
	return &cProbeExt;
}
CColorF* CGlobalColor::GetProbeBody()
{
	return &cProbeBody;
}
CColorF* CGlobalColor::GetProbeOffset()
{
	return &cProbeOffset;
}