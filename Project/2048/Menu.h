#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>

using namespace std;

void Pos(int x, int y);

void HideCursor();

int Menu_exam = 0;

void Drawmenu() { //打印菜单
	void HideCursor();
	if(!Menu_exam)
	{
		Menu_exam = 1;
		for (int i = 5; i > 0; i--) printf("\n");
		printf("\t\t\t\t☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆\n");
		printf("\t\t\t\t |                                                    |\n");
        printf("\t\t\t\t |       _______________      _____   ______          |\n");
        printf("\t\t\t\t |       \\_____  \\   _  \\    /  |  | /  __  \\         |\n");
        printf("\t\t\t\t |        /  ____/  /_\\  \\  /   |  |_>      <         |\n");
        printf("\t\t\t\t |       /       \\  \\_/   \\/    ^   /   --   \\        |\n");
        printf("\t\t\t\t |       \\_______ \\_____  /\\____   |\\______  /        |\n");
        printf("\t\t\t\t |               \\/     \\/      |__|       \\/         |\n");
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
		printf("\t\t\t\t                         WASD控制, Space选择, T获取提示\n");
		printf("\t\t\t\t                                        制作人:机长大大\n");
	}
}

void choice(int &x) { //确定菜单中的选项
	void HideCursor();
	static bool n=1;
	if (_kbhit()) {
		char temp=_getch();
		if (n&&temp=='s')
			n=0;
		if (!n&&temp=='w')
			n=1;
		if (temp==' '&&n) {
			x=2;
			return;
		}
		if (temp==' '&&!n) {
			x=4;
			return;
		}
	}
	switch (n) {
	case 1:
		Pos(52, 15);
		cout<<"●";
		Pos(52, 18);
		cout<<"  ";
		break;
	case 0:
		Pos(52, 15);
		cout<<"  ";
		Pos(52, 18);
		cout<<"●";
		break;
	}
	Sleep(10);
}
