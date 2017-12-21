// ImageManager.cpp: implementation of the CImageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AFX_EXTENSION_MODULE PanaSkinDLLModule;

CImageManager::CImageManager()
{
	m_imageList.RemoveAll();
	m_IDList.RemoveAll();
	m_alphaList.RemoveAll();
}

CImageManager::~CImageManager()
{
	CBitmapPng *pImage;

	int iCount=m_imageList.GetCount();
	POSITION pos=m_imageList.GetHeadPosition();
	for(int i=0;i<iCount;i++)
	{
		pImage=m_imageList.GetNext(pos);
		if(pImage)
		{
			HBITMAP hBmp=(HBITMAP)pImage->Detach();
			if(hBmp!=NULL)
				DeleteObject(hBmp);
			pImage->DeleteObject();
			delete pImage;
		}
	}

	m_imageList.RemoveAll();
	m_IDList.RemoveAll();
	m_alphaList.RemoveAll();

}

CBitmapPng * CImageManager::LoadImage(UINT ID,BOOL &bAlpha)
{
	HMODULE hModule=PanaSkinDLLModule.hModule;
	return LoadImage(hModule,ID,bAlpha);
}

CBitmapPng * CImageManager::LoadImage(HMODULE hModule,UINT ID,BOOL &bAlpha)
{
	
	CBitmapPng *pImage;
	IMAGE_INDEX imageIndex;
	int iCount=m_imageList.GetCount();
	POSITION posImage=m_imageList.GetHeadPosition();
	POSITION posAlpha=m_alphaList.GetHeadPosition();
	POSITION posID=m_IDList.GetHeadPosition();
	for(int i=0;i<iCount;i++)
	{
		pImage=m_imageList.GetNext(posImage);
		bAlpha=m_alphaList.GetNext(posAlpha);
		imageIndex=m_IDList.GetNext(posID);

		if(imageIndex.ID==ID && imageIndex.hModule==hModule)
			return pImage;
	}

	bAlpha = FALSE;

	pImage=new CBitmapPng();

	BOOL bPngBitmap = IsPngBitmapResource(hModule,ID);
	if (bPngBitmap)
	{
		HRSRC hResource = ::FindResource(hModule, MAKEINTRESOURCE(ID), _T("PNG"));

		if (!pImage->LoadFromResource(hModule, hResource))
		{
			delete pImage;
			return NULL;
		}

		bAlpha = pImage->IsAlpha();
	}else
	{
		if(!pImage->LoadBitmap(ID))
		{
			delete pImage;
			return NULL;
		}

		bAlpha=IsAlphaBitmapResource(hModule, MAKEINTRESOURCE(ID));
		
	}

	if(bAlpha)
	{
		HBITMAP hAlphaBmp,hBmp=(HBITMAP)pImage->Detach();
		hAlphaBmp = PreMultiplyAlphaBitmap(hBmp, &bAlpha);
		
		DeleteObject(hBmp);
		pImage->Attach(hAlphaBmp);
	}

	imageIndex.ID=ID;
	imageIndex.hModule=hModule;
	imageIndex.sFilePath="";
	m_IDList.AddTail(imageIndex);
	m_imageList.AddTail(pImage);
	m_alphaList.AddTail(bAlpha);

	return pImage;
}


BOOL CImageManager::IsPngBitmapResource(HMODULE hModule,UINT IDBitmap)
{
	BYTE png_signature[4] = {137, 80, 78, 71};

	HRSRC hResource = ::FindResource(hModule, MAKEINTRESOURCE(IDBitmap), _T("PNG"));

	if (hResource == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hModule, hResource);
	if (hGlobal == NULL)
		return FALSE;

	LPBYTE pBitmapInfoHeader = (LPBYTE)::LockResource(hGlobal);
	BOOL bPngBitmap = memcmp(pBitmapInfoHeader, &png_signature, 4) == 0;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return (bPngBitmap);
}

BOOL CImageManager::IsAlphaBitmapResource(HMODULE hModule, LPCTSTR lpBitmapName)
{
	HRSRC hResource = ::FindResource(hModule, lpBitmapName, RT_BITMAP);

	if (hResource == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hModule, hResource);
	if (hGlobal == NULL)
		return FALSE;

	LPBITMAPINFOHEADER pBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hGlobal);
	ASSERT(pBitmapInfoHeader != NULL);
	if (!pBitmapInfoHeader)
		return FALSE;

	BOOL bAlpahBitmap = pBitmapInfoHeader->biBitCount == 32;
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return (bAlpahBitmap);
}

HBITMAP CImageManager::PreMultiplyAlphaBitmap(HBITMAP hBitmap, BOOL* pbAlpha)
{
	//if (lpBits) *lpBits = NULL;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PBYTE pBitsSrc = 0;
	PBITMAPINFO pBitmapInfo = 0;

	HBITMAP hBitmapAlpha = 0;

	TRY
	{
		UINT nSize;
		if (!GetBitmapBits(dcSrc, hBitmap, pBitmapInfo, (LPVOID&)pBitsSrc, nSize))
			return 0;

		BYTE* pBitsDest = NULL;

		hBitmapAlpha = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pBitsDest, NULL, 0);
		if (pBitsDest == NULL || hBitmapAlpha == NULL)
			AfxThrowMemoryException();

		memcpy(pBitsDest, pBitsSrc, nSize);

		BOOL bAlpha0Found = FALSE;

		BYTE* pBitsI = pBitsDest;
		UINT nCount = nSize / 4;

		for (UINT i = 0; i < nCount; i++)
		{
			int nAlpha = pBitsI[3];
			pBitsI[0] = BYTE(pBitsI[0] * nAlpha / 255);
			pBitsI[1] = BYTE(pBitsI[1] * nAlpha / 255);
			pBitsI[2] = BYTE(pBitsI[2] * nAlpha / 255);

			if (pbAlpha)
			{
				if (nAlpha == 0)
					bAlpha0Found = TRUE;

				if ((nAlpha != 255 && nAlpha != 0) || (nAlpha == 255 && bAlpha0Found) || (nAlpha == 255 && *pbAlpha == -1))
					*pbAlpha = TRUE;
			}
			pBitsI += 4;
		}

		if (pbAlpha && (*pbAlpha == FALSE))
		{
			memcpy(pBitsDest, pBitsSrc, nSize);
		}

		//if (lpBits)
		//{
		//	*lpBits = pBitsDest;
		//}

	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
	}
	END_CATCH

	free(pBitsSrc);
	free(pBitmapInfo);

	return hBitmapAlpha;
}

BOOL CImageManager::GetBitmapBits(CDC& dcSrc, HBITMAP hBitmap, PBITMAPINFO& pBitmapInfo, LPVOID& pBits, UINT& nSize)
{
	if (hBitmap == 0)
		return FALSE;

	BITMAPINFO  bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);


	if (GetDIBits(dcSrc, hBitmap, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
		return FALSE;

	if (bmi.bmiHeader.biBitCount != 32)
		return FALSE;

	nSize = bmi.bmiHeader.biHeight * bmi.bmiHeader.biWidth * 4;
	pBits = (PBYTE)malloc(nSize);
	if (pBits == NULL)
		return FALSE;

	if ((pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + 3 * sizeof(COLORREF))) == NULL)
	{
		free(pBits);
		return FALSE;
	}

	memcpy(pBitmapInfo, &bmi, sizeof(BITMAPINFOHEADER));

	if (GetDIBits(dcSrc, hBitmap, 0, bmi.bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS) == NULL)
	{
		free(pBits);
		free(pBitmapInfo);
		return FALSE;
	}
	return TRUE;
}

CBitmapPng* CImageManager::LoadImage(CString sFilePath, BOOL &bAlpha)
{
	CBitmapPng *pImage;
	IMAGE_INDEX imageIndex;
	int iCount=m_imageList.GetCount();

	POSITION posImage=m_imageList.GetHeadPosition();
	POSITION posAlpha=m_alphaList.GetHeadPosition();
	POSITION posID=m_IDList.GetHeadPosition();
	for(int i=0;i<iCount;i++)
	{
		pImage=m_imageList.GetNext(posImage);
		bAlpha=m_alphaList.GetNext(posAlpha);
		imageIndex=m_IDList.GetNext(posID);

		if(imageIndex.sFilePath==sFilePath)
			return pImage;
	}

	bAlpha = FALSE;
	pImage=new CBitmapPng();
	
	if(!pImage->LoadFromFile(sFilePath))
	{
		delete pImage;
		return NULL;
	}

	bAlpha = pImage->IsAlpha();
	
	if(bAlpha)
	{
		HBITMAP hAlphaBmp,hBmp=(HBITMAP)pImage->Detach();
		hAlphaBmp = PreMultiplyAlphaBitmap(hBmp, &bAlpha);
		
		DeleteObject(hBmp);
		pImage->Attach(hAlphaBmp);
	}

	imageIndex.ID=0;
	imageIndex.hModule=0;
	imageIndex.sFilePath=sFilePath;
	m_IDList.AddTail(imageIndex);
	m_imageList.AddTail(pImage);
	m_alphaList.AddTail(bAlpha);

	return pImage;
}
