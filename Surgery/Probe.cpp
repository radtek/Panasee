// Probe.cpp: implementation of the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surgery.h"
#include "Probe.h"
#include "ConfigMgr.h"
#include "Intersection.h"
#include "ErrorCodes.h"
#include "Rectangle.h"
#include "VirtualTarget.h"
#include "GlobalFuncs.h"
#include "GlobalColor.h"
#include "DlgMain.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbe::CProbe()
{
	m_dbExtLength=EXTEND_LENGTH;
	SetRadius(1);
	SetLength(100);
	SetTipInProbe(0,0,0);
	SetOrientationInProbe(0,0,1);
	
	m_mProbeMarker2Tracker.SetDimension(4,4);
	m_mProbeMarker2Tracker.Identity();

	m_iceBall0 = NULL;
	m_iceBall20 = NULL;
	m_iceBall40 = NULL;

	m_dbIceBallOffset0 = 0.0;
	m_dbIceBallOffset20 = 0.0;
	m_dbIceBallOffset40 = 0.0;

	m_bSphereSimulating = FALSE;

	SetVisible(TRUE);
	m_tipSphere.SetVisible(FALSE);
	m_tipSphere.SetUsingTransparency(TRUE);
	CColorF color( 1.0f, 0.0f, 0.0f, 0.4f );
	m_tipSphere.SetColor(&color);

	CColorF* colorExt=CGlobalColor::GetColor()->GetProbeExt();
	CColorF* colorBody=CGlobalColor::GetColor()->GetProbeBody();
	CColorF* colorOffSet=CGlobalColor::GetColor()->GetProbeOffset();

	// Cylinder display
	m_cylBody.SetColorRef(RGB(colorBody->GetR()*255,colorBody->GetG()*255,
		                      colorBody->GetB()*255));
	m_cylExtednLine.SetColorRef(RGB(colorExt->GetR()*255,colorExt->GetG()*255,
		                        colorExt->GetB()*255));
	m_cylOffset.SetColorRef(RGB(colorOffSet->GetR()*255,colorOffSet->GetG()*255,
		                        colorOffSet->GetB()*255));

	// Segment display
	m_sgExtendLine.SetColor(colorExt);
	m_sgExtendLine.SetWidth(3.0);
	m_sgExtendLine.m_bRealLine=FALSE;
	m_sgBody.SetColor(colorBody);
	m_sgBody.SetWidth(3.0);
	m_sgOffset.SetColor(colorOffSet);
	m_sgOffset.SetWidth(3.0);
	
	m_dbLocalizationOffset = 0.0;



	m_pTrackTool = NULL;

	m_iProbeCreateType = PCT_REAL;

	m_pVirtualTarget = NULL;
	m_lineToVTarget.SetWidth(1.0);
	CColorF color2(1.0f, 0.0f, 0.0f, 0.4f);
	m_lineToVTarget.SetColor(&color2);
	SetShowConnectLineToVTarget(FALSE);
	m_dbDistVTarget2Tip = 0.0;

	m_bLastScanOK = FALSE;

	Matrix mInitMatrix(4,4);
	UpdatePosOri(&mInitMatrix);

	m_bControlMRIScanner = FALSE;

	m_sName = "Default";
	// m_iScanPlaneType = PLANE_MARKER_PLANE;
	
	//m_mTip2Probe.SetDimension(4,4);
	//m_mTip2Probe.Identity();
}

CProbe::~CProbe()
{
//	DisconnectVirtualTarget();
	if (m_iceBall0)
	{
		delete m_iceBall0;
		m_iceBall0 = NULL;
	}
	if (m_iceBall20)
	{
		delete m_iceBall20;
		m_iceBall20 = NULL;
	}
	if (m_iceBall40)
	{
		delete m_iceBall40;
		m_iceBall40 = NULL;
	}
}

void CProbe::SetRadius(double dbRadius)
{
	m_dbProbeRadius = dbRadius;

	m_cylBody.SetRadius(m_dbProbeRadius);
	m_cylExtednLine.SetRadius(m_dbProbeRadius);
	m_cylOffset.SetRadius(m_dbProbeRadius);
	m_tipSphere.SetRadius(10);
}

double CProbe::GetRadius()
{
	return m_dbProbeRadius;
}

void CProbe::SetLength(double dbLength)
{
	m_dbProbeLength = dbLength;

	m_cylBody.SetHalfHeight(m_dbProbeLength/2);
	m_cylExtednLine.SetHalfHeight(m_dbExtLength/2);
	m_cylOffset.SetHalfHeight(fabs(m_dbLocalizationOffset));
}

double CProbe::GetLength()
{
	return m_dbProbeLength;
}

void CProbe::SetColor(COLORREF cfColor)
{
	m_cylBody.SetColorRef(cfColor);

	CColorF color2(GetRValue(cfColor)/255.0, GetGValue(cfColor)/255.0, GetBValue(cfColor)/255.0, 0.4f);
	m_sgBody.SetColor(&color2);

}

COLORREF CProbe::GetColor()
{
	if (PROBE_DISPLAY_CYLINDER)
	{
		return m_cylBody.GetColorRef();
	}
	else 
	{
		CColorF* pColor = m_sgBody.GetColor();
		COLORREF color2 = RGB(pColor->GetR()*255, pColor->GetG()*255, pColor->GetB()*255);
		return color2;
	}
}

void CProbe::SetTipInProbe(double x, double y, double z)
{
	m_vTipInProbe.x = x;
	m_vTipInProbe.y = y;
	m_vTipInProbe.z = z;
}

void CProbe::SetOrientationInProbe(double Vx, double Vy, double Vz)
{
	m_vOrientationInProbe.x = Vx;
	m_vOrientationInProbe.y = Vy;
	m_vOrientationInProbe.z = Vz;
	m_vOrientationInProbe.SetUnit();
}

void CProbe::UpdateDisplay()
{
	if (PROBE_DISPLAY_CYLINDER)
	{
		UpdateCylinder();
	}
	else
	{
		UpdateDisplayLine();
	}
}

void CProbe::UpdateCylinder()
{
	m_cylBody.SetAxis(m_vOrientationInTrack);
	m_cylExtednLine.SetAxis(m_vOrientationInTrack);
	m_cylOffset.SetAxis(m_vOrientationInTrack);

	Vector3D vTipPos, vExtPos;
	m_vVirtualTipInTrack = m_vRealTipInTrack - m_vOrientationInTrack * m_dbLocalizationOffset;

	vTipPos = m_vRealTipInTrack - m_vOrientationInTrack * (m_dbLocalizationOffset < 0 ? m_dbLocalizationOffset : 0);
	m_cylBody.SetCentroid( vTipPos + m_cylBody.GetHalfHeight() * m_vOrientationInTrack);
	if (m_dbLocalizationOffset >= 0)
	{
		m_cylExtednLine.SetCentroid( m_vVirtualTipInTrack - m_cylExtednLine.GetHalfHeight() * m_vOrientationInTrack);
	}
	else
	{
		m_cylExtednLine.SetCentroid( m_vRealTipInTrack - m_cylExtednLine.GetHalfHeight() * m_vOrientationInTrack);
	}
	m_cylOffset.SetHalfHeight(fabs(m_dbLocalizationOffset) / 2);
	m_cylOffset.SetCentroid( m_vVirtualTipInTrack + m_vOrientationInTrack * m_dbLocalizationOffset / 2);
	
	m_stLabel.vPosition = m_vRealTipInTrack + m_dbProbeLength * m_vOrientationInTrack;;
	//m_stLabel.bShow = TRUE;

	m_tipSphere.SetCenter(vTipPos);
	m_line3D.SetOrigin(vTipPos);
	m_line3D.SetOrientation(m_vOrientationInTrack);

	m_vProbeCenter2World = vTipPos + m_vOrientationInTrack * m_dbProbeLength / 2;

	UpdateLineFromVTarget2Tip();
	
	UpdateIceBallPos();

	/*if (m_pVirtualTarget)
	{
		m_pVirtualTarget->UpdateIceBallOrientation();
	}*/
}

void CProbe::UpdateDisplayLine()
{
	Vector3D vTipPos;
	m_vVirtualTipInTrack = m_vRealTipInTrack - m_vOrientationInTrack * m_dbLocalizationOffset;
	vTipPos = m_vRealTipInTrack - m_vOrientationInTrack * (m_dbLocalizationOffset < 0 ? m_dbLocalizationOffset : 0);
	m_sgBody.Construct( vTipPos, m_vOrientationInTrack, m_dbProbeLength);
	m_sgExtendLine.Construct( m_vVirtualTipInTrack, -m_vOrientationInTrack, m_dbExtLength);
	m_sgOffset.Construct( m_vVirtualTipInTrack, m_vOrientationInTrack, m_dbLocalizationOffset);

	m_stLabel.vPosition = vTipPos + m_dbProbeLength * m_vOrientationInTrack;
	//m_stLabel.bShow = TRUE;

	m_tipSphere.SetCenter(vTipPos);
	m_line3D.SetOrigin(vTipPos);
	m_line3D.SetOrientation(m_vOrientationInTrack);

	m_vProbeCenter2World = vTipPos + m_vOrientationInTrack * m_dbProbeLength / 2;

	UpdateLineFromVTarget2Tip();
	
	UpdateIceBallPos();

	/*if (m_pVirtualTarget)
	{
		m_pVirtualTarget->UpdateIceBallOrientation();
	}*/
}


void CProbe::UpdateLineFromVTarget2Tip()
{
	if (m_pVirtualTarget && m_bShowLineToVTarget)
	{
		m_lineToVTarget.Construct(m_vVirtualTipInTrack, m_pVirtualTarget->GetPos());

		int iSign;
		iSign = ((m_pVirtualTarget->GetPos() - m_vVirtualTipInTrack) * m_vOrientationInTrack) > 0 
			? 1 : -1;
		m_dbDistVTarget2Tip = iSign * m_lineToVTarget.GetLength();
	}
}

Vector3D CProbe::GetOrientationInProbe()
{
	return m_vOrientationInProbe;
}

Vector3D CProbe::GetOrientationInTrack()
{
	return m_vOrientationInTrack;
}

Vector3D CProbe::GetTipInProbe()
{
	return m_vTipInProbe;
}

Vector3D CProbe::GetRealTipInTrack()
{
	return m_vRealTipInTrack;
}

Vector3D CProbe::GetVirtualTipInTrack()
{
	return m_vVirtualTipInTrack;
}

Matrix CProbe::UpdatePosOri(Matrix *mOrigin, Matrix mTransfer)
{
	if (mOrigin != NULL)
	{
		m_mProbeMarker2Tracker = *mOrigin;
	}
	Matrix mProbe;
	mProbe = m_mProbeMarker2Tracker; //quatOrigin.toMatrix();

	//mProbe = mTransfer * m_mProbeMarker2Tracker * mProbe;
	mProbe = mTransfer * mProbe;
	
	Vector vTip2Track;
	Vector vTip2Probe;
	vTip2Probe.SetSize(4);
	vTip2Probe.SetValue(4, m_vTipInProbe.x, m_vTipInProbe.y, m_vTipInProbe.z, 1.0);
	vTip2Track = mProbe * vTip2Probe;
	m_vRealTipInTrack.x = vTip2Track.value[0];
	m_vRealTipInTrack.y = vTip2Track.value[1];
	m_vRealTipInTrack.z = vTip2Track.value[2];

	Vector vOri2Track;
	Vector vOri2Probe;
	vOri2Probe.SetSize(4);
	vOri2Probe.SetValue(4, m_vOrientationInProbe.x, m_vOrientationInProbe.y, m_vOrientationInProbe.z, 0.0);
	vOri2Track = mProbe * vOri2Probe;
	m_vOrientationInTrack.x = vOri2Track.value[0];
	m_vOrientationInTrack.y = vOri2Track.value[1];
	m_vOrientationInTrack.z = vOri2Track.value[2];
	m_vOrientationInTrack.SetUnit();

	UpdateDisplay();
	
	Matrix mRet(4,4);
	if (mOrigin)
	{
		mRet = *mOrigin;
		mRet.value[0][3] = vTip2Track.value[0];
		mRet.value[1][3] = vTip2Track.value[1];
		mRet.value[2][3] = vTip2Track.value[2];
	}

	return mRet;
}

void CProbe::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	if (PROBE_DISPLAY_CYLINDER)
	{
		m_cylBody.SetVisible(m_bVisible);
		m_cylExtednLine.SetVisible(m_bVisible);
		m_cylOffset.SetVisible(m_bVisible);
	//	m_backLine.SetVisible(m_bVisible);
	//	m_stLabel.bShow = bVisible;
	}
	else
	{
		m_sgBody.SetVisible(bVisible);
		m_sgExtendLine.SetVisible(bVisible);
		m_sgOffset.SetVisible(bVisible);
	}

	if (m_iceBall0)
	{
		m_iceBall0->SetVisible(bVisible && m_bSphereSimulating);
	}
	if (m_iceBall20)
	{
		m_iceBall20->SetVisible(bVisible && m_bSphereSimulating);
	}
	if (m_iceBall40)
	{
		m_iceBall40->SetVisible(bVisible && m_bSphereSimulating);
	}
	
	if (m_bShowLineToVTarget)
	{
		m_lineToVTarget.SetVisible(bVisible);
	}
}

BOOL CProbe::IsVisible()
{
	return m_bVisible;
}


void CProbe::SetIndex(int iIndex)
{
	m_iIndex = iIndex;
	CString sPre = "";
	if (m_iProbeCreateType == PCT_VIRTUAL)
	{
		sPre = "V";
	}
	m_sName.Format("%sProbe %d", sPre, m_iIndex);
	m_stLabel.sText.Format("%s%d", sPre, m_iIndex);
}

int CProbe::GetIndex()
{
	return m_iIndex;
}

CString CProbe::GetName()
{
	return m_sName;
}

void CProbe::SetName(CString sName)
{
	m_sName = sName;
	if (m_sName == PREPROBE_NAME)
	{
		m_iIndex = PREPROBE_INDEX;
	}
}

void CProbe::EnableSimulate(BOOL bSimulate)
{
	m_bSphereSimulating = bSimulate;
	if (m_iceBall0)
	{
		m_iceBall0->SetVisible(m_bSphereSimulating);		
	}
	if (m_iceBall20)
	{
		m_iceBall20->SetVisible(m_bSphereSimulating);		
	}
	if (m_iceBall40)
	{
		m_iceBall40->SetVisible(m_bSphereSimulating);		
	}
}

BOOL CProbe::IsSimulating()
{
	return m_bSphereSimulating;
}

void CProbe::SetMaxSphereRadius(double dbRadius)
{
	m_dbMaxSphereRadius = dbRadius;
}

double CProbe::GetMaxSphereRadius()
{
	return m_dbMaxSphereRadius;
}

void CProbe::SetSphereTransparency(double dbTransparency)
{
	m_dbSphereTransparency = dbTransparency;
	
	CColorF *pColor;
	
	pColor = m_tipSphere.GetColor();
	pColor->SetA(m_dbSphereTransparency);

	if (m_iceBall0)
	{
		pColor = m_iceBall0->GetColor();
		pColor->SetA((float)0.3);
	}
	if (m_iceBall20)
	{
		pColor = m_iceBall20->GetColor();
		pColor->SetA((float)0.5);
	}
	if (m_iceBall40)
	{
		pColor = m_iceBall40->GetColor();
		pColor->SetA((float)0.7);
	}
}

double CProbe::GetSphereTransparency()
{
	return m_dbSphereTransparency;
}

BOOL CProbe::SetProbePar(OPERATIONTYPE iOPERType, CString sProbeName)
{
	m_eOperType = iOPERType;
	m_sProbeTypeName = sProbeName;
	switch (iOPERType)
	{
	case OPERATION_TYPE_CRYOTHERAPY:
		{
			CDlgMain* pDlgMain = g_DlgMain;
//			CSurgeryDoc* pDoc = (CSurgeryDoc*)pMainFrame->GetActiveDocument();
			struct PROBE_PAR_CRY *pPar = pDlgMain->m_dlgCryotherapy.m_ProbeParPool.GetProbeParCryByName(sProbeName);
			if (pPar == NULL)
				return FALSE;

			SetLength(pPar->dbLength);
			SetRadius(pPar->dbRadius);
			SetIceBall0(pPar->dbLAxis0, pPar->dbSAxis0, pPar->dbOffset0);
			SetIceBall20(pPar->dbLAxis20, pPar->dbSAxis20, pPar->dbOffset20);
			SetIceBall40(pPar->dbLAxis40, pPar->dbSAxis40, pPar->dbOffset40);
			SetSphereTransparency(100.0/255.0);
		}
		break;
	case OPERATION_TYPE_BIOPSY:
		{
			CDlgMain* pDlgMain = g_DlgMain;
//			CSurgeryDoc* pDoc = (CSurgeryDoc*)pMainFrame->GetActiveDocument();
			struct PROBE_PAR_BIO *pPar = pDlgMain->m_dlgCryotherapy.m_ProbeParPool.GetProbeParBioByName(sProbeName);
			if (pPar == NULL)
				return FALSE;

			SetRadius(pPar->dbRadius);
		}
		break;
	default:
		break;
	}

/*	SetLength(m_dbProbeParameters[m_iProbeType][PROBE_LENGTH]);
	SetRadius(m_dbProbeParameters[m_iProbeType][PROBE_DIAMETER] / 2);

	// Set ice ball parameters
	SetIceBallParameters(m_iceBall1, m_iceBall2);

	SetIceBallOffset(m_dbProbeParameters[m_iProbeType][PROBE_OFFSET]);
*/
	return TRUE;
}

void CProbe::UpdateIceBallPos()
{	
	Vector3D vIceBallCenter0, vCenter20, vCenter40;
	vIceBallCenter0 = m_vVirtualTipInTrack + m_vOrientationInTrack * m_dbIceBallOffset0;
	vCenter20 = m_vVirtualTipInTrack + m_vOrientationInTrack * m_dbIceBallOffset20;
	vCenter40 = m_vVirtualTipInTrack + m_vOrientationInTrack * m_dbIceBallOffset40;

	if (m_iceBall0)
	{
		m_iceBall0->Transformation(vIceBallCenter0, m_vOrientationInTrack);
	}
	if (m_iceBall20)
	{
		m_iceBall20->Transformation(vCenter20, m_vOrientationInTrack);
	}
	if (m_iceBall40)
	{
		m_iceBall40->Transformation(vCenter40, m_vOrientationInTrack);
	}
}

void CProbe::SetVirtualTipInTrack(Vector3D vTip)
{
	m_vVirtualTipInTrack = vTip;
}

void CProbe::SetOrientationInTrack(Vector3D vOri)
{
	m_vOrientationInTrack = vOri;
	m_vOrientationInTrack.SetUnit();
}

Matrix CProbe::GetMarkerOrientationInTrack()
{
	return m_mProbeMarker2Tracker;
}

BOOL CProbe::SetProbeCreateType(PROBECREATETYPE iProbeCreateType)
{
	if (iProbeCreateType < 0 || iProbeCreateType > PCT_VIRTUAL)
		return FALSE;

	m_iProbeCreateType = iProbeCreateType;
	return TRUE;
}

void CProbe::SetVirtualTarget(CVirtualTarget *pVTarget)
{
	m_pVirtualTarget = pVTarget;
}

CVirtualTarget* CProbe::GetVirtualTarget()
{
	return m_pVirtualTarget;
}

//void CProbe::DisconnectVirtualTarget()
//{
//	if (m_pVirtualTarget)
//	{
//		if (m_iProbeCreateType == PCT_VIRTUAL)
//		{
//			m_pVirtualTarget->DisconnectVirtualProbe();
//		}
//		else if (m_iProbeCreateType == PCT_REAL)
//		{
//			m_pVirtualTarget->DisconnectRealProbe();
//		}
//	}
//}

BOOL CProbe::IsShowConnectLineToVTarget()
{
	return m_bShowLineToVTarget;
}

void CProbe::SetShowConnectLineToVTarget(BOOL bShow)
{
	m_bShowLineToVTarget = bShow;
	m_lineToVTarget.SetVisible(bShow);
}

void CProbe::ShowDistFromTipToVTarget()
{
	if (m_pVirtualTarget)
	{
		Vector3D vDist = m_vVirtualTipInTrack - m_pVirtualTarget->GetSphere()->GetCenter();
		double dbDist = vDist.GetLen();
		CString sOut;
		sOut.Format("Distance from tip to virtual target is:\n\t\t%.2f mm\n", dbDist);
		PanaMessageBox(sOut);
	}
}

double CProbe::GetDistFromTip2VTarget()
{
	return m_dbDistVTarget2Tip;
}

Vector3D CProbe::GetVFromVTarget2Tip()
{
	Vector3D vTip2VTarget;
	if (m_pVirtualTarget)
	{
		vTip2VTarget =  m_pVirtualTarget->GetPos() - m_vVirtualTipInTrack;
	}
	return -vTip2VTarget;
}

void CProbe::SetIceBall0(double dbLAxis, double dbSAxis, double dbOffset)
{
	if (m_iceBall0 == NULL)
	{
		m_iceBall0 = new CEllipsoid(m_vVirtualTipInTrack, m_vOrientationInTrack, dbSAxis, dbLAxis);
		CColorF colorIceBall1( 1.0f, 0.0f, 0.0f, 1.0f );
		m_iceBall0->SetColor(&colorIceBall1);
		m_iceBall0->SetUsingTransparency(TRUE);
		m_iceBall0->SetVisible(FALSE);
	}
	m_iceBall0->SetHeight(dbLAxis);
	m_iceBall0->SetDiameter(dbSAxis);
	m_dbIceBallOffset0 = dbOffset;
}

void CProbe::SetIceBall20(double dbLAxis, double dbSAxis, double dbOffset)
{
	if (m_iceBall20 == NULL)
	{
		m_iceBall20 = new CEllipsoid(m_vVirtualTipInTrack, m_vOrientationInTrack, dbSAxis, dbLAxis);
		CColorF colorIceBall1( 1.0f, 1.0f, 0.0f, 1.0f );
		m_iceBall20->SetColor(&colorIceBall1);
		m_iceBall20->SetUsingTransparency(TRUE);
		m_iceBall20->SetVisible(FALSE);
	}
	m_iceBall20->SetHeight(dbLAxis);
	m_iceBall20->SetDiameter(dbSAxis);
	m_dbIceBallOffset20 = dbOffset;
}

void CProbe::SetIceBall40(double dbLAxis, double dbSAxis, double dbOffset)
{
	if (m_iceBall40 == NULL)
	{
		m_iceBall40 = new CEllipsoid(m_vVirtualTipInTrack, m_vOrientationInTrack, dbSAxis, dbLAxis);
		CColorF colorIceBall1( 1.0f, 0.0f, 1.0f, 1.0f );
		m_iceBall40->SetColor(&colorIceBall1);
		m_iceBall40->SetUsingTransparency(TRUE);
		m_iceBall40->SetVisible(FALSE);
	}
	m_iceBall40->SetHeight(dbLAxis);
	m_iceBall40->SetDiameter(dbSAxis);
	m_dbIceBallOffset40 = dbOffset;
}

void CProbe::EnableControlMRIScanner(BOOL bControl)
{
	m_bControlMRIScanner = bControl;
	m_dbLocalizationOffset = 0;
}

void CProbe::SetExtendLine(double dbExtLength)
{
	m_dbExtLength = dbExtLength;
	UpdateDisplay();
}

void CProbe::SetOffset(double dbOffset)
{
	m_dbLocalizationOffset = dbOffset;
	UpdateDisplay();
}

void CProbe::SaveLastScanPar(Vector3D vTipInScan, Vector3D vOriInScan, Matrix mTranNeedle2Scan,int iLastOffset)
{
	m_vTipInScanner = vTipInScan;
	m_vOriInScanner = vOriInScan;
	m_mTranNeedle2Scanner = mTranNeedle2Scan;
	m_bLastScanOK = TRUE;
	m_iLastOffset = iLastOffset;
}

BOOL CProbe::IsLastScanOK()
{
	return m_bLastScanOK;
}

void CProbe::GetLastScanPar(Vector3D &vTipInScan, Vector3D &vOriInScan, Matrix &m_mTranNeedle2Scan)
{
	vTipInScan = m_vTipInScanner;
	vOriInScan = m_vOriInScanner;
	m_mTranNeedle2Scan = m_mTranNeedle2Scanner;
}

void CProbe::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_sName;
		ar << m_eOperType;
		ar << m_sProbeTypeName;
		ar << m_vTipInProbe.x << m_vTipInProbe.y << m_vTipInProbe.z;
		ar << m_vOrientationInProbe.x << m_vOrientationInProbe.y << m_vOrientationInProbe.z;
		ar << m_dbProbeLength;
		m_mProbeMarker2Tracker.Serialize(ar);
		ar << m_bLastScanOK;
		if (m_bLastScanOK)
		{
			ar << m_vTipInScanner.x << m_vTipInScanner.y << m_vTipInScanner.z;
			ar << m_vOriInScanner.x << m_vOriInScanner.y << m_vOriInScanner.z;
			m_mTranNeedle2Scanner.Serialize(ar);
		}
	}
	else
	{
		ar >> m_sName;
		int nOperType;
		ar >> nOperType;
		m_eOperType = (OPERATIONTYPE)nOperType;
		ar >> m_sProbeTypeName;
		SetProbePar(m_eOperType, m_sProbeTypeName);
		ar >> m_vTipInProbe.x >> m_vTipInProbe.y >> m_vTipInProbe.z;
		ar >> m_vOrientationInProbe.x >> m_vOrientationInProbe.y >> m_vOrientationInProbe.z;
		ar >> m_dbProbeLength;
		SetLength(m_dbProbeLength);
		m_mProbeMarker2Tracker.Serialize(ar);
		UpdatePosOri(&m_mProbeMarker2Tracker);
		ar >> m_bLastScanOK;
		if (m_bLastScanOK)
		{
			ar >> m_vTipInScanner.x >> m_vTipInScanner.y >> m_vTipInScanner.z;
			ar >> m_vOriInScanner.x >> m_vOriInScanner.y >> m_vOriInScanner.z;
			m_mTranNeedle2Scanner.Serialize(ar);
		}
	}
}
