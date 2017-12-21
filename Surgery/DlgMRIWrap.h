#if !defined(AFX_DLGMRIWRAP_H__1D73FF34_8500_439D_8AB2_88233750BD90__INCLUDED_)
#define AFX_DLGMRIWRAP_H__1D73FF34_8500_439D_8AB2_88233750BD90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMRIWrap.h : header file
//

#include "header.h"
#include "Geometry.h"
#include "ImriUlSrv.h"
#include "IGTImageHeader.h"
#include "DlgMriScanOptions.h"
#include "GradientCorrection.h"
class CDlgMain;
class CMRISeries;



/////////////////////////////////////////////////////////////////////////////
// CDlgMRIWrap dialog

class CDlgMRIWrap : public CDialog, public CVImriUlServerApp
{
// Construction
public:
	void ChangeProtocolFamily(IMRI_PROTOCOLGROUP_DESC proFamily);
	CDlgMRIWrap(CWnd* pParent = NULL);   // standard constructor
	~CDlgMRIWrap();

	BOOL InitMRI();
	BOOL ConnectMRI();

	void SendMRIScannerLocalization(Vector3D& vCenter, Vector3D& vPhaseDir, Vector3D& vFreqDir);

	// Send IMRI command with protocol V2
	int  SendIMRICommand(ENUM_COMMANDS eCommands, IMRI_PROTOCOL *pProtocol = NULL);

	BOOL InsertImgToDB(CMRISeries *pMRISeries);

	
	BOOL m_bMRIConneted;

	BOOL m_bEnableMRICtrl;

	CString m_sCurModalityID;
	IGT_PatientField m_CurPatient;
	IMRI_STUDY m_CurStudy;
	int m_iCurPatientUID;

	BOOL m_bProtocolsReceived;
	CList<IMRI_PROTOCOL_FAMILY, IMRI_PROTOCOL_FAMILY> m_lstProtocolFamily;

	// Parmeters used for communication with protocol V2
	CImriUlSrv* m_pImriSrv;

	CDlgMain* m_pDlgMain;

	
	ENUM_COMMANDS m_hSendingCommand;
	int m_iScanCount; //扫描计数,扫描若干次后,进行匀场情况测量


// Dialog Data
	//{{AFX_DATA(CDlgMRIWrap)
	enum { IDD = IDD_DLG_MRI_WRAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMRIWrap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMRIWrap)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnImriULMsg(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void ConvertCoordFromDevice2Patient(BYTE iPatPos, BYTE iPatEntry,double *vCenter, double  *vPhaseDir, double *vFreqDir);
	double GetAngle(Vector3D &v1,Vector3D &v2);
	void CalcDirection(MRIScanOptions *pScanOption,Vector3D& vPhaseDir,Vector3D& vFreqDir);

	BOOL InitWrap();
	
	BOOL m_bInitMRIConnection; 

	int m_iCurStudyUID;

	// Following functions implement interface of CVImriUlServerApp
	int	 Imri_DecodeImages(LPBYTE lpBuff, int nDataLen);
	int  Imri_DealCriticalMsg(LPBYTE lpBuff);
	int  Imri_DealConfirmMsg(LPBYTE lpBuff);
	int  Imri_DealWarningMsg(LPBYTE lpBuff);
	int  Imri_DealErrorMsg(LPBYTE lpBuff);
	int  Imri_DealStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
		unsigned short nSeriesStatus, int nOperResult);
	int  Imri_DealProtocolGrps(enum ENUM_INFOR_RESULT eResult, CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*> &lstprotocolGrp);
	int  Imri_DealProtocolFamily(ENUM_INFOR_RESULT eRet, CList<IMRI_PROTOCOL_FAMILY, IMRI_PROTOCOL_FAMILY> &lstprotocolGrp);
	int  Imri_DealModality(enum ENUM_INFOR_RESULT eResult, IMRI_MODALITY &modality);
	int  Imri_DealPatient(enum ENUM_INFOR_RESULT eResult, IMRI_PATIENT &patient);
	int  Imri_DealStudy(enum ENUM_INFOR_RESULT eResult, IMRI_STUDY &study);
	int  Imri_DealShimSNR(ENUM_INFOR_RESULT eRet, IMRI_SHIM_SNR_TUNING_INFO *pShimSNR);
	// end of CVImriUlServerApp

	BOOL m_bEnableGradCorrect;
	CString m_sGradFile;
	CGradientCorrection m_GradCorrecter;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRIWRAP_H__1D73FF34_8500_439D_8AB2_88233750BD90__INCLUDED_)
