/******************************************************************************
|* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
|* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
|* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
|* PARTICULAR PURPOSE.
|* 
|* Copyright 1995-2004 Ahead Software AG. All Rights Reserved.
|*-----------------------------------------------------------------------------
|* NeroSDK / NeroAPI
|*
|* PROGRAM: NeroPacketWriting.h
|*
|* PURPOSE: This file defines a lowlevel packetwriting/MRW API for NeroAPI
******************************************************************************/
#ifndef NEROPACKETWRITING_H
#define NEROPACKETWRITING_H

#include "NeroAPI.h"

#if defined(__BORLANDC__)
// NEROAPI expects structs to be 8byte aligned
#pragma pack(push, 8) 
// tell Borland C++ Builder to treat enums as int
#pragma option push -b 
#endif

#ifdef __cplusplus
class INeroFileSystemBlockAccess;
class INeroFileSystemBlockReader;
extern "C" {
#else
typedef void INeroFileSystemBlockAccess;
typedef void INeroFileSystemBlockReader;
#endif


typedef enum
{
	eNoWriting			=0x0000,		/* Use this to instantiate an INeroFileSystemBlockAccess object for read-only access */
	ePacketWriting		=0x0001,		/* Use this for DVD+RW,DVD-RW,CD-RW media in non-MRW mode */
	eManagedMRW			=0x0002,		/* Use this for defective managed MRW mode for all media types */
	eRawMRW				=0x0004,		/* Use this for raw MRW mode (defective management turned off) */
	eIllegalAccessMode	=0xffffffff
} AccessMode;

/*
// Use the following two functions to obtain a block reader/writer to a specified NeroAPI device.
// Please note that ownership of the aDeviceHandle is *not* transferred to the block writer/reader so you're
// still responsible to dispose of the device handle after disposing of the writer/reader interface.
//
// Please note that only one object created by either NeroCreateBlockWriterInterface or 
// NeroCreateBlockReaderInterface may exist at a time. Also make sure to delete the object before using
// the referred drive for another purpose (e.g. importing multisession data, starting a recording- or 
// digital audio extraction process).
// Opening a secondary device handle for the drive is *not* sufficient!
*/
NEROAPI_API INeroFileSystemBlockAccess* NADLL_ATTR NeroCreateBlockWriterInterface(NERO_DEVICEHANDLE aDeviceHandle,AccessMode eAccessMode);
NEROAPI_API INeroFileSystemBlockReader* NADLL_ATTR NeroCreateBlockReaderInterface(NERO_DEVICEHANDLE aDeviceHandle,AccessMode eAccessMode);

typedef enum
{
	eIAReadOnly			= 0x0000,
	eIAReadWrite			= 0x0001,
	eIAIllegalAccessMode	= 0xffffffff
} ImageAccessMode;

/*
// Create a block access interface for the specified image file
// Instead of an image file, you may pass a drive letter
// here to read from a specific device supported by the operating system
*/
NEROAPI_API INeroFileSystemBlockAccess* NADLL_ATTR NeroCreateBlockAccessFromImage(const char *szFilename, ImageAccessMode eAccessMode);

/*
// This function will return a DWORD mask containing values as declared in enum AccessMode.
// Use ((result&eDesiredMode)!=0) to determine whether a specific mode is supported.
*/
NEROAPI_API DWORD					   NADLL_ATTR NeroGetSupportedAccessModesForDevice(NERO_DEVICEHANDLE aDeviceHandle);

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma pack(pop)
#pragma option pop 
#endif


#endif // NEROPACKETWRITING_H
