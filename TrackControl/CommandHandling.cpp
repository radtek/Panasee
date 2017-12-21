/*****************************************************************
Name:			CommandHandling

Description:    This class controls all sending and recieving of
				communication with the system.  It also parses
				all the responses from the system and validates
				the responses.


All Northern Digital Inc. ("NDI") Media and/or Sample Code and/or
Sample Code Documentation (collectively referred to as "Sample Code")
is licensed and provided "as is" without warranty of any kind.  The
licensee, by use of the Sample Code, warrants to NDI that the Sample
Code is fit for the use and purpose for which the licensee intends to
use the Sample Code. NDI makes no warranties, express or implied, that
the functions contained in the Sample Code will meet the licensee’s
requirements or that the operation of the programs contained therein
will be error free.  This warranty as expressed herein is exclusive
and NDI expressly disclaims any and all express and/or implied, in fact
or in law, warranties, representations, and conditions of every kind
pertaining in any way to the Sample Code licensed and provided by NDI
hereunder, including without limitation, each warranty and/or condition
of quality, merchantability, description, operation, adequacy,
suitability, fitness for particular purpose, title, interference with
use or enjoyment, and/or non infringement, whether express or implied
by statute, common law, usage of trade, course of dealing, custom, or
otherwise.  No NDI dealer, distributor, agent or employee is authorized
to make any modification or addition to this warranty.

In no event shall NDI nor any of its employees be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages,
sundry damages or any damages whatsoever, including, but not limited to,
procurement of substitute goods or services, loss of use, data or profits,
or business interruption, however caused.  In no event shall NDI’s
liability to the licensee exceed the amount paid by the licensee for the S
ample Code or any NDI products that accompany the Sample Code. The said
imitations and exclusions of liability shall apply whether or not any
such damages are construed as arising from a breach of a representation,
warranty, guarantee, covenant, obligation, condition or fundamental term
or on any theory of liability, whether in contract, strict liability, or
tort (including negligence or otherwise) arising in any way out of the
use of the Sample Code even if advised of the possibility of such damage.
In no event shall NDI be liable for any claims, losses, damages, judgments,
costs, awards, expenses or liabilities of any kind whatsoever arising
directly or indirectly from any injury to person or property, arising
from the Sample Code or any use thereof.


Copyright (C) 2002, 2003, Northern Digital Inc. All rights reserved.


*****************************************************************/


/*****************************************************************
C Library Files Included
*****************************************************************/
/* none */

/*****************************************************************
Project Files Included
*****************************************************************/
#include "stdafx.h"
#include "CommandHandling.h"
#include "Conversions.h"
#include "INIFileRW.h"
#include "ComPortTimeout.h"
#include "Comm32.h"

/*****************************************************************
Defines
*****************************************************************/
/* none */

/*****************************************************************
Global Variables
*****************************************************************/
/* none */


/*****************************************************************
Name:			CCommandHandling	

Inputs:
	None.

Return Value:
	None.

Description:   CCommandHandling Constructor
*****************************************************************/
CCommandHandling::CCommandHandling()
{
	/* set up com port class, start from new. */
	pCOMPort = NULL;
	pCOMPort = new Comm32Port;
	/* reinitialize the com ports */
	for ( int i = 0; i < NUM_COM_PORTS; i++ )
		bComPortOpen[i] = false;
	/* set variables */
	m_bLogToFile =
	m_bDateTimeStampFile = FALSE;
	ReadINIParm( "Logging Options", "Log File Name", "", MAX_PATH, m_szLogFile );
	ReadINIParm( "Logging Options", "Log To File", "", &m_bLogToFile );
	ReadINIParm( "Logging Options", "Date Time Stamp", "", &m_bDateTimeStampFile );
	/*
	 * the only timeout that the default should have to be used for is a
	 * serial break...this is because of the face the you can't read the
	 * timeout values before the system in a communicating state
	 */
	ReadINIParm( "Communication", "Timeout Time", "3", &m_nDefaultTimeout );
	/* variable intialization */
 	m_bClearLogFile = FALSE;
	m_bDisplayErrorsWhileTracking = FALSE;

	m_nRefHandle = -1;
	m_nTimeout = 3;

	m_bPassivePortSROMFilesOutSeted = FALSE;
} /* CCommandHandling()

/*****************************************************************
Name:			~CCommandHandling	

Inputs:
	None.

Return Value:
	None.

Description:   CCommandHandling Destructor
*****************************************************************/
CCommandHandling::~CCommandHandling()
{
	/* clean up */
	if ( pCOMPort )
		delete( pCOMPort );
} /* ~CCommandHandling */

/*****************************************************************
Name:				nCloseComPorts

Inputs:
	None.

Return Value:
	int , 0 if fails and 1 is passes

Description: 
	This routine closes all open COM ports.   
*****************************************************************/
int CCommandHandling::nCloseComPorts()
{
	/*
	 * if the COM Port is already closed no need to check to see if 
	 * we should close it
	 */
	for ( int i = 0; i < NUM_COM_PORTS; i++ )
	{
		if ( (bComPortOpen[i]) && (pCOMPort != NULL) )
		{
			pCOMPort->SerialClose();
			bComPortOpen[i] = false;
			return 1;
		}/* if */
	}/* for */
	return 0;
} /* nCloseComPorts */

/*****************************************************************
Name:				nOpenComPort

Inputs:
	int nPort - the port number to be opened

Return Value:
	int - 1if successful, 0 otherwise

Description:   
	This routine opens the selected com port and sets its settings
	to the default communication parameters
*****************************************************************/
int CCommandHandling::nOpenComPort( int nPort )
{
	/*
	 * If the COM Port is open there is no sense in re-opening it.
	 * You can still change the PARAMETERS with no problem.  This
	 * reduces the time it takes to re-initialize the System
	 */
	if ( bComPortOpen[nPort] )
		return 1;
	else
	{
		if ( pCOMPort != NULL )
		{
			/* set the parameters to the defaults */
			if ( pCOMPort->SerialOpen( nPort, 9600, COMM_8N1, FALSE, 256 ) )
			{
				bComPortOpen[nPort] = TRUE;
				return 1;
			} /* if */ 
		} /* if */
	} /* else */

	return 0;
} /* nOpenComPort */
/*****************************************************************
Name:				

Inputs:
	None.

Return Value:
	int - 0 if it fails, nCheckResponse if passes

Description:   
	This routine sends a serial break to the system, reseting it.
*****************************************************************/
int CCommandHandling::nHardWareReset(bool bWireless)
{
	int
		nResponse = 0,
		nInitTO = 3; 

	/* Check COM port */
	if( pCOMPort == NULL )
	{
		return 0;
	}/* if */

	if( !bWireless )
	{
		/* send serial break */
		pCOMPort->SerialBreak();

		Sleep(500); /* Give the break sometime to set */

		memset(m_szCommand, 0, sizeof(m_szCommand));
		if (!nGetResponse( ))
		{
			return 0;
		}/* if */

		/* check for the RESET response */
		nResponse = nVerifyResponse(m_szLastReply, TRUE);
		if ( !nCheckResponse( nResponse ) )
		{
			return 0;
		}/* if */

		/*
		 * In order to support NDI enhanced Tool Interface Unit and Tool Docking Station,
		 * a time delay is recommended so that the Tool Docking Station
		 * can be recognised by the eTIU.
		 */
		Sleep(nInitTO * 1000);

		if ( nResponse & REPLY_RESET )
		{
			if (!SystemCheckCRC( m_szLastReply ) )
				return REPLY_BADCRC;
			else
				return nResponse;
		} /* if */
		else
		{
			return nResponse;
		}/* else */
	} /* if */
	else
	{
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "RESET 0" );
		if (nSendMessage( m_szCommand, TRUE ))
			if (nGetResponse( ))
				return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );
		return 0;
	} /* else */
} /* nHardwareReset */

/*****************************************************************
Name:				nSetSystemComParms

Inputs:
	int nBaudRate - the baud rate to set
	int nDateBits - the data bit setting
	int nParity - the parity setting
	int nStopBits - the stop bit setting
	int nHardware - whether or not to use hardware handshaking

Return Value:
	int - 0 if fails, else nCheckResponse

Description:   
	This routine sets the systems com port parameters, remember
	to immediatley set the computers com port settings after this
	routine is called.
*****************************************************************/
int CCommandHandling::nSetSystemComParms( int nBaudRate, 
										  int nDataBits, 
										  int nParity, 
										  int nStopBits, 
										  int nHardware)
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "COMM %d%d%d%d%d", nBaudRate, 
											nDataBits, 
											nParity, 
											nStopBits, 
											nHardware );
	if (nSendMessage( m_szCommand, TRUE ))
		if (nGetResponse( ))
			return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );

	return 0;
} /* nSetSystemComParms */
/*****************************************************************
Name:				nSetSystemComParms

Inputs:
	int nBaud - the baud rate to set
	int nDateBits - the data bit setting
	int nParity - the parity setting
	int nStop - the stop bit setting
	int nHardware - whether or not to use hardware handshaking

Return Value:
	int - 0 if fails, else 1

Description:   
	This routine sets the computer's com port parameters, remember
	to immediatley set the computer's com port settings after the 
	system's com port parameters.
*****************************************************************/
int CCommandHandling::nSetCompCommParms( int nBaud,
					 				     int nDataBits,
									     int nParity,
									     int nStop,
									     int nFlowControl)
{
	unsigned uFormat;

	/* Check COM port */
	if( pCOMPort == NULL )
	{
		return 0;
	}/* if */

	switch( nBaud )
	{
		case 0:
			nBaud = 9600;
			break;
		case 1:
			nBaud = 14400;
			break;
		case 2:
			nBaud = 19200;
			break;
		case 3:
			nBaud = 38400;
			break;
		case 4:
			nBaud = 57600;
			break;
		case 5:
			nBaud = 115200;
			break;
		case 6:
			nBaud = 921600;
			break;
		case 7:
			nBaud = 1228739;
			break;
		default:
			nBaud = 9600;
			break;
	} /* switch */
	
	uFormat = (nDataBits * 6) + (nParity * 2) + (nStop);
	
	if ( pCOMPort->SerialSetBaud( nBaud, uFormat, nFlowControl ? true : false, 256) )
		return 1;

	return 0;
} /* nSetCompCommParms */

/*****************************************************************
Name:				nBeepSystem

Inputs:
	int nBeeps - the number of times the system should beep

Return Value:
	int - 0 if fails, else nCheckResponse

Description:   
	This routine sends the beep command to the System, causing
	it to beep.
*****************************************************************/
int CCommandHandling::nBeepSystem( int nBeeps )
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "BEEP %d", nBeeps );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( nGetResponse( ) )
			return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );
	} /* if */

	return 0;
} /* nBeepSystem */

/*****************************************************************
Name:				nSelectVolume

Inputs:
	int nVolueIndex - the index of volume 
		1 - Polaris standard measurement volume
		2 - Polaris pyramid measurement volume
		3 - Polaris Vicra measurement volume

Return Value:
	int - 0 if fails, else REPLY_OKAY

Description:   
	This routine change the measurement volume
*****************************************************************/

int CCommandHandling::nSelectVolume( int nVolumeIndex)
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "VSEL %d", nVolumeIndex );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( nGetResponse( ) )
			return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );
	} /* if */

	return 0;	
}

/*****************************************************************
Name:				nInitializeSystem

Inputs:
	None.

Return Value:
	int - 0 if fails, else nCheckResponse

Description:   This routine initializes the System by sending the
			   INIT command.  Remember to reset the appropriate 
			   variables.
*****************************************************************/
int CCommandHandling::nInitializeSystem()
{
	FILE
		*pfOut = NULL;

	/* get the log file option settings */
	ReadINIParm( "Logging Options", "Log File Name", "", MAX_PATH, m_szLogFile );
	ReadINIParm( "Logging Options", "Clear File", "", &m_bClearLogFile );

	if ( m_bLogToFile && m_bClearLogFile )
	{
		if( m_szLogFile[0] != 0 )
		{
			pfOut = fopen(m_szLogFile,"w+t");
			if(pfOut == NULL)
			{
				m_bLogToFile = FALSE;
				WriteINIParm( "Logging Options", "Log To File", m_bLogToFile );
				MessageBox( NULL, "Failed to open log file.", "Log File Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
				return 0;
			}/* if */
			fclose(pfOut);
		}
		else
		{
			m_bLogToFile = FALSE;
			WriteINIParm( "Logging Options", "Log To File", m_bLogToFile );	
			m_bClearLogFile = FALSE;
			WriteINIParm( "Logging Options", "Clear File", m_bClearLogFile );

		}/* else */
	}/* if */

	/* clear the handle information */
	for ( int i = 0; i < NO_HANDLES; i++ )
	{
		/* EC-03-0071
		 sprintf( m_dtHandleInformation[i].szPhysicalPort, "" );
		 */
		memset(m_dtHandleInformation[i].szPhysicalPort, 0, 5);
		m_dtHandleInformation[i].HandleInfo.bInitialized = FALSE;
		m_dtHandleInformation[i].HandleInfo.bEnabled = FALSE;
	} /* for */

	/* send the message */
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "INIT " );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( nGetResponse( ) )
			return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );
	} /* if */

	return 0;
} /* nInitializeSystem */


/*****************************************************************
Name:				nSetFiringRate

Inputs:
	None.

Return Value:
	int - 0 if fails, else nCheckResponse

Description:   This routine sets the POLARIS System's firing rate
			   by sending the IRATE command.
*****************************************************************/
int CCommandHandling::nSetFiringRate()
{
	int
		nFiringRate = 0;

	ReadINIParm( "POLARIS Options", "Activation Rate", "0", &nFiringRate );

	/*  
	 * Polaris Accedo and Vicra only support default rate of 20Hz (nFiringRate = 0).  
	 */
	if ( ((m_dtSystemInformation.nTypeofSystem == ACCEDO_SYSTEM) || (m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM)) 
			&& (nFiringRate != 0) )
	{
		nFiringRate = 0;
		WriteINIParm( "POLARIS Options", "Activation Rate", nFiringRate );
	}/* if */

	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "IRATE %d", nFiringRate );			

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( nGetResponse( ) )
			return nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) );
	} /* if */

	return 0;
} /* nSetFiringRate */
/*****************************************************************
Name:				nGetSystemInfo

Inputs:
	None.

Return Value:
	int - 0 if fails, 1 if passes

Description:   This routine gets the System information through
			   the VER and SFLIST commands, remember that it is
			   not the same calls for the AURORA and POLARIS the
			   reply mode numbers are different.
*****************************************************************/
int CCommandHandling::nGetSystemInfo()
{
	int
		nHexResponse = 0,
		i = 0;

	/* version Information */
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "VER 4" );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( !nGetResponse( ) )
		{
			return 0;
		}/* if */

		if ( !nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
		{
			return 0;
		}/* if */
		
		if ( !strncmp( m_szLastReply, "POLARIS", 7 )||
			 !strncmp( m_szLastReply, "polaris", 7 )||
			 !strncmp( m_szLastReply, "Polaris", 7 ) )
		{
			if ( strstr( m_szLastReply, "ACCEDO" ) )
			{
				m_dtSystemInformation.nTypeofSystem = ACCEDO_SYSTEM;
			}
			else if ( strstr( m_szLastReply, "VICRA" )
				|| strstr( m_szLastReply, "vicra" )
				|| strstr( m_szLastReply, "Vicra" ))
			{
				m_dtSystemInformation.nTypeofSystem = VICRA_SYSTEM;
			}
			else if ( strstr( m_szLastReply, "SPECTRA" )
				|| strstr( m_szLastReply, "spectra" )
				|| strstr( m_szLastReply, "Spectra" ))
			{
				m_dtSystemInformation.nTypeofSystem = SPECTRA_SYSTEM;
			}			
			else			
			{
				m_dtSystemInformation.nTypeofSystem = POLARIS_SYSTEM;
			}/* else */
		}
		else if ( !strncmp( m_szLastReply, "AURORA", 6 )||
			 !strncmp( m_szLastReply, "aurora", 6 )||
			 !strncmp( m_szLastReply, "Aurora", 6 ) )
		{ 
			m_dtSystemInformation.nTypeofSystem = AURORA_SYSTEM;
		}
		else
		{
			return 0;
		}/* else */

		sprintf( m_dtSystemInformation.szVersionInfo, m_szLastReply );
		/* EC-03-0071 */
		m_dtSystemInformation.szVersionInfo[strlen(m_dtSystemInformation.szVersionInfo) - 5] = 0;
	} /* if */

	if (m_dtSystemInformation.nTypeofSystem != AURORA_SYSTEM)
	{
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 00" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
				return 0;
			/* supported features summary list */
			nHexResponse = uASCIIToHex( m_szLastReply, 8 );
			m_dtSystemInformation.bActivePortsAvail = (0x01 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bPassivePortsAvail = (0x02 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bMultiVolumeParms = (0x04 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bTIPSensing = (0x08 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bActiveWirelessAvail = (0x10 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bMagneticPortsAvail = (0x8000 & nHexResponse ? 1 : 0);
			m_dtSystemInformation.bFieldGeneratorAvail = (0x40000 & nHexResponse ? 1 : 0);
		} /* if */

		sprintf( m_szCommand, "SFLIST 01" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of active ports */
			m_dtSystemInformation.nNoActivePorts = uASCIIToHex( &m_szLastReply[0], 1 );
		} /* if */

		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 02" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of passive ports */
			m_dtSystemInformation.nNoPassivePorts = uASCIIToHex( &m_szLastReply[0], 1 );
		} /* if */

		// Get volumn info
/*		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 03" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			//m_dtSystemInformation.nNoOfVolume = uASCIIToHex( &m_szLastReply[0], 1 );
			int iN = uASCIIToHex( &m_szLastReply[0], 1 );
		} */

		

		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 04" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of active tool ports supporting TIP detection */
			m_dtSystemInformation.nNoActTIPPorts = uASCIIToHex( &m_szLastReply[0], 1 );
		} /* if */

		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 05" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of active wireless ports */
			m_dtSystemInformation.nNoActWirelessPorts = uASCIIToHex( &m_szLastReply[0], 1 );
		} /* if */

	} /* if */
	else
	{
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 10" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of magnetic ports */
			m_dtSystemInformation.nNoMagneticPorts = uASCIIToHex( &m_szLastReply[0], 2 );
		} /* if */

		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "SFLIST 12" );
		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			/* number of FGs */
			m_dtSystemInformation.nNoFGCards = uASCIIToHex( &m_szLastReply[0], 1 );
			m_dtSystemInformation.nNoFGs = uASCIIToHex( &m_szLastReply[1], 1 );
		} /* if */

		/* EC-03-0071 
		 * - Add VER 7 and VER 8 commands
		 */
		/* Field Generator Version Information */
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "VER 7" );

		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
			{
				return 0;
			}/* if */

			if ( !nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
			{
				return 0;
			}/* if */
			
			strcat( m_dtSystemInformation.szVersionInfo, "\n" );
			strcat( m_dtSystemInformation.szVersionInfo, m_szLastReply );
			m_dtSystemInformation.szVersionInfo[strlen(m_dtSystemInformation.szVersionInfo) - 5] = 0;
		} /* if */

		/* SIU Version Information */
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "VER 8" );

		if(nSendMessage( m_szCommand, TRUE ))
		{
			if ( !nGetResponse( ) )
			{
				return 0;
			}/* if */

			if ( !nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
			{
				return 0;
			}/* if */
			
			strcat( m_dtSystemInformation.szVersionInfo, "\n" );
			strcat( m_dtSystemInformation.szVersionInfo, m_szLastReply );
			m_dtSystemInformation.szVersionInfo[strlen(m_dtSystemInformation.szVersionInfo) - 5] = 0;
		} /* if */
	} /* else */

	return 1;
} /* nGetSystemInfo */
/*****************************************************************
Name:				nInitializeAllPorts

Inputs:
	None.

Return Value:
	int - 1 is successful, 0 otherwise

Description:   This routine intializes all the ports using the
			   PINIT call.  It also makes calls to the PVWR routine
			   and TTCFG routine to virtual load tool definitions.
*****************************************************************/
int CCommandHandling::nInitializeAllPorts()
{
	int
		i = 0,
		nRet = 0,
		nPhysicalPorts = 0,
		nNoHandles = 0,
		nHandle = 0,
		n = 0;
	char
		pszINISection[25],
		pszPortID[8],
		pszROMFileName[MAX_PATH],
		szHandleList[MAX_REPLY_MSG],
		szErrorMessage[50];

	// passive
	for ( i = 0; i < m_dtSystemInformation.nNoPassivePorts; i++ )
	{
		sprintf( pszPortID, "Wireless Tool %02d", i + 1 );
		if (m_bPassivePortSROMFilesOutSeted)
		{
			strcpy(pszROMFileName, m_strPassivePortSROMFiles[i].GetBuffer(1));
		}
		else
		{
			/* load the ROM if one is specified */
			ReadINIParm( "POLARIS SROM Image Files", pszPortID, "", MAX_PATH, pszROMFileName );
		}
		
		if ( *pszROMFileName )
		{
			nLoadVirtualSROM( pszROMFileName, pszPortID, TRUE );
		} /* if */
	} /* for */

	do
	{
		n = 0;
		/* get the handles that need to be initialized */
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "PHSR 02" );

		if (!nSendMessage( m_szCommand, TRUE ))
			return 0;

		if (!nGetResponse( ))
			return 0;

		if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
			return 0;

		sprintf( szHandleList, m_szLastReply );
		nNoHandles = uASCIIToHex( &m_szLastReply[n], 2 );
		n+=2;

		if ( nNoHandles > 0 )
		{
			// active
			nPhysicalPorts = (m_dtSystemInformation.nNoActivePorts > 0 ?
							  m_dtSystemInformation.nNoActivePorts :
							  m_dtSystemInformation.nNoMagneticPorts > 0 ?
							  m_dtSystemInformation.nNoMagneticPorts : 0);

			if ( m_dtSystemInformation.nNoActivePorts == 4 )
				nPhysicalPorts = 12;

			sprintf( pszINISection, m_dtSystemInformation.nNoActivePorts > 0 ?
									"POLARIS SROM Image Files\0" :
									m_dtSystemInformation.nNoMagneticPorts > 0 ?
									"AURORA SROM Image Files\0" : "" );

			for ( i = 0; i < nPhysicalPorts; i++ )
			{
				/* if the tool has a ROM defined load it, if TTCFG is defined, load that */
				sprintf( pszPortID, "Port %d", i+1 );
				ReadINIParm( pszINISection, pszPortID, "", MAX_PATH, pszROMFileName );
				if ( *pszROMFileName )
				{
					if (!strncmp(pszROMFileName, "TTCFG", 5 ))
						nLoadTTCFG( pszPortID );
					else
						nLoadVirtualSROM( pszROMFileName, pszPortID, FALSE );
					nHandle = nGetHandleForPort( pszPortID );
					if ( nHandle != 0 )
					{
						if (!nInitializeHandle( nHandle ))
						{
							/* Inform user which port fails on PINIT */
							sprintf(szErrorMessage, "Port %s could not be initialized.\n"
							"Check your SROM image file settings.", pszPortID );
							MessageBox( NULL,  szErrorMessage,
							"PINIT ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
							return 0;
						}/* if */
					} /* if */
				} /* if */
			} /* for */

			for ( int i = 0; i < nNoHandles; i++ )
			{
				nHandle = uASCIIToHex( &szHandleList[n], 2 );
				if ( !nGetPortInformation( nHandle ) )
					return 0;

				if ( !m_dtHandleInformation[nHandle].HandleInfo.bInitialized )
				{
					if (!nInitializeHandle( nHandle ))
					{
						/* Inform user which port fails on PINIT */
						sprintf(szErrorMessage, "Port %s could not be initialized.\n"
						"Check your SROM image file settings.", m_dtHandleInformation[nHandle].szPhysicalPort );
						MessageBox( NULL,  szErrorMessage,
						"PINIT ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
						return 0;
					}/* if */
					n+=5;
				} /* if */
			} /* for */
		} /* if */
		/* do this until there are no new handles */
	}while( nNoHandles > 0 );

	return 1;
} /* nInitializeAllPorts */
/*****************************************************************
Name:				nInitializeHandle

Inputs:
	int nHandle - the handle to be intialized

Return Value:
	int - 1 if successful, otherwise 0

Description:   This routine initializes the specified handle using
			   the PINIT command.
*****************************************************************/
int CCommandHandling::nInitializeHandle( int nHandle )
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PINIT %02X", nHandle );
	if ( !nSendMessage( m_szCommand, TRUE ))
		return 0;
	if ( !nGetResponse() )
		return 0;

	if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
	{
		return 0;
	} /* if */
	m_dtHandleInformation[nHandle].HandleInfo.bInitialized = TRUE;

	return 1;
} /* nInitializeHandle */

/*****************************************************************
Name:				nEnableAllPorts

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:   This routine enables all the port handles that need
			   to be enabled using the PENA command.
*****************************************************************/
int CCommandHandling::nEnableAllPorts()
{
	int
		nNoHandles = 0,
		nPortHandle = 0,
		n = 0;
	char
		szHandleList[MAX_REPLY_MSG];

	m_nPortsEnabled = 0;
	/* get all the ports that need to be enabled */
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PHSR 03" );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if (!nGetResponse( ))
			return 0;

		if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
			return 0;

		sprintf( szHandleList, m_szLastReply );
		nNoHandles = uASCIIToHex( &szHandleList[n], 2 );
		n+=2;

		for ( int i = 0; i < nNoHandles; i++ )
		{
			nPortHandle = uASCIIToHex( &szHandleList[n], 2 );
			memset(m_szCommand, 0, sizeof(m_szCommand));
			sprintf( m_szCommand, "PENA %02X%c", nPortHandle, 'D' );
			n+=5;
			if (!nSendMessage( m_szCommand, TRUE ))
				return 0;
			if ( !nGetResponse() )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			nGetPortInformation( nPortHandle );
			m_nPortsEnabled++;
		} /* for */
		return 1;
	} /* if */
	return 0;
} /* nEnableAllPorts */

/*****************************************************************
Name:				nEnableOnePorts

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:   This routine enables specified port handles that need
			   to be enabled using the PENA command.
*****************************************************************/
int CCommandHandling::nEnableOnePorts( int nPortHandle )
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PENA %02X%c", nPortHandle, 'D' );
	if (!nSendMessage( m_szCommand, TRUE ))
		return 0;
	if ( !nGetResponse() )
		return 0;
	if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
		return 0;
	nGetPortInformation( nPortHandle );
	return 1;
} /* nEnableOnePorts */

/*****************************************************************
Name:				nDisablePort

Inputs:
	int nPortHandle - handle to be disabled

Return Value:
	int - 1 if successful, 0 otherwise

Description:    This routine disables the supplied port handle
				using the PDIS call.
*****************************************************************/
int CCommandHandling::nDisablePort( int nPortHandle )
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PDIS %02X", nPortHandle );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if ( !nGetResponse( ) )
			return 0;
		if (nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) ))
		{
			nGetPortInformation(nPortHandle);
			return 1;
		} /* if */
	} /* if */

	return 0;
} /* nDisablePort */
/*****************************************************************
Name:				nActivateAllPorts

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:    This is the routine that activates all ports using
				
*****************************************************************/
int CCommandHandling::nActivateAllPorts()
{
	if (!nFreePortHandles())
		return 0;

	if (!nInitializeAllPorts())
		return 0;

	if (!nEnableAllPorts())
		return 0;

	return 1;
} /* nActivateAllPorts */
/*****************************************************************
Name:				nLoadVirtualSROM

Inputs:
	char * pszFileName - the file to be loaded
	char * pszPhysicalPortID - the physical port id that is being
								loaded to.
	bool bPassive - is this a passive port or not

Return Value:
	int - 1 if successful, 0 otherwise.

Description:   
	This routine virtual loads a SROM file to the specified port.
	It uses the PVWR command to do this.
*****************************************************************/
int CCommandHandling::nLoadVirtualSROM( char * pszFileName, 
									    char * pszPhysicalPortID, 
										bool bPassive )
{
	FILE
	    *pFileHandle = NULL;
	int
		nRet = 0,
		nHandle = 0,
		nBytes = 0,
		nCnt = 0,
		i = 0,
		n = 0;
	static unsigned char
        gruchBuff[ 1024 ];
	char
		cMessage[256];


	if ( !*pszFileName )
		return 0;

	if ( bPassive )
	{
		for ( i = 0; i < NO_HANDLES; i++ )
		{
			if ( !strncmp( m_dtHandleInformation[i].szPhysicalPort, pszPhysicalPortID, 16 ) )
				return 0;
		}
		/* if passive we need a port handle */
		memset(m_szCommand, 0, sizeof(m_szCommand));
		sprintf( m_szCommand, "PHRQ ********01****"); 
		if (!nSendMessage( m_szCommand, TRUE ))
			return 0;

		if (!nGetResponse( ))
			return 0;
		if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
			return 0;
		nHandle = uASCIIToHex(&m_szLastReply[n], 2);
		if ( m_dtHandleInformation[nHandle].HandleInfo.bInitialized == 1 )
			return 0;

		if( m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM ||
			m_dtSystemInformation.nTypeofSystem == SPECTRA_SYSTEM )
			sprintf( m_dtHandleInformation[nHandle].szPhysicalPort, pszPhysicalPortID );
	}/* if */
	else
	{
		/* if active a handle has already been assigned */
		nHandle = nGetHandleForPort( pszPhysicalPortID );
		if ( nHandle == 0 || m_dtHandleInformation[nHandle].HandleInfo.bInitialized == 1 )
			return 0;
	}/* else */

    if( !(pFileHandle = fopen( pszFileName, "rb" )) )
    {
		return 0;
    } /* if */

    if( (nBytes = fread( gruchBuff, 1, sizeof(gruchBuff), pFileHandle )) < 1 )
    {
		sprintf(cMessage, "Unable to read ROM image file %s.", pszFileName );
			MessageBox( NULL, cMessage, "SROM Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND);
        goto cleanup;
    } /* if */

    for( nCnt = 0; nCnt < nBytes; )
    {
		/*
		 * write the data to the tool description section of 
		 * the virtual SROM on a per port basis
		 */
		memset(m_szCommand, 0, sizeof(m_szCommand));
        sprintf( m_szCommand, "PVWR:%02X%04X", nHandle, nCnt );

        for( i = 0; i < 64; i++, nCnt++ )
        {
			/* (plus eleven for the PVWR:XX0000 ) */
            sprintf( m_szCommand + 11 + 2 * i, "%02X", gruchBuff[nCnt] ); 
        } /* for */
		int n = strlen( m_szCommand );
        if (!nSendMessage( m_szCommand, TRUE ))
			goto cleanup;

		if (!nGetResponse( ))
        {
			sprintf(cMessage, "Failed writing to %s.", pszPhysicalPortID );
				MessageBox( NULL, cMessage, "SROM Image Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND);
            goto cleanup;
		} /* if */
		if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE)))
        {
            goto cleanup;
		} /* if */
    } /* for */

cleanup:
    if( !(fclose( pFileHandle ) ) )
		return 0;

	return 1;
} /* nLoadVirtualSROM */
/*****************************************************************
Name:			nLoadTTCFG	

Inputs:
	char * pszPortID - the physical port to be loaded with TTCFG

Return Value:
	int - 1 if successful, 0 otherwise

Description:   
	This routine loads the Test Tool ConFiGutation to the specified
	port.
*****************************************************************/
int CCommandHandling::nLoadTTCFG( char * pszPortID )
{
	int
		nPortHandle = 0;

	/* get the handle for the port */
	nPortHandle = nGetHandleForPort( pszPortID );
	if ( nPortHandle == 0 )
		return 0;

	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "TTCFG %02X", nPortHandle );
	if ( nSendMessage( m_szCommand, TRUE ) )
	{
		if ( nGetResponse() )
			return( nCheckResponse( nVerifyResponse(m_szLastReply, TRUE)));
	} /* if */
	return 0;	
} /* nLoadTTCFG */
/*****************************************************************
Name:				nGetHandleForPort

Inputs:
	char * pszPortID - the physical port to match a handle to

Return Value:
	int - 0 if fails, port handle if passes

Description:   
	This routine takes a physical port location and matches a handle
	to it.  These handles have already been defined with a PHSR
*****************************************************************/
int CCommandHandling::nGetHandleForPort( char * pszPortID )
{
	int
		nPortHandle = 0,
		nNoHandles = 0,
		n = 0;
	char
		szHandleList[MAX_REPLY_MSG];

	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PHSR 00" );

	if ( nSendMessage( m_szCommand, TRUE ) )
	{
		if ( nGetResponse() )
			if (!nCheckResponse( nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
	} /* if */

	sprintf(szHandleList, m_szLastReply);
	nNoHandles = uASCIIToHex( &szHandleList[n], 2 );
	n+=2;

	for ( int i = 0; i < nNoHandles; i++ )
	{
		/* for all the handles, get their physical location with PHINF */
		nPortHandle = uASCIIToHex(&szHandleList[n], 2);
		n+=5;
		nGetPortInformation(nPortHandle);
		/* if the physical location match pszPortID, return the handle */
		if ( !strncmp(m_dtHandleInformation[nPortHandle].szPhysicalPort, pszPortID, 2) )
			return nPortHandle;
	} /* for */

	return 0;
} /* nGetHandleForPort */
/*****************************************************************
Name:				nFreePortHandles

Inputs:
	None.

Return Value:
	int - 0 if fails, 1 if passes

Description:   
	This routine frees all port handles that need to be freed 
	using the PHF command.
*****************************************************************/
int CCommandHandling::nFreePortHandles()
{
	int
		nNoHandles = 0,
		nHandle = 0,
		n = 0;
	char
		szHandleList[MAX_REPLY_MSG];

	/* get all the handles that need freeing */
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "PHSR 01" );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if (!nGetResponse( ))
			return 0;
		if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
			return 0;

		sprintf(szHandleList, m_szLastReply);
		nNoHandles = uASCIIToHex(&szHandleList[n], 2);
		n+=2;
		for ( int i = 0; i < nNoHandles; i++ )
		{
			nHandle = uASCIIToHex( &szHandleList[n], 2 );
			memset(m_szCommand, 0, sizeof(m_szCommand));
			sprintf( m_szCommand, "PHF %02X", nHandle);
			n+=5;
			if (!nSendMessage( m_szCommand, TRUE ))
				return 0;
			if ( !nGetResponse() )
				return 0;
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
			m_dtHandleInformation[nHandle].HandleInfo.bInitialized = FALSE;
			m_dtHandleInformation[nHandle].HandleInfo.bEnabled = FALSE;
			/* EC-03-0071 */
			memset(m_dtHandleInformation[nHandle].szPhysicalPort, 0, 5);
		} /* for */
		return 1;
	} /* if */

	return 0;
} /* nFreePortHandles */
/*****************************************************************
Name:				nGetPortInformation

Inputs:
	int nPortHandle - the handle to get information for

Return Value:
	int - 1 if successful, 0 otherwise

Description:   
	This routine gets the port handling information for the supplied
	handle.  It uses the PHINF call to get this information.
*****************************************************************/
int CCommandHandling::nGetPortInformation(int nPortHandle)
{
	unsigned int
		uASCIIConv = 0; 
	char
		*pszPortInformation = NULL;

	memset(m_szCommand, 0, sizeof(m_szCommand));
	if( m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM ||
		m_dtSystemInformation.nTypeofSystem == SPECTRA_SYSTEM)
		sprintf( m_szCommand, "PHINF %02X0005", nPortHandle );
	else
		sprintf( m_szCommand, "PHINF %02X0025", nPortHandle );

	if ( nSendMessage( m_szCommand, TRUE ) )
	{
		if ( nGetResponse() )
		{
			if ( !nCheckResponse( nVerifyResponse(m_szLastReply, TRUE )))
				return 0;

			pszPortInformation = m_szLastReply;

			strncpy( m_dtHandleInformation[nPortHandle].szToolType, pszPortInformation, 8 );
			m_dtHandleInformation[nPortHandle].szToolType[8] = '\0';
			pszPortInformation+=8;
			strncpy( m_dtHandleInformation[nPortHandle].szManufact, pszPortInformation, 12 );
			m_dtHandleInformation[nPortHandle].szManufact[12] = '\0';
			pszPortInformation+=12;
			strncpy( m_dtHandleInformation[nPortHandle].szRev, pszPortInformation, 3 );
			m_dtHandleInformation[nPortHandle].szRev[3] = '\0';
			pszPortInformation+=3;
			strncpy( m_dtHandleInformation[nPortHandle].szSerialNo, pszPortInformation, 8 );
			m_dtHandleInformation[nPortHandle].szSerialNo[8] = '\0';
			pszPortInformation+=8;
			uASCIIConv = uASCIIToHex( pszPortInformation, 2 );
			pszPortInformation+=2;
			m_dtHandleInformation[nPortHandle].HandleInfo.bToolInPort = ( uASCIIConv & 0x01 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bGPIO1 = ( uASCIIConv & 0x02 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bGPIO2 = ( uASCIIConv & 0x04 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bGPIO3 = ( uASCIIConv & 0x08 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bInitialized = ( uASCIIConv & 0x10 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bEnabled = ( uASCIIConv & 0x20 ? 1 : 0 );
			m_dtHandleInformation[nPortHandle].HandleInfo.bTIPCurrentSensing = ( uASCIIConv & 0x80 ? 1 : 0 );

			/* parse the part number 0x0004 */
			strncpy( m_dtHandleInformation[nPortHandle].szPartNumber, pszPortInformation, 20 );
			m_dtHandleInformation[nPortHandle].szPartNumber[20] = '\0';
			pszPortInformation+=20;

			if( m_dtSystemInformation.nTypeofSystem != VICRA_SYSTEM &&
				m_dtSystemInformation.nTypeofSystem != SPECTRA_SYSTEM )
			{
				pszPortInformation+=10;
				sprintf( m_dtHandleInformation[nPortHandle].szPhysicalPort, "%d", nPortHandle );
				strncpy( m_dtHandleInformation[nPortHandle].szPhysicalPort, pszPortInformation, 2 );
				/* EC-03-0071
				m_dtHandleInformation[nPortHandle].szPhysicalPort[2] = '\0';
				 */
				pszPortInformation+=2;
				strncpy( m_dtHandleInformation[nPortHandle].szChannel, pszPortInformation, 2 );
				m_dtHandleInformation[nPortHandle].szChannel[2] = '\0';
				if ( !strncmp( m_dtHandleInformation[nPortHandle].szChannel, "01", 2 ) )
				{
					/* EC-03-0071
					strncat(m_dtHandleInformation[nPortHandle].szPhysicalPort, "-b", 2 );
					 */
					strncpy(&m_dtHandleInformation[nPortHandle].szPhysicalPort[2], "-b", 2 );
					for ( int i = 0; i < NO_HANDLES; i++ )
					{
						if ( strncmp( m_dtHandleInformation[i].szPhysicalPort, m_dtHandleInformation[nPortHandle].szPhysicalPort, 4 ) &&
							 !strncmp( m_dtHandleInformation[i].szPhysicalPort, m_dtHandleInformation[nPortHandle].szPhysicalPort, 2 ) )
							/* EC-03-0071
							strncat(m_dtHandleInformation[i].szPhysicalPort, "-a", 2 );
							 */
							strncpy(&m_dtHandleInformation[i].szPhysicalPort[2], "-a", 2 );
					} /* for */
				} /* if */
			} /* if */
		} /* if */
		else
			return 0;
	} /* if */

	return 1;
} /* nGetPortInformation */
/*****************************************************************
Name:				nStartTracking

Inputs:
	None.

Return Value:
	int - 0 if fails, else nCheckResponse

Description:    This routine starts that System tracking.  It uses
				the TSTART command to do this.
*****************************************************************/
int CCommandHandling::nStartTracking()
{
	ReadINIParm( "Reporting Options", "Report While Tracking", "1", &m_bDisplayErrorsWhileTracking );

	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "TSTART " );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if (!nGetResponse( ))
			return 0;

		return  nVerifyResponse(m_szLastReply, TRUE) ;
	} /* if */

	return 0;
} /* nStartTracking */
/*****************************************************************
Name:				nGetTXTransforms

Inputs:
	bool bReturnOOV - whether or not to return values outside
					  of the characterized volume.

Return Value:
	int - 1 if successful, 0 otherwise.

Description:   
	This routine gets the transformation information using the TX
	command.  Remember that if you want to track outside the
	characterized volume you need to set the flag.
*****************************************************************/
int CCommandHandling::nGetTXTransforms(bool bReturn0x0800Option)
{
	int
		nReplyMode = 0x0001,
		nNoHandles = 0,
		nHandle = 0,
		nRet = 0;
	unsigned int
		unHandleStatus = 0,
		unSystemStatus = 0;
	bool
		bDisabled = FALSE;
	char
		*pszTransformInfo = NULL;

	/* report in volume only or out of volume as well */
	nReplyMode = bReturn0x0800Option ? 0x0801 : 0x0001;

	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "TX %04X", nReplyMode );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if (!nGetResponse( ))
		{
			return 0;
		}/* if */

		if ( m_bDisplayErrorsWhileTracking )
		{
			if (!nCheckResponse(nVerifyResponse(m_szLastReply, TRUE )))
				return 0;
		}
		else
		{
			if (!nVerifyResponse( m_szLastReply, FALSE ))
				return 0;
		}/* else */

		/* TX Parsing Routines */
		pszTransformInfo = m_szLastReply;

		nNoHandles = uASCIIToHex( pszTransformInfo, 2 );
		pszTransformInfo+=2;

		for ( int i = 0; i < nNoHandles; i++ )
		{
			nHandle = uASCIIToHex( pszTransformInfo, 2 );
			pszTransformInfo+=2;

			bDisabled = FALSE;

			if ( strlen( pszTransformInfo ) < 18 )
				return 0;

			if( !strncmp( pszTransformInfo, "MISSING", 7 ) ||
				!strncmp( pszTransformInfo, "DISABLED", 8 ) || 
				!strncmp( pszTransformInfo, "UNOCCUPIED", 10 ))
			{
				if ( !strncmp( pszTransformInfo, "UNOCCUPIED", 10 ))
				{
					
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_UNOCCUPIED;
					pszTransformInfo+=10;					
					bDisabled = TRUE;
				} /* if */
				else if ( !strncmp( pszTransformInfo, "DISABLED", 8 ))
				{
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_DISABLED;
					pszTransformInfo+=8;
					bDisabled = TRUE;
				} /* else if */
				else
				{
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_MISSING;
					pszTransformInfo+=7;
				} /* else */
				m_dtHandleInformation[nHandle].Xfrms.rotation.q0 =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qx =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qy =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qz =
				m_dtHandleInformation[nHandle].Xfrms.translation.x =
				m_dtHandleInformation[nHandle].Xfrms.translation.y =
				m_dtHandleInformation[nHandle].Xfrms.translation.z =
				m_dtHandleInformation[nHandle].Xfrms.fError = BAD_FLOAT;
			} /* if */
			else
			{
				m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_VALID;

				if (!bExtractValue( pszTransformInfo, 6, 10000., 
									&m_dtHandleInformation[nHandle].Xfrms.rotation.q0 ) ||
					!bExtractValue( pszTransformInfo + 6, 6, 10000., 
									&m_dtHandleInformation[nHandle].Xfrms.rotation.qx ) ||
					!bExtractValue( pszTransformInfo + 12, 6, 10000., 
									&m_dtHandleInformation[nHandle].Xfrms.rotation.qy ) ||
					!bExtractValue( pszTransformInfo + 18, 6, 10000., 
									&m_dtHandleInformation[nHandle].Xfrms.rotation.qz ) ||
					!bExtractValue( pszTransformInfo + 24, 7, 100., 
									&m_dtHandleInformation[nHandle].Xfrms.translation.x ) ||
					!bExtractValue( pszTransformInfo + 31, 7, 100., 
									&m_dtHandleInformation[nHandle].Xfrms.translation.y ) ||
					!bExtractValue( pszTransformInfo + 38, 7, 100., 
									&m_dtHandleInformation[nHandle].Xfrms.translation.z ) ||
					!bExtractValue( pszTransformInfo + 45, 6, 10000., 
									&m_dtHandleInformation[nHandle].Xfrms.fError ) )
				{
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_MISSING;
					return 0;
				} /* if */
				else
				{
					pszTransformInfo+=51;
				} /* else */
			} /* else */		
			/*get handle status...*/
			if ( !bDisabled )
			{
				unHandleStatus = uASCIIToHex( pszTransformInfo, 8 );
				pszTransformInfo+=8;
				m_dtHandleInformation[nHandle].HandleInfo.bToolInPort = ( unHandleStatus & 0x01 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO1 = ( unHandleStatus & 0x02 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO2 = ( unHandleStatus & 0x04 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO3 = ( unHandleStatus & 0x08 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bInitialized = ( unHandleStatus & 0x10 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bEnabled = ( unHandleStatus & 0x20 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bOutOfVolume = ( unHandleStatus & 0x40 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bPartiallyOutOfVolume = ( unHandleStatus & 0x80 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bDisturbanceDet = ( unHandleStatus & 0x200 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bSignalTooSmall = ( unHandleStatus & 0x400 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bSignalTooBig = ( unHandleStatus & 0x800 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bProcessingException = ( unHandleStatus & 0x1000 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bHardwareFailure = ( unHandleStatus & 0x2000 ? 1 : 0 );

				/*get frame number...*/
				m_dtHandleInformation[nHandle].Xfrms.ulFrameNumber = 
					uASCIIToHex( pszTransformInfo, 8 );
				pszTransformInfo+=8;
			} /* if */
			pszTransformInfo++; /*for the carriage return*/
		} /* for */

		unSystemStatus = uASCIIToHex( pszTransformInfo, 4 );
		m_dtSystemInformation.bCommunicationSyncError = ( unSystemStatus & 0x01 ? 1 : 0 );
		m_dtSystemInformation.bTooMuchInterference = ( unSystemStatus & 0x02 ? 1 : 0 );
		m_dtSystemInformation.bSystemCRCError = ( unSystemStatus & 0x04 ? 1 : 0 );
		m_dtSystemInformation.bRecoverableException = ( unSystemStatus & 0x08 ? 1 : 0 );
		m_dtSystemInformation.bHardwareFailure = ( unSystemStatus & 0x10 ? 1 : 0 );
		m_dtSystemInformation.bHardwareChange = ( unSystemStatus & 0x20 ? 1 : 0 );
		m_dtSystemInformation.bPortOccupied = ( unSystemStatus & 0x40 ? 1 : 0 );
		m_dtSystemInformation.bPortUnoccupied = ( unSystemStatus & 0x80 ? 1 : 0 );
		m_dtSystemInformation.bDiagnosticsPending = ( unSystemStatus & 0x100 ? 1 : 0 );
		m_dtSystemInformation.bTemperatureOutOfRange = ( unSystemStatus & 0x200 ? 1 : 0 );

	}else /*2009.3.19 ÄþËæ¾ü ·¢ËÍÃüÁîÊ§°ÜÔò·µ»Ø0*/
	{
		ApplyXfrms();
		return 0;
	}

	/*
	 * reference tracking...apply at the end of all the parsing so that
	 * it is all the latest xfrms being applied
	 */
	ApplyXfrms();

	return 1;
} /* nGetTXTransforms */


/*****************************************************************
Name:				nGetBXTransforms

Inputs:
	bool bReturnOOV - whether or not to return values outside
					  of the characterized volume.

Return Value:
	int - 1 if successful, 0 otherwise.

Description:   
	This routine gets the transformation information using the BX
	command.  Remember that if you want to track outside the
	characterized volume you need to set the flag.
*****************************************************************/
int CCommandHandling::nGetBXTransforms(bool bReturn0x0800Option)
{
	int
		nReplyMode = 0x0001,
		nReplySize = 0,
		nSpot = 0,
		nNoHandles = 0,
		nHandle = 0,
		nRet  = 0;
	unsigned int
		unSystemStatus = 0,
		uTransStatus = 0,
		unHandleStatus = 0,
		uHeaderCRC = 0,
		uBodyCRC = 0,
		uCalcCRC = 0;
	char
		*pszTransformInfo = NULL;

	/* set reply mode depending on bReturnOOV */
	nReplyMode = bReturn0x0800Option ? 0x0801 : 0x0001;
	
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "BX %04X", nReplyMode );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		if (!nGetBinaryResponse( ))
		{
			return 0;
		}/* if */

		if ( m_bDisplayErrorsWhileTracking )
		{
			if (!nCheckResponse(nVerifyResponse( m_szLastReply, TRUE )))
				return 0;
		}
		else
		{
			if (!nVerifyResponse( m_szLastReply, FALSE ))
				return 0;
		}/* else */

		pszTransformInfo = m_szLastReply;
		uCalcCRC = SystemGetCRC(m_szLastReply, 4 );

		/* check for preamble ( A5C4 ) */
        while(((pszTransformInfo[0]&0xff)!=0xc4))
		{
            pszTransformInfo++;
		}/* while */

		if ( (pszTransformInfo[0]&0xff)!=0xc4 || (pszTransformInfo[1]&0xff)!=0xa5 )
		{
			return REPLY_INVALID;
		}/* if */

		/* parse the header */
        nSpot+=2;
		nReplySize = nGetHex2(&pszTransformInfo[nSpot]);
        nSpot+=2;
		uHeaderCRC = nGetHex2(&pszTransformInfo[nSpot]); 
        nSpot+=2;

		if ( uCalcCRC != uHeaderCRC )
		{
			if ( m_bDisplayErrorsWhileTracking )
				nCheckResponse( REPLY_BADCRC ); /* display the Bad CRC error message */
			return REPLY_BADCRC;
		} /* if */

		nNoHandles = nGetHex1(&pszTransformInfo[nSpot]);
		nSpot++;

		for ( int i = 0; i < nNoHandles; i++ )
		{
			nHandle = nGetHex1(&pszTransformInfo[nSpot]);
			nSpot++;

			uTransStatus = nGetHex1(&pszTransformInfo[nSpot]);
			nSpot++;

			if ( uTransStatus == 1 ) /* one means that the transformation was returned */
			{
				/* parse out the individual components by converting binary to floats */
				m_dtHandleInformation[nHandle].Xfrms.rotation.q0 = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.rotation.qx = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.rotation.qy = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.rotation.qz = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.translation.x = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.translation.y = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.translation.z = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.fError = fGetFloat(&pszTransformInfo[nSpot]);
				nSpot+=4;
				unHandleStatus = nGetHex4(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.ulFrameNumber = nGetHex4(&pszTransformInfo[nSpot]);
				nSpot+=4;
				m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_VALID;
			} /* if */

			if ( uTransStatus == 2 || uTransStatus == 4 ) /* 2 means the tool is missing and */
														  /* 4 means DISABLED */
			{
				/*
				 * no transformation information is returned but the port status and time
				 * are return
				 */
				if ( uTransStatus == 2 )
				{
					unHandleStatus = nGetHex4(&pszTransformInfo[nSpot]);
					nSpot+=4;
					m_dtHandleInformation[nHandle].Xfrms.ulFrameNumber = nGetHex4(&pszTransformInfo[nSpot]);
					nSpot+=4;
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_MISSING;
				} /* if */
				else
					m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_DISABLED;

				m_dtHandleInformation[nHandle].Xfrms.rotation.q0 =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qx =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qy =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qz =
				m_dtHandleInformation[nHandle].Xfrms.translation.x =
				m_dtHandleInformation[nHandle].Xfrms.translation.y =
				m_dtHandleInformation[nHandle].Xfrms.translation.z =
				m_dtHandleInformation[nHandle].Xfrms.fError = BAD_FLOAT;
			}/* if */

			if ( uTransStatus == 1 || uTransStatus == 2 )
			{
				m_dtHandleInformation[nHandle].HandleInfo.bToolInPort = ( unHandleStatus & 0x01 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO1 = ( unHandleStatus & 0x02 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO2 = ( unHandleStatus & 0x04 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bGPIO3 = ( unHandleStatus & 0x08 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bInitialized = ( unHandleStatus & 0x10 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bEnabled = ( unHandleStatus & 0x20 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bOutOfVolume = ( unHandleStatus & 0x40 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bPartiallyOutOfVolume = ( unHandleStatus & 0x80 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bDisturbanceDet = ( unHandleStatus & 0x200 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bSignalTooSmall = ( unHandleStatus & 0x400 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bSignalTooBig = ( unHandleStatus & 0x800 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bProcessingException = ( unHandleStatus & 0x1000 ? 1 : 0 );
				m_dtHandleInformation[nHandle].HandleInfo.bHardwareFailure = ( unHandleStatus & 0x2000 ? 1 : 0 );
			}/* if */
		} /* for */

		unSystemStatus = nGetHex2( &pszTransformInfo[nSpot] );
		nSpot+=2;
		uBodyCRC = nGetHex2(&pszTransformInfo[nSpot]); 
		m_dtSystemInformation.bCommunicationSyncError = ( unSystemStatus & 0x01 ? 1 : 0 );
		m_dtSystemInformation.bTooMuchInterference = ( unSystemStatus & 0x02 ? 1 : 0 );
		m_dtSystemInformation.bSystemCRCError = ( unSystemStatus & 0x04 ? 1 : 0 );
		m_dtSystemInformation.bRecoverableException = ( unSystemStatus & 0x08 ? 1 : 0 );
		m_dtSystemInformation.bHardwareFailure = ( unSystemStatus & 0x10 ? 1 : 0 );
		m_dtSystemInformation.bHardwareChange = ( unSystemStatus & 0x20 ? 1 : 0 );
		m_dtSystemInformation.bPortOccupied = ( unSystemStatus & 0x40 ? 1 : 0 );
		m_dtSystemInformation.bPortUnoccupied = ( unSystemStatus & 0x80 ? 1 : 0 );

		uCalcCRC = SystemGetCRC(pszTransformInfo+=6, nSpot-6 );
		if ( uCalcCRC != uBodyCRC )
		{
			nCheckResponse( REPLY_BADCRC ); /* display the Bad CRC error message */
			return REPLY_BADCRC;
		} /* if */


	} /* if */

	/*
	 * reference tracking...apply at the end of all the parsing so that
	 * it is all the latest xfrms being applied
	 */
	ApplyXfrms();


	return 1;
} /* nGetBXTransforms */

/*****************************************************************
Name:				ApplyXfrm

Inputs:
	int nHandle - the handle to change

Return Value:
	
Description:   Switch the tools coordinate system to that of the
			   specified reference handle.
*****************************************************************/
void CCommandHandling::ApplyXfrms()
{
	QuatTransformation
		dtRefQuatXfrm,
		dtRefQuatXfrmInv,
		dtPortQuatXfrm,
		dtNewQuatXfrm;

	if( m_nRefHandle == -1 ) //if no reference handle do nothing
		return;

	for( int nHandle = 0; nHandle < NO_HANDLES; nHandle++ )
	{
		if( nHandle != m_nRefHandle &&							  // don't apply the reference to itself
			m_dtHandleInformation[nHandle].HandleInfo.bEnabled )  // only apply if the handle is enabled
		{
			if( m_dtHandleInformation[m_nRefHandle].Xfrms.translation.x <= MAX_NEGATIVE )
			{
				m_dtHandleInformation[nHandle].Xfrms.translation.x =
				m_dtHandleInformation[nHandle].Xfrms.translation.y =
				m_dtHandleInformation[nHandle].Xfrms.translation.z = 
				m_dtHandleInformation[nHandle].Xfrms.rotation.q0 =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qx =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qy =
				m_dtHandleInformation[nHandle].Xfrms.rotation.qz = BAD_FLOAT;
				m_dtHandleInformation[nHandle].Xfrms.ulFlags = TRANSFORM_MISSING;
			}/* if */
			else
			{
				dtRefQuatXfrm.rotation = m_dtHandleInformation[m_nRefHandle].Xfrms.rotation;
				dtRefQuatXfrm.translation = m_dtHandleInformation[m_nRefHandle].Xfrms.translation;
				QuatInverseXfrm(&dtRefQuatXfrm, &dtRefQuatXfrmInv);

				dtPortQuatXfrm.rotation = m_dtHandleInformation[nHandle].Xfrms.rotation;
				dtPortQuatXfrm.translation = m_dtHandleInformation[nHandle].Xfrms.translation;
				QuatCombineXfrms( &dtPortQuatXfrm, &dtRefQuatXfrmInv, &dtNewQuatXfrm );
				m_dtHandleInformation[nHandle].Xfrms.rotation = dtNewQuatXfrm.rotation;
				m_dtHandleInformation[nHandle].Xfrms.translation = dtNewQuatXfrm.translation;
			}/* else */
		} /* if */
	} /* for */
} /* ApplyXfrm */

/*****************************************************************
Name:				nStopTracking

Inputs:
	None.

Return Value:
	int - 0 if fails, nCheckResponse if passes

Description:   This routine stops the System's tracking by using
			   the TSTOP call.
*****************************************************************/
int CCommandHandling::nStopTracking()
{
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "TSTOP " );

	if(nSendMessage( m_szCommand, TRUE ))
	{
		nGetResponse( );
		return nVerifyResponse(m_szLastReply, TRUE) ;
	} /* if */

	return 0;
} /* nStopTracking */


/*****************************************************************
Name:				nGetNewAlerts

Inputs:
	None.

Return Value:
	int - 0 if fails, 1 if passes

Description:   This routine gets the new alerts and fills the
			   new alerts structure with the appropriate flags.
***********************************************************/
int CCommandHandling::nGetAlerts(bool bNewAlerts)
{
	unsigned int
		unAlertsStatus = 0;
	char 
		*pszAlertsInfo = NULL;

	memset(m_szCommand, 0, sizeof(m_szCommand));
	if( bNewAlerts )
		sprintf( m_szCommand, "GET Info.Status.New Alerts" );
	else
		sprintf( m_szCommand, "GET Info.Status.Alerts" );

	if ( nSendMessage( m_szCommand, TRUE ) )
	{
		if (!nGetResponse( ))
		{
			return 0;
		}/* if */

		if( !nCheckResponse( nVerifyResponse(m_szLastReply, TRUE) ))
			return 0;

		/* Parsing Timeouts */

		m_szLastReply[strlen(m_szLastReply) - 5] = '\0'; // 4 for CRC and 1 for <CR>
		pszAlertsInfo = m_szLastReply;

		while( *pszAlertsInfo != '=' )
			pszAlertsInfo++;

		pszAlertsInfo++;

		//convert the ascii to integer format
		unAlertsStatus = atoi(pszAlertsInfo);

		m_dtNewAlerts.bFatalParamterFault		= ( unAlertsStatus & ALERT_FATAL_PARAM_FAULT ? 1 : 0 );
		m_dtNewAlerts.bSensorParameterFault		= ( unAlertsStatus & ALERT_SENSOR_PARAM_FAULT ? 1 : 0 );
		m_dtNewAlerts.bMainVoltageFault			= ( unAlertsStatus & ALERT_MAIN_VOLTAGE_FAULT ? 1 : 0 );
		m_dtNewAlerts.bSensorVoltageFault		= ( unAlertsStatus & ALERT_SENSOR_VOLTAGE_FAULT ? 1 : 0 );
		m_dtNewAlerts.bIlluminatorVoltageFault	= ( unAlertsStatus & ALERT_ILLUM_VOLTAGE_FAULT ? 1 : 0 );
		m_dtNewAlerts.bIlluminatorCurrentFault	= ( unAlertsStatus & ALERT_ILLUM_CURRENT_FAULT ? 1 : 0 );
		m_dtNewAlerts.bLeftSensorTempFault		= ( unAlertsStatus & ALERT_LEFT_SENSOR_TEMP ? 1 : 0 );
		m_dtNewAlerts.bRightSensorTempFault		= ( unAlertsStatus & ALERT_RIGHT_SENSOR_TEMP ? 1 : 0 );
		m_dtNewAlerts.bMainBoardTempFault		= ( unAlertsStatus & ALERT_MAIN_BOARD_TEMP ? 1 : 0 );
		m_dtNewAlerts.bBatteryFaultAlarm		= ( unAlertsStatus & ALERT_BATTERY_FAULT ? 1 : 0 );
		m_dtNewAlerts.bBumpDetectedAlarm		= ( unAlertsStatus & ALERT_BUMP_DETECTED ? 1 : 0 );
		m_dtNewAlerts.bCableFaultAlarm			= ( unAlertsStatus & ALERT_CABLE_FAULT ? 1 : 0 );
		m_dtNewAlerts.bFirmwareIncompatible		= ( unAlertsStatus & ALERT_FIRMWARE_INCOMPAT ? 1 : 0 );
		m_dtNewAlerts.bNonFatalParamFault		= ( unAlertsStatus & ALERT_NON_FATAL_PARAM_FAULT ? 1 : 0 );
		m_dtNewAlerts.bInternalFlashFull		= ( unAlertsStatus & ALERT_INTERNAL_FLASH_FULL ? 1 : 0 );
		m_dtNewAlerts.bLaserBatteryFaultAlarm	= ( unAlertsStatus & ALERT_LASER_BATTERY_FAULT ? 1 : 0 );
		m_dtNewAlerts.bTempTooHigh				= ( unAlertsStatus & ALERT_TEMP_TOO_HIGH ? 1 : 0 );
		m_dtNewAlerts.bTempTooLow				= ( unAlertsStatus & ALERT_TEMP_TOO_LOW ? 1 : 0 );
	} /* if */

	return 1;	
}

/***********************************************************
  Name: nLookupTimeout

  Returns: int 
  Argument: char *szCommand

  Description: Looks up the command in the Timeout value list
				(m_dtTimeoutValues) and returns the timeout 
				value for the specified command
***********************************************************/
int CCommandHandling::nLookupTimeout(char *szCommand)
{
	CString sCommand( szCommand );
	int nTimeoutValue;

	/*
	 * Please note that the Get Info.Timeout.* functionality is
	 * enabled on the Aurora, but the timeout values returned may
	 * not work for the communication speed selected. For this reason
	 * we will use the Default Timeout Value.
	 */
	if( m_dtSystemInformation.nTypeofSystem == AURORA_SYSTEM )
		return m_nDefaultTimeout;

	int nPos = sCommand.FindOneOf( " :" );

	if (nPos > 0)
		sCommand = sCommand.Left( nPos );
	else 
		return m_nDefaultTimeout;
	
	if (!m_dtTimeoutValues.Lookup( sCommand, nTimeoutValue ))
		return m_nDefaultTimeout;

	return nTimeoutValue;	
}


/***********************************************************
  Name: CreateTimeoutTable

  Returns: int 

  Description: Retrieves the timeout values for each API 
				command and creates a list of them
***********************************************************/
int CCommandHandling::CreateTimeoutTable()
{
	char *pszTimeoutInfo = NULL;
	char szTimeoutString[] = "Info.Timeout.\0";

	m_dtTimeoutValues.RemoveAll();
	memset(m_szCommand, 0, sizeof(m_szCommand));
	sprintf( m_szCommand, "GET Info.Timeout.*" );
	if ( nSendMessage( m_szCommand, TRUE ) )
	{
		if (!nGetResponse( ))
		{
			return 0;
		}/* if */

		if( !nVerifyResponse(m_szLastReply, TRUE))
			return 0;

		/* Parsing Timeouts */
		pszTimeoutInfo = m_szLastReply;

		char *chValue, *chNext;
		while ( strchr( pszTimeoutInfo, '=') )
		{
			pszTimeoutInfo += strlen(szTimeoutString);
			chValue = strchr( pszTimeoutInfo, '=' );
			*chValue++ = 0;
			chNext = strchr( chValue, LINE_FEED );
			*chNext++ = 0;

			int a = atoi( chValue );
			m_dtTimeoutValues.SetAt( pszTimeoutInfo, a );

			pszTimeoutInfo = chNext;
		}

	} /* if */

	return 1;	
}

/*****************************************************************
Name:				nSendMessage

Inputs:
	char * m_szCommand - the command string, coming in.  The command
						 to be sent to the System
	bool bAddCRC - if true, we add the CRC to the command and replace the
				   space with the :

Return Value:
	int -  0 if fails, 1 if passes.

Description:   
	This command takes in a command string and parses it depending
	on the value of bAddCRC.  If bAddCRC is true, we replace the 
	space with a : and calculate and add the CRC to the command.
	We then send the command to the System.
*****************************************************************/
int CCommandHandling::nSendMessage( char *m_szCommand, bool bAddCRC )
{
	unsigned int 
		i;
	bool 
		bComplete = FALSE;

	/* Check COM port */
	if( pCOMPort == NULL )
	{
		return bComplete;
	}/* if */

	m_nTimeout = nLookupTimeout( m_szCommand );

	/* build the command, by adding a carraige return to it and crc if specified */
	if (!nBuildCommand( m_szCommand, bAddCRC ))
		return bComplete;

	if(strlen(m_szCommand) >= (MAX_COMMAND_MSG))
	{
		return bComplete;
	} /* if */

	for( i = 0; i < strlen(m_szCommand); i++ )
	{
		if ( pCOMPort->SerialPutChar( m_szCommand[i] ) <= 0 )
		{
			bComplete = FALSE;
			break;
		} /* if */
		else if( m_szCommand[i] == CARRIAGE_RETURN )
		{
			bComplete = TRUE;
			break;
		}/* if */
	} /* for */

	/*flush the COM Port...this sends out any info still sitting in the buffer*/
	pCOMPort->SerialFlush();
	LogToFile( 0, m_szCommand );

	return bComplete;
} /* nSendMessage */


/*****************************************************************
Name:				nGetResponse

Inputs:
	None.

Return Value:
	int - 1 if passes, else 0

Description:   
	This routine gets the response from the system that is to be 
	poled through the com port.  The routine gets it start time 
	( the time it started polling ) and continues until one of two
	things happens.  First, if the end of response (Carriage Return)
	if found the reply is complete and 1 is returned.  If the time
	it takes to get a response exceeds the timeout value, the system
	assumes no response is coming and timeouts.  The timeout dialog
	is then displayed.  Use this response routine for all calls except
	the BX call.
*****************************************************************/
int CCommandHandling::nGetResponse()
{
	char
		chChar;
	time_t 
		currenttime,
		starttime;
	BOOL 
		bDone = FALSE;
	int 
		nCount = 0,
		nRet = 0,
		nRetry = 0;

	CComPortTimeout
		*ComPortTimeout = NULL;

    memset(m_szLastReply, 0, sizeof( m_szLastReply ) );

	/* 
	 * Get the start time that the call was initialized. 
	 */
	time( &starttime );

	do
	{
		/* Check COM port */
		if( pCOMPort == NULL )
		{
			return FALSE;
		}/* if */

		while ( (pCOMPort->SerialCharsAvailable() > 0) && (!bDone) )
		{
        	chChar = pCOMPort->SerialGetChar();
			/* if carriage return, we are done */
			if ( chChar == '\r' )
   			{
				m_szLastReply[nCount] = CARRIAGE_RETURN;
				m_szLastReply[nCount+1] = '\0';
			   	bDone = TRUE;
			} /* if */
			else
			{
				m_szLastReply[nCount] = chChar;
				nCount++;
			} /* else */
		} /* while */

		if ( !bDone )
		{
			/*
			 * Get the current time and compare with start time
			 * if longer "timeout" assume no response and timeout
			 */
			time( &currenttime );

			if ( difftime( currenttime, starttime ) >= m_nTimeout ) 
			{
				/*
				 * If a COM port timeout is noted, we will try to
				 * send the command again, up to 3 times.
				 */
				nCount = 0;
				if( nRetry < 1 )
				{
					nRetry++;
					memset( m_szLastReply, 0, sizeof(m_szLastReply) );

					/* 
					 * Do not clear the m_szCommand at this point, since
					 * we are re-sending the same command.
					 */
					//nSendMessage( m_szCommand, FALSE ); /* Command already has CRC */
					
					 /* Reset the start time. */
					time( &starttime );
				}
				else
				{
					return FALSE;

				}/* else */
			}/* if */
		} /* if */
	} while ( !bDone );

	LogToFile( 1, m_szLastReply );

	return 1;
} /* nGetResponse */


/*****************************************************************
Name:				nGetBinaryResponse

Inputs:
	None.

Return Value:
	int - 1 if passes, else 0

Description:   
	This routine gets the response from the system that is to be 
	poled through the com port.  The routine gets its start time 
	( the time it started polling ) and continues until one of two
	things happens.  First, if the end of response ( the number of bytes
	specified in the header is found ) the reply is complete and 1 is 
	returned.  If the time it takes to get a response exceeds the timeout 
	value, the system assumes no response is coming and timeouts.  The 
	timeout dialog 	is then displayed.  Use this response routine for 
	all calls except the BX call.
*****************************************************************/
int CCommandHandling::nGetBinaryResponse( )
{
	char
		chChar;
	time_t 
		currenttime,
		starttime;
	BOOL 
		bDone = FALSE;
	int 
		nTotalBinaryLength = -1, //initialize it to a number smaller than nCount
		nCount = 0,
		nRet = 0,
		nRetry = 0;

	CComPortTimeout
		*ComPortTimeout = NULL;

    memset(m_szLastReply, 0, sizeof( m_szLastReply ) );


	/* 
	 * Get the start time that the call was initialized. 
	 */
	time( &starttime );

	do
	{
		/* Check COM port */
		if( pCOMPort == NULL )
		{
			return FALSE;
		}/* if */

		while ( (pCOMPort->SerialCharsAvailable() > 0) && (!bDone) )
		{
			chChar = pCOMPort->SerialGetChar();

			m_szLastReply[nCount] = chChar;

			/*
			 * Get the total length of the buffer
			 */
			if ( nCount == 3 )  
			{
				/* + 7 to account for header information */					
				nTotalBinaryLength = nGetHex2(&m_szLastReply[2]) + 7 + 1; 
			}/* if */

			nCount++;

			if ( nCount == nTotalBinaryLength )
			{
			   	bDone = TRUE;			
			}/* if */
		} /* while */

		if ( !bDone )
		{
			/*
			 * Get the current time and compare with start time
			 * if longer "timeout" assume no response and timeout
			 */
			time( &currenttime );

			if ( difftime( currenttime, starttime ) >= m_nTimeout ) 
			{
				/*
				 * If a COM port timeout is noted, we will try to
				 * send the command again, up to 3 times.
				 */
				nCount = 0;
				if( nRetry < 3 )
				{
					nRetry++;
					memset( m_szLastReply, 0, sizeof(m_szLastReply) );

					/* 
					 * Do not clear the m_szCommand at this point, since
					 * we are re-sending the same command.
					 */
					nSendMessage( m_szCommand, FALSE ); /* Command already has CRC */

					 /* Reset the start time. */
					time( &starttime );
				}
				else
				{
					/*
					 * If a COM port timeout is noted again, the communication 
					 * error seems not recoverable and we will stop sending 
					 * the command and spawns a dialog. The dialog allows 
					 * the user to retry the current command, restart the application
					 * or close the application. You could eliminate this dialog and 
					 * return false from this area and handle the false return 
					 * directly.
					 */
					ComPortTimeout = new CComPortTimeout;
					memset( m_szLastReply, 0, sizeof(m_szLastReply) );
					ComPortTimeout->DoModal();
					nRet = ComPortTimeout->m_nReturnValue;
					delete( ComPortTimeout );

					/* 
					 * if the user chooses to retry sending the command
					 * handle that here.
					 */
					if ( nRet == ERROR_TIMEOUT_CONT )
					{
						if ( strlen(m_szCommand) > 0 )
						{
							nRetry = 1;
							memset( m_szLastReply, 0, sizeof(m_szLastReply) );

							/* 
							 * Do not clear the m_szCommand at this point, since
							 * we are re-sending the same command.
							 */
							nSendMessage( m_szCommand, FALSE ); /* Command already has CRC */

							 /* Reset the start time. */
							time( &starttime );
						}
						else
						{
							pCOMPort->SerialBreak();
						} /* else */
					} 
					else
					{
						return FALSE;
					}/* else */
				}/* else */
			}/* if */
		} /* if */
	} while ( !bDone );

	LogToFile( 1, m_szLastReply );
	return bDone;

} /* nGetBinaryResponse */


/*****************************************************************
Name:				nCheckResponse

Inputs:
	int nReturnedValue - the value returned by nVerifyResponse

Return Value:
	int - 1 if the response is valid, 0 if the response is invalid
		  or an error.

Description:   
	This routine checks the value from nVerifyResponse.
	The following occurs:
	REPLY_ERROR - the response from the system was an error, we
				  beep the system if required and post the error
				  message ( ErrorMessage() )
	REPLY_BADCRC - a bad crc was returned with the response
				   i.e. the crc returned doesn't match the one
				   calculated for the response. Post a message
	REPLY_WARNING - the warning message was recieve from the system
					while intializing a tool (see API for reasons)
					post a message and beep if required.
	REPLY_INVALID - an invalid response was received from the system
					post a message
*****************************************************************/
int CCommandHandling::nCheckResponse( int nReturnedValue )
{
	if ( nReturnedValue == REPLY_ERROR )
	{
		//ErrorMessage();
		return 0;
	} /* if */

	if ( nReturnedValue == REPLY_BADCRC )
	{
		//MessageBox( NULL, "Bad CRC received with reply.", "CRC Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
		return 0;
	} /* if */

	if ( nReturnedValue == REPLY_WARNING )
	{
		//WarningMessage();
		return 1;
	} /* if */

	
	if ( nReturnedValue == REPLY_INVALID )
	{
		MessageBox( NULL, "Invalid response received from the system", 
					"Invalid Response", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );

		return 0;
	} /* if */


	return 1;
} /* nCheckResponse */
/*****************************************************************
Name:				nVerifyResponse

Inputs:
	char * pszReply - the reply to verify
	bool bCheckCRC - perform the CRC check on the response

Return Value:
	int - the response that is found, defined in the APIStructures.h

Description:    This routine checks the given response for the predetermined
				response values and returns the corresponding reply value.
*****************************************************************/
int CCommandHandling::nVerifyResponse( char *pszReply, bool bCheckCRC )
{
	int
		nResponse = 0;

	/* verify the response by comparing it with the possible responses */
	if (!strnicmp(pszReply, "RESET",5))
		nResponse = REPLY_RESET;
	else if (!strnicmp(pszReply, "OKAY",4))
		nResponse = REPLY_OKAY;
	else if (!strnicmp(pszReply, "ERROR",5))
		nResponse = REPLY_ERROR;
	else if (!strnicmp(pszReply, "WARNING",7))
		nResponse = REPLY_WARNING;
	else if ( strlen( pszReply ) > 0 )
		nResponse = REPLY_OTHER;
	else
		return REPLY_OTHER;

	if ( nResponse & REPLY_OKAY  || nResponse & REPLY_OTHER && bCheckCRC )
	{
		if (!SystemCheckCRC( pszReply ) )
			return REPLY_BADCRC;
		else
			return nResponse;
	} /* if */
	else
		return nResponse;
} /* nVerifyResponse */
/*****************************************************************
Name:				ErrorMessage

Inputs:
	None.

Return Value:
	None.

Description:    This routine loads and displays the error that
				corresponds with the reply string from the system.
*****************************************************************/
void CCommandHandling::ErrorMessage()
{
	char
		pchErrorMessage[256],
		pchErrorNumber[8];
	bool
		bBeepOnError = FALSE;
	int
		nNoErrorBeeps = 0;

	/* get the error response and display it */
	GetErrorResponse( m_szLastReply, pchErrorMessage );
	strncpy( pchErrorNumber, m_szLastReply, 7 );
	pchErrorNumber[7] = '\0';

	ReadINIParm( "Beeping Options", "Beep On Error", "0", &bBeepOnError );
	ReadINIParm( "Beeping Options", "No Error Beeps", "1", &nNoErrorBeeps );

	if (bBeepOnError)
		nBeepSystem( nNoErrorBeeps );

	MessageBox( NULL, pchErrorMessage, pchErrorNumber, MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
} /* ErrorMessage */

/*****************************************************************
Name:				WarningMessage

Inputs:
	None.

Return Value:
	None.

Description:    This routine loads and displays the warning that
				corresponds with the reply string from the system.
*****************************************************************/
void CCommandHandling::WarningMessage()
{
	char
		pchWarningMessage[256],
		pchWarningNumber[10];
	bool
		bBeepOnWarning = FALSE;
	int
		nNoWarningBeeps = 0;

	GetErrorResponse( m_szLastReply, pchWarningMessage );
	strncpy( pchWarningNumber, m_szLastReply, 9 );
	pchWarningNumber[9] = '\0';

	ReadINIParm( "Beeping Options", "Beep On Warning", "0", &bBeepOnWarning );
	ReadINIParm( "Beeping Options", "No Warning Beeps", "1", &nNoWarningBeeps );

	if (bBeepOnWarning)
		nBeepSystem( nNoWarningBeeps );

	MessageBox( NULL, pchWarningMessage, pchWarningNumber, MB_ICONWARNING|MB_SYSTEMMODAL|MB_SETFOREGROUND );

} /* ErrorMessage */

/*****************************************************************

Name:			  LogToFile

Inputs:			int nDirection - the direction of the message
					0 for out
					1 for in
				char * psz - the message to write to the file
Return Value:
	None.

Description:	this routine writes the information sent and 
				received from the COM port to a file, who's name
				is defined by the user and stored in the INI file			
*****************************************************************/
void CCommandHandling::LogToFile(int nDirection,char *psz)
{
	int
		n;
	FILE
		*pfOut = NULL;
	time_t 
		ltime;
	char 
		*pszTimeStamp = NULL,
		szMessage[256];

	if (!m_bLogToFile)
		return;

	pfOut = fopen(m_szLogFile,"a+t");
	if(pfOut == NULL)
	{
		sprintf( szMessage, "The log file (%s) could not be opened.\n"
							"COM Port logging will be turned off.\n\n"
							"To reactivate this feature, please do so under\n"
							"the Options menu.", m_szLogFile );
		m_bLogToFile = FALSE;
		WriteINIParm( "Logging Options", "Log To File", m_bLogToFile );
		MessageBox( NULL, szMessage, "Log File Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
		return;
	}

	if ( m_bDateTimeStampFile )
	{
		time( &ltime );
		pszTimeStamp = ctime( &ltime );
		pszTimeStamp = _strtime(pszTimeStamp);
		do
		{
			putc( *pszTimeStamp, pfOut );
			pszTimeStamp++;
		} while (*pszTimeStamp);		
	} /* if */

	putc(nDirection?'<':'>',pfOut);
	putc(' ',pfOut);

	if ( !*psz )
	{
		psz[0] = CARRIAGE_RETURN;
	}/* if */

	/* BX reply */
	if ( ((psz[0] & 0xff) == 0xc4) &&
		 ((psz[1] & 0xff) == 0xa5) ) 
	{
		fprintf(pfOut, "BINARY REPLY");		
	}/* if */
	else
	{
		for(n=0;
			 (n<MAX_REPLY_MSG) && (psz[n] != CARRIAGE_RETURN);
			 putc(psz[n++],pfOut));
	}/* else */

	putc(LINE_FEED,pfOut); 
	putc(LINE_FEED,pfOut);

	fclose (pfOut);
}

void CCommandHandling::SetINIFileName(char* pFileName)
{
	SetMGRINIFileName(pFileName);
}

int CCommandHandling::nSetSROMFiles(CArray<CString, CString> *arrSROMFiles)
{
	int iCnt = arrSROMFiles->GetSize();
	for (int i=0; i<iCnt; i++)
	{
		m_strPassivePortSROMFiles[i] = arrSROMFiles->GetAt(i);
	}
	m_bPassivePortSROMFilesOutSeted = TRUE;
	return 1;
}

/**************************END OF FILE***************************/

