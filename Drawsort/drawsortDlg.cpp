// drawsortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "drawsort.h"
#include "drawsortDlg.h"

#include <time.h>
#include <stdlib.h>
#include <stack>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include <vector>
#include <time.h>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CDrawsortDlg dialog

CString SORTNAME[TYPE_MAX]={"冒泡排序","冒泡排序改进","双向冒泡排序","快速排序","插入排序",
		"二分插入排序","希尔排序","选择排序","锦标赛排序","堆排序","归并排序","基数排序","",""};


CDrawsortDlg::CDrawsortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawsortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawsortDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawsortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawsortDlg)
	DDX_Control(pDX, IDC_BLOCKSIZE, m_blocksize);
	DDX_Control(pDX, IDC_VELOCITY, m_velocity);
	DDX_Control(pDX, IDC_TYPELIST, m_typelist);
	DDX_Control(pDX, IDC_DRAW, m_draw);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrawsortDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawsortDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDDRAW, OnDraw)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VELOCITY, OnCustomdrawVelocity)
	ON_BN_CLICKED(IDGOON, OnGoon)
	ON_BN_CLICKED(IDPAUSE, OnPause)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawsortDlg message handlers

BOOL CDrawsortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	srand(time(NULL));
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_typelist.InsertString(0,"左半：");
	m_typelist.InsertString(1,"冒泡排序");
	m_typelist.InsertString(2,"+冒泡排序改进(哨兵)");
	m_typelist.InsertString(3,"++冒泡排序改进(双向)");
	m_typelist.InsertString(4,"+++快速排序");
	m_typelist.InsertString(5,"插入排序");
	m_typelist.InsertString(6,"+二分插入排序");
	m_typelist.InsertString(7,"++希尔排序(最优系数)");
	m_typelist.InsertString(8,"");
	m_typelist.InsertString(9,"右半");
	m_typelist.InsertString(10,"选择排序");
	m_typelist.InsertString(11,"+锦标赛排序(树形排序)");
	m_typelist.InsertString(12,"++堆排序");
	m_typelist.InsertString(13,"归并排序");
	m_typelist.InsertString(14,"基数排序(桶排序)");

	m_typelist.SetCurSel(0);

	for(int i=0;i<TYPE_MAX;i++)
	{
		toDrawQueue.push_back(queue<DrawData>());
	}

	SetTimer(1,200,NULL);
	srand(time(NULL));
	m_velocity.SetRange(10,1000);	
	m_velocity.SetPos(200);

	m_blocksize.SetRange(2,20);
	m_blocksize.SetPos(10);

	running=true;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrawsortDlg::OnPaint() 
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

		int queuenum=toDrawQueue.size();

		RECT drawrt;
		m_draw.GetClientRect(&drawrt);
		CClientDC dc(&m_draw);
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(&dc);
		MemBitmap.CreateCompatibleBitmap(&dc,drawrt.right,drawrt.bottom);
		MemDC.SelectObject(&MemBitmap);
		int BARSIZE=m_blocksize.GetPos();
		
		CBrush clearbrush(RGB(255,255,255));
		MemDC.FillRect(&drawrt,&clearbrush);
		CBrush normalbrush(RGB(0,0,255)),stressbrush(RGB(255,0,0));
//		CPen normalpen(PS_SOLID,1,RGB(0,0,255)),stresspen(PS_SOLID,1,RGB(255,0,0));
		int blockheight=drawrt.bottom*2/queuenum;

		for(int i=0;i < queuenum;i++)
		{
			if(toDrawQueue.at(i).empty())
				continue;
			queue<DrawData>& toDraw=toDrawQueue.at(i);

			DrawData& curdata=toDraw.front();


			int line=i%(TYPE_MAX/2);
			int column=drawrt.right/2*(i*2/TYPE_MAX);
			CString str;
			str.Format("%s 步数：%d",SORTNAME[i],curdata.step);
			MemDC.TextOut(column,blockheight*line,str);

			for(int j=0;j<curdata.data.size();j++)
			{
				int num=curdata.data.at(j);
				
				if(curdata.stress.count(j))
				{
//					MemDC.SelectObject(&stresspen);
					MemDC.SelectObject(&stressbrush);
					curdata.stress.erase(num);
				}
				else
				{
//					MemDC.SelectObject(&normalpen);
					MemDC.SelectObject(&normalbrush);
				}
				MemDC.Rectangle(j*BARSIZE+column,blockheight*(line+1)-num,j*BARSIZE+BARSIZE+column,blockheight*(line+1));
			}	
			
			if(toDraw.size() > 1 && running)
				toDraw.pop();
		}
		dc.BitBlt(0,0,drawrt.right,drawrt.bottom,&MemDC,0,0,SRCCOPY);

		MemDC.DeleteDC();
		MemBitmap.DeleteObject();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDrawsortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI CalcThread(LPVOID lpParameter)
{
	CDrawsortDlg* dlg=(CDrawsortDlg*)lpParameter;
	dlg->toDrawQueue.clear();
	for(int l=0;l<TYPE_MAX;l++)
	{
		dlg->toDrawQueue.push_back(queue<DrawData>());
	}
	
	int sizeq=dlg->toDrawQueue.size();
	
	vector<int> data;
	RECT drawrect;
	dlg->m_draw.GetClientRect(&drawrect);
	int blockheight=drawrect.bottom*2/dlg->toDrawQueue.size();
	int num=drawrect.right/(2*dlg->m_blocksize.GetPos());
	for(int i=0;i<num;i++)
	{
		data.push_back(rand()%blockheight);
	}
	
	dlg->MAOPAO(data);
	dlg->MAOPAO1(data);
	dlg->MAOPAO2(data);
	dlg->KUAISU(data);
	dlg->CHARU(data);
	dlg->ERFENCHARU(data);
	dlg->XIER(data);
	
	dlg->XUANZE(data);
	dlg->JINBIAOSAI(data);
	dlg->HEAP(data);
	dlg->MERGE(data);
	dlg->BUCKET(data);

	return 0;
};

void CDrawsortDlg::OnDraw() 
{
	CreateThread(NULL,0,CalcThread,this,0,NULL);
	// TODO: Add your control notification handler code her
}

void CDrawsortDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);

	CDialog::OnTimer(nIDEvent);
}


void CDrawsortDlg::MAOPAO(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();

	toDrawQueue.at(TYPE_MAOPAO).push(DrawData(data,set<int>(),step));

	for(int i=size-1;i > 0;i--)
	{
		step++;
		for(int j=0;j < i;j++)
		{
			step++;
			set<int> curset;
			curset.insert(size-j-1);
			curset.insert(size-j-2);
			
			if(data[size-j-1] < data[size-j-2])
			{
				int tmp=data[size-j-1];
				data[size-j-1]=data[size-j-2];
				data[size-j-2]=tmp;
			}
			toDrawQueue.at(TYPE_MAOPAO).push(DrawData(data,curset,step));
		}
	}

	toDrawQueue.at(TYPE_MAOPAO).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::MAOPAO1(const vector<int>& _data)
{//设置哨兵位监视某个点之后不需要交换
	vector<int> data=_data;
	int step=0;
	int size=data.size();


	toDrawQueue.at(TYPE_MAOPAO1).push(DrawData(data,set<int>(),step));

	for(int i=size-1;i > 0;i--)
	{
		step++;
		int flag=0;
		for(int j=0;j < i;j++)
		{
			step++;
			set<int> curset;
			curset.insert(size-j-1);
			curset.insert(size-j-2);
			
			if(data[size-j-1] < data[size-j-2])
			{
				int tmp=data[size-j-1];
				data[size-j-1]=data[size-j-2];
				data[size-j-2]=tmp;
				flag=j;
			}
			toDrawQueue.at(TYPE_MAOPAO1).push(DrawData(data,curset,step));
		}
		i=flag+1;
	}
	
	toDrawQueue.at(TYPE_MAOPAO1).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::MAOPAO2(const vector<int>& _data)
{//设置哨兵位监视不需要交换的位置，且进行双向冒泡排序
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	int begin=0,end=size-1;

	toDrawQueue.at(TYPE_MAOPAO2).push(DrawData(data,set<int>(),step));

	while(begin < end)
	{
		step++;
		int j;
		int flag;
		for(j=flag=end;j > begin;j--)
		{	
			step++;
			set<int> curset;
			curset.insert(j);
			curset.insert(j-1);

			if(data[j] < data[j-1])
			{
				int tmp=data[j];
				data[j]=data[j-1];
				data[j-1]=tmp;
				flag=j;
			}
			toDrawQueue.at(TYPE_MAOPAO2).push(DrawData(data,curset,step));
		}
		begin=flag;
		
		for(j=flag=begin;j < end;j++)
		{		
			step++;
			set<int> curset;
			curset.insert(j);
			curset.insert(j+1);

			if(data[j+1] < data[j])
			{
				int tmp=data[j+1];
				data[j+1]=data[j];
				data[j]=tmp;
				flag=j;
			}
			toDrawQueue.at(TYPE_MAOPAO2).push(DrawData(data,curset,step));
		}
		end=flag;
	}

	toDrawQueue.at(TYPE_MAOPAO2).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::KUAISU(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,set<int>(),step));

	stack<int> emulate;
	emulate.push(0);
	emulate.push(size-1);
	set<int> curset;

	while(!emulate.empty())
	{
		step++;
		int finish=emulate.top();
		emulate.pop();
		int start=emulate.top();
		emulate.pop();
		int begin=start,end=finish;
		int tmp=data[begin];
		
		while(begin < end)
		{
			step++;
			while(begin < end && data[end] >= tmp)
			{
				end--;

				step++;
				curset.clear();
				curset.insert(begin);
				curset.insert(end);
				toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,curset,step));
			}
			data[begin]=data[end];

			curset.clear();
			curset.insert(begin);
			curset.insert(end);			
			toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,curset,step));


			while(begin < end && data[begin] <= tmp)
			{
				begin++;

				step++;
				curset.clear();
				curset.insert(begin);
				curset.insert(end);
				toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,curset,step));
			}
			data[end]=data[begin];

			curset.clear();
			curset.insert(begin);
			curset.insert(end);			
			toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,curset,step));
		}

		data[begin]=tmp;
		
		curset.clear();
		curset.insert(begin);
		curset.insert(end);			
		toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,curset,step));

		if(start < begin-1)
		{
			emulate.push(start);
			emulate.push(begin-1);
		}
		if(finish > begin+1)
		{
			emulate.push(begin+1);
			emulate.push(finish);
		}
		
	}

	toDrawQueue.at(TYPE_KUAISU).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::CHARU(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();

	toDrawQueue.at(TYPE_CHARU).push(DrawData(data,set<int>(),step));

	for(int i=1;i<size;i++)
	{//把i插入已排序的n-i+1的数组中进行排序
		int tmp=data[i];
		
		if(tmp < data[i-1])
		{
			for(int j=i-1;j >= 0;j--)
			{
				step++;
				set<int> curset;
				curset.insert(i);
				curset.insert(j);

				if(tmp<data[j])
					data[j+1]=data[j];
				else
					break;

				toDrawQueue.at(TYPE_CHARU).push(DrawData(data,curset,step));
			}
			data[j+1]=tmp;
		}
	}

	toDrawQueue.at(TYPE_CHARU).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::ERFENCHARU(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_ERFENCHARU).push(DrawData(data,set<int>(),step));

	for(int i=1;i<size;i++)
	{//把i插入已排序的n-i+1的数组中进行排序
		step++;

		int tmp=data[i];
		
		if(tmp < data[i-1])
		{
			//在begin和end之间折半查找
			int begin=0;
			int end=i;
			int mid;
			while(begin < end)
			{
				step++;
				set<int> curset;
				curset.insert(begin);
				curset.insert(end);

				mid=(begin+end)/2;
				if(data[mid] < tmp)
					begin=mid+1;
				else if(data[mid] > tmp)
					end=mid;
				else
					break;

				toDrawQueue.at(TYPE_ERFENCHARU).push(DrawData(data,curset,step));
			}

			if(begin == end)
				mid=begin;
			
			for(int j=i-1;j >= mid;j--)
			{
				step++;
				set<int> curset;
				curset.insert(j);
				curset.insert(j+1);

				data[j+1]=data[j];

				toDrawQueue.at(TYPE_ERFENCHARU).push(DrawData(data,curset,step));
			}
			data[j+1]=tmp;

			toDrawQueue.at(TYPE_ERFENCHARU).push(DrawData(data,set<int>(),step));
		}
	}

	toDrawQueue.at(TYPE_ERFENCHARU).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::XIER(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_XIER).push(DrawData(data,set<int>(),step));

	int steparr[5]={1,5,19,41,109};//最优希尔步长
	int stepindx=4;
	while(stepindx)
	{
		if(steparr[stepindx]*2 < size)
			break;
		stepindx--;
	}
	
	while(stepindx >= 0)
	{
		step++;
		int stepl=steparr[stepindx];
		for(int i=0;i<stepl;i++)
		{
			step++;
			for(int j=i+stepl;j < size;j += stepl)
			{//对{i,i+step,i+2step,....}插入排序
				step++;
				int tmp=data[j];
				if(tmp < data[j-stepl])
				{
					int k=j-stepl;
					while(k >= i)
					{
						step++;
						set<int> curset;
						curset.insert(k);
						curset.insert(k+stepl);

						if(tmp < data[k])
							data[k+stepl]=data[k];
						else
							break;
						k -= stepl;

						toDrawQueue.at(TYPE_XIER).push(DrawData(data,curset,step));
					}
					data[k+stepl]=tmp;

					toDrawQueue.at(TYPE_XIER).push(DrawData(data,set<int>(),step));
				}
			}
		}
		stepindx--;
	}

	toDrawQueue.at(TYPE_XIER).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::XUANZE(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_XUANZE).push(DrawData(data,set<int>(),step));

	for(int i=0;i<size;i++)
	{
		step++;
		int minindx=i;
		for(int j=i+1;j<size;j++)
		{
			step++;
			set<int> curset;
			curset.insert(minindx);
			curset.insert(i);
			curset.insert(j);

			if(data[j] < data[minindx])
				minindx=j;

			toDrawQueue.at(TYPE_XUANZE).push(DrawData(data,curset,step));
		}
		int tmp=data[i];
		data[i]=data[minindx];
		data[minindx]=tmp;

		toDrawQueue.at(TYPE_XUANZE).push(DrawData(data,set<int>(),step));
	}

	toDrawQueue.at(TYPE_XUANZE).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::JINBIAOSAI(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_JINBIAOSAI).push(DrawData(data,set<int>(),step));

	int MAX=size;//只要不在数组索引中即可
	vector<int> treedata;//二叉树
	int volumn=2;
	
	while(volumn < size)
		volumn *= 2;
	
	int newsize=volumn-1+size;//计算完全二叉树容量
	
	vector<int> copydata;
	
	treedata.resize(newsize);
	for(int i=0;i < volumn-1;i++)
	{
		treedata[i]=MAX;
	}
	
	for(i=0;i<size;i++)
	{
		treedata[i+volumn-1]=i;
	}
	
	//生成初始树  总长度为newsize
	for(i=volumn-2;i >= 0;i--)
	{
		step++;
		if(i*2+1 < newsize && treedata[i*2+1] != MAX)//有左子树且合法
		{//如果没有左子树，根据完全二叉树性质，肯定没有右子树，同理初始化时，如果左子树为MAX，那么右子树一定是MAX
			int tochange=i*2+1;
			if(i*2+2 < newsize && treedata[i*2+2] != MAX)//如果有右子树
			{
				if(data[treedata[i*2+2]] < data[treedata[tochange]])
					tochange=i*2+2;
			}
			treedata[i]=treedata[tochange];
		}
	}
	
	int left=size;
	while(left--) //做size次，每次从顶端取得最小值并相应吧索引值置为无效
	{
		step++;
		copydata.push_back(data[treedata[0]]);
		int index=volumn-1+treedata[0];//索引位置
		treedata[index]=MAX;
		//从索引位置往父节点递归
		do
		{
			index = (index-1)/2;//得到父节点
			treedata[index]=MAX;//重置父节点
			
			set<int> curset;
	
			if(index*2+1 < newsize)//有左子树
			{//仍然是如果没有左子树肯定没有右子树
				int tochange=index*2+1;//可能为MAX
				if(index*2+2 < newsize && treedata[index*2+2] != MAX)
				{
					if(treedata[tochange] == MAX || 
						(treedata[tochange] != MAX && data[treedata[tochange]] > data[treedata[index*2+2]]))
						tochange=index*2+2;

					if(treedata[index*2+1] != MAX)
					curset.insert(treedata[index*2+1]);
					curset.insert(treedata[index+2+2]);
				}
				treedata[index]=treedata[tochange];//可能赋值为MAX
			}
			
			step++;
			vector<int> temp=copydata;
			//为了便于了解排序过程，这里将已经排出序的部分和其他部分进行组合
			for(int l=0;l<size;l++)
			{
				if(treedata[l+volumn-1] != MAX)
					temp.push_back(data[treedata[l+volumn-1]]);
			}
			toDrawQueue.at(TYPE_JINBIAOSAI).push(DrawData(temp,curset,step));
		} 
		while(index);
	} 

	data=copydata;

	toDrawQueue.at(TYPE_JINBIAOSAI).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::HEAP(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_HEAP).push(DrawData(data,set<int>(),step));

	int j;
	set<int> curset;
	//初始化大顶堆
	for(int i=size/2-1;i >= 0;i--)
	{//从最后节点的父节点往根节点遍历
		step++;

		while(2*i+1<size)
		{			
			j=2*i+1;
			curset.clear();
			curset.insert(j);
			curset.insert(j+1);
			curset.insert(i);
			
			step++;
			toDrawQueue.at(TYPE_HEAP).push(DrawData(data,curset,step));

			
			if(j+1<size && data[j]<data[j+1])
				j++;
			if(data[i] < data[j])
			{
				int t=data[i];
				data[i]=data[j];
				data[j]=t;
				i=j;
			}
			else
				break;
		}
	}
	
	for(i=size-1;i > 0;i--)
	{//每次把大顶堆顶部最大值插在当前位置
		//交换最大值和当前位置
		int temp=data[i];
		data[i]=data[0];
		data[0]=temp;
		int k=0;
		
		curset.clear();
		curset.insert(i);
		curset.insert(0);
		
		step++;
		toDrawQueue.at(TYPE_HEAP).push(DrawData(data,curset,step));
		//调整堆使其为大顶堆
		
		while(2*k+1<i)
		{
			j=2*k+1;

			curset.clear();
			curset.insert(j);
			curset.insert(j+1);
			curset.insert(k);
			
			step++;
			toDrawQueue.at(TYPE_HEAP).push(DrawData(data,curset,step));

			if(j+1<i && data[j]<data[j+1])
				j++;
			if(data[k] < data[j])
			{
				int t=data[k];
				data[k]=data[j];
				data[j]=t;
				k=j;
			}
			else
				break;
		}
	}

	toDrawQueue.at(TYPE_HEAP).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::MERGE(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_MERGE).push(DrawData(data,set<int>(),step));

	set<int> curset;
	vector<int> tmp;
	
	for(int stepl=1;stepl < size;stepl *= 2)
	{
		int leftbegin,leftend,rightbegin,rightend;
		tmp.clear();
		
		for(leftbegin=0;leftbegin < size;leftbegin =leftend+stepl+1)
		{//将[leftbegin,leftend]和[rightbegin,rightend]合并
			leftend=leftbegin+stepl-1;
			rightbegin=leftend+1;
			step++;
			if(rightbegin >= size)
			{
				curset.clear();
				while(leftbegin <= leftend)
				{
					step++;
					curset.insert(leftbegin);
					tmp.push_back(data[leftbegin++]);
				}
				toDrawQueue.at(TYPE_MERGE).push(DrawData(data,curset,step));

				break;
			}
			rightend=rightbegin+stepl-1 < size-1?rightbegin+stepl-1:size-1;
			//归并

			curset.clear();
			while(leftbegin <= leftend && rightbegin <= rightend)
			{
				step++;

				curset.insert(leftbegin);
				curset.insert(rightbegin);

				if(data[leftbegin] > data[rightbegin])
				{
					tmp.push_back(data[rightbegin++]);
				}
				else
				{
					tmp.push_back(data[leftbegin++]);
				}
			}
			if(leftbegin <= leftend)
			{
				while(leftbegin <= leftend)
				{
					step++;
					curset.insert(leftbegin);
					tmp.push_back(data[leftbegin++]);
				}
			}
			else
			{
				while(rightbegin <= rightend)
				{
					step++;
					curset.insert(rightbegin);
					tmp.push_back(data[rightbegin++]);
				}
			}

			toDrawQueue.at(TYPE_MERGE).push(DrawData(data,curset,step));
		}
		
		data=tmp;
		toDrawQueue.at(TYPE_MERGE).push(DrawData(data,set<int>(),step));
	}

	toDrawQueue.at(TYPE_MERGE).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::BUCKET(const vector<int>& _data)
{
	vector<int> data=_data;
	int step=0;
	int size=data.size();
	
	toDrawQueue.at(TYPE_BUCKET).push(DrawData(data,set<int>(),step));

#define radix 16//根据桶排序算法可知复杂度为：log(radix,max{data})*(3*datasize+radix)
	//在datasize为25-500，max{data}=100时，可以得到最优radix为16
	int p[]={0,4,8,12};
#define get_part(x,y) (x>>p[y]&radix-1)
	vector<int> bucket;
	bucket.resize(size);
    int count[radix];

	set<int> curset;
	
	int i,j;
	
    for(i=0;i<2;++i)
    {
		step++;
        memset(count,0,sizeof(int)*radix);
		curset.clear();
		
        for(j=0;j<size;++j)
        {
			step++;
            count[get_part(data[j],i)]++;
        }
		
        for(j=1;j<radix;++j)
        {
			step++;
            count[j]+=count[j-1];
        }
		
		bucket=data;

        for(j=size-1;j>=0;--j)
        {
			step++;
            int k=get_part(data[j],i);
            bucket[count[k]-1]=data[j];
            count[k]--;

			curset.insert(count[k]);
			toDrawQueue.at(TYPE_BUCKET).push(DrawData(bucket,curset,step));
        }
		
		data=bucket;

		toDrawQueue.at(TYPE_BUCKET).push(DrawData(data,set<int>(),step));
    }
	
	toDrawQueue.at(TYPE_BUCKET).push(DrawData(data,set<int>(),step));
}

void CDrawsortDlg::OnCustomdrawVelocity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	SetTimer(1,m_velocity.GetPos(),NULL);

	*pResult = 0;
}

void CDrawsortDlg::OnGoon() 
{
	// TODO: Add your control notification handler code here
	running=true;
}

void CDrawsortDlg::OnPause() 
{
	// TODO: Add your control notification handler code here
	running=false;
}
