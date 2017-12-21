//MPRView.cpp : implementation file
//

#include "stdafx.h"
#include "MPRView.h"
#include "GalilIceBallDistribute.h"
#include "Commands.h"
#include "DisplayDlg.h"
#include "ConfigMgr.h"
#include "vtkOrientedGlyphContourRepresentation.h"
#include "vtkImageCast.h"
#include "vtkLookupTable.h"
#include "ModalContour.h"
#include "ModalSeg.h"
#include "ModalVolume.h"
#include "vtkVertex.h"
#include "vtkPolyLine.h"
#include "vtkCellArray.h"
#include "vtkUnsignedShortArray.h"
#include "PanaGlobal.h"
#include "VImage1.h"
#include "globalDef.h"
#include "Volume.h"
#include "mv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMPRView dialog
BOOL ConfirmDeleteDlg(CString sText,CString sCaption="");

CMPRView::CMPRView(CWnd* pParent /*=NULL*/)
	: CDlgMPRSkin(CMPRView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMPRView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_instanceIndex=-1;
	m_bDisplayMeasure=TRUE;

	m_reslice = vtkImageReslice::New();
	m_imageInfo.bValid=FALSE;
	
	m_pFrmMgr=NULL;
	m_pFrmMgr=NULL;
	m_pRenderer=NULL;
	m_pOldImage2Scanner = vtkMatrix4x4::New();
	m_pNewImage2Scanner = vtkMatrix4x4::New();
	m_pImageData = NULL;
		
	m_pMFCWindow = NULL;
	
	m_pViewer = NULL;
	
	m_pPicker = vtkCellPicker::New();
	m_pointPicker=vtkWorldPointPicker::New();
	m_pCorAnnotate = vtkCornerAnnotation::New();
	
	m_pPropPicker = vtkPropPicker::New();
		
	
	m_pRuler = vtkAxisActor2D::New();
	m_pMeasureList=vtkCollection::New();

	m_dResliceMatrix[0]=1;
	m_dResliceMatrix[1]=0;
	m_dResliceMatrix[2]=0;
	m_dResliceMatrix[3]=0;
	m_dResliceMatrix[4]=0;
	m_dResliceMatrix[5]=1;
	m_dResliceMatrix[6]=0;
	m_dResliceMatrix[7]=0;
	m_dResliceMatrix[8]=0;
	m_dResliceMatrix[9]=0;
	m_dResliceMatrix[10]=1;
	m_dResliceMatrix[11]=0;
	m_dResliceMatrix[12]=0;
	m_dResliceMatrix[13]=0;
	m_dResliceMatrix[14]=0;
	m_dResliceMatrix[15]=1;

	m_pProbe=NULL;

	m_pGlobalManager=CGlobalManager::Manager();
}

CMPRView::~CMPRView()
{
		
}

void CMPRView::DoDataExchange(CDataExchange* pDX)
{
	CDlgMPRSkin::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPRView)
	DDX_Control(pDX, IDC_BUTTON_3D, m_btn3DImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPRView, CDlgMPRSkin)
	//{{AFX_MSG_MAP(CMPRView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_DISPLAYMEASURE, OnPopupmenuDisplayDisplaymeasure)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_DELETEMEASURE, OnPopupmenuDisplayDeletemeasure)
	ON_COMMAND(IDR_POPUPMENU_MEASURE_DIS, OnPopupmenuMeasureDis)
	ON_COMMAND(IDR_POPUPMENU_MEASURE_ANGLE, OnPopupmenuMeasureAngle)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_2DRESETVIEW, OnPopupmenuDisplay2dresetview)
	ON_COMMAND(IDR_POPUPMENU_DISPLAY_RESETWINDOWLEVEL, OnPopupmenuDisplayResetwindowlevel)
	ON_BN_CLICKED(IDC_BUTTON_3D, OnButton3d)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPRView message handlers

void CMPRView::KeyPressCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();
		
}

void CMPRView::KeyReleaseCallBack(vtkObject *obj, unsigned long event, void * clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
	char code = iren->GetKeyCode();
	char* cKeysys = iren->GetKeySym();
	
}

void CMPRView::MouseDownCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
	
	CString sTmp;

	
	if (m_pView->m_pViewer&&m_pView->m_pGlobalManager->IsLocateProbeTip())
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);
		
		sTmp.Format("%.2f,%.2f,%.2f,0",pos[0],pos[1],pos[2]);
		
		m_pView->SendCommand(CMD_TPS_MPR_PROBE_POS,sTmp); //change tip postion of selected probe
		
		return;
	}

	if (m_pView->m_pViewer&&m_pView->m_pGlobalManager->IsLocateProbeTail())
	{
		m_pView->m_pointPicker->Pick(iren->GetEventPosition()[0],iren->GetEventPosition()[1],0, m_pView->GetRenderer());
		
		double pos[3];
		m_pView->m_pointPicker->GetPickPosition(pos);

		sTmp.Format("%.2f,%.2f,%.2f,1",pos[0],pos[1],pos[2]);
		
		m_pView->SendCommand(CMD_TPS_MPR_PROBE_POS,sTmp); //change tip postion of selected probe
		
		return;
	}

	if (iren )  //&& airen->GetShiftKey())
	{
		
		m_pView->m_pPropPicker->PickProp(iren->GetEventPosition()[0],iren->GetEventPosition()[1], m_pView->GetRenderer());
		vtkProp3D *pProp3D;
		pProp3D= m_pView->m_pPropPicker->GetProp3D();
		vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();
				
		m_pView->SendCommand(CMD_TPS_MPR_RELEASE_PROBE,sTmp);

		if(pProp3D==NULL)
		{
			
			m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
			return;
		}


		if (m_pView->m_pProbe && (m_pView->m_pProbe == pProp3D||m_pView->m_pProbe->probe4==pProp3D) )
		{	
			m_pView->SendCommand(CMD_TPS_SELECTPROBE,m_pView->m_pProbe->m_name);
						
		}		
		
		m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
	}

	
	
}

void CMPRView::RightButtonDownCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata)
{
	
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
	CMenu menu,*pSubMenu;

	iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
	
	CString sPara;
	if (iren ) 
	{
		
		m_pView->m_pPropPicker->PickProp(iren->GetEventPosition()[0],iren->GetEventPosition()[1], m_pView->GetRenderer());
		vtkProp3D *pProp3D;
		pProp3D= m_pView->m_pPropPicker->GetProp3D();
		vtkActor2D* pActor2D=m_pView->m_pPropPicker->GetActor2D();
				
		m_pView->SendCommand(CMD_TPS_MPR_RELEASE_PROBE,sPara);

		if (m_pView->m_pProbe && (m_pView->m_pProbe == pProp3D||m_pView->m_pProbe->probe4==pProp3D) )
		{	
			m_pView->SendCommand(CMD_TPS_SELECTPROBE,m_pView->m_pProbe->m_name);						
		}		
		m_pView->SendCommand(CMD_TPS_RENDERREFRESHALL,sPara);		
	}

	
	m_pView->SendCommand(CMD_TPS_MPR_GET_SELECTEDPROBE_COUNT,sPara);
	int iCount=atoi(sPara);
	if(iCount>0)
	{
		m_pView->SendCommand(CMD_TPS_CONTROLDLG_RIGHTCLICK,sPara);
		return;
	}
	
	LPPOINT lpoint=new tagPOINT;
	::GetCursorPos(lpoint);
	
	menu.LoadMenu(IDR_POPUPMENU_MPR_VIEW);
	pSubMenu=menu.GetSubMenu(0);
	
  	if(m_pView->m_bDisplayMeasure)
	{
		pSubMenu->CheckMenuItem(IDR_POPUPMENU_DISPLAY_DISPLAYMEASURE,MF_CHECKED|MF_BYCOMMAND);
	}		
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,m_pView);
	
	delete lpoint;

}

void CMPRView::MoveMouseCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata)
{
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
	
	
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


void CMPRView::OnDestroy() 
{
	if(m_reslice)
	{
		m_reslice->Delete();
		m_reslice=NULL;
	}
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

	

	m_pOldImage2Scanner->Delete();
	m_pNewImage2Scanner->Delete();
		
	
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

	
	CDlgMPRSkin::OnDestroy();
}

BOOL CMPRView::OnEraseBkgnd(CDC* pDC)
{

 	return TRUE;
}


BOOL CMPRView::OnInitDialog() 
{
	CDlgMPRSkin::OnInitDialog();
		
	// TODO: Add extra initialization here

	Set3DShowImageBtn(&m_btn3DImage);
	m_btn3DImage.SetButtonMode(CImageButton::ImageBtn_CheckButton);
	m_btn3DImage.SetChecked(TRUE);

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
	callback->SetCallback(CMPRView::KeyPressCallBack);
	callback->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyPressEvent, callback);
	callback->Delete();
	
	vtkCallbackCommand* callback1 = vtkCallbackCommand::New();
	callback1->SetCallback(CMPRView::MouseDownCallBack);
	callback1->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::LeftButtonPressEvent,callback1);
	callback1->Delete();

	vtkCallbackCommand* callback2 = vtkCallbackCommand::New();
	callback2->SetCallback(CMPRView::KeyReleaseCallBack);
	callback2->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::KeyReleaseEvent,callback2);
	callback2->Delete();
	
	vtkCallbackCommand* callback3 = vtkCallbackCommand::New();
	callback3->SetCallback(CMPRView::MoveMouseCallBack);
	callback3->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::MouseMoveEvent,callback3);
	callback3->Delete();
			
	vtkCallbackCommand* callback4 = vtkCallbackCommand::New();
	callback4->SetCallback(CMPRView::WheelCallBack);
	callback4->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::MouseWheelForwardEvent,callback4);
	m_pInteractor->AddObserver(vtkCommand::MouseWheelBackwardEvent ,callback4);
	callback4->Delete();
	
	vtkCallbackCommand* callback5 = vtkCallbackCommand::New();
	callback5->SetCallback(CMPRView::RightButtonDownCallBack);
	callback5->SetClientData(this);
	m_pInteractor->AddObserver(vtkCommand::RightButtonPressEvent,callback5);
	callback5->Delete();
	
	
	
	vtkObject* pObj = vtkObject::New();
	pObj->GlobalWarningDisplayOff();
	pObj->Delete();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMPRView::OnSize(UINT nType, int cx, int cy) 
{
	CDlgMPRSkin::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pMFCWindow)
	{
		
		m_pMFCWindow->MoveWindow(0, 0, cx, cy);	
						
		CRect rc,rcSelf;
		GetClientRect(&rc);
		m_btn3DImage.GetClientRect(&rcSelf);
		m_btn3DImage.MoveWindow((cx-rcSelf.Width())/2,rc.Height()-rcSelf.Height(),
			rcSelf.Width(),rcSelf.Height());
	}
	

}

void CMPRView::RemoveAll()
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
		m_pImageData = NULL;
	}
	m_imageInfo.bValid=FALSE;

	m_pProbe=NULL;
	GetRenderer()->RemoveAllViewProps();
	
	
	
	vtkAbstractWidget* pWidget;
	m_pMeasureList->InitTraversal();
	for	(int i= 0;i<m_pMeasureList->GetNumberOfItems();i++)
	{
		pWidget=(vtkAbstractWidget* )m_pMeasureList->GetNextItemAsObject();
		pWidget->SetInteractor(NULL);
		

	}
	m_pMeasureList->RemoveAllItems();
	
}


void CMPRView::SetupDisplayEnv()
{
	
	vtkTransform* pTransform = vtkTransform::New();
	pTransform->SetMatrix(m_pNewImage2Scanner);
	m_pViewer->GetImageActor()->SetUserTransform(pTransform);
	pTransform->Delete();

	m_pViewer->GetImageActor()->SetOpacity(0.8);

	vtkCamera *cam=GetRenderer()->GetActiveCamera();
	double pos[3];
	
	pos[0] = m_dNewImage2Scanner[0][3]+m_imageInfo.FovX/2*m_pNewImage2Scanner->GetElement(0,0)+m_imageInfo.FovY/2*m_pNewImage2Scanner->GetElement(0,1);
	pos[1] = m_dNewImage2Scanner[1][3]+m_imageInfo.FovX/2*m_pNewImage2Scanner->GetElement(1,0)+m_imageInfo.FovY/2*m_pNewImage2Scanner->GetElement(1,1);
	pos[2] = m_dNewImage2Scanner[2][3]+m_imageInfo.FovX/2*m_pNewImage2Scanner->GetElement(2,0)+m_imageInfo.FovY/2*m_pNewImage2Scanner->GetElement(2,1);
	cam->SetFocalPoint(pos);
	
	pos[0]=pos[0]-m_pNewImage2Scanner->GetElement(0,2)*100;
	pos[1]=pos[1]-m_pNewImage2Scanner->GetElement(1,2)*100;
	pos[2]=pos[2]-m_pNewImage2Scanner->GetElement(2,2)*100;	
    cam->SetPosition(pos); 
	
	pos[0]=m_pNewImage2Scanner->GetElement(0,1);
	pos[1]=m_pNewImage2Scanner->GetElement(1,1);
	pos[2]=m_pNewImage2Scanner->GetElement(2,1);	
    cam->SetViewUp(pos);

	cam->GetFocalPoint(m_viewFocus);
    cam->GetPosition(m_viewPoint);
	
}

int CMPRView::SetupImageViewer(vtkImageData* pImage)
{
	m_pImageData=pImage;
	if (!m_pViewer==NULL)
	{
		m_pViewer->Delete();
		m_pViewer = CExtendImageView2::New();
	}
	else
	{
		m_pViewer = CExtendImageView2::New();
	}

	GetRenderer()->SetBackground(0.0,0.0,0.0);
    
	m_pCorAnnotate->SetImageActor(m_pViewer->GetImageActor());
	m_pCorAnnotate->ShowSliceAndImageOn();
	
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

	//m_pCorAnnotate->SetTextProperty(pTextPro);
	m_pCorAnnotate->SetText(2,"MPR Result");
	m_pCorAnnotate->SetVisibility(1);

	pTextPro->Delete();

		
	return 0;
}




void CMPRView::WheelCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata)
{
	CMPRView* m_pView = reinterpret_cast<CMPRView*>( clientdata );
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

void CMPRView::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == this)
		return;
		
	switch(nCmdID)
	{
	case CMD_TPS_REMOVEALL:
		RemoveAll();
		break;
	case CMD_TPS_RENDERREFRESHALL:
	case CMD_TPS_RENDERREFRESH:
		Invalidate(FALSE);
		GetRenderer()->Render();
		
		break;	
	default:
		break;
	}
}


void CMPRView::SendCommand(int nCmdID,CString& sPara)
{
	if(m_pFrmMgr)
	{
		m_pFrmMgr->SendCommand(this, nCmdID,sPara);
	}
}


void CMPRView::OnPopupmenuDisplayDisplaymeasure() 
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

void CMPRView::OnPopupmenuDisplayDeletemeasure() 
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



void CMPRView::OnCreateMeasureTool(CString sPara,double* pos)
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













BOOL CMPRView::PreTranslateMessage(MSG* pMsg) 
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

	return CDlgMPRSkin::PreTranslateMessage(pMsg);
}


void CMPRView::OnPopupmenuMeasureDis() 
{
	// TODO: Add your command handler code here
	CString sPara="Distance";
	OnCreateMeasureTool(sPara);
}

void CMPRView::OnPopupmenuMeasureAngle() 
{
	// TODO: Add your command handler code here
	CString sPara="Angle";
	OnCreateMeasureTool(sPara);
}


void CMPRView::OnPopupmenuDisplay2dresetview() 
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

void CMPRView::OnPopupmenuDisplayResetwindowlevel() 
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


int CMPRView::InitImage(vtkImageData* pImage,vtkMatrix4x4 *pMatrix)
{
	if(!pImage || !pMatrix)
		return -1;

	GetRenderer()->RemoveAllViewProps();
	if(m_pProbe)
	{
		GetRenderer()->AddActor(m_pProbe);
		GetRenderer()->AddActor(m_pProbe->probe4);
	}

	int dim[3];
	double origin[3],space[3];
	
	
	vtkImageData *pNewImage=NULL;
	try
	{
		int extent[6];
		
		vtkMatrix4x4 *resliceAxes = vtkMatrix4x4::New();
		resliceAxes->DeepCopy(m_dResliceMatrix);

		// Extract a slice in the desired orientation
		m_reslice->SetInput(pImage);
		m_reslice->SetOutputDimensionality(2);
		m_reslice->SetResliceAxes(resliceAxes);
		m_reslice->SetInterpolationModeToCubic();
		resliceAxes->Delete();
		
		m_reslice->Update();
		pNewImage=m_reslice->GetOutput();
		if(!pNewImage)
		{
			return NULL;
		}
		pNewImage->UpdateInformation();
		pNewImage->GetWholeExtent(extent);
		pNewImage->GetSpacing(space);
		pNewImage->GetOrigin(origin);

		dim[0]=extent[1]-extent[0]+1;
		dim[1]=extent[3]-extent[2]+1;
		dim[2]=1;
	
	}catch(...)
	{
		try
		{
			return NULL;
		}catch(...)
		{
			return NULL;
		}
	}
	
	
	Matrix mOldImage(4,4);
	Matrix mNewImage(4,4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_pOldImage2Scanner->Element[i][j]=pMatrix->Element[i][j];
			mOldImage.value[i][j]=m_pOldImage2Scanner->Element[i][j];
		}
	}
	
	Matrix mTmp(4,4);
	mTmp.value[0][0]=m_dResliceMatrix[0];
	mTmp.value[0][1]=m_dResliceMatrix[1];
	mTmp.value[0][2]=m_dResliceMatrix[2];
	mTmp.value[0][3]=m_dResliceMatrix[3];
	mTmp.value[1][0]=m_dResliceMatrix[4];
	mTmp.value[1][1]=m_dResliceMatrix[5];
	mTmp.value[1][2]=m_dResliceMatrix[6];
	mTmp.value[1][3]=m_dResliceMatrix[7];
	mTmp.value[2][0]=m_dResliceMatrix[8];
	mTmp.value[2][1]=m_dResliceMatrix[9];
	mTmp.value[2][2]=m_dResliceMatrix[10];
	mTmp.value[2][3]=m_dResliceMatrix[11];
	mTmp.value[3][0]=m_dResliceMatrix[12];
	mTmp.value[3][1]=m_dResliceMatrix[13];
	mTmp.value[3][2]=m_dResliceMatrix[14];
	mTmp.value[3][3]=m_dResliceMatrix[15];

	mTmp=mOldImage*mTmp;
	
	//先计算实际图像的空间坐标
	mNewImage.value[0][0]=1;
	mNewImage.value[0][1]=0;
	mNewImage.value[0][2]=0;
	mNewImage.value[0][3]=origin[0];
	mNewImage.value[1][0]=0;
	mNewImage.value[1][1]=1;
	mNewImage.value[1][2]=0;
	mNewImage.value[1][3]=origin[1];
	mNewImage.value[2][0]=0;
	mNewImage.value[2][1]=0;
	mNewImage.value[2][2]=1;
	mNewImage.value[2][3]=origin[2];
	mNewImage.value[3][0]=0;
	mNewImage.value[3][1]=0;
	mNewImage.value[3][2]=0;
	mNewImage.value[3][3]=1;

	mNewImage=mTmp*mNewImage;
	
	Vector vNorm;
	vNorm.value[0]=mNewImage.value[0][0];
	vNorm.value[1]=mNewImage.value[1][0];
	vNorm.value[2]=mNewImage.value[2][0];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][0]=vNorm.value[0];
	mNewImage.value[1][0]=vNorm.value[1];
	mNewImage.value[2][0]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][1];
	vNorm.value[1]=mNewImage.value[1][1];
	vNorm.value[2]=mNewImage.value[2][1];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][1]=vNorm.value[0];
	mNewImage.value[1][1]=vNorm.value[1];
	mNewImage.value[2][1]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][2];
	vNorm.value[1]=mNewImage.value[1][2];
	vNorm.value[2]=mNewImage.value[2][2];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][2]=vNorm.value[0];
	mNewImage.value[1][2]=vNorm.value[1];
	mNewImage.value[2][2]=vNorm.value[2];

	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_dNewImage2Scanner[i][j]=mNewImage.value[i][j];
		}
	}
	//再计算用于vtk显示使用的空间坐标。
	//因为vtk reslice后origin不为空。所以
	mNewImage=mTmp;
	
	
	vNorm.value[0]=mNewImage.value[0][0];
	vNorm.value[1]=mNewImage.value[1][0];
	vNorm.value[2]=mNewImage.value[2][0];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][0]=vNorm.value[0];
	mNewImage.value[1][0]=vNorm.value[1];
	mNewImage.value[2][0]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][1];
	vNorm.value[1]=mNewImage.value[1][1];
	vNorm.value[2]=mNewImage.value[2][1];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][1]=vNorm.value[0];
	mNewImage.value[1][1]=vNorm.value[1];
	mNewImage.value[2][1]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][2];
	vNorm.value[1]=mNewImage.value[1][2];
	vNorm.value[2]=mNewImage.value[2][2];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][2]=vNorm.value[0];
	mNewImage.value[1][2]=vNorm.value[1];
	mNewImage.value[2][2]=vNorm.value[2];

	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_pNewImage2Scanner->Element[i][j]=mNewImage.value[i][j];
		}
	}
	m_imageInfo.bValid=TRUE;
	m_imageInfo.ResolutionX=dim[0];
	m_imageInfo.ResolutionY=dim[1];
	m_imageInfo.NumOfImage=dim[2];
	m_imageInfo.spacing[0]=space[0];
	m_imageInfo.spacing[1]=space[1];
	m_imageInfo.spacing[2]=space[2];
	m_imageInfo.FovX = dim[0] * space[0];
	m_imageInfo.FovY = dim[1] * space[1];

	SetupImageViewer(pNewImage);
	SetupDisplayEnv();

	m_pViewer->GetInteractorStyle()->SetAutoAdjustCameraClippingRange(0);
	GetRenderer()->GetActiveCamera()->SetParallelScale(170);
	m_pViewer->UpdateDisplayExtent();

	Invalidate(FALSE);
	m_pViewer->Render();
		 		
	DisplayIn3D();	

	return 0;
}



void CMPRView::Reslice(double *matrix)
{
	if(!m_pImageData)
		return;

	if(!matrix)
		return;

	for(int i=0;i<16;i++)
	{
		m_dResliceMatrix[i]=matrix[i];
	}

	int dim[3];
	double origin[3],space[3];
	
	vtkImageData* pNewImage;
	try
	{
		int extent[6];
		
		vtkMatrix4x4 *resliceAxes = vtkMatrix4x4::New();
		resliceAxes->DeepCopy(m_dResliceMatrix);

		// Extract a slice in the desired orientation
		m_reslice->SetResliceAxes(resliceAxes);
		m_reslice->SetInterpolationModeToCubic();
		resliceAxes->Delete();
		
		m_reslice->Update();
		pNewImage=m_reslice->GetOutput();
		if(!pNewImage)
		{
			return;
		}
		pNewImage->UpdateInformation();
		pNewImage->GetWholeExtent(extent);
		pNewImage->GetSpacing(space);
		pNewImage->GetOrigin(origin);

		dim[0]=extent[1]-extent[0]+1;
		dim[1]=extent[3]-extent[2]+1;
		dim[2]=1;
	
	}catch(...)
	{
		return ;
	}
	
	
	Matrix mOldImage(4,4);
	Matrix mNewImage(4,4);
	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mOldImage.value[i][j]=m_pOldImage2Scanner->Element[i][j];
		}
	}
	
	Matrix mTmp(4,4);
	mTmp.value[0][0]=m_dResliceMatrix[0];
	mTmp.value[0][1]=m_dResliceMatrix[1];
	mTmp.value[0][2]=m_dResliceMatrix[2];
	mTmp.value[0][3]=m_dResliceMatrix[3];
	mTmp.value[1][0]=m_dResliceMatrix[4];
	mTmp.value[1][1]=m_dResliceMatrix[5];
	mTmp.value[1][2]=m_dResliceMatrix[6];
	mTmp.value[1][3]=m_dResliceMatrix[7];
	mTmp.value[2][0]=m_dResliceMatrix[8];
	mTmp.value[2][1]=m_dResliceMatrix[9];
	mTmp.value[2][2]=m_dResliceMatrix[10];
	mTmp.value[2][3]=m_dResliceMatrix[11];
	mTmp.value[3][0]=m_dResliceMatrix[12];
	mTmp.value[3][1]=m_dResliceMatrix[13];
	mTmp.value[3][2]=m_dResliceMatrix[14];
	mTmp.value[3][3]=m_dResliceMatrix[15];

	mTmp=mOldImage*mTmp;

	//先计算实际图像的空间坐标
	mNewImage.value[0][0]=1;
	mNewImage.value[0][1]=0;
	mNewImage.value[0][2]=0;
	mNewImage.value[0][3]=origin[0];
	mNewImage.value[1][0]=0;
	mNewImage.value[1][1]=1;
	mNewImage.value[1][2]=0;
	mNewImage.value[1][3]=origin[1];
	mNewImage.value[2][0]=0;
	mNewImage.value[2][1]=0;
	mNewImage.value[2][2]=1;
	mNewImage.value[2][3]=origin[2];
	mNewImage.value[3][0]=0;
	mNewImage.value[3][1]=0;
	mNewImage.value[3][2]=0;
	mNewImage.value[3][3]=1;

	mNewImage=mTmp*mNewImage;
	
	Vector vNorm;
	vNorm.value[0]=mNewImage.value[0][0];
	vNorm.value[1]=mNewImage.value[1][0];
	vNorm.value[2]=mNewImage.value[2][0];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][0]=vNorm.value[0];
	mNewImage.value[1][0]=vNorm.value[1];
	mNewImage.value[2][0]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][1];
	vNorm.value[1]=mNewImage.value[1][1];
	vNorm.value[2]=mNewImage.value[2][1];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][1]=vNorm.value[0];
	mNewImage.value[1][1]=vNorm.value[1];
	mNewImage.value[2][1]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][2];
	vNorm.value[1]=mNewImage.value[1][2];
	vNorm.value[2]=mNewImage.value[2][2];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][2]=vNorm.value[0];
	mNewImage.value[1][2]=vNorm.value[1];
	mNewImage.value[2][2]=vNorm.value[2];

	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_dNewImage2Scanner[i][j]=mNewImage.value[i][j];
		}
	}
	//再计算用于vtk显示使用的空间坐标。
	//因为vtk reslice后origin不为空。所以
	mNewImage=mTmp;
	
	
	vNorm.value[0]=mNewImage.value[0][0];
	vNorm.value[1]=mNewImage.value[1][0];
	vNorm.value[2]=mNewImage.value[2][0];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][0]=vNorm.value[0];
	mNewImage.value[1][0]=vNorm.value[1];
	mNewImage.value[2][0]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][1];
	vNorm.value[1]=mNewImage.value[1][1];
	vNorm.value[2]=mNewImage.value[2][1];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][1]=vNorm.value[0];
	mNewImage.value[1][1]=vNorm.value[1];
	mNewImage.value[2][1]=vNorm.value[2];
	vNorm.value[0]=mNewImage.value[0][2];
	vNorm.value[1]=mNewImage.value[1][2];
	vNorm.value[2]=mNewImage.value[2][2];
	vNorm=vNorm.Normalize();
	mNewImage.value[0][2]=vNorm.value[0];
	mNewImage.value[1][2]=vNorm.value[1];
	mNewImage.value[2][2]=vNorm.value[2];

	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_pNewImage2Scanner->Element[i][j]=mNewImage.value[i][j];
		}
	}
	m_imageInfo.bValid=TRUE;
	m_imageInfo.ResolutionX=dim[0];
	m_imageInfo.ResolutionY=dim[1];
	m_imageInfo.NumOfImage=dim[2];
	m_imageInfo.spacing[0]=space[0];
	m_imageInfo.spacing[1]=space[1];
	m_imageInfo.spacing[2]=space[2];
	m_imageInfo.FovX = dim[0] * space[0];
	m_imageInfo.FovY = dim[1] * space[1];

	SetupDisplayEnv();

	m_pViewer->UpdateDisplayExtent();
	Invalidate(FALSE);
	m_pViewer->Render();

	DisplayIn3D();
}

void CMPRView::AddProbe(CProbeCryo *probe)
{
	m_pProbe=probe;
	GetRenderer()->AddActor(m_pProbe);
	GetRenderer()->AddActor(m_pProbe->probe4); 

	Invalidate(FALSE);
	GetRenderer()->Render();
}

void CMPRView::DeleteProbe()
{
	if(m_pProbe)
	{
		GetRenderer()->RemoveActor(m_pProbe);
		GetRenderer()->RemoveActor(m_pProbe->probe4);
	}
	m_pProbe=NULL;
}

void CMPRView::AddRealProbe(CProbeCryo *probe)
{
	m_pRenderer->AddActor(probe);
	m_pRenderer->AddActor(probe->probe4);
	Invalidate(FALSE);
}

void CMPRView::DisplayIn3D()
{
	if(!m_pViewer)
		return;

	//将图像显示在3D窗口中
	vtkImageActor *imageActor;
	if(m_instanceIndex==1)
	{
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActorMPR1;
	}else
	{
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActorMPR2;
	}
	
	vtkTransform* pTransform = vtkTransform::New();
	pTransform->SetMatrix(m_pNewImage2Scanner);
	imageActor->SetUserTransform(pTransform);
	imageActor->SetInput(m_pViewer->GetWindowLevel()->GetOutput());
	pTransform->Delete();
	
	imageActor->GetInput()->UpdateInformation();
	int *w_ext = imageActor->GetInput()->GetWholeExtent();
	imageActor->SetDisplayExtent(w_ext);
	imageActor->SetZSlice(0);

	CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(imageActor);
}

void CMPRView::OnButton3d() 
{
	// TODO: Add your control notification handler code here
	vtkImageActor *imageActor;
	if(m_instanceIndex==1)
	{
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActorMPR1;
	}else
	{
		imageActor=CGlobalManager::Manager()->m_3DView->m_pImageActorMPR2;
	}

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
