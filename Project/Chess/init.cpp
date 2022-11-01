#include "init.h"
#include "piece.h"

void init(){
	initgraph(960, 960);
	setrendermode(RENDER_MANUAL);
	setcaption("Chess V1.0 By:机长大大");
	/*
	setbkmode(TRANSPARENT);
	PIMAGE image = newimage();
	getimage(image, "icon\\bp.png");
	int width = 100;
	PIMAGE target = newimage(width, width);
	putimage(target, 0, 0, width, width, image, 0, 0, getwidth(image), getheight(image));
	putimage_withalpha(NULL, target, 80, 74);
	//putimage_withalpha(NULL, image, 100, 100, 0, 0, 100, 100);
	*/
}
