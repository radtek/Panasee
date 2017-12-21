// EvaluateDlg.cpp : implementation file
//

#include "stdafx.h"

#include "EvaluateDlg.h"
#include "DisplayDlg.h"
#include "GalilIceBallDistribute.h"
#include "vtkProperty.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvaluateDlg dialog


CEvaluateDlg::CEvaluateDlg(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CEvaluateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvaluateDlg)
	m_sResult = _T("");
	//}}AFX_DATA_INIT
	m_pActor = vtkActor::New();
}

CEvaluateDlg::~CEvaluateDlg()
{
	//CDisplayDlg::m_3DView->GetRenderer()->RemoveActor(m_pActor);
	m_pActor->Delete();
}

void CEvaluateDlg::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvaluateDlg)
	DDX_Control(pDX, IDC_EDIT_VOLUME_RESULT, m_textCtrl);
	DDX_Control(pDX, IDC_LIST_PROBE, m_lstProbe);
	DDX_Control(pDX, IDC_LIST_MODAL, m_lstModal);
	DDX_Text(pDX, IDC_EDIT_VOLUME_RESULT, m_sResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvaluateDlg, CPanaseeDialog)
	//{{AFX_MSG_MAP(CEvaluateDlg)
	ON_BN_CLICKED(IDC_BNT_EVA_RETURN, OnBntEvaReturn)
	ON_BN_CLICKED(IDC_BNT_EVA_REFRESHLIST, OnBntEvaRefreshlist)
	ON_BN_CLICKED(IDC_BUTTON_MODAL_VOLUME, OnButtonModalVolume)
	ON_BN_CLICKED(IDC_BUTTON_PROBE_VOLUME, OnButtonProbeVolume)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvaluateDlg message handlers

BOOL CEvaluateDlg::OnInitDialog() 
{
	EnableSeprateLine(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	this->CenterWindow(CWnd::GetDesktopWindow());
	OnBntEvaRefreshlist();
	GetDlgItem(IDC_EDIT_DISPLAY_INDEX)->SetWindowText("3");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEvaluateDlg::OnBntEvaReturn() 
{
	// TODO: Add your control notification handler code here
	this->OnOK();
}

void CEvaluateDlg::OnBntEvaRefreshlist() 
{
	// TODO: Add your control notification handler code here
	vtkProp3DCollection *pAssemblys;
	CString sName;
	
	m_lstProbe.DeleteAllItems();
	m_lstModal.DeleteAllItems();

	pAssemblys=CGlobalManager::Manager()->m_pAssemblys;
	pAssemblys->InitTraversal();
	for(int i=0;i<pAssemblys->GetNumberOfItems();i++)
	{
		vtkAssembly * pAssembly;
		pAssembly = (vtkAssembly*)pAssemblys->GetNextProp3D();
		sName=((CProbeCryo*)pAssembly)->m_name;
		//m_lstProbe.AddItem(sName);
		m_lstProbe.InsertItem(
			LVIF_TEXT|LVIF_STATE, i, sName, 
			0, LVIS_SELECTED,
			0, 0);
	}

	CList<CModal*,CModal*> *modalGrpList;
	CModal* modalinfo;
	modalGrpList = &CGlobalManager::Manager()->m_modalGrpList;
	POSITION pos;
	pos=modalGrpList->GetHeadPosition();
	for(i=0;i<modalGrpList->GetCount();i++)
	{
		modalinfo=modalGrpList->GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		m_lstModal.InsertItem(
			LVIF_TEXT|LVIF_STATE, i, sName, 
			0, LVIS_SELECTED,
			0, 0);
	}
		
}

void CEvaluateDlg::OnButtonModalVolume() 
{
	// TODO: Add your control notification handler code here
	int count=m_lstModal.GetItemCount();
	CList<CModal*,CModal*> *modalGrpList;
	modalGrpList = &CGlobalManager::Manager()->m_modalGrpList;
	CString sName,sTmp;
	POSITION modalPos;
	CModal* modalinfo;
	
	POSITION pos = m_lstModal.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_lstModal.GetNextSelectedItem(pos);
		sName=m_lstModal.GetItemText(nItem,0);

		modalPos = modalGrpList->GetHeadPosition();
		for (int i=0;i<modalGrpList->GetCount();i++)
		{
			modalinfo = modalGrpList->GetNext(modalPos);
			sTmp.Format("%s",modalinfo->m_name);
			if(sTmp == sName )
			{
				
				sTmp.Format("Volume of %s is: %.2f\r\n",sName,modalinfo->m_volume);
				ShowText(sTmp);
				
				break;
			}
		}
		
	}
	

}
double CEvaluateDlg::GetShellVolume(int iIndex)
{
	vtkImplicitBoolean* pIB = vtkImplicitBoolean::New();
	pIB->SetOperationTypeToUnion();

	double pos[3];
	int nBallCount;
	CString sName;
	CString sTmp;
	nBallCount=0;
	pos[0]=pos[1]=pos[2]=0;

	CProbeCryo *pAssembly;
	POSITION posProbe = m_lstProbe.GetFirstSelectedItemPosition();
	while (posProbe)
	{
		int nItem = m_lstProbe.GetNextSelectedItem(posProbe);
		sName=m_lstProbe.GetItemText(nItem,0);

		pAssembly=CGlobalManager::Manager()->GetProbeByName(sName);
		if(!pAssembly)
			continue;

		if (pAssembly->m_pAttachIceBall)
		{
			
			vtkQuadric* pQuad = vtkQuadric::New();
			double a[10];
			((vtkQuadric*)pAssembly->m_pAttachIceBall->m_pQuadrics->GetItemAsObject(iIndex))->GetCoefficients(a);
			pQuad->SetCoefficients(a);
			
			vtkTransform* pTran = vtkTransform::New();
			pTran->SetMatrix(pAssembly->GetMatrix());
			pTran->PreMultiply();
			pTran->Concatenate(((vtkProp3D *)pAssembly->m_pAttachIceBall->m_pShell->GetItemAsObject(iIndex))->GetMatrix());
			
			double data[3];
			pTran->GetPosition(data);
			pos[0]+=data[0];
			pos[1]+=data[1];
			pos[2]+=data[2];
						
			pQuad->SetTransform(pTran->GetInverse());
			pIB->AddFunction(pQuad);
			pTran->Delete();
			pQuad->Delete();
			nBallCount++;
		}
		
	}
	
	if(nBallCount<=0)
	{
		pIB->Delete();
		return 0;
	}

	pos[0]/=nBallCount;
	pos[1]/=nBallCount;
	pos[2]/=nBallCount;

	vtkSampleFunction* theSample = vtkSampleFunction::New();
	theSample->SetImplicitFunction(pIB);
	double dOffset=60;
	theSample->SetModelBounds (pos[0]-dOffset,pos[0]+dOffset,pos[1]-dOffset,
			pos[1]+dOffset,pos[2]-dOffset,pos[2]+dOffset); 
	theSample->SetSampleDimensions(60,60,60);
	theSample->ComputeNormalsOff();
		
	vtkContourFilter * theSurface = vtkContourFilter::New();
	theSurface->SetInputConnection(theSample->GetOutputPort());
	theSurface->SetValue(0,1);
	vtkMassProperties * pMassPro = vtkMassProperties::New();
	pMassPro->SetInputConnection(theSurface->GetOutputPort());
	double vol = pMassPro->GetVolume()/1000;

	

	CString index;
	GetDlgItem(IDC_EDIT_DISPLAY_INDEX)->GetWindowText(index);
	if(iIndex==atoi(index))
	{
		vtkPolyDataMapper* pMapper = vtkPolyDataMapper::New();
		pMapper->SetInputConnection(theSurface->GetOutputPort());
		pMapper->ImmediateModeRenderingOn();

	
		m_pActor->SetMapper(pMapper);
		m_pActor->SetVisibility(1);
		vtkProperty *pProperty = vtkProperty::New();
		pProperty->SetColor(1,0,0);
		pProperty->SetOpacity(0.5);
		m_pActor->SetProperty(pProperty);
		m_pActor->SetPickable(1);
		CGlobalManager::Manager()->m_3DView->GetRenderer()->AddActor(m_pActor);
		pProperty->Delete();
		pMapper->Delete();
	}
		
	pMassPro->Delete();
	pIB->Delete();
	theSample->Delete();
	theSurface->Delete();

	return vol;
		
}

void CEvaluateDlg::OnButtonProbeVolume() 
{
	// TODO: Add your control notification handler code here
	double vol;
	
	vol=GetShellVolume(0);
	CString sTmp;
	sTmp.Format("Volume of 0 degree probe is:   %.2f\r\n",vol);
	ShowText(sTmp);

	vol=GetShellVolume(1);
	sTmp.Format("Volume of -20 degree probe is: %.2f\r\n",vol);
	ShowText(sTmp);

	vol=GetShellVolume(2);
	sTmp.Format("Volume of -40 degree probe is: %.2f\r\n",vol);
	ShowText(sTmp);
	

	
}

void CEvaluateDlg::OnDestroy() 
{

	
	CPanaseeDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}

void CEvaluateDlg::ShowText(CString sText)
{
	int i=m_textCtrl.GetLineCount();
	m_sResult+=sText;
	UpdateData(FALSE);
	int j=m_textCtrl.GetLineCount();
	m_textCtrl.LineScroll(j);
	
}
