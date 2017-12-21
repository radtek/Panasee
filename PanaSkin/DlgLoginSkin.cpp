// DlgLoginSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLoginSkin.h"
#include "ShowImage.h"
#include "MemDC.h"
#include "resource.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginSkin dialog


CDlgLoginSkin::CDlgLoginSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoginSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_SelectedStatus=PASK_Sel_None;
	m_btnBiopsy=m_btnCryotherapy=NULL;
	m_btnQuit=NULL;
	m_btnReview=m_btnConfig=m_btnTPS=NULL;
}


void CDlgLoginSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoginSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoginSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgLoginSkin)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginSkin message handlers

BOOL CDlgLoginSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CShowImage bitBtn;
	bitBtn.LoadBitmap(IDB_BITMAP_DLG_LOGIN_SKIN);
	BITMAP bmpinfo;
	bitBtn.GetBitmap(&bmpinfo);

	CRect rc;
	int x=GetSystemMetrics(SM_CXSCREEN);
	int y=GetSystemMetrics(SM_CYSCREEN);
	rc.left=(x-bmpinfo.bmWidth)/2;
	rc.right=rc.left+bmpinfo.bmWidth;
	rc.top=(y-bmpinfo.bmHeight)/2;
	rc.bottom=rc.top+bmpinfo.bmHeight;
	MoveWindow(rc);

	CRgn rgn;
	GetClientRect(&rc);
	rgn.CreateRoundRectRgn(0,0,rc.Width(),rc.Height(),15,15);
	this->SetWindowRgn(rgn,FALSE);
	
	

	//calculate display region
	int iWidth=64;
	int iHeight=64;
	CFont *pParentFont=GetFont(); 

	
	bitBtn.LoadBitmap(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME);
	bitBtn.GetBitmap(&bmpinfo);
	int iSelectedWidth=bmpinfo.bmWidth;
	int iSelectedHeight=bmpinfo.bmHeight;

	//TPS按钮位置
	m_rcTps.top=260;
	m_rcTps.left=40;
	m_rcTps.right=m_rcTps.left+iWidth;
	m_rcTps.bottom=m_rcTps.top+iHeight;
	m_rcTpsSelected.left=m_rcTps.left-(iSelectedWidth-iWidth)/2;
	m_rcTpsSelected.top=m_rcTps.top-(iSelectedHeight-iHeight)/2;
	m_rcTpsSelected.right=m_rcTpsSelected.left+iSelectedWidth;
	m_rcTpsSelected.bottom=m_rcTpsSelected.top+iSelectedHeight;

	m_rcTpsSelected.bottom+=22 * 1; //增加一个按钮的高度


	if(m_btnTPS)
	{
		m_btnTPS->MoveWindow(m_rcTpsSelected.left+4,
			m_rcTpsSelected.top+145,m_rcTpsSelected.Width()-4*2,20);
		m_btnTPS->LoadBitmaps(IDB_BITMAP_BTN_LOGIN_SELECTED,0,0,0,0);
		m_btnTPS->SetFontColor(RGB(255,255,255));
		m_btnTPS->SetTextFont(*pParentFont);
		m_btnTPS->ShowWindow(SW_HIDE);
	}	

	//Application位置
	m_rcApplication=m_rcTps;
	m_rcApplication.left+=iWidth +55;
	m_rcApplication.right=m_rcApplication.left+iWidth;
	m_rcApplicationSelected.left=m_rcApplication.left-(iSelectedWidth-iWidth)/2;
	m_rcApplicationSelected.top=m_rcApplication.top-(iSelectedHeight-iHeight)/2;
	m_rcApplicationSelected.right=m_rcApplicationSelected.left+iSelectedWidth;
	m_rcApplicationSelected.bottom=m_rcApplicationSelected.top+iSelectedHeight;
	
	/*********** ningsj *********************
	说明:设置功能按钮的位置
	*****************************************/
	m_rcApplicationSelected.bottom+=22 * 2+15; //增加两个按钮的高度
	
	if(m_btnBiopsy)
	{
		m_btnBiopsy->MoveWindow(m_rcApplicationSelected.left+4,  //按钮大小缩进4个像素
			m_rcApplicationSelected.top+145,m_rcApplicationSelected.Width()-4*2,35);
		m_btnBiopsy->LoadBitmaps(IDB_BITMAP_BTN_LOGIN_SELECTED,0,0,0,0);
		m_btnBiopsy->SetFontColor(RGB(255,255,255));
		m_btnBiopsy->SetTextFont(*pParentFont);
		m_btnBiopsy->ShowWindow(SW_HIDE);
		m_btnBiopsy->EnableMultiLine(TRUE);
	}	
	
	if(m_btnCryotherapy)
	{
		m_btnCryotherapy->MoveWindow(m_rcApplicationSelected.left+4,
			m_rcApplicationSelected.top+145+37,m_rcApplicationSelected.Width()-4*2,20);
		m_btnCryotherapy->LoadBitmaps(IDB_BITMAP_BTN_LOGIN_SELECTED,0,0,0,0);
		m_btnCryotherapy->SetFontColor(RGB(255,255,255));
		m_btnCryotherapy->SetTextFont(*pParentFont);
		m_btnCryotherapy->ShowWindow(SW_HIDE);
	}	

	//Database位置
	m_rcPatientDB=m_rcApplication;
	m_rcPatientDB.left+=iWidth +55;
	m_rcPatientDB.right=m_rcPatientDB.left+iWidth;
	m_rcPatientDBSelected.left=m_rcPatientDB.left-(iSelectedWidth-iWidth)/2;
	m_rcPatientDBSelected.top=m_rcPatientDB.top-(iSelectedHeight-iHeight)/2;
	m_rcPatientDBSelected.right=m_rcPatientDBSelected.left+iSelectedWidth;
	m_rcPatientDBSelected.bottom=m_rcPatientDBSelected.top+iSelectedHeight;

	m_rcTools=m_rcPatientDB;
	m_rcTools.left+=iWidth +55;
	m_rcTools.right=m_rcTools.left+iWidth;
	m_rcToolsSelected.left=m_rcTools.left-(iSelectedWidth-iWidth)/2;
	m_rcToolsSelected.top=m_rcTools.top-(iSelectedHeight-iHeight)/2;
	m_rcToolsSelected.right=m_rcToolsSelected.left+iSelectedWidth;
	m_rcToolsSelected.bottom=m_rcToolsSelected.top+iSelectedHeight;

	m_rcToolsSelected.bottom+=22 * 1; //增加一个按钮的高度


	if(m_btnReview)
	{
		m_btnReview->MoveWindow(m_rcToolsSelected.left+4,
			m_rcToolsSelected.top+145,m_rcToolsSelected.Width()-4*2,20);
		m_btnReview->LoadBitmaps(IDB_BITMAP_BTN_LOGIN_SELECTED,0,0,0,0);
		m_btnReview->SetFontColor(RGB(255,255,255));
		m_btnReview->SetTextFont(*pParentFont);
		m_btnReview->ShowWindow(SW_HIDE);
	}	

	m_rcConfig=m_rcTools;
	m_rcConfig.left+=iWidth +55;
	m_rcConfig.right=m_rcConfig.left+iWidth;
	m_rcConfigSelected.left=m_rcConfig.left-(iSelectedWidth-iWidth)/2;
	m_rcConfigSelected.top=m_rcConfig.top-(iSelectedHeight-iHeight)/2;
	m_rcConfigSelected.right=m_rcConfigSelected.left+iSelectedWidth;
	m_rcConfigSelected.bottom=m_rcConfigSelected.top+iSelectedHeight;

	m_rcConfigSelected.bottom+=22 * 1; //增加一个按钮的高度


	if(m_btnConfig)
	{
		m_btnConfig->MoveWindow(m_rcConfigSelected.left+4,
			m_rcConfigSelected.top+145,m_rcConfigSelected.Width()-4*2,20);
		m_btnConfig->LoadBitmaps(IDB_BITMAP_BTN_LOGIN_SELECTED,0,0,0,0);
		m_btnConfig->SetFontColor(RGB(255,255,255));
		m_btnConfig->SetTextFont(*pParentFont);
		m_btnConfig->ShowWindow(SW_HIDE);
	}	

	
	if(m_btnQuit)
	{
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_QUIT);
		bitBtn.GetBitmap(&bmpinfo);

		m_btnQuit->LoadBitmaps(IDB_BITMAP_BTN_QUIT,0,0,0,0);
		m_btnQuit->MoveWindow(635,323,bmpinfo.bmWidth/4,bmpinfo.bmHeight);
		
		CRgn rgn;
		CRect rcTmp;
		m_btnQuit->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnQuit->SetWindowRgn(rgn,FALSE);
		
	}	

	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoginSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	static int iFirstDraw=0;

	if(iFirstDraw==0)

		m_skinDC.SetAnimation(2,255,250);
	else
		m_skinDC.SetAnimation(-1,20,20);  //第二次刷新时去掉动画演示

	m_skinDC.DrawBack(&dc);
	
	iFirstDraw=1;
	
	
	if(m_SelectedStatus==PASK_Sel_Applications)
	{
		m_btnCryotherapy->ShowWindow(SW_SHOW);
		m_btnBiopsy->ShowWindow(SW_SHOW);
	}else
	{
		m_btnCryotherapy->ShowWindow(SW_HIDE);
		m_btnBiopsy->ShowWindow(SW_HIDE);
	}

	if(m_SelectedStatus==PASK_Sel_Tools)
	{
		m_btnReview->ShowWindow(SW_SHOW);
	}else
	{
		m_btnReview->ShowWindow(SW_HIDE);
	}

	if(m_SelectedStatus==PASK_Sel_Config)
	{
		m_btnConfig->ShowWindow(SW_SHOW);
	}else
	{
		m_btnConfig->ShowWindow(SW_HIDE);
	}

	if(m_SelectedStatus==PASK_Sel_Tps)
	{
		m_btnTPS->ShowWindow(SW_SHOW);
	}else
	{
		m_btnTPS->ShowWindow(SW_HIDE);
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgLoginSkin::DrawTools(CDC *pDC)
{
	CRect rc;
	CShowImage memImage;

	CRect rcShow=m_rcTools;
	if(m_SelectedStatus==PASK_Sel_Tools)
	{
		memImage.SetTransparentColor(RGB(255,0,255));
		memImage.SetImage(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME,1);
		memImage.SetSizingMargin(20,141,20,10);
		memImage.SetShowRegion(m_rcToolsSelected);
		memImage.DrawImage(pDC,0);

		CRect rcTmp;
		m_btnReview->GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.top=rcTmp.bottom;
		rcTmp.bottom=rcTmp.top+2;
		memImage.SetImage(IDB_BITMAP_LOGIN_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		rcShow.OffsetRect(-1,-1);
		memImage.SetImage(IDB_BITMAP_BTN_TOOLS2,1);
	}else
	{
		memImage.SetImage(IDB_BITMAP_BTN_TOOLS,1);
	}

	memImage.SetTransparentColor(RGB(214,222,230));
	pDC->SetBkMode(TRANSPARENT);

	
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rcShow);
	memImage.DrawImage(pDC,0);
	rc=rcShow;
	rc.top=rc.bottom;
	rc.bottom+=20;
	pDC->DrawText("Tools",rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CDlgLoginSkin::DrawConfig(CDC *pDC)
{
	CRect rc;
	CShowImage memImage;

	CRect rcShow=m_rcConfig;
	if(m_SelectedStatus==PASK_Sel_Config)
	{
		memImage.SetTransparentColor(RGB(255,0,255));
		memImage.SetImage(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME,1);
		memImage.SetSizingMargin(20,141,20,10);
		memImage.SetShowRegion(m_rcConfigSelected);
		memImage.DrawImage(pDC,0);

		CRect rcTmp;
		m_btnConfig->GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.top=rcTmp.bottom;
		rcTmp.bottom=rcTmp.top+2;
		memImage.SetImage(IDB_BITMAP_LOGIN_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		rcShow.OffsetRect(-1,-1);

		memImage.SetImage(IDB_BITMAP_BTN_CONFIG2,1);
		memImage.SetSizingMargin(0,0,0,0);
	}else
	{
		memImage.SetImage(IDB_BITMAP_BTN_CONFIG,1);
		memImage.SetSizingMargin(0,0,0,0);
	}

	memImage.SetTransparentColor(RGB(214,222,230));
	pDC->SetBkMode(TRANSPARENT);

	
	memImage.SetShowRegion(rcShow);
	memImage.DrawImage(pDC,0);
	rc=rcShow;
	rc.top=rc.bottom;
	rc.bottom+=20;
	pDC->DrawText("Config",rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CDlgLoginSkin::DrawPatientDB(CDC *pDC)
{
	CRect rc;
	CShowImage memImage;

	CRect rcShow=m_rcPatientDB;
	if(m_SelectedStatus==PASK_Sel_PatientDB)
	{
		memImage.SetTransparentColor(RGB(255,0,255));
		memImage.SetImage(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME,1);
		memImage.SetSizingMargin(20,141,20,10);
		memImage.SetShowRegion(m_rcPatientDBSelected);
		memImage.DrawImage(pDC,0);
		rcShow.OffsetRect(-1,-1);

		memImage.SetImage(IDB_BITMAP_BTN_PATIENTDB2,1);
	}else
	{
		memImage.SetImage(IDB_BITMAP_BTN_PATIENTDB,1);
	}

	memImage.SetTransparentColor(RGB(214,222,230));
	pDC->SetBkMode(TRANSPARENT);

	
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rcShow);
	memImage.DrawImage(pDC,0);
	rc=rcShow;
	rc.left-=5;
	rc.right+=5;
	rc.top=rc.bottom;
	rc.bottom+=20;
	pDC->DrawText("Patient DB",rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CDlgLoginSkin::DrawApplications(CDC *pDC)
{

	CRect rc;
	CShowImage memImage;

	CRect rcShow=m_rcApplication;
	if(m_SelectedStatus==PASK_Sel_Applications)
	{
		

		//memImage.SetTransparentColor(RGB(255,0,255));
		memImage.SetImage(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME,1);
		memImage.SetSizingMargin(20,141,20,10);
		memImage.SetShowRegion(m_rcApplicationSelected);
		memImage.DrawImage(pDC,0);

		CRect rcTmp;
		m_btnCryotherapy->GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.top=rcTmp.bottom;
		rcTmp.bottom=rcTmp.top+2;
		memImage.SetImage(IDB_BITMAP_LOGIN_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		m_btnBiopsy->GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.top=rcTmp.bottom;
		rcTmp.bottom=rcTmp.top+2;
		memImage.SetImage(IDB_BITMAP_LOGIN_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);
		
		rcShow.OffsetRect(-1,-1);
		memImage.SetImage(IDB_BITMAP_BTN_APPLICATION2,1);
	}else
	{
	
		memImage.SetImage(IDB_BITMAP_BTN_APPLICATION,1);
	}

	memImage.SetTransparentColor(RGB(214,222,230));
	pDC->SetBkMode(TRANSPARENT);
		
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rcShow);
	memImage.DrawImage(pDC,0);
	rc=rcShow;
	rc.left-=10;
	rc.right+=10;
	rc.top=rc.bottom;
	rc.bottom+=20;
	pDC->DrawText("Applications",rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CDlgLoginSkin::DrawTPS(CDC *pDC)
{
	CRect rc;
	CShowImage memImage;

	CRect rcShow=m_rcTps;
	if(m_SelectedStatus==PASK_Sel_Tps)
	{
		memImage.SetTransparentColor(RGB(255,0,255));
		memImage.SetImage(IDB_BITMAP_BTN_LOGIN_SELECTED_FRAME,1);
		memImage.SetSizingMargin(20,141,20,10);
		memImage.SetShowRegion(m_rcTpsSelected);
		memImage.DrawImage(pDC,0);

		CRect rcTmp;
		m_btnTPS->GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.top=rcTmp.bottom;
		rcTmp.bottom=rcTmp.top+2;
		memImage.SetImage(IDB_BITMAP_LOGIN_SEPRATE,1);
		memImage.SetSizingMargin(0,0,0,0);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		rcShow.OffsetRect(-1,-1);

		memImage.SetImage(IDB_BITMAP_BTN_TPS,1);
	}else
	{
		memImage.SetImage(IDB_BITMAP_BTN_TPS2,1);
	}

	memImage.SetTransparentColor(RGB(214,222,230));
	pDC->SetBkMode(TRANSPARENT);

	
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rcShow);
	memImage.DrawImage(pDC,0);
	rc=rcShow;
	rc.top=rc.bottom;
	rc.bottom+=20;
	pDC->DrawText("TPS",rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CDlgLoginSkin::SetBiopsyBtn(CImageButton *pBtn)
{
	m_btnBiopsy=pBtn;
}

void CDlgLoginSkin::SetQuitBtn(CImageButton *pBtn)
{
	m_btnQuit=pBtn;
}


void CDlgLoginSkin::SetCryotherapyBtn(CImageButton *pBtn)
{
	m_btnCryotherapy=pBtn;
}

void CDlgLoginSkin::SetTPSBtn(CImageButton *pBtn)
{
	m_btnTPS=pBtn;
}

void CDlgLoginSkin::SetConfigBtn(CImageButton *pBtn)
{
	m_btnConfig=pBtn;
}


void CDlgLoginSkin::SetReviewBtn(CImageButton *pBtn)
{
	m_btnReview=pBtn;
}


/*********** ningsj *********************
 说明:鼠标移动时判断鼠标位置，决定当前选择了那个图标
      改变图标的状态
*****************************************/
void CDlgLoginSkin::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	PASK_SelectedStatus selStatus;
	
	//select applications
	if(m_rcApplication.PtInRect(point) ||
		(m_SelectedStatus==PASK_Sel_Applications && 
		m_rcApplicationSelected.PtInRect(point)))
	{
		selStatus=PASK_Sel_Applications;
	}else if(m_rcConfig.PtInRect(point) ||
		(m_SelectedStatus==PASK_Sel_Config && 
		m_rcConfigSelected.PtInRect(point)))
	{
		selStatus=PASK_Sel_Config;
	}else if(m_rcTools.PtInRect(point) ||
		(m_SelectedStatus==PASK_Sel_Tools && 
		m_rcToolsSelected.PtInRect(point)))
	{
		selStatus=PASK_Sel_Tools;
	}else if(m_rcPatientDB.PtInRect(point) ||
		(m_SelectedStatus==PASK_Sel_PatientDB && 
		m_rcPatientDBSelected.PtInRect(point)))
	{
		selStatus=PASK_Sel_PatientDB;
	}else if(m_rcTps.PtInRect(point) ||
		(m_SelectedStatus==PASK_Sel_Tps && 
		m_rcTpsSelected.PtInRect(point)))
	{
		selStatus=PASK_Sel_Tps;
	}else
	{
		selStatus=PASK_Sel_None;
	}
	
	if(selStatus==PASK_Sel_None)
	{
		
		switch(m_SelectedStatus)
		{
		case PASK_Sel_Applications:
			m_SelectedStatus=selStatus;
			CreateDraw();
			InvalidateRect(&m_rcApplicationSelected,FALSE);
			break;
		case PASK_Sel_Config:
			m_SelectedStatus=selStatus;
			CreateDraw();
			InvalidateRect(&m_rcConfigSelected,FALSE);
			break;
		case PASK_Sel_Tools:
			m_SelectedStatus=selStatus;
			CreateDraw();
			InvalidateRect(&m_rcToolsSelected,FALSE);
			break;
		case PASK_Sel_Tps:
			m_SelectedStatus=selStatus;
			CreateDraw();
			InvalidateRect(&m_rcTpsSelected,FALSE);
			break;
		case PASK_Sel_PatientDB:
			m_SelectedStatus=selStatus;
			CreateDraw();
			InvalidateRect(&m_rcPatientDBSelected,FALSE);
			break;
		default:
			break;
		}
	}else
	{
		if(m_SelectedStatus!=selStatus)
		{
			/*********** ningsj *********************
			说明:恢复背景
			*****************************************/
			
			switch(m_SelectedStatus)
			{
			case PASK_Sel_Applications:
				m_SelectedStatus=selStatus;
				CreateDraw();
				InvalidateRect(&m_rcApplicationSelected,FALSE);
				break;
			case PASK_Sel_Config:
				m_SelectedStatus=selStatus;
				CreateDraw();
				InvalidateRect(&m_rcConfigSelected,FALSE);
				break;
			case PASK_Sel_Tools:
				m_SelectedStatus=selStatus;
				CreateDraw();
				InvalidateRect(&m_rcToolsSelected,FALSE);
				break;
			case PASK_Sel_Tps:
				m_SelectedStatus=selStatus;
				CreateDraw();
				InvalidateRect(&m_rcTpsSelected,FALSE);
				break;
			case PASK_Sel_PatientDB:
				m_SelectedStatus=selStatus;
				CreateDraw();
				InvalidateRect(&m_rcPatientDBSelected,FALSE);
				break;
			default:
				m_SelectedStatus=selStatus;
				CreateDraw();
				break;
			}

			m_SelectedStatus=selStatus;
			//CreateDraw();
			/*********** ningsj *********************
			说明:改变图标的状态
			*****************************************/
			switch(m_SelectedStatus)
			{
			case PASK_Sel_Applications:
				InvalidateRect(&m_rcApplicationSelected,FALSE);
				break;
			case PASK_Sel_Config:
				InvalidateRect(&m_rcConfigSelected,FALSE);
				break;
			case PASK_Sel_Tools:
				InvalidateRect(&m_rcToolsSelected,FALSE);
				break;
			case PASK_Sel_Tps:
				InvalidateRect(&m_rcTpsSelected,FALSE);
				break;
			case PASK_Sel_PatientDB:
				InvalidateRect(&m_rcPatientDBSelected,FALSE);
				break;
			default:
				break;
			}
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);

}

LONG CDlgLoginSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;
}


void CDlgLoginSkin::CreateDraw()
{
	
	CClientDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dc,rc);
	
	DrawBackGround(&m_skinDC);
	DrawApplications(&m_skinDC);
	DrawConfig(&m_skinDC);
	DrawTools(&m_skinDC);
	DrawPatientDB(&m_skinDC);
	DrawTPS(&m_skinDC);
}

void CDlgLoginSkin::DrawBackGround(CDC *pDC)
{
	CRect rc;
	GetClientRect(&rc);
	
	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_DLG_LOGIN_SKIN,1);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetTransparentColor(RGB(255,0,255));
	memImage.SetShowRegion(rc);
	memImage.DrawImage(pDC,0);
}
