// DlgWizardSkin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgWizardSkin.h"
#include "resource.h"
#include "ShowImage.h"
#include "GlobalDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardSkin dialog
extern PASK_DEVICE_TYPE m_deviceType;

CDlgWizardSkin::CDlgWizardSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_btnPre=m_btnQuit=NULL;

	for(int i=0;i<6;i++)
	{
		m_iResult[i]=PASK_WSRET_WAIT;
	}
	m_sStatus="";
	m_iOperationType=PASK_OPERATION_TYPE_BIOPSY;

	m_iUIMode = 1;
}


void CDlgWizardSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgWizardSkin)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_PANA_GETPARENTBK, OnGetParentBK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardSkin message handlers

BOOL CDlgWizardSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	MoveWindow(0,0,1024,768);

	CShowImage bitSkin;
	if(m_btnPre)
	{
		
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_PRE);
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth/4;
		int iHeight= bmpinfo.bmHeight;
		m_btnPre->MoveWindow(113,698,iWidth,iHeight);
		m_btnPre->LoadBitmaps(IDB_BITMAP_BTN_WIZARD_PRE,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnPre->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnPre->SetWindowRgn(rgn,FALSE);

	}	
	if(m_btnQuit)
	{
		
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_QUIT);
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth/4;
		int iHeight= bmpinfo.bmHeight;
		m_btnQuit->MoveWindow(162,692,iWidth,iHeight);
		m_btnQuit->LoadBitmaps(IDB_BITMAP_BTN_WIZARD_QUIT,0,0,0,0);

		CRgn rgn;
		CRect rcTmp;
		m_btnQuit->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnQuit->SetWindowRgn(rgn,FALSE);
	}	

	m_skinDC.EnableDrawBack(FALSE);
	CreateDraw();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardSkin::SetQuitBtn(CImageButton *pBtn)
{
	m_btnQuit=pBtn;
}

void CDlgWizardSkin::SetPreBtn(CImageButton *pBtn)
{
	m_btnPre=pBtn;
}

void CDlgWizardSkin::SetWizardStatus(PASK_WIZARD_STEP iStep,PASK_WIZARD_STEP_RESULT iResult)
{
	switch(iStep)
	{
	case PASK_WSTEP_CAM:
		m_iResult[0]=iResult;
		break;
	case PASK_WSTEP_MRI:
		m_iResult[1]=iResult;
		break;
	case PASK_WSTEP_PAT:
		m_iResult[2]=iResult;
		break;
	case PASK_WSTEP_BAS:
		m_iResult[3]=iResult;
		break;
	case PASK_WSTEP_CAL:
		m_iResult[4]=iResult;
		break;
	default:
		return;
	}
	CreateDraw();
	InvalidateRect(&m_rcUpdate,FALSE);
}

void CDlgWizardSkin::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CShowImage bitSkin;
	

	if(m_btnPre)
	{		
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_PRE);
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth/4;
		int iHeight= bmpinfo.bmHeight;

		m_btnPre->MoveWindow(113,cy-768+698,iWidth,iHeight);
		
		CRgn rgn;
		CRect rcTmp;
		m_btnPre->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnPre->SetWindowRgn(rgn,FALSE);

	}	
	if(m_btnQuit)
	{		
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_QUIT);
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth/4;
		int iHeight= bmpinfo.bmHeight;

		m_btnQuit->MoveWindow(162,cy-768+692,iWidth,iHeight);
		
		CRgn rgn;
		CRect rcTmp;
		m_btnQuit->GetWindowRect(&rcTmp);
		rcTmp.OffsetRect(-rcTmp.left,-rcTmp.top);
		rgn.CreateEllipticRgn(rcTmp.left+5,rcTmp.top+5,rcTmp.right-5,rcTmp.bottom-5);
		m_btnQuit->SetWindowRgn(rgn,FALSE);
	}	
	
	CreateDraw();
}

void CDlgWizardSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_skinDC.DrawBack(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

LONG CDlgWizardSkin::OnGetParentBK(WPARAM w,LPARAM l)
{
	CRect *rc=(CRect *)w;
	CDC *pDC=(CDC *)l;

	pDC->BitBlt(0,0,rc->Width(),rc->Height(),&m_skinDC,rc->left,rc->top,SRCCOPY);
	return TRUE;

}

void CDlgWizardSkin::SetOperation(PASK_OPERATIONTYPE iOperationType)
{
	m_iOperationType=iOperationType;

}

void CDlgWizardSkin::CreateDraw()
{
	CClientDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc,rcTmp;
	GetClientRect(&rc);

	
	m_skinDC.LoadDC(&dc,rc);

	CShowImage memImage;
	memImage.SetTransparentColor(RGB(255,0,255));

	if(m_iUIMode==1)
	{
		memImage.SetImage(IDB_BITMAP_DLG_WIZARDSKIN,2);
		memImage.SetSizingMargin(253,122,724,84);
	}else
	{
		memImage.SetImage(IDB_BITMAP_DLG_WIZARDSKINCT,2);
		memImage.SetSizingMargin(253,122,724,500);
	}
	
	memImage.SetShowRegion(rc);

	if(m_deviceType == PASK_DEVICE_IGS_MF)
	{
		
		switch(m_iOperationType)
		{
		case PASK_OPERATION_TYPE_CRYOTHERAPY:
			memImage.DrawImage(&m_skinDC,1);
			break;
		case PASK_OPERATION_TYPE_BIOPSY:
			memImage.DrawImage(&m_skinDC,0);
			break;
		default:
			memImage.DrawImage(&m_skinDC,0);
			break;
		}
		
		m_skinDC.SetBkMode(TRANSPARENT);
		
		CShowImage bitSkin;
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_SELECTED);
		
		CString sTmp;
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth;
		int iHeight= bmpinfo.bmHeight;
		
		rc.left=9;
		rc.top=177;
		rc.bottom=rc.top+iHeight;
		rc.right=rc.left+iWidth;
		m_rcUpdate=rc;
		switch(m_iResult[0])
		{
		case PASK_WSRET_OK:
			sTmp="Camera initialized    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Camera initialization failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Camera initializing ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Camera initialization    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[0]);
		
		rc.top=rc.top+iHeight-1;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[1])
		{
		case PASK_WSRET_OK:
			sTmp="MRI initialized    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="MRI initialization failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="MRI initializing ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="MRI initialization    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[1]);
		
		rc.top=rc.top+iHeight-2;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[2])
		{
		case PASK_WSRET_OK:
			sTmp="Patient prepared    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Patient preparation failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Patient preparing ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Patient preparation    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[2]);
		
		rc.top=rc.top+iHeight-3;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[3])
		{
		case PASK_WSRET_OK:
			sTmp="Base selected    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Base selection failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Base selecting ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Base selection    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[3]);
		
		rc.top=rc.top+iHeight-4;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[4])
		{
		case PASK_WSRET_OK:
			sTmp="Calibrated    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Calibration failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Calibrating ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Calibration    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[4]);
		
		m_rcUpdate.bottom=rc.bottom;
		
		m_skinDC.SetTextColor(RGB(0,0,0));
		
		GetClientRect(&rc);
		rc.top=rc.bottom-40;
		rc.bottom=rc.top+30;
		rc.left=290;
		rc.right=rc.left+500;
		m_skinDC.DrawText(m_sStatus,rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}else
	{
		switch(m_iOperationType)
		{
		case PASK_OPERATION_TYPE_CRYOTHERAPY:
			memImage.DrawImage(&m_skinDC,1);
			break;
		case PASK_OPERATION_TYPE_BIOPSY:
			memImage.DrawImage(&m_skinDC,0);
			break;
		default:
			memImage.DrawImage(&m_skinDC,0);
			break;
		}
		
		m_skinDC.SetBkMode(TRANSPARENT);
		
		CShowImage bitSkin;
		bitSkin.LoadBitmap(IDB_BITMAP_BTN_WIZARD_SELECTED);
		
		CString sTmp;
		BITMAP bmpinfo;
		bitSkin.GetBitmap(&bmpinfo);
		int iWidth= bmpinfo.bmWidth;
		int iHeight= bmpinfo.bmHeight;
		
		rc.left=9;
		rc.top=177;
		rc.bottom=rc.top+iHeight;
		rc.right=rc.left+iWidth;
		m_rcUpdate=rc;
		switch(m_iResult[0])
		{
		case PASK_WSRET_OK:
			sTmp="Tracker initialized    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Tracker initialization failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Tracker initialing ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Tracker initialization    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[0]);
		
		rc.top=rc.top+iHeight-1;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[2])
		{
		case PASK_WSRET_OK:
			sTmp="Patient prepared    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Patient preparation failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Patient preparing ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Patient preparation    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[2]);
		
		rc.top=rc.top+iHeight-2;
		rc.bottom=rc.top+iHeight;
		switch(m_iResult[4])
		{
		case PASK_WSRET_OK:
			sTmp="Calibrated    ";
			break;
			
		case PASK_WSRET_ERR:
			sTmp="Calibration failed    ";
			break;
		case PASK_WSRET_RUNNING:
			sTmp="Calibrating ...    ";
			break;
		case PASK_WSRET_WAIT:
			sTmp="Calibration    ";
			break;
		default:
			sTmp="Invalid prompt";
			break;
		}
		DrawWizardStatus(&m_skinDC,rc,sTmp,
			m_iResult[4]);
		
		m_rcUpdate.bottom=rc.bottom;
		
		m_skinDC.SetTextColor(RGB(0,0,0));
		
		GetClientRect(&rc);
		rc.top=rc.bottom-40;
		rc.bottom=rc.top+30;
		rc.left=290;
		rc.right=rc.left+500;
		m_skinDC.DrawText(m_sStatus,rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	
}

void CDlgWizardSkin::DrawWizardStatus(CDC *pDC, CRect rc, CString sText,PASK_WIZARD_STEP_RESULT status)
{
	sText+="  ";
	CShowImage memImage;
	memImage.SetTransparentColor(RGB(255,0,255));
	CRect rcTmp;
	switch(status)
	{
	case PASK_WSRET_OK:
		m_skinDC.SetTextColor(RGB(14,15,110));
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_UNSELECTED,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(pDC,0);

		break;
	case PASK_WSRET_ERR:
		m_skinDC.SetTextColor(RGB(253,167,38));
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_SELECTED,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(pDC,0);

		rcTmp=rc;
		rcTmp.left=rcTmp.right-16;
		rcTmp.right=rcTmp.left+12;
		rcTmp.top+=12;
		rcTmp.bottom=rcTmp.top+12;
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_SELECTEDFAIL,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		break;
	case PASK_WSRET_RUNNING:
		m_skinDC.SetTextColor(RGB(253,167,38));
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_SELECTED,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(pDC,0);

		rcTmp=rc;
		rcTmp.left=rcTmp.right-16;
		rcTmp.right=rcTmp.left+12;
		rcTmp.top+=12;
		rcTmp.bottom=rcTmp.top+12;
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_SELECTEDARROW,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rcTmp);
		memImage.DrawImage(pDC,0);

		break;
	case PASK_WSRET_WAIT:
		m_skinDC.SetTextColor(RGB(107,113,167));
		memImage.SetImage(IDB_BITMAP_BTN_WIZARD_UNSELECTED,1);
		memImage.SetSizingMargin(4,4,4,4);
		memImage.SetShowRegion(rc);
		memImage.DrawImage(pDC,0);
		break;
	}

	pDC->DrawText(sText,rc,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

void CDlgWizardSkin::SetStatusMsg(CString sMsg)
{
	m_sStatus=sMsg;
	CreateDraw();

	CRect rc;
	rc.top=730;
	rc.bottom=rc.top+30;
	rc.left=290;
	rc.right=rc.left+500;
	InvalidateRect(&rc,FALSE);
	UpdateWindow();
}

void CDlgWizardSkin::SetUIMode(int mode)
{
	m_iUIMode = mode;
}
