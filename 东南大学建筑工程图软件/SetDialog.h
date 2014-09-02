#if !defined(AFX_SETDIALOG_H__CBC9CBB4_3BF2_48B1_93E3_1B3E87948A36__INCLUDED_)
#define AFX_SETDIALOG_H__CBC9CBB4_3BF2_48B1_93E3_1B3E87948A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDialog.h : header file
//
#include <afxtempl.h>
#include "MyStatic.h"
#include "llDlg.h"
#include "ElementListBox.h"
#include "SectionSet.h"
#include "SectionDraw.h"
/////////////////////////////////////////////////////////////////////////////
// CSetDialog dialog

class CSetDialog : public CDialog
{
// Construction
public:
	CSetDialog(CArray<int,int>&,CArray<int,int>&,CWnd* pParent = NULL);   // standard constructor
	CArray<int,int>& drawhorizontal;
	CArray<int,int>& drawvertical;
	CLlDlg* m_dlg;
	CSectionSet* m_secset;
	CSectionDraw* SectionHor;
	CSectionDraw* SectionVer;
	void AddElements(BOOL IsHorizontal);//增加轴线更新元素
	void DeleteRelateElements(BOOL IsHorizontal);//删除轴线更新元素
// Dialog Data
	//{{AFX_DATA(CSetDialog)
	enum { IDD = IDD_SHOW_DIALOG };
	CListBox	m_propertylist;
	CElementListBox	m_elementlist;
	CListBox	m_listdesty;
	CListBox	m_listdestx;
	CListBox	m_listdefault;
	CString	m_spec;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDialog)
	afx_msg void OnAdddefault();
	afx_msg void OnAddspec();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNone();
	afx_msg void OnPillar();
	afx_msg void OnPrimarybeam();
	afx_msg void OnSecondarybeam();
	afx_msg void OnPolyzone();
	afx_msg LRESULT OnUpdateElement(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDblclkPropertylist();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDIALOG_H__CBC9CBB4_3BF2_48B1_93E3_1B3E87948A36__INCLUDED_)
