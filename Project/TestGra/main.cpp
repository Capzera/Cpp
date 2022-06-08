#include <graphics.h>

int main()
{
	initgraph(1920, 1080, INIT_RENDERMANUAL);			//初始化窗口
	setcaption("EGE鼠标消息处理");	//设置窗口标题
	
	setbkcolor(WHITE);		//设置窗口背景为白色
	setcolor(RED);			//设置前景色为红色
	setfillcolor(RED);		//设置填充色为红色
	
	//创建图像缓存
	PIMAGE imageBuff = newimage(1920, 1080);
	
	//将图像设置成全透明，这个是必要步骤
	ege_setalpha(0, imageBuff);
	
	//设置ARGB颜色
	setfillcolor(EGEACOLOR(0xff, YELLOW), imageBuff);
	
	//把很复杂的绘制操作绘制到图像缓存上
	for (int i = 0; i < 192; i++) {
		for (int j = 0; j < 108; j++)
			//使用高级函数绘图
			ege_fillellipse(i * 10, j * 10, 10, 10, imageBuff);
	}
	int flag=1;//设置标记记录背景是否打开
	for (; is_run(); delay_fps(144)) {
		mouse_msg msg;
		char k;//记录按键操作
		k=' ';//初始化按键
		if (kbhit()) k=getch();//获取按键
		if (k=='p') flag^=1;//如果按键为'p'，更改背景开关选项
		while (mousemsg()) {
			msg = getmouse();
		}
		
		//鼠标移动则绘画
		if (msg.is_move()) {
			cleardevice();
			
			//把图像缓存绘制到窗口上,
			if (flag==1) putimage_alphablend(NULL,imageBuff,0, 0);//设置是否显示背景
			
			//把上面那句换成这句试试看
			//putimage(0, 0, imageBuff);
			
			//画动的部分
			fillellipse(msg.x, msg.y, 30, 30);
		}
	}
	
	closegraph();
	
	return 0;
}
