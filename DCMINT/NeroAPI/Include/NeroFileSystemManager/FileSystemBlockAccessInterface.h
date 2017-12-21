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
|* PROGRAM: FileSystemBlockAccessInterface.h
|*
|* PURPOSE: The file system block access interface
|* The NeroAPI packet writing API will return INeroFileSystemBlockAccess that
|* can be used to have block access to a file system.
******************************************************************************/
#ifndef FILESYSTEMBLOCKACCESSINTERFACE_H
#define FILESYSTEMBLOCKACCESSINTERFACE_H

#include "FileSystemBlockWriterInterface.h"
#include "FileSystemBlockAccessExtensions.h"

/* This interface contains an extension scheme that will allow us to 
 * transparently extend the interface's functionality without losing binary 
 * compatibility.
 */
class INeroFileSystemBlockAccess
	:public INeroFileSystemBlockWriter
{
public:

	// Returns the number of supported extension fields
	virtual DWORD GetNumSupportedExtensions() = 0;
	// In combination with the method above, this method can be used to copy a set of extensions
	// without knowing which ones are actually there. Extensions have to be passed through to
	// any of the Nero filesystem generators, so it is essential to have some means of copying them
	virtual INeroFileSystemBlockAccessExtension *GetFileExtensionByNumber(int iNumExt) = 0;
	// Returns specified extension or NULL if not present
	virtual INeroFileSystemBlockAccessExtension *GetFileExtension(NeroFSBlockAccessExtensionsType eExtType) = 0;

};


#endif // FILESYSTEMBLOCKACCESSINTERFACE_H
