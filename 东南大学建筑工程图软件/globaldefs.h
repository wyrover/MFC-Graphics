#pragma once

#include <afxtempl.h>

#define margin 30
#define SHIFTED 0x8000 

#define WM_MYUPDATEELEMENTLIST (WM_USER+66)

enum PROPERTYTYPE
{
	PARAM_ADDBEAM,//添加梁配置
	PARAM_ADDBOARD,//添加板配置
	PARAM_MODBEAMSECTION,//修改梁截面参数
	PARAM_MODBOARDSECTION,//修改板截面参数
	PARAM_MODLAYERHEIGHT,//修改层高
	PARAM_MODBEAMRODARRAY,//修改梁下杆阵列
	PARAM_MODDISTANCE,//修改轴间距
};

enum STATE//一次仅处于一个状态
{
	STATE_NONE,				//无选择选择
	SELECT_LINE,			//选择坐标轴	图像外层选取
	SELECT_ELEMENT,			//选择元素		图像内选取
	CREATE_PBEAM_STEP1,		//创建主梁第一个点
	CREATE_SBEAM_STEP1,		//创建次梁第一个点
	CREATE_POLYZONE_STEPN,	//创建区域，通过闭合曲线确定是否结束
	CREATE_SCISSORSLINE,	//剪刀撑
};

enum ELEMENTTYPE//一次只可选择一个
{//选中色 RGB(8,46,84)
	ELEMENT_NONE,			//空对象
	ELEMENT_AXISLINE,		//选择坐标轴		图像外层选取		点划线			宽度1					RGB(255,0,0)
	ELEMENT_SECTIONLINE,	//选择截面线		图像外层选取		实线			宽度1					RGB(255,0,0)
	ELEMENT_PILLAR,
	ELEMENT_MPILLAR,		//选择明柱			图像内选取			实体正方形实线							RGB(128,42,42)
	ELEMENT_APILLAR,
	ELEMENT_BEAM,
	ELEMENT_PBEAM,			//选择主梁			图像内选取			双实线			宽度:无属性1 有属性3	RGB(160,82,45)
	ELEMENT_SBEAM,			//选择次梁			图像内选取			双实线			宽度:无属性1 有属性3	RGB(244,164,96)
	ELEMENT_RECTZONE,		//选择内部区域		图像内选取			实体正方形实线	有属性RGB(245,222,179)	无属性RGB(245,222,255)
	ELEMENT_POLYZONE,		//选择内部区域		图像内选取			实体多边形实线	有属性RGB(245,222,179)	无属性RGB(245,222,255)
	ELEMENT_BOARD,
	ELEMENT_ZONEOUTLINE,	//多边形内部区域构成线					实线			宽度1					RGB(8,46,84)
	ELEMENT_POLYLINE,
};

struct archtemplate
{
	ELEMENTTYPE type;
	CString tempname;
	union
	{
		struct  
		{	
			int beamheight;
			int beamwidth;
		}beam;
		struct  
		{
			int boardthick;
		}board;
	}tem;
};

struct Params
{
	int		paramtype;
	CString paramname1;
	CString param1;
	CString paramname2;
	CString param2;
	CString paramname3;
	CString param3;
	CArray<CString,CString> chooselist;
};

#define HORIZONTAL	TRUE
#define VERTICAL	FALSE

struct MYRECT
{
	int left;
	int right;
	int top;
	int bottom;
};

struct MYPOINT
{
	int x;
	int y;
};

struct SCISSORSLINE
{
	int beginx;
	int beginy;
	int endx;
	int endy;

	SCISSORSLINE(int x1,int y1,int x2,int y2)
	{
		beginx=x1;
		beginy=y1;
		endx=x2;
		endy=y2;
	}
};

class BASE_ELEMENT//ELEMENT_NONE 
{
public:
	BASE_ELEMENT(ELEMENTTYPE type,int index):Type(type),Index(index){};

	int GetIndex() const
	{
		return Index;
	};

	ELEMENTTYPE GetType() const
	{
		return Type;
	};

	void SetUnused()
	{
		IsUsed=FALSE;
	};

	void SetUsed()
	{
		IsUsed=TRUE;
	};

	BOOL IsCurrentlyUsed() const
	{
		return IsUsed;
	}

protected:
	BOOL IsUsed;//元素删除只需标注为FALSE即可
	ELEMENTTYPE Type;
	int Index;//用于删除时的查找元素，可不连续，由全局序号分配唯一标识    同时该index也是该元素在CArray中的位置
};

class LINE_PROPERTY:public BASE_ELEMENT
{
public:
	LINE_PROPERTY(ELEMENTTYPE type,int index,BOOL IsHorizontal):BASE_ELEMENT(type,index)
	{
		this->IsHorizontal=IsHorizontal;
	};

private:
	BOOL IsHorizontal;
};

class PILLAR_PROPERTY:public BASE_ELEMENT//ELEMENT_MPILLAR
{
public:
	PILLAR_PROPERTY(ELEMENTTYPE type,int index,int x0,int y0):BASE_ELEMENT(type,index)
	{
		Lineindex[0]=x0;
		Lineindex[1]=y0;
		SetUnused();
	};

	BOOL RelevantWithLine(BOOL IsHorizontal,int index) const
	{
		if((IsHorizontal && index == Lineindex[0]) || (!IsHorizontal && index == Lineindex[1]))
			return TRUE;
		return FALSE;
	};

	BOOL GetRegion(MYRECT* rect,const CArray<int,int>& horizon, const CArray<int,int>& verti) const
	{
		if(Lineindex[0] >= 0 && Lineindex[0] < horizon.GetSize() && Lineindex[1] >= 0 && Lineindex[1] < verti.GetSize())
		{	
			rect->top=verti.GetAt(Lineindex[1]);
			rect->bottom=verti.GetAt(Lineindex[1]);
			rect->right=horizon.GetAt(Lineindex[0]);
			rect->left=horizon.GetAt(Lineindex[0]);
			return TRUE;
		}
		return FALSE;
	};

private:
	int		Lineindex[2];//被交于的2线序号 必须一横一纵  [0]horizontal [1]vertical
};

class SELECTABLE:public BASE_ELEMENT
{
public:
	SELECTABLE(ELEMENTTYPE type,int index):BASE_ELEMENT(type,index)
	{
		Initialized=FALSE;
		TemplateName="";
		LayerHeight=0;
	}
public:
	BOOL	Initialized;//是否赋予属性
	CString TemplateName;//模板名称
	int		LayerHeight;//层高
};

class BEAM_PROPERTY:public SELECTABLE//ELEMENT_PBEAM ELEMENT_SBEAM
{
public:
	BEAM_PROPERTY(ELEMENTTYPE type,int index,int x1,int y1,int x2,int y2):SELECTABLE(type,index)//参数检验交给调用者
	{//x1 y1为构成第一个交叉点的2个坐标轴序号	x2 y2为构成第二个交叉点的2个坐标轴序号
		IsHorizontal=(x1 != x2)?TRUE:FALSE;//when x1 == x2 so y1 != y2       when x1 != x2 so y1 == y2
		if(IsHorizontal)
		{
			if(x1 < x2)
			{
				Lineindex[0]=x1;
				Lineindex[1]=y1;
				Lineindex[2]=x2;
				Lineindex[3]=y2;
			}
			else
			{
				Lineindex[0]=x2;
				Lineindex[1]=y2;
				Lineindex[2]=x1;
				Lineindex[3]=y1;
			}
		}
		else
		{
			if(y1 < y2)
			{
				Lineindex[0]=x1;
				Lineindex[1]=y1;
				Lineindex[2]=x2;
				Lineindex[3]=y2;
			}
			else
			{
				Lineindex[0]=x2;
				Lineindex[1]=y2;
				Lineindex[2]=x1;
				Lineindex[3]=y1;
			}
		}
		Initialized=FALSE;
		SetUsed();
	};

	BOOL RelevantWithLine(BOOL IsHorizontal,int index) const
	{
		if(IsHorizontal)//水平间距 垂直方向
		{
			if(Lineindex[0] == index || Lineindex[2] == index)
				return TRUE;
		}
		else
		{
			if(Lineindex[1] == index || Lineindex[3] == index)
				return TRUE;
		}
		return FALSE;
	};

	BOOL RelevantWithLine(BOOL IsHorizontal,int index1,int index2) const
	{
		if(index1>index2)
		{
			int tmp=index1; 
			index1=index2;
			index2=tmp;
		}
		if(IsHorizontal)//水平间距 垂直方向
		{
			if(Lineindex[3] >= index2 && Lineindex[1] <= index1)
				return TRUE;
		}
		else
		{
			if(Lineindex[2] >= index2 && Lineindex[0] <= index1)
				return TRUE;
		}
		return FALSE;
	}

	int GetCommonLine(BOOL IsHorizontal) const
	{
		if(IsHorizontal)
			return Lineindex[1];
		else 
			return Lineindex[0];
	}

	BOOL GetRegion(MYRECT* rect,const CArray<int,int>& horizon,const CArray<int,int>& verti) const
	{
		if(Lineindex[0] >= 0 && Lineindex[0] <= horizon.GetSize() && Lineindex[1] >= 0 && Lineindex[1] <= verti.GetSize() &&
			Lineindex[2] >= 0 && Lineindex[2] <= horizon.GetSize() && Lineindex[3] >= 0 && Lineindex[3] <= verti.GetSize())
		{
			rect->left=horizon.GetAt(Lineindex[0]);
			rect->right=horizon.GetAt(Lineindex[2]);
			rect->bottom=verti.GetAt(Lineindex[1]);
			rect->top=verti.GetAt(Lineindex[3]);
		}
		return TRUE;
	};

private:
	int		Lineindex[4];//下上或左右4个坐标轴序号 顺序横纵横纵
public:
	BOOL	IsHorizontal;//方向
	int		BeamHeight;//梁高
	int		BeamWidth;//梁宽
	CArray<int,int> RodArrayHor;//杆阵列
	CArray<int,int> RodArrayVer;//杆阵列
};

class SELECTABLE_BOARD:public SELECTABLE
{
public:
	SELECTABLE_BOARD(ELEMENTTYPE type,int index):SELECTABLE(type,index)
	{
		Thickness=0;
	}
public:
	int		Thickness;//板厚
};

class RECTZONE_PROPERTY:public SELECTABLE_BOARD//ELEMENT_ZONE
{//先根据坐标生成交点矩阵，然后根据用户需求增加删除
public:
	RECTZONE_PROPERTY(ELEMENTTYPE type,int index,int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3):SELECTABLE_BOARD(type,index)
	{	//x0 y0为构成左下交叉点的2个坐标轴序号		x1 y1为构成右下交叉点的2个坐标轴序号	
		//x2 y2为构成右上交叉点的2个坐标轴序号		x3 y3为构成左上交叉点的2个坐标轴序号      由于是自动生成的，因此无需考虑数据合法性，只需要按照顺序即可
		Lineindex[0]=x0;
		Lineindex[1]=y0;
		Lineindex[2]=x1;
		Lineindex[3]=y1;
		Lineindex[4]=x2;
		Lineindex[5]=y2;
		Lineindex[6]=x3;
		Lineindex[7]=y3;
		Initialized=FALSE;
		SetUsed();
	};

	BOOL RelevantWithLine(BOOL IsHorizontal,int index) const
	{
		if(IsHorizontal)//水平间隔线  垂直方向
		{
			for(int i=0;i<4;i++)
			{
				if(Lineindex[2*i] == index)
					return TRUE;
			}
		}
		else
		{
			for(int i=0;i<4;i++)
			{
				if(Lineindex[2*i+1] == index)
					return TRUE;
			}
		}
		return FALSE;
	};

	void GetRangeByAxis(int& hormin,int& hormax,int& vermin,int& vermax)
	{
		hormin=INT_MAX;
		hormax=0;
		vermin=INT_MAX;
		vermax=0;
		for(int i=0;i<4;i++)
		{
			if(Lineindex[2*i] < hormin)
				hormin=Lineindex[2*i];
			if(Lineindex[2*i] > hormax)
				hormax=Lineindex[2*i];
			if(Lineindex[2*i+1] < vermin)
				vermin=Lineindex[2*i+1];
			if(Lineindex[2*i+1] > vermax)
				vermax=Lineindex[2*i+1];
		}
	}

	BOOL GetOneSideRangeSides(BOOL IsHorizontal,int& min,int& max)//水平方向(min max定垂直界) | 垂直方向(min max定水平界) 切割
	{
		int minlim=min,maxlim=max;
		int hormin,hormax,vermin,vermax;
		GetRangeByAxis(hormin,hormax,vermin,vermax);
		if(IsHorizontal)//水平方向
		{
			if(minlim != hormin || maxlim != hormax)
				return FALSE;
			min=vermin;
			max=vermax;
		}
		else
		{
			if(minlim != vermin || maxlim != vermax)
				return FALSE;
			min=hormin;
			max=hormax;
		}
		return TRUE;
	}

	BOOL GetRegion(MYRECT* rect,const CArray<int,int>& horizon,const CArray<int,int>& verti) const
	{
		int i;
		bool IsValid=true;
		int min=INT_MAX,max=0;
		for(i=0;i<4;i++)
		{
			if(Lineindex[2*i] >= 0 && Lineindex[2*i] < horizon.GetSize())
			{			
			}
			else
			{
				IsValid=false;
				break;
			}
			if(horizon.GetAt(Lineindex[2*i]) < min)
				min=horizon.GetAt(Lineindex[2*i]);
			if(horizon.GetAt(Lineindex[2*i]) > max)
				max=horizon.GetAt(Lineindex[2*i]);
		}
		rect->left=min;
		rect->right=max;
		if(!IsValid)
			return FALSE;
		min=INT_MAX,max=0;
		for(i=0;i<4;i++)
		{
			if(Lineindex[2*i+1] >= 0 && Lineindex[2*i+1] < verti.GetSize())
			{		
			}
			else
			{
				IsValid=false;
				break;
			}
			if(verti.GetAt(Lineindex[2*i+1]) < min)
				min=verti.GetAt(Lineindex[2*i+1]);
			if(verti.GetAt(Lineindex[2*i+1]) > max)
				max=verti.GetAt(Lineindex[2*i+1]);
		}
		if(!IsValid)
			return FALSE;
		rect->bottom=min;
		rect->top=max;
		return TRUE;
	};

private:
	int		Lineindex[8];//确定该区域的 4个坐标轴序号顺时针或逆时针序    顺序横纵横纵横纵横纵
};

class POLYZONE_PROPERTY:public SELECTABLE_BOARD//ELEMENT_ZONE
{//根据用户需求合并
	
public:
	POLYZONE_PROPERTY(ELEMENTTYPE type,int index,const CArray<int,int>& array):SELECTABLE_BOARD(type,index)
	{//array为围成区域的各点横纵坐标序号
		LineArray.Copy(array);
		Initialized=FALSE;
		SetUsed();
	};
	
	BOOL IsExist(const CArray<int,int>& array) const//若序号完全相重则重复
	{
		int sum1=0,sum2=0,i;
		if(array.GetSize() == LineArray.GetSize())
		{
			for(i=0;i<LineArray.GetSize();i++)
			{
				sum1+=LineArray.GetAt(i);
			}
			for(i=0;i<array.GetSize();i++)
			{
				sum2+=array.GetAt(i);
			}
			if(sum1 == sum2)//做粗略判断
				return TRUE;
		}
		return FALSE;
	};
	
	BOOL RelevantWithLine(BOOL IsHorizontal,int index) const
	{
		for(int i=IsHorizontal?0:1;i<LineArray.GetSize();i+=2)
		{
			if(index == LineArray.GetAt(i))
				return TRUE;
		}
		return FALSE;
	};
	
	const CArray<int,int>& GetData() const
	{
		return LineArray;
	}
	
	void GetRangeByAxis(int& hormin,int& hormax,int& vermin,int& vermax)
	{
		hormin=INT_MAX;
		hormax=0;
		vermin=INT_MAX;
		vermax=0;
		for(int i=0;i<LineArray.GetSize()/2;i++)
		{
			if(LineArray[2*i] < hormin)
				hormin=LineArray[2*i];
			if(LineArray[2*i] > hormax)
				hormax=LineArray[2*i];
			if(LineArray[2*i+1] < vermin)
				vermin=LineArray[2*i+1];
			if(LineArray[2*i+1] > vermax)
				vermax=LineArray[2*i+1];
		}
	}

	BOOL GetOneSideRangeSides(BOOL IsHorizontal,IN OUT int& min,IN OUT int& max)//水平方向(min max定垂直界) | 垂直方向(min max定水平界) 切割
	{
		int minlim=min,maxlim=max;
		int hormin,hormax,vermin,vermax;
		GetRangeByAxis(hormin,hormax,vermin,vermax);
		if(IsHorizontal)//水平方向
		{
			if(minlim < vermin || maxlim > vermax)
				return FALSE;
			min=hormax;
			max=hormin;
			for(int i=0;i<LineArray.GetSize()/2;i++)
			{
				if(LineArray[2*i+1] == minlim || LineArray[2*i+1] == maxlim)
				{
					if(LineArray[2*i]<min)
						min=LineArray[2*i];
					if(LineArray[2*i]>max)
						max=LineArray[2*i];
				}
			}
		}
		else
		{
			if(minlim < hormin || maxlim > hormax)
				return FALSE;
			min=vermax;
			max=vermin;
			for(int i=0;i<LineArray.GetSize()/2;i++)
			{
				if(LineArray[2*i] == minlim || LineArray[2*i] == maxlim)
				{
					if(LineArray[2*i+1]<min)
						min=LineArray[2*i];
					if(LineArray[2*i+1]>max)
						max=LineArray[2*i];
				}
			}
		}
		return TRUE;
	}

private:
	CArray<int,int>		LineArray;
};


class ZONEOUTLINE_PROPERTY:public BASE_ELEMENT//围成POLYZONE的轮廓线，用选择线标出
{//判断是否闭合
public:
	ZONEOUTLINE_PROPERTY(ELEMENTTYPE objtype,int x,int y):BASE_ELEMENT(ELEMENT_POLYLINE,0)
	{
		ObjType=objtype;
		LineArray.Add(x);
		LineArray.Add(y);
	};

	void AddPoint(int x,int y)
	{
		LineArray.Add(x);
		LineArray.Add(y);
	}

	BOOL IsClosedLine(int x,int y)
	{
		if(LineArray[0] == x && LineArray[1] == y)
			return TRUE;
		return FALSE;
	}

	ELEMENTTYPE GetObjType()
	{
		return ObjType;
	}

	BASE_ELEMENT* ChangeToObjElement(int index)
	{
		switch(ObjType)
		{
			case ELEMENT_PBEAM:
				if(LineArray.GetSize() == 4)
				{
					if((LineArray[0] == LineArray[2] && LineArray[1] != LineArray[3]) || 
							(LineArray[0] != LineArray[2] && LineArray[1] == LineArray[3]))
						return new BEAM_PROPERTY(ELEMENT_PBEAM,index,LineArray[0],LineArray[1],LineArray[2],LineArray[3]);
				}
				break;

			case ELEMENT_SBEAM:
				if(LineArray.GetSize() == 4)
				{
					if((LineArray[0] == LineArray[2] && LineArray[1] != LineArray[3]) || 
							(LineArray[0] != LineArray[2] && LineArray[1] == LineArray[3]))
						return new BEAM_PROPERTY(ELEMENT_SBEAM,index,LineArray[0],LineArray[1],LineArray[2],LineArray[3]);
				}
				break;

			case ELEMENT_POLYZONE:
				if(LineArray.GetSize() >= 8 && (LineArray.GetSize()&1) == 0)
				{
					LineArray.RemoveAt(LineArray.GetSize()-2,2);
					return new POLYZONE_PROPERTY(ELEMENT_POLYZONE,index,LineArray);
				}
				break;

			default:
				return NULL;
		}
		return NULL;
	}

	const CArray<int,int>& GetData() const
	{
		return LineArray;
	}

private:
	CArray<int,int>	LineArray;//端点坐标对应横纵轴序号
	ELEMENTTYPE		ObjType;//将会形成的元素类型
};
