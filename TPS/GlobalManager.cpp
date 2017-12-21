// GlobalManager.cpp: implementation of the CGlobalManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalManager.h"
#include "ConfigMgr.h"
#include "PanaGlobal.h"
#include "Commands.h"
#include "ModalContour.h"
#include "ModalSeg.h"
#include "ModalVolume.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalManager::CGlobalManager()
{
	m_pTreeCtrl=NULL;

	m_pPrivateWnd=new CWnd();

	m_pFrmMgr=NULL;
	m_iTraceCount=0;
	m_iProbeCount=0;
	m_locateProbe=-1;

	m_pZonePoly = NULL;
	m_pAssemblys=vtkProp3DCollection::New();
	m_pProbeOnDrag=vtkProp3DCollection::New();
	m_pTracerOnDrag=vtkProp3DCollection::New();
	m_pModalOnDrag=vtkProp3DCollection::New();
	m_pZonePoly = vtkPolyDataCollection::New();
	m_pLabels = vtkProp3DCollection::New();

	m_bCanAddNewTrace=FALSE;
}

CGlobalManager::~CGlobalManager()
{
	delete m_pPrivateWnd;

	if (m_pZonePoly)	{	m_pZonePoly->Delete();	m_pZonePoly = NULL;	}
	if (m_pProbeOnDrag){ m_pProbeOnDrag->Delete(); m_pProbeOnDrag= NULL;}
	if (m_pTracerOnDrag){ m_pTracerOnDrag->Delete(); m_pTracerOnDrag= NULL;}
	if (m_pModalOnDrag){ m_pModalOnDrag->Delete(); m_pModalOnDrag= NULL;}
	if (m_pAssemblys)
	{	
		m_pAssemblys->RemoveAllItems();
			
		m_pAssemblys->Delete();	
		m_pAssemblys = NULL;	
	}
	if (m_pLabels) {m_pLabels->Delete(); }
	if(m_pRealProbe) { m_pRealProbe->Delete();}
}

CGlobalManager* CGlobalManager::Manager(void)
{
	static CGlobalManager manager;
	return &manager;
}


void CGlobalManager::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == m_pPrivateWnd)
		return;
	
	switch(nCmdID)
	{	
	case CMD_TPS_SELECTPROBE:
		SelectProbe(sPara);
		break;
	case CMD_TPS_SAVETPSINFOTOFILE:
		SaveTPSInfoToFile(sPara);
		break;	
	case CMD_TPS_MPR_GET_SELECTEDPROBE_COUNT:
		{
			int iCount=0;
			iCount+=m_pProbeOnDrag->GetNumberOfItems();
			iCount+=m_pTracerOnDrag->GetNumberOfItems();
			iCount+=m_pModalOnDrag->GetNumberOfItems();
			sPara.Format("%i",iCount);
		}
		break;
	case CMD_TPS_MPR_RELEASE_PROBE:
		ReleaseAssemblys(TOOL);
		break;
	case CMD_TPS_MPR_PROBE_POS:
		{
			double pos[3];
			int iFind;
			int iMode;
			iFind=sPara.Find(",",0);
			pos[0]=atof(sPara.Left(iFind));
			sPara=sPara.Right(sPara.GetLength()-iFind-1);
			
			iFind=sPara.Find(",",0);
			pos[1]=atof(sPara.Left(iFind));
			sPara=sPara.Right(sPara.GetLength()-iFind-1);
			
			iFind=sPara.Find(",",0);
			pos[2]=atof(sPara.Left(iFind));
			sPara=sPara.Right(sPara.GetLength()-iFind-1);
			iMode=atoi(sPara);
			
			ChangeProbePosition(pos,iMode);
		}
		break;
	case CMD_TPS_SETDEFAULTGROUP:
		SetDefaultTracerGroup(sPara);
		break;
	case CMD_TPS_INTERLINEVISIBLE:
		SetLineVisible(sPara);
		break;
	case CMD_TPS_LINEVISIBLESTATUS:
		InterLineVisibleStatus(sPara);
		break;
	case CMD_TPS_CHANGETRACERCOLOR:
		ChangeTracerColor(sPara);
		break;
	case CMD_TPS_SELECTTRACER:
		SelectTracer(sPara,FALSE);
		break;
	case CMD_TPS_SELECTTRACERTRACK:
		SelectTracer(sPara,TRUE);
		break;
	case CMD_TPS_SELECTMODAL:
		SelectModal(sPara);
		break;
	
	case CMD_TPS_DELETETRACE:
		DeleteTrace(sPara);
		break;
	case CMD_TPS_BALLVISIBLESTATUS:
		BallVisibleStatus(sPara);
		break;
	case CMD_TPS_ICEBALLVISIBLE:
		SetBallVisible(sPara);
		break;
	case CMD_TPS_CREATETOOL:
		CreateProbe();
		break;
	default:
		break;
	}
}
void CGlobalManager::LocateProbeTip()
{
	m_locateProbe=0;
}

void CGlobalManager::LocateProbeTail()
{
	m_locateProbe=1;
}

void CGlobalManager::LocateProbeNone()
{
	m_locateProbe=-1;
}

BOOL CGlobalManager::IsLocateProbeTip()
{
	return m_locateProbe==0;
}

BOOL CGlobalManager::IsLocateProbeTail()
{
	return m_locateProbe==1;
}

void CGlobalManager::SendCommand(int nCmdID,CString& sPara)
{
	if(m_pFrmMgr)
	{
		m_pFrmMgr->SendCommand(m_pPrivateWnd, nCmdID,sPara);
	}
}

void CGlobalManager::SelectProbe(CString para)
{
	ReleaseAssemblys(TOOL);
	ReleaseAssemblys(TRACER);
	ReleaseAssemblys(MODAL);


	int i;
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly && pAssembly->m_name==para)
		{
			AddAssemblys(pAssembly,TOOL);
									
					
			m_2DView1->Invalidate(FALSE);
			m_2DView2->Invalidate(FALSE);
			m_2DView3->Invalidate(FALSE);
			m_3DView->Invalidate(FALSE);

		}
	}	
}

void CGlobalManager::AssemblyPickableOff()
{
	m_pAssemblys->InitTraversal();
	vtkAssembly * pAssembly;
	
	for(int i=0; i<m_pAssemblys->GetNumberOfItems();i++)
	{
		pAssembly = (vtkAssembly*)m_pAssemblys->GetNextProp3D();
		vtkProp3DCollection* pProps = pAssembly->GetParts();
		pProps->InitTraversal();
		for(int j=0;j<pProps->GetNumberOfItems();j++)
		{
			vtkProp3D* pProp3D = pProps->GetNextProp3D();
			pProp3D->SetPickable(0);
		}
		pAssembly->SetPickable(0);
	}
	
}

void CGlobalManager::AssemblyPickableOn()
{
	m_pAssemblys->InitTraversal();
	vtkAssembly * pAssembly;
	
	for(int i=0; i<m_pAssemblys->GetNumberOfItems();i++)
	{
		pAssembly = (vtkAssembly*)m_pAssemblys->GetNextProp3D();
		vtkProp3DCollection* pProps = pAssembly->GetParts();
		pProps->InitTraversal();
		for(int j=0;j<pProps->GetNumberOfItems();j++)
		{
			vtkProp3D* pProp3D = pProps->GetNextProp3D();
			pProp3D->SetPickable(1);
		}
		pAssembly->SetPickable(1);
	}
	
}

void CGlobalManager::ReadProbeInfo()
{
	CConfigMgr conf;
	CString sTmp;
	CString str1,str2;
	str1="..\\config\\probe\\cryotype.txt";
	str2="..\\config\\probe\\biotype.txt";

	//读取冰球显示颜色
	COLORREF SHELL1COLOR=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL1_COLOR,RGB(0,255,128));
	COLORREF SHELL2COLOR=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL2_COLOR,RGB(0,255,255));
	COLORREF SHELL3COLOR=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL3_COLOR,RGB(0,128,255));


	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	
	CString m_strPath = szStylesPath;
	int nIndex  = m_strPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		m_strPath = m_strPath.Left(nIndex);
	}
	else {
		m_strPath.Empty();
	}

	if(str1=="")
	{
		PanaMessageBox("Please Set Cryotherapy Probe Information.");
		return;
	}
	if(str2=="")
	{
		PanaMessageBox("Please Set Biopsy Probe Information.");
		return;
	}
	sTmp.Format("%s\\%s",m_strPath,str1);
	str1=sTmp;
	sTmp.Format("%s\\%s",m_strPath,str2);
	str2=sTmp;

	CStdioFile file;
	if(!file.Open(str1,CFile::modeRead))
	{
		sTmp.Format("Can't open file %s",str1);
		PanaMessageBox(sTmp);
		return;
	}
	m_cryoProbeInfo.RemoveAll();
	
	NEEDLE_INFO needle;
	needle.shell1.Visibility=1;
	needle.shell1.Opacity = 50;
	needle.shell1.Color=SHELL1COLOR;
	needle.shell1.TipPos = 0;
	needle.shell1.EffectContour = 0;
	needle.shell1.Index = 0;

	needle.shell2.Visibility=1;
	needle.shell2.Opacity = 50;
	needle.shell2.Color=SHELL2COLOR;
	needle.shell2.TipPos = 0;
	needle.shell2.EffectContour = -20;
	needle.shell2.Index = 1;

	needle.shell3.Visibility=1;
	needle.shell3.Opacity = 50;
	needle.shell3.Color=SHELL3COLOR;
	needle.shell3.TipPos = 0;
	needle.shell3.EffectContour = -40;
	needle.shell3.Index = 2;
		
	int i;
	sTmp="";
	file.ReadString(sTmp);
	while(sTmp.GetLength()>0)
	{
		sTmp.TrimLeft();
		sTmp.TrimRight();
		i=sTmp.Find(" ",0);
		if(i>0)
			sprintf(needle.name,"%s",sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.needleLength = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.needleDiamter = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell1.Height  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell1.Diameter = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell1.doseOffset  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell2.Height  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell2.Diameter = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell2.doseOffset  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell3.Height  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		i=sTmp.Find(" ",0);
		if(i>0)
			needle.shell3.Diameter  = atof(sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();
			
		
		if(sTmp.GetLength()>0)
			needle.shell3.doseOffset  = atof(sTmp);
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str1);
			PanaMessageBox(sTmp);
			return;
		}
		needle.type = 1;
		m_cryoProbeInfo.AddTail(needle);
		file.ReadString(sTmp);
		sTmp.TrimLeft();
	}

	file.Close();

	if(!file.Open(str2,CFile::modeRead))
	{
		sTmp.Format("Can't open file %s.",str2);
		PanaMessageBox(sTmp);
		return;
	}
	m_bioProbeInfo.RemoveAll();
	
	file.ReadString(sTmp);
	while(sTmp.GetLength()>0)
	{
		sTmp.TrimLeft();
		sTmp.TrimRight();
		i=sTmp.Find(" ",0);
		if(i>0)
			sprintf(needle.name,"%s", sTmp.Left(i));
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str2);
			PanaMessageBox(sTmp);
			return;
		}
		sTmp=sTmp.Right(sTmp.GetLength()-i);
		sTmp.TrimLeft();

		if(sTmp.GetLength()>0)
			needle.needleDiamter = atof(sTmp);
		else
		{
			file.Close();
			sTmp.Format("File %s is not a correct config file.",str2);
			PanaMessageBox(sTmp);
			return;
		}

		needle.type = 0;
		m_bioProbeInfo.AddTail(needle);
		file.ReadString(sTmp);
		
	}

	file.Close();
}

CProbeCryo *CGlobalManager::GetProbeByName(CString sName)
{
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(int i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==sName)
		{
			return pAssembly;
		}
	}

	return NULL;
}

void CGlobalManager::CreateProbe()
{
	if(m_2DView1->m_pViewer==NULL &&
		m_2DView2->m_pViewer==NULL &&
		m_2DView3->m_pViewer==NULL)
	{
		return;
	}
	CTPS2DView *p2DView=NULL;
	if(m_2DView1->m_pViewer!=NULL)
	{
		p2DView=m_2DView1;
	}else if(m_2DView2->m_pViewer!=NULL)
	{
		p2DView=m_2DView2;
	}else
	{
		p2DView=m_2DView3;
	}

	double pos[3];
	double Ori[3];
	vtkTransform* pTrans = vtkTransform::New();
	pTrans->SetMatrix(p2DView->m_pImageFrame_scanner);
	pTrans->GetPosition(pos);
	pTrans->GetOrientation(Ori);
	
	pTrans->Delete();

	/*double dz = m_pViewer->GetSlice()*(m_PixelSpacing[2]);
	
	pos[0] = 100; //100;
	pos[1] = 100; //100;
	pos[2] = dz; //dz;
	
	Ori[0] = 0;
	Ori[1] = 0;
	Ori[2] = 0;*/
	NEEDLE_INFO needle;
	POSITION POS = m_cryoProbeInfo.GetHeadPosition();
	if(m_cryoProbeInfo.GetCount()<=0)
		return;

	for (int i=0;i<m_cryoProbeInfo.GetCount();i++)
	{
		needle = m_cryoProbeInfo.GetNext(POS);
		break;
	}

	//读取冰球显示颜色
	CConfigMgr conf;
	needle.shell1.Color=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL1_COLOR,RGB(0,255,128));
	needle.shell2.Color=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL2_COLOR,RGB(0,255,255));
	needle.shell3.Color=conf.Read(TPS_ICEBALL,TPS_ICEBALL_SHELL3_COLOR,RGB(0,128,255));

	CreateProbe(pos,Ori,needle);
	
	
}

void CGlobalManager::DeleteProbe(CString para)
{
	
	int i;
	CTPS3DView* p3Dview = m_3DView;
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==para)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				pAssembly->m_pAttachIceBall->m_pIntersections->InitTraversal();
				for(int j=0;j<pAssembly->m_pAttachIceBall->m_pIntersections->GetNumberOfItems();j++)
				{
					vtkActor* pActor = (vtkActor*)(pAssembly->m_pAttachIceBall->m_pIntersections->GetNextProp3D());
					if(m_2DView1)
						m_2DView1->GetRenderer()->RemoveActor(pActor);
					if(m_2DView2)
						m_2DView2->GetRenderer()->RemoveActor(pActor);
					if(m_2DView3)
						m_2DView3->GetRenderer()->RemoveActor(pActor);
				}
				pAssembly->m_pAttachIceBall->m_pIntersections->RemoveAllItems();
				pAssembly->m_pAttachIceBall->m_arrIntersectionInfo.RemoveAll();
				
			}
			m_pAssemblys->RemoveItem(pAssembly);
			m_pProbeOnDrag->RemoveItem(pAssembly);
			
			m_2DView1->GetRenderer()->RemoveActor(pAssembly);
			m_2DView1->GetRenderer()->RemoveActor(pAssembly->probe1);
			m_2DView1->GetRenderer()->RemoveActor(pAssembly->cross1);
			
			m_2DView2->GetRenderer()->RemoveActor(pAssembly);
			m_2DView2->GetRenderer()->RemoveActor(pAssembly->probe2);
			m_2DView2->GetRenderer()->RemoveActor(pAssembly->cross2);			
			
			m_2DView3->GetRenderer()->RemoveActor(pAssembly);
			m_2DView3->GetRenderer()->RemoveActor(pAssembly->probe3);
			m_2DView3->GetRenderer()->RemoveActor(pAssembly->cross3);
			
			
			p3Dview->GetRenderer()->RemoveActor(pAssembly->probe4);
			p3Dview->RemoveProbe(pAssembly);
			

			m_2DView1->Invalidate(FALSE);
			m_2DView2->Invalidate(FALSE);
			m_2DView3->Invalidate(FALSE);
			m_3DView->Invalidate(FALSE);

		}
	}	
}

void CGlobalManager::CreateProbe(double pos[], double Ori[], NEEDLE_INFO eNeedleType,CString probeName)
{
	if(m_2DView1->m_pViewer==NULL &&
		m_2DView2->m_pViewer==NULL &&
		m_2DView3->m_pViewer==NULL)
	{
		return;
	}
	CTPS2DView *p2DView=NULL;
	if(m_2DView1->m_pViewer!=NULL)
	{
		p2DView=m_2DView1;
	}else if(m_2DView2->m_pViewer!=NULL)
	{
		p2DView=m_2DView2;
	}else
	{
		p2DView=m_2DView3;
	}

	//get unique name
	CString sName;
	if(probeName!="")
	{
		sName=probeName;
	}else
	{
		sName.Format("CryoProbe%i",m_iProbeCount);
		m_pAssemblys->InitTraversal();
		int nCount = m_pAssemblys->GetNumberOfItems(); 
		for(int i=0;i<nCount;i++)
		{
			CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
			if(pAssembly->m_name==sName)
			{
				m_iProbeCount++;
				sName.Format("CryoProbe%i",m_iProbeCount);
				m_pAssemblys->InitTraversal();
				i=-1;
			}
		}
		m_iProbeCount++;
	}
	
	
	CProbeCryo* pCryProbe = CProbeCryo::New();
	pCryProbe->m_pMsgWindow=p2DView;
	pCryProbe->Initial(eNeedleType);
	pCryProbe->ToolAssembly();
	pCryProbe->m_name=sName;
	
	
	pCryProbe->SetInitPosition(pos);
	//pCryProbe->SetOrientation(Ori);
	pCryProbe->RotateZ(Ori[2]);
	pCryProbe->RotateX(Ori[0]);
	pCryProbe->RotateY(Ori[1]);
	pCryProbe->UpdateProbeCoord();

	m_pAssemblys->AddItem(pCryProbe);
		
	
	if(m_2DView1->m_pViewer!=NULL)
	{
		m_2DView1->GetRenderer()->AddActor(pCryProbe);
		m_2DView1->GetRenderer()->AddActor(pCryProbe->probe1); 
		m_2DView1->GetRenderer()->AddActor(pCryProbe->cross1);
		
	}
	if(m_2DView2->m_pViewer!=NULL)
	{
		m_2DView2->GetRenderer()->AddActor(pCryProbe);
		m_2DView2->GetRenderer()->AddActor(pCryProbe->probe2); 
		m_2DView2->GetRenderer()->AddActor(pCryProbe->cross2);
		
	}
	if(m_2DView3->m_pViewer!=NULL)
	{
		m_2DView3->GetRenderer()->AddActor(pCryProbe);
		m_2DView3->GetRenderer()->AddActor(pCryProbe->probe3); 
		m_2DView3->GetRenderer()->AddActor(pCryProbe->cross3); 
	}
	
	
	SendCommand(CMD_TPS_CREATEPROBEOVER,sName);
	
	CTPS3DView* p3Dview = m_3DView;
	if(p3Dview!=NULL)
	{
		
		p3Dview->AddIceBall(pCryProbe);
		p3Dview->GetRenderer()->AddActor(pCryProbe->probe4); 
		p3Dview->RefreshPoly();
	}
			
	pCryProbe->Delete();

	if(m_2DView1!=NULL&&m_2DView1->m_pViewer!=NULL)
	{
		m_2DView1->m_pViewer->UpdateDisplayExtent();
		m_2DView1->m_pViewer->Render();
	}
				
	if(m_2DView2!=NULL&&m_2DView2->m_pViewer!=NULL)
	{
		m_2DView2->m_pViewer->UpdateDisplayExtent();
		m_2DView2->m_pViewer->Render();
	}
	if(m_2DView3!=NULL&&m_2DView3->m_pViewer!=NULL)
	{
		m_2DView3->m_pViewer->UpdateDisplayExtent();
		m_2DView3->m_pViewer->Render();
	}

	

}

void CGlobalManager::ChangeProbePosition(double *pos, int status)
{
	CString sPara;
	
	if(m_pProbeOnDrag->GetNumberOfItems()>0)
	{
		m_pProbeOnDrag->InitTraversal();
		for(int i=0;i<m_pProbeOnDrag->GetNumberOfItems();i++)
		{
			
			CProbeCryo * pAssembly=NULL;
			pAssembly = (CProbeCryo*)m_pProbeOnDrag->GetNextProp3D();
			pAssembly->ChangePositionByTarget(pos,status);
			
			PlaneIntersection(pAssembly);
			
			m_2DView1->GetRenderer()->Render();
			m_2DView1->Invalidate(FALSE);
			SendCommand(CMD_TPS_RENDERREFRESH,sPara);
			
		}
	}else
	{
		SendCommand(CMD_TPS_MPR_SHOWSTATUS,sPara);
		if(sPara=="1")
		{
			SendCommand(CMD_TPS_MPR_PROBENAME,sPara);
			CProbeCryo * pAssembly=GetProbeByName(sPara);
			if(pAssembly==NULL)
				return;

			pAssembly->ChangePositionByTarget(pos,status);
			
			PlaneIntersection(pAssembly);
			
			m_2DView1->GetRenderer()->Render();
			m_2DView1->Invalidate(FALSE);
			SendCommand(CMD_TPS_RENDERREFRESH,sPara);
		}
	}
}

void CGlobalManager::AddAssemblys(vtkProp3D *pAssemblys, POLYTYPE ePolyType)
{
	if(!pAssemblys)
		return;

	CString sPara;
	POSITION pos;
	double color[3];
	if(ePolyType==TOOL)
	{
		m_pProbeOnDrag->AddItem(pAssemblys);
		((CProbeCryo*)pAssemblys)->SetColor(1,0,0);

		sPara=((CProbeCryo*)pAssemblys)->m_name;
		((CProbeCryo*)pAssemblys)->DisplayAxes();
		SendCommand(CMD_TPS_SELECTPROBE,sPara);
		SendCommand(CMD_TPS_SHOW_PROBECONTROL,sPara);
	}
	if(ePolyType==TRACER)
	{
		POSITION pos1;
		m_pTracerOnDrag->AddItem(pAssemblys);
		ACTOR_INFO actorinfo;
		POLY_INFO polyinfo;
		pos=m_ActorInfoList.GetHeadPosition();
		pos1=m_PolyInfoList.GetHeadPosition();
		for(int i=0;i<m_ActorInfoList.GetCount();i++)
		{
			actorinfo=m_ActorInfoList.GetNext(pos);
			polyinfo=m_PolyInfoList.GetNext(pos1);
			if(actorinfo.p==pAssemblys)
			{
				actorinfo.p->GetProperty()->GetColor(color);
				actorinfo.color = RGB(color[0]*255,color[1]*255,color[2]*255);
				actorinfo.p->GetProperty()->SetColor(1,0,0);
				m_ActorInfoList.SetAt(m_ActorInfoList.FindIndex(i),actorinfo);
				sPara.Format("%s",polyinfo.sDscr);
				SendCommand(CMD_TPS_SELECTTRACER,sPara);
				break;
			}
		}
	}

	if(ePolyType==MODAL)
	{
		m_pModalOnDrag->AddItem(pAssemblys);
		//((vtkActor *)pAssemblys)->GetProperty()->SetColor(1,0,0);

		CModal* modalinfo;
		
		for(int i=0;i<m_modalGrpList.GetCount();i++)
		{
			modalinfo=m_modalGrpList.GetAt(m_modalGrpList.FindIndex(i));
			vtkActor *pActor;
			pActor=(vtkActor*)modalinfo->m_actor;
			if(pActor==pAssemblys)
			{
								
				sPara.Format("%s",modalinfo->m_name);
				SendCommand(CMD_TPS_SELECTMODAL,sPara);
				break;
			}
		}
		
	}
	
}

void CGlobalManager::ReleaseAssemblys(POLYTYPE ePolyType)
{
	POSITION pos;
	double color[3];
	if(ePolyType==TOOL)
	{
		m_pProbeOnDrag->InitTraversal();
		for(int i=0;i<m_pProbeOnDrag->GetNumberOfItems();i++)
		{
			vtkAssembly * pAssembly;
			pAssembly = (vtkAssembly*)m_pProbeOnDrag->GetNextProp3D();
		
			((CProbeCryo*)pAssembly)->RestoreColor();
			((CProbeCryo*)pAssembly)->HideAxes();
				
		}
		m_pProbeOnDrag->RemoveAllItems();

		CString sTmp="";
		SendCommand(CMD_TPS_HIDE_PROBECONTROL,sTmp);
	}
	if(ePolyType==TRACER)
	{ 
		m_pTracerOnDrag->InitTraversal();
		for(int i=0;i<m_pTracerOnDrag->GetNumberOfItems();i++)
		{
			vtkActor *pActor;
			pActor = (vtkActor*)m_pTracerOnDrag->GetNextProp3D();
			ACTOR_INFO actorinfo;
			pos=m_ActorInfoList.GetHeadPosition();
			for(int j=0;j<m_ActorInfoList.GetCount();j++)
			{
				actorinfo=m_ActorInfoList.GetNext(pos);
				if(actorinfo.p==pActor)
				{
					color[0]=GetRValue(actorinfo.color)/255.0;
					color[1]=GetGValue(actorinfo.color)/255.0;
					color[2]=GetBValue(actorinfo.color)/255.0;
					actorinfo.p->GetProperty()->SetColor(color);
					break;
				}
			}	
		}
		m_pTracerOnDrag->RemoveAllItems();
	}

	if(ePolyType==MODAL)
	{ 
		CModal* modalinfo;
		m_pModalOnDrag->InitTraversal();
		for(int i=0;i<m_pModalOnDrag->GetNumberOfItems();i++)
		{
			vtkActor *pActor,*pActor2;
			pActor = (vtkActor*)m_pModalOnDrag->GetNextProp3D();

			
			for(int j=0;j<m_modalGrpList.GetCount();j++)
			{
				modalinfo=m_modalGrpList.GetAt(m_modalGrpList.FindIndex(j));
				pActor2 = (vtkActor*)modalinfo->m_actor;
				if(pActor==pActor2)
				{
					
					color[0]=GetRValue(modalinfo->m_color)/255.0;
					color[1]=GetGValue(modalinfo->m_color)/255.0;
					color[2]=GetBValue(modalinfo->m_color)/255.0;
					//pActor->GetProperty()->SetColor(color);
					break;
				}
			}	
		}
		m_pModalOnDrag->RemoveAllItems();
	}
}

void CGlobalManager::PlaneIntersection(CProbeCryo* pCrypProbe,CTPS2DView *p2DView)
{
	vtkPlaneCollection* pPlanes = vtkPlaneCollection::New();
	float dZpos;
	double position[3];
	vtkPlane *pPlane;
	if(p2DView)
	{
		pPlane = vtkPlane::New();
		dZpos = p2DView->m_imageInfo.spacing[2]*p2DView->m_pViewer->GetSlice();
	
	
		position[0]=p2DView->m_pImageFrame_scanner->GetElement(0,3)+p2DView->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
		position[1]=p2DView->m_pImageFrame_scanner->GetElement(1,3)+p2DView->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
		position[2]=p2DView->m_pImageFrame_scanner->GetElement(2,3)+p2DView->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

		pPlane->SetOrigin( position[0], position[1], position[2]);
		pPlane->SetNormal(p2DView->m_pImageFrame_scanner->GetElement(0,2),
			p2DView->m_pImageFrame_scanner->GetElement(1,2),p2DView->m_pImageFrame_scanner->GetElement(2,2));
		pPlanes->AddItem(pPlane);
		pPlane->Delete();
	
	
		if (pCrypProbe->m_pAttachIceBall)
		{
			pCrypProbe->m_pAttachIceBall->GetImageIntersection(pPlanes,p2DView->m_instanceIndex);
			
		}
	}else
	{
		
		if(m_2DView1->m_imageInfo.bValid)
		{
			pPlane= vtkPlane::New();
			dZpos =  m_2DView1->m_imageInfo.spacing[2]*m_2DView1->m_pViewer->GetSlice();
	
	
			position[0]=m_2DView1->m_pImageFrame_scanner->GetElement(0,3)+m_2DView1->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
			position[1]=m_2DView1->m_pImageFrame_scanner->GetElement(1,3)+m_2DView1->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
			position[2]=m_2DView1->m_pImageFrame_scanner->GetElement(2,3)+m_2DView1->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

			pPlane->SetOrigin( position[0], position[1], position[2]);
			pPlane->SetNormal(m_2DView1->m_pImageFrame_scanner->GetElement(0,2),
				m_2DView1->m_pImageFrame_scanner->GetElement(1,2),m_2DView1->m_pImageFrame_scanner->GetElement(2,2));
			pPlanes->AddItem(pPlane);
			pPlane->Delete();
	
	
			if (pCrypProbe->m_pAttachIceBall)
			{
				pCrypProbe->m_pAttachIceBall->GetImageIntersection(pPlanes,1);
			
			}
		}

		if(m_2DView2->m_imageInfo.bValid)
		{
		
			//get 2stimage intersection
			pPlanes->RemoveAllItems();
			pPlane= vtkPlane::New();
			dZpos =  m_2DView2->m_imageInfo.spacing[2]*m_2DView2->m_pViewer->GetSlice();
	
	
			position[0]=m_2DView2->m_pImageFrame_scanner->GetElement(0,3)+m_2DView2->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
			position[1]=m_2DView2->m_pImageFrame_scanner->GetElement(1,3)+m_2DView2->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
			position[2]=m_2DView2->m_pImageFrame_scanner->GetElement(2,3)+m_2DView2->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

			pPlane->SetOrigin( position[0], position[1], position[2]);
			pPlane->SetNormal(m_2DView2->m_pImageFrame_scanner->GetElement(0,2),
				m_2DView2->m_pImageFrame_scanner->GetElement(1,2),m_2DView2->m_pImageFrame_scanner->GetElement(2,2));
			pPlanes->AddItem(pPlane);
			pPlane->Delete();
	
	
			if (pCrypProbe->m_pAttachIceBall)
			{
				pCrypProbe->m_pAttachIceBall->GetImageIntersection(pPlanes,2);
			
			}
		}

		if(m_2DView3->m_imageInfo.bValid)
		{
		

			//get 3stimage intersection
			pPlanes->RemoveAllItems();
			pPlane= vtkPlane::New();
			dZpos =  m_2DView3->m_imageInfo.spacing[2]*m_2DView3->m_pViewer->GetSlice();
	
	
			position[0]=m_2DView3->m_pImageFrame_scanner->GetElement(0,3)+m_2DView3->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
			position[1]=m_2DView3->m_pImageFrame_scanner->GetElement(1,3)+m_2DView3->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
			position[2]=m_2DView3->m_pImageFrame_scanner->GetElement(2,3)+m_2DView3->m_pImageFrame_scanner->GetElement(2,2)*dZpos;

			pPlane->SetOrigin( position[0], position[1], position[2]);
			pPlane->SetNormal(m_2DView3->m_pImageFrame_scanner->GetElement(0,2),
				m_2DView3->m_pImageFrame_scanner->GetElement(1,2),m_2DView3->m_pImageFrame_scanner->GetElement(2,2));
			pPlanes->AddItem(pPlane);
			pPlane->Delete();
	
	
			if (pCrypProbe->m_pAttachIceBall)
			{
				pCrypProbe->m_pAttachIceBall->GetImageIntersection(pPlanes,3);
			
			}
		}
	}
	pPlanes->Delete();
	
}

void CGlobalManager::ReplaceProbe(double dpos[], double dori[])
{
	
	m_pProbeOnDrag->InitTraversal();
	for(int i=0;i<m_pProbeOnDrag->GetNumberOfItems();i++)
	{
		
		CProbeCryo * pAssembly;
		pAssembly = (CProbeCryo*)m_pProbeOnDrag->GetNextProp3D();
		pAssembly->AddPositions(dpos);
		pAssembly->RotateZ(dori[2]);
		pAssembly->RotateX(dori[0]);
		pAssembly->RotateY(dori[1]);

		PlaneIntersection(pAssembly);

		pAssembly->UpdateProbeCoord();
		//GetRenderer()->Render();
		
	}
	
}

BOOL CGlobalManager::SaveTPSInfoToFile(CString fileName)
{
	if( !m_2DView1->m_imageInfo.bValid &&
		!m_2DView2->m_imageInfo.bValid &&
		!m_2DView3->m_imageInfo.bValid)
	{
		return TRUE;
	}
	CString sTmp;

	//分离出tps文件所在的目录
	fileName.MakeLower();
	if(fileName.GetLength()<4 || fileName.Right(4)!=".tps")
	{
		PanaMessageBox("Not valid TPS file.");
		return FALSE;
	}
	int iFind=fileName.ReverseFind('\\');
	if(iFind<2)
	{
		PanaMessageBox("Not valid TPS file.");
		return FALSE;
	}
	CString sTpsFolder=fileName.Left(iFind);
	CString sTpsFilename=fileName.Mid(iFind+1,fileName.GetLength()-iFind-5);
	sTmp.Format("%s\\%s",sTpsFolder,sTpsFilename);
	sTpsFolder=sTmp;
	
	//创建临时目录
	CFileFind finder;
	if(!finder.FindFile(sTpsFolder))
	{
		if(!::CreateDirectory( sTpsFolder, NULL ))
		{			
			sTmp.Format("Failed to create %s. Please check it.",sTpsFolder);
			PanaMessageBox(sTmp);
			return FALSE;
		}
	}
	sTmp.Format("%s\\Series",sTpsFolder);
	if(!finder.FindFile(sTmp))
	{
		if(!::CreateDirectory( sTmp, NULL ))
		{			
			sTmp.Format("Failed to create %s. Please check it.",sTmp);
			PanaMessageBox(sTmp);
			return FALSE;
		}
	}
	sTmp.Format("%s\\Modal",sTpsFolder);
	if(!finder.FindFile(sTmp))
	{
		if(!::CreateDirectory( sTmp, NULL ))
		{			
			sTmp.Format("Failed to create %s. Please check it.",sTmp);
			PanaMessageBox(sTmp);
			return FALSE;
		}
	}

	FILE* file;
	if((file = fopen (fileName, "w+b")) == NULL)
	{
		TRACE(_T("CTPS2DView::OpenDataFile failed"));
		PanaMessageBox("Failed to open data file.");
		return FALSE;
	}
	fseek(file,0,SEEK_SET);

	//Save head info
	char headInfo[100];
	sprintf(headInfo,"TPS version 1.2. XinAoMDT Technology Co.,Ltd");
	fwrite(headInfo,sizeof(char),100,file);

	//save image path
	char imagePath[100];
	if(m_2DView1->m_imageInfo.bValid)
	{
		CString sOldPath;
		sOldPath.Format("%s",m_2DView1->m_imageInfo.ImagePath);
		CString sNewPath;
		sNewPath.Format("%s\\Series\\%s",sTpsFolder,m_2DView1->m_imageInfo.ImageName);
		sprintf(imagePath,"Series\\%s",m_2DView1->m_imageInfo.ImageName);
		if(sOldPath!=sNewPath)
		{
			CFileFind finder;
			if(!finder.FindFile(sNewPath))
			{
				if(m_2DView1->m_pSeries)
				{
					m_2DView1->m_pSeries->Save(sNewPath);
					m_2DView1->m_pSeries->SaveFusionCoord(sNewPath);
				}else
				{
					CopyFolder(sOldPath,sNewPath,"*.img");
					CopyFolder(sOldPath,sNewPath,"*.mtx");
				}
			}else
			{
				if(m_2DView1->m_pSeries)
				{
					m_2DView1->m_pSeries->SaveFusionCoord(sNewPath);
				}else
				{
					CopyFolder(sOldPath,sNewPath,"*.mtx");
				}
			}
		}
	}else
	{
		sprintf(imagePath,"");
	}
	fwrite(imagePath,sizeof(char),100,file);
	if(m_2DView2->m_imageInfo.bValid)
	{
		CString sOldPath;
		sOldPath.Format("%s",m_2DView2->m_imageInfo.ImagePath);
		CString sNewPath;
		sNewPath.Format("%s\\Series\\%s",sTpsFolder,m_2DView2->m_imageInfo.ImageName);
		sprintf(imagePath,"Series\\%s",m_2DView2->m_imageInfo.ImageName);
		if(sOldPath!=sNewPath)
		{
			CFileFind finder;
			if(!finder.FindFile(sNewPath))
			{
				if(m_2DView2->m_pSeries)
				{
					m_2DView2->m_pSeries->Save(sNewPath);
					m_2DView2->m_pSeries->SaveFusionCoord(sNewPath);
				}
			}else
			{
				if(m_2DView2->m_pSeries)
				{
					m_2DView2->m_pSeries->SaveFusionCoord(sNewPath);
				}
			}
		}
	}else
	{
		sprintf(imagePath,"");
	}
	fwrite(imagePath,sizeof(char),100,file);
	if(m_2DView3->m_imageInfo.bValid)
	{
		CString sOldPath;
		sOldPath.Format("%s",m_2DView3->m_imageInfo.ImagePath);
		CString sNewPath;
		sNewPath.Format("%s\\Series\\%s",sTpsFolder,m_2DView3->m_imageInfo.ImageName);
		sprintf(imagePath,"Series\\%s",m_2DView3->m_imageInfo.ImageName);
		if(sOldPath!=sNewPath)
		{
			CFileFind finder;
			if(!finder.FindFile(sNewPath))
			{
				if(m_2DView3->m_pSeries)
				{
					m_2DView3->m_pSeries->Save(sNewPath);
					m_2DView3->m_pSeries->SaveFusionCoord(sNewPath);
				}
			}else
			{
				if(m_2DView3->m_pSeries)
				{
					m_2DView3->m_pSeries->SaveFusionCoord(sNewPath);
				}
			}
		}
	}else
	{
		sprintf(imagePath,"");
	}
	fwrite(imagePath,sizeof(char),100,file);

	//save tracer group
	int iTracerGroupCount;
	iTracerGroupCount=m_tracerGrpList.GetCount();
	fwrite(&iTracerGroupCount,sizeof(int),1,file);

	if(iTracerGroupCount>0)
	{
		ZONEGROUP_INFO *zoneGroup=new ZONEGROUP_INFO[iTracerGroupCount];
		POSITION pos = m_tracerGrpList.GetHeadPosition();
		for (int i=0;i<m_tracerGrpList.GetCount();i++)
		{
			zoneGroup[i] = m_tracerGrpList.GetNext(pos);
			
		}
		fwrite(zoneGroup,sizeof(ZONEGROUP_INFO),iTracerGroupCount,file);
		delete []zoneGroup;
	}

	//save tracer count
	int iTracerCount=0;
	iTracerCount=m_PolyInfoList.GetCount();
	fwrite(&iTracerCount,sizeof(int),1,file);

	//save tracer info
	if(iTracerCount>0)
	{
		POLY_INFO *polyinfo=new POLY_INFO[iTracerCount];
		POLY_INFO tmpPoly;
		POSITION pos = m_PolyInfoList.GetHeadPosition();
		for (int i=0;i<m_PolyInfoList.GetCount();i++)
		{
			tmpPoly = m_PolyInfoList.GetNext(pos);
			polyinfo[i]=tmpPoly;
		}
		fwrite(polyinfo,sizeof(POLY_INFO),iTracerCount,file);
		delete []polyinfo;
	}


	//save trace path
	vtkPolyData *pData;
	vtkPoints * pPoints;
	int polyCount=0;
	polyCount=m_pZonePoly->GetNumberOfItems();
	if(polyCount!=iTracerCount){
		TRACE(_T("CTPS2DView::SaveDataFile failed"));
		PanaMessageBox("CTPS2DView::SaveTPSInfoToFile:Internal failure.");
		fclose(file);
		return FALSE;
	}
	fwrite(&polyCount,sizeof(int),1,file);

	double *dPoints,*dPoint;
	m_pZonePoly->InitTraversal();
	for(int i=0;i<polyCount;i++)
	{
		pData=m_pZonePoly->GetNextItem();
		pPoints = pData->GetPoints();
		int num = pPoints->GetNumberOfPoints();
		fwrite(&num,sizeof(int),1,file);
		dPoints=new double[num*3];
		for(int j=0;j<num;j++)
		{
			dPoint = pPoints->GetPoint(j);
			dPoints[j*3]=dPoint[0];
			dPoints[j*3+1]=dPoint[1];
			dPoints[j*3+2]=dPoint[2];
		}
		fwrite(dPoints,sizeof(double),num*3,file);
		delete dPoints;
	}

	//save probe count
	int probeCount;
	NEEDLE_INFO needletype;
	char probeName[100];
	probeCount= m_pAssemblys->GetNumberOfItems();
	fwrite(&probeCount,sizeof(int),1,file);

	//save probe
	m_pAssemblys->InitTraversal();
	for(i=0;i<probeCount;i++)
	{
		double pos[3];
		double ori[3];
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		
		pAssembly->GetPosition(pos);
		pAssembly->GetOrientation(ori);
		
				
		double tmpData[3];

		tmpData[0]=pos[0];
		tmpData[1]=pos[1];
		tmpData[2]=pos[2];
		fwrite(tmpData,sizeof(double),3,file);
		
		tmpData[0]=ori[0];
		tmpData[1]=ori[1];
		tmpData[2]=ori[2];
		fwrite(tmpData,sizeof(double),3,file);

		needletype=pAssembly->m_eNeedleType;
		fwrite(&needletype,sizeof(NEEDLE_INFO),1,file);

		sprintf(probeName,"%s",pAssembly->m_name);
		fwrite(probeName,sizeof(char),100,file);
	}
		
	//save modal group
	int iModalGroupCount;
	iModalGroupCount=m_modalGrpList.GetCount();
	fwrite(&iModalGroupCount,sizeof(int),1,file);

	CModal* pModal;
	POSITION pos=m_modalGrpList.GetHeadPosition();
	while(pos)
	{
		pModal=m_modalGrpList.GetNext(pos);
		if(pModal->m_modalType==ModalDrag)
		{
			CModalDrag* pModalDrag=(CModalDrag*)pModal;
			if(!pModalDrag->save(file))
			{
				//
			}
		}else if(pModal->m_modalType==ModalContour)
		{
			CModalContour* pModalContour=(CModalContour*)pModal;
			if(!pModalContour->save(file))
			{
				//
			}
		}else if(pModal->m_modalType==ModalSeg)
		{
			CModalSeg* pModalSeg=(CModalSeg*)pModal;
			if(!pModalSeg->save(file,fileName))
			{
				//
			}
		}else if(pModal->m_modalType==ModalVolume)
		{
			CModalVolume* pModalVolume=(CModalVolume*)pModal;
			if(!pModalVolume->save(file))
			{
				//
			}
		}
	}

	//save measurement result
	int iMeasureCount;
	char cMeasureType[20];
	iMeasureCount=m_2DView1->m_pMeasureList->GetNumberOfItems();
	fwrite(&iMeasureCount,sizeof(int),1,file);


	if(iMeasureCount>0)
	{
		double pos[3];
		vtkAbstractWidget* pWidget;
		m_2DView1->m_pMeasureList->InitTraversal();
		for	(int i= 0;i<m_2DView1->m_pMeasureList->GetNumberOfItems();i++)
		{
			pWidget=(vtkAbstractWidget* )m_2DView1->m_pMeasureList->GetNextItemAsObject();
			if(pWidget->IsA("vtkDistanceWidget"))
			{
				sprintf(cMeasureType,"Distance");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkDistanceRepresentation2D *DisRep;
				DisRep=(vtkDistanceRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}

			if(pWidget->IsA("vtkAngleWidget"))
			{
				sprintf(cMeasureType,"Angle");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkAngleRepresentation2D *DisRep;
				DisRep=(vtkAngleRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetCenterWorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}
		

		}

	}

	iMeasureCount=m_2DView2->m_pMeasureList->GetNumberOfItems();
	fwrite(&iMeasureCount,sizeof(int),1,file);


	if(iMeasureCount>0)
	{
		double pos[3];
		vtkAbstractWidget* pWidget;
		m_2DView2->m_pMeasureList->InitTraversal();
		for	(int i= 0;i<m_2DView2->m_pMeasureList->GetNumberOfItems();i++)
		{
			pWidget=(vtkAbstractWidget* )m_2DView2->m_pMeasureList->GetNextItemAsObject();
			if(pWidget->IsA("vtkDistanceWidget"))
			{
				sprintf(cMeasureType,"Distance");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkDistanceRepresentation2D *DisRep;
				DisRep=(vtkDistanceRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}

			if(pWidget->IsA("vtkAngleWidget"))
			{
				sprintf(cMeasureType,"Angle");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkAngleRepresentation2D *DisRep;
				DisRep=(vtkAngleRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetCenterWorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}
		

		}

	}

	iMeasureCount=m_2DView3->m_pMeasureList->GetNumberOfItems();
	fwrite(&iMeasureCount,sizeof(int),1,file);


	if(iMeasureCount>0)
	{
		double pos[3];
		vtkAbstractWidget* pWidget;
		m_2DView3->m_pMeasureList->InitTraversal();
		for	(int i= 0;i<m_2DView3->m_pMeasureList->GetNumberOfItems();i++)
		{
			pWidget=(vtkAbstractWidget* )m_2DView3->m_pMeasureList->GetNextItemAsObject();
			if(pWidget->IsA("vtkDistanceWidget"))
			{
				sprintf(cMeasureType,"Distance");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkDistanceRepresentation2D *DisRep;
				DisRep=(vtkDistanceRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}

			if(pWidget->IsA("vtkAngleWidget"))
			{
				sprintf(cMeasureType,"Angle");
				fwrite(cMeasureType,sizeof(char),20,file);
				vtkAngleRepresentation2D *DisRep;
				DisRep=(vtkAngleRepresentation2D *)pWidget->GetRepresentation();
				DisRep->GetPoint1WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetCenterWorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
				DisRep->GetPoint2WorldPosition(pos);
				fwrite(pos,sizeof(double),3,file);
			}		

		}
	}	

	fclose(file);
	return TRUE;
	
}

BOOL CGlobalManager::LoadTPSInfoFile(CString fileName)
{	
	
	FILE* file;
	CString sTmp;
	int iReadCount;

	//分离出tps文件所在的目录
	fileName.MakeLower();
	if(fileName.GetLength()<4 || fileName.Right(4)!=".tps")
	{
		PanaMessageBox("Not valid TPS file.");
		return FALSE;
	}
	int iFind=fileName.ReverseFind('\\');
	if(iFind<2)
	{
		PanaMessageBox("Not valid TPS file.");
		return FALSE;
	}
	CString sTpsFolder=fileName.Left(iFind);
	CString sTpsFilename=fileName.Mid(iFind+1,fileName.GetLength()-iFind-5);
	sTmp.Format("%s\\%s\\",sTpsFolder,sTpsFilename);
	sTpsFolder=sTmp;
	
	
	if((file = fopen (fileName, "rb")) == NULL)
	{
		TRACE(_T("CTPS2DView::OpenDataFile failed"));
		PanaMessageBox("Failed to open data file.");
		return FALSE;
	}
	fseek(file,0,SEEK_SET);

	//read head info
	char headInfo[100];
	iReadCount=fread(headInfo,sizeof(char),100,file);
	if(iReadCount!=100)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		
		return FALSE;
	}
	sTmp.Format("%s",headInfo);
	if(sTmp!="TPS version 1.2. XinAoMDT Technology Co.,Ltd")
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		
		return FALSE;
	}

	//read image path
	char imagePath[100];
	CString sFolderName,sFolderName2,sFolderName3;
	iReadCount=fread(imagePath,sizeof(char),100,file);
	if(iReadCount!=100)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		
		return FALSE;
	}
	sFolderName.Format("%s%s",sTpsFolder,imagePath);
	sTmp.Format("%s",imagePath);
	ShowWaiting(1,"Loading Series...");
	if(sTmp!="" && !m_2DView1->LoadSeries(sFolderName))
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}

	iReadCount=fread(imagePath,sizeof(char),100,file);
	if(iReadCount!=100)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}
	sFolderName2.Format("%s%s",sTpsFolder,imagePath);
	sTmp.Format("%s",imagePath);
	if(m_2DView2)
	{
		if(sTmp!="" && !m_2DView2->LoadSeries(sFolderName2))
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
	}

	iReadCount=fread(imagePath,sizeof(char),100,file);
	if(iReadCount!=100)
	{
		PanaMessageBox("Failed to open data file.");
		fclose(file);
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}
	sFolderName3.Format("%s%s",sTpsFolder,imagePath);
	sTmp.Format("%s",imagePath);
	if(m_2DView3)
	{
		if(sTmp !="" && !m_2DView3->LoadSeries(sFolderName3))
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
	}
	
	ShowWaiting(1,"Loading Tracer...");
	//read tracer group
	int iTracerGroupCount;
	iTracerGroupCount=m_tracerGrpList.GetCount();
	iReadCount=fread(&iTracerGroupCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}

	if(iTracerGroupCount>0)
	{
		m_tracerGrpList.RemoveAll();
		for(int i=0;i<iTracerGroupCount;i++)
		{
			ZONEGROUP_INFO zoneGroup;
			iReadCount=fread(&zoneGroup,sizeof(ZONEGROUP_INFO),1,file);
			if(iReadCount!=1)
			{
				fclose(file);
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
			m_tracerGrpList.AddTail(zoneGroup);
		}
	}

	CString sPara;
	SendCommand(CMD_TPS_RECONSTRUCT_TRACERGROUP,sPara);

	//read tracer count
	int iTracerCount=0;
	int polyCount;
	double **dPoints;
	int *num;
	POLY_INFO *polyinfo;

	iReadCount=fread(&iTracerCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}
	
	//read tracer info
	if(iTracerCount>0)
	{
		polyinfo=new POLY_INFO[iTracerCount];
		iReadCount=fread(polyinfo,sizeof(POLY_INFO),iTracerCount,file);
		if(iReadCount!=iTracerCount)
		{
			delete []polyinfo;
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
	}
	
	//read tracer path
		
	iReadCount=fread(&polyCount,sizeof(int),1,file);
	if(iReadCount!=1||polyCount!=iTracerCount)
	{
		if(iTracerCount>0)
			delete []polyinfo;
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}
		
	if(polyCount>0)
	{
		num=new int[polyCount];
		
		dPoints=new double*[polyCount];
		for(int ii=0;ii<polyCount;ii++)
			dPoints[ii]=NULL;
	}

	for(int i=0;i<polyCount;i++)
	{
		

		iReadCount=fread(&num[i],sizeof(int),1,file);
		if(iReadCount!=1)
		{
			delete []num;
			delete []polyinfo;
			for(int j=0;j<polyCount;j++)
			{
				if(dPoints[j]!=NULL)
					delete []dPoints[j];
			}
			delete []dPoints;
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
		dPoints[i]=new double[num[i]*3];
		iReadCount=fread(dPoints[i],sizeof(double),num[i]*3,file);
		if(iReadCount!=num[i]*3)
		{
			delete []num;
			delete []polyinfo;
			for(int j=0;j<polyCount;j++)
			{
				if(dPoints[j]!=NULL)
					delete []dPoints[j];
			}
			delete []dPoints;
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
	}
	
	
	if(iTracerCount>0&&polyCount==iTracerCount)
	{
		for(int i=0;i<iTracerCount;i++)
		{
			vtkPolyData* pTmpPoly = vtkPolyData::New();
			vtkPoints * pTmpPoint = vtkPoints::New();
			pTmpPoly->SetPoints(pTmpPoint);
			double data[3];
			for(int j=0;j<num[i];j++)
			{
				data[0]=dPoints[i][j*3];
				data[1]=dPoints[i][j*3+1];
				data[2]=dPoints[i][j*3+2];
				pTmpPoly->GetPoints()->InsertNextPoint(data);
			}
			CreateTracer(polyinfo[i],pTmpPoly,m_2DView1,FALSE);
			
			//
			pTmpPoly->Delete();
			pTmpPoint->Delete();
		}
		delete []num;
		delete []polyinfo;
		for(int j=0;j<polyCount;j++)
		{
			if(dPoints[j]!=NULL)
				delete []dPoints[j];
		}
		delete []dPoints;
	}

	m_2DView1->ChangeSlice(0);

	ShowWaiting(1,"Loading Probe Defination...");
	//read probe count
	int probeCount;
	NEEDLE_INFO needletype;
	char probeName[100];
	double pos[3],ori[3];
	
	
	iReadCount=fread(&probeCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}

	//read probe
	for( i=0;i<probeCount;i++)
	{
		
		iReadCount=fread(pos,sizeof(double),3,file);
		if(iReadCount!=3)
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}

		iReadCount=fread(ori,sizeof(double),3,file);
		if(iReadCount!=3)
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
		

		iReadCount=fread(&needletype,sizeof(NEEDLE_INFO),1,file);
		if(iReadCount!=1)
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}

		iReadCount=fread(probeName,sizeof(char),100,file);
		if(iReadCount!=100)
		{
			fclose(file);
			PanaMessageBox("Failed to open data file.");
			ShowWaiting(-1,"Loading Series");
			return FALSE;
		}
		sTmp.Format("%s",probeName);

		CreateProbe(pos,ori,needletype,sTmp);

	}

	CTPS3DView* p3DView = m_3DView;

	ShowWaiting(1,"Loading 3D Modal...");
	//read modal group
	int iModalGroupCount;
	iReadCount=fread(&iModalGroupCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}

	CModal modal;
	MODALTYPE modalType;
	for(i=0;i<iModalGroupCount;i++)
	{
		modal.readHeader(file,modalType);
		if(modalType==ModalDrag)
		{
			CModalDrag* pModalDrag=new CModalDrag();
			if(!pModalDrag->readContent(file,p3DView))
			{
				fclose(file);
				delete pModalDrag;
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
		}else if(modalType==ModalContour)
		{
			CModalContour* pModalContour=new CModalContour();
			if(!pModalContour->readContent(file))
			{
				fclose(file);
				delete pModalContour;
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
		}else if(modalType==ModalSeg)
		{
			CModalSeg* pModalSeg=new CModalSeg();
			if(!pModalSeg->readContent(file,fileName))
			{
				fclose(file);
				delete pModalSeg;
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
		}else if(modalType==ModalVolume)
		{
			CModalVolume* pModalVolume=new CModalVolume();
			pModalVolume->m_pRenderWindow=m_3DView->m_pMFCWindow->GetRenderWindow();
			if(!pModalVolume->readContent(file))
			{
				fclose(file);
				delete pModalVolume;
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
		}
	}

	ShowWaiting(1,"Loading Measurement Data...");
	//read measurement result
	int iMeasureCount;
	char cMeasureType[20];
	iReadCount=fread(&iMeasureCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}


	if(iMeasureCount>0)
	{
		double pos[9];
		CString sTmp;
		for	(int i= 0;i<iMeasureCount;i++)
		{
			iReadCount=fread(cMeasureType,sizeof(char),20,file);
			if(iReadCount!=20)
			{
				fclose(file);
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
			sTmp.Format("%s",cMeasureType);
			if(sTmp=="Distance")
			{
				iReadCount=fread(pos,sizeof(double),6,file);
				if(iReadCount!=6)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView1->OnCreateMeasureTool(sTmp,pos);
			}

			if(sTmp=="Angle")
			{
				iReadCount=fread(pos,sizeof(double),9,file);
				if(iReadCount!=9)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView1->OnCreateMeasureTool(sTmp,pos);
			}
			

		}

	}

	iReadCount=fread(&iMeasureCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}
	
	

	

	if(iMeasureCount>0)
	{
		double pos[9];
		CString sTmp;
		for	(int i= 0;i<iMeasureCount;i++)
		{
			iReadCount=fread(cMeasureType,sizeof(char),20,file);
			if(iReadCount!=20)
			{
				fclose(file);
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
			sTmp.Format("%s",cMeasureType);
			if(sTmp=="Distance")
			{
				iReadCount=fread(pos,sizeof(double),6,file);
				if(iReadCount!=6)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView2->OnCreateMeasureTool(sTmp,pos);
			}

			if(sTmp=="Angle")
			{
				iReadCount=fread(pos,sizeof(double),9,file);
				if(iReadCount!=9)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView2->OnCreateMeasureTool(sTmp,pos);
			}
			

		}

	}

	iReadCount=fread(&iMeasureCount,sizeof(int),1,file);
	if(iReadCount!=1)
	{
		fclose(file);
		PanaMessageBox("Failed to open data file.");
		ShowWaiting(-1,"Loading Series");
		return FALSE;
	}


	if(iMeasureCount>0)
	{
		double pos[9];
		CString sTmp;
		for	(int i= 0;i<iMeasureCount;i++)
		{
			iReadCount=fread(cMeasureType,sizeof(char),20,file);
			if(iReadCount!=20)
			{
				fclose(file);
				PanaMessageBox("Failed to open data file.");
				ShowWaiting(-1,"Loading Series");
				return FALSE;
			}
			sTmp.Format("%s",cMeasureType);
			if(sTmp=="Distance")
			{
				iReadCount=fread(pos,sizeof(double),6,file);
				if(iReadCount!=6)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView3->OnCreateMeasureTool(sTmp,pos);
			}

			if(sTmp=="Angle")
			{
				iReadCount=fread(pos,sizeof(double),9,file);
				if(iReadCount!=9)
				{
					fclose(file);
					PanaMessageBox("Failed to open data file.");
					ShowWaiting(-1,"Loading Series");
					return FALSE;
				}
				m_2DView3->OnCreateMeasureTool(sTmp,pos);
			}
			

		}

	}

	fclose(file);

	ShowWaiting(-1,"Loading Series");

	m_3DView->GetRenderer()->ResetCamera();
	return TRUE;

	
}

void CGlobalManager::ChangeTracerColor(CString sPara)
{
	
	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	BOOL bFind=FALSE;
	pos = m_tracerGrpList.GetHeadPosition();
	for (int i=0;i<m_tracerGrpList.GetCount();i++)
	{
		zoneGroup = m_tracerGrpList.GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp == sPara )
		{
			bFind=TRUE;
			break;
		}
	}
	if(!bFind)
		return;

	ACTOR_INFO actor_info;
	POLY_INFO polyinfo;
	pos = m_PolyInfoList.GetHeadPosition();
	for ( i=0;i<m_PolyInfoList.GetCount();i++)
	{
		polyinfo = m_PolyInfoList.GetNext(pos);
		if(polyinfo.iIndex ==zoneGroup.groupIndex)
		{
			actor_info=m_ActorInfoList.GetAt(m_ActorInfoList.FindIndex(i));
			actor_info.p->GetProperty()->SetColor((float )GetRValue(zoneGroup.color)/255.0,
				(float )GetGValue(zoneGroup.color)/255.0,(float )GetBValue(zoneGroup.color)/255.0);
		}
		
	}

}

void CGlobalManager::SelectTracer(CString para,BOOL bTrack)
{
	ReleaseAssemblys(TOOL);
	ReleaseAssemblys(TRACER);
	ReleaseAssemblys(MODAL);

	
	POSITION pos1,pos2;
	POLY_INFO polyinfo;
	ACTOR_INFO actorinfo;
	CString sTmp;

	pos1=m_PolyInfoList.GetHeadPosition();
	pos2=m_ActorInfoList.GetHeadPosition();
	
	for(int i=0;i<m_PolyInfoList.GetCount();i++)
	{
		polyinfo=m_PolyInfoList.GetNext(pos1);
		actorinfo=m_ActorInfoList.GetNext(pos2);
		sTmp.Format("%s",polyinfo.sDscr);
		if(sTmp==para)
		{
			if(bTrack)
			{
				if(polyinfo.imageIndex==1)
					m_2DView1->ChangeSlice(polyinfo.iSliceIndex);
				if(polyinfo.imageIndex==2)
					m_2DView2->ChangeSlice(polyinfo.iSliceIndex);
				if(polyinfo.imageIndex==3)
					m_2DView3->ChangeSlice(polyinfo.iSliceIndex);
			}

			AddAssemblys(actorinfo.p,TRACER);
		
			m_2DView1->Invalidate(FALSE);
			m_2DView2->Invalidate(FALSE);
			m_2DView3->Invalidate(FALSE);
		}
	}	
}

BOOL CGlobalManager::CreateTracer(POLY_INFO &polyinfo, vtkPolyData *pData,CTPS2DView *p2DView,BOOL bTransform)
{
	CString sDscr,sTmp;
	sDscr.Format("Tracer%i",m_iTraceCount);
	POLY_INFO tmppolyinfo;
	POSITION pos;
	
	//get unique name 
	pos=m_PolyInfoList.GetHeadPosition();
	for (int i=0;i<m_PolyInfoList.GetCount();i++)
	{
		tmppolyinfo = m_PolyInfoList.GetNext(pos);
		sTmp.Format("%s",tmppolyinfo.sDscr);
		if(sTmp ==sDscr)
		{
			m_iTraceCount++;
			sDscr.Format("Tracer%i",m_iTraceCount);
			pos=m_PolyInfoList.GetHeadPosition();
			i=-1;
		}
		
	}
	m_iTraceCount++;

	ZONEGROUP_INFO zoneGroup;
	
	BOOL bFind=FALSE;
	pos = m_tracerGrpList.GetHeadPosition();
	for (i=0;i<m_tracerGrpList.GetCount();i++)
	{
		zoneGroup = m_tracerGrpList.GetNext(pos);
		
		if(zoneGroup.groupIndex ==polyinfo.iIndex  )
		{
			bFind=TRUE;
			break;
		}
	}

	if(!bFind)
		return FALSE;
	
	sprintf(polyinfo.sDscr,"%s",sDscr);
	vtkPoints * pPoints;

	if(pData->GetPoints()==NULL)
		return FALSE;
		
	double dCenter[3];
	GetPolyDataCenter(pData,dCenter);
	vtkProperty * base_text_prop = vtkProperty::New();
	base_text_prop->SetColor((float )GetRValue(zoneGroup.textColor)/255.0, 
		(float )GetGValue(zoneGroup.textColor)/255.0, (float )GetBValue(zoneGroup.textColor)/255.0);
	vtkVectorText * atext = vtkVectorText::New();
	atext->SetText(sDscr);
	vtkPolyDataMapper* textMapper = vtkPolyDataMapper::New();
	textMapper->ImmediateModeRenderingOn();
	textMapper->SetInputConnection(atext->GetOutputPort());
	vtkFollower * textActor = vtkFollower::New();
	textActor->SetMapper(textMapper);
	textActor->SetScale(zoneGroup.textScale, zoneGroup.textScale, zoneGroup.textScale);
	textActor->SetProperty(base_text_prop);
	textActor->SetVisibility(0);
	base_text_prop->Delete();
			
	double dZpos;
	double dTolen=0.1;
	if(p2DView->m_imageInfo.bHFlip)
	{
		dZpos= (polyinfo.iSliceIndex)*(p2DView->m_imageInfo.spacing[2])+dTolen+p2DView->m_dZRange;
	}else
	{
		dZpos= (polyinfo.iSliceIndex)*(p2DView->m_imageInfo.spacing[2])-dTolen+p2DView->m_dZRange;
	}
	double position[3];
	position[0]=dCenter[0]+p2DView->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
	position[1]=dCenter[1]+p2DView->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
	position[2]=dCenter[2]+p2DView->m_pImageFrame_scanner->GetElement(2,2)*dZpos;
	
	if(bTransform)
		textActor->SetPosition(position[0],position[1],position[2]); //dZpos);
	else
		textActor->SetPosition(dCenter[0],dCenter[1],dCenter[2]); //dZpos);

	vtkTransform* pTrans = vtkTransform::New();
	if(bTransform)
	{
		pTrans->SetMatrix(p2DView->m_pImageFrame_scanner);
	}else
	{
		if(polyinfo.imageIndex==1)
			pTrans->SetMatrix(m_2DView1->m_pImageFrame_scanner);
		if(polyinfo.imageIndex==2)
		{
			if(m_2DView2!=NULL)
				pTrans->SetMatrix(m_2DView2->m_pImageFrame_scanner);
			else
				pTrans->SetMatrix(p2DView->m_pImageFrame_scanner);
		}
		if(polyinfo.imageIndex==3)
		{
			if(m_2DView3!=NULL)
				pTrans->SetMatrix(m_2DView3->m_pImageFrame_scanner);
			else
				pTrans->SetMatrix(p2DView->m_pImageFrame_scanner);
		}
		
	}
	pTrans->RotateX(180);
	//pTrans->RotateY(90);
	textActor->SetOrientation(pTrans->GetOrientation());
	pTrans->Delete();

	//textActor->SetOrientation(0,90,0);
	atext->Delete();

	textMapper->Delete();
	if(p2DView->m_instanceIndex==1)
	{
		if(polyinfo.imageIndex==1)
			m_2DView1->GetRenderer()->AddActor(textActor);
		if(polyinfo.imageIndex==2)
			m_2DView2->GetRenderer()->AddActor(textActor);
		if(polyinfo.imageIndex==3)
			m_2DView3->GetRenderer()->AddActor(textActor);
	}else
	{
		p2DView->GetRenderer()->AddActor(textActor);
	}
		
	pPoints = pData->GetPoints();
	int num = pPoints->GetNumberOfPoints();
	
	for( i=0;i<num;i++)
	{
		double* dPoint = pPoints->GetPoint(i);
		if(p2DView->m_imageInfo.bHFlip)
		{
			dZpos= (polyinfo.iSliceIndex)*(p2DView->m_imageInfo.spacing[2])+dTolen+p2DView->m_dZRange;
		}else
		{
			dZpos= (polyinfo.iSliceIndex)*(p2DView->m_imageInfo.spacing[2])-dTolen+p2DView->m_dZRange;
		}
		

		position[0]=dPoint[0]+p2DView->m_pImageFrame_scanner->GetElement(0,2)*dZpos;
		position[1]=dPoint[1]+p2DView->m_pImageFrame_scanner->GetElement(1,2)*dZpos;
		position[2]=dPoint[2]+p2DView->m_pImageFrame_scanner->GetElement(2,2)*dZpos;
	
		if(bTransform)
			pPoints->SetPoint(i,position);
		else
			pPoints->SetPoint(i,dPoint);
		
	}
		
		
	m_pZonePoly->AddItem(pData);
	m_pLabels->AddItem(textActor);
	m_PolyInfoList.AddTail(polyinfo);

	vtkPolygon *pPolygon = vtkPolygon::New();
	pPolygon->GetPointIds()->SetNumberOfIds(pPoints->GetNumberOfPoints());
	for(i=0;i<pPoints->GetNumberOfPoints();i++)
	{
		pPolygon->GetPointIds()->SetId(i,i);
	}
	pData->Allocate(1,1);
	pData->InsertNextCell(pPolygon->GetCellType(),pPolygon->GetPointIds());
	
	
	vtkTriangleFilter *pTri=vtkTriangleFilter::New();
	pTri->SetInput(pData);

	vtkPolyDataMapper *pPolyMapper = vtkPolyDataMapper::New();
	pPolyMapper->SetInput(pTri->GetOutput());
	

	vtkActor* pActor = vtkActor::New();
	pActor->SetMapper(pPolyMapper);
	pActor->GetProperty()->SetColor((float )GetRValue(zoneGroup.color)/255.0,
		(float )GetGValue(zoneGroup.color)/255.0,(float )GetBValue(zoneGroup.color)/255.0);
	pActor->GetProperty()->SetLineWidth(2);
	pActor->GetProperty()->SetEdgeColor(1,1,1);
	pActor->GetProperty()->SetOpacity(0.5);
	//pActor->GetProperty()->SetRepresentationToWireframe();
	pActor->SetVisibility(1);
	if(p2DView->m_instanceIndex==1)
	{
		if(polyinfo.imageIndex==1)
			m_2DView1->GetRenderer()->AddActor(pActor);
		if(polyinfo.imageIndex==2)
			m_2DView2->GetRenderer()->AddActor(pActor);
		if(polyinfo.imageIndex==3)
			m_2DView3->GetRenderer()->AddActor(pActor);
	}else
	{
		p2DView->GetRenderer()->AddActor(pActor);
	}
		
	
	ACTOR_INFO actorinfo;
	actorinfo.iSliceIndex = polyinfo.iSliceIndex;
	actorinfo.p = pActor;
	actorinfo.pFollower = textActor;
	actorinfo.imageIndex =polyinfo.imageIndex;

	m_ActorInfoList.AddTail(actorinfo);

	pActor->Delete();
	pPolyMapper->Delete();
	pPolygon->Delete();
	pTri->Delete();
	textActor->Delete();
	SendCommand(CMD_TPS_CREATETRACEOVER,sDscr);
		
	return TRUE;
}

void CGlobalManager::DeleteTrace(CString para)
{
	
	CTPS3DView* p3Dview = m_3DView;
	ACTOR_INFO actor_info;
	POLY_INFO polyinfo;
	POSITION pos,pos1;
	CString sDscr;
	
	pos=m_PolyInfoList.GetHeadPosition();
	pos1=m_ActorInfoList.GetHeadPosition();
	for (int i=0;i<m_PolyInfoList.GetCount();i++)
	{
		polyinfo = m_PolyInfoList.GetNext(pos);
		actor_info=m_ActorInfoList.GetNext(pos1);
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==para){
			m_2DView1->GetRenderer()->RemoveActor(actor_info.p);
			m_2DView1->GetRenderer()->RemoveActor(actor_info.pFollower);
			m_2DView2->GetRenderer()->RemoveActor(actor_info.p);
			m_2DView2->GetRenderer()->RemoveActor(actor_info.pFollower);
			m_2DView3->GetRenderer()->RemoveActor(actor_info.p);
			m_2DView3->GetRenderer()->RemoveActor(actor_info.pFollower);
			m_ActorInfoList.RemoveAt(m_ActorInfoList.FindIndex(i));
			m_PolyInfoList.RemoveAt(m_PolyInfoList.FindIndex(i));
			m_pZonePoly->RemoveItem(i);
			m_pLabels->RemoveItem(i);
			break;
		}
		
	}
	m_2DView1->Invalidate(FALSE);
	m_2DView2->Invalidate(FALSE);
	m_2DView3->Invalidate(FALSE);
	//GetRenderer()->Render();
}

void CGlobalManager::SelectModal(CString para)
{
	ReleaseAssemblys(TOOL);
	ReleaseAssemblys(TRACER);
	ReleaseAssemblys(MODAL);

	POSITION pos;
	CModal* modalinfo;
	CString sTmp;
	vtkActor *pActor;

	pos=m_modalGrpList.GetHeadPosition();
	for(int i=0;i<m_modalGrpList.GetCount();i++)
	{
		modalinfo=m_modalGrpList.GetNext(pos);
		sTmp.Format("%s",modalinfo->m_name);
		if(sTmp==para && modalinfo->m_actor)
		{
			pActor=(vtkActor *)modalinfo->m_actor;
			AddAssemblys(pActor,MODAL);
			m_3DView->Invalidate(FALSE);
			
		}
	}	
}

void CGlobalManager::BallVisibleStatus(CString para)
{
	int i;
	CString sStatus="UNVISIBLE";
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==para)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;
				if(pBall->m_ballVisible)
					sStatus="VISIBLE";
				break;
				
			}
			
			

		}
	}
	SendCommand(CMD_TPS_BALLVISIBLESTATUS,sStatus);
}

void CGlobalManager::SetBallVisible(CString para)
{
	
	int i;
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly==NULL)
			continue;
		if(pAssembly->m_name==para)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;
				if(pBall->m_ballVisible)
					pBall->m_ballVisible=FALSE;
				else
					pBall->m_ballVisible=TRUE;

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					ball_shell.Visibility=pBall->m_ballVisible;
					pBall->SetShellProperty(ball_shell);
				}
				
				CString sTmp="";
				SendCommand(CMD_TPS_RENDERREFRESHALL,sTmp);
			}

			break;
			

		}
	}	
}

void CGlobalManager::InterLineVisibleStatus(CString para)
{
	int i;
	CString sStatus="UNVISIBLE";
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==para)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;
				if(pBall->m_bDisplayInterLine)
					sStatus="VISIBLE";
				break;
				
			}
			
			

		}
	}
	SendCommand(CMD_TPS_LINEVISIBLESTATUS,sStatus);
}

void CGlobalManager::SetLineVisible(CString sPara)
{
	int i;
	m_pAssemblys->InitTraversal();
	int nCount = m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)m_pAssemblys->GetNextProp3D();
		if(pAssembly==NULL)
			continue;
		if(pAssembly->m_name==sPara)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;
				if(pBall->m_bDisplayInterLine)
					pBall->m_bDisplayInterLine=FALSE;
				else
					pBall->m_bDisplayInterLine=TRUE;

				pBall->ShowIntersections(pBall->m_bDisplayInterLine);
				PlaneIntersection(pAssembly);
				m_2DView1->Invalidate(FALSE);
				m_2DView2->Invalidate(FALSE);
				m_2DView3->Invalidate(FALSE);
								
			}

			break;
			
		}
	}	
}

void CGlobalManager::SetDefaultTracerGroup(CString sPara)
{
	CString sTmp;
	POSITION pos;
	ZONEGROUP_INFO zoneGroup;
	BOOL bFind=FALSE;
	if(m_tracerGrpList.GetCount()<=0)
	{
		PanaMessageBox("Internal Error!");
		return;
	}
		
	pos = m_tracerGrpList.GetHeadPosition();
	for (int i=0;i<m_tracerGrpList.GetCount();i++)
	{
		zoneGroup = m_tracerGrpList.GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp==sPara)
		{
			m_defaultTracerGroup=zoneGroup;
			bFind=TRUE;
			break;
		}
	}

	if(!bFind)
	{
		PanaMessageBox("Internal Error!");
		return;
	}
}

int CGlobalManager::GetPolyDataCenter(vtkPolyData *pPolyData, double dCenter[])
{
	double x,y,z;
	x = 0;
	y = 0;
	z = 0;
	double pos[3];
	int num = pPolyData->GetPoints()->GetNumberOfPoints();
	for (int i=0;i<num;i++)
	{
		pPolyData->GetPoints()->GetPoint(i,pos);
		x = x+pos[0];
		y = y+pos[1];
		z = z+pos[2];
	}
	dCenter[0] = x/num;
	dCenter[1] = y/num;
	dCenter[2] = z/num;
	return 0;
}

CProbeCryo* CGlobalManager::GetRealProbe()
{
	if(m_pRealProbe)
		return m_pRealProbe;

	NEEDLE_INFO needle;
	POSITION POS = m_bioProbeInfo.GetHeadPosition();
	for (int i=0;i<m_bioProbeInfo.GetCount();i++)
	{
		needle = m_bioProbeInfo.GetNext(POS);
		break;
	}

	m_pRealProbe= CProbeCryo::New();
	m_pRealProbe->m_pMsgWindow=NULL;
	m_pRealProbe->Initial(needle);
	m_pRealProbe->ToolAssembly();
	m_pRealProbe->m_name="RealProbe";
	m_pRealProbe->SetColor(1,1,0);
	
	double pos[3]={0,0,0};
	m_pRealProbe->SetInitPosition(pos);
	m_pRealProbe->RotateZ(0);
	m_pRealProbe->RotateX(0);
	m_pRealProbe->RotateY(0);
	m_pRealProbe->UpdateProbeCoord();

	return m_pRealProbe;
}

void CGlobalManager::DeleteVolumeModalByImage(vtkImageData *pImage)
{
	POSITION pos;
	CModal* modalinfo;
	CString sTmp;
	CList<CString,CString> m_sList; 

	pos=m_modalGrpList.GetHeadPosition();
	for(int i=0;i<m_modalGrpList.GetCount();i++)
	{
		modalinfo=m_modalGrpList.GetNext(pos);
		sTmp.Format("%s",modalinfo->m_name);
		if(modalinfo->m_modalType == ModalVolume && ((CModalVolume*)modalinfo)->m_pImage==pImage)
		{
			
			if(m_pTreeCtrl)
			{
				m_pTreeCtrl->DeleteItem(modalinfo->m_treeItem);
			}
			
			m_sList.AddTail(sTmp);
		}
	}	
	pos=m_sList.GetHeadPosition();
	while(pos)
	{
		sTmp=m_sList.GetNext(pos);
		SendCommand(CMD_TPS_DELETEMODAL,sTmp);
	}
}

void CGlobalManager::CopyFolder(CString src, CString sDest, CString sExt)
{
	CreateDirectory(sDest,NULL);

	CString sTmp;
	sTmp.Format("%s\\%s",src,sExt);
	CFileFind finder;
	
	BOOL bWorking = finder.FindFile(sTmp);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CString sFilePath = finder.GetFilePath();
		CString sFileName = finder.GetFileName();
		CString sDestFile;
		sDestFile.Format("%s\\%s",sDest,sFileName);
		CopyFile(sFilePath,sDestFile,FALSE);
	}
	finder.Close();
}
