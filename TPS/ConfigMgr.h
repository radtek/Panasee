#ifndef _CCONFIGMGR_H
#define _CCONFIGMGR_H

#define TPS_ICEBALL "Ice Ball"
#define TPS_ICEBALL_SHELL1_COLOR "Shell1 Color"
#define TPS_ICEBALL_SHELL2_COLOR "Shell2 Color"
#define TPS_ICEBALL_SHELL3_COLOR "Shell3 Color"
#define TPS_ICEBALL_DISPLAY_INTERSECT "Intersect Default"

#define  GPU	    "GPU"
#define  GPU_MEMORY_SIZE "MemorySize"

class CConfigMgr
{
public:
	COLORREF Read(LPCTSTR pszSession, LPCTSTR pszEntry,COLORREF  color);
	CConfigMgr(void);

	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, int nValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, float fValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, double dbValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszValue);

	int Read(LPCTSTR pszSession, LPCTSTR pszEntry, int nDefault=0);
	float Read(LPCTSTR pszSession, LPCTSTR pszEntry, float nfefault=1.0);
	CString Read(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL);
	double Read(LPCTSTR pszSession, LPCTSTR pszEntry, double dbDefault=0.0);
	
	CString ReadREG(HKEY hKey, LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault /*=NULL*/);
private:
	char pszStringValue[256];
	char pszFileName[MAX_PATH]; //文件全路径
	CString cFileName; //文件名
	void GetINIFileName();
};


#endif
