#if !defined(AFX_SECTIONSET_H__50C2185D_F888_4E7C_BAB6_36BEDCA57D92__INCLUDED_)
#define AFX_SECTIONSET_H__50C2185D_F888_4E7C_BAB6_36BEDCA57D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectionSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectionSet dialog

class CSectionSet : public CDialog
{
// Construction
public:
	CSectionSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectionSet)
	enum { IDD = IDD_SETSECTION };
	CListCtrl	m_boardtemp;
	CListCtrl	m_beamtemp;
	CString	m_horpoledis;
	CString	m_heipoledis;
	CString	m_verpoledis;
	//}}AFX_DATA
	CDialog* SectionVer;
	CDialog* SectionHor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionSet)
	afx_msg void OnAddbeamparam();
	afx_msg void OnAddboardparam();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnApplychange();
	afx_msg void OnDestroy();
	afx_msg void OnRemoveclip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONSET_H__50C2185D_F888_4E7C_BAB6_36BEDCA57D92__INCLUDED_)
