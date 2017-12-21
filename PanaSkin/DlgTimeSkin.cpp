// DlgTimeSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTimeSkin.h"
#include "resource.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSkin dialog


CDlgTimeSkin::CDlgTimeSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTimeSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bWaitingFlag=FALSE;
}


void CDlgTimeSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTimeSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTimeSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgTimeSkin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSkin message handlers

BOOL CDlgTimeSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CShowImage bitSkin;
	bitSkin.LoadBitmap(IDB_BITMAP_TIME_SKIN);

	BITMAP bmpinfo;
	bitSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth;
	int iHeight= bmpinfo.bmHeight;

	CRect rcParent,rc(0,640,iWidth,640+iHeight);
	if(GetParent())
	{
		GetParent()->GetClientRect(&rcParent);
		if(rcParent.Width()>rc.Width())
		{
			rc.left=0;
			rc.right=rcParent.Width();
		}
	}
	MoveWindow(rc);

	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTimeSkin::CreateDraw()
{
	while(m_bWaitingFlag)
	{
		Sleep(50);
	}
	m_bWaitingFlag=TRUE;

	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dc,rc);

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_TIME_SKIN,1);
	memImage.SetSizingMargin(60,12,60,12);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	m_bWaitingFlag=FALSE;
	
}


CRect CDlgTimeSkin::GetTimeRegion()
{
	CRect rc;
	GetClientRect(&rc);
	rc.left+=58;
	rc.right-=60;
	rc.top+=12;
	rc.bottom-=12;

	return rc;
}


void CDlgTimeSkin::DrawTime()
{
	CTime time = CTime::GetCurrentTime();
	CString sTime;
	sTime.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
		
	CClientDC dc(this);

	CRect rc=GetTimeRegion();
	
	CShowImage memImage;
	memImage.SetTransparentColor(RGB(255,0,255));
	BITMAP bmpInfo;
	
	memImage.SetImage(IDR_PNG_0,1);
	memImage.GetBitmap(&bmpInfo);
	rc.top=rc.top+(rc.Height()-bmpInfo.bmHeight)/2;
	rc.bottom=rc.top+bmpInfo.bmHeight;

	CRect rcSrc;

	for(int i=0;i<sTime.GetLength();i++)
	{
		rc.right=rc.left+bmpInfo.bmWidth-2;
		rcSrc=CRect(1,0,bmpInfo.bmWidth-2,bmpInfo.bmHeight);
		if(sTime.Mid(i,1)=="0")
			memImage.SetImage(IDR_PNG_0,1);
		else if(sTime.Mid(i,1)=="1")
			memImage.SetImage(IDR_PNG_1,1);
		else if(sTime.Mid(i,1)=="2")
			memImage.SetImage(IDR_PNG_2,1);
		else if(sTime.Mid(i,1)=="3")
			memImage.SetImage(IDR_PNG_3,1);
		else if(sTime.Mid(i,1)=="4")
			memImage.SetImage(IDR_PNG_4,1);
		else if(sTime.Mid(i,1)=="5")
			memImage.SetImage(IDR_PNG_5,1);
		else if(sTime.Mid(i,1)=="6")
			memImage.SetImage(IDR_PNG_6,1);
		else if(sTime.Mid(i,1)=="7")
			memImage.SetImage(IDR_PNG_7,1);
		else if(sTime.Mid(i,1)=="8")
			memImage.SetImage(IDR_PNG_8,1);
		else if(sTime.Mid(i,1)=="9")
			memImage.SetImage(IDR_PNG_9,1);
		else if(sTime.Mid(i,1)==":")
		{
			rcSrc.left+=4;
			rcSrc.right-=3;
			rc.right-=7;
			memImage.SetImage(IDR_PNG_DOT,1);
		}
		else
			break;
		
		memImage.SetSizingMargin(0,0,0,0);
		memImage.DrawImagePart(&dc,rc,rcSrc);

		
		rc.left=rc.right;
	}
}

void CDlgTimeSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	while(m_bWaitingFlag)
	{
		Sleep(50);
	}
	m_bWaitingFlag=TRUE;

	m_skinDC.DrawBack(&dc);

	m_bWaitingFlag=FALSE;
	
	// Do not call CDialog::OnPaint() for painting messages
}
