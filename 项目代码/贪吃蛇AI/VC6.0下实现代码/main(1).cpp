#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#define ROW 10
#define COL  10

//枚举
enum game
{
	SPACE, WALL, SNAKE, FOOD, HEAD,MAKE//空地  墙  蛇  食物 蛇头 标记(bfs)
};

//计算方向（bfs）
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}}; //右 左 下 上

//寻路栈
struct node
{
	int x;
	int y;	
	int headnum;//记录上一个坐标点的下标

};
node que[10000];  //记录所有查找的坐标点
/*******************************************全局变量*******************************************************/
int mode = 0;			//游戏模式
int score = 0;			//分数
DWORD t1, t2;			//定义两个时间控制移动速度
int map[ROW][COL];		//地图大小
COORD snake[1024];		//蛇			typedef struct _COORD {SHORT X;SHORT Y;} COORD, *PCOORD;
size_t SnakeSize;		//蛇的尺度		typedef unsigned int     size_t;
char SnakeDir;			//蛇移动方向


//寻路算法所需变量
COORD food;				//记录食物坐标
int maptest[ROW][COL];   //虚拟地图  虚拟地图标记寻找过的坐标，不影响真实地图的坐标
char location[10000];	//记录坐标   记录下虚拟地图中找到的最短路径，供真是地图使用
bool judge=false;       //是否开启寻路 (按下o改变自动或者手动模式，寻路状态下不能操作方向键.)
int startstep; //判断步数
int endstep;  //判断结束步数
COORD target;    //寻路的目标
/*******************************************全局函数***************************************************/
void start();	//开始界面
void chose();	//选择模式  0:普通模式  1：穿墙模式  2：无敌模式
void DrawMap();	//画图
void init();	//初始化游戏数据
void addfood();	  //添加食物
void ChangeDir();//修改蛇的方向
void move();//蛇的移动


//寻路算法所需函数
void bfs();//吃满屏策略
bool bfsfood(int startx,int starty);//自动寻路算法.   跟随食物
bool bfstail(int startx,int starty);// 跟随尾巴
bool bfsfindtail(int startx,int starty,int tailx,int taily,int maptest[ROW][COL]);//模拟蛇头跑到food后是否还能找到尾部
int basicbfs(int startx,int starty,int endx,int endy); //最原始的bfs
char direction(int x1,int y1,int x2,int y2); ///bfs计算方向 计算坐标点之间的方向


int main()
{
	initgraph(640,480);
	start();
	outtextxy(260,150,"->");		//初始默认模式一
	chose();
	init();
	while (1)
	{
		t2 = GetTickCount();	
		
		
		if (kbhit())
		{
			ChangeDir();
			move();
			t2 = GetTickCount();
			t1 = t2;
		}
		if (t2 - t1 > 500&&!judge)
		{
			move();
			t1 = t2;
		}
		if (t2 - t1 >5&&judge)  //开启寻路（可以修改自动寻路的快慢）
		{
			bfs();
			move();
			t1 = t2;
		}
		DrawMap();
	}
	getchar();
	closegraph();
	return 0;
}
void printfdata(){   //打印虚拟地图寻路数据
	for (int i = 0; i <ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
	//		printf("%d",maptest[i][j]);
		}
	//	printf("\n");
	}
	
	
}
void bfs(){
	if(bfsfood(snake[0].X,snake[0].Y))   //策略一：判断吃到food后能否找到尾巴，找到就最短路径往food跑
	{
	}
	else if(bfstail(snake[0].X,snake[0].Y)) //策略二：跟随尾巴走，走更远的路径(填补空格)
	{
	}
}
bool bfsfood(int startx,int starty)   //加上虚拟蛇判断如果虚拟蛇吃了food后找不到蛇尾巴就返回false
{	
	startstep=0;
	endstep=1;
	memcpy(maptest,map,sizeof(map));
	int head=0,tail=0;
	tail++;
	que[tail].x=startx;
	que[tail].y=starty;
	while(head<tail)
	{
		head++;
		for(int i=0;i<4;i++)
		{
			int xx=que[head].x+dir[i][0];
			int  yy=que[head].y+dir[i][1];
			if(xx>=1&&xx<=ROW&&yy>=1&&yy<=COL)
			{
				if(maptest[xx][yy]==SPACE)  
				{
					maptest[xx][yy]=MAKE; 				 
					tail++;
					que[tail].x=xx;
					que[tail].y=yy;
					que[tail].headnum=head;
				}else if(xx==food.X&&yy==food.Y)  //找到目标坐标
				{
					memcpy(maptest,map,sizeof(map));
					tail++;
					que[tail].x=xx;
					que[tail].y=yy;
					que[tail].headnum=head;
					memset(location,0,sizeof(location));//初始化location数组为0
					//找路径
					int quelocation[10000];//临时记录下标栈 最短路径的数组下标
					int crrentnum=tail;//记录下标
					//遍历寻路最短路径
					while(crrentnum!=1)
					{
						maptest[que[crrentnum].x][que[crrentnum].y]=7;
						quelocation[endstep]=crrentnum;
						crrentnum=que[crrentnum].headnum;
						endstep++;
					}
					maptest[que[tail].x][que[tail].y]=3;
					quelocation[endstep]=crrentnum;
					for(int o=endstep;o>1;o--)
					{
						int x=que[quelocation[o]].x;
						int y=que[quelocation[o]].y;
						location[endstep-o]=direction(que[quelocation[o]].x,que[quelocation[o]].y,que[quelocation[o-1]].x,que[quelocation[o-1]].y);
					}
					//printfdata();//打印虚拟地图寻路数据

					//for(int ii=1;ii<endstep;ii++) //打印相关记录坐标数据
					//{
					//	printf("当前下标:  %d 下标: %d ,坐标: %d  %d SnakeSize: %d\n",ii,quelocation[ii],que[quelocation[ii]].x,que[quelocation[ii]].y,SnakeSize);
					//}
					for(int jj=0;jj<SnakeSize;jj++)
					{
						maptest[snake[jj].X][snake[jj].Y]=0;
					}
					//判断吃food后蛇头能否找到蛇尾巴
					int tailx,taily;
					if(endstep>SnakeSize)
					{
						for(jj=1;jj<=SnakeSize+1;jj++)
						{
							maptest[que[quelocation[jj]].x][que[quelocation[jj]].y]=2;
						}
						maptest[que[quelocation[1]].x][que[quelocation[1]].y]=4;
						tailx=que[quelocation[jj-1]].x;
						taily=que[quelocation[jj-1]].y;
						for(;jj<endstep;jj++)
						{
							maptest[que[quelocation[jj]].x][que[quelocation[jj]].y]=0;
						}
					}else if(endstep<=SnakeSize){
						for(jj=1;jj<endstep;jj++)
						{
							maptest[que[quelocation[jj]].x][que[quelocation[jj]].y]=2;
						}
						maptest[que[quelocation[1]].x][que[quelocation[1]].y]=4;
						for(int jj=0;jj<=SnakeSize+1-endstep;jj++)
						{
							maptest[snake[jj].X][snake[jj].Y]=2;
						}		
						tailx=snake[jj-1].X;
						taily=snake[jj-1].Y;

				
					}
				//	printf("que[quelocation[1]].x: %d que[quelocation[1]].y: %d tailx: %d  taily: %d \n",que[quelocation[1]].x,que[quelocation[1]].y,tailx,taily);
				//	printf("虚拟头出动:\n");
				//	printfdata();//打印虚拟地图寻路数据
				//	printf("\n");
					if(bfsfindtail(que[quelocation[1]].x,que[quelocation[1]].y,tailx,taily,maptest)){  
						return true;
					}else
						return false;
				}
			}
		}
	}
	//printf("\n找不到目标跟随尾巴\n");
	return false;
}

bool bfstail(int startx,int starty)
{	
	int maxstep=0;
	int maxnum=-1;
	for(int test1=0;test1<4;test1++)    //从蛇头三个方向出发寻找比较远路径填补空格
	{
		memcpy(maptest,map,sizeof(map));
		int dirx=snake[0].X+dir[test1][0];
		int  diry=snake[0].Y+dir[test1][1];
		if(dirx>=1&&dirx<=ROW&&diry>=1&&diry<=COL)
		{
			if(maptest[dirx][diry]==SPACE)  
			{
			//	printf("\n第%d次:\n",test1);
				int mid=basicbfs(dirx,diry,snake[SnakeSize-1].X,snake[SnakeSize-1].Y);
			//	printf("step: %d\n",mid);
				if(maxstep<mid)
				{
					maxstep=mid;
					maxnum=test1;
				}
			}
		}
	}

//	printf("maxstep: %d maxnum: %d",maxstep,maxnum);
	if(maxnum!=-1)
	{
			location[0]=direction(snake[0].X,snake[0].Y,snake[0].X+dir[maxnum][0],snake[0].Y+dir[maxnum][1]);
		return true;
	}else return false;
}

int basicbfs(int startx,int starty,int endx,int endy)  //原始bfs
{
	startstep=0;
	endstep=1;
	memcpy(maptest,map,sizeof(map));
	int head=0,tail=0;
	tail++;
	que[tail].x=startx;
	que[tail].y=starty;
	while(head<tail)
	{
		head++;
		for(int i=0;i<4;i++)
		{
			int xx=que[head].x+dir[i][0];
			int  yy=que[head].y+dir[i][1];
			if(xx>=1&&xx<=ROW&&yy>=1&&yy<=COL)
			{
				if(maptest[xx][yy]==SPACE)  
				{
					maptest[xx][yy]=MAKE; 				 
					tail++;
					que[tail].x=xx;
					que[tail].y=yy;
					que[tail].headnum=head;
				}else if(xx==endx&&yy==endy)  //找到目标坐标
				{
					memcpy(maptest,map,sizeof(map));
					tail++;
					que[tail].x=xx;
					que[tail].y=yy;
					que[tail].headnum=head;
					memset(location,0,sizeof(location));//初始化location数组为0
					//找路径
					int quelocation[10000];//临时记录下标栈 最短路径的数组下标
					int crrentnum=tail;//记录下标
					//遍历寻路最短路径
					while(crrentnum!=1)
					{
						maptest[que[crrentnum].x][que[crrentnum].y]=7;
						quelocation[endstep]=crrentnum;
						crrentnum=que[crrentnum].headnum;
						endstep++;
					}
					maptest[que[tail].x][que[tail].y]=3;
					quelocation[endstep]=crrentnum;
					for(int o=endstep;o>1;o--)
					{
						int x=que[quelocation[o]].x;
						int y=que[quelocation[o]].y;
						location[endstep-o]=direction(que[quelocation[o]].x,que[quelocation[o]].y,que[quelocation[o-1]].x,que[quelocation[o-1]].y);
					}
				/*	printfdata();//打印虚拟地图寻路数据
					for(int ii=1;ii<endstep;ii++)
					{
						printf("当前下标:  %d 下标: %d ,坐标: %d  %d SnakeSize: %d\n",ii,quelocation[ii],que[quelocation[ii]].x,que[quelocation[ii]].y,SnakeSize);
					}*/
				/*	maptest[que[tail].x][que[tail].y]=8;
					quelocation[endstep]=crrentnum;
					printfdata();//打印虚拟地图寻路数据
					printf("\nstep：%d",endstep);
					printf("\n");*/
					return endstep;
					//exit(0);
				}
			}
		}
	}
	return 0;
}
bool bfsfindtail(int startx,int starty,int tailx,int taily,int maptest[ROW][COL])//虚拟头去到food位置能否找到尾部
{
	startstep=0;
	endstep=1;
	int head=0,tail=0;
	node quetest[10000];
	tail++;
	quetest[tail].x=startx;
	quetest[tail].y=starty;
	while(head<tail)
	{
		head++;
		for(int i=0;i<4;i++)
		{
			int xx=quetest[head].x+dir[i][0];
			int  yy=quetest[head].y+dir[i][1];
			if(xx>=1&&xx<=ROW&&yy>=1&&yy<=COL)
			{
				if(maptest[xx][yy]==SPACE)  
				{
					maptest[xx][yy]=MAKE; 				 
					tail++;
					quetest[tail].x=xx;
					quetest[tail].y=yy;
					quetest[tail].headnum=head;
				}else if(xx==tailx&&yy==taily&&head!=1)  //找到目标坐标
				{
					//printf("能找到尾部\n");
					return true;
				}
			}
		}
	}
	//printf("不能找到尾部\n");
	return false;
}
char direction(int x1,int y1,int x2,int y2){    ///bfs计算方向
	//printf("计算参数:%d %d %d %d ",x1,y1,x2,y2);
	if(x2-x1>0)
	{
	//	printf(" 方向:%c\n",'S');
		return 'S';
	}else if(x2-x1<0){
	//	printf(" 方向:%c\n",'W');
		return 'W';
	}else if(y2-y1>0){
	//	printf(" 方向:%c\n",'D');
		return 'D';
	}else if(y2-y1<0){
	//	printf(" 方向:%c\n",'A');
		return 'A';
	}
}
void init()
{
	srand((unsigned)time(NULL));	//随机种子
	setbkcolor(WHITE);				//设置背景颜色
	memset(map, SPACE, sizeof(map));//初始化map数组为0即SPACE
	//每一行的 第一个 和 最后一个 是墙
	for (int i = 0; i < ROW; i++)
	{
		map[i][0] = map[i][COL - 1] = WALL;
	}
	//每一列的 第二个 和 倒数第二 个是墙
	for (int j = 1; j < COL - 1; j++)
	{
		map[0][j] = map[ROW - 1][j] = WALL;
	}
	//定义蛇头和蛇的身体
	map[3][5] = HEAD;
	map[3][4] = map[3][3] = SNAKE;
	//初始化蛇
	SnakeSize = 3;	//蛇 长
	SnakeDir = 'S';	//蛇方向向右
	snake[0].X = 3;
	snake[0].Y = 5;
	snake[1].X = 3;
	snake[1].Y = 4;
	snake[2].X = 3;
	snake[2].Y = 3;
	
	
	addfood();
	
}	

void start()
{
	setbkcolor(WHITE);		//设置窗口背景色为白色
	cleardevice();			//刷新屏幕
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextcolor(BLACK);	//设置字体颜色为红色
	/***********************************游戏规则*********************************/
	outtextxy(150, 40, "小游戏-贪食蛇（Version 5.2）:");
	outtextxy(290, 80, "功能选择:");
	outtextxy(280, 150, "1.普通模式");
	outtextxy(280, 200, "2.穿墙模式");
	outtextxy(280, 250, "3.无敌模式");
	outtextxy(200, 280, "数字键 1,2,3 选择模式，Enter键进入游戏");
	outtextxy(200, 300, "字母键 W,S,A,D 方向键 上下左右 控制方向");
	outtextxy(130, 350, "作者简介：");
	outtextxy(160, 380, "博文管理学院信息工程院-2019级计算机专业(寻路算法)");
	outtextxy(280, 410, "1902班---黄家嘉");
}
void chose()
{
	while (1)
	{
		switch (getch())
		{
		case '1':
			start();
			outtextxy(260, 150, "->");
			mode = 0;
			break;
		case '2':
			start();
			outtextxy(260, 200, "->");
			mode = 1;
			break;
		case '3':
			start();
			outtextxy(260, 250, "->");
			mode = 2;
			break;
		case 13:
			return;
			break;
		}
	}
}
void DrawMap()
{
	
	BeginBatchDraw();	//开始绘图
	setbkcolor(WHITE);	//设置背景颜色为白色
	settextcolor(RGB(238,0,0));
	cleardevice();		//清屏
	char arr[10];		//保存成绩
	sprintf(arr, "总分：%d", score);	//将成绩格式化输出到字符串arr中 
	outtextxy(0, 0, arr);				//显示成绩
	for (int y = 0; y < ROW; y++)		//y轴方向向下
	{
		for (int x = 0; x < COL; x++)	//x轴方向下上
		{
			switch (map[y][x])
			{
			case SPACE:
				break;
			case WALL:
				setlinecolor(BLACK);	
				setfillcolor(RGB(238, 233, 233));	//灰色
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case SNAKE:
				setlinecolor(RGB(0, 245, 255));		//绿色 
				setfillcolor(YELLOW);
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case HEAD:
				//画七彩蛇头
				switch (rand() % 7)
				{
				case 0:
					setfillcolor(RGB(255, 0, 0));		//红色 255 0 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 1:
					setfillcolor(RGB(255, 165, 0));		//橙  255 165 0 
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 2:
					setfillcolor(RGB(255, 255, 0));		//黄  255 255 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 3:
					setfillcolor(RGB(0, 255, 0));		//绿色  0, 255, 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 4:
					setfillcolor(RGB(0, 255, 255));		//青  0 255 255
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 5:
					setfillcolor(RGB(0, 0, 255));		//蓝  0 0 255
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 6:
					setfillcolor(RGB(160, 32, 240));	//紫  160 32 240
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				default:
					break;
				}
				break;
				case FOOD:
					setfillcolor(RGB(255, 0, 0));			//红色
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				default:
					break;
			}
		}
	}
	EndBatchDraw();
	
}
void addfood()
{
	int row, col;
	do
	{	
		row = rand() % (ROW - 1) + 1;
		col = rand() % (COL - 1) + 1;
	} while (map[row][col] != SPACE);
	map[row][col] = FOOD;
	///////////记录食物坐标 
	food.X=row;
	food.Y=col;
}
void ChangeDir()
{
	if(judge==false)
	{
		switch (getch())
		{
		case'A':
		case'a':
		case 75:
			if (SnakeDir != 'D') SnakeDir = 'A';	//蛇不能后退
			break;
		case'D':
		case'd':
		case 77:
			if (SnakeDir != 'A') SnakeDir = 'D';
			break;
		case'W':
		case'w':
		case 72:
			if (SnakeDir != 'S') SnakeDir = 'W';
			break;
		case'S':
		case's':
		case 80:
			if (SnakeDir != 'W') SnakeDir = 'S';
			break;
		case'o':
		case'O':
			bfs();
			judge=!judge;
			break;
		case 32:
			getch();
			break;
		default:
			break;
		}
	}else
	{
		switch (getch())
		{
		case'o':
		case'O':
			judge=!judge;
			break;
		case 32:
			getch();
			break;
		default:
			break;
		}
	}
	
}


void move()
{
		if(score==ROW*COL-(2*ROW+2*COL))
	{
		MessageBox(GetHWnd(), "YOU WIN!", "SORRY", MB_OK);
			exit(0);
	}
	COORD next;		//蛇头的下一个位置
	int i;
	if(judge){   //启动bfs后自动获取方向
		SnakeDir=location[startstep];
		startstep++;
	//	printf("当前方向: %c\n",SnakeDir);
	}
	switch (SnakeDir)
	{
	case'A':
		next.X = snake[0].X;
		next.Y = snake[0].Y - 1;
		break;
	case'W':
		next.X = snake[0].X - 1;
		next.Y = snake[0].Y;
		break;
	case'D':
		next.X = snake[0].X;
		next.Y = snake[0].Y + 1;
		break;
	case'S':
		next.X = snake[0].X + 1;
		next.Y = snake[0].Y;
		break;
	default:
		break;
	}
	
	switch (map[next.X][next.Y])
	{
	case SPACE://直接移动
		map[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE;//地图蛇尾所在地置空
		for (i = SnakeSize - 1; i > 0; i--)						//蛇尾到蛇头整体移动一位
		{
			snake[i] = snake[i - 1];
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//蛇头置 蛇
		snake[0] = next;											//将下一个位置赋值给蛇头
		map[snake[0].X][snake[0].Y] = HEAD;							//设置头
		break;
	case WALL:
		if (mode)		//模式1模式2可穿墙
		{
			map[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE;//蛇尾置空
			for (i = SnakeSize - 1; i > 0; i--)						//蛇尾到蛇头整体移动一位
			{
				snake[i] = snake[i - 1];
			}
			map[snake[0].X][snake[0].Y] = SNAKE;						//蛇头置 蛇
			switch (SnakeDir)											//穿墙
			{
			case'A':next.Y = COL - 2; break;
			case 'D':next.Y = 1; break;
			case 'W': next.X = ROW - 2; break;
			case 'S':next.X = 1; break;
			default:
				break;
			}
			snake[0] = next;											//蛇头移动到新位置
			map[snake[0].X][snake[0].Y] = HEAD;							//新的蛇头所在的位置
		}
		else {
			MessageBox(GetHWnd(), "游戏结束", "SORRY", MB_OK);
			exit(0);
		}
		break;
	case SNAKE:
		if (mode == 2)		//模式二无敌
		{
			break;
		}
		else {
			MessageBox(GetHWnd(), "游戏结束", "SORRY", MB_OK);
			exit(0);
		}
		break;
	case FOOD://食物				蛇尾不变
		for (i = SnakeSize; i > 0; i--)							//蛇尾到蛇头整体移动一位
		{
			snake[i] = snake[i - 1];								
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//蛇头 置 蛇
		snake[0] = next;											//将下一个位置赋值给蛇头
		score++;			//分数加一				
		(SnakeSize)++;		//蛇尺度加一
		map[snake[0].X][snake[0].Y] = HEAD;							//地图上重置蛇头
		addfood();
		break;
	default:break;
	}

}
