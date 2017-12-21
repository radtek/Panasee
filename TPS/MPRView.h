#if !defined(AFX_MPRVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_)
#define AFX_MPRVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPRView.h : header file
//
#include "resource.h"
#include "VtkHeader.h"
#include "TPSGlobalConst.h"
#include "TpsCryoProbe.h"
#include "FrmMgr.h"
#include "ProbeControlDlg.h"
#include "EvaluateDlg.h"
#include "MRISeries.h"
#include "ExtendVTKClass.h"
#include "vtkImageReslice.h"
#include "GlobalManager.h"
#include "ImageButton.h"
#include "DlgMPRSkin.h"

/////////////////////////////////////////////////////////////////////////////
// CMPRView dialog
class  CMPRView : public CDlgMPRSkin
{
// Construction
public:
	void AddRealProbe(CProbeCryo *probe);
	void DeleteProbe();
	void AddProbe(CProbeCryo *probe);
	void Reslice(double* matrix);
	
	void DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara);
	static void MouseDownCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void KeyReleaseCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void KeyPressCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void WheelCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void MoveMouseCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	static void RightButtonDownCallBack(vtkObject* obj, unsigned long event, void* clientdata, void* calldata);
	
	CMPRView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMPRView();
		
	IMAGE_INFO m_imageInfo;

	CGlobalManager *m_pGlobalManager;
		
	//vtkImageViewer2	*m_pViewer;
	CExtendImageView2 *m_pViewer;
		
	BOOL m_bDisplayMeasure;
	
	int InitImage(vtkImageData* pImage,vtkMatrix4x4 *pMatrix);
		
	void SetFrmMgr(CFrmMgr* pFrmMgr){m_pFrmMgr = pFrmMgr;}
	vtkRenderer* GetRenderer() { ASSERT(m_pRenderer); return m_pRenderer; }
	void SendCommand(int nCmdID,CString& sPara);

	int m_instanceIndex;

// Dialog Data
	//{{AFX_DATA(CMPRView)
	enum { IDD = IDD_DIALOG_MPR };
	CImageButton	m_btn3DImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPRView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMPRView)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupmenuDisplayDisplaymeasure();
	afx_msg void OnPopupmenuDisplayDeletemeasure();
	afx_msg void OnPopupmenuMeasureDis();
	afx_msg void OnPopupmenuMeasureAngle();
	afx_msg void OnPopupmenuDisplay2dresetview();
	afx_msg void OnPopupmenuDisplayResetwindowlevel();
	afx_msg void OnButton3d();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
		
private:	
	void DisplayIn3D();
	void OnCreateMeasureTool(CString sPara,double* pos=NULL);
		
	double m_dResliceMatrix[16];  //mpr 切割矩阵
	vtkMatrix4x4* m_pOldImage2Scanner; //Image Frame in scanner frame
	vtkMatrix4x4* m_pNewImage2Scanner; //切割后的图像在scanner下的坐标，仅用于vtk显示使用
	double m_dNewImage2Scanner[4][4]; //切割后图像实际空间坐标
	
	CFrmMgr* m_pFrmMgr;
	void SetupDisplayEnv();
	int SetupImageViewer(vtkImageData* pImage);
		
	void RemoveAll();
		
	vtkRenderer            *m_pRenderer;
	vtkMFCWindow          *m_pMFCWindow;
	vtkRenderWindowInteractor *m_pInteractor;
	vtkCornerAnnotation	* m_pCorAnnotate;	
	
	vtkCollection *m_pMeasureList;
	
	vtkAxisActor2D* m_pRuler;
	
	
	vtkCellPicker * m_pPicker;
	vtkPropPicker * m_pPropPicker;
	vtkWorldPointPicker *m_pointPicker;
	
	vtkImageReslice *m_reslice;
	vtkImageData* m_pImageData;

	CProbeCryo* m_pProbe;

	double m_viewFocus[4], m_viewPoint[3];

	
			
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPRVIEW_H__21A07D46_9879_4365_8D94_76794DF1E104__INCLUDED_)
