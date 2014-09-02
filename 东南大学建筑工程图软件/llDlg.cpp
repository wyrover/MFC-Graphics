// llDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "llDlg.h"
#include "SetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLlDlg dialog

CLlDlg::CLlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLlDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLlDlg)
	DDX_Control(pDX, IDC_DRAW, m_draw);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLlDlg, CDialog)
	//{{AFX_MSG_MAP(CLlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLlDlg message handlers

BOOL CLlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	this->SetWindowText("平面图");

	showdlg=new CSetDialog(drawhorizontal,drawvertical);
	((CSetDialog*)showdlg)->m_dlg=this;

	showdlg->Create(IDD_SHOW_DIALOG);
	showdlg->ShowWindow(SW_SHOWNORMAL);
	curstate=STATE_NONE;
	pillarsindex=0;
	beamsindex=0;
	rectzonesindex=0;
	polyzonesindex=0;
	jiemianhorizontal=2*margin+10;
	jiemianvertical=2*margin+10;
	drawvertical.Add(0);
	drawhorizontal.Add(0);
	pillars.Add(new PILLAR_PROPERTY(ELEMENT_MPILLAR,pillarsindex++,0,0));
	m_draw.drawhorizontal=&drawhorizontal;
	m_draw.drawvertical=&drawvertical;
	createelement=NULL;

	CRect rect;
	m_draw.GetClientRect(&rect);
	tempdc=m_draw.GetDC();
	MyDC.CreateCompatibleDC(tempdc);
	bmp.CreateCompatibleBitmap(tempdc,rect.right,rect.bottom);
	oldbmp=MyDC.SelectObject(&bmp);
	CBrush brush(RGB(255,255,255));
	MyDC.FillRect(&rect,&brush);
	m_draw.m_dlg=this;
	m_draw.m_paintDC=&MyDC;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		if(drawhorizontal.GetSize() < 2 || drawvertical.GetSize() < 2)
			return;
		RECT rt;
		m_draw.GetClientRect(&rt);
		CDC* pdc=&MyDC;
		CBrush brush(RGB(255,255,255));
		MyDC.FillRect(&rt,&brush);
		int i;
		//只绘制IsCurrentlyUsed()
		//第一图层绘制顺序 rectzones > polyzones > beams > pillars
		//第二图层绘制顺序 对于selelement元素 执行polyzone = rectzone = beam  > createelement
		//第三图层绘制顺序 drawhorizontal > drawvertical > jiemianvertical > jiemianhorizontal
		CPen nullpen(PS_NULL,1,RGB(255,255,255));
		pdc->SelectObject(nullpen);
		CBrush polybrushwithproperty;
		polybrushwithproperty.CreateSolidBrush(RGB(245,200,179));
		CBrush polybrushwithoutproperty;
		polybrushwithoutproperty.CreateSolidBrush(RGB(245,200,255));
		CBrush rectbrushwithproperty;
		rectbrushwithproperty.CreateSolidBrush(RGB(245,222,179));
		CBrush rectbrushwithoutproperty;
		rectbrushwithoutproperty.CreateSolidBrush(RGB(245,222,255));

		//第一层rectzones
		for(i=0;i<rectzones.GetSize();i++)
		{
			if(rectzones[i]->IsCurrentlyUsed())
			{
				pdc->SelectObject(rectzones[i]->Initialized?rectbrushwithproperty:rectbrushwithoutproperty);
				MYRECT currect;
				rectzones[i]->GetRegion(&currect,drawhorizontal,drawvertical);
				PhysicalRECTToUIRECT(currect,rt);
				pdc->Rectangle(currect.left,currect.top,currect.right,currect.bottom);
			}
		}

		//第一层polyzones
		for(i=0;i<polyzones.GetSize();i++)
		{
			if(polyzones[i]->IsCurrentlyUsed())
			{
				pdc->SelectObject(polyzones[i]->Initialized?polybrushwithproperty:polybrushwithoutproperty);
				const CArray<int,int>& LineArray=polyzones[i]->GetData();
				POINT* pts=new POINT[LineArray.GetSize()/2];
				for(int j=0;j<LineArray.GetSize()/2;j++)
				{
					 PhysicalPointToUIPoint(pts+j,rt,drawhorizontal[LineArray[2*j]],drawvertical[LineArray[2*j+1]]);
				}
				CRgn rgn;
				rgn.CreatePolygonRgn(pts,LineArray.GetSize()/2,ALTERNATE);
				pdc->PaintRgn(&rgn);
				delete []pts;
			}
		}

		//第一层beams
		CPen primarybeamwithproperty(PS_SOLID,1,RGB(160,82,45));
		CPen primarybeamwithoutproperty(PS_SOLID,3,RGB(160,82,45));
		CPen secondarybeamwithproperty(PS_SOLID,1,RGB(244,164,96));
		CPen secondarybeamwithoutproperty(PS_SOLID,3,RGB(244,164,96));
		for(i=0;i<beams.GetSize();i++)
		{
			if(beams[i]->IsCurrentlyUsed())
			{
				if(beams[i]->GetType() == ELEMENT_PBEAM)
				{
					if(beams[i]->Initialized)
						pdc->SelectObject(primarybeamwithproperty);
					else
						pdc->SelectObject(primarybeamwithoutproperty);
				}
				else
				{
					if(beams[i]->Initialized)
						pdc->SelectObject(secondarybeamwithproperty);
					else
						pdc->SelectObject(secondarybeamwithoutproperty);
				}
				MYRECT currect;
				beams[i]->GetRegion(&currect,drawhorizontal,drawvertical);
				PhysicalRECTToUIRECT(currect,rt);
				if(beams[i]->IsHorizontal)
				{
					pdc->MoveTo(currect.left,currect.top-5);
					pdc->LineTo(currect.right,currect.top-5);
					pdc->MoveTo(currect.left,currect.bottom+5);
					pdc->LineTo(currect.right,currect.bottom+5);
				}
				else
				{
					pdc->MoveTo(currect.left-5,currect.top);
					pdc->LineTo(currect.left-5,currect.bottom);
					pdc->MoveTo(currect.right+5,currect.top);
					pdc->LineTo(currect.right+5,currect.bottom);
				}
			}
		}

		//第一层pillars
		CBrush pillarbrush;
		CPen pillarpen(PS_SOLID,1,RGB(128,42,42));
		pillarbrush.CreateSolidBrush(RGB(128,42,42));
		pdc->SelectObject(pillarpen);
		pdc->SelectObject(pillarbrush);
		MYRECT currect;
		for(i=0;i<pillars.GetSize();i++)
		{
			if(pillars[i]->IsCurrentlyUsed())
			{
				pillars[i]->GetRegion(&currect,drawhorizontal,drawvertical);
				PhysicalRECTToUIRECT(currect,rt);
				pdc->Rectangle(currect.left-10,currect.top-10,currect.right+10,currect.bottom+10);
			}
		}

		//第二图层polyzone = rectzone = beam
		CBrush selectelebrush;
		CPen selectelepen(PS_SOLID,5,RGB(8,46,84));
		selectelebrush.CreateSolidBrush(RGB(8,46,84));
		pdc->SelectObject(selectelebrush);
		pdc->SelectObject(selectelepen);
		for(i=0;i<selelement.GetSize();i++)
		{
			int type=selelement[i]->GetType();
			if(ELEMENT_POLYZONE == type)
			{
				const CArray<int,int>& temp=((POLYZONE_PROPERTY*)selelement[i])->GetData();
				int arraysize=temp.GetSize()/2;
				POINT* pts=new POINT[arraysize];
				for(int j=0;j<arraysize;j++)
				{
					PhysicalPointToUIPoint(pts+j,rt,drawhorizontal[temp[2*j]],drawvertical[temp[2*j+1]]);
				}
				CRgn rgn;
				rgn.CreatePolygonRgn(pts,arraysize,ALTERNATE);
				pdc->PaintRgn(&rgn);
				delete []pts;
			}
			else if(ELEMENT_RECTZONE == type)
			{
				MYRECT currect;
				((RECTZONE_PROPERTY*)selelement[i])->GetRegion(&currect,drawhorizontal,drawvertical);
				PhysicalRECTToUIRECT(currect,rt);
				pdc->Rectangle(currect.left,currect.top,currect.right,currect.bottom);
			}
			else if(ELEMENT_PBEAM == type || ELEMENT_SBEAM == type)
			{
				MYRECT currect;
				((BEAM_PROPERTY*)selelement[i])->GetRegion(&currect,drawhorizontal,drawvertical);
				PhysicalRECTToUIRECT(currect,rt);
				if(((BEAM_PROPERTY*)selelement[i])->IsHorizontal)
				{
					pdc->MoveTo(currect.left,currect.top-5);
					pdc->LineTo(currect.right,currect.top-5);
					pdc->MoveTo(currect.left,currect.bottom+5);
					pdc->LineTo(currect.right,currect.bottom+5);
				}
				else
				{
					pdc->MoveTo(currect.left-5,currect.top);
					pdc->LineTo(currect.left-5,currect.bottom);
					pdc->MoveTo(currect.right+5,currect.top);
					pdc->LineTo(currect.right+5,currect.bottom);
				}
			}
		}

		//第二图层createelement
		if(curstate == CREATE_PBEAM_STEP1 || curstate == CREATE_SBEAM_STEP1 || curstate == CREATE_POLYZONE_STEPN)
		{
			CPen createele(PS_SOLID,5,RGB(8,46,84));
			pdc->SelectObject(createele);
			const CArray<int,int>& temp=createelement->GetData();
			POINT pttemp;
			if(temp.GetSize()>=4)
			{
				PhysicalPointToUIPoint(&pttemp,rt,drawhorizontal[temp[0]],drawvertical[temp[1]]);
				pdc->MoveTo(pttemp);
				for(i=1;i<temp.GetSize()/2;i++)
				{
					PhysicalPointToUIPoint(&pttemp,rt,drawhorizontal[temp[2*i]],drawvertical[temp[2*i+1]]);
					pdc->LineTo(pttemp);
				}
			}
			pdc->MoveTo(Fixone);
			pdc->LineTo(Moveone);
		}

		//第三层drawhorizontal
		CPen axispen(PS_DASHDOT,1,RGB(255,0,0));
		CPen kedu(PS_SOLID,1,RGB(0,0,0));
		pdc->SelectObject(axispen);
		int sum,relative;
		
		int last;
		sum=drawhorizontal.GetAt(drawhorizontal.GetSize()-1);
		pdc->MoveTo(margin*2,margin);
		pdc->LineTo(margin*2,rt.bottom-margin);
		last=margin*2;
		for(i=1;i<drawhorizontal.GetSize();i++)
		{
			int curdata=drawhorizontal.GetAt(i)-drawhorizontal.GetAt(i-1);
			relative = int((rt.right-margin*4)*(float)drawhorizontal.GetAt(i)/sum);
			pdc->MoveTo(margin*2+relative,margin);
			pdc->LineTo(margin*2+relative,rt.bottom-margin);	
			char buffer[20];
			
			CFont font;
			font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Arial");
			pdc->SelectObject(font);
			pdc->TextOut((margin*2+relative+last)/2-10,rt.bottom-margin,itoa(curdata,buffer,10));
			font.DeleteObject();
			
			pdc->SelectObject(kedu);
			pdc->MoveTo(last,rt.bottom-margin+10);
			pdc->LineTo(last,rt.bottom-margin+20);
			pdc->MoveTo(last,rt.bottom-margin+15);
			pdc->LineTo(margin*2+relative,rt.bottom-margin+15);
			pdc->MoveTo(margin*2+relative,rt.bottom-margin+10);
			pdc->LineTo(margin*2+relative,rt.bottom-margin+20);
			pdc->SelectObject(axispen);
			last=margin*2+relative;
		}

		//第三层drawvertical
		pdc->SelectObject(axispen);
		sum=drawvertical.GetAt(drawvertical.GetSize()-1);
		pdc->MoveTo(margin,rt.bottom-margin*2);
		pdc->LineTo(rt.right-margin,rt.bottom-margin*2);
		last=rt.bottom-margin*2;
		relative=0;
		for(i=1;i<drawvertical.GetSize();i++)
		{
			int curdata=drawvertical.GetAt(i)-drawvertical.GetAt(i-1);
			relative = int((rt.bottom-margin*4)*(float)drawvertical.GetAt(i)/sum);
			pdc->MoveTo(margin,rt.bottom-margin*2-relative);
			pdc->LineTo(rt.right-margin,rt.bottom-margin*2-relative);
			char buffer[20];
			
			CFont font;
			font.CreateFont(10,0,-900,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Arial");
			pdc->SelectObject(font);
			pdc->TextOut(40,(rt.bottom-margin*2-relative+last)/2-10,itoa(curdata,buffer,10));
			font.DeleteObject();
			
			pdc->SelectObject(kedu);
			pdc->MoveTo(margin-10,last);
			pdc->LineTo(margin-20,last);
			pdc->MoveTo(margin-15,last);
			pdc->LineTo(margin-15,rt.bottom-margin*2-relative);
			pdc->MoveTo(margin-10,rt.bottom-margin*2-relative);
			pdc->LineTo(margin-20,rt.bottom-margin*2-relative);
			pdc->SelectObject(axispen);

			last=rt.bottom-margin*2-relative;
		}
		
		CPen sectionpen(PS_SOLID,3,RGB(255,0,0));
		pdc->SelectObject(sectionpen);
		//第三层jiemianhorizontal
		pdc->MoveTo(0,jiemianhorizontal);
		pdc->LineTo(2*margin-5,jiemianhorizontal);
		pdc->MoveTo(rt.right-margin*2+5,jiemianhorizontal);
		pdc->LineTo(rt.right,jiemianhorizontal);

		//第三层jiemianvertical		
		pdc->MoveTo(jiemianvertical,0);
		pdc->LineTo(jiemianvertical,2*margin-5);
		pdc->MoveTo(jiemianvertical,rt.bottom-margin*2+5);
		pdc->LineTo(jiemianvertical,rt.bottom);

		tempdc->BitBlt(0,0,rt.right,rt.bottom,pdc,0,0,SRCCOPY);
	}
}

void CLlDlg::PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize)
{//rt		src原点位于左下　dst原点位于左上
	int maxx=drawhorizontal.GetAt(drawhorizontal.GetSize()-1);
	int maxy=drawvertical.GetAt(drawvertical.GetSize()-1);
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

void CLlDlg::PhysicalPointToUIPoint(POINT* pt,const RECT& uirect,int x,int y)
{
	int maxx=drawhorizontal.GetAt(drawhorizontal.GetSize()-1);
	int maxy=drawvertical.GetAt(drawvertical.GetSize()-1);
	if(maxx == 0)
		maxx=1;
	if(maxy == 0)
		maxy=1;
	float ratiox=(float)(uirect.right-4*margin)/maxx,ratioy=(float)(uirect.bottom-4*margin)/maxy;
	pt->x=long(x*ratiox+2*margin);
	pt->y=long(uirect.bottom-y*ratioy-2*margin);
}

void CLlDlg::UIPointToPhysicalPoint(POINT* pt,int x,int y)
{
	RECT uirect;
	m_draw.GetClientRect(&uirect);
	int maxx=drawhorizontal.GetAt(drawhorizontal.GetSize()-1);
	int maxy=drawvertical.GetAt(drawvertical.GetSize()-1);
	float ratiox=maxx/(float)(uirect.right-4*margin),ratioy=maxy/(float)(uirect.bottom-4*margin);
	pt->x=long((x-2*margin)*ratiox);
	pt->y=long((uirect.bottom-y-2*margin)*ratioy);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLlDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	if(::IsWindow(m_draw.m_hWnd))
	{
		m_draw.MoveWindow(&rt);
		MyDC.SelectObject(oldbmp);
		bmp.DeleteObject();
		m_draw.GetClientRect(&rt);
		bmp.CreateCompatibleBitmap(tempdc,rt.right,rt.bottom);
		MyDC.SelectObject(bmp);
		CBrush brush(RGB(255,255,255));
		MyDC.FillRect(&rt,&brush);
	}

	Invalidate(FALSE);
}

BOOL CLlDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE; 
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE; 
	return CDialog::PreTranslateMessage(pMsg);
}

void CLlDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int i;
	for(i=0;i<pillars.GetSize();i++)
	{
		delete pillars.GetAt(i);
	}
	for(i=0;i<beams.GetSize();i++)
	{
		delete beams.GetAt(i);
	}
	for(i=0;i<rectzones.GetSize();i++)
	{
		delete rectzones.GetAt(i);
	}
	for(i=0;i<polyzones.GetSize();i++)
	{
		delete polyzones.GetAt(i);
	}
	
	MyDC.SelectObject(oldbmp);
	bmp.DeleteObject();
	MyDC.DeleteDC();
	if(showdlg)
		delete showdlg;
	if(createelement)
		delete createelement;

	CDialog::OnClose();
}
