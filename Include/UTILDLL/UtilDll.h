// UtilDll.h :
//

#ifndef _UTILDLL_H_
#define _UTILDLL_H_


#ifdef _BUILD_UTIL_DLL
	#define UTILDLL_API		__declspec(dllexport)
#else
	#define UTILDLL_API		__declspec(dllimport)

//////////////////////////////////////////////////////
// setup the library we need to link with...
#if defined(_DEBUG)
#pragma comment(lib, "UtilDllD.lib" )
#else
#pragma comment(lib, "UtilDll.lib" )
#endif

#endif

#endif //_UTILDLL_H_
