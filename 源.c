#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#define WIDETH 20
#define HEIGHT 15

struct role
{
	int x;
	int y;//��ʾ��������
	int por_num;
};
struct cell
{
	bool downwall;
	bool rightwall;//��ʾ������ǽ�����ޣ�0��ʾû�У�1��ʾ��
	bool portal;//��ʾ�����豸�����ޣ�0��ʾ�ޣ�1��ʾ��
	bool exit;//��ʾ����
};

struct cell map[HEIGHT][WIDETH];//15*20�ĵ�ͼ
struct role player[10];
int mission_num = 0;
int key = 0;//key=1���ʾͨ��
bool force;

void mapping_border()//����ͼ�ı߽�����Ϊǽ
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
	map[7][13].portal = 1;//�����豸λ��
	map[3][17].exit = 1;//����λ��
	player[0].x = 2;
	player[0].y = 12;//��ҳ�ʼλ��
}

void rightmove()
{
	int *p, *q, *r;
	r = &mission_num;
	p = &player[*r].x; q = &player[*r].y;
	if (force == 0)//�ж��ж��Ƿ�ǿ��
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
	if (force == 0)//�ж��ж��Ƿ�ǿ��
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
	if (force == 0)//�ж��ж��Ƿ�ǿ��
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
	if (force == 0)//�ж��ж��Ƿ�ǿ��
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
	mapping1();//����ؿ���ͼ��
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
			case 'f'://��fǿ����һ���ж�������ǿ������ȡ��
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