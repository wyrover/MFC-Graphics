// MyStatic.cpp : implementation file
//
#include "stdafx.h"
#include "ll.h"
#include "MyStatic.h"
#include "globaldefs.h"
#include "llDlg.h"
#include "InputDialog.h"
#include "SectionDraw.h"
#include "SetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatic

CMyStatic::CMyStatic()
{
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	//{{AFX_MSG_MAP(CMyStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatic message handlers

LRESULT CMyStatic::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(point);
	return HTCLIENT;
}

BOOL CMyStatic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		CancelSelect();
		return TRUE; 
	}
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE; 
	}
	return CStatic::PreTranslateMessage(pMsg);
}

#define IDC_HAND            MAKEINTRESOURCE(32649)
BOOL CMyStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(pWnd);
	return TRUE;
}

void CMyStatic::CancelSelect()
{
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->selelement.RemoveAll();
	if(dlg->createelement)
	{
		delete dlg->createelement;
		dlg->createelement=NULL;
	}
	dlg->curstate=STATE_NONE;
	dlg->Invalidate(FALSE);
	UpdateElements();
}

int CMyStatic::AxisFromPoint(const CPoint& point,const CRect& uisize,int& hor,int& ver)
{
	//find horizontal
	int i,result=0;
	float ratiox=0,ratioy=0;
	int maxx=drawhorizontal->GetAt(drawhorizontal->GetSize()-1);
	int maxy=drawvertical->GetAt(drawvertical->GetSize()-1);
	if(maxx == 0)
		maxx=1;
	if(maxy == 0)
		maxy=1;
	ratiox=(float)(uisize.right-4*margin)/maxx;
	ratioy=(float)(uisize.bottom-4*margin)/maxy;
	for(i=0;i<drawhorizontal->GetSize();i++)
	{
		float relative=drawhorizontal->GetAt(i)*ratiox+2*margin;
		if(point.x >= relative-5 && point.x <= relative+5)
		{
			result |= MATCH_HORIZONTAL;
			hor=i;
			break;
		}
	}
	for(i=0;i<drawvertical->GetSize();i++)
	{
		float relative=uisize.bottom-drawvertical->GetAt(i)*ratioy-2*margin;
		if(point.y >= relative-5 && point.y <= relative+5)
		{
			result |= MATCH_VERTICAL;
			ver=i;
			break;
		}
	}
	return result;
}

void CMyStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rt;
	GetClientRect(&rt);
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	if(BST_CHECKED == ((CButton*)dlg->showdlg->GetDlgItem(IDC_NONE))->GetCheck())
	{//选择元素状态，可以多选
		//判断是否选择外框轴
		if((point.x < 2*margin-5 || point.x > rt.right-2*margin+5) && (point.y > 2*margin+5 && point.y < rt.bottom-2*margin-5))
		{//纵向
			int hor,ver;
			if(MATCH_VERTICAL == AxisFromPoint(point,rt,hor,ver) && ver > 0 && ver < dlg->drawvertical.GetSize()-1)
			{//判断是否是坐标轴
				//ctrl按下则添加，否则重置selelement
				dlg->selelement.RemoveAll();
				dlg->curstate=SELECT_LINE;
				dlg->selectline.Type=ELEMENT_AXISLINE;
				dlg->selectline.IsHorizontal=FALSE;
				dlg->selectline.Index=ver;
				SetCursor(LoadCursor(NULL,IDC_CROSS));
			}
			//判断是否是截面线
			else if(point.y > dlg->jiemianhorizontal-5 && point.y < dlg->jiemianhorizontal+5)
			{
				dlg->selelement.RemoveAll();	
				dlg->curstate=SELECT_LINE;
				dlg->selectline.Type=ELEMENT_SECTIONLINE;
				dlg->selectline.IsHorizontal=FALSE;
				SetCursor(LoadCursor(NULL,IDC_CROSS));
			}
		}
		else if((point.x > 2*margin+5 && point.x < rt.right-2*margin-5) && (point.y < 2*margin-5 || point.y > rt.bottom-2*margin+5))
		{//横向
			int hor,ver;
			if(MATCH_HORIZONTAL == AxisFromPoint(point,rt,hor,ver) && hor > 0 && hor < dlg->drawhorizontal.GetSize()-1)
			{//判断是否是坐标轴
				dlg->selelement.RemoveAll();
				dlg->curstate=SELECT_LINE;
				dlg->selectline.Type=ELEMENT_AXISLINE;
				dlg->selectline.IsHorizontal=TRUE;
				dlg->selectline.Index=hor;
				SetCursor(LoadCursor(NULL,IDC_CROSS));
			}
			//判断是否是截面线
			else if(point.x > dlg->jiemianvertical-5 && point.x < dlg->jiemianvertical+5)
			{
				dlg->selelement.RemoveAll();	
				dlg->curstate=SELECT_LINE;
				dlg->selectline.Type=ELEMENT_SECTIONLINE;
				dlg->selectline.IsHorizontal=TRUE;
				SetCursor(LoadCursor(NULL,IDC_CROSS));
			}
		}
		else if(point.x > 2*margin-5 && point.x < rt.right-2*margin+5 && point.y > 2*margin-5 && point.y < rt.bottom-2*margin+5)
		{//判断是否选择内框元素
			if((SHIFTED & GetKeyState(VK_CONTROL)) && dlg->selelement.GetSize() > 0)
			{//ctrl按下则添加，否则重置selelement
				BASE_ELEMENT* ele=NULL;
				int type=dlg->selelement.GetAt(0)->GetType();
				if(type == ELEMENT_PBEAM || type == ELEMENT_SBEAM)
				{
					if(GetBEAMFromPoint(point,rt,ele) && ele->IsCurrentlyUsed())
						dlg->selelement.Add(ele);
				}
				else if(type == ELEMENT_POLYZONE)
				{
					if(GetPOLYZONEFromPoint(point,rt,ele) && ele->IsCurrentlyUsed())
						dlg->selelement.Add(ele);
				}
				else if(type == ELEMENT_RECTZONE)
				{
					if(GetRECTZONEFromPoint(point,rt,ele) && ele->IsCurrentlyUsed())
						dlg->selelement.Add(ele);
				}

				dlg->Invalidate(FALSE);
				UpdateElements();
			}
			else
			{
				BASE_ELEMENT* old=NULL;
				if(dlg->selelement.GetSize() == 1)
					old=dlg->selelement.GetAt(0);
				dlg->selelement.RemoveAll();
				//找到该元素类型 搜寻优先级：BEAM RECTZONE POLYZONE
				BASE_ELEMENT* ele=NULL;
				if(GetBEAMFromPoint(point,rt,ele) || GetRECTZONEFromPoint(point,rt,ele) || GetPOLYZONEFromPoint(point,rt,ele))
				{
					if(ele != old)
					{
						dlg->selelement.Add(ele);
					}
					dlg->Invalidate(FALSE);
				}
				UpdateElements();
			}
		}
	}
	else if(BST_CHECKED == ((CButton*)dlg->showdlg->GetDlgItem(IDC_PILLAR))->GetCheck())
	{//创建柱
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			for(int i=0;i<dlg->pillars.GetSize();i++)
			{
				PILLAR_PROPERTY* curp=dlg->pillars.GetAt(i);
				if(curp->RelevantWithLine(TRUE,hor) && curp->RelevantWithLine(FALSE,ver))
				{
					curp->SetUsed();
					dlg->Invalidate(FALSE);
					break;
				}
			}
		}
	}
	else if(BST_CHECKED == ((CButton*)dlg->showdlg->GetDlgItem(IDC_PRIMARYBEAM))->GetCheck())
	{//创建主梁
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->curstate == STATE_NONE)
			{
				if(dlg->createelement)
				{
					delete dlg->createelement;
					dlg->createelement=NULL;
				}
				dlg->createelement=new ZONEOUTLINE_PROPERTY(ELEMENT_PBEAM,hor,ver);
				dlg->Fixone=point;
				dlg->curstate=CREATE_PBEAM_STEP1;
			}
		}
	}
	else if(BST_CHECKED == ((CButton*)dlg->showdlg->GetDlgItem(IDC_SECONDARYBEAM))->GetCheck())
	{//创建次梁
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->curstate == STATE_NONE)
			{
				if(dlg->createelement)
					delete dlg->createelement;
				dlg->createelement=new ZONEOUTLINE_PROPERTY(ELEMENT_SBEAM,hor,ver);
				dlg->Fixone=point;
				dlg->curstate=CREATE_SBEAM_STEP1;			
			}
		}
	}
	else if(BST_CHECKED == ((CButton*)dlg->showdlg->GetDlgItem(IDC_POLYZONE))->GetCheck())
	{//创建多边区域
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->curstate == STATE_NONE)
			{
				if(dlg->createelement)
				{
					delete dlg->createelement;
					dlg->createelement=NULL;
				}
				dlg->createelement=new ZONEOUTLINE_PROPERTY(ELEMENT_POLYZONE,hor,ver);
				dlg->Fixone=point;
				dlg->curstate=CREATE_POLYZONE_STEPN;			
			}
			else if(dlg->curstate == CREATE_POLYZONE_STEPN)
			{
				if(dlg->createelement)
					dlg->createelement->AddPoint(hor,ver);
				dlg->Fixone=point;
			}
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CMyStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	CRect rt;
	GetClientRect(&rt);
	if(dlg->curstate == SELECT_LINE)
	{
		dlg->curstate=STATE_NONE;
	}
	else if(dlg->curstate == CREATE_PBEAM_STEP1)
	{
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->createelement && dlg->createelement->GetObjType() == ELEMENT_PBEAM)
			{
				dlg->createelement->AddPoint(hor,ver);
				dlg->beams.Add((BEAM_PROPERTY*)dlg->createelement->ChangeToObjElement(dlg->beamsindex++));
				dlg->Invalidate(FALSE);
			}
		}
		dlg->Moveone=point;
		dlg->curstate=STATE_NONE;
	}
	else if(dlg->curstate == CREATE_SBEAM_STEP1)
	{
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->createelement && dlg->createelement->GetObjType() == ELEMENT_SBEAM)
			{
				dlg->createelement->AddPoint(hor,ver);
				dlg->beams.Add((BEAM_PROPERTY*)dlg->createelement->ChangeToObjElement(dlg->beamsindex++));
				dlg->Invalidate(FALSE);
			}
		}
		dlg->Moveone=point;
		dlg->curstate=STATE_NONE;
	}
	else if(dlg->curstate == CREATE_POLYZONE_STEPN)
	{
		//若闭合，则结束
		int hor,ver;
		if((MATCH_VERTICAL | MATCH_HORIZONTAL) == AxisFromPoint(point,rt,hor,ver))
		{
			if(dlg->createelement && dlg->createelement->GetObjType() == ELEMENT_POLYZONE)
			{
				if(dlg->createelement->IsClosedLine(hor,ver))
				{
					dlg->Moveone=point;
					dlg->curstate=STATE_NONE;
					dlg->polyzones.Add((POLYZONE_PROPERTY*)dlg->createelement->ChangeToObjElement(dlg->polyzonesindex++));
				}
				else
				{
					dlg->createelement->AddPoint(hor,ver);
				}
				dlg->Invalidate(FALSE);
			}
		}
	}

	UpdateElements();
	CStatic::OnLButtonUp(nFlags, point);
}

void CMyStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	CRect rt;
	GetClientRect(&rt);	
	int maxx;
	int maxy;
	float ratiox,ratioy;
	
	HCURSOR arrow=LoadCursor(NULL,IDC_ARROW);
	SetCursor(arrow);

	if(dlg->curstate == SELECT_LINE)
	{//移动坐标轴和截面线  判断位置是否合法，始终把线框在合法范围内，截面线位于坐标线间
		LONG low;//低限位
		LONG high;//高限位
		if(dlg->selectline.IsHorizontal)
		{
			maxx=dlg->drawhorizontal.GetAt(dlg->drawhorizontal.GetSize()-1);
			if(maxx == 0)
				maxx=1;
			ratiox=(float)(rt.right-4*margin)/maxx;
			if(dlg->selectline.Type == ELEMENT_AXISLINE)
			{
				low=LONG(ratiox*dlg->drawhorizontal.GetAt(dlg->selectline.Index-1)+2*margin+2);
				high=LONG(ratiox*dlg->drawhorizontal.GetAt(dlg->selectline.Index+1)+2*margin-2);
				if(point.x < low)
				{
					point.x=low;
				}
				else if(point.x > high)
				{
					point.x=high;
				}
				//调整轴线
				int data=int((float)(point.x-2*margin)/ratiox);
				drawhorizontal->SetAt(dlg->selectline.Index,data);
			}
			else if(dlg->selectline.Type == ELEMENT_SECTIONLINE)
			{
				low=2*margin+2;
				high=rt.right-2*margin-2;
				if(point.x < low)
				{
					point.x=low;
				}
				else if(point.x > high)
				{
					point.x=high;
				}
				dlg->jiemianvertical=point.x;

				CSectionDraw* draw=((CSetDialog*)dlg->showdlg)->SectionVer;
				if(draw)
					draw->Update();
				draw=((CSetDialog*)dlg->showdlg)->SectionHor;
				if(draw)
					draw->Update();
			}
		}
		else
		{
			maxy=dlg->drawvertical.GetAt(dlg->drawvertical.GetSize()-1);
			if(maxy == 0)
				maxy=1;
			ratioy=(float)(rt.bottom-4*margin)/maxy;
			if(dlg->selectline.Type == ELEMENT_AXISLINE)
			{
				high=LONG(rt.bottom-ratioy*dlg->drawvertical.GetAt(dlg->selectline.Index-1)-2*margin+2);
				low=LONG(rt.bottom-ratioy*dlg->drawvertical.GetAt(dlg->selectline.Index+1)-2*margin-2);
				if(point.y < low)
				{
					point.y=low;
				}
				else if(point.y > high)
				{
					point.y=high;
				}
				//调整轴线
				int data=int((float)(rt.bottom-point.y-2*margin)/ratioy);
				drawvertical->SetAt(dlg->selectline.Index,data);
			}
			else if(dlg->selectline.Type == ELEMENT_SECTIONLINE)
			{
				low=2*margin+2;
				high=rt.bottom-2*margin-2;
				if(point.y < low)
				{
					point.y=low;
				}
				else if(point.y > high)
				{
					point.y=high;
				}
				dlg->jiemianhorizontal=point.y;
			}
		}
		dlg->Invalidate(FALSE);
	}
	else if(dlg->curstate == CREATE_PBEAM_STEP1)
	{
		dlg->Moveone=point;
		dlg->Invalidate(FALSE);
	}
	else if(dlg->curstate == CREATE_SBEAM_STEP1)
	{
		dlg->Moveone=point;
		dlg->Invalidate(FALSE);
	}
	else if(dlg->curstate == CREATE_POLYZONE_STEPN)
	{
		dlg->Moveone=point;
		dlg->Invalidate(FALSE);
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CMyStatic::UpdateElements()
{//更新元素列表
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	dlg->showdlg->SendMessage(WM_MYUPDATEELEMENTLIST);
}

BOOL PointInRect(MYRECT& rt,CPoint& pt)
{
	if(pt.x > rt.left-5 && pt.x < rt.right+5 && pt.y > rt.top-5 && pt.y < rt.bottom+5)
		return TRUE;
	return FALSE;
}

BOOL CMyStatic::GetBEAMFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj)
{
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	for(int i=0;i<dlg->beams.GetSize();i++)
	{
		MYRECT rect;
		if(dlg->beams.GetAt(i)->IsCurrentlyUsed() && dlg->beams.GetAt(i)->GetRegion(&rect,dlg->drawhorizontal,dlg->drawvertical))
		{
			PhysicalRECTToUIRECT(rect,uisize);
			if(PointInRect(rect,pt))
			{
				obj=dlg->beams.GetAt(i);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CMyStatic::GetRECTZONEFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj)
{
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	for(int i=0;i<dlg->rectzones.GetSize();i++)
	{
		MYRECT rect;
		if(dlg->rectzones.GetAt(i)->IsCurrentlyUsed() && dlg->rectzones.GetAt(i)->GetRegion(&rect,dlg->drawhorizontal,dlg->drawvertical))
		{
			PhysicalRECTToUIRECT(rect,uisize);
			if(PointInRect(rect,pt))
			{
				obj=dlg->rectzones.GetAt(i);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CMyStatic::GetPOLYZONEFromPoint(CPoint& pt,const CRect& uisize,BASE_ELEMENT*& obj)
{
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	for(int i=0;i<dlg->polyzones.GetSize();i++)
	{
		if(dlg->polyzones.GetAt(i)->IsCurrentlyUsed())
		{
			int maxx=dlg->drawhorizontal.GetAt(dlg->drawhorizontal.GetSize()-1);
			int maxy=dlg->drawvertical.GetAt(dlg->drawvertical.GetSize()-1);
			if(maxx == 0)
				maxx=1;
			if(maxy == 0)
				maxy=1;
			float ratiox=(float)(uisize.right-4*margin)/maxx,ratioy=(float)(uisize.bottom-4*margin)/maxy;
			const CArray<int,int>& axisarray=dlg->polyzones.GetAt(i)->GetData();
			POINT* ptarray=new POINT[axisarray.GetSize()/2];
			for(int j=0;j<axisarray.GetSize()/2;j++)
			{
				ptarray[j].x=LONG(axisarray[j]*ratiox+2*margin);
				ptarray[j].y=LONG(uisize.bottom-axisarray[j+1]*ratioy-2*margin);
			}

			CRgn region;
			region.CreatePolygonRgn(ptarray,axisarray.GetSize()/2,ALTERNATE);
			if(region.PtInRegion(pt))
			{
				obj=dlg->polyzones.GetAt(i);
				return TRUE;
			}
			delete []ptarray;
		}
	}
	return FALSE;
}

void CMyStatic::PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize)
{//rt		src原点位于左下　dst原点位于左上
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	int maxx=dlg->drawhorizontal.GetAt(dlg->drawhorizontal.GetSize()-1);
	int maxy=dlg->drawvertical.GetAt(dlg->drawvertical.GetSize()-1);
	if(maxx == 0)
		maxx=1;
	if(maxy == 0)
		maxy=1;
	float ratiox=(float)(uisize.right-4*margin)/maxx,ratioy=(float)(uisize.bottom-4*margin)/maxy;
	rt.left=int(rt.left*ratiox+2*margin);
	rt.right=int(rt.right*ratiox+2*margin);
	rt.top=int(uisize.bottom-rt.top*ratioy-2*margin);
	rt.bottom=int(uisize.bottom-rt.bottom*ratioy-2*margin);
	if(rt.top > rt.bottom)
	{
		int swap=rt.top;
		rt.top=rt.bottom;
		rt.bottom=swap;
	}
}

void CMyStatic::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	CRect rt;
 	GetClientRect(&rt);
 	Params pa;
 	pa.paramtype=PARAM_MODDISTANCE;
 	int i;
 
 	//判断是否选择外框轴
 	if((point.x < 2*margin-5 || point.x > rt.right-2*margin+5) && (point.y > 2*margin+5 && point.y < rt.bottom-2*margin-5))
 	{//纵向
 		CInputDialog dlg(this);
 		dlg.ppa=&pa;
 		CString temp="";
 		for(i=0;i<drawvertical->GetSize();i++)
 		{
 			temp.Format("%d ",drawvertical[i]);
			pa.param1+=temp;
 		}
 		if(IDOK == dlg.DoModal())
 		{
 			CArray<int,int> newarray;
 			int begin=0,end=0;
 			while((end=pa.param2.Find(' ',begin)) != -1)
 			{
 				temp=pa.param2.Mid(begin,end-begin);
 				newarray.Add(atoi(temp));
 				begin=end+1;
 			}
 			if(newarray.GetSize() == drawvertical->GetSize())
 			{
 				drawvertical->Copy(newarray);
 			}
 		}
 	}
 	else if((point.x > 2*margin+5 && point.x < rt.right-2*margin-5) && (point.y < 2*margin-5 || point.y > rt.bottom-2*margin+5))
 	{//横向
 		CInputDialog dlg(this);
 		dlg.ppa=&pa;
 		CString temp="";
 		for(i=0;i<drawhorizontal->GetSize();i++)
 		{
 			temp.Format("%d ",drawhorizontal[i]);
 			pa.param1+=temp;
 		}
 		if(IDOK == dlg.DoModal())
 		{
 			CArray<int,int> newarray;
 			int begin=0,end=0;
 			while((end=pa.param2.Find(' ',begin)) != -1)
 			{
 				temp=pa.param2.Mid(begin,end-begin);
 				newarray.Add(atoi(temp));
 				begin=end+1;
 			}
 			if(newarray.GetSize() == drawhorizontal->GetSize())
			{
 				drawhorizontal->Copy(newarray);
			}
 		}
 	}
 	m_dlg->Invalidate(FALSE);



	CStatic::OnLButtonDblClk(nFlags, point);
}

void CMyStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CLlDlg* dlg=(CLlDlg*)m_dlg;
	
	if(VK_DELETE == nChar)
	{
		for(int i=0;i<dlg->selelement.GetSize();i++)
		{
			dlg->selelement.GetAt(i)->SetUnused();
		}
	}
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}
