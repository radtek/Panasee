/*****************************************************************
Name:			Comm32.cpp

Description:    this is the class that will control COM Port
				(serial) communication within a 32 bit environment.
				It will work with all levels of Windows (excluding
				old 16 bit versions)


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

/*****************************************************************
ND Library Files Included
*****************************************************************/

/*****************************************************************
Project Files Included
*****************************************************************/
#include "comm32.h"

/*****************************************************************
Defines
*****************************************************************/

/*****************************************************************
Global Variables
*****************************************************************/

Comm32Port::Comm32Port()
{
	m_bRtsCts = TRUE;
	m_ulSerBreakDelay = 100;  /* 100 milliseconds */
	m_nNumBytes = 0;
	m_nIndexBuffer = 0;
	m_hCommPort = INVALID_HANDLE_VALUE;

	/*
	CommTimeouts: Set COMM timeouts to allow ReadFile return immediately with
				  number of bytes read and WriteFile return after the specified 
				  timeouts.
	*/
	CommTimeouts.ReadIntervalTimeout = MAXWORD;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;
	
	memset(m_gruchInputBuffer, '\0', sizeof(m_gruchInputBuffer));
}

Comm32Port::~Comm32Port()
{
	SerialClose();
}
/*****************************************************************

Routine:   SerialClose

Inputs:
    None.

Returns:
	None.

Description:
	This routine closes the COM Port handle and thus the COM Port.
	It releases the hold it has on it so other programs can use it.
	Always close the COM Port once you are finished.
*****************************************************************/
void Comm32Port::SerialClose()
{
	if( m_hCommPort != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hCommPort );
		m_hCommPort = INVALID_HANDLE_VALUE;
	} /* if */
} /* SerialClose */
/*****************************************************************

Routine:   SerialOpen

Inputs:		unsigned uPort - the port number to open ( 0 - 8, 0 = COM1 )
			unsigned long ulBaudRate - the baud rate for the COM Port
			unsigned uFormat - the format to open the COM port with COMM_7E1, COMM_7O1, COMM_8N1
			boolean bRtsCts - whether or not to include hardware handshaking
			unsigned long ulSerBreakDelay - how long to wait on a reset call

Returns:
	int - COMM_OK (1) if successful

Description:
	This routine opens up the selected COM port to be used for all
	calls to the hardware and responses from the hardware
*****************************************************************/
int Comm32Port::SerialOpen(unsigned uPort, unsigned long ulBaudRate, unsigned uFormat, 
					   bool bRtsCts, unsigned long ulSerBreakDelay)
{
	/*
	 * attempt to establish communciation with the COMM port
	 */
	DCB dcb = {0};

	/*
	 * give the ability to support COM ports that are higher than
	 * COM 9, with the USB convertor, Windows will assign the next
	 * available COM port and this can be above COM 9
	 */ 
	char szCom[19][15] =
						 {"COM1",
						  "COM2",
						  "COM3",
						  "COM4",
						  "COM5",
						  "COM6",
						  "COM7",
						  "COM8",
						  "COM9",
						  "\\\\.\\COM10",
						  "\\\\.\\COM11",
						  "\\\\.\\COM12",
						  "\\\\.\\COM13",
						  "\\\\.\\COM14",
						  "\\\\.\\COM15",
						  "\\\\.\\COM16",
						  "\\\\.\\COM17",
						  "\\\\.\\COM18",
						  "\\\\.\\COM19"};

	m_bRtsCts = bRtsCts;
	m_ulSerBreakDelay = ulSerBreakDelay;	

	/*
	 * 32 bit serial comm port implementation
	 */
	m_hCommPort = CreateFile(szCom[uPort],
					 GENERIC_READ | GENERIC_WRITE,
					 0,    /* comm devices must be opened w/exclusive-access */
					 0,    /* no security attrs */
					 OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
					 FILE_FLAG_OVERLAPPED,    /* Required for asynchronous write operations */
					 NULL  /* hTemplate must be NULL for comm devices */
					);

	if( m_hCommPort == INVALID_HANDLE_VALUE)
		return 0;//COMM_NOT_PRESENT;

	SetupComm(m_hCommPort, READ_BUFFER_SIZE, WRITE_BUFFER_SIZE );

	/*
	 * Get the current configuration.
	 */
	if (!GetCommState(m_hCommPort, &dcb))
		return 0;//COMM_NOT_PRESENT;

	dcb.BaudRate = ulBaudRate;

	if( uFormat == (unsigned)COMM_8N1 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* if */

	else if( uFormat == (unsigned)COMM_8N2 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8O1 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8O2 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8E1 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8E2 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7N1 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7N2 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7O1 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7O2 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7E1 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7E2 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */
	
	else
	{
		::MessageBox(NULL,"Invalid COM Port Settings. Please change your settings.",
					 "COM Port Settings Error",MB_ICONERROR|MB_OK|MB_SETFOREGROUND);
	} /* else if */


	if( m_bRtsCts )
	{
		dcb.fOutxCtsFlow = 1;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	} /* if */
	else
	{
		dcb.fOutxCtsFlow = 0;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
	} /* else */

	if (!SetCommState(m_hCommPort, &dcb))
		return 0;

	/* Set COMM timeouts */
	if (!SetCommTimeouts(m_hCommPort, &CommTimeouts))
		return 0;

	/* Discards all characters from the output or input buffer */
	PurgeComm(m_hCommPort, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	return 1;

} /* SerialOpenComm */
/*****************************************************************

Routine:   SerialSetBaud

Inputs:		unsigned long ulBaudRate - the Baud Rate setting for the COM Port
			unsigned uFormat - the format to set for the COM Port ( stop bits, data bits and parity )
			boolean bRtsCts - hardware handshaking ( on or off )
			unsigned long ulSerBreakDelay - the delay to wait while send a reset command

Returns:
	int 1 if successful

Description:
	This routine sets the COM Port settings after the COM Port has
	been open, usually the COM Port is opened with default settings
	(ie BAUD_RATE = 9600) and after it has been open the other settings
	are set.
*****************************************************************/
int Comm32Port::SerialSetBaud( unsigned long ulBaudRate, unsigned uFormat, bool bRtsCts, 
					   unsigned long ulSerBreakDelay)
{
	DCB dcb = {0};
	m_bRtsCts = bRtsCts;
	m_ulSerBreakDelay = ulSerBreakDelay;

	/*
	 * Get the current configuration.
	 */
	if (!GetCommState(m_hCommPort, &dcb))
		return COMM_NOT_INITIALIZED;

	dcb.BaudRate = ulBaudRate;

	if( uFormat == (unsigned)COMM_8N1 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* if */

	else if( uFormat == (unsigned)COMM_8N2 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8O1 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8O2 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8E1 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_8E2 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 8;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7N1 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7N2 )
	{
		dcb.fParity = 0;
		dcb.Parity = NOPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7O1 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7O2 )
	{
		dcb.fParity = 1;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7E1 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 7;
	} /* else if */

	else if( uFormat == (unsigned)COMM_7E2 )
	{
		dcb.fParity = 1;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.ByteSize = 7;
	} /* else if */
	
	else
	{
		::MessageBox(NULL,"Invalid COM Port Settings. Please change your settings.",
					 "COM Port Settings Error",MB_ICONERROR|MB_OK|MB_SETFOREGROUND);
	} /* else if */

	if( m_bRtsCts )
	{
		dcb.fOutxCtsFlow = 1;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	} /* if */
	else
	{
		dcb.fOutxCtsFlow = 0;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
	} /* else */

	if (!SetCommState(m_hCommPort, &dcb))
		return COMM_NOT_INITIALIZED;

	/* Set COM timeouts. */
	if (!SetCommTimeouts(m_hCommPort, &CommTimeouts))
		return COMM_ERROR_SETTIMEOUT;

	return COMM_OK;

} /* SerialSetBaudRate */
/*****************************************************************

Routine:   SerialBreak()

Inputs:
	None.

Returns:
	int COMM_OK if successful

Description:
	This routine resets the COM PORT by sending a Serial Break to
	it.
*****************************************************************/
int Comm32Port::SerialBreak()
{
	if( m_hCommPort == INVALID_HANDLE_VALUE )
		return COMM_NOT_INITIALIZED;

	if (!SetCommBreak( m_hCommPort ))
		return COMM_ERROR_SETBREAK;

	Sleep( m_ulSerBreakDelay );

	if (!ClearCommBreak( m_hCommPort ))
		return COMM_ERROR_CLEARBREAK;

	return COMM_OK;

} /* SerialBreak */
/*****************************************************************

Routine:   SerialFlush

Inputs:
	None.

Returns:
	int

Description:
	This routine flushed the output buffer of the COM Port to insure
	that any commands waiting in the buffer are sent out of the 
	buffer and to the attached hardware.
*****************************************************************/
int Comm32Port::SerialFlush()
{
	if( m_hCommPort == INVALID_HANDLE_VALUE )
		return COMM_NOT_INITIALIZED;

	if (!FlushFileBuffers( m_hCommPort ))
		return COMM_ERROR_CLEARBUFFER;
	
	return COMM_OK;

} /* SerialFlush */
/*****************************************************************

Routine:   SerialPutChar

Inputs:
	unsigned char uch - the char to be placed in the COM Port
	out buffer

Returns:
	int

Description:
	This routine takes a single character and places it in the output
	buffer to be sent ot the attached hardware.
*****************************************************************/
int Comm32Port::SerialPutChar(unsigned char uch)
{
	int
		nRes = 0;

	unsigned char 
		szString[1];
	
	szString[0] = uch;

	nRes = SerialPutString(szString, 1);
	
	if (nRes < 0)
		return nRes;
	else
		return COMM_OK;

} /* SerialPutChar */
/*****************************************************************

Routine:   SerialErrorStatus

Inputs:
	None.

Returns:
	int

Description:
	This routine checks the error status of the COM Port (serial)
	and returns the error status.
*****************************************************************/
int Comm32Port::SerialErrorStatus()
{
	COMSTAT comStat  = {0};
	DWORD   dwErrors = 0;
	int nErrorCode  = 0;

	if( m_hCommPort == INVALID_HANDLE_VALUE )
		return COMM_NOT_INITIALIZED;

	if( !ClearCommError( m_hCommPort, &dwErrors, &comStat ))
		return COMM_NOT_PRESENT;

	if( dwErrors & CE_BREAK )
		nErrorCode |= COMM_ERR_BREAK;

	if(dwErrors & CE_FRAME )
		nErrorCode |= COMM_ERR_FRAME;

	if(dwErrors & CE_RXPARITY )
		nErrorCode |= COMM_ERR_PARITY;

	if( dwErrors & CE_OVERRUN )
		nErrorCode |= COMM_ERR_OVERRUN;

	return nErrorCode;

} /* SerialErrorStatus */
/*****************************************************************

Routine:   SerialPutString

Inputs:		
	unsigned char *pszString - the string to be place in the out buffer
	of the serial port
	unsigned long ulLen - the length of the buffer to be placed in the 
	out buffer of the serial port

Returns:
	int

Description:
	this routine takes the string and size to be placed in the out buffer
	of the serial port and then writes that string to the hardware.  It checks
	to see if it wrote as much as was requested.
*****************************************************************/
int Comm32Port::SerialPutString( unsigned char *pszString, unsigned long ulLen )
{
	OVERLAPPED	
		osWrite = {0};

	DWORD	
		dwResult  = 0;  
	
	unsigned long 
		ulBytesWritten = 0;

	BOOL
		bRes = FALSE;
	int 
		nRet = 0;

	if( m_hCommPort == INVALID_HANDLE_VALUE )
		return COMM_NOT_INITIALIZED;
	
	/* Create the overlapped event. Must be closed before exiting */
	/* to avoid a handle leak. */
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if( osWrite.hEvent == NULL )
	   	return COMM_ERROR_CREATEEVENT;
	
	/* Issue write operation */
 	if( !WriteFile(m_hCommPort, pszString, ulLen, &ulBytesWritten, &osWrite) )
	{
		if( GetLastError() != ERROR_IO_PENDING )
		{
			nRet = COMM_ERROR_WRITE; 
			bRes = FALSE; /* write failed, report error */
		} /* if */
		else
		{
			/* Write is pending */
			dwResult = WaitForSingleObject(osWrite.hEvent, INFINITE); 
			switch(dwResult)
			{
			case WAIT_OBJECT_0:
				if( !GetOverlappedResult(m_hCommPort, &osWrite, &ulBytesWritten, FALSE))
				{
					nRet = COMM_ERROR_GETOVERLAPPED;
					bRes = FALSE;
				} /* if */
				else
				{
					if (ulBytesWritten == ulLen)
						bRes = TRUE;
					else
					{
						nRet = COMM_ERROR_WRITETIMEOUT;
						bRes = FALSE;/*write operation timed out, abort.*/
					} /* else */
				} /* else */
				break;

			case WAIT_TIMEOUT:
				nRet = COMM_ERROR_WAITTIMEOUT;
				break;

			default:
				nRet = COMM_ERROR_WAIT;
				bRes = FALSE;
				break;
			} /* siwtch */
		} /* else */
    } /* if */
	else
	{
		/* WriteFile completed immediately */
		if (ulBytesWritten == ulLen)
			bRes = TRUE;
		else
		{
			nRet = COMM_ERROR_WRITETIMEOUT;
			bRes = FALSE;//write operation timed out, abort.
		} /* else */
	} /* else */

	CloseHandle(osWrite.hEvent);

	if (bRes == TRUE)
		return ulBytesWritten;
	else
		return nRet; 

} /* SerialPutString */
/*****************************************************************

Routine:   SerialGetChar

Inputs:		
	None.

Returns:
	int

Description:
	This routine gets a single char from the input buffer of the serial
	port.  It can only get what is there so it is a good idea to check
	if there is data in the input buffer using SerialCharsAvailable
*****************************************************************/
int Comm32Port::SerialGetChar()
{
	int nRetChar = COMM_NO_DATA;

	if( !m_nNumBytes )
	{
		m_nNumBytes = SerialGetString( m_gruchInputBuffer, READ_BUFFER_SIZE );
		m_nIndexBuffer = 0;
	} /* if */

	if( m_nNumBytes > 0 )
	{
		nRetChar = m_gruchInputBuffer[m_nIndexBuffer];
		m_nIndexBuffer++;
		m_nNumBytes--;
	} /* if */

	return nRetChar;
} /* SerialGetChar */
/*****************************************************************

Routine:   SerialCharsAvailable

Inputs:		
	None.

Returns:
	int

Description:
	This routine check to see if there are characters in the input buffer
	to be read.  It is a good idea to call this routine before actually 
	reading the buffer.
*****************************************************************/
int Comm32Port::SerialCharsAvailable()
{
	/* 
	 * to get the number of avalable chararacters .
	*/
	if( !m_nNumBytes )
	{
		m_nNumBytes = SerialGetString( m_gruchInputBuffer, READ_BUFFER_SIZE );
		m_nIndexBuffer = 0;
	}
	return m_nNumBytes;

} /* SerialCharsAvailable */
/*****************************************************************

Routine:   SerialGetString

Inputs:	
	unsigned char *pszString -  the buffer to hold the string that was read
	unsigned long ulMaxLen - the maximum length the input string can be

Returns:
	int

Description:
	This routine reads in a string value from the input buffer of the 
	COM Port.  It will read in a buffer up to ulMaxLen or the number of 
	bytes recieved which ever is smaller.
*****************************************************************/
int Comm32Port::SerialGetString(unsigned char *pszString, unsigned long ulMaxLen )
{
	OVERLAPPED	
				osRead = {0};

	DWORD		dwResult = 0,
				dwRead   = 0,
				dwErrors = 0;  
	
	unsigned long 
				ulBytesRead   = 0,
				ulBytesToRead = 0;

	COMSTAT		comStat = {0};

	BOOL		bWaitingOnRead = FALSE,
				bRes = FALSE;

	int
				nRet = 0;

	if( m_hCommPort == INVALID_HANDLE_VALUE )
		return COMM_NOT_INITIALIZED;

	if( !ClearCommError( m_hCommPort, &dwErrors, &comStat ))
		return COMM_NOT_PRESENT;

	if( comStat.cbInQue < ulMaxLen)
		ulBytesToRead = comStat.cbInQue;
	else
		ulBytesToRead = ulMaxLen;

	/* Create the overlapped event. Must be closed before exiting */
	/* to avoid a handle leak. */
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if( osRead.hEvent == NULL )
	   	return COMM_ERROR_CREATEEVENT;

	/* issue read operation */
	if (!bWaitingOnRead)
	{
		/* Issue read operation */
		if( !ReadFile(m_hCommPort, pszString, ulBytesToRead, &ulBytesRead, &osRead) )
		{
			if( GetLastError() != ERROR_IO_PENDING )
			{
				nRet = COMM_ERROR_READ;
				bRes = FALSE; /* read failed, report error */
			} /* if */
			else
				bWaitingOnRead = TRUE;
		} /* if */
		else
		{
			/* read completed immediately */
			bRes = TRUE;
		} /* else */
	} /* if */

	//detection of completion
	if (bWaitingOnRead)
	{
		dwResult = WaitForSingleObject(osRead.hEvent, 1000); // 1 second time-out
		switch(dwResult)
		{
		case WAIT_OBJECT_0:
			if( !GetOverlappedResult(m_hCommPort, &osRead, &dwRead, FALSE))
			{
				nRet = COMM_ERROR_GETOVERLAPPED;
				bRes = FALSE; // error in communication, report error
			} /* if */
			else
				bRes = TRUE;
			
			//Reset flag so that another operation can be issued
			bWaitingOnRead = FALSE;
			break;

		case WAIT_TIMEOUT:
			nRet = COMM_ERROR_WAITTIMEOUT;
			break; // operation is not completed yet

		default:
			nRet = COMM_ERROR_WAIT;
			bRes = FALSE; // error in the waitForSingleObject, abort.
			break;
		} /* switch */
	} /* if */

	CloseHandle(osRead.hEvent);

	if (bRes == TRUE)
		return ulBytesRead;
	else
		return nRet;

} /* SerialGetString */
/**************************END OF FILE***************************/
