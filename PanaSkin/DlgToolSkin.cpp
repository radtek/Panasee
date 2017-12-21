// DlgToolSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgToolSkin.h"
#include "resource.h"
#include "ShowImage.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToolSkin dialog

CDlgToolSkin::CDlgToolSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgToolSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_btnCamera=NULL;
	m_btnTarget=NULL;
	m_btnMeasure=NULL;
	m_btnPatientTable=NULL;
	m_btnMoveCamera=NULL;
	m_btnBrowseImage=NULL;
	m_btnCompareImage=NULL;
	m_btnCapture=NULL;
	m_btnSystem=NULL;
}


void CDlgToolSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgToolSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgToolSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgToolSkin)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgToolSkin message handlers

BOOL CDlgToolSkin::OnInitDialog() 
{
	//AFX_MANAGE_STATE(AfxGetAppModuleState());
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CShowImage bitTitleSkin,bitToolSkin;
	BITMAP bmpinfo;

	//get height of title
	bitTitleSkin.LoadBitmap(IDB_BITMAP_TITLE_SKIN);
	bitTitleSkin.GetBitmap(&bmpinfo);
	int iTitleHeight= bmpinfo.bmHeight;

	//get size of toolbar
	bitToolSkin.LoadBitmap(IDB_BITMAP_TOOL_SKIN);
	bitToolSkin.GetBitmap(&bmpinfo);
	int iWidth= bmpinfo.bmWidth;
	int iHeight= bmpinfo.bmHeight;

	CRect rc(0,iTitleHeight,iWidth,iHeight+iTitleHeight-5);
	if(GetParent())
	{
		CRect rcParent;
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
	
	//set buttons
	CShowImage bitBtn;
	bitBtn.LoadBitmap(IDB_BITMAP_TOOL_CAMERA);
	bitBtn.GetBitmap(&bmpinfo);
	iWidth= bmpinfo.bmWidth/4;
	iHeight= bmpinfo.bmHeight;

	if(m_btnCamera)
	{
		m_btnCamera->MoveWindow(14,14,iWidth,iHeight);
		m_btnCamera->LoadBitmaps(IDB_BITMAP_TOOL_CAMERA,0,0,0,0);
	}

	if(m_btnMeasure)
	{
		m_btnMeasure->MoveWindow(14+41*1,14,iWidth,iHeight);
		m_btnMeasure->LoadBitmaps(IDB_BITMAP_TOOL_MEASURE,0,0,0,0);
	}

	if(m_btnTarget)
	{
		m_btnTarget->MoveWindow(14+41*2,14,iWidth,iHeight);
		m_btnTarget->LoadBitmaps(IDB_BITMAP_TOOL_TARGET,0,0,0,0);
	}

	if(m_btnPatientTable)
	{
		m_btnPatientTable->MoveWindow(14+41*3,14,iWidth,iHeight);
		m_btnPatientTable->LoadBitmaps(IDB_BITMAP_TOOL_PATIENTTABLE,0,0,0,0);
	}

	if(m_btnMoveCamera)
	{
		m_btnMoveCamera->MoveWindow(14,14+41,iWidth,iHeight);
		m_btnMoveCamera->LoadBitmaps(IDB_BITMAP_TOOL_MOVECAMERA,0,0,0,0);
	}

	if(m_btnBrowseImage)
	{
		m_btnBrowseImage->MoveWindow(14+41*1,14+41,iWidth,iHeight);
		m_btnBrowseImage->LoadBitmaps(IDB_BITMAP_TOOL_BROWSEIMAGE,0,0,0,0);
	}

	if(m_btnCompareImage)
	{
		m_btnCompareImage->MoveWindow(14+41*2,14+41,iWidth,iHeight);
		m_btnCompareImage->LoadBitmaps(IDB_BITMAP_TOOL_COMPAREIMAGE,0,0,0,0);
	}

	if(m_btnCapture)
	{
		m_btnCapture->MoveWindow(14+41*3,14+41,iWidth,iHeight);
		m_btnCapture->LoadBitmaps(IDB_BITMAP_TOOL_CAPTURE,0,0,0,0);
	}

	CShowImage bitBtnSystem;
	bitBtnSystem.LoadBitmap(IDB_BITMAP_TOOL_SYSTEM);
	bitBtnSystem.GetBitmap(&bmpinfo);
	iWidth= bmpinfo.bmWidth/4;
	iHeight= bmpinfo.bmHeight;

	if(m_btnSystem)
	{
		m_btnSystem->MoveWindow(rc.Width()-13-iWidth,20,iWidth,iHeight);
		m_btnSystem->LoadBitmaps(IDB_BITMAP_TOOL_SYSTEM,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnSystem->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnSystem->SetWindowRgn(rgn,FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LONG CDlgToolSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;
}

BOOL CDlgToolSkin::OnEraseBkgnd(CDC* pDC)
{
	m_skinDC.DrawBack(pDC);
 	return TRUE;
}


void CDlgToolSkin::SetCameraBtn(CImageButton *pBtn)
{
	m_btnCamera=pBtn;
}

void CDlgToolSkin::SetTargetBtn(CImageButton *pBtn)
{
	m_btnTarget=pBtn;
}
void CDlgToolSkin::SetMeasureBtn(CImageButton *pBtn)
{
	m_btnMeasure=pBtn;
}
void CDlgToolSkin::SetPatientTableBtn(CImageButton *pBtn)
{
	m_btnPatientTable=pBtn;
}
void CDlgToolSkin::SetMoveCameraBtn(CImageButton *pBtn)
{
	m_btnMoveCamera=pBtn;
}
void CDlgToolSkin::SetBrowseImageBtn(CImageButton *pBtn)
{
	m_btnBrowseImage=pBtn;
}
void CDlgToolSkin::SetCompareImageBtn(CImageButton *pBtn)
{
	m_btnCompareImage=pBtn;
}
void CDlgToolSkin::SetCaptureBtn(CImageButton *pBtn)
{
	m_btnCapture=pBtn;
}
void CDlgToolSkin::SetSystemBtn(CImageButton *pBtn)
{
	m_btnSystem=pBtn;
}


void CDlgToolSkin::CreateDraw()
{
	CClientDC dcClient(this);

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dcClient,rc);

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_TOOL_SKIN,1);
	memImage.SetSizingMargin(12,100,85,2);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);
}
