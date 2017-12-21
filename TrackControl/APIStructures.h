#ifndef _APISTRUCTURES_H
#define _APISTRUCTURES_H


/*****************************************************************
Name:               APIStructures.h

Description:	This header contains the defines and structure used
				by the program while communicating with the systems.


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
/* number of handles the system can support */
#define NO_HANDLES				0xFF

/* Reply definitions */
#define REPLY_ERROR				0x00
#define REPLY_OKAY				0x01
#define REPLY_RESET				0x02
#define REPLY_OTHER				0x04
#define REPLY_BADCRC			0x08
#define REPLY_WARNING			0x10
#define REPLY_INVALID			0x20

/* command variables */
#define CARRIAGE_RETURN			0xD

/*
 * Since the introduction of NDI enhanced Tool Interface Unit
 * and Tool Docking Station, upto 20 port handles can now be
 * allocated and maximum reply message length shall be increased.
 *
 * Command message length shall be made different from the reply
 * message and the command message is much shorter.
 */
#define MAX_REPLY_MSG			4096

#define MAX_COMMAND_MSG			1024
#define LINE_FEED    			0xA

/* transform return values */
#define TRANSFORM_VALID			0x0000
#define	TRANSFORM_MISSING		0x1000
#define TRANSFORM_UNOCCUPIED	0x2000
#define TRANSFORM_DISABLED		0x3000
#define TRANSFORM_ERROR			0x4000

/* timeout returns */
#define ERROR_TIMEOUT_CLOSE		0
#define ERROR_TIMEOUT_RESTART	1
#define ERROR_TIMEOUT_CONT		2

/* math numbers */
#define BAD_FLOAT    (float)-3.697314E28
#define MAX_NEGATIVE (float)-3.0E28

/* message handler */
#define WM_COM_PORT_TO			WM_USER+2

/*****************************************************************
Structures
*****************************************************************/
/* POSITION 3D Structure */
typedef struct Position3dStruct
{
    float   x;
    float   y;
    float   z;
} Position3d;

/* QUATERNION Structure */
typedef struct QuatRotationStruct
{
    float   q0;
    float   qx;
    float   qy;
    float   qz;
} QuatRotation;

typedef struct QuatTransformationStruct
{
    QuatRotation     rotation;
    Position3d       translation;
} QuatTransformation;

typedef float
    RotationMatrix[3][3]; 

typedef struct Rotation
{ 

    float
    fRoll, /* rotation about the object's z-axis (Euler angle) */
    fPitch, /* rotation about the object's y-axis (Euler angle) */
    fYaw; /* rotation about the object's x-axis (Euler angle) */
} Rotation; 

/* Transformation Information Structure */
typedef struct
{
	unsigned long
		ulFlags,
		ulFrameNumber;
	QuatRotation
		rotation;
	Position3d
		translation;
	float
		fError;
}TransformInformation;

/* Handle Status Structure */
typedef struct
{
	int
		bToolInPort,
		bGPIO1,
		bGPIO2,
		bGPIO3,
		bInitialized,
		bEnabled,
		bOutOfVolume,
		bPartiallyOutOfVolume,
		bDisturbanceDet,
		bSignalTooSmall,
		bSignalTooBig,
		bProcessingException,
		bHardwareFailure,
		bTIPCurrentSensing;
}HandleStatus;

/* Handle Information Structure */
typedef struct
{
	char
		szPhysicalPort[20],
		szToolType[9],
		szManufact[13],
		szSerialNo[9],
		szRev[4],
		szChannel[3],
		szPartNumber[21];
	TransformInformation
		Xfrms;
	HandleStatus
		HandleInfo;
}HandleInformation;

/*System Information Structure*/
typedef struct
{
	int
		nTypeofSystem;
	char
		szVersionInfo[1024];
	int
		bActivePortsAvail,
		bPassivePortsAvail,
		bMultiVolumeParms,
		bTIPSensing,
		bActiveWirelessAvail,
		bMagneticPortsAvail,
		bFieldGeneratorAvail;
	/* POLARIS ONLY FIELDS */
	int
		nNoActivePorts,
		nNoPassivePorts,
		nNoActTIPPorts,
		nNoActWirelessPorts;
	/* AURORA ONLY FIELDS */
	int
		nNoMagneticPorts,
		nNoFGCards,
		nNoFGs;
	/* TRACKING INFORMATION */
	int	bCommunicationSyncError;
	int bTooMuchInterference;
	int bSystemCRCError;
	int bRecoverableException;
	int bHardwareFailure;
	int bHardwareChange;
	int bPortOccupied;
	int bPortUnoccupied;
	int bDiagnosticsPending;
	int bTemperatureOutOfRange;
}SystemInformation;

/*System Information Structure*/
typedef struct
{
	int
		bFatalParamterFault,
		bSensorParameterFault,
		bMainVoltageFault,
		bSensorVoltageFault,
		bIlluminatorVoltageFault,
		bIlluminatorCurrentFault,
		bLeftSensorTempFault,
		bRightSensorTempFault,
		bMainBoardTempFault,
		bBatteryFaultAlarm,
		bBumpDetectedAlarm,
		bCableFaultAlarm,
		bFirmwareIncompatible,
		bNonFatalParamFault,
		bInternalFlashFull,
		bLaserBatteryFaultAlarm,
		bTempTooHigh,
		bTempTooLow;
}DiagNewAlertFlags;

#define ALERT_FATAL_PARAM_FAULT		0x00000001
#define ALERT_SENSOR_PARAM_FAULT	0x00000002
#define ALERT_MAIN_VOLTAGE_FAULT	0x00000004
#define ALERT_SENSOR_VOLTAGE_FAULT	0x00000008
#define ALERT_ILLUM_VOLTAGE_FAULT	0x00000010
#define ALERT_ILLUM_CURRENT_FAULT	0x00000020
#define ALERT_LEFT_SENSOR_TEMP		0x00000040
#define ALERT_RIGHT_SENSOR_TEMP		0x00000080
#define ALERT_MAIN_BOARD_TEMP		0x00000100
#define ALERT_BATTERY_FAULT			0x00100000
#define ALERT_BUMP_DETECTED			0x00200000
#define ALERT_CABLE_FAULT			0x00400000
#define ALERT_FIRMWARE_INCOMPAT		0x00800000
#define ALERT_NON_FATAL_PARAM_FAULT	0x01000000
#define ALERT_INTERNAL_FLASH_FULL   0x02000000
#define ALERT_LASER_BATTERY_FAULT   0x04000000
#define ALERT_TEMP_TOO_HIGH			0x20000000
#define ALERT_TEMP_TOO_LOW			0x40000000

/*****************************************************************
Variables
*****************************************************************/
/* NONE */

/*****************************************************************
Routine Definitions
*****************************************************************/
/* NONE */

#endif

/************************END OF FILE*****************************/
