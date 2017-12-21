// DlgTPSViewSkin.cpp: implementation of the CDlgTPSViewSkin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgTPSViewSkin.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgTPSViewSkin::CDlgTPSViewSkin()
{

}

CDlgTPSViewSkin::~CDlgTPSViewSkin()
{

}


void CDlgTPSViewSkin::SetFirstImageBtn(CImageButton *pBtn)
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
		pBtn->MoveWindow(0,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_FIRST,0,0,0,0);
	}	

		
}
void CDlgTPSViewSkin::SetLastImageBtn(CImageButton *pBtn)
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
		pBtn->MoveWindow(iWidth * 3,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_LAST,0,0,0,0);
	}	

	
}

void CDlgTPSViewSkin::SetPreImageBtn(CImageButton *pBtn)
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
		pBtn->MoveWindow(iWidth,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_PRE,0,0,0,0);
	}	

	
}

void CDlgTPSViewSkin::SetNextImageBtn(CImageButton *pBtn)
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
		pBtn->MoveWindow(iWidth * 2,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_NEXT,0,0,0,0);
	}

	

}
void CDlgTPSViewSkin::Set3DShowImageBtn(CImageButton *pBtn)
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

void CDlgTPSViewSkin::SetTopWndBtn(CImageButton *pBtn)
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
		pBtn->MoveWindow(iWidth * 5,iTop,iWidth,iHeight);
		pBtn->LoadBitmaps(IDR_BITMAP_BTN_TOPWND,0,0,0,0);
	}
}

void CDlgTPSViewSkin::SetSelectImageBtn(CImageButton *pBtn,int iIndex)
{
	if(pBtn)
	{
		if(iIndex==1)
			pBtn->LoadBitmaps(IDR_BITMAP_BTN_IMAGE1,0,0,0,0);
		else if(iIndex==2)
			pBtn->LoadBitmaps(IDR_BITMAP_BTN_IMAGE2,0,0,0,0);
		else
			pBtn->LoadBitmaps(IDR_BITMAP_BTN_IMAGE3,0,0,0,0);
	}
}