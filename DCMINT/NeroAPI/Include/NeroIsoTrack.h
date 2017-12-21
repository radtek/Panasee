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
|* PROGRAM: NeroIsoTrack.h
|*
|* PURPOSE: Classes for writing ISO 9660/Joliet track
|*
|* NOTE: In contrast to NeroAPI.h, this code is not pure C, but rather C++
******************************************************************************/


#ifndef _NEROISOTRACK_
#define _NEROISOTRACK_

#include "NeroAPI.h"

#ifndef __cplusplus

#error The Nero ISO Track feature can only be used by C++ code.

#else  /* __cplusplus */


#include <windows.h> // the data types used below are those of windows

//
// The application has to specify the complete layout of the
// ISO track. The NeroAPI does not care at all where the
// data for the files comes from. This also means that the
// application has to provide access to the filename or the
// data itself when the API needs it.
//

//
// Data can be fed into the API directly (i.e. without intermediate
// files) with CNeroDataCallback:
//
class CNeroDataCallback
{
public:
	virtual ~CNeroDataCallback () {}

	virtual DWORD IOCallback(BYTE *pBuffer, DWORD dwLen) = 0;     // same semantic as NERO_IO_CALLBACK in "NeroAPI.h"
	virtual BOOL EOFCallback () = 0;                              //  "       "       NERO_IO.nioEOFCallback
	virtual BOOL ErrorCallback () = 0;                            //  "       "       NERO_IO.nioErrorCallback
};

//
// The API builds an internal representation of the complete
// ISO tree and uses a CNeroIsoHandle acquired from the
// application for each file to access the data later.
// Usually only one file at once will be left open.
//
class CNeroIsoHandle
{
public:
	virtual ~CNeroIsoHandle () {}
	virtual CNeroIsoHandle * Clone () = 0;                      // make a copy of yourself

	virtual int GetFileName (char *strBuffer, UINT nBufferSize) = 0;  // If the application wants the API to read files, it has to fill
	                                                                  // the buffer of size nBufferSize with a null-terminated string and return
	                                                                  // the length of the full name, even if the given buffer was to small.
	                                                                  // The API will try again with a larger buffer then.

	// return 0 in GetFileName() if you want to provide the data via a CNeroDataCallback:
	virtual CNeroDataCallback * Open () = 0;                    // return instance ready to read the data associated with this handle or NULL for error;
	                                                            // this instance will be deleted by NeroAPI;
	                                                            // usually only one file at once will be left open
};

//
// Iterators are used to walk through directories while the API builds
// its internal copy of the tree. Iterators point to an entry or to NULL,
// if the last entry was passed, and can only be incremented.
//
class CNeroIsoEntry;
class CNeroIsoIterator
{
public:
	virtual ~CNeroIsoIterator () {}

	virtual CNeroIsoEntry * GetCurrentEntry () = 0;             // get pointer to current entry or NULL if last one passed;
	                                                            // entry not deleted by API, so the iterator may point to itself
	                                                            // and implement the required interface (as in the NeroAPI demo),
	                                                            // or to some permanent entry
	virtual void Next () = 0;                                   // go to next entry
};

//
// An entry (directory or file) is described like this:
//
struct CImportInfo;
class CNeroIsoEntry
{
public:
	virtual ~CNeroIsoEntry () {}

	virtual CNeroIsoIterator * CreateDirectoryIterator() = 0;   // NULL if no directory, otherwise an iterator to step through all child entries;
	                                                            // iterator will be deleted by NeroAPI
	
	virtual const char *       GetName () = 0;                  // the name for this entry; will be copied by API
	virtual __int64            GetLength () = 0;                // the size of this entry in bytes, or -1 if a directory
	virtual CNeroIsoHandle *   CreateHandle () = 0;             // creates a handle stored by the API to open a file later, NULL for directory;
	                                                            // handle will be deleted by NeroAPI when deleting the internal ISO tree

	// The following entries are only needed when e.g. creating your
	// own Video CD ISO track and not implemented yet.
	#if 0
	virtual BOOL               IsMode2 () { return FALSE; }     // TRUE if the application delivers mode 2 data (2336 bytes/block);
	                                                            // NOTE: the size above are the number of bytes delivered by the application
	                                                            // NOTE to ahead implementor: in contrast, the ISO entry length always assumes a
	                                                            // lock size of 2048 and thus GetSize() has to be multiplied by 2048/2336 before using it as CIsoListEntry::size
	virtual int                GetBlockOffset () { return -1; } // file data is to be written in this block (relative to beginning of ISO track),
	                                                            // or in a block chosen by NeroAPI if -1
	#endif

	// Can be used to reference files from previous session
	virtual DWORD GetDataStartSec() { return 0;}
	virtual BOOL IsDataFixed() { return FALSE;}
	virtual BOOL GetEntryTime(struct tm *tm)
	{
		return FALSE;	// Unspecified time
	}
	// This method was formerly known as GetRockRidgeInfo.
	// The object returned is a bit different internally now.
	// Since it is a private structure of NeroAPI this change doesn't matter.
    virtual CImportInfo *GetImportInfo() const
    {
	   // no ImportInfo by default
	   return NULL;
	};
	virtual void GetPriorities(int &iPriority,int &iDirPriority) 
	{
		iPriority=0;
		iDirPriority=0;
	};
	// Up from NeroAPI 5.5.9.0
	virtual CNeroIsoIterator * CreateDirectoryIteratorWrapper() { return NULL;	}
	// See CreateHandle(). Creates rsc fork handle for HFS filesystems
	// Will be preferred to reading the resource fork from the file specified by GetName() if !=NULL
	virtual CNeroIsoHandle *   CreateResourceHandle () { return NULL; };

	virtual const WCHAR* GetUnicodeName() { return 0; } // the name for this entry in unicode format; will be copied by API

	// NeroAPI>=6.0.0.14: Not necessary to implement this. Only used for internal purposes.
	virtual void *GetInterface(const char *name) { return NULL; }
private:
	// Reserved for future use
        virtual int reserved1() { return 0;}
        virtual int reserved2() { return 0;}
        virtual int reserved3() { return 0;}
        virtual int reserved4() { return 0;}
        virtual int reserved5() { return 0;}
        virtual int reserved6() { return 0;}
        virtual int reserved7() { return 0;}
};


//
// An ISO track is a special directory entry:
//
struct CNeroIsoTrack : public CNeroIsoEntry
{
	friend class CNeroIsoTrackProxy5039;	// Internal compatibility stuff;
        friend class CNeroIsoTrackProxy55915;   // Internal compatibility stuff;

public:
	~CNeroIsoTrack () {}

	// essential functions:
	virtual const char *       GetName () = 0;		            // ISO volume name, copied by API
	virtual CNeroIsoIterator * CreateDirectoryIterator () = 0;  // iterator for root directory; will be deleted by API

	// these new functions have reasonable defaults:
	virtual BOOL               UseJoliet () { return TRUE; }    // TRUE if track shall contain Joliet names in addition to ISO
	virtual BOOL               UseMode2 () { return FALSE; }    // TRUE if track shall be written as mode 2/XA

	// The following 3 functions exist from NeroAPI version 5.5.0.0
    virtual BOOL               UseRockRidge () { return FALSE; } // RockRidge requires additional informations, so it is off by default
    virtual BOOL               BurnISO() { return TRUE; } // TRUE if ISO should be created
    virtual BOOL               BurnUDF() { return FALSE; } // TRUE if UDF should be created

protected:
	virtual __int64            GetLength () { return -1; }					// we are a directory, ...
	virtual CNeroIsoHandle *   CreateHandle () { return NULL; }				// ... so we cannot be read
	virtual CNeroIsoHandle *   CreateResourceHandle () { return NULL; }     // ... so we cannot be read

public:
	// From NeroAPI version 5.5.1.2
	// You can set your burn options simply be redefining this function instead of
	// UseJoliet, UseMode2, UseRockRidge, BurnISO and BurnUDF
    // See NeroAPI.h for the signification of the NCITEF flags
    virtual DWORD              BurnOptions() 
     {
		return (UseJoliet() ? NCITEF_USE_JOLIET : 0)
			  |(UseMode2() ? NCITEF_USE_MODE2 : 0)
			  |(UseRockRidge() ? NCITEF_USE_ROCKRIDGE : 0)
			  |(BurnISO() ? NCITEF_CREATE_ISO_FS : 0)
			  |(BurnUDF() ? NCITEF_CREATE_UDF_FS : 0);
     };

	// Up from NeroAPI 5.5.9.0
	virtual CNeroIsoIterator *CreateDirectoryIteratorWrapper()
	{
		return NULL;
	}

	virtual BOOL HasWrapper(void)
	{
		return FALSE;
	}
   
    // for internal use only. Do not reimplement this method!
	virtual const void *dummy() const { return NULL; };

	virtual void GetVolumeDescriptor(const char **systemIdentifier, const char **volumeSet
		, const char **publisher, const char **dataPreparer, const char **application
		, const char **copyright, const char **abstract, const char **bibliographic)
	{
		*systemIdentifier = 0;
		*volumeSet = 0;
		*publisher = 0;
		*dataPreparer = 0;
		*application = 0;
		*copyright = 0;
		*abstract = 0;
		*bibliographic = 0;
	}

   
        // Reserved for future use
        virtual int reserved1() { return 0;}
        virtual int reserved2() { return 0;}
        virtual int reserved3() { return 0;}
        virtual int reserved4() { return 0;}
        virtual int reserved5() { return 0;}
        virtual int reserved6() { return 0;}
        virtual int reserved7() { return 0;}
};


#endif /* __cplusplus */


#endif /* _NEROISOTRACK_ */
