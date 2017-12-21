// DlgTPSSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTPSSkin.h"
#include "resource.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTPSSkin dialog


CDlgTPSSkin::CDlgTPSSkin()
{
	m_pWnd=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgTPSSkin message handlers

void CDlgTPSSkin::funCreateDraw()
{
	if(!m_pWnd)
		return;

	CClientDC dc(m_pWnd);

	CRect rc;
	m_pWnd->GetClientRect(&rc);
	
	rc.right-=2;
	rc.bottom-=2;
	m_skinDC.LoadDC(&dc,rc);

	CShowImage memImage;
	memImage.SetImage(IDR_BITMAP_TPS_BK,1);
	memImage.SetSizingMargin(2,2,5,5);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);
	
}

BOOL CDlgTPSSkin::funOnEraseBkgnd(CDC* pDC)
{
	m_skinDC.DrawBack(pDC);
 	return TRUE;
}

BOOL CDlgTPSSkin::funOnInitDialog() 
{
	if(!m_pWnd)
		return TRUE;

	// TODO: Add extra initialization here
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	m_pWnd->MoveWindow(x-271,0,271,y);

	funCreateDraw();
	m_skinDC.EnableDrawBack(FALSE);

	return TRUE;  
}

void CDlgTPSSkin::funSetHandle(CWnd *pWnd)
{ 
	m_pWnd= pWnd; 
}

void CDlgTPSSkin::SetEvaluateBtn(CImageButton *pBtn)
{
	
	m_btnEvaluate=pBtn;
	if(pBtn==NULL)
		return;
	m_btnEvaluate->LoadBitmaps(IDR_PNG_TPS_BTN_BK,8,8,8,8);
	m_btnEvaluate->LoadSecondBitmaps(IDR_PNG_TPS_BTN_EVALUATE,2,2,2,2);
}

void CDlgTPSSkin::SetOpenBtn(CImageButton *pBtn)
{
	
	m_btnOpen=pBtn;
	if(pBtn==NULL)
		return;
	m_btnOpen->LoadBitmaps(IDR_PNG_TPS_BTN_BK,8,8,8,8);
	m_btnOpen->LoadSecondBitmaps(IDR_PNG_TPS_BTN_OPEN,2,2,2,2);
}

void CDlgTPSSkin::SetSaveBtn(CImageButton *pBtn)
{
	
	m_btnSave=pBtn;
	if(pBtn==NULL)
		return;
	m_btnSave->LoadBitmaps(IDR_PNG_TPS_BTN_BK,8,8,8,8);
	m_btnSave->LoadSecondBitmaps(IDR_PNG_TPS_BTN_SAVE,2,2,2,2);
}

void CDlgTPSSkin::SetProbeBtn(CImageButton *pBtn)
{
	
	m_btnProbe=pBtn;
	if(pBtn==NULL)
		return;
	m_btnProbe->LoadBitmaps(IDR_PNG_TPS_BTN_BK,8,8,8,8);
	m_btnProbe->LoadSecondBitmaps(IDR_PNG_TPS_BTN_PROBE,2,2,2,2);
}

LONG CDlgTPSSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;
}
