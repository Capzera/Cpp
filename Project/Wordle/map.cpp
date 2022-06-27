#include "Map.h"

using namespace std;

void graph::initlocation(int r,int c){
	int space=(SCREENWIDTH-len*blockx-(len-1)*r)/2;
	int cnt=0;
	for (int i=0;i<raw;i++) {
		for (int j=0;j<len;j++) {
			blocks[cnt++].setloc(space+(70+r)*j,200+(70+c)*i);
		}
	}
}

void graph::showmap(){
	int cnt=0;
	setbkcolor(WHITE);
	setfont(15/ZOOM,0,"Consolas");
	setcolor(BLACK);
	settextjustify(LEFT_TEXT,LEFT_TEXT);
	int textline=11;
	char m[textline][144]={//保存字符画
		"  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  ",
		"| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |",
		"| | _____  _____ | || |     ____     | || |  _______     | || |  ________    | || |   _____      | || |  _________   | |",
		"| ||_   _||_   _|| || |   .'    `.   | || | |_   __ \\    | || | |_   ___ `.  | || |  |_   _|     | || | |_   ___  |  | |",
		"| |  | | /\\ | |  | || |  /  .--.  \\  | || |   | |__) |   | || |   | |   `. \\ | || |    | |       | || |   | |_  \\_|  | |",
		"| |  | |/  \\| |  | || |  | |    | |  | || |   |  __ /    | || |   | |    | | | || |    | |   _   | || |   |  _|  _   | |",
		"| |  |   /\\   |  | || |  \\  `--'  /  | || |  _| |  \\ \\_  | || |  _| |___.' / | || |   _| |__/ |  | || |  _| |___/ |  | |",
		"| |  |__/  \\__|  | || |   `.____.'   | || | |____| |___| | || | |________.'  | || |  |________|  | || | |_________|  | |",
		"| |              | || |              | || |              | || |              | || |              | || |              | |",
		"| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |",
		"'----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'"
	};
	for (int i=0;i<textline;i++) {
		outtextxy((SCREENWIDTH-textwidth(m[i]))/2,15*i,m[i]);
	}
	initlocation(15,20);//控制方格间距，默认水平间距15，竖直间距20

	for (int i=0;i<raw;i++) {
		for (int j=0;j<len;j++) {
			blocks[cnt++].show();//打印方格
		}
	}
}

void graph::changechar(char target,int cnt){
	this->blocks[cnt].setchar(target);
}

void graph::changestate(int target,int cnt){
	this->blocks[cnt].setstate(target);
}

char graph::whattext(int cnt){
	return this->blocks[cnt].whattext();
}
