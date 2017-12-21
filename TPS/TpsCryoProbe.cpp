// TpsCryoProbe.cpp: implementation of the CProbeCryo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TpsCryoProbe.h"
#include "GalilIceBallDistribute.h"
#include "ExtendVTKClass.h"
#include "DisplayDlg.h"
#include "GlobalDef.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbeCryo::CProbeCryo()
{
	m_pMsgWindow=NULL;

	m_ucsFlag=FALSE;
	m_fDiamter=1;
	m_fLength=150;
	m_fTipLength=3;
	m_iResolution = 10;
	m_dColor[0][0] = 1;
	m_dColor[0][1] = 0.5;
	m_dColor[0][2] = 0.5;
	m_dColor[1][0] = 0;
	m_dColor[1][1] = 1;
	m_dColor[1][2] = 0;
	
	m_pIceBall=NULL;
	//m_pIceBall = CBallIceGalil::New();
	m_pIceBall = new CBallIceGalil;
	XAxisShaft=NULL;
	XAxisTip=NULL;
	YAxisShaft=NULL;
	YAxisTip=NULL;
	ZAxisShaft=NULL;
	ZAxisTip=NULL;

	probe1=vtkActor::New(); //for iamge1
	probe2=vtkActor::New(); //for image2
	probe3=vtkActor::New(); //for image3
	probe4=vtkActor::New(); //for 3D

	cross1=CCrossLine::New(); //for iamge1
	cross2=CCrossLine::New(); //for iamge2
	cross3=CCrossLine::New(); //for iamge3
			
}

CProbeCryo::~CProbeCryo()
{
	if(m_pIceBall)
	{
		//m_pIceBall->Delete();
		delete m_pIceBall;
		m_pIceBall=NULL;
		m_pAttachIceBall=NULL;
	}
	if(XAxisShaft)
		XAxisShaft->Delete();
	if(XAxisTip)
		XAxisTip->Delete();
	if(YAxisShaft)
		YAxisShaft->Delete();
	if(YAxisTip)
		YAxisTip->Delete();
	if(ZAxisShaft)
		ZAxisShaft->Delete();
	if(ZAxisTip)
		ZAxisTip->Delete();

	probe1->Delete();
	probe2->Delete();
	probe3->Delete();
	probe4->Delete();

	cross1->Delete();
	cross2->Delete();
	cross3->Delete();

}
CProbeCryo::ToolAssembly()
{
	this->GetParts()->RemoveAllItems();
	vtkCylinderSource* pCylinder = vtkCylinderSource::New();
	pCylinder->SetRadius(m_fDiamter/2);
	pCylinder->SetResolution(m_iResolution);
	pCylinder->SetHeight(m_fLength);

	vtkPolyDataMapper* pCylinderMapper = vtkPolyDataMapper::New();
	pCylinderMapper->SetInputConnection(pCylinder->GetOutputPort());
	pCylinderMapper->ImmediateModeRenderingOn();
	
	probe1->SetMapper(pCylinderMapper);
	probe2->SetMapper(pCylinderMapper);
	probe3->SetMapper(pCylinderMapper);
	probe4->SetMapper(pCylinderMapper);
	probe1->GetProperty()->GetColor(m_probeColor[0]);
	probe2->GetProperty()->GetColor(m_probeColor[1]);
	probe3->GetProperty()->GetColor(m_probeColor[2]);
	probe4->GetProperty()->GetColor(m_probeColor[3]);

	vtkConeSource * pTip = vtkConeSource::New();
	pTip->SetRadius(m_fDiamter/2);
	pTip->SetHeight(m_fTipLength);
	pTip->SetResolution(20);

	vtkPolyDataMapper *pConeMapper = vtkPolyDataMapper::New();
	pConeMapper->SetInputConnection(pTip->GetOutputPort());
	pConeMapper->ImmediateModeRenderingOn();
	
	vtkActor * pConeActor = vtkActor::New();
	pConeActor->SetMapper(pConeMapper);
	pConeActor->SetPosition(0,m_fTipLength/2,0);
	pConeActor->SetOrientation(0,0,270);
	pConeActor->GetProperty()->SetColor(1,0,0);
	pConeActor->SetPickable(1);
	AddPart(pConeActor);
	
	pCylinderMapper->Delete();
	pCylinder->Delete();
	
	pConeMapper->Delete();
	pTip->Delete();
	pConeActor->Delete();
	
	
	CreateAxis();
	CreateDose();
	//DisplayAxes();
	UpdateProbeCoord();
	
}

int CProbeCryo::GetTipPos(double* pTipPos)
{
	
	pTipPos[0]=0;
	pTipPos[1]=0;
	pTipPos[2]=0;
	return 0;
}



void CProbeCryo::Initial(NEEDLE_INFO eNeedleType)
{
	CProbe::Initial();
	m_eNeedleType = eNeedleType;
	
	m_fLength = eNeedleType.needleLength ;
	m_fDiamter = eNeedleType.needleDiamter ;

}

void CProbeCryo::CreateDose()
{
	if(m_eNeedleType.type==1)
	{
		m_pAttachIceBall=m_pIceBall;
		if(m_pAttachIceBall!=NULL)
			m_pAttachIceBall->CreateDose(this);
		//m_pAttachIceBall=NULL;
	}else
	{
		m_pAttachIceBall=NULL;
	}

	
}

void CProbeCryo::SetColor(double color[3])
{
	
	if(m_pAttachIceBall)
	{
		vtkProp3DCollection* pProps = m_pAttachIceBall->m_pShell;
		pProps->InitTraversal();
		for(int j=0;j<pProps->GetNumberOfItems();j++)
		{
			vtkProp3D* pProp3D = pProps->GetNextProp3D();
			((vtkActor*)pProp3D)->GetProperty()->SetColor(color);
			
		}
	}
	
	probe1->GetProperty()->SetColor(color);
	probe2->GetProperty()->SetColor(color);
	probe3->GetProperty()->SetColor(color);
	probe4->GetProperty()->SetColor(color);
}

void CProbeCryo::SetColor(double r,double g,double b)
{
	double color[3];
	color[0]=r;color[1]=g;color[2]=b;
	SetColor(color);
}

void CProbeCryo::RestoreColor()
{
	if(m_pAttachIceBall)
	{
		m_pAttachIceBall->m_pShell->InitTraversal();
		for(int j=0;j<m_pAttachIceBall->m_pShell->GetNumberOfItems();j++)
		{
			BALL_SHELL ball_shell;
			ball_shell = m_pAttachIceBall->m_lstBallShell.GetAt(j);
			m_pAttachIceBall->SetShellProperty(ball_shell);

		}
	}

	probe4->GetProperty()->SetColor(m_probeColor[3]);
	UpdateProbeColor();
	
}

void CProbeCryo::CreateAxis()
{
	if(XAxisShaft)
		XAxisShaft->Delete();
	if(XAxisTip)
		XAxisTip->Delete();
	if(YAxisShaft)
		YAxisShaft->Delete();
	if(YAxisTip)
		YAxisTip->Delete();
	if(ZAxisShaft)
		ZAxisShaft->Delete();
	if(ZAxisTip)
		ZAxisTip->Delete();

	float height=40.0,tipHeight=5.0;

	XAxisShaft = vtkActor::New();
	XAxisShaft->GetProperty()->SetColor(1, 0, 0);
	YAxisShaft = vtkActor::New();
	YAxisShaft->GetProperty()->SetColor(0, 1, 0);
	ZAxisShaft = vtkActor::New();
	ZAxisShaft->GetProperty()->SetColor(0, 0, 1);

	XAxisTip = vtkActor::New();
	XAxisTip->GetProperty()->SetColor(1, 0, 0);
	YAxisTip = vtkActor::New();
	YAxisTip->GetProperty()->SetColor(0, 1, 0);
	ZAxisTip = vtkActor::New();
	ZAxisTip->GetProperty()->SetColor(0, 0, 1);

	vtkCylinderSource* pCylinder = vtkCylinderSource::New();
	pCylinder->SetRadius(0.5);
	pCylinder->SetResolution(16);
	pCylinder->SetHeight(height);
	vtkPolyDataMapper* pCylinderMapper = vtkPolyDataMapper::New();
	pCylinderMapper->SetInputConnection(pCylinder->GetOutputPort());
	pCylinderMapper->ImmediateModeRenderingOn();

	XAxisShaft->SetMapper(pCylinderMapper);
	YAxisShaft->SetMapper(pCylinderMapper);
	ZAxisShaft->SetMapper(pCylinderMapper);
	
	vtkConeSource * pTip = vtkConeSource::New();
	pTip->SetRadius(1);
	pTip->SetHeight(tipHeight);
	pTip->SetResolution(16);
	vtkPolyDataMapper *pConeMapper = vtkPolyDataMapper::New();
	pConeMapper->SetInputConnection(pTip->GetOutputPort());
	pConeMapper->ImmediateModeRenderingOn();
	
	XAxisTip->SetMapper(pConeMapper);
	YAxisTip->SetMapper(pConeMapper);
	ZAxisTip->SetMapper(pConeMapper);

	XAxisShaft->SetPosition(height/2,0,0);
	XAxisShaft->SetOrientation(0,0,90);
	XAxisTip->SetPosition(height,0,0);
	XAxisTip->SetOrientation(0,0,0);

	YAxisShaft->SetPosition(0,height/2,0);
	YAxisShaft->SetOrientation(0,0,0);
	YAxisTip->SetPosition(0,height,0);
	YAxisTip->SetOrientation(0,0,90);

	ZAxisShaft->SetPosition(0,0,height/2);
	ZAxisShaft->SetOrientation(90,0,0);
	ZAxisTip->SetPosition(0,0,height);
	ZAxisTip->SetOrientation(0,-90,270);
	

	pConeMapper->Delete();
	pTip->Delete();
	pCylinderMapper->Delete();
	pCylinder->Delete();
	

}

void CProbeCryo::AddPositions(double pTipPos[])
{
	
	vtkMatrix4x4* m = vtkMatrix4x4::New();
	this->GetMatrix(m);
	double x[3],y[3],z[3],pos[3];
	x[0] = pTipPos[0]*m->GetElement(0,0);
	x[1] = pTipPos[0]*m->GetElement(1,0);
	x[2] = pTipPos[0]*m->GetElement(2,0);
	y[0] = pTipPos[1]*m->GetElement(0,1);
	y[1] = pTipPos[1]*m->GetElement(1,1);
	y[2] = pTipPos[1]*m->GetElement(2,1);
	z[0] = pTipPos[2]*m->GetElement(0,2);
	z[1] = pTipPos[2]*m->GetElement(1,2);
	z[2] = pTipPos[2]*m->GetElement(2,2);

	pos[0]=m_position[0]+x[0]+y[0]+z[0];
	pos[1]=m_position[1]+x[1]+y[1]+z[1];
	pos[2]=m_position[2]+x[2]+y[2]+z[2];
	this->SetPosition(pos);
	m_position[0]=pos[0];
	m_position[1]=pos[1];
	m_position[2]=pos[2];
	m->Delete();
}

void CProbeCryo::SetInitPosition(double pos[])
{
	this->SetPosition(pos);
	m_position[0]=pos[0];
	m_position[1]=pos[1];
	m_position[2]=pos[2];
}

void CProbeCryo::GetTipPosInWorld(double *pos)
{
	vtkMatrix4x4* m = vtkMatrix4x4::New();
	this->GetMatrix(m);
	double x = m->GetElement(0,3);
	double y = m->GetElement(1,3);
	double z = m->GetElement(2,3);
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	m->Delete();
	
}

void CProbeCryo::DisplayAxes()
{
	AddPart(XAxisShaft);
	AddPart(YAxisShaft);
	AddPart(ZAxisShaft);
	AddPart(XAxisTip);
	AddPart(YAxisTip);
	AddPart(ZAxisTip);
}

void CProbeCryo::HideAxes()
{
	if(m_ucsFlag)
		return;

	RemovePart(XAxisShaft);
	RemovePart(YAxisShaft);
	RemovePart(ZAxisShaft);
	RemovePart(XAxisTip);
	RemovePart(YAxisTip);
	RemovePart(ZAxisTip);
}

void CProbeCryo::UpdateProbeCoord(BOOL bSendProbeMovMessage)
{
	vtkTransform *trans=vtkTransform::New();
	vtkMatrix4x4 *ma;
	double pos[3];
	ma=this->GetMatrix();
	trans->SetMatrix(this->GetMatrix());
	pos[0] = ma->GetElement(0,3)+(m_fLength/2+m_fTipLength)*ma->GetElement(0,1);
	pos[1] = ma->GetElement(1,3)+(m_fLength/2+m_fTipLength)*ma->GetElement(1,1);
	pos[2] = ma->GetElement(2,3)+(m_fLength/2+m_fTipLength)*ma->GetElement(2,1);
	
	probe1->SetPosition(pos);
	probe1->SetOrientation(trans->GetOrientation());
	probe2->SetPosition(pos);
	probe2->SetOrientation(trans->GetOrientation());
	probe3->SetPosition(pos);
	probe3->SetOrientation(trans->GetOrientation());
	probe4->SetPosition(pos);
	probe4->SetOrientation(trans->GetOrientation());

	//get segment points
	m_SegPoint[0][0] = ma->GetElement(0,3);
	m_SegPoint[0][1] = ma->GetElement(1,3);
	m_SegPoint[0][2] = ma->GetElement(2,3);
	m_SegPoint[1][0] = ma->GetElement(0,3)+(m_fLength+m_fTipLength)*ma->GetElement(0,1);
	m_SegPoint[1][1] = ma->GetElement(1,3)+(m_fLength+m_fTipLength)*ma->GetElement(1,1);
	m_SegPoint[1][2] = ma->GetElement(2,3)+(m_fLength+m_fTipLength)*ma->GetElement(2,1);
	

	trans->Delete();

	UpdateProbeColor();

	if(m_pMsgWindow && bSendProbeMovMessage)
	{
		m_pMsgWindow->PostMessage(WM_TPS_PROBE_MOVING);
	}
}
void CProbeCryo::LinePlaneIntersection(double xl, double yl, double zl, double nlx, double nly, double nlz, double npx, double npy, double npz, double d, double *xi, double *yi, double *zi)
{
 
	double t, ang;

	ang = nlx*npx + nly*npy + nlz*npz;
		
	t  = (d - (xl*npx+yl*npy+zl*npz))/ang;

	*xi = xl + nlx*t;
	*yi = yl + nly*t;
	*zi = zl + nlz*t;
}
// 
void CProbeCryo::UpdateProbeColor()
{
	double tip[3];
	double dZpos;
	double position[3];
	double normal[3];
	double offset;
	double dis1,dis2,point[3],ori[3];

	
	vtkTransform *tr=vtkTransform::New();

	//set probe color
	GetTipPosInWorld(tip);
	if(CGlobalManager::Manager()->m_2DView1->m_imageInfo.bValid)
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
				
		//get distance between segment point and image plane
		dis1=normal[0]*m_SegPoint[0][0]+normal[1]*m_SegPoint[0][1]+normal[2]*m_SegPoint[0][2]-offset;
		dis2=normal[0]*m_SegPoint[1][0]+normal[1]*m_SegPoint[1][1]+normal[2]*m_SegPoint[1][2]-offset;
		//get intersection point 
		LinePlaneIntersection(m_SegPoint[0][0],m_SegPoint[0][1],m_SegPoint[0][2],
			m_SegPoint[0][0]-m_SegPoint[1][0],m_SegPoint[0][1]-m_SegPoint[1][1],
			m_SegPoint[0][2]-m_SegPoint[1][2],normal[0],normal[1],normal[2],offset,
			&point[0],&point[1],&point[2]);
		//get orientation
		tr->SetMatrix(CGlobalManager::Manager()->m_2DView1->m_pImageFrame_scanner);
		tr->GetOrientation(ori);
		//Set position and orientation
		cross1->SetPosition(point);
		cross1->SetOrientation(ori);
		if(dis1<=0 && dis2>=0 ||dis1>=0 && dis2<=0)
		{
			cross1->SetColor(m_dColor[0]);  
			probe1->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}else if(abs(dis1)<abs(dis2))
		{
			cross1->SetColor(m_dColor[1]);  
			probe1->GetProperty()->SetColor(m_dColor[1][0],m_dColor[1][1],m_dColor[1][2]);
		}else
		{
			cross1->SetColor(m_dColor[0]);  
			probe1->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}
	}

	if(CGlobalManager::Manager()->m_2DView2->m_imageInfo.bValid)
	{
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
		
		//get distance between segment point and image plane
		dis1=normal[0]*m_SegPoint[0][0]+normal[1]*m_SegPoint[0][1]+normal[2]*m_SegPoint[0][2]-offset;
		dis2=normal[0]*m_SegPoint[1][0]+normal[1]*m_SegPoint[1][1]+normal[2]*m_SegPoint[1][2]-offset;
		//get intersection point 
		LinePlaneIntersection(m_SegPoint[0][0],m_SegPoint[0][1],m_SegPoint[0][2],
			m_SegPoint[0][0]-m_SegPoint[1][0],m_SegPoint[0][1]-m_SegPoint[1][1],
			m_SegPoint[0][2]-m_SegPoint[1][2],normal[0],normal[1],normal[2],offset,
			&point[0],&point[1],&point[2]);
		//get orientation
		tr->SetMatrix(CGlobalManager::Manager()->m_2DView2->m_pImageFrame_scanner);
		tr->GetOrientation(ori);
		//Set position and orientation
		cross2->SetPosition(point);
		cross2->SetOrientation(ori);
		if(dis1<=0 && dis2>=0 ||dis1>=0 && dis2<=0)
		{
			cross2->SetColor(m_dColor[0]);  
			probe2->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}else if(abs(dis1)<abs(dis2))
		{
			cross2->SetColor(m_dColor[1]);  
			probe2->GetProperty()->SetColor(m_dColor[1][0],m_dColor[1][1],m_dColor[1][2]);
		}else
		{
			cross2->SetColor(m_dColor[0]);  
			probe2->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}
	}

	if(CGlobalManager::Manager()->m_2DView3->m_imageInfo.bValid)
	{
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
		
		//get distance between segment point and image plane
		dis1=normal[0]*m_SegPoint[0][0]+normal[1]*m_SegPoint[0][1]+normal[2]*m_SegPoint[0][2]-offset;
		dis2=normal[0]*m_SegPoint[1][0]+normal[1]*m_SegPoint[1][1]+normal[2]*m_SegPoint[1][2]-offset;
		//get intersection point 
		LinePlaneIntersection(m_SegPoint[0][0],m_SegPoint[0][1],m_SegPoint[0][2],
			m_SegPoint[0][0]-m_SegPoint[1][0],m_SegPoint[0][1]-m_SegPoint[1][1],
			m_SegPoint[0][2]-m_SegPoint[1][2],normal[0],normal[1],normal[2],offset,
			&point[0],&point[1],&point[2]);
		//get orientation
		tr->SetMatrix(CGlobalManager::Manager()->m_2DView3->m_pImageFrame_scanner);
		tr->GetOrientation(ori);
		//Set position and orientation
		cross3->SetPosition(point);
		cross3->SetOrientation(ori);
		if(dis1<=0 && dis2>=0 ||dis1>=0 && dis2<=0)
		{
			cross3->SetColor(m_dColor[0]);  
			probe3->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}else if(abs(dis1)<abs(dis2))
		{
			cross3->SetColor(m_dColor[1]);  
			probe3->GetProperty()->SetColor(m_dColor[1][0],m_dColor[1][1],m_dColor[1][2]);
		}else
		{
			cross3->SetColor(m_dColor[0]);  
			probe3->GetProperty()->SetColor(m_dColor[0][0],m_dColor[0][1],m_dColor[0][2]);
		}
	}
	
	tr->Delete();
	
}

void CProbeCryo::ChangePositionByTarget(double *pos, int status)
{
	
	vtkMatrix4x4 *ma;
	ma=vtkMatrix4x4::New();
	vtkTransform *trans=vtkTransform::New();
	vtkMatrix4x4 *maNew;
	maNew=vtkMatrix4x4::New();

	double x[3],y[3],z[3]; 
	double pos2[3],ori[3];
	double SegPoint[2][3];

	this->GetMatrix(ma);

	
	//get segment points
	SegPoint[0][0] = ma->GetElement(0,3);
	SegPoint[0][1] = ma->GetElement(1,3);
	SegPoint[0][2] = ma->GetElement(2,3);
	SegPoint[1][0] = ma->GetElement(0,3)+(m_fLength+m_fTipLength)*ma->GetElement(0,1);
	SegPoint[1][1] = ma->GetElement(1,3)+(m_fLength+m_fTipLength)*ma->GetElement(1,1);
	SegPoint[1][2] = ma->GetElement(2,3)+(m_fLength+m_fTipLength)*ma->GetElement(2,1);
	
	//get old x,y,z direction
	x[0]=ma->GetElement(0,0);
	x[1]=ma->GetElement(1,0);
	x[2]=ma->GetElement(2,0);
	y[0]=ma->GetElement(0,1);
	y[1]=ma->GetElement(1,1);
	y[2]=ma->GetElement(2,1);
	z[0]=ma->GetElement(0,2);
	z[1]=ma->GetElement(1,2);
	z[2]=ma->GetElement(2,2);

	if(status==0)
	{
		SegPoint[0][0]=pos[0];
		SegPoint[0][1]=pos[1];
		SegPoint[0][2]=pos[2];
		
		//get new y direction
		y[0]=SegPoint[1][0]-SegPoint[0][0];
		y[1]=SegPoint[1][1]-SegPoint[0][1];
		y[2]=SegPoint[1][2]-SegPoint[0][2];
		vtkMath::Normalize(y);

		
		//get new x direction
		vtkMath::Cross(y,z,x);
		//get new z direction
		vtkMath::Cross(x,y,z);

		//get new position
		pos2[0]=SegPoint[0][0];
		pos2[1]=SegPoint[0][1];
		pos2[2]=SegPoint[0][2];
		
		maNew->SetElement(0,0,x[0]);
		maNew->SetElement(1,0,x[1]);
		maNew->SetElement(2,0,x[2]);

		maNew->SetElement(0,1,y[0]);
		maNew->SetElement(1,1,y[1]);
		maNew->SetElement(2,1,y[2]);

		maNew->SetElement(0,2,z[0]);
		maNew->SetElement(1,2,z[1]);
		maNew->SetElement(2,2,z[2]);

		maNew->SetElement(0,3,pos2[0]);
		maNew->SetElement(1,3,pos2[1]);
		maNew->SetElement(2,3,pos2[2]);

		trans->SetMatrix(maNew);
		trans->GetPosition(pos2);
		trans->GetOrientation(ori);

		this->SetPosition(pos2);
				
		this->SetOrientation(ori);
		m_position[0]=pos2[0];
		m_position[1]=pos2[1];
		m_position[2]=pos2[2];
		
		
	}else
	{
		SegPoint[1][0]=pos[0];
		SegPoint[1][1]=pos[1];
		SegPoint[1][2]=pos[2];
		
		//get new y direction
		y[0]=SegPoint[1][0]-SegPoint[0][0];
		y[1]=SegPoint[1][1]-SegPoint[0][1];
		y[2]=SegPoint[1][2]-SegPoint[0][2];
		vtkMath::Normalize(y);

		
		//get new x direction
		vtkMath::Cross(y,z,x);
		//get new z direction
		vtkMath::Cross(x,y,z);

		//get new position
		pos2[0]=SegPoint[0][0];
		pos2[1]=SegPoint[0][1];
		pos2[2]=SegPoint[0][2];
		
		maNew->SetElement(0,0,x[0]);
		maNew->SetElement(1,0,x[1]);
		maNew->SetElement(2,0,x[2]);

		maNew->SetElement(0,1,y[0]);
		maNew->SetElement(1,1,y[1]);
		maNew->SetElement(2,1,y[2]);

		maNew->SetElement(0,2,z[0]);
		maNew->SetElement(1,2,z[1]);
		maNew->SetElement(2,2,z[2]);

		maNew->SetElement(0,3,pos2[0]);
		maNew->SetElement(1,3,pos2[1]);
		maNew->SetElement(2,3,pos2[2]);

		trans->SetMatrix(maNew);
		trans->GetPosition(pos2);
		trans->GetOrientation(ori);

		this->SetPosition(pos2);
				
		this->SetOrientation(ori);
		m_position[0]=pos2[0];
		m_position[1]=pos2[1];
		m_position[2]=pos2[2];
		
	}

	maNew->Delete();
	trans->Delete();
	ma->Delete();
	UpdateProbeCoord();
}
