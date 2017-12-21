/*****************************************************************
Name:			INIFileRW.cpp

Description:    This file controls reading from and writing to the
				ini files for this program.  There is an ini file
				for the project settings as well as the error received
				from the Systems.


All Northern Digital Inc. ("NDI") Media and/or Sample Code and/or
Sample Code Documentation (collectively referred to as "Sample Code")
is licensed and provided "as is" without warranty of any kind.  The
licensee, by use of the Sample Code, warrants to NDI that the Sample
Code is fit for the use and purpose for which the licensee intends to
use the Sample Code. NDI makes no warranties, express or implied, that
the functions contained in the Sample Code will meet the licensee抯
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
or business interruption, however caused.  In no event shall NDI抯
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
#include "stdio.h"
#include "Afxwin.h"
#include "ConfigMgr.h"

#include "io.h"

/*****************************************************************
Project Files Included
*****************************************************************/

/*****************************************************************
Defines
*****************************************************************/

/*****************************************************************
Global Variables
*****************************************************************/
char
	pszStringValue[256];
char
	pszFileName[MAX_PATH];
char cFileName[MAX_PATH]; //文件名

void GetINIFileName();


void SetMGRINIFileName(char* pFileName)//设置INI文件名,路径为当前或Windows目录
{
	if(pFileName==NULL) return ;
	strcpy(cFileName,pFileName);
}


/*****************************************************************
Name:				GetINIFileName

Inputs:
	None.

Return Value:
	None.

Description:    This routine gets the name of the ini file and stores
				it in the pszFileName variable.
*****************************************************************/
void GetINIFileName()
{
	char
		cExecPath[MAX_PATH];
	char 
		*pszPathofFile;

	/*
	 * Get the path of the program executable since this is where the ini
	 * files are stored
	 */
	 
	if(GetModuleFileName( (HMODULE)AfxGetInstanceHandle(),  cExecPath, sizeof(cExecPath)))
	{	
		pszPathofFile = strrchr(cExecPath, '\\'); /* remove the current EXE name from the path */
		if(pszPathofFile)
		{		
			*(pszPathofFile + 1) = '\0';		
		} /* if */
	} /* if */

	sprintf( pszFileName, "%s%s", cExecPath, cFileName);
	if ( _access( pszFileName , 02 ) == -1 )
	{
		/*
		 * because we can't write to the INI file (i.e. if the executable is on a CD)
		 * we will use the Windows default, else we use the executable's path as our
		 * INI file directory.
		 */
		sprintf( pszFileName, cFileName );
	} /* if */
} /* GetINIFileName */
/*****************************************************************
Name:				ReadINIParm

Inputs:
	char * pszSection - the section of the ini file to read info from
	char * pszVariableName - the variable name to read into from
	char * pszDefaultValue - the default value for the variable
	int nSize - the size of the buffer to be written to
	int * nValue - the int value that is read

Return Value:
	None.

Description:   This routine read a int value from the ini file
				under the specified header and the specified variable
*****************************************************************/
void ReadINIParm( char * pszSection,
				  char * pszVariableName,
				  char * pszDefaultValue,
				  int * nValue )
{
	GetINIFileName();
	GetPrivateProfileString( pszSection, pszVariableName, pszDefaultValue, pszStringValue, 
							 sizeof(pszStringValue), pszFileName);
	*nValue = atoi( pszStringValue );

//	CConfigMgr conf;
//	int nDedaultValue = atoi(pszDefaultValue);
//	*nValue = conf.Read(pszSection, pszVariableName,nDedaultValue);

} /* ReadINIParm */
/*****************************************************************
Name:				ReadINIParm

Inputs:
	char * pszSection - the section of the ini file to read info from
	char * pszVariableName - the variable name to read into from
	char * pszDefaultValue - the default value for the variable
	int nSize - the size of the buffer to be written to
	bool * bValue - the boolean value that is read

Return Value:
	None.

Description:   This routine reads a boolean value from the ini file
				under the specified header and the specified variable
*****************************************************************/
void ReadINIParm( char * pszSection,
				  char * pszVariableName,
				  char * pszDefaultValue,
				  bool * bValue )
{
	int
		nValue;
	GetINIFileName();
	GetPrivateProfileString( pszSection, pszVariableName, pszDefaultValue, pszStringValue, 
							 sizeof(pszStringValue), pszFileName);
	nValue = atoi( pszStringValue );
	*bValue = ( nValue == 1 ? TRUE : FALSE );

//	CConfigMgr conf;
//	int nDedaultValue = atoi(pszDefaultValue);
//	*bValue = conf.Read(pszSection, pszVariableName, nDedaultValue);

} /* ReadINIParm */
/*****************************************************************
Name:				ReadINIParm

Inputs:
	char * pszSection - the section of the ini file to read info from
	char * pszVariableName - the variable name to read into from
	char * pszDefaultValue - the default value for the variable
	int nSize - the size of the buffer to be written to
	char * pszValue - the char * value that is read

Return Value:
	None.

Description:   This routine read a char * value from the ini file
				under the specified header and the specified variable
*****************************************************************/
void ReadINIParm( char * pszSection,
				  char * pszVariableName,
				  char * pszDefaultValue,
				  int nSize,
				  char * pszValue )
{
	GetINIFileName();
	GetPrivateProfileString( pszSection, pszVariableName, pszDefaultValue, pszValue, 
							 nSize, pszFileName);

//	CConfigMgr conf;
//	CString sVal = conf.Read(pszSection, pszVariableName, pszDefaultValue);
//	strcpy(pszValue, sVal.GetBuffer(0));

} /* ReadINIParm */
/*****************************************************************
Name:				WriteINIParm

Inputs:
	char * pszSection - the section of the ini file to write info to
	char * pszVariableName - the variable name to write into to
	char * pszValue - the value to be written

Return Value:
	None.

Description:   This routine writes an char * value to the ini file
				under the specified header and the specified variable
*****************************************************************/
void WriteINIParm( char * pszSection,
				   char * pszVariableName,
				   char * pszValue )
{
	GetINIFileName();
	WritePrivateProfileString(pszSection, pszVariableName, pszValue, pszFileName);

//	CConfigMgr conf;
//	conf.Write(pszSection, pszVariableName, pszValue);

} /* WriteINIParm */
/*****************************************************************
Name:				WriteINIParm

Inputs:
	char * pszSection - the section of the ini file to write info to
	char * pszVariableName - the variable name to write into to
	int nValue - the value to be written

Return Value:
	None.

Description:   This routine writes an integer value to the ini file
				under the specified header and the specified variable
*****************************************************************/
void WriteINIParm( char * pszSection,
				   char * pszVariableName,
				   int nValue )
{
	GetINIFileName();
	sprintf( pszStringValue, "%d", nValue );
	WritePrivateProfileString(pszSection, pszVariableName, pszStringValue, pszFileName);

//	CConfigMgr conf;
//	conf.Write(pszSection, pszVariableName, nValue);


} /* WriteINIParm */
/*****************************************************************

Name:					GetErrorResponse

Input Values:		
				char
					*pszReply - the error message reply to extract
								the error number from		
Output Values:

Return Value:
				char * pszErrorMsg - the error string found for the
									 corresponding error code.

Description:		
					This routine takes the error message which is in
					form (ERRORXX) pasrses out the XX and finds the
					corresponding error message.
*****************************************************************/
void GetErrorResponse( char * pszReply, char * pszErrorMsg )
{
	char
		chErrorToSearchFor[8],
		chReturnedMsg[256],
		cExecPath[MAX_PATH],
		*pszPathofFile;

	/* 
	 * get the programs executable's path 
	 * this is also the path that will contain the ini files
	 * for this project
	 */

	if(GetModuleFileName( (HMODULE)AfxGetInstanceHandle(),  cExecPath, sizeof(cExecPath)))
	{	
		pszPathofFile = strrchr(cExecPath, '\\'); /* remove the current EXE name from the path */
		if(pszPathofFile)
		{		
			*(pszPathofFile + 1) = '\0';		
		} /* if */
	} /* if */

	/* search for the specified error message */
	sprintf( pszFileName, "%sNDITrackingErrors.ini", cExecPath );
	
	if (!strnicmp(pszReply, "ERROR",5))
	{
		pszReply += 5;
		sprintf( chErrorToSearchFor, "0x%c%c", pszReply[0], pszReply[1] );
		GetPrivateProfileString( "Error Messages", chErrorToSearchFor, "Unknown Error", 
								 chReturnedMsg, sizeof( chReturnedMsg ), pszFileName );
	} /* if */
	else if (!strnicmp(pszReply, "WARNING", 7 ))
	{
		pszReply += 7;
		sprintf( chErrorToSearchFor, "0x%c%c", pszReply[0], pszReply[1] );
		GetPrivateProfileString( "Warning Messages", chErrorToSearchFor,
								 "A non-fatal tool error has been encountered", 
								 chReturnedMsg, sizeof( chReturnedMsg ), pszFileName );
	} /* else if */
	else
		sprintf( chReturnedMsg, "Unknown Error response" );

	sprintf( pszErrorMsg, chReturnedMsg );
} /* GetErrorResponse */
/**************************END OF FILE***************************/
