/********************************************************************
File Name:	LUT.cpp
Written by: Yang Guang

Copyright(C) XinaoMDT, 2001 -

Purpose:
	This file defines following classes:

History:
	2001/12/2	Created.
*********************************************************************/

#include "stdafx.h"
#include "LUT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//! Constructor.
/*!
	\bug Quite a lot of hardcoding, only usable for unsigned short key values.
*/
CLUT::CLUT()
{
	m_nMaxValue = 255;
	m_nMinValue = 0;
	m_nMinKey = 0;
	m_nMaxKey = 4095;

	m_nWidth = 4095;
	m_nLevel = 2048;
	
	m_Table.SetSize(m_nMaxKey - m_nMinKey + 1);
	Update();
}

CLUT::~CLUT()
{
}

//! Set the window width.
bool CLUT::SetWidth(int nWidth)
{
	if (nWidth > 0 && nWidth <= (m_nMaxKey - m_nMinKey))
	{
		m_nWidth = nWidth;
		return true;
	}
	else
		return false;
}

//! Set the window level.
bool CLUT::SetLevel(int nLevel)
{
	if (nLevel >= m_nMinKey && nLevel <= m_nMaxKey)
	{
		m_nLevel = nLevel;
		return true;
	}
	else 
		return false;
}

//! Update the LUT according to the current level and width settings.
void CLUT::Update()
{
	int nLow = m_nLevel - (m_nWidth - 1) / 2;
	int nHigh = m_nLevel + m_nWidth / 2;

	if (nLow >= 0)
	{
		memset(m_Table.GetBuffer(), 0, sizeof (BYTE) * (nLow + 1));
	}
	if (nHigh <= m_nMaxKey)
	{
		memset(m_Table.GetBuffer() + nHigh, 0xff, 
			sizeof (BYTE) * (m_nMaxKey - nHigh + 1));
	}
	// (key - nLow)/(nHigh - nLow) * (m_nMaxValue - m_nMinValue) + m_nMinValue
	// if simplifies to "key * a + b", then a, b can be calculated as:

	double a, b;
	a = (m_nMaxValue - m_nMinValue)/static_cast<double>(nHigh - nLow);
	b = (nHigh * static_cast<double>(m_nMinValue) 
		- nLow * static_cast<double>(m_nMaxValue))/
		  static_cast<double>(nHigh - nLow);

	int nStart = max (nLow, 0);
	int nEnd = min (nHigh, m_nMaxKey);
	
	for (int i = nStart; i < nEnd; i++)
	{
		m_Table[i] = static_cast<unsigned char>(a * i + b);
	}
}

//////////////////////////////////////////////////////////////////////////
//  CLUT32                                                              // 
//////////////////////////////////////////////////////////////////////////
//! Constructor.
/*!
	\bug Quite a lot of hardcoding, only usable for unsigned short key values.
*/
CLUT32::CLUT32()
{
	m_bGreyColor=FALSE;

	m_nMaxValue = 255;
	m_nMinValue = 0;
	m_nMinKey = 0;
	m_nMaxKey = 65535;

	m_nWidth = 4095;
	m_nLevel = 2048;
//	m_nWidth = 65535;
//	m_nLevel = 32768;
	
	m_Table.SetSize(m_nMaxKey - m_nMinKey + 1);
	Update();
}

CLUT32::~CLUT32()
{
}

//! Set the window width.
bool CLUT32::SetWidth(int nWidth)
{
	if (nWidth > 0 && nWidth <= (m_nMaxKey - m_nMinKey))
	{
		m_nWidth = nWidth;
		return true;
	}
	else
		return false;
}

//! Set the window level.
bool CLUT32::SetLevel(int nLevel)
{
	if (nLevel >= m_nMinKey && nLevel <= m_nMaxKey)
	{
		m_nLevel = nLevel;
		return true;
	}
	else 
		return false;
}

//! Update the LUT according to the current level and width settings.
void CLUT32::Update()
{
	int nLow = m_nLevel - (m_nWidth - 1) / 2;
	int nHigh = m_nLevel + m_nWidth / 2;

	if (nLow >= 0)
	{
		memset(m_Table.GetBuffer(), 0, sizeof (int) * (nLow + 1));
	}

    COLORREF clrWhite = m_bGreyColor ? RGB(255,0,0) :RGB(255, 255, 255);
    for (int i = nHigh; i <= m_nMaxKey; i++)
    {
        m_Table[i] = clrWhite;    // RGB value for white
    }

    // (key - nLow)/(nHigh - nLow) * (m_nMaxValue - m_nMinValue) + m_nMinValue
	// if simplifies to "key * a + b", then a, b can be calculated as:

	double a, b;
	a = (m_nMaxValue - m_nMinValue)/static_cast<double>(nHigh - nLow);
	b = (nHigh * static_cast<double>(m_nMinValue) 
		- nLow * static_cast<double>(m_nMaxValue))/
		  static_cast<double>(nHigh - nLow);

	int nStart = max (nLow, 0);
	int nEnd = min (nHigh, m_nMaxKey);
	int iTmp;
	
    unsigned char* puc = 
        reinterpret_cast<unsigned char*>(m_Table.GetBuffer() + nStart);
    
	for (i = nStart; i <= nEnd; i++)
	{
		if(m_bGreyColor)
		{
			iTmp=a*i+b;
			//black->grey->pink->yellow->blue
			if(iTmp>=0 && iTmp<=200)
			{
				*(puc+2)=static_cast<unsigned char>(iTmp);
				*(puc+1)=static_cast<unsigned char>(iTmp);
				*puc=static_cast<unsigned char>(iTmp);
			}else if(iTmp>=201 && iTmp<=220)
			{
				*(puc+2)=static_cast<unsigned char>(200+(iTmp-201)*55/19.0);
				*(puc+1)=static_cast<unsigned char>(200-(iTmp-201)*200/19.0);
				*puc=static_cast<unsigned char>(200+(iTmp-201)*55/19.0);
			}else if(iTmp>=221 && iTmp<=240)
			{
				*(puc+2)=static_cast<unsigned char>(255);
				*(puc+1)=static_cast<unsigned char>((iTmp-221)*255/19.0);
				*puc=static_cast<unsigned char>(255-(iTmp-221)*255/19.0);
			}else if(iTmp>=241 && iTmp<=255)
			{
				*(puc+2)=static_cast<unsigned char>(255-(iTmp-241)*255/9.0);
				*(puc+1)=static_cast<unsigned char>(255-(iTmp-241)*255/9.0);
				*puc=static_cast<unsigned char>((iTmp-241)*255/9.0);
			}

			/*//black->grey->red->yellow->white
			if(iTmp>=0 && iTmp<=200)
			{
				*(puc+2)=static_cast<unsigned char>(iTmp);
				*(puc+1)=static_cast<unsigned char>(iTmp);
				*puc=static_cast<unsigned char>(iTmp);
			}else if(iTmp>=201 && iTmp<=220)
			{
				*(puc+2)=static_cast<unsigned char>(200+(iTmp-201)*55/19.0);
				*(puc+1)=static_cast<unsigned char>(200-(iTmp-201)*200/19.0);
				*puc=static_cast<unsigned char>(200-(iTmp-201)*200/19.0);
			}else if(iTmp>=221 && iTmp<=240)
			{
				*(puc+2)=static_cast<unsigned char>(255);
				*(puc+1)=static_cast<unsigned char>((iTmp-221)*255/19.0);
				*puc=static_cast<unsigned char>(0);
			}else if(iTmp>=241 && iTmp<=255)
			{
				*(puc+2)=static_cast<unsigned char>(255);
				*(puc+1)=static_cast<unsigned char>(255);
				*puc=static_cast<unsigned char>((iTmp-241)*255/9.0);
			}*/

			
			
		}else
		{
			*(puc+2) = *(puc+1) = *puc = static_cast<unsigned char>(a * i + b);
		}
        puc += 4;		
	}
}

CLUTManager CLUTManager::s_LUTManager;

CLUTManager::CLUTManager()
{
}

CLUTManager::~CLUTManager()
{
}

const CLUT32& CLUTManager::GetLUT32(int nWidth, int nLevel)
{
    if (nWidth != m_Lut32.GetWidth() || nLevel != m_Lut32.GetLevel())
    {
        m_Lut32.SetLevel(nLevel);
        m_Lut32.SetWidth(nWidth);
        m_Lut32.Update();
    }
    return m_Lut32;
}

const CLUT& CLUTManager::GetLUT(int nWidth, int nLevel)
{
    if (nWidth != m_Lut.GetWidth() || nLevel != m_Lut.GetLevel())
    {
        m_Lut.SetLevel(nLevel);
        m_Lut.SetWidth(nWidth);
        m_Lut.Update();
    }
    return m_Lut;
}

void CLUT32::SetGreyColor(BOOL bEnable)
{
	m_bGreyColor=bEnable;
	if(m_bGreyColor)
	{
		m_nMaxValue = 255;
		m_nMinValue = 0;
	}else
	{
		m_nMaxValue = 255;
		m_nMinValue = 0;
	}
	Update();
}

BOOL CLUT32::IsGreyColor()
{
	return m_bGreyColor;
}
