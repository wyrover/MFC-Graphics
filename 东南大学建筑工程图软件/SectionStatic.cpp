// SectionStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "SectionStatic.h"
#include "SectionDraw.h"
#include "globaldefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionStatic

CSectionStatic::CSectionStatic()
{
}

CSectionStatic::~CSectionStatic()
{
}


BEGIN_MESSAGE_MAP(CSectionStatic, CStatic)
	//{{AFX_MSG_MAP(CSectionStatic)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionStatic message handlers

LRESULT CSectionStatic::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(point);
	return HTCLIENT;
}

void CSectionStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CSectionDraw* draw=(CSectionDraw*)SectionDraw;
	//Ότµ¶³Ε
	draw->Fixone=point;
	draw->curstate=CREATE_SCISSORSLINE;

	CStatic::OnLButtonDown(nFlags, point);
}

void CSectionStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CSectionDraw* draw=(CSectionDraw*)SectionDraw;
	//Ότµ¶³Ε
	if(draw->curstate == CREATE_SCISSORSLINE)
	{
		SCISSORSLINE* line=new SCISSORSLINE(draw->Fixone.x,draw->Fixone.y,draw->Moveone.x,draw->Moveone.y);
		draw->Scissorsline.Add(line);
		draw->Invalidate(FALSE);
	}
	draw->curstate=STATE_NONE;

	CStatic::OnLButtonUp(nFlags, point);
}

void CSectionStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CSectionDraw* draw=(CSectionDraw*)SectionDraw;
	//Ότµ¶³Ε
	if(draw->curstate == CREATE_SCISSORSLINE)
	{
		draw->Moveone=point;
		draw->Invalidate(FALSE);
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

BOOL CSectionStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(pWnd);
	return TRUE;
}

BOOL CSectionStatic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		return TRUE; 
	}
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE; 
	}
	return CStatic::PreTranslateMessage(pMsg);
}
