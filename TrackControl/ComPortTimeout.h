/*****************************************************************
Name:               ComPortTimeout.h

Description: This is the header for the comm port timeout dialog. 


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
#if !defined(AFX_COMPORTTIMEOUT_H__299E83C3_C32B_48E2_85A1_D1B9A7130086__INCLUDED_)
#define AFX_COMPORTTIMEOUT_H__299E83C3_C32B_48E2_85A1_D1B9A7130086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComPortTimeout.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CComPortTimeout dialog

class CComPortTimeout : public CDialog
{
// Construction
public:
	CComPortTimeout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComPortTimeout)
	enum { IDD = IDD_COMMPORTTIMEOUT };
	CStatic	m_ctlDialogErrorIcon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComPortTimeout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComPortTimeout)
	virtual BOOL OnInitDialog();
	afx_msg void OnRetry();
	afx_msg void OnCloseapplication();
	afx_msg void OnRestart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int
		m_nReturnValue; /* the return value from the dialog, depending on the button pressed */

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPORTTIMEOUT_H__299E83C3_C32B_48E2_85A1_D1B9A7130086__INCLUDED_)
/************************END OF FILE*****************************/
