// ll.h : main header file for the LL application
//

#if !defined(AFX_LL_H__97E9F4BD_F422_4088_A691_C4AA659B0F67__INCLUDED_)
#define AFX_LL_H__97E9F4BD_F422_4088_A691_C4AA659B0F67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLlApp:
// See ll.cpp for the implementation of this class
//

class CLlApp : public CWinApp
{
public:
	CLlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LL_H__97E9F4BD_F422_4088_A691_C4AA659B0F67__INCLUDED_)
