// DlgNavigateSkin.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "DlgNavigateSkin.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNavigateSkin dialog
#define MRI_CONTROL_SCROLL_PUSHDEPTH_NORECALC 100

CDlgNavigateSkin::CDlgNavigateSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNavigateSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_btnScroll=NULL;
	m_btnAdd=NULL;
	m_btnDel=NULL;
	m_editProbeDepth=NULL;
	m_sProbeDepthUnit=NULL;
	m_iMinPushDepth=-200;
	m_iMaxPushDepth=200;
	m_iCurrentPushDepth=-201;

	m_btnView4=m_btnView3=m_btnView2=m_btnView1=NULL;

	m_bEnableProbeTool=FALSE;
}


void CDlgNavigateSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNavigateSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNavigateSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgNavigateSkin)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PANA_MOVECTRL, OnScrollPushDepth)
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNavigateSkin message handlers

LONG CDlgNavigateSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;
}

/*********** ningsj *********************
说明:获取针的Push depth的值
*****************************************/
int CDlgNavigateSkin::GetPushDepth()
{
	return m_iCurrentPushDepth;
}

/*********** ningsj *********************
说明:设置针的Push depth的值
*****************************************/

void CDlgNavigateSkin::SetPushDepth(int dDepth)
{
	if(dDepth==GetPushDepth())
		return;

	if(!m_btnScroll)
		return;

	int iOffset;

	if(dDepth<m_iMinPushDepth)
		iOffset=0;
	else if(dDepth>m_iMaxPushDepth)
		iOffset=m_rcPushDepthScroll.Width();
	else
		iOffset=(int)((dDepth-m_iMinPushDepth)/
			double(m_iMaxPushDepth-m_iMinPushDepth) *
			m_rcPushDepthScroll.Width());
	
	CRect rcWindow,rcWindowNew;
	m_btnScroll->GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	rcWindowNew=rcWindow;
	rcWindowNew.left=m_rcPushDepthScroll.left+iOffset;
	rcWindowNew.right=rcWindowNew.left+rcWindow.Width();
	m_btnScroll->MoveWindow(rcWindowNew);

	m_iCurrentPushDepth=dDepth;

	SendMessage(WM_PANA_MOVECTRL,(unsigned int)m_btnScroll->GetSafeHwnd(),MRI_CONTROL_SCROLL_PUSHDEPTH_NORECALC);
	
}

/*********** ningsj *********************
说明:设置针的Push depth的范围，默认范围是 -200~ 200
     必须指定范围才能正确调用SetPushDepth()和GetPushDepth()函数
*****************************************/
void CDlgNavigateSkin::SetPushDepthRange(int iMin,int iMax)
{
	m_iMinPushDepth=iMin;
	m_iMaxPushDepth=iMax;
}

/*********** ningsj *********************
说明:相应控件移动时消息，改变针的push depth大小
*****************************************/
LONG CDlgNavigateSkin::OnScrollPushDepth(WPARAM w,LPARAM l)
{
	
	HWND hWnd=(HWND)w;
	int iStatus=(int)l;

	if(m_btnScroll!=NULL && hWnd==m_btnScroll->GetSafeHwnd()
		&&iStatus!=MRI_CONTROL_SCROLL_PUSHDEPTH_NORECALC)
	{
		
		CRect rcWindow;
		m_btnScroll->GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);
	
		double dDepth=m_iMinPushDepth+double(m_iMaxPushDepth-m_iMinPushDepth) * 
			double((rcWindow.left-m_rcPushDepthScroll.left)/
			double(m_rcPushDepthScroll.Width()));
		m_iCurrentPushDepth=(int)dDepth;

		if(rcWindow.left<=m_rcPushDepthScroll.left+1)
			m_iCurrentPushDepth=m_iMinPushDepth;

		if(rcWindow.left>=m_rcPushDepthScroll.right-1)
			m_iCurrentPushDepth=m_iMaxPushDepth;
	}

	return 0;
}

BOOL CDlgNavigateSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0,197,50,620);
	if(GetParent())
	{
		CRect rcParent;
		GetParent()->GetClientRect(&rcParent);
		if(rcParent.Width()>rc.Width())
		{
			rc.left=21;
			rc.right=rcParent.Width()-43;
		}
	}
	MoveWindow(rc);

	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);

	CFont *pParentFont=GetFont(); 
	
	int iLeft=20;
	int iTop=20;
	int iWidth=100;
	int iHeight=21;
	
	iTop+=90;

	iLeft=12;
	iTop+=50;
	iWidth=61;
	iHeight=21;
	CShowImage bitBtn;

	if(m_editProbeDepth)
	{
		m_editProbeDepth->MoveWindow(iLeft+65,iTop+60,35,iHeight);
	}

	if(m_sProbeDepthUnit)
	{
		m_sProbeDepthUnit->MoveWindow(iLeft+100,iTop+60,iWidth,iHeight);
	}

	if(m_btnAdd)
	{
		m_btnAdd->MoveWindow(iLeft+151,iTop+iHeight+4,iWidth/2,iHeight);
		m_btnAdd->LoadBitmaps(IDB_BITMAP_BTN_RIGHTROUND,8,8,8,8);
		m_btnAdd->LoadSecondBitmaps(IDB_BITMAP_BTN_PUSHADD,0,0,0,0);
	}

	if(m_btnDel)
	{
		
		m_btnDel->MoveWindow(iLeft+1,iTop+iHeight+4,iWidth/2,iHeight);
		m_btnDel->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		m_btnDel->LoadSecondBitmaps(IDB_BITMAP_BTN_PUSHDEL,8,8,8,8);
	}

	if(m_btnScroll)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_SCROLL);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		m_btnScroll->SetMoveMode(CMoveableCtrl::CtrlMove_HORIZONTAL);
		m_btnScroll->MoveWindow(iLeft+45,iTop+iHeight+7,bmpinfo.bmWidth/4,bmpinfo.bmHeight);
		m_btnScroll->LoadBitmaps(IDB_BITMAP_BTN_SCROLL,8,8,8,8);
		m_rcPushDepthScroll;
		m_rcPushDepthScroll.left=iLeft+1+iWidth/2;
		m_rcPushDepthScroll.top=iTop+iHeight+7;
		m_rcPushDepthScroll.right=iLeft+151-bmpinfo.bmWidth/4; //m_rcPushDepthScroll.left+139-bmpinfo.bmWidth/4;
		m_rcPushDepthScroll.bottom=m_rcPushDepthScroll.top+bmpinfo.bmHeight;
		m_btnScroll->SetMoveRange(m_rcPushDepthScroll);
	}

	int left=10,top=80,width=41,height=38;	
	left+=5;top+=25;
	if(m_btnView1)
	{
		m_btnView1->LoadBitmaps(IDR_PNG_VIEW1,8,8,8,8);
		m_btnView1->MoveWindow(left,top,width,height);
	}
	if(m_btnView2)
	{
		m_btnView2->LoadBitmaps(IDR_PNG_VIEW2,8,8,8,8);
		m_btnView2->MoveWindow(left+width+5,top,width,height);
	}
	if(m_btnView3)
	{
		m_btnView3->LoadBitmaps(IDR_PNG_VIEW3,8,8,8,8);
		m_btnView3->MoveWindow(left+(width+5)*2,top,width,height);
	}
	if(m_btnView4)
	{
		m_btnView4->LoadBitmaps(IDR_PNG_VIEW4,8,8,8,8);
		m_btnView4->MoveWindow(left+(width+5)*3,top,width,height);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNavigateSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_skinDC.DrawBack(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgNavigateSkin::CreateDraw()
{
	CClientDC dcClient(this);

	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dcClient,rc);

	CShowImage memImage;
	memImage.SetImage(IDB_BITMAP_PANAL_SKIN,1);
	memImage.SetSizingMargin(2,2,2,2);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	if(m_bEnableProbeTool)
	{
		rc.left=19;
		rc.top=180;
		rc.right=rc.left+170;
		rc.bottom=rc.top+50;
		CRect rcSrc(0,50,170,100);
		memImage.SetImage(IDB_BITMAP_PROBE_FRAME,1);
		memImage.SetSizingMargin(2,2,2,2);
		memImage.SetShowRegion(rc);
		memImage.DrawImagePart(&m_skinDC,rc,rcSrc);
		
		rc.left=68;
		rc.top=86+130;
		rc.right=rc.left+69;
		rc.bottom=rc.top+21;
		memImage.SetImage(IDR_BITMAP_BTN_BOTHROUND_INVERT,4);
		memImage.SetSizingMargin(8,8,8,8);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(&m_skinDC,2);
		
		rc.left=30;
		rc.top=63+130;
		rc.right=rc.left+139;
		rc.bottom=rc.top+5;
		memImage.SetImage(IDB_BITMAP_SCROLLBAR,1);
		memImage.SetSizingMargin(2,2,2,2);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(&m_skinDC,0);
	}

}



void CDlgNavigateSkin::SetScrollBtn(CImageButton *pBtn)
{
	m_btnScroll = pBtn;
}

void CDlgNavigateSkin::SetAddBtn(CImageButton *pBtn)
{
	m_btnAdd = pBtn;
}

void CDlgNavigateSkin::SetDelBtn(CImageButton *pBtn)
{
	m_btnDel = pBtn;
}

void CDlgNavigateSkin::SetProbeDepthEdit(CPanaEdit *pEdit)
{
	m_editProbeDepth=pEdit;
}

void CDlgNavigateSkin::SetProbeDepthUnitStatic(CPanaStatic *pStatic)
{
	m_sProbeDepthUnit=pStatic;
}

void CDlgNavigateSkin::SetView1(CImageButton *pBtn)
{
	m_btnView1=pBtn;
}

void CDlgNavigateSkin::SetView2(CImageButton *pBtn)
{
	m_btnView2=pBtn;
}

void CDlgNavigateSkin::SetView3(CImageButton *pBtn)
{
	m_btnView3=pBtn;
}

void CDlgNavigateSkin::SetView4(CImageButton *pBtn)
{
	m_btnView4=pBtn;
}

void CDlgNavigateSkin::EnableProbeTool(BOOL bEnable)
{
	m_bEnableProbeTool=bEnable;
}
