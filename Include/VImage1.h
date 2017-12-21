// VImage1.h: interface for the CVImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIMAGE1_H__3F1CE88A_D598_47E8_93B6_2EC1A62D8AC0__INCLUDED_)
#define AFX_VIMAGE1_H__3F1CE88A_D598_47E8_93B6_2EC1A62D8AC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_VIMAGE_DLL
	#define VIMAGE_API		__declspec(dllexport)
#else
	#define VIMAGE_API		__declspec(dllimport)
#endif
 
class VIMAGE_API CVImage  
{
public:
	void Clear();
	void ShowContourDlg(void *pImageInfo,CWnd *pWnd);
	void ShowSegmentDlg(void *pImageInfo, CWnd *pWnd);
	void ShowVolumeDlg(void *pImageInfo, void *pConfig,CWnd *pWnd);

	CVImage();
	virtual ~CVImage();

private:
	void* GetImage(void* pImageInfo);
};

#endif // !defined(AFX_VIMAGE1_H__3F1CE88A_D598_47E8_93B6_2EC1A62D8AC0__INCLUDED_)
