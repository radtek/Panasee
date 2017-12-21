// DlgMRI2DDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "DlgMRI2DDisplay.h"
#include "MV.h"
#include "CryotherapyDlg.h"
#include "MRISeries.h"
#include "ImageAnalysis.h"
#include "MriCalibration.h"
#include "ErrorCodes.h"
#include "DlgDisplayWrap.h"
#include "Dispaly4Dlg.h"
#include "DisplayDlg.h"
#include "DlgMRIBrowse.h"
#include <math.h>
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMRI2DDisplay dialog


CDlgMRI2DDisplay::CDlgMRI2DDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMRI2DDisplay::IDD, pParent)
{
//	m_pGenerateBmpOfImage = new CGenerateBmpOfImage();
//	m_pDisplayBmpOfImage = new CDisplayBmpOfImage();
	
	m_pMriImage = NULL;
	m_pCryDlg=NULL;

	m_pDlgBrowse = NULL;
	m_pGenerateBmpOfImage = NULL;
	m_pDisplayBmpOfImage = NULL;

	for (int i=0; i<4; i++)
	{
		m_sMRIOriChar[i] = CString("");
	}

	m_bShowCaliResult = FALSE;

	memset((void*)&m_CaliResult, 0, sizeof(m_CaliResult));

	//{{AFX_DATA_INIT(CDlgMRI2DDisplay)
	m_bPolyline = FALSE;
	//}}AFX_DATA_INIT
}

CDlgMRI2DDisplay::~CDlgMRI2DDisplay()
{
	if (m_pDisplayBmpOfImage)
	{
		delete m_pDisplayBmpOfImage;
	}
	if (m_pGenerateBmpOfImage)
	{
		delete m_pGenerateBmpOfImage;
	}
	
}

void CDlgMRI2DDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMRI2DDisplay)
	DDX_Check(pDX, IDC_CHECK_POLYLINE, m_bPolyline);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMRI2DDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgMRI2DDisplay)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_POLYLINE, OnCheckPolyline)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CUT, OnButtonCut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMRI2DDisplay message handlers

void CDlgMRI2DDisplay::LoadMRIImage(CMRISeries* pMRISeries, int iImgIndex,BOOL bUpdate /*=TRUE*/)
{
	if (iImgIndex >= 0 && iImgIndex < pMRISeries->GetNumOfImages())
	{
		m_iImgIndex = iImgIndex;
		m_pMriImage = pMRISeries->GetImageAt(m_iImgIndex);
		if(bUpdate)
		{
			LoadMRIImage(m_pMriImage);
		}
	}
}

void CDlgMRI2DDisplay::LoadMRIImage()
{
	LoadMRIImage(m_pMriImage);
}

void CDlgMRI2DDisplay::LoadMRIImage(CMRIImage* pMriImage)
{
	m_pMriImage = pMriImage;

	if (pMriImage==NULL)
		return;
	
	if ( NULL != m_pDisplayBmpOfImage )
	{
		delete m_pDisplayBmpOfImage;
		m_pDisplayBmpOfImage = NULL;
	}
	if ( NULL != m_pGenerateBmpOfImage )
	{
		delete m_pGenerateBmpOfImage;
		m_pGenerateBmpOfImage = NULL;
	}

	CClientDC dcClient(this);
	m_pGenerateBmpOfImage = new CGenerateBmpOfImage();
	m_pDisplayBmpOfImage = new CDisplayBmpOfImage(); 

	// Modified to display image with window width and level information     Huagen Liu 2007/06/14
	CBitmap* pBitmap;
	pBitmap = m_pGenerateBmpOfImage->GetBitmapByDataOfImage( 
		m_pMriImage->GetRotatedResolutionX(), m_pMriImage->GetRotatedResolutionY(), 
		m_pMriImage->GetRotatedData(), m_pMriImage->GetWindowWidth(), 
		m_pMriImage->GetWindowLevel(), &dcClient);

	CString* psDispOri = m_pMriImage->GetDispOrientChar();
	if (psDispOri)
	{
		for (int i=0; i<4; i++)
		{
			m_sMRIOriChar[i] = psDispOri[i];
		}
	}

	/*设置显示字体*/
	CRect rc;
	GetClientRect(&rc);

	LOGFONT logFont;
	CFont cTextFont;
	this->GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = logFont.lfHeight * ((int)rc.Width()/340.0);
	if(logFont.lfHeight<-11)
	{
		logFont.lfHeight=-11;
	}
	cTextFont.CreateFontIndirect(&logFont);

	/*获取图像标签信息*/
	this->m_pCryDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.
		AddMRIRectLegend(m_pMriImage,m_stTextLegend,&rc,&m_iLegendNum,&cTextFont);

	CreateDraw(pBitmap);

	pBitmap->DeleteObject();
	cTextFont.DeleteObject();

	Invalidate(FALSE);
	UpdateWindow();
}


BOOL CDlgMRI2DDisplay::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CDlgMRI2DDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	m_skinDC.DrawBack(&dc);

	if(m_bPolyline)
	{
		CRect rc;
		GetClientRect(&rc);
		POINT pnt;
		POSITION pos=m_lpPolyline.GetHeadPosition();
		CPen newPen(PS_SOLID,1,RGB(255,0,0));
		CPen *pOldPen=dc.SelectObject(&newPen);
		BOOL bFirst=TRUE;

		while(pos)
		{
			pnt=m_lpPolyline.GetNext(pos);

			pnt.x=rc.Width()*pnt.x/m_pMriImage->GetRotatedResolutionX();
			pnt.y=rc.Height()*pnt.y/m_pMriImage->GetRotatedResolutionY();
			if(bFirst)
			{
				dc.MoveTo(pnt.x,pnt.y);
				dc.LineTo(pnt.x,pnt.y);
				bFirst=FALSE;
			}else
			{
				dc.LineTo(pnt.x,pnt.y);
			}
		}

		dc.SelectObject(pOldPen);
		
	}
}

BOOL CDlgMRI2DDisplay::GetClosestCircleCenter(CPoint point, int iCenterNum, double *pdbCenterXY, CPoint &centerPoint)
{
	if (iCenterNum>0)
	{
		CPoint myPoint;
		double dbMinDist = 99999;
		double dbDist;
		for (int i=0; i<iCenterNum; i++)
		{
			dbDist = sqrt( (point.x-pdbCenterXY[i*2]) * (point.x-pdbCenterXY[i*2]) 
				+ (point.y-pdbCenterXY[i*2+1]) * (point.y-pdbCenterXY[i*2+1]));
			if (dbDist < dbMinDist)
			{
				dbMinDist = dbDist;
				myPoint.x = pdbCenterXY[i*2];
				myPoint.y = pdbCenterXY[i*2+1];
			}
		}
		centerPoint.x = myPoint.x;
		centerPoint.y = myPoint.y;
		return TRUE;
	}
	else
		return FALSE;
}




void CDlgMRI2DDisplay::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDlgBrowse && !m_bPolyline)
	{
		m_pDlgBrowse->UpdateImage3Ds(m_iImgIndex);
		m_pDlgBrowse->ShowWindow(FALSE);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CDlgMRI2DDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pCryDlg = &((g_DlgMain)->m_dlgCryotherapy);

	
	m_skinDC.EnableDrawBack(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMRI2DDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptWindowLevelStart = point;
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMRI2DDisplay::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDlgBrowse && !m_bPolyline)
	{
		m_pDlgBrowse->UpdateImageBMPs();
	}

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMRI2DDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bPolyline && (nFlags & MK_RBUTTON || nFlags & MK_LBUTTON))
	{
		int nShiftOfWindowWidth, nShiftOfWindowLevel;
		nShiftOfWindowWidth = 5 * (point.y - m_ptWindowLevelStart.y);
		nShiftOfWindowLevel = 5 * (point.x - m_ptWindowLevelStart.x);
		m_pMriImage->m_pSeries->ChangeWindowWidthAndLevel(nShiftOfWindowWidth, nShiftOfWindowLevel);
		LoadMRIImage(m_pMriImage);
		m_ptWindowLevelStart = point;
		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgMRI2DDisplay::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect);
	
	m_ppiMRIOriCharPos[0][0] = rect.Width() / 2 - (m_sMRIOriChar[0].GetLength() - 1) * 10;
	m_ppiMRIOriCharPos[0][1] = 4;

	m_ppiMRIOriCharPos[1][0] = rect.Width() - 15;
	m_ppiMRIOriCharPos[1][1] = rect.Height() / 2 - 4 - (m_sMRIOriChar[1].GetLength() - 1) * 18;

	m_ppiMRIOriCharPos[2][0] = rect.Width() / 2  - (m_sMRIOriChar[2].GetLength() - 1) * 10;
	m_ppiMRIOriCharPos[2][1] = rect.Height() - 20;

	m_ppiMRIOriCharPos[3][0] = 4;
	m_ppiMRIOriCharPos[3][1] = rect.Height() / 2 - 4 - (m_sMRIOriChar[3].GetLength() - 1) * 18;

	if(m_pCryDlg)
	{
		GetDlgItem(IDC_CHECK_POLYLINE)->MoveWindow(5,cy-80,60,20);
		GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(5,cy-55,60,20);
		GetDlgItem(IDC_BUTTON_CUT)->MoveWindow(5,cy-30,60,20);
		if(m_pDlgBrowse)
		{
			if(!m_pDlgBrowse->m_bDisplayPolylineTool)
			{
				GetDlgItem(IDC_CHECK_POLYLINE)->ShowWindow(FALSE);
				GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(FALSE);
				GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(FALSE);
			}
		}
	}

}


void CDlgMRI2DDisplay::SetCaliResult(CList<POINT, POINT>* lstCirclePnts, CALI_ERR_RESULT& CaliResult )
{
	POSITION pos = lstCirclePnts->GetHeadPosition();
	m_lpCaliCircle.RemoveAll();
	for (;pos;)
	{
		POINT myPnt = lstCirclePnts->GetNext(pos);
		m_lpCaliCircle.AddTail(myPnt);
	}
	memcpy(&m_CaliResult, &CaliResult, sizeof(CALI_ERR_RESULT));
	m_bShowCaliResult = TRUE;
	
	LoadMRIImage(m_pMriImage);
}

void CDlgMRI2DDisplay::CreateDraw(CBitmap* pBitmap)
{
	CClientDC dcClient(this);

	
	CRect rc;
	GetClientRect(&rc);

	m_skinDC.LoadDC(&dcClient,rc);

	/*设置显示字体*/
	LOGFONT logFont;
	CFont cTextFont;
	this->GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = logFont.lfHeight * ((int)rc.Width()/340.0);
	if(logFont.lfHeight<-11)
	{
		logFont.lfHeight=-11;
	}
	cTextFont.CreateFontIndirect(&logFont);
	CFont* pOldFont = m_skinDC.SelectObject(&cTextFont);

	double dbRatio;
	if ( NULL != m_pMriImage )
	{
		CRect rect;
		GetClientRect(&rect);
		
		CSize mySize = pBitmap->GetBitmapDimension();
		m_pDisplayBmpOfImage->StretchBitmap(m_pMriImage->GetRotatedFOVX(), m_pMriImage->GetRotatedFOVY(), pBitmap, 
			&m_skinDC,rect, rect.Width(), rect.Height());

		dbRatio = ((double) rect.Width()) / mySize.cx;
		
		COLORREF clrWhite = RGB(255,255,255);
		m_skinDC.SetTextColor(clrWhite);
		SetBkMode(m_skinDC,TRANSPARENT);
		

		m_skinDC.TextOut(m_ppiMRIOriCharPos[0][0], m_ppiMRIOriCharPos[0][1], m_sMRIOriChar[0]);
		for (int i=0; i<m_sMRIOriChar[1].GetLength(); i++)
		{
			m_skinDC.TextOut(m_ppiMRIOriCharPos[1][0], m_ppiMRIOriCharPos[1][1] + i * 18, m_sMRIOriChar[1].Mid(i, 1));
		}
		m_skinDC.TextOut(m_ppiMRIOriCharPos[2][0], m_ppiMRIOriCharPos[2][1], m_sMRIOriChar[2]);
		for (i=0; i<m_sMRIOriChar[3].GetLength(); i++)
		{
			m_skinDC.TextOut(m_ppiMRIOriCharPos[3][0], m_ppiMRIOriCharPos[3][1] + i * 18, m_sMRIOriChar[3].Mid(i, 1));
		}

		if(!m_bShowCaliResult)
		{
			for(i=0;i<m_iLegendNum;i++)
			{
				m_skinDC.TextOut(m_stTextLegend[i].vPosition.x,
					m_stTextLegend[i].vPosition.y,m_stTextLegend[i].sText);
			}
		}
		
	}

	if (m_bShowCaliResult)
	{
		if (m_lpCaliCircle.GetCount() > 0)
		{
			POSITION pos = m_lpCaliCircle.GetHeadPosition();
			for (;pos;)
			{
				POINT myPoint = m_lpCaliCircle.GetNext(pos);
				m_skinDC.FillSolidRect(myPoint.x * dbRatio, myPoint.y * dbRatio, 1, 1, RGB(0, 255, 0)); 
			}
		}
		
		CPen pen(PS_SOLID,1,RGB(0,255,0));
		CPen *pOldPen=m_skinDC.SelectObject (&pen);
		for (int i=0; i<PHATOM_SPHERE_NUM; i++)
		{
			int iX = (int)(m_CaliResult.fCircleCenter[i][0]);
			int iY = (int)(m_CaliResult.fCircleCenter[i][1]);
			m_skinDC.MoveTo((iX-5)  * dbRatio, iY  * dbRatio);
			m_skinDC.LineTo((iX+5)  * dbRatio, iY * dbRatio);
			m_skinDC.MoveTo(iX * dbRatio, (iY-5) * dbRatio);
			m_skinDC.LineTo(iX * dbRatio, (iY+5) * dbRatio);
			
			//Added by hiker 2008.08.19----------------------------------------------------
			int iR = (int)(m_CaliResult.fRadius[i]);
			CBrush *pBrush = CBrush::FromHandle((HBRUSH)(GetStockObject(NULL_BRUSH)));
			CBrush *pOldBrush = m_skinDC.SelectObject(pBrush);
			CPen pen (PS_SOLID, 1, RGB (255,0,0));
			CPen* pOldPen = m_skinDC.SelectObject (&pen);
			m_skinDC.Ellipse((iX+iR) * dbRatio, (iY-iR) * dbRatio, (iX-iR) * dbRatio, (iY+iR) * dbRatio);
			m_skinDC.SelectObject(pOldPen);
			//-----------------------------------------------------------------------------
		}
		m_skinDC.SelectObject(pOldPen);

		COLORREF clrWhite = RGB(255,255,255);
		m_skinDC.SetTextColor(clrWhite);
		SetBkMode(m_skinDC,TRANSPARENT);
		int iLeft = 1;
		int iTop = 1;
		int iFontHeight = 13;
		CString sOut;
		for (i=0; i<PHATOM_SPHERE_NUM; i++)
		{
			sOut.Format("%.1f %.1f %.1f | %.2f %.2f", 
				m_CaliResult.fCircleCenter[i][0], m_CaliResult.fCircleCenter[i][1],
				m_CaliResult.fRadius[i], 
				m_CaliResult.fFittingstd[i], m_CaliResult.fFittingMaxerr[i]);
			m_skinDC.TextOut(iLeft, iTop, sOut);
			iTop += iFontHeight;
		}
		sOut.Format("%d %d", m_pMriImage->GetWindowWidth(), m_pMriImage->GetWindowLevel());		
		m_skinDC.TextOut(iLeft, iTop, sOut);
	}
	
	m_skinDC.SelectObject(pOldFont);
	cTextFont.DeleteObject();

}

void CDlgMRI2DDisplay::OnKeyDown(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDlgBrowse)
	{
		m_pDlgBrowse->OnKeyDown(nChar);
	}
	
}

void CDlgMRI2DDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptWindowLevelStart=point;
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMRI2DDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDlgBrowse && !m_bPolyline)
	{
		m_pDlgBrowse->UpdateImageBMPs();
	}

	if(m_bPolyline)
	{
		CRect rc;
		GetClientRect(&rc);
		POINT pnt;

		pnt.x=m_pMriImage->GetRotatedResolutionX()*point.x/rc.Width();
		pnt.y=m_pMriImage->GetRotatedResolutionY()*point.y/rc.Height();
		m_lpPolyline.AddTail(pnt);
		
		Invalidate(FALSE);
		UpdateWindow();
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CDlgMRI2DDisplay::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMRI2DDisplay::OnCheckPolyline() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_lpPolyline.RemoveAll();

	Invalidate(FALSE);
	UpdateWindow();
}

void CDlgMRI2DDisplay::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_lpPolyline.RemoveAll();

	Invalidate(FALSE);
	UpdateWindow();
}

void CDlgMRI2DDisplay::OnButtonCut() 
{
	// TODO: Add your control notification handler code here
	if(m_lpPolyline.GetCount()<3)
		return;
	if(!m_pMriImage)
		return;

	int iWidth=m_pMriImage->GetRotatedResolutionX();
	int iHeight=m_pMriImage->GetRotatedResolutionY();
	IMG_DATA_TYPE *pData=m_pMriImage->GetRotatedData();
	
	POINT pnt;
	POSITION pos=m_lpPolyline.GetHeadPosition();
	CRgn rgn;
	POINT *pnts=new POINT[m_lpPolyline.GetCount()];
	for(int i=0;i<m_lpPolyline.GetCount();i++)
	{
		pnt=m_lpPolyline.GetAt(m_lpPolyline.FindIndex(i));
		pnts[i].x=pnt.x;
		pnts[i].y=pnt.y;
	}
	rgn.CreatePolygonRgn(pnts,m_lpPolyline.GetCount(),WINDING);
	delete []pnts;

	for(i=0;i<iWidth;i++)
	{
		for(int j=0;j<iHeight;j++)
		{
			pnt.x=i;
			pnt.y=j;
			if(rgn.PtInRegion(pnt))
			{
				pData[j*iWidth+i]=0;
			}
		}
	}

	m_lpPolyline.RemoveAll();
	LoadMRIImage();
}
