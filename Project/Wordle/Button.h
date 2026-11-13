#ifndef _MY_BUTTON_H_
#define _MY_BUTTON_H_

#include <graphics.h>
#include <bits/stdc++.h>

using namespace std;

static int PressId=-1;
static int FocusId=-1;
static int HoverId=-1;

struct ButtonStates {
	bool pressed;	/* 按下状态 */
	bool hovered;	/* 悬停状态 */
	bool focused;	/* 焦点状态 */
};

struct Circle{
	unsigned int id;
	int x, y;		/* 圆心*/
	int radius;		/* 半径*/
	color_t normalColor;
	color_t pressedColor;
	const char* text;
	ButtonStates state;
};

struct RoundRectangle{
	unsigned int id;
	int x, y;
	int width, height;
	float radius;
	color_t normalColor;
	color_t pressdColor;
	const char* text;
	ButtonStates state;
};

struct NormalRectangle{
	unsigned int id;
	int x, y;
	int width, height;
	color_t normalColor;
	color_t pressedColor;
	const char* text;
	ButtonStates state;
};

class CircleButton{
public:
	CircleButton(){};
	void newButton(int,int,int,const char*);//初始化一个新按钮*
	void initButtonState(struct Circle&);//初始化按钮状态*
	bool insideButton(struct Circle&,int,int);//判断xy是否在按钮内*
	void DrawButton();//画按钮*
	int SearchButton(int,int);//寻找x,y属于哪个按钮
	color_t interpColor(color_t, color_t, float);//颜色
	void PutButton();//显示按钮
	~CircleButton(){};
private:
	vector<struct Circle> button;
	bool redraw=true;
	int xMouse,yMouse;
};

class RectangleButton{
public:
	RectangleButton(){};
	void newButton(int,int,int,int,const char*);//初始化矩形按钮
	void initButtonState(struct NormalRectangle&);
	bool insideButton(struct NormalRectangle&,int x,int y);
	void DrawButton();//画按钮
	int SearchButton(int,int);//寻找x,y属于哪个按钮
	color_t interpColor(color_t, color_t, float);//颜色
	void PutButton(int&);//显示按钮
	~RectangleButton(){};
private:
	vector<struct NormalRectangle> button;
	bool redraw=true;
	int xMouse,yMouse;
};

#endif
