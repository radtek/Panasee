// DlgSeriesSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSeriesSkin.h"
#include "resource.h"
#include "ShowImage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSeriesSkin dialog


CDlgSeriesSkin::CDlgSeriesSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSeriesSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSeriesList=NULL;
	m_btnDicom=NULL;
	m_btnFusion=NULL;
}


void CDlgSeriesSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSeriesSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSeriesSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgSeriesSkin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSeriesSkin message handlers
void CDlgSeriesSkin::CreateDraw()
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

}

void CDlgSeriesSkin::SetSeriesListCtrl(CListCtrl *pListCtrl)
{
	m_pSeriesList=pListCtrl;
}

BOOL CDlgSeriesSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0,187,50,622);
	if(GetParent())
	{
		CRect rcParent;
		GetParent()->GetClientRect(&rcParent);
		if(rcParent.Width()>rc.Width())
		{
			rc.left=18;
			rc.right=rcParent.Width()-39;
		}
	}
	MoveWindow(rc);


	CreateDraw();
	m_skinDC.EnableDrawBack(FALSE);

	rc.OffsetRect(-rc.left,-rc.top);
	rc.bottom-=1;

	if(m_pSeriesList)
	{
		if(m_btnDicom || m_btnFusion)
		{
			m_pSeriesList->MoveWindow(rc.left,rc.top,rc.Width(),rc.Height()-25);
			if(m_btnDicom)
			{
				m_btnDicom->MoveWindow(rc.left+5,rc.bottom-24,65,22);
				if(m_btnFusion)
				{
					m_btnFusion->MoveWindow(rc.left+5+70,rc.bottom-24,45,22);
				}
			}else
			{
				if(m_btnFusion)
				{
					m_btnFusion->MoveWindow(rc.left+5,rc.bottom-24,45,22);
				}
			}
		}else
		{
			m_pSeriesList->MoveWindow(rc);
		}
	}

	CRgn rgn;
	rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);

	this->SetWindowRgn(rgn,FALSE);
	rgn.DeleteObject();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSeriesSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	m_skinDC.DrawBack(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgSeriesSkin::SetDicomBtn(CImageButton *pBtn)
{
	m_btnDicom=pBtn;
}

void CDlgSeriesSkin::SetFusionBtn(CImageButton *pBtn)
{
	m_btnFusion=pBtn;
}
