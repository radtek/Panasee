// Show3DSeries.h: interface for the CShow3DSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOW3DSERIES_H__2D4827B2_7318_44BF_B90D_3B20AD2E9A3A__INCLUDED_)
#define AFX_SHOW3DSERIES_H__2D4827B2_7318_44BF_B90D_3B20AD2E9A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _BUILD_VIMAGE_DLL
	#define VIMAGE_API		__declspec(dllexport)
#else
	#define VIMAGE_API		__declspec(dllimport)
#endif

class VIMAGE_API CShow3DSeries  
{
public:
	CWnd* GetVolumeDlg();
	CWnd* GetSurfaceDlg();
	void ShowCtrlPanel(BOOL bShow);
	void ShowWindow(BOOL bShow);
	void MoveSurfaceRenderDlg(int left,int top,int width,int height);
	void MoveVolumeRenderDlg(int left,int top,int width,int height);
	CWnd* ShowVolumeRenderDlg(void *pImageInfo, CWnd *pWnd);
	CWnd* ShowSurfaceRenderDlg(void *pImageInfo, CWnd *pWnd);

	CShow3DSeries();
	virtual ~CShow3DSeries();

private:
	void Clear();
	void* GetImage(void* pImageInfo);
	void* m_pSeries;
	
	CWnd *m_pSRWnd,*m_pVRWnd;

};

#endif // !defined(AFX_SHOW3DSERIES_H__2D4827B2_7318_44BF_B90D_3B20AD2E9A3A__INCLUDED_)
