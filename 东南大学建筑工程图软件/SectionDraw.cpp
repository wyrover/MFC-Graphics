// SectionDraw.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "SectionDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionDraw dialog


CSectionDraw::CSectionDraw(CWnd* pParent /*=NULL*/,CSectionSet* pSet)
	: CDialog(CSectionDraw::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectionDraw)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dlg=NULL;
	m_setdlg=pSet;
	tempdc=NULL;

}


void CSectionDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionDraw)
	DDX_Control(pDX, IDC_DRAW, m_sectiondraw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSectionDraw, CDialog)
	//{{AFX_MSG_MAP(CSectionDraw)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionDraw message handlers

void CSectionDraw::Update()
{
	Invalidate(FALSE);
}

void CSectionDraw::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
}

BOOL CSectionDraw::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sectiondraw.dlg=m_dlg;
	CRect rect;
	m_sectiondraw.GetClientRect(&rect);
	tempdc=m_sectiondraw.GetDC();
	MyDC.CreateCompatibleDC(tempdc);
	bmp.CreateCompatibleBitmap(tempdc,rect.right,rect.bottom);
	oldbmp=MyDC.SelectObject(&bmp);
	CBrush brush(RGB(255,255,255));
	m_sectiondraw.SectionDraw=this;

	if(m_dlg->drawhorizontal.GetSize() > 1)
	{
		m_setdlg->m_heipoledis.Format("%d",int(m_dlg->drawhorizontal[m_dlg->drawhorizontal.GetSize()-1]/10));
		m_setdlg->UpdateData(FALSE);
	}
	curstate=STATE_NONE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSectionDraw::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	MyDC.SelectObject(oldbmp);
	bmp.DeleteObject();
	MyDC.DeleteDC();
}

void CSectionDraw::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	if(::IsWindow(m_sectiondraw.m_hWnd))
	{
		m_sectiondraw.MoveWindow(&rt);
		MyDC.SelectObject(oldbmp);
		bmp.DeleteObject();
		m_sectiondraw.GetClientRect(&rt);
		bmp.CreateCompatibleBitmap(tempdc,rt.right,rt.bottom);
		MyDC.SelectObject(bmp);
		CBrush brush(RGB(255,255,255));
		MyDC.FillRect(&rt,&brush);
	}
}

void CSectionDraw::PhysicalPointToUIPoint(POINT* pt,const RECT& uirect,int maxx,int maxy,int x,int y)
{
	float ratiox=(float)(uirect.right-4*margin)/maxx,ratioy=(float)(uirect.bottom-4*margin)/maxy;
	pt->x=long(x*ratiox+2*margin);
	pt->y=long(uirect.bottom-y*ratioy-2*margin);
}

void CSectionDraw::PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize,int maxx,int maxy)
{
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

void CSectionDraw::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	UpdateData(TRUE);
	// TODO: Add your message handler code here
	int i,j;
	CArray<BEAM_PROPERTY*,BEAM_PROPERTY*> beamarray;//梁阵列，该面没有为NULL，元素个数等同beamwidtharray
	CArray<int,int> beamwidtharray;//该层梁宽属性，如果该行（列）没有则改到该列第一个，按轴线分布，每个轴线必有一个，与轴线数量同
	CArray<int,int> beamwidtharrayext;//不能为0，画杆用
	CArray<SELECTABLE_BOARD*,SELECTABLE_BOARD*> boardarray;//轴线划分方块所归属的板，可以相同，可以为NULL，比轴线数量小1，和间距数量相等

	for(i=0;i<beamarray.GetSize();i++)
	{
		beamarray[i]=NULL;
	}
	for(i=0;i<beamwidtharray.GetSize();i++)
	{
		beamwidtharray[i]=0;
	}
	for(i=0;i<boardarray.GetSize();i++)
	{
		boardarray[i]=NULL;
	}

	//由截面线位置确定轴线方块区簇
	if(m_dlg->drawhorizontal.GetSize() <= 1 || m_dlg->drawvertical.GetSize() <= 1)
		return;

	POINT form;
	m_dlg->UIPointToPhysicalPoint(&form,m_dlg->jiemianvertical,m_dlg->jiemianhorizontal);//HORIZONTAL用到form.y VERTICAL用到form.x

	if(DrawType == HORIZONTAL)
	{
		//得到截线对应轴线的位置
		for(i=1;i<m_dlg->drawvertical.GetSize();i++)
		{
			if(form.x >= m_dlg->drawvertical[i])
				break;
		}
		
		beamarray.SetSize(m_dlg->drawhorizontal.GetSize());
		beamwidtharray.SetSize(m_dlg->drawhorizontal.GetSize());
		boardarray.SetSize(m_dlg->drawhorizontal.GetSize()-1);

		for(j=0;j<m_dlg->beams.GetSize();j++)
		{
			if(m_dlg->beams[j]->IsCurrentlyUsed() && m_dlg->beams[j]->Initialized && m_dlg->beams[j]->IsHorizontal)
			{
				beamwidtharrayext.SetAt(m_dlg->beams[j]->GetCommonLine(TRUE),m_dlg->beams[j]->BeamWidth);
				if(m_dlg->beams[j]->RelevantWithLine(FALSE,i-1,i))
				{
					TRACE("\t%d",m_dlg->beams[j]->BeamWidth);
					beamarray.SetAt(m_dlg->beams[j]->GetCommonLine(TRUE),m_dlg->beams[j]);
					beamwidtharray.SetAt(m_dlg->beams[j]->GetCommonLine(TRUE),m_dlg->beams[j]->BeamWidth);
				}
			}
		}
		//先寻找rectzone使得后找的polyzone覆盖rectzone
		for(j=0;j<m_dlg->rectzones.GetSize();j++)
		{
			if(m_dlg->rectzones[j]->IsCurrentlyUsed() && m_dlg->rectzones[j]->Initialized)
			{
				int mi=i-1,ma=i;
				m_dlg->rectzones[j]->GetOneSideRangeSides(TRUE,mi,ma);//正常ma=mi+1
				boardarray.SetAt(mi,m_dlg->rectzones[j]);
			}
		}
		for(j=0;j<m_dlg->polyzones.GetSize();j++)
		{
			if(m_dlg->polyzones[j]->IsCurrentlyUsed() && m_dlg->polyzones[j]->Initialized)
			{
				int mi=i-1,ma=i;
				m_dlg->polyzones[j]->GetOneSideRangeSides(TRUE,mi,ma);
				for(i=mi;i<ma;i++)
				{
					boardarray.SetAt(i,m_dlg->polyzones[j]);
				}
			}
		}
	}
	else
	{
		//得到截线对应轴线的位置
		for(i=1;i<m_dlg->drawhorizontal.GetSize();i++)
		{
			if(form.y < m_dlg->drawhorizontal[i])
				break;
		}
		
		beamarray.SetSize(m_dlg->drawvertical.GetSize());
		beamwidtharray.SetSize(m_dlg->drawvertical.GetSize());
		boardarray.SetSize(m_dlg->drawvertical.GetSize()-1);
		
		for(int k=0;k<m_dlg->beams.GetSize();k++)
		{
			if(m_dlg->beams[k])
			{
				TRACE("beams[%d]:beamwidth=%d,beamheight=%d,layerheight=%d\n",k,m_dlg->beams[k]->BeamWidth,m_dlg->beams[k]->BeamHeight,m_dlg->beams[k]->LayerHeight);
			}
			else
			{
				TRACE("beams[%d]=0\n",k);
			}
		}

		for(j=0;j<m_dlg->beams.GetSize();j++)
		{
			if(m_dlg->beams[j]->IsCurrentlyUsed() && m_dlg->beams[j]->Initialized && !m_dlg->beams[j]->IsHorizontal)
			{
				TRACE("\tindex:%d,beamwidth=%d",j,m_dlg->beams[j]->BeamWidth);
				beamwidtharray.SetAt(m_dlg->beams[j]->GetCommonLine(FALSE),m_dlg->beams[j]->BeamWidth);
				if(m_dlg->beams[j]->RelevantWithLine(TRUE,i-1,i))
				{
					beamarray.SetAt(m_dlg->beams[j]->GetCommonLine(FALSE),m_dlg->beams[j]);
					beamwidtharray.SetAt(m_dlg->beams[j]->GetCommonLine(FALSE),m_dlg->beams[j]->BeamWidth);
				}
			}
		}
		//先寻找rectzone使得后找的polyzone覆盖rectzone
		for(j=0;j<m_dlg->rectzones.GetSize();j++)
		{
			if(m_dlg->rectzones[j]->IsCurrentlyUsed() && m_dlg->rectzones[j]->Initialized)
			{
				int mi=i-1,ma=i;
				m_dlg->rectzones[j]->GetOneSideRangeSides(FALSE,mi,ma);//正常ma=mi+1
				boardarray.SetAt(mi,m_dlg->rectzones[j]);		
			}
		}
		for(j=0;j<m_dlg->polyzones.GetSize();j++)
		{
			if(m_dlg->polyzones[j]->IsCurrentlyUsed() && m_dlg->polyzones[j]->Initialized)
			{
				int mi=i-1,ma=i;
				m_dlg->polyzones[j]->GetOneSideRangeSides(FALSE,mi,ma);
				for(i=mi;i<ma;i++)
				{
					boardarray.SetAt(i,m_dlg->polyzones[j]);
				}
			}
		}
	}

	float RedunthickLeft=0;//左侧多余处的板厚
	float RedunlayerheightLeft=0;//左侧多余处的层高
	float Redunwidth=0;//两端多余处长度=TotalWidth/20
	float TotalWidth=0;//两端轴线距离*10/9
	float TotalHeight=0;//高度=最大层高
	float Maxbeamheight=0;//最大梁高
	float ratiox,ratioy;
	float temp1,temp2;
	for(i=0;i<boardarray.GetSize();i++)
	{
		if(boardarray[i] && boardarray[i]->Thickness)
		{
			RedunthickLeft=(float)boardarray[i]->Thickness;
			break;
		}
	}
	for(i=0;i<beamarray.GetSize();i++)
	{
		if(beamarray[i] && beamarray[i]->LayerHeight)
		{
			RedunlayerheightLeft=(float)beamarray[i]->LayerHeight;
			break;
		}
	}
	for(i=0;i<beamarray.GetSize();i++)
	{
		if(beamarray[i] && TotalHeight < beamarray[i]->LayerHeight)
		{
			TotalHeight=(float)beamarray[i]->LayerHeight;
		}
		if(beamarray[i] && Maxbeamheight < beamarray[i]->BeamHeight)
		{
			Maxbeamheight=(float)beamarray[i]->BeamHeight;
		}
	}
	for(i=0;i<boardarray.GetSize();i++)
	{
		if(boardarray[i] && TotalHeight < boardarray[i]->LayerHeight)
		{
			TotalHeight=(float)boardarray[i]->LayerHeight;
		}
	}
	if(DrawType == HORIZONTAL)
	{
		if(m_dlg->drawhorizontal.GetSize()>1)
			TotalWidth=(float)m_dlg->drawhorizontal[m_dlg->drawhorizontal.GetSize()-1]*(float)10/9;
	}
	else
	{
		if(m_dlg->drawvertical.GetSize()>1)
			TotalWidth=(float)m_dlg->drawvertical[m_dlg->drawvertical.GetSize()-1]*(float)10/9;
	}
	Redunwidth=TotalWidth/20;//一边一个


	RECT rt;
	m_sectiondraw.GetClientRect(&rt);
	ratiox=(float)(rt.right-4*margin)/TotalWidth;
	ratioy=(float)(rt.bottom-4*margin)/TotalHeight;//用于长度转换
	CBrush celling;
	CPen pen(PS_SOLID,1,RGB(0,0,0));
	celling.CreateHatchBrush(HS_BDIAGONAL,RGB(0,0,0));

	CDC* curdc=&dc;

	if(DrawType == HORIZONTAL)
	{
		temp1=RedunthickLeft;
		temp2=RedunlayerheightLeft;
		int x=2*margin,y=2*margin;
		curdc->SelectObject(celling);
		curdc->SelectObject(pen);
		curdc->BeginPath();
		curdc->MoveTo(2*margin,2*margin);
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[0]/2));
		curdc->LineTo(x,y);
		//左数第一根梁
		y=int(2*margin+ratioy*(beamarray[0]?beamarray[0]->BeamHeight:RedunthickLeft));
		curdc->LineTo(x,y);
		x+=int(ratiox*(beamwidtharray[0]));
		curdc->LineTo(x,y);
		for(i=0;i<boardarray.GetSize();i++)
		{
			//画板
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x=int(ratiox*(m_dlg->drawhorizontal[i+1]+Redunwidth-(float)beamwidtharray[i+1]/2));
			curdc->LineTo(x,y);
			//画梁
			if(beamarray[i+1] && beamarray[i+1]->BeamHeight)
				RedunthickLeft=(float)beamarray[i+1]->BeamHeight;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
			curdc->LineTo(x,y);
		}

		//补全右接板
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[i]/2));
		curdc->LineTo(x,y);
		curdc->LineTo(x,2*margin);
		curdc->LineTo(2*margin,2*margin);
		curdc->EndPath();
		curdc->FillPath();
		//描边
		RedunthickLeft=temp1;
		RedunlayerheightLeft=temp2;
		curdc->MoveTo(2*margin,2*margin);
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[0]/2));
		curdc->LineTo(x,y);


		
		//左数第一根梁
		y=int(2*margin+ratioy*(beamarray[0]?beamarray[0]->BeamHeight:RedunthickLeft));
		curdc->LineTo(x,y);
		x+=int(ratiox*(beamwidtharray[0]));
		curdc->LineTo(x,y);
		for(i=0;i<boardarray.GetSize();i++)
		{
			//画板
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x=int(2*margin+ratiox*(m_dlg->drawhorizontal[i+1]+Redunwidth-(float)beamwidtharray[i+1]/2));
			curdc->LineTo(x,y);
			//画梁
			if(beamarray[i+1] && beamarray[i+1]->BeamHeight)
				RedunthickLeft=(float)beamarray[i+1]->BeamHeight;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
			curdc->LineTo(x,y);
		}
		
		//补全右接板
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[i]/2));
		curdc->LineTo(x,y);
		curdc->LineTo(x,2*margin);
		curdc->LineTo(2*margin,2*margin);

		//按层高画底板+画横杆
		RedunthickLeft=temp1;
		RedunlayerheightLeft=temp2;
		x=2*margin;
		y=int(2*margin+ratioy*RedunlayerheightLeft);
		curdc->Rectangle(x,y,int(x+ratiox*(Redunwidth+(float)beamwidtharray[0]/2)),y+5);
		x+=int(ratiox*(Redunwidth+(float)beamwidtharray[0]/2));
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->Rectangle(x,y,int(ratiox*(Redunwidth-(float)beamwidtharray[i+1]/2+m_dlg->drawhorizontal[i+1])),y+5);
			x=int(ratiox*(Redunwidth-(float)beamwidtharray[i+1]/2+m_dlg->drawhorizontal[i+1]));
			curdc->Rectangle(x,y,int(x+ratiox*(float)beamwidtharray[i+1]),y+5);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
		}
		curdc->Rectangle(x,y,int(x+ratiox*Redunwidth),y+5);

		//画横杆
		int heipoledis=atoi(m_setdlg->m_heipoledis);
		CArray<int,int> LayerHeightArray;
		int num=int((TotalHeight-Maxbeamheight-200)/heipoledis);
		for(i=num;i>=0;i--)
		{
			LayerHeightArray.Add(int(TotalHeight-i*heipoledis-200));
		}
		for(i=0;i<beamarray.GetSize();i++)
		{
			if(beamarray[i] && beamarray[i]->LayerHeight)
			{
				for(j=1;j<LayerHeightArray.GetSize();j++)
				{//找到下界
					if(LayerHeightArray[j]>beamarray[i]->LayerHeight)
						break;
				}
				
				for(int k=0;k<j;k++)
				{
					if(i == 0)//补全左侧
					{
						y=int(2*margin+ratioy*LayerHeightArray[k]);
						curdc->MoveTo(2*margin,y-3);
						curdc->LineTo(int(2*margin+ratiox*(Redunwidth+m_dlg->drawhorizontal[i]-(float)beamarray[i]->BeamWidth/2)),y-3);
						curdc->MoveTo(2*margin,y+3);
						curdc->LineTo(int(2*margin+ratiox*(Redunwidth+m_dlg->drawhorizontal[i]-(float)beamarray[i]->BeamWidth/2)),y+3);
					}
					x=int(2*margin+ratiox*(Redunwidth+m_dlg->drawhorizontal[i]-(float)beamarray[i]->BeamWidth/2));
					y=int(2*margin+ratioy*LayerHeightArray[k]);
					curdc->MoveTo(x,y-3);
					curdc->LineTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y-3);
					curdc->MoveTo(x,y+3);
					curdc->LineTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y+3);
					if(i == beamarray.GetSize()-1)
					{
						y=int(2*margin+ratioy*LayerHeightArray[k]);
						curdc->MoveTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y-3);
						curdc->LineTo(int(x+ratiox*Redunwidth),y-3);
						curdc->MoveTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y+3);
						curdc->LineTo(int(x+ratiox*Redunwidth),y+3);
					}
				}
			}
		}
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i] && boardarray[i]->LayerHeight)
			{
				for(j=1;j<LayerHeightArray.GetSize();j++)
				{//找到下界
					if(LayerHeightArray[j]>boardarray[i]->LayerHeight)
						break;
				}
				
				for(int k=0;k<j;k++)
				{
					int x1,x2;
					x1=int(2*margin+ratiox*(Redunwidth+m_dlg->drawhorizontal[i]+(float)beamwidtharray[k]/2));
					x2=int(2*margin+ratiox*(Redunwidth+m_dlg->drawhorizontal[i+1]-(float)beamwidtharray[k+1]/2));
					y=int(2*margin+ratioy*LayerHeightArray[k]);
					curdc->MoveTo(x1,y-3);
					curdc->LineTo(x2,y-3);
					curdc->MoveTo(x1,y+3);
					curdc->LineTo(x2,y+3);
				}
			}
		}

		int horpoledis=atoi(m_setdlg->m_horpoledis);
		m_setdlg->m_horpoledis.Format("%d",(int)Redunwidth);
		//画板下竖杆
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i])
			{
				CArray<float,float> relative;//存储杆绝对位置
				float length=m_dlg->drawhorizontal[i+1]-m_dlg->drawhorizontal[i]-(float)beamwidtharray[i]/2-(float)beamwidtharray[i+1]/2;//取中间部分
				if(length>300 && length<600)
				{
					relative.Add(Redunwidth+m_dlg->drawhorizontal[i]+(float)beamwidtharray[i]/2+length/2);
				}
				else if(length >= 600)
				{
					int num=int((float)(length-500)/horpoledis);
					relative.Add(Redunwidth+m_dlg->drawhorizontal[i]+(float)beamwidtharray[i]/2+250);//首尾250
					relative.Add(Redunwidth+m_dlg->drawhorizontal[i+1]-(float)beamwidtharray[i+1]/2-250);
					for(j=1;j<=num;j++)
					{
						relative.Add(Redunwidth+m_dlg->drawhorizontal[i]+(float)beamwidtharray[i]/2+250+j*horpoledis);
					}
				}
				for(j=0;j<relative.GetSize();j++)
				{
					curdc->MoveTo(int(2*margin+ratiox*relative[j]-5),int(2*margin+ratioy*boardarray[i]->Thickness));
					curdc->LineTo(int(2*margin+ratiox*relative[j]-5),int(2*margin+ratioy*boardarray[i]->LayerHeight));
					curdc->MoveTo(int(2*margin+ratiox*relative[j]+5),int(2*margin+ratioy*boardarray[i]->Thickness));
					curdc->LineTo(int(2*margin+ratiox*relative[j]+5),int(2*margin+ratioy*boardarray[i]->LayerHeight));
				} 
			}
		}
		

		//画梁下竖杆
	}
	else
	{
		RECT rtt;
		m_sectiondraw.GetClientRect(&rtt);
		CBrush bb;
		bb.CreateSolidBrush(RGB(255,255,255));
		curdc->FillRect(&rtt,&bb);
		UpdateWindow();

		for(i=0;i<beamarray.GetSize();i++)
		{
			if(beamarray[i])
				TRACE("\nbeamarray[%d]:height=%d,width=%d,layerheight=%d\n",i,beamarray[i]->BeamHeight,beamarray[i]->BeamWidth,beamarray[i]->LayerHeight);
			else
				TRACE("beamarray[%d]=0\n",i);
		}
		for(i=0;i<beamwidtharray.GetSize();i++)
		{
			if(beamarray[i])
				TRACE("beamwidtharray[%d]=%d\n",i,beamwidtharray[i]);
			else
				TRACE("beamwidtharray[%d]=0\n",i);
		}
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i])
				TRACE("boardarray[%d]:thickness=%d,layerheight=%d\n",i,boardarray[i]->Thickness,boardarray[i]->LayerHeight);
			else
				TRACE("boardarray[%d]=0\n",i);
		}

		temp1=RedunthickLeft;
		temp2=RedunlayerheightLeft;
		int x=2*margin,y=2*margin;

		curdc->SelectObject(celling);
		curdc->SelectObject(pen);
//		curdc->BeginPath();
		curdc->MoveTo(2*margin,2*margin);

		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[0]/2));
		curdc->LineTo(x,y);
		//左数第一根梁
		y=int(2*margin+ratioy*(beamarray[0]?beamarray[0]->BeamHeight:RedunthickLeft));
		curdc->LineTo(x,y);
		x+=int(ratiox*(beamwidtharray[0]));
		curdc->LineTo(x,y);
		for(i=0;i<boardarray.GetSize();i++)
		{
			//画板
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			TRACE("%d,%d,%d\n",m_dlg->drawvertical[i+1],beamwidtharray[i+1],RedunthickLeft);
			x=int(2*margin+ratiox*(m_dlg->drawvertical[i+1]+Redunwidth-(float)beamwidtharray[i+1]/2));
			curdc->LineTo(x,y);
			//画梁
			if(beamarray[i+1] && beamarray[i+1]->BeamHeight)
				RedunthickLeft=(float)beamarray[i+1]->BeamHeight;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
			curdc->LineTo(x,y);
		}

		
		//补全右接板
		RedunthickLeft=temp1;
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=(float)beamwidtharray[i];
		curdc->LineTo(x,y);
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[i]));
		curdc->LineTo(x,y);
		curdc->LineTo(x,2*margin);
		curdc->LineTo(2*margin,2*margin);
//		curdc->EndPath();
//		curdc->FillPath();

		/*
//错误斜线		
// 		//描边
		RedunthickLeft=temp1;
		RedunlayerheightLeft=temp2;
		curdc->MoveTo(2*margin,2*margin);
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[0]/2));
		curdc->LineTo(x,y);
		//左数第一根梁
		y=int(2*margin+ratioy*(beamarray[0]?beamarray[0]->BeamHeight:RedunthickLeft));
		curdc->LineTo(x,y);
		x+=int(ratiox*(beamwidtharray[0]));
		curdc->LineTo(x,y);
		for(i=0;i<boardarray.GetSize();i++)
		{
			//画板
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x=int(ratiox*(m_dlg->drawvertical[i+1]+Redunwidth-(float)beamwidtharray[i+1]/2));
			curdc->LineTo(x,y);
			//画梁
			if(beamarray[i+1] && beamarray[i+1]->BeamHeight)
				RedunthickLeft=(float)beamarray[i+1]->BeamHeight;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->LineTo(x,y);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
			curdc->LineTo(x,y);
		}
		
		
		//补全右接板
		y=int(2*margin+ratioy*RedunthickLeft);
		curdc->LineTo(x,y);
		x+=int(ratiox*(Redunwidth-(float)beamwidtharray[i]/2));
		curdc->LineTo(x,y);
		curdc->LineTo(x,2*margin);
		curdc->LineTo(2*margin,2*margin);

		//按层高画底板+画横杆
		RedunthickLeft=temp1;
		RedunlayerheightLeft=temp2;
		x=2*margin;
		y=int(2*margin+ratioy*RedunlayerheightLeft);
		curdc->Rectangle(x,y,int(x+ratiox*(Redunwidth+(float)beamwidtharray[0]/2)),y+5);
		x+=int(ratiox*(Redunwidth+(float)beamwidtharray[0]/2));
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i] && boardarray[i]->Thickness)
				RedunthickLeft=(float)boardarray[i]->Thickness;
			y=int(2*margin+ratioy*RedunthickLeft);
			curdc->Rectangle(x,y,int(ratiox*(Redunwidth-(float)beamwidtharray[i+1]/2+m_dlg->drawvertical[i+1])),y+5);
			x=int(ratiox*(Redunwidth-(float)beamwidtharray[i+1]/2+m_dlg->drawvertical[i+1]));
			curdc->Rectangle(x,y,int(x+ratiox*(float)beamwidtharray[i+1]),y+5);
			x+=int(ratiox*(float)beamwidtharray[i+1]);
		}
		curdc->Rectangle(x,y,int(x+ratiox*Redunwidth),y+5);

		//画横杆
		int heipoledis=atoi(m_setdlg->m_heipoledis);
		CArray<int,int> LayerHeightArray;
		int num=int((TotalHeight-Maxbeamheight-200)/heipoledis);
		for(i=num;i>=0;i--)
		{
			LayerHeightArray.Add(int(TotalHeight-i*heipoledis-200));
		}
		for(i=0;i<beamarray.GetSize();i++)
		{
			if(beamarray[i] && beamarray[i]->LayerHeight)
			{
				for(j=1;j<LayerHeightArray.GetSize();j++)
				{//找到下界
					if(LayerHeightArray[j]>beamarray[i]->LayerHeight)
						break;
				}
				
				for(int k=0;k<j-1;k++)
				{
					if(i == 0)//补全左侧
					{
						y=int(2*margin+ratioy*LayerHeightArray[k]);
						curdc->MoveTo(2*margin,y-3);
						curdc->LineTo(int(2*margin+ratiox*(Redunwidth+m_dlg->drawvertical[i]-(float)beamarray[i]->BeamWidth/2)),y-3);
						curdc->MoveTo(2*margin,y+3);
						curdc->LineTo(int(2*margin+ratiox*(Redunwidth+m_dlg->drawvertical[i]-(float)beamarray[i]->BeamWidth/2)),y+3);
					}
					x=int(2*margin+ratiox*(Redunwidth+m_dlg->drawvertical[i]-(float)beamarray[i]->BeamWidth/2));
					y=int(2*margin+ratioy*LayerHeightArray[k]);
					curdc->MoveTo(x,y-3);
					curdc->LineTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y-3);
					curdc->MoveTo(x,y+3);
					curdc->LineTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y+3);
					if(i == beamarray.GetSize()-1)
					{
						y=int(2*margin+ratioy*LayerHeightArray[k]);
						curdc->MoveTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y-3);
						curdc->LineTo(int(x+ratiox*Redunwidth),y-3);
						curdc->MoveTo(int(x+ratiox*(float)beamarray[i]->BeamWidth),y+3);
						curdc->LineTo(int(x+ratiox*Redunwidth),y+3);
					}
				}
			}
		}
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i] && boardarray[i]->LayerHeight)
			{
				for(j=1;j<LayerHeightArray.GetSize();j++)
				{//找到下界
					if(LayerHeightArray[j]>boardarray[i]->LayerHeight)
						break;
				}
				
				for(int k=0;k<j;k++)
				{
					int x1,x2;
					x1=int(2*margin+ratiox*(Redunwidth+m_dlg->drawvertical[i]+(float)beamwidtharray[k]/2));
					x2=int(2*margin+ratiox*(Redunwidth+m_dlg->drawvertical[i+1]-(float)beamwidtharray[k+1]/2));
					y=int(2*margin+ratioy*LayerHeightArray[k]);
					curdc->MoveTo(x1,y-3);
					curdc->LineTo(x2,y-3);
					curdc->MoveTo(x1,y+3);
					curdc->LineTo(x2,y+3);
				}
			}
		}

		int verpoledis=atoi(m_setdlg->m_verpoledis);
		m_setdlg->m_verpoledis.Format("%d",(int)Redunwidth);
		//画板下竖杆
		for(i=0;i<boardarray.GetSize();i++)
		{
			if(boardarray[i])
			{
				CArray<float,float> relative;//存储杆绝对位置
				float length=m_dlg->drawvertical[i+1]-m_dlg->drawvertical[i]-(float)beamwidtharray[i]/2-(float)beamwidtharray[i+1]/2;//取中间部分
				if(length>300 && length<600)
				{
					relative.Add(Redunwidth+m_dlg->drawvertical[i]+(float)beamwidtharray[i]/2+length/2);
				}
				else if(length >= 600)
				{
					int num=int((float)(length-500)/verpoledis);
					relative.Add(Redunwidth+m_dlg->drawvertical[i]+(float)beamwidtharray[i]/2+250);//首尾250
					relative.Add(Redunwidth+m_dlg->drawvertical[i+1]-(float)beamwidtharray[i+1]/2-250);
					for(j=1;j<=num;j++)
					{
						relative.Add(Redunwidth+m_dlg->drawvertical[i]+(float)beamwidtharray[i]/2+250+j*verpoledis);
					}
				}
				for(j=0;j<relative.GetSize();j++)
				{
					curdc->MoveTo(int(2*margin+ratiox*relative[j]-5),int(2*margin+ratioy*boardarray[i]->Thickness));
					curdc->LineTo(int(2*margin+ratiox*relative[j]-5),int(2*margin+ratioy*boardarray[i]->LayerHeight));
					curdc->MoveTo(int(2*margin+ratiox*relative[j]+5),int(2*margin+ratioy*boardarray[i]->Thickness));
					curdc->LineTo(int(2*margin+ratiox*relative[j]+5),int(2*margin+ratioy*boardarray[i]->LayerHeight));
				}
			}
		}
		*/

		//画梁下竖杆

	}

	//画剪刀撑
	CPen penclip(PS_SOLID,2,RGB(222,0,0));
	curdc->SelectObject(penclip);
	curdc->MoveTo(Fixone);
	curdc->LineTo(Moveone);
	for(i=0;i<Scissorsline.GetSize();i++)
	{
		curdc->MoveTo(Scissorsline[i]->beginx,Scissorsline[i]->beginy);
		curdc->LineTo(Scissorsline[i]->endx,Scissorsline[i]->endy);
	}
	

//	dc.BitBlt(0,0,rt.right,rt.bottom,&MyDC,0,0,SRCCOPY);
	// Do not call CDialog::OnPaint() for painting messages
	
}
