// ModalDrag.h: interface for the CModalDrag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODALDRAG_H__666900D6_B1DA_4C2D_8B37_68CCFAF033B1__INCLUDED_)
#define AFX_MODALDRAG_H__666900D6_B1DA_4C2D_8B37_68CCFAF033B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "modal.h"

class CTPS3DView;

class CModalDrag  :public CModal
{
public:
	CModalDrag();
	virtual ~CModalDrag();

	void create();

	virtual void clear();
	virtual BOOL readContent(FILE* file,CTPS3DView *p3DView);
	virtual BOOL save(FILE *file);
	
	
	vtkPoints *m_pPoints;
};

#endif // !defined(AFX_MODALDRAG_H__666900D6_B1DA_4C2D_8B37_68CCFAF033B1__INCLUDED_)
