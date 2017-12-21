// ColorB.h: interbyAce for the CColorB class.
// Created by Wei Wei [Apr.30, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORB_H__25CCAA0C_8DC7_4736_85A8_5DE7C93013D9__INCLUDED_)
#define AFX_COLORB_H__25CCAA0C_8DC7_4736_85A8_5DE7C93013D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __declspec(dllexport) CColorB  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////

	CColorB();
	CColorB( unsigned char byR, unsigned char byG, unsigned char byB, unsigned char byA );
	virtual ~CColorB();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get color
	void GetColor( unsigned char& byR, unsigned char& byG, unsigned char& byB, unsigned char& byA );

	// Set color
	void SetColor( unsigned char byR, unsigned char byG, unsigned char byB, unsigned char byA );

	// Get red component
	unsigned char GetR();

	// Set red component
	void SetR( unsigned char byR );

	// Get green component
	unsigned char GetG();

	// Set green component
	void SetG( unsigned char byG );

	// Get blue component
	unsigned char GetB();

	// Set blue component
	void SetB( unsigned char byB );

	// Get alpha component
	unsigned char GetA();

	// Set alpha component
	void SetA( unsigned char byA );

private:

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Red component
	unsigned char m_byR;

	// Green component
	unsigned char m_byG;

	// Blue component
	unsigned char m_byB;

	// Alpha component
	unsigned char m_byA;

};

#endif // !defined(AFX_COLORB_H__25CCAA0C_8DC7_4736_85A8_5DE7C93013D9__INCLUDED_)
