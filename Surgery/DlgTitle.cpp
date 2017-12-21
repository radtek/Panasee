// DlgTitle.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgTitle dialog


CDlgTitle::CDlgTitle(CWnd* pParent /*=NULL*/)
	: CDlgTitleSkin(CDlgTitle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTitle)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sTitle = "XinAoMDT";

	HBITMAP		hBitmap		= NULL;
	HINSTANCE	hInstResource	= NULL;
	
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_T_XINAO), RT_BITMAP);

	// Load bitmap In
//	hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(IDB_T_XINAO), IMAGE_BITMAP, 0, 0, 0);
//
//	m_bmpTitle.Attach(hBitmap);

	SetOperation(PASK_OPERATION_TYPE_NONE);
}

CDlgTitle::~CDlgTitle()
{
//	m_bmpTitle.Detach();
}


void CDlgTitle::DoDataExchange(CDataExchange* pDX)
{
	CDlgTitleSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTitle)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTitle, CDlgTitleSkin)
	//{{AFX_MSG_MAP(CDlgTitle)
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTitle message handlers

/*void CDlgTitle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBitmap* pBitmap;
	CRect rcPaint = dc.m_ps.rcPaint;	
    int nSavedDC = dc.SaveDC();

	CDC dcMem1;
    int nSavedDC1;
    nSavedDC1 = dc.SaveDC();

	pBitmap = &m_bmpTitle;

    dcMem1.CreateCompatibleDC(&dc);
    dcMem1.SelectObject(pBitmap);
	
	int iEdge = 0;
    dc.BitBlt( rcPaint.left + iEdge, 
		rcPaint.top + iEdge, 
		rcPaint.Width(), 
		rcPaint.Height(),
		&dcMem1, 
		rcPaint.left, 
		rcPaint.top, 
		SRCCOPY );
	
    dcMem1.RestoreDC(nSavedDC1);	
    dc.RestoreDC(nSavedDC);	

	// Do not call CDialog::OnPaint() for painting messages
}
*/

void CDlgTitle::SetTitle(OPERATIONTYPE nOperType)
{
	if (nOperType >= OPERATION_TYPE_NONE && nOperType <= OPERATION_TYPE_REVIEW )
	{

		switch (nOperType)
		{
		case OPERATION_TYPE_NONE:
			SetOperation(PASK_OPERATION_TYPE_NONE);
			break;
		case OPERATION_TYPE_BIOPSY:
			SetOperation(PASK_OPERATION_TYPE_BIOPSY);

			break;
		case OPERATION_TYPE_CRYOTHERAPY:
			SetOperation(PASK_OPERATION_TYPE_CRYOTHERAPY);

			break;
		case OPERARION_TYPE_BRACHYTHERAPY:

			break;
		case OPERATION_TYPE_OZONETHERAPY:

			break;
		case OPERATION_TYPE_TPS:
			SetOperation(PASK_OPERATION_TYPE_TPS);
			break;
		case OPERATION_TYPE_REVIEW:
			SetOperation(PASK_OPERATION_TYPE_REVIEW);

			break;
		default:
			break;
		}
	}	
}
