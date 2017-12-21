// PanaseeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PanaseeDialog.h"
#include "MemDC.h"
#include "ShowImage.h"
#include "PanaMetrics.h"
#include "ImageManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaseeDialog dialog
extern HINSTANCE hUser32DllInst;

CPanaseeDialog::CPanaseeDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanaseeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTitleBarVisable=TRUE;
	m_bSeprateLine=FALSE;
	m_marginLeftTop=CRect(0,0,1,1);
	m_marginRightTop=CRect(1,0,1,1);
	m_marginLeftBottom=CRect(0,1,1,1);
	m_marginRightBottom=CRect(1,1,1,1);
	m_marginCenterTop=CRect(1,0,1,1);

	m_bCenterBtn=FALSE;
}

CPanaseeDialog::~CPanaseeDialog()
{
	DeleteObject(m_hbrCtrlColor);
	//m_frameRgn.DeleteObject();
}


void CPanaseeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanaseeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanaseeDialog, CDialog)
	//{{AFX_MSG_MAP(CPanaseeDialog)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaseeDialog message handlers

BOOL CPanaseeDialog::OnInitDialog() 
{
	

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*********** ningsj *********************
	说明:描述图像4个角和顶部中间部分在窗口模板图上的位置
	*****************************************/
	
	if(m_bTitleBarVisable)
	{
		m_marginLeftTop=CRect(0,0,16,45);
		m_marginRightTop=CRect(129,0,145,45);
		m_marginLeftBottom=CRect(0,76,16,91);
		m_marginRightBottom=CRect(129,76,145,91);
		m_marginCenterTop=CRect(60,0,105,45);
	}else
	{
		m_marginLeftTop=CRect(0,0,14,16);
		m_marginRightTop=CRect(45,0,60,16);
		m_marginLeftBottom=CRect(0,50,14,60);
		m_marginRightBottom=CRect(45,50,60,60);
		m_marginCenterTop=CRect(20,0,20,16);
	}

	
	/*
	//使用xp的api实现窗口透明 ningsj
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL; 
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
		if(fun) 
			fun(this->GetSafeHwnd(),RGB(255,0,0),255,1); 

		FreeLibrary(hInst); 
	} */

	m_hbrCtrlColor =::CreateSolidBrush(RGB(232,244,244));

	CalcFrameRegion();
	
	m_btnList.RemoveAll();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPanaseeDialog::OnNcActivate( BOOL bActive )
{
	return TRUE;
}

void CPanaseeDialog::OnSize( UINT nType, int cx, int cy )
{
	CalcFrameRegion();
	Layout();
}

BOOL CPanaseeDialog::OnEraseBkgnd( CDC* pDC )
{
	CRgn rgn;
	
	CRect rc;
	GetClientRect(&rc);
	rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right+1,rc.bottom+1,10,10);

	pDC->SelectClipRgn(&rgn,RGN_COPY);

	pDC->FillSolidRect(rc,RGB(232,244,244));

	if(m_bSeprateLine)
	{
		CShowImage memImage;
	
		rc.top=rc.bottom-CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_SEPRATE);
		rc.bottom=rc.top+2;
		rc.left+=8;
		rc.right-=8;
		memImage.SetImage(IDB_BITMAP_SEPRATE,1);
		memImage.SetSizingMargin(20,1,20,1);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(pDC,0);
	}
	return TRUE;
}

/*********** ningsj *********************
 说明:根据IDB_BITMAP_DLGSKIN生成panasee框架窗口
 功能:使用贴图，先贴四角，再贴4个边框
*****************************************/
void CPanaseeDialog::OnNcPaint()
{
	CWindowDC dc(this);
	CRect rc,rcTemp;
	GetWindowRect(&rc);

	CWindowDC dcc(GetDesktopWindow());
	//dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcc,rc.left,rc.top,SRCCOPY);
	
	
	CRect rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	rcClient.OffsetRect(-rc.left,-rc.top);
	rc.OffsetRect(-rc.left,-rc.top);

	
	CRgn rgn,rgnTmp;
	rgn.CreateRectRgnIndirect(&rc);
	rgnTmp.CreateRoundRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,10,10);
	rgn.CombineRgn(&rgn,&rgnTmp,RGN_DIFF);
	dc.SelectClipRgn(&rgn);
	rgnTmp.DeleteObject();
	rgn.DeleteObject();
	
	//Initialize skin image
	CBitmap *pOldImage;
	CBitmapPng *dlgImage;
	BOOL bAlpha=FALSE;
	if(m_bTitleBarVisable)
		dlgImage=CImageManager::GetImageManager()->LoadImage(IDB_BITMAP_DLGSKIN,bAlpha);
	else
		dlgImage=CImageManager::GetImageManager()->LoadImage(IDB_BITMAP_DLGSKIN2,bAlpha);

	CDC memImageDC;
	memImageDC.CreateCompatibleDC(&dc);
	pOldImage=memImageDC.SelectObject(dlgImage);

	BLENDFUNCTION bf;
	ZeroMemory(&bf, sizeof(BLENDFUNCTION));
	bf.AlphaFormat  = 0x01;
	bf.SourceConstantAlpha =255;
	bf.BlendOp = AC_SRC_OVER;

		
	//draw left top
	rcTemp=m_marginLeftTop;
	::AlphaBlend(dc.GetSafeHdc(),0,0,m_marginLeftTop.Width(),m_marginLeftTop.Height(),
		memImageDC.GetSafeHdc(),m_marginLeftTop.left,m_marginLeftTop.top,m_marginLeftTop.Width(),m_marginLeftTop.Height(),bf);


	//draw left bottom
	rcTemp=CRect(0,rc.Height()-m_marginLeftBottom.Height(),m_marginLeftBottom.Width(),rc.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginLeftBottom.left,m_marginLeftBottom.top,m_marginLeftBottom.Width(),m_marginLeftBottom.Height(),bf);


	//draw right bottom
	rcTemp=CRect(rc.Width()-m_marginRightBottom.Width(),rc.Height()-m_marginRightBottom.Height(),
		rc.Width(),rc.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginRightBottom.left,m_marginRightBottom.top,
		m_marginRightBottom.Width(),m_marginRightBottom.Height(),bf);

	//draw right top
	rcTemp=CRect(rc.Width()-m_marginRightTop.Width(),0,
		rc.Width(),m_marginRightTop.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginRightTop.left,m_marginRightTop.top,
		m_marginRightTop.Width(),m_marginRightTop.Height(),bf);

	//draw left margin
	rcTemp=CRect(0,m_marginLeftTop.Height(),
		m_marginLeftTop.Width(),rc.Height()-m_marginLeftBottom.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),0,m_marginLeftTop.Height(),m_marginLeftTop.Width(),
		m_marginLeftBottom.top-m_marginLeftTop.Height(),bf);

	//draw bottom margin
	rcTemp=CRect(m_marginLeftBottom.Width(),rc.Height()-m_marginLeftBottom.Height(),
		rc.Width()-m_marginRightBottom.Width(),rc.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginLeftBottom.right,m_marginLeftBottom.top,
		m_marginRightBottom.left-m_marginLeftBottom.right,m_marginLeftBottom.Height(),bf);

	//draw right margin
	rcTemp=CRect(rc.Width()-m_marginRightTop.Width(),m_marginRightTop.Height(),
		rc.Width(),rc.Height()-m_marginRightBottom.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginRightTop.left,m_marginRightTop.bottom,
		m_marginRightBottom.Width(),m_marginRightBottom.top-m_marginRightTop.bottom,bf);

	//draw top margin
	CString sTmp;
	this->GetWindowText(sTmp);
	CSize txtSize=memImageDC.GetTextExtent(sTmp);
	//draw text region
	rcTemp=CRect(m_marginLeftTop.Width(),0,m_marginLeftTop.Width()+txtSize.cx,m_marginLeftTop.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginLeftTop.right,m_marginLeftTop.top,
		m_marginCenterTop.left-m_marginLeftTop.right,m_marginCenterTop.Height(),bf);
	

	//draw center tilted edge margin
	rcTemp=CRect(m_marginLeftTop.Width()+txtSize.cx,0,
		m_marginLeftTop.Width()+txtSize.cx+m_marginCenterTop.Width(),
		m_marginCenterTop.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginCenterTop.left,m_marginCenterTop.top,
		m_marginCenterTop.Width(),m_marginCenterTop.Height(),bf);

	//draw other top region
	rcTemp=CRect(m_marginLeftTop.Width()+txtSize.cx+m_marginCenterTop.Width(),0,
		rc.Width()-m_marginRightTop.Width(),m_marginCenterTop.Height());
	::AlphaBlend(dc.GetSafeHdc(),rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),
		memImageDC.GetSafeHdc(),m_marginCenterTop.right,m_marginCenterTop.top,
		m_marginRightTop.left-m_marginCenterTop.right,m_marginCenterTop.Height(),bf);

	//destroy skin image
	memImageDC.SelectObject(pOldImage);
	memImageDC.DeleteDC();

	rcTemp.left=m_marginLeftTop.Width();
	rcTemp.right=m_marginLeftTop.Width()+txtSize.cx;
	rcTemp.top=0;
	rcTemp.bottom=(int)(m_marginLeftTop.Height()*0.8);
	dc.SetBkMode(TRANSPARENT);
	if(m_bTitleBarVisable)
		dc.DrawText(sTmp,rcTemp,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	
}
/*********** ningsj *********************
 说明:根据需要调整客户区域的大小
*****************************************/
void CPanaseeDialog::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp )
{
	if(m_bTitleBarVisable)
	{
		lpncsp->rgrc[0].top += 6;
		lpncsp->rgrc[0].left+=5;
		lpncsp->rgrc[0].right -= 5;
		lpncsp->rgrc[0].bottom -= 3;
	}else
	{
		lpncsp->rgrc[0].top -= 18;
		lpncsp->rgrc[0].left+= 5;
		lpncsp->rgrc[0].right -= 5;
		lpncsp->rgrc[0].bottom -= 3;
	}

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}


/*********** ningsj *********************
 说明:设置控件颜色
*****************************************/
HBRUSH CPanaseeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
		case CTLCOLOR_STATIC:
		case CTLCOLOR_BTN:
			hbr   =   m_hbrCtrlColor;   
            pDC->SetBkColor(RGB(232,244,244)); 
			pDC->SetBkMode(TRANSPARENT);
            //return   hbr;   
			
		default:
			break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CPanaseeDialog::SetTitleBarVisable(BOOL bVisable)
{
	m_bTitleBarVisable=bVisable;
}

void CPanaseeDialog::CalcFrameRegion()
{
	CWindowDC dc(this);

	CRect rc,rcTemp;
	GetWindowRect(&rc);
	rc.OffsetRect(-rc.left,-rc.top);

	CRgn m_frameRgn,tmpRgn;
	int i,j;
	m_frameRgn.CreateRectRgn(0,0,rc.Width(),rc.Height());

	//Initialize skin image
	CBitmap dlgImage,*pOldImage;
	
	if(m_bTitleBarVisable)
		dlgImage.LoadBitmap(IDB_BITMAP_DLGSKIN_MASK);
	else
		dlgImage.LoadBitmap(IDB_BITMAP_DLGSKIN2_MASK);

	CDC memImageDC;
	memImageDC.CreateCompatibleDC(&dc);
	pOldImage=memImageDC.SelectObject(&dlgImage);

	CMemDC memDC;
	
	//draw left top
	rcTemp=m_marginLeftTop;
	memDC.LoadDC(&dc,rcTemp);
	memDC.BitBlt(0,0,m_marginLeftTop.Width(),m_marginLeftTop.Height(),
		&memImageDC,m_marginLeftTop.left,m_marginLeftTop.top,SRCCOPY);
	
	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}


	//draw left bottom
	rcTemp=CRect(0,rc.Height()-m_marginLeftBottom.Height(),m_marginLeftBottom.Width(),rc.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.BitBlt(0,0,m_marginLeftBottom.Width(),m_marginLeftBottom.Height(),
		&memImageDC,m_marginLeftBottom.left,m_marginLeftBottom.top,SRCCOPY);
	
	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}

	//draw right bottom
	rcTemp=CRect(rc.Width()-m_marginRightBottom.Width(),rc.Height()-m_marginRightBottom.Height(),
		rc.Width(),rc.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.BitBlt(0,0,m_marginRightBottom.Width(),m_marginRightBottom.Height(),
		&memImageDC,m_marginRightBottom.left,m_marginRightBottom.top,SRCCOPY);
	
	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}

	//draw right top
	rcTemp=CRect(rc.Width()-m_marginRightTop.Width(),0,
		rc.Width(),m_marginRightTop.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.BitBlt(0,0,m_marginRightTop.Width(),m_marginRightTop.Height(),
		&memImageDC,m_marginRightTop.left,m_marginRightTop.top,SRCCOPY);
	
	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}

	

	//draw top margin
	CString sTmp;
	this->GetWindowText(sTmp);
	CSize txtSize=memDC.GetTextExtent(sTmp);
	//draw text region
	rcTemp=CRect(m_marginLeftTop.Width(),0,m_marginLeftTop.Width()+txtSize.cx,m_marginLeftTop.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.StretchBlt(0,0,rcTemp.Width(),rcTemp.Height(),
		&memImageDC,m_marginLeftTop.right,m_marginLeftTop.top,
		m_marginCenterTop.left-m_marginLeftTop.right,m_marginCenterTop.Height(),SRCCOPY);

	

	//draw center tilted edge margin
	rcTemp=CRect(m_marginLeftTop.Width()+txtSize.cx,0,
		m_marginLeftTop.Width()+txtSize.cx+m_marginCenterTop.Width(),
		m_marginCenterTop.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.StretchBlt(0,0,rcTemp.Width(),rcTemp.Height(),
		&memImageDC,m_marginCenterTop.left,m_marginCenterTop.top,
		m_marginCenterTop.Width(),m_marginCenterTop.Height(),SRCCOPY);

	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}

	//draw other top region
	rcTemp=CRect(m_marginLeftTop.Width()+txtSize.cx+m_marginCenterTop.Width(),0,
		rc.Width()-m_marginRightTop.Width(),m_marginCenterTop.Height());
	memDC.LoadDC(&dc,rcTemp);
	memDC.StretchBlt(0,0,rcTemp.Width(),rcTemp.Height(),
		&memImageDC,m_marginCenterTop.right,m_marginCenterTop.top,
		m_marginRightTop.left-m_marginCenterTop.right,m_marginCenterTop.Height(),SRCCOPY);
	
	for(i=0;i<rcTemp.Width();i++)
	for(j=0;j<rcTemp.Height();j++)
	{
		if(memDC.GetPixel(i,j)==RGB(255,0,0))
		{
			tmpRgn.CreateRectRgn(rcTemp.left+i,rcTemp.top+j,rcTemp.left+i+1,rcTemp.top+j+1);
			m_frameRgn.CombineRgn(&m_frameRgn,&tmpRgn,RGN_DIFF);
			tmpRgn.DeleteObject();
		}
	}

	//destroy skin image
	memImageDC.SelectObject(pOldImage);
	memImageDC.DeleteDC();

	SetWindowRgn(m_frameRgn,FALSE);
	m_frameRgn.DeleteObject();
}



void CPanaseeDialog::EnableSeprateLine(BOOL bEnable)
{
	m_bSeprateLine=bEnable;
}

void CPanaseeDialog::SetBottomBtnList(CList<CButton *,CButton *> *btnList,BOOL bCenter)
{
	if(btnList==NULL)
		return;

	m_btnList.RemoveAll();

	POSITION pos;
	CButton *pBtn;
	pos=btnList->GetHeadPosition();
	for(int i=0;i<btnList->GetCount();i++)
	{
		pBtn=btnList->GetNext(pos);
		m_btnList.AddTail(pBtn);
	}

	m_bCenterBtn=bCenter;
	
	Layout();
}

void CPanaseeDialog::Layout()
{
	if(m_btnList.GetCount()<=0)
		return;

	CRect rc;
	GetClientRect(&rc);
	int iTop=rc.bottom-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_SEPRATE)-
		(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_SEPRATE)-
		CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_HEIGHT))/2)+1;
	
	int iLeft=0;

	if(m_bCenterBtn==FALSE)
	{
		iLeft=rc.right-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH)*m_btnList.GetCount()+
			CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)*(m_btnList.GetCount()-1)+
			CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_RIGHTFRAME));
	}else
	{
		iLeft=(rc.Width()-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH)*m_btnList.GetCount()+
			CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)*(m_btnList.GetCount()-1)))/2;
	}

	POSITION pos;
	CButton *pBtn;
	pos=m_btnList.GetHeadPosition();
	for(int i=0;i<m_btnList.GetCount();i++)
	{
		pBtn=m_btnList.GetNext(pos);
		if(pBtn!=NULL)
		{
			pBtn->MoveWindow(iLeft,iTop,CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH),
				CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_HEIGHT));
			iLeft+=CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)+
				CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH);
		}

	}

	
}
