#if !defined(AFX_PROP3DTREE_H__2AE5DAEB_70C7_4E8D_BFF1_5C48F0BA854E__INCLUDED_)
#define AFX_PROP3DTREE_H__2AE5DAEB_70C7_4E8D_BFF1_5C48F0BA854E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Prop3DTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProp3DTree window

class CProp3DTree : public CTreeCtrl
{
// Construction
public:
	CProp3DTree();

// Attributes
public:

// Operations
public:
	HTREEITEM m_hDraggedItem;
	BOOL m_bDraggingNow;
	CImageList *m_pDragImageList;
	CString m_sOldGroup;
	CString m_sTraceName;
	CString m_sNewGroup;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProp3DTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProp3DTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProp3DTree)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROP3DTREE_H__2AE5DAEB_70C7_4E8D_BFF1_5C48F0BA854E__INCLUDED_)
