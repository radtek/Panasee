#if !defined(AFX_DLGIMRIIMPL_H__7D78CC16_75D1_4E1E_A5F5_0B9469C86347__INCLUDED_)
#define AFX_DLGIMRIIMPL_H__7D78CC16_75D1_4E1E_A5F5_0B9469C86347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImriImpl.h : header file
//
#include "ImriULProtocol.h"
#include "ImriLLCP.h"
#include "VIMRIulServerApp.h"
#include "Resource.h"
#include "ImriULSrv.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImriImpl dialog
 
class CDlgImriImpl : public CDialog
{
// Construction
public:
	CDlgImriImpl(CWnd* pParent = NULL);   // standard constructor
	~CDlgImriImpl();

	static CDlgImriImpl* GetHandle();			// Get Dlg handle
	WORD	GetIMRIACPHiVer();
	WORD	GetIMRIACPLoVer();

	int		StartServer(short sPort, HWND hMsgWnd);
	void	StopServer();
	BOOL	IsMRIConnected();
	void	SetAppImp(CVImriUlServerApp *pulInterfaceImp); 

	int		ConnectServer(short nPort, char* cIPAddr, HWND hMsgWnd);
	int		DisconnectServer();

	BOOL    CheckOperResult(int nOperResult);

	int		SendInfoRequest(ENUM_INFOR_TYPE eRequest);
	int		SendStatusRequest(ENUM_STATUS_TYPE eRequest);
	int		SendCommand(ENUM_COMMANDS eCommand, void *pBuffer = NULL, int nDataLen = 0);
	int		SendLocalization(CList<IMRI_LOCALIZATION*, IMRI_LOCALIZATION*>& lststcrLocalization);

	int		CheckIllegal(unsigned char* lpULPack, int nPackLen);
	int		SendULPack(ENUM_PACK_TYPE ePackType, unsigned char* pPackData, int nDataLen);

	int		DealNegotiatePkg(unsigned char* lpULPack, int &nPackLen);
	int		DealImagePkg(unsigned char* lpULPackData, int &nPackDataLen);
	int		DealMessagePkg(unsigned char* lpULPackData, int &nPackDataLen);
	int		DealStatusPkg(unsigned char* lpULPackData, int &nPackDataLen);
	int		DealInfoPkg(unsigned char* lpULPackData, int &nPackDataLen);
	int		DealCriticalMsgPkg(unsigned char* lpULPackData, int &nPackDataLen);
	
	CImriLLCP        m_llcpCommObj ;
	
	CVImriUlServerApp	*m_pvImriUlServerApp;

	BOOL	m_bSrvStarted;
	int		m_nClientCnt;		// Count of connect client
	BOOL	m_bSrvConnected;

// Dialog Data
	//{{AFX_DATA(CDlgImriImpl)
	enum { IDD = IDD_DLG_IMRI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImriImpl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImriImpl)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnImriNetMsg(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	

	WORD  m_wLoVer ;
	WORD  m_wHiVer ;
	WORD  m_wLoVerUsed ;
	WORD  m_wHiVerUsed ;


	HWND  m_wndMsgHandle;		// Window to receive WM_IMRIUL_MSG

	BOOL  m_bAsServer;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMRIIMPL_H__7D78CC16_75D1_4E1E_A5F5_0B9469C86347__INCLUDED_)
