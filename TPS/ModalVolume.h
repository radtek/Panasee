// ModalVolume.h: interface for the CModalVolume class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODALVOLUME_H__C9C80126_782C_49BE_B6DC_8968DE80E70B__INCLUDED_)
#define AFX_MODALVOLUME_H__C9C80126_782C_49BE_B6DC_8968DE80E70B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "globalDef.h"
#include "modal.h"
#include "vtkVolume.h"
class CTPS3DView;

class CModalVolume  :public CModal 
{
public:
	BOOL AddVolume(int iIndex,VOLRENDER_CONFIG config,BOOL bResetName);

	CModalVolume();
	virtual ~CModalVolume();

	virtual void clear();
	virtual BOOL readContent(FILE* file);
	virtual BOOL save(FILE *file);

	vtkRenderWindow* m_pRenderWindow;
	vtkImageData* m_pImage;

private:
	int m_iImageIndex;
	VOLRENDER_CONFIG m_config;
	CString GetValidName();

	int m_iGPUMemorySize;
};

#endif // !defined(AFX_MODALVOLUME_H__C9C80126_782C_49BE_B6DC_8968DE80E70B__INCLUDED_)
