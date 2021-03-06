// Agent009.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <conio.h>
#include <graphics.h>
#include <math.h>


#define PI 3.1415926536		// 圆周率
#define WIDETH 20
#define HEIGHT 15
#define MoveCoolDownTime 1//每次移动冷却时间

DWORD* g_bufMask;			// 指向“建筑物”IMAGE 的指针
DWORD* g_bufRender;			// 指向渲染 IMAGE 的指针


struct role
{
	int x;
	int y;//表示人物坐标
	int por_num;
};
struct cell
{
	bool downwall;
	bool rightwall;//表示各方向墙的有无，0表示没有，1表示有
	bool portal;//表示传送设备的有无，0表示无，1表示有
	bool exit;//表示出口
};

struct cell map[HEIGHT][WIDETH];//15*20的地图

//声明10个player 将会在每关地图生成时对player进行具体初始化
struct role player[10];
//重复的全局变量 在main中已定义
int mission_num = 0;
//key=1则表示通关
int key = 0;
//force表示能力是否开启 将会被用于绘制游戏界面故声明为全局变量
bool force;
//控制玩家移动速度的冷却
short int MoveCoolDown = 0;

void mapping_border()//将地图的边界设置为墙
{
	for (int i = 1; i < WIDETH - 1; i++)
	{
		map[0][i].downwall = 1;
		map[HEIGHT - 2][i].downwall = 1;
	}
	for (int i = 1; i < HEIGHT - 1; i++)
	{
		map[i][0].rightwall = 1;
		map[i][WIDETH - 2].rightwall = 1;
	}

}

void mapping1()
{
	for (int i = 4; i <= 13; i++)
		map[i][3].rightwall = 1;
	for (int i = 1; i <= 13; i++)
	{
		map[i][10].rightwall = 1;
		map[i][15].rightwall = 1;
	}
	map[7][7].portal = 1;
	map[13][6].portal = 1;
	map[13][7].portal = 1;
	map[13][8].portal = 1;
    /*该段用于测试
	map[13][1].portal = 1; map[13][2].portal = 1;
	map[13][3].portal = 1; map[13][4].portal = 1;
	map[13][5].portal = 1; map[13][9].portal = 1;
	map[13][10].portal = 1; map[13][11].portal = 1;
	map[13][12].portal = 1; map[13][13].portal = 1;
	map[13][14].portal = 1; map[13][15].portal = 1;
	*/
	map[7][13].portal = 1;//传送设备位置
	map[3][17].exit = 1;//出口位置
	player[0].x = 2;
	player[0].y = 12;//玩家初始位置

	player[0].por_num = 0;//初始化传送设备数目
}

void rightmove()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (force == 0)//判断行动是否强化
	{
		if (map[*q][*p].rightwall)
			return;
		else
		{
			player[*r].x++;
			//移动陷入冷却
			MoveCoolDown = MoveCoolDownTime;

			return;
		}
	}
	else if (force == 1)
	{
		player[*r].x++;
		player[*r].por_num--;
		force = 0;
		MoveCoolDown = MoveCoolDownTime;
		return;
	}
}
void leftmove()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (force == 0)//判断行动是否强化
	{
		if (map[*q][*p - 1].rightwall)
			return;
		else
		{
			player[*r].x--;
			MoveCoolDown = MoveCoolDownTime;
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].x--;
		player[*r].por_num--;
		MoveCoolDown = MoveCoolDownTime;
		force = 0;
		return;
	}
}
void upmove()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (force == 0)//判断行动是否强化
	{
		if (map[*q - 1][*p].downwall)
			return;
		else
		{
			player[*r].y--;
			MoveCoolDown = MoveCoolDownTime;
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].y--;
		player[*r].por_num--;
		MoveCoolDown = MoveCoolDownTime;
		force = 0;
		return;
	}
}
void downmove()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (force == 0)//判断行动是否强化
	{
		if (map[*q][*p].downwall)
			return;
		else
		{
			player[*r].y++;
			MoveCoolDown = MoveCoolDownTime;
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].y++;
		player[*r].por_num--;
		MoveCoolDown = MoveCoolDownTime;
		force = 0;
		return;
	}
}

void test()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (map[*q][*p].portal == 1)
	{
		player[*r].por_num++;
		map[*q][*p].portal = 0;
	}
	if (map[*q][*p].exit == 1)
		key = 1;
}







void drawgame(void)
{
	int i, j;//循环变量
	// 绘制玩家   暂时先以实心方格代表玩家
	setfillcolor(WHITE);
	solidrectangle(player[mission_num].x * 40 + 10, player[mission_num].y * 40 + 10, player[mission_num].x * 40 + 30, player[mission_num].y * 40 + 30);
	//遍历地图每个方格 进行绘制
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//绘制传送设备 暂时以蓝色圆球代表		
			if (map[i][j].portal == TRUE)
			{
				setfillcolor(BLUE);
				solidcircle(j * 40 + 20, i * 40 + 20, 10);
			}
			//绘制出口 暂时以绿色圆球代表
			if (map[i][j].exit == TRUE)
			{
				setfillcolor(GREEN);
				solidcircle(j * 40 + 20, i * 40 + 20, 10);
			}
			
		}
	}

	//绘制传送能力是否开启及剩余次数
	settextcolor(WHITE);
	settextstyle(30, 15, _T("Courier"));
	
	if (force)
	{
		outtextxy(40, 565, _T("传送能力：ON"));
	}
	else
	{
		outtextxy(40, 565, _T("传送能力：OFF"));
	}
	//定义变量储存用户剩余传送次数
	int PortalToUse = player[mission_num].por_num;
	
	RECT r = { 280, 565, 800, 600 };
	drawtext(_T("剩余传送次数: "),&r, DT_LEFT);
	//图形库自带输出变量能力受限 以switch输出剩余传送次数
	moveto(480, 565);
	if (PortalToUse > 9)
	{
		int temp = PortalToUse / 10;
		switch (temp)
		{
		case 1:
			outtext(_T("1"));
			break;
		case 2:
			outtext(_T("2"));
			break;
		case 3:
			outtext(_T("3"));
			break;
		case 4:
			outtext(_T("4"));
			break;
		case 5:
			outtext(_T("5"));
			break;
		case 6:
			outtext(_T("6"));
			break;
		case 7:
			outtext(_T("7"));
			break;
		case 8:
			outtext(_T("8"));
			break;
		case 9:
			outtext(_T("9"));
			break;
		}
	}
	int temp = PortalToUse % 10;
	switch (temp)
	{
	case 0:
		outtext(_T("0"));
		break;
	case 1:
		outtext(_T("1"));
		break;
	case 2:
		outtext(_T("2"));
		break;
	case 3:
		outtext(_T("3"));
		break;
	case 4:
		outtext(_T("4"));
		break;
	case 5:
		outtext(_T("5"));
		break;
	case 6:
		outtext(_T("6"));
		break;
	case 7:
		outtext(_T("7"));
		break;
	case 8:
		outtext(_T("8"));
		break;
	case 9:
		outtext(_T("9"));
		break;
	}
	


}


int choose_mission(void)
{
	//声明字符型变量储存用户输入
	char kin;
	//声明整型变量储存mission_num_chosen用户所选择的关卡序号
	int mission_num_chosen = 0;
	//新建IMAGE对象ChooseMission并载入图片
	IMAGE ChooseMission;
	loadimage(&ChooseMission, _T("choose_mission.jpg"));
	
	//改变工作区至默认窗口
	SetWorkingImage(NULL);
	//设定填充颜色为黄色
	setfillcolor(0x00a0a0);
	//进入循环 直到用户按'f'后跳出 0.1s刷新一次
	while (TRUE)
	{
		//清空屏幕准备好下一次绘制
		cleardevice();
		//对用户输入进行处理
		if (_kbhit())
		{
			kin = _getch();
			//对用户输入进行判断并改变被选关卡
			switch (kin)
			{
			case 'w':
				if (mission_num_chosen >= 5)
				{
					mission_num_chosen -= 5;
				}
				break;
			case 's':
				if (mission_num_chosen <= 4)
				{
					mission_num_chosen += 5;
				}
				break;
			case 'a':
				if (mission_num_chosen != 0 && mission_num_chosen != 5)
				{
					mission_num_chosen--;
				}
				break;
			case 'd':
				if (mission_num_chosen != 4 && mission_num_chosen != 9)
				{
					mission_num_chosen++;
				}
				break;
			case 'f':
				return mission_num_chosen;
			}
		}

		// 根据所选择的关卡序号填充方格
		fillrectangle(80 + 60 * (mission_num_chosen % 5), 240 + 60 * (mission_num_chosen / 5), 140 + 60 * (mission_num_chosen % 5), 300 + 60 * (mission_num_chosen / 5));

		//将图片展示在窗口 与默认窗口‘或’操作（srcpaint）
		putimage(0, 0, &ChooseMission, SRCPAINT);
		//使用batchdraw和Sleep 避免屏闪
		FlushBatchDraw();
		Sleep(100);
		BeginBatchDraw();
		
	}
}


IMAGE* drawgamebuildings(void)
{
	//声明static IMAGE 以确保在函数外可使用该变量
	static IMAGE g_imgMask(800, 600);
	//获取绘图设备的显存指针并储存于全局变量 用于之后的渲染判断
	g_bufMask = GetImageBuffer(&g_imgMask);
	// 设置绘图目标
	SetWorkingImage(&g_imgMask);

	//设置划线（墙）颜色及样式
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 3);

	int i, j;     //i,j用于循环控制
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if(map[i][j].rightwall==TRUE)
			{
				//绘制每个方格右边的墙
				line(40 + 40 * j, 40 * i, 40 + 40 * j, 40 + 40 * i);
			}
			if (map[i][j].downwall == TRUE)
			{
				//绘制每个方格下边的墙
				line( 40 * j, 40 + 40 * i, 40 + 40 * j, 40 + 40 * i);
			}
		}
	}

	// 恢复绘图目标为默认窗口
	SetWorkingImage(NULL);
	return &g_imgMask;
}


void ingame(int mission_num)
{
	//生成地图边界
	mapping_border();
	//这里缺少一个switch语句判断用户选择的关卡
	mapping1();

	//使用IMAGE指针变量储存drawgamebuildings函数返回的值 以用于后续的putimage函数
	IMAGE* maskpt = drawgamebuildings();
	//显示buildings
	putimage(0, 0, maskpt, SRCPAINT);
	//绘制游戏界面
	drawgame();
	
	//刷新界面
	FlushBatchDraw();

	BeginBatchDraw();
	//声明字符型变量储存用户输入
	char input;
	//进入游戏循环
	while (key == 0)
	{
		//移动冷却恢复
		if (MoveCoolDown > 0)
		{
			MoveCoolDown--;
		}
		
		cleardevice();
		if (_kbhit() && MoveCoolDown == 0)
		{
			input = _getch();
			
			switch (input)
			{
			case 'w':
				upmove();
				test();
				break;
			case 'a':
				leftmove();
				test();
				break;
			case 's':
				downmove();
				test();
				break;
			case 'd':
				rightmove();
				test();
				break;
			case 'f'://按f强化下一次行动，若已强化，则取消
				if (player[0].por_num != 0 && force == 0)
					force = 1;
				else if (force == 1)
				{
					force = 0;
					player[0].por_num++;
				}
				break;
			}
		}
		drawgame();
		putimage(0, 0, maskpt, SRCPAINT);
		//控制刷新频率
		Sleep(50);
		FlushBatchDraw();
		BeginBatchDraw();		
	}
	settextcolor(WHITE);
	settextstyle(50, 0, _T("Courier"));
	outtextxy(150, 250, _T("Mission Complete!"));
	FlushBatchDraw();
	_getch();
}





int main()
{
	//声明字符型变量‘kin’存储用户输入
	char kin;
	//声明整型变量‘mission_num’保存用户选择的关卡序号
	int mission_num = 0;
	//初始化窗口，分辨率 800 * 600.
	initgraph(800, 600);
	//新建IMAGE对象startmenu并载入图片
	IMAGE startmenu;
	loadimage(&startmenu, _T("agent009menu.jpg"));
	//将图片展示在窗口
	putimage(0, 0, &startmenu);
	//进行循环直到用户输入'f’
	do
	{
		kin = _getch();
	} while (kin != 'f' && kin != 'F');

	//进入choose_mission函数 获取用户选择的关卡序号 并储存在mission_num变量中 函数末尾开始batchdraw 未flush
	mission_num = choose_mission();

	ingame(mission_num);

	//关闭绘图设备
	closegraph();
	//_getch();
    return 0;
}

