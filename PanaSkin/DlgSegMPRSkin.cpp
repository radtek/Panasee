// DlgSegMPRSkin.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "DlgSegMPRSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSegMPRSkin dialog


CDlgSegMPRSkin::CDlgSegMPRSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSegMPRSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSegMPRSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSegMPRSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSegMPRSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgSegMPRSkin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSegMPRSkin message handlers

BOOL CDlgSegMPRSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSegMPRSkin::SetMax1Btn(CImageButton *pBtn)
{	
	if(pBtn)
	{
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_MAX,0,0,0,0);
	}
}

void CDlgSegMPRSkin::SetMax2Btn(CImageButton *pBtn)
{
	if(pBtn)
	{
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_MAX,0,0,0,0);
	}
}

void CDlgSegMPRSkin::SetMax3Btn(CImageButton *pBtn)
{
	if(pBtn)
	{
		pBtn->LoadBitmaps(IDB_BITMAP_BTN_IMAGE_MAX,0,0,0,0);
	}
}
