// CrashRstore.h: interface for the CCrashRstore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRASHRSTORE_H__9FA0BEB2_5146_487F_863C_51A6EA110751__INCLUDED_)
#define AFX_CRASHRSTORE_H__9FA0BEB2_5146_487F_863C_51A6EA110751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Restore.h"

class CCrashRstore  
{
public:
	CCrashRstore();
	virtual ~CCrashRstore();

	static void RestoreCrashState();
	static void SaveCrashState(CRASH_INFO eCurSaveType);
	static void Clean();
	static BOOL IsRestoredFromCrash();
	static BOOL IsUserConfirmedRestore();
	static void UserConfirmRestore(BOOL bRestore);

	static void SaveCrashStateProbe(BOOL bClean = FALSE);
	static void SaveCrashStateImage(BOOL bClean = FALSE);

private:
	static BOOL m_bRestoredFromCrash;
};

#endif // !defined(AFX_CRASHRSTORE_H__9FA0BEB2_5146_487F_863C_51A6EA110751__INCLUDED_)
