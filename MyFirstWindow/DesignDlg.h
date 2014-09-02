// DesignDlg.h : main header file for the DESIGNDLG application
//

#if !defined(AFX_DESIGNDLG_H__0B416EDF_66B4_4A0C_ACEB_E67F251FAB66__INCLUDED_)
#define AFX_DESIGNDLG_H__0B416EDF_66B4_4A0C_ACEB_E67F251FAB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDesignDlgApp:
// See DesignDlg.cpp for the implementation of this class
//

class CDesignDlgApp : public CWinApp
{
public:
	CDesignDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDesignDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNDLG_H__0B416EDF_66B4_4A0C_ACEB_E67F251FAB66__INCLUDED_)
