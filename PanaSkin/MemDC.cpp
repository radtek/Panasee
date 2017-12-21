// MemDC.cpp: implementation of the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemDC::CMemDC()
{
	m_oldBitmap=NULL;
	m_pDC=NULL;
	m_bInitialized=FALSE;
	m_bDrawBackEnable=TRUE;

	m_bTransparent=FALSE;

	m_bAnimation=FALSE;

	m_pDCWnd=NULL;
}

CMemDC::~CMemDC() 
{
	try
	{
		if(m_bDrawBackEnable)
			DrawBack();
		
		if(m_oldBitmap!=NULL)
		{
			SelectObject(m_oldBitmap);
			
			m_bitmap.DeleteObject();
			
			DeleteDC();
		}
	}catch(...)
	{
		//
	}
	
}



/*********** ningsj *********************
 说明:将内存中的图像复制到源DC中
*****************************************/

void CMemDC::DrawBack(CDC *pDC)
{

	if(pDC==NULL)
	{
		if(m_pDC==NULL)
			return;
		pDC=m_pDC;
	}


	if(!m_bTransparent)
	{
		if(!m_bAnimation)
		{
			pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
						this, 0,0,SRCCOPY); 
		}else
		{
			Animation(m_rcBounds,pDC,this,m_aniType,m_aniSteps,m_aniTime);
		}
		
		
	}else
	{
		TransparentBlt(pDC->GetSafeHdc(),m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
			this->GetSafeHdc(),0,0,m_rcBounds.Width(), m_rcBounds.Height(),m_color);
	}

	return;

	/*
	COLORREF crOldBack = pDC->SetBkColor(RGB(255,255,255));
	COLORREF crOldText = pDC->SetTextColor(RGB(0,0,0));
	CDC dcTrans;

	// Create memory dcs for the mask
	dcTrans.CreateCompatibleDC(pDC);
      
	// Create the mask bitmap
	CBitmap bitmapTrans;
	int nWidth = m_rcBounds.Width();
	int nHeight = m_rcBounds.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
	SetBkColor(m_color);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, this, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
						this, 0, 0, SRCINVERT);
	pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
						&dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
						this, 0, 0, SRCINVERT);

	// Restore settings
	dcTrans.SelectObject(pOldBitmapTrans);

	bitmapTrans.DeleteObject();
	dcTrans.DeleteDC();

	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText); */

}

void CMemDC::SetAnimation(int nType, int nSteps, int nAnimationTime)
{
	if(nType<0)
	{
		m_bAnimation=FALSE;
		return;
	}
	m_bAnimation=TRUE;
	m_aniType=nType;
	m_aniSteps=nSteps;
	m_aniTime=nAnimationTime;
}

void CMemDC::Animation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime)
{
	CRect r;
	switch (nType)
	{
		case 0:
			{
				int i;
				for (i = 0; i < nSteps; i++)
				{
					if(m_pDCWnd)
					{
						CRect rc;
						POINT point;
						::GetWindowRect(m_pDCWnd->GetSafeHwnd(),&rc);
						::GetCursorPos(&point);
						if(!rc.PtInRect(point))
							break;
					}

					int bottom = rc.Height() * (i + 1) / nSteps;
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), bottom, pSrcDC,
						0, rc.Height()- bottom , SRCCOPY);
					Sleep(nAnimationTime / nSteps);
				}

				/*
				const int bottom = rc.bottom - rc.top;
				int i;
				for (i = 0; i < rc.Height();
					i += (1 + (rc.Height() / nSteps)))
				{
					if(m_pDCWnd)
					{
						CRect rc;
						POINT point;
						::GetWindowRect(m_pDCWnd->GetSafeHwnd(),&rc);
						::GetCursorPos(&point);
						if(!rc.PtInRect(point))
							break;
					}
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), i, pSrcDC,
						0, bottom - i, SRCCOPY);
					Sleep(nAnimationTime / nSteps);
				}*/
			}
			break;

		case 1:
			{
				r = rc;
				int i;
				for (i = 0; i < nSteps; ++i)
				{
					if(m_pDCWnd)
					{
						CRect rc;
						POINT point;
						::GetWindowRect(m_pDCWnd->GetSafeHwnd(),&rc);
						::GetCursorPos(&point);
						if(!rc.PtInRect(point))
							break;
					}

					r.bottom = r.top + rc.Height() * (i + 1) / nSteps;
					r.right = r.left + rc.Width() * (i + 1) / nSteps;
					pDestDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pSrcDC,
						0, 0, SRCCOPY);
					Sleep(nAnimationTime / nSteps);
				}
			}
			break;

		case 2:
			{

				const int cx = rc.Width();
				const int cy = rc.Height();

				BITMAPINFOHEADER BMI;
				// Fill in the header info.
				ZeroMemory (&BMI, sizeof(BMI));
				BMI.biSize = sizeof(BITMAPINFOHEADER);
				BMI.biWidth = cx;
				BMI.biHeight = cy;
				BMI.biPlanes = 1;
				BMI.biBitCount = 32;
				BMI.biCompression = BI_RGB;   // No compression

				BYTE * pSrcBits = NULL;
				HBITMAP hbmSrc = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

				BYTE * pSrcBackBits = NULL;
				HBITMAP hbmSrcBack = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBackBits, 0, 0l);

				BYTE * pDestBits = NULL;
				HBITMAP hbmDest = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

				// Copy our source and destination bitmaps onto our DIBSections,
				// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

				CDC dc;
				dc.CreateCompatibleDC(NULL);
				HBITMAP hbmpOld = (HBITMAP) ::SelectObject(dc, hbmSrc);
				::BitBlt(dc, 0, 0, cx, cy, pSrcDC->GetSafeHdc(), 0, 0, SRCCOPY);

				::SelectObject(dc, hbmSrcBack);
				::BitBlt(dc, 0, 0, cx, cy, pDestDC->GetSafeHdc (), rc.left, rc.top, SRCCOPY);

				::SelectObject(dc, hbmDest);

				DWORD dwTimePer = nAnimationTime / nSteps;
				::SelectObject(dc, hbmDest);
				for (int i = 1; i < nSteps; ++i)
				{
					if(m_pDCWnd)
					{
						CRect rc;
						POINT point;
						::GetWindowRect(m_pDCWnd->GetSafeHwnd(),&rc);
						::GetCursorPos(&point);
						if(!rc.PtInRect(point))
						{
							//TRACE("break;\n");
							break;
						}
					}

					DWORD dwTime = GetTickCount ();
					AlphaBlendU(pDestBits, pSrcBackBits, cx, cy, pSrcBits, (BYTE)((255*i) / nSteps));
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
					dwTime = GetTickCount () - dwTime;
					if (dwTime < dwTimePer)
					{
						Sleep(dwTimePer - dwTime);
					}
				}
				::SelectObject(dc, hbmpOld);
				DeleteObject(hbmSrc);
				DeleteObject(hbmSrcBack);
				DeleteObject(hbmDest);
			}
			break;
	}
}

void CMemDC::AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha)
{
	BYTE byDiff = (BYTE)(255 - byAlpha);

	int iRowOffset = 0, j = cy;
	while (j > 0)
	{
		--j;
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrc)[iRowOffset];
		LPBYTE pbSrcBackRGB = (LPBYTE)&((DWORD*)pSrcBack)[iRowOffset];
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDest)[iRowOffset];
		iRowOffset += cx;
		int i = cx;
		while (i > 0)
		{
			--i;
			pbDestRGB[0] = (BYTE)((pbSrcBackRGB[0] * byDiff + pbSrcRGB[0] * byAlpha) >> 8);
			pbDestRGB[1] = (BYTE)((pbSrcBackRGB[1] * byDiff + pbSrcRGB[1] * byAlpha) >> 8);
			pbDestRGB[2] = (BYTE)((pbSrcBackRGB[2] * byDiff + pbSrcRGB[2] * byAlpha) >> 8);
			pbSrcRGB += 4;
			pbSrcBackRGB += 4;
			pbDestRGB += 4;
		}
	}
}

/*********** ningsj *********************
 说明:设置内存DC参数
      pDC:源DC
	  rcBounds:源DC的目标区域
	  bTransparent:是否使用透明色
	  TransparentColor:透明色
*****************************************/
void CMemDC::LoadDC(CDC *pDC, const CRect &rcBounds, BOOL bTransparent, COLORREF TransparentColor)
{
	
	if(pDC==NULL)
		return;

	if(m_bInitialized)
	{
		if(m_oldBitmap!=NULL)
		{
			SelectObject(m_oldBitmap);
		
			m_bitmap.DeleteObject();

			DeleteDC();
		}
	}


	CreateCompatibleDC(pDC);
	m_bitmap.CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
	m_oldBitmap = SelectObject(&m_bitmap);
	m_pDC = pDC;
	m_rcBounds = rcBounds;
    
	m_color=TransparentColor;
	m_bTransparent=bTransparent;

	BitBlt(0,0,rcBounds.Width(), rcBounds.Height(),
		pDC,0,0,SRCCOPY);

	m_bInitialized=TRUE;
}

/*********** ningsj *********************
 说明:是否在析构函数中自动将内存中的图像复制到源DC中
*****************************************/
void CMemDC::EnableDrawBack(BOOL bEnable)
{
	m_bDrawBackEnable=bEnable;
}

/*********** ningsj *********************
 说明:在将内存中的图像复制到源DC中,源DC的目标区域
      使用本函数将把以前的绘制结果删除
	  必须调用一次LoadDC之后才可以使用本函数
*****************************************/
void CMemDC::SetRect(CRect rc)
{
	if(m_pDC==NULL)
		return;

	if(m_bInitialized)
	{
		if(m_oldBitmap!=NULL)
		{
			SelectObject(m_oldBitmap);
		
			m_bitmap.DeleteObject();

		}
	}
	
	m_bitmap.CreateCompatibleBitmap(m_pDC, rc.Width(), rc.Height());
	m_oldBitmap = SelectObject(&m_bitmap);
	m_rcBounds = rc;
    
	
	m_bInitialized=TRUE;
}

void CMemDC::SetDCWindow(CWnd *pWnd)
{
	m_pDCWnd=pWnd;
}
