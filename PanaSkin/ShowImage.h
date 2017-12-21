// ShowImage.h: interface for the CShowImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWIMAGE_H__BE73DB8C_FCF1_4992_81F2_314BA7DE7045__INCLUDED_)
#define AFX_SHOWIMAGE_H__BE73DB8C_FCF1_4992_81F2_314BA7DE7045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panaskin.h"
#include "BitmapPng.h"
/*********** ningsj *********************
 ˵��:��ͼ����
      1.��ָ����������ͼ����������͸��ɫ
      2.��ͼʱ�������Զ���ͼƬ���ţ�����Ӧ����ͼ����Ĵ�С
	  3.ͼƬ���Ÿ���ԭʼͼƬ��4���ǵĴ�С�Զ�����
*****************************************/
class  PANASKIN_API CShowImage  
{
public:
	BOOL SetImage(CString sFilePath,int Count=1);
	CShowImage();
	virtual ~CShowImage();

	BOOL LoadBitmap(UINT IDBitmap);
	BOOL SetImage(UINT IDBitmap,int Count=1);
	BOOL SetImage(HMODULE hModule,UINT IDBitmap,int Count=1);
	BOOL DrawImage(CDC *pDC,int iImageIndex);
	void GetBitmap(BITMAP *bmpinfo);
	void SetShowRegion(CRect rc);
	void SetShowRegion(int iLeft,int iTop,int iWidth,int iHeight);
	/*********** ningsj *********************
	˵��:����͸��ɫ
	*****************************************/
	void SetTransparentColor(COLORREF color=-1);
	void SetSizingMargin(int Left,int Top,int Right,int Bottom);	
	BOOL DrawImagePart(CDC* pDCDest, const CRect& rcDest,const CRect& rcSrc);
private:
	
	BOOL DrawImagePart(CDC* pDCDest, int DestLeft,int DestTop,int DestRight,int DestBottom,
							   CDC* pDCSrc, int SrcLeft,int SrcTop,int SrcRight,int SrcBottom);
	BOOL DrawImagePart(CDC* pDCDest, const CRect& rcDest, CDC* pDCSrc, const CRect& rcSrc);

	COLORREF m_transparentColor;
	int m_iImageCount,m_iImageWidth,m_iImageHeight;
	int m_iMarginLeft,m_iMarginTop,m_iMarginRight,m_iMarginBottom;
	int m_iShowRegionLeft,m_iShowRegionTop,m_iShowRegionWidth,m_iShowRegionHeight;
	
	CBitmapPng *m_pImage;
	BOOL m_bAlpha; //ͼƬ�Ƿ���alphaͨ��
	BOOL m_bTransparent;  //ͼƬ�Ƿ���͸��ɫ
};

#endif // !defined(AFX_SHOWIMAGE_H__BE73DB8C_FCF1_4992_81F2_314BA7DE7045__INCLUDED_)
