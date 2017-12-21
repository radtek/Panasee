// ExtendVTKClass.h: interface for the CExtendVTKClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTENDVTKCLASS_H__8BAEE958_FBBF_4A1C_9CCB_5B49979688A7__INCLUDED_)
#define AFX_EXTENDVTKCLASS_H__8BAEE958_FBBF_4A1C_9CCB_5B49979688A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VtkHeader.h"
#include "TpsGlobalConst.h"

class CExtendDistanceWidget:public vtkDistanceWidget
{
public:
	void SetWidgetState(int state);
	static CExtendDistanceWidget *New()
	{
		return new CExtendDistanceWidget;
	}
	
};

class CExtendAngleWidget:public vtkAngleWidget
{
public:
	void SetWidgetState(int state);
	static CExtendAngleWidget *New()
	{
		return new CExtendAngleWidget;
	}
	
};

class CExtendBoxWidget:public vtkBoxWidget
{
public:
	static CExtendBoxWidget *New()
	{
		return new CExtendBoxWidget;
	}
	
	vtkAssembly *m_probe;
};


class myImageViewer2Callback;

class CExtendImageView2:public vtkImageViewer2
{
public:
	static CExtendImageView2 *New()
	{
		return new CExtendImageView2;
	}
	
	virtual void InstallPipeline();
	
	virtual void UpdateDisplayExtent();

	myImageViewer2Callback *m_cbk;
	
private:
	void ResetCameraRange(double *bounds);

	
};

class CExtendInteractorStyleImage:public vtkInteractorStyleImage
{
public:
	static CExtendInteractorStyleImage *New()
	{
		return new CExtendInteractorStyleImage;
	}
	
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	
};

class myImageViewer2Callback : public vtkCommand
{
public:
	static myImageViewer2Callback *New()
	{ 
		return new myImageViewer2Callback;
	}
	
	void Execute(vtkObject *caller, 
		unsigned long event, 
		void *vtkNotUsed(callData))
    {
		if (this->IV==NULL ||
			this->IV->GetRenderWindow()==NULL || 
			this->IV->GetInput() == NULL)
        {
			return;
        }
		
		// Reset
		
		if (event == vtkCommand::ResetWindowLevelEvent)
        {
			this->IV->GetInput()->UpdateInformation();
			this->IV->GetInput()->SetUpdateExtent
				(this->IV->GetInput()->GetWholeExtent());
			this->IV->GetInput()->Update();
			double *range = this->IV->GetInput()->GetScalarRange();
			this->IV->SetColorWindow(range[1] - range[0]);
			this->IV->SetColorLevel(0.5 * (range[1] + range[0]));
			this->IV->Render();
			return;
        }
		
		// Start
		
		if (event == vtkCommand::StartWindowLevelEvent)
        {
			this->InitialWindow = this->IV->GetColorWindow();
			this->InitialLevel = this->IV->GetColorLevel();
			return;
        }
		
		// Adjust the window level here
		
		CExtendInteractorStyleImage *isi = 
			static_cast<CExtendInteractorStyleImage *>(caller);
		
		int *size = this->IV->GetRenderWindow()->GetSize();
		double window = this->InitialWindow;
		double level = this->InitialLevel;
		
		double dy = 5.0 * (isi->GetWindowLevelCurrentPosition()[0] - 
			isi->GetWindowLevelStartPosition()[0]) ;
		double dx = 5.0 * (isi->GetWindowLevelCurrentPosition()[1] - 
			isi->GetWindowLevelStartPosition()[1]);

		double newWindow = window - dx;
		double newLevel = dy + level;
		if(newLevel<1)
		{
			newLevel=1;
		}
		if(newWindow<2)
		{
			newWindow=2;
		}
		// Compute normalized delta
		/*
		double dy = 4.0 * 
			(isi->GetWindowLevelCurrentPosition()[0] - 
			isi->GetWindowLevelStartPosition()[0]) / size[0];
		double dx = 4.0 * 
			(isi->GetWindowLevelStartPosition()[1] - 
			isi->GetWindowLevelCurrentPosition()[1]) / size[1];
		
		// Scale by current values
		
		if (fabs(window) > 0.01)
        {
			dx = dx * window;
        }
		else
        {
			dx = dx * (window < 0 ? -0.01 : 0.01);
        }
		if (fabs(level) > 0.01)
        {
			dy = dy * level;
        }
		else
        {
			dy = dy * (level < 0 ? -0.01 : 0.01);
        }
		
		// Abs so that direction does not flip
		
		if (window < 0.0) 
        {
			dx = -1*dx;
        }
		if (level < 0.0) 
        {
			dy = -1*dy;
        }
		
		// Compute new window level
		
		double newWindow = dx + window;
		double newLevel;
		newLevel = level - dy;
		
		// Stay away from zero and really
		
		if (fabs(newWindow) < 0.01)
        {
			newWindow = 0.01*(newWindow < 0 ? -1 : 1);
        }
		if (fabs(newLevel) < 0.01)
        {
			newLevel = 0.01*(newLevel < 0 ? -1 : 1);
        }*/
		
		this->IV->SetColorWindow(newWindow);
		this->IV->SetColorLevel(newLevel);
		this->IV->Render();
		}
		
	CExtendImageView2 *IV;
	double InitialWindow;
	double InitialLevel;
};

class CCrossLine:public vtkAssembly
{
public:
	void SetColor(double color[3]);
	static CCrossLine *New()
	{
		return new CCrossLine;
	}
	
	CCrossLine();
	virtual ~CCrossLine();
	
	vtkActor *line1,*line2;
	
};

class CBox:public vtkAssembly
{
public:
	vtkImageData * GetBoxImage(double &x,double&y);
	void Refresh();
	void SetColor(double color[3]);
	static CBox *New()
	{
		return new CBox;
	}
	
	CBox();
	virtual ~CBox();
	
	double pnt1[3],pnt2[3];
	vtkImageData *pImage;
	vtkMatrix4x4 *pMatrix;
	CExtendImageView2 *pViewer;
	
private:
	vtkActor *line1,*line2,*line3,*line4;
	vtkLineSource *pLine1,*pLine2,*pLine3,*pLine4;
	vtkImageData *pBoxImage;
};


class CExtendActor:public vtkActor
{
public:
	static CExtendActor *New()
	{
		return new CExtendActor;
	}
	
	void SetColor(double color[3])
	{
		//GetProperty()->GetColor(m_oldColor);
		//GetProperty()->SetColor(color);
	}
	
	void SetColor(double r,double g,double b)
	{
		double color[3];
		color[0]=r;color[1]=g;color[2]=b;
		SetColor(color);
	}
	
	void RestoreColor()
	{
		//GetProperty()->SetColor(m_oldColor);
	}
	
	POLYTYPE m_polytype;
private:	
	double m_oldColor[3];
	
	
};

#endif // !defined(AFX_EXTENDVTKCLASS_H__8BAEE958_FBBF_4A1C_9CCB_5B49979688A7__INCLUDED_)
