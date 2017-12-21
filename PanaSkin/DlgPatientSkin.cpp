// DlgPatient.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPatientSkin.h"
#include "resource.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatientSkin dialog


CDlgPatientSkin::CDlgPatientSkin(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPatientSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sName=m_sAge=m_sSex=m_sID=m_sWeight=m_sDesc="";
}


void CDlgPatientSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatientSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPatientSkin, CDialog)
	//{{AFX_MSG_MAP(CDlgPatientSkin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatientSkin message handlers

void CDlgPatientSkin::CreateDraw()
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

	/*memImage.SetImage(IDR_BITMAP_LOVEFLAG,1);
	memImage.SetSizingMargin(2,2,2,2);
	BITMAP bmpInfo;
	memImage.GetBitmap(&bmpInfo);
	memImage.SetShowRegion((rc.Width()-bmpInfo.bmWidth)/2,0,
		bmpInfo.bmWidth,bmpInfo.bmHeight);
	memImage.DrawImage(&m_skinDC,0);*/

	rc.top=10;

	m_skinDC.SetBkMode(TRANSPARENT);

	CFont *pFont=GetFont();
	CFont *pOldFont=m_skinDC.SelectObject(pFont);

	m_skinDC.SetTextColor(RGB(0,51,169)); //14,15,110));

	CString sTmp;
	int iHeight=30;
	int iSeprate=50;
	rc.left=5;

	rc.bottom=rc.top+iHeight;
	sTmp.Format("Name");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	sTmp.Format(": %s",m_sName);
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;

	rc.top=rc.bottom;
	rc.bottom=rc.top+iHeight;
	sTmp.Format("ID");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	sTmp.Format(": %s",m_sID);
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;

	rc.top=rc.bottom;
	rc.bottom=rc.top+iHeight;
	sTmp.Format("Sex");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	sTmp.Format(": %s",m_sSex);
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;

	rc.top=rc.bottom;
	rc.bottom=rc.top+iHeight;
	sTmp.Format("Age");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	sTmp.Format(": %s",m_sAge);
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;


	rc.top=rc.bottom;
	rc.bottom=rc.top+iHeight;
	sTmp.Format("Weight");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	sTmp.Format(": %s",m_sWeight);
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;


	rc.top=rc.bottom;
	rc.bottom=rc.top+iHeight;
	sTmp.Format("Desc");
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left+=iSeprate;
	m_skinDC.DrawText(":",-1,&rc,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

	rc.left+=10;
	sTmp.Format("%s",m_sDesc);

	rc.top+=7;
	rc.bottom=rc.top+iHeight*5;
	m_skinDC.DrawText(sTmp,-1,&rc,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	rc.left-=iSeprate;

	m_skinDC.SelectObject(pOldFont);

}

BOOL CDlgPatientSkin::OnInitDialog() 
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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatientSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_skinDC.DrawBack(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgPatientSkin::SetName(CString sName)
{
	m_sName=sName;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgPatientSkin::SetID(CString sID)
{
	m_sID=sID;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgPatientSkin::SetSex(CString sSex)
{
	m_sSex=sSex;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgPatientSkin::SetAge(CString sAge)
{
	m_sAge=sAge;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgPatientSkin::SetWeight(CString sWeight)
{
	m_sWeight=sWeight;
	CreateDraw();
	Invalidate(FALSE);
}

void CDlgPatientSkin::SetDesc(CString sDesc)
{
	m_sDesc=sDesc;
	CreateDraw();
	Invalidate(FALSE);
}
