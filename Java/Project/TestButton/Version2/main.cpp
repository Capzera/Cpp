#include "MyButton.h"
int main()
{
	srand((unsigned)time(NULL));
	setcaption("按钮测试");
    initgraph(-1,-1,INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	ege_enable_aa(true);
	/*初始化*/
    RectangleButton R;
    R.newButton(500,600,300,100,"开始游戏");
	R.newButton(1060,600,300,100,"退 出");
	for (;is_run();delay_fps(144)){
		R.PutButton();
	}
	getch();
	closegraph();
    return 0;
}
