
// ExpressionDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "Expression.h"
#include "ExpressionDlg.h"
#include "afxdialogex.h"

#include <float.h>
#include <assert.h>

#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#define E_VALUE		2.718281828
#define PI_VALUE		3.141592654
#define THRESHOLD	1000000000.0//判断奇点的门限
#define ADDMATHFUNC(X) tree.AddTreeTrunk(#X,TYPE_##X)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CExpressionDlg 对话框
CExpressionDlg::CExpressionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExpressionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_expression = _T("x^e");
	m_begin = -10.0;
	m_end = 10.0;
}

void CExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAW, m_draw);
	DDX_Text(pDX, IDC_EXPRESSION, m_expression);
	//  DDX_Text(pDX, IDC_BEGIN, m_begin);
	//  DDV_MaxChars(pDX, m_begin, 10);
	DDX_Text(pDX, IDC_BEGIN, m_begin);
	DDX_Text(pDX, IDC_END, m_end);
}

BEGIN_MESSAGE_MAP(CExpressionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GENERATE, &CExpressionDlg::OnBnClickedGenerate)
END_MESSAGE_MAP()


void CExpressionDlg::inittree()
{
	ADDMATHFUNC(SIN);
	ADDMATHFUNC(COS);
	ADDMATHFUNC(TAN);
	ADDMATHFUNC(ASIN);
	ADDMATHFUNC(ACOS);
	ADDMATHFUNC(ATAN);
	ADDMATHFUNC(SINH);
	ADDMATHFUNC(COSH);
	ADDMATHFUNC(TANH);
	ADDMATHFUNC(SQRT);
	ADDMATHFUNC(LN);
	ADDMATHFUNC(LOG10);
	ADDMATHFUNC(CEIL);
	ADDMATHFUNC(FLOOR);
	ADDMATHFUNC(SGN);
	ADDMATHFUNC(ABS);
	ADDMATHFUNC(FACT);
}

int resolvenum(in const char* pos,out double& data)
{
	bool negative=false;
	char* curpos=(char*)pos;
	if(*curpos == '-' || * curpos == '+')
	{
		curpos++;
		if(*curpos == '-')
			negative=true;
	}
	int integer=0;
	double decimal=0;
	while(*curpos >= '0' && *curpos <= '9')
	{
		integer=integer*10 + *curpos - '0';
		curpos++;
	}
	if(*curpos == '.')
	{
		curpos++;
		while(*curpos >= '0' && *curpos <= '9')
		{
			decimal=decimal*10 + *curpos - '0';
			curpos++;
		}
	}
	while(decimal >= 1.0)
	{
		decimal /= 10;
	}

	data=integer+decimal;
	if(curpos-pos > 1)
		return curpos-pos;
	else
		return 1;
}

double lagam(double x)
{ 
	int i;
	double y,t,s,u;
	double a[11]=
	{
		0.0000677106,
		-0.0003442342,
		0.0015397681,
		-0.0024467480,
		0.0109736958,
		-0.0002109075,
		0.0742379071,
		0.0815782188,
		0.4118402518,
		0.4227843370,
		1.0
	};
	x++;
	y=x;
	if (y<=1.0)
	{ 
		t=1.0/(y*(y+1.0)); 
		y=y+2.0;
	}
	else if (y<=2.0)
	{ 
		t=1.0/y; 
		y=y+1.0;
	}
	else if (y<=3.0) 
	{
		t=1.0;
	}
	else
	{ 
		t=1.0;
		while (y>3.0)
		{ 
			y=y-1.0; 
			t=t*y;
		}
	}
	s=a[0]; 
	u=y-2.0;
	for (i=1; i<=10; i++)
	{
		s=s*u+a[i];
	}
	s=s*t;
	return s;
}

bool CExpressionDlg::isBeginBracket(in const char* src,out numberstackdata& dst,out int& pos)
{
	char* ptr=(char*)src;
	MathFuncTreeNode* result;
	if(tree.SearchMathFunc(src,result))
	{
		dst.thistype=result->Type;
		pos += result->NameLen;
		return true;
	}
	else if(src[0] == '(' || src[0] == '[' || src[0] == '{')
	{
		dst.thistype=TYPE_BRACKET;
		pos ++;
		return true;
	}
	return false;
}

bool CExpressionDlg::makePostfileExpression(in const string& input,out vector<numberstackdata>& output)
{
	type state_machine=LEVEL4;//当前状态机  LEVEL4代替初始状态
	//状态转移： 
	//LEVEL1 -> LEVEL0	LEVEL2 -> LEVEL0	LEVEL3 -> LEVEL0	LEVEL4 -> LEVEL0
	//LEVEL0 -> LEVEL1	LEVEL4 -> LEVEL1(默认填0)
	//LEVEL0 -> LEVEL2	
	//LEVEL0 -> LEVEL3
	//LEVEL1 -> LEVEL4	LEVEL2 -> LEVEL4	LEVEL3 -> LEVEL4	LEVEL4 -> LEVEL4

	stack<numberstackdata> sigstack;//char str[]="9+(3-1)*3+10/2";//9 3 1 - 3 * + 10 2 / +

	int pos1=0,pos2=0;
	numberstackdata curdata;
	while(pos1<(int)input.length())
	{//符号栈始终保持上方元素优先级>=下方元素优先级    优先级Prev>Next时将Prev出栈Next进栈 Prev<=Next时仅将Next进栈
	  //遇到左括号或者空栈相当于Prev达到最低		遇到右括号时连续出栈直到遇到左括号
		if((input[pos1] >= '0' && input[pos1] <= '9') || input[pos1] == 'x' || input[pos1] == 'X' 
			|| !_strnicmp(input.c_str()+pos1,"PI",2) || toupper(input[pos1]) == 'E')
		{//LEVEL0
			if(state_machine == LEVEL0)
			{
				goto ERROR1;
			}
			if(input[pos1] >= '0' && input[pos1] <= '9')
			{
				double result;
				pos1 += resolvenum(input.c_str()+pos1,result);
				curdata.data=result;
				curdata.thistype=TYPE_NUMBER;
			}
			else if(input[pos1] == 'x' || input[pos1] == 'X')
			{
				pos1++;
				curdata.thistype=TYPE_UNKNOWN;
			}
			else
			{
				if(toupper(input[pos1]) == 'E')
				{
					curdata.thistype=TYPE_NUMBER;
					curdata.data=E_VALUE;
					pos1++;
				}
				else//PI
				{
					curdata.thistype=TYPE_NUMBER;
					curdata.data=PI_VALUE;
					pos1 += 2;
				}
			}
			output.push_back(curdata);
			state_machine=LEVEL0;
		}
		else if(input[pos1] == '+' || input[pos1] == '-')
		{//LEVEL1
			if(state_machine == LEVEL0 || state_machine == LEVEL4)
			{
				if(state_machine == LEVEL4)
				{
					if(input[pos1] == '+')
					{//空白或左括号之后的正号直接省略
						pos1++;
						continue;
					}
					else
					{//空白或左括号之后的符号认为前一个操作数为0，因此push 0
						curdata.thistype=TYPE_NUMBER;
						curdata.data=0;
						output.push_back(curdata);
					}
				}
				if(!sigstack.empty())
				{
					int maintype=getMainType(sigstack.top().thistype);
					if(maintype != LEVEL4 && maintype > LEVEL1)
					{
						output.push_back(sigstack.top());
						sigstack.pop();
					}
				}
				if(input[pos1] == '+')
					curdata.thistype=TYPE_ADD;
				else
					curdata.thistype=TYPE_SUB;
				sigstack.push(curdata);
			}
			else
			{
				goto ERROR1;
			}
			pos1++;
			state_machine=LEVEL1;
		}
		else if(input[pos1] == '*' || input[pos1] == '/')
		{
			if(state_machine == LEVEL0)
			{
				if(!sigstack.empty())
				{
					int maintype=getMainType(sigstack.top().thistype);
					if(maintype != LEVEL4 && maintype > LEVEL2)
					{
						output.push_back(sigstack.top());
						sigstack.pop();
					}
				}
				if(input[pos1] == '*')
					curdata.thistype=TYPE_MUL;
				else
					curdata.thistype=TYPE_DIV;
				sigstack.push(curdata);
			}
			else
			{
				goto ERROR1;
			}
			pos1++;
			state_machine=LEVEL2;
		}
		else if(input[pos1] == '^')
		{
			if(state_machine == LEVEL0)
			{//除了LEVEL4没有再比LEVEL3更高的级别了!!因此这里不用额外判断
				curdata.thistype=TYPE_POW;
				sigstack.push(curdata);
			}
			else
			{
				goto ERROR1;
			}
			pos1++;
			state_machine=LEVEL3;
		}
		else if(input[pos1] == ')' || input[pos1] == ']' || input[pos1] == '}')
		{
			if(state_machine != LEVEL0)
			{
				goto ERROR1;
			}
			if(sigstack.empty())
			{
				return false;
			}
			while(getMainType(sigstack.top().thistype) != LEVEL4)
			{
				output.push_back(sigstack.top());
				sigstack.pop();
				if(sigstack.empty())
				{
					return false;
				}
			}
			output.push_back(sigstack.top());
			sigstack.pop();
			pos1++;
			state_machine=LEVEL0;
		}
		else if(input[pos1] == ' ')
		{
			pos1++;
		}
		else if(isBeginBracket(input.c_str()+pos1,curdata,pos1))
		{
			if(state_machine == LEVEL0)
			{
				goto ERROR1;
			}
			sigstack.push(curdata);
			state_machine=LEVEL4;
		}
		else
		{
			goto ERROR1;
		}
	}
	while(!sigstack.empty())
	{
		output.push_back(sigstack.top());
		sigstack.pop();
	}
	return true;
ERROR1:
	return false;
}

bool CExpressionDlg::processOneValue(in const vector<numberstackdata>& datalist,in const numberstackdata* toreplace,out double& result)
{
	stack<numberstackdata> numstack;//	char str[]="9 3.1 1.2 - 3.3 * + 10.4 2.5 / +";

	for(vector<numberstackdata>::const_iterator it=datalist.begin();it != datalist.end(); ++it)
	{
		if(getMainType((*it).thistype) == LEVEL0)//数字
		{
			if((*it).thistype == TYPE_UNKNOWN)
			{
				assert(toreplace != NULL);//替换该值
				numstack.push(*toreplace);
			}
			else
			{
				numstack.push(*it);
			}
		}
		else//符号
		{
			if(getMainType((*it).thistype) == LEVEL4)
			{//单目操作符
				switch((*it).thistype)
				{
				case TYPE_SIN:
					numstack.top().data = sin(numstack.top().data);
					break;

				case TYPE_COS:
					numstack.top().data = cos(numstack.top().data);
					break;

				case TYPE_TAN:
					numstack.top().data = tan(numstack.top().data);
					if(0 == _finite(numstack.top().data))
					{
						goto ERROR2;
					}
					break;

				case TYPE_ASIN:
					{
						double data=numstack.top().data;
						if(data < -1 || data > 1)
						{
							goto ERROR2;
						}
						numstack.top().data = asin(data);
					}
					break;

				case TYPE_ACOS:
					{
						double data=numstack.top().data;
						if(data < -1 || data > 1)
						{
							goto ERROR2;
						}
						numstack.top().data = acos(data);
					}
					break;

				case TYPE_ATAN:
					numstack.top().data = atan(numstack.top().data);
					if(0 == _finite(numstack.top().data))
					{
						goto ERROR2;
					}
					break;

				case TYPE_SINH:
					numstack.top().data = sinh(numstack.top().data);
					break;

				case TYPE_COSH:
					numstack.top().data = cosh(numstack.top().data);
					break;

				case TYPE_TANH:
					numstack.top().data = tanh(numstack.top().data);
					break;

				case TYPE_SQRT:
					{
						double data=numstack.top().data;
						if(data<0)
						{
							goto ERROR2;
						}
						numstack.top().data = sqrt(numstack.top().data);
					}
					break;

				case TYPE_LN:
					{
						double data=numstack.top().data;
						if(data<0)
						{
							goto ERROR2;
						}
						numstack.top().data = log(numstack.top().data);
					}
					break;

				case TYPE_LOG10:
					{
						double data=numstack.top().data;
						if(data<0)
						{
							goto ERROR2;
						}
						numstack.top().data = log10(numstack.top().data);
					}
					break;

				case TYPE_CEIL:
					numstack.top().data=ceil(numstack.top().data);
					break;

				case TYPE_FLOOR:
					numstack.top().data=floor(numstack.top().data);
					break;

				case TYPE_SGN:
					{
						double data=numstack.top().data;
						if(data > 0.0)
						{
							numstack.top().data=1.0;
						}
						else if(data == 0.0)
						{
							numstack.top().data=0.0;
						}
						else if(data < 0.0)
						{
							numstack.top().data=-1.0;
						}
					}
					break;

				case TYPE_ABS:
					if(numstack.top().data<0)
						numstack.top().data=-numstack.top().data;
					break;

				case TYPE_FACT:
					{
						double data=numstack.top().data;
						if(data<0)
						{
							goto ERROR2;
						}
						if((int)data == data)
						{
							int begin=int(data);
							double fact=1.0;
							while(begin)
							{
								fact *= begin;
								begin--;
							}
							numstack.top().data=fact;
						}
						else
						{
							numstack.top().data=lagam(data);
						}
					}
					break;
				}
			}
			else
			{//双目操作符
				numberstackdata& curdata=numstack.top();
				numstack.pop();
				switch((*it).thistype)
				{
				case TYPE_ADD:
					numstack.top().data += curdata.data;
					break;
				case TYPE_SUB:
					numstack.top().data -= curdata.data;
					break;
				case TYPE_MUL:
					numstack.top().data *= curdata.data;
					break;
				case TYPE_DIV:
					if(numstack.top().data != 0)
					{
						numstack.top().data /= curdata.data;
					}
					else
					{
						goto ERROR2;
					}
					break;
				case TYPE_POW:
					{
						double result=pow(numstack.top().data,curdata.data);
						if(_finite(result))
						{
							numstack.top().data=result;
						}
						else
						{//浮点溢出
							goto ERROR2;
						}
					}
					break;
				}
			}
		}
	}
	result=numstack.top().data;
	return true;
ERROR2:
	return false;
}


// CExpressionDlg 消息处理程序

BOOL CExpressionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	inittree();
	UpdateData(FALSE);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExpressionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(!pts.points.empty())
		{
			CDialogEx::OnPaint();
			RECT rt;
			m_draw.GetClientRect(&rt);
			CClientDC dc(&m_draw);
			CBrush clear(RGB(255,255,255));
			dc.FillRect(&rt,&clear);
			vector<dPoint>::iterator itor=pts.points.begin();
			CBrush brush(RGB(255,0,0));
			CPen pen(PS_SOLID,2,RGB(255,0,0));
			dc.SelectObject(brush);
			dc.SelectObject(pen);

			CString str;
			str.Format("%lf",pts.xbegin);
			dc.TextOut(30,rt.bottom-5,str);
			str.Format("%lf",pts.xend);
			dc.TextOut(rt.right-30,rt.bottom-5,str);
			str.Format("%lf",pts.ybegin);
			dc.TextOut(-10,rt.bottom-30,str);
			str.Format("%lf",pts.yend);
			dc.TextOut(-10,0,str);

			int x1=int(((*itor).input-pts.xbegin)/(pts.xend-pts.xbegin)*rt.right);
			int y1=rt.bottom-int(((*itor).output-pts.ybegin)/(pts.yend-pts.ybegin)*rt.bottom);
			dc.MoveTo(x1,y1);
			double prevx=(*itor).input;
			double prevy=(*itor).output;
			while(itor != pts.points.end())
			{
				double temp=(*itor).output;
				int x=int(((*itor).input-pts.xbegin)/(pts.xend-pts.xbegin)*rt.right);
				int y=rt.bottom-int(((*itor).output-pts.ybegin)/(pts.yend-pts.ybegin)*rt.bottom);
				double xielv=((*itor).output-prevy)/((*itor).input-prevx)*(pts.xend-pts.xbegin)*(pts.xend-pts.xbegin);
				if(temp == pts.ybegin || temp == pts.yend || xielv < -THRESHOLD || xielv > THRESHOLD)
				{
					dc.MoveTo(x,y);
				}
				else
				{
					dc.LineTo(x,y);
				}
				//dc.SetPixel(x,y,RGB(255,0,0));
				prevx=(*itor).input;
				prevy=(*itor).output;
				++itor;
			}
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExpressionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExpressionDlg::OnBnClickedGenerate()
{
	UpdateData(TRUE);
	vector<numberstackdata> datalist;
	if(!makePostfileExpression((LPCTSTR)m_expression,datalist))
	{
		AfxMessageBox("表达式有误");
		return;
	}
	if(m_begin >= m_end)
	{
		AfxMessageBox("输入范围有误");
		return;
	}

	pts.points.clear();
	RECT rt;
	m_draw.GetClientRect(&rt);
	pts.xbegin=m_begin;
	pts.xend=m_end;
	double xstep=(m_end-m_begin)/rt.right;

	for(long i=0;i <= rt.right;i++)
	{
		numberstackdata data={TYPE_NUMBER,i*xstep+pts.xbegin};
		double result;
		if(processOneValue(datalist,&data,result) && _finite(result))
		{
			pts.points.push_back(dPoint(data.data,result));
		}
	}
	//保持连续性
	numberstackdata data={TYPE_NUMBER,m_end};
	double result;
	if(processOneValue(datalist,&data,result) && _finite(result))
		pts.points.push_back(dPoint(m_end,result));

	vector<dPoint>::iterator itor=pts.points.begin();
	pts.ybegin=pts.yend=(*itor).output;
	while(itor != pts.points.end())
	{
		if((*itor).output<pts.ybegin)
		{
			pts.ybegin=(*itor).output;
		}
		else if((*itor).output>pts.yend)
		{
			pts.yend=(*itor).output;
		}
		++itor;
	}

	Invalidate(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}
