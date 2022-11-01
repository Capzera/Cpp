#include "game.h"
#include "common.h"
#include "Menu.h"
using namespace std;

int main()
{
	setcaption("Wordle V2.0");
	initgraph(SCREENWIDTH,SCREENHEIGHT,INIT_NOBORDER | INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	ege_enable_aa(true);
	menu m;
	Game game;
	while (1){
		cleardevice();
		m.showmenu();
		cleardevice();
		game.Init();
		game.Start();
	}
	closegraph();
	return 0;
}

