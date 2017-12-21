// DlgMRIBrowse.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMRIBrowse.h"
#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"
#include "math.h"
#include "DlgMain.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMRIBrowse dialog
extern CString m_sLogPicFolder;

CDlgMRIBrowse::CDlgMRIBrowse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMRIBrowse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMRIBrowse)
	//}}AFX_DATA_INIT

	m_pDispMRISeries = NULL;
	m_pCryDlg = NULL;
	m_iColumn=3;
	m_iRow=3;
	m_iBeginIndex=0;
	m_bLayouted=FALSE;
	m_bDisplayPolylineTool=FALSE;
}

CDlgMRIBrowse::~CDlgMRIBrowse()
{
	Clean();
}


void CDlgMRIBrowse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMRIBrowse)
	DDX_Control(pDX, IDC_BTN_BROW_PRT_SCR, m_btnPrtScr);
	DDX_Control(pDX, IDC_BTN_BROW_QUIT, m_btnQuit);
	DDX_Control(pDX, IDC_BTN_BROW_PAGEUP, m_btnPageUp);
	DDX_Control(pDX, IDC_BTN_BROW_PAGEDOWN, m_btnPageDown);
	DDX_Control(pDX, IDC_BTN_BROW_F9, m_btnF9);
	DDX_Control(pDX, IDC_BTN_BROW_F8, m_btnF8);
	DDX_Control(pDX, IDC_BTN_BROW_F7, m_btnF7);
	DDX_Control(pDX, IDC_BTN_BROW_F6, m_btnF6);
	DDX_Control(pDX, IDC_BTN_BROW_F5, m_btnF5);
	DDX_Control(pDX, IDC_BTN_BROW_F4, m_btnF4);
	DDX_Control(pDX, IDC_BTN_BROW_F3, m_btnF3);
	DDX_Control(pDX, IDC_BTN_BROW_F2, m_btnF2);
	DDX_Control(pDX, IDC_BTN_BROW_F1, m_btnF1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMRIBrowse, CDialog)
	//{{AFX_MSG_MAP(CDlgMRIBrowse)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_BROWSE_ADD_ROW, OnAddRow)
	ON_COMMAND(IDM_BROWSE_DELETE_ROW, OnDeleteRow)
	ON_COMMAND(IDM_BROWSE_ADD_COLUMN, OnAddColumn)
	ON_COMMAND(IDM_BROWSE_DELETE_COLUMN, OnDeleteColumn)
	ON_BN_CLICKED(IDC_BTN_BROW_F1, OnBtnBrowF1)
	ON_BN_CLICKED(IDC_BTN_BROW_F2, OnBtnBrowF2)
	ON_BN_CLICKED(IDC_BTN_BROW_F3, OnBtnBrowF3)
	ON_BN_CLICKED(IDC_BTN_BROW_F4, OnBtnBrowF4)
	ON_BN_CLICKED(IDC_BTN_BROW_F5, OnBtnBrowF5)
	ON_BN_CLICKED(IDC_BTN_BROW_F6, OnBtnBrowF6)
	ON_BN_CLICKED(IDC_BTN_BROW_F7, OnBtnBrowF7)
	ON_BN_CLICKED(IDC_BTN_BROW_F8, OnBtnBrowF8)
	ON_BN_CLICKED(IDC_BTN_BROW_F9, OnBtnBrowF9)
	ON_BN_CLICKED(IDC_BTN_BROW_PAGEDOWN, OnBtnBrowPagedown)
	ON_BN_CLICKED(IDC_BTN_BROW_PAGEUP, OnBtnBrowPageup)
	ON_BN_CLICKED(IDC_BTN_BROW_QUIT, OnBtnBrowQuit)
	ON_BN_CLICKED(IDC_BTN_BROW_PRT_SCR, OnBtnBrowPrtScr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMRIBrowse message handlers

BOOL CDlgMRIBrowse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_BROWSE_ADD_COLUMN, "Add Column");
		pSysMenu->AppendMenu(MF_STRING, IDM_BROWSE_DELETE_COLUMN, "Delete Column");
		pSysMenu->AppendMenu(MF_STRING, IDM_BROWSE_ADD_ROW, "Add Row");
		pSysMenu->AppendMenu(MF_STRING, IDM_BROWSE_DELETE_ROW, "Delete Row");

		
	}

	int ScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(0,0,ScreenWidth,ScreenHeight);

	m_btnF1.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F1,0,0,0,0);
	m_btnF2.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F2,0,0,0,0);
	m_btnF3.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F3,0,0,0,0);
	m_btnF4.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F4,0,0,0,0);
	m_btnF5.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F5,0,0,0,0);
	m_btnF6.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F6,0,0,0,0);
	m_btnF7.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F7,0,0,0,0);
	m_btnF8.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F8,0,0,0,0);
	m_btnF9.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BROW_F9,0,0,0,0);
	m_btnPageDown.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BLACKGROUND,0,0,0,0);
	m_btnPageDown.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_DOWN,0,0,0,0);
	m_btnPageUp.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BLACKGROUND,0,0,0,0);
	m_btnPageUp.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_UP,0,0,0,0);
	m_btnQuit.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BLACKGROUND,0,0,0,0);
	m_btnQuit.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_QUIT,0,0,0,0);
	m_btnPrtScr.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BLACKGROUND,0,0,0,0);
	m_btnPrtScr.LoadSecondBitmaps(AfxGetResourceHandle(),IDB_PNG_TOOL_CAPTURE,0,0,0,0);
	
	m_btnF1.MoveWindow(10+0 * 30,10,20,20);
	m_btnF2.MoveWindow(10+1 * 30,10,20,20);
	m_btnF3.MoveWindow(10+2 * 30,10,20,20);
	m_btnF4.MoveWindow(10+3 * 30,10,20,20);
	m_btnF5.MoveWindow(10+4 * 30,10,20,20);
	m_btnF6.MoveWindow(10+5 * 30,10,20,20);
	m_btnF7.MoveWindow(10+6 * 30,10,20,20);
	m_btnF8.MoveWindow(10+7 * 30,10,20,20);
	m_btnF9.MoveWindow(10+8 * 30,10,20,20);
	m_btnPageDown.MoveWindow(10+9 * 30,10,22,20);
	m_btnPageUp.MoveWindow(10+9 * 30 +32 ,10,22,20);
	m_btnPrtScr.MoveWindow(10+9 * 30 + 32 + 32,10,41*0.6,38*0.6);

	m_btnQuit.MoveWindow(ScreenWidth-40,10,20,20);
	m_rcTextLabel.left=10+10*30+32*2;
	m_rcTextLabel.top=10;
	m_rcTextLabel.right=m_rcTextLabel.left+200;
	m_rcTextLabel.bottom=m_rcTextLabel.top+20;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMRIBrowse::OnAddColumn()
{
	m_iColumn++;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
	
}

void CDlgMRIBrowse::OnDeleteColumn()
{
	if(m_iColumn>1)
	{
		m_iColumn--;
		m_bLayouted=FALSE;
		SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
		
	}
}

void CDlgMRIBrowse::OnPageDown()
{
	if(m_pDispMRISeries==NULL)
		return;
	if(m_pDispMRISeries->GetNumOfImages()<= 
		(m_iBeginIndex + m_iRow*m_iColumn))
		return;

	
	m_iBeginIndex=m_iBeginIndex+m_iRow*m_iColumn;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
	
}

void CDlgMRIBrowse::OnPageUp()
{
	if(m_pDispMRISeries==NULL)
		return;
	if(m_iBeginIndex<=0)
		return;

	m_iBeginIndex=m_iBeginIndex-m_iRow*m_iColumn;
	if(m_iBeginIndex<=0)
		m_iBeginIndex=0;
	
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);

}

void CDlgMRIBrowse::OnAddRow()
{
	m_iRow++;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);

}

void CDlgMRIBrowse::OnDeleteRow()
{
	if(m_iRow>1)
	{
		m_iRow--;
		m_bLayouted=FALSE;
		SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
		
	}
}

void CDlgMRIBrowse::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == IDM_BROWSE_ADD_COLUMN)
	{
		OnAddColumn();
	}
	if (nID == IDM_BROWSE_DELETE_COLUMN)
	{
		OnDeleteColumn();
	}
	if (nID == IDM_BROWSE_ADD_ROW)
	{
		OnAddRow();
	}
	if (nID == IDM_BROWSE_DELETE_ROW)
	{
		OnDeleteRow();
	}

	CDialog::OnSysCommand(nID, lParam);
	
}

void CDlgMRIBrowse::Clean()
{
	if (m_lstDispMRIDlgs.GetCount() > 0)
	{
		POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
		for (;pos;)
		{
			CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			if (pDlg)
			{
				pDlg->DestroyWindow();
				delete pDlg;
			}
		}
		m_lstDispMRIDlgs.RemoveAll();
	}

	if (m_pDispMRISeries && !m_bDisplayPolylineTool)
	{
		for(int i=0;i<m_pDispMRISeries->GetNumOfImages();i++)
		{
			m_pDispMRISeries->GetImageAt(i)->RemoveRotatedData();
		}
		m_pDispMRISeries=NULL;
	}

	
}

void CDlgMRIBrowse::SetMRISeries(CMRISeries* pMRISeries,int iIndex /*=0*/)
{
	m_pDispMRISeries = pMRISeries;
	if (m_pDispMRISeries == NULL)
	{
		return;
	}

	/*计算需要多少个2D显示窗口*/
	int iNeededDlg=0;
	for (int i=iIndex; i<m_pDispMRISeries->GetNumOfImages() && i<m_iBeginIndex + m_iRow * m_iColumn; i++)
	{
		iNeededDlg++;
	}


	/*生成2D显示窗口*/
	if (m_lstDispMRIDlgs.GetCount() != iNeededDlg)
	{
		while(m_lstDispMRIDlgs.GetCount() > iNeededDlg)
		{
			POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
			CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			if (pDlg)
			{
				pDlg->DestroyWindow();
				delete pDlg;
			}
			m_lstDispMRIDlgs.RemoveHead();
			m_bLayouted=FALSE;
		}

		while(m_lstDispMRIDlgs.GetCount() < iNeededDlg)
		{
			CDlgMRI2DDisplay* pDlg = new CDlgMRI2DDisplay;
			pDlg->Create(IDD_DLG_MRI_2D_DISPLAY, this);
			pDlg->SetParent(this);
			pDlg->m_pDlgBrowse = this;
			m_lstDispMRIDlgs.AddTail(pDlg);
			m_bLayouted=FALSE;
		}
		
	}

	POSITION pos=m_lstDispMRIDlgs.GetHeadPosition();
	for (i=iIndex; i<m_pDispMRISeries->GetNumOfImages() && i<m_iBeginIndex + m_iRow * m_iColumn; i++)
	{
		CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			
		pDlg->LoadMRIImage(m_pDispMRISeries, i,FALSE);
		pDlg->ShowWindow(SW_NORMAL);
	}

	m_iBeginIndex = iIndex;
	
	Layout();

	if(m_pDispMRISeries->GetNumOfImages()<=
		(m_iBeginIndex + m_iRow*m_iColumn))
	{
		m_btnPageDown.EnableWindow(FALSE);
	}else
	{
		m_btnPageDown.EnableWindow(TRUE);
	}
	if(m_iBeginIndex>0)
	{
		m_btnPageUp.EnableWindow(TRUE);
	}else
	{
		m_btnPageUp.EnableWindow(FALSE);
	}

	InvalidateRect(&m_rcTextLabel,FALSE);
}

void CDlgMRIBrowse::UpdateImageBMPs()
{
	if (m_pDispMRISeries)
	{
		int iIndex = m_iBeginIndex;
		POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
		for (;pos;)
		{
			CDlgMRI2DDisplay* pDlg = m_lstDispMRIDlgs.GetNext(pos);
			pDlg->LoadMRIImage(m_pDispMRISeries, iIndex,TRUE);
			iIndex ++;
		}
	}
}

void CDlgMRIBrowse::UpdateImage3Ds(int iInitImgIndex /*= -1*/)
{
	if (m_pDispMRISeries && m_pCryDlg)
	{
		for (int i=1; i<5; i++)
		{
			CMRISeries* pMRISeries = m_pCryDlg->m_MRIRectSeries[i].GetMRISeries();
			if ( (pMRISeries && 
				pMRISeries->GetStudyID() == m_pDispMRISeries->GetStudyID() &&
				pMRISeries->GetSeriesID() == m_pDispMRISeries->GetSeriesID()))
				//|| m_pCryDlg->m_pDisplay4Dlg->GetDispWrap(i)->m_dlgDisplay.m_bUpdate)
			{
				m_pCryDlg->DisplayMRISeriesToWindow(m_pDispMRISeries, i, iInitImgIndex);
			}
		}
	}
}

void CDlgMRIBrowse::Layout()
{
	if(m_pDispMRISeries == NULL)
		return;

	//if(m_bLayouted)
		//return;

	int iGap=1;
	int iDisplayWidth;
	int iDisplayHeight;
	int nLeft=0;
	int nTop=0;

	CRect rect;
	GetClientRect(rect);
	rect.top=40;

	iDisplayWidth=(rect.Width()-iGap*(m_iColumn+1))/m_iColumn;
	iDisplayHeight=(rect.Height()-iGap*(m_iRow+1))/m_iRow;
	if(iDisplayWidth<iDisplayHeight)
	{
		iDisplayHeight=iDisplayWidth;
	}else
	{
		iDisplayWidth=iDisplayHeight;
	}

	nLeft=(rect.Width()-iGap*(m_iColumn+1)-iDisplayWidth*m_iColumn)/2;
	nTop=rect.top+(rect.Height()-iGap*(m_iRow+1)-iDisplayHeight*m_iRow)/2;

	InvalidateRect(&rect,FALSE);
	
	// layout images
	int iIndex = 0;
	int nTmpLeft=nLeft;
	CDlgMRI2DDisplay *pDlg2D;
	
	POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
	for (;pos;)
	{		
		pDlg2D = m_lstDispMRIDlgs.GetNext(pos);
		if(pDlg2D)
		{

			CRect rs;
			rs.left=nTmpLeft+iGap;
			rs.top =nTop+iGap;
			rs.right  = rs.left + iDisplayWidth;
			rs.bottom = rs.top + iDisplayHeight;
			pDlg2D->MoveWindow(rs,FALSE);
			pDlg2D->LoadMRIImage();

			iIndex ++;
			if(iIndex % m_iColumn == 0 && pos) 
			{
				nTmpLeft=nLeft;
				nTop += iDisplayHeight + iGap;
			}
			else 
			{
				nTmpLeft += iDisplayWidth + iGap;
			}
		}
	}

	m_bLayouted=TRUE;
	
}

void CDlgMRIBrowse::OnClose() 
{

	//UpdateImage3Ds(-2);
	Clean();  //清除显示窗口
	m_iBeginIndex=0;

	CDialog::OnClose();
}



int CDlgMRIBrowse::GetDlgNum()
{
	return m_lstDispMRIDlgs.GetCount();
}

CDlgMRI2DDisplay* CDlgMRIBrowse::GetDispDlgAt(int iIndex)
{
	if (iIndex >= m_lstDispMRIDlgs.GetCount())
	{
		return NULL;
	}

	POSITION pos = m_lstDispMRIDlgs.GetHeadPosition();
	for (int i=0; i<iIndex; i++)
	{
		m_lstDispMRIDlgs.GetNext(pos);
	}
	return m_lstDispMRIDlgs.GetNext(pos);
}

BOOL CDlgMRIBrowse::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CDlgMRIBrowse::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
		
	dc.FillSolidRect(rect,RGB(0,0,0));  
	
	CString sTmp;
	if(m_pDispMRISeries)
	{
		CFont *pOldFont=dc.SelectObject(this->GetFont());
		int iCur=m_iBeginIndex + m_iRow *m_iColumn;
		if(iCur>m_pDispMRISeries->GetNumOfImages())
			iCur=m_pDispMRISeries->GetNumOfImages();
		sTmp.Format("MRI %s - %i/%i", m_pDispMRISeries->GetName(),iCur,m_pDispMRISeries->GetNumOfImages());
		dc.SetTextColor(RGB(200,200,200));
		dc.TextOut(m_rcTextLabel.left,m_rcTextLabel.top+5,sTmp);
		dc.SelectObject(pOldFont);
	}
	// Do not call CScrollDialog::OnPaint() for painting messages
}


void CDlgMRIBrowse::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu m_menu;
//	m_menu.SetPopupMenu(TRUE);
	m_menu.CreatePopupMenu();

	m_menu.AppendMenu(MF_STRING,IDM_BROWSE_ADD_COLUMN,"Add Column");
	m_menu.AppendMenu(MF_STRING,IDM_BROWSE_DELETE_COLUMN,"Delete Column");
	m_menu.AppendMenu(MF_STRING,IDM_BROWSE_ADD_ROW,"Add Row");
	m_menu.AppendMenu(MF_STRING,IDM_BROWSE_DELETE_ROW,"Delete Row");
	
//	m_menu.AdjustMenuItem();
	m_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMRIBrowse::OnKeyDown(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==39 || nChar ==40 || nChar == 34 || nChar ==88 ) //right row
	{
		OnPageDown();
	}

	if(nChar==37 || nChar ==38 || nChar == 33 || nChar ==90) //left arrow
	{
		OnPageUp();
	}

	
}

void CDlgMRIBrowse::OnBtnBrowF1() 
{
	// TODO: Add your control notification handler code here
	
	m_iRow=1;
	m_iColumn=1;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF2() 
{
	// TODO: Add your control notification handler code here
	m_iRow=1;
	m_iColumn=2;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF3() 
{
	// TODO: Add your control notification handler code here
	m_iRow=2;
	m_iColumn=1;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF4() 
{
	// TODO: Add your control notification handler code here
	m_iRow=1;
	m_iColumn=3;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF5() 
{
	// TODO: Add your control notification handler code here
	m_iRow=3;
	m_iColumn=1;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF6() 
{
	// TODO: Add your control notification handler code here
	m_iRow=2;
	m_iColumn=2;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF7() 
{
	// TODO: Add your control notification handler code here
	m_iRow=2;
	m_iColumn=3;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF8() 
{
	// TODO: Add your control notification handler code here
	m_iRow=3;
	m_iColumn=2;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowF9() 
{
	// TODO: Add your control notification handler code here
	m_iRow=3;
	m_iColumn=3;
	m_bLayouted=FALSE;
	SetMRISeries(m_pDispMRISeries,m_iBeginIndex);
}

void CDlgMRIBrowse::OnBtnBrowPagedown() 
{
	// TODO: Add your control notification handler code here
	OnPageDown();
}

void CDlgMRIBrowse::OnBtnBrowPageup() 
{
	// TODO: Add your control notification handler code here
	OnPageUp();
}

void CDlgMRIBrowse::OnBtnBrowQuit() 
{
	// TODO: Add your control notification handler code here
	this->PostMessage(WM_CLOSE);
}

BOOL CDlgMRIBrowse::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message==WM_KEYDOWN))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
		{
			return TRUE;   
		}
		else
		{
			OnKeyDown(pMsg->wParam);
		}
    }else if((pMsg->message==WM_KEYUP))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
		{
			return TRUE;   
		}
    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMRIBrowse::OnBtnBrowPrtScr() 
{
	// TODO: Add your control notification handler code here
	CDlgMain* pMainFrm = g_DlgMain;
	CDC* hSrcCDC;
	HDC   hScrDC,   hMemDC;           
	HBITMAP   hBitmap,hOldBitmap;     
	int               nX,   nY,   nX2,   nY2;           
	int               nWidth,   nHeight;           
	int               xScrn,   yScrn;                 
	
	//为屏幕创建设备描述表 
	hSrcCDC   =   pMainFrm->GetWindowDC();
	if   (hSrcCDC==0)     
	{
		return;
	}

	hScrDC = hSrcCDC->m_hDC;
	
	//为屏幕设备描述表创建兼容的内存设备描述表 
	hMemDC   =   CreateCompatibleDC(hScrDC); 
	if   (hMemDC==0)     
	{
		return;
	}

	CRect rect;
	pMainFrm->GetWindowRect(&rect);
	//   获得屏幕分辨率 
	xScrn   =   GetDeviceCaps(hScrDC,   HORZRES); 
	yScrn   =   GetDeviceCaps(hScrDC,   VERTRES); 
	
	//   获得选定区域坐标 
	nX   =   rect.left > 0 ? rect.left : 0; 
	nY   =   rect.top > 0 ? rect.top : 0; 
	nX2   =   rect.right; 
	nY2   =   rect.bottom; 
	nWidth   =   nX2   -   nX+1; 
	nHeight   =   nY2   -   nY+1; 

	//   创建一个与屏幕设备描述表兼容的位图 
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight); 
	if   (hBitmap==0)     
	{
		return;
	}
	//   把新位图选到内存设备描述表中 
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap); 
	//   把屏幕设备描述表拷贝到内存设备描述表中 
	BitBlt(hMemDC,0,0,   nWidth,nHeight,hScrDC,   nX,   nY,   SRCCOPY); 
	//得到屏幕位图的句柄 
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap); 
	
	//清除 
	ReleaseDC(hSrcCDC); 
	DeleteDC(hMemDC); 
	DeleteObject(hOldBitmap);
	
	// Get bmp store file name
	CString sFileName;
	CreateDirectory(m_sLogPicFolder, 0);	
	CTime c = CTime::GetCurrentTime();
	sFileName.Format("%s\\%02d%02d%02d.bmp", m_sLogPicFolder, c.GetHour(), c.GetMinute(), c.GetSecond());
	
	CBitmap bmp;
	bmp.Attach(hBitmap);
	CString sMsg;
	if (CGlobalFuncs::SaveBitmapFile(sFileName, bmp))
	{
		sMsg.Format("保存屏幕图像到 %s", sFileName);
	}
	else
	{
		sMsg = "保存屏幕图像失败!";
	}
	CLog::Log(sMsg);
	bmp.Detach();
	DeleteObject(hBitmap);
}
