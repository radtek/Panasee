// CrashRestore.h 
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(__RESTORE_H__)
#define __RESTORE_H__

enum CRASH_INFO
{
	CI_IMAGES,
	CI_DISPLAY,
	CI_PROBES,
	CI_SCANNER
};

#define PROBES_RST_FILE	"\\Probes.rst"
#define IMAGE_RST_FILE  "\\Images.rst"

#define SYS_RESTORE		"Sys"
#define RESTORED		"Restored from crash"


#define RESTORE_IMG		"Images"
//#define IMG_NUM			"Num"
//#define IMG_NAME		"Name "

#define RESTORE_DISP	"Display"
#define DISPLAY			"Display "

#define RESTORE_PROBE	"Probe"


#endif

