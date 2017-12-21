// DlgMyAbout.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMyAbout.h"
#include "GlobalFuncs.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMyAbout dialog


CDlgMyAbout::CDlgMyAbout(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgMyAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMyAbout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

//	HBITMAP hBk = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOGO),IMAGE_BITMAP,0,0,LR_COPYFROMRESOURCE|LR_LOADTRANSPARENT|LR_DEFAULTSIZE);       
//	if(NULL!=hBk) 
//	{ 
//		m_bmpLogo.Attach(hBk);
//	} 

}


void CDlgMyAbout::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyAbout)
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyAbout, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgMyAbout)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyAbout message handlers

BOOL CDlgMyAbout::OnInitDialog() 
{
	SetWindowText("About Panasee");
	SetTitleBarVisable(TRUE);
	EnableSeprateLine(TRUE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnOK);
	SetBottomBtnList(&lstBtn);	

	
	// TODO: Add extra initialization here
	//m_btnOK.SetThemeHelper(&m_ThemeHelper);
	CString sVersion = CGlobalFuncs::GetProductVersion(NULL);
	CString sMsg;
	sMsg.Format("Version %s", sVersion);

	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(sMsg);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMyAbout::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	

	CShowImage showImg;
	showImg.SetImage(AfxGetResourceHandle(), IDR_LOGO, 1);

	BITMAP bmp;
	showImg.GetBitmap(&bmp);
	
	showImg.SetShowRegion(20, 20, bmp.bmWidth, bmp.bmHeight);
	showImg.DrawImage(&dc, 0);
	
/*
	CBitmap* pBitmap;
//	CRect rcPaint = dc.m_ps.rcPaint;	
    int nSavedDC = dc.SaveDC();

	CDC dcMem1;
    int nSavedDC1;
    nSavedDC1 = dc.SaveDC();

	pBitmap = &m_bmpLogo;

    dcMem1.CreateCompatibleDC(&dc);
    dcMem1.SelectObject(pBitmap);

	BITMAP bmp;
	pBitmap->GetBitmap(&bmp); 

	BLENDFUNCTION bf;
	ZeroMemory(&bf, sizeof(BLENDFUNCTION));

	bf.AlphaFormat  = 0x01;
	bf.SourceConstantAlpha =255;
	bf.BlendOp = AC_SRC_OVER;
	::AlphaBlend(dc.GetSafeHdc(),20, 20, bmp.bmWidth/2, bmp.bmHeight/2,
			dcMem1.GetSafeHdc(), 0, 0, bmp.bmWidth, bmp.bmHeight,bf);
	
	dcMem1.RestoreDC(nSavedDC1);	
    dc.RestoreDC(nSavedDC);	 
	
*/ 
	// Do not call CDialog::OnPaint() for painting messages
}
