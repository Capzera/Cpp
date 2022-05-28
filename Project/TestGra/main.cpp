#include"ege.h"
#include"graphics.h"
using namespace std;

int main()
{
	initgraph(640, 480);


	setviewport(200, 100, 330, 130, 1);
	//设置视口矩形区域为(200,100) - (330, 130)
	//最后一个参数为1表示出了这个区域的图形会被裁剪
	//后面所绘画的图形的原点坐标(0,0)，会映射到(200,100)

	setcolor(EGERGB(0x0, 0xFF, 0x0));
	setfontbkcolor(RGB(0x80, 0x00, 0x80));
	setfont(18, 0, "宋体");
	outtextxy(0, 0, "Hello EGE Graphics");

	setbkmode(TRANSPARENT);
	outtextxy(0, 20, "Hello EGE Graphics");

	setviewport(0, 0, getwidth(), getheight(), 1);
	//重置视口为整个窗口
	outtextxy(0, 0, "Hello EGE Graphics");

	getch();

	closegraph();
	return 0;
}
