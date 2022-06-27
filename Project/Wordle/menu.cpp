#include "Menu.h"

using namespace std;

static int Select=-1;
void menu::showmenu(){
	RectangleButton R;
	R.newButton(500,600,300,100,"开始游戏");
	R.newButton(1060,600,300,100,"退 出");
	for (;is_run();delay_fps(144)){
		R.PutButton(Select);
		if (Select>-1) return;
	}
}
