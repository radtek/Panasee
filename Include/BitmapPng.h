/////////////////////////////////////////////////////////////////////////////

//{{AFX_PRIVATE
#if !defined(__GRAPHICBITMAPPNG_H__)
#define __GRAPHICBITMAPPNG_H__
//}}AFX_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*********** ningsj *********************
 说明:读取png文件的类
*****************************************/

#ifdef _BUILD_PNGBITMAP_LIB
	#define PNGBITMAP_API		__declspec(dllexport)
#else
	#define PNGBITMAP_API		 __declspec(dllimport)


	//////////////////////////////////////////////////////
	// setup the library we need to link with...
	#if defined(_DEBUG)
		#pragma comment(lib, "PngLibD.lib" )
	#else
		#pragma comment(lib, "PngLIb.lib" )
	#endif

#endif //_BUILD_PNGBITMAP_LIB 


class CBitmapPng : public CBitmap
{
public:
	CBitmapPng();

public:
	BOOL LoadFromFile(LPCTSTR lpszFileName);
	BOOL LoadFromResource(HMODULE hModule, HRSRC hRes);

	BOOL LoadFromFile(CFile* pFile);

	BOOL IsAlpha() const;

private:
	HBITMAP ConvertToBitmap(BYTE* pbImage, CSize szImage,  int cImgChannels) const;

protected:
	BOOL m_bAlpha;
};

AFX_INLINE BOOL CBitmapPng::IsAlpha() const {
	return m_bAlpha;
}

#endif // #if !defined(_GRAPHICBITMAPPNG_H__)
