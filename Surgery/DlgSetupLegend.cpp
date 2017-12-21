// DlgSetupLegend.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgSetupLegend.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupLegend dialog
#define PANA_SETUP_LEGEND_BLANK "***Central Blank Space***"

CDlgSetupLegend::CDlgSetupLegend(CWnd* pParent /*=NULL*/)
: CPanaseeDialog(CDlgSetupLegend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetupLegend)
	//}}AFX_DATA_INIT
}


void CDlgSetupLegend::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupLegend)
	DDX_Control(pDX, IDC_LIST_RIGHT, m_lstRight);
	DDX_Control(pDX, IDC_LIST_LEFT, m_lstLeft);
	DDX_Control(pDX, IDC_LIST_CENTER, m_lstCenter);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_cmbCategory);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_DEFAULT, m_btnDefault);
	DDX_Control(pDX, IDC_BTN_RIGHT_UP, m_btnRightUp);
	DDX_Control(pDX, IDC_BTN_RIGHT_RIGHT, m_btnRightRight);
	DDX_Control(pDX, IDC_BTN_RIGHT_LEFT, m_btnRightLeft);
	DDX_Control(pDX, IDC_BTN_RIGHT_DOWN, m_btnRightDown);
	DDX_Control(pDX, IDC_BTN_LEFT_UP, m_btnLeftUp);
	DDX_Control(pDX, IDC_BTN_LEFT_RIGHT, m_btnLeftRight);
	DDX_Control(pDX, IDC_BTN_LEFT_LEFT, m_btnLeftLeft);
	DDX_Control(pDX, IDC_BTN_LEFT_DOWN, m_btnLeftDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupLegend, CPanaseeDialog)
//{{AFX_MSG_MAP(CDlgSetupLegend)
ON_BN_CLICKED(IDC_BTN_LEFT_UP, OnBtnLeftUp)
ON_BN_CLICKED(IDC_BTN_LEFT_DOWN, OnBtnLeftDown)
ON_BN_CLICKED(IDC_BTN_LEFT_LEFT, OnBtnLeftLeft)
ON_BN_CLICKED(IDC_BTN_LEFT_RIGHT, OnBtnLeftRight)
ON_BN_CLICKED(IDC_BTN_RIGHT_LEFT, OnBtnRightLeft)
ON_BN_CLICKED(IDC_BTN_RIGHT_RIGHT, OnBtnRightRight)
ON_BN_CLICKED(IDC_BTN_RIGHT_UP, OnBtnRightUp)
ON_BN_CLICKED(IDC_BTN_RIGHT_DOWN, OnBtnRightDown)
ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelchangeComboCategory)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupLegend message handlers

BOOL CDlgSetupLegend::OnInitDialog() 
{
	SetTitleBarVisable(TRUE);
	//EnableSeprateLine(TRUE);
	
	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_btnLeftDown.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnLeftDown.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_DOWN,0,0,0,0);
	m_btnLeftUp.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnLeftUp.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_UP,0,0,0,0);
	m_btnLeftLeft.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnLeftLeft.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_LEFT,0,0,0,0);
	m_btnLeftRight.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnLeftRight.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_RIGHT,0,0,0,0);
	m_btnRightDown.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnRightDown.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_DOWN,0,0,0,0);
	m_btnRightUp.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnRightUp.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_UP,0,0,0,0);
	m_btnRightLeft.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnRightLeft.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_LEFT,0,0,0,0);
	m_btnRightRight.LoadBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_NORMAL,8,8,8,8);
	m_btnRightRight.LoadSecondBitmaps(AfxGetResourceHandle(),IDR_PNG_BTN_RIGHT,0,0,0,0);
	
	CList<CButton*, CButton*> lstBtn;
	lstBtn.AddTail(&m_btnDefault);
	lstBtn.AddTail(&m_btnOK);
	lstBtn.AddTail(&m_btnCancel);
	SetBottomBtnList(&lstBtn);	
	
	m_cmbCategory.SetCurSel(0);
	
	lstCTLegend.AddTail("Manufacturer's Modal Name");
	lstCTLegend.AddTail("Study ID");
	lstCTLegend.AddTail("Series Number");
	lstCTLegend.AddTail("Image Number");
	lstCTLegend.AddTail("Reconstruction Diameter / mm");
	lstCTLegend.AddTail("KVP / kV");
	lstCTLegend.AddTail("X-ray Tube Current / mA");
	lstCTLegend.AddTail("Slice Thickness / mm");
	lstCTLegend.AddTail("Gantry / Detector Tilt / Degrees");
	lstCTLegend.AddTail("Image Time");
	lstCTLegend.AddTail("Window/Level");
	lstCTLegend.AddTail("Institution Name");
	lstCTLegend.AddTail("Patient Name");
	lstCTLegend.AddTail("Study Date");
	lstCTLegend.AddTail("Image Dimension / pixels");
	lstCTLegend.AddTail("Software Version");
	lstCTLegend.AddTail("Freq Dir");
	lstCTLegend.AddTail("Image Position");
	
	lstMRLegend.AddTail("Image Name");
	lstMRLegend.AddTail("Protocol");
	lstMRLegend.AddTail("ReadPoints / PhasePoints");
	lstMRLegend.AddTail("Thickness / Gap and ACQ");
	lstMRLegend.AddTail("TE/TR");
	lstMRLegend.AddTail("Manufacturer's Modal Name");
	lstMRLegend.AddTail("Patient ID");
	lstMRLegend.AddTail("Patient Name");
	lstMRLegend.AddTail("Patient Age and Sex");
	lstMRLegend.AddTail("Series Time");
	lstMRLegend.AddTail("Institution Name");
	lstMRLegend.AddTail("Image Position");
	lstMRLegend.AddTail("FOV");
	lstMRLegend.AddTail("Number of scans");
	lstMRLegend.AddTail("Flip Angle");
	lstMRLegend.AddTail("Echo Train Length");
	lstMRLegend.AddTail("Acquisition Band Width");
	lstMRLegend.AddTail("Window/Level");
	lstMRLegend.AddTail("Software Version");
	lstMRLegend.AddTail("Freq Dir");
		
	Initial();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetupLegend::OnBtnLeftUp() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstLeft.GetCurSel();
	if(iSel == LB_ERR || iSel == 0)
		return;
	CString sTmp;
	m_lstLeft.GetText(iSel,sTmp);
	m_lstLeft.DeleteString(iSel);
	m_lstLeft.InsertString(iSel-1,sTmp);
	m_lstLeft.SetCurSel(iSel-1);
	Resort();
}

void CDlgSetupLegend::OnBtnLeftDown() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstLeft.GetCurSel();
	if(iSel == LB_ERR || iSel == m_lstLeft.GetCount()-1)
		return;
	CString sTmp;
	m_lstLeft.GetText(iSel,sTmp);
	m_lstLeft.DeleteString(iSel);
	m_lstLeft.InsertString(iSel+1,sTmp);
	m_lstLeft.SetCurSel(iSel+1);
	Resort();
}

void CDlgSetupLegend::OnBtnLeftLeft() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstCenter.GetCurSel();
	if(iSel == LB_ERR)
		return;
	CString sTmp;
	m_lstCenter.GetText(iSel,sTmp);
	m_lstCenter.DeleteString(iSel);
	m_lstLeft.AddString(sTmp);
	Resort();
}

void CDlgSetupLegend::OnBtnLeftRight() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstLeft.GetCurSel();
	if(iSel == LB_ERR)
		return;
	CString sTmp;
	m_lstLeft.GetText(iSel,sTmp);
	m_lstLeft.DeleteString(iSel);
	m_lstCenter.AddString(sTmp);
	Resort();
}

void CDlgSetupLegend::OnBtnRightLeft() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstRight.GetCurSel();
	if(iSel == LB_ERR)
		return;
	CString sTmp;
	m_lstRight.GetText(iSel,sTmp);
	m_lstRight.DeleteString(iSel);
	m_lstCenter.AddString(sTmp);
	Resort();
}

void CDlgSetupLegend::OnBtnRightRight() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstCenter.GetCurSel();
	if(iSel == LB_ERR)
		return;
	CString sTmp;
	m_lstCenter.GetText(iSel,sTmp);
	m_lstCenter.DeleteString(iSel);
	m_lstRight.AddString(sTmp);
	Resort();
}

void CDlgSetupLegend::OnBtnRightUp() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstRight.GetCurSel();
	if(iSel == LB_ERR || iSel == 0)
		return;
	CString sTmp;
	m_lstRight.GetText(iSel,sTmp);
	m_lstRight.DeleteString(iSel);
	m_lstRight.InsertString(iSel-1,sTmp);
	m_lstRight.SetCurSel(iSel-1);
	Resort();
}

void CDlgSetupLegend::OnBtnRightDown() 
{
	// TODO: Add your control notification handler code here
	int iSel=m_lstRight.GetCurSel();
	if(iSel == LB_ERR || iSel == m_lstRight.GetCount()-1)
		return;
	CString sTmp;
	m_lstRight.GetText(iSel,sTmp);
	m_lstRight.DeleteString(iSel);
	m_lstRight.InsertString(iSel+1,sTmp);
	m_lstRight.SetCurSel(iSel+1);
	Resort();
}

void CDlgSetupLegend::OnBtnDefault() 
{
	// TODO: Add your control notification handler code here
	CString sTmp;
	sTmp=CONF_SET_DISPLAY_MR_LEFTTOP_DEFAULT;
	GetIndex(sTmp,&lstMRLeftTop);
	sTmp=CONF_SET_DISPLAY_MR_LEFTBOTTOM_DEFAULT;
	GetIndex(sTmp,&lstMRLeftBottom);
	sTmp=CONF_SET_DISPLAY_MR_RIGHTTOP_DEFAULT;
	GetIndex(sTmp,&lstMRRightTop);
	sTmp=CONF_SET_DISPLAY_MR_RIGHTBOTTOM_DEFAULT;
	GetIndex(sTmp,&lstMRRightBottom);
	sTmp=CONF_SET_DISPLAY_CT_LEFTTOP_DEFAULT;
	GetIndex(sTmp,&lstCTLeftTop);
	sTmp=CONF_SET_DISPLAY_CT_LEFTBOTTOM_DEFAULT;
	GetIndex(sTmp,&lstCTLeftBottom);
	sTmp=CONF_SET_DISPLAY_CT_RIGHTTOP_DEFAULT;
	GetIndex(sTmp,&lstCTRightTop);
	sTmp=CONF_SET_DISPLAY_CT_RIGHTBOTTOM_DEFAULT;
	GetIndex(sTmp,&lstCTRightBottom);
	
	Initial();
}

void CDlgSetupLegend::OnOK() 
{
	// TODO: Add extra validation here
	CString sTmp;
	CConfigMgr conf;
	
	SetIndexStr(sTmp,&lstMRLeftTop);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_LEFTTOP,sTmp);
	
	SetIndexStr(sTmp,&lstMRLeftBottom);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_LEFTBOTTOM,sTmp);
	
	SetIndexStr(sTmp,&lstMRRightTop);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_RIGHTTOP,sTmp);
	
	SetIndexStr(sTmp,&lstMRRightBottom);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_RIGHTBOTTOM,sTmp);
	
	SetIndexStr(sTmp,&lstCTLeftTop);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_LEFTTOP,sTmp);
	
	SetIndexStr(sTmp,&lstCTLeftBottom);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_LEFTBOTTOM,sTmp);
	
	SetIndexStr(sTmp,&lstCTRightTop);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_RIGHTTOP,sTmp);
	
	SetIndexStr(sTmp,&lstCTRightBottom);
	conf.Write(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_RIGHTBOTTOM,sTmp);
	
	
	
	CPanaseeDialog::OnOK();
}

void CDlgSetupLegend::OnSelchangeComboCategory() 
{
	// TODO: Add your control notification handler code here
	Initial();
}

void CDlgSetupLegend::SetIndexStr(CString &sIndex, CList<int,int> *m_pList)
{
	sIndex = "";
	POSITION pos=m_pList->GetHeadPosition();
	while(pos)
	{
		int i=m_pList->GetNext(pos);
		CString sTmp=sIndex;
		sIndex.Format("%s,%i",sTmp,i);
	}
	
	if(sIndex.GetLength()>0)
	{
		sIndex = sIndex.Right(sIndex.GetLength()-1);
	}
}

void CDlgSetupLegend::GetIndex(CString sIndex, CList<int,int> *m_pList)
{
	m_pList->RemoveAll();
	sIndex.TrimLeft();
	sIndex.TrimRight();
	while(sIndex.GetLength()>0)
	{
		int iFind = sIndex.Find(",",0);
		if(iFind>0)
		{
			m_pList->AddTail(atoi(sIndex.Left(iFind)));
			sIndex = sIndex.Right(sIndex.GetLength()-iFind -1);
			sIndex.TrimLeft();
			sIndex.TrimRight();
		}else
		{
			m_pList->AddTail(atoi(sIndex));
			return;
		}
	}
	return;
}

void CDlgSetupLegend::Initial()
{
	try
	{
		m_lstRight.ResetContent();
		m_lstLeft.ResetContent();
		m_lstCenter.ResetContent();
		CString sTmp;
		
		if(m_cmbCategory.GetCurSel()==0) //CT Image
		{
			int i=0;
			POSITION pos=lstCTLeftTop.GetHeadPosition();
			while(pos)
			{
				i=lstCTLeftTop.GetNext(pos);
				if(!lstCTLegend.FindIndex(i-1))
					continue;
				sTmp=lstCTLegend.GetAt(lstCTLegend.FindIndex(i-1));
				m_lstLeft.AddString(sTmp);
			}
			m_lstLeft.AddString(PANA_SETUP_LEGEND_BLANK);
			pos=lstCTLeftBottom.GetHeadPosition();
			while(pos)
			{
				i=lstCTLeftBottom.GetNext(pos);
				if(!lstCTLegend.FindIndex(i-1))
					continue;
				sTmp=lstCTLegend.GetAt(lstCTLegend.FindIndex(i-1));
				m_lstLeft.AddString(sTmp);
			}
			pos=lstCTRightTop.GetHeadPosition();
			while(pos)
			{
				i=lstCTRightTop.GetNext(pos);
				if(!lstCTLegend.FindIndex(i-1))
					continue;
				sTmp=lstCTLegend.GetAt(lstCTLegend.FindIndex(i-1));
				m_lstRight.AddString(sTmp);
			}
			m_lstRight.AddString(PANA_SETUP_LEGEND_BLANK);
			pos=lstCTRightBottom.GetHeadPosition();
			while(pos)
			{
				i=lstCTRightBottom.GetNext(pos);
				if(!lstCTLegend.FindIndex(i-1))
					continue;
				sTmp=lstCTLegend.GetAt(lstCTLegend.FindIndex(i-1));
				m_lstRight.AddString(sTmp);
			}
			
			pos=lstCTLegend.GetHeadPosition();
			while(pos)
			{
				sTmp=lstCTLegend.GetNext(pos);
				if(m_lstLeft.FindString(0,sTmp) == LB_ERR && 
					m_lstRight.FindString(0,sTmp) == LB_ERR)
				{
					m_lstCenter.AddString(sTmp);
				}
			}
		}else
		{
			int i=0;
			POSITION pos=lstMRLeftTop.GetHeadPosition();
			while(pos)
			{
				i=lstMRLeftTop.GetNext(pos);
				if(!lstMRLegend.FindIndex(i-1))
					continue;
				sTmp=lstMRLegend.GetAt(lstMRLegend.FindIndex(i-1));
				m_lstLeft.AddString(sTmp);
			}
			m_lstLeft.AddString(PANA_SETUP_LEGEND_BLANK);
			pos=lstMRLeftBottom.GetHeadPosition();
			while(pos)
			{
				i=lstMRLeftBottom.GetNext(pos);
				if(!lstMRLegend.FindIndex(i-1))
					continue;
				sTmp=lstMRLegend.GetAt(lstMRLegend.FindIndex(i-1));
				m_lstLeft.AddString(sTmp);
			}
			pos=lstMRRightTop.GetHeadPosition();
			while(pos)
			{
				i=lstMRRightTop.GetNext(pos);
				if(!lstMRLegend.FindIndex(i-1))
					continue;
				sTmp=lstMRLegend.GetAt(lstMRLegend.FindIndex(i-1));
				m_lstRight.AddString(sTmp);
			}
			m_lstRight.AddString(PANA_SETUP_LEGEND_BLANK);
			pos=lstMRRightBottom.GetHeadPosition();
			while(pos)
			{
				i=lstMRRightBottom.GetNext(pos);
				if(!lstMRLegend.FindIndex(i-1))
					continue;
				sTmp=lstMRLegend.GetAt(lstMRLegend.FindIndex(i-1));
				m_lstRight.AddString(sTmp);
			}
			
			pos=lstMRLegend.GetHeadPosition();
			while(pos)
			{
				sTmp=lstMRLegend.GetNext(pos);
				if(m_lstLeft.FindString(0,sTmp) == LB_ERR && 
					m_lstRight.FindString(0,sTmp) == LB_ERR)
				{
					m_lstCenter.AddString(sTmp);
				}
			}
		}
	}catch(...)
	{
		return;
	}
}

void CDlgSetupLegend::Resort()
{
	CString sTmp;
	if(m_cmbCategory.GetCurSel()==0) //CT Image
	{
		lstCTLeftTop.RemoveAll();
		lstCTLeftBottom.RemoveAll();
		lstCTRightTop.RemoveAll();
		lstCTRightBottom.RemoveAll();
		
		//left
		BOOL bBelongTop=TRUE;
		for(int i=0;i<m_lstLeft.GetCount();i++)
		{
			m_lstLeft.GetText(i,sTmp);
			if(sTmp==PANA_SETUP_LEGEND_BLANK)
			{
				bBelongTop=FALSE;
				continue;
			}
			POSITION pos=lstCTLegend.GetHeadPosition();
			int index=0;
			CString sLegend;
			while(pos)
			{
				sLegend=lstCTLegend.GetNext(pos);
				index++;
				if(sLegend == sTmp)
				{
					if(bBelongTop)
					{
						lstCTLeftTop.AddTail(index);
					}else
					{
						lstCTLeftBottom.AddTail(index);
					}
					break;
				}
			}
		}
		
		//right
		bBelongTop=TRUE;
		for(i=0;i<m_lstRight.GetCount();i++)
		{
			m_lstRight.GetText(i,sTmp);
			if(sTmp==PANA_SETUP_LEGEND_BLANK)
			{
				bBelongTop=FALSE;
				continue;
			}
			POSITION pos=lstCTLegend.GetHeadPosition();
			int index=0;
			CString sLegend;
			while(pos)
			{
				sLegend=lstCTLegend.GetNext(pos);
				index++;
				if(sLegend == sTmp)
				{
					if(bBelongTop)
					{
						lstCTRightTop.AddTail(index);
					}else
					{
						lstCTRightBottom.AddTail(index);
					}
					break;
				}
			}
		}
	}else
	{
		lstMRLeftTop.RemoveAll();
		lstMRLeftBottom.RemoveAll();
		lstMRRightTop.RemoveAll();
		lstMRRightBottom.RemoveAll();
		
		//left
		BOOL bBelongTop=TRUE;
		for(int i=0;i<m_lstLeft.GetCount();i++)
		{
			m_lstLeft.GetText(i,sTmp);
			if(sTmp==PANA_SETUP_LEGEND_BLANK)
			{
				bBelongTop=FALSE;
				continue;
			}
			POSITION pos=lstMRLegend.GetHeadPosition();
			int index=0;
			CString sLegend;
			while(pos)
			{
				sLegend=lstMRLegend.GetNext(pos);
				index++;
				if(sLegend == sTmp)
				{
					if(bBelongTop)
					{
						lstMRLeftTop.AddTail(index);
					}else
					{
						lstMRLeftBottom.AddTail(index);
					}
					break;
				}
			}
		}
		
		//right
		bBelongTop=TRUE;
		for(i=0;i<m_lstRight.GetCount();i++)
		{
			m_lstRight.GetText(i,sTmp);
			if(sTmp==PANA_SETUP_LEGEND_BLANK)
			{
				bBelongTop=FALSE;
				continue;
			}
			POSITION pos=lstMRLegend.GetHeadPosition();
			int index=0;
			CString sLegend;
			while(pos)
			{
				sLegend=lstMRLegend.GetNext(pos);
				index++;
				if(sLegend == sTmp)
				{
					if(bBelongTop)
					{
						lstMRRightTop.AddTail(index);
					}else
					{
						lstMRRightBottom.AddTail(index);
					}
					break;
				}
			}
		}
	}
}
