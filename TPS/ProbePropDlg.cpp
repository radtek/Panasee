// ProbePropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProbePropDlg.h"
#include "TpsCryoProbe.h"
#include "GlobalManager.h"
#include "GalilIceBallDistribute.h"
#include "Commands.h"
#include "CustomItems.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ICEBALL "Ice Ball"
#define BALLSHELLVISIBLE "Visible"
#define BALLSHELLOPACITY "Opacity"
#define BALLSHELLCOLOR "Color"
/////////////////////////////////////////////////////////////////////////////
// CPropDlgProbe dialog


CPropDlgProbe::CPropDlgProbe(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPropDlgProbe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropDlgProbe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropDlgProbe::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlgProbe)
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_PROBEPROP_PLACEHODER, m_placeHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropDlgProbe, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPropDlgProbe)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlgProbe message handlers

BOOL CPropDlgProbe::OnInitDialog() 
{
	EnableSeprateLine(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnClose);
	//SetBottomBtnList(&btnList);
	
	// TODO: Add extra initialization here
	m_bInitial=FALSE;

	SetWindowText(m_probeName);
	
	CRect rc;
	m_placeHolder.GetWindowRect( &rc );
	ScreenToClient( &rc );
	
	m_pItemIceBall=NULL;
	m_pItemBallShell1=NULL;
	m_pItemBallShell2=NULL;
	m_pItemBallShell3=NULL;

	// create the property grid.
	if ( m_PropGrid.Create( rc, this, 0 ) )
	{
		// create document settings category.
        pSettings = m_PropGrid.AddCategory(_T("Probe Settings"));
		m_pItemName=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Name"), _T("")));
		m_pItemName->SetDescription(_T("Set the probe name"));
		

		m_pItemStyle= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Style"), _T("Biopsy")));
		m_pItemStyle->SetDescription(_T("Set the probe style"));
		CXTPPropertyGridItemConstraints* pList = m_pItemStyle->GetConstraints();
		pList->AddConstraint(_T("Biopsy"));
		pList->AddConstraint(_T("Cryotherapy"));
		m_pItemStyle->SetFlags(xtpGridItemHasComboButton);

		m_pItemType= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Type"), _T("Biopsy")));
		m_pItemType->SetDescription(_T("Set the probe type"));

		m_pItemLength=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Length"), _T("")));
		m_pItemLength->SetDescription(_T("The length of probe"));
		m_pItemLength->SetReadOnly(TRUE);

		m_pItemDiameter=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Diameter"), _T("")));
		m_pItemDiameter->SetDescription(_T("The diameter of probe"));
		m_pItemDiameter->SetReadOnly(TRUE);

		        
	}

	m_PropGrid.SetPropertySort(xtpGridSortCategorized);
	m_PropGrid.SetCustomColors( RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);
	//m_wndPropertyGrid.ShowWindow(SW_HIDE);
	InitialPara();
	this->CenterWindow(CWnd::GetDesktopWindow());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPropDlgProbe::OnGridNotify(WPARAM wParam, LPARAM lParam)
{

	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		CString sTmp=pItem->GetCaption();
			
		if (pItem->GetCaption() == _T("Probe Style"))
		{
			OnChangeBallStyle(pItem->GetValue());
			return 0;
		}

		if (pItem->GetCaption() == _T("Type"))
		{
			OnChangeStyle(pItem->GetValue());
				
		}

		if (pItem->GetCaption() == _T("Ice Ball"))
		{
			OnChangeBallVisible(pItem->GetValue());
				
		}

		if (pItem->GetCaption() == _T("Name"))
		{
			OnChangeName(pItem->GetValue());
				
		}
		if (sTmp == _T(BALLSHELLVISIBLE))
		{
			OnChangeBallShellVisible(pItem->GetValue());
				
		}

		if (sTmp == _T(BALLSHELLOPACITY))
		{
			OnChangeBallShellOpacity(pItem->GetValue());
				
		}

		if (sTmp == _T(BALLSHELLCOLOR))
		{
			OnChangeBallShellColor(pItem->GetValue());
				
		}
		

	}
	return 0;
}

void CPropDlgProbe::OnChangeStyle(CString para)
{

	int i;
	POSITION pos;
	NEEDLE_INFO needle;
	CString sTmp;
	if(m_pItemStyle->GetValue()=="Biopsy")
	{
		pos=CGlobalManager::Manager()->m_bioProbeInfo.GetHeadPosition();
		for (int i=0;i<CGlobalManager::Manager()->m_bioProbeInfo.GetCount();i++)
		{
			needle = CGlobalManager::Manager()->m_bioProbeInfo.GetNext(pos);
			sTmp.Format("%s",needle.name);
			if(sTmp==para)
				break;
		}
		
	}

	if(m_pItemStyle->GetValue()=="Cryotherapy")
	{
		pos=CGlobalManager::Manager()->m_cryoProbeInfo.GetHeadPosition();
		for (int i=0;i<CGlobalManager::Manager()->m_cryoProbeInfo.GetCount();i++)
		{
			needle = CGlobalManager::Manager()->m_cryoProbeInfo.GetNext(pos);
			sTmp.Format("%s",needle.name);
			if(sTmp==para)
				break;
		}
		
	}

	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pNeedle = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pNeedle->m_name==m_probeName)
		{
			CString str;
			pNeedle->Initial(needle);
			pNeedle->ToolAssembly();

			str.Format("%.2f",pNeedle->m_eNeedleType.needleDiamter);
			m_pItemDiameter->SetValue(str);
			str.Format("%.2f",pNeedle->m_eNeedleType.needleLength);
			m_pItemLength->SetValue(str);
			
			m_pFrmMgr->SendCommand(this,CMD_TPS_RENDERREFRESH,m_probeName);

			if(pNeedle->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pNeedle->m_pAttachIceBall;
				
				if(pBall->m_ballVisible)
					m_pItemIceBall->SetValue("True");
				else
					m_pItemIceBall->SetValue("False");

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					if(j==0){
						if(ball_shell.Visibility)
							m_pBS1Visible->SetValue("True");
						else
							m_pBS1Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS1Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS1Color->SetValue(str);
					}

					if(j==1){
						if(ball_shell.Visibility)
							m_pBS2Visible->SetValue("True");
						else
							m_pBS2Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS2Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS2Color->SetValue(str);
					}

					if(j==2){
						if(ball_shell.Visibility)
							m_pBS3Visible->SetValue("True");
						else
							m_pBS3Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS3Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS3Color->SetValue(str);
					}

				}
				

				
			}//end

			
		}
	}	
}

void CPropDlgProbe::InitialPara()
{
	m_bInitial=TRUE;
	int i;
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==m_probeName)
		{
			CString str;
			m_pItemName->SetValue(m_probeName);
			if(pAssembly->m_eNeedleType.type==0)
			{
				m_pItemStyle->SetValue("Biopsy");
				ChangeBallStyle("Biopsy");
			}
			if(pAssembly->m_eNeedleType.type==1)
			{
				m_pItemStyle->SetValue("Cryotherapy");
				ChangeBallStyle("Cryotherapy");
			}
			str.Format("%s",pAssembly->m_eNeedleType.name);
			m_pItemType->SetValue(str);
			
			
			str.Format("%.2f",pAssembly->m_eNeedleType.needleDiamter);
			m_pItemDiameter->SetValue(str);
			str.Format("%.2f",pAssembly->m_eNeedleType.needleLength);
			m_pItemLength->SetValue(str);
			

			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;
				
				if(pBall->m_ballVisible)
					m_pItemIceBall->SetValue("True");
				else
					m_pItemIceBall->SetValue("False");

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					if(j==0){
						if(ball_shell.Visibility)
							m_pBS1Visible->SetValue("True");
						else
							m_pBS1Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS1Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS1Color->SetValue(str);
					}

					if(j==1){
						if(ball_shell.Visibility)
							m_pBS2Visible->SetValue("True");
						else
							m_pBS2Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS2Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS2Color->SetValue(str);
					}

					if(j==2){
						if(ball_shell.Visibility)
							m_pBS3Visible->SetValue("True");
						else
							m_pBS3Visible->SetValue("False");
						str.Format("%i",ball_shell.Opacity);
						m_pBS3Opacity->SetValue(str);
						str=RGBToString(ball_shell.Color);
						m_pBS3Color->SetValue(str);
					}

				}
				

				
			}//end

			break;
			
			

		}
	}	
	
}

void CPropDlgProbe::OnChangeBallVisible(CString para)
{
	m_pFrmMgr->SendCommand(this,CMD_TPS_ICEBALLVISIBLE,m_probeName);
	m_pBS1Visible->SetValue(m_pItemIceBall->GetValue());
	m_pBS2Visible->SetValue(m_pItemIceBall->GetValue());
	m_pBS3Visible->SetValue(m_pItemIceBall->GetValue());
}

void CPropDlgProbe::OnChangeName(CString para)
{
	

	int i;
	BOOL bFound=FALSE;
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==para)
		{
			bFound=TRUE;

			break;
		}
		
	}

	if(bFound)
	{
		PanaMessageBox("Name must be unique.");
		m_pItemName->SetValue(m_probeName);
		return;
	}

	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==m_probeName)
		{
			pAssembly->m_name=para;
			m_pItemName->SetValue(para);
			m_probeName=para;
			SetWindowText(m_probeName);

			break;
		}
		
	}
}

void CPropDlgProbe::OnChangeBallShellVisible(CString sPara)
{
	int i;
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==m_probeName)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					if(j==0){
						if(m_pBS1Visible->GetValue()=="True")
							ball_shell.Visibility =TRUE;
						else
							ball_shell.Visibility =FALSE;
					}
					if(j==1){
						if(m_pBS2Visible->GetValue()=="True")
							ball_shell.Visibility =TRUE;
						else
							ball_shell.Visibility =FALSE;
					}
					if(j==2){
						if(m_pBS3Visible->GetValue()=="True")
							ball_shell.Visibility =TRUE;
						else
							ball_shell.Visibility =FALSE;
					}
					pBall->SetShellProperty(ball_shell);

				}
				

				
			}//end
			m_pFrmMgr->SendCommand(this,CMD_TPS_RENDERREFRESHALL,m_probeName);
			
			break;
			
			

		}
	}	
}


void CPropDlgProbe::OnChangeBallStyle(CString sPara)
{
	m_PropGrid.ResetContent();

	pSettings = m_PropGrid.AddCategory(_T("Probe Settings"));
	m_pItemName=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Name"), _T("")));
	m_pItemName->SetDescription(_T("Set the probe name"));
	m_pItemName->SetValue(m_probeName);

	m_pItemStyle= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Style"), _T("Biopsy")));
	m_pItemStyle->SetDescription(_T("Set the probe style"));
	CXTPPropertyGridItemConstraints* pList = m_pItemStyle->GetConstraints();
	pList->AddConstraint(_T("Biopsy"));
	pList->AddConstraint(_T("Cryotherapy"));
	m_pItemStyle->SetFlags(xtpGridItemHasComboButton);
	m_pItemStyle->SetValue(sPara);

	m_pItemType= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Type"), _T("Biopsy")));
	m_pItemType->SetDescription(_T("Set the probe type"));

	m_pItemLength=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Length"), _T("")));
	m_pItemLength->SetDescription(_T("The length of probe"));
	m_pItemLength->SetReadOnly(TRUE);

	m_pItemDiameter=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Probe Diameter"), _T("")));
	m_pItemDiameter->SetDescription(_T("The diameter of probe"));
	m_pItemDiameter->SetReadOnly(TRUE);

	ChangeBallStyle(sPara);

	CString sTmp;
	sTmp=m_pItemType->GetValue();
	

	//if(!m_bInitial)
		OnChangeStyle(sTmp);
	
}

void CPropDlgProbe::ChangeBallStyle(CString sPara)
{
	CXTPPropertyGridItemConstraints* pList = m_pItemType->GetConstraints();
	pList->RemoveAll();
	
	POSITION pos;
	NEEDLE_INFO needle;
	CString sTmp;
	if(sPara=="Biopsy")
	{
		pos=CGlobalManager::Manager()->m_bioProbeInfo.GetHeadPosition();
		for (int i=0;i<CGlobalManager::Manager()->m_bioProbeInfo.GetCount();i++)
		{
			needle = CGlobalManager::Manager()->m_bioProbeInfo.GetNext(pos);
			sTmp.Format("%s",needle.name);
			pList->AddConstraint(_T(sTmp));
		}
		m_pItemType->SetValue(sTmp);

	}
	if(sPara=="Cryotherapy")
	{
		pos=CGlobalManager::Manager()->m_cryoProbeInfo.GetHeadPosition();
		for (int i=0;i<CGlobalManager::Manager()->m_cryoProbeInfo.GetCount();i++)
		{
			needle = CGlobalManager::Manager()->m_cryoProbeInfo.GetNext(pos);
			sTmp.Format("%s",needle.name);
			pList->AddConstraint(_T(sTmp));
		}
		m_pItemType->SetValue(sTmp);

		m_pItemIceBall=pSettings->AddChildItem(new CXTPPropertyGridItemBool(_T(ICEBALL), TRUE));
		m_pItemIceBall->SetDescription(_T("If display ice ball on this probe"));

		m_pItemBallShell1=m_PropGrid.AddCategory(_T("0 Degree Ball Shell"));
		m_pBS1Visible=m_pItemBallShell1->AddChildItem(new CXTPPropertyGridItemBool(_T(BALLSHELLVISIBLE), TRUE));
		m_pBS1Color = m_pItemBallShell1->AddChildItem(new CCustomItemColor(_T(BALLSHELLCOLOR), RGB(0,0,0)));
		m_pBS1Opacity=m_pItemBallShell1->AddChildItem(new CCustomItemSlider(_T(BALLSHELLOPACITY)));

		m_pItemBallShell2=m_PropGrid.AddCategory(_T("-20 Degree Ball Shell"));
		m_pBS2Visible=m_pItemBallShell2->AddChildItem(new CXTPPropertyGridItemBool(_T(BALLSHELLVISIBLE), TRUE));
		m_pBS2Color = m_pItemBallShell2->AddChildItem(new CCustomItemColor(BALLSHELLCOLOR, RGB(0,0,0)));
		m_pBS2Opacity=m_pItemBallShell2->AddChildItem(new CCustomItemSlider(BALLSHELLOPACITY));

		m_pItemBallShell3=m_PropGrid.AddCategory(_T("-40 Degree Ball Shell"));
		m_pBS3Visible=m_pItemBallShell3->AddChildItem(new CXTPPropertyGridItemBool(_T(BALLSHELLVISIBLE), TRUE));
		m_pBS3Color = m_pItemBallShell3->AddChildItem(new CCustomItemColor(BALLSHELLCOLOR, RGB(0,0,0)));
		m_pBS3Opacity=m_pItemBallShell3->AddChildItem(new CCustomItemSlider(BALLSHELLOPACITY));

		m_pItemBallShell1->Collapse();
		m_pItemBallShell1->Expand();
		m_pItemBallShell2->Collapse();
		m_pItemBallShell2->Expand();
		m_pItemBallShell3->Collapse();
		m_pItemBallShell3->Expand();
		

	}
	m_pItemType->SetFlags(xtpGridItemHasComboButton);
	m_PropGrid.SetPropertySort(xtpGridSortCategorized);
	
	pSettings->Collapse();
	pSettings->Expand();
	
}

void CPropDlgProbe::OnChangeBallShellOpacity(CString sPara)
{
	int i;
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==m_probeName)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					if(j==0){
						ball_shell.Opacity=atoi(m_pBS1Opacity->GetValue());
						
					}
					if(j==1){
						ball_shell.Opacity=atoi(m_pBS2Opacity->GetValue());
					}
					if(j==2){
						ball_shell.Opacity=atoi(m_pBS3Opacity->GetValue());
					}
					pBall->SetShellProperty(ball_shell);

				}
				

				
			}//end
			m_pFrmMgr->SendCommand(this,CMD_TPS_RENDERREFRESHALL,m_probeName);
			
			break;
			
			

		}
	}	
}

void CPropDlgProbe::OnChangeBallShellColor(CString sPara)
{
	int i;
	CGlobalManager::Manager()->m_pAssemblys->InitTraversal();
	int nCount = CGlobalManager::Manager()->m_pAssemblys->GetNumberOfItems(); 
	for(i=0;i<nCount;i++)
	{
		CProbeCryo* pAssembly = (CProbeCryo*)CGlobalManager::Manager()->m_pAssemblys->GetNextProp3D();
		if(pAssembly->m_name==m_probeName)
		{
			if(pAssembly->m_pAttachIceBall)
			{
				CBallIceGalil* pBall=pAssembly->m_pAttachIceBall;

				pBall->m_pShell->InitTraversal();
				for(int j=0;j<pBall->m_pShell->GetNumberOfItems();j++)
				{
					BALL_SHELL ball_shell;
					ball_shell = pBall->m_lstBallShell.GetAt(j);
					if(j==0){
						ball_shell.Color=StringToRGB(m_pBS1Color->GetValue());
						
					}
					if(j==1){
						ball_shell.Color=StringToRGB(m_pBS2Color->GetValue());
					}
					if(j==2){
						ball_shell.Color=StringToRGB(m_pBS3Color->GetValue());
					}
					pBall->SetShellProperty(ball_shell);

				}
				

				
			}//end
			m_pFrmMgr->SendCommand(this,CMD_TPS_RENDERREFRESHALL,m_probeName);
			
			break;
			
			

		}
	}	
}
