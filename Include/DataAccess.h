// DataManager.h: interface for the CDataAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__MRI_DATA_ACCESS_H__)
#define __MRI_DATA_ACCESS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ==========================================================================
#pragma warning(disable:4786)
#include <vector>

#include <fstream>
#include <exception>
#include <iostream>
#include <cassert>
#include <afxtempl.h>
#include "BI_ImageHeader.h"

// ==========================================================================
typedef std::vector<MR_STRUCT_IMAGE> vImagesContainer;
#define CONST_MAX_NUM_OF_IMAGE_FILES		500

// ==========================================================================
class __declspec(dllexport) CDataAccess  
{

protected:
	static CDataAccess o_Object;

public:
	CDataAccess();
	virtual ~CDataAccess();
	static CDataAccess& GetHandle();

private:
	// delete all files of one directory
	void DeleteAllFilesInDirectory( CString sDirectory );
	
		
public:
	// write data of one image to file
	void WriteImageData( MR_STRUCT_IMAGE oImage, CString sImageFile );
	// read data of one image from file
	bool ReadImageData( MR_STRUCT_IMAGE& oImage, CString sImageFile );

	void WriteImage(MR_STRUCT_IMAGE* csImg,CString sFolder);
	// ¡¾API¡¿
	// Save data of all images of a scan to files
	void WriteImagesDataOfScan( vImagesContainer* pImages, CString sFolder );
	void WriteImagesDataOfScan( CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> *caImgs, CString sFolder );
	
	// ¡¾API¡¿
	// Read data of all images of a scan from files
	void ReadImagesDataOfScan( vImagesContainer* pImages, CString sFolder );
	
};

#endif // !defined(__MRI_DATA_ACCESS_H__)
