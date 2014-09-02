#if !defined(AFX_MYSTATIC_H__A8E0083E_623C_4320_B3CB_1BF3F664DF0C__INCLUDED_)
#define AFX_MYSTATIC_H__A8E0083E_623C_4320_B3CB_1BF3F664DF0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "globaldefs.h"

// MyStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyStatic window

class CMyStatic : public CStatic
{
// Construction
public:
	CMyStatic();
// Attributes
public:
	CArray<int,int>* drawhorizontal;
	CArray<int,int>* drawvertical;
	CDialog* m_dlg;//CLlDlg
	CDC* m_paintDC;
// Operations
public:
	enum
	{
		MATCH_HORIZONTAL=0x01,
		MATCH_VERTICAL=0x02,
	};
	int AxisFromPoint(const CPoint& point,const CRect& uisize,int& hor,int& ver);//返回值4种情况

	void UpdateElements();
	void CancelSelect();

	BOOL GetPILLARFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj);
	BOOL GetBEAMFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj);
	BOOL GetRECTZONEFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj);
	BOOL GetPOLYZONEFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj);
	void PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyStatic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATIC_H__A8E0083E_623C_4320_B3CB_1BF3F664DF0C__INCLUDED_)
