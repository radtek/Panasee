// DlgMriScanOptions.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgMriScanOptions.h"
#include "DlgScannerControl.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMriScanOptions dialog


CDlgMriScanOptions::CDlgMriScanOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMriScanOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMriScanOptions)
	m_bCycle = FALSE;
	m_fGap = 0.0f;
	m_iInterval = 0;
	m_iNex = 0;
	m_iSlice = 0;
	m_fThickness = 0.0f;
	m_iOrient = 3;
	m_iReferPos = 0;
	m_bNew = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgMriScanOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMriScanOptions)
	DDX_Check(pDX, IDC_MRIOPTION_CHECK_CYCLE_SCAN, m_bCycle);
	DDX_Text(pDX, IDC_MRIOPTION_EDIT_GAP, m_fGap);
	DDX_Text(pDX, IDC_MRIOPTION_EDIT_INTERVAL, m_iInterval);
	DDX_Text(pDX, IDC_MRIOPTION_EDIT_NEX, m_iNex);
	DDX_Text(pDX, IDC_MRIOPTION_EDIT_SLICE, m_iSlice);
	DDX_Text(pDX, IDC_MRIOPTION_EDIT_THICKNESS, m_fThickness);
	DDX_Radio(pDX, IDC_MRIOPTION_RADIO_COR, m_iOrient);
	DDX_Radio(pDX, IDC_MRIOPTION_RADIO_PROBE_CENTER, m_iReferPos);
	DDX_Check(pDX, IDC_MRIOPTION_CHECK_NEW, m_bNew);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMriScanOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgMriScanOptions)
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_SAG, OnMrioptionRadioSag)
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_PROBE, OnMrioptionRadioProbe)
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_COR, OnMrioptionRadioCor)
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_AXI, OnMrioptionRadioAxi)
	ON_BN_CLICKED(IDC_MRIOPTION_CHECK_CYCLE_SCAN, OnMrioptionCheckCycleScan)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_MRI_CENTER, OnMrioptionRadioMriCenter)
	ON_BN_CLICKED(IDC_MRIOPTION_RADIO_PROBE_CENTER, OnMrioptionRadioProbeCenter)
	ON_BN_CLICKED(IDC_MRIOPTION_CHECK_NEW, OnMrioptionCheckNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMriScanOptions message handlers

BOOL CDlgMriScanOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*GetDlgItem(IDC_MRIOPTION_EDIT_THICKNESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_MRIOPTION_EDIT_SLICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_MRIOPTION_EDIT_GAP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MRIOPTION_EDIT_NEX)->EnableWindow(FALSE);*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMriScanOptions::OnMrioptionRadioSag() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_MRIOPTION_RADIO_MRI_CENTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_MRIOPTION_RADIO_PROBE_CENTER)->EnableWindow(TRUE);

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(FALSE);

	if(m_iReferPos==0)
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	}else
	{
	
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(FALSE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(FALSE);
	}
}

void CDlgMriScanOptions::OnMrioptionRadioProbe() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_MRIOPTION_RADIO_MRI_CENTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_MRIOPTION_RADIO_PROBE_CENTER)->EnableWindow(FALSE);

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(TRUE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(TRUE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(TRUE);

	
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	
}

void CDlgMriScanOptions::OnMrioptionRadioCor() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_MRIOPTION_RADIO_MRI_CENTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_MRIOPTION_RADIO_PROBE_CENTER)->EnableWindow(TRUE);

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(FALSE);

	if(m_iReferPos==0)
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	}else
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(FALSE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(FALSE);
	}
}

void CDlgMriScanOptions::OnMrioptionRadioAxi() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_MRIOPTION_RADIO_MRI_CENTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_MRIOPTION_RADIO_PROBE_CENTER)->EnableWindow(TRUE);

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(FALSE);
	if(m_iReferPos==0)
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	}else
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(FALSE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(FALSE);
	}
		
}

void CDlgMriScanOptions::OnMrioptionCheckCycleScan() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bCycle)
	{
		GetDlgItem(IDC_MRIOPTION_EDIT_INTERVAL)->EnableWindow(TRUE);
	}else
	{
		GetDlgItem(IDC_MRIOPTION_EDIT_INTERVAL)->EnableWindow(FALSE);
	}
}


void CDlgMriScanOptions::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString sTmp;
	GetDlgItem(IDC_MRIOPTION_EDIT_THICKNESS)->GetWindowText(sTmp);
	double fThk=atof(sTmp);
	if(fThk<2 || fThk>200)
	{
		PanaMessageBox("Thickness should be between 2.0 to 200.0 mm.");
		GetDlgItem(IDC_MRIOPTION_EDIT_THICKNESS)->SetFocus();
		return;
	}
	sTmp.Format("%.1f",fThk);
	GetDlgItem(IDC_MRIOPTION_EDIT_THICKNESS)->SetWindowText(sTmp);

	GetDlgItem(IDC_MRIOPTION_EDIT_SLICE)->GetWindowText(sTmp);
	int iSlice=atoi(sTmp);
	if(iSlice<1 || iSlice>30)
	{
		PanaMessageBox("Slice should be between 1 to 30 .");
		GetDlgItem(IDC_MRIOPTION_EDIT_SLICE)->SetFocus();
		return;
	}

	GetDlgItem(IDC_MRIOPTION_EDIT_GAP)->GetWindowText(sTmp);
	double fGap=atof(sTmp);
	if(fGap<0 || fGap>200)
	{
		PanaMessageBox("Gap should be between 0.0 to 200.0 mm.");
		GetDlgItem(IDC_MRIOPTION_EDIT_GAP)->SetFocus();
		return;
	}
	sTmp.Format("%.1f",fGap);
	GetDlgItem(IDC_MRIOPTION_EDIT_GAP)->SetWindowText(sTmp);

	GetDlgItem(IDC_MRIOPTION_EDIT_NEX)->GetWindowText(sTmp);
	int iNex=atoi(sTmp);
	if(iNex<1 || iNex>4)
	{
		PanaMessageBox("Nex should be between 1 to 4.");
		GetDlgItem(IDC_MRIOPTION_EDIT_NEX)->SetFocus();
		return;
	}

	GetDlgItem(IDC_MRIOPTION_EDIT_INTERVAL)->GetWindowText(sTmp);
	int iInterval=atoi(sTmp);
	if(m_bCycle && (iInterval<20 || iInterval>1000))
	{
		PanaMessageBox("Interval should be between 20 to 1000 second.");
		GetDlgItem(IDC_MRIOPTION_EDIT_INTERVAL)->SetFocus();
		return;
	}
	
	
	Refresh(1);

	CDialog::OnClose();
}

void CDlgMriScanOptions::Refresh(int iMode)
{
	if(iMode==0)
	{
		
		m_bCycle=m_pScanOption->bCycle;
		m_iInterval=m_pScanOption->iInterval;
	
		m_fGap=m_pScanOption->fGap;
		m_iNex=m_pScanOption->iNex;
		m_iSlice=m_pScanOption->iSlice;
		m_fThickness=m_pScanOption->fThickness;
	
		m_iOrient=m_pScanOption->iOrient;
		m_iReferPos=m_pScanOption->iReferPos;

		m_bNew=m_pScanOption->bNewOrient;

		UpdateData(FALSE);
	}else
	{
		UpdateData();

		
		m_pScanOption->bCycle=m_bCycle;
		m_pScanOption->iInterval=m_iInterval;
	
		m_pScanOption->fGap=m_fGap;
		m_pScanOption->iNex=m_iNex;
		m_pScanOption->iSlice=m_iSlice;
		m_pScanOption->fThickness=m_fThickness;
	
		m_pScanOption->iOrient=m_iOrient;
		m_pScanOption->iReferPos=m_iReferPos;

		m_pScanOption->bNewOrient=m_bNew;
	}
}

void CDlgMriScanOptions::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	this->PostMessage(WM_CLOSE);
}

void CDlgMriScanOptions::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CShowImage memImage;
	
	CRect rc;
	rc.left=8;
	rc.top=37;
	rc.right=rc.left+185;
	rc.bottom=rc.top+2;
	memImage.SetImage(AfxGetResourceHandle(),IDB_BITMAP_SEPRATE1,1);
	memImage.SetSizingMargin(0,0,0,0);
	memImage.SetShowRegion(rc);
	memImage.DrawImage(&dc,0);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgMriScanOptions::OnMrioptionRadioMriCenter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(FALSE);

	if(m_iReferPos==0)
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	}else
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(FALSE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(FALSE);
	}
}

void CDlgMriScanOptions::OnMrioptionRadioProbeCenter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane1.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane2.EnableWindow(FALSE);
	((CDlgScannerControl*)m_pDlgScannerControl)->m_btnPlane3.EnableWindow(FALSE);

	if(m_iReferPos==0)
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(TRUE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(TRUE);
	}else
	{
		
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioProbe.EnableWindow(FALSE);
		((CDlgScannerControl*)m_pDlgScannerControl)->m_btnRadioLastPos.EnableWindow(FALSE);
	}
}

void CDlgMriScanOptions::OnMrioptionCheckNew() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_pScanOption->bNewOrient=m_bNew;
}
