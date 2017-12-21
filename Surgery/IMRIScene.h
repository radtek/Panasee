// IMRIScene.h: interface for the CIMRIScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMRISCENE_H__B23FBD73_B4E3_4533_BF25_F9637ADC4C26__INCLUDED_)
#define AFX_IMRISCENE_H__B23FBD73_B4E3_4533_BF25_F9637ADC4C26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "scene.h"
#include "Box.h"
#include "Geometry.h"
#include "MV.h"
#include "USRectangle.h"

class CProbe;
class CMRIRectangle;
class CVirtualTarget;

class CIMRIScene : public CScene
{
public:
	void SetCross(Vector3D p1,Vector3D p2,Vector3D p3,Vector3D p4);
	Vector3D GetSDir() {return m_vSInWorld;};
	Vector3D GetLDir() {return m_vLInWorld;};

	void AddLine(Vector3D vP1, Vector3D vP2);
	void RemoveAllLine();

	void AddVirtualTarget(CVirtualTarget *pVTarget);
	void RemoveVirtualTarget(CVirtualTarget *pVTarget);

	void AddModelMesh(CMesh *pMesh);
	void RemoveAllDistMeasurement();
	void ResetDistMeasure();
	void AddDistMeasurePoint(Vector3D vPoint);
	
	/*���õ�ǰ��ʾ��ͼ�����*/
	void SetRectIndex(int iIndex);
	BOOL BoxFrameIsVisible();
	// pBoxChar is 6 size array representing X+, X-, Y+, Y-, Z+, Z-
	void UpdateFrameBox(char* pBoxLabel, Matrix* pMatrix , Vector3D* vMov, BOOL bKeepEyrDirToBox = TRUE);
	void ShowBoxFrame(BOOL bShow);
	void TransferBoxFrame(Matrix* pMatrix);
	void TranslationFrameBox(Vector3D vMov);
	void SetBoxFrameLabel(char* pBoxChar); // pBoxChar is 6 size array representing X+, X-, Y+, Y-, Z+, Z-
	CIMRIScene();
	virtual ~CIMRIScene();

	void AddUSRect(CUSRectangle *pUSRect, BOOL bKeepView);
	void RemoveUSRect(CUSRectangle *pUSRect);

	void AddProbe(CProbe *pProbe);
	void RemoveProbe(CProbe *pProbe);

	void AddMRIRect(CMRIRectangle *pMRIRect, BOOL bKeepView = FALSE);
	void RemoveMRIRect(CMRIRectangle *pMRIRect);

	void ChangeViewToRect(CRectangle* pRect);
	void ChangeViewToUSRect(CUSRectangle* pUSRect,BOOL bChangeDistance =TRUE);
	void ChangeViewToMRIRect(CMRIRectangle* pMRIRect,BOOL bChangeDistance = TRUE);
	void ChangeViewToProbe(CProbe* pProbe);

	void ResetView();   // This is used for 3d Window

	BOOL m_b3DDisplay; /*if is a 3D window*/
	CString GetBoxLabel();

	

	// Select virtual target
	int SelectVirtualTargetByClicking(CList<CVirtualTarget*, CVirtualTarget*>* plstVirtualTarget, CPoint point);

	void ShowSphereInVT(BOOL bShow) { m_bShowSphereInVT=bShow;};

	/*���������ݵ�λ�ý��б任*/
	void TransformAllDistanceSegment(double** ppdtran);
	void TranslateAllDistanceSegment(Vector3D vMov);
	/*pCryoWnd������CCryotherapyDlg����
	  ���ݲ����㱣���Ĵ���ʱ����λ��,���㲡��λ��*/
	void TranslateAllDistanceSegment(CWnd* pCryoWnd,double dCurPTPos,BOOL bInverse);
private:
	void ChangeViewToRect(CRectangle* pRect,char** sOrientChar,	double** ppdCharPosition,BOOL bChangeDistance);

	struct SEGMENT_DIST {
		CSegment *sg_segment;
		//CCylinder *sg_segment;
		ST_TEXT  sg_label;
		double   sg_dbDist;
		
		CString  sg_sName;
		int      iIndex;  /*����λ����һ��ͼ����*/
	};

	void CleanDistList();
	void AddDistance(Vector3D vP1, Vector3D vP2);
	ST_TEXT m_stTextAPSI[6];
	void ChangeViewToBoxFrame(Matrix *pMatrix = NULL, bool bKeepOrigEyeToGazeDir = FALSE);;
	BOOL m_bBoxFrameVisible;
	CBox m_BoxFrame;
	CBox m_BoxFrameBackup;

	CList<CSegment*,CSegment*> m_lstSegment;
	CList<SEGMENT_DIST*, SEGMENT_DIST*> m_lstSegDist;
	PointCloud *pcDistA;
	PointCloud *pcDistB;
	BOOL m_bPointAAdded;
	BOOL m_bPointBAdded;
	int m_iDistClickNum;

	Vector3D m_vLInWorld;   // Box frame L side direction   
	Vector3D m_vSInWorld;   // Box frame S side direction
	Vector3D m_vBoxFrameLabelPos[6]; // X+, X-, Y+, Y-, Z+, Z-
	Vector3D m_vAPSIOri[6]; // X+, X-, Y+, Y-, Z+, Z-

	Matrix m_mTransferMatrix;	

	/*�Ƿ���ʾ��VirtualTarget�ڲ�����,�������ɫ
	  ��ʾVTλ��ͼ���ǰ�滹�Ǻ���
	*/
	BOOL m_bShowSphereInVT; 

	
	int m_iImageIndex; /*��ǰ��ʾ��ͼ������*/

	CSegment m_crossLine[2];
	BOOL     m_bCrossDisplayed;
};

#endif // !defined(AFX_IMRISCENE_H__B23FBD73_B4E3_4533_BF25_F9637ADC4C26__INCLUDED_)
