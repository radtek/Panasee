// DlgNavigate.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgNavigate.h"
#include "DlgSelectSeries.h"
#include "ImageReslice.h"
#include "DlgMain.h"
#include "TrackTool.h"
#include "pgmath.h"
#include "DcmInt.h"
#include "config.h"
#include "ConfigMgr.h"
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNavigate dialog
#define MIN_PUSH_DEPTH (-200)
#define MAX_PUSH_DEPTH (200)

CDlgNavigate::CDlgNavigate(CWnd* pParent /*=NULL*/)
	: CDlgNavigateSkin(CDlgNavigate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNavigate)
	m_bSameWWWL = FALSE;
	m_bShowFusion = FALSE;
	m_bCurTipType = 0;
	//}}AFX_DATA_INIT

	m_bFreezing=FALSE;
	m_bIsPausing=FALSE;

	m_sSeriesName=m_sFusionSeriesName="";
	m_bTracking=FALSE;

	m_nOffset=1;
	m_bNoRefreshInScroll=FALSE;
	m_pCurRealProbe=NULL;
	m_iPushDepth=0;

	m_dPreImageProbeTip[0]=m_dPreImageProbeTip[1]=m_dPreImageProbeTip[2]=0;

	m_iLookDepth=0;
	m_iVirtualUSAngle=0;
	
	m_ppcOrientation = new char*[4];
	for (int i = 0; i < 4; i++)
	{		
		m_ppcOrientation[i] = new char[10];
	}

	m_viewMode=m_newViewMode=TVIEW_TAPV;

	m_dStandardProbeLength=100;

}

CDlgNavigate::~CDlgNavigate()
{
	
	if ( NULL != m_ppcOrientation )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( NULL != m_ppcOrientation[i] )
			{
				delete []m_ppcOrientation[i];
				m_ppcOrientation[i] = NULL;
			}	
		}
		delete [] m_ppcOrientation;
		m_ppcOrientation = NULL;
	}
}


void CDlgNavigate::DoDataExchange(CDataExchange* pDX)
{
	CDlgNavigateSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNavigate)
	DDX_Control(pDX, IDC_BUTTON_FREEZE, m_btnFreeze);
	DDX_Control(pDX, IDC_STATIC_PROBE, m_staticProbe);
	DDX_Control(pDX, IDC_CHECK_SHOW_FUSION, m_chkShowFusion);
	DDX_Control(pDX, IDC_BUTTON_SELECT_FUSION, m_btnSelectFusion);
	DDX_Control(pDX, IDC_BUTTON_VIEW_AV, m_btnView4);
	DDX_Control(pDX, IDC_BUTTON_VIEW_AST, m_btnView3);
	DDX_Control(pDX, IDC_BUTTON_VIEW_ASVP, m_btnView2);
	DDX_Control(pDX, IDC_BUTTON_VIEW_TAPV, m_btnView1);
	DDX_Control(pDX, IDC_CHECK_SAME_WWWL, m_chkSameWL);
	DDX_Control(pDX, IDC_STATIC_OFFSET, m_lblLengthUnit);
	DDX_Control(pDX, IDC_EDIT_OFFSET, m_edtOffset);
	DDX_Control(pDX, IDC_BTN_SUB_OFFSET, m_btnSubOffset);
	DDX_Control(pDX, IDC_BTN_ADD_OFFSET, m_btnAddOffset);
	DDX_Control(pDX, IDC_BTN_SCROLL_OFFSET, m_btnScrollOffset);
	DDX_Control(pDX, IDC_BUTTON_TRACKING, m_btnTracking);
	DDX_Control(pDX, IDC_BUTTON_SERIES, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_CT_CALIBRATE, m_btnCalibrateImage);
	DDX_Check(pDX, IDC_CHECK_SAME_WWWL, m_bSameWWWL);	
	DDX_Check(pDX, IDC_CHECK_SHOW_FUSION, m_bShowFusion);
	DDX_Radio(pDX, IDC_RADIO_REALTIP, m_bCurTipType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNavigate, CDlgNavigateSkin)
	//{{AFX_MSG_MAP(CDlgNavigate)
	ON_BN_CLICKED(IDC_BUTTON_SERIES, OnButtonSeries)
	ON_BN_CLICKED(IDC_CHECK_SAME_WWWL, OnCheckSameWwwl)
	ON_BN_CLICKED(IDC_BUTTON_CT_CALIBRATE, OnButtonCtCalibrate)
	ON_BN_CLICKED(IDC_BUTTON_TRACKING, OnButtonTracking)
	ON_BN_CLICKED(IDC_BTN_ADD_OFFSET, OnBtnAddOffset)
	ON_BN_CLICKED(IDC_BTN_SUB_OFFSET, OnBtnSubOffset)
	ON_MESSAGE(WM_PANA_MOVECTRL, OnMoveCtrl)
	ON_MESSAGE(WM_PANA_MOVECTRL_LBTN_UP,OnMoveCtrl_LUP)
	ON_MESSAGE(WM_PANA_MOVECTRL_LBTN_DOWN,OnMoveCtrl_LDOWN)
	ON_MESSAGE(WM_SYS_UPDATE_DEPTH,OnUpdateDepth)
	ON_MESSAGE(WM_SYS_UPDATE_ANGLE,OnUpdateAngle)
	ON_MESSAGE(WM_SYS_DEPTHSLIDER_SHOW,OnDepthSliderShow)
	ON_MESSAGE(WM_SYS_DEPTHSLIDER_HIDE,OnDepthSliderHide)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_AV, OnButtonViewAv)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_ASVP, OnButtonViewAsvp)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_AST, OnButtonViewAst)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_TAPV, OnButtonViewTapv)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FUSION, OnButtonSelectFusion)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FUSION, OnCheckShowFusion)
	ON_BN_CLICKED(IDC_BUTTON_FREEZE, OnButtonFreeze)
	ON_BN_CLICKED(IDC_RADIO_REALTIP, OnRadioRealtip)
	ON_BN_CLICKED(IDC_RADIO_VIRTUALTIP, OnRadioVirtualtip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNavigate message handlers
LONG CDlgNavigate::OnDepthSliderShow(WPARAM w,LPARAM l)
{
	return 0;
	CDlgMain *pDlgMain = g_DlgMain;	
	for ( int i = 0; i < pDlgMain->m_dlgCryotherapy.m_iMaxNumOfTrackTool; i ++ )
	{
		if (pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i] &&
			pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_TrackType == TT_NEEDLE)
		{
			pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe->SetExtendLine(m_iLookDepth);
			break;
		}
	}


	return 0;
}

LONG CDlgNavigate::OnDepthSliderHide(WPARAM w,LPARAM l)
{
	return 0;
	CDlgMain *pDlgMain = g_DlgMain;	
	for ( int i = 0; i < pDlgMain->m_dlgCryotherapy.m_iMaxNumOfTrackTool; i ++ )
	{
		if (pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i] &&
			pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_TrackType == TT_NEEDLE)
		{
			pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe->SetExtendLine(EXTEND_LENGTH);
			break;
		}
	}

	return 0;
}

LONG CDlgNavigate::OnUpdateAngle(WPARAM w,LPARAM l)
{
	int *p=(int*)w;
	m_iVirtualUSAngle=*p;

	return 0;
}

LONG CDlgNavigate::OnUpdateDepth(WPARAM w,LPARAM l)
{
	return 0;
	int *p=(int*)w;
	m_iLookDepth=*p;

	return 0;
}

LONG CDlgNavigate::OnMoveCtrl_LDOWN(WPARAM w,LPARAM l)
{
	m_bIsPausing=TRUE;
	return 0;
}

LONG CDlgNavigate::OnMoveCtrl_LUP(WPARAM w,LPARAM l)
{
	
	m_bIsPausing=FALSE;
	return 0;
}

LONG CDlgNavigate::OnMoveCtrl(WPARAM w,LPARAM l)
{
	CDlgNavigate::OnScrollPushDepth(w,l);

	if(m_bCurTipType==0) //real tip
	{
	m_iPushDepth = GetPushDepth();
	if(m_pCurRealProbe)
	{
		Vector3D vTip(m_dPreImageProbeTip[0],m_dPreImageProbeTip[1],m_dPreImageProbeTip[2]);
			Vector3D vOri=m_pCurRealProbe->GetOrientationInProbe();
		Vector3D vNewTip=vTip + vOri*m_iPushDepth;
			m_pCurRealProbe->SetTipInProbe(vNewTip.x,vNewTip.y,vNewTip.z);
			
			CString str;
			double dLength = m_dStandardProbeLength+m_iPushDepth;
			str.Format("%.1f",dLength);
			m_edtOffset.SetWindowText(str);
		}
	}else
	{
		double value = GetPushDepth();
		if(m_pCurRealProbe)
		{
			m_iLookDepth=value-MIN_PUSH_DEPTH;
			m_pCurRealProbe->SetOffset(m_iLookDepth);
			
			CString str;
			str.Format("%i",m_iLookDepth);
			m_edtOffset.SetWindowText(str);	
		}
	}
	if(!m_bTracking && m_pCurRealProbe)
	{
		CDlgMain *pDlgMain = g_DlgMain;
		pDlgMain->m_display4Dlg.Refresh(); 
	}

	return 0;
}

//计算在2D显示时图像应该如何旋转和反转
//才能满足以下显示条件
/*
  A         S         S
R   L     R   L     A   P
  P         I         I
*/
void CDlgNavigate::CorrectImageDispOrientation()
{
	char** sOrientChar;
	sOrientChar = m_ppcOrientation;
	BOOL bA = FALSE;
	BOOL bS = FALSE;
	BOOL bR = FALSE;
	int iSPos, iIPos, iLPos, iRPos, iAPos, iPPos;
	for ( int j = 0; j < 4; j++ )
	{
		if (sOrientChar[j][0] == 'A')
		{
			bA = TRUE;
			iAPos = j;
		}
		if (sOrientChar[j][0] == 'P')
		{
			iPPos = j;
		}
		if (sOrientChar[j][0] == 'S')
		{
			bS = TRUE;
			iSPos = j;
		}
		if (sOrientChar[j][0] == 'I')
		{
			iIPos = j;
		}
		if (sOrientChar[j][0] == 'R')
		{
			bR = TRUE;
			iRPos = j;
		}
		if (sOrientChar[j][0] == 'L')
		{
			iLPos = j;
		}
	}

	BOOL bHFlip = FALSE;
	BOOL bVFlip = FALSE;
	double dbAngle = 0;
	if (bR && bS)
	{
		if ( (iRPos + 1)%4 != iSPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iLPos - 1) * (bHFlip ? -1 : 1);
	}
	if (bA && bR)
	{
		if ( (iRPos + 1)%4 != iAPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iLPos - 1) * (bHFlip ? -1 : 1);
	}
	else if (bA && bS)
	{
		if ( (iAPos + 1)%4 != iSPos)
		{
			bVFlip = TRUE;
		}
		dbAngle = PI / 2 * (iPPos - 1) * (bHFlip ? -1 : 1);
	}
	

	/*计算频率编码方向
	  缺省编码方向是从左到右
	
	fdLeftToRight,		// →
	fdTopToBottom,		// ↓
	fdRightToLeft,		// ←
	fdBottomToTop,		// ↑
	*/
	ENUM_CODING_DIR_ON_2D_IMAGE bLeftRight=fdLeftToRight;

	// change orientation char
	for (int i=0; i<4; i++)
	{
		m_sDispOri[i] = CString(m_ppcOrientation[i]);
	}
	CString sTmp;
	/*两个double型变量做比较，不能直接使用==操作符*/
	if (fabs(dbAngle-PI/2)<TOLER || fabs(dbAngle+PI*3/2)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[2];
		m_sDispOri[2] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		bLeftRight=fdBottomToTop;
		
	}
	else if (fabs(dbAngle-PI)<TOLER || fabs(dbAngle+PI)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[2];
		m_sDispOri[2] = sTmp;
		sTmp = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		bLeftRight=fdRightToLeft;
	}
	else if (fabs(dbAngle-PI*3/2)<TOLER || fabs(dbAngle+PI/2)<TOLER)
	{
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[3];
		m_sDispOri[3] = m_sDispOri[2];
		m_sDispOri[2] = m_sDispOri[1];
		m_sDispOri[1] = sTmp;
		bLeftRight=fdTopToBottom;
	}

	if (bHFlip)
	{
		CString sTmp;
		sTmp = m_sDispOri[1];
		m_sDispOri[1] = m_sDispOri[3];
		m_sDispOri[3] = sTmp;
		if(bLeftRight == fdRightToLeft)
		{
			bLeftRight = fdLeftToRight;
		}else if(bLeftRight == fdLeftToRight)
		{
			bLeftRight = fdRightToLeft;
		}
	}
	if (bVFlip)
	{
		CString sTmp;
		sTmp = m_sDispOri[0];
		m_sDispOri[0] = m_sDispOri[2];
		m_sDispOri[2] = sTmp;
		if(bLeftRight == fdBottomToTop)
		{
			bLeftRight = fdTopToBottom;
		}else if(bLeftRight == fdTopToBottom)
		{
			bLeftRight = fdBottomToTop;
		}
	}
}

void CDlgNavigate::ComputeOrientChar(US_STRUCT_IMAGE *pImage)
{
	// Compute box orient char
	char sOrient[6];	
	
	if ( beHeadFirst == pImage->oHeader.studyField.PatEntry )
	{
		switch( pImage->oHeader.studyField.PatPosition )
		{
		case bpSupine:
			{
				sOrient[0] = 'L';
				sOrient[1] = 'R';
				sOrient[2] = 'S';
				sOrient[3] = 'I';
				sOrient[4] = 'A';
				sOrient[5] = 'P';
			}
			break;
		case bpProne:
			{
				sOrient[0] = 'R';
				sOrient[1] = 'L';
				sOrient[2] = 'S';
				sOrient[3] = 'I';
				sOrient[4] = 'P';
				sOrient[5] = 'A';
			}
			break;
		case bpDecubitusRight:
			{
				sOrient[0] = 'P';
				sOrient[1] = 'A';
				sOrient[2] = 'S';
				sOrient[3] = 'I';
				sOrient[4] = 'L';
				sOrient[5] = 'R';
			}
			break;
		case bpDecubitusLeft:
			{
				sOrient[0] = 'A';
				sOrient[1] = 'P';
				sOrient[2] = 'S';
				sOrient[3] = 'I';
				sOrient[4] = 'R';
				sOrient[5] = 'L';
			}
			break;
		default:
			break;
		}
	} 
	else
	{
		switch( pImage->oHeader.studyField.PatPosition )
		{
		case bpSupine:
			{
				sOrient[0] = 'R';
				sOrient[1] = 'L';
				sOrient[2] = 'I';
				sOrient[3] = 'S';
				sOrient[4] = 'A';
				sOrient[5] = 'P';
			}
			break;
		case bpProne:
			{
				sOrient[0] = 'L';
				sOrient[1] = 'R';
				sOrient[2] = 'I';
				sOrient[3] = 'S';
				sOrient[4] = 'P';
				sOrient[5] = 'A';
			}
			break;
		case bpDecubitusRight:
			{
				sOrient[0] = 'A';
				sOrient[1] = 'P';
				sOrient[2] = 'I';
				sOrient[3] = 'S';
				sOrient[4] = 'L';
				sOrient[5] = 'R';
			}
			break;
		case bpDecubitusLeft:
			{
				sOrient[0] = 'P';
				sOrient[1] = 'A';
				sOrient[2] = 'I';
				sOrient[3] = 'S';
				sOrient[4] = 'R';
				sOrient[5] = 'L';
			}
			break;
		default:
			break;
		}
	}
		
	// End of compute box orient char
	double dFreqDirection[3],dPhaseDirection[3],dCenter[3],dFOV[3];
	// Freq direction
	dFreqDirection[0] = (double) pImage->oHeader.scanField.FreqDirX;
	dFreqDirection[1] = (double) pImage->oHeader.scanField.FreqDirY;
	dFreqDirection[2] = (double) pImage->oHeader.scanField.FreqDirZ;

	// Phase direction
	dPhaseDirection[0] = (double) pImage->oHeader.scanField.PhaseDirX;
	dPhaseDirection[1] = (double) pImage->oHeader.scanField.PhaseDirY;
	dPhaseDirection[2] = (double) pImage->oHeader.scanField.PhaseDirZ;

	// Center
	dCenter[0] = (double) pImage->oHeader.imageField.fCenterPointX;	
	dCenter[1] = (double) pImage->oHeader.imageField.fCenterPointY;	
	dCenter[2] = (double) pImage->oHeader.imageField.fCenterPointZ;	

	// Field of view
	dFOV[0] = (double) pImage->oHeader.seriesField.FovX;
	dFOV[1] = (double) pImage->oHeader.seriesField.FovY;
	dFOV[2] = (double) pImage->oHeader.seriesField.FovZ;
		
	// Calculate the char corresponding to the edge
	Vector3D vAB( dFreqDirection[0], dFreqDirection[1], dFreqDirection[2] );
	Vector3D vAD( dPhaseDirection[0], dPhaseDirection[1], dPhaseDirection[2] );
	
	// Edge 0, 2
	Vector3D vEdgeDir = -vAD;
	double dFactor[3], dAbsFactor[3], dTmpFactor;
	int nAxis[3] = { 0, 1, 2 }, nTmpAxis;
	dFactor[0] = vEdgeDir.x;// * vXAxis;
	if (fabs(dFactor[0]) < TOLER)
	{
		dFactor[0] = 0.0;
	}
	dFactor[1] = vEdgeDir.y;// * vYAxis;
	if (fabs(dFactor[1]) < TOLER)
	{
		dFactor[1] = 0.0;
	}
	dFactor[2] = vEdgeDir.z;// * vZAxis;
	if (fabs(dFactor[2]) < TOLER)
	{
		dFactor[2] = 0.0;
	}
	dAbsFactor[0] = fabs(dFactor[0]);
	dAbsFactor[1] = fabs(dFactor[1]);
	dAbsFactor[2] = fabs(dFactor[2]);

	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	} 
	if (dAbsFactor[1] < dAbsFactor[2])
	{
		dTmpFactor = dAbsFactor[1];
		dAbsFactor[1] = dAbsFactor[2];
		dAbsFactor[2] = dTmpFactor;
		nTmpAxis = nAxis[1];
		nAxis[1] = nAxis[2];
		nAxis[2] = nTmpAxis;
	}
	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	}

	char* pcBoxChar = NULL;
	pcBoxChar=sOrient;
	
	// char cOrient[4][10];
	if (dFactor[nAxis[0]] > 0)
	{
		sprintf( m_ppcOrientation[0], "%c", pcBoxChar[2*nAxis[0]] );
		sprintf( m_ppcOrientation[2], "%c", pcBoxChar[2*nAxis[0]+1] );
	} 
	else if (dFactor[nAxis[0]] < 0)
	{
		sprintf( m_ppcOrientation[0], "%c", pcBoxChar[2*nAxis[0]+1] );
		sprintf( m_ppcOrientation[2], "%c", pcBoxChar[2*nAxis[0]] );
	}
	if (dFactor[nAxis[1]] > 0)
	{
		sprintf( &m_ppcOrientation[0][1], "%c", pcBoxChar[2*nAxis[1]] );
		sprintf( &m_ppcOrientation[2][1], "%c", pcBoxChar[2*nAxis[1]+1] );
	} 
	else if (dFactor[nAxis[1]] < 0)
	{
		sprintf( &m_ppcOrientation[0][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		sprintf( &m_ppcOrientation[2][1], "%c", pcBoxChar[2*nAxis[1]] );
	}
	if (dFactor[nAxis[2]] > 0)
	{
		sprintf( &m_ppcOrientation[0][2], "%c", pcBoxChar[2*nAxis[2]] );
		sprintf( &m_ppcOrientation[2][2], "%c", pcBoxChar[2*nAxis[2]+1] );
	} 
	else if (dFactor[nAxis[2]] < 0)
	{
		sprintf( &m_ppcOrientation[0][2], "%c", pcBoxChar[2*nAxis[2]+1] );
		sprintf( &m_ppcOrientation[2][2], "%c", pcBoxChar[2*nAxis[2]] );
	}

	// Edge 1,3
	nAxis[0] = 0;
	nAxis[1] = 1;
	nAxis[2] = 2;
	vEdgeDir = vAB;
	dFactor[0] = vEdgeDir.x;// * vXAxis;
	if (fabs(dFactor[0]) < TOLER)
	{
		dFactor[0] = 0.0;
	}
	dFactor[1] = vEdgeDir.y;// * vYAxis;
	if (fabs(dFactor[1]) < TOLER)
	{
		dFactor[1] = 0.0;
	}
	dFactor[2] = vEdgeDir.z;// * vZAxis;
	if (fabs(dFactor[2]) < TOLER)
	{
		dFactor[2] = 0.0;
	}
	dAbsFactor[0] = fabs(dFactor[0]);
	dAbsFactor[1] = fabs(dFactor[1]);
	dAbsFactor[2] = fabs(dFactor[2]);

	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	} 
	if (dAbsFactor[1] < dAbsFactor[2])
	{
		dTmpFactor = dAbsFactor[1];
		dAbsFactor[1] = dAbsFactor[2];
		dAbsFactor[2] = dTmpFactor;
		nTmpAxis = nAxis[1];
		nAxis[1] = nAxis[2];
		nAxis[2] = nTmpAxis;
	}
	if (dAbsFactor[0] < dAbsFactor[1])
	{
		dTmpFactor = dAbsFactor[0];
		dAbsFactor[0] = dAbsFactor[1];
		dAbsFactor[1] = dTmpFactor;
		nTmpAxis = nAxis[0];
		nAxis[0] = nAxis[1];
		nAxis[1] = nTmpAxis;
	}
	
	if (dFactor[nAxis[0]] > 0)
	{
		sprintf( m_ppcOrientation[1], "%c", pcBoxChar[2*nAxis[0]] );
		sprintf( m_ppcOrientation[3], "%c", pcBoxChar[2*nAxis[0]+1] );
	} 
	else if (dFactor[nAxis[0]] < 0)
	{
		sprintf( m_ppcOrientation[1], "%c", pcBoxChar[2*nAxis[0]+1] );
		sprintf( m_ppcOrientation[3], "%c", pcBoxChar[2*nAxis[0]] );
	}
	if (dFactor[nAxis[1]] > 0)
	{
		sprintf( &m_ppcOrientation[1][1], "%c", pcBoxChar[2*nAxis[1]] );
		sprintf( &m_ppcOrientation[3][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		if ( fabs(fabs(dFactor[nAxis[0]])-fabs(dFactor[nAxis[1]])) < TOLER )
		{
			if ( m_ppcOrientation[1][0] == m_ppcOrientation[0][0] 
				|| m_ppcOrientation[1][0] == m_ppcOrientation[2][0] )
			{
				char cTmp;
				cTmp = m_ppcOrientation[1][0];
				m_ppcOrientation[1][0] = m_ppcOrientation[1][1];
				m_ppcOrientation[1][1] = cTmp;
				cTmp = m_ppcOrientation[3][0];
				m_ppcOrientation[3][0] = m_ppcOrientation[3][1];
				m_ppcOrientation[3][1] = cTmp;
			}
		}
	} 
	else if (dFactor[nAxis[1]] < 0)
	{
		sprintf( &m_ppcOrientation[1][1], "%c", pcBoxChar[2*nAxis[1]+1] );
		sprintf( &m_ppcOrientation[3][1], "%c", pcBoxChar[2*nAxis[1]] );
		if ( fabs(fabs(dFactor[nAxis[0]])-fabs(dFactor[nAxis[1]])) < TOLER )
		{
			if ( m_ppcOrientation[1][0] == m_ppcOrientation[0][0] 
				|| m_ppcOrientation[1][0] == m_ppcOrientation[2][0] )
			{
				char cTmp;
				cTmp = m_ppcOrientation[1][0];
				m_ppcOrientation[1][0] = m_ppcOrientation[1][1];
				m_ppcOrientation[1][1] = cTmp;
				cTmp = m_ppcOrientation[3][0];
				m_ppcOrientation[3][0] = m_ppcOrientation[3][1];
				m_ppcOrientation[3][1] = cTmp;
			}
		}
	}
	if (dFactor[nAxis[2]] > 0)
	{
		sprintf( &m_ppcOrientation[1][2], "%c", pcBoxChar[2*nAxis[2]] );
		sprintf( &m_ppcOrientation[3][2], "%c", pcBoxChar[2*nAxis[2]+1] );
	} 
	else if (dFactor[nAxis[2]] < 0)
	{
		sprintf( &m_ppcOrientation[1][2], "%c", pcBoxChar[2*nAxis[2]+1] );
		sprintf( &m_ppcOrientation[3][2], "%c", pcBoxChar[2*nAxis[2]] );
	}

	
}

BOOL CDlgNavigate::OnInitDialog() 
{
	SetScrollBtn(&m_btnScrollOffset);
	SetAddBtn(&m_btnAddOffset);
	SetDelBtn(&m_btnSubOffset);
	SetProbeDepthEdit(&m_edtOffset);
	SetProbeDepthUnitStatic(&m_lblLengthUnit);
	SetView1(&m_btnView1);
	SetView2(&m_btnView2);
	SetView3(&m_btnView3);
	SetView4(&m_btnView4);
	CDlgMain *pDlgMain = g_DlgMain;

	if(pDlgMain->m_DeviceType == DEVICE_IGS_CP)
	{
		EnableProbeTool(TRUE);
	}else
	{
		EnableProbeTool(FALSE);
	}

	CDlgNavigateSkin::OnInitDialog();
	
	// TODO: Add extra initialization here
	//根据设备类型不同，改变界面
	SetUI();

	CString str;
	double dTipLength = m_dStandardProbeLength+m_iPushDepth;
	str.Format("%.1f",dTipLength);
	m_edtOffset.SetWindowText(str);
	SetPushDepth(m_iPushDepth);

	//获取预扫描针的参数
	double dTip[3],dOri[3];
	double dLength,dDiameter;
	CConfigMgr con;
	CString sFilePath = CONFIG_FILES[CF_PROBE_PREIMAGE];
	if( CGlobalFuncs::LoadProbeFile(sFilePath, dOri, dTip, dLength, dDiameter) )
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s successfully. ", sFilePath);
		CLog::Log(sLog);

		m_dPreImageProbeTip[0]=dTip[0];
		m_dPreImageProbeTip[1]=dTip[1];
		m_dPreImageProbeTip[2]=dTip[2];
	}
	else
	{
		CString sLog;
		sLog.Format("Load pre-scan probe from %s fail. ", sFilePath);
		CLog::Log(sLog);
		PanaMessageBox("Fail to load prescan probe!");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNavigate::OnButtonSeries() 
{
	// TODO: Add your control notification handler code here
	CDlgSelectSeries dlgSeries;
	if (dlgSeries.DoModal() == IDOK)
	{
		m_sSeriesName=dlgSeries.m_sName;
		m_sFusionSeriesName="";
		m_chkShowFusion.EnableWindow(FALSE);
		CDlgMain *pDlgMain = g_DlgMain;
		pDlgMain->m_dlgFusion.ShowWindow(FALSE);
		if(MPR(m_sSeriesName))
		{
			CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
			
			Matrix mFusion=pMRISeries->GetFusionCoord();
			pDlgMain->m_dlgTracking.m_dlgTargeting.SetBase2Scanner(mFusion.Inverse());
		}

	}

}

BOOL CDlgNavigate::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
		return   TRUE;   
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_RETURN)   
		return   TRUE;     
	return CDlgNavigateSkin::PreTranslateMessage(pMsg);
}


void CDlgNavigate::ResliceSeries2D_AxisRelative() 
{
	if(m_sSeriesName=="")
		return;

	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
	if(!pMRISeries)
		return ;

	
	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return;
	}

	Matrix mProbeInScanner(4,4);
	if(!GetProbeInScanner(mProbeInScanner,pMRISeries))
		return;

	Matrix mImageInScanner(4,4),mScannerInImage(4,4),mProbeInImage(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mImageInScanner.value[i][j]=reslice.m_dOldImageMatrix[i][j];
		}
	}
	mScannerInImage=mImageInScanner.Inverse();
	mProbeInImage=mScannerInImage * mProbeInScanner;

	Vector3D xAxis(1,0,0);//mProbeInImage.value[0][0],mProbeInImage.value[1][0],mProbeInImage.value[2][0]);//1,0,0);
	Vector3D yAxis(mProbeInImage.value[0][1],mProbeInImage.value[1][1],mProbeInImage.value[2][1]);
	Vector3D zAxis(mProbeInImage.value[0][2],mProbeInImage.value[1][2],mProbeInImage.value[2][2]);
	zAxis= xAxis ^ yAxis;
	yAxis= zAxis ^ xAxis;
	zAxis.normalize();
	yAxis.normalize();

		
	CMRISeries *pNewData=NULL;
	double dResliceMatrix[16];
	double dCenter[3];
	dCenter[0]=mProbeInImage.value[0][3];
	dCenter[1]=mProbeInImage.value[1][3];
	dCenter[2]=mProbeInImage.value[2][3];
	
	
	dResliceMatrix[0]=xAxis.x;
	dResliceMatrix[1]=yAxis.x;
	dResliceMatrix[2]=zAxis.x;
	dResliceMatrix[3]=dCenter[0];
	dResliceMatrix[4]=xAxis.y;
	dResliceMatrix[5]=yAxis.y;
	dResliceMatrix[6]=zAxis.y;
	dResliceMatrix[7]=dCenter[1];
	dResliceMatrix[8]=xAxis.z;
	dResliceMatrix[9]=yAxis.z;
	dResliceMatrix[10]=zAxis.z;
	dResliceMatrix[11]=dCenter[2];
	dResliceMatrix[12]=0;
	dResliceMatrix[13]=0;
	dResliceMatrix[14]=0;
	dResliceMatrix[15]=1;
	
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp4",FALSE);
	reslice.SetDestSeries(pNewData);
	reslice.SetResliceAxis(dResliceMatrix);
	pNewData=(CMRISeries *)reslice.Reslice2D(4);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp4",FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
	}
	pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
	pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,4);

	
}

void CDlgNavigate::ResliceSeries2D() 
{
	if(m_sSeriesName=="")
		return;


	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
	if(!pMRISeries)
		return ;

	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return;
	}

	Matrix mProbeInScanner(4,4);
	if(!GetProbeInScanner(mProbeInScanner,pMRISeries))
		return;

	Matrix mImageInScanner(4,4),mScannerInImage(4,4),mProbeInImage(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mImageInScanner.value[i][j]=reslice.m_dOldImageMatrix[i][j];
		}
	}
	mScannerInImage=mImageInScanner.Inverse();
	mProbeInImage=mScannerInImage * mProbeInScanner;
		

	CMRISeries *pNewData=NULL;
	double dResliceMatrix[16];
	double dCenter[3];
	dCenter[0]=mProbeInImage.value[0][3];
	dCenter[1]=mProbeInImage.value[1][3];
	dCenter[2]=mProbeInImage.value[2][3];
	mProbeInImage.value[0][3]=0;
	mProbeInImage.value[1][3]=0;
	mProbeInImage.value[2][3]=0;
	
	dResliceMatrix[0]=mProbeInImage.value[0][0];
	dResliceMatrix[1]=mProbeInImage.value[0][1];
	dResliceMatrix[2]=mProbeInImage.value[0][2];
	dResliceMatrix[3]=dCenter[0];
	dResliceMatrix[4]=mProbeInImage.value[1][0];
	dResliceMatrix[5]=mProbeInImage.value[1][1];
	dResliceMatrix[6]=mProbeInImage.value[1][2];
	dResliceMatrix[7]=dCenter[1];
	dResliceMatrix[8]=mProbeInImage.value[2][0];
	dResliceMatrix[9]=mProbeInImage.value[2][1];
	dResliceMatrix[10]=mProbeInImage.value[2][2];
	dResliceMatrix[11]=dCenter[2];
	dResliceMatrix[12]=mProbeInImage.value[3][0];
	dResliceMatrix[13]=mProbeInImage.value[3][1];
	dResliceMatrix[14]=mProbeInImage.value[3][2];
	dResliceMatrix[15]=mProbeInImage.value[3][3];
	
	//probe eye
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp1",FALSE);
	reslice.SetDestSeries(pNewData);
	reslice.SetResliceAxis(dResliceMatrix);
	pNewData=(CMRISeries *)reslice.Reslice2D(1);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp1",FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
	}
	pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
	//pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,1);

	mProbeInImage=mProbeInImage.RotateX(PI/2.0);
	dResliceMatrix[0]=mProbeInImage.value[0][0];
	dResliceMatrix[1]=mProbeInImage.value[0][1];
	dResliceMatrix[2]=mProbeInImage.value[0][2];
	dResliceMatrix[3]=dCenter[0];
	dResliceMatrix[4]=mProbeInImage.value[1][0];
	dResliceMatrix[5]=mProbeInImage.value[1][1];
	dResliceMatrix[6]=mProbeInImage.value[1][2];
	dResliceMatrix[7]=dCenter[1];
	dResliceMatrix[8]=mProbeInImage.value[2][0];
	dResliceMatrix[9]=mProbeInImage.value[2][1];
	dResliceMatrix[10]=mProbeInImage.value[2][2];
	dResliceMatrix[11]=dCenter[2];
	dResliceMatrix[12]=mProbeInImage.value[3][0];
	dResliceMatrix[13]=mProbeInImage.value[3][1];
	dResliceMatrix[14]=mProbeInImage.value[3][2];
	dResliceMatrix[15]=mProbeInImage.value[3][3];
	
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp2",FALSE);
	reslice.SetDestSeries(pNewData);
	reslice.SetResliceAxis(dResliceMatrix);
	pNewData=(CMRISeries *)reslice.Reslice2D(2);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp2",FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
	}
	pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
	//pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,2);

	mProbeInImage=mProbeInImage.RotateX(-PI/2.0);
	mProbeInImage=mProbeInImage.RotateY(PI/2.0);
	dResliceMatrix[0]=mProbeInImage.value[0][0];
	dResliceMatrix[1]=mProbeInImage.value[0][1];
	dResliceMatrix[2]=mProbeInImage.value[0][2];
	dResliceMatrix[3]=dCenter[0];
	dResliceMatrix[4]=mProbeInImage.value[1][0];
	dResliceMatrix[5]=mProbeInImage.value[1][1];
	dResliceMatrix[6]=mProbeInImage.value[1][2];
	dResliceMatrix[7]=dCenter[1];
	dResliceMatrix[8]=mProbeInImage.value[2][0];
	dResliceMatrix[9]=mProbeInImage.value[2][1];
	dResliceMatrix[10]=mProbeInImage.value[2][2];
	dResliceMatrix[11]=dCenter[2];
	dResliceMatrix[12]=mProbeInImage.value[3][0];
	dResliceMatrix[13]=mProbeInImage.value[3][1];
	dResliceMatrix[14]=mProbeInImage.value[3][2];
	dResliceMatrix[15]=mProbeInImage.value[3][3];
	
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp3",FALSE);
	reslice.SetDestSeries(pNewData);
	reslice.SetResliceAxis(dResliceMatrix);
	pNewData=(CMRISeries *)reslice.Reslice2D(3);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries("MPR_Tmp3",FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
	}
	pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
	//pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,3);
}

void CDlgNavigate::ResliceSeries3D() 
{
	if(m_sSeriesName=="")
		return;


	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
	if(!pMRISeries)
		return ;

	Matrix mProbeInScanner(4,4);
	if(!GetProbeInScanner(mProbeInScanner,pMRISeries))
		return;
		
	double dCenter[3];
	dCenter[0]=mProbeInScanner.value[0][3];
	dCenter[1]=mProbeInScanner.value[1][3];
	dCenter[2]=mProbeInScanner.value[2][3];

	MPR(m_sSeriesName);
	int iIndex;
	double dis=9999;
	double xp,yp,zp;
	
	if(pDlgMain->m_dlgCryotherapy.m_MRIRectSeries[1].GetMRISeries()!=pMRISeries)
	{
		pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pMRISeries);
		pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pMRISeries,1);
	}
	dis=9999;
	for(int i=0;i<pMRISeries->GetNumOfImages();i++)
	{
		double *dImageCenter=pMRISeries->GetImageAt(i)->GetCenterInScanner();
		double *dImageFreq=pMRISeries->GetImageAt(i)->GetFreqDirectionInScanner();
		double *dImagePhas=pMRISeries->GetImageAt(i)->GetPhaseDirectionInScanner();
		Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
		Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
		Vector vSlice= vFreq * vPhas;
		double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
		CPGMath math;
		math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
			vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
		double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
			+(dCenter[2]-zp)*(dCenter[2]-zp));
		if(distance<dis)
		{
			dis=distance;
			iIndex=i;
		}
	}
	pDlgMain->m_display4Dlg.m_DlgDispWrap2.m_dlgDispControl.ShowImage(iIndex);

	CString sTmp;
	CString sName=pMRISeries->GetName();
	
	CMRISeries *pNewData=NULL;

	sTmp.Format("MPR_Sag_%s",sName);
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}

	if(pDlgMain->m_dlgCryotherapy.m_MRIRectSeries[2].GetMRISeries()!=pNewData)
	{
		pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
		pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,2);
	}
	dis=9999;
	for(i=0;i<pNewData->GetNumOfImages();i++)
	{
		double *dImageCenter=pNewData->GetImageAt(i)->GetCenterInScanner();
		double *dImageFreq=pNewData->GetImageAt(i)->GetFreqDirectionInScanner();
		double *dImagePhas=pNewData->GetImageAt(i)->GetPhaseDirectionInScanner();
		Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
		Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
		Vector vSlice= vFreq * vPhas;
		double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
		CPGMath math;
		math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
			vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
		double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
			+(dCenter[2]-zp)*(dCenter[2]-zp));
		if(distance<dis)
		{
			dis=distance;
			iIndex=i;
		}
	}
	pDlgMain->m_display4Dlg.m_DlgDispWrap3.m_dlgDispControl.ShowImage(iIndex);

	sTmp.Format("MPR_Cor_%s",sName);
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	if(!pNewData)
	{
		PanaMessageBox("Failed to reconstruct image.");
		return;
	}

	if(pDlgMain->m_dlgCryotherapy.m_MRIRectSeries[3].GetMRISeries()!=pNewData)
	{
		pDlgMain->m_dlgCryotherapy.TransformMRISeriesToFinalCor(pNewData);
		pDlgMain->m_dlgCryotherapy.DisplayMRISeriesToWindow(pNewData,3);
	}
	dis=9999;
	for(i=0;i<pNewData->GetNumOfImages();i++)
	{
		double *dImageCenter=pNewData->GetImageAt(i)->GetCenterInScanner();
		double *dImageFreq=pNewData->GetImageAt(i)->GetFreqDirectionInScanner();
		double *dImagePhas=pNewData->GetImageAt(i)->GetPhaseDirectionInScanner();
		Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
		Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
		Vector vSlice= vFreq * vPhas;
		double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
		CPGMath math;
		math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
			vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
		double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
			+(dCenter[2]-zp)*(dCenter[2]-zp));
		if(distance<dis)
		{
			dis=distance;
			iIndex=i;
		}
	}
	pDlgMain->m_display4Dlg.m_DlgDispWrap4.m_dlgDispControl.ShowImage(iIndex);
}

void CDlgNavigate::ResliceSeries() 
{
	
	// TODO: Add your command handler code here
	if(m_bTracking && !m_bIsPausing && !m_bFreezing)
	{		
		if(m_newViewMode!=m_viewMode)
		{		
			CDlgMain *pDlgMain = g_DlgMain;
			CRect rc;
			if(m_newViewMode==TVIEW_AV) //需要切换成AV界面
			{
				pDlgMain->m_dlgTracking.GetWindowRect(&rc);
			
				pDlgMain->m_dlgTracking.MoveWindow(0,0,rc.Width(),rc.Height()/2);
				pDlgMain->m_dlgCryotherapy.m_dlgTps.ChangeDisplaySize(0,rc.Height()/2+1,rc.Width(),rc.Height()/2);
				pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowDisp(TRUE);
				pDlgMain->m_display4Dlg.ShowWindow(FALSE);
			}else if(m_viewMode == TVIEW_AV) //旧显示模式是AV
			{
				pDlgMain->GetWindowRect(&rc);
				pDlgMain->m_dlgTracking.MoveWindow(0,0,rc.Width()-270,rc.Height());
				pDlgMain->m_dlgCryotherapy.m_dlgTps.ChangeDisplaySize(0,0,rc.Width()-270,rc.Height());
				pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowDisp(FALSE);
				pDlgMain->m_display4Dlg.ShowWindow(FALSE);
			}

			m_viewMode=m_newViewMode;
			
			pDlgMain->m_dlgTracking.SetViewMode(m_viewMode);
		}
		
		ResliceProbeEyeAndVirtualUS();
		ResliceFusion();
		
	}
}

BOOL CDlgNavigate::MPR(CString sSeriesName)
{
	if(sSeriesName=="")
		return FALSE;


	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(sSeriesName);
	if(!pMRISeries)
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to find series.");
		return FALSE;
	}

	int nWindowWidth = pMRISeries->GetImageAt(0)->GetInterWindowWidth();
	int nWindowLevel = pMRISeries->GetImageAt(0)->GetInterWindowLevel();
	pDlgMain->m_dlgTracking.SetWL(nWindowWidth,nWindowLevel);
	
	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return FALSE;
	}
	
	CString sTmp;
	CString sTmp2;
	CString sName=pMRISeries->GetName();
	int iFind=sName.Find(".",0);
	if(iFind>0)
	{
		sName=sName.Left(iFind);
	}

	CMRISeries *pNewData=NULL;

	
	sTmp.Format("MPR_Cor_%s",sName);
	sTmp2.Format("MPR_Cor_%s",sSeriesName);
	ShowWaiting(1,"Creating "+sSeriesName);
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp2,FALSE);
	if(!pNewData)
	{
		pNewData=(CMRISeries *)reslice.Reslice(sTmp,1); //Cor
		if(pNewData)
		{
			ShowWaiting(1,"Loading "+sSeriesName);
			pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
			//pNewData->Save("d:\\tmp\\1");
		}else
		{
			PanaMessageBox("Image sagittal reconstruct failed.");
		}
	}

	sTmp.Format("MPR_Sag_%s",sName);
	sTmp2.Format("MPR_Sag_%s",sSeriesName);
	ShowWaiting(1,"Creating "+sSeriesName);
	pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp2,FALSE);
	if(!pNewData)
	{
		pNewData=(CMRISeries *)reslice.Reslice(sTmp,2); //Sag
		if(pNewData)
		{
			ShowWaiting(1,"Loading "+sSeriesName);
			pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData,FALSE);
			//pNewData->Save("d:\\tmp\\2");
		}else
		{
			PanaMessageBox("Image coronal reconstruct failed.");
		}
	}

	ShowWaiting(-1,"");
	return TRUE;
}

void CDlgNavigate::OnCheckSameWwwl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CDlgNavigate::OnButtonCtCalibrate() 
{
	// TODO: Add your control notification handler code here
	CDlgSelectSeries dlgSeries;
	CString sName;
	if (dlgSeries.DoModal() == IDOK)
	{
		sName=dlgSeries.m_sName;
	}else
	{
		return;
	}

	//1. begin MPR
	CDlgMain *pDlgMain = g_DlgMain;
	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(sName);
	if(!pMRISeries)
		return;

	//prepare data
	//检查序列是否合格
	if(pMRISeries==NULL)
		return;
	CMRIImage *pMRIImage = pMRISeries->GetImageAt(0);
	float fFOVX = pMRIImage->GetFOVX();
	float fFOVY = pMRIImage->GetFOVY();
	int iWidth  = pMRIImage->GetResolutionX();
	int iHeight = pMRIImage->GetResolutionY();
	if(fFOVX!=fFOVY || iWidth!=iHeight)
	{
		PanaMessageBox("Incorrect images. \r\nFovX of image must be equal to FovY.");
		return;
	}

	ShowWaiting(1,"Preparing data ...");
	for(int dx=0;dx<pMRIImage->GetResolutionX();dx++)
	{
		for(int dy=0;dy<pMRIImage->GetResolutionY();dy++)
		{
			int x=dx-iWidth/2;
			int y=dy-iHeight/2;
			int dis=sqrt(x*x+y*y);
			if(dis>iWidth/2-5)
			{
				for(int i=0;i<pMRISeries->GetNumOfImages();i++)
				{
					CMRIImage *img=pMRISeries->GetImageAt(i);
					IMG_DATA_TYPE *pData=img->GetColorOfRawData();
					pData[dy*iWidth+dx]=1000;
				}
			}
		}
		
	}

	int iFind=sName.Find(".",0);
	if(iFind>0)
	{
		sName=sName.Right(sName.GetLength()-iFind);
	}

	ShowWaiting(1,"Deleting old series...");
	pDlgMain->m_dlgCryotherapy.DeleteMRISeries("CT_Cali_1"+sName);
	pDlgMain->m_dlgCryotherapy.DeleteMRISeries("CT_Cali_2"+sName);
	pDlgMain->m_dlgCryotherapy.DeleteMRISeries("CT_Cali_3"+sName);

	
	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return;
	}
	double space[]={0.4,0.4,2};
	int extent[]={512,512,100};
	reslice.SetSpaceExtend(space,extent);

	CString sTmp;
	
	CMRISeries *pNewData1=NULL;
	CMRISeries *pNewData2=NULL;
	CMRISeries *pNewData3=NULL;

	
	ShowWaiting(1,"Creating CT_Cali_1 ...");
	sTmp="CT_Cali_1";
	pNewData1=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	reslice.SetDestSeries(pNewData1);
	pNewData1=(CMRISeries *)reslice.Reslice(sTmp,1); //Cor
	if(!pNewData1)
	{
		ShowWaiting(-1,"");
		PanaMessageBox("Image reconstruct failed.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData1,FALSE);
	}

	ShowWaiting(1,"Creating CT_Cali_2 ...");
	sTmp="CT_Cali_2";
	pNewData2=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	reslice.SetDestSeries(pNewData2);
	pNewData2=(CMRISeries *)reslice.Reslice(sTmp,2); //Sag
	if(!pNewData2)
	{
		ShowWaiting(-1,"");
		PanaMessageBox("Image reconstruct failed.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData2,FALSE);
	}

	ShowWaiting(1,"Creating CT_Cali_3 ...");
	sTmp="CT_Cali_3";
	pNewData3=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
	reslice.SetDestSeries(pNewData3);
	pNewData3=(CMRISeries *)reslice.Reslice(sTmp,0); //Axi
	if(!pNewData3)
	{
		ShowWaiting(-1,"");
		PanaMessageBox("Image reconstruct failed.");
		return;
	}
	if(pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE)==NULL)
	{
		pDlgMain->m_dlgCryotherapy.LoadMRISeries(pNewData3,FALSE);
	}
	

	ShowWaiting(1,"Processing CT_Cali_1 ...");
	CMRIImage *pImage=NULL;
	pImage=pNewData1->GetImageAt(0);
	IMG_DATA_TYPE *p=pImage->GetColorOfRawData();
	for(int j=0;j<pImage->GetResolutionX()*pImage->GetResolutionY();j++)
	{
		double x=0;
		for(int i=0;i<pNewData1->GetNumOfImages();i++)
		{
			CMRIImage *img=pNewData1->GetImageAt(i);
			IMG_DATA_TYPE *pData=img->GetColorOfRawData();
			x+=pData[j];
		}
		x=x/pNewData1->GetNumOfImages();
		p[j]=x;
	}
	pNewData1->SetNumberOfImages(1);  //此代码造成内存泄露 大于1的CMriImage不能被释放
	pNewData1->SetWindowWidthAndLevel(200,1059);
	//pNewData1->Save("d:\\tmp\\2");

	ShowWaiting(1,"Processing CT_Cali_2 ...");
	pImage=pNewData2->GetImageAt(0);
	p=pImage->GetColorOfRawData();
	for(j=0;j<pImage->GetResolutionX()*pImage->GetResolutionY();j++)
	{
		double x=0;
		for(int i=0;i<pNewData2->GetNumOfImages();i++)
		{
			CMRIImage *img=pNewData2->GetImageAt(i);
			IMG_DATA_TYPE *pData=img->GetColorOfRawData();
			x+=pData[j];
		}
		x=x/pNewData2->GetNumOfImages();
		p[j]=x;
	}
	pNewData2->SetNumberOfImages(1); //此代码造成内存泄露 大于1的CMriImage不能被释放
	pNewData2->SetWindowWidthAndLevel(200,1059);
	//pNewData2->Save("d:\\tmp\\3");

	ShowWaiting(1,"Processing CT_Cali_3 ...");
	pImage=pNewData3->GetImageAt(0);
	p=pImage->GetColorOfRawData();
	for(j=0;j<pImage->GetResolutionX()*pImage->GetResolutionY();j++)
	{
		double x=0;
		for(int i=0;i<pNewData3->GetNumOfImages();i++)
		{
			CMRIImage *img=pNewData3->GetImageAt(i);
			IMG_DATA_TYPE *pData=img->GetColorOfRawData();
			x+=pData[j];
		}
		x=x/pNewData3->GetNumOfImages();
		p[j]=x;
	}
	pNewData3->SetNumberOfImages(1); //此代码造成内存泄露 大于1的CMriImage不能被释放
	pNewData3->SetWindowWidthAndLevel(80,820);
	//pNewData2->Save("d:\\tmp\\3");

	ShowWaiting(-1,"");
}


void CDlgNavigate::OnButtonTracking() 
{
	// TODO: Add your control notification handler code here
	if(m_sSeriesName=="")
		return;

	CDlgMain *pDlgMain = g_DlgMain;

	

	if(m_bTracking)
	{
		m_bTracking=FALSE;
		m_btnTracking.SetWindowText("Start\nNavigation");		
		pDlgMain->m_dlgTracking.m_dlgTargeting.Disable();
		pDlgMain->m_dlgCryotherapy.m_updateRate=160;

		
		m_bShowFusion=FALSE;
		UpdateData(FALSE);
		pDlgMain->m_dlgFusion.ShowWindow(FALSE);

		CRect rc;
		pDlgMain->GetWindowRect(&rc);
		pDlgMain->m_dlgTracking.ShowWindow(FALSE);
		pDlgMain->m_dlgTracking.MoveWindow(0,0,rc.Width()-270,rc.Height());
		pDlgMain->m_dlgCryotherapy.m_dlgTps.ChangeDisplaySize(0,0,rc.Width()-270,rc.Height());
		pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowDisp(FALSE);
		pDlgMain->m_display4Dlg.ShowWindow(TRUE);

		m_btnFreeze.EnableWindow(FALSE);
	}else
	{
		CString sTmp;		
		sTmp.Format("MPR_Sag_%s",m_sSeriesName);
		CMRISeries *pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
		if(!pNewData)
		{
			if(!MPR(m_sSeriesName))
			{
				PanaMessageBox("Failed to reconstruct image.");
				return;
			}

			//融合操作可能改变融合坐标，所以需要重置Targeting中的基坐标
			CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
			Matrix mFusion=pMRISeries->GetFusionCoord();
			pDlgMain->m_dlgTracking.m_dlgTargeting.SetBase2Scanner(mFusion.Inverse());
			
		}
		
		//更新导航窗口target列表
		pDlgMain->m_dlgTracking.m_dlgTargeting.Disable();
		pDlgMain->m_dlgTracking.RemoveVirtualTarget();
		POSITION pos = pDlgMain->m_dlgCryotherapy.m_TargetPool.m_lstVirtualTargets.GetHeadPosition();
		while(pos)
		{
			CVirtualTarget *pVTarget = pDlgMain->m_dlgCryotherapy.m_TargetPool.m_lstVirtualTargets.GetNext(pos);
			pDlgMain->m_dlgTracking.AddVirtualTarget(pVTarget);
		}

		//导入TPS中的针形成的靶点
		int iCount;
		double pProbeInfo[100];
		pDlgMain->m_dlgCryotherapy.m_dlgTps.GetProbeInfo(iCount,pProbeInfo);
		if(iCount>0)
		{			
			for(int i=0;i<iCount;i++)
			{
				Matrix m(4,4);
				m.Identity();
				m.value[0][3]=pProbeInfo[i*3];
				m.value[1][3]=pProbeInfo[i*3+1];
				m.value[2][3]=pProbeInfo[i*3+2];
				//m= pDlgMain->m_dlgCryotherapy.m_tranScanner2Marker * m;  //在病床有marker的情况下，m中已经是位于marker下的坐标
				Vector3D pos(m.value[0][3],m.value[1][3],m.value[2][3]);
				CVirtualTarget target(pos);
				pDlgMain->m_dlgTracking.AddVirtualTarget(&target);
			}
			
		}

		//设置自动选中的靶点
		pDlgMain->m_dlgTracking.CheckSelectedTarget();

		if(m_viewMode==TVIEW_AV) //需要切换成AV界面
		{
			CRect rc;
			pDlgMain->m_dlgTracking.GetWindowRect(&rc);
			
			pDlgMain->m_dlgTracking.MoveWindow(0,0,rc.Width(),rc.Height()/2);
			pDlgMain->m_dlgCryotherapy.m_dlgTps.ChangeDisplaySize(0,rc.Height()/2+1,rc.Width(),rc.Height()/2);
			pDlgMain->m_dlgCryotherapy.m_dlgTps.ShowDisp(TRUE);
			pDlgMain->m_display4Dlg.ShowWindow(FALSE);
		}

		m_bIsPausing=FALSE;
		m_bTracking=TRUE;
		m_btnTracking.SetWindowText("Stop\nNavigation");
		pDlgMain->m_dlgTracking.ShowWindow(TRUE);
		pDlgMain->m_display4Dlg.ShowWindow(FALSE);		
		pDlgMain->m_dlgCryotherapy.m_updateRate=50;

		m_btnFreeze.EnableWindow(TRUE);
		m_bFreezing = FALSE;
		m_btnFreeze.SetWindowText("Freeze");
	}

	pDlgMain->m_dlgCryotherapy.m_dlgCmdBar.UpdateUI();	
}



BOOL CDlgNavigate::GetProbeInScanner(Matrix &mProbe2Scanner,CMRISeries *pSeries)
{
	CDlgMain *pDlgMain = g_DlgMain;

	BOOL bFind=FALSE;
	Vector3D tipInTrack,oriInProbe;
	Matrix mProbeMarkerInTrack(4,4);
	for ( int i = 0; i < pDlgMain->m_dlgCryotherapy.m_iMaxNumOfTrackTool; i ++ )
	{
		if (pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i] &&
			pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_TrackType == TT_NEEDLE)
		{
			tipInTrack=pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe->GetRealTipInTrack();
			mProbeMarkerInTrack=pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe->GetMarkerOrientationInTrack();
			oriInProbe=pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe->GetOrientationInProbe();
			bFind=TRUE;
			m_pCurRealProbe=pDlgMain->m_dlgCryotherapy.m_ppTrackTool[i]->m_pAttachedProbe;
		}
	}

	if(!bFind)
		return FALSE;
	
	if(pDlgMain->m_dlgCryotherapy.m_patientMarker==PATIENT_NO_MARKER)
	{
		mProbeMarkerInTrack.value[0][3]=tipInTrack.x-pDlgMain->m_dlgCryotherapy.m_vPTMovInTracker.x;
		mProbeMarkerInTrack.value[1][3]=tipInTrack.y-pDlgMain->m_dlgCryotherapy.m_vPTMovInTracker.y;
		mProbeMarkerInTrack.value[2][3]=tipInTrack.z-pDlgMain->m_dlgCryotherapy.m_vPTMovInTracker.z;
		
		Matrix mTrackInScanner(4,4);
		
		mTrackInScanner=pDlgMain->m_dlgCryotherapy.m_tranScanner2Tracker.Inverse();
		mProbe2Scanner=mTrackInScanner * mProbeMarkerInTrack;
	}else 
	{
		Matrix mTrack2Marker = pDlgMain->m_dlgCryotherapy.m_tMarker.Inverse();
		Matrix mProbeInMarker(4,4);
		mProbeInMarker = mTrack2Marker * mProbeMarkerInTrack;

		//病床上有marker时,tipInTrack实际代表针尖在病床marker下的坐标
		mProbeInMarker.value[0][3]=tipInTrack.x;
		mProbeInMarker.value[1][3]=tipInTrack.y;
		mProbeInMarker.value[2][3]=tipInTrack.z;
				
		Matrix mMarkerInScanner(4,4);
		
		mMarkerInScanner=pSeries->GetFusionCoord().Inverse();
		mProbe2Scanner=mMarkerInScanner * mProbeInMarker;
	}

	//将针的实际方向进行修正,Y方向沿着针体方向
	double x,y,z;
	x=mProbe2Scanner.value[0][3];
	y=mProbe2Scanner.value[1][3];
	z=mProbe2Scanner.value[2][3];
	mProbe2Scanner.value[0][3]=0;
	mProbe2Scanner.value[1][3]=0;
	mProbe2Scanner.value[2][3]=0;

	Vector vRealOriInScanner(4),vY(3),vX(3),vZ(3);
	vRealOriInScanner.value[0]=oriInProbe.x;
	vRealOriInScanner.value[1]=oriInProbe.y;
	vRealOriInScanner.value[2]=oriInProbe.z;
	vRealOriInScanner.value[3]=1;
	vRealOriInScanner = mProbe2Scanner * vRealOriInScanner;

	vY.value[0]=vRealOriInScanner.value[0];
	vY.value[1]=vRealOriInScanner.value[1];
	vY.value[2]=vRealOriInScanner.value[2];
	vY = vY.Normalize();  

	vX.value[0]=mProbe2Scanner.value[0][0];
	vX.value[1]=mProbe2Scanner.value[1][0];
	vX.value[2]=mProbe2Scanner.value[2][0];
	
	vZ = vX * vY;
	vZ = vZ.Normalize();
	vX = vY * vZ;
	vX = vX.Normalize();
	mProbe2Scanner.value[0][0]=vX.value[0];
	mProbe2Scanner.value[1][0]=vX.value[1];
	mProbe2Scanner.value[2][0]=vX.value[2];
	mProbe2Scanner.value[0][1]=vY.value[0];
	mProbe2Scanner.value[1][1]=vY.value[1];
	mProbe2Scanner.value[2][1]=vY.value[2];
	mProbe2Scanner.value[0][2]=vZ.value[0];
	mProbe2Scanner.value[1][2]=vZ.value[1];
	mProbe2Scanner.value[2][2]=vZ.value[2];
	mProbe2Scanner.value[0][3]=x;
	mProbe2Scanner.value[1][3]=y;
	mProbe2Scanner.value[2][3]=z;
	
	return TRUE;
}

void CDlgNavigate::ResliceProbeEyeAndVirtualUS()
{
	if(m_sSeriesName=="")
		return;


	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sSeriesName);
	if(!pMRISeries)
		return ;

	Matrix mFusion = pMRISeries->GetFusionCoord();
	
	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return;
	}

	Matrix mProbeInScanner(4,4);
	if(!GetProbeInScanner(mProbeInScanner,pMRISeries))
		return;

	Matrix mImageInScanner(4,4),mScannerInImage(4,4),mProbeInImage(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mImageInScanner.value[i][j]=reslice.m_dOldImageMatrix[i][j];
		}
	}
	mScannerInImage=mImageInScanner.Inverse();
	mProbeInImage=mScannerInImage * mProbeInScanner;
		
	LPUS_STRUCT_IMAGE pUSData;

	double dCenter[3];
	dCenter[0]=mProbeInScanner.value[0][3];
	dCenter[1]=mProbeInScanner.value[1][3];
	dCenter[2]=mProbeInScanner.value[2][3];
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][1];
		vz.value[1]=mProbeInImage.value[1][1];
		vz.value[2]=mProbeInImage.value[2][1];
		dCenter[0] = dCenter[0] - vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] - vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] - vz.value[2]*m_iLookDepth;
	}
	
	//Axis
	if(m_viewMode == TVIEW_TAPV || m_viewMode == TVIEW_AST || m_viewMode == TVIEW_ASPV ||m_viewMode == TVIEW_AV)
	{
		int iIndex;
		double dis=9999;
		double xp,yp,zp;
		
		
		dis=9999;
		for(i=0;i<pMRISeries->GetNumOfImages();i++)
		{
			double *dImageCenter=pMRISeries->GetImageAt(i)->GetCenterInScanner();
			double *dImageFreq=pMRISeries->GetImageAt(i)->GetFreqDirectionInScanner();
			double *dImagePhas=pMRISeries->GetImageAt(i)->GetPhaseDirectionInScanner();
			Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
			Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
			Vector vSlice= vFreq * vPhas;
			double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
			CPGMath math;
			math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
				vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
			double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
				+(dCenter[2]-zp)*(dCenter[2]-zp));
			if(distance<dis)
			{
				dis=distance;
				iIndex=i;
			}
		}
		pUSData=GetUSImage(pMRISeries,iIndex);
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgTracking.SetAxial(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
	}

	//Sagittal
	if(m_viewMode == TVIEW_AST || m_viewMode == TVIEW_ASPV)
	{
		CString sTmp;
		CString sName=pMRISeries->GetName();		
		CMRISeries *pNewData=NULL;
		
		sTmp.Format("MPR_Sag_%s",sName);
		pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
		if(!pNewData)
		{
			return;
		}

		int iIndex=0;
		double dis=9999;
		double xp,yp,zp;
		
		
		dis=9999;
		for(i=0;i<pNewData->GetNumOfImages();i++)
		{
			double *dImageCenter=pNewData->GetImageAt(i)->GetCenterInScanner();
			double *dImageFreq=pNewData->GetImageAt(i)->GetFreqDirectionInScanner();
			double *dImagePhas=pNewData->GetImageAt(i)->GetPhaseDirectionInScanner();
			Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
			Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
			Vector vSlice= vFreq * vPhas;
			double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
			CPGMath math;
			math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
				vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
			double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
				+(dCenter[2]-zp)*(dCenter[2]-zp));
			if(distance<dis)
			{
				dis=distance;
				iIndex=i;
			}
		}
		pUSData=GetUSImage(pNewData,iIndex);
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgTracking.SetSagittal(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;

	}
	
	//probe eye
	double dResliceMatrix[16];
	dCenter[0]=mProbeInImage.value[0][3];
	dCenter[1]=mProbeInImage.value[1][3];
	dCenter[2]=mProbeInImage.value[2][3];
	mProbeInImage.value[0][3]=0;
	mProbeInImage.value[1][3]=0;
	mProbeInImage.value[2][3]=0;
	//if(pDlgMain->m_trackerManager.GetHandler()->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][1];
		vz.value[1]=mProbeInImage.value[1][1];
		vz.value[2]=mProbeInImage.value[2][1];
		dCenter[0] = dCenter[0] - vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] - vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] - vz.value[2]*m_iLookDepth;

		mProbeInImage=mProbeInImage.RotateX(PI/2.0);
		dResliceMatrix[0]=mProbeInImage.value[0][0];
		dResliceMatrix[1]=mProbeInImage.value[0][1];
		dResliceMatrix[2]=mProbeInImage.value[0][2];
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=mProbeInImage.value[1][0];
		dResliceMatrix[5]=mProbeInImage.value[1][1];
		dResliceMatrix[6]=mProbeInImage.value[1][2];
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=mProbeInImage.value[2][0];
		dResliceMatrix[9]=mProbeInImage.value[2][1];
		dResliceMatrix[10]=mProbeInImage.value[2][2];
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=mProbeInImage.value[3][0];
		dResliceMatrix[13]=mProbeInImage.value[3][1];
		dResliceMatrix[14]=mProbeInImage.value[3][2];
		dResliceMatrix[15]=mProbeInImage.value[3][3];
		mProbeInImage=mProbeInImage.RotateX(-PI/2.0);
	}/*else
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][2];
		vz.value[1]=mProbeInImage.value[1][2];
		vz.value[2]=mProbeInImage.value[2][2];
		dCenter[0] = dCenter[0] + vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] + vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] + vz.value[2]*m_iLookDepth;

		dResliceMatrix[0]=mProbeInImage.value[0][0];
		dResliceMatrix[1]=mProbeInImage.value[0][1];
		dResliceMatrix[2]=mProbeInImage.value[0][2];
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=mProbeInImage.value[1][0];
		dResliceMatrix[5]=mProbeInImage.value[1][1];
		dResliceMatrix[6]=mProbeInImage.value[1][2];
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=mProbeInImage.value[2][0];
		dResliceMatrix[9]=mProbeInImage.value[2][1];
		dResliceMatrix[10]=mProbeInImage.value[2][2];
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=mProbeInImage.value[3][0];
		dResliceMatrix[13]=mProbeInImage.value[3][1];
		dResliceMatrix[14]=mProbeInImage.value[3][2];
		dResliceMatrix[15]=mProbeInImage.value[3][3];
	}*/
	if(m_viewMode == TVIEW_TAPV || m_viewMode == TVIEW_ASPV)
	{
		reslice.SetResliceAxis(dResliceMatrix);
		pUSData=(LPUS_STRUCT_IMAGE)reslice.Reslice2D();
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgTracking.SetProbeEye(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
	}

	//Probe Axis
	//if(pDlgMain->m_trackerManager.GetHandler()->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
	{
		Vector3D xAxis,yAxis,zAxis;
		if(pDlgMain->m_dlgTracking.m_dlgVirtualUS.IsSliding())
		{
			xAxis.x=mProbeInImage.value[0][0];
			xAxis.y=mProbeInImage.value[1][0];
			xAxis.z=mProbeInImage.value[2][0];
			yAxis.x=mProbeInImage.value[0][1];
			yAxis.y=mProbeInImage.value[1][1];
			yAxis.z=mProbeInImage.value[2][1];
			zAxis.x=mProbeInImage.value[0][2];
			zAxis.y=mProbeInImage.value[1][2];
			zAxis.z=mProbeInImage.value[2][2];
			Matrix m(4,4);
			m.Identity();
			m.value[0][0]=xAxis.x;
			m.value[1][0]=xAxis.y;
			m.value[2][0]=xAxis.z;
			m.value[0][1]=yAxis.x;
			m.value[1][1]=yAxis.y;
			m.value[2][1]=yAxis.z;
			m.value[0][2]=zAxis.x;
			m.value[1][2]=zAxis.y;
			m.value[2][2]=zAxis.z;
			m=m.RotateY(PI*m_iVirtualUSAngle/180.0);
			xAxis.x=m.value[0][0];
			xAxis.y=m.value[1][0];
			xAxis.z=m.value[2][0];
			yAxis.x=m.value[0][1];
			yAxis.y=m.value[1][1];
			yAxis.z=m.value[2][1];
			zAxis.x=m.value[0][2];
			zAxis.y=m.value[1][2];
			zAxis.z=m.value[2][2];
		}else
		{
			if(fabs(mProbeInImage.value[0][1])>0.71)
			{
				xAxis.x=0;
				xAxis.y=1;
				xAxis.z=0;
				yAxis.x=mProbeInImage.value[0][1];
				yAxis.y=mProbeInImage.value[1][1];
				yAxis.z=mProbeInImage.value[2][1];

			}else
			{
			xAxis.x=1;
			xAxis.y=0;
			xAxis.z=0;
			yAxis.x=mProbeInImage.value[0][1];
			yAxis.y=mProbeInImage.value[1][1];
			yAxis.z=mProbeInImage.value[2][1];
			}
			
			zAxis= xAxis ^ yAxis;
			yAxis= zAxis ^ xAxis;
			zAxis.normalize();
			yAxis.normalize();	
		}
		
		dResliceMatrix[0]=xAxis.x;
		dResliceMatrix[1]=yAxis.x;
		dResliceMatrix[2]=zAxis.x;
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=xAxis.y;
		dResliceMatrix[5]=yAxis.y;
		dResliceMatrix[6]=zAxis.y;
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=xAxis.z;
		dResliceMatrix[9]=yAxis.z;
		dResliceMatrix[10]=zAxis.z;
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=0;
		dResliceMatrix[13]=0;
		dResliceMatrix[14]=0;
		dResliceMatrix[15]=1;
	}/*else
	{
		Vector3D xAxis,yAxis,zAxis;
		if(pDlgMain->m_dlgTracking.m_dlgVirtualUS.IsSliding())
		{
			xAxis.x=mProbeInImage.value[0][0];
			xAxis.y=mProbeInImage.value[1][0];
			xAxis.z=mProbeInImage.value[2][0];
			zAxis.x=-mProbeInImage.value[0][1];
			zAxis.y=-mProbeInImage.value[1][1];
			zAxis.z=-mProbeInImage.value[2][1];
			yAxis.x=mProbeInImage.value[0][2];
			yAxis.y=mProbeInImage.value[1][2];
			yAxis.z=mProbeInImage.value[2][2];
			Matrix m(4,4);
			m.Identity();
			m.value[0][0]=xAxis.x;
			m.value[1][0]=xAxis.y;
			m.value[2][0]=xAxis.z;
			m.value[0][1]=yAxis.x;
			m.value[1][1]=yAxis.y;
			m.value[2][1]=yAxis.z;
			m.value[0][2]=zAxis.x;
			m.value[1][2]=zAxis.y;
			m.value[2][2]=zAxis.z;
			m=m.RotateY(PI*m_iVirtualUSAngle/180.0);
			xAxis.x=m.value[0][0];
			xAxis.y=m.value[1][0];
			xAxis.z=m.value[2][0];
			yAxis.x=m.value[0][1];
			yAxis.y=m.value[1][1];
			yAxis.z=m.value[2][1];
			zAxis.x=m.value[0][2];
			zAxis.y=m.value[1][2];
			zAxis.z=m.value[2][2];
		}else
		{
			xAxis.x=1;
			xAxis.y=0;
			xAxis.z=0;
			yAxis.x=mProbeInImage.value[0][2];
			yAxis.y=mProbeInImage.value[1][2];
			yAxis.z=mProbeInImage.value[2][2];
			
			zAxis= xAxis ^ yAxis;
			yAxis= zAxis ^ xAxis;
			zAxis.normalize();
			yAxis.normalize();	
		}
		

		dResliceMatrix[0]=xAxis.x;
		dResliceMatrix[1]=yAxis.x;
		dResliceMatrix[2]=zAxis.x;
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=xAxis.y;
		dResliceMatrix[5]=yAxis.y;
		dResliceMatrix[6]=zAxis.y;
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=xAxis.z;
		dResliceMatrix[9]=yAxis.z;
		dResliceMatrix[10]=zAxis.z;
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=0;
		dResliceMatrix[13]=0;
		dResliceMatrix[14]=0;
		dResliceMatrix[15]=1;
	}*/
	
	if(m_viewMode == TVIEW_TAPV || m_viewMode == TVIEW_ASPV ||m_viewMode == TVIEW_AV)
	{
		reslice.SetResliceAxis(dResliceMatrix);
		pUSData=(LPUS_STRUCT_IMAGE)reslice.Reslice2D();
		if(!pUSData)
		{
			return;
		}	
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgTracking.SetVirtualUS(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
		
	}
		
}

LPUS_STRUCT_IMAGE CDlgNavigate::GetUSImage(CMRISeries *pSeries, int index)
{
	if(pSeries==NULL || pSeries->GetNumOfImages()<=index || index<0)
		return NULL;

	MR_STRUCT_IMAGE* pHeader=pSeries->GetImageAt(index)->GetImage();
	US_STRUCT_IMAGE* pUSImage=new US_STRUCT_IMAGE;
	
	try
	{		
		
		if(pSeries->GetImageAt(index)->GetImageType() == IMS_DCMCT)
		{
			CT_STRUCT_IMAGE *pCTImage=(CT_STRUCT_IMAGE*)pHeader;
			pUSImage->pData=new unsigned short[pCTImage->oHeader.seriesField.ResolutionX*pCTImage->oHeader.seriesField.ResolutionY];
			memcpy(pUSImage->pData,pCTImage->pData,sizeof(unsigned short)*pCTImage->oHeader.seriesField.ResolutionX*
				pCTImage->oHeader.seriesField.ResolutionY);
			
			//header info
			pUSImage->oHeader.headInfo.BITitle[0] = 'D';
			pUSImage->oHeader.headInfo.BITitle[1] = 'C';
			pUSImage->oHeader.headInfo.BITitle[2] = 'M';
			pUSImage->oHeader.headInfo.BITitle[3] = 'U';
			pUSImage->oHeader.headInfo.BITitle[4] = 'S';
			pUSImage->oHeader.headInfo.Version = int (1) << 16 + 0;
			
			//study field
			sprintf(pUSImage->oHeader.studyField.Id,"%s",pCTImage->oHeader.studyField.Id);
			pUSImage->oHeader.studyField.PatEntry       =pCTImage->oHeader.studyField.PatEntry;
			pUSImage->oHeader.studyField.PatPosition    =pCTImage->oHeader.studyField.PatPosition;
			pUSImage->oHeader.studyField.Time.year      =pCTImage->oHeader.studyField.Time.year;
			pUSImage->oHeader.studyField.Time.month     =pCTImage->oHeader.studyField.Time.month;
			pUSImage->oHeader.studyField.Time.day       =pCTImage->oHeader.studyField.Time.day;
			pUSImage->oHeader.studyField.Time.hour      =pCTImage->oHeader.studyField.Time.hour;
			pUSImage->oHeader.studyField.Time.minute    =pCTImage->oHeader.studyField.Time.minute;
			pUSImage->oHeader.studyField.Time.second    =pCTImage->oHeader.studyField.Time.second;
			
			
			//series field
			pUSImage->oHeader.seriesField.Id            =pCTImage->oHeader.seriesField.Id;
			pUSImage->oHeader.seriesField.SliceGap      =pCTImage->oHeader.seriesField.SliceGap;
			pUSImage->oHeader.seriesField.SliceThickness=pCTImage->oHeader.seriesField.SliceThickness;
			pUSImage->oHeader.seriesField.FovX          =pCTImage->oHeader.seriesField.FovX;
			pUSImage->oHeader.seriesField.FovY          =pCTImage->oHeader.seriesField.FovY;
			pUSImage->oHeader.seriesField.FovZ          =pCTImage->oHeader.seriesField.FovZ;
			pUSImage->oHeader.seriesField.ResolutionX   =pCTImage->oHeader.seriesField.ResolutionX;
			pUSImage->oHeader.seriesField.ResolutionY   =pCTImage->oHeader.seriesField.ResolutionY;
			pUSImage->oHeader.seriesField.ResolutionZ   =pCTImage->oHeader.seriesField.ResolutionZ;
			
			//scan field
			pUSImage->oHeader.scanField.Id              =pCTImage->oHeader.scanField.Id;
			pUSImage->oHeader.scanField.FreqDirX        =pCTImage->oHeader.scanField.FreqDirX;
			pUSImage->oHeader.scanField.FreqDirY        =pCTImage->oHeader.scanField.FreqDirY;
			pUSImage->oHeader.scanField.FreqDirZ        =pCTImage->oHeader.scanField.FreqDirZ;
			pUSImage->oHeader.scanField.PhaseDirX       =pCTImage->oHeader.scanField.PhaseDirX;
			pUSImage->oHeader.scanField.PhaseDirY       =pCTImage->oHeader.scanField.PhaseDirY;
			pUSImage->oHeader.scanField.PhaseDirZ       =pCTImage->oHeader.scanField.PhaseDirZ;
			pUSImage->oHeader.scanField.SliceDirX       =pCTImage->oHeader.scanField.SliceDirX;
			pUSImage->oHeader.scanField.SliceDirY       =pCTImage->oHeader.scanField.SliceDirY;
			pUSImage->oHeader.scanField.SliceDirZ       =pCTImage->oHeader.scanField.SliceDirZ;
			
			//image field
			pUSImage->oHeader.imageField.Id             =pCTImage->oHeader.imageField.Id;
			pUSImage->oHeader.imageField.fCenterPointX  =pCTImage->oHeader.imageField.fCenterPointX;
			pUSImage->oHeader.imageField.fCenterPointY  =pCTImage->oHeader.imageField.fCenterPointY;
			pUSImage->oHeader.imageField.fCenterPointZ  =pCTImage->oHeader.imageField.fCenterPointZ;
			pUSImage->oHeader.imageField.WindowLevel    =pCTImage->oHeader.imageField.WindowLevel;
			pUSImage->oHeader.imageField.WindowWidth    =pCTImage->oHeader.imageField.WindowWidth;
		}else
		{
			MR_STRUCT_IMAGE *pMRImage=pHeader;
			pUSImage->pData=new unsigned short[pMRImage->oHeader.seriesField.ResolutionX*pMRImage->oHeader.seriesField.ResolutionY];
			memcpy(pUSImage->pData,pMRImage->pData,sizeof(unsigned short)*pMRImage->oHeader.seriesField.ResolutionX*
				pMRImage->oHeader.seriesField.ResolutionY);
			
			//header info
			pUSImage->oHeader.headInfo.BITitle[0] = 'D';
			pUSImage->oHeader.headInfo.BITitle[1] = 'C';
			pUSImage->oHeader.headInfo.BITitle[2] = 'M';
			pUSImage->oHeader.headInfo.BITitle[3] = 'U';
			pUSImage->oHeader.headInfo.BITitle[4] = 'S';
			pUSImage->oHeader.headInfo.Version = int (1) << 16 + 0;
			
			//study field
			sprintf(pUSImage->oHeader.studyField.Id,"%s",pMRImage->oHeader.studyField.Id);
			pUSImage->oHeader.studyField.PatEntry       =pMRImage->oHeader.studyField.PatEntry;
			pUSImage->oHeader.studyField.PatPosition    =pMRImage->oHeader.studyField.PatPosition;
			pUSImage->oHeader.studyField.Time.year      =pMRImage->oHeader.studyField.Time.year;
			pUSImage->oHeader.studyField.Time.month     =pMRImage->oHeader.studyField.Time.month;
			pUSImage->oHeader.studyField.Time.day       =pMRImage->oHeader.studyField.Time.day;
			pUSImage->oHeader.studyField.Time.hour      =pMRImage->oHeader.studyField.Time.hour;
			pUSImage->oHeader.studyField.Time.minute    =pMRImage->oHeader.studyField.Time.minute;
			pUSImage->oHeader.studyField.Time.second    =pMRImage->oHeader.studyField.Time.second;
			
			
			//series field
			pUSImage->oHeader.seriesField.Id            =pMRImage->oHeader.seriesField.Id;
			pUSImage->oHeader.seriesField.SliceGap      =pMRImage->oHeader.seriesField.SliceGap;
			pUSImage->oHeader.seriesField.SliceThickness=pMRImage->oHeader.seriesField.SliceThickness;
			pUSImage->oHeader.seriesField.FovX          =pMRImage->oHeader.seriesField.FovX;
			pUSImage->oHeader.seriesField.FovY          =pMRImage->oHeader.seriesField.FovY;
			pUSImage->oHeader.seriesField.FovZ          =pMRImage->oHeader.seriesField.FovZ;
			pUSImage->oHeader.seriesField.ResolutionX   =pMRImage->oHeader.seriesField.ResolutionX;
			pUSImage->oHeader.seriesField.ResolutionY   =pMRImage->oHeader.seriesField.ResolutionY;
			pUSImage->oHeader.seriesField.ResolutionZ   =pMRImage->oHeader.seriesField.ResolutionZ;
			
			//scan field
			pUSImage->oHeader.scanField.Id              =pMRImage->oHeader.scanField.Id;
			pUSImage->oHeader.scanField.FreqDirX        =pMRImage->oHeader.scanField.FreqDirX;
			pUSImage->oHeader.scanField.FreqDirY        =pMRImage->oHeader.scanField.FreqDirY;
			pUSImage->oHeader.scanField.FreqDirZ        =pMRImage->oHeader.scanField.FreqDirZ;
			pUSImage->oHeader.scanField.PhaseDirX       =pMRImage->oHeader.scanField.PhaseDirX;
			pUSImage->oHeader.scanField.PhaseDirY       =pMRImage->oHeader.scanField.PhaseDirY;
			pUSImage->oHeader.scanField.PhaseDirZ       =pMRImage->oHeader.scanField.PhaseDirZ;
			pUSImage->oHeader.scanField.SliceDirX       =pMRImage->oHeader.scanField.SliceDirX;
			pUSImage->oHeader.scanField.SliceDirY       =pMRImage->oHeader.scanField.SliceDirY;
			pUSImage->oHeader.scanField.SliceDirZ       =pMRImage->oHeader.scanField.SliceDirZ;
			
			//image field
			pUSImage->oHeader.imageField.Id             =pMRImage->oHeader.imageField.Id;
			pUSImage->oHeader.imageField.fCenterPointX  =pMRImage->oHeader.imageField.fCenterPointX;
			pUSImage->oHeader.imageField.fCenterPointY  =pMRImage->oHeader.imageField.fCenterPointY;
			pUSImage->oHeader.imageField.fCenterPointZ  =pMRImage->oHeader.imageField.fCenterPointZ;
			pUSImage->oHeader.imageField.WindowLevel    =pMRImage->oHeader.imageField.WindowLevel;
			pUSImage->oHeader.imageField.WindowWidth    =pMRImage->oHeader.imageField.WindowWidth;
		}
		
				
		
	}catch(...)
	{
		try
		{
			delete pUSImage;
			return NULL;
		}catch(...)
		{
			return NULL;
		}
	}

	return pUSImage;
}

void CDlgNavigate::Transform(LPUS_STRUCT_IMAGE pImage, Matrix m)
{
	Matrix tmp(4,4);
	tmp.Identity();
	tmp.value[0][0]=pImage->oHeader.scanField.FreqDirX;
	tmp.value[1][0]=pImage->oHeader.scanField.FreqDirY;
	tmp.value[2][0]=pImage->oHeader.scanField.FreqDirZ;
	tmp.value[0][1]=pImage->oHeader.scanField.PhaseDirX;
	tmp.value[1][1]=pImage->oHeader.scanField.PhaseDirY;
	tmp.value[2][1]=pImage->oHeader.scanField.PhaseDirZ;
	tmp.value[0][2]=pImage->oHeader.scanField.SliceDirX;
	tmp.value[1][2]=pImage->oHeader.scanField.SliceDirY;
	tmp.value[2][2]=pImage->oHeader.scanField.SliceDirZ;
	tmp.value[0][3]=pImage->oHeader.imageField.fCenterPointX;
	tmp.value[1][3]=pImage->oHeader.imageField.fCenterPointY;
	tmp.value[2][3]=pImage->oHeader.imageField.fCenterPointZ;

	tmp = m * tmp;

	pImage->oHeader.scanField.FreqDirX=tmp.value[0][0];
	pImage->oHeader.scanField.FreqDirY=tmp.value[1][0];
	pImage->oHeader.scanField.FreqDirZ=tmp.value[2][0];

	pImage->oHeader.scanField.PhaseDirX=tmp.value[0][1];
	pImage->oHeader.scanField.PhaseDirY=tmp.value[1][1];
	pImage->oHeader.scanField.PhaseDirZ=tmp.value[2][1];

	pImage->oHeader.scanField.SliceDirX=tmp.value[0][2];
	pImage->oHeader.scanField.SliceDirY=tmp.value[1][2];
	pImage->oHeader.scanField.SliceDirZ=tmp.value[2][2];

	pImage->oHeader.imageField.fCenterPointX=tmp.value[0][3];
	pImage->oHeader.imageField.fCenterPointY=tmp.value[1][3];
	pImage->oHeader.imageField.fCenterPointZ=tmp.value[2][3];
}

BOOL CDlgNavigate::IsTracking()
{
	return m_bTracking;
}

void CDlgNavigate::OnBtnAddOffset() 
{
	// TODO: Add your control notification handler code here

	int iPos = (int)GetPushDepth();
	iPos+=m_nOffset;
	if (iPos >MAX_PUSH_DEPTH)
	{
		iPos=MAX_PUSH_DEPTH;
	}
	m_iPushDepth=iPos;
	SetPushDepth(m_iPushDepth);

}

void CDlgNavigate::OnBtnSubOffset() 
{
	// TODO: Add your control notification handler code here

	int iPos = (int)GetPushDepth();
	iPos-=m_nOffset;
	if (iPos <MIN_PUSH_DEPTH)
	{
		iPos=MIN_PUSH_DEPTH;
	}
	m_iPushDepth=iPos;
	SetPushDepth(m_iPushDepth);
	
}


void CDlgNavigate::SetUI()
{	
	CDlgMain *pDlgMain = g_DlgMain;

	int left=7,top=10,width=41,height=38;
	m_btnSelect.MoveWindow(left,top,60,50);
	m_btnTracking.MoveWindow(left+64,top,60,50);
	m_btnFreeze.MoveWindow(left+128,top,60,50);
	top=75;

	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(left,top-5,190,80);
	m_staticProbe.MoveWindow(left,top+85,190,110);

	if(pDlgMain->m_DeviceType == DEVICE_IGS_CP)
	{
		m_chkSameWL.ShowWindow(FALSE);

		m_staticProbe.ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_REALTIP)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_VIRTUALTIP)->ShowWindow(TRUE);
		m_lblLengthUnit.ShowWindow(TRUE);
		m_edtOffset.ShowWindow(TRUE);
		m_btnSubOffset.ShowWindow(TRUE);
		m_btnAddOffset.ShowWindow(TRUE);
		m_btnScrollOffset.ShowWindow(TRUE);

		m_btnTracking.ShowWindow(TRUE);
		m_btnSelect.ShowWindow(TRUE);
		m_btnCalibrateImage.ShowWindow(FALSE);
		top+=222;
		m_btnSelectFusion.MoveWindow(left,top+4,80,40);
		m_chkShowFusion.MoveWindow(left,top+50,50,20);
	}else
	{
		m_chkSameWL.MoveWindow(left,top+82,300,20);
		top+=222;
		m_chkSameWL.ShowWindow(TRUE);

		m_staticProbe.ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_REALTIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_VIRTUALTIP)->ShowWindow(FALSE);
		m_lblLengthUnit.ShowWindow(FALSE);
		m_edtOffset.ShowWindow(FALSE);
		m_btnSubOffset.ShowWindow(FALSE);
		m_btnAddOffset.ShowWindow(FALSE);
		m_btnScrollOffset.ShowWindow(FALSE);

		m_btnTracking.ShowWindow(TRUE);
		m_btnSelect.ShowWindow(TRUE);
		m_btnCalibrateImage.ShowWindow(FALSE);
		m_btnSelectFusion.MoveWindow(left,top+4,80,40);
		m_chkShowFusion.MoveWindow(left,top+50,50,20);
	}
}

void CDlgNavigate::OnButtonViewAv() 
{
	// TODO: Add your control notification handler code here
	
	m_newViewMode=TVIEW_AV;
	
}

void CDlgNavigate::OnButtonViewAsvp() 
{
	// TODO: Add your control notification handler code here
	
	m_newViewMode=TVIEW_ASPV;
	
}

void CDlgNavigate::OnButtonViewAst() 
{
	// TODO: Add your control notification handler code here
	
	m_newViewMode=TVIEW_AST;
}

void CDlgNavigate::OnButtonViewTapv() 
{
	// TODO: Add your control notification handler code here
	
	m_newViewMode=TVIEW_TAPV;
}




void CDlgNavigate::OnButtonSelectFusion() 
{
	// TODO: Add your control notification handler code here
	CDlgSelectSeries dlgSeries;
	if (dlgSeries.DoModal() == IDOK)
	{
		if(dlgSeries.m_sName==m_sSeriesName)
		{
			PanaMessageBox("Fusion Series should be different with Navigating Series.");
			return;
		}else
		{
			m_sFusionSeriesName=dlgSeries.m_sName;
			MPR(m_sFusionSeriesName);
			m_chkShowFusion.EnableWindow(TRUE);
		}

	}
}

void CDlgNavigate::OnCheckShowFusion() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bShowFusion)
	{
		CDlgMain *pDlgMain = g_DlgMain;
		pDlgMain->m_dlgFusion.ShowWindow(TRUE);
	}else
	{
		CDlgMain *pDlgMain = g_DlgMain;
		pDlgMain->m_dlgFusion.ShowWindow(FALSE);
	}
}

void CDlgNavigate::ResliceFusion()
{
	if(m_sFusionSeriesName=="")
		return;
	if(!m_bShowFusion)
		return;


	CDlgMain *pDlgMain = g_DlgMain;

	CMRISeries *pMRISeries = pDlgMain->m_dlgCryotherapy.GetMRISeries(m_sFusionSeriesName);
	if(!pMRISeries)
		return ;
	
	Matrix mFusion = pMRISeries->GetFusionCoord();

	CImageReslice reslice;
	if(!reslice.InitSeries(pMRISeries))
	{
		pDlgMain->m_dlgCryotherapy.m_dlgTrackingStatus.AddMessage("Failed to initialize image reconstruction function.");
		return;
	}

	Matrix mProbeInScanner(4,4);
	if(!GetProbeInScanner(mProbeInScanner,pMRISeries))
		return;

	Matrix mImageInScanner(4,4),mScannerInImage(4,4),mProbeInImage(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mImageInScanner.value[i][j]=reslice.m_dOldImageMatrix[i][j];
		}
	}
	mScannerInImage=mImageInScanner.Inverse();
	mProbeInImage=mScannerInImage * mProbeInScanner;
		
	LPUS_STRUCT_IMAGE pUSData;

	double dCenter[3];
	dCenter[0]=mProbeInScanner.value[0][3];
	dCenter[1]=mProbeInScanner.value[1][3];
	dCenter[2]=mProbeInScanner.value[2][3];
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][1];
		vz.value[1]=mProbeInImage.value[1][1];
		vz.value[2]=mProbeInImage.value[2][1];
		dCenter[0] = dCenter[0] - vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] - vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] - vz.value[2]*m_iLookDepth;
	}
	
	//Axis
	if(pDlgMain->m_dlgFusion.m_fusionMode==CDlgFusion::FUSION_AXIAL)
	{
		int iIndex;
		double dis=9999;
		double xp,yp,zp;
		
		
		dis=9999;
		for(i=0;i<pMRISeries->GetNumOfImages();i++)
		{
			double *dImageCenter=pMRISeries->GetImageAt(i)->GetCenterInScanner();
			double *dImageFreq=pMRISeries->GetImageAt(i)->GetFreqDirectionInScanner();
			double *dImagePhas=pMRISeries->GetImageAt(i)->GetPhaseDirectionInScanner();
			Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
			Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
			Vector vSlice= vFreq * vPhas;
			double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
			CPGMath math;
			math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
				vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
			double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
				+(dCenter[2]-zp)*(dCenter[2]-zp));
			if(distance<dis)
			{
				dis=distance;
				iIndex=i;
			}
		}
		pUSData=GetUSImage(pMRISeries,iIndex);
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgFusion.SetImage(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
	}

	//Sagittal
	if(pDlgMain->m_dlgFusion.m_fusionMode==CDlgFusion::FUSION_SAGITAL)
	{
		CString sTmp;
		CString sName=pMRISeries->GetName();		
		CMRISeries *pNewData=NULL;
		
		sTmp.Format("MPR_Sag_%s",sName);
		pNewData=pDlgMain->m_dlgCryotherapy.GetMRISeries(sTmp,FALSE);
		if(!pNewData)
		{
			return;
		}

		int iIndex=0;
		double dis=9999;
		double xp,yp,zp;
		
		
		dis=9999;
		for(i=0;i<pNewData->GetNumOfImages();i++)
		{
			double *dImageCenter=pNewData->GetImageAt(i)->GetCenterInScanner();
			double *dImageFreq=pNewData->GetImageAt(i)->GetFreqDirectionInScanner();
			double *dImagePhas=pNewData->GetImageAt(i)->GetPhaseDirectionInScanner();
			Vector vFreq(dImageFreq[0],dImageFreq[1],dImageFreq[2]);
			Vector vPhas(dImagePhas[0],dImagePhas[1],dImagePhas[2]);
			Vector vSlice= vFreq * vPhas;
			double d=vSlice.value[0] * dImageCenter[0] +vSlice.value[1] * dImageCenter[1] +vSlice.value[2] * dImageCenter[2] ;
			CPGMath math;
			math.PointProjection(dCenter[0],dCenter[1],dCenter[2],vSlice.value[0],
				vSlice.value[1],vSlice.value[2],d,&xp,&yp,&zp);
			double distance=sqrt((dCenter[0]-xp)*(dCenter[0]-xp) +(dCenter[1]-yp)*(dCenter[1]-yp)
				+(dCenter[2]-zp)*(dCenter[2]-zp));
			if(distance<dis)
			{
				dis=distance;
				iIndex=i;
			}
		}
		pUSData=GetUSImage(pNewData,iIndex);
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgFusion.SetImage(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;

	}
	
	//probe eye
	double dResliceMatrix[16];
	dCenter[0]=mProbeInImage.value[0][3];
	dCenter[1]=mProbeInImage.value[1][3];
	dCenter[2]=mProbeInImage.value[2][3];
	mProbeInImage.value[0][3]=0;
	mProbeInImage.value[1][3]=0;
	mProbeInImage.value[2][3]=0;
	//if(pDlgMain->m_trackerManager.GetHandler()->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][1];
		vz.value[1]=mProbeInImage.value[1][1];
		vz.value[2]=mProbeInImage.value[2][1];
		dCenter[0] = dCenter[0] - vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] - vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] - vz.value[2]*m_iLookDepth;

		mProbeInImage=mProbeInImage.RotateX(PI/2.0);
		dResliceMatrix[0]=mProbeInImage.value[0][0];
		dResliceMatrix[1]=mProbeInImage.value[0][1];
		dResliceMatrix[2]=mProbeInImage.value[0][2];
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=mProbeInImage.value[1][0];
		dResliceMatrix[5]=mProbeInImage.value[1][1];
		dResliceMatrix[6]=mProbeInImage.value[1][2];
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=mProbeInImage.value[2][0];
		dResliceMatrix[9]=mProbeInImage.value[2][1];
		dResliceMatrix[10]=mProbeInImage.value[2][2];
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=mProbeInImage.value[3][0];
		dResliceMatrix[13]=mProbeInImage.value[3][1];
		dResliceMatrix[14]=mProbeInImage.value[3][2];
		dResliceMatrix[15]=mProbeInImage.value[3][3];
		mProbeInImage=mProbeInImage.RotateX(-PI/2.0);
	}/*else
	{
		Vector vz;
		vz.value[0]=mProbeInImage.value[0][2];
		vz.value[1]=mProbeInImage.value[1][2];
		vz.value[2]=mProbeInImage.value[2][2];
		dCenter[0] = dCenter[0] + vz.value[0]*m_iLookDepth;
		dCenter[1] = dCenter[1] + vz.value[1]*m_iLookDepth;
		dCenter[2] = dCenter[2] + vz.value[2]*m_iLookDepth;

		dResliceMatrix[0]=mProbeInImage.value[0][0];
		dResliceMatrix[1]=mProbeInImage.value[0][1];
		dResliceMatrix[2]=mProbeInImage.value[0][2];
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=mProbeInImage.value[1][0];
		dResliceMatrix[5]=mProbeInImage.value[1][1];
		dResliceMatrix[6]=mProbeInImage.value[1][2];
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=mProbeInImage.value[2][0];
		dResliceMatrix[9]=mProbeInImage.value[2][1];
		dResliceMatrix[10]=mProbeInImage.value[2][2];
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=mProbeInImage.value[3][0];
		dResliceMatrix[13]=mProbeInImage.value[3][1];
		dResliceMatrix[14]=mProbeInImage.value[3][2];
		dResliceMatrix[15]=mProbeInImage.value[3][3];
	}*/
	if(pDlgMain->m_dlgFusion.m_fusionMode==CDlgFusion::FUSION_PROBEEYE)
	{
		reslice.SetResliceAxis(dResliceMatrix);
		pUSData=(LPUS_STRUCT_IMAGE)reslice.Reslice2D();
		if(!pUSData)
		{
			return;
		}
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		
		Transform(pUSData,mFusion);
		pDlgMain->m_dlgFusion.SetImage(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
	}

	//Probe Axis
	//if(pDlgMain->m_trackerManager.GetHandler()->GetDeviceType()==TRACK_DEVICE_NDI_OPTIC)
	{
		Vector3D xAxis,yAxis,zAxis;
		if(pDlgMain->m_dlgTracking.m_dlgVirtualUS.IsSliding())
		{
			xAxis.x=mProbeInImage.value[0][0];
			xAxis.y=mProbeInImage.value[1][0];
			xAxis.z=mProbeInImage.value[2][0];
			yAxis.x=mProbeInImage.value[0][1];
			yAxis.y=mProbeInImage.value[1][1];
			yAxis.z=mProbeInImage.value[2][1];
			zAxis.x=mProbeInImage.value[0][2];
			zAxis.y=mProbeInImage.value[1][2];
			zAxis.z=mProbeInImage.value[2][2];
			Matrix m(4,4);
			m.Identity();
			m.value[0][0]=xAxis.x;
			m.value[1][0]=xAxis.y;
			m.value[2][0]=xAxis.z;
			m.value[0][1]=yAxis.x;
			m.value[1][1]=yAxis.y;
			m.value[2][1]=yAxis.z;
			m.value[0][2]=zAxis.x;
			m.value[1][2]=zAxis.y;
			m.value[2][2]=zAxis.z;
			m=m.RotateY(PI*m_iVirtualUSAngle/180.0);
			xAxis.x=m.value[0][0];
			xAxis.y=m.value[1][0];
			xAxis.z=m.value[2][0];
			yAxis.x=m.value[0][1];
			yAxis.y=m.value[1][1];
			yAxis.z=m.value[2][1];
			zAxis.x=m.value[0][2];
			zAxis.y=m.value[1][2];
			zAxis.z=m.value[2][2];
		}else
		{
			if(fabs(mProbeInImage.value[0][1])>0.71)
			{
				xAxis.x=0;
				xAxis.y=1;
				xAxis.z=0;
				yAxis.x=mProbeInImage.value[0][1];
				yAxis.y=mProbeInImage.value[1][1];
				yAxis.z=mProbeInImage.value[2][1];

			}else
			{
			xAxis.x=1;
			xAxis.y=0;
			xAxis.z=0;
			yAxis.x=mProbeInImage.value[0][1];
			yAxis.y=mProbeInImage.value[1][1];
			yAxis.z=mProbeInImage.value[2][1];
			}
			
			zAxis= xAxis ^ yAxis;
			yAxis= zAxis ^ xAxis;
			zAxis.normalize();
			yAxis.normalize();	
		}
		
		dResliceMatrix[0]=xAxis.x;
		dResliceMatrix[1]=yAxis.x;
		dResliceMatrix[2]=zAxis.x;
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=xAxis.y;
		dResliceMatrix[5]=yAxis.y;
		dResliceMatrix[6]=zAxis.y;
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=xAxis.z;
		dResliceMatrix[9]=yAxis.z;
		dResliceMatrix[10]=zAxis.z;
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=0;
		dResliceMatrix[13]=0;
		dResliceMatrix[14]=0;
		dResliceMatrix[15]=1;
	}/*else
	{
		Vector3D xAxis,yAxis,zAxis;
		if(pDlgMain->m_dlgTracking.m_dlgVirtualUS.IsSliding())
		{
			xAxis.x=mProbeInImage.value[0][0];
			xAxis.y=mProbeInImage.value[1][0];
			xAxis.z=mProbeInImage.value[2][0];
			zAxis.x=-mProbeInImage.value[0][1];
			zAxis.y=-mProbeInImage.value[1][1];
			zAxis.z=-mProbeInImage.value[2][1];
			yAxis.x=mProbeInImage.value[0][2];
			yAxis.y=mProbeInImage.value[1][2];
			yAxis.z=mProbeInImage.value[2][2];
			Matrix m(4,4);
			m.Identity();
			m.value[0][0]=xAxis.x;
			m.value[1][0]=xAxis.y;
			m.value[2][0]=xAxis.z;
			m.value[0][1]=yAxis.x;
			m.value[1][1]=yAxis.y;
			m.value[2][1]=yAxis.z;
			m.value[0][2]=zAxis.x;
			m.value[1][2]=zAxis.y;
			m.value[2][2]=zAxis.z;
			m=m.RotateY(PI*m_iVirtualUSAngle/180.0);
			xAxis.x=m.value[0][0];
			xAxis.y=m.value[1][0];
			xAxis.z=m.value[2][0];
			yAxis.x=m.value[0][1];
			yAxis.y=m.value[1][1];
			yAxis.z=m.value[2][1];
			zAxis.x=m.value[0][2];
			zAxis.y=m.value[1][2];
			zAxis.z=m.value[2][2];
		}else
		{
			xAxis.x=1;
			xAxis.y=0;
			xAxis.z=0;
			yAxis.x=mProbeInImage.value[0][2];
			yAxis.y=mProbeInImage.value[1][2];
			yAxis.z=mProbeInImage.value[2][2];
			
			zAxis= xAxis ^ yAxis;
			yAxis= zAxis ^ xAxis;
			zAxis.normalize();
			yAxis.normalize();	
		}
		

		dResliceMatrix[0]=xAxis.x;
		dResliceMatrix[1]=yAxis.x;
		dResliceMatrix[2]=zAxis.x;
		dResliceMatrix[3]=dCenter[0];
		dResliceMatrix[4]=xAxis.y;
		dResliceMatrix[5]=yAxis.y;
		dResliceMatrix[6]=zAxis.y;
		dResliceMatrix[7]=dCenter[1];
		dResliceMatrix[8]=xAxis.z;
		dResliceMatrix[9]=yAxis.z;
		dResliceMatrix[10]=zAxis.z;
		dResliceMatrix[11]=dCenter[2];
		dResliceMatrix[12]=0;
		dResliceMatrix[13]=0;
		dResliceMatrix[14]=0;
		dResliceMatrix[15]=1;
	}*/
	
	if(pDlgMain->m_dlgFusion.m_fusionMode==CDlgFusion::FUSION_VIRTUALUS)
	{
		reslice.SetResliceAxis(dResliceMatrix);
		pUSData=(LPUS_STRUCT_IMAGE)reslice.Reslice2D();
		if(!pUSData)
		{
			return;
		}	
		
		ComputeOrientChar(pUSData);
		CorrectImageDispOrientation();
		

		Transform(pUSData,mFusion);
		pDlgMain->m_dlgFusion.SetImage(pUSData,m_sDispOri,m_ppcOrientation);
		delete pUSData->pData;
		delete pUSData;
		
	}
}

void CDlgNavigate::OnButtonFreeze() 
{
	// TODO: Add your control notification handler code here
	m_bFreezing = !m_bFreezing;
	if(m_bFreezing)
	{
		m_btnFreeze.SetWindowText("Freezing");
	}else
	{
		m_btnFreeze.SetWindowText("Freeze");
	}
}

void CDlgNavigate::OnRadioRealtip() 
{
	// TODO: Add your control notification handler code here
	m_bCurTipType=0;
	if(m_bCurTipType==0) //real tip
	{			
		if (m_iPushDepth >MAX_PUSH_DEPTH)
		{
			m_iPushDepth=MAX_PUSH_DEPTH;
		}
		if (m_iPushDepth <MIN_PUSH_DEPTH)
		{
			m_iPushDepth=MIN_PUSH_DEPTH;
		}
		SetPushDepth(m_iPushDepth);
		m_iPushDepth = GetPushDepth();

		CString str;
		double dLength = m_dStandardProbeLength+m_iPushDepth;
		str.Format("%.1f",dLength);
		m_edtOffset.SetWindowText(str);		
	}else
	{
		double value=m_iLookDepth+MIN_PUSH_DEPTH;
		if(value>MAX_PUSH_DEPTH)
		{
			value=MAX_PUSH_DEPTH;
		}
		if (value <MIN_PUSH_DEPTH)
		{
			value=MIN_PUSH_DEPTH;
		}
		SetPushDepth(value);
		value = GetPushDepth();
		m_iLookDepth=value-MIN_PUSH_DEPTH;

		CString str;
		str.Format("%i",m_iLookDepth);
		m_edtOffset.SetWindowText(str);	
	}
}

void CDlgNavigate::OnRadioVirtualtip() 
{
	// TODO: Add your control notification handler code here
	m_bCurTipType=1;
	if(m_bCurTipType==0) //real tip
	{			
		if (m_iPushDepth >MAX_PUSH_DEPTH)
		{
			m_iPushDepth=MAX_PUSH_DEPTH;
		}
		if (m_iPushDepth <MIN_PUSH_DEPTH)
		{
			m_iPushDepth=MIN_PUSH_DEPTH;
		}
		SetPushDepth(m_iPushDepth);
		m_iPushDepth = GetPushDepth();

		CString str;
		double dLength = m_dStandardProbeLength+m_iPushDepth;
		str.Format("%.1f",dLength);
		m_edtOffset.SetWindowText(str);		
	}else
	{
		double value=m_iLookDepth+MIN_PUSH_DEPTH;
		if(value>MAX_PUSH_DEPTH)
		{
			value=MAX_PUSH_DEPTH;
		}
		if (value <MIN_PUSH_DEPTH)
		{
			value=MIN_PUSH_DEPTH;
		}
		SetPushDepth(value);
		value = GetPushDepth();
		m_iLookDepth=value-MIN_PUSH_DEPTH;

		CString str;
		str.Format("%i",m_iLookDepth);
		m_edtOffset.SetWindowText(str);	
	}
}
