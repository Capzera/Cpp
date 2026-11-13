#include"Menu.h"
#include"BeginGame.h"

//两个全局函数
//控制输出光标
void Pos(int x, int y)   
{
	COORD pos;  //pos为结构体
	pos.X = x;  //控制列
	pos.Y = y;  //控制行
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//读取输出句柄来控制光标
}
//隐藏光标
void HideCursor()           
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main()
{
	system("title 贪吃蛇");	//设置窗口名字
	system("color 3F");		//设置颜色
	static int x = 1;		//初始打印菜单
	void HideCursor();		//隐藏光标
	while (true)										//把一个mp3格式文件拷到工程目录下改名为bgm.mp3即可
	{
		switch (x)
		{
		case 1:
			DrawMenu();
			Chioce(x);
			Init_Examine = 0;
			break;
		case 2:
			BeginGame(x);
			break;
		}
	}
	system("pause");
	return 0;
}