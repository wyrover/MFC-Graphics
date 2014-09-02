// SetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "SetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <afxtempl.h>
#include "InputDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CSetDialog dialog


CSetDialog::CSetDialog(CArray<int,int>& mydrawhorizontal,CArray<int,int>& mydrawvertical,CWnd* pParent /*=NULL*/)
	: CDialog(CSetDialog::IDD, pParent),drawhorizontal(mydrawhorizontal),drawvertical(mydrawvertical)
{
	//{{AFX_DATA_INIT(CSetDialog)
	m_spec = _T("");
	//}}AFX_DATA_INIT
}


void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDialog)
	DDX_Control(pDX, IDC_PROPERTYLIST, m_propertylist);
	DDX_Control(pDX, IDC_ELEMENTLIST, m_elementlist);
	DDX_Control(pDX, IDC_LISTDESTY, m_listdesty);
	DDX_Control(pDX, IDC_LISTDESTX, m_listdestx);
	DDX_Control(pDX, IDC_LISTDEFAULT, m_listdefault);
	DDX_Text(pDX, IDC_SPEC, m_spec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialog)
	//{{AFX_MSG_MAP(CSetDialog)
	ON_BN_CLICKED(IDC_ADDDEFAULT, OnAdddefault)
	ON_BN_CLICKED(IDC_ADDSPEC, OnAddspec)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NONE, OnNone)
	ON_BN_CLICKED(IDC_PILLAR, OnPillar)
	ON_BN_CLICKED(IDC_PRIMARYBEAM, OnPrimarybeam)
	ON_BN_CLICKED(IDC_SECONDARYBEAM, OnSecondarybeam)
	ON_BN_CLICKED(IDC_POLYZONE, OnPolyzone)
	ON_MESSAGE(WM_MYUPDATEELEMENTLIST,OnUpdateElement)
	ON_LBN_DBLCLK(IDC_PROPERTYLIST, OnDblclkPropertylist)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDialog message handlers

void CSetDialog::OnAdddefault() 
{
	UpdateData(TRUE);
	int nIndex=m_listdefault.GetCurSel();
	CString str;
	if((nIndex != LB_ERR))
	{
		m_listdefault.GetText(nIndex,str.GetBuffer(m_listdefault.GetTextLen(nIndex)));
		if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SETX))->GetCheck())
		{
			m_listdestx.AddString(str);
			drawhorizontal.Add(drawhorizontal.GetAt(drawhorizontal.GetSize()-1)+atoi(str));
			AddElements(TRUE);
		}
		else
		{
			m_listdesty.AddString(str);
			drawvertical.Add(drawvertical.GetAt(drawvertical.GetSize()-1)+atoi(str));
			AddElements(FALSE);
		}
		str.ReleaseBuffer();
	}
	m_dlg->Invalidate(FALSE);
}

void CSetDialog::OnAddspec() 
{
	UpdateData(TRUE);
	m_spec=m_spec.SpanIncluding("0123456789");
	if((m_spec.GetLength() > 0))
	{
		if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SETX))->GetCheck())
		{
			m_listdestx.AddString(m_spec);
			drawhorizontal.Add(drawhorizontal.GetAt(drawhorizontal.GetSize()-1)+atoi(m_spec));
			AddElements(TRUE);
		}
		else
		{
			m_listdesty.AddString(m_spec);
			drawvertical.Add(drawvertical.GetAt(drawvertical.GetSize()-1)+atoi(m_spec));
			AddElements(FALSE);
		}
	}
	m_dlg->Invalidate(FALSE);
}

void CSetDialog::OnDelete() 
{//一次删除一个，且删除最后面元素
	UpdateData(TRUE);
	CArray<int,int> boxsel;
	CString str;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SETX))->GetCheck())
	{
		if(m_listdestx.GetCount())
		{
			m_listdestx.DeleteString(m_listdestx.GetCount()-1);
			drawhorizontal.RemoveAt(drawhorizontal.GetSize()-1);
		}
		DeleteRelateElements(TRUE);	
	}
	else
	{
		if(m_listdesty.GetCount())
		{
			m_listdesty.DeleteString(m_listdesty.GetCount()-1);
			drawvertical.RemoveAt(drawvertical.GetSize()-1);
		}
		DeleteRelateElements(FALSE);		
	}
	m_dlg->Invalidate(FALSE);	
}

void CSetDialog::AddElements(BOOL IsHorizontal)//添加最后一个坐标轴以后更新其他元素
{
	//增加新柱
	if(IsHorizontal)
	{
		for(int i=0;i<drawvertical.GetSize();i++)
		{
			m_dlg->pillars.Add(new PILLAR_PROPERTY(ELEMENT_MPILLAR,m_dlg->pillarsindex++,drawhorizontal.GetSize()-1,i));
		}
	}
	else
	{
		for(int i=0;i<drawhorizontal.GetSize();i++)
		{
			m_dlg->pillars.Add(new PILLAR_PROPERTY(ELEMENT_MPILLAR,m_dlg->pillarsindex++,i,drawvertical.GetSize()-1));
		}
	}
	//添加内部区域
	if(drawhorizontal.GetSize() > 1 && drawvertical.GetSize() > 1)
	{
		int n=drawhorizontal.GetSize(),m=drawvertical.GetSize();
		if(IsHorizontal)
		{
			for(int i=1;i<m;i++)
			{
				m_dlg->rectzones.Add(new RECTZONE_PROPERTY(ELEMENT_RECTZONE,m_dlg->rectzonesindex++,n-2,i-1,n-1,i-1,n-1,i,n-2,i));
			}
		}
		else
		{
			for(int i=1;i<n;i++)
			{
				m_dlg->rectzones.Add(new RECTZONE_PROPERTY(ELEMENT_RECTZONE,m_dlg->rectzonesindex++,i-1,m-2,i,m-2,i,m-1,i-1,m-2));
			}
		}
	}
	m_dlg->Invalidate(FALSE);
}

void CSetDialog::DeleteRelateElements(BOOL IsHorizontal)//删除最后一条线以后更新其他元素
{
	int i;
	int n=drawhorizontal.GetSize(),m=drawvertical.GetSize();//代表已删掉的线
	for(i=0;i<m_dlg->pillars.GetSize();i++)
	{//用户添加柱不会导致柱数量增加，仅仅设置used=TRUE;删除仅仅设置used=FALSE
		PILLAR_PROPERTY* curpillar=m_dlg->pillars[i];
		if(curpillar->RelevantWithLine(IsHorizontal,IsHorizontal?n:m))
			curpillar->SetUnused();
	}
	for(i=0;i<m_dlg->beams.GetSize();i++)
	{
		BEAM_PROPERTY* curbeam=m_dlg->beams[i];
		if(curbeam->RelevantWithLine(IsHorizontal,IsHorizontal?n:m))
			curbeam->SetUnused();
	}
	for(i=0;i<m_dlg->rectzones.GetSize();i++)
	{
		RECTZONE_PROPERTY* currectzone=m_dlg->rectzones[i];
		if(currectzone->RelevantWithLine(IsHorizontal,IsHorizontal?n:m))
			currectzone->SetUnused();
	}
	for(i=0;i<m_dlg->polyzones.GetSize();i++)
	{
		POLYZONE_PROPERTY* curpolyzone=m_dlg->polyzones[i];
		if(curpolyzone->RelevantWithLine(IsHorizontal,IsHorizontal?n:m))
			curpolyzone->SetUnused();
	}	
	m_dlg->Invalidate(FALSE);
}

BOOL CSetDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listdefault.InsertString(0,"300");
	m_listdefault.InsertString(1,"600");
	m_listdefault.InsertString(2,"900");
	m_listdefault.InsertString(3,"1200");
	m_listdefault.InsertString(4,"1500");
	m_listdefault.SetCurSel(0);
	((CButton*)GetDlgItem(IDC_SETX))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_SETY))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_NONE))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_PILLAR))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_PRIMARYBEAM))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_SECONDARYBEAM))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_POLYZONE))->SetCheck(BST_UNCHECKED);

	m_elementlist.propertyshow=&m_propertylist;
	m_elementlist.m_dlg=m_dlg;
	m_secset=new CSectionSet;
	m_secset->Create(IDD_SETSECTION,this);
	m_secset->ShowWindow(SW_SHOWNORMAL);
	SectionHor=new CSectionDraw(NULL,m_secset);
	SectionHor->m_dlg=m_dlg;
	SectionHor->Create(IDD_LL_DIALOG_SECTION);
	SectionHor->ShowWindow(SW_SHOWNORMAL);
	SectionVer=new CSectionDraw(NULL,m_secset);
	SectionVer->m_dlg=m_dlg;
	SectionVer->Create(IDD_LL_DIALOG_SECTION);
	SectionVer->ShowWindow(SW_SHOWNORMAL);
	m_secset->SectionHor=SectionHor;
	m_secset->SectionVer=SectionVer;
	SectionHor->m_dlg=m_dlg;
	SectionVer->m_dlg=m_dlg;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox("要退出请关闭平面图");
}

BOOL CSetDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return FALSE; 
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE;  
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetDialog::OnNone() 
{
	// TODO: Add your control notification handler code here
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->curstate=STATE_NONE;
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}
}

void CSetDialog::OnPillar() 
{
	// TODO: Add your control notification handler code here
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->curstate=STATE_NONE;
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}	
}

void CSetDialog::OnPrimarybeam() 
{
	// TODO: Add your control notification handler code here
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->curstate=STATE_NONE;
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}
}

void CSetDialog::OnSecondarybeam() 
{
	// TODO: Add your control notification handler code here
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->curstate=STATE_NONE;
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}
}

void CSetDialog::OnPolyzone() 
{
	// TODO: Add your control notification handler code here
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->curstate=STATE_NONE;
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}
}

LRESULT CSetDialog::OnUpdateElement(WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	//刷新列表，更新选中项
	m_elementlist.ResetContent();
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	int i;
	char buf[1024];
	for(i=0;i<dlg->beams.GetSize();i++)
	{
		BEAM_PROPERTY* curb=dlg->beams[i];
		if(curb->IsCurrentlyUsed())
		{
			if(curb->GetType() == ELEMENT_PBEAM)
			{
				sprintf(buf,"主梁 %d",curb->GetIndex());
				int index=m_elementlist.AddString(buf);
				m_elementlist.SetItemData(index,(curb->GetType()<<24)+curb->GetIndex());
			}
			else
			{
				sprintf(buf,"次梁 %d",curb->GetIndex());
				int index=m_elementlist.AddString(buf);
				m_elementlist.SetItemData(index,(curb->GetType()<<24)+curb->GetIndex());				
			}
		}
	}
	for(i=0;i<dlg->rectzones.GetSize();i++)
	{
		RECTZONE_PROPERTY* curr=dlg->rectzones[i];
		if(curr->IsCurrentlyUsed())
		{
			sprintf(buf,"板 %d",curr->GetIndex());
			int index=m_elementlist.AddString(buf);
			m_elementlist.SetItemData(index,(curr->GetType()<<24)+curr->GetIndex());
		}	
	}
	for(i=0;i<dlg->polyzones.GetSize();i++)
	{
		POLYZONE_PROPERTY* curp=dlg->polyzones[i];
		if(curp->IsCurrentlyUsed())
		{
			sprintf(buf,"板 %d",curp->GetIndex());
			int index=m_elementlist.AddString(buf);
			m_elementlist.SetItemData(index,(curp->GetType()<<24)+curp->GetIndex());
		}	
	}
	m_elementlist.SelItemRange(FALSE,0,m_elementlist.GetCount()-1);
	for(i=0;i<m_elementlist.GetCount();i++)
	{
		m_elementlist.SetSel(i,FALSE);
	}
	m_propertylist.ResetContent();
	CArray<SELECTABLE*,SELECTABLE*> selobj;
	for(i=0;i<dlg->selelement.GetSize();i++)
	{
		BASE_ELEMENT* curb=dlg->selelement[i];
		if(curb->IsCurrentlyUsed())
		{
			for(int j=0;j<m_elementlist.GetCount();j++)
			{
				if((int)m_elementlist.GetItemData(j) == (curb->GetType()<<24)+curb->GetIndex())
				{
					m_elementlist.SetSel(j,TRUE);
					selobj.Add((SELECTABLE*)curb);
				}
			}
		}
	}

	int index;
	CString cur;
	if(selobj.GetSize() == 1)
	{
		if(selobj[0]->GetType() == ELEMENT_PBEAM || selobj[0]->GetType() == ELEMENT_SBEAM)
		{
			index=m_propertylist.AddString("梁截面参数"+selobj[0]->TemplateName);
			m_propertylist.SetItemData(index,PARAM_MODBEAMSECTION);
		}
		else
		{
			index=m_propertylist.AddString("板截面参数："+selobj[0]->TemplateName);
			m_propertylist.SetItemData(index,PARAM_MODBOARDSECTION);
		}
		cur.Format("层高：%d",selobj[0]->LayerHeight);
		index=m_propertylist.AddString(cur);
		m_propertylist.SetItemData(index,PARAM_MODLAYERHEIGHT);
		if(selobj[0]->GetType() == ELEMENT_PBEAM || selobj[0]->GetType() == ELEMENT_SBEAM)
		{
			index=m_propertylist.AddString("杆位置阵列：");
			//............................待添加
			m_propertylist.SetItemData(index,PARAM_MODBEAMRODARRAY);
		}
	}
	else if(selobj.GetSize() > 1)
	{
		ELEMENTTYPE type=selobj[0]->GetType();
		BOOL sametype=TRUE;
		for(int i=1;i<selobj.GetSize();i++)
		{
			if(selobj[i]->GetType() != type)
			{
				sametype=FALSE;
				break;
			}
		}
		if(sametype)
		{
			BOOL sameparam;
			if(type == ELEMENT_PBEAM || type == ELEMENT_SBEAM)
			{
				CString templatename=selobj[0]->TemplateName;
				sameparam=TRUE;
				int i;
				for(i=1;i<selobj.GetSize();i++)
				{
					if(selobj[i]->TemplateName != templatename)
					{
						sameparam=FALSE;
						break;
					}
				}
				if(sameparam)
				{
					index=m_propertylist.AddString("梁截面参数："+selobj[0]->TemplateName);
				}
				else
				{
					index=m_propertylist.AddString("梁截面参数：不等");
				}
				m_propertylist.SetItemData(index,PARAM_MODBEAMSECTION);

				int height=selobj[0]->LayerHeight;
				sameparam=TRUE;
				for(i=1;i<selobj.GetSize();i++)
				{
					if(selobj[i]->LayerHeight != height)
					{
						sameparam=FALSE;
						break;
					}
				}
				if(sameparam)
				{
					cur.Format("层高：%d",selobj[0]->LayerHeight);
					index=m_propertylist.AddString(cur);			
				}
				else
				{
					index=m_propertylist.AddString("层高：不等");
				}
				m_propertylist.SetItemData(index,PARAM_MODLAYERHEIGHT);
			}
			else
			{
				index=m_propertylist.AddString("板截面参数："+selobj[0]->TemplateName);
				m_propertylist.SetItemData(index,PARAM_MODBOARDSECTION);
				cur.Format("层高：%d",selobj[0]->LayerHeight);
				index=m_propertylist.AddString(cur);
				m_propertylist.SetItemData(index,PARAM_MODLAYERHEIGHT);
			}
		}
		else
		{
			int height=selobj[0]->LayerHeight;
			BOOL sameheight=TRUE;
			for(int i=1;i<selobj.GetSize();i++)
			{
				if(selobj[i]->LayerHeight != height)
				{
					sameheight=FALSE;
					break;
				}
			}
			if(sameheight)
			{
				cur.Format("层高：%d",height);
				index=m_propertylist.AddString(cur);
			}
			else
			{
				index=m_propertylist.AddString("层高：不等");		
			}
			m_propertylist.SetItemData(index,PARAM_MODLAYERHEIGHT);
		}
	}

	return 0;
}


void CSetDialog::OnDblclkPropertylist() 
{//修改属性
	// TODO: Add your control notification handler code here
	int index=m_propertylist.GetCurSel();
	if(index<0)
		return;
	Params pa;
	switch(m_propertylist.GetItemData(index))
	{
		case PARAM_MODBEAMSECTION:
			{
				pa.paramtype=PARAM_MODBEAMSECTION;
				CInputDialog dlg(this);
				dlg.ppa=&pa;
				m_propertylist.GetText(index,pa.param1);
				for(int i=0;i<m_secset->m_beamtemp.GetItemCount();i++)
				{
					pa.chooselist.Add(m_secset->m_beamtemp.GetItemText(i,0));
				}
				if(IDOK == dlg.DoModal())
				{
					for(int i=0;i<m_secset->m_beamtemp.GetItemCount();i++)
					{
						if(pa.param2 == m_secset->m_beamtemp.GetItemText(i,0))
						{
							archtemplate* curd=(archtemplate*)m_secset->m_beamtemp.GetItemData(i);
							if(curd->type != ELEMENT_BEAM)
								return;
							//枚举选中元素中的beam
							int j;
							CArray<BEAM_PROPERTY*,BEAM_PROPERTY*> sel;
							for(j=0;j<m_dlg->selelement.GetSize();j++)
							{
								ELEMENTTYPE type=m_dlg->selelement[j]->GetType();
								if(type == ELEMENT_PBEAM || type == ELEMENT_SBEAM)
									sel.Add((BEAM_PROPERTY*)m_dlg->selelement[j]);
							}
							for(j=0;j<sel.GetSize();j++)
							{
								sel[j]->BeamHeight=curd->tem.beam.beamheight;
								sel[j]->BeamWidth=curd->tem.beam.beamwidth;
								sel[j]->TemplateName=curd->tempname;
								sel[j]->Initialized=TRUE;
							}
							break;
						}
					}
				}
			}
			break;

		case PARAM_MODBOARDSECTION:
			{
				pa.paramtype=PARAM_MODBOARDSECTION;
				CInputDialog dlg(this);
				dlg.ppa=&pa;
				m_propertylist.GetText(index,pa.param1);
				for(int i=0;i<m_secset->m_boardtemp.GetItemCount();i++)
				{
					pa.chooselist.Add(m_secset->m_boardtemp.GetItemText(i,0));
				}
				if(IDOK == dlg.DoModal())
				{
					for(int i=0;i<m_secset->m_boardtemp.GetItemCount();i++)
					{
						if(pa.param2 == m_secset->m_boardtemp.GetItemText(i,0))
						{
							archtemplate* curd=(archtemplate*)m_secset->m_boardtemp.GetItemData(i);
							if(curd->type != ELEMENT_BOARD)
								return;
							//枚举选中元素中的board
							int j;
							CArray<SELECTABLE_BOARD*,SELECTABLE_BOARD*> sel;
							for(j=0;j<m_dlg->selelement.GetSize();j++)
							{
								ELEMENTTYPE type=m_dlg->selelement[j]->GetType();
								if(type == ELEMENT_RECTZONE || type == ELEMENT_POLYZONE)
									sel.Add((SELECTABLE_BOARD*)m_dlg->selelement[j]);
							}
							for(j=0;j<sel.GetSize();j++)
							{
								sel[j]->Thickness=curd->tem.board.boardthick;
								sel[j]->TemplateName=curd->tempname;
								sel[j]->Initialized=TRUE;
							}
							break;
						}
					}
				}
			}
			break;

		case PARAM_MODLAYERHEIGHT:
			{
				pa.paramtype=PARAM_MODLAYERHEIGHT;
				CInputDialog dlg(this);
				dlg.ppa=&pa;
				m_propertylist.GetText(index,pa.param1);

				if(IDOK == dlg.DoModal())
				{
					int j;
					CArray<SELECTABLE*,SELECTABLE*> sel;
					for(j=0;j<m_dlg->selelement.GetSize();j++)
					{
						ELEMENTTYPE type=m_dlg->selelement[j]->GetType();
						if(type == ELEMENT_PBEAM || type == ELEMENT_SBEAM || type == ELEMENT_RECTZONE || type == ELEMENT_POLYZONE)
							sel.Add((SELECTABLE*)m_dlg->selelement[j]);
					}
					for(j=0;j<sel.GetSize();j++)
					{
						sel[j]->LayerHeight=atoi(pa.param2);
					}
					break;
				}
			}
			break;

		case PARAM_MODBEAMRODARRAY:
			if(m_elementlist.GetSelCount() == 1)
			{
				//修改梁下杆阵列
			}
			break;

		default:
			return;
	}
	SectionHor->Update();
	SectionVer->Update();
}

void CSetDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_secset)
		delete m_secset;
	if(SectionHor)
		delete SectionHor;
	if(SectionVer)
		delete SectionVer;
}
