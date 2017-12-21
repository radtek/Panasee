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
|* PROGRAM: FileSystemBlockWriterInterface.h
|*
|* PURPOSE: The FileSystem block writer interface is derived from the block 
|*          reader interface. It defines a path of access to RW filesystems 
|*          and partitions.
******************************************************************************/
#ifndef FILESYSTEMBLOCKWRITERINTERFACE_H
#define FILESYSTEMBLOCKWRITERINTERFACE_H

#include "FileSystemBlockReaderInterface.h"


#if defined(__BORLANDC__)
// NEROAPI expects structs to be 8byte aligned
#pragma pack(push, 8) 
// tell Borland C++ Builder to treat enums as int
#pragma option push -b 
#endif


// Specifies the block writer type.
// You can use this information to down-cast the interface to obtain specialized functionality...

// No extensions to the normal blockwrite interface are available so far...	
enum InterfaceType
{
	eBWIUnknown
};

class INeroFileSystemBlockWriter
	:public INeroFileSystemBlockReader
{
public:
	// As is the case with the reader interface, the writer interface also provides two methods for sector
	// access. While WriteSectorsUnBuffered will merely ensure the consistency of the read cache (write thru),
	// WriteSectorsBuffered will not write anything to the block device immediately but will cache 
	// a certain amount of sectors before doing so.
	// The latter increases performance considerably but is prone to data loss in an unstable environment.
	// Please note that regardless of which method you use, you *must* call FlushSectorCache() if you want
	// all your data to be at their final physical location because even when writing in UnBuffered mode,
	// the driver may decide to not write away your data immediately, depending on the underlying 
	// writing scheme (e.g. packet writing will always try to collect a certain amount of sectors)
	virtual NeroFSError WriteSectorsBuffered  (const void *pData, NeroFSSecNo startSector, NeroFSSecNo noSectors, NeroFSSecNo &noSectorsWritten) = 0;
	virtual NeroFSError WriteSectorsUnBuffered(const void *pData, NeroFSSecNo startSector, NeroFSSecNo noSectors, NeroFSSecNo &noSectorsWritten) = 0;
	// FlushSectorCache will be performed implicitly upon deleting the block writer object
	virtual void FlushSectorCache() = 0;

	// Runtime type information to be used for downcasting into specialized interfaces...
	virtual InterfaceType GetBlockWriterType() = 0;
};

#if defined(__BORLANDC__)
#pragma pack(pop)
#pragma option pop 
#endif


#endif // FILESYSTEMBLOCKWRITERINTERFACE_H
