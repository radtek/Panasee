
#ifndef _PANASKINDLL_H_
#define _PANASKINDLL_H_

#ifdef _BUILD_PANASKIN_DLL
	#define PANASKIN_API		__declspec(dllexport)
#else
	#define PANASKIN_API		 __declspec(dllimport)


	//////////////////////////////////////////////////////
	// setup the library we need to link with...
	#if defined(_DEBUG)
		#pragma comment(lib, "PanaSkinD.lib" )
	#else
		#pragma comment(lib, "PanaSkin.lib" )
	#endif

#endif //_BUILD_PANASKIN_DLL

#endif //_PANASKINDLL_H_