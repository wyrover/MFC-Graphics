// drawsort.h : main header file for the DRAWSORT application
//

#if !defined(AFX_DRAWSORT_H__5DB65568_67D7_4BE3_99BA_223585AABC42__INCLUDED_)
#define AFX_DRAWSORT_H__5DB65568_67D7_4BE3_99BA_223585AABC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawsortApp:
// See drawsort.cpp for the implementation of this class
//

class CDrawsortApp : public CWinApp
{
public:
	CDrawsortApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawsortApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDrawsortApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWSORT_H__5DB65568_67D7_4BE3_99BA_223585AABC42__INCLUDED_)
