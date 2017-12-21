// ComSetPage.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "ComSetPage.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSetPage property page

IMPLEMENT_DYNCREATE(CComSetPage, CPropertyPage)

CComSetPage::CComSetPage() : CPropertyPage(CComSetPage::IDD)
//CComSetPage::CComSetPage(CWnd* pParent /*=NULL*/)
//	: CDialog(CComSetPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSetPage)
	m_bHardwareHandShake = FALSE;
	m_bResetHardware = FALSE;
	m_bWireless = FALSE;
	//}}AFX_DATA_INIT
}

CComSetPage::~CComSetPage()
{
}

void CComSetPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSetPage)
	DDX_Control(pDX, IDC_STOP_BITS, m_ctlStopBits);
	DDX_Control(pDX, IDC_PARITY, m_ctlParity);
	DDX_Control(pDX, IDC_DATA_BITS, m_ctlDataBits);
	DDX_Control(pDX, IDC_COM_PORTS, m_ctlCOMPort);
	DDX_Control(pDX, IDC_BAUD_RATE, m_ctlBaudRate);
	DDX_Check(pDX, IDC_HARDWARE, m_bHardwareHandShake);
	DDX_Check(pDX, IDC_RESET, m_bResetHardware);
	DDX_Check(pDX, IDC_WIRELESS, m_bWireless);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSetPage, CDialog)
	//{{AFX_MSG_MAP(CComSetPage)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, OnSelchangeBaudRate)
	ON_CBN_SELCHANGE(IDC_COM_PORTS, OnSelchangeComPorts)
	ON_CBN_SELCHANGE(IDC_DATA_BITS, OnSelchangeDataBits)
	ON_CBN_SELCHANGE(IDC_PARITY, OnSelchangeParity)
	ON_CBN_SELCHANGE(IDC_STOP_BITS, OnSelchangeStopBits)
	ON_BN_CLICKED(IDC_WIRELESS, OnWireless)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSetPage message handlers

BOOL CComSetPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char cCOMPort[10];

	CDialog::OnInitDialog();
	CConfigMgr conf;

	m_iBaudRateSel = conf.Read(COM, COM_BAUD_RATE, 5);
	m_iStopBitsSel = conf.Read(COM, COM_STOP_BITS, 0);
	m_iCOMPortSel = conf.Read(COM, COM_PORT, 0);
	m_iParitySel = conf.Read(COM, COM_PARITY, 0);
	m_iDataBitsSel = conf.Read(COM, COM_DATA_BITS, 0);
	m_bHardwareHandShake = conf.Read(COM, COM_HARDWARE, 1);
	m_bResetHardware = conf.Read(COM, COM_RESET, 1);
	m_bWireless = conf.Read(COM, COM_WIRELESS, 0);

	for ( int i = 1; i < 20; i++ )
	{
		sprintf( cCOMPort, "COM%d", i );
		m_ctlCOMPort.AddString( cCOMPort );
	} /* for */
	m_ctlCOMPort.SetCurSel( m_iCOMPortSel );

	m_ctlBaudRate.AddString( "9600" );
	m_ctlBaudRate.AddString( "14400" );
	m_ctlBaudRate.AddString( "19200" );
	m_ctlBaudRate.AddString( "38400" );
	m_ctlBaudRate.AddString( "57600" );
	m_ctlBaudRate.AddString( "115200" );
	m_ctlBaudRate.AddString( "921600" );
	m_ctlBaudRate.AddString( "1228739" );
	m_ctlBaudRate.SetCurSel( m_iBaudRateSel );

	m_ctlParity.AddString( "None" );
	m_ctlParity.AddString( "Odd" );
	m_ctlParity.AddString( "Even" );
	m_ctlParity.SetCurSel( m_iParitySel );

	m_ctlStopBits.AddString( "1 bit" );
	m_ctlStopBits.AddString( "2 bits" );
	m_ctlStopBits.SetCurSel( m_iStopBitsSel );

	m_ctlDataBits.AddString( "8 bits" );
	m_ctlDataBits.AddString( "7 bits" );
	m_ctlDataBits.SetCurSel( m_iDataBitsSel );

	UpdateData( FALSE );

	OnWireless();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComSetPage::SaveChange()
{
	CConfigMgr conf;

	conf.Write(COM, COM_WIRELESS, m_bWireless);
	conf.Write(COM, COM_RESET, m_bResetHardware);
	conf.Write(COM, COM_HARDWARE, m_bHardwareHandShake);
	conf.Write(COM, COM_PORT, m_iCOMPortSel);
	conf.Write(COM, COM_DATA_BITS, m_iDataBitsSel);
	conf.Write(COM, COM_PARITY, m_iParitySel);
	conf.Write(COM, COM_BAUD_RATE, m_iBaudRateSel);
	conf.Write(COM, COM_STOP_BITS, m_iStopBitsSel);
}

void CComSetPage::OnSelchangeBaudRate() 
{
	// TODO: Add your control notification handler code here
	m_iBaudRateSel = m_ctlBaudRate.GetCurSel();
}

void CComSetPage::OnSelchangeComPorts() 
{
	// TODO: Add your control notification handler code here
	m_iCOMPortSel = m_ctlCOMPort.GetCurSel();
	
}

void CComSetPage::OnSelchangeDataBits() 
{
	// TODO: Add your control notification handler code here
	m_iDataBitsSel = m_ctlDataBits.GetCurSel();
}

void CComSetPage::OnSelchangeParity() 
{
	// TODO: Add your control notification handler code here
	m_iParitySel = m_ctlParity.GetCurSel();
}

void CComSetPage::OnSelchangeStopBits() 
{
	// TODO: Add your control notification handler code here
	m_iStopBitsSel = m_ctlStopBits.GetCurSel();
}

void CComSetPage::OnWireless() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bWireless)
	{
		GetDlgItem(IDC_RESET)->ShowWindow(FALSE);
		GetDlgItem(IDC_WIRELESS_NOTE)->ShowWindow(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_RESET)->ShowWindow(TRUE);
		GetDlgItem(IDC_WIRELESS_NOTE)->ShowWindow(FALSE);
	}
}

BOOL CComSetPage::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	SaveChange();
	
	return CPropertyPage::OnApply();
}
