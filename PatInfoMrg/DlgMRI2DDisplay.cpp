// DlgMRI2DDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMRI2DDisplay.h"
#include "PatientInfoMrgDlg.h"
#include "MV.h"
#include "MRISeries.h"
#include "ImageAnalysis.h"
#include "MriCalibration.h"
#include "ErrorCodes.h"
#include <math.h>


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
	m_pMriImage = NULL;
	m_pGenerateBmpOfImage = NULL;
	m_pDisplayBmpOfImage = NULL;

	for (int i=0; i<4; i++)
	{
		m_sMRIOriChar[i] = CString("");
	}

	m_bShowCaliResult = FALSE;

	memset((void*)&m_CaliResult, 0, sizeof(m_CaliResult));

	m_iTextLegendNum[0]=0;
	m_iTextLegendNum[1]=0;
	m_iTextLegendNum[2]=0;
	m_iTextLegendNum[3]=0;
	
	m_pTabPatientInfoPage=NULL;
	m_bBeyondImage = FALSE;
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
//	DDX_Check(pDX, IDC_CHECK_POLYLINE, m_bPolyline);
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

		m_bBeyondImage = FALSE;
	}
	else
	{
		m_pMriImage = NULL;
		m_bBeyondImage = TRUE;
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

/*	CString* psDispOri = m_pMriImage->GetDispOrientChar();
	if (psDispOri)
	{
		for (int i=0; i<4; i++)
		{
			m_sMRIOriChar[i] = psDispOri[i];
		}
	}

	//设置显示字体
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

	//获取图像标签信息
	AddMRIRectLegend(m_pMriImage,m_stTextLegend,&rc,&m_iLegendNum,&cTextFont);//Invalida
*/
	CreateDraw(pBitmap);

	pBitmap->DeleteObject();
//	cTextFont.DeleteObject();

 	Invalidate(FALSE);
	UpdateWindow();
}

/*如果pTextLst和pRect不为空，则表示需要向其中传递标签信息*/
// void CDlgMRI2DDisplay::AddMRIRectLegend(CMRIImage *pImage,ST_TEXT *pTextLst /*=NULL*/,
// 									   CRect *pRect /*=NULL*/,int *pLegendNum /*=NULL*/,
// 									   CFont *pFont /*=NULL*/)
// {
// 	
// 	CRect rect;
// 	GetClientRect(rect);
// 
// 	/*采用外面传递的区域大小*/
// 	if(pRect!=NULL)
// 	{
// 		rect = *pRect;
// 	}
// 
// 	
// 	CDC	 *pDC =AfxGetMainWnd()->GetDC();
// 	CFont *pOldFont=NULL;
// 	CString sOut;
// 	SIZE size;
// 
// 	if(pFont)
// 	{
// 		pOldFont = pDC->SelectObject(pFont);
// 	}
// 	else if(m_stTextLegend[5].pFont)
// 	{
// 		pOldFont=pDC->SelectObject(m_stTextLegend[5].pFont);
// 	}
// 
// 	int Legend_iX = 2;
// 	int Legend_iY = 11;
// 	int Legend_iYGap = 11;
// 	int Legend_iZ =0;
// 
// 	if(pFont)
// 	{
// 		LOGFONT logFont;
// 		pFont->GetLogFont(&logFont);
// 		Legend_iY=abs(logFont.lfHeight);
// 		Legend_iYGap = Legend_iY;
// 	}
// 
// 	m_legendType = pImage->GetImageType();
// 	if(m_legendType != IMS_DCMCT)
// 	{
// 		m_iTextLegendNum[0]=lstMRLeftTop.GetCount();
// 		m_iTextLegendNum[1]=lstMRLeftBottom.GetCount();
// 		m_iTextLegendNum[2]=lstMRRightTop.GetCount();
// 		m_iTextLegendNum[3]=lstMRRightBottom.GetCount();
// 		m_nNumLegendLine=m_iTextLegendNum[0]+m_iTextLegendNum[1]+m_iTextLegendNum[2]+m_iTextLegendNum[3];
// 		int iPos=0;
// 
// 		POSITION pos=lstMRLeftTop.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstMRLeftTop.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstMRLeftBottom.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstMRLeftBottom.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstMRRightTop.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstMRRightTop.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstMRRightBottom.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstMRRightBottom.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		iPos=0;
// 		for(int i=0;i<m_iTextLegendNum[0];i++)
// 		{
// 			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, Legend_iY+i*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[1];i++)
// 		{
// 			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, rect.Height()-(m_iTextLegendNum[1]-i)*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[2];i++)
// 		{
// 			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
// 			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, Legend_iY+i*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[3];i++)
// 		{
// 			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
// 			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, rect.Height()-(m_iTextLegendNum[3]-i)*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 	
// 	
// 	}else
// 	{
// 		m_iTextLegendNum[0]=lstCTLeftTop.GetCount();
// 		m_iTextLegendNum[1]=lstCTLeftBottom.GetCount();
// 		m_iTextLegendNum[2]=lstCTRightTop.GetCount();
// 		m_iTextLegendNum[3]=lstCTRightBottom.GetCount();
// 		m_nNumLegendLine=m_iTextLegendNum[0]+m_iTextLegendNum[1]+m_iTextLegendNum[2]+m_iTextLegendNum[3];
// 		int iPos=0;
// 
// 		POSITION pos=lstCTLeftTop.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstCTLeftTop.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstCTLeftBottom.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstCTLeftBottom.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstCTRightTop.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstCTRightTop.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		pos=lstCTRightBottom.GetHeadPosition();
// 		while(pos)
// 		{
// 			int i=lstCTRightBottom.GetNext(pos);
// 			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
// 			iPos++;
// 		}
// 
// 		iPos=0;
// 		for(int i=0;i<m_iTextLegendNum[0];i++)
// 		{
// 			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, Legend_iY+i*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[1];i++)
// 		{
// 			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, rect.Height()-(m_iTextLegendNum[1]-i)*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[2];i++)
// 		{
// 			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
// 			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, Legend_iY+i*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 		for(i=0;i<m_iTextLegendNum[3];i++)
// 		{
// 			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
// 			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, rect.Height()-(m_iTextLegendNum[3]-i)*Legend_iYGap, Legend_iZ);
// 			iPos++;
// 		}
// 	}
// 
// 	if(pOldFont)
// 	{
// 		pDC->SelectObject(pOldFont);
// 	}
// 	AfxGetMainWnd()->ReleaseDC(pDC);
// 
// 	for (int i=0; i<m_nNumLegendLine; i++)
// 	{
// 		if(!pTextLst)
// 		{
// 			m_stTextLegend[i].bShow = m_bShowLegend; //TRUE;
// 		}
// 		if(pTextLst)
// 		{
// 			pTextLst[i].sText = m_stTextLegend[i].sText;
// 			pTextLst[i].vPosition = m_stTextLegend[i].vPosition;
// 		}
// 
// 	}
// 	for(i=m_nNumLegendLine; i< PANA_2D_LEGEND_NUMBER;i++)
// 	{
// 		m_stTextLegend[i].bShow = FALSE;
// 	}
// 
// 	if(pLegendNum)
// 	{
// 		*pLegendNum = m_nNumLegendLine;
// 	}
// }

CString CDlgMRI2DDisplay::GetLegend(CMRIImage *pImage,ENUM_IMG_SOURCE legendType, int index)
{
	CString sOut="";
	CString sVersion;
	CString sTmp;
	CString sPA,sRL,sSI;
	char *pBoxChar;
	if(legendType != IMS_DCMCT)
	{
		
		MR_ImageHeader* pImageHeader = pImage->GetImageHeader();
		switch(index)
		{
		case 1:
			sOut.Format("%s/%d", pImage->GetImageName(), pImage->m_pSeries->GetNumOfImages());
			break;
		case 2:
			sOut.Format("%s", pImageHeader->seriesField.Desc);
			break;
		case 3:
			sOut.Format("Matrix:%dX%d", pImageHeader->seriesField.ReadPoints, pImageHeader->seriesField.PhasePoints);
			break;
		case 4:
			sOut.Format("Slice:%.1f/%.1f ACQ:%2d'%2d''", pImageHeader->seriesField.SliceThickness, pImageHeader->seriesField.SliceGap,pImageHeader->scanField.ACQ/60,pImageHeader->scanField.ACQ%60);
			break;
		case 5:
			sOut.Format("TR/TE: %d/%d", (int)(0.001f*(float)pImageHeader->seriesField.RepTime), (int)(0.001f*(float)pImageHeader->seriesField.EchoTime));
			break;
		case 6:
			sOut.Format("%s", pImageHeader->siteField.BISerialNum);
			break;
		case 7:
			sOut.Format("ID:%s", pImageHeader->patientField.Id);
			break;
		case 8:
			sOut.Format("%s", pImageHeader->patientField.Name);
			break;
		case 9:
			sOut.Format("%d.%s", pImageHeader->patientField.Age,
				pImageHeader->patientField.Sex == 'M' ? "M" : (pImageHeader->patientField.Sex == 'F' ? "F" : "U"));
			break;
		case 10:
			sOut.Format("%d.%d.%d %d:%d", pImageHeader->seriesField.DateTime.year, 
				pImageHeader->seriesField.DateTime.month,
				pImageHeader->seriesField.DateTime.day,
				pImageHeader->seriesField.DateTime.hour,
				pImageHeader->seriesField.DateTime.minute);
			break;
		case 11:
			sOut.Format("%s", pImageHeader->siteField.InstName);
			break;
		case 12:
			sOut="";
						
			switch(pImageHeader->seriesField.Plane)
			{
			case 0:
				sOut="Axi.";
				break;
			case 1:
				sOut="Cor.";
				break;
			case 2:
				sOut="Sag.";
				break;
			case 3:
				sOut="Obl.";
				break;
			}
			pBoxChar = pImage->m_pSeries->GetNewBoxOrientChar();
			if(pImageHeader->imageField.fCenterPointX>0)
			{
				sTmp.Format("%c%.1f ",pBoxChar[0],pImageHeader->imageField.fCenterPointX);
			}else
			{
				sTmp.Format("%c%.1f ",pBoxChar[1],-pImageHeader->imageField.fCenterPointX);
			}
			if(pBoxChar[0] == 'P' || pBoxChar[0] == 'A')
			{
				sPA= sTmp;
			}
			if(pBoxChar[0] == 'R' || pBoxChar[0] == 'L')
			{
				sRL= sTmp;
			}
			if(pBoxChar[0] == 'S' || pBoxChar[0] == 'I')
			{
				sSI= sTmp;
			}
			
			if(pImageHeader->imageField.fCenterPointY>0)
			{
				sTmp.Format("%c%.1f ",pBoxChar[2],pImageHeader->imageField.fCenterPointY);
			}else
			{
				sTmp.Format("%c%.1f ",pBoxChar[3],-pImageHeader->imageField.fCenterPointY);
			}
			if(pBoxChar[2] == 'P' || pBoxChar[2] == 'A')
			{
				sPA= sTmp;
			}
			if(pBoxChar[2] == 'R' || pBoxChar[2] == 'L')
			{
				sRL= sTmp;
			}
			if(pBoxChar[2] == 'S' || pBoxChar[2] == 'I')
			{
				sSI= sTmp;
			}
			if(pImageHeader->imageField.fCenterPointZ>0)
			{
				sTmp.Format("%c%.1f ",pBoxChar[4],pImageHeader->imageField.fCenterPointZ);
			}else
			{
				sTmp.Format("%c%.1f ",pBoxChar[5],-pImageHeader->imageField.fCenterPointZ);
			}
			if(pBoxChar[4] == 'P' || pBoxChar[4] == 'A')
			{
				sPA= sTmp;
			}
			if(pBoxChar[4] == 'R' || pBoxChar[4] == 'L')
			{
				sRL= sTmp;
			}
			if(pBoxChar[4] == 'S' || pBoxChar[4] == 'I')
			{
				sSI= sTmp;
			}
			sOut = sOut + sPA + sRL + sSI;
			break;
		case 13:
			sOut.Format("FOV:%ix%i", pImageHeader->seriesField.FovX,pImageHeader->seriesField.FovY);
			break;
		case 14:
			sOut.Format("NEX:%i", pImageHeader->seriesField.NumAvg);
			break;
		case 15:
			sOut.Format("FA:%.1f", pImageHeader->seriesField.FlipAngle);
			break;
		case 16:
			sOut.Format("ETL:%i", pImageHeader->seriesField.EchoTrainLength);
			break;
		case 17:
			sOut.Format("BandWidth:%.0f", pImageHeader->seriesField.AcqBandwidth);
			break;
		case 18:
			sOut.Format("W=%i,L=%i", pImage->GetWindowWidth(),pImage->GetWindowLevel());
			break;
		case 19:
//			sVersion = CGlobalFuncs::GetProductVersion(NULL);
//			sOut.Format("Ver %s", sVersion);
			break;
		case 20:
			ENUM_CODING_DIR_ON_2D_IMAGE bLeftRight = pImage->GetFreqFlag();
			switch(bLeftRight)
			{
			case  fdLeftToRight:		// →
				sOut= "→";
				break;
			case  fdTopToBottom:		// ↓
				sOut= "↓";
				break;
			case  fdRightToLeft:		// ←
				sOut= "←";
				break;
			case  fdBottomToTop:		// ↑
				sOut= "↑";
				break;
				
			}
			break;
		}
	}else
	{
		CT_ImageHeader* pImageHeader = (CT_ImageHeader*)pImage->GetImageHeader();
		switch(index)
		{
		case 1:
			sOut.Format("%s", pImageHeader->siteField.BISerialNum);
			break;
		case 2:
			sOut.Format("Ex:%s", pImageHeader->studyField.Id);
			break;
		case 3:
			sOut.Format("Se:%i", pImageHeader->seriesField.Id);
			break;
		case 4:
			sOut.Format("Im:%i/%i", pImageHeader->imageField.Id,pImage->m_pSeries->GetNumOfImages());
			break;
		case 5:
			sOut.Format("DFOV %i.0 mm", pImageHeader->seriesField.DataDiameter);
			break;
		case 6:
			sOut.Format("kV:%i", pImageHeader->seriesField.KVP);
			break;
		case 7:
			sOut.Format("mA:%i", pImageHeader->seriesField.TubeCurrent);
			break;
		case 8:
			sOut.Format("%.1f mm", pImageHeader->seriesField.SliceThickness);
			break;
		case 9:
			sOut.Format("Tilt:%i degrees", pImageHeader->seriesField.DetectorTilt);
			break;
		case 10:
			sOut.Format("%i:%i:%i", pImageHeader->imageField.DateTime.hour,pImageHeader->imageField.DateTime.minute,
				pImageHeader->imageField.DateTime.second);
			break;
		case 11:
			sOut.Format("W=%i,L=%i",pImage->GetWindowWidth(),pImage->GetWindowLevel());
			break;
		case 12:
			sOut.Format("%s", pImageHeader->siteField.InstName);
			break;
		case 13:
			sOut.Format("%s", pImageHeader->patientField.Name);
			break;
		case 14:
			sOut.Format("%i/%i/%i", pImageHeader->studyField.Time.month,pImageHeader->studyField.Time.day,
				pImageHeader->studyField.Time.year);
			break;
		case 15:
			sOut.Format("%i", pImageHeader->seriesField.ResolutionX);
			break;
		case 16:
//			sVersion = CGlobalFuncs::GetProductVersion(NULL);
//			sOut.Format("Ver %s", sVersion);
			break;
		case 17:
			{
				ENUM_CODING_DIR_ON_2D_IMAGE bLeftRight = pImage->GetFreqFlag();
				switch(bLeftRight)
				{
				case  fdLeftToRight:		// →
					sOut= "→";
					break;
				case  fdTopToBottom:		// ↓
					sOut= "↓";
					break;
				case  fdRightToLeft:		// ←
					sOut= "←";
					break;
				case  fdBottomToTop:		// ↑
					sOut= "↑";
					break;
				
				}
				break;
			}
		case 18:
			sOut.Format("x=%.2f,y=%.2f,z=%.2f", pImageHeader->imageField.fCenterPointX,
				pImageHeader->imageField.fCenterPointY,pImageHeader->imageField.fCenterPointZ);
			break;

		}
	}
	return sOut;
}

BOOL CDlgMRI2DDisplay::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CDlgMRI2DDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_pMriImage && !m_bBeyondImage)
		m_skinDC.DrawBack(&dc);
	else
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(0,0,0));    //设置为黑色背景
		CDialog::OnPaint();
	}

	


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

//	if (m_pDlgBrowse && !m_bPolyline)
//	{
// 		m_pDlgBrowse->UpdateImage3Ds(m_iImgIndex);
// 		m_pDlgBrowse->ShowWindow(FALSE);
//	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CDlgMRI2DDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

//	if (m_pDlgBrowse && !m_bPolyline)
//	{
//		m_pDlgBrowse->UpdateImageBMPs();
//	}

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMRI2DDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (!m_bPolyline && m_pMriImage && !m_bBeyondImage && (nFlags & MK_RBUTTON || nFlags & MK_LBUTTON))
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

// 	if(m_pCryDlg)
// 	{
// 		GetDlgItem(IDC_CHECK_POLYLINE)->MoveWindow(5,cy-80,60,20);
// 		GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(5,cy-55,60,20);
// 		GetDlgItem(IDC_BUTTON_CUT)->MoveWindow(5,cy-30,60,20);
// 		if(m_pDlgBrowse)
// 		{
// 			if(!m_pDlgBrowse->m_bDisplayPolylineTool)
// 			{
// 				GetDlgItem(IDC_CHECK_POLYLINE)->ShowWindow(FALSE);
// 				GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(FALSE);
// 				GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(FALSE);
// 			}
// 		}
// 	}

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
// 				m_skinDC.TextOut(m_stTextLegend[i].vPosition.x,
// 					m_stTextLegend[i].vPosition.y,m_stTextLegend[i].sText);
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
//	if (m_pDlgBrowse)
//	{
//		m_pDlgBrowse->OnKeyDown(nChar);
//	}
	
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

	if (GetParent() && !m_bPolyline)
	{
		CPatientInfoMrgDlg* pInfoPage = (CPatientInfoMrgDlg*)GetParent();
		pInfoPage->UpdateSeries();
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
