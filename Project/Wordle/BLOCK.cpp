#include "BLOCK.h"

using namespace std;

Block::Block(){
	state=IMAGEINIT;//设置默认方格状态为未装填
	text=' ';//用空格做占位
}

void Block::setstate(int target){
	state=target;
}

void Block::setloc(int x_Pos,int y_Pos){
	x=x_Pos;
	y=y_Pos;
}

void Block::show(){
	PIMAGE show=newimage();
	switch(state){
		case 0:
			getimage(show,"icon\\block.png");//获取对应图片，输出时才获取图片
		break;
		case 1:
			getimage(show,"icon\\block1.png");
		break;
		case 2:
			getimage(show,"icon\\block2.png");
		break;
		case 3:
			getimage(show,"icon\\block3.png");
		break;
	}
	setbkmode(TRANSPARENT);//设置图片为透明
	setfont(40,0,FONT);//设置图片内字体
	putimage(x,y,show);//输出图片
	outtextxy(x+25,y+14,text);//输出文字
	delimage(show);//释放内存
}

void Block::setchar(char target){
	text=target;
}

char Block::whattext(){
	return text;
}
