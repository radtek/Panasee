// TracerPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TracerPropDlg.h"
#include "GlobalManager.h"
#include "CustomItems.h"
#include "ConfigMgr.h"
#include "PanaGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDlgTracer dialog


CPropDlgTracer::CPropDlgTracer(CWnd* pParent /*=NULL*/)
	: CPanaseeDialog(CPropDlgTracer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropDlgTracer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropDlgTracer::DoDataExchange(CDataExchange* pDX)
{
	CPanaseeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlgTracer)
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_TRACERPROP_PLACEHODER, m_placeHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropDlgTracer, CPanaseeDialog)
	//{{AFX_MSG_MAP(CPropDlgTracer)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlgTracer message handlers

BOOL CPropDlgTracer::OnInitDialog() 
{
	EnableSeprateLine(FALSE);

	CPanaseeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CList<CButton *,CButton *> btnList;
	btnList.AddTail(&m_btnClose);
	//SetBottomBtnList(&btnList);
	
	// TODO: Add extra initialization here
	//GetDlgItem(IDC_PROPLIST)->ShowWindow(SW_HIDE);
	
	SetWindowText(m_TraceName);
	m_tracerGrpList=&(CGlobalManager::Manager()->m_tracerGrpList);
	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	

	CRect rc;
	m_placeHolder.GetWindowRect( &rc );
	ScreenToClient( &rc );
	

	// create the property grid.
	if ( m_PropGrid.Create( rc, this, 0 ) )
	{
		// create document settings category.
        CXTPPropertyGridItem* pSettings        = m_PropGrid.AddCategory(_T("Tracer Settings"));
		m_pItemName=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Name"), _T("")));
		m_pItemName->SetDescription(_T("Set the tracer name"));
		//m_pItemName->SetReadOnly(TRUE);

		m_pItemStyle= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Style"), _T("Tumor")));
		m_pItemStyle->SetDescription(_T("the tracer style"));
		m_pItemStyle->SetReadOnly(TRUE);

		m_pItemSliceIndex=pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Slice Index"), _T("")));
		m_pItemSliceIndex->SetReadOnly(TRUE);

		m_pItemGroup= pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Group"), _T("")));
		m_pItemGroup->SetDescription(_T("Set group index for it.\nCreate multi modals by group index."));
		CXTPPropertyGridItemConstraints* pList = m_pItemGroup->GetConstraints();
		pList = m_pItemGroup->GetConstraints();
		pos = m_tracerGrpList->GetHeadPosition();
		for (int i=0;i<m_tracerGrpList->GetCount();i++)
		{
			zoneGroup = m_tracerGrpList->GetNext(pos);
			sTmp.Format("%s",zoneGroup.groupName);
			pList->AddConstraint(_T(sTmp));
		}
		m_pItemGroup->SetFlags(xtpGridItemHasComboButton);
		
		
        
	}

	m_PropGrid.SetPropertySort(xtpGridSortNoSort);
	m_PropGrid.SetCustomColors( RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);
	
	
	POLY_INFO polyinfo;
	CString str,sDscr;
	
	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	
	for (int i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==m_TraceName){
			m_pItemName->SetValue(m_TraceName);
			
			str.Format("%i",polyinfo.iSliceIndex);
			m_pItemSliceIndex->SetValue(str);
			

			break;
		}
		
	}
	pos = m_tracerGrpList->GetHeadPosition();
	for (i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		if(zoneGroup.groupIndex==polyinfo.iIndex)
		{
			sTmp.Format("%s",zoneGroup.groupName);
			m_pItemGroup->SetValue(sTmp);
			m_groupName=sTmp;
			if(zoneGroup.zType== TUMOR)
				m_pItemStyle->SetValue("Tumor");
			else
				m_pItemStyle->SetValue("Organ");
		}
	}
	this->CenterWindow(CWnd::GetDesktopWindow());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPropDlgTracer::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		
		if(pItem->GetCaption()==_T("Name"))
		{
			OnChangeName(pItem->GetValue());
		}
		
		if(pItem->GetCaption()==_T("Group"))
		{
			OnChangeGroup(pItem->GetValue());
		}

	}
		
	return 0;
}



void CPropDlgTracer::OnChangeName(CString para)
{
	POLY_INFO polyinfo;
	ACTOR_INFO actorInfo;
	POSITION pos,pos1;
	BOOL bFound;
	int i;

	bFound=FALSE;
	
	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	pos1=CGlobalManager::Manager()->m_ActorInfoList.GetHeadPosition();
	for (i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		actorInfo=CGlobalManager::Manager()->m_ActorInfoList.GetNext(pos1);
		CString sDscr;
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==para)
		{
			bFound=TRUE;

			break;
		}
		
	}

	if(bFound)
	{
		PanaMessageBox("Name must be unique.");
		m_pItemName->SetValue(m_TraceName);
		return;
	}
	
	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	pos1=CGlobalManager::Manager()->m_ActorInfoList.GetHeadPosition();
	for (i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		actorInfo=CGlobalManager::Manager()->m_ActorInfoList.GetNext(pos1);
		CString sDscr;
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==m_TraceName)
		{
			sprintf(polyinfo.sDscr,"%s",para);
			m_TraceName=para;
			SetWindowText(m_TraceName);

			vtkVectorText * atext = vtkVectorText::New();
			atext->SetText(m_TraceName);
			vtkPolyDataMapper* textMapper = vtkPolyDataMapper::New();
			textMapper->ImmediateModeRenderingOn();
			textMapper->SetInputConnection(atext->GetOutputPort());
			actorInfo.pFollower->SetMapper(textMapper);
			textMapper->Delete();
			atext->Delete();
			CGlobalManager::Manager()->m_PolyInfoList.SetAt(
					CGlobalManager::Manager()->m_PolyInfoList.FindIndex(i),polyinfo);
			CGlobalManager::Manager()->m_2DView1->Invalidate(FALSE);

			break;
		}
		
	}


}

void CPropDlgTracer::OnChangeGroup(CString para)
{
	POLY_INFO polyinfo;
	POSITION pos;
	ZONEGROUP_INFO zoneGroup;
	
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		CString sName;
		sName.Format("%s",zoneGroup.groupName);
		if(sName==para)
		{
			m_groupName=sName;
			break;
		}
	}

	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	for ( i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		CString sDscr;
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==m_TraceName)
		{
			
			polyinfo.iIndex=zoneGroup.groupIndex;
			CGlobalManager::Manager()->m_PolyInfoList.SetAt(
				CGlobalManager::Manager()->m_PolyInfoList.FindIndex(i),polyinfo);
			
			break;
		}
		
	}

	
}




