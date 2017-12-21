// ExtendVTKClass.cpp: implementation of the CExtendVTKClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExtendVTKClass.h"
#include "vtkUnsignedShortArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCrossLine::CCrossLine()
{
	line1=vtkActor::New();
	vtkLineSource *pLine = vtkLineSource::New();
	pLine->SetResolution(20);
	pLine->SetPoint1(-10,0,0);
	pLine->SetPoint2(10,0,0);
	vtkPolyDataMapper* pMapper = vtkPolyDataMapper::New();
	pMapper->ImmediateModeRenderingOn();
	pMapper->SetInputConnection(pLine->GetOutputPort());
	line1->SetMapper(pMapper);
	
	line2=vtkActor::New();
	vtkLineSource *pLine2 = vtkLineSource::New();
	pLine2->SetResolution(20);
	pLine2->SetPoint1(0,-10,0);
	pLine2->SetPoint2(0,10,0);
	vtkPolyDataMapper* pMapper2 = vtkPolyDataMapper::New();
	pMapper2->SetInputConnection(pLine2->GetOutputPort());
	pMapper2->ImmediateModeRenderingOn();
	line2->SetMapper(pMapper2);
	AddPart(line1);
	AddPart(line2);
	
	pLine->Delete();
	pLine2->Delete();
	pMapper->Delete();
	pMapper2->Delete();
				
}
CCrossLine::~CCrossLine()
{
	line1->Delete();
	line2->Delete();
}

CBox::CBox()
{
	pBoxImage=NULL;
	
	line1=vtkActor::New();
	pLine1 = vtkLineSource::New();
	pLine1->SetResolution(20);
	pLine1->SetPoint1(pnt1[0],pnt1[1],pnt1[2]);
	pLine1->SetPoint2(pnt2[0],pnt1[1],pnt1[2]);
	vtkPolyDataMapper* pMapper1 = vtkPolyDataMapper::New();
	pMapper1->ImmediateModeRenderingOn();
	pMapper1->SetInputConnection(pLine1->GetOutputPort());
	line1->SetMapper(pMapper1);
	
	line2=vtkActor::New();
	pLine2 = vtkLineSource::New();
	pLine2->SetResolution(20);
	pLine2->SetPoint1(pnt2[0],pnt1[1],pnt1[2]);
	pLine2->SetPoint2(pnt2[0],pnt2[1],pnt2[2]);
	vtkPolyDataMapper* pMapper2 = vtkPolyDataMapper::New();
	pMapper2->ImmediateModeRenderingOn();
	pMapper2->SetInputConnection(pLine2->GetOutputPort());
	line2->SetMapper(pMapper2);
	
	line3=vtkActor::New();
	pLine3 = vtkLineSource::New();
	pLine3->SetResolution(20);
	pLine3->SetPoint1(pnt2[0],pnt2[1],pnt2[2]);
	pLine3->SetPoint2(pnt1[0],pnt2[1],pnt1[2]);
	vtkPolyDataMapper* pMapper3 = vtkPolyDataMapper::New();
	pMapper3->ImmediateModeRenderingOn();
	pMapper3->SetInputConnection(pLine3->GetOutputPort());
	line3->SetMapper(pMapper3);
	
	line4=vtkActor::New();
	pLine4 = vtkLineSource::New();
	pLine4->SetResolution(20);
	pLine4->SetPoint1(pnt1[0],pnt2[1],pnt1[2]);
	pLine4->SetPoint2(pnt1[0],pnt1[1],pnt1[2]);
	vtkPolyDataMapper* pMapper4 = vtkPolyDataMapper::New();
	pMapper4->ImmediateModeRenderingOn();
	pMapper4->SetInputConnection(pLine4->GetOutputPort());
	line4->SetMapper(pMapper4);
	
	
	AddPart(line1);
	AddPart(line2);
	AddPart(line3);
	AddPart(line4);
	
	double co[3];
	co[0]=1.0;
	co[1]=co[2]=0.0;
	SetColor(co);
	
	
	pMapper1->Delete();
	pMapper2->Delete();
	pMapper3->Delete();
	pMapper4->Delete();
				
}
CBox::~CBox()
{
	if(pBoxImage!=NULL)
		pBoxImage->Delete();
	
	pLine1->Delete();
	pLine2->Delete();
	pLine3->Delete();
	pLine4->Delete();
	
	line1->Delete();
	line2->Delete();
	line3->Delete();
	line4->Delete();
}

void CExtendDistanceWidget::SetWidgetState(int state)
{
	this->WidgetState=state;
}

void CExtendAngleWidget::SetWidgetState(int state)
{
	this->WidgetState=state;
}

void CExtendImageView2::UpdateDisplayExtent()
{
	vtkImageData *input = this->GetInput();
	if (!input || !this->ImageActor)
    {
		return;
    }
	
	input->UpdateInformation();
	int *w_ext = input->GetWholeExtent();
	
	// Is the slice in range ? If not, fix it
	
	int slice_min = w_ext[this->SliceOrientation * 2];
	int slice_max = w_ext[this->SliceOrientation * 2 + 1];
	if (this->Slice < slice_min || this->Slice > slice_max)
    {
		this->Slice = static_cast<int>((slice_min + slice_max) * 0.5);
    }
	
	// Set the image actor
	
	switch (this->SliceOrientation)
    {
    case vtkImageViewer2::SLICE_ORIENTATION_XY:
		this->ImageActor->SetDisplayExtent(
			w_ext[0], w_ext[1], w_ext[2], w_ext[3], this->Slice, this->Slice);
		break;
		
    case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		this->ImageActor->SetDisplayExtent(
			w_ext[0], w_ext[1], this->Slice, this->Slice, w_ext[4], w_ext[5]);
		break;
		
    case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		this->ImageActor->SetDisplayExtent(
			this->Slice, this->Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
		break;
    }
	
	// Figure out the correct clipping range
	
	if (this->Renderer)
    {
		if (this->InteractorStyle && 
			this->InteractorStyle->GetAutoAdjustCameraClippingRange())
		{
			this->Renderer->ResetCameraClippingRange();
		}
		else
		{
			vtkCamera *cam = this->Renderer->GetActiveCamera();
			if (cam)
			{
				double bounds[6];
				this->ImageActor->GetBounds(bounds);
				ResetCameraRange(bounds);
				/*double spos = (double)bounds[this->SliceOrientation * 2];
				double cpos = (double)cam->GetPosition()[this->SliceOrientation];
				double range = fabs(spos - cpos);
				double *spacing = input->GetSpacing();
				double avg_spacing = 
				((double)spacing[0] + (double)spacing[1] + (double)spacing[2]) / 3.0;
				
				  avg_spacing = 300;
				  
					cam->SetClippingRange(
				range - avg_spacing * 3.0, range + avg_spacing * 3.0);*/
			}
		}
    }
}

void CExtendImageView2::InstallPipeline()
{
	if (this->RenderWindow && this->Renderer)
    {
		this->RenderWindow->AddRenderer(this->Renderer);
    }
	
	if (this->Interactor)
    {
		if (!this->InteractorStyle)
		{
			this->InteractorStyle = CExtendInteractorStyleImage::New();
			myImageViewer2Callback *cbk = myImageViewer2Callback::New();
			cbk->IV = this;
			this->InteractorStyle->AddObserver(
				vtkCommand::WindowLevelEvent, cbk);
			this->InteractorStyle->AddObserver(
				vtkCommand::StartWindowLevelEvent, cbk);
			this->InteractorStyle->AddObserver(
				vtkCommand::ResetWindowLevelEvent, cbk);
			cbk->Delete();

			m_cbk=cbk;
		}
		
		this->Interactor->SetInteractorStyle(this->InteractorStyle);
		this->Interactor->SetRenderWindow(this->RenderWindow);
    }
	
	if (this->Renderer && this->ImageActor)
    {
		this->Renderer->AddViewProp(this->ImageActor);
    }
	
	if (this->ImageActor && this->WindowLevel)
    {
		this->ImageActor->SetInput(this->WindowLevel->GetOutput());
    }
}

void CExtendInteractorStyleImage::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	
	this->FindPokedRenderer(x, y);
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
	
	// Redefine this button to handle window/level
	this->GrabFocus(this->EventCallbackCommand);
	int i=this->Interactor->GetShiftKey();
	i=this->Interactor->GetControlKey();
	//if (!this->Interactor->GetShiftKey() && this->Interactor->GetControlKey()) 
    {
		this->WindowLevelStartPosition[0] = x;
		this->WindowLevelStartPosition[1] = y;       
		this->StartWindowLevel();
    }
	
	// The rest of the button + key combinations remain the same
	
	//else
    {
		//this->Superclass::OnLeftButtonDown();
    }
}

void CExtendInteractorStyleImage::OnLeftButtonUp()
{
	switch (this->State) 
    {
    case VTKIS_WINDOW_LEVEL:
		this->EndWindowLevel();
		if ( this->Interactor )
        {
			this->ReleaseFocus();
        }
		break;
    }
	
	// Call parent to handle all other states and perform additional work
	
	this->Superclass::OnLeftButtonUp();
}

void CExtendInteractorStyleImage::OnMouseMove()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	
	switch (this->State) 
    {
    case VTKIS_WINDOW_LEVEL:
		//if(this->Interactor->GetControlKey())
		{
			this->FindPokedRenderer(x, y);
			this->WindowLevel();
			this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		}
		break;
		
    case VTKIS_PICK:
		this->FindPokedRenderer(x, y);
		this->Pick();
		this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		break;
    }
	
	// Call parent to handle all other states and perform additional work
	
	this->Superclass::OnMouseMove();
}


void CCrossLine::SetColor(double color[])
{
	line1->GetProperty()->SetColor(color);
	line2->GetProperty()->SetColor(color);
}

void CBox::SetColor(double color[])
{
	line1->GetProperty()->SetColor(color);
	line2->GetProperty()->SetColor(color);
	line3->GetProperty()->SetColor(color);
	line4->GetProperty()->SetColor(color);
}

void CExtendImageView2::ResetCameraRange(double *bounds)
{
	double  vn[3], position[3], a, b, c, d;
	double  range[2], dist;
	int     i, j, k;
	
	
	// Find the plane equation for the camera view plane
	this->Renderer->GetActiveCamera()->GetViewPlaneNormal(vn);
	this->Renderer->GetActiveCamera()->GetPosition(position);
	a = -vn[0];
	b = -vn[1];
	c = -vn[2];
	d = -(a*position[0] + b*position[1] + c*position[2]);
	
	// Set the max near clipping plane and the min far clipping plane
	range[0] = a*bounds[0] + b*bounds[2] + c*bounds[4] + d;
	range[1] = 1e-18;
	
	// Find the closest / farthest bounding box vertex
	for ( k = 0; k < 2; k++ )
    {
		for ( j = 0; j < 2; j++ )
		{
			for ( i = 0; i < 2; i++ )
			{
				dist = a*bounds[i] + b*bounds[2+j] + c*bounds[4+k] + d;
				range[0] = (dist<range[0])?(dist):(range[0]);
				range[1] = (dist>range[1])?(dist):(range[1]);
			}
		}
    }
	
	// Do not let the range behind the camera throw off the calculation.
	if (range[0] < 0.0)
    {
		range[0] = 0.0;
    }
	
	// Give ourselves a little breathing room
	range[0] = 0.99*range[0] - (range[1] - range[0])*0.5;
	range[1] = 1.01*range[1] + (range[1] - range[0])*0.5;
	
	// Make sure near is not bigger than far
	range[0] = (range[0] >= range[1])?(0.01*range[1]):(range[0]);
	
	// Make sure near is at least some fraction of far - this prevents near
	// from being behind the camera or too close in front. How close is too
	// close depends on the resolution of the depth buffer
	if (!this->Renderer->GetNearClippingPlaneTolerance())
    {
		this->Renderer->SetNearClippingPlaneTolerance(0.01);
		if (this->Renderer->GetRenderWindow())
		{
			int ZBufferDepth = this->Renderer->GetRenderWindow()->GetDepthBufferSize();
			if ( ZBufferDepth > 16 )
			{
				this->Renderer->SetNearClippingPlaneTolerance(0.001);
			}
		}
    }
	
	// make sure the front clipping range is not too far from the far clippnig
	// range, this is to make sure that the zbuffer resolution is effectively
	// used
	if (range[0] < this->Renderer->GetNearClippingPlaneTolerance()*range[1])
    {
		range[0] = this->Renderer->GetNearClippingPlaneTolerance()*range[1];
    }
	
	
	range[0]-=500;
	range[1]+=500;
	this->Renderer->GetActiveCamera()->SetClippingRange( range );
}

void CBox::Refresh()
{
	double p1[3],p2[3];
	vtkTransform *tr,*tr2;
	
	//get p1 in image coord
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->Inverse();
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(pnt1);
	
	tr->Concatenate(tr2);
	tr->GetPosition(p1);
	
	tr->Delete();
	tr2->Delete();
	
	//get p2 in image coord
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->Inverse();
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(pnt2);
	
	tr->Concatenate(tr2);
	tr->GetPosition(p2);
	
	tr->Delete();
	tr2->Delete();
	
	double spacing[3];
	pImage->GetSpacing(spacing);
	p1[2]=pViewer->GetSlice()*spacing[2];;
	p2[2]=pViewer->GetSlice()*spacing[2];;
	
	double d1[3],d2[3],d3[3],d4[3];
	d1[0]=p1[0];
	d1[1]=p1[1];
	d1[2]=p1[2];
	d2[0]=p2[0];
	d2[1]=p1[1];
	d2[2]=p1[2];
	d3[0]=p2[0];
	d3[1]=p2[1];
	d3[2]=p2[2];
	d4[0]=p1[0];
	d4[1]=p2[1];
	d4[2]=p1[2];
	
	//get d1 in world
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(d1);
	
	tr->Concatenate(tr2);
	tr->GetPosition(d1);
	
	tr->Delete();
	tr2->Delete();
	
	//get d2 in world
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(d2);
	
	tr->Concatenate(tr2);
	tr->GetPosition(d2);
	
	tr->Delete();
	tr2->Delete();
	
	//get d3 in world
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(d3);
	
	tr->Concatenate(tr2);
	tr->GetPosition(d3);
	
	tr->Delete();
	tr2->Delete();
	
	//get d4 in world
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(d4);
	
	tr->Concatenate(tr2);
	tr->GetPosition(d4);
	
	tr->Delete();
	tr2->Delete();
	
	
	pLine1->SetPoint1(d1[0],d1[1],d1[2]);
	pLine1->SetPoint2(d2[0],d2[1],d2[2]);
	
	pLine2->SetPoint1(d2[0],d2[1],d2[2]);
	pLine2->SetPoint2(d3[0],d3[1],d3[2]);
	
	pLine3->SetPoint1(d3[0],d3[1],d3[2]);
	pLine3->SetPoint2(d4[0],d4[1],d4[2]);
	
	pLine4->SetPoint1(d4[0],d4[1],d4[2]);
	pLine4->SetPoint2(d1[0],d1[1],d1[2]);
	
	pLine1->Update();
	pLine2->Update();
	pLine3->Update();
	pLine4->Update();
}

vtkImageData * CBox::GetBoxImage(double &x,double&y)
{
	double p1[3],p2[3];
	vtkTransform *tr,*tr2;
	
	//get p1 in image coord
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->Inverse();
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(pnt1);
	
	tr->Concatenate(tr2);
	tr->GetPosition(p1);
	
	tr->Delete();
	tr2->Delete();
	
	//get p2 in image coord
	tr=vtkTransform::New();
	tr->SetMatrix(pMatrix);
	tr->Inverse();
	tr->PreMultiply();
	
	tr2=vtkTransform::New();
	tr2->Identity();
	tr2->Translate(pnt2);
	
	tr->Concatenate(tr2);
	tr->GetPosition(p2);
	
	tr->Delete();
	tr2->Delete();
	
	if(pBoxImage!=NULL)
		pBoxImage->Delete();
	pBoxImage=vtkImageData::New();
	
	double spacing[3];
	pImage->GetSpacing(spacing);
	
	int p1InImage[2],p2InImage[2];
	
	p1InImage[0]=p1[0]/spacing[0];
	p1InImage[1]=p1[1]/spacing[1];
	p2InImage[0]=p2[0]/spacing[0];
	p2InImage[1]=p2[1]/spacing[1];
	
	int minx,miny,sizex,sizey;
	minx=99999;
	if(minx>p1InImage[0])
		minx=p1InImage[0];
	if(minx>p2InImage[0])
		minx=p2InImage[0];
	miny=99999;
	if(miny>p1InImage[1])
		miny=p1InImage[1];
	if(miny>p2InImage[1])
		miny=p2InImage[1];
	
	sizex=-9999;
	if(sizex<p1InImage[0])
		sizex=p1InImage[0];
	if(sizex<p2InImage[0])
		sizex=p2InImage[0];
	sizex=sizex-minx;
	sizey=-9999;
	if(sizey<p1InImage[1])
		sizey=p1InImage[1];
	if(sizey<p2InImage[1])
		sizey=p2InImage[1];
	sizey=sizey-miny;
	
	if(sizex<=0||sizey<=0)
		return NULL;
	
	unsigned short * m_data = new unsigned short [sizex*sizey];	
	
	for(int i=0;i<sizex;i++)
		for(int j=0;j<sizey;j++)
		{
			m_data[j*sizex+i]=pImage->GetScalarComponentAsFloat(minx+i,miny+j,pViewer->GetSlice(),0);
		}
		vtkUnsignedShortArray *array =  vtkUnsignedShortArray::New();
		array ->SetArray(m_data, sizex*sizey,0) ;
		pBoxImage->GetPointData()->SetScalars(array);
		pBoxImage ->SetDimensions(sizex,sizey,1);
		pBoxImage ->SetScalarType(VTK_UNSIGNED_SHORT);
		
		pBoxImage ->SetSpacing(spacing);
		pBoxImage ->SetOrigin(0.0, 0.0, 0.0);
		array->Delete();
		
		x=minx*spacing[0];
		y=miny*spacing[1];
		return pBoxImage;
}
