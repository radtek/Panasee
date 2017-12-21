// PanaMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "panaskin.h"
#include "PanaMsgBox.h"
#include "PanaMetrics.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanaMsgBox dialog

 
CPanaMsgBox::CPanaMsgBox(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPanaMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanaMsgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_szTitle = "Panasee";
	m_szMsg="";
	m_nMsgType=MB_OK;
}


void CPanaMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanaMsgBox)
	//DDX_Control(pDX, IDC_STATIC_MSG, m_editMsg);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanaMsgBox, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPanaMsgBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanaMsgBox message handlers

BOOL CPanaMsgBox::OnInitDialog() 
{
	SetWindowText(m_szTitle);
	SetTitleBarVisable(TRUE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CClientDC pDC(this); 
	CFont *pOldFont=pDC.SelectObject(GetFont());
	
	int nLeft =20;
	int nTop = 30;
	int nRight=20;
	int nFrameLeftEdge=8;
	int nFrameTopEdge=36;
	int nMinWidth=250;

	
	if((m_nMsgType&MB_ICONWARNING)==MB_ICONWARNING||
		(m_nMsgType&MB_ICONERROR)==MB_ICONERROR||
		(m_nMsgType&MB_ICONINFORMATION)==MB_ICONINFORMATION)
	{
		nLeft=74;
	}


	CString sTmp=m_szMsg;
	char* cMsg = sTmp.GetBuffer(2);
	char cSeps[] = "\n";
    char* token = strtok( cMsg, cSeps );
	SIZE size;
	int nMax = 0;
	int nLine = 0;
	while( token != NULL )
	{
		CString stmp = token;
		size = pDC.GetTextExtent(stmp);
		nLine ++;
		if (nMax < size.cx)
			nMax = size.cx;

		token = strtok( NULL, cSeps );
	}
	
	pDC.SelectObject(pOldFont);

	CRect rect; 
	GetWindowRect(rect);

	/*限制窗口的大小*/
	if(nMax>500)
	{
		nMax=500;
	}
	if(nLine>8)
	{
		nLine=8;
	}

	MoveWindow(0,0,
		max(nMax + nFrameLeftEdge*2+nLeft + nRight, nMinWidth), //nFrameLeftEdge*2 + CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH) * 2 + CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_RIGHTFRAME) * 2 + CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)), 
		nFrameTopEdge + nTop*2 + size.cy*nLine + CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_CLIENT_SEPRATE));

	GetDlgItem(IDC_STATIC_PANA_MSG)->MoveWindow(nLeft, nTop, nMax+5, size.cy * nLine);

	m_szMsg.Replace("\n","\r\n");
	GetDlgItem(IDC_STATIC_PANA_MSG)->SetWindowText(m_szMsg);

	CList<CButton*, CButton*> lstBtn;

	if((m_nMsgType&MB_OKCANCEL)==MB_OKCANCEL)
	{
		lstBtn.AddTail(&m_btnOK);
		lstBtn.AddTail(&m_btnCancel);
	}else
	{
		lstBtn.AddTail(&m_btnOK);
		m_btnCancel.ShowWindow(SW_HIDE);
	}

	SetBottomBtnList(&lstBtn,TRUE);

	CenterWindow(GetDesktopWindow());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPanaMsgBox::SetMessage(CString szMsg,UINT nType)
{
	m_szMsg = szMsg;
	m_nMsgType=nType;
}

void CPanaMsgBox::SetTitle(CString szTitle)
{
	m_szTitle = szTitle;
}

void CPanaMsgBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CShowImage showImage;
	int iLeft=15;
	int iTop=20;
	if((m_nMsgType&MB_ICONWARNING)==MB_ICONWARNING)
	{
		showImage.LoadBitmap(IDB_BITMAP_WARNING);
		BITMAP bitinfo;
		showImage.GetBitmap(&bitinfo);
		showImage.SetShowRegion(iLeft,iTop,bitinfo.bmWidth,bitinfo.bmHeight);
		showImage.DrawImage(&dc,0);
	}else if((m_nMsgType&MB_ICONERROR)==MB_ICONERROR)
	{
		showImage.LoadBitmap(IDB_BITMAP_ERROR);
		BITMAP bitinfo;
		showImage.GetBitmap(&bitinfo);
		showImage.SetShowRegion(iLeft,iTop,bitinfo.bmWidth,bitinfo.bmHeight);
		showImage.DrawImage(&dc,0);
	}else if((m_nMsgType&MB_ICONINFORMATION)==MB_ICONINFORMATION)
	{
		showImage.LoadBitmap(IDB_BITMAP_INFORMATION);
		BITMAP bitinfo;
		showImage.GetBitmap(&bitinfo);
		showImage.SetShowRegion(iLeft,iTop,bitinfo.bmWidth,bitinfo.bmHeight);
		showImage.DrawImage(&dc,0);
	}
	// Do not call CPanaseeDialog::OnPaint() for painting messages
}

BOOL CPanaMsgBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;   

	return CPanaseeDialog::PreTranslateMessage(pMsg);
}
