// DlgImageControlSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgImageControlSkin.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageControlSkin dialog


CDlgImageControlSkin::CDlgImageControlSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageControlSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_btnFirst=m_btnLast=m_btnPre=m_btnNext=m_btn3D=m_btnMax=NULL;
}


void CDlgImageControlSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageControlSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageControlSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgImageControlSkin)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageControlSkin message handlers

void CDlgImageControlSkin::SetFirstBtn(CImageButton *pBtn)
{
	m_btnFirst=pBtn;
}

void CDlgImageControlSkin::SetLastBtn(CImageButton *pBtn)
{
	m_btnLast=pBtn;
}


void CDlgImageControlSkin::SetPreBtn(CImageButton *pBtn)
{
	m_btnPre=pBtn;
}


void CDlgImageControlSkin::SetNextBtn(CImageButton *pBtn)
{
	m_btnNext=pBtn;
}


void CDlgImageControlSkin::SetThreeDBtn(CImageButton *pBtn)
{
	m_btn3D=pBtn;
}


void CDlgImageControlSkin::SetMaxBtn(CImageButton *pBtn)
{
	m_btnMax=pBtn;
}

/*********** ningsj *********************
 说明:使用固定颜色填充客户区域
*****************************************/
BOOL CDlgImageControlSkin::OnEraseBkgnd(CDC* pDC)
{
	
	CRect rc;
 	GetClientRect(&rc);
  	
 	pDC->FillSolidRect(0,0,rc.Width(),rc.Height(),RGB(0,0,0));
 
 	return TRUE;
}

BOOL CDlgImageControlSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CShowImage bitSkin;
	BITMAP bmpinfo;
	bitSkin.LoadBitmap(IDB_BITMAP_BTN_IMAGE_FIRST);
	bitSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth/4;
	int iHeight= bmpinfo.bmHeight;
	int iTop=0;
	int iLeft=10;
	if(m_btnFirst)
	{
		m_btnFirst->MoveWindow(iLeft,iTop,iWidth,iHeight);
		m_btnFirst->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_FIRST,0,0,0,0);
	}	

	if(m_btnPre)
	{
		m_btnPre->MoveWindow(iLeft+iWidth,iTop,iWidth,iHeight);
		m_btnPre->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_PRE,0,0,0,0);
	}	

	if(m_btnNext)
	{
		m_btnNext->MoveWindow(iLeft+iWidth * 2,iTop,iWidth,iHeight);
		m_btnNext->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_NEXT,0,0,0,0);
	}	

	if(m_btnLast)
	{
		m_btnLast->MoveWindow(iLeft+iWidth * 3,iTop,iWidth,iHeight);
		m_btnLast->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_LAST,0,0,0,0);
	}	

	iLeft+=30;
	if(m_btnMax)
	{
		m_btnMax->MoveWindow(iLeft+iWidth * 4,iTop,iWidth,iHeight);
		m_btnMax->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_MAX,0,0,0,0);
	}	

	if(m_btn3D)
	{
		m_btn3D->MoveWindow(iLeft+iWidth * 5,iTop,iWidth,iHeight);
		m_btn3D->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_3D,0,0,0,0);
	}	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
