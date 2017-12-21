#ifndef _CCONFIGMGR_H
#define _CCONFIGMGR_H

#define  SYSINFO	    "System"
#define  SYS_HAS_CAMERA	"Camera"

#define  TRACK_DEVICE			"COM"
#define  TRACK_DEVICE_TYPE      "Tracker Type"
#define  COM_BAUD_RATE	"Baud Rate"
#define  COM_STOP_BITS	"Stop Bits"
#define  COM_PARITY		"Parity"
#define  COM_DATA_BITS	"Data Bits"
#define  COM_HARDWARE	"Hardware Handshaking"
#define  COM_PORT		"Port"
#define  COM_RESET		"Rest Hardware"
#define  COM_WIRELESS   "Wireless"
#define  COM_GETTXIGNOREERROR	"Get TX ignore error"

void SetINIFileName(char* pFileName);

class CConfigMgr
{
public:
	CConfigMgr(void);

	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, int nValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, float fValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, double dbValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszValue, LPCTSTR pFile = NULL);

	int Read(LPCTSTR pszSession, LPCTSTR pszEntry, int nDefault=0, LPCTSTR pFileName = NULL);
	float Read(LPCTSTR pszSession, LPCTSTR pszEntry, float nfefault=1.0, LPCTSTR pFile = NULL);
	CString Read(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL, LPCTSTR pFile = NULL);
	double Read(LPCTSTR pszSession, LPCTSTR pszEntry, double dbDefault=0.0, LPCTSTR pFile = NULL);

	CString ReadREG(HKEY hKey, LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL);
private:
};
#endif
