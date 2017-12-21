// GlobalManager.h: interface for the CGlobalManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALMANAGER_H__B4E6C2DB_1ED5_4FF5_A22C_41C9EFD4C5E9__INCLUDED_)
#define AFX_GLOBALMANAGER_H__B4E6C2DB_1ED5_4FF5_A22C_41C9EFD4C5E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VtkHeader.h"
#include "FrmMgr.h"
#include "tpsGlobalConst.h"
#include "TpsCryoProbe.h"
#include "Modal.h"
#include "TPS2DView.h"
#include "TPS3DView.h"
#include "Prop3DTree.h"

class CGlobalManager  
{
public:
	void DeleteVolumeModalByImage(vtkImageData *pImage);
	CProbeCryo* GetRealProbe();
	void SetDefaultTracerGroup(CString sPara);

	void SetLineVisible(CString sPara);
	void InterLineVisibleStatus(CString para);

	void BallVisibleStatus(CString para);
	void SetBallVisible(CString para);

	void SelectModal(CString para);

	BOOL CreateTracer(POLY_INFO &polyinfo,vtkPolyData *pData,CTPS2DView *p2DView,BOOL bTransform=TRUE);
	void SelectTracer(CString para,BOOL bTrack);
	void DeleteTrace(CString para);
	void ChangeTracerColor(CString sPara);

	BOOL SaveTPSInfoToFile(CString fileName);
	BOOL LoadTPSInfoFile(CString fileName);

	void PlaneIntersection(CProbeCryo* pCrypProbe,CTPS2DView *p2DView=NULL);
	void AddAssemblys(vtkProp3D *pAssemblys, POLYTYPE ePolyType);
	void ReleaseAssemblys(POLYTYPE ePolyType);

	CProbeCryo* GetProbeByName(CString sName);
	void ChangeProbePosition(double *pos, int status);
	void CreateProbe();
	void SelectProbe(CString para);
	void DeleteProbe(CString para);
	void ReadProbeInfo();
	void ReplaceProbe(double dpos[], double dori[]);

	void AssemblyPickableOff();
	void AssemblyPickableOn();
	BOOL IsLocateProbeTail();
	BOOL IsLocateProbeTip();
	void LocateProbeNone();
	void LocateProbeTail();
	void LocateProbeTip();

	CGlobalManager();
	virtual ~CGlobalManager();
	static CGlobalManager* Manager(void);
	void DispatchCommand(CWnd *pSender, int nCmdID,CString& sPara);

	vtkProp3DCollection *m_pProbeOnDrag;
	vtkProp3DCollection *m_pTracerOnDrag;
	vtkProp3DCollection *m_pModalOnDrag;

	vtkProp3DCollection * m_pLabels;
	vtkProp3DCollection *m_pAssemblys;		
	vtkPolyDataCollection* m_pZonePoly;

	CList<NEEDLE_INFO,NEEDLE_INFO&> m_bioProbeInfo;
	CList<NEEDLE_INFO,NEEDLE_INFO&> m_cryoProbeInfo;
	CList<ZONEGROUP_INFO,ZONEGROUP_INFO&> m_tracerGrpList;	
	CList<POLY_INFO,POLY_INFO&> m_PolyInfoList;
	CList<ACTOR_INFO,ACTOR_INFO&> m_ActorInfoList;
	CList<CModal*,CModal*> m_modalGrpList;
	
	ZONEGROUP_INFO m_defaultTracerGroup;

	CFrmMgr *m_pFrmMgr;
	CTPS2DView *m_2DView1;
	CTPS2DView *m_2DView2;
	CTPS2DView *m_2DView3;
	CTPS3DView *m_3DView;

	CProp3DTree* m_pTreeCtrl;

	int m_iTraceCount; //Create name for zone label by this variable
	int m_iProbeCount;

	BOOL m_bCanAddNewTrace;
	CString m_sTpsFileLoad,m_sTpsFileLoaded;
private:
	void CopyFolder(CString src,CString sDest,CString sExt);
	int GetPolyDataCenter(vtkPolyData* pPolyData,double dCenter[3]);
	void CreateProbe(double pos[], double Ori[], NEEDLE_INFO eNeedleType,CString probeName="");
	void SendCommand(int nCmdID,CString& sPara);
	int m_locateProbe;

	CWnd *m_pPrivateWnd;
	POLY_INFO m_defaultPolyInfo;

	CProbeCryo *m_pRealProbe;
};

#endif // !defined(AFX_GLOBALMANAGER_H__B4E6C2DB_1ED5_4FF5_A22C_41C9EFD4C5E9__INCLUDED_)
