
// ExpressionDlg.h : 头文件
//

#pragma once

#include <math.h>
#include <float.h>
#include <assert.h>

#include <vector>
#include <stack>
#include <string>
#include <iostream>
using namespace std;

#define MAX_CHILDNUM 256

enum type
{
	//MAINTYPES
	LEVEL0,//0 1 2 3 4 5 6 7 8 9 . PI E		(未知数)x
	LEVEL1,//()+() ()-()
	LEVEL2,//()*() ()/()
	LEVEL3,//()^()
	LEVEL4,//SIN() COS() TAN() ASIN() ACOS() ATAN() SINH() COSH() TANH() SQRT() LN() LOG10() CEIL() FLOOR() SGN() ABS() FACT()
	//( [ {
	LEVEL5,

	//SUBTYPES
	TYPE_INVALID=LEVEL0*0x100,	//无效
	TYPE_UNKNOWN,						//未知数
	TYPE_NUMBER,							//已知数
	TYPE_ADD=LEVEL1*0x100,		//和
	TYPE_SUB,								//差
	TYPE_MUL=LEVEL2*0x100,		//积
	TYPE_DIV,									//商
	TYPE_POW=LEVEL3*0x100,		//乘方
	TYPE_SIN=LEVEL4*0x100,		//正弦		全当做后缀处理
	TYPE_COS,								//余弦
	TYPE_TAN,								//正切
	TYPE_ASIN,								//反正弦
	TYPE_ACOS,								//反余弦
	TYPE_ATAN,								//反正切
	TYPE_SINH,								//双曲正弦
	TYPE_COSH,								//双曲余弦
	TYPE_TANH,								//双曲正切
	TYPE_SQRT,								//开平方
	TYPE_LN,									//自然对数
	TYPE_LOG10,							//以10为底的对数
	TYPE_CEIL,								//向上取整
	TYPE_FLOOR,							//向下取整
	TYPE_SGN,								//符号函数
	TYPE_ABS,								//绝对值
	TYPE_FACT,						//阶乘
	TYPE_BRACKET,						//括号
};

#define getMainType(x) (x/0x100)
#define in
#define out

struct numberstackdata
{
	int	thistype;
	double	data;
};

class MathFuncTreeNode//只存大写字母和数字，用于快速索引数学函数名
{
public:
	MathFuncTreeNode(int type,int len=-1)
	{
		for(int i=0;i<MAX_CHILDNUM;i++)
		{
			Child[i]=NULL;
		}
		NameLen=len;
		Type=type;
	}
	MathFuncTreeNode*& operator[](int Index)//获取子节点
	{
		return Child[Index];
	}

	MathFuncTreeNode* Child[MAX_CHILDNUM];
	int NameLen;
	int Type;
};

class CMathSigTree
{
public:
	CMathSigTree()
	{
		Root=new MathFuncTreeNode(TYPE_INVALID,-1);
	}

	virtual ~CMathSigTree()
	{
		DelTree(Root);
	}

	void AddTreeTrunk(in const char* obj,in int Type)
	{
		MathFuncTreeNode* cur=Root;
		char* ptr=(char*)obj;
		while(*ptr && *ptr != '(')
		{
			if(!cur->Child[*ptr])
			{
				cur->Child[*ptr]=new MathFuncTreeNode(TYPE_INVALID);
			}
			cur=cur->Child[*ptr++];
		}
		assert(*ptr != '(');
		cur->NameLen=strlen(obj)+1;//不算括号
		cur->Type=Type;
	}

	bool SearchMathFunc(in const char* toSearch,out MathFuncTreeNode*& result) const//搜索函数
	{//返回type
		MathFuncTreeNode* cur=Root;
		char* ptr=(char*)toSearch;
		while(*ptr != '(')
		{
			int index=toupper(*ptr);
			if(!cur->Child[index])
			{
				return false;
			}
			cur=cur->Child[index];
			ptr++;
		}
		if(cur->NameLen == -1 || cur->Type == TYPE_INVALID)
		{
			return false;
		}
		else
		{
			result=cur;
			return true;
		}
	}

private:
	void DelTree(MathFuncTreeNode* CurrentTrunk)//递归删除函数树
	{
		if(!CurrentTrunk)
		{
			return;
		}
		for(int i=0;i<MAX_CHILDNUM;i++)
		{
			DelTree(CurrentTrunk->Child[i]);
		}
		delete CurrentTrunk;
	}

	MathFuncTreeNode* Root;//函数树根节点
};

struct dPoint
{
	double input;
	double output;
	dPoint(double x,double y)
	{
		input=x;
		output=y;
	}
};

struct myPointArray
{
	double xbegin;
	double xend;
	double ybegin;
	double yend;
	vector<dPoint> points;//屏幕坐标阵列
};

// CExpressionDlg 对话框
class CExpressionDlg : public CDialogEx
{
// 构造
public:
	CExpressionDlg(CWnd* pParent = NULL);	// 标准构造函数
	CMathSigTree tree;
	myPointArray pts;
	void inittree();
	bool isBeginBracket(in const char* src,out numberstackdata& dst,out int& pos);
	bool makePostfileExpression(in const string& input,out vector<numberstackdata>& output);
	bool processOneValue(in const vector<numberstackdata>& datalist,in const numberstackdata* toreplace,out double& result);
// 对话框数据
	enum { IDD = IDD_EXPRESSION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGenerate();
	CStatic m_draw;
	CString m_expression;
	afx_msg void OnEnChangeExpression();
//	CString m_begin;
	double m_begin;
	double m_end;
};
