// VImriUlServerApp.h: interface for the CVImriUlServerApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIMRIULSERVERAPP_H__D1417DFE_5F47_4DAB_B4C5_42F219D173E1__INCLUDED_)
#define AFX_VIMRIULSERVERAPP_H__D1417DFE_5F47_4DAB_B4C5_42F219D173E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImriDefine.h"

class AFX_EXT_CLASS CVImriUlServerApp  
{
public:
	CVImriUlServerApp();
	virtual ~CVImriUlServerApp();

	virtual int	 Imri_DecodeImages(LPBYTE lpBuff, int nDataLen) {return 0;};

	virtual int  Imri_DealCriticalMsg(LPBYTE lpBuff) {return 0;};
	virtual int  Imri_DealConfirmMsg(LPBYTE lpBuff) {return 0;};
	virtual int  Imri_DealWarningMsg(LPBYTE lpBuff) {return 0;};
	virtual int  Imri_DealErrorMsg(LPBYTE lpBuff) {return 0;};

	virtual int  Imri_DealStatus(ENUM_MRI_SYS_STATUS nSysStatus, 
		unsigned short nSeriesStatus, int nOperResult) {return 0;};
	virtual int  Imri_DealProtocolGrps(ENUM_INFOR_RESULT eRet, CList<IMRI_PROTOCOL_GROUP*, IMRI_PROTOCOL_GROUP*> &lstprotocolGrp) {return 0;};
	virtual int  Imri_DealProtocolFamily(ENUM_INFOR_RESULT eRet, CList<IMRI_PROTOCOL_FAMILY, IMRI_PROTOCOL_FAMILY> &lstprotocolGrp) {return 0;};
	virtual int  Imri_DealModality(ENUM_INFOR_RESULT eRet, IMRI_MODALITY &modality) {return 0;};
	virtual int  Imri_DealPatient(ENUM_INFOR_RESULT eRet, IMRI_PATIENT &patient) {return 0;};
	virtual int  Imri_DealStudy(ENUM_INFOR_RESULT eRet, IMRI_STUDY &study) {return 0;};
	virtual int  Imri_DealShimSNR(ENUM_INFOR_RESULT eRet, IMRI_SHIM_SNR_TUNING_INFO *pShimSNR) {return 0;};
};

#endif // !defined(AFX_VIMRIULSERVERAPP_H__D1417DFE_5F47_4DAB_B4C5_42F219D173E1__INCLUDED_)
