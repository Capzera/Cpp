#include "Menu.h"

using namespace std;

void Drawmenu()
{
	if(Menu_Examine==0)
	{
		Menu_Examine = 1;
		for (int i = 5; i > 0; i--) printf("\n");
		printf("\t\t\t\t        ☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆-☆\n");
		printf("\t\t\t\t |                                                    |\n");
		printf("\t\t\t\t |                      2048PC版                      |\n");
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

void choice(int x) {
    void HideCursor();
    static char c='1';
    if (_kbhit()){
        char tmp=_getch();
        if (c=='1'&&tmp=='s')
            c='2';
        if (c=='2'&&tmp=='w')
            c='1';
        if (c=='1'&&tmp==' ')
            x=2;
            return;
        if (c=='2'&&tmp==' ')
            exit(0);
    }
    return;
}
