// Display.h: interface for the Display class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__34F69226_DB0E_4C42_A179_0C053E30F918__INCLUDED_)
#define AFX_DISPLAY_H__34F69226_DB0E_4C42_A179_0C053E30F918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"
#include "geometry.h"
#include "ColorF.h"
#include "ModelObject.h"
//#include "../Image/Dib.h"

#define DISPLAY_INTERFACE_OPENGL	1
#define DISPLAY_INTERFACE_DIRECT3D  2

//���ƶ���
#define DISPLAY_POINTS		1
#define DISPLAY_LINES		2
#define DISPLAY_TRIANGLES	3
#define DISPLAY_QUADS		4
#define DISPLAY_POLYGON		5

#define PICKRANGE 4             //ʰȡ�ķ�Χ
#define HALFPICKRANGE 2

//����
//#define METERIAL_BRASS		1
//#define METERIAL_BRONZE		2
//#define METERIAL_CHROME		3
//#define METERIAL_COPPER		4
//#define METERIAL_GOLD		5
//#define METERIAL_P_GOLD		6
//#define METERIAL_PEWTER		7
//#define METERIAL_SILVER		8
//#define METERIAL_P_SILVER	9
//#define METERIAL_EMERALD	10
//#define METERIAL_JADE		11
//#define METERIAL_OBSIDIAN	12
//#define METERIAL_PEARL		13
//#define METERIAL_RUDY		14
//#define METERIAL_TURQUOISE	15


class __declspec(dllexport) CDisplay  
{
public:
	CDisplay();
	virtual ~CDisplay();
	BOOL	CreateOpenGL(HDC hDC);
	void	LookAt(Vector3D vEye,Vector3D vGaze,Vector3D vUp);
	void	Reshape(int cx,int cy);
	void	Lock();
	void	Unlock();
	void	GLBegin(BYTE mode);
	void	GLEnd();
	void	GLVertex(Vector3D v);
	void	GLNormal(Vector3D v);
	void	GLTexture(double s,double t);
	void	GLPointSize(float f);
	void	GLLineWidth(float f);
	void	GLColor3f(float r, float g, float b);
	void	GLColor3ub(BYTE r, BYTE g, BYTE b);
	void	GLColor4f(float r, float g, float b, float a);
	void	GLColor4ub(BYTE r, BYTE g, BYTE b, BYTE a);
	void	SetBGColor(float r, float g, float b);
	void	SetMeterial(BYTE meterial);
	void	BeginPick();
	int		EndPick(int x,int y);
	void	GLBindTexture(int iTexture);
	void    GLText(Vector3D v, char *str, CFont *pFont = NULL, BOOL bDisableDepthTest = TRUE);
	void	GLColorBox(double dbMaxMiss);
	void	GLCylinder(Vector3D vCentroid, Vector3D vAxis, double dRadius, double dHalfHeight, COLORREF crColor = 0x00000000, BOOL bDisableDepthTest = TRUE);
	void	GLSphere( Vector3D vCentroid, double dRadius );
	void	GLEllipsoid( Vector3D vCentroid, double dSemiAxis[3] );	// Draw a standard ellipsoid which semi-axes are along the x, y and z axis respectively
	void	GLDrawPixels( GLint nXDest, GLint nYDest, GLint nDestWidth, GLint nDestHeight, 
										GLenum format, GLenum type, const GLvoid *pixels);
	void	GLSwapBuffers();
	void	GLCallList(int nlist);

	// Transformation
	void	GLMultMatrixd( double** ppdTrans );

	//���ó�������Ⱦ��ʽ
	void	RenderWireFrame();
	void	RenderHidenLine(bool bBack);
	void	RenderFlatShade(bool bLine);
	void	RenderSmoothShade();

//private:
	BOOL	SetWindowPixelFormat(HDC hDC);
	BOOL	CreateViewGLContext(HDC hDC);
	void	buildList();
//	void	createTexture(UINT textureArray[], LPSTR strFileName, int textureID);
	BOOL    MakeCurrent(){
		return wglMakeCurrent(m_hDC,m_hGLContext);
	};
public:
	BYTE	m_Interface;//OpenGL or Direct3D
	HDC		m_hDC;
	Vector3D m_vEyePoint;
	Vector3D m_vGazePoint;
	Vector3D m_vUp;
	float	m_BGcolor[3];
	int		m_Width;
	int		m_Height;
	BOOL	m_bShowCoordAxis;

	//for OpenGL
	int		m_GLPixelIndex;
	HGLRC	m_hGLContext;
	GLuint	m_ListNet;
	GLuint	m_ListXYZ;

	//picking
	UINT m_PickNum[PICKRANGE*PICKRANGE];//��RGBֵת�ɳ�����ֵ
	GLubyte m_Pick[PICKRANGE][PICKRANGE][3];//�洢ѡ�����������е��RGBֵ

	UINT m_Texture[10];
public:
	void GLLine(Vector3D pnt1,Vector3D pnt2);
	void SetMaterialNew( BYTE byMaterial,float fTransparency );
	/*
	 *	add by macintosh
	 */
//	void createTexture(CDib*, int);
	void CreateTexture( unsigned short* pImageData, int nWidth, int nHeight, unsigned int& nTextureID );	// Added by Wei Wei [Apr.9, 2006]
	void CreateTexture( float* pImageData, int nWidth, int nHeight, unsigned int& nTextureID );	// Added by Wei Wei [Apr.11, 2006]
	void UpdateSubTexture( float* pImageData, int nXOffset, int nYOffset, int nWidth, int nHeight, unsigned int nTextureID );	// Added by Wei Wei [Apr.12, 2006]

	COLORREF m_MaterialColor[15];
};

#endif // !defined(AFX_DISPLAY_H__34F69226_DB0E_4C42_A179_0C053E30F918__INCLUDED_)
