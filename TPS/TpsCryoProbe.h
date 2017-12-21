// TpsCryoProbe.h: interface for the CProbeCryo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPSCRYOPROBE_H__F6C793EB_7BA1_4E50_B771_4E982321B642__INCLUDED_)
#define AFX_TPSCRYOPROBE_H__F6C793EB_7BA1_4E50_B771_4E982321B642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TpsGlobalConst.h"
#include "TpsTool.h"
#include "ExtendVTKClass.h"
#include "GalilIceBallDistribute.h"

class CProbeCryo : public CProbe
{
public:
	CWnd *m_pMsgWindow;
	//status=0 : change tip position, don't change end position
	//status=1 :change end position,don't change tip position
	void ChangePositionByTarget(double *pos,int status);
	void SetColor(double color[3]);

	void SetColor(double r,double g,double b);

	void RestoreColor();
		
	void UpdateProbeColor();
	void UpdateProbeCoord(BOOL bSendProbeMovMessage=TRUE);
	void HideAxes();
	void DisplayAxes();
	void GetTipPosInWorld(double *pos);
	void SetInitPosition(double pos[3]);
	void AddPositions(double pTipPos[3]);
	static CProbeCryo *New()
	{
		return new CProbeCryo;
	}


	ToolAssembly();
	int GetTipPos(double* pTipPos);
	void Initial(NEEDLE_INFO eNeedleType);
	CBallIceGalil* m_pAttachIceBall;
	BOOL m_bCreateBall;
	NEEDLE_INFO m_eNeedleType;
	float m_fDiamter;
	float m_fLength;
	double m_dColor[2][3];
	vtkActor *probe1,*probe2,*probe3,*probe4;
	CCrossLine *cross1,*cross2,*cross3;

	POLYTYPE m_polytype;

	void SetUCS(BOOL flag)
	{
		if(flag)
		{
			m_ucsFlag=flag;
			DisplayAxes();
		}else
		{
			m_ucsFlag=flag;
			HideAxes();
		}
	}
	BOOL GetUCS(){return m_ucsFlag;}
	
protected:
	CProbeCryo();
	virtual ~CProbeCryo();
private:
	void LinePlaneIntersection(double xl, double yl, double zl, double nlx, double nly, double nlz, double npx, double npy, double npz, double d, double *xi, double *yi, double *zi);
	void CreateAxis();
	void CreateDose();

	float m_fTipLength;
	int m_iResolution;
	BOOL m_ucsFlag; //ucs: user coordicate system as reference
		
	CBallIceGalil* m_pIceBall;
	double m_position[3];
	vtkActor          *XAxisShaft;
	vtkActor          *YAxisShaft;
	vtkActor          *ZAxisShaft;

	vtkActor          *XAxisTip;
	vtkActor          *YAxisTip;
	vtkActor          *ZAxisTip;

	double m_SegPoint[2][3]; //two end points on probe.
	double m_probeColor[4][3];
};

#endif // !defined(AFX_TPSCRYOPROBE_H__F6C793EB_7BA1_4E50_B771_4E982321B642__INCLUDED_)
