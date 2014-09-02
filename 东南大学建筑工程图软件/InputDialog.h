#if !defined(AFX_INPUTDIALOG_H__C12C8F3A_2CEE_43E8_90C5_A8E0F441E135__INCLUDED_)
#define AFX_INPUTDIALOG_H__C12C8F3A_2CEE_43E8_90C5_A8E0F441E135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDialog.h : header file
//

#include "globaldefs.h"

/////////////////////////////////////////////////////////////////////////////
// CInputDialog dialog

class CInputDialog : public CDialog
{
// Construction
public:
	CInputDialog(CWnd* pParent = NULL);   // standard constructor
	Params* ppa;
// Dialog Data
	//{{AFX_DATA(CInputDialog)
	enum { IDD = IDD_INPUT };
	CComboBox	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDIALOG_H__C12C8F3A_2CEE_43E8_90C5_A8E0F441E135__INCLUDED_)
