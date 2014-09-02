// DesignDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DesignDlg.h"
#include "DesignDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignDlgDlg dialog

CDesignDlgDlg::CDesignDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDesignDlgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignDlgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDesignDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignDlgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDesignDlgDlg, CDialog)
	//{{AFX_MSG_MAP(CDesignDlgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SYSCOMMAND()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignDlgDlg message handlers

BOOL CDesignDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_csText="lichao的窗口";
	m_clText=RGB(0,255,255);
	m_BtnState=BS_NONE;

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDesignDlgDlg::OnPaint() 
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
	}
	DrawDialog(ALL);
	DrawTitlebarText();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDesignDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDesignDlgDlg::DrawDialog(int nFlag)
{
	int nFrameCY=GetSystemMetrics(SM_CYFIXEDFRAME);//对话框边框高度
	int nFrameCX=GetSystemMetrics(SM_CXFIXEDFRAME);//对话框边框宽度
	if(GetStyle()&WS_BORDER)
	{
		m_nBorderCY=GetSystemMetrics(SM_CYBORDER)+nFrameCY;
		m_nBorderCX=GetSystemMetrics(SM_CXBORDER)+nFrameCX;
	}
	m_nTitileBarCY=GetSystemMetrics(SM_CYCAPTION)+m_nBorderCY;//计算标题栏高度
	CRect WinRC;
	GetWindowRect(WinRC);//获取整个窗口区域
	CWindowDC WindowDC(this);//获取窗口设备上下文
	CBitmap Bmp;//定义位图文件
	CDC memDC;//定义内存画布
	memDC.CreateCompatibleDC(&WindowDC);//创建内存画布

	Bmp.LoadBitmap(IDB_BITMAP);//加载位图
	memDC.SelectObject(&Bmp);//选中位图
	
	if(nFlag&LEFTBAR)//绘制对话框左边框位图
	{
		WindowDC.StretchBlt(0,m_nTitileBarCY,10,WinRC.Height()-m_nTitileBarCY,
			&memDC,491,68,20,59,SRCCOPY);//在窗口中绘制位图
	}
	
	if(nFlag&LEFTTITLE)//绘制左标题栏
	{
		WindowDC.StretchBlt(0,0,60,m_nTitileBarCY,
			&memDC,4,107,43,18,SRCCOPY);//在窗口中绘制位图
	}
	if(nFlag&RIGHTTITLE)
	{
		WindowDC.StretchBlt(WinRC.Width()-100,0,100,m_nTitileBarCY,
			&memDC,4,107,43,18,SRCCOPY);
	}
	if(nFlag&MIDTITLE)
	{
		WindowDC.StretchBlt(60,0,WinRC.Width()-156,m_nTitileBarCY,
			&memDC,0,0,35,24,SRCCOPY);
	}
	if(nFlag&RIGHTBAR)
	{
		WindowDC.StretchBlt(WinRC.Width()-10,m_nTitileBarCY,10,WinRC.Height()-m_nTitileBarCY,
			&memDC,491,68,20,59,SRCCOPY);
	}
	if(nFlag&BOTTOMBAR)
	{
		WindowDC.StretchBlt(0,WinRC.Height()-10,WinRC.Width(),10,
			&memDC,59,190,56,23,SRCCOPY);
	}
	if(nFlag&CLOSEBUTTON)
	{
		WindowDC.StretchBlt(WinRC.Width()-90,9,20,20,
			&memDC,19,266,11,11,SRCCOPY);
	}
	if(nFlag&MAXBUTTON)
	{
		WindowDC.StretchBlt(WinRC.Width()-60,9,20,20,
			&memDC,64,266,11,11,SRCCOPY);
	}
	if(nFlag&MINBUTTON)
	{
		WindowDC.StretchBlt(WinRC.Width()-30,9,20,20,
			&memDC,64,266,11,11,SRCCOPY);
	}

	Bmp.DeleteObject();//释放加载的位图
}

void CDesignDlgDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
	// TODO: Add your message handler code here
	
}

void CDesignDlgDlg::DrawTitlebarText()
{
	if(!m_csText.IsEmpty())
	{
		CDC* pDC=GetWindowDC();
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clText);
		pDC->SetTextAlign(TA_CENTER);//设置文本对齐方式
		CRect rect;
		GetClientRect(rect);
		CSize szText=pDC->GetTextExtent(m_csText);//获取文本高度
		//在窗口中输出文本
		pDC->TextOut(rect.Width()/2,(m_nTitileBarCY-szText.cy)/2,m_csText);
		ReleaseDC(pDC);
	}
}

void CDesignDlgDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect WinRC;
	GetWindowRect(WinRC);//获取整个窗口区域
	CWindowDC WindowDC(this);//获取窗口设备上下文
	CBitmap Bmp;//定义位图文件
	CDC memDC;//定义内存画布
	GetWindowRect(WinRC);
	CRect MinRC(WinRC.left+WinRC.Width()-30,WinRC.top+9,WinRC.left+
		WinRC.Width()-10,WinRC.top+29);
	CRect MaxRC(WinRC.left+WinRC.Width()-60,WinRC.top+9,WinRC.left+
		WinRC.Width()-40,WinRC.top+29);
	CRect CloseRC(WinRC.left+WinRC.Width()-90,WinRC.top+9,WinRC.left+
		WinRC.Width()-70,WinRC.top+29);
	memDC.CreateCompatibleDC(&WindowDC);//创建内存画布
	Bmp.LoadBitmap(IDB_BITMAP);//加载位图
	memDC.SelectObject(&Bmp);//选中位图
	if(MinRC.PtInRect(point))
	{
		if(m_BtnState!=BS_MIN)
		{
			WindowDC.StretchBlt(WinRC.Width()-30,9,20,20,&memDC,49,266,11,11,SRCCOPY);
			DrawDialog(MAXBUTTON);//鼠标移动太快时容易激活两个以上的按钮
			DrawDialog(CLOSEBUTTON);
			m_BtnState=BS_MIN;
		}
	}
	else if(MaxRC.PtInRect(point))
	{
		if(m_BtnState!=BS_MAX)
		{
			WindowDC.StretchBlt(WinRC.Width()-60,9,20,20,&memDC,49,266,11,11,SRCCOPY);
			DrawDialog(MINBUTTON);
			DrawDialog(CLOSEBUTTON);
			m_BtnState=BS_MAX;
		}
	}
	else if(CloseRC.PtInRect(point))
	{
		if(m_BtnState!=BS_CLOSE)
		{
			WindowDC.StretchBlt(WinRC.Width()-90,9,20,20,&memDC,4,266,11,11,SRCCOPY);
			DrawDialog(MAXBUTTON);
			DrawDialog(MINBUTTON);
			m_BtnState=BS_CLOSE;
		}
	}
	else
	{
		switch(m_BtnState)
		{
			case BS_MIN:DrawDialog(MINBUTTON);break;
			case BS_MAX:DrawDialog(MAXBUTTON);break;
			case BS_CLOSE:DrawDialog(CLOSEBUTTON);break;
			default:break;
		}
		m_BtnState=BS_NONE;
	}
	Bmp.DeleteObject();//释放加载的位图
	CDialog::OnNcMouseMove(nHitTest, point);
}

void CDesignDlgDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{//鼠标左键按下时按键图片改变
	// TODO: Add your message handler code here and/or call default
	CRect WinRC;
	GetWindowRect(WinRC);//获取整个窗口区域
	CWindowDC WindowDC(this);//获取窗口设备上下文
	CBitmap Bmp;//定义位图文件
	CDC memDC;//定义内存画布
	memDC.CreateCompatibleDC(&WindowDC);//创建内存画布
	Bmp.LoadBitmap(IDB_BITMAP);//加载位图
	memDC.SelectObject(&Bmp);//选中位图
	switch(m_BtnState)
	{
		case BS_MIN://按下最小化
			WindowDC.StretchBlt(WinRC.Width()-30,9,20,20,&memDC,79,266,11,11,SRCCOPY);
			break;
		case BS_MAX://按下最大化
			WindowDC.StretchBlt(WinRC.Width()-60,9,20,20,&memDC,79,266,11,11,SRCCOPY);
			break;
		case BS_CLOSE://按下关闭
			WindowDC.StretchBlt(WinRC.Width()-90,9,20,20,&memDC,34,266,11,11,SRCCOPY);
			break;
		default:break;
	}	
	Bmp.DeleteObject();//释放加载的位图
	CDialog::OnLButtonDown(nHitTest, point);/*Nc*/
}

void CDesignDlgDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	//当在非客户区点击鼠标左键并释放时，系统会发送窗口的WM_LBUTTONUP(注意不是WM_NCLBUTTONUP，
	//这是客户区的鼠标释放消息，但是我们现在在非客户区释放鼠标！如之奈何)消息到线程的消息队列，
	// 但线程的消息队列压根就不把这个消息发送给这个窗口的窗口过程!
	// TODO: Add your message handler code here and/or call default
	CDialog::OnSysCommand(nID, lParam);
	if((nID&0xFFF0)==SC_MOVE)
	{
		PostMessage(WM_NCLBUTTONUP,HTCAPTION,lParam);
	}
}

void CDesignDlgDlg::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{//鼠标左键抬起时按钮图片改变

	CRect WinRC;
	GetWindowRect(WinRC);//获取整个窗口区域
	CWindowDC WindowDC(this);//获取窗口设备上下文
	CBitmap Bmp;//定义位图文件
	CDC memDC;//定义内存画布
	memDC.CreateCompatibleDC(&WindowDC);//创建内存画布
	Bmp.LoadBitmap(IDB_BITMAP);//加载位图
	memDC.SelectObject(&Bmp);//选中位图
	switch(m_BtnState)
	{
		case BS_MIN://按下最小化
			WindowDC.StretchBlt(WinRC.Width()-30,9,20,20,&memDC,49,266,11,11,SRCCOPY);
			ShowWindow(SW_SHOWMINIMIZED);
			break;
		case BS_MAX://按下最大化
			WindowDC.StretchBlt(WinRC.Width()-60,9,20,20,&memDC,49,266,11,11,SRCCOPY);
			ShowWindow(SW_SHOWMAXIMIZED);
			break;
		case BS_CLOSE://按下关闭
			WindowDC.StretchBlt(WinRC.Width()-90,9,20,20,&memDC,4,266,11,11,SRCCOPY);
			OnCancel();
			break;
		default:break;
	}	
	Bmp.DeleteObject();//释放加载的位图
	CDialog::OnNcLButtonUp(nHitTest, point);
}


UINT CDesignDlgDlg::OnNcHitTest(CPoint point) 
{//阻止按钮区拖动窗口
	// TODO: Add your message handler code here and/or call default
	CRect WinRC;
	GetWindowRect(WinRC);
	CRect MinRC(WinRC.left+WinRC.Width()-30,WinRC.top+9,WinRC.left+
		WinRC.Width()-10,WinRC.top+29);
	CRect MaxRC(WinRC.left+WinRC.Width()-60,WinRC.top+9,WinRC.left+
		WinRC.Width()-40,WinRC.top+29);
	CRect CloseRC(WinRC.left+WinRC.Width()-90,WinRC.top+9,WinRC.left+
		WinRC.Width()-70,WinRC.top+29);

	if(MinRC.PtInRect(point))	
	{
		return HTMINBUTTON;
	}
	else if(MaxRC.PtInRect(point))
	{
		return HTMAXBUTTON;
	}
	else if(CloseRC.PtInRect(point))
	{
		return HTCLOSE;
	}
	else return CDialog::OnNcHitTest(point);
}

void CDesignDlgDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{//不处于激活状态时也有按钮
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	DrawDialog(CAPTION);
	// TODO: Add your message handler code here
	
}
