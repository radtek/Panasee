// DlgTime.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTime.h"
#include "GlobalFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTime dialog

UINT TimeThread(LPVOID pParam);

CDlgTime::CDlgTime(CWnd* pParent /*=NULL*/)
	: CDlgTimeSkin(CDlgTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgTime::~CDlgTime()
{
//	StopTime();
}


void CDlgTime::DoDataExchange(CDataExchange* pDX)
{
	CDlgTimeSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTime)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTime, CDlgTimeSkin)
	//{{AFX_MSG_MAP(CDlgTime)
	ON_MESSAGE(WM_TIME_UPDATE, OnMsgTimeUpdate)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTime message handlers

BOOL CDlgTime::OnInitDialog() 
{
	CDlgTimeSkin::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_rectTime = GetTimeRegion();
	
	DWORD dwThreadID ;
	hCloseEvent = CreateEvent(NULL, FALSE, FALSE , NULL);
	m_hTimeThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL,					//安全属性
                                    0,											//堆栈大小
									(LPTHREAD_START_ROUTINE)TimeThread,	//线程函数
                                    (LPVOID)this,								//参数
									0,											//马上运行
									&dwThreadID);								//线程id
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTime::DrawTime()
{
	// 采用双缓存
	if (m_hWnd)
	{
		CDC* pDC = GetDC();
		CDC dcMem;
//		CRect rect;
//		GetClientRect(rect);
//		rect.top += 5;
//		rect.left += 50;
//		rect.bottom -= 10;
//		rect.right -= 100;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap bmp, *pOldBmp;
		bmp.CreateCompatibleBitmap(pDC, m_rectTime.Width(), m_rectTime.Height());
		pOldBmp = dcMem.SelectObject(&bmp);

		// 设置背景色
		dcMem.FillSolidRect(0,0,m_rectTime.Width(),m_rectTime.Height(),RGB(82,82,82));
		
		COLORREF clrWhite = RGB(3,255,255);
		dcMem.SetTextColor(clrWhite);
		SetBkMode(dcMem,TRANSPARENT);
		
		// Draw Time
		int iFontHeight = 34;
		int iFontWidth = 0;
		CTime time = CTime::GetCurrentTime();
		CString sTime;
		sTime.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
		CFont MyFont;
		MyFont.CreateFont(iFontHeight, iFontWidth, 
			0,0,FW_BOLD,0,0,0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			"Tahoma"); 
		dcMem.SelectObject(MyFont);
		dcMem.TextOut(11, -2, sTime);
		
		// 显示
		pDC->BitBlt(m_rectTime.left,m_rectTime.top,m_rectTime.Width(), m_rectTime.Height(), &dcMem,0,0, SRCCOPY);
		dcMem.SelectObject(pOldBmp);
		dcMem.DeleteDC();
		bmp.DeleteObject();
		ReleaseDC(pDC);
	}
}

void CDlgTime::StopTime()
{
	DWORD dwThreadStatus;
	SetEvent(hCloseEvent);
	BOOL N = GetExitCodeThread(m_hTimeThread, &dwThreadStatus);
	DWORD dr = GetLastError();
	while(STILL_ACTIVE == dwThreadStatus)
	{
		SetEvent(hCloseEvent);
		Sleep(10);
		if(!(GetExitCodeThread(m_hTimeThread, &dwThreadStatus)))
			break;
	}
}

LRESULT CDlgTime::OnMsgTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	if ((int)wParam == 1)
	{
		DrawTime();
	}
	return 0;
}

UINT TimeThread(LPVOID pParam)
{
	
	CDlgTime *pDlgTime = (CDlgTime*)pParam;
	DWORD ret;

	CTime time;
	time.GetCurrentTime();

	while(1)
	{
	 	ret = WaitForSingleObject(pDlgTime->hCloseEvent, 1);
		if(WAIT_OBJECT_0 == ret)
			break;

		::PostMessage(pDlgTime->m_hWnd, WM_TIME_UPDATE, 1, 0);
	}
	AfxEndThread( 0, TRUE );
	return 0;
}

void CDlgTime::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	StopTime();
	CDlgTimeSkin::OnClose();
}
