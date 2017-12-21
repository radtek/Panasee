#if !defined(AFX_TPS2DVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_)
#define AFX_TPS2DVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPS2DView.h : header file
//
#include "resource.h"
#include "VtkHeader.h"
#include "TPSGlobalConst.h"
#include "TpsCryoProbe.h"
#include "FrmMgr.h"
#include "EvaluateDlg.h"
#include "MRISeries.h"
#include "ExtendVTKClass.h"

#include "vtkMarchingCubes.h"
#include "DlgTPSViewSkin.h"
#include "OleDropTargetEx.h"

/////////////////////////////////////////////////////////////////////////////
// CTPS2DView dialog
 
class  CTPS2DView : public CDialog,public CDlgTPSViewSkin
{
// Construction
public:
	void EventOnChangeSlice();
	void AddRealProbe(CProbeCryo *probe);
	
	vtkImageData *GetImage()
	{
		return m_pImageData;
	}
	
	void DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara);
	static void WheelCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void MoveMouseCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void MouseDownCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void KeyReleaseCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void KeyPressCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void RightButtonDownCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	
	CTPS2DView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPS2DView();
	
	int m_instanceIndex;
	IMAGE_INFO m_imageInfo;
	vtkCollection *m_pMeasureList;
	
	//vtkImageViewer2	*m_pViewer;
	CExtendImageView2 *m_pViewer;	
	
	BOOL m_bDisplayMeasure;
	vtkMatrix4x4* m_pImageFrame_scanner; //Image Frame in scanner frame
	double m_dZRange;
		
	CBox *m_pBox;
	vtkActor *m_pTraceActor;
	int m_bAutoTrace;
	int m_bBeginAutoTrace;
		
	CRgn m_nonClientRegion;//客户区域减去被按钮覆盖的部分

	void ChangeSlice(int iNum,BOOL bTriggerEvent=TRUE);
	void SetFrmMgr(CFrmMgr* pFrmMgr){m_pFrmMgr = pFrmMgr;}
	
	BOOL LoadSeries(CString fileName);
	BOOL LoadSeries(CMRISeries *pSeries);
	vtkRenderer* GetRenderer() { ASSERT(m_pRenderer); return m_pRenderer; }
	
	void OnCreateMeasureTool(CString sPara,double* pos=NULL);
	void SendCommand(int nCmdID,CString& sPara);

	CMRISeries *m_pSeries;

// Dialog Data
	//{{AFX_DATA(CTPS2DView)
	enum { IDD = IDD_DIALOG_2DVIEW };
	CImageButton	m_btn3DImage;
	CImageButton	m_btnLastImage;
	CImageButton	m_btnNextImage;
	CImageButton	m_btnPreImage;
	CImageButton	m_btnTopWndShow;
	CImageButton	m_btnFirstImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPS2DView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTPS2DView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopmenu2dviewDisplayaverage();
	afx_msg void OnPopupmenu2dviewDisplay2dtop();
	afx_msg void OnPopupmenuDisplayMax();
	afx_msg void OnPopupmenuDisplayDisplaymeasure();
	afx_msg void OnPopupmenuDisplayDeletemeasure();
	afx_msg void OnPopupmenuMeasureDis();
	afx_msg void OnPopupmenuMeasureAngle();
	afx_msg void OnPopupmenuDisplay2dresetview();
	afx_msg void OnPopupmenuDisplayResetwindowlevel();
	afx_msg void OnButtonFirstImage();
	afx_msg void OnButtonPreImage();
	afx_msg void OnButtonNextImage();
	afx_msg void OnButtonLastImage();
	afx_msg void OnButton3dImage();
	afx_msg void OnButtonTopwndShow();
	afx_msg void OnPopupmenuDisplaySegent();
	afx_msg void OnPopupmenuDisplayContour();
	afx_msg LONG OnContourUpdate(WPARAM w,LPARAM l);
	afx_msg LONG OnSegmentUpdate(WPARAM w,LPARAM l);
	afx_msg LONG OnVolumeRenderUpdate(WPARAM w,LPARAM l);
	afx_msg void OnPopupmenuDisplayVolume();
	afx_msg LRESULT OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	afx_msg LRESULT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	afx_msg LRESULT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);
	afx_msg LRESULT OnReceiveSeries(WPARAM wParam, LPARAM lParm);
	afx_msg LRESULT OnProbeMoving(WPARAM wParam, LPARAM lParm);
	afx_msg void OnPopupmenuDisplayMpr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	COleDropTargetEx m_dropEx;
	//处理OnDropEx消息是必须的，否则OnDrop函数不会被执行
	//当然也可以在OnDropEx函数中处理数据
	
private:
	BOOL InitImage(CMRISeries* pMRISeries);
	void ChangeAutoTraceStatus(CString sPara);
	void SetAutoTraceFilter(CString &sPara);
	void SetAutoTraceActor();
	
	
	
	void OnExtractContour(CString sPara);
	double GetImageAngle(CMRISeries *pMRISeries,BOOL &bHFlip,BOOL &bVFlip);
	
	
	vtkTextActor* m_pOriActor[4];
	
	CFrmMgr* m_pFrmMgr;
	
	void SetImageDisplayEnv();
	void SwitchTracerON();
	void SwitchTracerOFF();
	int SetupImageViewer();
	
	
	void RemoveAll();
	
	void CreateTracer();

	vtkContourFilter *m_pTraceFilter;
	vtkPolyData *m_pTracePolydata;	

	vtkRenderer            *m_pRenderer;
	vtkMFCWindow          *m_pMFCWindow;
	vtkRenderWindowInteractor *m_pInteractor;
	vtkCornerAnnotation	* m_pCorAnnotate;	
	
	vtkAxisActor2D* m_pRuler;	
	
	vtkCellPicker * m_pPicker;
	vtkPropPicker * m_pPropPicker;
	vtkWorldPointPicker *m_pointPicker;
	
		
	vtkImageData* m_pImageData;
	double m_dFirstImageCenter[3];
		
	vtkContourWidget *contourWidget;

	double m_viewFocus[4], m_viewPoint[3];
	
	int m_iUseAutoTrace;

	CMultiUsedObject *m_pImagesMemoryObject;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPS2DVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_)
