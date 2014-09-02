#if !defined(AFX_SECTIONSTATIC_H__BFB9FBDF_2C92_4E0A_89AF_4165E8704546__INCLUDED_)
#define AFX_SECTIONSTATIC_H__BFB9FBDF_2C92_4E0A_89AF_4165E8704546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "llDlg.h"
// SectionStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectionStatic window

class CSectionStatic : public CStatic
{
// Construction
public:
	CSectionStatic();
	CLlDlg* dlg;
	CDialog* SectionDraw;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSectionStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSectionStatic)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONSTATIC_H__BFB9FBDF_2C92_4E0A_89AF_4165E8704546__INCLUDED_)
