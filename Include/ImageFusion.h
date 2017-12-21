// ImageFusion.h: interface for the CImageFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEFUSION_H__CF8F593C_9633_4BB6_9177_9DDB02DD2371__INCLUDED_)
#define AFX_IMAGEFUSION_H__CF8F593C_9633_4BB6_9177_9DDB02DD2371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_IMGFUSION_DLL
	#define IMGFUSION_API		__declspec(dllexport)
#else
	#define IMGFUSION_API		 __declspec(dllimport)
#endif



class IMGFUSION_API CImageFusion  
{
public:
	void ShowFusionWnd(CWnd *parentWnd);
	CImageFusion();
	virtual ~CImageFusion();

};

#endif // !defined(AFX_IMAGEFUSION_H__CF8F593C_9633_4BB6_9177_9DDB02DD2371__INCLUDED_)
