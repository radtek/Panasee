// DlgInfoSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInfoSkin.h"
#include "resource.h"
#include "ShowImage.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoSkin dialog


CDlgInfoSkin::CDlgInfoSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfoSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bProbeVisable=FALSE;
	m_bBaseVisable=FALSE;
	m_btnPre=NULL;
	m_btnNext=NULL;
	m_sPrompt=NULL;
}


void CDlgInfoSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfoSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfoSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgInfoSkin)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoSkin message handlers

BOOL CDlgInfoSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int ResolutionY=GetSystemMetrics(SM_CYSCREEN);

	CShowImage bitSkin;
	bitSkin.LoadBitmap(IDB_BITMAP_INFO_SKIN);

	BITMAP bmpinfo;
	bitSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth;
	int iHeight= bmpinfo.bmHeight;

	CRect rcParent,rc(0,693,iWidth,ResolutionY-768+693+iHeight);
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

	CShowImage bitBtn;
	int iTop=ResolutionY-768+40;
	if(m_btnPre)
	{
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_PRE);
		
		bitBtn.GetBitmap(&bmpinfo);
		m_btnPre->MoveWindow(0,iTop,bmpinfo.bmWidth/4,bmpinfo.bmHeight);
		m_btnPre->LoadBitmaps(IDB_BITMAP_BTN_PRE,8,8,8,8);
		m_btnPre->SetTransparentColor(RGB(255,0,255));
	}

	if(m_btnNext)
	{
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_NEXT);
		
		bitBtn.GetBitmap(&bmpinfo);
		m_btnNext->MoveWindow(rc.Width()-bmpinfo.bmWidth/4,iTop,bmpinfo.bmWidth/4,bmpinfo.bmHeight);
		m_btnNext->LoadBitmaps(IDB_BITMAP_BTN_NEXT,8,8,8,8);
		m_btnNext->SetTransparentColor(RGB(255,0,255));
	}

	if(m_sPrompt)
	{
		m_sPrompt->MoveWindow(bmpinfo.bmWidth/4+2,iTop+8,rc.Width()-bmpinfo.bmWidth/4*2-4,14);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LONG CDlgInfoSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;

}

void CDlgInfoSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_skinDC.DrawBack(&dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgInfoSkin::SetNextBtn(CImageButton *pBtn)
{
	m_btnNext=pBtn;
}

void CDlgInfoSkin::SetPreBtn(CImageButton *pBtn)
{
	m_btnPre=pBtn;
}

void CDlgInfoSkin::SetPromptStatic(CPanaStatic *pStatic)
{
	m_sPrompt=pStatic;
}

void CDlgInfoSkin::SetBaseVisable(BOOL bVisable)
{
	m_bBaseVisable=bVisable;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgInfoSkin::SetProbeVisable(BOOL bVisable)
{
	m_bProbeVisable=bVisable;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgInfoSkin::CreateDraw()
{
	CClientDC dcClient(this);

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dcClient,rc);
	

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_INFO_SKIN,1);
	memImage.SetSizingMargin(18,8,18,30);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	memImage.SetTransparentColor(RGB(255,0,255));

	int iTop=5,iLeft=70;
	//draw base prompt
	CShowImage bitImage;
	bitImage.LoadBitmap(IDB_BITMAP_BASE_VISABLE);
	BITMAP bmpinfo;
	bitImage.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth/2;
	int iHeight= bmpinfo.bmHeight;

	rc.left=iLeft;
	rc.top=iTop;
	rc.right=rc.left+iWidth;
	rc.bottom=rc.top+iHeight;
	memImage.SetImage(IDB_BITMAP_BASE_VISABLE,2);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rc);
	if(m_bBaseVisable)
	{
		memImage.DrawImage(&m_skinDC,1);
	}else
	{
		memImage.DrawImage(&m_skinDC,0);
	}

	//draw probe prompt
	bitImage.LoadBitmap(IDB_BITMAP_PROBE_VISABLE);
	bitImage.GetBitmap(&bmpinfo);
	iWidth= bmpinfo.bmWidth/2;
	iHeight= bmpinfo.bmHeight;

	rc.left=iLeft+iWidth;
	rc.top=iTop;
	rc.right=rc.left+iWidth;
	rc.bottom=rc.top+iHeight;
	memImage.SetImage(IDB_BITMAP_PROBE_VISABLE,2);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rc);
	if(m_bProbeVisable)
	{
		memImage.DrawImage(&m_skinDC,1);
	}else
	{
		memImage.DrawImage(&m_skinDC,0);
	}
}
