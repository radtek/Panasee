// TPS2DView.cpp : implementation file
//

#include "stdafx.h"
#include "TPS2DView.h"
#include "GalilIceBallDistribute.h"
#include "Commands.h"
#include "DisplayDlg.h"
#include "ConfigMgr.h"
#include "vtkOrientedGlyphContourRepresentation.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkImageCast.h"
#include "vtkLookupTable.h"
#include "ModalContour.h"
#include "ModalSeg.h"
#include "ModalVolume.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkVertex.h"
#include "vtkPolyLine.h"
#include "vtkCellArray.h"
#include "vtkUnsignedShortArray.h"
#include "PanaGlobal.h"
#include "VImage1.h"
#include "globalDef.h"
#include "Volume.h"
#include "GlobalManager.h"
#include "CTCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTPS2DView dialog
BOOL ConfirmDeleteDlg(CString sText,CString sCaption="");

CTPS2DView::CTPS2DView(CWnd* pParent /*=NULL*/)
	: CDialog(CTPS2DView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTPS2DView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	for(int i=0;i<4;i++)
	{
		m_pOriActor[i]= vtkTextActor::New();
	}
	
	
	m_bDisplayMeasure=TRUE;


	m_imageInfo.bValid=FALSE;
	m_pSeries=NULL;
	
	m_instanceIndex=0;
	
	
	m_pFrmMgr=NULL;
	m_pFrmMgr=NULL;
	m_pRenderer=NULL;
	m_pImageFrame_scanner = vtkMatrix4x4::New();
	
	m_pImageData = NULL;
	m_pImagesMemoryObject=NULL;
	m_pMFCWindow = NULL;
	
	
	
	m_pViewer = NULL;
	
	m_pPicker = vtkCellPicker::New();
	m_pointPicker=vtkWorldPointPicker::New();
	m_pCorAnnotate = vtkCornerAnnotation::New();
	
	m_pPropPicker = vtkPropPicker::New();
	
	m_pRuler = vtkAxisActor2D::New();
	m_pMeasureList=vtkCollection::New();

	contourWidget=NULL;

	m_pBox=CBox::New();
	m_bAutoTrace=0;
	m_bBeginAutoTrace=0;

	m_pTraceFilter=vtkContourFilter::New();
	m_pTraceActor=vtkActor::New();
	m_pTracePolydata=NULL;

}


CTPS2DView::~CTPS2DView()
{
	m_nonClientRegion.DeleteObject();
	
}

void CTPS2DView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPS2DView)
	DDX_Control(pDX, IDC_BUTTON_3D_IMAGE, m_btn3DImage);
	DDX_Control(pDX, IDC_BUTTON_LAST_IMAGE, m_btnLastImage);
	DDX_Control(pDX, IDC_BUTTON_NEXT_IMAGE, m_btnNextImage);
	DDX_Control(pDX, IDC_BUTTON_PRE_IMAGE, m_btnPreImage);
	DDX_Control(pDX, IDC_BUTTON_TOPWND_SHOW, m_btnTopWndShow);
	DDX_Control(pDX, IDC_BUTTON_FIRST_IMAGE, m_btnFirstImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPS2DView, CDialog)
	//{{AFX_MSG_MAP(CTPS2DView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(IDR_POPMENU_2DVIEW_DISPLAYAVERAGE, OnPopmenu2dviewDisplayaverage)
	ON_COMMAND(IDR_POPUPMENU_2DVIEW_DISPLAY2DTOP, OnPopupmenu2dviewDisplay2dtop)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_MAX, OnPopupmenuDisplayMax)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_DISPLAYMEASURE, OnPopupmenuDisplayDisplaymeasure)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_DELETEMEASURE, OnPopupmenuDisplayDeletemeasure)
	ON_COMMAND(IDR_POPUPMENU_MEASURE_DIS, OnPopupmenuMeasureDis)
	ON_COMMAND(IDR_POPUPMENU_MEASURE_ANGLE, OnPopupmenuMeasureAngle)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_2DRESETVIEW, OnPopupmenuDisplay2dresetview)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_RESETWINDOWLEVEL, OnPopupmenuDisplayResetwindowlevel)
	ON_BN_CLICKED(IDC_BUTTON_FIRST_IMAGE, OnButtonFirstImage)
	ON_BN_CLICKED(IDC_BUTTON_PRE_IMAGE, OnButtonPreImage)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_IMAGE, OnButtonNextImage)
	ON_BN_CLICKED(IDC_BUTTON_LAST_IMAGE, OnButtonLastImage)
	ON_BN_CLICKED(IDC_BUTTON_3D_IMAGE, OnButton3dImage)
	ON_BN_CLICKED(IDC_BUTTON_TOPWND_SHOW, OnButtonTopwndShow)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_SEGENT, OnPopupmenuDisplaySegent)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_CONTOUR, OnPopupmenuDisplayContour)
	ON_MESSAGE(WM_CONTOUR_UPDATE,OnContourUpdate)
	ON_MESSAGE(WM_SEGMENT_UPDATE,OnSegmentUpdate)
	ON_MESSAGE(WM_VOLUMERENDER_UPDATE,OnVolumeRenderUpdate)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_VOLUME, OnPopupmenuDisplayVolume)
	ON_MESSAGE(DROPM_DROP,OnDrop)
	ON_MESSAGE(DROPM_DROPEX,OnDropEx)
	ON_MESSAGE(DROPM_DRAGOVER,OnDragOver)
	ON_MESSAGE(WM_TPS_RECEIVE_SERIES,OnReceiveSeries)
	ON_MESSAGE(WM_TPS_PROBE_MOVING,OnProbeMoving)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_MPR, OnPopupmenuDisplayMpr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPS2DView message handlers





void CTPS2DView::KeyPressCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();
	
	
	if ((code=='q'||code=='Q'))
	{
		m_pView->SwitchTracerON();
	}

	if ((code=='w'||code=='W'))
	{
		m_pView->SwitchTracerOFF();
	}

	if ((code=='e'||code=='E'))
	{
		m_pView->CreateTracer();
	}
				
	if((code=='h'||code=='H'))
	{
		if(m_pView->m_pViewer)
		{
			if(m_pView->m_pViewer->GetWindowLevel()->GetLookupTable())
			{
				m_pView->m_pViewer->GetWindowLevel()->SetLookupTable(NULL);
			}else
			{
				// Now create a lookup table that consists of the full hue circle
				// (from HSV).
				vtkLookupTable *hueLut = vtkLookupTable::New();
				hueLut->SetTableRange (0, 2000);
				hueLut->SetHueRange (0, 1);
				hueLut->SetSaturationRange (1, 1);
				hueLut->SetValueRange (1, 1);
				hueLut->Build(); //effective built

				m_pView->m_pViewer->GetWindowLevel()->SetLookupTable(hueLut);
				hueLut->Delete();
			}
		}
	}


	
	if( (iren && CString(cKeysys)=="Prior") || (code=='z'||code=='Z'))
    {
		if(m_pView->m_pViewer!=NULL){
			int nCurrentSlice= m_pView->m_pViewer->GetSlice();
			nCurrentSlice--;
			m_pView->ChangeSlice(nCurrentSlice,FALSE);
		}
	}
	if( (iren && CString(cKeysys)=="Next") || (code=='x'||code=='X'))
	{
		if(m_pView->m_pViewer!=NULL){
			int nCurrentSlice= m_pView->m_pViewer->GetSlice();
			nCurrentSlice++;
			m_pView->ChangeSlice(nCurrentSlice,FALSE);
		}
	}
	

	if(m_pView->m_pViewer!=NULL)
		m_pView->m_pViewer->Render();
}

void CTPS2DView::KeyReleaseCallBack(vtkObject *obj, unsigned long event, void * clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();

	if( (iren && CString(cKeysys)=="Prior") || (code=='z'||code=='Z'))
    {
		if(m_pView->m_pViewer!=NULL){
			m_pView->EventOnChangeSlice();
		}
	}
	if( (iren && CString(cKeysys)=="Next") || (code=='x'||code=='X'))
	{
		if(m_pView->m_pViewer!=NULL){
			m_pView->EventOnChangeSlice();
		}
	}
}

void CTPS2DView::MouseDownCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	
	if(m_pView->m_pViewer&&CGlobalManager::Manager()->m_bCanAddNewTrace)
	{
		m_pView->SwitchTracerON();
		CGlobalManager::Manager()->m_bCanAddNewTrace=FALSE;
	}

	CString sTmp;
	sTmp.Format("%i",m_pView->m_instanceIndex);
	m_pView->SendCommand(CMD_TPS_SETACTIVE_WINDOW,sTmp);
	if (m_pView->m_pViewer&&m_pView->m_bAutoTrace==1)
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		if(m_pView->m_bBeginAutoTrace==1)
		{
			m_pView->m_pBox->pnt1[0]=pos[0];
			m_pView->m_pBox->pnt1[1]=pos[1];
			m_pView->m_pBox->pnt1[2]=pos[2];
			m_pView->m_bBeginAutoTrace=0;
		}else
		{
			m_pView->m_bBeginAutoTrace=1;
		}
		m_pView->m_pBox->Refresh();
	}

	if (m_pView->m_pViewer&&CGlobalManager::Manager()->IsLocateProbeTip())
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);

		
		CGlobalManager::Manager()->ChangeProbePosition(pos,0); //change tip postion of selected probe
		
		return;
	}

	if (m_pView->m_pViewer&&CGlobalManager::Manager()->IsLocateProbeTail())
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);

		
		CGlobalManager::Manager()->ChangeProbePosition(pos,1); //change tip postion of selected probe
		
		return;
	}

	if (m_pView->m_pViewer)
	{		
		
		if (m_pView->m_pViewer->GetRenderWindow()->GetInteractor() ) //&& iren->GetShiftKey())
		{
			m_pView->m_pPropPicker->PickProp(m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[0], 
				m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[1], m_pView->m_pViewer->GetRenderer());
			vtkProp3D* pProp3D = m_pView->m_pPropPicker->GetProp3D();
			vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();
			
			
			
			CGlobalManager::Manager()->ReleaseAssemblys(TOOL);
			CGlobalManager::Manager()->ReleaseAssemblys(TRACER);
			CGlobalManager::Manager()->ReleaseAssemblys(MODAL);
						
			CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
			
			for(int i=0;i<CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems();i++)
			{
				CProbeCryo *pAssembly;
				pAssembly = (CProbeCryo *)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
				if (pAssembly!=NULL&&pAssembly == pProp3D||pAssembly->probe1==pProp3D||pAssembly->probe2==pProp3D
					||pAssembly->probe3==pProp3D||pAssembly->probe4==pProp3D)
				{
					CGlobalManager::Manager()->AddAssemblys(pAssembly,TOOL);
												
					break;
				}
			}
			
			ACTOR_INFO actorinfo;
			POSITION pos=CGlobalManager::Manager()->m_ActorInfoList.GetHeadPosition();
			for(int j=0;j<CGlobalManager::Manager()->m_ActorInfoList.GetCount();j++)
			{
				actorinfo=CGlobalManager::Manager()->m_ActorInfoList.GetNext(pos);
				if(actorinfo.p==pProp3D)
				{
					CGlobalManager::Manager()->AddAssemblys(pProp3D,TRACER);
					
					break;
				}
			}	
			
			
			
		}

		CString sTmp;		
		m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
		
	}
	
}

void CTPS2DView::RightButtonDownCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata)
{
	
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	CMenu menu,*pSubMenu;

	if(!m_pView->m_pViewer)
		return;

	CString sPara;
	iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
	if (m_pView->m_pViewer->GetRenderWindow()->GetInteractor() ) //&& iren->GetShiftKey())
	{
		m_pView->m_pPropPicker->PickProp(m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[0], 
			m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[1], m_pView->m_pViewer->GetRenderer());
		vtkProp3D* pProp3D = m_pView->m_pPropPicker->GetProp3D();
		vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();
				
		CGlobalManager::Manager()->ReleaseAssemblys(TOOL);
		CGlobalManager::Manager()->ReleaseAssemblys(TRACER);
		CGlobalManager::Manager()->ReleaseAssemblys(MODAL);
		
		CGlobalManager::Manager()->m_pAssemblys->InitTraversal();		
		for(int i=0;i<CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems();i++)
		{
			CProbeCryo *pAssembly;
			pAssembly = (CProbeCryo *)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
			if (pAssembly!=NULL&&pAssembly == pProp3D||pAssembly->probe1==pProp3D||pAssembly->probe2==pProp3D
				||pAssembly->probe3==pProp3D||pAssembly->probe4==pProp3D)
			{
				CGlobalManager::Manager()->AddAssemblys(pAssembly,TOOL);
				
				break;
			}
		}
		
		ACTOR_INFO actorinfo;
		POSITION pos=CGlobalManager::Manager()->m_ActorInfoList.GetHeadPosition();
		for(int j=0;j<CGlobalManager::Manager()->m_ActorInfoList.GetCount();j++)
		{
			actorinfo=CGlobalManager::Manager()->m_ActorInfoList.GetNext(pos);
			if(actorinfo.p==pProp3D)
			{
				CGlobalManager::Manager()->AddAssemblys(pProp3D,TRACER);				
				break;
			}
		}	
		
		m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sPara);
		
	}	
	

	
	
	int iCount=0;
	iCount+=CGlobalManager::Manager()->m_pProbeOnDrag->GetNumberOfItems();
	iCount+=CGlobalManager::Manager()->m_pTracerOnDrag->GetNumberOfItems();
	iCount+=CGlobalManager::Manager()->m_pModalOnDrag->GetNumberOfItems();
	if(iCount>0)
	{
		m_pView->SendCommand(CMD_TPS_CONTROLDLG_RIGHTCLICK,sPara);
		return;
	}

	LPPOINT lpoint=new tagPOINT;
	::GetCursorPos(lpoint);
	
	menu.LoadMenu(IDR_POPUPMENU_VIEW);
	pSubMenu=menu.GetSubMenu(0);
	
  	if(m_pView->m_bDisplayMeasure)
			pSubMenu->CheckMenuItem(IDR_POPUPMENU_DISPLAY_DISPLAYMEASURE,MF_CHECKED|MF_BYCOMMAND);
	
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,m_pView);
	
	delete lpoint;

}

void CTPS2DView::MoveMouseCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	if (m_pView->m_pViewer&&m_pView->m_bAutoTrace==1&&m_pView->m_bBeginAutoTrace==0)
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		m_pView->m_pBox->pnt2[0]=pos[0];
		m_pView->m_pBox->pnt2[1]=pos[1];
		m_pView->m_pBox->pnt2[2]=pos[2];
		m_pView->m_pBox->Refresh();
		CString sTmp="";
		m_pView->SetAutoTraceFilter(sTmp);
	}

	if (m_pView->m_imageInfo.bValid)
	{
		int * size = m_pView->GetRenderer()->GetSize();

		m_pView->GetRenderer()->SetDisplayPoint(size[0],size[1],0);
		m_pView->GetRenderer()->DisplayToWorld();
		double world1[4];
		world1[0] = m_pView->GetRenderer()->GetWorldPoint()[0];
		world1[1] = m_pView->GetRenderer()->GetWorldPoint()[1];
		world1[2] = m_pView->GetRenderer()->GetWorldPoint()[2];
		world1[3] = m_pView->GetRenderer()->GetWorldPoint()[3];
		m_pView->GetRenderer()->SetDisplayPoint(size[0],0,0);
		m_pView->GetRenderer()->DisplayToWorld();
		double world2[4];
		world2[0] = m_pView->GetRenderer()->GetWorldPoint()[0];
		world2[1] = m_pView->GetRenderer()->GetWorldPoint()[1];
		world2[2] = m_pView->GetRenderer()->GetWorldPoint()[2];
		world2[3] = m_pView->GetRenderer()->GetWorldPoint()[3];
		double dis;
		dis=sqrt((world1[0]-world2[0])*(world1[0]-world2[0])+
			(world1[1]-world2[1])*(world1[1]-world2[1])+
			(world1[2]-world2[2])*(world1[2]-world2[2]));
		CString str;
		str.Format("%4.0fmm",fabs(dis)*0.3);
		m_pView->m_pRuler->SetTitle(str);

	}

	
	CString sWindowLevel;
	if (m_pView->m_pViewer)
	{
		sWindowLevel.Format("Window: %.0f \nLevel: %.0f ",m_pView->m_pViewer->GetColorWindow(),m_pView->m_pViewer->GetColorLevel());
		m_pView->m_pCorAnnotate->SetText(1,sWindowLevel);

		int iShift = 0;
		int iAlt = 0;
		if (m_pView->m_pViewer->GetRenderWindow()->GetInteractor())
		{
			iShift = m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetShiftKey();
			iAlt = m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetAltKey();
			
			double *ptActual, *ptMapped, *ptPicked;
			int nSlice = m_pView->m_pViewer->GetSlice();
				
			
			m_pView->m_pPicker->Pick(m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[0], 
					m_pView->m_pViewer->GetRenderWindow()->GetInteractor()->GetEventPosition()[1], 
					0, m_pView->m_pViewer->GetRenderer());

			ptActual = m_pView->m_pPicker->GetSelectionPoint();
			ptMapped = m_pView->m_pPicker->GetMapperPosition();
			ptPicked =  m_pView->m_pPicker->GetPickPosition();
			float fPixel = 0;
			double pBounds[4];
			pBounds[0]=0;
			pBounds[1]=m_pView->m_imageInfo.ResolutionX * m_pView->m_imageInfo.spacing[0];
			pBounds[2]=0;
			pBounds[3]=m_pView->m_imageInfo.ResolutionY * m_pView->m_imageInfo.spacing[1];

			if (ptMapped[0]>pBounds[0] && ptMapped[0]<pBounds[1] && ptMapped[1]>pBounds[2] && ptMapped[1]<pBounds[3])
			{
				fPixel = m_pView->m_pImageData->GetScalarComponentAsFloat
				(ptMapped[0]/(m_pView->m_imageInfo.spacing[0]),ptMapped[1]/(m_pView->m_imageInfo.spacing[1]),m_pView->m_pViewer->GetSlice(),0);
			}
				
			char text[120];
			sprintf( text, "X:%5.2f\nY:%5.2f\nZ:%5.2f\nP:%.0f", ptPicked[0], ptPicked[1],ptPicked[2],fPixel);
			m_pView->m_pCorAnnotate->SetText(0,text);
						
		}
		m_pView->m_pViewer->Render();
	}
	
}

void CTPS2DView::ChangeSlice(int iNum,BOOL bTriggerEvent)
{
	if(m_pViewer==NULL)
		return;

	int nMin,nMax;
	m_pViewer->GetSliceRange(nMin,nMax);
	if (iNum>=nMin && iNum<=nMax)
	{
		m_pViewer->SetSlice(iNum);
		CString sText;
		sText.Format("%s\n%i/%i\n",m_imageInfo.ImageName,iNum+1,nMax+1);
		m_pCorAnnotate->SetText(2,sText);	
		
		CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(int i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		CGlobalManager::Manager()->PlaneIntersection(pAssembly,this);
		pAssembly->UpdateProbeColor();
	}

		if(bTriggerEvent)
		{
			EventOnChangeSlice();
		}

		SwitchTracerOFF();		
		
	}
}

void CTPS2DView::OnDestroy() 
{
	m_pTraceFilter->Delete();
	m_pTraceActor->Delete();

	m_pBox->Delete(); m_pBox=NULL;

	// TODO: Add your message handler code here
	
	if(m_pRenderer)
	{
		m_pRenderer->RemoveAllObservers();
		m_pRenderer->RemoveAllViewProps();
		if (m_pRenderer)
		{
			m_pRenderer->Delete();
		}
	}
	
	if (m_pMFCWindow)
	{
		m_pMFCWindow->GetRenderWindow()->Finalize();
		delete m_pMFCWindow;
	}

	

	m_pImageFrame_scanner->Delete();
	
	if (m_pImageData)
	{
		m_pImageData->Delete();
	}
	if(m_pImagesMemoryObject)
	{
		if(m_pImagesMemoryObject->Count()==1)
		{
			delete m_pImagesMemoryObject;
		}else
		{
			m_pImagesMemoryObject->Delete();
		}
		m_pImagesMemoryObject=NULL;
	}

	for(int i=0;i<4;i++)
	{
		m_pOriActor[i]->Delete();
	}
		
	
	if (m_pViewer)	{	m_pViewer->Delete();	m_pViewer = NULL;	}
	if (m_pCorAnnotate)	{	m_pCorAnnotate->Delete();	m_pCorAnnotate = NULL;	}
	if (m_pPicker)	{	m_pPicker->Delete();	m_pPicker = NULL;	}
	if (m_pPropPicker)	{	m_pPropPicker->Delete();	m_pPropPicker = NULL;	}
	m_pointPicker->Delete();
	
	if (m_pRuler)
	{
		m_pRuler->Delete();
	}
	
	if(m_pMeasureList)
	{
		m_pMeasureList->Delete();
	}

	if(contourWidget)
	{
		contourWidget->Delete();
	}
	
	CDialog::OnDestroy();
}

BOOL CTPS2DView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTPS2DView::OnPaint() 
{
	CPaintDC *pDC;
	pDC=new CPaintDC(this); // device context for painting
	
	// TODO: Add your message handler code here	
	if (m_pMFCWindow)
	{
		if (pDC->IsPrinting())
			m_pMFCWindow->DrawDC(pDC);
	}
	delete pDC;
	// Do not call CDialog::OnPaint() for painting messages
}

LRESULT CTPS2DView::OnProbeMoving(WPARAM wParam, LPARAM lParm)
{
	CString sTmp="";
	SendCommand(CMD_TPS_PROBE_MOVING,sTmp);
	return 0;
}


LRESULT CTPS2DView::OnReceiveSeries(WPARAM wParam, LPARAM lParm)
{
	CMRISeries *pSeries=(CMRISeries*)wParam;

	LoadSeries(pSeries);
	return 0;
}

LRESULT CTPS2DView::OnDragOver(WPARAM pDropInfoClass, LPARAM lParm) //拖动到上面时执行
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));
	
	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

LRESULT CTPS2DView::OnDropEx(WPARAM pDropInfoClass, LPARAM lParm)
{
	return (DROPEFFECT)-1;
}

LRESULT CTPS2DView::OnDrop(WPARAM pDropInfoClass, LPARAM lParm)
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));
	
	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
	{
		HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData( CF_TEXT );
		char* lp = (char *)GlobalLock((HGLOBAL) hMem);//lock source
		if (lp != NULL)
		{
			//send message to main window and receive returned message WM_TPS_RECEIVE_SERIES
			CWnd* pWnd = AfxGetMainWnd();
			pWnd->SendMessage(WM_TPS_GET_SERIES,(WPARAM)lp,(LPARAM)this);
			
		}
		GlobalUnlock( hMem );//unlock source
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CTPS2DView::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_dropEx.Register(this);

	SetFirstImageBtn(&m_btnFirstImage);
	SetPreImageBtn(&m_btnPreImage);
	SetNextImageBtn(&m_btnNextImage);
	SetLastImageBtn(&m_btnLastImage);
	Set3DShowImageBtn(&m_btn3DImage);
	SetTopWndBtn(&m_btnTopWndShow);

	m_btn3DImage.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_btn3DImage.SetChecked(TRUE);

	m_iUseAutoTrace=0;
	
	// TODO: Add extra initialization here
	m_pMFCWindow = new vtkMFCWindow(this);
	m_pRenderer = vtkRenderer::New();
	m_pMFCWindow->GetRenderWindow()->AddRenderer(m_pRenderer);
	m_pInteractor = m_pMFCWindow->GetInteractor();
	CExtendInteractorStyleImage *pImageStyle = CExtendInteractorStyleImage::New();
	m_pInteractor->SetInteractorStyle(pImageStyle);
	m_pInteractor->GetInteractorStyle()->On();
	pImageStyle->Delete();

	m_pInteractor->SetPicker(m_pPicker);
	m_pPicker->SetTolerance(0.005);

	vtkCallbackCommand* callback = vtkCallbackCommand::New();
	callback->SetCallback(CTPS2DView::KeyPressCallBack);
	callback->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyPressEvent, callback);
	callback->Delete();
	
	vtkCallbackCommand* callback1 = vtkCallbackCommand::New();
	callback1->SetCallback(CTPS2DView::MouseDownCallBack);
	callback1->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::LeftButtonPressEvent,callback1);
	callback1->Delete();

	vtkCallbackCommand* callback2 = vtkCallbackCommand::New();
	callback2->SetCallback(CTPS2DView::KeyReleaseCallBack);
	callback2->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyReleaseEvent,callback2);
	callback2->Delete();
	
	vtkCallbackCommand* callback3 = vtkCallbackCommand::New();
	callback3->SetCallback(CTPS2DView::MoveMouseCallBack);
	callback3->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::MouseMoveEvent,callback3);
	callback3->Delete();
	
		
	vtkCallbackCommand* callback4 = vtkCallbackCommand::New();
	callback4->SetCallback(CTPS2DView::WheelCallBack);
	callback4->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::MouseWheelForwardEvent,callback4);
	m_pInteractor->AddObserver(vtkCommand::MouseWheelBackwardEvent ,callback4);
	callback4->Delete();
	
	vtkCallbackCommand* callback5 = vtkCallbackCommand::New();
	callback5->SetCallback(CTPS2DView::RightButtonDownCallBack);
	callback5->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::RightButtonPressEvent,callback5);
	callback5->Delete();

		
	vtkObject* pObj = vtkObject::New();
	pObj->GlobalWarningDisplayOff();
	pObj->Delete();

	

	SetAutoTraceActor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTPS2DView::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pMFCWindow)
	{
		
		m_pMFCWindow->MoveWindow(0, 0, cx, cy);	
		int * nSize;
		nSize = GetRenderer()->GetSize();
		m_pOriActor[0]->SetDisplayPosition(nSize[0]/2,nSize[1]-40);
		m_pOriActor[1]->SetDisplayPosition(nSize[0]-25,nSize[1]/2);
		//m_pOriActor[1]->SetVisibility(0);
		m_pOriActor[2]->SetDisplayPosition(nSize[0]/2,25);
		m_pOriActor[3]->SetDisplayPosition(5,nSize[1]/2);
		

		CRect rc,rcSelf;
		GetClientRect(&rc);
		m_btnFirstImage.GetClientRect(&rcSelf);
		int iLeft=60;
		CRect btnRegion;
		btnRegion.left=iLeft; btnRegion.top=rc.Height()-rcSelf.Height();

		m_btnFirstImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		m_btnPreImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		m_btnNextImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		m_btnLastImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		m_btn3DImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		m_btnTopWndShow.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
		iLeft=iLeft+rcSelf.Width();

		
		btnRegion.right=iLeft;
		btnRegion.bottom=btnRegion.top+rcSelf.Height();

		CRgn rgnTmp;
		rgnTmp.CreateRectRgn(btnRegion.left,btnRegion.top,btnRegion.right,btnRegion.bottom);

		CRect rcClient;
		GetClientRect(rcClient);
		m_nonClientRegion.DeleteObject();
		m_nonClientRegion.CreateRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);
		m_nonClientRegion.CombineRgn(&m_nonClientRegion,&rgnTmp,RGN_DIFF);

		rgnTmp.DeleteObject();		
		
	}
	
	if(cx<400)
	{
		SwitchTracerOFF();
	}

	if(cx<300&&m_pCorAnnotate)
	{
		m_pCorAnnotate->SetVisibility(0);
	}else
	{
		m_pCorAnnotate->SetVisibility(1);
	}
	
}

void CTPS2DView::CreateTracer()
{
	POLY_INFO polyinfo;
	ZONEGROUP_INFO zoneGroup;
	
	zoneGroup=CGlobalManager::Manager()->m_defaultTracerGroup;
			
	if(contourWidget==NULL)
		return;
	
	sprintf(polyinfo.sDscr,"Tracer");
	
	polyinfo.iSliceIndex = m_pViewer->GetSlice();
	polyinfo.iIndex = zoneGroup.groupIndex ;
	polyinfo.imageIndex=m_instanceIndex;
	
	vtkOrientedGlyphContourRepresentation *pRep;
	vtkPolyData *pData;
	pRep=(vtkOrientedGlyphContourRepresentation *)contourWidget->GetRepresentation();
	pData=pRep->GetContourRepresentationAsPolyData();
	
	if(pData->GetPoints()==NULL)
		return;
	
	CGlobalManager::Manager()->CreateTracer(polyinfo,pData,this);
	SwitchTracerOFF();
				
	
	return;

	
}


void CTPS2DView::RemoveAll()
{
	if (!m_pViewer==NULL)
	{		
		m_pViewer->SetInput(NULL);
		m_pViewer->m_cbk->IV=NULL;
		m_pViewer->Delete();
		m_pViewer = NULL;
	}

	if (!m_pImageData==NULL)
	{
		m_pImageData->Delete();
		if(m_pImagesMemoryObject)
		{
			if(m_pImagesMemoryObject->Count()==1)
			{
				delete m_pImagesMemoryObject;
			}else
			{
				m_pImagesMemoryObject->Delete();
			}
		}
		m_pImagesMemoryObject=NULL;
		m_pImageData = NULL;
	}
	m_imageInfo.bValid=FALSE;
	m_pSeries=NULL;

	GetRenderer()->RemoveAllViewProps();

	CGlobalManager::Manager()->m_PolyInfoList.RemoveAll();	
	CGlobalManager::Manager()->m_ActorInfoList.RemoveAll();
	CGlobalManager::Manager()->m_pZonePoly->RemoveAllItems();
	CGlobalManager::Manager()->m_pLabels->RemoveAllItems();
	CGlobalManager::Manager()->m_pAssemblys->RemoveAllItems();
	CGlobalManager::Manager()->m_pProbeOnDrag->RemoveAllItems();
	CGlobalManager::Manager()->m_pTracerOnDrag->RemoveAllItems();
	CGlobalManager::Manager()->m_pModalOnDrag->RemoveAllItems();
			
	vtkAbstractWidget* pWidget;
	m_pMeasureList->InitTraversal();
	for	(int i= 0;i<m_pMeasureList->GetNumberOfItems();i++)
	{
		pWidget=(vtkAbstractWidget* )m_pMeasureList->GetNextItemAsObject();
		pWidget->SetInteractor(NULL);
		

	}
	m_pMeasureList->RemoveAllItems();
}




int CTPS2DView::SetupImageViewer()
{
	
	GetRenderer()->SetBackground(0.0,0.0,0.0);
    
	m_pViewer->GetImageActor()->SetOpacity(0.7);
	m_pCorAnnotate->SetImageActor(m_pViewer->GetImageActor());
	m_pCorAnnotate->ShowSliceAndImageOn();

	vtkTextProperty *tprop = m_pCorAnnotate->GetTextProperty();
	tprop->SetColor(1,1,1);
	tprop->SetFontSize(1);
	tprop->SetBold(0);
	tprop->SetFontFamilyToTimes();

	m_pRenderer->AddActor2D(m_pCorAnnotate);
	m_pViewer->SetInput(m_pImageData);

	
	double dRange[2];
	dRange[0]=0;
	dRange[1]=3000;
	m_pViewer->SetColorWindow(dRange[1]-dRange[0]);
	m_pViewer->SetColorLevel(dRange[0]+(dRange[1]-dRange[0])/2.0);

	m_pViewer->SetRenderWindow(m_pMFCWindow->GetRenderWindow());
	m_pViewer->SetRenderer(m_pRenderer);
	m_pViewer->SetupInteractor(m_pInteractor); 
	
	m_pInteractor->Initialize();
		
	vtkTextProperty* pTextPro = vtkTextProperty::New();
	pTextPro->SetFontSize(12);
	pTextPro->SetFontFamilyToTimes();
	pTextPro->SetBold(0);
	pTextPro->SetColor(1,1,1);
	
	for(int i=0;i<4;i++)
	{
		
		m_pOriActor[i]->SetTextProperty(pTextPro);
		m_pOriActor[i]->SetInput(m_imageInfo.MRIOriChar[i]);
		GetRenderer()->AddActor(m_pOriActor[i]);
	}

	

	int * nSize;
	nSize = GetRenderer()->GetSize();
	m_pOriActor[0]->SetDisplayPosition(nSize[0]/2,nSize[1]-40);
	m_pOriActor[1]->SetDisplayPosition(nSize[0]-25,nSize[1]/2);
	//m_pOriActor[1]->SetVisibility(0);
	m_pOriActor[2]->SetDisplayPosition(nSize[0]/2,25);
	m_pOriActor[3]->SetDisplayPosition(5,nSize[1]/2);
	
		

	m_pViewer->GetImageActor()->SetOrigin(m_pViewer->GetImageActor()->GetBounds()[1]/2,
		m_pViewer->GetImageActor()->GetBounds()[3]/2,0);
	int iParallelFlag = GetRenderer()->GetActiveCamera()->GetParallelProjection();
	double dScale = GetRenderer()->GetActiveCamera()->GetParallelScale(); 
	vtkCamera *cam=GetRenderer()->GetActiveCamera();
		
	m_pViewer->Render();  //删掉次句会导致勾画功能不正常。原因未知。

	m_pRuler->SetPoint1(0.99,0.9);
	m_pRuler->SetPoint2(0.99,0.6);
	m_pRuler->SetNumberOfLabels(6);
	m_pRuler->SetLabelFormat("%2.0f");
	m_pRuler->SetLabelVisibility(0);
	m_pRuler->SetTickVisibility(1);
	m_pRuler->SetTickLength(10); 
	m_pRuler->SetMinorTickLength(0);
	m_pRuler->SetNumberOfMinorTicks(10);
	m_pRuler->SetTitleTextProperty(pTextPro);
	m_pRuler->SetTickOffset(2);
	GetRenderer()->AddActor(m_pRuler);

	CString sText;
	sText.Format("%s\n1/%i\n",m_imageInfo.ImageName,m_imageInfo.NumOfImage);
	m_pCorAnnotate->SetText(2,sText);	
		
	pTextPro->Delete();

	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	for(i=0;i<CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems();i++)
	{
		CProbeCryo *pAssembly;
		pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		GetRenderer()->AddActor(pAssembly);
		if(m_instanceIndex==1)
		{
			GetRenderer()->AddActor(pAssembly->probe1);
			GetRenderer()->AddActor(pAssembly->cross1);
		}
		if(m_instanceIndex==2)
		{
			GetRenderer()->AddActor(pAssembly->probe2);
			GetRenderer()->AddActor(pAssembly->cross2);
		}
		if(m_instanceIndex==3)
		{
			GetRenderer()->AddActor(pAssembly->probe3);
			GetRenderer()->AddActor(pAssembly->cross3);
		}
		pAssembly->UpdateProbeCoord(FALSE);
		
	}
	

	m_pBox->pImage=m_pViewer->GetWindowLevel()->GetOutput();
	m_pBox->pMatrix=m_pImageFrame_scanner;
	m_pBox->pViewer=m_pViewer;
	m_pRenderer->AddActor(m_pBox);
	
	m_pRenderer->AddActor(m_pTraceActor);
	return 0;
}




void CTPS2DView::SwitchTracerOFF()
{	
	if(contourWidget)
	{
		contourWidget->Off();
		contourWidget->SetInteractor(NULL);
		contourWidget->Delete();
		contourWidget=NULL;

		Invalidate(FALSE);
	}

}

void CTPS2DView::SwitchTracerON()
{
	if(contourWidget)
	{
		contourWidget->Off();
		contourWidget->SetInteractor(NULL);
		contourWidget->Delete();
		contourWidget=vtkContourWidget::New();
	}else
	{
		contourWidget=vtkContourWidget::New();
	}
	
	((vtkOrientedGlyphContourRepresentation *)contourWidget->GetRepresentation())
		->GetLinesProperty()->SetColor(1,0,0);
		
	contourWidget->SetInteractor(m_pInteractor);
	
	contourWidget->On();
	
}

void CTPS2DView::SetImageDisplayEnv()
{
	
	if(m_imageInfo.bVFlip)
	{
		if(m_imageInfo.bHFlip)
			m_imageInfo.bHFlip=FALSE;
		else
			m_imageInfo.bHFlip=TRUE;
	}

	m_dZRange=(m_pViewer->GetSliceMax()-m_pViewer->GetSliceMin())*m_imageInfo.spacing[2];

	vtkTransform* pTransform = vtkTransform::New();
	pTransform->SetMatrix(m_pImageFrame_scanner);
	m_pViewer->GetImageActor()->SetUserTransform(pTransform);
	pTransform->Delete();

	vtkTransform* pTrans2 = vtkTransform::New();
	pTrans2->SetMatrix(m_pImageFrame_scanner);
	pTrans2->RotateZ(180*m_imageInfo.fAngle/3.1415926);
	vtkMatrix4x4 *pM;
	pM=pTrans2->GetMatrix();

	vtkCamera *cam=GetRenderer()->GetActiveCamera();
	double pos[3];
	
	if(m_imageInfo.bHFlip)
	{
		m_dZRange=-m_dZRange-10;
		pos[0]=m_dFirstImageCenter[0]-m_pImageFrame_scanner->GetElement(0,2)*(m_dZRange);
		pos[1]=m_dFirstImageCenter[1]-m_pImageFrame_scanner->GetElement(1,2)*(m_dZRange);
		pos[2]=m_dFirstImageCenter[2]-m_pImageFrame_scanner->GetElement(2,2)*(m_dZRange);
		cam->SetFocalPoint(pos);

		pos[0]=m_dFirstImageCenter[0]+m_pImageFrame_scanner->GetElement(0,2)*1000;
		pos[1]=m_dFirstImageCenter[1]+m_pImageFrame_scanner->GetElement(1,2)*1000;
		pos[2]=m_dFirstImageCenter[2]+m_pImageFrame_scanner->GetElement(2,2)*1000;
	}else
	{
		m_dZRange=10;
		pos[0]=m_dFirstImageCenter[0]-m_pImageFrame_scanner->GetElement(0,2)*m_dZRange;
		pos[1]=m_dFirstImageCenter[1]-m_pImageFrame_scanner->GetElement(1,2)*m_dZRange;
		pos[2]=m_dFirstImageCenter[2]-m_pImageFrame_scanner->GetElement(2,2)*m_dZRange;
		cam->SetFocalPoint(pos);

		pos[0]=m_dFirstImageCenter[0]-m_pImageFrame_scanner->GetElement(0,2)*1000;
		pos[1]=m_dFirstImageCenter[1]-m_pImageFrame_scanner->GetElement(1,2)*1000;
		pos[2]=m_dFirstImageCenter[2]-m_pImageFrame_scanner->GetElement(2,2)*1000;
	}
    cam->SetPosition(pos); 
	if(!m_imageInfo.bVFlip)
	{
		pos[1]=0-pM->GetElement(1,1);
		pos[0]=0-pM->GetElement(0,1);
		pos[2]=0-pM->GetElement(2,1);
	}else
	{
		pos[0]=pM->GetElement(0,1);
		pos[1]=pM->GetElement(1,1);
		pos[2]=pM->GetElement(2,1);
	}
    cam->SetViewUp(pos);

	cam->GetFocalPoint(m_viewFocus);
    cam->GetPosition(m_viewPoint);

	 
	m_pViewer->GetInteractorStyle()->SetAutoAdjustCameraClippingRange(0);
		
	GetRenderer()->GetActiveCamera()->SetParallelScale(170);
	
	
	m_pViewer->UpdateDisplayExtent();
	
	Invalidate(FALSE);
	GetRenderer()->Render();
	
	
	pTrans2->Delete();

	SwitchTracerON();
	SwitchTracerOFF();
	
	
}

void CTPS2DView::WheelCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	CTPS2DView* m_pView = reinterpret_cast<CTPS2DView*>( clientdata );
	int * size = m_pView->GetRenderer()->GetSize();

	m_pView->GetRenderer()->SetDisplayPoint(size[0],size[1],0);
	m_pView->GetRenderer()->DisplayToWorld();
	double world1[4];
	world1[0] = m_pView->GetRenderer()->GetWorldPoint()[0];
	world1[1] = m_pView->GetRenderer()->GetWorldPoint()[1];
	world1[2] = m_pView->GetRenderer()->GetWorldPoint()[2];
	world1[3] = m_pView->GetRenderer()->GetWorldPoint()[3];
	m_pView->GetRenderer()->SetDisplayPoint(size[0],0,0);
	m_pView->GetRenderer()->DisplayToWorld();
	double world2[4];
	world2[0] = m_pView->GetRenderer()->GetWorldPoint()[0];
	world2[1] = m_pView->GetRenderer()->GetWorldPoint()[1];
	world2[2] = m_pView->GetRenderer()->GetWorldPoint()[2];
	world2[3] = m_pView->GetRenderer()->GetWorldPoint()[3];
	double dis;
	dis=sqrt((world1[0]-world2[0])*(world1[0]-world2[0])+
		(world1[1]-world2[1])*(world1[1]-world2[1])+
		(world1[2]-world2[2])*(world1[2]-world2[2]));
	CString str;
	str.Format("%4.0fmm",fabs(dis)*0.3);
	m_pView->m_pRuler->SetTitle(str);
}

void CTPS2DView::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == this)
		return;
		
	switch(nCmdID)
	{
	
	case CMD_TPS_CHANGE_AUTOTRACE_STATUS:
		ChangeAutoTraceStatus(sPara);
		break;
	case CMD_TPS_SETUP_TRACE_FILTER:
		SetAutoTraceFilter(sPara);
		break;
	
	case CMD_TPS_EXTRACT_CONTOUR:
		OnExtractContour(sPara);
		break;	
	case CMD_TPS_REMOVEALL:
		RemoveAll();
		break;	
	case CMD_TPS_DELETE_TRACE:
		SwitchTracerOFF();
		break;
	case CMD_TPS_CREATE_TRACE:
		CreateTracer();
		break;
	
	case CMD_TPS_RENDERREFRESH:
		GetRenderer()->Render();
		Invalidate(FALSE);
		break;
	
	default:
		break;
	}
}



void CTPS2DView::SendCommand(int nCmdID,CString& sPara)
{
	if(m_pFrmMgr)
	{
		m_pFrmMgr->SendCommand(this, nCmdID,sPara);
	}
}



BOOL CTPS2DView::LoadSeries(CString fileName)
{
	fileName.TrimLeft();
	fileName.TrimRight();
	if(fileName=="")
		return FALSE;

	CMRISeries *pSeries=new CMRISeries();
	if(pSeries->Load(fileName)!=0)
	{
		CString sTmp;
		sTmp.Format("Can't open series from %s",fileName);
		PanaMessageBox(sTmp);
		return FALSE;
	}
	//加载融合坐标
	Matrix m(4,4);
	if(pSeries->LoadFusionCoord(fileName,m))
	{
		pSeries->SetFusionCoord(m.value);
	}

	BOOL bResult=LoadSeries(pSeries);
	delete pSeries;
	m_pSeries=NULL;

	return bResult;
}
BOOL CTPS2DView::LoadSeries(CMRISeries* pSeries)
{
	if(pSeries==NULL)
		return FALSE;

	//检查序列是否符合手术规划需求
	//序列基本要求：每一张图像的长宽必须是相等的。
	if(pSeries->GetNumOfImages()<=0)
		return FALSE;

	int iWidth=pSeries->GetImageAt(0)->GetResolutionX();
	int iHeight=pSeries->GetImageAt(0)->GetResolutionY();
	for(int i=1;i<pSeries->GetNumOfImages();i++)
	{
		if(iWidth!=pSeries->GetImageAt(i)->GetResolutionX() ||
			iHeight!=pSeries->GetImageAt(i)->GetResolutionY()) 
		{
			PanaMessageBox("Every image in the series must have same resolution.");
			return FALSE;
		}
	}
	
	BOOL bReturn;
	bReturn = InitImage(pSeries);
	if(!bReturn)
	{
		return FALSE;
	}
	
	CString sTmp;
	sTmp.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_CLEARZONE,sTmp);

	sprintf(m_imageInfo.ImagePath,pSeries->m_sFolder);
	SetupImageViewer();
	SetImageDisplayEnv();
	

	if(CGlobalManager::Manager()->m_3DView)
	{
		
		vtkImageActor *imageActor;
		if(m_instanceIndex==1)
		{
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor1;
		}
			
		if(m_instanceIndex==2)
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor2;
		if(m_instanceIndex==3)
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor3;

		vtkTransform* pTransform = vtkTransform::New();
		pTransform->SetMatrix(m_pImageFrame_scanner);
		imageActor->SetUserTransform(pTransform);
		imageActor->SetInput(m_pViewer->GetWindowLevel()->GetOutput());
			
		imageActor->GetInput()->UpdateInformation();
		int *w_ext = imageActor->GetInput()->GetWholeExtent();
		imageActor->SetDisplayExtent(w_ext);
		imageActor->SetZSlice(0);
		CGlobalManager::Manager()->m_3DView->RefreshPoly();
		
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(imageActor);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->ResetCamera();
		pTransform->Delete();

		if(m_instanceIndex==1)
			CGlobalManager::Manager()->m_3DView->OnSelectImage(1);
		if(m_instanceIndex==2)
			CGlobalManager::Manager()->m_3DView->OnSelectImage(2);
		if(m_instanceIndex==3)
			CGlobalManager::Manager()->m_3DView->OnSelectImage(3);
	}
		
	return TRUE;
}







void CTPS2DView::OnPopmenu2dviewDisplayaverage() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_AVERAGE,m_instanceName);

}

void CTPS2DView::OnPopupmenu2dviewDisplay2dtop() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_2DTOP,m_instanceName);
}

void CTPS2DView::OnPopupmenuDisplayMax() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_MAX,m_instanceName);
}

void CTPS2DView::OnPopupmenuDisplayDisplaymeasure() 
{
	// TODO: Add your command handler code here
	

	vtkAbstractWidget* pWidget;
	m_pMeasureList->InitTraversal();
	for	(int i= 0;i<m_pMeasureList->GetNumberOfItems();i++)
	{
		pWidget=(vtkAbstractWidget* )m_pMeasureList->GetNextItemAsObject();
		if(m_bDisplayMeasure)
		{
			pWidget->SetEnabled(0);
		}
		else
		{
			pWidget->SetEnabled(1);
			
		}
	}
	if(m_bDisplayMeasure)
	{
		
		m_bDisplayMeasure=FALSE;
	}
	else
	{
		
		m_bDisplayMeasure=TRUE;
	}
	
	GetRenderer()->Render();
	Invalidate(FALSE);
}

void CTPS2DView::OnPopupmenuDisplayDeletemeasure() 
{
	// TODO: Add your command handler code here
	
	vtkAbstractWidget* pWidget;
	m_pMeasureList->InitTraversal();
	for	(int i= 0;i<m_pMeasureList->GetNumberOfItems();i++)
	{
		pWidget=(vtkAbstractWidget* )m_pMeasureList->GetNextItemAsObject();
		pWidget->SetInteractor(NULL);
		

	}
	m_pMeasureList->RemoveAllItems();

	
	GetRenderer()->Render();
	Invalidate(FALSE);
}



void CTPS2DView::OnCreateMeasureTool(CString sPara,double* pos)
{
	if(sPara=="Distance")
	{
		CExtendDistanceWidget* m_pDW;
		m_pDW = CExtendDistanceWidget::New();
		vtkPointHandleRepresentation2D *handle = vtkPointHandleRepresentation2D::New();
		handle->GetProperty()->SetColor(1,0,0);

		vtkDistanceRepresentation2D *DisRep = vtkDistanceRepresentation2D::New();
		DisRep->SetHandleRepresentation(handle);

		DisRep->GetAxis()->SetNumberOfMinorTicks(4);
		DisRep->GetAxis()->SetTickLength(9);
		DisRep->GetAxis()->SetTitlePosition(0.2);
		
		m_pDW->SetInteractor(m_pInteractor);
		m_pDW->CreateDefaultRepresentation();
		m_pDW->SetRepresentation(DisRep);
		
		m_pDW->On();
		
		if(pos!=NULL)
		{
			DisRep->InstantiateHandleRepresentation();
			double point[3];
			point[0]=pos[0];
			point[1]=pos[1];
			point[2]=pos[2];
			DisRep->GetPoint1Representation()->SetWorldPosition(point);
			point[0]=pos[3];
			point[1]=pos[4];
			point[2]=pos[5];
			DisRep->GetPoint2Representation()->SetWorldPosition(point);

			DisRep->GetPoint1DisplayPosition(point);
			DisRep->SetPoint1DisplayPosition(point);
			DisRep->GetPoint2DisplayPosition(point);
			DisRep->SetPoint2DisplayPosition(point);

			DisRep->BuildRepresentation(); 
		
			m_pDW->SetWidgetState(2); 
			DisRep->SetVisibility(1);
		}
		
		
		m_pDW->SetEnabled(1);
		
		m_pMeasureList->AddItem(m_pDW);

		DisRep->Delete();
		handle->Delete();
		m_pDW->Delete();
	}

	if(sPara=="Angle")
	{
		CExtendAngleWidget* m_pAW;
		m_pAW = CExtendAngleWidget::New();
		vtkPointHandleRepresentation2D *handle = vtkPointHandleRepresentation2D::New();
		handle->GetProperty()->SetColor(1,0,0);

		vtkAngleRepresentation2D* AngleRep = vtkAngleRepresentation2D::New();
		AngleRep->SetHandleRepresentation(handle);
		
		
		if(pos!=NULL)
		{
			AngleRep->InstantiateHandleRepresentation();
			
			double point[3];
			point[0]=pos[0];
			point[1]=pos[1];
			point[2]=pos[2];
			AngleRep->GetPoint1Representation()->SetWorldPosition(point);
			point[0]=pos[3];
			point[1]=pos[4];
			point[2]=pos[5];
			AngleRep->GetCenterRepresentation()->SetWorldPosition(point);
			point[0]=pos[6];
			point[1]=pos[7];
			point[2]=pos[8];
			AngleRep->GetPoint2Representation()->SetWorldPosition(point);

			AngleRep->GetPoint1DisplayPosition(point);
			AngleRep->SetPoint1DisplayPosition(point);
			AngleRep->GetCenterDisplayPosition(point);
			AngleRep->SetCenterDisplayPosition(point);
			AngleRep->GetPoint2DisplayPosition(point);
			AngleRep->SetPoint2DisplayPosition(point);

			AngleRep->BuildRepresentation();

			m_pAW->SetWidgetState(2); 
			AngleRep->SetVisibility(1);
		} 

		

		m_pAW->SetInteractor(m_pInteractor);
		m_pAW->CreateDefaultRepresentation();
		m_pAW->SetRepresentation(AngleRep);
		
		m_pAW->On();
		m_pAW->SetEnabled(1);

		m_pMeasureList->AddItem(m_pAW);
	
		AngleRep->Delete();
		handle->Delete();
		m_pAW->Delete();
	}
	return ;
}













BOOL CTPS2DView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN){   
		switch(pMsg->wParam){   
		case   VK_ESCAPE:   
			 return   TRUE;   
			 break;
		case   VK_RETURN:   
			return   TRUE;   
			break;
		}   
	}   

	return CDialog::PreTranslateMessage(pMsg);
}

double CTPS2DView::GetImageAngle(CMRISeries *pMRISeries,BOOL &bHFlip,BOOL &bVFlip)
{
	char** sOrientChar;
	//double dPI=3.1415926;
	sOrientChar = pMRISeries->GetImageAt(0)->GetOrientChar();
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

	bHFlip = FALSE;
	bVFlip = FALSE;
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
	return dbAngle;
}



void CTPS2DView::OnExtractContour(CString sPara)
{
	if(m_pImageData==NULL||m_pTracePolydata==NULL)
		return;

	//sPara like "1|125". 1表示针对第一个窗口的调整 
	//125表示轮廓线的特征值
		
	if(sPara.Mid(1,1)!="|")
		return;
	if(atoi(sPara.Left(1))!=m_instanceIndex)
		return;

	double dContour=atof(sPara.Right(sPara.GetLength()-2));

	POLY_INFO polyinfo;
	ZONEGROUP_INFO zoneGroup;

	zoneGroup=CGlobalManager::Manager()->m_defaultTracerGroup;
		
	sprintf(polyinfo.sDscr,"Tracer");
		
	polyinfo.iSliceIndex = m_pViewer->GetSlice();
	polyinfo.iIndex = zoneGroup.groupIndex ;
	polyinfo.imageIndex=m_instanceIndex;
	
	vtkPolyData *pData=vtkPolyData::New();
	pData->DeepCopy(m_pTracePolydata);
	CGlobalManager::Manager()->CreateTracer(polyinfo,pData,this,FALSE);
	pData->Delete();
		
	m_pViewer->Render();
}


void CTPS2DView::OnPopupmenuMeasureDis() 
{
	// TODO: Add your command handler code here
	CString sPara="Distance";
	OnCreateMeasureTool(sPara);
}

void CTPS2DView::OnPopupmenuMeasureAngle() 
{
	// TODO: Add your command handler code here
	CString sPara="Angle";
	OnCreateMeasureTool(sPara);
}









void CTPS2DView::OnPopupmenuDisplay2dresetview() 
{
	// TODO: Add your command handler code here
	
	if(m_pViewer)
	{
		GetRenderer()->GetActiveCamera()->SetFocalPoint(m_viewFocus);
        GetRenderer()->GetActiveCamera()->SetPosition(m_viewPoint);
		GetRenderer()->GetActiveCamera()->SetParallelScale(170);
		m_pViewer->UpdateDisplayExtent();
		GetRenderer()->Render();
		Invalidate(FALSE);
	}
	
}

void CTPS2DView::OnPopupmenuDisplayResetwindowlevel() 
{
	// TODO: Add your command handler code here
	if(m_pViewer&&m_pImageData)
	{
		double dRange[2];
		dRange[0]=0;
		dRange[1]=3000;
		m_pViewer->SetColorWindow(dRange[1]-dRange[0]);
		m_pViewer->SetColorLevel(dRange[0]+(dRange[1]-dRange[0])/2.0);
	}
}





void CTPS2DView::SetAutoTraceActor()
{
	if(m_pBox==NULL)
		return;

	//double x,y;
	//m_pTraceFilter->SetInput(m_pBox->GetBoxImage(x,y));
	m_pTraceFilter->SetNumberOfContours(1);
    m_pTraceFilter->SetValue(0, 100);

	m_pTracePolydata=vtkPolyData::New();

	vtkPolyDataMapper* pPolyMapper = vtkPolyDataMapper::New();
	pPolyMapper->SetInput(m_pTracePolydata);

	m_pTraceActor->SetMapper(pPolyMapper);
	m_pTraceActor->GetProperty()->SetColor(1,1,0);
	m_pTraceActor->SetVisibility(0);


	m_pTracePolydata->Delete(); 
	m_pTracePolydata=NULL;
	pPolyMapper->Delete();
}



void CTPS2DView::SetAutoTraceFilter(CString &sPara)
{
	double dContour;
	double x,y;
	//sPara like "1|125". 1表示针对第一个窗口的调整 
	//125表示轮廓线的特征值
	if(m_pImageData==NULL || m_pViewer==NULL )
		return;
	
	if(sPara!="")
	{
		if(sPara.Mid(1,1)!="|")
			return;
		if(atoi(sPara.Left(1))!=m_instanceIndex)
			return;

		dContour=atof(sPara.Right(sPara.GetLength()-2));
	
		m_pTraceFilter->SetInput(m_pBox->GetBoxImage(x,y));
		m_pTraceFilter->SetNumberOfContours(1);
		m_pTraceFilter->SetValue(0, dContour);
	}else
	{
		m_pTraceFilter->SetInput(m_pBox->GetBoxImage(x,y));
	}

	vtkPolyDataConnectivityFilter *conn=vtkPolyDataConnectivityFilter::New();
    conn->SetInput(m_pTraceFilter->GetOutput());
    conn->SetExtractionModeToLargestRegion();

	conn->Update();
	
	
	//conn过滤器最后产生的结果是一系列的线段集合，需要把这些线段首尾连接起来
	//构成我们需要的轮廓线。因此首先需要检查线段集合是否已经产生
	int iLinesCount=conn->GetOutput()->GetLines()->GetNumberOfCells();
	if(iLinesCount<=0)
	{
		conn->Delete();
		return;
	}
	
	double dZpos;
	double dTolen=0.1;
	//从CContourfilter中取得的轮廓线的点的z值都是0，所以需要变换到正确的z坐标中
	if(m_imageInfo.bHFlip)
		dZpos= (m_pViewer->GetSlice())*(m_imageInfo.spacing[2])+dTolen;
	else
		dZpos= (m_pViewer->GetSlice())*(m_imageInfo.spacing[2])-dTolen;
	
		
	int iCount=0;
	double da[3];
	int iPreCell;
	int iFirstPntID;
	int iPrePntID;
	int iCurCell;
	int iCurPntID;
	vtkTransform* pTrans;
	vtkCell *cell;
	BOOL bFind;
	vtkIdList *cellIds=vtkIdList::New();
	
	
	//从lines中寻找一个起始点iFirstPntID
	bFind=FALSE;
	for(int i=0;i<conn->GetOutput()->GetNumberOfCells();i++)
	{
		if(conn->GetOutput()->GetCellType(i)==VTK_LINE)
		{
			
			cell=conn->GetOutput()->GetCell(i);
			conn->GetOutput()->GetPointCells(cell->PointIds->GetId(0), cellIds);
			
			
				cell->Points->GetPoint(0,da);
				da[0]+=x;
				da[1]+=y;
				da[2]=dZpos;
				iFirstPntID=cell->PointIds->GetId(0);
				iPrePntID=iFirstPntID;
				iPreCell=i;
				bFind=TRUE;
				
				break;
			
		}
	}
	if(!bFind)
	{
		cellIds->Delete();
		conn->Delete();
		return;
	}

	vtkPolyData *pData=vtkPolyData::New();
	vtkPoints *pPoints=vtkPoints::New();

	
	pTrans= vtkTransform::New();
	pTrans->SetMatrix(m_pImageFrame_scanner);
	pTrans->Translate(da);
	pTrans->GetPosition(da);
	pTrans->Delete();

	pPoints->InsertPoint(iCount,da);
	iCount++;
		
	conn->GetOutput()->GetPointCells(iPrePntID, cellIds);
		
	while(cellIds->GetNumberOfIds()>1)
	{
		if(iCount>iLinesCount)
			break;
		
		bFind=FALSE;
		for(int i=0;i<cellIds->GetNumberOfIds();i++)
		{
			iCurCell=cellIds->GetId(i);
			if(conn->GetOutput()->GetCellType(iCurCell)!=VTK_LINE)
				continue;

			if(iCurCell!=iPreCell)
			{
				cell=conn->GetOutput()->GetCell(iCurCell);
				for(int j=0;j<cell->PointIds->GetNumberOfIds();j++)
				{
					iCurPntID=cell->PointIds->GetId(j);
					if(iCurPntID==iFirstPntID&&iCount>2)
					{
						iPreCell=-1;
						bFind=TRUE;
						break;
					}
					if(iCurPntID!=iPrePntID)
					{
						
						cell->Points->GetPoint(j,da);
						da[0]+=x;
						da[1]+=y;
						da[2]=dZpos;
						pTrans= vtkTransform::New();
						pTrans->SetMatrix(m_pImageFrame_scanner);
						pTrans->Translate(da);
						pTrans->GetPosition(da);
						pTrans->Delete();

						pPoints->InsertPoint(iCount,da);
						iCount++;
						iPrePntID=iCurPntID;
						iPreCell=iCurCell;
						bFind=TRUE;
						break;
					}
				}
				if(bFind)
					break;
			}
		}
		if(iPreCell==-1)
			break;
		conn->GetOutput()->GetPointCells(iPrePntID, cellIds);
	}
		
	pData->SetPoints(pPoints);

	vtkPolyLine *pPolygon = vtkPolyLine::New();
	pPolygon->GetPointIds()->SetNumberOfIds(pPoints->GetNumberOfPoints());
	for(i=0;i<pPoints->GetNumberOfPoints();i++)
	{
		pPolygon->GetPointIds()->SetId(i,i);
	}
	pData->Allocate(1,1);
	pData->InsertNextCell(pPolygon->GetCellType(),pPolygon->GetPointIds());

	
	((vtkPolyDataMapper*)m_pTraceActor->GetMapper())->SetInput(pData); 
	m_pTracePolydata=pData;

	
	pPolygon->Delete();
	pData->Delete();
	pPoints->Delete();
	cellIds->Delete();
	conn->Delete();
		
	m_pViewer->Render();

}


void CTPS2DView::ChangeAutoTraceStatus(CString sPara)
{
	CString sTmp;
	int bAutoTrace=atoi(sPara);
	if (m_pViewer&&bAutoTrace==1)
	{
		
		m_bAutoTrace=0;
		m_bBeginAutoTrace=0;
		m_pBox->SetVisibility(0);
		m_pTraceActor->SetVisibility(0);
	}else if (m_pViewer&&bAutoTrace==0)
	{
		
		m_bAutoTrace=1;
		m_bBeginAutoTrace=1;
		m_pBox->SetVisibility(1);
		m_pTraceActor->SetVisibility(1);
	}
}


void CTPS2DView::OnButtonFirstImage() 
{
	// TODO: Add your control notification handler code here
	if(m_pViewer)
		ChangeSlice(m_pViewer->GetSliceMin());
	Invalidate(FALSE);
}

void CTPS2DView::OnButtonPreImage() 
{
	// TODO: Add your control notification handler code here
	if(m_pViewer)
		ChangeSlice(m_pViewer->GetSlice()-1);
	Invalidate(FALSE);
}

void CTPS2DView::OnButtonNextImage() 
{
	// TODO: Add your control notification handler code here
	if(m_pViewer)
		ChangeSlice(m_pViewer->GetSlice()+1);
	Invalidate(FALSE);
}

void CTPS2DView::OnButtonLastImage() 
{
	// TODO: Add your control notification handler code here
	if(m_pViewer)
		ChangeSlice(m_pViewer->GetSliceMax());
	Invalidate(FALSE);
}

void CTPS2DView::OnButton3dImage() 
{
	// TODO: Add your control notification handler code here
	if(!CGlobalManager::Manager()->m_3DView)
		return;
	vtkImageActor *imageActor;
	if(m_instanceIndex==1)
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor1;
	if(m_instanceIndex==2)
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor2;
	if(m_instanceIndex==3)
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor3;

	if(!imageActor)
		return;

	if(imageActor->GetVisibility())
	{
		imageActor->SetVisibility(0);
		m_btn3DImage.SetChecked(FALSE);
	}
	else
	{
		imageActor->SetVisibility(1);
		m_btn3DImage.SetChecked(TRUE);
	}

	CGlobalManager::Manager()->m_3DView->Invalidate(FALSE);
}

void CTPS2DView::OnButtonTopwndShow() 
{
	// TODO: Add your control notification handler code here
	OnPopupmenu2dviewDisplay2dtop();
}

BOOL CTPS2DView::InitImage(CMRISeries* pMRISeries)
{	
	if (!m_pViewer==NULL)
	{
		
		m_pViewer->SetInput(NULL);
		m_pViewer->Delete();
		m_pViewer = CExtendImageView2::New();
	}
	else
	{
		m_pViewer = CExtendImageView2::New();
	}

	if (!m_pImageData==NULL)
	{
		CGlobalManager::Manager()->DeleteVolumeModalByImage(m_pImageData);

		m_pImageData->Delete();
		if(m_pImagesMemoryObject)
		{
			if(m_pImagesMemoryObject->Count()==1)
			{
				delete m_pImagesMemoryObject;
			}else
			{
				m_pImagesMemoryObject->Delete();
			}
		}
		m_pImagesMemoryObject=NULL;
		
		m_pImageData = vtkImageData::New();
	}
	else
	{
		m_pImageData = vtkImageData::New();
	}
	
	GetRenderer()->RemoveAllViewProps();

	int iSize[3];
	iSize[0] = pMRISeries->GetImageAt(0)->GetResolutionX();
	iSize[1] = pMRISeries->GetImageAt(0)->GetResolutionY();
	iSize[2] = pMRISeries->GetNumOfImages();
	if(iSize[2]>150)
	{
		if(PanaMessageBox("Do you want to decrease image number to 150 ?",MB_ICONWARNING|MB_OKCANCEL ) == IDOK)
		{
			iSize[2]=150;
		}
		
	}

	double dPixelSpacing[3];
	dPixelSpacing[0] = pMRISeries->GetImageAt(0)->GetFOVX()/pMRISeries->GetImageAt(0)->GetResolutionX();
	dPixelSpacing[1] = pMRISeries->GetImageAt(0)->GetFOVY()/pMRISeries->GetImageAt(0)->GetResolutionY();
	if(iSize[2]<=1)
	{
		dPixelSpacing[2] = pMRISeries->GetImageAt(0)->GetThickness()+pMRISeries->GetImageAt(0)->GetGap();
	}else
	{
		double *center1=pMRISeries->GetImageAt(0)->GetCenterInScanner();
		double *center2=pMRISeries->GetImageAt(1)->GetCenterInScanner();
		
		dPixelSpacing[2]=sqrt((center1[0]-center2[0])*(center1[0]-center2[0])+
			(center1[1]-center2[1])*(center1[1]-center2[1])+
			(center1[2]-center2[2])*(center1[2]-center2[2]));
	}
	
	m_pImagesMemoryObject=pMRISeries->GetImagesMemory();
	if(m_pImagesMemoryObject==NULL || m_pImagesMemoryObject->m_pDataObject==NULL)
	{
		PanaMessageBox("Failed to load series. Continual memory is not enough.");
		return FALSE;
	}
	m_pImagesMemoryObject->AddOne();
	
	vtkUnsignedShortArray *array =  vtkUnsignedShortArray::New();
	array ->SetArray(m_pImagesMemoryObject->m_pDataObject, iSize[0]*iSize[1]*iSize[2],1) ;
	m_pImageData->GetPointData()->SetScalars(array);
	array->Delete();

	m_pImageData->SetDimensions(iSize[0],iSize[1],iSize[2]);
	m_pImageData->SetScalarType(VTK_UNSIGNED_SHORT);
	m_pImageData->SetSpacing(dPixelSpacing[0], dPixelSpacing[1], dPixelSpacing[2]);
	m_pImageData->SetOrigin(0.0, 0.0, 0.0);

	double dThickNess = pMRISeries->GetImageAt(0)->GetThickness();
	double dGap = pMRISeries->GetImageAt(0)->GetGap();
					
	
	//设置基座标矩阵
	Matrix mScanner=pMRISeries->GetImageInScanner();
	Matrix mFusion=pMRISeries->GetFusionCoord();
	mScanner = mFusion * mScanner;
	for(int i=0;i<4;i++)
	{
		m_pImageFrame_scanner->SetElement(i,0,mScanner.value[i][0]);
		m_pImageFrame_scanner->SetElement(i,1,mScanner.value[i][1]);
		m_pImageFrame_scanner->SetElement(i,2,mScanner.value[i][2]);
		m_pImageFrame_scanner->SetElement(i,3,mScanner.value[i][3]);	
	}		
	double* fPosition = pMRISeries->GetImageAt(0)->GetCenterInScanner(); //Last image position
	Vector vCenter(4);
	vCenter.value[0]=fPosition[0];
	vCenter.value[1]=fPosition[1];
	vCenter.value[2]=fPosition[2];
	vCenter.value[3]=1;
	vCenter = mFusion * vCenter;
	m_dFirstImageCenter[0]=vCenter.value[0];
	m_dFirstImageCenter[1]=vCenter.value[1];
	m_dFirstImageCenter[2]=vCenter.value[2];
	
	CString sTmp;	
	sTmp.Format("%s.%i.%i",pMRISeries->GetStudyID(),pMRISeries->GetSeriesID(),pMRISeries->GetScanID());
	
	
	sprintf(m_imageInfo.ImageName,"%s",sTmp);
	m_imageInfo.FovX=pMRISeries->GetImageAt(0)->GetFOVX();
	m_imageInfo.FovY=pMRISeries->GetImageAt(0)->GetFOVY();
	m_imageInfo.ResolutionX=iSize[0];
	m_imageInfo.ResolutionY=iSize[1];
	m_imageInfo.NumOfImage=iSize[2];
	m_imageInfo.SliceThickness=dThickNess;
	m_imageInfo.SliceGap=dGap;
	m_imageInfo.bValid=TRUE;
	m_pSeries=pMRISeries;
	
	m_imageInfo.fAngle=GetImageAngle(pMRISeries,m_imageInfo.bHFlip,m_imageInfo.bVFlip);
	
	m_imageInfo.spacing[0]=dPixelSpacing[0];
	m_imageInfo.spacing[1]=dPixelSpacing[1];
	m_imageInfo.spacing[2]=dPixelSpacing[2];	
	
	vtkMatrix4x4::DeepCopy(m_imageInfo.imageInScanner,*m_pImageFrame_scanner->Element);

	CString* psDispOri = pMRISeries->GetImageAt(0)->GetDispOrientChar();
	if (psDispOri)
	{
		for (int i=0; i<4; i++)
		{
			sprintf(m_imageInfo.MRIOriChar[i],psDispOri[i]);
		}
	}
		
	/*CCTCalibration cat;
	double mat[16];
	if(cat.GetEMCoord(pMRISeries,mat))
	{
		
		vtkActor* s1=vtkActor::New();
		vtkActor* s2=vtkActor::New();
		vtkActor* s3=vtkActor::New();
		
		vtkSphereSource *pLine = vtkSphereSource::New();
		//pLine->SetResolution(20);
		pLine->SetRadius(2);
		pLine->SetCenter(0,0,0);
		vtkPolyDataMapper* pMapper = vtkPolyDataMapper::New();
		pMapper->ImmediateModeRenderingOn();
		pMapper->SetInputConnection(pLine->GetOutputPort());
		pLine->Delete();
		
		s1->SetMapper(pMapper);
		s2->SetMapper(pMapper);
		s3->SetMapper(pMapper);
		pMapper->Delete();
		
		s1->SetPosition(mat[0],mat[1],mat[2]);
		s2->SetPosition(mat[3],mat[4],mat[5]);
		s3->SetPosition(mat[6],mat[7],mat[8]);
		
		
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s1);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s2);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s3);
		s1->Delete();
		s2->Delete();
		s3->Delete();
	}*/
	/*
	CCTCalibration cat;
	double mat[16];
	if(cat.GetOpticsCoord(pMRISeries,mat))
	{
		
		vtkActor* s1=vtkActor::New();
		vtkActor* s2=vtkActor::New();
		vtkActor* s3=vtkActor::New();
		vtkActor* s4=vtkActor::New();
		
		vtkSphereSource *pLine = vtkSphereSource::New();
		//pLine->SetResolution(20);
		pLine->SetRadius(6);
		pLine->SetCenter(0,0,0);
		vtkPolyDataMapper* pMapper = vtkPolyDataMapper::New();
		pMapper->ImmediateModeRenderingOn();
		pMapper->SetInputConnection(pLine->GetOutputPort());
		pLine->Delete();
		
		s1->SetMapper(pMapper);
		s2->SetMapper(pMapper);
		s3->SetMapper(pMapper);
		s4->SetMapper(pMapper);
		pMapper->Delete();
		
		s1->SetPosition(mat[0],mat[1],mat[2]);
		s2->SetPosition(mat[3],mat[4],mat[5]);
		s3->SetPosition(mat[6],mat[7],mat[8]);
		s4->SetPosition(mat[9],mat[10],mat[11]);
		
		
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s1);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s2);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s3);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(s4);
		s1->Delete();
		s2->Delete();
		s3->Delete();
		s4->Delete();
	}
	*/

	return TRUE;
}

void CTPS2DView::OnPopupmenuDisplaySegent() 
{
	// TODO: Add your command handler code here
	if(!m_pImageData)
		return;

	if(m_imageInfo.NumOfImage<2)
	{
		PanaMessageBox("It must have two images in this series at least.");
		return;
	}

	CVImage vImage;
	IMAGE_EXCHANGE_INFO imageInfo;
	
	unsigned short *data;
	m_pImageData->GetDimensions(imageInfo.dim);
	m_pImageData->GetSpacing(imageInfo.space);
	data=(unsigned short *)m_pImageData->GetScalarPointer(0,0,0);
	imageInfo.data=data;
	sprintf(imageInfo.name,"%s",this->m_imageInfo.ImageName);

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			imageInfo.coord[i][j]=m_pImageFrame_scanner->Element[i][j];
		}
	}

	vImage.ShowSegmentDlg(&imageInfo,this);
}

void CTPS2DView::OnPopupmenuDisplayContour() 
{
	// TODO: Add your command handler code here
	if(!m_pImageData)
		return;

	if(m_imageInfo.NumOfImage<2)
	{
		PanaMessageBox("It must have two images in this series at least.");
		return;
	}

	CVImage vImage;
	IMAGE_EXCHANGE_INFO imageInfo;
	
	unsigned short *data;
	m_pImageData->GetDimensions(imageInfo.dim);
	m_pImageData->GetSpacing(imageInfo.space);
	data=(unsigned short *)m_pImageData->GetScalarPointer(0,0,0);
	imageInfo.data=data;
	sprintf(imageInfo.name,"%s",this->m_imageInfo.ImageName);

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			imageInfo.coord[i][j]=m_pImageFrame_scanner->Element[i][j];
		}
	}

	vImage.ShowContourDlg(&imageInfo,this);
}

LONG CTPS2DView::OnContourUpdate(WPARAM w,LPARAM l)
{
	vtkActor *pActor=(vtkActor*)w;
	IMAGE_CONTOUR_INFO *pInfo=(IMAGE_CONTOUR_INFO*)l;

	CModalContour* pModal=new CModalContour();
	pModal->AddActor(m_instanceIndex,pActor,pInfo);


	return 0;
}

LONG CTPS2DView::OnSegmentUpdate(WPARAM w,LPARAM l)
{
	vtkActor *pActor=(vtkActor*)w;
	MESH_OPTIONS *pOption=(MESH_OPTIONS*)l;

	//避免将同一个actor加入到列表中
	CModal *pModalInfo;
	POSITION pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	while(pos)
	{
		pModalInfo=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		if(pModalInfo->m_actor==pActor)
		{
			return 0;
		}
	}
	
	CModalSeg* pModal=new CModalSeg();
	pModal->AddActor(m_instanceIndex,pActor,*pOption,TRUE,CGlobalManager::Manager()->m_sTpsFileLoaded);

	return 0;
}

void CTPS2DView::OnPopupmenuDisplayVolume() 
{
	// TODO: Add your command handler code here
	if(!m_pImageData)
		return;

	if(m_imageInfo.NumOfImage<2)
	{
		PanaMessageBox("It must have two images in this series at least.");
		return;
	}

	CVImage vImage;
	IMAGE_EXCHANGE_INFO imageInfo;
	
	unsigned short *data;
	m_pImageData->GetDimensions(imageInfo.dim);
	m_pImageData->GetSpacing(imageInfo.space);
	data=(unsigned short *)m_pImageData->GetScalarPointer(0,0,0);
	imageInfo.data=data;
	sprintf(imageInfo.name,"%s",this->m_imageInfo.ImageName);

	vImage.ShowVolumeDlg(&imageInfo,NULL,this);
}

LONG CTPS2DView::OnVolumeRenderUpdate(WPARAM w,LPARAM l)
{
	//get program path where it is running at.
	
	VOLRENDER_CONFIG *pOption=(VOLRENDER_CONFIG*)w;
	
	CModalVolume* pModal=new CModalVolume();
	pModal->m_pRenderWindow=this->m_pMFCWindow->GetRenderWindow();
	pModal->AddVolume(m_instanceIndex,*pOption,TRUE);


	return 0;
}


void CTPS2DView::OnPopupmenuDisplayMpr() 
{
	// TODO: Add your command handler code here
	if(m_imageInfo.bValid==FALSE)
		return;
	if(m_imageInfo.NumOfImage<2)
	{
		PanaMessageBox("It must have two images in this series at least.");
		return;
	}
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_MPR_IMAGE,m_instanceName);
}

void CTPS2DView::AddRealProbe(CProbeCryo *probe)
{
	m_pRenderer->AddActor(probe);
	m_pRenderer->AddActor(probe->probe4);
	Invalidate(FALSE);
}

void CTPS2DView::EventOnChangeSlice()
{
	if(!m_pViewer)
		return;
	
	
	
	ACTOR_INFO actor_info;
	POSITION pos = CGlobalManager::Manager()->m_ActorInfoList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_ActorInfoList.GetCount();i++)
	{
		actor_info = CGlobalManager::Manager()->m_ActorInfoList.GetNext(pos);
		if(actor_info.imageIndex==m_instanceIndex)
		{
			if (actor_info.iSliceIndex == m_pViewer->GetSlice())
			{
				actor_info.p->SetVisibility(1);
				actor_info.pFollower->SetVisibility(0);
			}
			else
			{
				actor_info.p->SetVisibility(0);
				actor_info.pFollower->SetVisibility(0);
			}
		}
	}
	
	if(CGlobalManager::Manager()->m_3DView)
	{
		vtkImageActor *imageActor;
		if(m_instanceIndex==1)
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor1;
		if(m_instanceIndex==2)
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor2;
		if(m_instanceIndex==3)
			imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActor3;
		imageActor->SetZSlice(m_pViewer->GetSlice());
		CGlobalManager::Manager()->m_3DView->RefreshPoly();
	}
	m_pViewer->Render();
}
