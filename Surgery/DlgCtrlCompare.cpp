// DlgCtrlCompare.cpp : implementation file
//

#include "stdafx.h"
#include "surgery.h"
#include "DlgCtrlCompare.h"
#include "CompareFrame.h"
#include "DlgDispCompare.h"
#include "PanaMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCtrlCompare dialog

CDlgCtrlCompare::CDlgCtrlCompare(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCtrlCompare::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCtrlCompare)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bBinded = FALSE;
}


void CDlgCtrlCompare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCtrlCompare)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, ID_CMP_CTRL_ASSOCIATE, m_btnBind);
	DDX_Control(pDX, IDC_CMP_LIST_SERIES, m_ctlCmpMRIList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCtrlCompare, CDialog)
	//{{AFX_MSG_MAP(CDlgCtrlCompare)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_CMP_CTRL_ASSOCIATE, OnCmpCtrlAssociate)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_CMP_LIST_SERIES, OnBegindragCmpListSeries)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(DROPM_DRAGOVER,OnDragOver)
	ON_MESSAGE(DROPM_DROPEX,OnDropEx)
	ON_MESSAGE(DROPM_DROP,OnDrop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCtrlCompare message handlers

int CDlgCtrlCompare::InitMIRList(CStringArray& saMRIs,CStringArray& saDesc)
{
	m_ctlCmpMRIList.DeleteAllItems();

	for (int i=0; i<saMRIs.GetSize(); i++)
	{
		int item=m_ctlCmpMRIList.InsertItem(m_ctlCmpMRIList.GetItemCount(), saMRIs[i]);
		m_ctlCmpMRIList.SetItemText(item, 1, saDesc[i]);
	}
	return 0;
}

void CDlgCtrlCompare::Clean()
{
	m_ctlCmpMRIList.DeleteAllItems();
	if (m_bBinded)
	{
		OnCmpCtrlAssociate();
	}
}

void CDlgCtrlCompare::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if (m_ctlCmpMRIList.GetSafeHwnd())
	{
		int nLeft = 5;
		int nTop = 5;
		CRect rs;
		GetClientRect(rs);
		m_ctlCmpMRIList.MoveWindow(nLeft, nTop, rs.Width()-2*nLeft, rs.Height() - 200);
		
		nTop = rs.Height() - 200 + 30;
		nLeft = 20;
		CRect rsBtn;
		GetDlgItem(IDOK)->GetClientRect(&rsBtn);

		GetDlgItem(ID_CMP_CTRL_ASSOCIATE)->MoveWindow(nLeft, nTop, rsBtn.Width(), rsBtn.Height());
		nLeft = nLeft + rsBtn.Width() + 20;
		GetDlgItem(IDOK)->MoveWindow(nLeft, nTop, rsBtn.Width(), rsBtn.Height());
	}

	if(m_btnBind.GetSafeHwnd())
	{
		CRect rcTmp,rcClient;
		GetClientRect(rcClient);
		m_btnBind.GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.left=(rcClient.Width()-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH)*2+
			CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)))/2.0;
		rcTmp.right=rcTmp.left+CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH);
		rcTmp.bottom=rcTmp.top+CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_HEIGHT);
		m_btnBind.MoveWindow(rcTmp);
	}
	if(m_btnOK.GetSafeHwnd())
	{
		CRect rcTmp,rcClient;
		GetClientRect(rcClient);
		m_btnOK.GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);
		rcTmp.right=rcClient.right-(rcClient.Width()-(CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH)*2+
			CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_SEPRATE)))/2.0;
		rcTmp.left=rcTmp.right-CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_WIDTH);
		rcTmp.bottom=rcTmp.top+CPanaMetrics::GetMetrics(CPanaMetrics::PASK_METRICS_BTN_POS_HEIGHT);
		m_btnOK.MoveWindow(rcTmp);
	}
}

void CDlgCtrlCompare::OnOK() 
{
	// TODO: Add extra validation here
	m_pCmpFrm->PostMessage(WM_CLOSE);
	//m_pCmpFrm->ShowWindow(SW_HIDE);
	//m_pCmpFrm->Clean();

	//CDialog::OnOK();
}

void CDlgCtrlCompare::OnCmpCtrlAssociate() 
{
	// TODO: Add your control notification handler code here
	m_bBinded = !m_bBinded;

	if (m_bBinded)
	{
		Bind();
		SetDlgItemText(ID_CMP_CTRL_ASSOCIATE, "Unbind");
	}
	else
	{
		Unbind();
		SetDlgItemText(ID_CMP_CTRL_ASSOCIATE, "Bind");
	}
}

void CDlgCtrlCompare::Bind()
{
	m_pCmpFrm->m_DlgDisp.BindMRISeries(TRUE);
}

void CDlgCtrlCompare::Unbind()
{
	m_pCmpFrm->m_DlgDisp.BindMRISeries(FALSE);
}

BOOL CDlgCtrlCompare::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dropEx.Register(this);
	
	DWORD dwExStyle = LVS_EX_FULLROWSELECT ;
	m_ctlCmpMRIList.SetExtendedStyle(dwExStyle);
	m_ctlCmpMRIList.InsertColumn( 0, "Series", LVCFMT_LEFT, 100);
	m_ctlCmpMRIList.InsertColumn( 1, "Description", LVCFMT_LEFT, 160);
	m_ctlCmpMRIList.InitHeader();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DROPEFFECT CDlgCtrlCompare::OnDragOver(WPARAM pDropInfoClass, LPARAM lParm) //拖动到上面时执行
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));

	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

DROPEFFECT CDlgCtrlCompare::OnDropEx(WPARAM pDropInfoClass, LPARAM lParm)
{
	return (DROPEFFECT)-1;
}

BOOL CDlgCtrlCompare::OnDrop(WPARAM pDropInfoClass, LPARAM lParm)
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));

	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
	{
		HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData( CF_TEXT );
		char* lp = (char *)GlobalLock((HGLOBAL) hMem);//lock source
		if (lp != NULL)
		{
			//MessageBox(lp);			
		}
		GlobalUnlock( hMem );//unlock source

		return TRUE;
	}
	else
		return FALSE;

}

void CDlgCtrlCompare::OnBegindragCmpListSeries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctlCmpMRIList.GetFirstSelectedItemPosition();	
	if (pos)
	{
		int iItemIndex = m_ctlCmpMRIList.GetNextSelectedItem(pos);
		CString sName = m_ctlCmpMRIList.GetItemText(iItemIndex, 0);
		
		HGLOBAL hData = GlobalAlloc(GHND|GMEM_SHARE, sName.GetLength()+1 );
		char *pS = (char *)GlobalLock( hData );
		memcpy( pS, (LPCTSTR)sName, sName.GetLength()+1 );
		GlobalUnlock( hData );

		//m_bDraging = TRUE;	//开始拖动
		COleDataSource source;					  //用于启动一次拖放操作，并向系统提供拖放对象的数据
		source.CacheGlobalData( CF_TEXT, hData ); //使用即时方式提供数据，亦可用DelayRenderData指定延时方式提供数据
		source.DoDragDrop( DROPEFFECT_MOVE );     //开始实施拖放操作
		GlobalFree( hData );
		//m_bDraging = FALSE;	//完成拖动
	}
	*pResult = 0;
}

void CDlgCtrlCompare::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_CMP_LIST_SERIES)
		m_ctlCmpMRIList.MeasureItem(lpMeasureItemStruct);

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
