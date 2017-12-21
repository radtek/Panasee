
#if !defined(_TPS_FRMMGR_H_)
#define _TPS_FRMMGR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPSFrmMgr.h : header file
//

#include "FrmMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CTPSFrmMgr command target

class AFX_EXT_CLASS CTPSFrmMgr : public CFrmMgr
{
public:
	void GetProbeInfo(int &count,double *pProbeData);
	void UpdateProbeInfo(double *tipPos,double *ori);

	CTPSFrmMgr();           // protected constructor used by dynamic creation
	virtual ~CTPSFrmMgr();
	
	// Implementation

private:	
	virtual void CreateContents();
	
};

/////////////////////////////////////////////////////////////////////////////

#endif // _TPS_FRMMGR_H_
