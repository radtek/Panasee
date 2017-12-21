/********************************************************************
File Name:	LUT.h
Written by: Yang Guang

Copyright(C) XinaoMDT, 2001 -

Purpose:
	This file defines following classes:
		CLUT	Lookup table used to map intensity to grayscale.

History:
	2002/4/4	Created.
*********************************************************************/

#if !defined(AFX_LUT_H__31C0E941_6425_4A23_ADC0_EFE43D49A457__INCLUDED_)
#define AFX_LUT_H__31C0E941_6425_4A23_ADC0_EFE43D49A457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"
 
#ifdef _BUILD_MRISERIES_DLL
	#define MRISERIES_API		__declspec(dllexport)
#else
	#define MRISERIES_API		__declspec(dllimport)
#endif

class CProcessToolManager;

// #ifdef IMAGEDISPLAY_IMP
//   #define CLASS_DECL_IMAGEDISPLAY  __declspec(dllexport)
// #else
//   #define CLASS_DECL_IMAGEDISPLAY  __declspec(dllimport)
// #endif

//! This class wraps a look up table.
/*!
	\remarks For the time being, only 8 bit gray image is supported, so we
	simply use a vector of BYTE.
	\bug key range and value range are not allowed to change for the time being.
*/
class MRISERIES_API CLUT  
{
public:

	//! \name Constructors and destructor
	//@{
	CLUT();
	virtual ~CLUT();
	//@}

	//! \name Accessor Functions
	//@{
	bool SetWidth(int nWidth);
	bool SetLevel(int nLevel);
	int  GetWidth(){
		return m_nWidth;
	}
	int GetLevel(){
		return m_nLevel;
	}
	//@}

	//! Calculate the lookup table according to the current level/width.
	void Update();
	//! Overloaded operator to facilitate use of the class.
	BYTE operator[](int n) const{
		return m_Table[n];
	}

// Implementation
protected:
	short m_nMaxValue;
	short m_nMinValue;
	short m_nMaxKey;
	short m_nMinKey;
	short m_nLevel;
	short m_nWidth;
    CBuffer <BYTE> m_Table;
};

//! This class wraps a 32-bit look up table.
/*!
	\remarks This lookup table can be used to accelerate the process of 
    mapping an image of 'short' to an 32-bit 256-grayscale bitmap. Use this 
    table, each pixel in the image can be mapped to an int (RGB) directly, it
    is faster than assgined each bytes in the RGB. According to a test carried 
    out on P4 1.7G, mapping a 256x256 image to 32-bit bitmap using CLUT will
    take about 3ms while using CLUT32 only takes 0.7ms. Filling a LUT of 4096
    items takes about 0.3ms.
    \note This only works when the image mode is set to 32-bit depth.
    For the time being, only 256 gray scales are supported.
	\bug key range and value range are not allowed to change for the time being.
*/
class MRISERIES_API CLUT32  
{
public:
	BOOL IsGreyColor();
	void SetGreyColor(BOOL bEnable);

	//! \name Constructors and destructor
	//@{
	CLUT32();
	virtual ~CLUT32();
	//@}

	//! \name Accessor Functions
	//@{
	bool SetWidth(int nWidth);
	bool SetLevel(int nLevel);
	int  GetWidth(){
		return m_nWidth;
	}
	int GetLevel(){
		return m_nLevel;
	}
	//@}

	//! Calculate the lookup table according to the current level/width.
	void Update();
	//! Overloaded operator to facilitate use of the class.
	int operator[](int n) const{
		return m_Table[n];
	}

// Implementation
protected:
	int m_nMaxValue;
	int m_nMinValue;
	int m_nMaxKey;
	int m_nMinKey;
	int m_nLevel;
	int m_nWidth;
    CBuffer<int> m_Table;

	BOOL m_bGreyColor;
};

/*! 
    This class is implemented as a Singleton, which provides a convenient way
    of global access and ensures there can be only one instance of the manager
    in the application in the mean time. 
*/
class CLUTManager
{
protected:
	static CLUTManager s_LUTManager;
	CLUTManager();
public:
	static CLUTManager& GetHandle(){
		return s_LUTManager;
	}
	virtual ~CLUTManager();
    const CLUT32& GetLUT32(int nWidth, int nLevel);
    const CLUT& GetLUT(int nWidth, int nLevel);

    CLUT    m_Lut;
    CLUT32  m_Lut32;
};

#endif // !defined(AFX_LUT_H__31C0E941_6425_4A23_ADC0_EFE43D49A457__INCLUDED_)
