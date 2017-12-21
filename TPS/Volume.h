// Volume.h: interface for the CVolume class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLUME_H__A68B8EB3_0612_4A91_ABBC_86E5B583FA1C__INCLUDED_)
#define AFX_VOLUME_H__A68B8EB3_0612_4A91_ABBC_86E5B583FA1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalDef.h"
#include "vtkImageData.h"
#include "vtkVolume.h"
#include "vtkRenderWindow.h"

class CVolume  
{
public:
	void SetGPUMemorySize(int memSize);
	
	void SetVolumeInfo(VOLRENDER_CONFIG *pConfig);
	vtkVolume* GetVolume();
	void SetImage(vtkImageData *pImage);

	CVolume();
	virtual ~CVolume();

	vtkRenderWindow* m_pRenderWindow;
private:
	void Update();

	vtkVolume *m_volume;

	vtkImageData *m_pImage;
	VOLRENDER_CONFIG m_config;

	int m_iGPUMemorySize; //default size is 60 Mb.

};

#endif // !defined(AFX_VOLUME_H__A68B8EB3_0612_4A91_ABBC_86E5B583FA1C__INCLUDED_)
