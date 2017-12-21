// ModalPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModalPropDlg.h"
#include "GlobalManager.h"
#include "CustomItems.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDlgModal dialog


CPropDlgModal::CPropDlgModal(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPropDlgModal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropDlgModal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropDlgModal::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlgModal)
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_MODALPROP_PLACEHODER, m_placeHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropDlgModal, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPropDlgModal)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlgModal message handlers

BOOL CPropDlgModal::OnInitDialog() 
{
	EnableSeprateLine(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnClose);
	//SetBottomBtnList(&btnList);
	
	// TODO: Add extra initialization here
	SetWindowText(m_modalName);
	
	CRect rc;
	m_placeHolder.GetWindowRect( &rc );
	ScreenToClient( &rc );
	

	// create the property grid.
	if ( m_PropGrid.Create( rc, this, 0 ) )
	{
		// create document settings category.
        
        //m_wndPropertyGrid.ShowWindow(SW_HIDE);
		InitialPara();
		m_PropGrid.SetPropertySort(xtpGridSortNoSort);
		m_PropGrid.SetCustomColors( RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);
		
	}
	else
	{
		PanaMessageBox("Create Property Grid Failer!");
	}

	this->CenterWindow(CWnd::GetDesktopWindow());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPropDlgModal::OnGridNotify(WPARAM wParam, LPARAM lParam)
{

	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
			
		if (pItem->GetCaption() == _T("Style"))
		{
			OnChangeStyle(pItem->GetValue());
				
		}

		if (pItem->GetCaption() == _T("Modal Color"))
		{
			OnChangeModalColor(pItem->GetValue());
				
		}

		if (pItem->GetCaption() == _T("Name"))
		{
			OnChangeName(pItem->GetValue());
				
		}
		if (pItem->GetCaption() == _T("Modal Opacity"))
		{
			OnChangeOpacity(pItem->GetValue());
				
		}
		

	}
	return 0;
}

void CPropDlgModal::InitialPara()
{
	CList<CModal*,CModal*> *m_modalGrpList;
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	CString sName;
	POSITION pos;
	BOOL bFind=FALSE;

	//find modal info by its name
	pos = m_modalGrpList->GetHeadPosition();
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		sName.Format("%s",m_modalInfo->m_name);
		if(m_modalName == sName )
		{
			bFind=TRUE;
			break;
		}
			
		
	}

	if(!bFind)
		return;

	CXTPPropertyGridItem* pSettings;
	CXTPPropertyGridItem* pItem;
	CString sTmp;

	pSettings= m_PropGrid.AddCategory(_T("Modal Settings"));
	
	sTmp.Format("%s",m_modalInfo->m_name);
	m_pItemName=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Name"), _T(sTmp)));
	m_pItemName->SetDescription(_T("Set the modal name"));
	
	if(m_modalInfo->m_zType ==TUMOR)
		pItem= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Style"), _T("Tumor")));
	else
		pItem= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Style"), _T("Organ")));
	
	pItem->SetDescription(_T("Set the modal style"));
	CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints();
	pList->AddConstraint(_T("Tumor"));
	pList->AddConstraint(_T("Organ"));
	pItem->SetFlags(xtpGridItemHasComboButton);


	pItem = pSettings->AddChildItem(new CCustomItemColor("Modal Color", m_modalInfo->m_color));

	pItem=pSettings->AddChildItem(new CCustomItemSlider(_T("Modal Opacity")));
	sTmp.Format("%i",m_modalInfo->m_opacity);
	pItem->SetValue(sTmp);
	
	

	sTmp.Format("%.2f",m_modalInfo->m_volume);
	pItem=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Volume"), _T(sTmp)));
	pItem->SetDescription(_T("Volume of this modal"));
	pItem->SetReadOnly(TRUE);

	
}

void CPropDlgModal::OnChangeName(CString sPara)
{
	CList<CModal*,CModal*> *m_modalGrpList;
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	CString sName;
	POSITION pos;
	BOOL bFind=FALSE;

	//find modal info by its name
	pos = m_modalGrpList->GetHeadPosition();
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		sName.Format("%s",m_modalInfo->m_name);
		if(sPara == sName )
		{
			bFind=TRUE;
			break;
		}
			
		
	}
	if(bFind)
	{
		PanaMessageBox("Name must be unique.");
		m_pItemName->SetValue(m_modalName);
		return;
	}

	pos = m_modalGrpList->GetHeadPosition();
	for ( i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		sName.Format("%s",m_modalInfo->m_name);
		if(m_modalName == sName )
		{
			m_modalInfo->m_name.Format("%s",sPara);
			m_modalName=sPara;
			m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),m_modalInfo);
			SetWindowText(sPara);
			//
			break;
		}
			
		
	}
}

void CPropDlgModal::OnChangeStyle(CString sPara)
{
	CList<CModal*,CModal*> *m_modalGrpList;
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	CString sName;
	POSITION pos;
	BOOL bFind=FALSE;

	//find modal info by its name
	pos = m_modalGrpList->GetHeadPosition();
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		sName.Format("%s",m_modalInfo->m_name);
		if(m_modalName == sName )
		{
			if(sPara=="Tumor")
				m_modalInfo->m_zType =TUMOR;
			if(sPara=="Organ")
				m_modalInfo->m_zType =ORGAN;

			m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),m_modalInfo);
			
			break;
		}
			
		
	}
	
}

void CPropDlgModal::OnChangeModalColor(CString sPara)
{
	CList<CModal*,CModal*> *m_modalGrpList;
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	vtkActor* pActor;

	CString sName;
	POSITION pos;
	BOOL bFind=FALSE;
	

	//find modal info by its name
	pos = m_modalGrpList->GetHeadPosition();
	
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		pActor=m_modalInfo->m_actor;
		sName.Format("%s",m_modalInfo->m_name);
		if(m_modalName == sName )
		{
			m_modalInfo->m_color=StringToRGB(sPara);
			
			pActor->GetProperty()->SetColor((float )GetRValue(m_modalInfo->m_color)/255.0,
				(float )GetGValue(m_modalInfo->m_color)/255.0,(float )GetBValue(m_modalInfo->m_color)/255.0);

			m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),m_modalInfo);
			CGlobalManager::Manager()->m_3DView->Invalidate(FALSE);
			break;
		}
			
		
	}
}

void CPropDlgModal::OnChangeOpacity(CString sPara)
{
	CList<CModal*,CModal*> *m_modalGrpList;
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	
	vtkActor* pActor;

	CString sName;
	POSITION pos;
	BOOL bFind=FALSE;
	

	//find modal info by its name
	pos = m_modalGrpList->GetHeadPosition();
	
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		m_modalInfo = m_modalGrpList->GetNext(pos);
		pActor=(vtkActor *)m_modalInfo->m_actor;
		sName.Format("%s",m_modalInfo->m_name);
		if(m_modalName == sName )
		{
			m_modalInfo->m_opacity=atoi(sPara);
			
			pActor->GetProperty()->SetOpacity(m_modalInfo->m_opacity/100.0);

			m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),m_modalInfo);
			CGlobalManager::Manager()->m_3DView->Invalidate(FALSE);
			break;
		}
			
		
	}
}
