// GlobalFuncs.cpp: implementation of the CGlobalFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "GlobalFuncs.h"
#include "Log.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString m_sLogPicFolder;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFont *g_pTxtFont=NULL;
CFont *g_pTxtFont40=NULL;
CFont *g_pTxtFont20=NULL;

CGlobalFuncs::CGlobalFuncs()
{
	
}

CGlobalFuncs::~CGlobalFuncs()
{
	
}

CString CGlobalFuncs::MRIFolderToName(CString sMRIFolder)
{
	return sMRIFolder;
}

CString CGlobalFuncs::MRINameToFolder(CString sMRIName)
{
	CString sFolder;
//	CString sModality = sMRIName.Mid(sMRIName.Find('S')+1, sMRIName.Find('-') - sMRIName.Find('S')-1);
//	int ipos = sMRIName.Find('-');
//	CString sStudy = sMRIName.Mid(sMRIName.Find('-', ipos)+1, sMRIName.Find('-', ipos+1) - sMRIName.Find('-', ipos) - 1);
//	ipos = sMRIName.Find('-', ipos+1);
//	CString sSeries = sMRIName.Mid(sMRIName.Find('-', ipos)+1, sMRIName.Find('-', ipos+1) - sMRIName.Find('-', ipos) - 1);

	CString sStudy = sMRIName.Left(sMRIName.Find('.'));
	int ipos = sMRIName.Find('.');
	CString sSeries = sMRIName.Mid(sMRIName.Find('.', ipos)+1, sMRIName.Find('.', ipos+1) - sMRIName.Find('.', ipos) - 1);

	CString sScan = sMRIName.Right(sMRIName.GetLength() - sMRIName.ReverseFind('.')-1);
	sFolder.Format("%s\\%s\\%s", sStudy, sSeries, sScan);
	return sFolder;
}


BOOL CGlobalFuncs::IsFileExist(CString sFileName)
{
	FILE *fp = fopen(sFileName.GetBuffer(1), "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	else 
	{
		fclose(fp);
		return TRUE;
	}
}

int CGlobalFuncs::LoadProbeFile(CString sFilePath, double dOri[3], double dTip[3], double &dLength, double &dDiameter)
{
	FILE *fp;
	fp = fopen(sFilePath, "r");
	if (fp == NULL)
	{
		PanaMessageBox("Can't open file !");
		return 0;
	}
	fscanf(fp, "%lf%lf%lf\n", &dTip[0], &dTip[1], &dTip[2]);
	fscanf(fp, "%lf%lf%lf\n", &dOri[0], &dOri[1], &dOri[2]);
	fscanf(fp, "%lf\n",&dLength);
	fscanf(fp, "%lf\n",&dDiameter);
	fclose(fp);
	return 1;
}

void CGlobalFuncs::StartLog()
{
	// begin log
	CConfigMgr conf;
	CString sLogPath = conf.ReadREG(HKEY_LOCAL_MACHINE, "SOFTWARE\\XinaoMDT\\Panasee", "Log", "C:\\MRILog");
	CreateDirectory(sLogPath, 0);
	CString strWildcard;
	strWildcard.Format("%s\\*.log", sLogPath);
	CTime c = CTime::GetCurrentTime();
	m_sLogPicFolder.Format("%s\\%d%02d%02d-%02d%02d", sLogPath, c.GetYear(), c.GetMonth(), c.GetDay(), c.GetHour(), c.GetMinute());
	CString sLogFile;
	sLogFile.Format("%s.log", m_sLogPicFolder);

	CLog::StartLog(sLogFile);

	CString sVersion = CGlobalFuncs::GetProductVersion(NULL);
	CString sTmp;
	sTmp.Format("Panasee %s start.",sVersion);
	CLog::Log(sTmp);
}

BOOL CGlobalFuncs::SaveBitmapFile(CString strFileName,CBitmap& bmp)
{
	BITMAP bm;
	bmp.GetBitmap (&bm);
	
	int nWidth = bm.bmWidth;
	int nHeight = bm.bmHeight;
	
	int nLineBits = bm.bmWidthBytes;
	if ((nLineBits % 8) != 0)
		nLineBits += 8 - nLineBits%8;

	int nBitCounts = nLineBits * bm.bmHeight ;

	int nBits = bm.bmBitsPixel;
	int nBitmapInfoSize = 0;

	if (nBits <= 8)
	{
		int nColors = 1 << nBits;
		int nPalUnitSize = sizeof(RGBQUAD);
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + nPalUnitSize * nColors;	
	}
	else
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER);

	BITMAPINFO*	pbmpinfo = NULL;

	pbmpinfo =(BITMAPINFO *)(new BYTE[nBitmapInfoSize]);
	ZeroMemory((void *)pbmpinfo,nBitmapInfoSize);
	
	BITMAPINFOHEADER* pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
	ZeroMemory((void *)pInfoHead,sizeof(BITMAPINFOHEADER));

	pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	pInfoHead->biBitCount = nBits;
	pInfoHead->biPlanes = 1;
	pInfoHead->biSizeImage = nBitCounts;
	pInfoHead->biCompression = BI_RGB;
	pInfoHead->biWidth = nWidth;
	pInfoHead->biHeight = nHeight;

	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);

	int nLx = dc.GetDeviceCaps (LOGPIXELSX);
	int nLy = dc.GetDeviceCaps (LOGPIXELSY);

	double dbInchPerMeter = 39.375;

	int nPMx = (int)((double)nLx * dbInchPerMeter);
	int nPMy = (int)((double)nLy * dbInchPerMeter);

	pInfoHead->biXPelsPerMeter = nPMx;//3780;
	pInfoHead->biYPelsPerMeter = nPMy;//3780;
	
	if (nBits <= 8)
	{
		int nColors = 1 << nBits;
		GetDIBColorTable(dc.m_hDC,0,nColors,pbmpinfo->bmiColors);
	}

	BYTE* pBits = NULL;
	
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED, nBitCounts);
	pBits = (BYTE *)::GlobalLock(hGlobal);
	ZeroMemory((void *)pBits,nBitCounts);
	
	GetDIBits(dc.m_hDC,(HBITMAP)bmp,0,nHeight,pBits,pbmpinfo,DIB_RGB_COLORS);

	BITMAPFILEHEADER	bmfHeader;
	int nFileHeadSize = sizeof(BITMAPFILEHEADER);
	
	ZeroMemory((void *)&bmfHeader,sizeof(BITMAPFILEHEADER));
	bmfHeader.bfType = 19778;//'BM'    ((WORD) ('M' << 8) | 'B'); 
	bmfHeader.bfSize = nFileHeadSize + nBitmapInfoSize + nBitCounts;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize;

	CFile file;
	if(!file.Open (strFileName,CFile::modeCreate | CFile::modeWrite))
	{
		PanaMessageBox("Write File Error!");
		return FALSE;
	}

	file.Write (&bmfHeader,sizeof(BITMAPFILEHEADER));
	file.Write (pbmpinfo,nBitmapInfoSize);

	file.WriteHuge (pBits, nBitCounts);
	file.Close ();

	::GlobalUnlock (hGlobal);
	::GlobalFree (hGlobal);
	delete[] pbmpinfo;

	return TRUE;
}

CString   CGlobalFuncs::GetProductVersion(LPCTSTR   lpszFileName)   
  /*========================================================================   
  功能: 获取文件版本信息.   
  说明: 要使用此函数必需在程序中加入   
  #pragma   comment(lib,   "Version.lib")   
  ----------------------------------------------------------------------------   
  参数: lpszFileName = 程序文件名,如果为空则是获取当前运行程序的版本信息   
  ==========================================================================*/   
  {   
  CString   strVersion;   
  TCHAR   szSrcfn[MAX_PATH]; // 要获取信息的文件   
  if(lpszFileName   ==   NULL)   
  ::GetModuleFileName(NULL,   szSrcfn,   sizeof(szSrcfn));   
  else   
  lstrcpy(szSrcfn,   lpszFileName);   
    
  // 读文件信息   
  DWORD   dwVerHnd =   0;   
          DWORD   dwVerInfoSize   =   ::GetFileVersionInfoSize(szSrcfn,   &dwVerHnd);   
          if(dwVerInfoSize)   
          {   
                  //   If   we   were   able   to   get   the   information,   process   it:   
                  HANDLE     hMem;   
                  LPVOID     lpvMem;   
  unsigned   int   uInfoSize   =   0;   
  VS_FIXEDFILEINFO   *   pFileInfo;   
    
                  hMem =   ::GlobalAlloc(GMEM_MOVEABLE,   dwVerInfoSize);   
                  lpvMem =   ::GlobalLock(hMem);   
                  ::GetFileVersionInfo(szSrcfn,   dwVerHnd,   dwVerInfoSize,   lpvMem);   
  ::VerQueryValue(lpvMem,   (LPTSTR)_T("\\"),   (void**)&pFileInfo,   &uInfoSize);   
    
  WORD   nVer[4];   
  nVer[0]   =   HIWORD(pFileInfo->dwProductVersionMS);     
  nVer[1]   =   LOWORD(pFileInfo->dwProductVersionMS);   
  nVer[2]   =   HIWORD(pFileInfo->dwProductVersionLS);   
  nVer[3]   =   LOWORD(pFileInfo->dwProductVersionLS);     
  strVersion.Format(_T("%d.%d.%d.%d"),   nVer[0],   nVer[1],nVer[2],nVer[3]);     
    
  ::GlobalUnlock(hMem);   
                  ::GlobalFree(hMem);   
  }   
    
  return   strVersion;   
  }



CFont* CGlobalFuncs::GetFont()
{
	if(g_pTxtFont==NULL)
	{
		g_pTxtFont = new CFont;
		g_pTxtFont->CreateFont(-11,0,0,0,FW_NORMAL,0,0,0,134,0,0,0,0,"Tahoma"); 
	}
	return g_pTxtFont;
}

CFont* CGlobalFuncs::GetFont40()
{
	if(g_pTxtFont40==NULL)
	{
		g_pTxtFont40 = new CFont;
		g_pTxtFont40->CreateFont(40,20,0,0,700,0,0,0,134,0,0,0,0,"楷体_GB2312");  
	}
	return g_pTxtFont40;
}

CFont* CGlobalFuncs::GetFont20()
{
	if(g_pTxtFont20==NULL)
	{
		g_pTxtFont20 = new CFont;
		g_pTxtFont20->CreateFont(20,10,0,0,700,0,0,0,134,0,0,0,0,"楷体_GB2312");
	}
	return g_pTxtFont20;
}

void CGlobalFuncs::Clean()
{
	if(g_pTxtFont)
	{
		delete g_pTxtFont;
		g_pTxtFont=NULL;
	}

	if(g_pTxtFont20)
	{
		delete g_pTxtFont20;
		g_pTxtFont20=NULL;
	}

	if(g_pTxtFont40)
	{
		delete g_pTxtFont40;
		g_pTxtFont40=NULL;
	}
}