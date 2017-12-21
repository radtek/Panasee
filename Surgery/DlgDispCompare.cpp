// DlgDispCompare.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgDispCompare.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDispCompare dialog

CDlgDispCompare::CDlgDispCompare(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDispCompare::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDispCompare)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bBinded = FALSE;
}


void CDlgDispCompare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDispCompare)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDispCompare, CDialog)
	//{{AFX_MSG_MAP(CDlgDispCompare)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDispCompare message handlers

BOOL CDlgDispCompare::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for (int i=0; i<4; i++)
	{
		m_DlgCmpDispWrap[i].m_dlgDisplay.m_pDlgDispCompare = this;
		m_DlgCmpDispWrap[i].m_bComparison = TRUE;		// Have to set this before create dialog because CDlgDisplayControl::Oninitdialog will use this
		if (m_DlgCmpDispWrap[i].GetSafeHwnd() == NULL)
		{
			m_DlgCmpDispWrap[i].Create(IDD_DLG_DISPLAY_WRAP, this);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDispCompare::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rs;
	GetClientRect(rs);

	int iGap = 1;
	
	if(!m_DlgCmpDispWrap[0].GetSafeHwnd()
		||!m_DlgCmpDispWrap[1].GetSafeHwnd()
		||!m_DlgCmpDispWrap[2].GetSafeHwnd()
		||!m_DlgCmpDispWrap[3].GetSafeHwnd())
		return;

	m_DlgCmpDispWrap[0].MoveWindow(rs.left+iGap, rs.top+iGap, rs.Width()/2-iGap*2, rs.Height()/2-iGap*2);
	m_DlgCmpDispWrap[1].MoveWindow(rs.left+rs.Width()/2, rs.top+iGap, rs.Width()/2-iGap*2, rs.Height()/2-iGap*2);
	m_DlgCmpDispWrap[2].MoveWindow(rs.left+iGap, rs.top+rs.Height()/2, rs.Width()/2-iGap*2, rs.Height()/2-iGap*2);
	m_DlgCmpDispWrap[3].MoveWindow(rs.left+rs.Width()/2, rs.top+rs.Height()/2, rs.Width()/2-iGap*2, rs.Height()/2-iGap*2);
	
	// TODO: Add your message handler code here
	
}

void CDlgDispCompare::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	for (int i=0; i<4; i++)
	{
		if (m_DlgCmpDispWrap[i].GetSafeHwnd())
		{
			m_DlgCmpDispWrap[i].ShowWindow(bShow);
		}
	}
}

int CDlgDispCompare::DisplayMRISeriesToWindow(CMRISeries* pMRISeries, CDisplayDlg* pDispDlg)
{
	int iTargetWndIndex = -1;
	for (int i=0; i<4; i++)
	{
		if (pDispDlg == &(m_DlgCmpDispWrap[i].m_dlgDisplay))
		{
			iTargetWndIndex = i;
			break;
		}
	}

	if (iTargetWndIndex == -1)
		return 1;

	MRIRectSeries* pMRIRectSeries = &(m_MRIRectSeriesCmp[iTargetWndIndex]);
	
	pMRIRectSeries->Init(pMRISeries);

	m_DlgCmpDispWrap[iTargetWndIndex].AddMRIRectSeries(pMRIRectSeries);

	return 0;

}

void CDlgDispCompare::ShowBindedMRIs(int nStep, CDlgDisplayWrap* pSendWrap)
{
	if (m_bBinded)
	{
		for (int i=0; i<4; i++)
		{
			CDlgDisplayWrap* pWrap = &(m_DlgCmpDispWrap[i]);
			if (pWrap && pWrap != pSendWrap)
			{
				pWrap->m_dlgDispControl.ShowImage(pWrap->m_iLinkIndex + nStep);
			}
		}
	}
}

void CDlgDispCompare::BindMRISeries(BOOL bBind)
{
	m_bBinded = bBind;
	for (int i=0; i<4; i++)
	{
		CDlgDisplayWrap *pWrap = &(m_DlgCmpDispWrap[i]);
		if (pWrap)
		{
			pWrap->SetBind();
		}
	}
}

void CDlgDispCompare::UpdateTextureSameSeries(MRIRectSeries *pMRIRectSeries,BOOL bUpdateAllImage)
{
	for (int i=0; i<4; i++)
	{
		CMRISeries *pMRISeries = NULL;
		pMRISeries = m_MRIRectSeriesCmp[i].GetMRISeries();
		if (pMRISeries)
		{
			if(m_bBinded)
			{
				if(pMRISeries->GetName() != pMRIRectSeries->GetMRISeries()->GetName())
				{
					pMRISeries->SetWindowWidthAndLevel(pMRIRectSeries->GetMRISeries()->GetWindowWidth(),
						pMRIRectSeries->GetMRISeries()->GetWindowLevel());
				}
				m_MRIRectSeriesCmp[i].UpdateTexture(bUpdateAllImage);
				m_DlgCmpDispWrap[i].m_dlgDisplay.AddMRIRectLegend(
					m_DlgCmpDispWrap[i].m_dlgDisplay.m_pCurDispMRIRect->GetMRIImg());
				m_DlgCmpDispWrap[i].m_dlgDisplay.m_Scene.DrawScene();
			}else
			{
				if(pMRISeries->GetName() == pMRIRectSeries->GetMRISeries()->GetName())
				{
					m_MRIRectSeriesCmp[i].UpdateTexture(bUpdateAllImage);
					m_DlgCmpDispWrap[i].m_dlgDisplay.AddMRIRectLegend(
						m_DlgCmpDispWrap[i].m_dlgDisplay.m_pCurDispMRIRect->GetMRIImg());
					m_DlgCmpDispWrap[i].m_dlgDisplay.m_Scene.DrawScene();
				}
			}
		}
	}
}

void CDlgDispCompare::Clean()
{
	for (int i=0; i<4; i++)
	{
		MRIRectSeries *pMRIRectSeries = &(m_MRIRectSeriesCmp[i]);
		if (pMRIRectSeries)
		{
			CMRIRectangle* pMRIDispRect = pMRIRectSeries->GetDispMRIRect();
			if (pMRIDispRect)
			{
				m_DlgCmpDispWrap[i].m_dlgDispControl.RemoveCurrentDispImg();
//				m_DlgCmpDispWrap[i].m_dlgDispControl.m_pDlgDisplay->RemoveAllText();
			}
		}
	}
}
