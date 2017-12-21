// LicApi.h: interface for the CLicApi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICAPI_H__BE932026_EBFC_44F8_972D_961D8DF8B8A0__INCLUDED_)
#define AFX_LICAPI_H__BE932026_EBFC_44F8_972D_961D8DF8B8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h> 

class CLicApi  
{
public:
	BOOL Validate(CString sSoftwareName);
	CLicApi();
	virtual ~CLicApi();

	
private:
	void GetCurTime();
	BOOL ValidateReal(CString sProduct);
	void GetRegInfo(CString sReg);
	void GetHardInfo(CString sMachine);
	BOOL ValidateFinal(CString sProduct);
	BOOL Validate1(CString sSoftwareName);
	BOOL DeadCircle1();
	BOOL DeadCircle2();
	BOOL DeadCircle3();
	BOOL DeadCircle4();

	
	char m_sHarddiskSerial[100];
	char m_sMacAddress[100];
	char m_sVersion[50];
	char m_sCurTime[20];
	char m_sProductPath[200];

	BOOL m_bResult;
	BOOL m_bIsMessaged;
	CString m_sSoftwareName;

};

#endif // !defined(AFX_LICAPI_H__BE932026_EBFC_44F8_972D_961D8DF8B8A0__INCLUDED_)
