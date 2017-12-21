// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ControlDlg.h"
#include "Commands.h"
#include "DisplayDlg.h"
#include "TracerPropDlg.h"
#include "ProbePropDlg.h"
#include "ModalPropDlg.h"
#include "ConfigMgr.h"
#include "GlobalDef.h"
#include "PanaGlobal.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)

BOOL ConfirmDeleteDlg(CString sText,CString sCaption="");


/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CAppDialog(CControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlDlg)
	//}}AFX_DATA_INIT

	CGlobalManager::Manager()->m_sTpsFileLoaded="";
	
}

CControlDlg::~CControlDlg()
{
	m_probeControl.DestroyWindow();
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlDlg)
	DDX_Control(pDX, IDC_BUTTON_MENU_HELP, m_btnMenuHelp);
	DDX_Control(pDX, IDC_BUTTON_MENU_FILE, m_btnMenuFile);
	DDX_Control(pDX, IDC_PROP3D_LIST, m_Prop3DList);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BMP_SAVEIMAGE, m_btnSaveImage);
	DDX_Control(pDX, IDC_BMP_LOADIMAGE, m_btnLoadImage);
	DDX_Control(pDX, IDC_BMP_EVALUATE, m_btnEvaluate);
	DDX_Control(pDX, IDC_BMP_CREATETOOL, m_btnCreateTool);
		
}


BEGIN_MESSAGE_MAP(CControlDlg, CAppDialog)
	//{{AFX_MSG_MAP(CControlDlg)
	ON_NOTIFY(NM_RCLICK, IDC_PROP3D_LIST, OnRclickProp3dList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PROP3D_LIST, OnSelchangedProp3dList)
	ON_COMMAND(IDR_POPMENU_ZONE_DELETE, OnPopmenuZoneDelete)
	ON_COMMAND(IDR_POPMENU_ZONE_PROP, OnPopmenuZoneProp)
	ON_COMMAND(IDR_POPMENU_PROBE_BALL_VISIBLE, OnPopmenuProbeBallVisible)
	ON_COMMAND(IDR_POPMENU_PROBE_DELETE, OnPopmenuProbeDelete)
	ON_COMMAND(IDR_POPMENU_PROBE_PROP, OnPopmenuProbeProp)
	ON_BN_CLICKED(IDC_BMP_EVALUATE, OnBmpEvaluate)
	ON_BN_CLICKED(IDC_BMP_CREATETOOL, OnBmpCreatetool)
	ON_COMMAND(IDR_POPUPMENU_FILE_NEW, OnPopupmenuFileNew)
	ON_COMMAND(IDR_POPUPMENU_FILE_OPEN, OnPopupmenuFileOpen)
	ON_COMMAND(IDR_POPUPMENU_FILE_SAVE, OnPopupmenuFileSave)
	ON_COMMAND(IDR_POPUPMENU_FILE_SAVEAS, OnPopupmenuFileSaveas)
	ON_BN_CLICKED(IDC_BMP_SAVEIMAGE, OnBmpSaveimage)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_CREATEMODAL, OnPopmenuZonegroupCreatemodal)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_DELETE, OnPopmenuZonegroupDelete)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_NEW, OnPopmenuZonegroupNew)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_RENAME, OnPopmenuZonegroupRename)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_PROP3D_LIST, OnEndlabeleditProp3dList)
	ON_COMMAND(IDR_POPUPMENU_MODAL_DELETE, OnPopupmenuModalDelete)
	ON_COMMAND(IDR_POPUPMENU_MODAL_PROP, OnPopupmenuModalProp)
	ON_COMMAND(IDR_POPUPMENU_MODAL_RENAME, OnPopupmenuModalRename)
	ON_COMMAND(IDR_POPUPMENU_MODAL_VISIBLE, OnPopupmenuModalVisible)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_ORGAN, OnPopmenuZonegroupOrgan)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_TUMOR, OnPopmenuZonegroupTumor)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_SETCOLOR, OnPopmenuZonegroupSetcolor)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BMP_LOADIMAGE, OnBmpLoadimage)
	ON_COMMAND(IDR_POPMENU_PROBE_LINE_VISIBLE, OnPopmenuProbeLineVisible)
	ON_COMMAND(IDR_POPMENU_ZONE_TRACKPOSITION, OnPopmenuZoneTrackposition)
	ON_COMMAND(IDR_POPMENU_ZONEGROUP_SETDEFAULT, OnPopmenuZonegroupSetdefault)
	ON_COMMAND(IDR_POPMENU_PROBE_UCS, OnPopmenuProbeUcs)
	ON_COMMAND(IDR_POPMENU_PROBE_COPYUCS, OnPopmenuProbeCopyucs)
	ON_BN_CLICKED(IDC_BUTTON_MENU_FILE, OnButtonMenuFile)
	ON_MESSAGE(WM_PANA_GETPARENTBK, GetParentBK)
	ON_WM_SIZE()
	ON_COMMAND(IDR_POPMENU_PROBE_MPR, OnPopmenuProbeMpr)
	ON_MESSAGE(WM_CHANGETRACEGROUP, OnChangeTracerGroup)
	ON_NOTIFY(NM_CLICK, IDC_PROP3D_LIST, OnClickProp3dList)
	//}}AFX_MSG_MAP
	
	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlDlg message handlers

BOOL CControlDlg::OnInitDialog() 
{
	
	CAppDialog::OnInitDialog();
	
	funSetHandle(this);
	//funCreateDraw();
	funOnInitDialog();

	// TODO: Add extra initialization here
	// get the size of the place holder, this will be used when creating the grid.

	m_btnMenuFile.SetFlat();
	m_btnMenuHelp.SetFlat();
	m_btnMenuFile.DrawTransparent();
	m_btnMenuHelp.DrawTransparent();

	short	shBtnColor = 30;
	
	SetEvaluateBtn(&m_btnEvaluate);
	SetSaveBtn(&m_btnSaveImage);
	SetOpenBtn(&m_btnLoadImage);
	SetProbeBtn(&m_btnCreateTool);

			
	CreateProp3DTree();
	
	m_bTrackTracer=FALSE;
	
	m_pDlgAutoTrace.Create(IDD_DIALOG_AUTO_TRACE,this);
	m_pDlgAutoTrace.SetParent(this);
	m_pDlgAutoTrace.m_pFrmMgr=GetFrameMgr();
	m_pDlgAutoTrace.ShowWindow(SW_HIDE);
	//HideDlgAutoTrace();

	m_probeControl.Create(IDD_DIALOG_PROBE_CONTROL,this);
	m_probeControl.ShowWindow(SW_HIDE);
	//HideProbeControl();

	CGlobalManager::Manager()->m_pTreeCtrl=&m_Prop3DList;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlDlg::CreateProp3DTree()
{
		
	// load the tree images bitmap and add it to the image list.
	if (m_ilstProp3D.Create(IDB_TOOL_TREE_IMAGE, 16, 1, RGB(0x00,0x80,0x80)))
		m_Prop3DList.SetImageList( &m_ilstProp3D, TVSIL_NORMAL );


	// add the parent item	
	m_probeItem = m_Prop3DList.InsertItem(_T("Probe"), 2, 2);
	m_zoneItem=m_Prop3DList.InsertItem(_T("Tracer"), 3, 3);
	m_modalItem=m_Prop3DList.InsertItem(_T("Modal"), 4, 4);
	m_tracerGrpList=&(CGlobalManager::Manager()->m_tracerGrpList);
	m_modalGrpList=&(CGlobalManager::Manager()->m_modalGrpList);
	OnPopmenuZonegroupNew();
	
	
}

void CControlDlg::DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara)
{
	if(pSender == this)
		return;

	switch(nCmdID)
	{
	case CMD_TPS_SHOW_PROBECONTROL:
		ShowProbeControl();
		break;
	case CMD_TPS_HIDE_PROBECONTROL:
		HideProbeControl();
		break;
	case CMD_TPS_SETACTIVE_WINDOW:
		m_pDlgAutoTrace.SetActiveWnd(sPara);
		break;
	case CMD_TPS_CONTROLDLG_RIGHTCLICK:
		OnRclickProp3dList(NULL,NULL);
		break;
	case CMD_TPS_SELECTTRACER:
		OnSelectTracer(sPara);
		break;
	case CMD_TPS_SELECTMODAL:
		OnSelectModal(sPara);
		break;
	case CMD_TPS_SELECTPROBE:
		OnSelectProbe(sPara);
		break;
	case CMD_TPS_LINEVISIBLESTATUS:
		m_lineVisibleStatus=sPara;
		break;
	case CMD_TPS_CREATEMODALOVER:
		OnCreateModalOver(sPara);
		break;
	case CMD_TPS_RECONSTRUCT_TRACERGROUP:
		OnReconstructTracerGroup();
		break;
	case CMD_TPS_CLEARZONE:
		{
			int index=atoi(sPara);
			OnClearZone(index);
			break;
		}
	case CMD_TPS_CLEARTREELIST:
		OnClearTreeList();
		break;
	case CMD_TPS_BALLVISIBLESTATUS:
		m_ballVisibleStatus=sPara;
		break;
	case CMD_TPS_CREATETRACEOVER:
		OnAddTracerToList(sPara);
		break;		
	case CMD_TPS_CREATEPROBEOVER:
		m_Prop3DList.InsertItem(_T(sPara), 2, 2,m_probeItem);
		m_Prop3DList.Expand(m_probeItem,TVE_EXPAND);
		break;
	default:
		break;
	}
}



void CControlDlg::OnRclickProp3dList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW *pNMTreeView=(NM_TREEVIEW *)pNMHDR;
	// Select the item that is at the point myPoint.
	UINT uFlags;
	CPoint lpoint;
	
	GetCursorPos(&lpoint);
	m_Prop3DList.ScreenToClient(&lpoint);
	HTREEITEM hItem = m_Prop3DList.HitTest(lpoint, &uFlags);
	if(hItem && (uFlags & TVHT_ONITEM) && !(uFlags & TVHT_ONITEMRIGHT))
		m_Prop3DList.SelectItem(hItem);

 	
  	HTREEITEM parent;
  	CMenu menu,*pSubMenu;
  	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
  	{
  		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
  	}else
  	{
  		return;
  	}
  
  	if(m_Prop3DList.GetItemText(parent)=="Image")
  	{
  		
  		LPPOINT lpoint=new tagPOINT;
  		::GetCursorPos(lpoint);
  		menu.LoadMenu(IDR_POPUPMENU_IMAGE);
  		pSubMenu=menu.GetSubMenu(0);
  		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
  		delete lpoint;
  
  	}
  
  	if(m_Prop3DList.GetItemText(parent)=="Probe")
  	{
  		CString sPara;
  		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
  		SendCommand(this,CMD_TPS_BALLVISIBLESTATUS,sPara);
		SendCommand(this,CMD_TPS_LINEVISIBLESTATUS,sPara);
  		LPPOINT lpoint=new tagPOINT;
  		::GetCursorPos(lpoint);
  		menu.LoadMenu(IDR_POPUPMENU_PROBE);
  		pSubMenu=menu.GetSubMenu(0);
  		if(m_ballVisibleStatus=="VISIBLE")
  			pSubMenu->CheckMenuItem(IDR_POPMENU_PROBE_BALL_VISIBLE,MF_CHECKED|MF_BYCOMMAND);
		if(m_lineVisibleStatus=="VISIBLE")
  			pSubMenu->CheckMenuItem(IDR_POPMENU_PROBE_LINE_VISIBLE,MF_CHECKED|MF_BYCOMMAND);
  
  		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
  		delete lpoint;
  	}
  
  	if(m_Prop3DList.GetItemText(parent)=="Tracer")
  	{
  		LPPOINT lpoint=new tagPOINT;
  		::GetCursorPos(lpoint);
  		menu.LoadMenu(IDR_POPUPMENU_ZONEGROUP);
  		pSubMenu=menu.GetSubMenu(0);
		if(GetCurSelectedGroupType()==TUMOR)
		{
			pSubMenu->CheckMenuItem(IDR_POPMENU_ZONEGROUP_TUMOR,MF_CHECKED|MF_BYCOMMAND);
		}else
		{
			pSubMenu->CheckMenuItem(IDR_POPMENU_ZONEGROUP_ORGAN,MF_CHECKED|MF_BYCOMMAND);
		}
  		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
  		delete lpoint;
  	}
  
  	if(m_Prop3DList.GetItemText(parent)=="Modal")
  	{
  		
		CString sPara;
  		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
  		SendCommand(this,CMD_TPS_GETMODALVISIBLESTATUS,sPara);

		LPPOINT lpoint=new tagPOINT;
  		::GetCursorPos(lpoint);
  		menu.LoadMenu(IDR_POPUPMENU_MODAL);
  		pSubMenu=menu.GetSubMenu(0);
		if(sPara=="VISIBLE")
  			pSubMenu->CheckMenuItem(IDR_POPUPMENU_MODAL_VISIBLE,MF_CHECKED|MF_BYCOMMAND);
  		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
  		delete lpoint;
  	}

	if(m_Prop3DList.GetItemText(m_Prop3DList.GetNextItem(parent,TVGN_PARENT))=="Tracer")
	{

		LPPOINT lpoint=new tagPOINT;
  		::GetCursorPos(lpoint);
  		menu.LoadMenu(IDR_POPUPMENU_ZONE);
  		pSubMenu=menu.GetSubMenu(0);
		if(m_bTrackTracer)
			pSubMenu->CheckMenuItem(IDR_POPMENU_ZONE_TRACKPOSITION,MF_CHECKED|MF_BYCOMMAND);

  		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
  		delete lpoint;
	}
  	
	if(pResult!=NULL)
		*pResult = 0;
}

void CControlDlg::OnClickProp3dList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_SELECTPROBE,sPara);
		
	}

	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_SELECTMODAL,sPara);
		
	}

	*pResult = 0;
}


void CControlDlg::OnSelchangedProp3dList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_SELECTPROBE,sPara);
		
	}

	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_SELECTMODAL,sPara);
		
	}

	if(m_Prop3DList.GetItemText(m_Prop3DList.GetNextItem
		(parent,TVGN_PARENT))=="Tracer")
	{	
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		if(m_bTrackTracer)
			SendCommand(this,CMD_TPS_SELECTTRACERTRACK,sPara);
		else
			SendCommand(this,CMD_TPS_SELECTTRACER,sPara);
	}

	if(m_Prop3DList.GetItemText(m_Prop3DList.GetNextItem(parent,TVGN_PARENT))=="Tracer" || 
	   m_Prop3DList.GetItemText(parent)=="Tracer" ||
	   m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem())=="Tracer")
	{
		ShowDlgAutoTrace();
	}else
	{
		HideDlgAutoTrace();
	}


	*pResult = 0;
}


void CControlDlg::OnPopmenuZoneDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(m_Prop3DList.GetNextItem(parent,TVGN_PARENT))=="Tracer")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		if(!ConfirmDeleteDlg(sPara))
			return;
		SendCommand(this,CMD_TPS_DELETETRACE,sPara);
		m_Prop3DList.DeleteItem(m_Prop3DList.GetSelectedItem());
	}
}

void CControlDlg::OnPopmenuZoneProp() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(m_Prop3DList.GetNextItem(parent,TVGN_PARENT))=="Tracer")
	{
		CString sPara,sOldGroupName,sGroupName;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		sOldGroupName=m_Prop3DList.GetItemText(parent);
		CPropDlgTracer dlg;
		dlg.m_TraceName=sPara;
		dlg.DoModal();
		sPara=dlg.m_TraceName;
		sGroupName=dlg.m_groupName;
		if(sGroupName==sOldGroupName)
		{
			m_Prop3DList.SetItemText(m_Prop3DList.GetSelectedItem(),sPara);
		}else
		{
			m_Prop3DList.DeleteItem(m_Prop3DList.GetSelectedItem());
			POSITION pos;
			ZONEGROUP_INFO zoneGroup;
	
			pos = m_tracerGrpList->GetHeadPosition();
			for (int i=0;i<m_tracerGrpList->GetCount();i++)
			{
				zoneGroup = m_tracerGrpList->GetNext(pos);
				CString sName;
				sName.Format("%s",zoneGroup.groupName);
				if(sName==sGroupName)
				{
					m_Prop3DList.InsertItem(_T(sPara), 3, 3,zoneGroup.treeItem);
					
					break;
				}
			}
		}
		
	}
	
}

void CControlDlg::OnPopmenuProbeBallVisible() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_ICEBALLVISIBLE,sPara);
		
	}
}

void CControlDlg::OnPopmenuProbeDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		if(!ConfirmDeleteDlg(sPara))
			return;
		SendCommand(this,CMD_TPS_DELETEPROBE,sPara);
		m_Prop3DList.DeleteItem(m_Prop3DList.GetSelectedItem());
	}
}

void CControlDlg::OnPopmenuProbeProp() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		CPropDlgProbe dlg;
		dlg.m_probeName=sPara;
		dlg.SetFrmMgr(GetFrameMgr());
		dlg.DoModal();
		sPara=dlg.m_probeName;
		m_Prop3DList.SetItemText(m_Prop3DList.GetSelectedItem(),sPara);
	}
}

void CControlDlg::OnClearTreeList()
{
	HTREEITEM childItem;
	childItem = m_Prop3DList.GetChildItem(m_zoneItem);
	while(childItem!=NULL)
	{
		m_Prop3DList.DeleteItem(childItem);
		childItem = m_Prop3DList.GetChildItem(m_zoneItem);
	}

	childItem = m_Prop3DList.GetChildItem(m_probeItem);
	while(childItem!=NULL)
	{
		m_Prop3DList.DeleteItem(childItem);
		childItem = m_Prop3DList.GetChildItem(m_probeItem);
	}

	childItem = m_Prop3DList.GetChildItem(m_modalItem);
	while(childItem!=NULL)
	{
		m_Prop3DList.DeleteItem(childItem);
		childItem = m_Prop3DList.GetChildItem(m_modalItem);
	}
	
	m_tracerGrpList->RemoveAll();
	
	OnPopmenuZonegroupNew();
	

}

LONG CControlDlg::OnChangeTracerGroup(WPARAM w,LPARAM l)
{
	CString *sOldGroup=(CString *)w;
	CString *sTrace=(CString *)l;
	CString sOld=*sOldGroup;
	CString sTraceName=*sTrace;
	CString sNew=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
	
	POLY_INFO polyinfo;
	POSITION pos;
	ZONEGROUP_INFO zoneGroup;
	
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		CString sName;
		sName.Format("%s",zoneGroup.groupName);
		if(sName==sNew)
		{
			break;
		}
	}

	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	for ( i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		CString sDscr;
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==sTraceName)
		{
			
			polyinfo.iIndex=zoneGroup.groupIndex;
			CGlobalManager::Manager()->m_PolyInfoList.SetAt(
				CGlobalManager::Manager()->m_PolyInfoList.FindIndex(i),polyinfo);
			
			break;
		}
		
	}

	m_Prop3DList.InsertItem(_T(sTraceName), 3, 3,zoneGroup.treeItem);

	pos = m_tracerGrpList->GetHeadPosition();
	for (i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		CString sName;
		sName.Format("%s",zoneGroup.groupName);
		if(sName==sOld)
		{
			break;
		}
	}

	HTREEITEM childItem;
	childItem = m_Prop3DList.GetChildItem(zoneGroup.treeItem);
	while(childItem!=NULL)
	{
		if(m_Prop3DList.GetItemText(childItem)==sTraceName)
		{
			m_Prop3DList.DeleteItem(childItem);
			break;
		}
		childItem = m_Prop3DList.GetNextItem(childItem, TVGN_NEXT);

	}


	return 0;
}

void CControlDlg::OnBmpEvaluate() 
{
	// TODO: Add your control notification handler code here
	SendCommand(this,CMD_TPS_MPR);
}

void CControlDlg::OnBmpCreatetool() 
{
	// TODO: Add your control notification handler code here
	SendCommand(this,CMD_TPS_CREATETOOL);
	SendCommand(this,CMD_TPS_RENDERREFRESHALL);
}



void CControlDlg::OnPopupmenuFileNew() 
{
	// TODO: Add your command handler code here
	SendCommand(this,CMD_TPS_REMOVEALL);
	SendCommand(this,CMD_TPS_RENDERREFRESHALL);
	SendCommand(this,CMD_TPS_MPR_HIDE);
	OnClearTreeList();
	CGlobalManager::Manager()->m_sTpsFileLoaded="";
}

void CControlDlg::OnPopupmenuFileOpen() 
{
	// TODO: Add your command handler code here
	CString szFileName,szFilter = _T("TPS Data(*.tps)|*.tps");
	CFileDialog fileDlg(TRUE,									// Save 
		_T("tps"),								// Default Extension 
		_T("*.tps"),							// Default file name
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// Flag
		szFilter);								// Filters
	
	fileDlg.m_ofn.nFilterIndex = 1;
	fileDlg.m_ofn.lpstrInitialDir = ".";		// Current dir or set other path
	
	fileDlg.m_ofn.lpstrTitle = _T("Load Data");
	if (fileDlg.DoModal() == IDOK) 
		szFileName = fileDlg.GetPathName();
	else
		return;
	
	OnPopupmenuFileNew();
	
	CGlobalManager::Manager()->m_sTpsFileLoad=szFileName;
	CGlobalManager::Manager()->m_sTpsFileLoaded="";
}

void CControlDlg::OnPopupmenuFileSave() 
{
	// TODO: Add your command handler code here
	CString sPara;
	if(CGlobalManager::Manager()->m_sTpsFileLoaded!="")
	{
		sPara=CGlobalManager::Manager()->m_sTpsFileLoaded;
		
	}else
	{
		CString szFileName,szFilter = _T("TPS Data(*.tps)|*.tps");
		CFileDialog fileDlg(FALSE,									// Save 
			_T("tps"),								// Default Extension 
			_T("*.tps"),							// Default file name
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// Flag
			szFilter);								// Filters
		
		fileDlg.m_ofn.nFilterIndex = 1;
		fileDlg.m_ofn.lpstrInitialDir = ".";		// Current dir or set other path
		
		fileDlg.m_ofn.lpstrTitle = _T("Save Data");
		if (fileDlg.DoModal() == IDOK) 
			szFileName = fileDlg.GetPathName();
		else
		return;

		sPara=szFileName;
		CGlobalManager::Manager()->m_sTpsFileLoaded=szFileName;
	}

	SendCommand(this,CMD_TPS_SAVETPSINFOTOFILE,sPara);
}

void CControlDlg::OnPopupmenuFileSaveas() 
{
	// TODO: Add your command handler code here
	CString szFileName,szFilter = _T("TPS Data(*.tps)|*.tps");
	CFileDialog fileDlg(FALSE,									// Save 
		_T("tps"),								// Default Extension 
		_T("*.tps"),							// Default file name
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// Flag
		szFilter);								// Filters
	
	fileDlg.m_ofn.nFilterIndex = 1;
	fileDlg.m_ofn.lpstrInitialDir = ".";		// Current dir or set other path
	
	fileDlg.m_ofn.lpstrTitle = _T("Save Data");
	if (fileDlg.DoModal() == IDOK) 
		szFileName = fileDlg.GetPathName();
	else
		return;
	
	SendCommand(this,CMD_TPS_SAVETPSINFOTOFILE,szFileName);
	CGlobalManager::Manager()->m_sTpsFileLoaded=szFileName;
}

void CControlDlg::OnBmpSaveimage() 
{
	// TODO: Add your control notification handler code here
	OnPopupmenuFileSave();
}

void CControlDlg::OnAddTracerToList(CString para)
{
	POLY_INFO polyinfo;
	POSITION pos;
	CString str,sDscr;
	
	pos=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
	for (int i=0;i<CGlobalManager::Manager()->m_PolyInfoList.GetCount();i++)
	{
		polyinfo = CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos);
		sDscr.Format("%s",polyinfo.sDscr);
		if(sDscr ==para){
			break;
		}
		
	}

	ZONEGROUP_INFO zoneGroup;

	BOOL bFind=FALSE;
	pos = m_tracerGrpList->GetHeadPosition();
	for (i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		if(zoneGroup.groupIndex == polyinfo.iIndex)
		{
			bFind=TRUE;
			break;
		}
		
	}
	if(!bFind)
	{
		PanaMessageBox("Can't find correct tracer group.");

		return;
	}
		
	m_Prop3DList.InsertItem(_T(para), 3, 3,zoneGroup.treeItem);
	m_Prop3DList.Expand(zoneGroup.treeItem,TVE_EXPAND);
}

void CControlDlg::OnPopmenuZonegroupCreatemodal() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		sName=m_Prop3DList.GetItemText(selectedItem);
		SendCommand(this,CMD_TPS_CREATEMODAL,sName);
	}
	else
	{
		return;
	}
}

void CControlDlg::OnPopmenuZonegroupDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM childItem,selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	BOOL bDeleted=FALSE;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		
		sName=m_Prop3DList.GetItemText(selectedItem);
		if(!ConfirmDeleteDlg(sName))
			return;

		childItem = m_Prop3DList.GetChildItem(selectedItem);
		while(childItem!=NULL)
		{
			sName=m_Prop3DList.GetItemText(childItem);
			SendCommand(this,CMD_TPS_DELETETRACE,sName);
			m_Prop3DList.DeleteItem(childItem);
			childItem = m_Prop3DList.GetChildItem(selectedItem);
		}
		sName=m_Prop3DList.GetItemText(selectedItem);
		
		m_Prop3DList.DeleteItem(selectedItem);
		if(selectedItem==m_defaultItem)
		{
			bDeleted=TRUE;
		}
	}
	else
	{
		return;
	}

	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp == sName )
		{
			m_tracerGrpList->RemoveAt(m_tracerGrpList->FindIndex(i));
		}
	}

	if(bDeleted)
	{
		pos = m_tracerGrpList->GetHeadPosition();
		for (int i=0;i<m_tracerGrpList->GetCount();i++)
		{
			zoneGroup = m_tracerGrpList->GetNext(pos);
			m_defaultItem=zoneGroup.treeItem;
			m_Prop3DList.SetItemImage(zoneGroup.treeItem,5,5);
			sTmp.Format("%s",zoneGroup.groupName);
			SendCommand(this,CMD_TPS_SETDEFAULTGROUP,sTmp);
			break;
		}
	}

	if(m_tracerGrpList->GetCount()<=0)
	{
		OnPopmenuZonegroupNew();
		
	}

	
}

void CControlDlg::OnPopmenuZonegroupNew() 
{
	// TODO: Add your command handler code here
	//

	ZONEGROUP_INFO zoneGroup,tmpGroup;
	CString sTmp,sName;
	POSITION pos;
		
	tmpGroup.groupIndex=1;
	
	sTmp="Group_1";
	int j=1;

	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		sName.Format("%s",zoneGroup.groupName);
		if(sTmp == sName )
		{
			pos = m_tracerGrpList->GetHeadPosition();
			j++;
			i=-1;
			sTmp.Format("Group_%i",j);
				
		}
		if(tmpGroup.groupIndex ==zoneGroup.groupIndex)
		{
			pos = m_tracerGrpList->GetHeadPosition();
			tmpGroup.groupIndex++;
			i=-1;
		}
		
	}
	tmpGroup.zType=ORGAN;
	tmpGroup.zType=ORGAN;
	tmpGroup.color=RGB(0,255,0);
	tmpGroup.textColor=RGB(255,0,0);
	tmpGroup.textScale=15.0;

	tmpGroup.treeItem=m_Prop3DList.InsertItem(_T(sTmp), 3, 3,m_zoneItem);
	sprintf(tmpGroup.groupName,"%s",sTmp);
	m_tracerGrpList->AddTail(tmpGroup);
	m_Prop3DList.Expand(m_zoneItem,TVE_EXPAND);


	if(m_tracerGrpList->GetCount()==1)
	{
		m_Prop3DList.SetItemImage(tmpGroup.treeItem,5,5);
		m_defaultItem=tmpGroup.treeItem;
		SendCommand(this,CMD_TPS_SETDEFAULTGROUP,sTmp);
		
	}
	
	
}

void CControlDlg::OnPopmenuZonegroupRename() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		m_editLabelOldName=m_Prop3DList.GetItemText(selectedItem);
		m_Prop3DList.EditLabel(selectedItem);
		m_editLabelStyle=1; //edit tracer group
	}else
	{
		return;
	}
	

}

void CControlDlg::OnEndlabeleditProp3dList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	TVITEM & item = pTVDispInfo->item;
		
	if(item.pszText) {
		m_Prop3DList.SetItem(&item);
		if(m_editLabelStyle==1)
		{
			ZONEGROUP_INFO zoneGroup;
			CString sTmp;
			POSITION pos;
			pos = m_tracerGrpList->GetHeadPosition();
			for (int i=0;i<m_tracerGrpList->GetCount();i++)
			{
				zoneGroup = m_tracerGrpList->GetNext(pos);
				sTmp.Format("%s",zoneGroup.groupName);
				if(sTmp == m_editLabelOldName )
				{
					sprintf(zoneGroup.groupName,"%s",item.pszText);
					m_tracerGrpList->SetAt(m_tracerGrpList->FindIndex(i),zoneGroup);
					
				}
			}
		}

		if(m_editLabelStyle==2)
		{
			CModal* modalinfo;
			CString sTmp;
			POSITION pos;
			pos = m_modalGrpList->GetHeadPosition();
			for (int i=0;i<m_modalGrpList->GetCount();i++)
			{
				modalinfo = m_modalGrpList->GetNext(pos);
				sTmp.Format("%s",modalinfo->m_name);
				if(sTmp == m_editLabelOldName )
				{
					modalinfo->m_name.Format("%s",item.pszText);
					m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),modalinfo);
				}
			}
		}
	}
	*pResult = 0;
	
	
}

void CControlDlg::OnReconstructTracerGroup()
{
	HTREEITEM childItem;
	childItem = m_Prop3DList.GetChildItem(m_zoneItem);
	while(childItem!=NULL)
	{
		m_Prop3DList.DeleteItem(childItem);
		childItem = m_Prop3DList.GetChildItem(m_zoneItem);
	}

	
	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		zoneGroup.treeItem=m_Prop3DList.InsertItem(_T(sTmp), 3, 3,m_zoneItem);
		m_tracerGrpList->SetAt(m_tracerGrpList->FindIndex(i),zoneGroup);
		
	}

	if(m_tracerGrpList->GetCount()>0)
	{
		m_defaultItem=zoneGroup.treeItem;
		m_Prop3DList.SetItemImage(zoneGroup.treeItem,5,5);
		sTmp.Format("%s",zoneGroup.groupName);
		SendCommand(this,CMD_TPS_SETDEFAULTGROUP,sTmp);
	}
	
}

void CControlDlg::OnCreateModalOver(CString sPara)
{
	CString sName;
	POSITION pos;
	CModal* modalinfo;
	pos = m_modalGrpList->GetHeadPosition();
	for (int i=0;i<m_modalGrpList->GetCount();i++)
	{
		modalinfo = m_modalGrpList->GetNext(pos);
		sName.Format("%s",modalinfo->m_name);
		if(sPara == sName )
		{
			modalinfo->m_treeItem=m_Prop3DList.InsertItem(_T(sPara), 4, 4,m_modalItem);
			m_modalGrpList->SetAt(m_modalGrpList->FindIndex(i),modalinfo);
			break;
		}
			
		
	}

	m_Prop3DList.Expand(m_modalItem,TVE_EXPAND);

	
}

void CControlDlg::OnPopupmenuModalDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		CString sPara;
		selectedItem=m_Prop3DList.GetSelectedItem();
		sPara=m_Prop3DList.GetItemText(selectedItem);
		if(!ConfirmDeleteDlg(sPara))
			return;
		SendCommand(this,CMD_TPS_DELETEMODAL,sPara);
		m_Prop3DList.DeleteItem(selectedItem);
	}else
	{
		return;
	}

}

void CControlDlg::OnPopupmenuModalProp() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		//find valid vtkactor;
		CString sIsActor=sPara;
		SendCommand(this,CMD_TPS_ISACTOR,sIsActor);
		if(sIsActor!="OK")
			return;
		//
		CPropDlgModal dlg;
		dlg.m_modalName=sPara;
		dlg.SetFrmMgr(GetFrameMgr());
		dlg.DoModal();
		sPara=dlg.m_modalName;
		m_Prop3DList.SetItemText(m_Prop3DList.GetSelectedItem(),sPara);
	}
}

void CControlDlg::OnPopupmenuModalRename() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		m_editLabelOldName=m_Prop3DList.GetItemText(selectedItem);
		m_Prop3DList.EditLabel(selectedItem);
		m_editLabelStyle=2; //edit modal name
	}else
	{
		return;
	}
}

void CControlDlg::OnPopupmenuModalVisible() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Modal")
	{
		CString sPara;
		selectedItem=m_Prop3DList.GetSelectedItem();
		sPara=m_Prop3DList.GetItemText(selectedItem);
		SendCommand(this,CMD_TPS_SETMODALVISIBLE,sPara);
		
	}else
	{
		return;
	}
}



void CControlDlg::OnPopmenuZonegroupOrgan() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		sName=m_Prop3DList.GetItemText(selectedItem);
		
	}
	else
	{
		return;
	}

	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp == sName )
		{
			zoneGroup.zType=ORGAN;
			zoneGroup.color=RGB(0,255,0);
			zoneGroup.textColor=RGB(255,0,0);
			zoneGroup.textScale=15.0;
			
			m_tracerGrpList->SetAt(m_tracerGrpList->FindIndex(i),zoneGroup);
			break;
		}
	}
}

void CControlDlg::OnPopmenuZonegroupTumor() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		sName=m_Prop3DList.GetItemText(selectedItem);
		
	}
	else
	{
		return;
	}

	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp == sName )
		{
			zoneGroup.zType=TUMOR;
			zoneGroup.color=RGB(255,255,0);
			zoneGroup.textColor=RGB(255,0,0);
			zoneGroup.textScale=15.0;
			m_tracerGrpList->SetAt(m_tracerGrpList->FindIndex(i),zoneGroup);
			break;
		}
	}
}

ZONETYPE CControlDlg::GetCurSelectedGroupType()
{
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return ORGAN;
	}

	CString sName;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		sName=m_Prop3DList.GetItemText(selectedItem);
		
	}
	else
	{
		return ORGAN;
	}

	ZONEGROUP_INFO zoneGroup;
	CString sTmp;
	POSITION pos;
	pos = m_tracerGrpList->GetHeadPosition();
	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		sTmp.Format("%s",zoneGroup.groupName);
		if(sTmp == sName )
		{
			return zoneGroup.zType ;
		}
	}

	return ORGAN;
}

void CControlDlg::OnPopmenuZonegroupSetcolor() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	COLORREF newColor;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		CXTColorDialog* pColorDlg = new CXTColorDialog(RGB(244,244,244), RGB(244,244,244),
			( (CPS_XT_SHOWHEXVALUE | CPS_XT_SHOW3DSELECTION)), this);
		
		if (pColorDlg->DoModal() == IDOK)
		{
			newColor = pColorDlg->GetColor();

			selectedItem=m_Prop3DList.GetSelectedItem();
			sName=m_Prop3DList.GetItemText(selectedItem);
			ZONEGROUP_INFO zoneGroup;
			CString sTmp;
			POSITION pos;
			pos = m_tracerGrpList->GetHeadPosition();
			for (int i=0;i<m_tracerGrpList->GetCount();i++)
			{
				zoneGroup = m_tracerGrpList->GetNext(pos);
				sTmp.Format("%s",zoneGroup.groupName);
				if(sTmp == sName )
				{
					zoneGroup.color=newColor;
					m_tracerGrpList->SetAt(m_tracerGrpList->FindIndex(i),zoneGroup);
					SendCommand(this,CMD_TPS_CHANGETRACERCOLOR,sName);
					SendCommand(this,CMD_TPS_RENDERREFRESHALL);
					break;
				}
			}
		}
		
		delete pColorDlg;		
		
	}
	else
	{
		return;
	}

	
}

void CControlDlg::OnPaint() 
{
	CPaintDC dc(this);
	funOnEraseBkgnd(&dc);

	//CAppDialog::OnPaint();

}


void CControlDlg::OnBmpLoadimage() 
{
	// TODO: Add your control notification handler code here
	OnPopupmenuFileOpen();
}

void CControlDlg::OnClearZone(int index)
{
	
	CList<CString,CString&> m_namelist;
	CList<HTREEITEM,HTREEITEM&> m_itemlist;
	ZONEGROUP_INFO zoneGroup;
	CString sTmp,sName;
	POSITION pos,pos1;
	pos = m_tracerGrpList->GetHeadPosition();
	HTREEITEM childItem,parent;

	for (int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		zoneGroup = m_tracerGrpList->GetNext(pos);
		
		parent=zoneGroup.treeItem;
		childItem = m_Prop3DList.GetChildItem(zoneGroup.treeItem);
		while(childItem!=NULL)
		{
			sName=m_Prop3DList.GetItemText(childItem);
			pos1=CGlobalManager::Manager()->m_PolyInfoList.GetHeadPosition();
			for(int j=0;j<CGlobalManager::Manager()->m_PolyInfoList.GetCount();j++)
			{
				POLY_INFO polyinfo=CGlobalManager::Manager()->m_PolyInfoList.GetNext(pos1);
				sTmp.Format("%s",polyinfo.sDscr);
				if(sTmp==sName)
				{
					if(polyinfo.imageIndex==index)
					{
						m_namelist.AddTail(sName);
						m_itemlist.AddTail(childItem);
					}
					break;
				}
			}
			childItem = m_Prop3DList.GetNextItem(childItem, TVGN_NEXT);
		}
	}
	pos=m_namelist.GetHeadPosition();
	pos1=m_itemlist.GetHeadPosition();
	for(i=0;i<m_namelist.GetCount();i++)
	{
		sName=m_namelist.GetNext(pos);
		childItem=m_itemlist.GetNext(pos1);

		SendCommand(this,CMD_TPS_DELETETRACE,sName);
		m_Prop3DList.DeleteItem(childItem);
	}
			
}

void CControlDlg::OnPopmenuProbeLineVisible() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_INTERLINEVISIBLE,sPara);
		
	}
}

void CControlDlg::OnSelectProbe(CString para)
{
	HTREEITEM childItem;
	childItem = m_Prop3DList.GetChildItem(m_probeItem);
	while(childItem!=NULL)
	{
		if(m_Prop3DList.GetItemText(childItem)==para)
		{
			m_Prop3DList.SelectItem(childItem);
			break;
		}
		childItem = m_Prop3DList.GetNextItem(childItem, TVGN_NEXT);

	}
}

void CControlDlg::OnSelectTracer(CString para)
{
	POSITION pos;
	ZONEGROUP_INFO groupinfo;
	HTREEITEM childItem;
	BOOL bFind=FALSE;

	pos=m_tracerGrpList->GetHeadPosition();
	for(int i=0;i<m_tracerGrpList->GetCount();i++)
	{
		groupinfo=m_tracerGrpList->GetNext(pos);
		childItem = m_Prop3DList.GetChildItem(groupinfo.treeItem);
		while(childItem!=NULL)
		{
			if(m_Prop3DList.GetItemText(childItem)==para)
			{
				m_Prop3DList.SelectItem(childItem);
				bFind=TRUE;
				break;
			}
			childItem = m_Prop3DList.GetNextItem(childItem, TVGN_NEXT);
		}
		if(bFind)
			break;
	}

}

void CControlDlg::OnSelectModal(CString para)
{
	HTREEITEM childItem;
	childItem = m_Prop3DList.GetChildItem(m_modalItem);
	while(childItem!=NULL)
	{
		if(m_Prop3DList.GetItemText(childItem)==para)
		{
			m_Prop3DList.SelectItem(childItem);
			break;
		}
		childItem = m_Prop3DList.GetNextItem(childItem, TVGN_NEXT);

	}
}

void CControlDlg::OnPopmenuZoneTrackposition() 
{
	// TODO: Add your command handler code here
	if(m_bTrackTracer)
		m_bTrackTracer=FALSE;
	else
		m_bTrackTracer=TRUE;
}


void CControlDlg::OnPopmenuZonegroupSetdefault() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	HTREEITEM selectedItem;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	CString sName;
	if(m_Prop3DList.GetItemText(parent)=="Tracer")
	{
		
		selectedItem=m_Prop3DList.GetSelectedItem();
		m_Prop3DList.SetItemImage(m_defaultItem,3,3);
		m_Prop3DList.SetItemImage(selectedItem,5,5);
		m_defaultItem=selectedItem;

		sName=m_Prop3DList.GetItemText(selectedItem);
		SendCommand(this,CMD_TPS_SETDEFAULTGROUP,sName);
		
	}
}

void CControlDlg::OnPopmenuProbeUcs() 
{
	// TODO: Add your command handler code here
	
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		
		SendCommand(this,CMD_TPS_PROBE_SETUCS,sPara);
		
	}
}

void CControlDlg::OnPopmenuProbeCopyucs() 
{
	// TODO: Add your command handler code here
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		
		SendCommand(this,CMD_TPS_PROBE_COPYUCS,sPara);
		
	}
}


void CControlDlg::ShowProbeControl()
{
	if(m_probeControl.IsWindowVisible())
		return;

	HideDlgAutoTrace();

	CRect rs;
	GetClientRect(rs);
	CRect rsSelf;
	m_probeControl.GetClientRect(rsSelf);
	m_probeControl.MoveWindow(0,rs.Height()-rsSelf.Height(),rs.Width(),rsSelf.Height());
	m_probeControl.ShowWindow(SW_SHOW);
	int iHeight=rsSelf.Height();

	m_Prop3DList.GetWindowRect(rsSelf);
	ScreenToClient(&rsSelf);
	m_Prop3DList.MoveWindow(rsSelf.left,rsSelf.top,rsSelf.Width(),rsSelf.Height()-iHeight);
}

void CControlDlg::HideProbeControl()
{
	if(!m_probeControl.IsWindowVisible())
		return;

	CRect rs;
	m_probeControl.ShowWindow(SW_HIDE);
	GetClientRect(rs);
	m_Prop3DList.MoveWindow(2,60,rs.Width()-4,rs.Height()-64);
}

void CControlDlg::ShowDlgAutoTrace()
{
	if(m_pDlgAutoTrace.IsWindowVisible())
		return;

	HideProbeControl();

	CRect rs;
	GetClientRect(rs);
	CRect rsSelf;
	m_pDlgAutoTrace.GetClientRect(rsSelf);
	m_pDlgAutoTrace.MoveWindow(0,rs.Height()-rsSelf.Height(),rs.Width(),rsSelf.Height());
	m_pDlgAutoTrace.ShowWindow(SW_SHOW);
	int iHeight=rsSelf.Height();

	m_Prop3DList.GetWindowRect(rsSelf);
	ScreenToClient(&rsSelf);
	m_Prop3DList.MoveWindow(rsSelf.left,rsSelf.top,rsSelf.Width(),rsSelf.Height()-iHeight);
			
}

void CControlDlg::HideDlgAutoTrace()
{
	if(!m_pDlgAutoTrace.IsWindowVisible())
		return;

	CRect rs;
	m_pDlgAutoTrace.ShowWindow(SW_HIDE);
	GetClientRect(rs);
	m_Prop3DList.MoveWindow(2,60,rs.Width()-4,rs.Height()-64);
			
}


void CControlDlg::OnButtonMenuFile() 
{
	// TODO: Add your control notification handler code here
	CMenu menu;
	menu.LoadMenu(IDR_MAINMENU);
	CRect rc;
	m_btnMenuFile.GetWindowRect(&rc);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,rc.left,rc.bottom,this);
}

BOOL CControlDlg::PreTranslateMessage(MSG* pMsg) 
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
	return CAppDialog::PreTranslateMessage(pMsg);
}

LONG CControlDlg::GetParentBK(WPARAM w,LPARAM l)
{
	return OnGetParentBK(w,l);
}


void CControlDlg::OnSize(UINT nType, int cx, int cy) 
{
	CAppDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(m_Prop3DList.GetSafeHwnd())
	{
		m_Prop3DList.MoveWindow(2,60,cx-4,cy-64);
	}
}

void CControlDlg::OnPopmenuProbeMpr() 
{
	// TODO: Add your command handler code here
	
	HTREEITEM parent;
	if(m_Prop3DList.GetSelectedItem()!=TVI_ROOT)
	{
		parent=m_Prop3DList.GetNextItem(m_Prop3DList.GetSelectedItem(),TVGN_PARENT);
	}else
	{
		return;
	}

	
	if(m_Prop3DList.GetItemText(parent)=="Probe")
	{
		CString sPara;
		sPara=m_Prop3DList.GetItemText(m_Prop3DList.GetSelectedItem());
		SendCommand(this,CMD_TPS_MPR_PROBE,sPara);
		
	}
}

