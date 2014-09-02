#if !defined(AFX_ELEMENTLISTBOX_H__0E47E490_F8D7_4658_A0FF_88D3C3F4EA40__INCLUDED_)
#define AFX_ELEMENTLISTBOX_H__0E47E490_F8D7_4658_A0FF_88D3C3F4EA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "llDlg.h"
// ElementListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElementListBox window

class CElementListBox : public CListBox
{//处理按键响应
// Construction
public:
	CElementListBox();

// Attributes
public:
	CListBox* propertyshow;
	CLlDlg* m_dlg;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementListBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CElementListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CElementListBox)
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTLISTBOX_H__0E47E490_F8D7_4658_A0FF_88D3C3F4EA40__INCLUDED_)
