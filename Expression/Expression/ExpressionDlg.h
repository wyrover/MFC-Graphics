
// ExpressionDlg.h : ͷ�ļ�
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
	LEVEL0,//0 1 2 3 4 5 6 7 8 9 . PI E		(δ֪��)x
	LEVEL1,//()+() ()-()
	LEVEL2,//()*() ()/()
	LEVEL3,//()^()
	LEVEL4,//SIN() COS() TAN() ASIN() ACOS() ATAN() SINH() COSH() TANH() SQRT() LN() LOG10() CEIL() FLOOR() SGN() ABS() FACT()
	//( [ {
	LEVEL5,

	//SUBTYPES
	TYPE_INVALID=LEVEL0*0x100,	//��Ч
	TYPE_UNKNOWN,						//δ֪��
	TYPE_NUMBER,							//��֪��
	TYPE_ADD=LEVEL1*0x100,		//��
	TYPE_SUB,								//��
	TYPE_MUL=LEVEL2*0x100,		//��
	TYPE_DIV,									//��
	TYPE_POW=LEVEL3*0x100,		//�˷�
	TYPE_SIN=LEVEL4*0x100,		//����		ȫ������׺����
	TYPE_COS,								//����
	TYPE_TAN,								//����
	TYPE_ASIN,								//������
	TYPE_ACOS,								//������
	TYPE_ATAN,								//������
	TYPE_SINH,								//˫������
	TYPE_COSH,								//˫������
	TYPE_TANH,								//˫������
	TYPE_SQRT,								//��ƽ��
	TYPE_LN,									//��Ȼ����
	TYPE_LOG10,							//��10Ϊ�׵Ķ���
	TYPE_CEIL,								//����ȡ��
	TYPE_FLOOR,							//����ȡ��
	TYPE_SGN,								//���ź���
	TYPE_ABS,								//����ֵ
	TYPE_FACT,						//�׳�
	TYPE_BRACKET,						//����
};

#define getMainType(x) (x/0x100)
#define in
#define out

struct numberstackdata
{
	int	thistype;
	double	data;
};

class MathFuncTreeNode//ֻ���д��ĸ�����֣����ڿ���������ѧ������
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
	MathFuncTreeNode*& operator[](int Index)//��ȡ�ӽڵ�
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
		cur->NameLen=strlen(obj)+1;//��������
		cur->Type=Type;
	}

	bool SearchMathFunc(in const char* toSearch,out MathFuncTreeNode*& result) const//��������
	{//����type
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
	void DelTree(MathFuncTreeNode* CurrentTrunk)//�ݹ�ɾ��������
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

	MathFuncTreeNode* Root;//���������ڵ�
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
	vector<dPoint> points;//��Ļ��������
};

// CExpressionDlg �Ի���
class CExpressionDlg : public CDialogEx
{
// ����
public:
	CExpressionDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CMathSigTree tree;
	myPointArray pts;
	void inittree();
	bool isBeginBracket(in const char* src,out numberstackdata& dst,out int& pos);
	bool makePostfileExpression(in const string& input,out vector<numberstackdata>& output);
	bool processOneValue(in const vector<numberstackdata>& datalist,in const numberstackdata* toreplace,out double& result);
// �Ի�������
	enum { IDD = IDD_EXPRESSION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
