// DlgDisplayControl.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgDisplayControl.h"
#include "MRIRectangle.h"
#include "MRIRectSeries.h"
#include "DisplayDlg.h"
#include "CryotherapyDlg.h"
#include "Dispaly4Dlg.h"
#include "DlgDisplayWrap.h"
#include "DlgDispCompare.h"
#include "Probe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayControl dialog


CDlgDisplayControl::CDlgDisplayControl(CWnd* pParent /*=NULL*/)
: CDlgImageControlSkin(CDlgDisplayControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDisplayControl)
	m_bChk3D = FALSE;
	m_bMaxWin = FALSE;
	m_bSeriesDisplay = FALSE;
	//}}AFX_DATA_INIT
	
	m_pMRIRectSeries = NULL;
	m_pDlgDisplay = NULL;
	//	m_dlgSeriesBrowse=NULL;
	m_iCurDispIndex = -1;
	m_iResolutionX = 0;
	m_iResolutionY = 0;
	m_bAddTo3D=TRUE;
}
CDlgDisplayControl::~CDlgDisplayControl()
{
	//	if(m_dlgSeriesBrowse)
	//		delete m_dlgSeriesBrowse;
	//	m_dlgSeriesBrowse=NULL;
	
}

void CDlgDisplayControl::DoDataExchange(CDataExchange* pDX)
{
	CDlgImageControlSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDisplayControl)
	DDX_Control(pDX, IDC_BTN_CHK_FULL, m_BtnChkShowMax);
	DDX_Control(pDX, IDC_BTN_CHK_3D, m_Btn3D);
	DDX_Control(pDX, IDC_CHK_3D, m_BtnChk3D);
	DDX_Control(pDX, IDC_SHOW_PRE, m_BtnPrev);
	DDX_Control(pDX, IDC_SHOW_NEXT, m_BtnNext);
	DDX_Control(pDX, IDC_SHOW_FIRST, m_BtnFirst);
	DDX_Control(pDX, IDC_SHOW_LAST, m_BtnLast);
	DDX_Control(pDX, IDC_CHK_SHOW_MAX, m_BtnShowMax);
	DDX_Check(pDX, IDC_CHK_3D, m_bChk3D);
	DDX_Check(pDX, IDC_CHK_SHOW_MAX, m_bMaxWin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDisplayControl, CDlgImageControlSkin)
//{{AFX_MSG_MAP(CDlgDisplayControl)
ON_BN_CLICKED(IDC_CHK_3D, OnChk3d)
ON_BN_CLICKED(IDC_CHK_SHOW_MAX, OnChkShowMax)
ON_BN_CLICKED(IDC_SHOW_FIRST, OnShowFirst)
ON_BN_CLICKED(IDC_SHOW_PRE, OnShowPre)
ON_BN_CLICKED(IDC_SHOW_NEXT, OnShowNext)
ON_BN_CLICKED(IDC_SHOW_LAST, OnShowLast)
ON_BN_CLICKED(IDC_BTN_CHK_3D, OnBtnChk3d)
ON_BN_CLICKED(IDC_BTN_CHK_FULL, OnBtnChkFull)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplayControl message handlers
//
void CDlgDisplayControl::SetMRIRectSeries(MRIRectSeries* pMRIRectSeries, int iInitDispImgIndex /*= 0*/)
{
	m_pMRIRectSeries = pMRIRectSeries;
	if (m_pMRIRectSeries)
	{
		m_pDlgDisplay->m_Scene.RemoveAllDistMeasurement();
		
		int iDispIndex = iInitDispImgIndex;
		if (iInitDispImgIndex == -1)
		{
			iDispIndex = 0;
		}
		else if (iInitDispImgIndex == -2)
		{
			iDispIndex = m_iCurDispIndex;
		}
		
		// If init display is larger than images number, display first image
		if (iInitDispImgIndex >= pMRIRectSeries->GetMRISeries()->GetNumOfImages())
		{
			iDispIndex = 0;
		}		
		
		if(m_pDlgDisplay->IsRcvWnd())
		{
			ShowImage(iDispIndex,TRUE);
		}else
		{
			ShowImage(iDispIndex, FALSE);
		}
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
		m_pDlgDisplay->ShowLegend(TRUE);
	}
}

void CDlgDisplayControl::AddToDisplay(int iImageIndex, BOOL bKeepView)
{
	if (m_pMRIRectSeries)
	{
		if (iImageIndex < 0)
		{
			return;
			//m_iCurDispIndex = 0;
		}
		else if (iImageIndex >= m_pMRIRectSeries->GetRectNum())
		{
			return;
			//m_iCurDispIndex = m_pMRIRectSeries->GetRectNum() - 1;
		}
		else 
		{
			m_iCurDispIndex = iImageIndex;
		}

		
		m_pDlgDisplay->m_Scene.SetRectIndex(m_iCurDispIndex);
		m_pDlgDisplay->EnableDraw(FALSE); 
		m_pMRIRectSeries->SetDispIndex(m_iCurDispIndex);

		CMRIRectangle *pCurMRIRect = m_pMRIRectSeries->GetDispMRIRect();
				
		// Only keep rectangle when rectangle has same resolution with new images. Huagen Liu  2007/08/28
		if (pCurMRIRect->m_bIsVisible 
			&& pCurMRIRect->GetMRIImg()->GetResolutionX() == m_iResolutionX
			&& pCurMRIRect->GetMRIImg()->GetResolutionY() == m_iResolutionY)
		{
			m_pDlgDisplay->RemoveAllProjectVTarget(); // Have to be before ADDMIRectAPSI, since ADDMRIRectAPSI will remove text
			m_pDlgDisplay->ADDMRIRectAPSI(pCurMRIRect);
			m_pDlgDisplay->AddMRIRectLegend(pCurMRIRect->GetMRIImg());
			m_pDlgDisplay->UpdateAllProjectVTarget(FALSE);
			if (!bKeepView)
			{
				//m_pDlgDisplay->m_Scene.ChangeViewToRect(pCurMRIRect->m_pRectangle);
				m_pDlgDisplay->m_Scene.ChangeViewToMRIRect(pCurMRIRect,TRUE);
			}else
			{
				m_pDlgDisplay->m_Scene.ChangeViewToMRIRect(pCurMRIRect,FALSE);
			}
			
		}
		else
		{
			if (pCurMRIRect->m_bIsVisible)  // In this case, rectangle is visible but has different resolution
			{
				m_pDlgDisplay->RemoveMRIRect(pCurMRIRect);
			}
			
			m_pDlgDisplay->AddMRIRectWithLabel(pCurMRIRect);
			m_pDlgDisplay->UpdateAllProjectVTarget();
		}
						
		m_pDlgDisplay->EnableDraw(TRUE);
		m_pDlgDisplay->m_Scene.DrawScene();
		 
		m_pDlgDisplay->ResetRuler();
		
		m_iResolutionX = pCurMRIRect->GetMRIImg()->GetResolutionX();
		m_iResolutionY = pCurMRIRect->GetMRIImg()->GetResolutionY();
		
		//m_ctlGBCtrl.SetWindowText(pCurMRIRect->GetMRIImg()->GetImageName());
		CMRIRectangle *pMRIRect = m_pMRIRectSeries->GetMRIRectAt(m_iCurDispIndex);
		//		pMRIRect->SetVisible3D(TRUE);
		if (pMRIRect)
		{
			//Commented by Wang fengliang 070809;
			//			m_bChk3D = pMRIRect->IsVisible3D(); //Modified by Dai Liang 2007.7.2
			m_bChk3D=TRUE;
			Update3DChkBtn(TRUE);
			//			m_BtnChk3D.SetIcon(IDI_LEDON);
		}
		UpdateData(FALSE);
	}
}

void CDlgDisplayControl::OnChk3d() 
{
	// TODO: Add your control notification handler code here
	m_bChk3D = !m_bChk3D;
	if (m_bChk3D)
	{
		AddCurrentTo3D();
		//		m_BtnChk3D.SetIcon(IDI_LEDON);
	}
	else
	{
		HideCurrentIn3D();
		//		m_BtnChk3D.SetIcon(IDI_LEDOFF);
	}
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
}

void CDlgDisplayControl::AddCurrentTo3D()
{
	CMRIRectangle *pMRIRect = m_pMRIRectSeries->GetMRIRectAt(m_iCurDispIndex);
	if (pMRIRect && !pMRIRect->m_bIn3DWindows)
	{
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->AddTo3DDisplay(pMRIRect);
	}
	else
	{
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->Show3DDisplay(pMRIRect, TRUE);
	}
	
}

void CDlgDisplayControl::HideCurrentIn3D()
{
	CMRIRectangle *pMRIRect = m_pMRIRectSeries->GetMRIRectAt(m_iCurDispIndex);
	if (pMRIRect && pMRIRect->m_bIn3DWindows)
	{
		HideImageIn3D(m_iCurDispIndex);
		m_bChk3D = FALSE;
		UpdateData(FALSE);
		Update3DChkBtn(FALSE);
	}
}

void CDlgDisplayControl::HideImageIn3D(int iImageIndex)
{
	CMRIRectangle *pMRIRect = m_pMRIRectSeries->GetMRIRectAt(iImageIndex);
	if (pMRIRect && pMRIRect->m_bIn3DWindows)
	{
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->RemoveFrom3DDisplay(pMRIRect);
	}
}

void CDlgDisplayControl::OnChkShowMax() //最大化显示窗口
{
	//	UpdateData(TRUE);
	m_bMaxWin = !m_bMaxWin;
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->MaxDispWindow(m_pDlgDisplay, m_bMaxWin?true:false);
}

void CDlgDisplayControl::ShowImage(int iPos, BOOL bKeepView /*= TRUE*/)
{
	if(m_pMRIRectSeries)
	{
		int iOldImageIndex = m_iCurDispIndex;
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.EnableDraw(FALSE);
		HideImageIn3D(iOldImageIndex);	
		
		AddToDisplay(iPos, bKeepView);
		UpdateUI();
		
		// Add to 3D display add by Huagen 09.28
		if (!m_pDisplayWrap->m_bComparison && m_bAddTo3D)
		{			
			AddCurrentTo3D();
			
		}else if(!m_pDisplayWrap->m_bComparison && (iPos == m_pMRIRectSeries->m_iRectNum-1 || iPos==0))
		{
			AddCurrentTo3D();
		}
		
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.EnableDraw(TRUE);
	}
}

void CDlgDisplayControl::UpdateUI()
{
	if (m_pMRIRectSeries)
	{
		CString sOut;
		sOut.Format("%d", 1);  
		GetDlgItem(IDC_LAL_MIN)->SetWindowText(sOut); 
		sOut.Format("%d", m_pMRIRectSeries->GetRectNum());
		GetDlgItem(IDC_LAL_MAX)->SetWindowText(sOut); 
		//CString strNode;
		//strNode.Format("%s %d/%d",m_pMRIRectSeries->GetMRISeries()->GetName(), m_iCurDispIndex+1,m_pMRIRectSeries->GetRectNum());
		//m_ctlGBCtrl.SetWindowText(strNode);
		
		if(m_iCurDispIndex==0)
		{
			GetDlgItem(IDC_SHOW_PRE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SHOW_FIRST)->EnableWindow(FALSE);
			GetDlgItem(IDC_SHOW_LAST)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOW_NEXT)->EnableWindow(TRUE);
		}
		else if(m_iCurDispIndex==m_pMRIRectSeries->GetRectNum()-1)
		{
			GetDlgItem(IDC_SHOW_PRE)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOW_FIRST)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOW_LAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_SHOW_NEXT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_SHOW_FIRST)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOW_LAST)->EnableWindow(TRUE);			
			GetDlgItem(IDC_SHOW_PRE)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOW_NEXT)->EnableWindow(TRUE);			
		}
	}
	else
	{
		GetDlgItem(IDC_LAL_MIN)->SetWindowText(""); 
		GetDlgItem(IDC_LAL_MAX)->SetWindowText(""); 
		//m_ctlGBCtrl.SetWindowText("");
		
		GetDlgItem(IDC_SHOW_FIRST)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOW_LAST)->EnableWindow(FALSE);			
		GetDlgItem(IDC_SHOW_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOW_NEXT)->EnableWindow(FALSE);	
		
		m_bChk3D = FALSE;
		Update3DChkBtn(FALSE);
	}
	UpdateData(FALSE);
}

void CDlgDisplayControl::OnShowFirst() 
{
	
	if (m_pMRIRectSeries)
	{
		int iPos=0;   //第一个
		if (m_iCurDispIndex == iPos) return;
		
		// Remove current 3D display add by Huagen 09.28
		ShowImage(iPos);
		m_bChk3D = TRUE;
		Update3DChkBtn(TRUE);
		if (m_pDisplayWrap->m_bComparison)
		{
			m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
		}
		
		
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	}	
}

void CDlgDisplayControl::OnShowPre() 
{
	m_bAddTo3D=TRUE;
	BackwardImage();
	if (m_pDisplayWrap->m_bComparison)
	{
		m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
	}
}

void CDlgDisplayControl::OnShowNext() 
{
	m_bAddTo3D=TRUE;
	ForwardImage();
	if (m_pDisplayWrap->m_bComparison)
	{
		m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
	}
}

void CDlgDisplayControl::ForwardImage()
{
	if (m_pMRIRectSeries)
	{
		int iPos=m_iCurDispIndex+1;   //下一个// Modified by Wei Wei [2007/6/13]
		if (iPos>m_pMRIRectSeries->m_iRectNum-1)  
			return;		
		
		ShowImage(iPos);
		
		m_bChk3D = TRUE;
		UpdateData(FALSE);
		Update3DChkBtn(TRUE);
		
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.PostMessage(WM_PAINT);
		//m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		//m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	}
}

void CDlgDisplayControl::BackwardImage()
{
	if (m_pMRIRectSeries)
	{
		int iPos=m_iCurDispIndex-1;   //前一个//Modified by Wei Wei [2007/6/13]
		if (iPos<0) return;
		
		ShowImage(iPos);
		
		m_bChk3D = TRUE;
		UpdateData(FALSE);
		Update3DChkBtn(TRUE);
		
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.PostMessage(WM_PAINT);
		//m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		//m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	}
}

void CDlgDisplayControl::OnShowLast() 
{
	
	if (m_pMRIRectSeries)
	{
		int iPos=m_pMRIRectSeries->m_iRectNum-1;   //最后一个
		if (iPos==m_iCurDispIndex)  return;
		
		ShowImage(iPos);
		
		m_bChk3D = TRUE;
		UpdateData(FALSE);
		Update3DChkBtn(TRUE);
		if (m_pDisplayWrap->m_bComparison)
		{
			m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
		}
		
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
	}	
}



BOOL CDlgDisplayControl::OnInitDialog() 
{
	SetFirstBtn(&m_BtnFirst);
	SetLastBtn(&m_BtnLast);
	SetPreBtn(&m_BtnPrev);
	SetNextBtn(&m_BtnNext);
	SetThreeDBtn(&m_Btn3D);
	SetMaxBtn(&m_BtnChkShowMax);
	
	CDlgImageControlSkin::OnInitDialog();
	
	m_Btn3D.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_BtnChkShowMax.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	
	m_BtnFirst.SetWindowText("");
	m_BtnLast.SetWindowText("");
	m_BtnPrev.SetWindowText("");
	m_BtnNext.SetWindowText("");
	m_Btn3D.SetWindowText("");
	m_BtnChkShowMax.SetWindowText("");
	
	
	if (m_pDisplayWrap->m_bComparison)
	{
		GetDlgItem(IDC_CHK_3D)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHK_SHOW_MAX)->ShowWindow(FALSE);
		
		m_BtnChkShowMax.ShowWindow(FALSE);
		m_Btn3D.ShowWindow(FALSE);
	}
	
	GetDlgItem(IDC_LAL_MAX)->ShowWindow(FALSE);
	GetDlgItem(IDC_LAL_MIN)->ShowWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgDisplayControl::PreTranslateMessage(MSG* pMsg) 
{
	//Added by Wang fengliang 070815;
	//避免 按[回车]和[Esc] Dialog 消失
	if((pMsg->message==WM_KEYDOWN))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
		{
			return TRUE;   
		}
		else
		{
			OnKeyDown(pMsg->wParam);
		}
    }else if((pMsg->message==WM_KEYUP))   
    {   
		if( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE )
		{
			return TRUE;   
		}
		else
		{
			OnKeyUp(pMsg->wParam);
		}
    }
	return CDlgImageControlSkin::PreTranslateMessage(pMsg);
}



void CDlgDisplayControl::RemoveCurrentDispImg()
{
	if (m_pMRIRectSeries)
	{
		if (m_pMRIRectSeries->GetDispMRIRect())
		{
			m_pDlgDisplay->RemoveAllText();
			m_pDlgDisplay->RemoveMRIRect(m_pMRIRectSeries->GetDispMRIRect());
			m_pMRIRectSeries->GetDispMRIRect()->m_bIsVisible = FALSE;
			m_iCurDispIndex = -1;
			m_pMRIRectSeries = NULL;
			UpdateUI();
		}
	}
}


void CDlgDisplayControl::OnBtnChk3d() 
{
	// TODO: Add your control notification handler code here
	Update3DChkBtn(!m_bChk3D);
	if (m_bChk3D)
	{
		AddCurrentTo3D();
	}
	else
	{
		HideCurrentIn3D();
	}
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.UpdateWindow();
}

void CDlgDisplayControl::OnBtnChkFull() 
{
	// TODO: Add your control notification handler code here
	m_bMaxWin = !m_bMaxWin;
	m_BtnChkShowMax.SetChecked(m_bMaxWin);
	m_pDlgDisplay->m_pCryotherapyDlg->m_pDisplay4Dlg->MaxDispWindow(m_pDlgDisplay, m_bMaxWin?true:false);
	
}

void CDlgDisplayControl::Update3DChkBtn(BOOL bCheck)
{
	m_bChk3D = bCheck;
	m_Btn3D.SetChecked(bCheck);
}

void CDlgDisplayControl::OnKeyUp(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
	//使用上下左右箭头切换显示序列
	m_bAddTo3D=TRUE;
	if(!m_pDisplayWrap->m_bComparison && (nChar==37 || nChar==38 || nChar == 90))
	{
		
		AddCurrentTo3D();
		//OnShowPre();
	}
	
	if(!m_pDisplayWrap->m_bComparison && (nChar==39 || nChar==40 || nChar == 88))
	{
		
		AddCurrentTo3D();
		//OnShowNext();
	}
	
}

void CDlgDisplayControl::OnKeyDown(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
	//使用上下左右箭头切换显示序列
	m_bAddTo3D=FALSE;
	if(nChar==37 || nChar==38 || nChar == 90 || nChar == 33)
	{
		
		BackwardImage();
		if (m_pDisplayWrap->m_bComparison)
		{
			m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
		}
	}
	
	if(nChar==39 || nChar==40 || nChar == 88 || nChar == 34)
	{
		
		ForwardImage();
		if (m_pDisplayWrap->m_bComparison)
		{
			m_pDlgDisplay->m_pDlgDispCompare->ShowBindedMRIs(m_iCurDispIndex - m_pDisplayWrap->m_iLinkIndex, m_pDisplayWrap);
		}
	}
	
}
