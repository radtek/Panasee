// Probe.h: interface for the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBE_H__6E84C28E_C551_4D09_8C5D_D9FC2E58FBE8__INCLUDED_)
#define AFX_PROBE_H__6E84C28E_C551_4D09_8C5D_D9FC2E58FBE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "APIStructures.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Ellipsoid.h"
#include "MV.h"
#include "MRISeries.h"
#include "MRIImage.h"
#include "MRIRectSeries.h"
#include "Line3D.h"
#include "Segment.h"

class CTrackTool;
class CVirtualTarget;

typedef enum ProbeCreateType
{
	PCT_REAL = 0,
	PCT_VIRTUAL
} PROBECREATETYPE;

#define PREPROBE_NAME	"Prescan"
#define PREPROBE_INDEX	0

//typedef enum ProbeType
//{
//	PROBETYPE_S_NEEDLE_17G = 0,
//	PROBETYPE_B_SPHERE_17G,
//	PROBETYPE_PROBE_2MM,
//	PROBETYPE_PROBE_3MM,
//	PROBETYPE_PROBE_TOUCH,
//	PROBETYPE_NUM
//} PROBETYPE;


#define EXTEND_LENGTH 300

#define PROBE_DISPLAY_CYLINDER	FALSE

class CProbe  
{
public:
	CProbe();
	virtual ~CProbe();

	// Set and get virtual target connected with this probe
	void SetVirtualTarget(CVirtualTarget *pVTarget);
	CVirtualTarget* GetVirtualTarget();
//	void DisconnectVirtualTarget();

	Vector3D GetVFromVTarget2Tip(); // Get vector from probe tip to virtual target
	double GetDistFromTip2VTarget();

	BOOL IsShowConnectLineToVTarget();
	void SetShowConnectLineToVTarget(BOOL bShow);

	void ShowDistFromTipToVTarget();

	void SetRadius(double dbRadius);
	double GetRadius();

	void SetLength(double dbLength);
	double GetLength();
	
	void SetColor(COLORREF cfColor);
	COLORREF GetColor();

	void SetTipInProbe(double x, double y, double z);
	Vector3D GetTipInProbe();   // Get tip position in probe coordinate

	void SetOrientationInProbe(double Vx, double Vy, double Vz);
	Vector3D GetOrientationInProbe();

	void UpdateIceBallPos();
	

	Vector3D GetOrientationInTrack();
	void SetOrientationInTrack(Vector3D vOri);

	Matrix GetMarkerOrientationInTrack();

	Vector3D GetRealTipInTrack();
	Vector3D GetVirtualTipInTrack();   // Get tip position in world coordinate
	void SetVirtualTipInTrack(Vector3D vTip);


	Matrix UpdatePosOri(Matrix *quatOrigin = NULL, Matrix mTransfer = Matrix(4,4));

	void SetVisible (BOOL bVisible);
	BOOL IsVisible();

	void SetIndex(int iIndex);
	int GetIndex();
	
	CString GetName();
	void SetName(CString sName);

	void EnableSimulate(BOOL bSimulate);
	BOOL IsSimulating();

	void SetMaxSphereRadius(double dbRadius);
	double GetMaxSphereRadius();

	void SetSphereTransparency(double dbTransparency); /* dbTransparency should between 0 to 1 */
	double GetSphereTransparency();

	BOOL SetProbePar(OPERATIONTYPE iOPERType, CString sProbeName);
//	PROBETYPE GetProbeType() { return m_iProbeType;};

	BOOL SetProbeCreateType(PROBECREATETYPE iProbeCreateType);
	PROBECREATETYPE GetProbeCreateType() { return m_iProbeCreateType;};
	
	void UpdateDisplay();
	void UpdateLineFromVTarget2Tip();

	void SetExtendLine(double dbExtLength);

	void SetOffset(double dbOffset);
	double GetOffset() {return m_dbLocalizationOffset;};

	void SetIceBall0(double dbLAxis, double dbSAxis, double dbOffset);
	void SetIceBall20(double dbLAxis, double dbSAxis, double dbOffset);
	void SetIceBall40(double dbLAxis, double dbSAxis, double dbOffset);

	void EnableControlMRIScanner(BOOL bControl);
	BOOL IsControllingMRIScanner() {return m_bControlMRIScanner;};

	void SaveLastScanPar(Vector3D vTipInScan, Vector3D vOriInScan, Matrix mTranNeedle2Scan,int iLastOffset);
	BOOL IsLastScanOK();
	void GetLastScanPar(Vector3D &vTipInScan, Vector3D &vOriInScan, Matrix &m_mTranNeedle2Scan);
	int  GetLastOffset(){ return m_iLastOffset; };

	void Serialize(CArchive& ar);		

	CCylinder m_cylBody;
	CCylinder m_cylExtednLine;
	CCylinder m_cylOffset;

	CSphere m_tipSphere;
	Line3D  m_line3D;   // Orientation line
	CEllipsoid *m_iceBall0;	
	CEllipsoid *m_iceBall20;		
	CEllipsoid *m_iceBall40;
	ST_TEXT   m_stLabel;    // probe label
	CSegment m_lineToVTarget;   // Line to virtual target

	CSegment m_sgBody;
	CSegment m_sgExtendLine;
	CSegment m_sgOffset;

	Matrix m_mProbeMarker2Tracker;

	CTrackTool* m_pTrackTool;

	//Matrix m_mTip2Probe;
private:

	Vector3D m_vTipInProbe;
	Vector3D m_vOrientationInProbe;
	Vector3D m_vVirtualTipInTrack;			// This is probe tip with virtual offset
	Vector3D m_vOrientationInTrack;
	Vector3D m_vProbeCenter2World;

	Vector3D m_vRealTipInTrack;		// This is real probe tip in tracking coordinates

	// Used to for record last scan position
	Vector3D m_vTipInScanner;
	Vector3D m_vOriInScanner;
	Matrix   m_mTranNeedle2Scanner;
	BOOL	 m_bLastScanOK;
	int      m_iLastOffset;

	double m_dbProbeLength;
	double m_dbProbeRadius;

	BOOL m_bVisible; // Default is TRUE;
	int  m_iIndex;   // Index of this probe;
	CString m_sName; // Name of this probe;

	BOOL m_bSphereSimulating; // Simulate cryotherapy sphere growing
	double m_dbMaxSphereRadius; // Max radius used to simulate cryotherapy ball growing
	double m_dbSphereTransparency; // Transparency during simulating

//	PROBETYPE m_iProbeType;
	PROBECREATETYPE m_iProbeCreateType;

	double m_dbIceBallOffset0;  // Offset to probe tip
	double m_dbIceBallOffset20;
	double m_dbIceBallOffset40;

	CVirtualTarget* m_pVirtualTarget;
	BOOL  m_bShowLineToVTarget; // show or hide line to virtual target
	double m_dbDistVTarget2Tip; // distance from virtual target to tip

	BOOL  m_bControlMRIScanner; // use this probe to control MRI scanner

	double   m_dbLocalizationOffset;
	double   m_dbExtLength; //延长线长度

	OPERATIONTYPE m_eOperType;		// operation type
	CString		m_sProbeTypeName;	// probe type name, eg. 17g

public:
	void UpdateCylinder();
	void UpdateDisplayLine();

};

#endif // !defined(AFX_PROBE_H__6E84C28E_C551_4D09_8C5D_D9FC2E58FBE8__INCLUDED_)
