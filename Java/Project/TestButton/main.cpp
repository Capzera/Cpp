#include "MyButton.h"
int main()
{
	setcaption("按钮测试");
    initgraph(-1,-1,INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	ege_enable_aa(true);
	/*初始化*/
	CircleButton C;
	C.newButton(600,700,100,"开始游戏");
	C.newButton(1200,700,100,"退出游戏");
    RectangleButton R;
    R.newButton(800,600,1100,700,"开始游戏");
	for (;is_run();delay_fps(144)){
		R.RectangleButton::PutButton();
	}
	getch();
	closegraph();
    return 0;
}
