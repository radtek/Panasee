/*****************************************************************
Name:               Comm32.h

Description:	This is the header for the comm32 routines that
				communicate with the RS-232 port.


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
Defines   
*****************************************************************/
#ifndef COMM32_H_INCLUDED
#define COMM32_H_INCLUDED

#include "windows.h"

/*
 * Serial data formats
 */
#define COMM_8N1    0x00 
#define COMM_8N2	0x01
#define COMM_8O1	0x02
#define COMM_8O2	0x03
#define COMM_8E1	0x04
#define COMM_8E2	0x05

#define COMM_7N1 	0x06   
#define COMM_7N2	0x07
#define COMM_7O1	0x08 
#define COMM_7O2	0x09
#define COMM_7E1	0x0A 
#define COMM_7E2	0x0B

#define COMM_OK                 1
#define COMM_IRQ_IN_USE         -1
#define COMM_OUT_OF_MEMORY      -2
#define COMM_NOT_INITIALIZED    -3
#define COMM_OUT_BUFFER_FULL    -4
#define COMM_NOT_PRESENT        -5
#define COMM_NO_DATA			-6

#define COMM_ERROR_SETTIMEOUT		-10
#define COMM_ERROR_SETBREAK			-11
#define COMM_ERROR_CLEARBREAK		-12
#define COMM_ERROR_CLEARBUFFER		-13
#define COMM_ERROR_CREATEEVENT		-14
#define COMM_ERROR_WRITE			-15
#define COMM_ERROR_WRITETIMEOUT		-16
#define COMM_ERROR_READ				-17
#define COMM_ERROR_READTIMEOUT		-18
#define COMM_ERROR_WAIT				-19
#define COMM_ERROR_WAITTIMEOUT		-20
#define COMM_ERROR_GETOVERLAPPED	-21

#define COMM_ERR_OVERRUN        0x02
#define COMM_ERR_PARITY         0x04
#define COMM_ERR_FRAME          0x08
#define COMM_ERR_BREAK          0x10

/*
 * Buffer size 
 */
#define READ_BUFFER_SIZE		3000
#define WRITE_BUFFER_SIZE		200

/*****************************************************************
Structures
*****************************************************************/
/* NONE */


class Comm32Port
{
public:
/*****************************************************************
Routine Definitions
*****************************************************************/

	Comm32Port();
	~Comm32Port();

	void SerialClose();
	int SerialOpen( unsigned uPort, unsigned long ulBaudRate,
				unsigned uFormat, bool bRtsCts, unsigned long ulSerBreakDelay);
	int SerialSetBaud( unsigned long ulBaudRate, unsigned uFormat, bool bRtsCts, 
					   unsigned long ulSerBreakDelay);

	int SerialInit();
	int SerialBreak();
	int SerialFlush();

	int SerialPutChar( unsigned char uch );
	int SerialPutString( unsigned char *pszString, unsigned long ulLen );

	int  SerialGetChar();
	int  SerialCharsAvailable();	
	int  SerialGetString(unsigned char *pszString, unsigned long ulMaxLen);

	int	SerialErrorStatus();

private:
/*****************************************************************
Variables
*****************************************************************/
	bool 
		m_bRtsCts;		/* hardware handshaking */
	unsigned long 
		m_ulSerBreakDelay;	/* delay after a serial break */

	unsigned char 
		m_gruchInputBuffer[READ_BUFFER_SIZE];	/* input buffer */
	int		
		m_nIndexBuffer;	/* index of buffer */
	int		
		m_nNumBytes;	/* number of bytes read or sent */

	HANDLE	
		m_hCommPort;	/* com port handle */

	COMMTIMEOUTS	
		CommTimeouts;	/* com port timeouts */
};
#endif
/************************END OF FILE*****************************/
