#ifndef _CCONFIGMGR_H
#define _CCONFIGMGR_H

#define  COM			"COM"
#define  COM_BAUD_RATE	"Baud Rate"
#define  COM_STOP_BITS	"Stop Bits"
#define  COM_PARITY		"Parity"
#define  COM_DATA_BITS	"Data Bits"
#define  COM_HARDWARE	"Hardware Handshaking"
#define  COM_PORT		"Port"
#define  COM_RESET		"Rest Hardware"

#define SROM_POLA			"POLARIS SROM Image Files"
#define SROM_AURO			"AURORA SROM Image Files"

void SetINIFileName(char* pFileName);

class CConfigMgr
{
public:
	CConfigMgr(void);

	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, int nValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, float fValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, double dbValue);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszValue);

	int Read(LPCTSTR pszSession, LPCTSTR pszEntry, int nDefault=0);
	float Read(LPCTSTR pszSession, LPCTSTR pszEntry, float nfefault=1.0);
	CString Read(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL);
	double Read(LPCTSTR pszSession, LPCTSTR pszEntry, double dbDefault=0.0);

private:
};
#endif
