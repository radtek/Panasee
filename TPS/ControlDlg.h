#if !defined(AFX_CONTROLDLG_H__B7FC03AA_4DA9_4542_91E3_600D8E843B9C__INCLUDED_)
#define AFX_CONTROLDLG_H__B7FC03AA_4DA9_4542_91E3_600D8E843B9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlDlg.h : header file
//
#include "resource.h"
#include "DlgTPSSkin.h"
#include "PanaStatic.h"
#include "TPSGlobalConst.h"
#include "AppDialog.h"
#include "BtnST.h"
#include "modal.h"
#include "Prop3DTree.h"
#include "DlgAutoTrace.h"
#include "ImageButton.h"
#include "ProbeControlDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog


class CControlDlg : public CAppDialog,public CDlgTPSSkin
{
// Construction
public:
	CControlDlg(CWnd* pParent = NULL);   // standard constructor

	virtual ~CControlDlg();

// Dialog Data
	//{{AFX_DATA(CControlDlg)
	enum { IDD = IDD_DIALOG_CONTROL };
	CButtonST	m_btnMenuHelp;
	CButtonST	m_btnMenuFile;
	CProp3DTree	m_Prop3DList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LONG OnChangeTracerGroup(WPARAM w,LPARAM l); 
protected:

	// Generated message map functions
	//{{AFX_MSG(CControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickProp3dList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedProp3dList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopmenuZoneDelete();
	afx_msg void OnPopmenuZoneProp();
	afx_msg void OnPopmenuProbeBallVisible();
	afx_msg void OnPopmenuProbeDelete();
	afx_msg void OnPopmenuProbeProp();
	afx_msg void OnBmpEvaluate();
	afx_msg void OnBmpCreatetool();
	afx_msg void OnPopupmenuFileNew();
	afx_msg void OnPopupmenuFileOpen();
	afx_msg void OnPopupmenuFileSave();
	afx_msg void OnPopupmenuFileSaveas();
	afx_msg void OnBmpSaveimage();
	afx_msg void OnPopmenuZonegroupCreatemodal();
	afx_msg void OnPopmenuZonegroupDelete();
	afx_msg void OnPopmenuZonegroupNew();
	afx_msg void OnPopmenuZonegroupRename();
	afx_msg void OnEndlabeleditProp3dList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupmenuModalDelete();
	afx_msg void OnPopupmenuModalProp();
	afx_msg void OnPopupmenuModalRename();
	afx_msg void OnPopupmenuModalVisible();
	afx_msg void OnPopmenuZonegroupOrgan();
	afx_msg void OnPopmenuZonegroupTumor();
	afx_msg void OnPopmenuZonegroupSetcolor();
	afx_msg void OnPaint();
	afx_msg void OnBmpLoadimage();
	afx_msg void OnPopmenuProbeLineVisible();
	afx_msg void OnPopmenuZoneTrackposition();
	afx_msg void OnPopmenuZonegroupSetdefault();
	afx_msg void OnPopmenuProbeUcs();
	afx_msg void OnPopmenuProbeCopyucs();
	afx_msg void OnButtonMenuFile();
	afx_msg LONG GetParentBK(WPARAM w,LPARAM l);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopmenuProbeMpr();
	afx_msg void OnClickProp3dList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void HideProbeControl();
	void ShowProbeControl();
	void HideDlgAutoTrace();
	void ShowDlgAutoTrace();
	void OnSelectProbe(CString para);
	void OnSelectModal(CString para);
	void OnSelectTracer(CString para);
	void OnClearZone(int index);
	ZONETYPE GetCurSelectedGroupType();
	void OnCreateModalOver(CString sPara);
	void OnReconstructTracerGroup();
	void OnAddTracerToList(CString para);
	
	CImageButton m_btnSaveImage;
	CImageButton m_btnLoadImage,m_btnEvaluate,m_btnCreateTool;
	
	void OnClearTreeList();
	void DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara);
	void CreateProp3DTree();

			


	CImageList m_ilstProp3D;
	HTREEITEM m_probeItem,m_zoneItem,m_modalItem;

	HTREEITEM m_defaultItem;
	CString m_ballVisibleStatus;
	CString m_lineVisibleStatus;

	
	CList<ZONEGROUP_INFO,ZONEGROUP_INFO&> *m_tracerGrpList;
	CList<CModal*,CModal*> *m_modalGrpList;
	int m_editLabelStyle;
	CString m_editLabelOldName;

	
	//track position of tracer
	//when click this tracer, will change image to slice where tracer exists in 2d window
	BOOL m_bTrackTracer; 

	CDlgAutoTrace m_pDlgAutoTrace;
	CProbeControlDlg m_probeControl;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLDLG_H__B7FC03AA_4DA9_4542_91E3_600D8E843B9C__INCLUDED_)
