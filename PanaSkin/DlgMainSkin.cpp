// DlgMainSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMainSkin.h"
#include "ShowImage.h"
#include "MemDC.h"
#include "AutoFont.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainSkin dialog


CDlgMainSkin::CDlgMainSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_tabMode=PASK_Tab_Patient;
	m_bNewSeriesAlarm=FALSE;

	m_bOperateType = PASK_OPERATION_TYPE_REVIEW;

	m_iUIMode=1;

}


void CDlgMainSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgMainSkin)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainSkin message handlers

BOOL CDlgMainSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc;
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	rc.left=x-270;
	rc.right=x;
	rc.top=0;
	rc.bottom=y;
	MoveWindow(rc);

	CShowImage bitBtn;
	BITMAP bmpinfo;
	
	int iLeft=x-rc.left-38;
	int iTop=180;

	
	bitBtn.LoadBitmap(IDR_BITMAP_TAB_PATIENT);
	bitBtn.GetBitmap(&bmpinfo);
	m_rcPatientBtn=CRect(iLeft,iTop,iLeft+bmpinfo.bmWidth/2,iTop+70);
	iTop=iTop+70-5;

	
	m_rcSeriesBtn=CRect(iLeft,iTop,iLeft+bmpinfo.bmWidth/2,iTop+60);
	iTop=iTop+60-5;

	if(m_iUIMode==1)
	{
		m_rcMriControlBtn=CRect(iLeft,iTop,iLeft+bmpinfo.bmWidth/2,iTop+85);
		iTop=iTop+85-5;
	}else
	{
		m_rcMriControlBtn=CRect(0,0,1,1);
	}

	m_rcNavigateBtn=CRect(iLeft,iTop,iLeft+bmpinfo.bmWidth/2,iTop+60);
	iTop=iTop+60-5;

	m_rcTpsBtn=CRect(iLeft,iTop,iLeft+bmpinfo.bmWidth/2,iTop+60);
	iTop=iTop+60-5;

	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMainSkin::SetTabMode(PASK_TabMode mode)
{
	if(mode==m_tabMode)
		return;

	CRect rc,rcOld;
	if(mode==PASK_Tab_Patient)
	{
		rc=m_rcPatientBtn;
	}

	if(mode==PASK_Tab_Series)
	{	
		rc=m_rcSeriesBtn;
	}

	if(mode==PASK_Tab_MriControl)
	{
		rc=m_rcMriControlBtn;
	}

	if(mode==PASK_Tab_Navigate)
	{
		rc=m_rcNavigateBtn;
	}

	if(mode==PASK_Tab_Tps)
	{
		rc=m_rcTpsBtn;
	}

	if(m_tabMode==PASK_Tab_Patient)
	{
		rcOld=m_rcPatientBtn;
	}

	if(m_tabMode==PASK_Tab_Series)
	{	
		rcOld=m_rcSeriesBtn;
	}

	if(m_tabMode==PASK_Tab_MriControl)
	{
		rcOld=m_rcMriControlBtn;
	}

	if(m_tabMode==PASK_Tab_Navigate)
	{
		rcOld=m_rcNavigateBtn;
	}

	if(m_tabMode==PASK_Tab_Tps)
	{
		rcOld=m_rcTpsBtn;
	}

	
	m_tabMode = mode;
	
	CreateDraw();

	rc.UnionRect(&rc,&rcOld);
	//InvalidateRect(rcOld);
	InvalidateRect(rc);
}

void CDlgMainSkin::GetTabPosition(PASK_TabMode mode,CRect &rc)
{
	if(mode==PASK_Tab_Patient)
	{
		rc=m_rcPatientBtn;
	}

	if(mode==PASK_Tab_Series)
	{	
		rc=m_rcSeriesBtn;
	}

	if(mode==PASK_Tab_MriControl)
	{
		rc=m_rcMriControlBtn;
	}

	if(mode==PASK_Tab_Navigate)
	{
		rc=m_rcNavigateBtn;
	}

	if(mode==PASK_Tab_Tps)
	{
		rc=m_rcTpsBtn;
	}
}

BOOL CDlgMainSkin::OnEraseBkgnd(CDC* pDC)
{
	
	m_skinDC.DrawBack(pDC);	
	
 	return TRUE;
}


void CDlgMainSkin::CreateDraw()
{

	try
	{
		CClientDC dcClient(this);
		
		CRect rc;
		GetClientRect(&rc);
		
		m_skinDC.LoadDC(&dcClient,rc);
		
		m_skinDC.FillSolidRect(&rc,RGB(0,0,0));
		
		rc.top=180;
		rc.bottom=640;
		CShowImage memImage;
		memImage.SetImage(IDB_BITMAP_MAIN_SKIN,1);
		memImage.SetSizingMargin(26,26,50,26);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(&m_skinDC,0);
		
		memImage.SetTransparentColor(RGB(255,0,255));
		
		
		if(m_tabMode==PASK_Tab_Patient)
		{
			
			rc=m_rcMriControlBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcSeriesBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcNavigateBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcTpsBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcPatientBtn;
			memImage.SetImage(IDR_BITMAP_TAB_PATIENT,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,1);
			
			
		}
		
		if(m_tabMode==PASK_Tab_Series)
		{
			
			rc=m_rcMriControlBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcNavigateBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcTpsBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcPatientBtn;
			memImage.SetImage(IDR_BITMAP_TAB_PATIENT,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcSeriesBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,1);
			
		}
		
		if(m_tabMode==PASK_Tab_MriControl)
		{
			
			
			rc=m_rcSeriesBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcNavigateBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcTpsBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcPatientBtn;
			memImage.SetImage(IDR_BITMAP_TAB_PATIENT,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcMriControlBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,1);
			
		}
		
		if(m_tabMode==PASK_Tab_Navigate)
		{
			
			
			rc=m_rcSeriesBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcPatientBtn;
			memImage.SetImage(IDR_BITMAP_TAB_PATIENT,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcMriControlBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcTpsBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcNavigateBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,1);
			
		}
		
		if(m_tabMode==PASK_Tab_Tps)
		{
			
			
			rc=m_rcSeriesBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcPatientBtn;
			memImage.SetImage(IDR_BITMAP_TAB_PATIENT,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcMriControlBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcNavigateBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,0);
			
			rc=m_rcTpsBtn;
			memImage.SetImage(IDR_BITMAP_TAB_SERIES,2);
			memImage.SetSizingMargin(8,18,8,18);
			memImage.SetShowRegion(rc);
			memImage.DrawImage(&m_skinDC,1);
			
			
			
		}
		
		COLORREF clNormal=RGB(65,65,65),clSelected=RGB(35,35,35),clDisable=RGB(161,161,146);
		CAutoFont font;
		font.SetEscapement(-900);
		font.SetBold(TRUE);
		font.SetHeight(17);
		
		CFont *pOldFont;
		pOldFont=m_skinDC.SelectObject(&font);
		m_skinDC.SetBkMode(TRANSPARENT);
		
		
		CRect rcTmp;
		rcTmp=m_rcPatientBtn;
		rcTmp.left+=25;
		rcTmp.top+=5;
		
		if(m_bOperateType > PASK_OPERATION_TYPE_NONE &&
			m_bOperateType < PASK_OPERATION_TYPE_TPS)
		{
			if(m_tabMode==PASK_Tab_Patient)
			{
				m_skinDC.SetTextColor(clSelected);
			}else
			{
				m_skinDC.SetTextColor(clNormal);
			}
		}else
		{
			m_skinDC.SetTextColor(clDisable);
		}
		
		m_skinDC.TextOut(rcTmp.left,rcTmp.top,"Patient",7);
		
		rcTmp=m_rcSeriesBtn;
		rcTmp.left+=25;
		rcTmp.top+=5;
		if(m_tabMode==PASK_Tab_Series)
		{
			m_skinDC.SetTextColor(clSelected);
		}else
		{
			m_skinDC.SetTextColor(clNormal);
		}
		if(m_bNewSeriesAlarm)
		{
			m_skinDC.SetTextColor(RGB(255,0,0));
		}
		m_skinDC.TextOut(rcTmp.left,rcTmp.top,"Series",6);
		
		rcTmp=m_rcMriControlBtn;
		rcTmp.left+=25;
		rcTmp.top+=5;
		
		if(m_bOperateType > PASK_OPERATION_TYPE_NONE &&
			m_bOperateType < PASK_OPERATION_TYPE_TPS)
		{
			if(m_tabMode==PASK_Tab_MriControl)
			{
				m_skinDC.SetTextColor(clSelected);
			}else
			{
				m_skinDC.SetTextColor(clNormal);
			}
		}else
		{
			m_skinDC.SetTextColor(clDisable);
		}
		
		if(m_iUIMode==1)
		{
			m_skinDC.TextOut(rcTmp.left,rcTmp.top,"MRI Scan",8);
		}
		
		rcTmp=m_rcNavigateBtn;
		rcTmp.left+=25;
		rcTmp.top+=5;
		
		if(m_bOperateType > PASK_OPERATION_TYPE_NONE &&
			m_bOperateType < PASK_OPERATION_TYPE_TPS)
		{
			if(m_tabMode==PASK_Tab_Navigate)
			{
				m_skinDC.SetTextColor(clSelected);
			}else
			{
				m_skinDC.SetTextColor(clNormal);
			}
		}else
		{
			m_skinDC.SetTextColor(clDisable);
		}
		m_skinDC.TextOut(rcTmp.left,rcTmp.top,"Navi",4);
		
		rcTmp=m_rcTpsBtn;
		rcTmp.left+=25;
		rcTmp.top+=5;
		if(m_bOperateType != PASK_OPERATION_TYPE_REVIEW)
		{
			if(m_tabMode==PASK_Tab_Tps)
			{
				m_skinDC.SetTextColor(clSelected);
			}else
			{
				m_skinDC.SetTextColor(clNormal);
			}
		}else
		{
			m_skinDC.SetTextColor(clDisable);
		}
		CString sTmp="Plan";
		m_skinDC.TextOut(rcTmp.left,rcTmp.top,"Plan",4);
		
		m_skinDC.SelectObject(pOldFont);
	}catch(...)
	{
		//
	}
		
}

void CDlgMainSkin::SetNewSeriesAlarm(BOOL bAlarm)
{
	m_bNewSeriesAlarm=bAlarm;
	CreateDraw();
	InvalidateRect(m_rcSeriesBtn);
}

void CDlgMainSkin::SetUIMode(int mode)
{
	m_iUIMode=mode;
}
