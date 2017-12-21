// Surgery.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Surgery.h"

#include "DlgMain.h"
#include <initguid.h>
#include "ConfigMgr.h"
#include "DlgMyAbout.h"
#include "GlobalFuncs.h"
#include "Config.h"
#include "DatabaseConfig.h"
#include "LicApi.h"
//#include "XTToolkitPro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern OPERATIONTYPE gStarOperType;    // get from command line argument           

extern CImriDBI m_DBmri;

extern char g_cCrashFile[MAX_PATH];  //File to store crash restore information

CString g_ExecPath;

CString SROM_FILES[] = {
	"\\Config\\SROM\\Base1.rom",
	"\\Config\\SROM\\Base2.rom",
	"\\Config\\SROM\\Base3.rom",
	"\\Config\\SROM\\Base4.rom",
	"\\Config\\SROM\\Phantom.rom",
	"\\Config\\SROM\\Probe.rom",
	"\\Config\\SROM\\PT.rom"
};

CString CONFIG_FILES[] = {
	"\\Config\\Base\\Base1.mtx",
	"\\Config\\Base\\Base2.mtx",
	"\\Config\\Base\\Base3.mtx",
	"\\Config\\Base\\Base4.mtx",
	"\\Config\\Phantom\\Phantom.asc",
	"\\Config\\PT\\PT.txt",
	"\\Config\\Probe\\PreImage.PIN",
	"\\Config\\Probe\\CaliProbe.PIN",
	"\\Config\\Probe\\cryotype.txt",
	"\\Config\\Probe\\biotype.txt",
	"\\Config\\Base\\Marker2PT.mtx"
};

// Following folder need to be created.
CString CONFIG_FOLDER[] = {
	"\\Config\\Base",
	"\\Config\\PT"
};
/////////////////////////////////////////////////////////////////////////////
// CSurgeryApp

BEGIN_MESSAGE_MAP(CSurgeryApp, CWinApp)
	//{{AFX_MSG_MAP(CSurgeryApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurgeryApp construction

CSurgeryApp::CSurgeryApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSurgeryApp object

CSurgeryApp theApp;
CDlgMain *g_DlgMain;

WNDPROC oldMenuWndProc = NULL; 
/*********** ningsj *********************
 说明:菜单钩子函数
 功能:去掉菜单的边框
      过滤某种颜色，使菜单窗口局部透明
*****************************************/
LRESULT CALLBACK MenuWndProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam) 
{ 
	LRESULT lResult;

	
	switch (message) 
	{ 

	//首先要去掉菜单窗口的WS_BORDER风格和WS_EX_DLGMODALFRAME , WS_EX_WINDOWEDGE扩展风格 
	case WM_CREATE: 
	{ 
		lResult = CallWindowProc(oldMenuWndProc, hWnd, message, wParam, lParam); 
		
		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE); 
		DWORD dwNewStyle = (dwStyle & ~WS_BORDER); 
		//::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle); 
		
		DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE); 
		DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE)); 
		::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewExStyle); 

		/*
		::SetWindowLong(hWnd,GWL_EXSTYLE,
			::GetWindowLong(hWnd,GWL_EXSTYLE)^0x80000); 
		HINSTANCE hInst = LoadLibrary("User32.DLL"); 
		if(hInst) 
		{ 
			typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
			MYFUNC fun = NULL; 
			//取得SetLayeredWindowAttributes函数指针 
			fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
			if(fun) 
				fun(hWnd,RGB(255,0,0),255,1); //过滤掉窗口中的红色

			FreeLibrary(hInst); 
		} */

		return lResult; 
		break; 
	} 
	
	//这里/阻止非客户区的绘制 
	case WM_PRINT: 
		return CallWindowProc (oldMenuWndProc, hWnd, WM_PRINTCLIENT, wParam, lParam); 
		break; 

	//最后, 因为你在MeasureItem里指定的菜单大小，系统会自动替你加上边框，我们必须去掉这部分额外的尺寸 
	case WM_WINDOWPOSCHANGING: 
	{ 
		//将菜单大小改小 
		LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam; 
		lpPos->cx -= 2*GetSystemMetrics(SM_CXBORDER)+2; 
		lpPos->cy -= 2*GetSystemMetrics(SM_CYBORDER)+2; 
		lResult = CallWindowProc(oldMenuWndProc, hWnd, message, wParam, lParam); 
		
		return lResult; 
	} 
	case WM_GETICON: 

		return 0; 
		break; 
	default: 
		return CallWindowProc (oldMenuWndProc, hWnd, message, wParam, lParam); 
	} 
} 

/*********** ningsj *********************
 说明:设置菜单钩子函数
 功能:去掉菜单的边框
      过滤某种颜色，使菜单窗口局部透明
*****************************************/
LRESULT CALLBACK WindowsHook (int code, WPARAM wParam, LPARAM lParam) 
{ 
	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam; 

	while ( code == HC_ACTION ) 
	{ 
		HWND hWnd = pStruct->hwnd; 

		if ( pStruct->message != WM_CREATE && pStruct->message != 0x01E2) 
			break; 
		TCHAR sClassName[10]; 
		int Count = ::GetClassName (hWnd, sClassName, sizeof(sClassName)/sizeof(sClassName[0])); 

		
		//检查是否菜单窗口 
		if ( Count == 6 && _tcscmp (sClassName, _T("#32768")) == 0 ) 
		{ 
			WNDPROC lastWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC); 
			if (lastWndProc != MenuWndProc) 
			{ 
				//替换菜单窗口过程函数 
				SetWindowLong(hWnd, GWL_WNDPROC, (long)MenuWndProc); 


				//保留旧的窗口过程 
				oldMenuWndProc = lastWndProc; 
			} 
			break; 
		} 
		
		break; 
	} 
	return CallNextHookEx ((HHOOK)WH_CALLWNDPROC, code, wParam, lParam); 
} 

/////////////////////////////////////////////////////////////////////////////
// CSurgeryApp initialization

BOOL CSurgeryApp::InitInstance()
{
	
	if( !AfxOleInit() )
		TRACE("Ole init failed");
	
	AfxEnableControlContainer();
	AfxInitRichEdit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	{
		char
			cExecPath[MAX_PATH];  //路径
		char 
			*pszPathofFile;
		if(GetModuleFileName( (HMODULE)AfxGetInstanceHandle(),  cExecPath, sizeof(cExecPath)))
		{	
			pszPathofFile = strrchr(cExecPath, '\\'); /* remove the current EXE name from the path */
			if(pszPathofFile)
			{		
				*(pszPathofFile + 1) = '\0';		
			} /* if */
		} /* if */
		char pszFileName[MAX_PATH];
		sprintf( pszFileName, "%s\\Surgery.INI", cExecPath);
		SetINIFileName(pszFileName);

		sprintf(g_cCrashFile, "%s\\Restore.INI", cExecPath);
		g_ExecPath = cExecPath;
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if ( strcmp(cmdInfo.m_strPrinterName, "") != 0)
	{
		int iType = atoi(cmdInfo.m_strPrinterName);
		if (iType > OPERATION_TYPE_NONE && iType <= OPERATION_TYPE_REVIEW)
		{
			gStarOperType = (OPERATIONTYPE)iType;
		}else
		{
			gStarOperType = OPERATION_TYPE_REVIEW;
		}
	}
	else
	{
		gStarOperType = OPERATION_TYPE_REVIEW;
	}


	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}

	CConfigMgr conf;
	PATIENT_MARKER NavigationMode = (PATIENT_MARKER)conf.Read(SYSINFO, SYS_NAVI_MODE, 0);
	if(NavigationMode == PATIENT_NO_MARKER)
	{
		SROM_FILE_NUM=6;
	}
	if(NavigationMode == PATIENT_WITH_MARKER)
	{
		SROM_FILE_NUM=7;
	}

	CString sRootFolder = g_ExecPath.Left(g_ExecPath.ReverseFind('\\'));
	sRootFolder = sRootFolder.Left(sRootFolder.ReverseFind('\\'));
	for (int i=0; i<CF_CONFIG_FILE_SUM; i++)
	{
		CONFIG_FILES[i] = sRootFolder + CONFIG_FILES[i];
	}
	for (i=0; i<SROM_FILE_NUM; i++)
	{
		SROM_FILES[i] = sRootFolder + SROM_FILES[i];
	}
	for (i=0; i<2; i++)
	{
		CONFIG_FOLDER[i] = sRootFolder + CONFIG_FOLDER[i];
		CreateDirectory(CONFIG_FOLDER[i], 0);
	}

	if (!SysValidate())
	{
		return FALSE;
	}


	//钩子函数，去掉菜单边框
	::SetWindowsHookEx (WH_CALLWNDPROC, WindowsHook, m_hInstance, ::GetCurrentThreadId()); 

	int DeviceType = conf.Read(SYSINFO, SYS_DEVICE_TYPE, 1);
	if(DeviceType!=DEVICE_IGS_MF && DeviceType!=DEVICE_IGS_CP)
	{
		DeviceType = DEVICE_IGS_MF;
	}

	CDlgMain dlg;
	g_DlgMain=&dlg;
	dlg.m_DeviceType=DeviceType;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return TRUE;
}


// App command to run the dialog
void CSurgeryApp::OnAppAbout()
{

}

/////////////////////////////////////////////////////////////////////////////
// CSurgeryApp message handlers


int CSurgeryApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_af.Remove(); //deleted by ningsj 2009.2.9
	
	
	return CWinApp::ExitInstance();
}


void CSurgeryApp::OnHelpAbout() 
{
	CDlgMyAbout aboutDlg;
	aboutDlg.DoModal();
}


BOOL CSurgeryApp::SysValidate()
{
	// Check license
	BOOL bValid = FALSE;

	//如果C盘卷标是xinaomdt，则不进行license验证
	TCHAR szStylesPath[_MAX_PATH];
	CString sDriver="c:\\";	
	if(GetVolumeInformation(sDriver,szStylesPath,20,NULL,NULL,NULL,NULL,10))
	{

		CString sVolume=szStylesPath;
		sVolume.MakeUpper();
		if(sVolume=="XINAOMDT")
		{
			bValid = TRUE;
		}
	}
	if(!bValid)
	{
		CLicApi lic;
		bValid =  lic.Validate("Panasee");
	}

	if (!bValid)
		return FALSE;
	
	// Validate database
	if(m_DBmri.Validate()!=TRUE)
	{
		CDatabaseConfig conf;
		if (conf.DoModal() != IDOK)
		{
			return FALSE;
		}
	}

	// Validate files
	if (!(gStarOperType < OPERATION_TYPE_TPS && gStarOperType > OPERATION_TYPE_NONE))
	{
	}
	else 
	{
		CStringArray arrMissFile;
		CString sFileName;
		for (int i=0; i<SROM_FILE_NUM; i++)
		{
			sFileName = SROM_FILES[i];
			if (!CGlobalFuncs::IsFileExist(sFileName))
			{
				arrMissFile.Add(sFileName.Right(sFileName.GetLength() - sFileName.ReverseFind('\\') - 1));
			}
		}

		sFileName = CONFIG_FILES[CF_PROBE_CALI];
		if (!CGlobalFuncs::IsFileExist(sFileName))
		{
			arrMissFile.Add(sFileName.Right(sFileName.GetLength() - sFileName.ReverseFind('\\') - 1));
		}

		sFileName = CONFIG_FILES[CF_PROBE_PREIMAGE];
		if (!CGlobalFuncs::IsFileExist(sFileName))
		{
			arrMissFile.Add(sFileName.Right(sFileName.GetLength() - sFileName.ReverseFind('\\') - 1));
		}

		switch (gStarOperType)
		{
		case OPERATION_TYPE_BIOPSY:
			sFileName = CONFIG_FILES[CF_PROBE_BIOTYPE];
			break;
		case OPERATION_TYPE_CRYOTHERAPY:
			sFileName = CONFIG_FILES[CF_PROBE_CRYOTYPE];
			break;
		default:
			sFileName = "";
			break;
		}
		if (!CGlobalFuncs::IsFileExist(sFileName))
		{
			arrMissFile.Add(sFileName.Right(sFileName.GetLength() - sFileName.ReverseFind('\\') - 1));
		}
		
		if (arrMissFile.GetSize() > 0)
		{
			CString sMsg = "Following file is missing. Please install software again or contact provider.\n";
			for (int i=0; i<arrMissFile.GetSize(); i++)
			{
				sMsg += "  " + arrMissFile[i];
			}
			PanaMessageBox(sMsg);
			return FALSE;
		}
	}

	return TRUE;
}
