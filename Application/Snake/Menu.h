#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include<ctime>
#include <stdio.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

void Pos(int x, int y);
void HideCursor();

int Menu_Examine = 0;			//用来控制打印菜单的时候只打印一次，不然会在循环的时候一直打印
void DrawMenu()					//打印菜单
{
	void HideCursor();
	if(Menu_Examine==0)
	{ 
		Menu_Examine = 1;
		for (int i = 5; i > 0; i--) printf("\n");
		printf("\t\t\t\t☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |    贪吃蛇之无敌香蕉蛇皮怪之盘古开天地之最原始版    |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                      开始游戏                      |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                      退出游戏                      |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆\n");
		printf("\t\t\t\t                                    wasd控制，space选择\n");
		printf("\t\t\t\t                                        制作人:机长大大\n");
		}
}

void Chioce(int &x)
{
	void HideCursor();
	static char n = '1';
	if (_kbhit())
	{
		char temp = _getch();	//临时变量储存键盘输入的值
		if ((n == '1') && (temp == 'w'))
		{
			n = '1';			//选择‘1’的时候不能在往上翻
		}	
		else if ((n == '1') && (temp == 's'))
		{
			n = '2';
		}
		else if ((n == '2') && (temp == 'w'))
		{
			n = '1';
		}
		else if ((n == '2') && (temp == 's'))
		{
			n = '2';			//选择‘2’的时候不能在往下翻
		}
		else if ((temp == ' ') && (n == '1'))
		{
			x = 2;				//进入游戏
			return;
		}
		else if ((temp == ' ') && (n == '2'))
		{
			exit(0);			//退出
		}
	}
	switch (n)
	{
	case '1':					//选择标识在选择‘1’
		Pos(52, 10);
		cout << "●";
		Pos(52, 13);
		cout << "  ";
		break;
	case '2':					//选择标识在选择‘2’
		Pos(52, 10);
		cout << "  ";
		Pos(52, 13);
		cout << "●";
		break;
	}
	Sleep(10);				//设置间隔以免看出闪烁
}