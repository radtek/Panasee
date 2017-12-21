/*****************************************************************
Name:               SystemCRC.cpp

Description:	This class contains all the CRC calls needed
				through out the program, some of the following
				routines were taken from the API


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
#include <stdio.h>

/*****************************************************************
Project Files Included
*****************************************************************/
#include "CommandHandling.h"
#include "Conversions.h"

/*****************************************************************
Defines
*****************************************************************/
/* None */

/*****************************************************************
Global Variables
*****************************************************************/
static unsigned int CrcTable[256];
static int bFirst=1;

/*****************************************************************
Name:			InitCrcTable

Inputs:
		None.

Return Value:
		None.

Description:   Sets up CRC table for use with CalcCRC16
			   Sets up static global var CrcTable
*****************************************************************/
void CCommandHandling::InitCrcTable()
{
	int i,j;
	long lCrcTable;
	/*
	 * Create the CRC lookup table
	 */
	for( i=0; i<256; i++ )
	{
		lCrcTable = i;
		for( j=0; j<8; j++ )
			lCrcTable = ( lCrcTable >> 1 ) ^ (( lCrcTable & 1 ) ? 0xA001L : 0 );

		CrcTable[i] = (unsigned int) lCrcTable & 0xFFFF;
	} /* for */
} /* InitCrcTable */
/*****************************************************************
Name:		    CalcCRC16

Inputs:
	int
		data			: Data value to add to running CRC16
	unsigned int
		*puCRC16		: Ptr. to running CRC16

Return Value:
	None.

Description:   
	This routine calcualted a running CRC16 using the polynomial
	X^16 + X^15 + X^2 + 1.

    NOTE:  This routine was taken from the back of the System API.
*****************************************************************/
unsigned int CCommandHandling::CalcCrc16( unsigned int crc, int data )
{
	if(bFirst) /* if this is the first time perform this */
	{
		bFirst = 0;
		InitCrcTable();
	} /* if */
	crc = CrcTable[ (crc ^ data) & 0xFF] ^ (crc >> 8);
	return (crc & 0xFFFF);
} /* CalcCRC16 */

/*****************************************************************
Name:                   CalcCRCByLen

Input Values:
    char *pszString : String for which CRC should be computed.
    int  nLen       : Length of string (since string is not ASCII)

Output Values:
    None.

Returned Value:
    unsigned        : CRC for computed for input string.

Description:
    This routine calculates a running CRC16 using the polynomial
    X^16 + X^15 + X^2 + 1.

*****************************************************************/
unsigned CCommandHandling::CalcCRCByLen( char *pszString, int nLen )
{
static unsigned char
        oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1,
                          1, 0, 0, 1, 0, 1, 1, 0 };
    unsigned
        data,
        uCrc = 0;
    unsigned char
        *puch = (unsigned char *)pszString;
    int
       	nCnt = 0;

    while ( nCnt < nLen )
    {
        data = (*puch ^ (uCrc & 0xff)) & 0xff;
        uCrc >>= 8;

        if ( oddparity[data & 0x0f] ^ oddparity[data >> 4] )
        {
            uCrc ^= 0xc001;
        } /* if */

        data <<= 6;
        uCrc ^= data;
        data <<= 1;
        uCrc ^= data;
        puch++;
        nCnt++;
    } /* while */

    return uCrc;

} /* CalcCRCByLen */

/*****************************************************************
Name:				SystemCheckCRC

Inputs:
			char * pointer to string to check

Return Value:
			int - 0 if CRC failure
				  1 if CRC successful

Description:   
	 This command will check the System reply for the correct
	 CRC value.
*****************************************************************/
int CCommandHandling::SystemCheckCRC(char *psz)
{

	unsigned int
		uCrc = 0,
		uReplyCrc = 0,
		uReplySize = 0;

	int
		m, n;
	/*
	 * calculate CRC
	 */
	uCrc = 0;

	/*
	 * We need to check if the reply is for BX, in binary format.
	 * The start byte shall be 0xA5C4
	 */
	if ( ((psz[0] & 0xff) == 0xc4) &&
		 ((psz[1] & 0xff) == 0xa5) ) 
	{
		uReplyCrc = (psz[4] & 0xff) | ((psz[5] & 0xff) << 8); //get the header CRC

		if (CalcCRCByLen(psz, 4) == uReplyCrc) //Check the header CRC
		{
			/*
			 *  Get the reply size. 
			 *  = reply size at [2] and [3] + 6 header bytes + 2 CRC bytes.
			 */
			uReplySize = ((psz[2] & 0xff) | ((psz[3] & 0xff) << 8)) + 8; 

			/* Get the body CRC */
			uReplyCrc = (psz[uReplySize-2] & 0xff) | ((psz[uReplySize-1] & 0xff) << 8); 

			if (CalcCRCByLen(&psz[6], (uReplySize-8)) == uReplyCrc) // Check the CRC
			{
				return 1; /* CRC check OK */
			}
			else
			{
				return 0; /* Bad CRC */
			}/* else */
		}
		else
		{
			return 0; /* Bad CRC */
		}/* else */
	}
	else
	{
		for( n = 0; (psz[n]!= CARRIAGE_RETURN) && (n< MAX_REPLY_MSG); n++)
		{
			; /* get total number of bytes n */
		}/* for */

		/*
		 * if rolled over the buffer size then something is wrong and
		 * we will say the CRC is bad
		 */
		if(n>=MAX_REPLY_MSG)
			return 0;

		/*
		 * NULL terminate the string to be tidy
		 */
		psz[n+1] = 0;

		/*
		 * determine 16 bit CRC
		 */
		for(m=0;m<(n-4);m++)
				uCrc = CalcCrc16(uCrc,psz[m]);

		/*
		 * read CRC from message
		 */
		sscanf(&(psz[n-4]),"%04x",&uReplyCrc);

		/*
		 * return the comparison of the calculated and received CRC values
		 */
		return (uCrc == uReplyCrc);

	}/* else */
} /* SystemCheckCrc */


/*****************************************************************
Name:				SystemGetCRC

Inputs:
			char * pointer to string to check

Return Value:
			int - the CRC calculated for the string
Description:   
	 This command will calc the crc for the inputed reply string
*****************************************************************/
unsigned int CCommandHandling::SystemGetCRC(char *psz, int nLength)
{
	unsigned int
		uCrc = 0;
	int
		m = 0;

	for(m=0; m<(nLength);m++)
	{
		uCrc = CalcCrc16(uCrc, psz[m]);
	}

	return uCrc;
}
/**************************END OF FILE***************************/
