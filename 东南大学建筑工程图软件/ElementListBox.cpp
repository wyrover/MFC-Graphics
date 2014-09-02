// ElementListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "ElementListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElementListBox

CElementListBox::CElementListBox()
{
}

CElementListBox::~CElementListBox()
{
}


BEGIN_MESSAGE_MAP(CElementListBox, CListBox)
	//{{AFX_MSG_MAP(CElementListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementListBox message handlers

BOOL CElementListBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE; 
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 
	return CListBox::PreTranslateMessage(pMsg);
}

void CElementListBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	int selcount=GetSelCount();
	if(selcount < 0)
		return;
	if(selcount == 0)
	{
		propertyshow->ResetContent();
	}
	else if(selcount == 1)
	{
		int cursel;
		GetSelItems(1,&cursel);
		switch(GetItemData(cursel)>>24)
		{
			case ELEMENT_PBEAM:
			case ELEMENT_SBEAM:
				m_dlg->selelement.Add(m_dlg->beams.GetAt(GetItemData(cursel)&0xFFFFFF));
				m_dlg->Invalidate(FALSE);
				break;

			case ELEMENT_RECTZONE:
				m_dlg->selelement.Add(m_dlg->rectzones.GetAt(GetItemData(cursel)&0xFFFFFF));
				m_dlg->Invalidate(FALSE);
				break;

			case ELEMENT_POLYZONE:
				m_dlg->selelement.Add(m_dlg->polyzones.GetAt(GetItemData(cursel)&0xFFFFFF));
				m_dlg->Invalidate(FALSE);
				break;

			default:
				break;
		}
	}
	else
	{
		CArray<int,int> curselarray;
		curselarray.SetSize(selcount);
		GetSelItems(selcount,curselarray.GetData());
		m_dlg->selelement.RemoveAll();

		for(int i=selcount-1;i>=0;i--)
		{
			switch(GetItemData(curselarray[i])>>24)
			{
				case ELEMENT_PBEAM:
				case ELEMENT_SBEAM:
					m_dlg->selelement.Add(m_dlg->beams.GetAt(GetItemData(curselarray[i])&0xFFFFFF));
					break;
					
				case ELEMENT_RECTZONE:
					m_dlg->selelement.Add(m_dlg->rectzones.GetAt(GetItemData(curselarray[i])&0xFFFFFF));
					break;
					
				case ELEMENT_POLYZONE:
					m_dlg->selelement.Add(m_dlg->polyzones.GetAt(GetItemData(curselarray[i])&0xFFFFFF));
					break;
					
				default:
					break;
			}
		}
		m_dlg->Invalidate(FALSE);
		m_dlg->showdlg->SendMessage(WM_MYUPDATEELEMENTLIST);
	}
}
