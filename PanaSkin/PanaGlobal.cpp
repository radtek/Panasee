#include "stdafx.h"
#include "PanaGlobal.h"
#include "PanaMsgBox.h"
#include "DlgWaiting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_EXTENSION_MODULE PanaSkinDLLModule;

PANASKIN_API int PanaMessageBox(LPCTSTR lpszText, UINT nType, UINT nIDHelp )
{
	HINSTANCE hOldHangle=AfxGetResourceHandle();
	AfxSetResourceHandle(PanaSkinDLLModule.hModule);

	CPanaMsgBox msgBox;
	msgBox.SetMessage(lpszText, nType);
	int iReturn=msgBox.DoModal();

	AfxSetResourceHandle(hOldHangle);
	return iReturn;
}

PANASKIN_API int PanaMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	HINSTANCE hOldHangle=AfxGetResourceHandle();
	AfxSetResourceHandle(PanaSkinDLLModule.hModule);

	CPanaMsgBox msgBox;
	msgBox.SetTitle(lpszCaption);
	msgBox.SetMessage(lpszText, nType);
	int iReturn=msgBox.DoModal();

	AfxSetResourceHandle(hOldHangle);
	return iReturn;
}

static BOOL m_bWaitInited=FALSE;
CDlgWaiting m_dlgWait;

PANASKIN_API void ShowWaiting(int i, LPCTSTR lpszText)
{
	HINSTANCE hOldHangle=AfxGetResourceHandle();
	AfxSetResourceHandle(PanaSkinDLLModule.hModule);

	CString str;
	str.Format("%s",lpszText);

	if(!m_bWaitInited)
	{
		m_dlgWait.Create(IDD_DLG_WAITING,NULL);
		m_dlgWait.ShowWindow(FALSE);
		m_bWaitInited=TRUE;
	}
	
	if(i<0 || i>=100)
	{
		HCURSOR m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		::SetCursor(m_hCursor);
		m_dlgWait.ShowWindow(FALSE);
	}else
	{
		HCURSOR m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);
		::SetCursor(m_hCursor);
		m_dlgWait.ShowWindow(TRUE);
		m_dlgWait.showText(str);
	}

	AfxSetResourceHandle(hOldHangle);
}

PASK_DEVICE_TYPE m_deviceType = PASK_DEVICE_IGS_MF;

PANASKIN_API void SetDeviceType(PASK_DEVICE_TYPE type)
{
	if(type == PASK_DEVICE_IGS_MF || PASK_DEVICE_IGS_CP)
	{
		m_deviceType = type;
	}else
	{
		m_deviceType = PASK_DEVICE_IGS_MF;
	}
}