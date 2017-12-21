// ModelObject.h: interface for the CModelObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELOBJECT_H__C2472CC2_3D2D_48CF_9497_A03907D780A9__INCLUDED_)
#define AFX_MODELOBJECT_H__C2472CC2_3D2D_48CF_9497_A03907D780A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/ColorF.h"
#include "BoundingBox.h"
#include "afxwin.h"
#include "../Common/Texture2D.h"

class CTexture2D;
class Vector2D;

typedef enum Draw_Property {
	DP_LIGHTING,
} DRAW_PROPERTY;

typedef struct SModelingText {
	CString sText;
	Vector3D vPosition;
	CColorF cfColor;
	BOOL	bShow;
	CFont   *pFont;
	BOOL m_bDisableDepthTest;
	
	SModelingText()
	{
		pFont = NULL;
		m_bDisableDepthTest=TRUE;
	}

	~SModelingText()
	{
		if (pFont)
		{
			pFont = NULL;
		}
	}
} ST_TEXT;

typedef enum
{
	 METERIAL_BRASS,     //黄铜	（黄铜色）
	 METERIAL_BRONZE, 	 //青铜（青铜色）
	 METERIAL_CHROME,	 //铬合金（褐色白）
	 METERIAL_COPPER,	 //铜（铜黄色）
	 METERIAL_GOLD,	     //金（金黄色）
	 METERIAL_P_GOLD,	 //褐色金（金褐色）
	 METERIAL_PEWTER,	 //白蜡（蜡白色）
	 METERIAL_SILVER,	 //银（银白色）
	 METERIAL_P_SILVER,	 //褐色银（银褐色）
	 METERIAL_EMERALD,	 //绿翡翠（翠绿色）
	 METERIAL_JADE,	     //碧玉（碧玉色）
	 METERIAL_OBSIDIAN,	 //黑曜石（暗黑色）
	 METERIAL_PEARL,	 //珍珠（珍珠粉色）
	 METERIAL_RUDY,	     //红宝石（宝石红色）
	 METERIAL_TURQUOISE	 //绿松石（青绿色）
}MATERIAL_TYPE;

class __declspec(dllexport) CModelObject  
{
public:
	void SetTexture(STRUCT_COLORF *pImageData, int nWidth, int nHeight);
	CModelObject();
	virtual ~CModelObject();

	// Set model name
	void SetName( CString sName);

	// Get model name
	CString GetName();

	// Get bounding box
	CBoundingBox* GetBoundingBox();

	// Set whether to show the model or not
	void SetVisible(BOOL bVisible);

	// Get whether the model is visible or not
	BOOL IsVisible();

	// Get color
	CColorF* GetColor();

	// Set color 
	void SetColor(CColorF* pColor );

	// Get the flag of using transparency
	bool IsUsingTransparency();

	// Set the flag of using transparency
	void SetUsingTransparency( bool bUseTransparency );

	// Get texture coordinates
	Vector2D* GetTexCoor();

	// Set texture coordinates
	virtual void SetTexCoor( Vector2D* pvTexCoor ) = 0;

	// Get texture
	CTexture2D* GetTexture();

	// Set texture
	void SetTexture( CTexture2D* pTexture );

	// Set draw properties
	void Enable(DRAW_PROPERTY iDrawProperty);
	void Disable(DRAW_PROPERTY iDrawProperty);

	BOOL IsUsingLighting();

	// Get the polygon rasterization mode
	POLYGON_RASTERIZATION_MODE GetPolygonRasterizationMode();

	// Set the polygon rasterization mode
	void SetPolygonRasterizationMode( POLYGON_RASTERIZATION_MODE mode );

	// Get the flag of using texture mapping
	bool IsUsingTextureMapping();

	// Set the flag of using texture mapping
	void SetUsingTextureMapping( bool bUsingTextureMapping );

	// Get the flag of updating sub texture
	bool IsUpdatingSubTexture();

	// Set the flag of updating sub texture
	void SetUpdatingSubTexture( bool bUpdatingSubTexture );

	void IncreaseReferenceCnt();
	void DecreaseReferenceCnt();

	void IncreaseVisibleCnt();
	void DecreaseVisibleCnt();
	
	// Transform by 4*4 matrix
	// ppdTrans: 4*4
	virtual void Transformation( double** ppdTrans ) = 0;

	// Translation by 3*1 vector
	virtual void Translation( Vector3D vTranslation )  = 0;

	// Rotate by 3*3 matrix
	virtual void Rotation( double** ppdRot )  = 0;

	// Rotate dAngle degree about an axis
	virtual void Rotation( Vector3D vAxis, double dAngle )  = 0;

	// Scale from a point
	virtual void Scale( Vector3D vScaleCenter, double dRatio )  = 0;


protected:

	//////////////////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////////////////
	
	// Bounding box
	CBoundingBox m_BoundingBox;

	// Color
	// R, G, B, A: 0~255
	CColorF m_Color;

	// Texture coordinates
	Vector2D* m_pvTexCoor;

	// Pointer to texture
	CTexture2D* m_pTexture;
	
private:

	// Name
	CString  m_sName;

	// Show this model or not, default is TRUE;
	BOOL m_bVisible;

	// Flag of using transparency
	bool m_bUsingTransparency;

	// Polygon rasterization mode:
	// { MODELING_GL_POINT, MODELING_GL_LINE, MODELING_GL_FILL}
	// The default is MODELING_GL_FILL (i.e. The interior of the polygon is fill)
	POLYGON_RASTERIZATION_MODE m_PolygonRasterizationMode;

	// Flag of using texture mapping
	bool m_bUsingTextureMapping;

	// Flag of updating sub texture
	bool m_bUpdatingSubTexture;

	// Count of display window
	int  m_iReferenceCnt;

	// Count of visible window to display this object
	int  m_iVisibleCnt;

	// Count of times texture need to be updated, this corresponding to visible window showing this object
	int  m_iUpdateTextureCnt;
	void DecreaseUpdateTextureCnt();

	BOOL m_bGLUsingLighting;

};

#endif // !defined(AFX_MODELOBJECT_H__C2472CC2_3D2D_48CF_9497_A03907D780A9__INCLUDED_)
