// ModalDrag.cpp: implementation of the CModalDrag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModalDrag.h"
#include "TPS3DView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModalDrag::CModalDrag()
{
	m_actor=NULL;
	m_zType=ORGAN;
	m_pPoints=NULL;
	m_modalType=ModalDrag;
}

CModalDrag::~CModalDrag()
{
	clear();
}

void CModalDrag::clear()
{
	
	if(m_pPoints)
	{
		m_pPoints->Delete();
	}

	m_actor=NULL;
	m_pPoints=NULL;
}

void CModalDrag::create()
{
	clear();
	
	m_pPoints=vtkPoints::New();
}

BOOL CModalDrag::save(FILE *file)
{
	if(!m_pPoints)
		return FALSE;

	if(!CModal::save(file))
		return FALSE;

	MODAL_INFO modalGroup;
	modalGroup.color=m_color;
	sprintf(modalGroup.name,"%s",m_name);
	modalGroup.Opacity=m_opacity;
	modalGroup.treeItem=m_treeItem;
	modalGroup.volume=m_volume;
	modalGroup.zType=m_zType;
	
	
	fwrite(&modalGroup,sizeof(MODAL_INFO),1,file);
	
	
	//save trace path
	
	double *dPoints,*dPoint;
	
	
	int num = m_pPoints->GetNumberOfPoints();
	fwrite(&num,sizeof(int),1,file);
	dPoints=new double[num*3];
	for(int j=0;j<num;j++)
	{
		dPoint = m_pPoints->GetPoint(j);
		dPoints[j*3]=dPoint[0];
		dPoints[j*3+1]=dPoint[1];
		dPoints[j*3+2]=dPoint[2];
	}
	fwrite(dPoints,sizeof(double),num*3,file);
	delete dPoints;
		
	return TRUE;
}


BOOL CModalDrag::readContent(FILE *file,CTPS3DView *p3DView)
{
	
	MODAL_INFO modalGroup;
	int iReadCount=fread(&modalGroup,sizeof(MODAL_INFO),1,file);
	if(iReadCount!=1)
	{		
		return FALSE;
	}
	
	m_color=modalGroup.color;
	m_name.Format("%s",modalGroup.name);
	m_opacity=modalGroup.Opacity;
	m_treeItem=modalGroup.treeItem;
	m_volume=modalGroup.volume;
	m_zType=modalGroup.zType;

	//read modal point
	double *dPoints;	
	int num=0;

	iReadCount=fread(&num,sizeof(int),1,file);
	if(iReadCount!=1 || num<=0)
	{				
		return FALSE;
	}
	dPoints=new double[num*3];
	iReadCount=fread(dPoints,sizeof(double),num*3,file);
	if(iReadCount!=num*3)
	{				
		delete []dPoints;				
		return FALSE;
	}
	
	//create modal	
	vtkPoints * pTmpPoint = vtkPoints::New();
	double data[3];
	for(int j=0;j<num;j++)
	{
		data[0]=dPoints[j*3];
		data[1]=dPoints[j*3+1];
		data[2]=dPoints[j*3+2];
		pTmpPoint->InsertNextPoint(data);
	}
	if(p3DView!=NULL)
		p3DView->CreateModal(this,pTmpPoint);
	
	pTmpPoint->Delete();
	
	delete []dPoints;
	
	return TRUE;
}
