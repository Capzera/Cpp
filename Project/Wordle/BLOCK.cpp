#include "BLOCK.h"

using namespace std;

Block::Block(){
	state=IMAGEINIT;
	text=' ';
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
			getimage_pngfile(show,"D:\\Program\\C++\\Project\\Wordle\\icon\\block.png");
		break;
		case 1:
			getimage_pngfile(show,"D:\\Program\\C++\\Project\\Wordle\\icon\\block1.png");
		break;
		case 2: 
			getimage_pngfile(show,"D:\\Program\\C++\\Project\\Wordle\\icon\\block2.png");
		break;
		case 3:
			getimage_pngfile(show,"D:\\Program\\C++\\Project\\Wordle\\icon\\block3.png");
		break;
	}
	setbkmode(TRANSPARENT);
	setfont(40,0,FONT);
	putimage(x,y,show);
	outtextxy(x+25,y+14,text);
	delimage(show); 
}

void Block::setchar(char target){
	text=target;
}

char Block::whattext(){
	return text;
}
