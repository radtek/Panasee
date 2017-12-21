#ifndef _CCOMMANDHANDLING_H
#define _CCOMMANDHANDLING_H

/******************************************************************/
#include <stdlib.h>
#include <afxtempl.h>
#include "APIStructures.h"

/*****************************************************************
Defines   
*****************************************************************/
#define NUM_COM_PORTS		10	/* number of com ports */
#define POLARIS_SYSTEM		1	/* type of system, POLARIS */
#define AURORA_SYSTEM		2	/* or AURORA */
#define ACCEDO_SYSTEM		3	/* or ACCEDO */
#define VICRA_SYSTEM		4	/* or VICRA */
#define SPECTRA_SYSTEM	    5	/* or SPECTRA */

/*****************************************************************
Structures
*****************************************************************/
/* NONE */

/*****************************************************************
Routine Definitions
*****************************************************************/
class Comm32Port;

class _declspec(dllexport) CCommandHandling
{
public:
	CCommandHandling();
	virtual ~CCommandHandling();

	void SetINIFileName(char* pFileName);

	int nCloseComPorts();
	int nOpenComPort( int nPort );
	int nHardWareReset(bool bWireless);
	int nSetSystemComParms( int nBaudRate, 
						    int nDataBits, 
						    int nParity, 
						    int nStopBits, 
						    int nHardware);
	int nSetCompCommParms(int  nBaud,
		 				  int nDataBits,
						  int nParity,
						  int nStop,
						  int nFlowControl);
	int nBeepSystem( int nBeeps );
	int nInitializeSystem();
	int nSetFiringRate();
	int nGetSystemInfo();
	int nInitializeAllPorts();
	int nInitializeHandle( int nHandle );
	int nEnableAllPorts();
	int nEnableOnePorts( int nPortHandle );
	int nDisablePort( int nPortHandle );
	int nActivateAllPorts();
	int nLoadTTCFG( char * pszPortID );
	int nGetHandleForPort( char * pszPortID );
	int nLoadVirtualSROM( char * pszFileName, 
					      char * pszPhysicalPortID, 
						  bool bPassive );
	int nFreePortHandles();
	int nGetPortInformation(int nPortHandle);
	int nStartTracking();
	int nGetTXTransforms(bool bReportOOV);
	int nGetBXTransforms(bool bReportOOV);
	int nStopTracking();
	int nGetAlerts(bool bNewAlerts);

	int nSelectVolume( int nVolumeIndex);

	void ErrorMessage();
	void WarningMessage();
	int CreateTimeoutTable();
	int nLookupTimeout( char *szCommand );

	int nSetSROMFiles(CArray<CString, CString> *arrSROMFiles);

/*****************************************************************
Variables
*****************************************************************/
	bool
		m_bLogToFile,				/* log to file */
		m_bDateTimeStampFile;		/* include date and time stamp in log file */
	char
		m_szLogFile[_MAX_PATH];		/* log file name */

	SystemInformation
		m_dtSystemInformation;		/* System Information variable - structure */

	HandleInformation
		m_dtHandleInformation[NO_HANDLES];	/* Handle Information varaible - structure */

	int
		m_nRefHandle; /* the handle for the tool acting as the reference tool */

	DiagNewAlertFlags
		m_dtNewAlerts; /* alert information */

	CMap<CString, LPCTSTR, int, int>
		m_dtTimeoutValues;

protected:
/*****************************************************************
Routine Definitions
*****************************************************************/
	void ApplyXfrms();
	int nSendMessage( char * pszCommand, bool bAddCRC );
	int nGetResponse();
	int nGetBinaryResponse( );
	int nVerifyResponse( char * pszReply, bool bCheckCRC );
	int nCheckResponse( int nResponse );
	void LogToFile(int nDirection,char *psz);

	void InitCrcTable();
	unsigned int CalcCrc16( unsigned int crc, int data );
	unsigned CalcCRCByLen( char *pszString, int nLen );
	int SystemCheckCRC(char *psz);
	unsigned int SystemGetCRC(char *psz, int nLength);

	int nAddCRCToCommand( char * pszCommandString );
	int nAddCRToCommand( char * pszCommandString );
	int nBuildCommand( char * pszCommandString, bool bAddCRC );

/*****************************************************************
Variables
*****************************************************************/
	Comm32Port
		*pCOMPort;					/* pointer to the Comm32 class */

	char
		m_szLastReply[MAX_REPLY_MSG],	/* Last reply received from the system */
		m_szCommand[MAX_COMMAND_MSG];		/* command to send to the system */
	bool
		m_bClearLogFile,				/* clear log file on intialization */
		m_bDisplayErrorsWhileTracking;	/* display the error while tracking */
	int
		m_nTimeout,
		m_nDefaultTimeout;						/* timeout value in seconds */
	bool
		bComPortOpen[NUM_COM_PORTS];	/* array of com ports - if true they are open */
	int
		m_nPortsEnabled;				/* the number of port enable by nEnableAllPorts */
	CString 
		m_strPassivePortSROMFiles[NO_HANDLES];
	BOOL m_bPassivePortSROMFilesOutSeted; 
};

#endif
/************************END OF FILE*****************************/


