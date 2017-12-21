/*****************************************************************
Name:               CommandConstruction.cpp

Description:	This cpp file has the code to add the carriage
				return to the end of the command and the CRC/:
				to the command if the user selects to add the 
				CRC to the command.
				

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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************
Project Files Included
*****************************************************************/
#include "CommandHandling.h"

/*****************************************************************
Defines
*****************************************************************/

/*****************************************************************
Global Variables
*****************************************************************/


/*****************************************************************

Name:				AddCRCToCommand

Input Values:		
				char
					*pszCommandString - the message to have a CRC added
					remeber, we substitute the space with a :
								
Output Values:
				char *

Return Value:
				int - 1 if succesful, 0 otherwise

Description:		
					This function adds a CRC to the end of a command
					and replaces the space with a :.  The CRC is calc'd
					using the CRC functionality found in the API			
*****************************************************************/
int CCommandHandling::nAddCRCToCommand( char * pszCommandString )
{
	int 
		m,n;
	unsigned int 
		uCrc;
	bool
		bFirstSpace = FALSE;

	if(strlen(pszCommandString) >= (MAX_COMMAND_MSG-6))
		return 0;

	n=strlen(pszCommandString);
	/*
	 * determine 16 bit CRC
	 */
	uCrc = 0;
	for(m=0;m<n;m++)
	{
		/*
		 * replace space character with : if sending CRC
		 * since parameter names can have spaces we need to 
		 * replace only the first space with the :
		 */
		if(pszCommandString[m]==' ' && !bFirstSpace )
		{
			pszCommandString[m]=':';
			bFirstSpace = TRUE;
		}
		uCrc = CalcCrc16(uCrc,pszCommandString[m]);
	} /* for */
	sprintf(&pszCommandString[n],"%04X",uCrc);
	n+=4;

	/*
	 *Add the carriage return to the end
	 */
	return 1;
} /* nAddCRCToCommand */
/*****************************************************************

Name:				AddCRToCommand

Input Values:		
				char
					*pszCommandString - the message to have a carriage
					return added to the end
					remeber, all command need a carriage return
								
Output Values:
	char *

Return Value:
				int - 1 if successful, 0 otherwise

Description:		
					This function adds a carriage return  to the end of 
					a command					
*****************************************************************/
int CCommandHandling::nAddCRToCommand( char * pszCommandString )
{
	int 
		n;

	if(strlen(pszCommandString) >= (MAX_COMMAND_MSG-1))
		return 0;

	n=strlen(pszCommandString);
	pszCommandString[n++] = CARRIAGE_RETURN; 
	pszCommandString[n++] = '\0';
	return 1;
} /* nAddCRToCommand */
/*****************************************************************

Name:				nBuildCommand

Input Values:		
	char *pszCommandString - the message to be built
	bool bAddCRC - whether or not to add the CRC to the command
								
Output Values:
	char *

Return Value:
	int - 1 if successful, 0 otherwise

Description:		
	This routine builds the message.  If bAddCRC is true, replace
	the space with a : and add the commands CRC to the end of it.
*****************************************************************/
int CCommandHandling::nBuildCommand( char *pszCommandString, bool bAddCRC )
{
	int
		nCnt = 0,
		nNumCR = 0;

	/*
	 * If the command is re-sent, it should already have a CR added.
	 */
	for ( nCnt = 0; nCnt < MAX_COMMAND_MSG; nCnt++)
	{
		if( pszCommandString[nCnt++] == CARRIAGE_RETURN )
		{
			pszCommandString[nCnt++] = '\0';
			return 1;
		}/* if */
	}/* if */

	if ( bAddCRC )
		if (!nAddCRCToCommand( pszCommandString ))
			return 0;

	if (!nAddCRToCommand( pszCommandString ))
		return 0;

	return 1;
} /* nBuilCommand */
/**************************END OF FILE***************************/
