// ImageManager.h: interface for the CImageManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEMANAGER_H__FA607453_8001_4605_A6C8_5E3707188673__INCLUDED_)
#define AFX_IMAGEMANAGER_H__FA607453_8001_4605_A6C8_5E3707188673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BitmapPng.h"

/*********** ningsj *********************
 说明:管理图像的打开、关闭等操作，避免重复打开同一幅图像
      提高程序运行速度
*****************************************/
class CImageManager  
{
public:
	CBitmapPng* LoadImage(CString sFilePath,BOOL &bAlpha);
	
	CImageManager();
	virtual ~CImageManager();

	static CImageManager* GetImageManager(void)
	{
		static CImageManager manager;
		return &manager;
	}

	CBitmapPng * LoadImage(UINT ID,BOOL &bAlpha);
	CBitmapPng * LoadImage(HMODULE hModule,UINT ID,BOOL &bAlpha);

private:
	BOOL IsPngBitmapResource(HMODULE hModule,UINT IDBitmap);
	BOOL IsAlphaBitmapResource(HMODULE hModule, LPCTSTR lpBitmapName);
	HBITMAP PreMultiplyAlphaBitmap(HBITMAP hBitmap, BOOL* pbAlpha); //, LPBYTE* lpBits);
	BOOL GetBitmapBits(CDC& dcSrc, HBITMAP hBitmap, PBITMAPINFO& pBitmapInfo, LPVOID& pBits, UINT& nSize);
	
	typedef struct _IMAGE_INDEX
	{
		UINT ID;
		HMODULE hModule;
		CString sFilePath;
	}IMAGE_INDEX;
	CList<BOOL,BOOL> m_alphaList;
	CList<IMAGE_INDEX, IMAGE_INDEX> m_IDList;
	CList<CBitmapPng*,CBitmapPng*> m_imageList;
};

#endif // !defined(AFX_IMAGEMANAGER_H__FA607453_8001_4605_A6C8_5E3707188673__INCLUDED_)
