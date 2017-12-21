// DlgSelectSeries.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgSelectSeries.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectSeries dialog


CDlgSelectSeries::CDlgSelectSeries(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CDlgSelectSeries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectSeries)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSeries=NULL;
}


void CDlgSelectSeries::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectSeries)
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_LIST_SERIES, m_seriesList);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_btnImport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectSeries, CPanaseeDialog)
	//{{AFX_MSG_MAP(CDlgSelectSeries)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectSeries message handlers

void CDlgSelectSeries::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_LIST_SERIES)
		m_seriesList.MeasureItem(lpMeasureItemStruct);

	CPanaseeDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CDlgSelectSeries::OnInitDialog() 
{
	SetTitleBarVisable(TRUE);
	EnableSeprateLine(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnImport);
	lstBtn.AddTail(&m_btnSelect);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	
	
	// TODO: Add extra initialization here
	CRect rc;
	GetClientRect(&rc);

	m_seriesList.MoveWindow(5,5,rc.Width()-10,rc.Height()-63);

	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_seriesList.SetExtendedStyle(dwExStyle);
	m_seriesList.InsertColumn( 0, "Series", LVCFMT_LEFT, 180);
	m_seriesList.InsertColumn( 1, "Description", LVCFMT_LEFT, 150);
	m_seriesList.InsertColumn( 2, "Probe", LVCFMT_LEFT, 65);
	m_seriesList.InitHeader();

	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectSeries::Refresh()
{
	CDlgMain *pDlgMain = g_DlgMain;
	CStringArray saMRIs,saProbe,saDesc;

	pDlgMain->m_dlgCryotherapy.m_dlgSeries.GetMRISeriesName(saMRIs,&saProbe,&saDesc);

	int iLineIndex=0;
	for (int i=0; i<saMRIs.GetSize(); i++)
	{
		if(saMRIs[i].Find("MPR",0)>=0)
		{
			continue;
		}
		int nItem = m_seriesList.InsertItem(iLineIndex++, saMRIs[i]);
		m_seriesList.SetItemText(nItem, 1, saDesc[i]);
		m_seriesList.SetItemText(nItem, 2,saProbe[i]);
	}

}


void CDlgSelectSeries::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here
	CDlgMain *pDlgMain = g_DlgMain;

	POSITION pos = m_seriesList.GetFirstSelectedItemPosition();	
	CString sName="";
	if(pos)
	{
		int iItemIndex = m_seriesList.GetNextSelectedItem(pos);
		sName = m_seriesList.GetItemText(iItemIndex, 0);
		
	}else
	{
		return;
	}

	if(sName=="")
		return;

	if(sName.Find("MPR",0)>=0)
	{
		PanaMessageBox("Only original series can be reconstructed.");
		return;
	}


	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(sName);
	if(!pMRISeries)
		return;

	

	if(pMRISeries->GetNumOfImages()<10)
	{
		PanaMessageBox("Images in series must be more than 10.");
		return;
	}

	m_pSeries=pMRISeries;
	m_sName=sName;

	this->OnOK();
}

void CDlgSelectSeries::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE);
}
