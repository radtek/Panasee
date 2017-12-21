// Ruler.h: interface for the CRuler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULER_H__E819C250_8C9F_4B29_A8F5_C9BB71FA16C3__INCLUDED_)
#define AFX_RULER_H__E819C250_8C9F_4B29_A8F5_C9BB71FA16C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\Geometry.h"
#include "Afxwin.h"

class AFX_EXT_CLASS CRuler  
{
public:
	CRuler();
	virtual ~CRuler();
	
	Vector3D m_pnt1,m_pnt2;
	BOOL m_bVisible;
	UINT m_iSeprateCount;
	CString m_sTitle;
	CFont *m_pFont;
};

#endif // !defined(AFX_RULER_H__E819C250_8C9F_4B29_A8F5_C9BB71FA16C3__INCLUDED_)
