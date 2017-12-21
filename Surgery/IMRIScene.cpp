// IMRIScene.cpp: implementation of the CIMRIScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surgery.h"
#include "IMRIScene.h"
#include "Probe.h"
#include "VirtualTarget.h"
#include "CryotherapyDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HALF_FRAME_SIZE 175.0

char TEXT_APSI[6] = {
	'L',
	'R',
	'S',
	'I',
	'A',
	'P'
};

#define DIS_APSI (HALF_FRAME_SIZE+10)
Vector3D vPOS_APSI[6] = {
	Vector3D(DIS_APSI, 0, 0),
	Vector3D(-DIS_APSI, 0, 0),
	Vector3D(0, DIS_APSI, 0),
	Vector3D(0, -DIS_APSI, 0),
	Vector3D(0, 0, DIS_APSI),
	Vector3D(0, 0, -DIS_APSI),
};

Vector3D vAPSI_ORI[6] = {
	Vector3D(1, 0, 0),
	Vector3D(-1, 0, 0),
	Vector3D(0, 1, 0),
	Vector3D(0, -1, 0),
	Vector3D(0, 0, 1),
	Vector3D(0, 0, -1),
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMRIScene::CIMRIScene() : CScene()
{
	Vector3D vVertex[8] = { Vector3D(-HALF_FRAME_SIZE,-HALF_FRAME_SIZE,-HALF_FRAME_SIZE),
							Vector3D(HALF_FRAME_SIZE,-HALF_FRAME_SIZE,-HALF_FRAME_SIZE),
							Vector3D(HALF_FRAME_SIZE,HALF_FRAME_SIZE,-HALF_FRAME_SIZE),
							Vector3D(-HALF_FRAME_SIZE,HALF_FRAME_SIZE,-HALF_FRAME_SIZE),
							Vector3D(-HALF_FRAME_SIZE,-HALF_FRAME_SIZE,HALF_FRAME_SIZE),
							Vector3D(HALF_FRAME_SIZE,-HALF_FRAME_SIZE,HALF_FRAME_SIZE),
							Vector3D(HALF_FRAME_SIZE,HALF_FRAME_SIZE,HALF_FRAME_SIZE),
							Vector3D(-HALF_FRAME_SIZE,HALF_FRAME_SIZE,HALF_FRAME_SIZE) };
	m_BoxFrame.SetVertices(vVertex);
	m_BoxFrameBackup.SetVertices(vVertex);
	CColorF color( 1.0, 1.0, 1.0, 1.0 );
	m_BoxFrame.SetColor( &color );
	m_bBoxFrameVisible = FALSE;


	BYTE byColor[4] = { 255, 255, 255, 10};
	for (int i=0; i<6; i++)
	{
		
		m_stTextAPSI[i].pFont = CGlobalFuncs::GetFont();
		m_stTextAPSI[i].vPosition = vPOS_APSI[i];
		m_stTextAPSI[i].cfColor.SetColor( 1.0, 1.0, 1.0, 1.0 );
		m_stTextAPSI[i].bShow = TRUE;
		m_vBoxFrameLabelPos[i] = vPOS_APSI[i]; // Added by Wei Wei [2007.6.26]
	}
	Matrix mInitTransfer(4,4);
	m_mTransferMatrix = mInitTransfer;
	UpdateFrameBox(TEXT_APSI, &mInitTransfer, NULL);

	pcDistA = new PointCloud(1);
	pcDistA->SetColor(1.0, 0.0, 0.0);
	pcDistA->SetPointSize(2.0);
	pcDistA->vertexArr[0].p = Vector3D(0,0,0);
	pcDistB = new PointCloud(1);
	pcDistB->SetPointSize(2.0);
	pcDistB->SetColor(1.0, 0.0, 0.0);
	pcDistB->vertexArr[0].p = Vector3D(0,0,0);
	ResetDistMeasure();

	m_Display.m_bShowCoordAxis = FALSE;

	m_bShowSphereInVT=TRUE;

	

	m_b3DDisplay = FALSE;
	m_bCrossDisplayed=FALSE;
	
}

CIMRIScene::~CIMRIScene()
{
	if (pcDistA)
	{
		delete pcDistA;
		pcDistA = NULL;
	}
	if (pcDistB)
	{
		delete pcDistB;
		pcDistB = NULL;
	}
	CleanDistList();
}

void CIMRIScene::AddProbe(CProbe *pProbe)
{
	CString sName = pProbe->GetName();
	CString sCylName;

	// Added by Wei Wei [2007/6/22]
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;
	GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	// - End

	if (PROBE_DISPLAY_CYLINDER)
	{
		sCylName.Format("%s-Ex", sName);
		AddCylinder(&(pProbe->m_cylExtednLine), sCylName);
		
	//	sCylName.Format("%s-Ba", sName);
	//	AddCylinder(&(pProbe->m_backLine), sCylName);

		AddCylinder(&(pProbe->m_cylBody), sName);

		sCylName.Format("%s-Offset", sName);
		AddCylinder(&(pProbe->m_cylOffset), sName);
	}
	else 
	{
		sCylName.Format("%s-SgBody", sName);
		AddSegment(&(pProbe->m_sgBody), sName);
		
		sCylName.Format("%s-SgExt", sName);
		AddSegment(&(pProbe->m_sgExtendLine), sName);

		sCylName.Format("%s-SgOffset", sName);
		AddSegment(&(pProbe->m_sgOffset), sName);
	}
	
//	sCylName.Format("%s-TipSphere", sName);
//	AddSphere(&(pProbe->m_tipSphere), sCylName);

	if (pProbe->m_iceBall40)
	{
		sCylName.Format("%s-Iceball40", sName);
		AddEllipsoid(pProbe->m_iceBall40, sCylName);
	}

	if (pProbe->m_iceBall20)
	{
		sCylName.Format("%s-Iceball20", sName);
		AddEllipsoid(pProbe->m_iceBall20, sCylName);
	}

	if (pProbe->m_iceBall0)
	{
		sCylName.Format("%s-Iceball0", sName);
		AddEllipsoid(pProbe->m_iceBall0, sCylName);
	}

	// Added by Wei Wei [2007/6/22]
	SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	// - End

//	AddText(&(pProbe->m_stLabel));

//	sCylName.Format("%s-lineToVTarget", sName);
//	AddSegment(&(pProbe->m_lineToVTarget), sCylName);
}

void CIMRIScene::RemoveProbe(CProbe *pProbe)
{
	if (pProbe)
	{
		if (PROBE_DISPLAY_CYLINDER)
		{
			RemoveCylinder(&(pProbe->m_cylExtednLine));
//		RemoveCylinder(&(pProbe->m_backLine));
			RemoveCylinder(&(pProbe->m_cylBody));
			RemoveCylinder(&(pProbe->m_cylOffset));
		}
		else
		{
			RemoveSegment(&(pProbe->m_sgBody));
			RemoveSegment(&(pProbe->m_sgExtendLine));
			RemoveSegment(&(pProbe->m_sgOffset));
		}
//		Remove(&(pProbe->m_tipSphere));
		Remove(pProbe->m_iceBall0);
		Remove(pProbe->m_iceBall20);
		Remove(pProbe->m_iceBall40);
//		Remove(&(pProbe->m_lineToVTarget));
//		RemoveText(&(pProbe->m_stLabel));
	}
}

void CIMRIScene::RemoveUSRect(CUSRectangle *pUSRect)
{
	Remove(pUSRect->m_pRectangle);
	
}

void CIMRIScene::AddUSRect(CUSRectangle *pUSRect, BOOL bKeepView /*= FALSE*/)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;

	if (bKeepView)
	{
		GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	}
	
	CRectangle *pRectangle;
	
	
	pRectangle = pUSRect->m_pRectangle;
	AddData( pRectangle, pRectangle->GetName(), DATATYPE_RECTANGLE );

	if (bKeepView)
	{
		SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	}
	else
	{
		ChangeViewToUSRect(pUSRect);
	}
	
	pRectangle->SetUsingTransparency(FALSE);

	
}

void CIMRIScene::AddMRIRect(CMRIRectangle *pMRIRect, BOOL bKeepView /*= FALSE*/)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;

	if (bKeepView)
	{
		GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	}
	
	CRectangle *pRectangle;
	
	
	pRectangle = pMRIRect->m_pRectangle;
	AddData( pRectangle, pRectangle->GetName(), DATATYPE_RECTANGLE );

	if (bKeepView)
	{
		SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	}
	else
	{
		//ChangeViewToRect(pRectangle);
		ChangeViewToMRIRect(pMRIRect,TRUE);
	}
	
	pRectangle->SetUsingTransparency(FALSE);

	
}

void CIMRIScene::SetRectIndex(int iIndex)
{
	/*设置当前显示的图像序号
	  将非当前图像上的测量数据隐藏*/
	m_iImageIndex = iIndex;
	if(m_b3DDisplay)
		return;

	POSITION pos = m_lstSegDist.GetHeadPosition();
	for (;pos;)
	{
		SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);
		if(pSgDist->iIndex == m_iImageIndex)
		{
			pSgDist->sg_label.bShow = TRUE;
			pSgDist->sg_segment->SetVisible(TRUE);
		}else
		{
			pSgDist->sg_label.bShow = FALSE;
			pSgDist->sg_segment->SetVisible(FALSE);
		}
	}
}

void CIMRIScene::RemoveMRIRect(CMRIRectangle *pMRIRect)
{
	Remove(pMRIRect->m_pRectangle);
	
}

void CIMRIScene::ChangeViewToProbe(CProbe* pProbe)
{
	if (pProbe)
	{
		pProbe->SetVisible(TRUE);
		Vector3D vEyePoint, vGazePoint, vUp, vFront;
		double dDistance;
		GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );

		vGazePoint = pProbe->GetVirtualTipInTrack();
		vEyePoint = vGazePoint + dDistance * vFront;

		SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	}
}

void CIMRIScene::ChangeViewToRect(CRectangle* pRect)
{
	double dbWidth = pRect->GetWidth();
	double dbHeight = pRect->GetHeight();
	double dbMaxSize = max(dbWidth, dbHeight);
	double dbDist = (dbMaxSize + 30) / 2 / tan(20*PI/180);
	Vector3D vNormal = pRect->GetNormal();
	Vector3D vCenter = pRect->GetCenter();
	Vector3D vAD = pRect->GetVectorAD();
	Vector3D vAB = pRect->GetVectorAB();
	vAD.SetUnit();
	vAB.SetUnit();
	SetFront(-vNormal.x, -vNormal.y, -vNormal.z);	
	SetUp(vAD.x, vAD.y, vAD.z);
	SetDistance(dbDist);
	SetGazePoint(vCenter.x, vCenter.y, vCenter.z);
}

/*bChangeDistance=TRUE 切换图像时，改变图像的缩放比例
                 =FALSE 切换图像时，保持原来图像的缩放比例不变
*/
void CIMRIScene::ChangeViewToMRIRect(CMRIRectangle* pMRIRect,BOOL bChangeDistance /*=TRUE*/)
{
	CRectangle *pRect = pMRIRect->m_pRectangle;
	CMRIImage* pMriImage;
	char** sOrientChar;
	double** ppdCharPosition;
	pMriImage = pMRIRect->GetMRIImg();
	sOrientChar = pMriImage->GetOrientChar();
	ppdCharPosition = pMriImage->GetOrientCharPos();

	ChangeViewToRect(pRect,sOrientChar,ppdCharPosition,bChangeDistance);
}

void CIMRIScene::ChangeViewToUSRect(CUSRectangle* pUSRect,BOOL bChangeDistance /*=TRUE*/)
{
	CRectangle *pRect = pUSRect->m_pRectangle;
	
	char** sOrientChar=new char*[4];
	double** ppdCharPosition=new double*[4];
	for(int i=0;i<4;i++)
	{
		sOrientChar[i]=new char[10];
		for(int j=0;j<10;j++)
		{
			sOrientChar[i][j]=pUSRect->m_ppcOrientation[i][j];
		}
		ppdCharPosition[i]=new double[4];
		for(j=0;j<3;j++)
		{
			ppdCharPosition[i][j]=pUSRect->m_ppdOrientCharPos[i][j];
		}
	}
	ChangeViewToRect(pRect,sOrientChar,ppdCharPosition,bChangeDistance);
	for(i=0;i<4;i++)
	{
		delete sOrientChar[i];
		delete ppdCharPosition[i];
	}
	delete []sOrientChar;
	delete []ppdCharPosition;
}

void CIMRIScene::ChangeViewToRect(CRectangle* pRect,char** sOrientChar,	double** ppdCharPosition,BOOL bChangeDistance)
{
	
	double dbWidth = pRect->GetWidth();
	double dbHeight = pRect->GetHeight();
	double dbMaxSize = max(dbWidth, dbHeight);
	double dbDist = (dbMaxSize + 30) / 2 / tan(20*PI/180);
	
	
	Vector3D vPosA, vPosS, vPosR;
	Vector3D vEyeDir, vUp;
	Vector3D vCenter = pRect->GetCenter();
	BOOL bA = FALSE;
	BOOL bS = FALSE;
	BOOL bR = FALSE;
	for ( int j = 0; j < 4; j++ )
	{
		if (sOrientChar[j][0] == 'A')
		{
			vPosA.x = ppdCharPosition[j][0];
			vPosA.y = ppdCharPosition[j][1];
			vPosA.z = ppdCharPosition[j][2];
			bA = TRUE;
		}
		if (sOrientChar[j][0] == 'S')
		{
			vPosS.x = ppdCharPosition[j][0];
			vPosS.y = ppdCharPosition[j][1];
			vPosS.z = ppdCharPosition[j][2];
			bS = TRUE;
		}
		if (sOrientChar[j][0] == 'R')
		{
			vPosR.x = ppdCharPosition[j][0];
			vPosR.y = ppdCharPosition[j][1];
			vPosR.z = ppdCharPosition[j][2];
			bR = TRUE;
		}
	}
	if (bA && bR)
	{
		vEyeDir = (vPosA - vCenter) ^ (vPosR - vCenter);
		vUp = vPosA - vCenter;
	}
	else if (bA && bS)
	{
		vEyeDir = (vPosS - vCenter) ^ (vPosA - vCenter);
		vUp = vPosS - vCenter;
	}
	else if (bS && bR)
	{
		vEyeDir = (vPosS - vCenter) ^ (vPosR - vCenter);
		vUp = vPosS - vCenter;
	}
	else
	{
		vEyeDir = pRect->GetNormal();
		vUp = pRect->GetVectorAD();
	}
	vEyeDir.SetUnit();
	vUp.SetUnit();
	SetFront(vEyeDir.x, vEyeDir.y, vEyeDir.z);
	SetUp(vUp.x, vUp.y, vUp.z);
	if(bChangeDistance)
	{
		SetDistance(dbDist);
	}

	SetGazePoint(vCenter.x, vCenter.y, vCenter.z);

	
	
}

BOOL CIMRIScene::BoxFrameIsVisible()
{
	return m_bBoxFrameVisible;
}

void CIMRIScene::ChangeViewToBoxFrame(Matrix *pMatrix, bool bKeepOrigEyeToGazeDir /*= FALSE*/)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;

	if (!bKeepOrigEyeToGazeDir)
	{
		vGazePoint = m_BoxFrame.GetCenter();
		vUp = Vector3D(0, 0, 1);
		vFront = Vector3D(0, -1, 0);
	}
	else
	{
		GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
		vGazePoint = m_BoxFrame.GetCenter();
/*		dDistance = HALF_FRAME_SIZE * 4;
		if (pMatrix)
		{
			Matrix mRotateM = pMatrix->GetSubMatrix(3,3);
			vUp.Rotate(mRotateM.value);
			vFront.Rotate(mRotateM.value);

		}
		vEyePoint = vGazePoint + dDistance * vFront;
		SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance ); */
	}

		if (pMatrix)
		{
			Matrix mRotateM = pMatrix->GetSubMatrix(3,3);
			vUp.Rotate(mRotateM.value);
			vFront.Rotate(mRotateM.value);

//			double **pMatrixValue = pMatrix->value; 
//			
//			double **pRotateM;
//			pRotateM = new double*[3];
//			for (int i=0; i<3; i++)
//			{
//				pRotateM[i] = new double[3];
//				for (int j=0; j<3; j++)
//				{
//					pRotateM[i][j] = pMatrixValue[i][j];
//				}
//			}
//			vUp.Rotate(pRotateM);
//			vFront.Rotate(pRotateM);
//			for (i=0; i<3; i++)
//			{
//				delete[] pRotateM[i];
//			}
//			delete[] pRotateM; 
		}
		dDistance = HALF_FRAME_SIZE * 4;
		vEyePoint = vGazePoint + dDistance * vFront;
		SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
}

void CIMRIScene::ResetView()
{
	ChangeViewToBoxFrame(&m_mTransferMatrix, FALSE);
}

void CIMRIScene::RemoveAllLine()
{
	if (m_lstSegment.GetCount() > 0)
	{
		POSITION pos = m_lstSegment.GetHeadPosition();
		for (;pos;)
		{
			CSegment *sg_segment = m_lstSegment.GetNext(pos);
			Remove(sg_segment);
			delete sg_segment;
		}
	}
	m_lstSegment.RemoveAll();
	
}

void CIMRIScene::AddLine(Vector3D vP1, Vector3D vP2)
{
	CSegment *sg_segment = new CSegment(vP1, vP2);
	sg_segment->m_bDisableDepthTest=FALSE;

	CColorF pColor(1.0, 0.0, 0.0, 1.0);
	sg_segment->SetColor(&pColor);
	
	m_lstSegment.AddTail(sg_segment);
	AddSegment(sg_segment,"");
}

void CIMRIScene::AddDistance(Vector3D vP1, Vector3D vP2)
{
	// Add segment and label

	/*采用Segmeng作为距离线段显示元素*/
	SEGMENT_DIST *sgDist = new SEGMENT_DIST;
	sgDist->sg_segment = new CSegment(vP1, vP2);
	if(m_b3DDisplay)
	{
		sgDist->sg_segment->m_bDisableDepthTest=FALSE;
		sgDist->sg_label.m_bDisableDepthTest=FALSE;
	}

	CColorF pColor(1.0, 0.0, 0.0, 1.0);
	sgDist->sg_segment->SetColor(&pColor);
	sgDist->sg_dbDist = sgDist->sg_segment->GetLength();
	sgDist->sg_label.sText.Format("%.1f", sgDist->sg_dbDist);
	sgDist->sg_label.cfColor.SetColor(1.0, 0.0, 0.0, 1.0);
	sgDist->sg_label.vPosition = (vP1 + vP2) / 2;
	sgDist->sg_label.bShow = TRUE;
	sgDist->sg_sName.Format("DistSeg%02d", m_lstSegDist.GetCount());
	
	sgDist->iIndex = m_iImageIndex;
	m_lstSegDist.AddTail(sgDist);
	AddSegment(sgDist->sg_segment, sgDist->sg_sName);
	AddText(&(sgDist->sg_label));

	/*采用cylinder作为距离线段显示元素
	Vector3D vCenter,vAxis;
	double dHalfHeight= vP1.disFrom(vP2)/2.0;
	vCenter.x = (vP1.x+vP2.x)/2.0;
	vCenter.y = (vP1.y+vP2.y)/2.0;
	vCenter.z = (vP1.z+vP2.z)/2.0;
	vAxis.x=vP1.x-vP2.x;
	vAxis.y=vP1.y-vP2.y;
	vAxis.z=vP1.z-vP2.z;
	vAxis.normalize();

	SEGMENT_DIST *sgDist = new SEGMENT_DIST;
	
	sgDist->sg_segment = new CCylinder(vCenter, vAxis ,2.0,dHalfHeight);
	sgDist->sg_segment->m_bDisableDepthTest=FALSE;
	sgDist->sg_label.m_bDisableDepthTest=FALSE;

	
	sgDist->sg_segment->SetColorRef(RGB(255,0,0));
	sgDist->sg_dbDist = dHalfHeight * 2.0;
	sgDist->sg_label.sText.Format("%.1f", sgDist->sg_dbDist);
	sgDist->sg_label.cfColor.SetColor(1.0, 0.0, 0.0, 1.0);
	sgDist->sg_label.vPosition = (vP1 + vP2) / 2;
	sgDist->sg_label.bShow = TRUE;
	sgDist->sg_sName.Format("DistSeg%02d", m_lstSegDist.GetCount());
	m_lstSegDist.AddTail(sgDist);
	AddCylinder(sgDist->sg_segment, sgDist->sg_sName);
	AddText(&(sgDist->sg_label));*/
}

void CIMRIScene::AddDistMeasurePoint(Vector3D vPoint)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;

	m_iDistClickNum ++;
	if (m_iDistClickNum == 1)
	{
		pcDistA->vertexArr[0].p = vPoint;
		if (!m_bPointAAdded)
		{
			GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
			AddPointCloud(pcDistA, "DistPtA");
			SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
			m_bPointAAdded = TRUE;
		}
	}
	else if (m_iDistClickNum == 2)
	{
		if ((pcDistA->vertexArr[0].p - vPoint).GetLen() > 0)
		{
			pcDistB->vertexArr[0].p = vPoint;
			if (!m_bPointBAdded)
			{
				GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
				AddPointCloud(pcDistB, "DistPtB");
				SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
				m_bPointBAdded = TRUE;
			}
			AddDistance(pcDistA->vertexArr[0].p, pcDistB->vertexArr[0].p);
		}
		else
		{
			Remove(pcDistA);
			m_bPointAAdded = FALSE;
		}
		m_iDistClickNum = 0;
	}
}

void CIMRIScene::ResetDistMeasure()
{
	Remove(pcDistA);
	Remove(pcDistB);
	m_bPointAAdded = FALSE;
	m_bPointBAdded = FALSE;
	m_iDistClickNum = 0; 
}

void CIMRIScene::RemoveAllDistMeasurement()
{
	ResetDistMeasure();
	if (m_lstSegDist.GetCount() > 0)
	{
		POSITION pos = m_lstSegDist.GetHeadPosition();
		for (;pos;)
		{
			SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);
			Remove(pSgDist->sg_segment);
			RemoveText(&(pSgDist->sg_label));
		}
	}
	CleanDistList();
}

void CIMRIScene::CleanDistList()
{
	if (m_lstSegDist.GetCount() > 0)
	{
		POSITION pos = m_lstSegDist.GetHeadPosition();
		for (;pos;)
		{
			SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);
			if (pSgDist)
			{
				if (pSgDist->sg_segment)
				{
					delete pSgDist->sg_segment;
				}
				delete pSgDist;
			}
		}
		m_lstSegDist.RemoveAll();
	}
}

void CIMRIScene::AddModelMesh(CMesh *pMesh)
{
	Vector3D vEyePoint, vGazePoint, vUp, vFront;
	double dDistance;
	GetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
	AddMesh(pMesh, pMesh->GetName());
	SetLookAtPar( vEyePoint, vGazePoint, vUp, vFront, dDistance );
}

void CIMRIScene::ShowBoxFrame(BOOL bShow)
{
	if (m_bBoxFrameVisible && !bShow)
	{
		Remove(&m_BoxFrame);
		for (int i=0; i<6; i++)
		{
			RemoveText(&(m_stTextAPSI[i]));
		}
	}
	else if (!m_bBoxFrameVisible && bShow)
	{
		AddData(&m_BoxFrame, "MRIBox", DATATYPE_BOX );
		ChangeViewToBoxFrame();
		for (int i=0; i<6; i++)
		{
			AddText(&(m_stTextAPSI[i]));
		}
		SetIsTextDrawn( TRUE );
	}
	m_bBoxFrameVisible = bShow;
}

void CIMRIScene::AddVirtualTarget(CVirtualTarget *pVTarget)
{
	if (pVTarget)
	{		
		AddSphere(pVTarget->GetSphere(), pVTarget->GetSphere()->GetName());
		if(pVTarget->GetLabel()->sText!="")
		{
			SetIsTextDrawn( TRUE );
			AddText(pVTarget->GetLabel());
		}
		if(m_bShowSphereInVT)
		{
			AddEllipsoid(pVTarget->GetSphereInside(), pVTarget->GetSphereInside()->GetName());
		}
	}
}

void CIMRIScene::RemoveVirtualTarget(CVirtualTarget *pVTarget)
{
	if (pVTarget)
	{
		RemoveText(pVTarget->GetLabel());
		Remove(pVTarget->GetSphere());
		if(m_bShowSphereInVT)
		{
			Remove(pVTarget->GetSphereInside());
		}
	}
}

void CIMRIScene::TransferBoxFrame(Matrix* pMatrix)
{
// Commented by Wei Wei, since pMatrix Value is a relative transform now [2007.6.26]
// - Begin
//	Vector3D vVertices[8];
//	m_BoxFrameBackup.GetVertices(vVertices);
//	m_BoxFrame.SetVertices(vVertices);
// - End
	
	if (pMatrix)
	{
		Vector3D vVertices[8];
		m_BoxFrameBackup.GetVertices(vVertices);
		m_BoxFrame.SetVertices(vVertices);
		m_BoxFrame.Transformation(pMatrix->value);

		m_mTransferMatrix = *pMatrix;

		Matrix mRotate;
		mRotate = pMatrix->GetSubMatrix(3,3);
//		double **ppRotate;
//		ppRotate = new double*[3];
//		for (int i=0; i<3; i++)
//		{
//			ppRotate[i] = new double[3];
//			for (int j=0; j<3; j++)
//			{
//				ppRotate[i][j] = pMatrix->value[i][j];
//			}
//		}
		for (int i=0; i<6; i++)
		{
			// Commented by Wei Wei, since pMatrix Value is a relative transform now [2007.6.26]
			// - Begin
			m_vBoxFrameLabelPos[i] = vPOS_APSI[i];
			// - End
			m_vBoxFrameLabelPos[i].Transformation(pMatrix->value);
			m_stTextAPSI[i].vPosition = m_vBoxFrameLabelPos[i];

			m_vAPSIOri[i] = vAPSI_ORI[i];
			m_vAPSIOri[i].Rotate(mRotate.value);
		}

//		for (i=0; i<3; i++)
//		{
//			delete[] ppRotate[i];
//		}
//		delete[] ppRotate;
	}

//	ChangeViewToBoxFrame(pMatrixValue);
	//ChangeViewToBoxFrame();
/*	if (m_bBoxFrameVisible)
	{
		ShowBoxFrame(FALSE);
		ShowBoxFrame(TRUE);
	} */
}

void CIMRIScene::SetBoxFrameLabel(char* pBoxChar)
{
	for (int i=0; i<6; i++)
	{

		m_stTextAPSI[i].sText = pBoxChar[i];
		if (pBoxChar[i] == 'L')
		{
			m_vLInWorld = m_vAPSIOri[i];
			m_vLInWorld.SetUnit();
		}
		if (pBoxChar[i] == 'S')
		{
			m_vSInWorld = m_vAPSIOri[i];
			m_vSInWorld.SetUnit();
		}
	}
}

CString CIMRIScene::GetBoxLabel()
{
	CString sLabel;
	for (int i=0; i<6; i++)
	{
		sLabel += m_stTextAPSI[i].sText;
	}
	return sLabel;
}

void CIMRIScene::UpdateFrameBox(char* pBoxLabel, Matrix* pMatrix, Vector3D* vMov, BOOL bKeepEyrDirToBox /*= TRUE*/)
{
	if (pMatrix)
	{
		TransferBoxFrame(pMatrix);
	}
	if (vMov)
	{
		TranslationFrameBox(*vMov);
	}
	SetBoxFrameLabel(pBoxLabel);

	ChangeViewToBoxFrame(pMatrix, bKeepEyrDirToBox?true:false);
}

void CIMRIScene::TranslationFrameBox(Vector3D vMov)
{
	if (vMov.x != 0 || vMov.y != 0 || vMov.z != 0)
	{
		m_BoxFrame.Translation(vMov);
		for (int i=0; i<6; i++)
		{
			m_vBoxFrameLabelPos[i] += vMov;
			m_stTextAPSI[i].vPosition += vMov;
		}
	}
}

// Added by Wei Wei [2008/3/11]
int CIMRIScene::SelectVirtualTargetByClicking(CList<CVirtualTarget*, CVirtualTarget*>* plstVirtualTarget, CPoint point)
{
	CVirtualTarget* pTarget = NULL;

	if (plstVirtualTarget->GetCount() > 0)
	{
		CList<CSphere*, CSphere*> lstSphere;
		POSITION pos = plstVirtualTarget->GetHeadPosition();
		for (int i = 0; i < plstVirtualTarget->GetCount(); i++)
		{
			CVirtualTarget* pTmpTarget = plstVirtualTarget->GetNext(pos);
			lstSphere.AddTail(pTmpTarget->GetSphere());
		}
		int nSelectedSphere = SelectSphereByClicking(&lstSphere, point.x, point.y);
		if (nSelectedSphere >= 0
			&& nSelectedSphere < plstVirtualTarget->GetCount())
		{
			return nSelectedSphere;
//			//////////////////////////////////////////////////////////////////////////
//			// This is what we want!!!
//			// The selected target
//			pTarget = plstVirtualTarget->GetAt(plstVirtualTarget->FindIndex(nSelectedSphere));
		}
	}

	return -1;
}

void CIMRIScene::TransformAllDistanceSegment(double** ppdtran)
{
	Vector3D pFirstPnt,pSecondPnt;
	if (m_lstSegDist.GetCount() > 0)
	{
		POSITION pos = m_lstSegDist.GetHeadPosition();
		for (;pos;)
		{
			SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);

			pFirstPnt = pSgDist->sg_segment->GetFirstEndpoint();
			pSecondPnt = pSgDist->sg_segment->GetSecondEndpoint();
			pFirstPnt.Transformation(ppdtran);
			pSecondPnt.Transformation(ppdtran);
			pSgDist->sg_segment->Construct(pFirstPnt,pSecondPnt);

			pSgDist->sg_label.vPosition.Transformation(ppdtran);
		}
	}
}

void CIMRIScene::TranslateAllDistanceSegment(Vector3D vMov)
{
	Vector3D pFirstPnt,pSecondPnt;
	if (m_lstSegDist.GetCount() > 0)
	{
		POSITION pos = m_lstSegDist.GetHeadPosition();
		for (;pos;)
		{
			SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);
			
			pFirstPnt = pSgDist->sg_segment->GetFirstEndpoint();
			pSecondPnt = pSgDist->sg_segment->GetSecondEndpoint();
			pFirstPnt = pFirstPnt + vMov;
			pSecondPnt = pSecondPnt + vMov;
			pSgDist->sg_segment->Construct(pFirstPnt,pSecondPnt);

			pSgDist->sg_label.vPosition = pSgDist->sg_label.vPosition + vMov;
		}
	}

}

void CIMRIScene::TranslateAllDistanceSegment(CWnd* pCryoWnd,double dCurPTPos,BOOL bInverse)
{
	CCryotherapyDlg* pCryoDlg = (CCryotherapyDlg*)pCryoWnd;
	Vector3D pFirstPnt,pSecondPnt;
	Vector3D vMov;
	if (m_lstSegDist.GetCount() > 0)
	{
		POSITION pos = m_lstSegDist.GetHeadPosition();
		for (;pos;)
		{
			SEGMENT_DIST *pSgDist = m_lstSegDist.GetNext(pos);
			vMov = pCryoDlg->GetPTMoveVectorInTracker(dCurPTPos);
			if(bInverse)
			{
				vMov = -vMov;
			}
			pFirstPnt = pSgDist->sg_segment->GetFirstEndpoint();
			pSecondPnt = pSgDist->sg_segment->GetSecondEndpoint();
			pFirstPnt = pFirstPnt + vMov;
			pSecondPnt = pSecondPnt + vMov;
			pSgDist->sg_segment->Construct(pFirstPnt,pSecondPnt);

			pSgDist->sg_label.vPosition = pSgDist->sg_label.vPosition + vMov;
		}
	}

}

void CIMRIScene::SetCross(Vector3D p1, Vector3D p2, Vector3D p3, Vector3D p4)
{
	CColorF pColor(0.0, 1.0, 0.0, 1.0);
	m_crossLine[0].SetColor(&pColor);
	m_crossLine[1].SetColor(&pColor);
	m_crossLine[0].Construct(p1,p2);
	m_crossLine[1].Construct(p3,p4);
	m_crossLine[0].m_bDisableDepthTest=FALSE;
	m_crossLine[1].m_bDisableDepthTest=FALSE;

	if(!m_bCrossDisplayed)
	{
		AddSegment(&m_crossLine[0],"cross 1");
		AddSegment(&m_crossLine[1],"cross 2");
		m_bCrossDisplayed=TRUE;
	}
}
