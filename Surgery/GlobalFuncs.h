// GlobalFuncs.h: interface for the CGlobalFuncs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALFUNCS_H__4F975046_B1DC_40EA_9741_C83D4C42B706__INCLUDED_)
#define AFX_GLOBALFUNCS_H__4F975046_B1DC_40EA_9741_C83D4C42B706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>
#include "GlobalDef.h"

#define DEBUG_MENU  FALSE

#define DISTANCE(x1, y1, z1, x2, y2, z2) sqrt(((x1)-(x2)) * ((x1)-(x2)) + ((y1)-(y2)) * ((y1)-(y2)) + ((z1)-(z2)) * ((z1)-(z2)) )


enum CALIBRATION_TYPE
{
	CALIBRATION_NOW,
	CALIBRATION_MANUAL,
	CALIBRATION_USE_LAST
};

typedef enum _tagOPERATIONTYPE
{
	OPERATION_TYPE_NONE = 0,
	OPERATION_TYPE_BIOPSY,
	OPERATION_TYPE_CRYOTHERAPY,
	OPERARION_TYPE_BRACHYTHERAPY,
	OPERATION_TYPE_OZONETHERAPY,
	OPERATION_TYPE_TPS,
	OPERATION_TYPE_REVIEW	
} OPERATIONTYPE;

struct PROBE_PAR_CRY
{
	CString sName;
	double	dbLength;
	double	dbRadius;
	double	dbSAxis0;
	double	dbLAxis0;
	double	dbOffset0;
	double	dbSAxis20;
	double	dbLAxis20;
	double	dbOffset20;
	double	dbSAxis40;
	double	dbLAxis40;
	double	dbOffset40;
};

struct PROBE_PAR_BIO
{
	CString sName;
	double  dbRadius;
};


class CGlobalFuncs  
{
public:
	static void Clean();
	static CFont* GetFont();
	static CFont* GetFont40();
	static CFont* GetFont20();
	
	CGlobalFuncs();
	virtual ~CGlobalFuncs();

	//static CFont *m_pFont;

	static int LoadProbeFile(CString sFilePath, double dOri[3], double dTip[3], double &dLength, double &dDiameter); //Add by Dai Liang 2007.6.18

	static CString MRINameToFolder(CString sMRIName);
	static CString MRIFolderToName(CString sMRIFolder);

	static BOOL IsFileExist(CString sFileName);


	static void StartLog();
	static BOOL SaveBitmapFile(CString strFileName,CBitmap& bmp);

	static CString   CGlobalFuncs::GetProductVersion(LPCTSTR   lpszFileName);

private:
	
};

#endif // !defined(AFX_GLOBALFUNCS_H__4F975046_B1DC_40EA_9741_C83D4C42B706__INCLUDED_)
