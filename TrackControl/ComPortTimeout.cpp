/*****************************************************************
Name:			ComPortTimeout.cpp

Description:    This dialog appears when there has been a timeout
				in the communications between the software and the
				Systems.


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
//#include "CombinedAPISample.h"
#include "ComPortTimeout.h"
#include "APIStructures.h"

/*****************************************************************
Defines
*****************************************************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************************
Global Variables
*****************************************************************/
/* none */

/////////////////////////////////////////////////////////////////////////////
// CComPortTimeout dialog


CComPortTimeout::CComPortTimeout(CWnd* pParent /*=NULL*/)
	: CDialog(CComPortTimeout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComPortTimeout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComPortTimeout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComPortTimeout)
	DDX_Control(pDX, IDC_ICON_FRAME, m_ctlDialogErrorIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComPortTimeout, CDialog)
	//{{AFX_MSG_MAP(CComPortTimeout)
	ON_BN_CLICKED(IDC_RETRY, OnRetry)
	ON_BN_CLICKED(IDC_CLOSEAPPLICATION, OnCloseapplication)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComPortTimeout message handlers
/*****************************************************************
				OnInitDialog
			VISUAL C++ FUNCTION CALL
*****************************************************************/
BOOL CComPortTimeout::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nReturnValue = ERROR_TIMEOUT_RESTART;

	HICON hIcon = (HICON)IDI_ERROR;
	m_ctlDialogErrorIcon.SetIcon( hIcon );

	UpdateData( TRUE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
} /* OnInitDialog */
/*****************************************************************
Name:				OnRetry

Inputs:
	None.

Return Value:
	None.

Description:   This routine handles what happens when the user
			   chooses to retry the command.
*****************************************************************/
void CComPortTimeout::OnRetry() 
{
	/* return Continue value */
	m_nReturnValue = ERROR_TIMEOUT_CONT;	
	CDialog::OnOK();
} /* OnRetry */
/*****************************************************************
Name:				OnCloseApplication	

Inputs:
	None.

Return Value:
	None.

Description:	This routine handles what happens when the user
				chooses to close the application.
*****************************************************************/
void CComPortTimeout::OnCloseapplication() 
{
	/* get the main window handle */
	CWnd *mainwnd;
	m_nReturnValue = ERROR_TIMEOUT_CLOSE;
	mainwnd = GetParent();

	/* post message to main window to close */
	mainwnd->PostMessage( WM_COM_PORT_TO, m_nReturnValue, 0l );
	CDialog::OnOK();

} /* OnCloseapplication */
/*****************************************************************
Name:				OnRestart

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles what happens when the user
				pressed the Restart button.
*****************************************************************/
void CComPortTimeout::OnRestart() 
{
	/* get the main window handle */
	CWnd *mainwnd;
	m_nReturnValue = ERROR_TIMEOUT_RESTART;		
	mainwnd = GetParent();

	/* post message to restart to main window */
	mainwnd->PostMessage( WM_COM_PORT_TO, m_nReturnValue, 0l );
	CDialog::OnOK();	
} /* OnRestart */
/**************************END OF FILE***************************/
