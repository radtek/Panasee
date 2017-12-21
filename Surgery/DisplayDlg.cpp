// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Surgery.h"
#include "DisplayDlg.h"
#include "DataManager.h"
#include "Probe.h"
#include "CryotherapyDlg.h"
#include "MRIRectangle.h"
#include "DlgDisplayControl.h"
#include "DlgDisplayWrap.h"
#include "Intersection.h"
#include "ErrorCodes.h"
#include "VirtualTarget.h"
#include "DlgDispCompare.h"
#include "DlgMain.h"
#include "BI_ImageHeader.h"
#include "Dispaly4Dlg.h"
#include "DlgSetupLegend.h"
#include "ConfigMgr.h"
#include "Plane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg dialog


CList<int, int> lstCTLeftTop;
CList<int, int> lstCTLeftBottom;
CList<int, int> lstCTRightTop;
CList<int, int> lstCTRightBottom;
CList<int, int> lstMRLeftTop;
CList<int, int> lstMRLeftBottom;
CList<int, int> lstMRRightTop;
CList<int, int> lstMRRightBottom;

//static int index;
int CDisplayDlg::_globalIndex=0;  //Added by wangfengliang 070515;

CDisplayDlg::CDisplayDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDisplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iTransRotateFlag = 0;
	m_bLRArrow = FALSE;
	m_bEnableMouseMove = FALSE;
	m_pDisplayControl = NULL;
	m_pDispWrap = NULL;
	m_pCurDispMRIRect = NULL;
	m_bUpdate = FALSE;
	m_b3DDisplay = FALSE;
	m_bSelected = FALSE;
	//Added by wang fengliang 070515;
	//--begin
	m_index = ++_globalIndex;//记录窗口索引
	
	m_nPointNum=0;
	//--end	
	
	m_bEnableDraw=TRUE;
	
	m_pCryotherapyDlg = NULL;
	m_pRightClickTarget = NULL;
	
	m_Scene.SetUsingPCColor(TRUE);
	m_bWinWidthLevelUpdate=FALSE;
	m_iTextLegendNum[0]=0;
	m_iTextLegendNum[1]=0;
	m_iTextLegendNum[2]=0;
	m_iTextLegendNum[3]=0;

	m_bMoveImageWhenMouseMove=FALSE;
}

CDisplayDlg::~CDisplayDlg()
{
	m_popupmenuTarget.DestroyMenu();
	m_popupmenu.DestroyMenu();     //Added  by Wang fengliang 070512;
	RemoveAllProjectVTarget();
}


void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDisplayDlg, CDialog)
//{{AFX_MSG_MAP(CDisplayDlg)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_SIZE()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_SPOPUP_CLEAR_DIST, OnSpopupClearDist)
ON_UPDATE_COMMAND_UI(ID_SPOPUP_CLEAR_DIST, OnUpdateSpopupClearDist)
ON_COMMAND(ID_SPOPUP_UPDATE, OnSpopupUpdate)
ON_COMMAND(ID_SPOPUP_RESET_EYE, OnSpopupResetEye)
ON_COMMAND(ID_VIEW_ONE3D, OnViewOne3d)
ON_COMMAND(ID_VIEW_FRAME_FOUR_2D, OnViewFrameFour2d)
ON_COMMAND(ID_SPOPUP_SETUP_LEGEND, OnSetupLegend)
ON_COMMAND(ID_VIEW_THREE2D_ONE3D_LEFT_TOP, OnViewThree2dOne3dLeftTop)
ON_COMMAND(ID_VIEW_THREE2D_ONE3D_TOP, OnViewThree2dOne3dTop)
ON_COMMAND(ID_POPUP_SHOWIMGLEGEND, OnPopupShowimglegend)
ON_WM_LBUTTONDBLCLK()
ON_COMMAND(ID_POPUP_SELECT, OnPopupSelect)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_PF,OnWindowLevel_PF)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_BRAIN,OnWindowLevel_BRAIN)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_IAC,OnWindowLevel_IAC)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_BONE,OnWindowLevel_BONE)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_LUNG,OnWindowLevel_LUNG)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_ABDOMEN,OnWindowLevel_ABDOMEN)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_LIVERA,OnWindowLevel_LIVERA)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_LIVERB,OnWindowLevel_LIVERB)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_SPINE,OnWindowLevel_SPINE)
ON_COMMAND(ID_SPOPUP_WINDOWLEVEL_FAT,OnWindowLevel_FAT)
ON_COMMAND(ID_SPOPUP_RESET_WINDOWLEVEL,OnResetWindowLevel)
ON_COMMAND(ID_POPUP_TARGET_SEL, OnPopupTargetSel)
ON_COMMAND(ID_POPUP_TARGET_DEL, OnPopupTargetDel)
ON_COMMAND(ID_DEBUG_VIEW_TO_PROBE, OnDebugViewToProbe)
ON_WM_ERASEBKGND()
ON_WM_SETCURSOR()
ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_SPOPUP_COLOR_IMAGE, OnSpopupColorImage)
	//}}AFX_MSG_MAP
ON_MESSAGE(DROPM_DRAGOVER,OnDragOver)
ON_MESSAGE(DROPM_DROPEX,OnDropEx)
ON_MESSAGE(DROPM_DROP,OnDrop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg message handlers

void CDisplayDlg::AddProbe(CProbe* pProbe)
{
	
	m_Scene.AddProbe(pProbe);
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::RemoveProbe(CProbe* pProbe)
{
	m_Scene.RemoveProbe(pProbe);
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::AddMRIRectWithLabel(CMRIRectangle *pMRIRect, 
									  BOOL bClean /* = TRUE */, BOOL bKeepView /*= FALSE*/)
{
	AddMRIRect(pMRIRect, bClean, bKeepView);
	ADDMRIRectAPSI(pMRIRect);
	AddMRIRectLegend(pMRIRect->GetMRIImg());
}

void CDisplayDlg::AddMRIRect(CMRIRectangle *pMRIRect, BOOL bClean, BOOL bKeepView /*= FALSE*/)
{
	if (bClean)
	{
		m_Scene.RemoveAllRectangle();
		m_lstImgNames.RemoveAll();
	}
	
	if (pMRIRect->m_pRectangle == NULL)
	{
		pMRIRect->CreateMRIRect();
	}
	
	m_pCurDispMRIRect = pMRIRect;

	
	m_Scene.AddMRIRect(pMRIRect, bKeepView);
	
	m_lstImgNames.AddTail(pMRIRect->m_sName);
	pMRIRect->m_bIsVisible = TRUE;
}

void CDisplayDlg::RemoveMRIRect(CMRIRectangle *pMRIRect)
{
	m_Scene.RemoveMRIRect(pMRIRect);
	m_lstImgNames.RemoveAt(m_lstImgNames.Find(pMRIRect->m_sName));
	pMRIRect->m_bIsVisible = FALSE;
	
	if (m_pCurDispMRIRect == pMRIRect)
	{
		m_pCurDispMRIRect = NULL;
		for (int i=0; i<PANA_2D_LEGEND_NUMBER; i++)
		{
			m_stTextLegend[i].sText = "";
			m_stTextLegend[i].bShow = FALSE;
		}
		for (i=0; i<4; i++)
		{
			m_stTextAPSI4[i].sText = "";
			m_stTextAPSI4[i].bShow = FALSE;
		}
	}
	ResetRuler();
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::RemoveAllText()
{
	m_Scene.RemoveAllText();
}

/*如果pTextLst和pRect不为空，则表示需要向其中传递标签信息*/
void CDisplayDlg::AddMRIRectLegend(CMRIImage *pImage,ST_TEXT *pTextLst /*=NULL*/,
								   CRect *pRect /*=NULL*/,int *pLegendNum /*=NULL*/,
								   CFont *pFont /*=NULL*/)
{
	
	CRect rect;
	GetClientRect(rect);

	/*采用外面传递的区域大小*/
	if(pRect!=NULL)
	{
		rect = *pRect;
	}

	
	CDC	 *pDC =g_DlgMain->GetDC();
	CFont *pOldFont=NULL;
	CString sOut;
	SIZE size;

	if(pFont)
	{
		pOldFont = pDC->SelectObject(pFont);
	}
	else if(m_stTextLegend[5].pFont)
	{
		pOldFont=pDC->SelectObject(m_stTextLegend[5].pFont);
	}

	int Legend_iX = 2;
	int Legend_iY = 11;
	int Legend_iYGap = 11;
	int Legend_iZ =0;

	if(pFont)
	{
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
		Legend_iY=abs(logFont.lfHeight);
		Legend_iYGap = Legend_iY;
	}

	m_legendType = pImage->GetImageType();
	if(m_legendType != IMS_DCMCT)
	{
		m_iTextLegendNum[0]=lstMRLeftTop.GetCount();
		m_iTextLegendNum[1]=lstMRLeftBottom.GetCount();
		m_iTextLegendNum[2]=lstMRRightTop.GetCount();
		m_iTextLegendNum[3]=lstMRRightBottom.GetCount();
		m_nNumLegendLine=m_iTextLegendNum[0]+m_iTextLegendNum[1]+m_iTextLegendNum[2]+m_iTextLegendNum[3];
		int iPos=0;

		POSITION pos=lstMRLeftTop.GetHeadPosition();
		while(pos)
		{
			int i=lstMRLeftTop.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstMRLeftBottom.GetHeadPosition();
		while(pos)
		{
			int i=lstMRLeftBottom.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstMRRightTop.GetHeadPosition();
		while(pos)
		{
			int i=lstMRRightTop.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstMRRightBottom.GetHeadPosition();
		while(pos)
		{
			int i=lstMRRightBottom.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		iPos=0;
		for(int i=0;i<m_iTextLegendNum[0];i++)
		{
			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, Legend_iY+i*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[1];i++)
		{
			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, rect.Height()-(m_iTextLegendNum[1]-i)*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[2];i++)
		{
			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, Legend_iY+i*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[3];i++)
		{
			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, rect.Height()-(m_iTextLegendNum[3]-i)*Legend_iYGap, Legend_iZ);
			iPos++;
		}
	
	
	}else
	{
		m_iTextLegendNum[0]=lstCTLeftTop.GetCount();
		m_iTextLegendNum[1]=lstCTLeftBottom.GetCount();
		m_iTextLegendNum[2]=lstCTRightTop.GetCount();
		m_iTextLegendNum[3]=lstCTRightBottom.GetCount();
		m_nNumLegendLine=m_iTextLegendNum[0]+m_iTextLegendNum[1]+m_iTextLegendNum[2]+m_iTextLegendNum[3];
		int iPos=0;

		POSITION pos=lstCTLeftTop.GetHeadPosition();
		while(pos)
		{
			int i=lstCTLeftTop.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstCTLeftBottom.GetHeadPosition();
		while(pos)
		{
			int i=lstCTLeftBottom.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstCTRightTop.GetHeadPosition();
		while(pos)
		{
			int i=lstCTRightTop.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		pos=lstCTRightBottom.GetHeadPosition();
		while(pos)
		{
			int i=lstCTRightBottom.GetNext(pos);
			m_stTextLegend[iPos].sText=GetLegend(pImage,m_legendType,i);
			iPos++;
		}

		iPos=0;
		for(int i=0;i<m_iTextLegendNum[0];i++)
		{
			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, Legend_iY+i*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[1];i++)
		{
			m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, rect.Height()-(m_iTextLegendNum[1]-i)*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[2];i++)
		{
			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, Legend_iY+i*Legend_iYGap, Legend_iZ);
			iPos++;
		}
		for(i=0;i<m_iTextLegendNum[3];i++)
		{
			size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
			m_stTextLegend[iPos].vPosition = Vector3D(rect.Width()-size.cx, rect.Height()-(m_iTextLegendNum[3]-i)*Legend_iYGap, Legend_iZ);
			iPos++;
		}
	}

	if(pOldFont)
	{
		pDC->SelectObject(pOldFont);
	}
	g_DlgMain->ReleaseDC(pDC);

	for (int i=0; i<m_nNumLegendLine; i++)
	{
		if(!pTextLst)
		{
			m_stTextLegend[i].bShow = m_bShowLegend; //TRUE;
		}
		if(pTextLst)
		{
			pTextLst[i].sText = m_stTextLegend[i].sText;
			pTextLst[i].vPosition = m_stTextLegend[i].vPosition;
		}

	}
	for(i=m_nNumLegendLine; i< PANA_2D_LEGEND_NUMBER;i++)
	{
		m_stTextLegend[i].bShow = FALSE;
	}

	if(pLegendNum)
	{
		*pLegendNum = m_nNumLegendLine;
	}
}

void CDisplayDlg::ADDMRIRectAPSI(CMRIRectangle *pMRIRect, BOOL bClean /* = TRUE */)
{
	if (bClean)
	{
		//m_Scene.RemoveAllText();
		for (int i=0; i<4; i++)
		{
			m_Scene.RemoveText(&(m_stTextAPSI4[i]));
		}
	}
	// Labe APSI
	CMRIImage* pMriImage;
	pMriImage = pMRIRect->GetMRIImg();
	//	sOrientChar = pMriImage->GetOrientChar();
	//	ppdCharPosition = pMriImage->GetOrientCharPos();
	CString* psDispOri = pMriImage->GetDispOrientChar();
	for ( int j = 0; j < 4; j++ )
	{		
		m_stTextAPSI4[j].sText = psDispOri[j];
		m_stTextAPSI4[j].bShow = TRUE;
	}
	m_Scene.SetIsTextDrawn( TRUE );
}


//Added by  Wang fengliang 070512;
BOOL CDisplayDlg::CreatePopupMenu()
{
	m_popupmenu.DestroyMenu();
	this->m_popupmenu.CreatePopupMenu();
	
	if(!m_b3DDisplay) //3dWindow不需要  Update MRI
	{
		m_popupmenu.AppendMenu(MF_STRING,ID_SPOPUP_UPDATE,(LPCTSTR)"&Update MRI");
		m_popupmenu.AppendMenu(MF_STRING, ID_POPUP_SHOWIMGLEGEND, (LPCTSTR)"&Show legend");
		m_popupmenu.AppendMenu(MF_STRING, ID_POPUP_SELECT, (LPCTSTR)"Select series");
		m_popupmenu.AppendMenu(MF_SEPARATOR);
	}
	m_popupmenu.AppendMenu(MF_STRING, ID_SPOPUP_RESET_EYE, (LPCTSTR)"&Reset view");
	m_popupmenu.AppendMenu(MF_STRING, ID_SPOPUP_COLOR_IMAGE, (LPCTSTR)"&Color Image");
	if(!m_b3DDisplay) //3dWindow不需要  Update MRI
	{
		m_popupmenu.AppendMenu(MF_STRING, ID_SPOPUP_RESET_WINDOWLEVEL, (LPCTSTR)"Reset &Window/Level");
	}
	m_popupmenu.AppendMenu(MF_STRING, ID_SPOPUP_CLEAR_DIST, (LPCTSTR)"&Clear distance measurement");	
	m_popupmenu.AppendMenu(MF_SEPARATOR);
	m_popupmenu.AppendMenu(MF_STRING,ID_VIEW_ONE3D,(LPCTSTR)"Display in 3D");
	m_popupmenu.AppendMenu(MF_STRING,ID_VIEW_FRAME_FOUR_2D,(LPCTSTR)"Display in Four2D");
	m_popupmenu.AppendMenu(MF_STRING,ID_VIEW_THREE2D_ONE3D_LEFT_TOP,(LPCTSTR)"Display in Three 2D+3D LT");
	m_popupmenu.AppendMenu(MF_STRING,ID_VIEW_THREE2D_ONE3D_TOP,(LPCTSTR)"Display in Three 2D+3D Top");
	
#ifdef _DEBUG
	{
		m_popupmenu.AppendMenu(MF_SEPARATOR);
		m_popupmenu.AppendMenu(MF_STRING, ID_DEBUG_VIEW_TO_PROBE, (LPCTSTR)"View to probe");
	}
#endif
	m_popupmenu.AppendMenu(MF_SEPARATOR);
	m_popupmenu.AppendMenu(MF_STRING, ID_SPOPUP_SETUP_LEGEND,(LPCTSTR)"Setup Legend...");

	if(!m_b3DDisplay)
	{
		if(m_pCurDispMRIRect)
		{
			if(m_pCurDispMRIRect->GetMRIImg()->GetImageType() == IMS_DCMCT)
			{
				CMenu subMenu;
				subMenu.CreatePopupMenu();
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_PF,(LPCTSTR)"P.F. (Posterior Fossa) (150/1059)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_BRAIN,(LPCTSTR)"Brain (70/1059)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_IAC,(LPCTSTR)"IAC (4095/1624)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_BONE,(LPCTSTR)"Bone (2000/1824)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_LUNG,(LPCTSTR)"Lung (1600/424)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_ABDOMEN,(LPCTSTR)"Abdomen (350/1084)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_LIVERA,(LPCTSTR)"Liver A (200/1124)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_LIVERB,(LPCTSTR)"Liver B (220/1049)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_SPINE,(LPCTSTR)"Spine (300/1084)");
				subMenu.AppendMenu(MF_STRING,ID_SPOPUP_WINDOWLEVEL_FAT,(LPCTSTR)"Fat (270/989)");
		
				m_popupmenu.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, (LPCTSTR)"Window/Level Presets");
			}
		}
	}
		
	m_popupmenuTarget.DestroyMenu();
	this->m_popupmenuTarget.CreatePopupMenu();
	//	m_popupmenuTarget.SetPopupMenu(TRUE);
	m_popupmenuTarget.AppendMenu(MF_STRING, ID_POPUP_TARGET_SEL, (LPCTSTR)"Select target");
	m_popupmenuTarget.AppendMenu(MF_STRING, ID_POPUP_TARGET_DEL, (LPCTSTR)"Delete target");
	//	m_popupmenuTarget.AdjustMenuItem();
	
	return TRUE;
}

BOOL CDisplayDlg::SetCheckMenu(UINT menuItemID,UINT isChecked)
{
	this->m_popupmenu.CheckMenuItem(menuItemID,isChecked);
	return TRUE;
}

//-end 
BOOL CDisplayDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hwnd = this->GetSafeHwnd();
	m_Scene.Create(hwnd);
	
	m_Scene.SetRenderMode( RENDER_SMOOTHSHADE);
	m_Scene.m_Display.SetBGColor( 0.0f, 0.0f, 0.0f );
	
	{
		//Init legend
		CString sOut = "";
				
		for (int i=0; i<PANA_2D_LEGEND_NUMBER; i++)
		{
			m_stTextLegend[i].sText = sOut;
			m_stTextLegend[i].cfColor.SetColor(1.0, 1.0, 1.0, 1.0);
			m_stTextLegend[i].bShow = FALSE;
			
			
			m_stTextLegend[i].pFont = CGlobalFuncs::GetFont();
			
			m_Scene.Add2DText(&m_stTextLegend[i]);
		}
		
		
		for (i=0; i<4; i++)
		{
			m_stTextAPSI4[i].sText = "";
			m_stTextAPSI4[i].cfColor.SetColor(1.0, 1.0, 1.0, 1.0);
			m_stTextAPSI4[i].bShow = FALSE;
			
			m_stTextAPSI4[i].pFont = CGlobalFuncs::GetFont();
			
			m_Scene.Add2DText(&(m_stTextAPSI4[i]));
		}

		m_Scene.m_ruler.m_pFont=CGlobalFuncs::GetFont();
	}
	
	m_stRT.sText = "RT";
	m_stRT.vPosition = Vector3D(-60, -65, -200);
	m_stRT.cfColor.SetColor(0, 1.0, 0, 1.0);
	m_stRT.bShow = FALSE;
    
	m_stRT.pFont = CGlobalFuncs::GetFont40();
	m_Scene.Add2DText(&m_stRT);
	
	m_stSel.sText = "S";
	m_stSel.vPosition = Vector3D(60, -65, -200);
	m_stSel.cfColor.SetColor(0, 1.0, 0, 1.0);
	m_stSel.bShow = FALSE;
    
	m_stSel.pFont = CGlobalFuncs::GetFont20();
	m_Scene.Add2DText(&m_stSel);
	
	
	m_bEnalbeDistMeasure = FALSE;
	m_bEnable3DDistMeasure = FALSE;
	m_bEnableClickTargetOnMRI = FALSE;
	
	m_bShowLegend = TRUE;
	
	CreatePopupMenu();//Added by Wang fengliang 070512;
	
	m_dropEx.Register(this);
	
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	//	m_hCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ARROW));   
	
	CConfigMgr conf;
	CString sTmp;
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_LEFTTOP,CONF_SET_DISPLAY_MR_LEFTTOP_DEFAULT);
	GetIndex(sTmp,&lstMRLeftTop);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_LEFTBOTTOM,CONF_SET_DISPLAY_MR_LEFTBOTTOM_DEFAULT);
	GetIndex(sTmp,&lstMRLeftBottom);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_RIGHTTOP,CONF_SET_DISPLAY_MR_RIGHTTOP_DEFAULT);
	GetIndex(sTmp,&lstMRRightTop);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_MR_RIGHTBOTTOM,CONF_SET_DISPLAY_MR_RIGHTBOTTOM_DEFAULT);
	GetIndex(sTmp,&lstMRRightBottom);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_LEFTTOP,CONF_SET_DISPLAY_CT_LEFTTOP_DEFAULT);
	GetIndex(sTmp,&lstCTLeftTop);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_LEFTBOTTOM,CONF_SET_DISPLAY_CT_LEFTBOTTOM_DEFAULT);
	GetIndex(sTmp,&lstCTLeftBottom);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_RIGHTTOP,CONF_SET_DISPLAY_CT_RIGHTTOP_DEFAULT);
	GetIndex(sTmp,&lstCTRightTop);
	sTmp=conf.Read(CONF_SET_DISPLAY,CONF_SET_DISPLAY_CT_RIGHTBOTTOM,CONF_SET_DISPLAY_CT_RIGHTBOTTOM_DEFAULT);
	GetIndex(sTmp,&lstCTRightBottom);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDisplayDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_bEnableDraw)
	{
		if(IsWindowVisible())
		{
			m_Scene.DrawScene();
		}
	}else
	{
		if(m_index!=1)
		{
			int i=0;
		}
	}
			
	// Do not call CDialog::OnPaint() for painting messages
}

void CDisplayDlg::OnSize(UINT nType, int cx, int cy) 
{
	if(!this->GetSafeHwnd())
		return;

	CDialog::OnSize(nType, cx, cy);
	
	int Legend_iX = 2;
	int Legend_iY = 12;
	int Legend_iYGap = 11;
	int Legend_iZ =0;

	m_stTextAPSI4[0].vPosition = Vector3D(cx/2, 20, 0);
	m_stTextAPSI4[1].vPosition = Vector3D(cx-30, cy/2, 0);
	m_stTextAPSI4[2].vPosition = Vector3D(cx/2, cy-20, 0);
	m_stTextAPSI4[3].vPosition = Vector3D(20, cy/2, 0);	
	m_stRT.vPosition = Vector3D(20, cy-20, 0);
	m_stSel.vPosition = Vector3D(cx-40,cy-20, 0);
	
	CDC	 *pDC =g_DlgMain->GetDC();
	CFont *pOldFont=NULL;
	SIZE size;

	if(m_stTextLegend[5].pFont)
			pOldFont=pDC->SelectObject(m_stTextLegend[5].pFont);

	int iPos=0;
	for(int i=0;i<m_iTextLegendNum[0];i++)
	{
		m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, Legend_iY+i*Legend_iYGap, Legend_iZ);
		iPos++;
	}
	for(i=0;i<m_iTextLegendNum[1];i++)
	{
		m_stTextLegend[iPos].vPosition = Vector3D(Legend_iX, cy-(m_iTextLegendNum[1]-i)*Legend_iYGap, Legend_iZ);
		iPos++;
	}
	for(i=0;i<m_iTextLegendNum[2];i++)
	{
		size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
		m_stTextLegend[iPos].vPosition = Vector3D(cx-size.cx, Legend_iY+i*Legend_iYGap, Legend_iZ);
		iPos++;
	}
	for(i=0;i<m_iTextLegendNum[3];i++)
	{
		size=pDC->GetTextExtent(m_stTextLegend[iPos].sText);
		m_stTextLegend[iPos].vPosition = Vector3D(cx-size.cx, cy-(m_iTextLegendNum[3]-i)*Legend_iYGap, Legend_iZ);
		iPos++;
	}
	
	if(m_stTextLegend[5].pFont && pOldFont)
			pDC->SelectObject(pOldFont);

	g_DlgMain->ReleaseDC(pDC);
	
	// TODO: Add your message handler code here
	m_Scene.Reshape(cx,cy);
	ResetRuler();
	UpdateWindow();
	
}

void CDisplayDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_bMoveImageWhenMouseMove=FALSE;
	m_bEnableMouseMove=TRUE;
	
	m_iLRPoint = point;

	SetFocus();
	m_ptStart = point;
	m_ptWindowLevelStart=point;
	
	if (nFlags & MK_SHIFT || nFlags & MK_CONTROL)
	{
		m_ptStart = point;
	}
	
	//if ((nFlags & MK_CONTROL) || (nFlags & MK_SHIFT))
	//	m_bDrawSelectRect = FALSE;
	CDialog::OnLButtonDown(nFlags, point);
}

void CDisplayDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_bEnableMouseMove=TRUE;
	
	m_ptStart = point;
	
	CDialog::OnMButtonDown(nFlags, point);
}

void CDisplayDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bEnableMouseMove=FALSE;
	
	m_ptStart = point;
	
	CDialog::OnMButtonDown(nFlags, point);
}

void CDisplayDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bEnableMouseMove=FALSE;
	
	m_iLRPoint = point;

	if (!m_pDispWrap->m_bComparison)
	{
		if (m_bSelected)
		{
			//SelectDisplayMRISeries(FALSE);
		}
		else
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->SetSelectedWnd(this);
		}
	}

	if(this->m_bMoveImageWhenMouseMove)
	{
		m_pDisplayControl->AddCurrentTo3D();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);

	}

	if(nFlags & MK_CONTROL)
		return;
	if(nFlags & MK_SHIFT)
		return;
	if(::GetKeyState(VK_MENU) & 0xf0)
		return;
	
	if(m_bWinWidthLevelUpdate)
	{
		m_pDispWrap->SetWindowWidthAndLevel( point, point,TRUE );
		m_bWinWidthLevelUpdate=FALSE;
	}
	
	BOOL bShouldRefresh=FALSE;
	// 2D distance measurement
	if (m_bEnalbeDistMeasure && m_pCurDispMRIRect)
	{
		Line3D lLine;
		m_Scene.Click( (int) point.x, (int)point.y, &lLine );
		bShouldRefresh=TRUE;

		CRectangle* pRectangle = m_pCurDispMRIRect->m_pRectangle;
		CIntersection intersectionOper;
		Vector3D vIntersectionPt;
		if ( ERR_OK != intersectionOper.Intersect( &lLine, pRectangle, vIntersectionPt ) )
		{
			if(bShouldRefresh)
			{
				m_Scene.DrawScene();
			}
			return;
		}
		
		m_Scene.AddDistMeasurePoint(vIntersectionPt);
		if(++m_nPointNum>=2)
		{
			m_nPointNum=0;
			m_pCryotherapyDlg->m_dlgCmdBar.EnableMeasure(FALSE);
		}
		
	}
	
	// 3D distance measurement
	if (m_bEnable3DDistMeasure)
	{
		Line3D lLine;
		m_Scene.Click( (int) point.x, (int)point.y, &lLine );
		bShouldRefresh=TRUE;

		Vector3D vPos;
		if (GetIntersectPosWithVisibleMRIIn3D(&lLine, vPos))
		{
			m_Scene.AddDistMeasurePoint(vPos);
			if(++m_nPointNum>=2)
			{
				m_nPointNum=0;
				m_pCryotherapyDlg->m_dlgCmdBar.EnableMeasure(FALSE);
			}
		}
		
	}
	
	// Create virtual target
	if (m_bEnableClickTargetOnMRI)
	{
		Line3D lLine;
		m_Scene.Click( (int) point.x, (int)point.y, &lLine );
		bShouldRefresh=TRUE;

		if (m_bEnableClickTargetOnMRI)
		{
			Vector3D vPos;
			BOOL bRet;
			if (m_Scene.GetSceneID() == 1)
			{
				bRet = GetIntersectPosWithVisibleMRIIn3D(&lLine, vPos);
			}
			else 
			{
				bRet = GetIntersectPosWithCurMRI(&lLine, vPos);
			}
			if (bRet)
			{
				m_pCryotherapyDlg->m_TargetPool.CreateATarget(vPos);
				bShouldRefresh=FALSE;
			}
			m_pCryotherapyDlg->m_dlgCmdBar.EnableCreatVTarget(FALSE);
		}
		if(bShouldRefresh)
		{
			m_Scene.DrawScene();
		}
		return;
	}
	
	// For target selecting 
	// - Begin
	CList<CVirtualTarget*, CVirtualTarget*>* plstVirtualTarget;
	if (m_lstPrjVTargets.GetCount() <= 0) /*ningsj 3D窗口中使用TargetTool中的VT数据*/
	{
		plstVirtualTarget = &(m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets);
	}
	else
	{
		plstVirtualTarget = &m_lstPrjVTargets;
	}
	CVirtualTarget* pSelectedTarget = NULL;
	int nSeleted = -1;
	nSeleted = m_Scene.SelectVirtualTargetByClicking(plstVirtualTarget, point);
	if (nSeleted >= 0)
	{
		int iRealIndex= plstVirtualTarget->GetAt(plstVirtualTarget->FindIndex(nSeleted))->GetIndex();
		plstVirtualTarget = &(m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets);
		for(int i=0;i<plstVirtualTarget->GetCount();i++)
		{
			pSelectedTarget = plstVirtualTarget->GetAt(plstVirtualTarget->FindIndex(i));
			if(pSelectedTarget->GetIndex()==iRealIndex)
			{
			    /*
			      单击靶点选中该靶点，再次单击则取消选中靶点
				*/
				if(pSelectedTarget->IsSelected()==TRUE)
				{
					pSelectedTarget->SetSelected(FALSE);
				}
				else
				{
					pSelectedTarget->SetSelected(TRUE);
				}
				
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.UpdateAllProjectVTarget();
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.UpdateAllProjectVTarget();
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.UpdateAllProjectVTarget();
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.UpdateAllProjectVTarget();
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene();

				bShouldRefresh=FALSE;
			}
		}
		
		
	}
	// - End
	
	/*if (m_bRgnSelect)
	{
	m_ptRectSelectEnd = point;
	m_Scene.SelectRectangle(m_Scene.m_pCurrentPointCloud, m_ptRectSelectStart.x, m_ptRectSelectStart.y, m_ptRectSelectEnd.x, m_ptRectSelectEnd.y);
	}
	else
	{
	Vector3D* v = m_Scene.Pick(point.x, point.y);
	if (v!=NULL)
	{
	m_paPickedPnt.Add(v);
	((CDisplayDlg*)GetParent())->CountSelected(this, m_paPickedPnt.GetSize());
	}
	}*/
	
	if(bShouldRefresh)
	{
		m_Scene.DrawScene();
	}

	if(m_pCryotherapyDlg->m_dlgNavigate.m_bSameWWWL) 
	{
		if (m_pDispWrap->m_pDispSeries)
		{
			int iWidth=m_pDispWrap->m_pDispSeries->GetMRISeries()->GetWindowWidth();
			int iLevel=m_pDispWrap->m_pDispSeries->GetMRISeries()->GetWindowLevel();
			if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_pDispSeries)
			{
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_pDispSeries->GetMRISeries()->SetWindowWidthAndLevel(iWidth,iLevel);
				m_pCryotherapyDlg->UpdateTextureSameSeries(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_pDispSeries,FALSE);
				int index=m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.m_iCurDispIndex;
				//m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.ShowImage(index);
			}
			if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_pDispSeries)
			{
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_pDispSeries->GetMRISeries()->SetWindowWidthAndLevel(iWidth,iLevel);
				m_pCryotherapyDlg->UpdateTextureSameSeries(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_pDispSeries,FALSE);
				int index=m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.m_iCurDispIndex;
				//m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.ShowImage(index);
			}
			if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_pDispSeries)
			{
				m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_pDispSeries->GetMRISeries()->SetWindowWidthAndLevel(iWidth,iLevel);
				m_pCryotherapyDlg->UpdateTextureSameSeries(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_pDispSeries,FALSE);
				int index=m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.m_iCurDispIndex;
				//m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.ShowImage(index);
			}

			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.Invalidate(FALSE);
		}
	}

	
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDisplayDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bEnableMouseMove)
		return;
			
	if(m_b3DDisplay)  //3D窗口保留
	{
		if( (nFlags & MK_LBUTTON) || (nFlags & MK_MBUTTON) )
		{		
			if(nFlags & MK_LBUTTON)
			{
				nFlags = MK_MBUTTON;
			}else if(nFlags & MK_MBUTTON)
			{
				nFlags = MK_CONTROL;
			}
			if( m_iTransRotateFlag==0 )		// OpenGL view change
			{		
				CSize dif = m_ptStart - point;
				m_ptStart = point;
				m_Scene.MouseMove( nFlags, dif.cx, dif.cy );
			}
			Invalidate( FALSE );
			UpdateWindow();
		}	
	} 
	else//2D窗口去除旋转，只保留缩放和移动
	{
		if((nFlags & MK_LBUTTON) && ( nFlags & MK_CONTROL)) 
		{		
			if( m_iTransRotateFlag==0 )		// OpenGL view change
			{		
				CSize dif = m_ptStart - point;
				m_ptStart = point;
				m_Scene.MouseMove( nFlags, dif.cx, dif.cy );				
			}
			ResetRuler();
			Invalidate( FALSE );
			UpdateWindow();

		}else if((nFlags & MK_LBUTTON) && (nFlags & MK_SHIFT)) 
		{
			int index=m_pDisplayControl->m_iCurDispIndex;
			
			if(point.x-m_ptWindowLevelStart.x>0)
			{
				index++; 
			}else
			{
				index--;
			}
			m_bMoveImageWhenMouseMove=TRUE;
			m_pDisplayControl->ShowImage(index);
			m_ptWindowLevelStart = point;

		}else if(nFlags & MK_MBUTTON)
		{
			nFlags=MK_CONTROL;
			if( m_iTransRotateFlag==0 )		// OpenGL view change
			{		
				CSize dif = m_ptStart - point;
				m_ptStart = point;
				m_Scene.MouseMove( nFlags, dif.cx, dif.cy );				
			}
			ResetRuler();
			Invalidate( FALSE );
			UpdateWindow();
		}else if( (nFlags & MK_LBUTTON) )  // 改变窗宽窗位 ningsj 2009.2.12
		{
			if(m_bLRArrow)
			{
				if(point.x > m_iLRPoint.x )
					m_pDisplayControl->BackwardImage();
				if(point.x < m_iLRPoint.x )
					m_pDisplayControl->ForwardImage();				

				m_iLRPoint = point;
			}
			else
			{
			if(m_ptWindowLevelStart.x!=point.x || m_ptWindowLevelStart.y!=point.y)
			{
				m_pDispWrap->SetWindowWidthAndLevel( m_ptWindowLevelStart, point,FALSE );
			}
			m_ptWindowLevelStart = point;
			m_bWinWidthLevelUpdate=TRUE;
		}
	}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


void CDisplayDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		
	if (!(nFlags & MK_SHIFT) && !m_pDispWrap->m_bComparison)
	{
		CreatePopupMenu();

		POINT lpPoint;
		lpPoint.x = point.x;
		lpPoint.y = point.y;
		ClientToScreen(&lpPoint);
		
		CList<CVirtualTarget*, CVirtualTarget*>* plstVirtualTarget;
		if (m_lstPrjVTargets.GetCount() <= 0) /*ningsj 3D窗口中使用TargetTool中的VT数据*/
		{
			plstVirtualTarget = &(m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets);
		}
		else
		{
			plstVirtualTarget = &m_lstPrjVTargets;
		}
		CVirtualTarget* pSelectedTarget = NULL;
		int nSeleted = -1;
		nSeleted = m_Scene.SelectVirtualTargetByClicking(plstVirtualTarget, point);
		
		if (nSeleted >= 0)
		{
			int iRealIndex= plstVirtualTarget->GetAt(plstVirtualTarget->FindIndex(nSeleted))->GetIndex();
			plstVirtualTarget = &(m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets);
			for(int i=0;i<plstVirtualTarget->GetCount();i++)
			{
				m_pRightClickTarget = plstVirtualTarget->GetAt(plstVirtualTarget->FindIndex(i));
				if(m_pRightClickTarget->GetIndex()==iRealIndex)
				{
					break;
				}
			}
			if(i<plstVirtualTarget->GetCount())
			{
				m_popupmenuTarget.CheckMenuItem(ID_POPUP_TARGET_SEL, m_pRightClickTarget->IsSelected() ? MF_CHECKED : MF_UNCHECKED);
				m_popupmenuTarget.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, lpPoint.x, lpPoint.y, this);	//显示弹出菜单
			}
		}
		else
		{
			SetCheckMenu(ID_SPOPUP_UPDATE, m_bUpdate ? MF_CHECKED : MF_UNCHECKED);
			SetCheckMenu(ID_POPUP_SHOWIMGLEGEND, m_bShowLegend ? MF_CHECKED : MF_UNCHECKED);
			SetCheckMenu(ID_POPUP_SELECT, m_bSelected ? MF_CHECKED : MF_UNCHECKED);
			if (m_pDispWrap->m_pDispSeries)
			{
				if(m_pDispWrap->m_pDispSeries->GetMRISeries()->m_oClut32.IsGreyColor())
				{
					SetCheckMenu(ID_SPOPUP_COLOR_IMAGE, MF_CHECKED);
				}else
				{
					SetCheckMenu(ID_SPOPUP_COLOR_IMAGE, MF_UNCHECKED);
				}
			}else
			{
				SetCheckMenu(ID_SPOPUP_COLOR_IMAGE, MF_UNCHECKED);
			}
			m_popupmenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, lpPoint.x, lpPoint.y, this);	//显示弹出菜单
		}
	}
	
	CDialog::OnRButtonDown(nFlags, point);
}


void CDisplayDlg::EnableLRArrow(BOOL bEnable)
{
	m_bLRArrow = bEnable;
}

void CDisplayDlg::EnableDistanceMeasure(BOOL bEnable)
{
	m_bEnalbeDistMeasure = bEnable;
	m_Scene.ResetDistMeasure();
}

void CDisplayDlg::Enable3DDistanceMeasure(BOOL bEnable)
{
	m_bEnable3DDistMeasure = bEnable;
	m_Scene.ResetDistMeasure();
}




void CDisplayDlg::OnSpopupClearDist() 
{
	// TODO: Add your command handler code here
	m_Scene.RemoveAllDistMeasurement();
	
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::OnUpdateSpopupClearDist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CDisplayDlg::AddMesh(CMesh *pMesh)
{
	m_Scene.AddModelMesh(pMesh);
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::EnableClickTargetOnMRI(BOOL bEnable)
{
	m_bEnableClickTargetOnMRI = bEnable;
}

void CDisplayDlg::AddVirtualTarget(CVirtualTarget *pVTarget)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;
	m_Scene.GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	m_Scene.AddVirtualTarget(pVTarget);
	m_Scene.SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	
	
}

bool CDisplayDlg::AddProjectVirtualTarget(CVirtualTarget *pVTarget)
{
	if (m_pCurDispMRIRect)
	{
		return AddProjectVirtualTarget(m_pCurDispMRIRect, pVTarget);
		
	}else
	{
		return false;
	}
}

bool CDisplayDlg::AddProjectVirtualTarget(CMRIRectangle *pMRIRect, CVirtualTarget *pVTarget)
{
	double* dbSliceDir = pMRIRect->GetMRIImg()->GetSliceDirection();
	
	Vector3D vEyePos = m_Scene.GetEyePoint();
	
	Line3D *plLine = new Line3D(pVTarget->GetPos(), Vector3D(dbSliceDir[0], dbSliceDir[1], dbSliceDir[2]));
	Vector3D vPos;
	if (IntersectMRIWithLine(plLine, m_pCurDispMRIRect, vPos))
	{
		CVirtualTarget *pVPrjTarget = new CVirtualTarget(vPos,pVTarget->GetColor(),
			pMRIRect->GetMRIImg()->GetSliceDirection());
		pVPrjTarget->SetSelected(pVTarget->IsSelected());
		pVPrjTarget->SetIndex(pVTarget->GetIndex());
		
		double dbDisEyeTarget = (vEyePos - pVTarget->GetPos()).GetLen();
		double dbDisEyeProTarget = (vEyePos - vPos).GetLen();
		double dbDisVTarget2VPrjTarget = (vPos - pVTarget->GetPos()).GetLen();
		if (fabs(dbDisEyeProTarget - dbDisEyeTarget) < 0.001 
			|| dbDisVTarget2VPrjTarget <= pMRIRect->GetMRIImg()->GetThickness() / 2)
		{
			pVPrjTarget->SetRelativePosition(0);
		}
		else if (dbDisEyeProTarget > dbDisEyeTarget)
		{
			pVPrjTarget->SetRelativePosition(2);
		}
		else if (dbDisEyeProTarget < dbDisEyeTarget)
		{
			pVPrjTarget->SetRelativePosition(1);
		}
		
		AddVirtualTarget(pVPrjTarget);
		m_lstPrjVTargets.AddTail(pVPrjTarget);
		
		
	}
	delete plLine;
	return true;
}

void CDisplayDlg::UpdateAllProjectVTarget(BOOL bClean /*= TRUE*/)
{
	if (bClean)
	{
		RemoveAllProjectVTarget();
	}
	if (m_pCurDispMRIRect)
	{
		POSITION pos = m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets.GetHeadPosition();
		for (;pos;)
		{
			CVirtualTarget *pVTarget = m_pCryotherapyDlg->m_TargetPool.m_lstVirtualTargets.GetNext(pos);
			AddProjectVirtualTarget(m_pCurDispMRIRect, pVTarget);
		}
	}
	
	if (bClean)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CDisplayDlg::RemoveAllProjectVTarget()
{
	if (m_lstPrjVTargets.GetCount() > 0)
	{
		POSITION pos = m_lstPrjVTargets.GetHeadPosition();
		for (;pos;)
		{
			CVirtualTarget* pVTarget = m_lstPrjVTargets.GetNext(pos);
			RemoveVirtualTarget(pVTarget);
			delete pVTarget;
		}
		m_lstPrjVTargets.RemoveAll();
	}
}

void CDisplayDlg::RemoveVirtualTarget(CVirtualTarget *pVTarget)
{
	m_Scene.RemoveVirtualTarget(pVTarget);
}



BOOL CDisplayDlg::GetIntersectPosWithVisibleMRIIn3D(Line3D *pLine, Vector3D& vIntersectPos)
{
	if (pLine == NULL)
		return FALSE;
	
	MRIRectSeries *pMRIRectSeries;
	CIntersection interOper;
	double dbMinDist = 999999.0;
	double dbDist;
	Vector3D vTmpIntersectPos;
	Vector3D vEyePos = m_Scene.GetEyePoint();
	BOOL bRet = FALSE;
	
	for (int i=1; i<5; i++)
	{
		pMRIRectSeries = &(m_pCryotherapyDlg->m_MRIRectSeries[i]);
		if (pMRIRectSeries)
		{
			int iRectNum = pMRIRectSeries->GetRectNum();
			CMRIRectangle *pMRIRect;
			for (int j=0; j<iRectNum; j++)
			{
				pMRIRect = pMRIRectSeries->GetMRIRectAt(j);
				if (pMRIRect && pMRIRect->IsVisible3D())
				{
					if ( ERR_OK == interOper.Intersect(pLine, pMRIRect->m_pRectangle, vTmpIntersectPos))
						//if (IntersectMRIWithLine(pLine, pMRIRect, vTmpIntersectPos))
					{
						dbDist = (vTmpIntersectPos - vEyePos).GetLen();
						if (dbDist < dbMinDist)
						{
							vIntersectPos = vTmpIntersectPos;
							dbMinDist = dbDist;
							bRet = TRUE;
						}
					}
				}
			}
		}
	}
	return bRet;
}

BOOL CDisplayDlg::GetIntersectPosWithCurMRI(Line3D *pLine, Vector3D& vIntersectPos)
{
	if (pLine == NULL || m_pCurDispMRIRect == NULL)
		return FALSE;
	
	CIntersection interOper;
	Vector3D vTmpIntersectPos;
	//	Vector3D vEyePos = m_Scene.GetEyePoint();
	BOOL bRet = FALSE;
	
	if (ERR_OK == interOper.Intersect(pLine, m_pCurDispMRIRect->m_pRectangle, vTmpIntersectPos))
	{
		vIntersectPos = vTmpIntersectPos;
		bRet = TRUE;
	}
	return bRet;
}

BOOL CDisplayDlg::IntersectMRIWithLine(Line3D *pLine, CMRIRectangle *pMRIRect, Vector3D& vInterSectPos)
{
	BOOL bRet = FALSE;
	CIntersection interOper;
	if (ERR_OK == interOper.Intersect(pLine, pMRIRect->m_pRectangle, vInterSectPos))
	{
		bRet = TRUE;
	}
	return bRet; 
}

BOOL CDisplayDlg::IsRcvWnd()
{
	return m_bUpdate;
}

void CDisplayDlg::SetRcvWnd(BOOL bRcv)
{
	m_bUpdate = bRcv;
	m_stRT.bShow = bRcv;
	SetCheckMenu(ID_SPOPUP_UPDATE, m_bUpdate ? MF_CHECKED : MF_UNCHECKED);
	Invalidate(FALSE);
	UpdateWindow();
}

void CDisplayDlg::OnSpopupUpdate() //Popup menu "Update MRI"，显示"RT"
{
	m_bUpdate = !m_bUpdate;
	SetRcvWnd(m_bUpdate);
	
	if(m_bUpdate)
	{
		if(m_index!=1) //第一窗口没有，Update 菜单项
		{
			//无需代码
		}
		if(m_index!=2) 
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.SetRcvWnd(FALSE);
		}
		if(m_index!=3) 
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.SetRcvWnd(FALSE);
		}
		if(m_index!=4) 
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.SetRcvWnd(FALSE);
		}
		if(m_index!=5) 
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.SetRcvWnd(FALSE);
		} 
	}
}

void CDisplayDlg::OnSpopupResetEye() // "Reset View"
{
	ResetView();	
}

void CDisplayDlg::ResetView() // "Reset View"
{
	if (m_b3DDisplay)
	{
		m_Scene.ResetView();
	}
	else if (m_pCurDispMRIRect)
	{
		m_Scene.ChangeViewToMRIRect(m_pCurDispMRIRect,TRUE);

	}
	
	Invalidate(FALSE);
	UpdateWindow();
	ResetRuler();
	Invalidate(FALSE); /*再次刷屏，否则标尺可能不会刷新*/
	UpdateWindow();
}

void CDisplayDlg::OnViewFrameFour2d()   // 2D 2D
{										// 2D 2D
	m_pCryotherapyDlg->m_pDisplay4Dlg->SetDispMode(DM_22_2D);
	m_pCryotherapyDlg->m_pDisplay4Dlg->UpdatePopupMenuDispMode(ID_VIEW_FRAME_FOUR_2D);
}

void CDisplayDlg::OnViewThree2dOne3dLeftTop()   // 3D 2D
{												// 2D 2D
	m_pCryotherapyDlg->m_pDisplay4Dlg->SetDispMode(DM_22_3D);
	m_pCryotherapyDlg->m_pDisplay4Dlg->UpdatePopupMenuDispMode(ID_VIEW_THREE2D_ONE3D_LEFT_TOP);
}

void CDisplayDlg::OnViewThree2dOne3dTop()   //    3D
{											// 2D 2D 2D
	m_pCryotherapyDlg->m_pDisplay4Dlg->SetDispMode(DM_13);
	m_pCryotherapyDlg->m_pDisplay4Dlg->UpdatePopupMenuDispMode(ID_VIEW_THREE2D_ONE3D_TOP);
}
void CDisplayDlg::OnViewOne3d() 
{
	m_pCryotherapyDlg->m_pDisplay4Dlg->SetDispMode(DM_1);
	m_pCryotherapyDlg->m_pDisplay4Dlg->UpdatePopupMenuDispMode(ID_VIEW_ONE3D);
}


void CDisplayDlg::OnPopupShowimglegend() 
{
	// TODO: Add your command handler code here
	ShowLegend(!m_bShowLegend);
}

void CDisplayDlg::ShowLegend(BOOL bShow)
{
	m_bShowLegend = bShow;
	for (int i=0; i<m_nNumLegendLine; i++)
	{
		m_stTextLegend[i].bShow = m_bShowLegend;
	}
	ResetRuler();
	Invalidate(FALSE);
	UpdateWindow();
}

DROPEFFECT CDisplayDlg::OnDragOver(WPARAM pDropInfoClass, LPARAM lParm) //拖动到上面时执行
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));
	
	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

DROPEFFECT CDisplayDlg::OnDropEx(WPARAM pDropInfoClass, LPARAM lParm)
{
	return (DROPEFFECT)-1;
}

BOOL CDisplayDlg::OnDrop(WPARAM pDropInfoClass, LPARAM lParm)
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));
	
	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
	{
		HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData( CF_TEXT );
		char* lp = (char *)GlobalLock((HGLOBAL) hMem);//lock source
		if (lp != NULL)
		{
			if (m_pDispWrap->m_bComparison)
			{
				DisplayMRIToCompareWindow(lp);
			}
			else
			{
				DisplayMRIToMainDispalyWnd(lp);
			}
		}
		GlobalUnlock( hMem );//unlock source
		return TRUE;
	}
	else
		return FALSE;
}

int CDisplayDlg::DisplayMRIToCompareWindow(CString sMRIName)
{
	if (m_pCryotherapyDlg == NULL)
	{
		m_pCryotherapyDlg = &(g_DlgMain->m_dlgCryotherapy);
	}
	CMRISeries *pMRISeries = m_pCryotherapyDlg->GetMRISeries(sMRIName);
	if (pMRISeries && m_pDlgDispCompare)
	{
		m_pDlgDispCompare->DisplayMRISeriesToWindow(pMRISeries, this);
	}
	return 0;
}

void CDisplayDlg::DisplayMRIToMainDispalyWnd(CString sMRIName)
{
	if (m_pCryotherapyDlg == NULL)
	{
		m_pCryotherapyDlg = &(g_DlgMain->m_dlgCryotherapy);
	}
	CMRISeries *pMRISeries = m_pCryotherapyDlg->GetMRISeries(sMRIName);
	if (pMRISeries)
	{
		int iWndIndex = m_pCryotherapyDlg->m_pDisplay4Dlg->GetWndIndex(this);
		m_pCryotherapyDlg->DisplayMRISeriesToWindow(pMRISeries, iWndIndex);
	}
}

void CDisplayDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDisplayDlg::SelectDisplayMRISeries(BOOL bSelected)
{
	if (m_bSelected != bSelected)
	{
		m_bSelected = bSelected;
		m_stSel.bShow = bSelected;
		Invalidate(FALSE);
		UpdateWindow();
	}
	if (m_bSelected)
	{
		if (m_pCurDispMRIRect && m_pCurDispMRIRect->m_pMRIRectSeries)
		{
			m_pCryotherapyDlg->m_pCurSelSeries = m_pCurDispMRIRect->m_pMRIRectSeries->GetMRISeries();
		}
	}
	else
	{
		if (m_pCurDispMRIRect && m_pCurDispMRIRect->m_pMRIRectSeries)
		{
			m_pCryotherapyDlg->m_pCurSelSeries = NULL;
		}
	}
}

void CDisplayDlg::OnPopupSelect() 
{
	// TODO: Add your command handler code here
	if (m_bSelected)
	{
		m_bSelected = FALSE;
		m_stSel.bShow = FALSE;
		m_pCryotherapyDlg->m_pCurSelSeries = NULL;
		Invalidate(FALSE);
		UpdateWindow();
	}
	else
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->SetSelectedWnd(this);	
	}
}

void CDisplayDlg::OnPopupTargetSel() 
{
	// TODO: Add your command handler code here
	if (m_pRightClickTarget)
	{
		m_pRightClickTarget->SetSelected(!m_pRightClickTarget->IsSelected());
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDisplay.UpdateAllProjectVTarget();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap1.m_dlgDisplay.m_Scene.DrawScene();
	}
}

void CDisplayDlg::OnPopupTargetDel() 
{
	// TODO: Add your command handler code here
	if (m_pRightClickTarget)
	{
		m_pCryotherapyDlg->m_TargetPool.DeleteATarget(m_pRightClickTarget);
		m_pRightClickTarget = NULL;
	}
}

BOOL CDisplayDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//An OpenGL window must be created with the following flag 
	// and must not include CS_PARENTIDC for the class style.
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	
	return CDialog::PreCreateWindow(cs);
}

void CDisplayDlg::OnDebugViewToProbe() 
{
	// TODO: Add your command handler code here
	//	CProbe* pProbe = m_pCryotherapyDlg->m_dlgTabCry.m_dlgSgcPlan.m_pCurRealProbe;
	//	if (pProbe)
	//	{
	//		m_Scene.ChangeViewToProbe(pProbe);
	//		Invalidate(FALSE);
	//		UpdateWindow();
	//	}
}

void CDisplayDlg::OnSetupLegend() 
{
	CDlgSetupLegend dlg;

	POSITION pos;
	dlg.lstCTLeftTop.RemoveAll();
	pos=lstCTLeftTop.GetHeadPosition();
	while(pos)
	{
		dlg.lstCTLeftTop.AddTail(lstCTLeftTop.GetNext(pos));
	}
	dlg.lstCTLeftBottom.RemoveAll();
	pos=lstCTLeftBottom.GetHeadPosition();
	while(pos)
	{
		dlg.lstCTLeftBottom.AddTail(lstCTLeftBottom.GetNext(pos));
	}
	dlg.lstCTRightTop.RemoveAll();
	pos=lstCTRightTop.GetHeadPosition();
	while(pos)
	{
		dlg.lstCTRightTop.AddTail(lstCTRightTop.GetNext(pos));
	}
	dlg.lstCTRightBottom.RemoveAll();
	pos=lstCTRightBottom.GetHeadPosition();
	while(pos)
	{
		dlg.lstCTRightBottom.AddTail(lstCTRightBottom.GetNext(pos));
	}
	dlg.lstMRLeftTop.RemoveAll();
	pos=lstMRLeftTop.GetHeadPosition();
	while(pos)
	{
		dlg.lstMRLeftTop.AddTail(lstMRLeftTop.GetNext(pos));
	}
	dlg.lstMRLeftBottom.RemoveAll();
	pos=lstMRLeftBottom.GetHeadPosition();
	while(pos)
	{
		dlg.lstMRLeftBottom.AddTail(lstMRLeftBottom.GetNext(pos));
	}
	dlg.lstMRRightTop.RemoveAll();
	pos=lstMRRightTop.GetHeadPosition();
	while(pos)
	{
		dlg.lstMRRightTop.AddTail(lstMRRightTop.GetNext(pos));
	}
	dlg.lstMRRightBottom.RemoveAll();
	pos=lstMRRightBottom.GetHeadPosition();
	while(pos)
	{
		dlg.lstMRRightBottom.AddTail(lstMRRightBottom.GetNext(pos));
	}

	if (dlg.DoModal() != IDOK)
		return;

	lstCTLeftTop.RemoveAll();
	pos=dlg.lstCTLeftTop.GetHeadPosition();
	while(pos)
	{
		lstCTLeftTop.AddTail(dlg.lstCTLeftTop.GetNext(pos));
	}
	lstCTLeftBottom.RemoveAll();
	pos=dlg.lstCTLeftBottom.GetHeadPosition();
	while(pos)
	{
		lstCTLeftBottom.AddTail(dlg.lstCTLeftBottom.GetNext(pos));
	}
	lstCTRightTop.RemoveAll();
	pos=dlg.lstCTRightTop.GetHeadPosition();
	while(pos)
	{
		lstCTRightTop.AddTail(dlg.lstCTRightTop.GetNext(pos));
	}
	lstCTRightBottom.RemoveAll();
	pos=dlg.lstCTRightBottom.GetHeadPosition();
	while(pos)
	{
		lstCTRightBottom.AddTail(dlg.lstCTRightBottom.GetNext(pos));
	}
	lstMRLeftTop.RemoveAll();
	pos=dlg.lstMRLeftTop.GetHeadPosition();
	while(pos)
	{
		lstMRLeftTop.AddTail(dlg.lstMRLeftTop.GetNext(pos));
	}
	lstMRLeftBottom.RemoveAll();
	pos=dlg.lstMRLeftBottom.GetHeadPosition();
	while(pos)
	{
		lstMRLeftBottom.AddTail(dlg.lstMRLeftBottom.GetNext(pos));
	}
	lstMRRightTop.RemoveAll();
	pos=dlg.lstMRRightTop.GetHeadPosition();
	while(pos)
	{
		lstMRRightTop.AddTail(dlg.lstMRRightTop.GetNext(pos));
	}
	lstMRRightBottom.RemoveAll();
	pos=dlg.lstMRRightBottom.GetHeadPosition();
	while(pos)
	{
		lstMRRightBottom.AddTail(dlg.lstMRRightBottom.GetNext(pos));
	}

	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.
		m_dlgDispControl.m_pMRIRectSeries)
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.
			m_pDlgDisplay->AddMRIRectLegend(m_pCryotherapyDlg->
			m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.
			m_pMRIRectSeries->GetDispMRIRect()->GetMRIImg());
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.
			m_pDlgDisplay->m_Scene.DrawScene();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.
		m_dlgDispControl.m_pMRIRectSeries)
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.
			m_pDlgDisplay->AddMRIRectLegend(m_pCryotherapyDlg->
			m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.
			m_pMRIRectSeries->GetDispMRIRect()->GetMRIImg());
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.
			m_pDlgDisplay->m_Scene.DrawScene();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.
		m_dlgDispControl.m_pMRIRectSeries)
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.
			m_pDlgDisplay->AddMRIRectLegend(m_pCryotherapyDlg->
			m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.
			m_pMRIRectSeries->GetDispMRIRect()->GetMRIImg());
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.
			m_pDlgDisplay->m_Scene.DrawScene();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.
		m_dlgDispControl.m_pMRIRectSeries)
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.
			m_pDlgDisplay->AddMRIRectLegend(m_pCryotherapyDlg->
			m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.
			m_pMRIRectSeries->GetDispMRIRect()->GetMRIImg());
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.
			m_pDlgDisplay->m_Scene.DrawScene();
	}
}

BOOL CDisplayDlg::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;

	CRect rs;
	GetClientRect(&rs);
	pDC->FillSolidRect(&rs,RGB(0,0,0));
	return TRUE;
}

BOOL CDisplayDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDisplayDlg::OnKeyUp(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
		
	//使用左右上下键切换当前图片
	if(!m_b3DDisplay)
	{
		m_pDisplayControl->OnKeyUp(nChar);
		
	}
	
}

void CDisplayDlg::OnKeyDown(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default
		
	//使用左右上下键切换当前图片
	if(!m_b3DDisplay)
	{
		m_pDisplayControl->OnKeyDown(nChar);
		
	}
	
}

BOOL CDisplayDlg::PreTranslateMessage(MSG* pMsg) 
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
		else
		{
			OnKeyUp(pMsg->wParam);
		}
    }
	return CDialog::PreTranslateMessage(pMsg);
}


void CDisplayDlg::EnableDraw(BOOL bEnable)
{
	m_bEnableDraw=bEnable;
}

BOOL CDisplayDlg::IsEnableDraw()
{
	//切换图像时，不需要刷新针位置，否则会闪烁
	return m_bEnableDraw && (!m_bLRArrow && !m_bEnableMouseMove);
}


BOOL CDisplayDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	while(fabs(zDelta)>80)
	{
		zDelta/=1.2;
	}
	nFlags = MK_SHIFT;

	Vector3D vEyePoint,vGazePoint,vUp,vFront;
	double dbDistance;
	if(!m_b3DDisplay)
	{
		m_Scene.GetLookAtPar(vEyePoint, vGazePoint, vUp, vFront, dbDistance);
	
		if(dbDistance<100 && zDelta<0)
		{
			m_Scene.MouseMove( nFlags, 0-zDelta, 0-zDelta );
		}else if(dbDistance>800 && zDelta>0)
		{
			m_Scene.MouseMove( nFlags, 0-zDelta, 0-zDelta );
		}else if(dbDistance>100 && dbDistance <800)
		{
			m_Scene.MouseMove( nFlags, 0-zDelta, 0-zDelta );
		}
		Invalidate( FALSE );
		UpdateWindow();
		ResetRuler();
	}else
	{
		m_Scene.MouseMove( nFlags, 0-zDelta, 0-zDelta );
	}
		
	
	Invalidate( FALSE );
	UpdateWindow();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

CString CDisplayDlg::GetLegend(CMRIImage *pImage,ENUM_IMG_SOURCE legendType, int index)
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
			sVersion = CGlobalFuncs::GetProductVersion(NULL);
			sOut.Format("Ver %s", sVersion);
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
			sVersion = CGlobalFuncs::GetProductVersion(NULL);
			sOut.Format("Ver %s", sVersion);
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

void CDisplayDlg::GetIndex(CString sIndex, CList<int,int> *m_pList)
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

void CDisplayDlg::OnWindowLevel_PF()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;

	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(150, 35+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_BRAIN()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(70, 35+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_IAC()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(4095, 600+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_BONE()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(2000, 800+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_LUNG()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(1600, -600+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_ABDOMEN()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(350, 60+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_LIVERA()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(200, 100+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_LIVERB()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(220, 25+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_SPINE()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(300, 60+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnWindowLevel_FAT()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(270, -35+1024);
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}
void CDisplayDlg::OnResetWindowLevel()
{
	if(!(m_pDispWrap->m_pDispSeries))
		return;
	if(m_pDispWrap->m_pDispSeries->GetMRISeries()->
		GetNumOfImages()<=0)
		return;

	int iWindow,iLevel;
	/*
	iWindow=m_pDispWrap->m_pDispSeries->GetMRISeries()->GetImageAt(0)->
		GetInterWindowWidth();
	iLevel=m_pDispWrap->m_pDispSeries->GetMRISeries()->GetImageAt(0)->
		GetInterWindowLevel();*/
	int iDispIndex= m_pDispWrap->m_pDispSeries->GetDispIndex();
	IMG_DATA_TYPE* pData=m_pDispWrap->m_pDispSeries->GetMRISeries()->GetImageAt(iDispIndex)->GetColorOfRawData();
	int iMin=9999,iMax=0;
	for(int i=0;i<m_pDispWrap->m_pDispSeries->GetMRISeries()->GetImageAt(iDispIndex)->GetNumOfPts();i++)
	{
		if(iMin>pData[i])
			iMin=pData[i];
		if(iMax<pData[i])
			iMax=pData[i];
	}

	iWindow = iMax-iMin;
	if(iWindow<=0)
		iWindow = 2;
	iLevel = iMin + iWindow/2;
	
	//更新序列的窗宽窗位
	m_pDispWrap->m_pDispSeries->GetMRISeries()->
		SetWindowWidthAndLevel(iWindow, iLevel);

	//更新3D窗口中的图像
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.m_pMRIRectSeries &&
		(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
		m_pDispWrap->m_pDispSeries->GetMRISeries()))
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.HideCurrentIn3D();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.AddCurrentTo3D();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.m_pMRIRectSeries &&
		(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
		m_pDispWrap->m_pDispSeries->GetMRISeries()))
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.HideCurrentIn3D();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.AddCurrentTo3D();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.m_pMRIRectSeries &&
		(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
		m_pDispWrap->m_pDispSeries->GetMRISeries()))
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.HideCurrentIn3D();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.AddCurrentTo3D();
	}
	if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.m_pMRIRectSeries &&
		(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
		m_pDispWrap->m_pDispSeries->GetMRISeries()))
	{
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.HideCurrentIn3D();
		m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.AddCurrentTo3D();
	}

	//更新2D窗口中的图像
	m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
		UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
}

void CDisplayDlg::ResetRuler()
{
	if(!m_pCurDispMRIRect || m_b3DDisplay)
	{
		m_Scene.m_ruler.m_bVisible=FALSE;
		return;
	}

	CRect rc;
	GetClientRect(&rc);
	int cx=rc.Width();
	int cy=rc.Height();

	m_Scene.m_ruler.m_iSeprateCount=10;
	m_Scene.m_ruler.m_bVisible=m_bShowLegend;
	m_Scene.m_ruler.m_pnt1.x=cx-5;
	m_Scene.m_ruler.m_pnt1.y=85;
	m_Scene.m_ruler.m_pnt2.x=cx-5;
	m_Scene.m_ruler.m_pnt2.y=cy-80;

	Line3D lLine;
	m_Scene.Click( (int) m_Scene.m_ruler.m_pnt1.x, 
			       (int)m_Scene.m_ruler.m_pnt1.y, &lLine );
	double *ptOnImage=m_pCurDispMRIRect->GetMRIImg()->GetCenter();
	double *dir=m_pCurDispMRIRect->GetMRIImg()->GetSliceDirection();
	double dis=dir[0]*ptOnImage[0]+dir[1]*ptOnImage[1]+dir[2]*ptOnImage[2];

	Vector3D vNormal(dir[0],dir[1],dir[2]);
	CPlane plane(vNormal,dis);
	//TRACE("%.2f,%.2f,%.2f,%.2f",dir[0],dir[1],dir[2],-dis);
	CIntersection intersectionOper;
	Vector3D Pt1;
	if ( ERR_OK != intersectionOper.Intersect( &lLine, &plane, Pt1 ) )
	{
		m_Scene.m_ruler.m_bVisible=FALSE;
		return;
	}
	//TRACE("=%.2f,%.2f,%.2f\n",Pt1.x,Pt1.y,Pt1.z);

	m_Scene.Click( (int) m_Scene.m_ruler.m_pnt2.x, 
			       (int)m_Scene.m_ruler.m_pnt2.y, &lLine );
	Vector3D Pt2;
	if ( ERR_OK != intersectionOper.Intersect( &lLine, &plane, Pt2 ) )
	{
		m_Scene.m_ruler.m_bVisible=FALSE;
		return;
	}

	dis=sqrt((Pt1.x-Pt2.x)*(Pt1.x-Pt2.x)+(Pt1.y-Pt2.y)*(Pt1.y-Pt2.y)+
		    (Pt1.z-Pt2.z)*(Pt1.z-Pt2.z));

	double dMMperPixel=dis/(m_Scene.m_ruler.m_pnt2.y-m_Scene.m_ruler.m_pnt1.y);
	int iRealDisOnCm=int(dis/10.0);
	m_Scene.m_ruler.m_pnt2.y=m_Scene.m_ruler.m_pnt1.y+iRealDisOnCm*10.0/dMMperPixel;
	dis=iRealDisOnCm;
	m_Scene.m_ruler.m_iSeprateCount=iRealDisOnCm;

	m_Scene.m_ruler.m_sTitle.Format("%.0fCM",dis);
	m_Scene.m_ruler.m_sTitle.Format("10mm/div");

}

void CDisplayDlg::OnSpopupColorImage() 
{
	// TODO: Add your command handler code here
	if (m_pDispWrap->m_pDispSeries)
	{
		int index=m_pDisplayControl->m_iCurDispIndex;
		
			
		if(m_pDispWrap->m_pDispSeries->GetMRISeries()->m_oClut32.IsGreyColor())
		{
			m_pDispWrap->m_pDispSeries->GetMRISeries()->m_oClut32.SetGreyColor(FALSE);
		}else
		{
			m_pDispWrap->m_pDispSeries->GetMRISeries()->m_oClut32.SetGreyColor(TRUE);
		}

		//更新3D窗口中的图像
		if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.m_pMRIRectSeries &&
			(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
			m_pDispWrap->m_pDispSeries->GetMRISeries()))
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.HideCurrentIn3D();
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap2.m_dlgDispControl.AddCurrentTo3D();
		}
		if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.m_pMRIRectSeries &&
			(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
			m_pDispWrap->m_pDispSeries->GetMRISeries()))
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.HideCurrentIn3D();
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap3.m_dlgDispControl.AddCurrentTo3D();
		}
		if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.m_pMRIRectSeries &&
			(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
			m_pDispWrap->m_pDispSeries->GetMRISeries()))
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.HideCurrentIn3D();
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap4.m_dlgDispControl.AddCurrentTo3D();
		}
		if(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.m_pMRIRectSeries &&
			(m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.m_pMRIRectSeries->GetMRISeries()==
			m_pDispWrap->m_pDispSeries->GetMRISeries()))
		{
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.HideCurrentIn3D();
			m_pCryotherapyDlg->m_pDisplay4Dlg->m_DlgDispWrap5.m_dlgDispControl.AddCurrentTo3D();
		}
		
		//更新2D窗口中的图像
		m_pDispWrap->m_dlgDisplay.m_pCryotherapyDlg->
			UpdateTextureSameSeries(m_pDispWrap->m_pDispSeries,FALSE);
		//m_pDisplayControl->ShowImage(index);
	}
}
