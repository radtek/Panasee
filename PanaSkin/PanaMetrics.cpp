// PanaMetrics.cpp: implementation of the CPanaMetrics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PanaMetrics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanaMetrics::CPanaMetrics()
{

}

CPanaMetrics::~CPanaMetrics()
{

}



CFont* CPanaMetrics::GetDlgFont()
{
	static CFont fontDlg;
	LOGFONT lf;
	lf.lfHeight=-11;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=DEFAULT_CHARSET; //ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Tahoma");

	fontDlg.DeleteObject();
	fontDlg.CreateFontIndirect(&lf);
	return &fontDlg;
}

UINT CPanaMetrics::GetMetrics(PASK_UI_METRICS metrics)
{
	UINT iReturn=0;

	switch(metrics)
	{
	case PASK_METRICS_BTN_POS_RIGHTFRAME:
		iReturn=13;
		break;
	case PASK_METRICS_BTN_POS_BOTTOMFRAME:
		iReturn=12;
		break;
	case PASK_METRICS_BTN_POS_SEPRATE:
		iReturn=6;
		break;
	case PASK_METRICS_CTRL_POS_SEPRATE_HORZ:
		iReturn=8;
		break;
	case PASK_METRICS_BTN_POS_HEIGHT:
		iReturn=21;
		break;
	case PASK_METRICS_BTN_POS_WIDTH:
		iReturn=75;
		break;
	case PASK_METRICS_CLIENT_LEFT:
		iReturn=10;
		break;
	case PASK_METRICS_CLIENT_TOP:
		iReturn=10;
		break;
	case PASK_METRICS_CLIENT_RIGHT:
		iReturn=10;
		break;
	case PASK_METRICS_CLIENT_BOTTOM:
		iReturn=10;
		break;
	case PASK_METRICS_CLIENT_SEPRATE:
		iReturn=60;
		break;
	case PASK_METRICS_FRAME_LEFT:
		iReturn=10;
		break;
	case PASK_METRICS_FRAME_TOP:
		iReturn=10;
		break;
	case PASK_METRICS_FRAME_BOTTOM:
		iReturn=10;
		break;
	case PASK_METRICS_FRAME_RIGHT:
		iReturn=10;
		break;
	case PASK_METRICS_TEXT_POS_BOTTOMCTRL:
		iReturn=3;
		break;
	case PASK_METRICS_TEXT_POS_RIGHTCTRL:
		iReturn=3;
		break;
	case PASK_METRICS_CTRL_POS_SEPRATE_VERT:
		iReturn=8;
		break;
	}
	return iReturn;
}

