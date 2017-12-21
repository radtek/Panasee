// TPS3DView.cpp : implementation file
//

#include "stdafx.h"

#include "TPS3DView.h"
#include "DisplayDlg.h"
#include "Commands.h"
#include "GalilIceBallDistribute.h"
#include "ExtendVTKClass.h"
#include "PanaGlobal.h"
#include "GlobalManager.h"
#include <XTToolkitPro.h>
#include "Points2Mesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPS3DView dialog


CTPS3DView::CTPS3DView(CWnd* pParent /*=NULL*/)
	: CDialog(CTPS3DView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTPS3DView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bEnableFlyTo=FALSE;

	m_outline = vtkActor::New();
	
	m_pMFCWindow = NULL;
	m_pRenderer=NULL;
	m_pOMW = vtkOrientationMarkerWidget::New(); 
	
	
	m_pPropPicker = vtkPropPicker::New();
	m_pCorAnnotate = vtkCornerAnnotation::New();

	m_pFrmMgr=NULL;

	m_pImageActor1=vtkImageActor::New();
	m_pImageActor2=vtkImageActor::New();
	m_pImageActor3=vtkImageActor::New();
	m_pProbeActors=vtkProp3DCollection::New();

	m_pImageActorMPR1=vtkImageActor::New();
	m_pImageActorMPR2=vtkImageActor::New();

	m_pointPicker=vtkVolumePicker::New();
	m_pointPicker->SetTolerance(1e-6);
	m_pointPicker->SetVolumeOpacityIsovalue(0.05);
	//m_pointPicker->UseVolumeGradientOpacityOn();
	//m_pointPicker->PickClippingPlanesOn();

	m_bMouseDown = FALSE;
}

CTPS3DView::~CTPS3DView()
{
	
	m_nonClientRegion.DeleteObject();
}

void CTPS3DView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPS3DView)
	DDX_Control(pDX, IDC_BUTTON_TOPWND_SHOW, m_btnTopWndShow);
	DDX_Control(pDX, IDC_BUTTON_SELECT_IMAGE, m_btnSelectImage);
	DDX_Control(pDX, IDC_BUTTON_PRE_IMAGE, m_btnPreImage);
	DDX_Control(pDX, IDC_BUTTON_NEXT_IMAGE, m_btnNextImage);
	DDX_Control(pDX, IDC_BUTTON_LAST_IMAGE, m_btnLastImage);
	DDX_Control(pDX, IDC_BUTTON_FIRST_IMAGE, m_btnFirstImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPS3DView, CDialog)
	//{{AFX_MSG_MAP(CTPS3DView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(IDR_POPUPMENU_2DVIEW_DISPLAY2DTOP, OnPopupmenu2dviewDisplay2dtop)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_MAX, OnPopupmenuDisplayMax)
	ON_COMMAND(IDR_POPMENU_2DVIEW_DISPLAYAVERAGE, OnPopmenu2dviewDisplayaverage)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_RETCAMERA, OnPopupmenuDisplayRetcamera)
	ON_COMMAND(IDR_POPUPMENU_VIEW3D_DISPIMAGE1, OnPopupmenuView3dDispimage1)
	ON_COMMAND(IDR_POPUPMENU_VIEW3D_DISPIMAGE2, OnPopupmenuView3dDispimage2)
	ON_COMMAND(IDR_POPUPMENU_VIEW3D_DISPIMAGE3, OnPopupmenuView3dDispimage3)
	ON_COMMAND(IDR_POPUPMENU_MEASURE_DIS_3D, OnPopupmenuMeasureDis3d)
	ON_BN_CLICKED(IDC_BUTTON_FIRST_IMAGE, OnButtonFirstImage)
	ON_BN_CLICKED(IDC_BUTTON_PRE_IMAGE, OnButtonPreImage)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_IMAGE, OnButtonNextImage)
	ON_BN_CLICKED(IDC_BUTTON_LAST_IMAGE, OnButtonLastImage)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE, OnButtonSelectImage)
	ON_BN_CLICKED(IDC_BUTTON_TOPWND_SHOW, OnButtonTopwndShow)
	ON_COMMAND(IDR_POPUPMENU_VIEW3D_FLYTO, OnPopupmenuView3dFlyto)
	ON_COMMAND(IDR_POPUPMENU_VIEW3D_BKCOLOR, OnPopupmenuView3dBkcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPS3DView message handlers

BOOL CTPS3DView::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetFirstImageBtn(&m_btnFirstImage);
	SetPreImageBtn(&m_btnPreImage);
	SetNextImageBtn(&m_btnNextImage);
	SetLastImageBtn(&m_btnLastImage);
	SetSelectImageBtn(&m_btnSelectImage,1);
	SetTopWndBtn(&m_btnTopWndShow);
	// TODO: Add extra initialization here
	
	this->m_pMFCWindow = new vtkMFCWindow(this);
	this->m_pRenderer = vtkRenderer::New();
	this->m_pMFCWindow->GetRenderWindow()->AddRenderer(this->m_pRenderer);
	
	m_pInteractor = this->m_pMFCWindow->GetInteractor();
	vtkInteractorStyleTrackballCamera* pStyle = vtkInteractorStyleTrackballCamera::New();
	m_pInteractor->SetInteractorStyle(pStyle);
	pStyle->Delete();
	
	
	
	m_pRenderer->SetBackground(0.0,0.0,0);
    m_pRenderer->ResetCamera(); 
	
	m_pRenderer->SetRenderWindow(m_pMFCWindow->GetRenderWindow());
	
	SetCallBack();
	CreatCornnerAxes();
	m_pRenderer->AddActor(m_pImageActor1);
	m_pRenderer->AddActor(m_pImageActor2);	
	m_pRenderer->AddActor(m_pImageActor3);
	m_pRenderer->AddActor(m_pImageActorMPR1);
	m_pRenderer->AddActor(m_pImageActorMPR2);
	
	m_bMeasureProbeTip[0]=FALSE;
			
	vtkTextProperty *tprop = m_pCorAnnotate->GetTextProperty();
	tprop->SetColor(1,1,0);
	tprop->SetFontSize(1);
	tprop->SetBold(0);
	tprop->SetFontFamilyToTimes();
	m_pRenderer->AddActor2D(m_pCorAnnotate);
	
	
	int * nSize;
	nSize = GetRenderer()->GetSize();
	
	m_curImage=1;
	vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
	outlineData->SetInput(m_pImageActor1->GetInput());
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
    mapOutline->SetInputConnection(outlineData->GetOutputPort());
	
    m_outline->SetMapper(mapOutline);
    m_outline->GetProperty()->SetColor(0,1,0);
	//GetRenderer()->AddActor(m_outline);

	outlineData->Delete();
	mapOutline->Delete();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTPS3DView::OnEraseBkgnd(CDC* pDC)
{

 	return TRUE;
}

void CTPS3DView::OnPaint() 
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

void CTPS3DView::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pMFCWindow)
	{
		m_pMFCWindow->MoveWindow(0, 0, cx, cy);	
		
		int * nSize;
		nSize = GetRenderer()->GetSize();
		/*
		m_pResetView->SetDisplayPosition(nSize[0]/2 - 65,nSize[1]-21);
		m_pSelectImage->SetDisplayPosition(nSize[0]/2 - 35,nSize[1]-21);
		m_pPrePage->SetDisplayPosition(nSize[0]/2 - 5,nSize[1]-20);
		m_pNextPage->SetDisplayPosition(nSize[0]/2 +25,nSize[1]-20);
		m_pUpTop->SetDisplayPosition(nSize[0]/2 +55,nSize[1]-21);*/

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

		m_btnSelectImage.MoveWindow(iLeft,rc.Height()-rcSelf.Height(),
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
}

void CTPS3DView::OnDestroy() 
{
	
	// TODO: Add your message handler code here
	POSITION pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	while(pos)
	{
		CModal* modal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		delete modal;
	}
	CGlobalManager::Manager()->m_modalGrpList.RemoveAll();
	
	if (m_pRenderer)
	{
		m_pRenderer->RemoveAllObservers();
		m_pRenderer->RemoveAllViewProps();
		m_pRenderer->Delete();
	}
	
	if (m_pMFCWindow)
	{
		m_pMFCWindow->GetRenderWindow()->Finalize();
		delete m_pMFCWindow;
	}
	
	if (m_pOMW)
	{
		m_pOMW->Delete();
		m_pOMW = NULL;
	}

	m_pImageActor1->Delete();
	m_pImageActor2->Delete();
	m_pImageActor3->Delete();
	m_pImageActorMPR1->Delete();
	m_pImageActorMPR2->Delete();
	
		
	if (m_pProbeActors)
	{
		m_pProbeActors->RemoveAllItems();
		m_pProbeActors->Delete();
		m_pProbeActors = NULL;
	}

	if (m_pPropPicker)
	{
		m_pPropPicker->Delete();
	}	
	m_pointPicker->Delete();
	if (m_pCorAnnotate)	{	m_pCorAnnotate->Delete();	m_pCorAnnotate = NULL;	}

	/*
	m_pResetView->Delete();
	m_pPrePage->Delete();
	m_pNextPage->Delete();
	m_pSelectImage->Delete();
	m_pUpTop->Delete();*/
	
	m_outline->Delete();
	
	CDialog::OnDestroy();
}

void CTPS3DView::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == this)
		return;
		
	switch(nCmdID)
	{
	case CMD_TPS_RENDERREFRESHALL:
		Invalidate(FALSE);
		break;
	case CMD_TPS_PROBE_SETUCS:
		OnProbeSetUcs(sPara);
		break;
	case CMD_TPS_PROBE_COPYUCS:
		OnProbeCopyUcs(sPara);
		break;
		
	case CMD_TPS_GETMODALVISIBLESTATUS:
		OnGetModalVisibleStatus(sPara);
		break;
	case CMD_TPS_SETMODALVISIBLE:
		OnSetModalVisible(sPara);
		break;
	case CMD_TPS_DELETEMODAL:
		OnDeleteModal(sPara);
		break;
	case CMD_TPS_REMOVEALL:
		RemoveAll();
		break;
	case CMD_TPS_RENDERREFRESH:
		RefreshPoly();
		break;
	case CMD_TPS_CREATEMODAL:
		OnCreateModal(sPara);
		
		break;
	case CMD_TPS_ISACTOR:
		OnIsActor(sPara);
		break;
	default:
		break;
	}
}


void CTPS3DView::CreatCornnerAxes()
{
	vtkAxesActor *pAxes = vtkAxesActor::New();
	pAxes->SetShaftTypeToCylinder();
	pAxes->SetXAxisLabelText("x");
	pAxes->SetYAxisLabelText("y");
	pAxes->SetZAxisLabelText("z");
	pAxes->SetTotalLength(50, 50, 50);
	pAxes->SetAxisLabels(1);

	m_pOMW->SetOrientationMarker(pAxes);
	m_pOMW->SetInteractor(m_pInteractor);
	m_pOMW->EnabledOn();
	m_pOMW->InteractiveOff();
	//pAxes->SetPosition(0,0,0);
	//pAxes->SetOrientation(0,0,0);
	//m_pRenderer->AddActor(pAxes);

	pAxes->Delete();

}

void CTPS3DView::SetCallBack()
{
	vtkCallbackCommand* callback1 = vtkCallbackCommand::New();
	callback1->SetCallback(CTPS3DView::MouseDownCallBack);
	callback1->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::LeftButtonPressEvent,callback1);
	callback1->Delete();
	
	

	vtkCallbackCommand* callback2 = vtkCallbackCommand::New();
	callback2->SetCallback(KeyPressCallBack);
	callback2->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyPressEvent, callback2);
	callback2->Delete();
	
	vtkCallbackCommand* callback3 = vtkCallbackCommand::New();
	callback3->SetCallback(KeyReleaseCallBack);
	callback3->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyReleaseEvent,callback3);
	callback3->Delete();

	vtkCallbackCommand* callback4 = vtkCallbackCommand::New();
	callback4->SetCallback(RenderAbortCheckCallBack);
	callback4->SetClientData(this);
	m_pMFCWindow->GetRenderWindow()->AddObserver(vtkCommand::AbortCheckEvent, callback4);
	callback4->Delete();

	vtkCallbackCommand* callback5 = vtkCallbackCommand::New();
	callback5->SetCallback(CTPS3DView::RightButtonDownCallBack);
	callback5->SetClientData(this);
	callback5->AbortFlagOnExecuteOn();
	m_pInteractor->AddObserver(vtkCommand::RightButtonPressEvent,callback5);
	callback5->Delete();

}


void CTPS3DView::RefreshPoly()
{
	//
	GetRenderer()->Render();
	Invalidate(FALSE);
}

void CTPS3DView::RightButtonDownCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata)
{
	
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	
	CTPS3DView* m_pView = reinterpret_cast<CTPS3DView*>( clientdata );
	CMenu menu,*pSubMenu;

	//iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
	CString sPara;
	if (iren )  //&& airen->GetShiftKey())
	{
		POSITION pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		CModal* pModal;
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			if(pModal->m_modalType==ModalVolume)
			{
				pModal->m_pVolume->SetPickable(0);
			}
		}

		m_pView->m_pPropPicker->PickProp(iren->GetEventPosition()[0],iren->GetEventPosition()[1], m_pView->GetRenderer());
		
		pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			if(pModal->m_modalType==ModalVolume)
			{
				pModal->m_pVolume->SetPickable(1);
			}
		}
		
		vtkProp3D *pProp3D;
		pProp3D= m_pView->m_pPropPicker->GetProp3D();
		vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();		
		
		CGlobalManager::Manager()->ReleaseAssemblys(TOOL);
		CGlobalManager::Manager()->ReleaseAssemblys(TRACER);
		CGlobalManager::Manager()->ReleaseAssemblys(MODAL);
		
		
		vtkActor *pActor;
		pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			pActor = (vtkActor*)pModal->m_actor;
			if(pActor==pProp3D)
			{
				CGlobalManager::Manager()->AddAssemblys(pProp3D,MODAL);
				break;
			}
		}	


		m_pView->m_pProbeActors->InitTraversal();		
		for(int i=0;i<m_pView->m_pProbeActors->GetNumberOfItems();i++)
		{
			CProbeCryo * pAssembly;
			pAssembly = (CProbeCryo*)m_pView->m_pProbeActors->GetNextProp3D();
			if (pAssembly == pProp3D||pAssembly->probe4==pProp3D)
			{	
				CGlobalManager::Manager()->AddAssemblys(pAssembly,TOOL);					
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
		iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
		return;
	}

	LPPOINT lpoint=new tagPOINT;
	::GetCursorPos(lpoint);
	
	menu.LoadMenu(IDR_POPUPMENU_VIEW3D);
	pSubMenu=menu.GetSubMenu(0);
	
  	if(CGlobalManager::Manager()->m_2DView1->m_bDisplayMeasure)
			pSubMenu->CheckMenuItem(IDR_POPUPMENU_DISPLAY_DISPLAYMEASURE,MF_CHECKED|MF_BYCOMMAND);
	if(m_pView->m_pImageActor1->GetVisibility())
			pSubMenu->CheckMenuItem(IDR_POPUPMENU_VIEW3D_DISPIMAGE1,MF_CHECKED|MF_BYCOMMAND);
	if(m_pView->m_pImageActor2->GetVisibility())
			pSubMenu->CheckMenuItem(IDR_POPUPMENU_VIEW3D_DISPIMAGE2,MF_CHECKED|MF_BYCOMMAND);
	if(m_pView->m_pImageActor3->GetVisibility())
			pSubMenu->CheckMenuItem(IDR_POPUPMENU_VIEW3D_DISPIMAGE3,MF_CHECKED|MF_BYCOMMAND);
		
		
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,m_pView);
		
	
	delete lpoint;

	iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
	
	
}

void CTPS3DView::RenderAbortCheckCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	CTPS3DView* m_pView = reinterpret_cast<CTPS3DView*>( clientdata );
	vtkRenderWindowInteractor* iren = m_pView->m_pInteractor;
	if(m_pView->m_pRenderer->GetRenderWindow()->GetEventPending()!= 0)
	{
        m_pView->m_pRenderer->GetRenderWindow()->SetAbortRender(1);
	}
	
}


void CTPS3DView::MouseDownCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata)
{
	
	
	CTPS3DView* m_pView = reinterpret_cast<CTPS3DView*>( clientdata );
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	
	
	CString sTmp;
	
	if (iren && m_pView->m_bEnableFlyTo)
	{
		if(m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, 
			m_pView->m_pRenderer)==0)
			return;
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		
		m_pView->m_pInteractor->SetDolly(1.0);
		m_pView->m_pInteractor->FlyTo(m_pView->m_pRenderer,pos[0],pos[1],pos[2]);
		
		m_pView->m_bEnableFlyTo=FALSE;
		return;
	}	
	
	if (iren&&CGlobalManager::Manager()->IsLocateProbeTip())
	{
		if(m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer())==0)
			return;
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		
		CGlobalManager::Manager()->ChangeProbePosition(pos,0); //change tip postion of selected probe
		
		return;
	}

	if (iren&&CGlobalManager::Manager()->IsLocateProbeTail())
	{
		if(m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer())==0)
			return;
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		
		CGlobalManager::Manager()->ChangeProbePosition(pos,1); //change tip postion of selected probe
		
		return;
	}

	if (iren )  //&& airen->GetShiftKey())
	{
		POSITION pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		CModal* pModal;
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			if(pModal->m_modalType==ModalVolume)
			{
				pModal->m_pVolume->SetPickable(0);
			}
		}

		m_pView->m_pPropPicker->PickProp(iren->GetEventPosition()[0],iren->GetEventPosition()[1], m_pView->GetRenderer());
		
		pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			if(pModal->m_modalType==ModalVolume)
			{
				pModal->m_pVolume->SetPickable(1);
			}
		}

		vtkProp3D *pProp3D;
		pProp3D= m_pView->m_pPropPicker->GetProp3D();
		vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();		
				
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		double worldPos[3];
		m_pView->m_pointPicker->GetPickPosition(worldPos);

		if(m_pView->IsOnImage(worldPos,1)&&iren->GetShiftKey())
			m_pView->OnSelectImage(1);
		else if(m_pView->IsOnImage(worldPos,2)&&iren->GetShiftKey())
			m_pView->OnSelectImage(2);
		else if(m_pView->IsOnImage(worldPos,3)&&iren->GetShiftKey())
			m_pView->OnSelectImage(3);
		//else
			//m_pView->OnSelectImage(0);

		CGlobalManager::Manager()->ReleaseAssemblys(TOOL);
		CGlobalManager::Manager()->ReleaseAssemblys(TRACER);
		CGlobalManager::Manager()->ReleaseAssemblys(MODAL);
		
		
		if(pProp3D==NULL)
		{
			
			m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
			return;
		}

		vtkActor *pActor;
		pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
		while(pos)
		{
			pModal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
			pActor = (vtkActor*)pModal->m_actor;
			if(pActor==pProp3D)
			{
				CGlobalManager::Manager()->AddAssemblys(pProp3D,MODAL);
				break;
			}
		}	

		m_pView->m_pProbeActors->InitTraversal();
		
		for(int i=0;i<m_pView->m_pProbeActors->GetNumberOfItems();i++)
		{
			CProbeCryo * pAssembly;
			pAssembly = (CProbeCryo*)m_pView->m_pProbeActors->GetNextProp3D();
			if (pAssembly == pProp3D||pAssembly->probe4==pProp3D)
			{	
				CGlobalManager::Manager()->AddAssemblys(pAssembly,TOOL);
				
			
				if(m_pView->m_bMeasureProbeTip[0]==TRUE)
				{
					double measure[3],m_measure1[3],m_measure2[3];
					if(m_pView->m_bMeasureProbeTip[1]==FALSE)
					{
						((CProbeCryo *)pAssembly)->GetTipPosInWorld(measure);
						m_pView->m_Measure1[0]=measure[0];
						m_pView->m_Measure1[1]=measure[1];
						m_pView->m_Measure1[2]=measure[2];
						m_pView->m_bMeasureProbeTip[1]=TRUE;
						m_pView->m_pCorAnnotate->SetText(2,"Click second probe");
					}else if(m_pView->m_bMeasureProbeTip[2]==FALSE)
					{
						m_pView->m_pCorAnnotate->SetText(2,"");
						((CProbeCryo *)pAssembly)->GetTipPosInWorld(m_measure2);
						m_measure1[0]=m_pView->m_Measure1[0];
						m_measure1[1]=m_pView->m_Measure1[1];
						m_measure1[2]=m_pView->m_Measure1[2];
						double dis;
						dis=sqrt((m_measure1[0]-m_measure2[0])*(m_measure1[0]-m_measure2[0])+
							(m_measure1[1]-m_measure2[1])*(m_measure1[1]-m_measure2[1])+
							(m_measure1[2]-m_measure2[2])*(m_measure1[2]-m_measure2[2]));
						sTmp.Format("Distance between two probe is:%.2fmm",dis);
						m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
						
						m_pView->DisplayMeasureResult(sTmp);
						//iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent,NULL);
						//iren->InvokeEvent(vtkCommand::LeftButtonPressEvent,NULL);
						
						m_pView->m_bMeasureProbeTip[0]=FALSE;
					}
				}
					
				break;
				
			}
		}
		
		m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
	}
	
	
}

void CTPS3DView::KeyPressCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS3DView* m_pView = reinterpret_cast<CTPS3DView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();

}

void CTPS3DView::KeyReleaseCallBack(vtkObject *obj, unsigned long event, void * clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CTPS3DView* m_pView = reinterpret_cast<CTPS3DView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();
	
}

void CTPS3DView::RemoveAll()
{
	
	GetRenderer()->RemoveAllViewProps();
	m_pProbeActors->RemoveAllItems();

	POSITION pos=CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	while(pos)
	{
		CModal* modal=CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		delete modal;
	}
	CGlobalManager::Manager()->m_modalGrpList.RemoveAll();
			
}


void CTPS3DView::OnPopupmenu2dviewDisplay2dtop() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_2DTOP,m_instanceName);
}

void CTPS3DView::OnPopupmenuDisplayMax() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_MAX,m_instanceName);
}

void CTPS3DView::OnPopmenu2dviewDisplayaverage() 
{
	// TODO: Add your command handler code here
	CString m_instanceName;
	m_instanceName.Format("%i",m_instanceIndex);
	SendCommand(CMD_TPS_DISPLAY_AVERAGE,m_instanceName);
}

void CTPS3DView::SendCommand(int nCmdID,CString& sPara)
{
	if(m_pFrmMgr)
	{
		m_pFrmMgr->SendCommand(this, nCmdID,sPara);
	}
}

void CTPS3DView::OnCreateModal(CString sPara)
{
	
	ZONEGROUP_INFO zoneGroup;
	CModalDrag* modalinfo=NULL;
	CString sTmp;
	POSITION pos;
	BOOL bFind=FALSE;

	pos = CGlobalManager::Manager()->m_tracerGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_tracerGrpList.GetCount();i++)
	{
		zoneGroup = CGlobalManager::Manager()->m_tracerGrpList.GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp==sPara)
		{
			modalinfo=new CModalDrag();
			modalinfo->m_color=zoneGroup.color ;
			modalinfo->m_zType = zoneGroup.zType ;
			
			bFind=TRUE;
			break;
		}
	}
	if(!bFind)
		return;

	
	CString sName;
	CModal* tmpModalInfo;
	int j=1;
	sTmp="Modal_1";
	pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		tmpModalInfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",tmpModalInfo->m_name);
		if(sTmp == sName )
		{
			pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
			j++;
			sTmp.Format("Modal_%i",j);
			i=-1;
		}
			
		
	}
	modalinfo->m_name=sTmp;

	vtkPoints * pTmpPoint = vtkPoints::New();
	
	int nCount = 0;
	vtkPolyDataCollection *p2DZonePoly = CGlobalManager::Manager()->m_pZonePoly;
	POLY_INFO polyinfo;
	
	nCount = p2DZonePoly->GetNumberOfItems();
	
	p2DZonePoly->InitTraversal();
	pos = CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	vtkPolyData * pPolyData = NULL;
	for(i=0;i<nCount;i++)
	{
		pPolyData = p2DZonePoly->GetNextItem();
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		if(polyinfo.iIndex!=zoneGroup.groupIndex)
			continue;

		
		vtkPoints * pPoints;
		pPoints = pPolyData->GetPoints();
		int num = pPoints->GetNumberOfPoints();
		for(int j=0;j<pPoints->GetNumberOfPoints();j++)
		{			
			//use contourwidget
			double* pdPoint = pPoints->GetPoint(j);
			pTmpPoint->InsertNextPoint(pdPoint);
			
		}
	}

	if(pTmpPoint->GetNumberOfPoints()<=0)
		return;

	modalinfo->m_opacity=100;
	CreateModal(modalinfo,pTmpPoint);
	pTmpPoint->Delete();

	RefreshPoly();	
}

void CTPS3DView::OnSetModalVisible(CString sPara)
{
	CString sTmp,sName;
	POSITION pos;
	CModal* modalinfo;
	BOOL bFind=FALSE;
	int index;

	pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		modalinfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		if(sPara == sName )
		{
			index=i;
			bFind=TRUE;
			if(modalinfo->m_actor)
			{
				if(modalinfo->m_actor->GetVisibility())
					modalinfo->m_actor->SetVisibility(0);
				else
					modalinfo->m_actor->SetVisibility(1);
			}
			if(modalinfo->m_pVolume)
			{
				if(modalinfo->m_pVolume->GetVisibility())
					modalinfo->m_pVolume->SetVisibility(0);
				else
					modalinfo->m_pVolume->SetVisibility(1);
			}
			break;
		}
			
		
	}
	if(!bFind)
		return;

	RefreshPoly();
	
}

void CTPS3DView::OnIsActor(CString &sPara)
{
	CString sTmp=sPara;
	CString sName;
	POSITION pos;
	CModal* modalinfo;
	BOOL bFind=FALSE;
	
	pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		modalinfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		if(sTmp == sName )
		{
			if(modalinfo->m_actor!=NULL)
			{
				sPara="OK";
			}else
			{
				sPara="";
			}
			break;
		}			
		
	}
		
}

void CTPS3DView::OnDeleteModal(CString sPara)
{
	CString sTmp,sName;
	POSITION pos;
	CModal* modalinfo;
	BOOL bFind=FALSE;
	int index;

	pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		modalinfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		if(sPara == sName )
		{
			index=i;
			
			GetRenderer()->RemoveActor(modalinfo->m_actor);
			GetRenderer()->RemoveVolume(modalinfo->m_pVolume);
			modalinfo->clear();
			CGlobalManager::Manager()->m_modalGrpList.RemoveAt(
				CGlobalManager::Manager()->m_modalGrpList.FindIndex(index));
			delete modalinfo;
			RefreshPoly();
			break;
		}
			
		
	}
		
}

void CTPS3DView::OnGetModalVisibleStatus(CString &sPara)
{
	CString sTmp,sName;
	POSITION pos;
	CModal* modalinfo;
	BOOL bFind=FALSE;
	


	pos = CGlobalManager::Manager()->m_modalGrpList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_modalGrpList.GetCount();i++)
	{
		modalinfo = CGlobalManager::Manager()->m_modalGrpList.GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		if(sPara == sName )
		{
			if(modalinfo->m_actor && modalinfo->m_actor->GetVisibility())
				sPara="VISIBLE";
			else if(modalinfo->m_pVolume && modalinfo->m_pVolume->GetVisibility())
			{
				sPara="VISIBLE";
			}else
			{
				sPara="UNVISIBLE";
			}
			break;
		}
			
		
	}
	return;
}

void CTPS3DView::CreateModal(CModalDrag* pModalInfo, vtkPoints *pPoints)
{
	if(pPoints->GetNumberOfPoints()<=3)
		return;

	PNT3D *pnts=new PNT3D[pPoints->GetNumberOfPoints()];
	for(int j=0;j<pPoints->GetNumberOfPoints();j++)
	{
		
		double* pdPoint = pPoints->GetPoint(j);
		pnts[j].x=pdPoint[0];
		pnts[j].y=pdPoint[1];
		pnts[j].z=pdPoint[2];
		
	}

	CPoints2Mesh mesh;
	mesh.m_meshType=POWERSUFR;
	mesh.m_iCount=pPoints->GetNumberOfPoints();
	mesh.m_pPnts=pnts;
	mesh.m_sTextureFile="";

	vtkActor *pActor=mesh.GetActor();
	delete []pnts;

	double fColor[3];	
	fColor[0] = (float )GetRValue(pModalInfo->m_color)/255.0;
	fColor[1] = (float )GetGValue(pModalInfo->m_color)/255.0;
	fColor[2] = (float )GetBValue(pModalInfo->m_color)/255.0;
		
	pModalInfo->create();
	pModalInfo->m_pPoints->DeepCopy(pPoints);

	pActor->GetProperty()->SetColor(fColor);
	pActor->GetProperty()->SetOpacity(pModalInfo->m_opacity/100.0);
	m_pRenderer->AddActor(pActor);
	
	
	vtkMassProperties * pMassPro = vtkMassProperties::New();
	pMassPro->SetInput(pActor->GetMapper()->GetInput());
	double vol = pMassPro->GetVolume();
	pMassPro->Delete();

	pModalInfo->m_volume =vol/1000.0;
	pModalInfo->m_actor=pActor;
			
	pActor->Delete();	
	
	CGlobalManager::Manager()->m_modalGrpList.AddTail(pModalInfo);
	CString sTmp;
	sTmp=pModalInfo->m_name;
	SendCommand(CMD_TPS_CREATEMODALOVER,sTmp);
	
}

BOOL CTPS3DView::PreTranslateMessage(MSG* pMsg) 
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
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		m_bMouseDown=TRUE;
	}else if(pMsg->message==WM_LBUTTONUP)
	{
		m_bMouseDown=FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTPS3DView::AddIceBall(vtkProp3D *pBall)
{
	
	m_pProbeActors->AddItem(pBall);
	m_pRenderer->AddActor(pBall);
	
	
}

void CTPS3DView::OnPopupmenuDisplayRetcamera() 
{
	// TODO: Add your command handler code here
	m_pRenderer->ResetCamera();
	Invalidate(FALSE);

}



void CTPS3DView::OnPopupmenuView3dDispimage1() 
{
	// TODO: Add your command handler code here
	if(m_pImageActor1->GetVisibility())
		m_pImageActor1->SetVisibility(0);
	else
		m_pImageActor1->SetVisibility(1);
	Invalidate(FALSE);
}

void CTPS3DView::OnPopupmenuView3dDispimage2() 
{
	// TODO: Add your command handler code here
	if(m_pImageActor2->GetVisibility())
		m_pImageActor2->SetVisibility(0);
	else
		m_pImageActor2->SetVisibility(1);
	Invalidate(FALSE);
}

void CTPS3DView::OnPopupmenuView3dDispimage3() 
{
	// TODO: Add your command handler code here
	if(m_pImageActor3->GetVisibility())
		m_pImageActor3->SetVisibility(0);
	else
		m_pImageActor3->SetVisibility(1);
	Invalidate(FALSE);
}

void CTPS3DView::RemoveProbe(CProbeCryo *probe)
{
	vtkAssembly *pAssembly,*pAssemblySub;
	m_pProbeActors->InitTraversal();
	for(int i=0;i<m_pProbeActors->GetNumberOfItems();i++)
	{
		pAssembly=(vtkAssembly*)m_pProbeActors->GetNextProp3D();
		vtkProp3DCollection* pProps = pAssembly->GetParts();
		pProps->InitTraversal();
		for(int jj=0;jj<pProps->GetNumberOfItems();jj++)
		{
			pAssemblySub=(vtkAssembly*)pProps->GetNextProp3D();
			if(pAssembly==probe)
			{
				m_pRenderer->RemoveActor(pAssembly);
				m_pProbeActors->RemoveItem(i);
			
				return;
			}
		}
	}

}


void CTPS3DView::OnPopupmenuMeasureDis3d() 
{
	// TODO: Add your command handler code here
	if(m_bMeasureProbeTip[0]==FALSE)
	{
		m_bMeasureProbeTip[0]=TRUE;
		m_bMeasureProbeTip[1]=FALSE;
		m_bMeasureProbeTip[2]=FALSE;
		m_pCorAnnotate->SetText(2,"Click first probe");
	}
}

void CTPS3DView::DisplayMeasureResult(CString sPara)
{
	PanaMessageBox(sPara);
}

void CTPS3DView::OnSelectImage(int i)
{
	if(i>0)
	{
		m_curImage=i;
	}
	else if(i<0)
	{
		m_curImage++;
		if(m_curImage>3)
			m_curImage=1;
	}

	CString sTmp;
	sTmp.Format("[%i]",m_curImage);
	//m_pSelectImage->SetInput(sTmp);

	vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	if(i!=0)
	{
		if(m_curImage==1)
			outlineData->SetInput(m_pImageActor1->GetInput());
		if(m_curImage==2)
			outlineData->SetInput(m_pImageActor2->GetInput());
		if(m_curImage==3)
			outlineData->SetInput(m_pImageActor3->GetInput());
	
		mapOutline->SetInputConnection(outlineData->GetOutputPort());
	
		m_outline->SetMapper(mapOutline);
		m_outline->GetProperty()->SetColor(0,1,0);
	
		if(m_curImage==1)
			m_outline->SetUserTransform(m_pImageActor1->GetUserTransform());
		if(m_curImage==2)
			m_outline->SetUserTransform(m_pImageActor2->GetUserTransform());
		if(m_curImage==3)
			m_outline->SetUserTransform(m_pImageActor3->GetUserTransform());
		//GetRenderer()->AddActor(m_outline);
	}
	else
	{
		GetRenderer()->RemoveActor(m_outline);
	}

	
	outlineData->Delete();
	mapOutline->Delete();

	SetSelectImageBtn(&m_btnSelectImage,m_curImage);

}

void CTPS3DView::OnProbeSetUcs(CString para)
{
	int i;
	
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==para)
		{
			pAssembly->SetUCS(TRUE);
		}else
		{
			pAssembly->SetUCS(FALSE);
		}
	}
}

void CTPS3DView::OnProbeCopyUcs(CString para)
{
	int i;
	CProbeCryo* pUCS=NULL;
	CProbeCryo* pCur=NULL;

	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->GetUCS())
		{
			pUCS=pAssembly;
		}
		if(pAssembly->m_name==para)
		{
			pCur=pAssembly;
		}
	}
	if(pUCS&&pCur&&pUCS!=pCur)
	{
		double d[3];
		pUCS->GetPosition(d);
		pCur->SetInitPosition(d);
		pUCS->GetOrientation(d);
		pCur->SetOrientation(d);
		pCur->UpdateProbeCoord();
		CGlobalManager::Manager()->PlaneIntersection(pCur);
		Invalidate(FALSE);
		CString sTmp;
		SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
	}

}

BOOL CTPS3DView::IsOnImage(double *pos, int i)
{
	
	double dZpos;
	double position[3];
	double normal[3];
	double dis,offset;
	
	BOOL bReturn=FALSE;
	if(i==1&&CGlobalManager::Manager()->m_2DView1->m_imageInfo.bValid)
	{
		//get current image plane
		dZpos =  CGlobalManager::Manager()->m_2DView1->m_imageInfo.spacing[2]*CGlobalManager::Manager()->m_2DView1->m_pViewer->GetSlice();
	
		position[0]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(0,3)
			+CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(0,2)*dZpos;

		position[1]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(1,3)
			+CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(1,2)*dZpos;

		position[2]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(2,3)
			+CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

		normal[0]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(0,2);
		normal[1]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(1,2);
		normal[2]=CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner->GetElement(2,2);

		offset=position[0]*normal[0]+position[1]*normal[1]+position[2]*normal[2];
		
		//get distance between tip and image plane
		dis=normal[0]*pos[0]+normal[1]*pos[1]+normal[2]*pos[2]-offset;
		
		//set probe color
		if(abs(dis)<=0.5)
		{
			bReturn= TRUE;
		}else
		{
			bReturn= FALSE;
		}
	}

	if(i==2&&CGlobalManager::Manager()->m_2DView2->m_imageInfo.bValid)
	{
		//get current image plane
		dZpos =  CGlobalManager::Manager()->m_2DView2->m_imageInfo.spacing[2]*CGlobalManager::Manager()->m_2DView2->m_pViewer->GetSlice();
	
		position[0]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(0,3)
			+CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(0,2)*dZpos;

		position[1]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(1,3)
			+CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(1,2)*dZpos;

		position[2]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(2,3)
			+CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

		normal[0]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(0,2);
		normal[1]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(1,2);
		normal[2]=CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner->GetElement(2,2);

		offset=position[0]*normal[0]+position[1]*normal[1]+position[2]*normal[2];
		
		//get distance between tip and image plane
		dis=normal[0]*pos[0]+normal[1]*pos[1]+normal[2]*pos[2]-offset;
		
		//set probe color
		if(abs(dis)<=0.5)
		{
			bReturn= TRUE;
		}else
		{
			bReturn= FALSE;
		}
	}

	if(i==3&&CGlobalManager::Manager()->m_2DView3->m_imageInfo.bValid)
	{
		//get current image plane
		dZpos =  CGlobalManager::Manager()->m_2DView3->m_imageInfo.spacing[2]*CGlobalManager::Manager()->m_2DView3->m_pViewer->GetSlice();
	
		position[0]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(0,3)
			+CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(0,2)*dZpos;

		position[1]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(1,3)
			+CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(1,2)*dZpos;

		position[2]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(2,3)
			+CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

		normal[0]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(0,2);
		normal[1]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(1,2);
		normal[2]=CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner->GetElement(2,2);

		offset=position[0]*normal[0]+position[1]*normal[1]+position[2]*normal[2];
		
		//get distance between tip and image plane
		dis=normal[0]*pos[0]+normal[1]*pos[1]+normal[2]*pos[2]-offset;
		
		//set probe color
		if(abs(dis)<=0.5)
		{
			bReturn= TRUE;
		}else
		{
			bReturn= FALSE;
		}
	}

	return bReturn;
	
	
}

void CTPS3DView::OnButtonFirstImage() 
{
	// TODO: Add your control notification handler code here

	if(CGlobalManager::Manager()->m_2DView1!=NULL&&m_curImage==1&&CGlobalManager::Manager()->m_2DView1->m_pViewer)
		CGlobalManager::Manager()->m_2DView1->ChangeSlice(CGlobalManager::Manager()->m_2DView1->m_pViewer->GetSliceMin());
	if(CGlobalManager::Manager()->m_2DView2!=NULL&&m_curImage==2&&CGlobalManager::Manager()->m_2DView2->m_pViewer)
		CGlobalManager::Manager()->m_2DView2->ChangeSlice(CGlobalManager::Manager()->m_2DView2->m_pViewer->GetSliceMin());
	if(CGlobalManager::Manager()->m_2DView3!=NULL&&m_curImage==3&&CGlobalManager::Manager()->m_2DView3->m_pViewer)
		CGlobalManager::Manager()->m_2DView3->ChangeSlice(CGlobalManager::Manager()->m_2DView3->m_pViewer->GetSliceMin());
}

void CTPS3DView::OnButtonPreImage() 
{
	// TODO: Add your control notification handler code here
	if(CGlobalManager::Manager()->m_2DView1!=NULL&&m_curImage==1&&CGlobalManager::Manager()->m_2DView1->m_pViewer)
		CGlobalManager::Manager()->m_2DView1->ChangeSlice(CGlobalManager::Manager()->m_2DView1->m_pViewer->GetSlice()-1);
	if(CGlobalManager::Manager()->m_2DView2!=NULL&&m_curImage==2&&CGlobalManager::Manager()->m_2DView2->m_pViewer)
		CGlobalManager::Manager()->m_2DView2->ChangeSlice(CGlobalManager::Manager()->m_2DView2->m_pViewer->GetSlice()-1);
	if(CGlobalManager::Manager()->m_2DView3!=NULL&&m_curImage==3&&CGlobalManager::Manager()->m_2DView3->m_pViewer)
		CGlobalManager::Manager()->m_2DView3->ChangeSlice(CGlobalManager::Manager()->m_2DView3->m_pViewer->GetSlice()-1);
}

void CTPS3DView::OnButtonNextImage() 
{
	// TODO: Add your control notification handler code here
	if(CGlobalManager::Manager()->m_2DView1!=NULL&&m_curImage==1&&CGlobalManager::Manager()->m_2DView1->m_pViewer)
		CGlobalManager::Manager()->m_2DView1->ChangeSlice(CGlobalManager::Manager()->m_2DView1->m_pViewer->GetSlice()+1);
	if(CGlobalManager::Manager()->m_2DView2!=NULL&&m_curImage==2&&CGlobalManager::Manager()->m_2DView2->m_pViewer)
		CGlobalManager::Manager()->m_2DView2->ChangeSlice(CGlobalManager::Manager()->m_2DView2->m_pViewer->GetSlice()+1);
	if(CGlobalManager::Manager()->m_2DView3!=NULL&&m_curImage==3&&CGlobalManager::Manager()->m_2DView3->m_pViewer)
		CGlobalManager::Manager()->m_2DView3->ChangeSlice(CGlobalManager::Manager()->m_2DView3->m_pViewer->GetSlice()+1);
}

void CTPS3DView::OnButtonLastImage() 
{
	// TODO: Add your control notification handler code here
	if(CGlobalManager::Manager()->m_2DView1!=NULL&&m_curImage==1&&CGlobalManager::Manager()->m_2DView1->m_pViewer)
		CGlobalManager::Manager()->m_2DView1->ChangeSlice(CGlobalManager::Manager()->m_2DView1->m_pViewer->GetSliceMax());
	if(CGlobalManager::Manager()->m_2DView2!=NULL&&m_curImage==2&&CGlobalManager::Manager()->m_2DView2->m_pViewer)
		CGlobalManager::Manager()->m_2DView2->ChangeSlice(CGlobalManager::Manager()->m_2DView2->m_pViewer->GetSliceMax());
	if(CGlobalManager::Manager()->m_2DView3!=NULL&&m_curImage==3&&CGlobalManager::Manager()->m_2DView3->m_pViewer)
		CGlobalManager::Manager()->m_2DView3->ChangeSlice(CGlobalManager::Manager()->m_2DView3->m_pViewer->GetSliceMax());
}

void CTPS3DView::OnButtonSelectImage() 
{
	// TODO: Add your control notification handler code here
	OnSelectImage(-1);
	Invalidate(FALSE);
}

void CTPS3DView::OnButtonTopwndShow() 
{
	// TODO: Add your control notification handler code here
	OnPopupmenu2dviewDisplay2dtop();
}

void CTPS3DView::AddActor(CModal* pModalInfo,vtkActor *pActor)
{

	m_pRenderer->AddActor(pActor);

	CGlobalManager::Manager()->m_modalGrpList.AddTail(pModalInfo);
	CString sTmp;
	sTmp=pModalInfo->m_name;
	SendCommand(CMD_TPS_CREATEMODALOVER,sTmp);
	
}

void CTPS3DView::AddVolume(CModal *pModal, vtkVolume *pVolume)
{
	m_pRenderer->AddVolume(pVolume);

	CGlobalManager::Manager()->m_modalGrpList.AddTail(pModal);
	CString sTmp;
	sTmp=pModal->m_name;
	SendCommand(CMD_TPS_CREATEMODALOVER,sTmp);
}

void CTPS3DView::AddRealProbe(CProbeCryo *probe)
{	
	m_pRenderer->AddActor(probe);
	m_pRenderer->AddActor(probe->probe4);
	
	Invalidate(FALSE);
	UpdateWindow();
}

void CTPS3DView::OnPopupmenuView3dFlyto() 
{
	// TODO: Add your command handler code here
	m_bEnableFlyTo=TRUE;
}

void CTPS3DView::OnPopupmenuView3dBkcolor() 
{
	// TODO: Add your command handler code here
	CXTColorDialog* pColorDlg = new CXTColorDialog(RGB(0,0,0),RGB(200,200,200),
		( (CPS_XT_SHOWHEXVALUE | CPS_XT_SHOW3DSELECTION)), NULL);
	
	if (pColorDlg->DoModal() == IDOK)
	{
		COLORREF clrDlgColor = pColorDlg->GetColor();
		m_pRenderer->SetBackground(GetRValue(clrDlgColor)/255.0,GetGValue(clrDlgColor)/255.0,GetBValue(clrDlgColor)/255.0);				
	}
	
	delete pColorDlg;
	
}
