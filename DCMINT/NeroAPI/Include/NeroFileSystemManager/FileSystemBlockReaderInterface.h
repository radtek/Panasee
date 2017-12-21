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
|* PROGRAM: FileSystemBlockReaderInterface.h
|*
|* PURPOSE: This file contains an abstract interface for reading from block 
|*          devices. It will provide necessary data about the underlying 
|*          medium as well as cache data if necessary.
******************************************************************************/
#ifndef FILESYSTEMBLOCKREADERINTERFACE_H
#define FILESYSTEMBLOCKREADERINTERFACE_H


#include <windows.h>


#if defined(__BORLANDC__)
// NEROAPI expects structs to be 8byte aligned
#pragma pack(push, 8) 
// tell Borland C++ Builder to treat enums as int
#pragma option push -b 
#endif


typedef __int64 NeroFSSecNo; // All sector references use this type. LBA adressing is used throughout the interface

typedef enum
{
	errOK=0,
	errEndOfDir,	// Deprecated. Should never be returned, to be treated as errOK
	errEndOfFile,	// Behaviour see libc read command
	errReadError,
	errInvalidFS,
	errNoDirectory,
	errNoFile,
	errNotSupported,
	errIllegalArgument,
	errWriteError,
	errInternalError,
	errFileLocked
} NeroFSError;


typedef enum
{
	vtData=0,		// track/partition contains data sectors
	vtAudio=1,		// track/partition contains audio sectors
	vtReserved=2	// track/partition hasn't been written yet, 
					// returned for reserved fragments of DVD+R media (NeroAPI 6.0.0.11+)
} NeroFSTrackType;

typedef struct 
{
	int				PartitionNum;    // The current partition number
	NeroFSSecNo     PartitionStart;  // The start sector for this Partition
	NeroFSSecNo		PartitionSize;   // The number of sectors this Partition contains
	NeroFSTrackType	PartitionType;	 // The type of Partition
	DWORD			sectorSize;      // Sector size for this Partition
} NeroFSPartitionInfo;

class INeroFileSystemBlockReader
{
public:
	virtual int					       GetNumPartitions() = 0;
	virtual const NeroFSPartitionInfo &GetPartitionInfo(int iNumPartition) = 0; 

	// Returns the partition a given sector resides in
	virtual const NeroFSPartitionInfo &GetPartitionForSector(NeroFSSecNo secNo) = 0;

	// Reading methods. The Buffered varient will use a cache to optimize filesystem access.
	// It should be used when reading directory structures while the UnBuffered method should be
	// used when reading file contents
	// Both methods return error codes as described in NeroFSError
	// Your read requests may not cross partition boundaries!  
	virtual NeroFSError ReadSectorsBuffered  (void *pData, NeroFSSecNo startSector, NeroFSSecNo noSectors, NeroFSSecNo &noSectorsRead) = 0;
	virtual NeroFSError ReadSectorsUnBuffered(void *pData, NeroFSSecNo startSector, NeroFSSecNo noSectors, NeroFSSecNo &noSectorsRead) = 0;

	virtual ~INeroFileSystemBlockReader() {};
};


#if defined(__BORLANDC__)
#pragma pack(pop)
#pragma option pop 
#endif


#endif // FILESYSTEMBLOCKREADERINTERFACE_H
