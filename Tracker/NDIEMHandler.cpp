// NDIHandler.cpp: implementation of the CNDIEMHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDIEMHandler.h"
#include "ConfigMgr.h"
#include "GlobalDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNDIEMHandler::CNDIEMHandler()
{
	CTrackerHandler::CTrackerHandler();

	m_pNDICommandHandler = NULL;
	m_iMinValidHandle=10;
	

	m_pNDICommandHandler = new CCommandHandling();
	m_pNDICommandHandler->SetINIFileName("Surgery.INI");

	m_bNDISystemConnected = FALSE;  
	m_bPortsActivated = FALSE;	

	m_minIndex=10;
		
}

CNDIEMHandler::~CNDIEMHandler()
{
	if (m_pNDICommandHandler)
		delete m_pNDICommandHandler; 
}

int CNDIEMHandler::GetDeviceType()
{
	return TRACK_DEVICE_NDI_EM;
}

BOOL CNDIEMHandler::InitNDI()
{
	if (!ConnectNDI())
	{
		
		return FALSE;
	} 
	Sleep(1000);
	if (!ActiveTool())
	{
		
		
		return FALSE;
	}
	return TRUE;
}

BOOL CNDIEMHandler::ConnectNDI()
{
	int
		nBaudRate = 0,
		nStopBits = 0,
		nParity   = 0,
		nDataBits = 0,
		nHardware = 0,
		nCOMPort = 0,
		nWireless = 0,
	    nReset = 0;

	CWaitCursor wait;


	//if (m_controlView->m_dlgCryotherapy.m_bTracking)
	if (m_bNDISystemConnected)
		return TRUE;

	/*
	 * read the COM port parameters from the ini file
	 */
	CConfigMgr conf;
	nBaudRate = conf.Read(TRACK_DEVICE,COM_BAUD_RATE, 0);
	nStopBits = conf.Read(TRACK_DEVICE, COM_STOP_BITS, 0);
	nCOMPort = conf.Read(TRACK_DEVICE, COM_PORT, 0);
	nParity = conf.Read(TRACK_DEVICE, COM_PARITY, 0);
	nDataBits = conf.Read(TRACK_DEVICE, COM_DATA_BITS, 0);
	nHardware = conf.Read(TRACK_DEVICE, COM_HARDWARE, 0);
	nReset = conf.Read(TRACK_DEVICE, COM_RESET, 1);
	nWireless = conf.Read(TRACK_DEVICE, COM_WIRELESS, 0);

	/*
	 * close, then open the port
	 */
	m_pNDICommandHandler->nCloseComPorts();
	m_bNDISystemConnected = FALSE;

	if (nCOMPort < 0 || !m_pNDICommandHandler->nOpenComPort( nCOMPort ))
	{
		m_sErrorInfo="COM Port could not be opened.  \nCheck your COM Port settings and\nmake sure you NDI system is turned on.";
		return FALSE;
	} /* if */

	/*
	 * if we are supposed to reset, call the reset now
	 */
	if ( nReset )
	{	
		bool bWrireless=nWireless==0?false:true;
		if (!m_pNDICommandHandler->nHardWareReset(bWrireless))
		{
			m_sErrorInfo="COM Port could not be reseted";
			m_pNDICommandHandler->nCloseComPorts();
			return FALSE;
		}
	}/* if */

	/*
	 * get the timeout values for the commands
	 * this will return an error with all other systems, other than Vicra
	 */
	m_pNDICommandHandler->CreateTimeoutTable();

	/*
	 * set the System COM Port parameters, then the computers COM Port parameters.
	 * if that is successful, initialize the system
	 */
	if(m_pNDICommandHandler->nSetSystemComParms( nBaudRate, nDataBits, nParity, nStopBits, nHardware ))
	{
		if(m_pNDICommandHandler->nSetCompCommParms( nBaudRate, nDataBits, nParity, nStopBits, nHardware ))
		{
			if(m_pNDICommandHandler->nInitializeSystem())
			{
				/*
				 * get the system information
				 */
				if (!m_pNDICommandHandler->nGetSystemInfo())
				{
					/*
					 * Check system type: Polaris, Polaris Accedo, and Aurora
					 */
					m_sErrorInfo="Could not determine type of NDI system. ";
					m_pNDICommandHandler->nCloseComPorts();
					return FALSE;
				} /* if */

				/* 
				 * Set firing rate if system type is Polaris or Polaris Accedo.
				 */
				if ( (m_pNDICommandHandler->m_dtSystemInformation.nTypeofSystem == POLARIS_SYSTEM) ||
					 (m_pNDICommandHandler->m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM) ||
					 (m_pNDICommandHandler->m_dtSystemInformation.nTypeofSystem == ACCEDO_SYSTEM) )
				{
					m_pNDICommandHandler->nSetFiringRate(); 
				}/* if */
				//m_szSystemMode = "System Initialized";
				//SetMode( MODE_INIT );
				m_bNDISystemConnected = TRUE;
				

				//平板磁场发生器不支持这条命令
				/*if(!m_pNDICommandHandler->nSelectVolume(2)) //SELECT DOME VOLUME BY NINGSUIJUN
				{
					m_sErrorInfo="NDI system could not select volume. \nCheck volume type supported by this device.";
					m_pNDICommandHandler->nCloseComPorts();
					return FALSE;
				}*/
				
				//MessageBox( "NDI system successfully intialized", "Initialization", MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_SETFOREGROUND );
				return TRUE;
			} /* if */
			else
			{
				
				m_sErrorInfo="NDI system could not be initialized. \nCheck your COM Port settings, make sure NDI\nsystem is turned on and the system components are compatible.";
				m_pNDICommandHandler->nCloseComPorts();
				return FALSE;
			}/* else */
		}/* if */
	}/* if */
	m_sErrorInfo="COM Port could not be reseted";
	m_pNDICommandHandler->nCloseComPorts(); 
	return FALSE;
}

BOOL CNDIEMHandler::RefreshMatrix()
{
	bool bGetTXIgnoreError=false;
	if ( !m_pNDICommandHandler->nGetTXTransforms( bGetTXIgnoreError ) )
	{
		return FALSE;
	}
	return TRUE;

}
//激活 Handle
BOOL CNDIEMHandler::ActiveTool()
{
	if (m_bPortsActivated)
		return TRUE;

	CWaitCursor
		wait;

	

	if (m_pNDICommandHandler->nActivateAllPorts())
	{
		m_bPortsActivated = TRUE;
			
		return TRUE;
		
	} /* if */
	else 
	{
		m_bPortsActivated = FALSE;

		m_sErrorInfo="Ports could not be activated.\nCheck your NDI SROM image file settings";
		return FALSE;
	}
}


BOOL CNDIEMHandler::StartTracking()
{
	int bResult=m_pNDICommandHandler->nStartTracking();
	if(bResult!=REPLY_OKAY)
	{
		m_bNDISystemConnected=FALSE;
		m_bPortsActivated=FALSE;
		return FALSE;
	}
	return TRUE;
}

BOOL CNDIEMHandler::StopTracking()
{
	int bResult=m_pNDICommandHandler->nStopTracking();
	if(bResult!=REPLY_OKAY)
	{
		m_bNDISystemConnected=FALSE;
		m_bPortsActivated=FALSE;
		return FALSE;
	}
	return TRUE;
}

BOOL CNDIEMHandler::IsValidDevice()
{
	return (m_bPortsActivated && m_bNDISystemConnected);
}

int CNDIEMHandler::GetToolCount()
{
	m_iToolCount=m_pNDICommandHandler->m_dtSystemInformation.nNoMagneticPorts;
	return m_iToolCount;
}

BOOL CNDIEMHandler::GetMatrix(int iToolIndex, Matrix &m)
{
	if(!IsValidDevice())
		return FALSE;
	if(iToolIndex<0 || iToolIndex>=m_iToolCount)
		return FALSE;

	QUAT		quatMarker;		// Marker origin position and orientation
	Matrix		mMarker;		// Marker origin position and orientation
		
	if ( m_pNDICommandHandler->m_dtSystemInformation.bPortOccupied )
	{
		if ( m_pNDICommandHandler->nStopTracking() &&
			ActiveTool() && 
			m_pNDICommandHandler->nStartTracking() )
		{
			return FALSE;
		}/* if */
		
		return FALSE;
	} /* if */
	
	if ( m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].HandleInfo.bInitialized > 0 &&
		m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].szToolType[1] != '8' )
	{
		/* only update the frame if the handle isn't disabled*/
		
		if ( m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.ulFlags == TRANSFORM_VALID )
		{
			if(m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.fError>1.0)
			{
				return FALSE;
			}
			quatMarker.x = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.translation.x;
			quatMarker.y = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.translation.y;
			quatMarker.z = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.translation.z;
			quatMarker.q1 = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.rotation.q0;
			quatMarker.q2 = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.rotation.qx;
			quatMarker.q3 = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.rotation.qy;
			quatMarker.q4 = m_pNDICommandHandler->m_dtHandleInformation[iToolIndex+m_minIndex].Xfrms.rotation.qz;
			
			m = quatMarker.toMatrix();
			return TRUE;
			
		}
	}
	
	return FALSE;
					
}

BOOL CNDIEMHandler::IsValidTool(int index)
{
	if(!IsValidDevice())
		return FALSE;
	if(index<0 || index>=m_iToolCount)
		return FALSE;

	if(m_pNDICommandHandler->m_dtHandleInformation[index+m_minIndex].HandleInfo.bInitialized > 0 &&
			m_pNDICommandHandler->m_dtHandleInformation[index+m_minIndex].szToolType[1] != '8')
	{
		return TRUE;
	}else
	{
		return FALSE;
	}
}