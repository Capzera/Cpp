#include<bits/stdc++.h>
#include<stdlib.h>
#include "Menu.h"
#include "game.h"

using namespace std;

void Pos(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main()
{
    game g;
    system("title 2048 V1.0");
    system("color 3F");
    static int choose=1;
    void HideCursor();
    while (choose!=3){
        switch (choose) {
            case 1:
                Drawmenu();
                choice(choose);
            break;
            case 2:
                system("cls");
                g.NewNumber();
                g.play(choose);
            break;
        }
    }
    system("pause");
    return 0;
}
