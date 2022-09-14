#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#define ROW 10
#define COL 10

//ö��
enum game
{
	SPACE, WALL, SNAKE, FOOD, HEAD,MAKE//�յ�  ǽ  ��  ʳ�� ��ͷ ���(bfs)
};

//���㷽��bfs��
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

//Ѱ·ջ
struct node
{
	int x;
	int y;	
	int headnum;//��¼��һ���������±�
};
node que[10000];  //��¼���в��ҵ������
/*******************************************ȫ�ֱ���*******************************************************/
int mode = 0;			//��Ϸģʽ
int score = 0;			//����
DWORD t1, t2;			//��������ʱ������ƶ��ٶ�
int map[ROW][COL];		//��ͼ��С

COORD snake[1024];		//��			typedef struct _COORD {SHORT X;SHORT Y;} COORD, *PCOORD;
size_t SnakeSize;		//�ߵĳ߶�		typedef unsigned int     size_t;
char SnakeDir;			//���ƶ�����


//Ѱ·�㷨�������
COORD food;				//��¼ʳ������
int maptest[ROW][COL];   //�����ͼ  �����ͼ���Ѱ�ҹ������꣬��Ӱ����ʵ��ͼ������
char location[100];//��¼����   ��¼�������ͼ���ҵ������·���������ǵ�ͼʹ��
bool judge=false;       //�Ƿ���Ѱ· (����o�ı��Զ������ֶ�ģʽ��Ѱ·״̬�²��ܲ��������.)
int startstep; //�жϲ���
int endstep;  //�жϽ�������
COORD target;    //Ѱ·��Ŀ��
/*******************************************ȫ�ֺ���***************************************************/
void start();	//��ʼ����
void chose();	//ѡ��ģʽ  0:��ͨģʽ  1����ǽģʽ  2���޵�ģʽ
void DrawMap();	//��ͼ
void init();	//��ʼ����Ϸ����
void addfood();	  //���ʳ��
void ChangeDir();//�޸��ߵķ���
void move();//�ߵ��ƶ�


//Ѱ·�㷨���躯��
void bfs(int startx,int starty);//�Զ�Ѱ���㷨.
char direction(int x1,int y1,int x2,int y2); ///bfs���㷽�� �����¼����֮��ķ���


int main()
{
	initgraph(640,480);
	start();
	outtextxy(260,150,"->");		//��ʼĬ��ģʽһ
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
		if (t2 - t1 > 50&&judge)  //����Ѱ·��
		{
			move();
			t1 = t2;
		}
		DrawMap();
	}
	getchar();
	closegraph();
	return 0;
}
void printfdata(){   //��ӡ�����ͼѰ·����
	for (int i = 0; i <ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			printf("%d",maptest[i][j]);
		}
		printf("\n");
	}
	
	
}
void bfs(int startx,int starty,int targetx,int targety)
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
				}else if(xx==targetx&&yy==targety)  //�ҵ�Ŀ������
				{
					memcpy(maptest,map,sizeof(map));
					tail++;
					que[tail].x=xx;
					que[tail].y=yy;
					que[tail].headnum=head;
					memset(location,0,sizeof(location));//��ʼ��location����Ϊ0
					//��·��
					int quelocation[100];//��ʱ��¼����ջ 
					int crrentnum=tail;//��¼�±�
					//����Ѱ·���·��
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
					printfdata();//��ӡ�����ͼѰ·����
					return;
				}
			}
		}
	}
//	endstep=1;
//	printf("\n�Ҳ���Ŀ��\n");
	printfdata();
	//	exit(0);
}
char direction(int x1,int y1,int x2,int y2){    ///bfs���㷽��
	printf("�������:%d %d %d %d ",x1,y1,x2,y2);
	if(x2-x1>0)
	{
		printf(" ����:%c\n",'S');
		return 'S';
	}else if(x2-x1<0){
		printf(" ����:%c\n",'W');
		return 'W';
	}else if(y2-y1>0){
		printf(" ����:%c\n",'D');
		return 'D';
	}else if(y2-y1<0){
		printf(" ����:%c\n",'A');
		return 'A';
	}
}
void init()
{
	srand((unsigned)time(NULL));	//�������
	setbkcolor(WHITE);				//���ñ�����ɫ
	memset(map, SPACE, sizeof(map));//��ʼ��map����Ϊ0��SPACE
	//ÿһ�е� ��һ�� �� ���һ�� ��ǽ
	for (int i = 0; i < ROW; i++)
	{
		map[i][0] = map[i][COL - 1] = WALL;
	}
	//ÿһ�е� �ڶ��� �� �����ڶ� ����ǽ
	for (int j = 1; j < COL - 1; j++)
	{
		map[0][j] = map[ROW - 1][j] = WALL;
	}
	//������ͷ���ߵ�����
	map[3][5] = HEAD;
	map[3][4] = map[3][3] = SNAKE;
	//��ʼ����
	SnakeSize = 3;	//�� ��
	SnakeDir = 'S';	//�߷�������
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
	setbkcolor(WHITE);		//���ô��ڱ���ɫΪ��ɫ
	cleardevice();			//ˢ����Ļ
	setbkmode(TRANSPARENT);	//�������屳��ɫΪ͸��
	settextcolor(BLACK);	//����������ɫΪ��ɫ
	/***********************************��Ϸ����*********************************/
	outtextxy(150, 40, "С��Ϸ-̰ʳ�ߣ�Version 5.2��:");
	outtextxy(290, 80, "����ѡ��:");
	outtextxy(280, 150, "1.��ͨģʽ");
	outtextxy(280, 200, "2.��ǽģʽ");
	outtextxy(280, 250, "3.�޵�ģʽ");
	outtextxy(200, 280, "���ּ� 1,2,3 ѡ��ģʽ��Enter��������Ϸ");
	outtextxy(200, 300, "��ĸ�� W,S,A,D ����� �������� ���Ʒ���");
	outtextxy(130, 350, "���߼�飺");
	outtextxy(160, 380, "���Ĺ���ѧԺ��Ϣ����Ժ-2020�������רҵ");
	outtextxy(160, 410, "5��---��С��");
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
	
	BeginBatchDraw();	//��ʼ��ͼ
	setbkcolor(WHITE);	//���ñ�����ɫΪ��ɫ
	settextcolor(RGB(238,0,0));
	cleardevice();		//����
	char arr[10];		//����ɼ�
	sprintf(arr, "�ܷ֣�%d", score);	//���ɼ���ʽ��������ַ���arr�� 
	outtextxy(0, 0, arr);				//��ʾ�ɼ�
	for (int y = 0; y < ROW; y++)		//y�᷽������
	{
		for (int x = 0; x < COL; x++)	//x�᷽������
		{
			switch (map[y][x])
			{
			case SPACE:
				break;
			case WALL:
				setlinecolor(BLACK);	
				setfillcolor(RGB(238, 233, 233));	//��ɫ
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case SNAKE:
				setlinecolor(RGB(0, 245, 255));		//��ɫ 
				setfillcolor(WHITE);
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case HEAD:
				//���߲���ͷ
				switch (rand() % 7)
				{
				case 0:
					setfillcolor(RGB(255, 0, 0));		//��ɫ 255 0 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 1:
					setfillcolor(RGB(255, 165, 0));		//��  255 165 0 
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 2:
					setfillcolor(RGB(255, 255, 0));		//��  255 255 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 3:
					setfillcolor(RGB(0, 255, 0));		//��ɫ  0, 255, 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 4:
					setfillcolor(RGB(0, 255, 255));		//��  0 255 255
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 5:
					setfillcolor(RGB(0, 0, 255));		//��  0 0 255
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				case 6:
					setfillcolor(RGB(160, 32, 240));	//��  160 32 240
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					break;
				default:
					break;
				}
				break;
				case FOOD:
					setfillcolor(RGB(255, 0, 0));			//��ɫ
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
	///////////��¼ʳ������ 
	food.X=row;
	food.Y=col;
	//Ѱ·Ŀ��
	target.X=food.X;
	target.Y=food.Y;
	//bfs�㷨����
	if(judge)
		bfs(snake[0].X,snake[0].Y,target.X,target.Y);
	//printf("foodX-Y:%d %d\n",food.X,food.Y);
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
			if (SnakeDir != 'D') SnakeDir = 'A';	//�߲��ܺ���
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
			bfs(snake[0].X,snake[0].Y,target.X,target.Y);
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
	COORD next;		//��ͷ����һ��λ��
	int i;
	if(judge){
	//	if(endstep==1)
	//	{
		//	target.X=snake[SnakeSize - 1].X;
		//	target.Y=snake[SnakeSize - 1].Y;
			//bfs(snake[0].X,snake[0].Y,target.X,target.Y);
		//	printf("β������: %d %d\n",target.X,target.Y);
		//	exit(0);
	//		return;
	//	}
		SnakeDir=location[startstep];
		startstep++;
		printf("��ǰ����: %c\n",SnakeDir);
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
	case SPACE://ֱ���ƶ�
		map[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE;//��ͼ��β���ڵ��ÿ�
		for (i = SnakeSize - 1; i > 0; i--)						//��β����ͷ�����ƶ�һλ
		{
			snake[i] = snake[i - 1];
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//��ͷ�� ��
		snake[0] = next;											//����һ��λ�ø�ֵ����ͷ
		map[snake[0].X][snake[0].Y] = HEAD;							//����ͷ
		break;
	case WALL:
		if (mode)		//ģʽ1ģʽ2�ɴ�ǽ
		{
			map[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE;//��β�ÿ�
			for (i = SnakeSize - 1; i > 0; i--)						//��β����ͷ�����ƶ�һλ
			{
				snake[i] = snake[i - 1];
			}
			map[snake[0].X][snake[0].Y] = SNAKE;						//��ͷ�� ��
			switch (SnakeDir)											//��ǽ
			{
			case'A':next.Y = COL - 2; break;
			case 'D':next.Y = 1; break;
			case 'W': next.X = ROW - 2; break;
			case 'S':next.X = 1; break;
			default:
				break;
			}
			snake[0] = next;											//��ͷ�ƶ�����λ��
			map[snake[0].X][snake[0].Y] = HEAD;							//�µ���ͷ���ڵ�λ��
		}
		else {
			MessageBox(GetHWnd(), "��Ϸ����", "SORRY", MB_OK);
			exit(0);
		}
		break;
	case SNAKE:
		if (mode == 2)		//ģʽ���޵�
		{
			break;
		}
		else {
			MessageBox(GetHWnd(), "��Ϸ����", "SORRY", MB_OK);
			exit(0);
		}
		break;
	case FOOD://ʳ��				��β����
		for (i = SnakeSize; i > 0; i--)							//��β����ͷ�����ƶ�һλ
		{
			snake[i] = snake[i - 1];								
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//��ͷ �� ��
		snake[0] = next;											//����һ��λ�ø�ֵ����ͷ
		score++;			//������һ				
		(SnakeSize)++;		//�߳߶ȼ�һ
		map[snake[0].X][snake[0].Y] = HEAD;							//��ͼ��������ͷ
		addfood();
		break;
	default:break;
	}
}
