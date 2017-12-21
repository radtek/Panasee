
#if !defined(AFX_WBBUTTON_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
#define AFX_WBBUTTON_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MoveableCtrl.h"
#include "panaskin.h"
#include "AutoFont.h"
#include "ShowImage.h"


/*********** ningsj *********************
 ˵��:ͼƬ��ť
      ֧��normal,disable,focus,selected����״̬
	  ��������͸��ɫ�͸����ڵı����ں�
	  ���Զ�̬��������
*****************************************/



// This class implements a Window Blinds like button
class PANASKIN_API CImageButton : public CButton,public CMoveableCtrl
{

public:
	CImageButton();
	virtual ~CImageButton();
// Methods
public:
	void EnableTrigger(BOOL bEnable);
	BOOL m_bBtnDown;
	void SetFontBkColor(COLORREF color);
	void EnableMultiLine(BOOL bEnable);
	void SetTextPosition(UINT pos=DT_CENTER);
    // Bitmap Loding and Initialize
    bool LoadBitmaps(UINT id ,int Top, int Bottom,int Left, int Right,BOOL bRedraw=FALSE);
	bool LoadSecondBitmaps(UINT id ,int Top, int Bottom,int Left, int Right,BOOL bRedraw=FALSE);
	bool LoadBitmaps(HMODULE hMoudle,UINT id ,int Top, int Bottom,int Left, int Right,BOOL bRedraw=FALSE);
	bool LoadSecondBitmaps(HMODULE hMoudle,UINT id ,int Top, int Bottom,int Left, int Right,BOOL bRedraw=FALSE);
	
	void SetChecked(BOOL bStatus);
	BOOL GetChecked(){ return m_bChecked; }
	
	void SetTransparentColor( COLORREF color ) { m_TransparentColor = color; m_bTransparentColor=TRUE; }
    COLORREF GetTransparentColor() { return m_TransparentColor; }
	enum ImageBtn_ButtonMode
    {
        ImageBtn_PushButton  = 0,
		ImageBtn_CheckButton = 1,
        ImageBtn_RadioButton = 2
    };

	void SetButtonMode(ImageBtn_ButtonMode mode);
	void SetTextFont( CFont & fnt );
    void SetTextFont( CAutoFont & fnt );
    CFont * GetTextFont() { return (CFont *)&m_Font; }
    CAutoFont * GetTextAutoFont() { return &m_Font; }
    void SetFontColor( COLORREF color );
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate);
	void ActivateTooltip(BOOL bActivate);

	void SetBtnMenu(CMenu *pMenu);

	DWORD SetClickedCallback(HWND hWnd, UINT nMessage, LPARAM lParam = 0);

protected:

    enum ImageBtn_mode
    {
        ImageBtn_normal  = 0,
        ImageBtn_select  = 1,
        ImageBtn_focus   = 2,
        ImageBtn_disable = 3
    };

	typedef struct _ImageBtn_STRUCT_CALLBACK
	{
		HWND		hWnd;			// Handle to window
		UINT		nMessage;		// Message identifier
		WPARAM		wParam;
		LPARAM		lParam;
	} ImageBtn_STRUCT_CALLBACK;

	ImageBtn_STRUCT_CALLBACK	m_csCallbacks;


	//{{AFX_VIRTUAL(CImageButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	
protected:
	//{{AFX_MSG(CImageButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnClicked();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	
	BOOL m_bEnableTrigger;
	void InitToolTip();

	CAutoFont m_Font;
	BOOL m_bMouseOnButton;
	BOOL m_bChecked;
	ImageBtn_ButtonMode m_ButtonMode;

	COLORREF m_TransparentColor;
	BOOL m_bTransparentColor;

	CShowImage m_memImage;
	
	//��ťͼƬ������ͼƬ���
	//�ڶ���ͼƬ�����ڵ�һ��ͼƬ����
	CShowImage m_memImage2; 
	BOOL m_bSecondImage;

	CToolTipCtrl m_ToolTip;			// Tooltip

	UINT m_txtPosition; //�ı��ڰ�ť�ϵ�λ�ã�Ĭ���Ǿ��С�

	CMenu *m_btnMenu;

	BOOL m_bFirstEnter; //�Ӵ�����������봰�ں��һ��ˢ��ʱ��ִ�ж���Ч����
	int m_iAniType,m_iAniStep,m_iAniTime;  //�������� �������� ����ʱ��

	BOOL m_bEnableMultiLine;

	BOOL m_bEnableFontBkColor;
	COLORREF m_fontBkColor;
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_WBBUTTON_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
