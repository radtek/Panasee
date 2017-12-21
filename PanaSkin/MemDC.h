// MemDC.h: interface for the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMDC_H__87FE0D5C_6361_4466_A72B_BD073DC80C1B__INCLUDED_)
#define AFX_MEMDC_H__87FE0D5C_6361_4466_A72B_BD073DC80C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panaskin.h"
#include <afxwin.h>

/*********** ningsj *********************
 ˵��:�ڴ滭ͼ���
      �������������Զ����ڴ��е�ͼ���Ƶ�ԴDC��
*****************************************/
class PANASKIN_API CMemDC : public CDC
{

public:
	void SetDCWindow(CWnd *pWnd);
	void SetRect(CRect rc);
	void EnableDrawBack(BOOL bEnable);
	void LoadDC(CDC* pDC, const CRect& rcBounds,BOOL bTransparent=FALSE,COLORREF TransparentColor=RGB(255,0,0));
	CMemDC();
	virtual ~CMemDC();

	

	CMemDC* operator->() {
		return this;
	}

	void DrawBack(CDC *pDC=NULL);

	void SetAnimation(int nType, int nSteps, int nAnimationTime);

private:
	void Animation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);
	void AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha);
	CBitmap	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
	COLORREF m_color;
	BOOL m_bTransparent;
	BOOL m_bDrawBackEnable;

	BOOL m_bInitialized;

	BOOL m_bAnimation;
	int m_aniType,m_aniSteps,m_aniTime;

	CWnd *m_pDCWnd; //DC���ڵĴ���ָ�� �ڶ���Ч��ʱ���ж��Ƿ��������Ч��
};


#endif // !defined(AFX_MEMDC_H__87FE0D5C_6361_4466_A72B_BD073DC80C1B__INCLUDED_)
