// DlgMPRSkin.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "DlgMPRSkin.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMPRSkin dialog


CDlgMPRSkin::CDlgMPRSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMPRSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMPRSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMPRSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMPRSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgMPRSkin)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMPRSkin message handlers

void CDlgMPRSkin::Set3DShowImageBtn(CImageButton *pBtn)
{
	CShowImage bitSkin;
	BITMAP bmpinfo;
	bitSkin.LoadBitmap(IDB_BITMAP_BTN_IMAGE_FIRST);
	bitSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth/4;
	int iHeight= bmpinfo.bmHeight;
	int iTop=0;

	if(pBtn)
	{
		pBtn->MoveWindow(iWidth * 4,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_3D,0,0,0,0);
	}
}
