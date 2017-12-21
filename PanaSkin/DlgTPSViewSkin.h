// DlgTPSViewSkin.h: interface for the CDlgTPSViewSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGTPSVIEWSKIN_H__213EEB2C_8E36_4225_9B03_9F7D0CAEE8CD__INCLUDED_)
#define AFX_DLGTPSVIEWSKIN_H__213EEB2C_8E36_4225_9B03_9F7D0CAEE8CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panaskin.h"
#include "MemDC.h"
#include "ImageButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTPSSkin dialog


class PANASKIN_API CDlgTPSViewSkin  
{
public:
	CDlgTPSViewSkin();
	virtual ~CDlgTPSViewSkin();

	void SetFirstImageBtn(CImageButton *pBtn);
	void SetLastImageBtn(CImageButton *pBtn);
	void SetPreImageBtn(CImageButton *pBtn);
	void SetNextImageBtn(CImageButton *pBtn);
	void Set3DShowImageBtn(CImageButton *pBtn);
	void SetTopWndBtn(CImageButton *pBtn);
	void SetSelectImageBtn(CImageButton *pBtn,int iIndex);
};

#endif // !defined(AFX_DLGTPSVIEWSKIN_H__213EEB2C_8E36_4225_9B03_9F7D0CAEE8CD__INCLUDED_)
