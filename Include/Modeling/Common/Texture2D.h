// Texture2D.h: interface for the CTexture2D class.
// Created by Wei Wei [Apr.9, 2006]
// InterSmart Robotic Systems Co., Ltd.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE2D_H__759625AB_9245_4F51_8EEE_1637966BC6E2__INCLUDED_)
#define AFX_TEXTURE2D_H__759625AB_9245_4F51_8EEE_1637966BC6E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SCENE	20

// ------------------------------------------------
// 颜色结构，替换CColorF，提高复制CTexture2D的速度 
//ningsj 2009.2.10
struct STRUCT_COLORF
{
	float R,G,B,A;
};

class __declspec(dllexport) CTexture2D  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CTexture2D();
	CTexture2D( CTexture2D* pTexture );
	CTexture2D( STRUCT_COLORF* pImageData, int nWidth, int nHeight );
	virtual ~CTexture2D();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Get texture ID
	unsigned int GetTextureID();

	// Set texture ID
	void SetTextureID( unsigned int nTextureID );

	// Get texture ID given scene ID
	unsigned int GetTextureID(int iSceneID);

	// Set texture ID attached with scene ID
	void SetTextureID(int iSceneID, unsigned int iTextureID);

	// Copy textureID from another texture
	void CopyTextureID(CTexture2D *pSrcTexture2D);
	
	// Get image data
	STRUCT_COLORF* GetImageData();

	// Set image data
	void SetImageData( STRUCT_COLORF* pImageData, int nWidth, int nHeight );

	// Set image data at point(x, y)
	void SetImageDataAt( STRUCT_COLORF* pColor, int nPointX, int nPointY );

	// Get width
	int GetWidth();

	// Get height
	int GetHeight();

	void SaveToFile( CString sFileName );


	int m_ppnTextureID[MAX_SCENE][2];
private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////
	
	// Reset internal variables. This function should be called only by constructors.
    void Reset();

	// Initialize GL texture
    void Update();

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
//	// If true, texture bitmap has not yet been sent to video card.
//    bool m_bUpdateTextureFlag;

    // OpenGL texture ID number.
    unsigned int m_nTextureID;


//    // Texture wrap parameter along S and T (GL_REPEAT or GL_CLAMP)
//    int m_nWrapSmode;
//    int m_nWrapTmode;
//
//    // Texture magnification function. (GL_NEAREST or GL_LINEAR)
//    int m_nMagnificationFunction;
//
//    // Texture minifying function. (GL_NEAREST or GL_LINEAR)
//    int m_nMinifyingFunction;
//
//    // If true, we use GLU to build mipmaps.
//    bool m_bUseMipmaps;
//
//    // If true, we use spherical mapping.
//    bool m_bUseSphericalMapping;
//
//    // OpenGL texture mode (GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE)
//    int m_nEnvironmentMode;
	
	// The dimensions of the current image
    int m_nWidth, m_nHeight;

	// Image data, RGBA format only
	STRUCT_COLORF* m_pImageData;

};

#endif // !defined(AFX_TEXTURE2D_H__759625AB_9245_4F51_8EEE_1637966BC6E2__INCLUDED_)
