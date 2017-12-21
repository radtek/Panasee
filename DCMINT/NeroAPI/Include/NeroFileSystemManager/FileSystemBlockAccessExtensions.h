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
|* PROGRAM: FileSystemBlockAccessExtensions.h
|*
|* PURPOSE: All block device access extensions are derived from this one.
|*
******************************************************************************/
#ifndef FILESYSTEMBLOCKACCESSEXTENSIONS_H
#define FILESYSTEMBLOCKACCESSEXTENSIONS_H

#if defined(__BORLANDC__)
// NEROAPI expects structs to be 8byte aligned
#pragma pack(push, 8) 
// tell Borland C++ Builder to treat enums as int
#pragma option push -b 
#endif

/* 
 * Type of an extension.
 * Currently no extension is provided within the NeroSDK
 */
enum NeroFSBlockAccessExtensionsType
{
	etMRWReadDefectiveManagementInfo,
	etDVDPRWFormatExtension,
	etSectorMappingControlExtension,
	etBlankAreaControlExtension,
	etIllegalBlockAccessExtension,
	etHDPartitionInfo,
	etHDUsedBlockAccessExtention,
	etSectorPatchControlExtension,
	etHDPartitionLocker,
	etMediumCDStamp,
	etFragmentReservation,
	etUDFImageEditExtension,
	etSessionControl,
	etNeroFSBAExtensionReserved1,
	etNeroFSBAExtensionReserved2,
	etMediumInfo
};

class INeroFileSystemBlockAccessExtension
{
public:
	virtual NeroFSBlockAccessExtensionsType GetExtensionType() const = 0;

	virtual ~INeroFileSystemBlockAccessExtension() {}; 
};


#if defined(__BORLANDC__)
#pragma pack(pop)
#pragma option pop 
#endif


#endif // FILESYSTEMBLOCKACCESSEXTENSIONS_H
