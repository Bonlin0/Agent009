#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#define WIDETH 20
#define HEIGHT 15

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
struct role player[10];
int mission_num = 0;
int key = 0;//key=1则表示通关
bool force;

void mapping_border()//将地图的边界设置为墙
{
	for (int i = 0; i < WIDETH; i++)
	{
		map[0][i].downwall = 1;
		map[HEIGHT - 2][i].downwall = 1;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		map[i][0].rightwall = 1;
		map[i][WIDETH-2].rightwall = 1;
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
	map[7][13].portal = 1;//传送设备位置
	map[3][17].exit = 1;//出口位置
	player[0].x = 2;
	player[0].y = 12;//玩家初始位置
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
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].x++;
		player[*r].por_num--;
		force = 0;
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
		if (map[*q][*p-1].rightwall)
			return;
		else
		{
			player[*r].x--;
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].x--;
		player[*r].por_num--;
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
		if (map[*q-1][*p].downwall)
			return;
		else
		{
			player[*r].y--;
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].y--;
		player[*r].por_num--;
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
			return;
		}
	}
	else if (force == 1)
	{
		player[*r].y++;
		player[*r].por_num--;
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


int main(void)
{
	mapping1();//载入关卡地图；
	char input;
	while(key==0)
	{
		input=_getch();
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
		}
	}
	printf("Mission Complete!");
	system("pause");
	return 0;
}