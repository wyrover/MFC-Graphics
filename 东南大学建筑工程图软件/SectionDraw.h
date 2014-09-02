#if !defined(AFX_SECTIONDRAW_H__8A4B3537_9E04_40A1_9100_DF5803F7A4AF__INCLUDED_)
#define AFX_SECTIONDRAW_H__8A4B3537_9E04_40A1_9100_DF5803F7A4AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SectionStatic.h"
#include "SectionSet.h"
// SectionDraw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectionDraw dialog

class CSectionDraw : public CDialog
{
// Construction
public:
	CSectionDraw(CWnd* pParent = NULL,CSectionSet* pSet=NULL);   // standard constructor
	CLlDlg* m_dlg;
	CSectionSet* m_setdlg;
	CDC MyDC;
	CDC* tempdc;
	CBitmap bmp;
	CBitmap* oldbmp;

	CPoint Fixone;//Ότµ¶³Ε
	CPoint Moveone;
	CArray<SCISSORSLINE*,SCISSORSLINE*> Scissorsline;

	BOOL DrawType;
	STATE curstate;
// Dialog Data
	//{{AFX_DATA(CSectionDraw)
	enum { IDD = IDD_LL_DIALOG_SECTION };
	CSectionStatic	m_sectiondraw;
	//}}AFX_DATA
	void Update();
	inline void PhysicalPointToUIPoint(POINT* pt,const RECT& uirect,int maxx,int maxy,int x,int y);
	inline void PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize,int maxx,int maxy);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionDraw)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionDraw)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONDRAW_H__8A4B3537_9E04_40A1_9100_DF5803F7A4AF__INCLUDED_)
