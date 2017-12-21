// Config.h 
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(__CONFIG_H__)
#define __CONFIG_H__

extern CString SROM_FILES[];

enum ENUM_NDI_OPTIC_TRACKING_HANDLE
{
	TH_NDI_OPTIC_BASE1 = 1,
	TH_NDI_OPTIC_BASE2 = 2,
	TH_NDI_OPTIC_BASE3 = 3,
	TH_NDI_OPTIC_BASE4 = 4,
	TH_NDI_OPTIC_PHANTOM = 5,
	TH_NDI_OPTIC_PROBE = 6,
	TH_NDI_OPTIC_MARKER = 7
};

enum ENUM_NDI_EM_TRACKING_HANDLE
{
	TH_NDI_EM_PROBE = 0,
	TH_NDI_EM_MARKER = 1
};

static int SROM_FILE_NUM = 7;

extern CString CONFIG_FILES[];

// Following folder need to be created.
extern CString CONFIG_FOLDER[];

enum ENUM_CONFIG_FILE 
{
	CF_BASE1	= 0,
	CF_BASE2	= 1,
	CF_BASE3	= 2,
	CF_BASE4	= 3,
	CF_PHANTOM	= 4,
	CF_PT_DIR	= 5,
	CF_PROBE_PREIMAGE	= 6,
	CF_PROBE_CALI		= 7,
	CF_PROBE_CRYOTYPE	= 8,
	CF_PROBE_BIOTYPE	= 9,
	CF_MARKER2PT =10,
	CF_CONFIG_FILE_SUM
};

#endif

