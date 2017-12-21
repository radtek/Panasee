/*

VTK_FREETYPE_CHANGE this file is new for VTK.

*/

#ifndef _freetypeConfig_h
#define _freetypeConfig_h

#if defined(__BORLANDC__)
#pragma warn -8004 /* "assigned a value that is never used" */
#pragma warn -8012 /* "comparing signed and unsigned values" */
#pragma warn -8060 /* "possibly incorrect assignment" */
#endif

/* 
   Static or shared lib flags
*/
/* #undef VTKFREETYPE_DLL */
#define VTKFREETYPE_STATIC

/* 
   HPUX workaround
*/
/* #undef VTKFREETYPE_HPUX_SOURCE */
#if defined(VTKFREETYPE_HPUX_SOURCE) && !defined(_HPUX_SOURCE)
#define _HPUX_SOURCE
#endif

/* 
   Enable or disable freetype's Mac font support.
   DARWIN_NO_CARBON is the freetype define that makes the choice.
*/
/* #undef VTKFREETYPE_MAC_FONT_SUPPORT */
#if !defined(VTKFREETYPE_MAC_FONT_SUPPORT) && !defined(DARWIN_NO_CARBON)
#define DARWIN_NO_CARBON
#endif

#define FT2_BUILD_LIBRARY 1

#endif
