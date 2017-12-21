// GlobalColor.h: interface for the CGlobalColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALCOLOR_H__E2769DE0_7FC0_4978_87F9_DD0AB79DF0C0__INCLUDED_)
#define AFX_GLOBALCOLOR_H__E2769DE0_7FC0_4978_87F9_DD0AB79DF0C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Colorf.h"
class CGlobalColor  
{
public:
	CGlobalColor();
	virtual ~CGlobalColor();
	
	static CGlobalColor* GetColor(void)
	{
		static CGlobalColor manager;
		return &manager;
	}
	CColorF* GetVTColor();
	CColorF* GetVTAfter();
	CColorF* GetVTBefore();
	CColorF* GetVTSelected();
	CColorF* GetVTDefaultLabel();
	CColorF* GetProbeExt();
	CColorF* GetProbeBody();
	CColorF* GetProbeOffset();

private:
	short m_iCurColorIndex;
	CColorF cVTColor[10];
	CColorF cVTDefaultLabel;
	CColorF cVTBefore;
	CColorF cVTAfter;
	CColorF cVTSelected;
	CColorF cProbeExt;
	CColorF cProbeBody;
	CColorF cProbeOffset;
};

#endif // !defined(AFX_GLOBALCOLOR_H__E2769DE0_7FC0_4978_87F9_DD0AB79DF0C0__INCLUDED_)
