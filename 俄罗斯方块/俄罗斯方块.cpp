/* -----------------------------------------------------
//	�ļ���		:	J:\TEMP\1.cpp
//	������		:	lichao890427
//	����ʱ��	:	2012��2��13�� 21:40:15
//	��������	:	����˹����
//
//--------------------------------------------------*/
#pragma comment(lib,"winmm.lib")
#include <stdio.h>
#include <graphics.h>
#define M 14	//���泤��:�Է����������
#define SIZE 40 //�����С
#define NUM 7	//7�����
#define MATRIX 4//ÿ��������϶���4*4����
struct PT
{
	short x;
	short y;
};
bool istobottom(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2);
bool istoleft(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2);
bool istoright(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2);
void rotate(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2);
void linefull(short DATA[M+4][M],short* goal);
void main()
{
	short i,j;
	char s[10];
	short BOX[NUM][2][MATRIX]= {{0,0,0,0, 1,1,1,1},{1,0,0,0, 1,1,1,0},
								{0,0,1,0, 1,1,1,0},{0,1,0,0, 1,1,1,0},
								{0,1,1,0, 1,1,0,0},{1,1,0,0, 0,1,1,0},
								{1,1,0,0, 1,1,0,0}};
	short DATA[M+4][M];//ȫ������ ��������ͼƬ1~7
	initgraph(860,630);
	IMAGE img[NUM],frame1,frame2;
	PT pt1,pt2;//���ͼ�����Ͻ�λ�ú����½�λ��:0~MATRIX-1  ǰ��:ͼ�α��볯���¶���
	short pre=random(NUM),cur[MATRIX][MATRIX],count=0,later,goal=0,delta=1;
	short count2=0,HEIGHT=100/*��ǰ�ۼƸ߶�*/;
	bool begin=true,lose=false;
	for(i=0;i<NUM;i++)
	{
		sprintf(s,"%02d.jpg",i+1);
		getimage(img+i,s);
	}
	getimage(&frame1,"��A.jpg");
	getimage(&frame2,"��B.jpg");
	for(i=0;i<M+4;i++)
		for(j=0;j<M;j++)
			DATA[i][j]=0;
	//��ʼ����Ϸ����

	setfillcolor(WHITE);
	putimage(0,0,&frame1);

	setfont(16,16,"����",0,0,900,false,false,false);
	setcolor(WHITE);
	outtextxy(635,40,"��ǰ�÷�:");
	outtextxy(635,100,"�¸�ͼ��:");
	outtextxy(635,240,"��Ϸ�����С:");
	setcolor(GREEN);
	outtextxy(650,280,"18��18��");
	setcolor(WHITE);
	outtextxy(635,320,"��Ϸ����:");
	setcolor(LIGHTCYAN);
	outtextxy(650,420,"F2���¿�ʼ");
	outtextxy(650,440,"����ת");
	outtextxy(650,460,"������");
	outtextxy(650,480,"������");
	outtextxy(650,500,"������");
	outtextxy(650,520,"F3�˳�");
	outtextxy(650,540,"�ո����ͣ");
	putimage(620,0,&frame2,SRCERASE);
	outtextxy(200,200,"���������ʼ!");
	getch();
	setcolor(WHITE);
	BeginBatchDraw();
	while(1)
	{
		if(begin)//׼�����·���
		{
			later=pre;
			for(i=0;i<MATRIX;i++)
			{
				cur[0][i]=0;
				cur[1][i]=0;
				cur[2][i]=BOX[later][0][i];
				cur[3][i]=BOX[later][1][i];
			}
			begin=false;
			pt1.x=3,pt1.y=6;
			if(later==0) pt2.x=4,pt2.y=10;
			else if(later==6) pt2.x=5,pt2.y=8;
			else pt2.x=5,pt2.y=9;
			randomize();
			pre=random(NUM);
			bar(635,140,800,240);

			for(i=0;i<2;i++)
				for(j=0;j<MATRIX;j++)
					if(BOX[pre][i][j])
						putimage(650+SIZE*j,140+SIZE*i,img+pre);
						else bar(650+SIZE*j,140+SIZE*i,650+SIZE*(j+1),140+SIZE*(i+1));
			putimage(620,0,&frame2,SRCERASE);
			putimage(620,0,&frame2,SRCERASE);
		}

		for(i=-2;i<M;i++)
		{
			for(j=0;j<M;j++)
			{
				if(DATA[i+4][j])//�����Ϊ��
					putimage(30+SIZE*j,30+SIZE*i,img+DATA[i+4][j]-1);
				else
					bar(30+SIZE*j,30+SIZE*i,30+SIZE*j+SIZE,30+SIZE*i+SIZE);
			}
		}

		if(keystate(VK_DOWN)) count+=5;
		if(keystate(VK_LEFT))
			if(!istoleft(DATA,cur,pt1,pt2)) pt1.y--,pt2.y--;
		if(keystate(VK_RIGHT))
			if(!istoright(DATA,cur,pt1,pt2)) pt1.y++,pt2.y++;
		if(keystate(VK_UP)) rotate(DATA,cur,pt1,pt2);
		if(keystate(VK_F3)) break;
		if(keystate(VK_F2))
		{
start1:		begin=true,goal=0,delta=1,count2=0,HEIGHT=100;
			for(i=0;i<M+4;i++)
				for(j=0;j<M;j++)
					DATA[i][j]=0;
			setfont(16,16,"����",0,0,900,false,false,false);
			lose=false;
		}
		if(keystate(VK_SPACE)) 
		{
			outtextxy(200,200,"���������ʼ!");
			getch();getch();getch();getch();
		}
		if(istobottom(DATA,cur,pt1,pt2))
		{//��鷽���Ƿ񵽵ײ�
			begin=true;
			for(i=pt2.x-1;i>=pt1.x;i--)
				for(j=pt1.y;j<pt2.y;j++)
				{
					DATA[i][j]+=(later+1)*cur[i-pt2.x+4][j-pt1.y];//��ֹdata����λ���޸�
				}
			HEIGHT=pt1.x;
		}
		linefull(DATA,&goal);
		if(HEIGHT<4) 
		{
			lose=TRUE;
			setfont(30,30,"����",0,0,900,false,false,false);
			setcolor(LIGHTCYAN);
			outtextxy(150,100,"YOU LOSE!");
			outtextxy(100,200,"�����������!");
			PlaySound("lose.wav",NULL,SND_FILENAME|SND_ASYNC);
			getch();
			goto start1;
		}
		for(i=0;i<MATRIX;i++)
			for(j=0;j<MATRIX;j++)
				if(cur[i][j])
					putimage(30+SIZE*(j+pt1.y),30+SIZE*(i+pt2.x-8),img+later);
		if(!lose) count+=delta;
		if(count>10) 
		{
			count-=10,pt1.x++,pt2.x++;
			sprintf(s,"%5d",goal);
			putimage(620,0,&frame2,SRCERASE);
			setcolor(GREEN);
			outtextxy(640,70,s);
			sprintf(s,"�Ѷȵȼ�:%03d",delta);
			outtextxy(630,360,s);
			putimage(620,0,&frame2,SRCERASE);
		}
		if(!lose)
		{
			count2++;
			if(count2==600) delta++;
		}
		delay(100);
	}
	EndBatchDraw();
	getch();
	closegraph();
}
bool istobottom(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2)
{
	short i,j;
	if(pt2.x==M+4)/*����Ļ�ױ߿�*/return true;
	for(i=pt2.x-1;i>=pt1.x;i--)
		for(j=pt1.y;j<pt2.y;j++)
			if(cur[i-pt2.x+4][j-pt1.y] && DATA[i+1][j]) return true;
	return false;
}
bool istoleft(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2)
{//�Ƿ���߼���
	short i,j;
	if(pt1.y==0)/*����Ļ��߿�*/return true;
	for(j=pt1.y;j<pt2.y;j++)
		for(i=pt1.x;i<pt2.x;i++)
			if(cur[i-pt2.x+4][j-pt1.y] && DATA[i][j-1]) return true;
	return false;
}
bool istoright(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2)
{//�Ƿ��ұ߼���
	short i,j;
	if(pt2.y==M)/*����Ļ�ұ߿�*/return true;
	for(j=pt2.y-1;j>=pt1.y;j--)
		for(i=pt1.x;i<pt2.x;i++)
			if(cur[i-pt2.x+4][j-pt1.y] && DATA[i][j+1]) return true;
	return false;
}
void rotate(short DATA[M+4][M],short cur[MATRIX][MATRIX],PT& pt1,PT& pt2)
{//������ת
	short temp[MATRIX][MATRIX],i,j,p1x=pt2.x-pt2.y+pt1.y,p2y=pt1.y+pt2.x-pt1.x;
	short tt,sum;
	for(i=0;i<MATRIX;i++)
		for(j=0;j<MATRIX;j++)
			temp[j][MATRIX-1-i]=cur[i][j];
	if(p2y>M) return;
	for(i=MATRIX-1,tt=0,sum=0;i>0;i--)
	{
		for(j=0;j<MATRIX;j++)
			if(temp[i][j]==0) tt++;
		if(tt==MATRIX) tt=0,sum++;
		else break;
	}
	if(sum)
	{//���·�����
		for(i=MATRIX-1;i>=sum;i--)
			for(j=0;j<MATRIX;j++)
				temp[i][j]=temp[i-sum][j];
		for(i=0;i<sum;i++)
			for(j=0;j<MATRIX;j++)
				temp[i][j]=0;
	}
	for(j=0,tt=0,sum=0;j<MATRIX-1;j++)
	{
		for(i=0;i<MATRIX;i++)
			if(temp[i][j]==0) tt++;
		if(tt==MATRIX) tt=0,sum++;
		else break;
	}
	if(sum)
	{//���󷽶���
		for(j=0;j<MATRIX-sum;j++)
			for(i=0;i<MATRIX;i++)
				temp[i][j]=temp[i][j+sum];
		for(j=MATRIX-1;j>=MATRIX-sum;j--)
			for(i=0;i<MATRIX;i++)
				temp[i][j]=0;
	}
	for(j=p2y-1;j>=pt1.y;j--)//����Ƿ���ת�������ͻ
		for(i=p1x;i<pt2.x;i++)
			if(temp[i-pt2.x+4][j-pt1.y] && DATA[i][j]) return;
	for(i=0;i<MATRIX;i++)
		for(j=0;j<MATRIX;j++)
			cur[i][j]=temp[i][j];
	pt1.x=p1x;pt2.y=p2y;
}
void linefull(short DATA[M+4][M],short* goal)
{
	int i,j,sum=0,m,n;
	for(i=M+3;i>4;)
	{
		sum=0;
		for(j=0;j<M;j++)
			if(DATA[i][j]) sum++;
		if(sum==M)
		{
			*goal+=10;
			for(m=i;m>0;m--)
				for(n=0;n<M;n++)
					DATA[m][n]=DATA[m-1][n];
			PlaySound("start.wav",NULL,SND_FILENAME|SND_ASYNC);
		}
		else i--;
	}
}