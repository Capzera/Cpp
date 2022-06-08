#include "Map.h"

using namespace std;

graph::graph(){
	setcaption("Wordle V1.0");
	initgraph(-1,-1,INIT_RENDERMANUAL);
}

graph::~graph(){
	getch();
	closegraph();
	
}

void graph::initlocation(int r,int c){
	int space=(1920-len*blockx-(len-1)*r)/2;
	int cnt=0;
	for (int i=0;i<raw;i++) {
		for (int j=0;j<len;j++) {
			blocks[cnt++].setloc(space+(70+r)*j,200+(70+c)*i);
		}
	}
}

void graph::showmap(){
	int cnt=0;
	//setbkcolor(RGB(0xEC,0xD8,0x20));//青色
	setbkcolor(WHITE);
	setfont(15,0,"Consolas");
	setcolor(BLACK);
	outtextxy(540,0,".----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.");
	outtextxy(540,15,"| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |");
	outtextxy(540,30,"| | _____  _____ | || |     ____     | || |  _______     | || |  ________    | || |   _____      | || |  _________   | |");
	outtextxy(540,45,"| ||_   _||_   _|| || |   .'    `.   | || | |_   __ \\    | || | |_   ___ `.  | || |  |_   _|     | || | |_   ___  |  | |");
	outtextxy(540,60,"| |  | | /\\ | |  | || |  /  .--.  \\  | || |   | |__) |   | || |   | |   `. \\ | || |    | |       | || |   | |_  \\_|  | |");
	outtextxy(540,75,"| |  | |/  \\| |  | || |  | |    | |  | || |   |  __ /    | || |   | |    | | | || |    | |   _   | || |   |  _|  _   | |");
	outtextxy(540,90,"| |  |   /\\   |  | || |  \\  `--'  /  | || |  _| |  \\ \\_  | || |  _| |___.' / | || |   _| |__/ |  | || |  _| |___/ |  | |");
	outtextxy(540,105,"| |  |__/  \\__|  | || |   `.____.'   | || | |____| |___| | || | |________.'  | || |  |________|  | || | |_________|  | |");
	outtextxy(540,120,"| |              | || |              | || |              | || |              | || |              | || |              | |");
	outtextxy(540,135,"| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |");
	outtextxy(540,150,"'----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'");
	initlocation(15,20);
	
	for (int i=0;i<raw;i++) {
		for (int j=0;j<len;j++) {
			blocks[cnt++].show();
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

