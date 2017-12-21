#if !defined(AFX_TPS3DVIEW_H__607A73EB_031F_4190_9D8B_200EFBDAD43A__INCLUDED_)
#define AFX_TPS3DVIEW_H__607A73EB_031F_4190_9D8B_200EFBDAD43A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPS3DView.h : header file
//
#include "resource.h"
#include "VtkHeader.h"
#include "TPSGlobalConst.h"
#include "ModalDrag.h"
#include "TpsCryoProbe.h"
#include "FrmMgr.h"
#include "ImageButton.h"
#include "DlgTPSViewSkin.h"
#include "modal.h"

/////////////////////////////////////////////////////////////////////////////
// CTPS3DView dialog

class CTPS3DView : public CDialog,public CDlgTPSViewSkin
{
// Construction
public:
	void AddRealProbe(CProbeCryo *probe);
	void AddVolume(CModal* pModal,vtkVolume* pVolume);
	void AddActor(CModal* pModalInfo,vtkActor* pActor);
	vtkActor *m_outline;
	vtkVolumePicker *m_pointPicker;
	void OnSelectImage(int i);
	void RemoveProbe(CProbeCryo *probe);
	void AddIceBall(vtkProp3D *pBall);
	void CreateModal(CModalDrag* pModalInfo, vtkPoints *pPoints);
	void DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara);
	CTPS3DView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPS3DView();
	vtkRenderer* GetRenderer() {  return m_pRenderer; }
	void RemoveAll();
	void RefreshPoly();
	
	int m_instanceIndex;
	BOOL m_bEnableFlyTo;
	
	vtkCornerAnnotation	*m_pCorAnnotate;	

	vtkProp3DCollection* m_pProbeActors;
	vtkRenderWindowInteractor *m_pInteractor;

	vtkImageActor *m_pImageActor1,*m_pImageActor2,*m_pImageActor3;
	vtkImageActor *m_pImageActorMPR1,*m_pImageActorMPR2;
		
	void SetFrmMgr(CFrmMgr* pFrmMgr){m_pFrmMgr = pFrmMgr;}

	BOOL m_bMeasureProbeTip[3];
	double m_Measure1[3],m_measure2[3];

	CRgn m_nonClientRegion;
	vtkMFCWindow          *m_pMFCWindow;
	BOOL m_bMouseDown;
	
	
// Dialog Data
	//{{AFX_DATA(CTPS3DView)
	enum { IDD = IDD_DIALOG_3DVIEW };
	CImageButton	m_btnTopWndShow;
	CImageButton	m_btnSelectImage;
	CImageButton	m_btnPreImage;
	CImageButton	m_btnNextImage;
	CImageButton	m_btnLastImage;
	CImageButton	m_btnFirstImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPS3DView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTPS3DView)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPopupmenu2dviewDisplay2dtop();
	afx_msg void OnPopupmenuDisplayMax();
	afx_msg void OnPopmenu2dviewDisplayaverage();
	afx_msg void OnPopupmenuDisplayRetcamera();
	afx_msg void OnPopupmenuView3dDispimage1();
	afx_msg void OnPopupmenuView3dDispimage2();
	afx_msg void OnPopupmenuView3dDispimage3();
	afx_msg void OnPopupmenuMeasureDis3d();
	afx_msg void OnButtonFirstImage();
	afx_msg void OnButtonPreImage();
	afx_msg void OnButtonNextImage();
	afx_msg void OnButtonLastImage();
	afx_msg void OnButtonSelectImage();
	afx_msg void OnButtonTopwndShow();
	afx_msg void OnPopupmenuView3dFlyto();
	afx_msg void OnPopupmenuView3dBkcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	BOOL IsOnImage(double *pos,int i);
	void OnProbeCopyUcs(CString para);
	void OnProbeSetUcs(CString para);
	void DisplayMeasureResult(CString sPara);
	void OnGetModalVisibleStatus(CString& sPara);
	void OnDeleteModal(CString sPara);
	void OnSetModalVisible(CString sPara);
	void OnCreateModal(CString sPara);
	void OnIsActor(CString &sPara);
	void SendCommand(int nCmdID,CString& sPara);
	static void MouseDownCallBack(vtkObject* obj, unsigned long, void*, void*);
	static void RightButtonDownCallBack(vtkObject *obj, unsigned long event , void *clientdata, void *calldata);
	static void KeyReleaseCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void KeyPressCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void RenderAbortCheckCallBack(vtkObject *obj, unsigned long event, void *clientdata, void *calldata);
		
	CFrmMgr* m_pFrmMgr;
	
	vtkRenderer            *m_pRenderer;
	
	vtkPropPicker * m_pPropPicker;
	
	vtkOrientationMarkerWidget* m_pOMW;
	
	void CreatCornnerAxes();
	void SetCallBack();
		
	int m_curImage;

	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPS3DVIEW_H__607A73EB_031F_4190_9D8B_200EFBDAD43A__INCLUDED_)
