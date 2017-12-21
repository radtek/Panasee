// DlgMriCtrlSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMriCtrlSkin.h"
#include "ShowImage.h"
#include "Resource.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMriCtrlSkin dialog
#define MRI_CONTROL_FRAME_SEPRATE1 120
#define MRI_CONTROL_FRAME_SEPRATE2 250
#define MRI_CONTROL_SCROLL_PUSHDEPTH_NORECALC 100
 
CDlgMriCtrlSkin::CDlgMriCtrlSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMriCtrlSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_btnNewProbe=NULL;
	m_btnSimProbe=NULL;
	m_btnDelProbe=NULL;
	m_btnSelectProbe=NULL;
	m_btnScroll=NULL;
	m_btnAdd=NULL;
	m_btnDel=NULL;
	m_editProbeDepth=NULL;
	m_sProbeDepthUnit=NULL;
	m_iMinPushDepth=-200;
	m_iMaxPushDepth=200;
	m_iCurrentPushDepth=-201;

	m_sPosition=NULL;
	m_sOrient=NULL;
	m_sSwap=NULL;
	m_btnOrient1=NULL;
	m_btnOrient2=NULL;
	m_btnOrient3=NULL;
	m_btnPosition1=NULL;
	m_btnPosition2=NULL;
	m_btnSwap1=NULL;
	m_btnSwap2=NULL;
	
	m_sProtocal=NULL;
	m_btnProtocal=NULL;
	
	m_progressCtrl=NULL;

	m_btnLoad=NULL;
	m_btnPrescan=NULL;
	m_btnScan=NULL;
	m_btnPrescanScan=NULL;
	m_btnStop=NULL;
	m_btnAdvance=NULL;
	m_btnCycle=NULL;
	m_editCycleTime=NULL;
	m_sCycleTimeUnit=NULL;

	m_prescanScanBtnStatus=PASK_PSBT_SCAN;

	m_bCycleEditVisable=TRUE;
	m_bCycleEditEnable=TRUE;
	m_bPushDepthEditEnable=TRUE;

	m_sProtocalTime=NULL;
}


void CDlgMriCtrlSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMriCtrlSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMriCtrlSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgMriCtrlSkin)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PANA_MOVECTRL, OnScrollPushDepth)
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMriCtrlSkin message handlers

/*********** ningsj *********************
说明:获取针的Push depth的值
*****************************************/
int CDlgMriCtrlSkin::GetPushDepth()
{
	return m_iCurrentPushDepth;
}

/*********** ningsj *********************
说明:设置针的Push depth的值
*****************************************/

void CDlgMriCtrlSkin::SetPushDepth(int dDepth)
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
void CDlgMriCtrlSkin::SetPushDepthRange(int iMin,int iMax)
{
	m_iMinPushDepth=iMin;
	m_iMaxPushDepth=iMax;
}

/*********** ningsj *********************
说明:相应控件移动时消息，改变针的push depth大小
*****************************************/
LONG CDlgMriCtrlSkin::OnScrollPushDepth(WPARAM w,LPARAM l)
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

LONG CDlgMriCtrlSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;
}

BOOL CDlgMriCtrlSkin::OnInitDialog() 
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

	//set buttons
	CShowImage bitBtn;

	int iWidth=61;
	int iHeight=21;
	int iTop=30;
	int iLeft=12;
	
	CFont *pParentFont=GetFont(); 
	

	if(m_btnSelectProbe)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_ARROW_DOWN);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		m_btnSelectProbe->MoveWindow(iLeft+55,iTop-28,bmpinfo.bmWidth/4,bmpinfo.bmHeight+1);
		m_btnSelectProbe->LoadBitmaps(IDB_BITMAP_BTN_ARROW_DOWN,8,8,18,8);
		m_btnSelectProbe->SetTextPosition(DT_LEFT);
	}

	if(m_btnNewProbe)
	{
		m_btnNewProbe->MoveWindow(iLeft+1,iTop,iWidth,iHeight);
		m_btnNewProbe->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		
		m_btnNewProbe->SetTextFont(*pParentFont);
	}

	if(m_btnSimProbe)
	{
		m_btnSimProbe->MoveWindow(iLeft+iWidth-1,iTop,iWidth,iHeight);
		m_btnSimProbe->LoadBitmaps(IDB_BITMAP_BTN_NORMAL,8,8,8,8);
		m_btnSimProbe->SetTextFont(*pParentFont);
	}

	if(m_btnDelProbe)
	{
		m_btnDelProbe->MoveWindow(iLeft+iWidth*2-2,iTop,iWidth,iHeight);
		m_btnDelProbe->LoadBitmaps(IDB_BITMAP_BTN_RIGHTROUND,8,8,8,8);
		m_btnDelProbe->SetTextFont(*pParentFont);
	}

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
	
	iLeft=12;
	iTop=MRI_CONTROL_FRAME_SEPRATE1+20;
	iWidth=30;
	iHeight=15;
	if(m_sPosition)
	{
		m_sPosition->MoveWindow(iLeft-5,iTop,iWidth,iHeight);
	}
	if(m_sOrient)
	{
		m_sOrient->MoveWindow(iLeft-5,iTop+30,iWidth,iHeight);
	}
	if(m_sSwap)
	{
		m_sSwap->MoveWindow(iLeft-5,iTop+30*2,iWidth,iHeight);
	}
	
	iLeft=iLeft+30;
	iTop=MRI_CONTROL_FRAME_SEPRATE1+15;
	iWidth=153/3;
	iHeight=21;
	if(m_btnOrient1)
	{
		m_btnOrient1->MoveWindow(iLeft,iTop,iWidth,iHeight);
		m_btnOrient1->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		m_btnOrient1->LoadSecondBitmaps(IDB_BITMAP_BTN_ORIENT1,8,8,4,4);
	}

	if(m_btnOrient2)
	{
		m_btnOrient2->MoveWindow(iLeft+iWidth-1,iTop,iWidth,iHeight);
		m_btnOrient2->LoadBitmaps(IDB_BITMAP_BTN_NORMAL,8,8,8,8);
		m_btnOrient2->LoadSecondBitmaps(IDB_BITMAP_BTN_ORIENT2,8,8,4,4);
	}

	if(m_btnOrient3)
	{
		m_btnOrient3->MoveWindow(iLeft+iWidth*2-2,iTop,iWidth,iHeight);
		m_btnOrient3->LoadBitmaps(IDB_BITMAP_BTN_RIGHTROUND,8,8,8,8);
		m_btnOrient3->LoadSecondBitmaps(IDB_BITMAP_BTN_ORIENT3,8,8,4,4);
	}
	
	iTop+=30;
	iWidth=153/2;

	if(m_btnPosition1)
	{
		m_btnPosition1->MoveWindow(iLeft,iTop,iWidth,iHeight);
		m_btnPosition1->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		m_btnPosition1->LoadSecondBitmaps(IDB_BITMAP_BTN_POSITION1,8,8,4,4);
	}

	if(m_btnPosition2)
	{
		m_btnPosition2->MoveWindow(iLeft+iWidth-1,iTop,iWidth,iHeight);
		m_btnPosition2->LoadBitmaps(IDB_BITMAP_BTN_RIGHTROUND,8,8,8,8);
		m_btnPosition2->LoadSecondBitmaps(IDB_BITMAP_BTN_POSITION2,8,8,4,4);
	}

	iTop+=30;
	iWidth=153/2;

	if(m_btnSwap1)
	{
		m_btnSwap1->MoveWindow(iLeft,iTop,iWidth,iHeight);
		m_btnSwap1->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		m_btnSwap1->LoadSecondBitmaps(IDB_BITMAP_BTN_SWAP1,8,8,4,4);
	}

	if(m_btnSwap2)
	{
		m_btnSwap2->MoveWindow(iLeft+iWidth-1,iTop,iWidth,iHeight);
		m_btnSwap2->LoadBitmaps(IDB_BITMAP_BTN_RIGHTROUND,8,8,8,8);
		m_btnSwap2->LoadSecondBitmaps(IDB_BITMAP_BTN_SWAP2,8,8,4,4);
	}

	if(m_btnAdvance)
	{
		
		bitBtn.LoadBitmap(IDR_BITMAP_MRI_OPTION);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnAdvance->MoveWindow(iLeft-30,iTop+30,iWidth,iHeight);
		m_btnAdvance->LoadBitmaps(IDR_BITMAP_MRI_OPTION,0,0,0,0);
	}
	
	iLeft=1;
	iTop=MRI_CONTROL_FRAME_SEPRATE2+10;
	
	if(m_btnProtocal)
	{
		/*bitBtn.LoadBitmap(IDB_BITMAP_BTN_ARROW);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnProtocal->MoveWindow(iLeft+3,iTop,iWidth,iHeight);
		m_btnProtocal->LoadBitmaps(IDB_BITMAP_BTN_ARROW,8,8,8,8);*/
		m_btnProtocal->ShowWindow(FALSE);
	}

	if(m_sProtocal)
	{
		m_sProtocal->MoveWindow(iLeft+3,iTop+5,150+iWidth,iHeight-5);
	}

	if(m_sProtocalTime)
	{
		m_sProtocalTime->MoveWindow(iLeft+iWidth+3+150,iTop+5,30,iHeight-5);
	}

	bitBtn.LoadBitmap(IDB_BITMAP_BTN_ARROW);
	BITMAP bmpinfo;
	bitBtn.GetBitmap(&bmpinfo);
	iWidth= bmpinfo.bmWidth/4;
	iHeight= bmpinfo.bmHeight;

	if(m_progressCtrl)
	{
		m_progressCtrl->MoveWindow(iLeft+12,iTop+25,190-10,27); //iHeight-1+6);
	}

	iLeft+=13;
	iTop+=25+iHeight+31;
	if(m_btnLoad)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_LOAD);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnLoad->MoveWindow(iLeft-4,iTop-6,iWidth,iHeight);
		m_btnLoad->LoadBitmaps(IDB_BITMAP_BTN_LOAD,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnLoad->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnLoad->SetWindowRgn(rgn,FALSE);
	}

	if(m_btnPrescan)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_PRESCAN);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnPrescan->MoveWindow(iLeft+51,iTop-17,iWidth,iHeight);
		m_btnPrescan->LoadBitmaps(IDB_BITMAP_BTN_PRESCAN,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnPrescan->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rcTmp.bottom*=2;
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		CRgn rgnTmp;
		rgnTmp.CreateRectRgn(rcTmp.left+5,rcTmp.bottom/2,rcTmp.right-5,rcTmp.bottom-5);
		rgn.CombineRgn(&rgn,&rgnTmp,RGN_DIFF);

		m_btnPrescan->SetWindowRgn(rgn,FALSE);
	}

	if(m_btnScan)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_SCAN);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnScan->MoveWindow(iLeft+51,iTop+20,iWidth,iHeight);
		m_btnScan->LoadBitmaps(IDB_BITMAP_BTN_SCAN,0,0,0,0);

		
		CRgn rgn;
		CRect rcTmp;
		m_btnScan->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rcTmp.bottom*=2;
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		CRgn rgnTmp;
		rgnTmp.CreateRectRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom/2);
		rgn.CombineRgn(&rgn,&rgnTmp,RGN_DIFF);
		rgn.OffsetRgn(0,-rcTmp.bottom/2);

		m_btnScan->SetWindowRgn(rgn,FALSE);
	}

	if(m_btnPrescanScan)
	{
		
		//bitBtn.LoadBitmap(IDB_BITMAP_BTN_PRESCANSCAN);
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_PAUSE); 
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnPrescanScan->MoveWindow(iLeft+51,iTop-17,iWidth,iHeight);
		m_btnPrescanScan->LoadBitmaps(IDB_BITMAP_BTN_PAUSE,0,0,0,0);
		//m_btnPrescanScan->LoadBitmaps(IDB_BITMAP_BTN_PRESCANSCAN,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnPrescanScan->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnPrescanScan->SetWindowRgn(rgn,FALSE);
	}

	
	if(m_btnStop)
	{
		
		bitBtn.LoadBitmap(IDB_BITMAP_BTN_STOP);
		BITMAP bmpinfo;
		bitBtn.GetBitmap(&bmpinfo);
		iWidth= bmpinfo.bmWidth/4;
		iHeight= bmpinfo.bmHeight;
		m_btnStop->MoveWindow(iLeft+129,iTop-6,iWidth,iHeight);
		m_btnStop->LoadBitmaps(IDB_BITMAP_BTN_STOP,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnStop->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnStop->SetWindowRgn(rgn,FALSE);
	}

	if(m_btnCycle)
	{
		iWidth=76;
		iHeight=21;
		m_btnCycle->MoveWindow(iLeft+28,iTop+74,iWidth,iHeight);
		m_btnCycle->LoadBitmaps(IDB_BITMAP_BTN_LEFTROUND,8,8,8,8);
		m_btnCycle->LoadSecondBitmaps(IDB_BITMAP_BTN_CYCLE,8,8,4,4);
	}

	if(m_editCycleTime)
	{
		m_editCycleTime->MoveWindow(iLeft+35+iWidth,iTop+78,45,iHeight);
	}

	if(m_sCycleTimeUnit)
	{
		m_sCycleTimeUnit->MoveWindow(iLeft+83+iWidth,iTop+78,6,iHeight);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMriCtrlSkin::SetNewProbeBtn(CImageButton *pBtn)
{
	m_btnNewProbe = pBtn;
}

void CDlgMriCtrlSkin::SetSimProbeBtn(CImageButton *pBtn)
{
	m_btnSimProbe = pBtn;
}

void CDlgMriCtrlSkin::SetDelProbeBtn(CImageButton *pBtn)
{
	m_btnDelProbe = pBtn;
}

void CDlgMriCtrlSkin::SetSelectProbeBtn(CImageButton *pBtn)
{
	m_btnSelectProbe = pBtn;
}


void CDlgMriCtrlSkin::SetScrollBtn(CImageButton *pBtn)
{
	m_btnScroll = pBtn;
}

void CDlgMriCtrlSkin::SetAddBtn(CImageButton *pBtn)
{
	m_btnAdd = pBtn;
}

void CDlgMriCtrlSkin::SetDelBtn(CImageButton *pBtn)
{
	m_btnDel = pBtn;
}

void CDlgMriCtrlSkin::SetProgressCtrl(CProgressCtrl *pCtrl)
{
	m_progressCtrl = pCtrl;
}

//begin
void CDlgMriCtrlSkin::SetOrient1Btn(CImageButton *pBtn)
{
	m_btnOrient1 = pBtn;
}

void CDlgMriCtrlSkin::SetOrient2Btn(CImageButton *pBtn)
{
	m_btnOrient2 = pBtn;
}


void CDlgMriCtrlSkin::SetOrient3Btn(CImageButton *pBtn)
{
	m_btnOrient3 = pBtn;
}

void CDlgMriCtrlSkin::SetPosition1Btn(CImageButton *pBtn)
{
	m_btnPosition1 = pBtn;
}

void CDlgMriCtrlSkin::SetPosition2Btn(CImageButton *pBtn)
{
	m_btnPosition2 = pBtn;
}

void CDlgMriCtrlSkin::SetSwapOffBtn(CImageButton *pBtn)
{
	m_btnSwap1 = pBtn;
}

void CDlgMriCtrlSkin::SetSwapOnBtn(CImageButton *pBtn)
{
	m_btnSwap2 = pBtn;
}

void CDlgMriCtrlSkin::SetOrientStatic(CPanaStatic *pStatic)
{
	m_sOrient = pStatic;
}

void CDlgMriCtrlSkin::SetPositionStatic(CPanaStatic *pStatic)
{
	m_sPosition = pStatic;
}

void CDlgMriCtrlSkin::SetSwapStatic(CPanaStatic *pStatic)
{
	m_sSwap = pStatic;
}

void CDlgMriCtrlSkin::SetProtocalStatic(CPanaStatic *pStatic)
{
	m_sProtocal = pStatic;
}

void CDlgMriCtrlSkin::SetProtocalBtn(CImageButton *pBtn)
{
	m_btnProtocal = pBtn;
}

void CDlgMriCtrlSkin::SetLoadBtn(CImageButton *pBtn)
{
	m_btnLoad = pBtn;
}

void CDlgMriCtrlSkin::SetPrescanBtn(CImageButton *pBtn)
{
	m_btnPrescan = pBtn;
}

void CDlgMriCtrlSkin::SetScanBtn(CImageButton *pBtn)
{
	m_btnScan = pBtn;
}

void CDlgMriCtrlSkin::SetPrescanScanBtn(CImageButton *pBtn)
{
	m_btnPrescanScan = pBtn;
}


void CDlgMriCtrlSkin::SetStopBtn(CImageButton *pBtn)
{
	m_btnStop = pBtn;
}

void CDlgMriCtrlSkin::SetAdvanceBtn(CImageButton *pBtn)
{
	m_btnAdvance = pBtn;
}



void CDlgMriCtrlSkin::SetCycleBtn(CImageButton *pBtn)
{
	m_btnCycle = pBtn;
}

void CDlgMriCtrlSkin::SetProbeDepthEdit(CPanaEdit *pEdit)
{
	m_editProbeDepth=pEdit;
}

void CDlgMriCtrlSkin::SetCycleTimeEdit(CPanaEdit *pEdit)
{
	m_editCycleTime=pEdit;
}

void CDlgMriCtrlSkin::SetProtocalTimeStatic(CPanaStatic *pStatic)
{
	m_sProtocalTime=pStatic;
}


void CDlgMriCtrlSkin::SetProbeDepthUnitStatic(CPanaStatic *pStatic)
{
	m_sProbeDepthUnit=pStatic;
}

void CDlgMriCtrlSkin::SetCycleTimeUnitStatic(CPanaStatic *pStatic)
{
	m_sCycleTimeUnit=pStatic;
}

/*********** ningsj *********************
 说明:
*****************************************/
BOOL CDlgMriCtrlSkin::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDlgMriCtrlSkin::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	m_skinDC.DrawBack(&dc);
	return;

	// Do not call CDialog::OnPaint() for painting messages
}



void CDlgMriCtrlSkin::CreateDraw()
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

	rc.left=19;
	rc.top=0;
	rc.right=rc.left+170;
	rc.bottom=rc.top+109;
	memImage.SetImage(IDB_BITMAP_PROBE_FRAME,1);
	memImage.SetSizingMargin(2,2,2,2);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	rc.left=68;
	rc.top=86;
	rc.right=rc.left+69;
	rc.bottom=rc.top+21;
	memImage.SetImage(IDR_BITMAP_BTN_BOTHROUND_INVERT,4);
	memImage.SetSizingMargin(8,8,8,8);
	memImage.SetShowRegion(rc);
	if(m_bPushDepthEditEnable)
		memImage.DrawImage(&m_skinDC,2);
	else
		memImage.DrawImage(&m_skinDC,3);
	
	rc.left=30;
	rc.top=63;
	rc.right=rc.left+139;
	rc.bottom=rc.top+5;
	memImage.SetImage(IDB_BITMAP_SCROLLBAR,1);
	memImage.SetSizingMargin(2,2,2,2);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);
	
	rc.left=8;
	rc.top=MRI_CONTROL_FRAME_SEPRATE1;
	rc.right=rc.left+185;
	rc.bottom=rc.top+2;
	memImage.SetImage(IDB_BITMAP_SEPRATE,1);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	rc.left=8;
	rc.top=MRI_CONTROL_FRAME_SEPRATE2;
	rc.right=rc.left+185;
	rc.bottom=rc.top+2;
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);

	rc.left=7;
	rc.top=MRI_CONTROL_FRAME_SEPRATE2+57;
	rc.right=rc.left+193;
	rc.bottom=rc.top+98;
	memImage.SetImage(IDB_BITMAP_SCAN_FRAME,1);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&m_skinDC,0);
	
	if(m_bCycleEditVisable)
	{
		rc.left=116;
		rc.top=392;
		rc.right=rc.left+77;
		rc.bottom=rc.top+21;
		memImage.SetImage(IDR_BITAMP_BTN_RIGHTROUND_INVERT,4);
		memImage.SetSizingMargin(8,8,8,8);
		memImage.SetShowRegion(rc);
		memImage.SetTransparentColor(RGB(255,0,255));
		if(m_bCycleEditEnable)
			memImage.DrawImage(&m_skinDC,2);
		else
			memImage.DrawImage(&m_skinDC,3);
	}
}


void CDlgMriCtrlSkin::SetPrescanScanBtnStatus(PASK_UI_PRESCANSCAN_BTN_STATUS status)
{
	m_prescanScanBtnStatus=status;
	if(m_btnPrescanScan)
	{
		if(m_prescanScanBtnStatus==PASK_PSBT_PAUSE)
		{
			m_btnPrescanScan->LoadBitmaps(IDB_BITMAP_BTN_PAUSE,0,0,0,0);
		}else
		{
			m_btnPrescanScan->LoadBitmaps(IDB_BITMAP_BTN_PRESCANSCAN,0,0,0,0);
		}
		m_btnPrescanScan->Invalidate();
	}
}

void CDlgMriCtrlSkin::SetCycleEditVisible(BOOL bVisable)
{
	m_bCycleEditVisable=bVisable;
	CreateDraw();
	CRect rc;
	rc.left=100;
	rc.top=380;
	rc.right=rc.left+190;
	rc.bottom=rc.top+41;
	InvalidateRect(&rc,FALSE);

	if(m_bCycleEditVisable)
	{
		if(m_editCycleTime)
		{
			m_editCycleTime->ShowWindow(SW_SHOW);
		}

		if(m_sCycleTimeUnit)
		{
			m_sCycleTimeUnit->ShowWindow(SW_SHOW);
		}
	}else
	{
		if(m_editCycleTime)
		{
			m_editCycleTime->ShowWindow(SW_HIDE);
		}

		if(m_sCycleTimeUnit)
		{
			m_sCycleTimeUnit->ShowWindow(SW_HIDE);
		}
	}
}

void CDlgMriCtrlSkin::SetCycleEditEnable(BOOL bEnable)
{
	m_bCycleEditEnable=bEnable;
	CreateDraw();
	CRect rc;
	rc.left=100;
	rc.top=380;
	rc.right=rc.left+190;
	rc.bottom=rc.top+41;
	InvalidateRect(&rc,FALSE);

	if(m_editCycleTime)
		m_editCycleTime->EnableWindow(bEnable);
	if(m_sCycleTimeUnit)
		m_sCycleTimeUnit->EnableWindow(bEnable);
}

void CDlgMriCtrlSkin::SetPushDepthEditEnable(BOOL bEnable)
{
	m_bPushDepthEditEnable=bEnable;
	CreateDraw();
	CRect rc;
	rc.left=68;
	rc.top=86;
	rc.right=rc.left+69;
	rc.bottom=rc.top+21;
	InvalidateRect(&rc,FALSE);

	if(m_editProbeDepth)
		m_editProbeDepth->EnableWindow(bEnable);
	if(m_sProbeDepthUnit)
		m_sProbeDepthUnit->EnableWindow(bEnable);
}
