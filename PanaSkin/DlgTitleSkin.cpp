// DlgTitleSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTitleSkin.h"
#include "ShowImage.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTitleSkin dialog


CDlgTitleSkin::CDlgTitleSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTitleSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iOperationType=PASK_OPERATION_TYPE_NONE;
}


void CDlgTitleSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTitleSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTitleSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgTitleSkin)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTitleSkin message handlers
BOOL CDlgTitleSkin::OnEraseBkgnd(CDC* pDC)
{
	m_skinDC.DrawBack(pDC);
 	return TRUE;
}



BOOL CDlgTitleSkin::OnInitDialog() 
{
	//AFX_MANAGE_STATE(AfxGetAppModuleState());
	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CShowImage bitSkin;
	bitSkin.LoadBitmap(IDB_BITMAP_TITLE_SKIN);

	BITMAP bmpinfo;
	bitSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth/3;
	int iHeight= bmpinfo.bmHeight;

	CRect rcParent,rc(0,0,iWidth,iHeight);
	if(GetParent())
	{
		GetParent()->GetClientRect(&rcParent);
		if(rcParent.Width()>rc.Width())
		{
			rc.left=(rcParent.Width()-rc.Width())/2;
			rc.right=rc.left+iWidth;
		}
	}
	MoveWindow(rc);

	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTitleSkin::SetOperation(PASK_OPERATIONTYPE iOperationType)
{
	m_iOperationType=iOperationType;

}
void CDlgTitleSkin::CreateDraw()
{
	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dc,rc);

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_TITLE_SKIN,3);
	memImage.SetSizingMargin(2,2,2,2);
	memImage.SetShowRegion(rc);
	switch(m_iOperationType)
	{
	case PASK_OPERATION_TYPE_CRYOTHERAPY:
		memImage.DrawImage(&m_skinDC,0);
		break;
	case PASK_OPERATION_TYPE_BIOPSY:
		memImage.DrawImage(&m_skinDC,2);
		break;
	default:
		memImage.DrawImage(&m_skinDC,1);
		break;
	}
}
