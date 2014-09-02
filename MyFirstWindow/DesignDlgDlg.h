// DesignDlgDlg.h : header file
//

#if !defined(AFX_DESIGNDLGDLG_H__29A91CF5_9E3B_4D5B_9264_7E43C297DDE5__INCLUDED_)
#define AFX_DESIGNDLGDLG_H__29A91CF5_9E3B_4D5B_9264_7E43C297DDE5__INCLUDED_

#include "Defs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDesignDlgDlg dialog

class CDesignDlgDlg : public CDialog
{
// Construction
public:
	ButtonState m_BtnState;
	void DrawTitlebarText();
	COLORREF m_clText;
	CString m_csText;
	int m_nBorderCY;
	int m_nBorderCX;
	int m_nTitileBarCY;
	void DrawDialog(int nFlag);
	CDesignDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignDlgDlg)
	enum { IDD = IDD_DESIGNDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDesignDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNDLGDLG_H__29A91CF5_9E3B_4D5B_9264_7E43C297DDE5__INCLUDED_)
