//ImagePlus.h :
//

#ifndef _IMAGEPLUS_H_
#define _IMAGEPLUS_H_

#ifdef _BUILD_IMAGEPLUS_DLL
	#define IMAGEPLUS_API		__declspec(dllexport)
#else
	#define IMAGEPLUS_API		__declspec(dllimport)

//////////////////////////////////////////////////////
// setup the library we need to link with...
#if defined(_DEBUG)
#pragma comment(lib, "ImagePlusD.lib" )
#else
#pragma comment(lib, "ImagePlus.lib" )
#endif

#endif

#endif //_IMAGEPLUS_H_
