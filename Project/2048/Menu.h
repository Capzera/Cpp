#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include<ctime>
#include <stdio.h>

using namespace std;

void Pos(int x, int y);
void HideCursor();

int Menu_exam = 0;
void Drawmenu()
{
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
		printf("\t\t\t\t                                    wasd控制，space选择\n");
		printf("\t\t\t\t                                        制作人:机长大大\n");
		}
}

void choice(int &x)
{
	void HideCursor();
	static char n='1';
	if (_kbhit())
	{
		char temp=_getch();
		if (n=='1'&&temp=='s')
			n='2';
		if (n=='2'&&temp=='w')
			n='1';
		if (temp==' '&&n=='1') {
			x=2;
			return;
		}
		if (temp==' '&&n=='2') {
			x=3;
			return;
		}
	}
	switch (n)
	{
	case '1':
		Pos(52, 15);
		cout<<"●";
		Pos(52, 18);
		cout<<"  ";
		break;
	case '2':
		Pos(52, 15);
		cout << "  ";
		Pos(52, 18);
		cout<<"●";
		break;
	}
	Sleep(10);
}
