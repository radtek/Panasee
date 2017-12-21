//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_CRYOTHERAPYDLG_H__1422BB9C_6F38_4E93_B483_69343BA9C1D9__INCLUDED_)
#define AFX_CRYOTHERAPYDLG_H__1422BB9C_6F38_4E93_B483_69343BA9C1D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CryotherapyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCryotherapyDlg dialog
#include "MRIBuffer.h"
#include "MRIRectSeries.h"
#include "header.h"
#include "DlgScannerControl.h"
#include "DlgTrackingStatus.h"
#include "OleDropTargetEx.h"
#include "DlgCmdBar.h"
#include "DlgTitle.h"
#include "Probe.h"
#include "Restore.h"
#include "PopupMenu.h"  
#include "ProbePool.h"
#include "TargetPool.h"
#include "DlgTime.h"
#include "DlgPatient.h"
#include "DlgNavigate.h"
#include "DlgTps.h"
#include "DlgSeries.h"
#include "DlgMainSkin.h"
#include "ProtocolManager.h"
#include "DataPool.h"
#include "TrackerHandler.h"


#define MAX_BASE_HANDLES		4

#define FLAG_NONE				0
#define FLAG_PT_CALI			3


class CCommandHandling;
class CDispaly4Dlg;
class CDataManager;
class CProbe;
class CMRISeries;
class CTrackTool;
class CMesh;
class CDlgMain;


typedef struct base_handle
{
	int  bh_BaseIndex;
	int  bh_Handle;
	Matrix bh_mToBase0;
} BASE_HANDLE;

//定义了 导航模式 NAVI MODE
/*
	TABLE_NO_MARKER:图像、靶点、虚拟针、测量的坐标全部位于相机坐标系下
	TABLE_WITH_MARKER:图像、靶点、虚拟针、测量的坐标全部位于病床坐标系下
*/
enum PATIENT_MARKER 
{    
	PATIENT_NO_MARKER=0,  //病人身上没有导航marker,病人移动需要手工输入移动参数
	PATIENT_WITH_MARKER   //病人身上安装了导航marker,病人可任意移动
};	// For two modes of navigation // Added by Ningsj [2007.4.13]

/*定义了病床移动方式 Navi Mode
  GE的病床可以根据扫描序列自动移动病床，
  将序列中心移动到扫描中心，扫描完成生成
  图像数据时，进行调整，相当于没有移动病床
  时图像的位置。Navi mode
*/
enum TABLE_SHIFT_MODE 
{    
	NO_SHIFT_IN_SCANING=0,  //扫描前后病床无自动移动
	AUTO_SHIFT_IN_SCANING   //扫描前病床自动调整位置,扫描后图像位置信息自动补偿
};	// For two modes of navigation // Added by Ningsj [2007.4.13]

class CCryotherapyDlg : public CDlgMainSkin
{
// Construction
public:

	int NDITracking();

	CCryotherapyDlg(CWnd* pParent = NULL);   // standard constructor
	~CCryotherapyDlg();

	void SelTab(enum PASK_TabMode eTab);

	void TransformEverythingToFinalCor();

	// This function transform MRI images to final coordinates in order to display with probe
	void TransformMRISeriesToFinalCor(CMRISeries* pMRISeries);

	void SetOperType(OPERATIONTYPE nType);

	void TransformAllMRISeries();


	CMRISeries* GetMRISeries(CString sName,BOOL bFindInDatabase=TRUE);


	CDispaly4Dlg* m_pDisplay4Dlg;

	BOOL IsBaseCouldbeVisible(int iBaseIndex);
	
	BASE_HANDLE m_baseHandles[MAX_BASE_HANDLES];   // Base handles information

	void ImportBasePattern(CString szFileName);
	void ExportBasePattern(CString szFileName);

	void AddDatabaseResult(int iNum, CString csSutId[], int nSerId[], CString csMId[]);
	void AddDatabaseResult(int iNum, char csSutId[100][256], int nSerId[100], char csMId[100][256]);
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);

	void InsertMap(CString sKey, CString sData);

	void AddMRISeries(CString sPath, BOOL bUpdateList = TRUE);

	BOOL LoadMRISeries(CMRISeries* pMRISeries,BOOL bSave =TRUE);	
	void DeleteMRISeries(CString sMRIName);

	void UpdateMRIPosition();	// This is called to update MRI series position
	void UpdateFrameBoxPosition(); // This is called to update frame box position
	void InitFrameBoxPosition(); //Added by Dai Liang 2007.6.26

	void SaveTransformation(CString sPathFile, double** pValue);//Add by DaiLiang 2007/6/5

	void UserSimulateProbeIceBall(int iProbeID);
	void StopSimulateProbeIceBall(int iProbeID);


	void UpdateTextureSameSeries(MRIRectSeries *pMRIRectSeries,BOOL bUpdateAllImage);

	void UpdateMRIList(CMRISeries *pMRISeries);



	void InitTrackingTool();
	void CleanTrackingTool(bool bDetachFirst = false);
	
	void ChangeBaseTracking(int iPortHandle, int iBaseNumber);
	void ChangePhatomTrackingTo(int iPortHandle);					//Add by Dai Liang 2007.6.5
	int StartTracking();
	int StopTracking();
	void UpdateCurrentMRI(BOOL bUpdateRTWnd = TRUE);				//Modified by Dai Liang move from private functions.
	void CreateTransform();											// Use to create transform from scanner to tracker Add by Dai Liang 2007.6.11
	Vector3D GetPTMoveVectorInTracker(double dCurPTPos);
	BOOL ShowMRIImage(CMRISeries*,int,int=2);						//显示MRI图象
	BOOL ShowMRIImage(CMRIImage*,int);     
	bool LoadTransformation(CString sPathFile,double** pValue);		//Add by Dai Liang


	void DisplayMRISeriesToWindow(CMRISeries *pMRISeries, int iWndIndex, int iInitDispImgIndex = -1);  // iInitDispImgIndex: -1: display 0, -2: display former index
	BOOL LookupMap(CString sKey, CString &sRetData);

	void CameraMove();
	
	void CalibratePT();
	void StartCollectTrackingData(int iTrackingHandle, HWND wHandle);
	HWND m_wHandle;	// window handle to receive collection done message
	void StopCollectTrackingData();
	int	 m_nCollectNum, m_nCollectCnt;

	BOOL m_bInDataTracking;

	CMenu	m_mPopupMenu;
	
	CTrackerHandler *m_pTrackerHandler;
	
	UINT	m_uiFlag;
	int		m_iDataCollectionHandlerID;
	CList<double, double> m_lstTrackingData;
	CList<double, double> m_lstTestCollectPoint;

	// Tracking update rate
	int m_updateRate;

	CTrackTool**  m_ppTrackTool;
	int			  m_iMaxNumOfTrackTool;
	
	CMRISeries* m_pCurSelSeries;	// Point to selected displayed MRI series
	
	CMRIBuffer  m_MRIBuffer;	// Buffer to store MRI images
	CProbePool  m_ProbePool;	// Pool to store probes
	CDataPool	m_ProbeParPool;		// Probe parameter pool
	CTargetPool m_TargetPool;	// Pool to store virtual target
	CProtocolManager	m_ProtMng;	// Protocol manager
	
	MRIRectSeries m_MRIRectSeries[5];	// Showing rectangle series corresponding to window index
	
	BOOL	m_bPreSurgeryCaliDone;  // Determine whether pre-surgery calibration result is got or not
	
	
	int		m_iSelectHandle;  // record selected handle in probe list
	int     m_iProbeHandle;
	int		m_iPhatomHandle;	// Handle to tracking calibration phantom 
	int		m_iMarkerHandle;		// Handle to tracking patient table
	

	CString m_sMRIStorageFolder;
	
	
	CDlgTitle   m_dlgTitle;
	CDlgCmdBar  m_dlgCmdBar;
	CDlgPatient m_dlgPatient;
	CDlgSeries	m_dlgSeries;
	CDlgScannerControl m_dlgScannerControl;
	CDlgTime	m_dlgTime;
	CDlgTrackingStatus m_dlgTrackingStatus;
	CDlgNavigate m_dlgNavigate;
	CDlgTps      m_dlgTps;

	CDlgMain* m_pDlgMain;
	

	CList<double, double> m_lstPhantomCenterXYZ;  // Phantom center used to calculate distance from needle tip
	BOOL m_bModePreCali;
	PATIENT_MARKER m_patientMarker;	// For two modes of navigation
	TABLE_SHIFT_MODE m_TableShiftMode;  //Table mode

	HANDLE m_hTrackingStartEvent;			// event to indicate tracking started
	HANDLE m_hTrackingStopEvent;			// event to indicate tracking stoped

	BOOL m_bTracking;
	BOOL m_bStopTracking;
	int m_nGetTXTransformsErrorCount;
public: //Add by Dai Liang 2007/6/5
	//C_tumor_track=C_base_track*C_table_base*C_patient_table*C_scanner_patient*C_tumor_patient;
	//C_tumor_track = C_scanner_track*C_tumor_scanner
	//1 On site:C_scanner_track = C_base_track*C_scanner_base
	//2 Off site: C_scanner_track = C_table_track*C_scanner_table
	//3 Patient in Frame(assume that the frame is same as patient): C_scanner_track = C_frame_track*C_scanner_frame
	//All the reference frame are defined in the Tracker frame and presented in Matrix(4,4); 

	//All the tools coordiante are defined in the tracker frame and presented in Matrix(4,4);
	
	Matrix m_tPhatom;  //水模在相机下坐标
	Matrix m_tBase;    //当前选中base在相机下的坐标
	Matrix m_tBaseLast;
	Matrix m_tMarker;	   //Marker在相机下的坐标
	Matrix m_tProbe;

	BOOL m_bRecordBase;
	BOOL m_bPromptCameraMove;  //是否提示相机移动了

	//All the transform A to B are m_tranA2B;
	Matrix m_tranScanner2Base;		// Calibration result
	Matrix m_tranScanner2Tracker;
	Matrix m_tranScanner2Marker;
	
	double m_dbCurrentPTPos;        // current position of patient table which is used to calculate coordinate.
	double m_dbRealPTPos;           // real position of patient table from dialog
	Vector3D m_vPTDirInScan;		// patient table moving direction in scan coordinates, this is pre calibrated.
	Vector3D m_vPTMovInTracker;		// patient table move distance and direction in tracking coordinates

	bool LoadPatientTableDirFile(CString sPathFile);
	bool SavePatientTableDirFile(CString sPathFile);
	
	BOOL m_bScanningPosRecorded;	// Indicate patient table position at scanning is recorded or not
	BOOL m_bBaseVisible;
	BOOL m_bAllBaseVisible[MAX_BASE_HANDLES];
	BOOL m_bMarkerVisible;
	BOOL m_bProbeVisible;
	BOOL m_bPhantomVisible;
	

// Dialog Data
	//{{AFX_DATA(CCryotherapyDlg)
	enum { IDD = IDD_DLG_CRYOTHERAPY };
//	CListCtrl	m_ctlMRIList;
//	CListCtrl	m_ctlTrackList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryotherapyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:
public:
	void SetCurBaseIndex(int index);
	void RestoreTabDisplay();

	COleDropTargetEx m_dropEx;

	// Generated message map functions
	//{{AFX_MSG(CCryotherapyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LONG NDITracking( UINT wParam, LONG lParam );
	afx_msg LONG StopTrackingFromThread( UINT wParam, LONG lParam );
	afx_msg LONG SimulateSphere( UINT wParam, LONG lParam);
	afx_msg void OnPopup1ChangeImgCoord();
	afx_msg void OnProbeSimulatePopDlg();
	afx_msg void OnProbeSimulate();
	afx_msg void OnProbeStopSimulate();
	afx_msg void OnDisp2dOne();
	afx_msg void OnDisp2dFour();
	afx_msg void OnDisp2dThree();
	afx_msg void OnDisp2dTwo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	
	void DisplaySelectMRIImageToWindow(int iWndIndex);

	void ChangeWidthLevel(CMRISeries* pMRISeries);
//	void MRISeriesTransformation();



	long          m_lFrameNo[MAX_HANDLES];

	CMRISeries* m_pMRISeries;	// Point to latest received/opened MRI series
	
	// Used for display
	CList<int, int> m_lst3DDisp;

	void StartMRIPlayThread();

	CMapStringToString m_mapMRIName2Folder;

	
	BOOL m_bDlgInitialized;
	
	CRect m_rcTabPat, m_rcTabSeries, m_rcTabNavCtrl,m_rcTabMriCtrl,m_rcTabTpsCtrl;

	CRITICAL_SECTION m_csLoadSeries;

	PASK_TabMode m_tabMode;

	OPERATIONTYPE m_enumOperateType;

	int m_iCurBaseIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYOTHERAPYDLG_H__1422BB9C_6F38_4E93_B483_69343BA9C1D9__INCLUDED_)

//C_tumor_track=C_base_track*C_table_base*C_patient_table*C_scanner_patient*C_tumor_patient;
//C_tumor_track = C_scanner_track*C_tumor_scanner
//On site:C_scanner_track = C_base_track*C_scanner_base
//Off site: C_scanner_track = C_table_track*C_scanner_table
//Patient in Frame,assume that the frame is same as patient:
//C_scanner_track = C_frame_track*C_scanner_frame
