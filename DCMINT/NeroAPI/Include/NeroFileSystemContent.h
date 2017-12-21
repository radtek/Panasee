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
|* PROGRAM: NeroFileSystemContent.h
|*
|* PURPOSE: 
|* This is the third NeroAPI interface for preparing data CDs/DVDs. Unlike NeroIsoTrack.h,
|* it is not much "callback based",thus most of the process will be driven by the 
|* application, making it easier to write. This interface is closely connected to the
|* internal engine of NeroAPI, this improves the cooperation of NeroAPI and the application.
|*
|* This set of classes describe the content of the file system of a disc.
|* The application will build a file structure using the IFileSystemContent object. 
|* During the burn process, NeroAPI will request the content of files using the 
|* IFileContent interface.
|*
|* Use the NeroCreateFileSystemContainer function of NeroAPI.h to get an instance of an
|* IFileSystemDescContainer object. Then, use the NERO_WRITE_FILE_SYSTEM_CONTAINER structure 
|* to burn the file structure created.
******************************************************************************/
#ifndef __NEROFILESYSTEMCONTENT_H
#define __NEROFILESYSTEMCONTENT_H

#if defined(__BORLANDC__)
// NEROAPI expects structs to be 8byte aligned
#pragma pack(push, 8) 
// tell Borland C++ Builder to treat enums as int
#pragma option push -b 
#endif


// To make sure we do not interfere with other classes
namespace FileSystemContent
{

class InterfaceBase
{
public:
	// Get another interface for the same object. This will be used to extend the DLL interface without
	// loosing binary compatibility. Returns NULL if no interface with this ID was found
	// This is inspired from the COM QueryInterface function
	virtual void *GetOtherInterface(int interfaceId)			const	// Using an ID number
	{
		return 0;	// Not other interface available by default
	}
	virtual void *GetOtherInterface(const char *interfaceName)	const	// Using a string
	{
		return 0;	// Not other interface available by default
	}

protected:
	virtual ~InterfaceBase()	{}
};

////////////////////////////////////////////////////////////////////////////////////////
// This first set of interfaces will be used by the burn engine to read the content of
// the file system
////////////////////////////////////////////////////////////////////////////////////////

class IFileContent : public InterfaceBase
{
public:
	virtual unsigned Read(void *pBuffer,unsigned length)	=0;
	virtual bool EndOfFile()								=0;
	virtual bool Error()									=0;

	// Called by the application when the object is not needed anymore
	virtual void Release()									= 0;
};

class IDirectory;

class IDirectoryEntry : public InterfaceBase
{
public:
	enum ENTRY_ERROR
	{
		ENTRY_NO_ERROR,
		SEQUENCING_ERROR,		// The content for this file may not be requested at this moment	
		ERROR_NOT_A_FILE,		// This entry is not a file 
		NOT_AVAILABLE,			// The content of this file cannot be requested at all
		INTERFACE_ERROR,		// The overriden function has tried to get an other interface for one object and has failed
		FEATURE_NOT_AVAILABLE	// This feature is not available for this file system type
	};

	enum {
		MODE2_FORM2					=1<<0,
		FIXED_INSIDE_VOLUME_SPACE	=1<<1,
		FIXED_OUTSIDE_VOLUME_SPACE	=1<<2,
		NO_OWN_CONTENT				=1<<3
	};
		
	virtual const char *GetName()					const	=0;	// File or directory name

	virtual const IDirectory *GetSubDirectory()		const	=0;
	virtual ENTRY_ERROR GetContent(IFileContent **)	const	=0;
	virtual const char *GetSourceFilePath()			const	=0;	// Return the source file path, NULL if the file is generated
	virtual __int64	GetSize()						const	=0;
	virtual int GetFilePriority()					const   =0;
	virtual int GetDirOrder()						const   =0;
	virtual int	GetId()								const	=0;	// Id number that can be used to find the file again later
	virtual unsigned GetDataStartSec()				const	=0;
};

class IDirectory : public InterfaceBase
{
public:
	virtual int GetNumEntries()								const =0;
	virtual const IDirectoryEntry *GetDirectoryEntry(int i)	const =0;
};


class IFileSystemContent : public InterfaceBase
{
public:
	virtual const char *GetName()							const =0;	// Volume name
	virtual const IDirectory *GetRoot()						const =0;
};


////////////////////////////////////////////////////////////////////////////////////////
// This second set of interfaces will be used by the application to produce the content of
// the file system
////////////////////////////////////////////////////////////////////////////////////////


// Allows the file producer to return the data
class IDataInputStream : public InterfaceBase
{
public:
	virtual void Write(const void *buffer,int size)								 = 0;
};

// Produce the content of a file. This interface must be derived and its implementation must
// create the content of the file in the ProduceFile function
class IFileProducer : public InterfaceBase
{
public:
	// Calling this method will automatically update the file size to the amount of data
	// delivered by the producer
	virtual IDirectoryEntry::ENTRY_ERROR ProduceFile(IDataInputStream *str) const = 0;

	// Called by NeroAPI when the object is not needed anymore
	virtual void Release()													const = 0;
};


class IDirectoryContainer;

// Description of a file
class IDirectoryEntryContainer : public IDirectoryEntry
{
public:
	// This object can be accessed in several ways
	enum
	{
		IID_IDirectoryEntryContainer,
		IID_IFileProducer,				// If the file entry was created using an IFileProducer 
										// object, this one can be retrieved using GetOtherInterface
		IID_IDirectoryEntryContainer2,	// Reserved
		IID_IDirectoryEntry2			// Reserved
	};

	// Using this function, the file size can be changed after having added the entry to the directory
	virtual void SetSize(__int64 size)						=0;

	// The two functions below can be used to readjust the directory priority
	// Priority numbers will be used in upward order: the file with smaller values first
	virtual void SetPriority(int priority)					=0;
	virtual void SetDirOrder(int directoryPriority)			=0;

	// Set the sector number that will be saved into the directory structure
	virtual void SetDataStartSec(unsigned)					=0;	

	// Set the physical position of the file in the filesystem
	virtual void SetFixedDataStartSec(unsigned)				=0;

	virtual void SetFileNumber(int)							=0;
	virtual void SetId(int)									=0;
	virtual void SetFlags(bool enable,unsigned f)			=0;	// Enable/disable the given flag
};

// Extension of the IDirectoryEntryContainer interface
class IDirectoryEntryContainer2 : public IDirectoryEntryContainer
{
public:
	// If the file entry was created using an IFileProducer, returns a pointer on it
	virtual const IFileProducer *GetFileProducer() const			=0;

	// Update the size attribute of this file by producing its content without writing
	// it anywhere
	virtual ENTRY_ERROR MeasureSize()								=0;

	// Set the size that is stored in the media directory record but do not change the
	// size of allocated and requested data 
	// This is currently only available for ISO filesystems
	virtual ENTRY_ERROR SetDirRecordSize(__int64 size)						=0;
};


// Represents the content of a directory
class IDirectoryContainer : public IDirectory
{
public:
	// Add a directory a returns a pointer on it
	// directoryPriority specifies the position in the directory. See this->AddFile
	// for details
	virtual IDirectoryContainer *AddDirectory(const char *name, int directoryPriority) =0;

	// Add a file the directory. The fp object will be automatically deleted when the directory
	// container will be deleted
	// 
	// the filesize passed here does *not* need to be correct, it will be used by the
	// filesystem generator to preallocate space so it must be the *maximum* space the final
	// version of the file may need (worst-case).
	//
	// Priority specifies some user-defined ordinal defining the order in which the files are
	// being written to the disc physically (like .ifo comes before .vob).
	// Priorities are valid across directories
	// The fileentry order in a directory is defined by the directoryPriority parameter which is the primary
	// sort criterium when arranging the files in a directory (Note that this is only true for
	// filesystems that do not require files to be sorted in the directory, e.g. UDF)
	// If any of the priority specifiers is -1, the producer doesn't care about the priority
	// and Nero will put the file where it thinks it fit
	// AddFile will return NULL if a file with the same name already exists
	virtual IDirectoryEntryContainer *AddFile(const char *name,
											  const IFileProducer *fp,__int64 size,
											  int priority, int directoryPriority) = 0;										 

	// Add a file which exists in the real file system
	virtual IDirectoryEntryContainer *AddFile(const char *name,
											  const char *sourcePath,
											  int priority, int directoryPriority) = 0;										 
	
	// Remove an entry from the directory
	virtual bool RemoveEntry(const char *name)								=0;

	virtual IDirectoryEntryContainer *Entry(const char *name)				=0;
	virtual IDirectoryEntryContainer *Entry(int i)							=0;
	virtual IDirectoryContainer *SubDirectory(const char *name)				=0;
};


// Supplemental method to the IDirectoryContainer interface
class IDirectoryContainerSearch {
	public:
		enum {
			SEARCH_DEPTH_INCL,   // Searches whole tree including given start object
			SEARCH_CHILDREN_EXCL // Searches children of start object only
		};
		// Like 'SubDirectory' of 'IDirectoryContainer' but with different search modes.
		// 'reserved' is intended for future use and MUST be initialized with NULL for now.
		virtual IDirectoryContainer *SubDirectoryEx(const char *name, unsigned mode, void *reserved) = 0;
};


// Represents the content of a file system
struct IFileSystemDescContainer : public IFileSystemContent
{
	virtual void SetName(const char *)	=0;		// Set the volume name of the file system
	virtual IDirectoryContainer *Root()	=0;		// Access the root directory for changing it

	// Called by the application when the object is not needed anymore
	virtual void Release()													const = 0;
};

} // namespace FileSystemContent

#if defined(__BORLANDC__)
#pragma pack(pop)
#pragma option pop 
#endif

#endif//__NEROFILESYSTEMCONTENT_H
