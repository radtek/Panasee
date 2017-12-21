// ColorF.h: interface for the CColorF class.
// Created by Wei Wei [Apr.30, 2006]
// InterSmart Robotic System Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORF_H__6FB5CF73_A8C1_49D6_BC2E_1088401E4BD8__INCLUDED_)
#define AFX_COLORF_H__6FB5CF73_A8C1_49D6_BC2E_1088401E4BD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __declspec(dllexport) CColorF  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CColorF();
	CColorF( float fR, float fG, float fB, float fA );
	virtual ~CColorF();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get color
	void GetColor( float& fR, float& fG, float& fB, float& fA );

	// Set color
	void SetColor( float fR, float fG, float fB, float fA );

	// Get red component
	float GetR();

	// Set red component
	void SetR( float fR );

	// Get green component
	float GetG();

	// Set green component
	void SetG( float fG );

	// Get blue component
	float GetB();

	// Set blue component
	void SetB( float fB );

	// Get alpha component
	float GetA();

	// Set alpha component
	void SetA( float fA );

private:

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Red component
	float m_fR;

	// Green component
	float m_fG;

	// Blue component
	float m_fB;

	// Alpha component
	float m_fA;

};

#endif // !defined(AFX_COLORF_H__6FB5CF73_A8C1_49D6_BC2E_1088401E4BD8__INCLUDED_)
