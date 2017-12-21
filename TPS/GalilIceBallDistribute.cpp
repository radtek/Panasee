// GalilIceBallDistribute.cpp: implementation of the CBallIceGalil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GalilIceBallDistribute.h"
#include "TpsCryoProbe.h"
#include "DisplayDlg.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBallIceGalil::CBallIceGalil()
{
	//∂¡»°±˘«Úœ‘ æ—’…´
	CConfigMgr conf;
	m_bDisplayInterLine=conf.Read(TPS_ICEBALL,TPS_ICEBALL_DISPLAY_INTERSECT,0);

	//m_bDisplayInterLine=TRUE;
	m_fGrowthRatio = 1;
	for(int i=0;i<3;i++)
	{
		m_fDoseOffSet[i][0] = 0;
		m_fDoseOffSet[i][1] = 0;
		m_fDoseOffSet[i][2] = 0;
		m_fDoseOffSet[i][3] = 0;
		m_fDoseOffSet[i][4] = 0;
		m_fDoseOffSet[i][5] = 0;
	}
	vtkActor* m_p1stShell = NULL;
	vtkActor* m_p2ndShell = NULL;
	vtkActor* m_p3rddShell = NULL;
	vtkPolyData* m_p1stShellData = NULL;
	vtkPolyData* m_p2ndShellData = NULL;
	vtkPolyData* m_p3rdShellData = NULL;
	m_pIntersections = vtkProp3DCollection::New();
	m_pShell = vtkProp3DCollection::New();
	m_pShellData = vtkPolyDataCollection::New();
	m_pQuadrics = vtkImplicitFunctionCollection::New();
}

CBallIceGalil::~CBallIceGalil()
{
	if (!m_pIntersections == NULL) {m_pIntersections->Delete();m_pIntersections=NULL;}
	if (!m_pShellData == NULL) {m_pShellData->Delete();m_pShellData=NULL;}
	if (!m_pQuadrics == NULL) {m_pQuadrics->Delete();m_pQuadrics=NULL;}
	if(!m_pShell==NULL){m_pShell->Delete();m_pShell=NULL;}
}
CBallIceGalil::InitialBall(NEEDLE_INFO eNeedleType)
{
	
	m_lstBallShell.RemoveAll();
	
	m_lstBallShell.Add(eNeedleType.shell1);
	m_lstBallShell.Add(eNeedleType.shell2);
	m_lstBallShell.Add(eNeedleType.shell3);

	m_fDoseOffSet[0][1] = eNeedleType.shell1.doseOffset;
	m_fDoseOffSet[1][1] = eNeedleType.shell2.doseOffset;
	m_fDoseOffSet[2][1] = eNeedleType.shell3.doseOffset;
}

CBallIceGalil::CreateDose(CProbeCryo* pTool)
{
	InitialBall(pTool->m_eNeedleType);
	m_pToolAttach = pTool;
	CreateShell();
		
}
void CBallIceGalil::CreateShell()
{
	//remove shell information
	m_pQuadrics->RemoveAllItems();
	m_pShellData->RemoveAllItems();
	m_pShell->InitTraversal();
	int nCount = m_pShell->GetNumberOfItems(); 
	for(int i=0;i<nCount;i++)
	{
		vtkActor* pActor = (vtkActor*)m_pShell->GetNextProp3D();
		m_pToolAttach->RemovePart(pActor);
	}
	m_pShell->RemoveAllItems();
	
	//remove interaction information
	m_pIntersections->InitTraversal();
	nCount = m_pIntersections->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		vtkActor* pActor = (vtkActor*)m_pIntersections->GetNextProp3D();
		if(CGlobalManager::Manager()->m_2DView1)
				CGlobalManager::Manager()->m_2DView1->GetRenderer()->RemoveActor(pActor);
		if(CGlobalManager::Manager()->m_2DView2)
				CGlobalManager::Manager()->m_2DView2->GetRenderer()->RemoveActor(pActor);
		if(CGlobalManager::Manager()->m_2DView3)
				CGlobalManager::Manager()->m_2DView3->GetRenderer()->RemoveActor(pActor);
		
	}
	m_pIntersections->RemoveAllItems();
	m_arrIntersectionInfo.RemoveAll();

	int nShellCount = m_lstBallShell.GetSize();
	for(i=0;i<nShellCount;i++)
	{
		vtkQuadric* quadric = vtkQuadric::New();
		vtkSampleFunction * theBallSample = vtkSampleFunction::New();
		vtkContourFilter * theBallSurface = vtkContourFilter::New();
		BALL_SHELL ball_shell;
		ball_shell = m_lstBallShell.GetAt(i);
		float a0 = 1/pow(ball_shell.Diameter*m_fGrowthRatio/2,2);
		float a1 = 1/pow(ball_shell.Height*m_fGrowthRatio/2,2);
		float a2 = 1/pow(ball_shell.Diameter*m_fGrowthRatio/2,2);
		quadric->SetCoefficients(a0,a1,a2,0,0,0,0,0,0,0);
		m_pQuadrics->AddItem(quadric);

		theBallSample->SetModelBounds(-ball_shell.Height*m_fGrowthRatio/2,ball_shell.Height*m_fGrowthRatio/2,-ball_shell.Height*m_fGrowthRatio/2,ball_shell.Height*m_fGrowthRatio/2,-ball_shell.Height*m_fGrowthRatio/2,ball_shell.Height*m_fGrowthRatio/2); 
		theBallSample->SetSampleDimensions(10, 10, 10);
		theBallSample->SetImplicitFunction(quadric);

		theBallSurface->SetInputConnection(theBallSample->GetOutputPort());
		theBallSurface->SetValue(0,1);
		theBallSurface->GenerateValues(1,1,1);
		
		theBallSurface->ComputeScalarsOff();
		
		vtkPolyDataMapper* pMapper = vtkPolyDataMapper::New();
		pMapper->SetInput(theBallSurface->GetOutput());
		
		pMapper->ImmediateModeRenderingOn();
		vtkActor * pActor = vtkActor::New();
		pActor->SetMapper(pMapper);
		pActor->SetVisibility(ball_shell.Visibility);
		double tipPos[3];
		m_pToolAttach->GetTipPos(tipPos);
		tipPos[1]=tipPos[1]+m_fGrowthRatio*m_fDoseOffSet[i][1];
		pActor->SetPosition(tipPos);

		vtkProperty* pProperty = vtkProperty::New();
		pProperty->SetColor((float )GetRValue(ball_shell.Color)/255.0,
				(float )GetGValue(ball_shell.Color)/255.0,(float )GetBValue(ball_shell.Color)/255.0);
		pProperty->SetOpacity(ball_shell.Opacity/100.0);
		pActor->SetProperty(pProperty);
		pActor->SetPickable(1);
		
		m_pToolAttach->AddPart(pActor);
		m_pShell->AddItem(pActor);
		m_pShellData->AddItem(pMapper->GetInput());

		pProperty->Delete();
		pActor->Delete();
		theBallSample->Delete();
		theBallSurface->Delete();
		pMapper->Delete();
		quadric->Delete();

	}

}

void CBallIceGalil::SetShellProperty(BALL_SHELL ball_shell)
{
	vtkActor* pActor;
	m_pShell->InitTraversal();
	pActor = (vtkActor*)m_pShell->GetItemAsObject(ball_shell.Index);
	vtkProperty* pProperty = vtkProperty::New();
	pProperty->SetColor((float )GetRValue(ball_shell.Color)/255.0,
				(float )GetGValue(ball_shell.Color)/255.0,(float )GetBValue(ball_shell.Color)/255.0);
	pProperty->SetOpacity(ball_shell.Opacity/100.0);
	pActor->SetVisibility(ball_shell.Visibility);
	pActor->SetProperty(pProperty);
	pProperty->Delete();
	m_lstBallShell.SetAt(ball_shell.Index,ball_shell);
	if(ball_shell.Index==0)
		m_pToolAttach->m_eNeedleType.shell1=ball_shell;
	if(ball_shell.Index==1)
		m_pToolAttach->m_eNeedleType.shell2=ball_shell;
	if(ball_shell.Index==2)
		m_pToolAttach->m_eNeedleType.shell3=ball_shell;
}


int CBallIceGalil::GetImageIntersection(vtkPlaneCollection* pPlanes,int imageIndex)
{
	if(m_pToolAttach->m_eNeedleType.type!=1)
		return 0;

	if(!m_bDisplayInterLine)
		return 0;

	vtkPolyData* pPolyData;
	vtkPlane* pPlane;
	vtkCutter* Cutter = vtkCutter::New();
	vtkActor* pActor;
	INTERSECTLINE_PROPERTY interline;
	
	POSITION POS;
	POS=m_arrIntersectionInfo.GetHeadPosition();
	for(int i=0;i<m_arrIntersectionInfo.GetCount();i++)
	{
		interline=m_arrIntersectionInfo.GetNext(POS);
		if(interline.ImageIndex ==imageIndex)
		{
		
			pActor = (vtkActor*)m_pIntersections->GetItemAsObject(i);
			if(imageIndex==1)
				CGlobalManager::Manager()->m_2DView1->GetRenderer()->RemoveActor(pActor);
			if(imageIndex==2)
				CGlobalManager::Manager()->m_2DView2->GetRenderer()->RemoveActor(pActor);
			if(imageIndex==3)
				CGlobalManager::Manager()->m_2DView3->GetRenderer()->RemoveActor(pActor);

			m_pIntersections->RemoveItem(i);
			m_arrIntersectionInfo.RemoveAt(m_arrIntersectionInfo.FindIndex(i));
			i--;
		}
	}
	

	m_pShellData->InitTraversal();
	for(i=0;i<m_pShellData->GetNumberOfItems();i++)
	{
		BALL_SHELL ballshell=m_lstBallShell.GetAt(i);
		if(ballshell.Visibility==0)
			continue;

		vtkTransformFilter* Transfilter = vtkTransformFilter::New();
		vtkTransform* Transform = vtkTransform::New();

		//set Cutter
		pPolyData = m_pShellData->GetNextItem();
		
		Transform->SetMatrix(m_pToolAttach->GetMatrix());
		Transform->PreMultiply();
		double tipPos[3];
		m_pToolAttach->GetTipPos(tipPos);
		tipPos[1]=tipPos[1]+m_fGrowthRatio*m_fDoseOffSet[i][1];
		Transform->Translate(tipPos);
		
		Transfilter->SetTransform(Transform);
		Transfilter->SetInput(pPolyData);
		Cutter->SetInput(Transfilter->GetOutput());
		Transfilter->Delete();
		Transform->Delete();

		pPlanes->InitTraversal();
		for(int j=0;j<pPlanes->GetNumberOfItems();j++)
		{
			vtkStripper* cutStrips = vtkStripper::New();
			vtkTriangleFilter* cutTriangles = vtkTriangleFilter::New();
			vtkMassProperties* cutArea = vtkMassProperties::New();
			vtkFeatureEdges* Edge = vtkFeatureEdges::New();
			vtkPolyDataMapper * pEdgeMapper = vtkPolyDataMapper::New();
			vtkProperty* pProperty = vtkProperty::New();
			vtkActor* pEdgeActor = vtkActor::New();
			vtkPolyData* cutPoly = vtkPolyData::New();

			Edge->ColoringOn();

			pPlane = pPlanes->GetNextItem();
			Cutter->SetCutFunction(pPlane);
			Cutter->GenerateCutScalarsOff();
			Cutter->SetValue(0,0);
			
			cutStrips->SetInputConnection(Cutter->GetOutputPort());
			cutStrips->Update();
			
			cutPoly->SetPoints(cutStrips->GetOutput()->GetPoints());
			cutPoly->SetPolys(cutStrips->GetOutput()->GetLines());
			cutTriangles->SetInput(cutPoly);
			float fArea = 0;
			if (cutPoly->GetNumberOfPolys()>0)
			{
				cutArea->SetInput(cutTriangles->GetOutput());
				fArea = cutArea->GetSurfaceArea();
				Edge->SetInputConnection(cutTriangles->GetOutputPort());
				pEdgeMapper->ImmediateModeRenderingOn();
				pEdgeMapper->SetInputConnection(Edge->GetOutputPort());
				pEdgeActor->SetMapper(pEdgeMapper);

				INTERSECTLINE_PROPERTY LineInfo;
				LineInfo.Area = fArea;
				LineInfo.PlaneIndex = j;
				LineInfo.ShellIndex = i;
				LineInfo.Visibility = 1; //ballshell.Visibility;
				LineInfo.Color[0] = 0;
				LineInfo.Color[1] = 1;
				LineInfo.Color[2] = 0;
				LineInfo.Opacity = 1;
				LineInfo.ImageIndex=imageIndex;
				pProperty->SetLineWidth(3);
				pProperty->SetEdgeColor(LineInfo.Color);
				pProperty->SetAmbientColor(LineInfo.Color);
				pProperty->SetColor(LineInfo.Color);
				pProperty->SetDiffuseColor(LineInfo.Color);
				pProperty->SetSpecularColor(LineInfo.Color);
				pEdgeActor->SetProperty(pProperty);
				
				pEdgeActor->SetVisibility(LineInfo.Visibility);
				

				vtkTransform* Transform2 = vtkTransform::New();
				Transform2->SetMatrix(m_pToolAttach->GetMatrix());
				Transform2->Inverse();
				//pEdgeActor->SetPosition(Transform2->GetPosition());
				//pEdgeActor->SetOrientation(Transform2->GetOrientation());
				Transform2->Delete();
				if(imageIndex==1)
					CGlobalManager::Manager()->m_2DView1->GetRenderer()->AddActor(pEdgeActor);
				if(imageIndex==2)
					CGlobalManager::Manager()->m_2DView2->GetRenderer()->AddActor(pEdgeActor);
				if(imageIndex==3)
					CGlobalManager::Manager()->m_2DView3->GetRenderer()->AddActor(pEdgeActor);

				m_pIntersections->AddItem(pEdgeActor);
				m_arrIntersectionInfo.AddTail(LineInfo);
			}

			cutPoly->Delete();
			pEdgeActor->Delete();
			pEdgeMapper->Delete();
			pProperty->Delete();
			Edge->Delete();
			cutStrips->Delete();

			cutTriangles->Delete();
			cutArea->Delete();
		
		}
	}
	Cutter->Delete();
	
	return 0;
}
int CBallIceGalil::ShowIntersections(BOOL bShow)
{
	if(m_pToolAttach->m_eNeedleType.type!=1)
		return 0;

	m_bDisplayInterLine=bShow;
	POSITION POS;
	POS=m_arrIntersectionInfo.GetHeadPosition();
	m_pIntersections->InitTraversal();
	for(int i=0;i<m_arrIntersectionInfo.GetCount();i++)
	{
		INTERSECTLINE_PROPERTY interline=m_arrIntersectionInfo.GetNext(POS);
		vtkActor* pActor = (vtkActor*)(m_pIntersections->GetNextProp3D());
		if (bShow && (interline.Visibility == 1))
		{
			pActor->SetVisibility(1);
		}
		else
		{
			pActor->SetVisibility(0);
		}
	}
	return 0;
}

void CBallIceGalil::SetIntersectionProperty(INTERSECTLINE_PROPERTY intersection_property)
{
	if(m_pToolAttach->m_eNeedleType.type!=1)
		return ;

	int iIndex = -1;
	POSITION POS;
	POS=m_arrIntersectionInfo.GetHeadPosition();
	for(int i=0;i<m_arrIntersectionInfo.GetCount();i++)
	{
		INTERSECTLINE_PROPERTY interline=m_arrIntersectionInfo.GetNext(POS);
		if (intersection_property.PlaneIndex == interline.PlaneIndex 
			&& intersection_property.ImageIndex == interline.ImageIndex 
			&& intersection_property.ShellIndex == interline.ShellIndex)
		{
			iIndex = i;
		}
	}
	if(iIndex<0)
		return;

	vtkActor* pActor;
	this->m_pIntersections->InitTraversal();
	pActor = (vtkActor*)(this->m_pIntersections->GetItemAsObject(iIndex));
	vtkProperty* pProperty = vtkProperty::New();
	pProperty->SetColor(intersection_property.Color);
	pProperty->SetOpacity(intersection_property.Opacity);
	pActor->SetVisibility(intersection_property.Visibility);
	pActor->SetProperty(pProperty);
	pProperty->Delete();
	m_arrIntersectionInfo.SetAt(m_arrIntersectionInfo.FindIndex(iIndex),intersection_property);
}


void CBallIceGalil::GetBounds(double *bound)
{
	m_pShell->InitTraversal();
	int nCount = m_pShell->GetNumberOfItems(); 
	for(int i=0;i<nCount;i++)
	{
		vtkActor* pActor = (vtkActor*)m_pShell->GetNextProp3D();
		pActor->GetBounds(bound);
	}
}
