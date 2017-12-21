// TpsTool.h: interface for the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPSTOOL_H__2444C5C4_F858_4622_8AB9_3E2DB7EB2B8F__INCLUDED_)
#define AFX_TPSTOOL_H__2444C5C4_F858_4622_8AB9_3E2DB7EB2B8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VtkHeader.h"

class CProbe: public vtkAssembly
{
public:
	/*static CProbe *New()
	{
		return new CProbe;
	}*/
	
	virtual ToolAssembly();
	Initial();
	CString m_name;
	vtkBoxWidget *m_boxWidget;
protected:
	CProbe();
	virtual ~CProbe();
};

#endif // !defined(AFX_TPSTOOL_H__2444C5C4_F858_4622_8AB9_3E2DB7EB2B8F__INCLUDED_)
