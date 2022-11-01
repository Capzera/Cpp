#ifndef _MY_BUTTON_H_
#define _MY_BUTTON_H_

#include <graphics.h>
#include <bits/stdc++.h>

using namespace std;

struct ButtonStates
{
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
	color_t pressdColor;
	const char* text;
	ButtonStates state;
};

class CircleButton{
public:
	CircleButton();
	void newButton(int,int,int,const char*);//初始化一个新按钮
	void PutButton();//显示按钮
	void initButtonState(struct Circle&);//初始化按钮状态
	bool insideButton(struct Circle&,int,int);//判断xy是否在按钮内
	void DrawButton();//画按钮
	void PressOn(struct Circle&);//按下开
	void PressOff(struct Circle&);//按下关
	void HoverOn(struct Circle&);//悬停开
	void HoverOff(struct Circle&);//悬停关
	void FocusOn(struct Circle&);//焦点开
	void FocusOff(struct Circle&);//焦点关
	bool getPress(struct Circle&);//获取按下状态
	bool getFocus(struct Circle&);//获取焦点状态
	bool getHover(struct Circle&);//获取悬停状态
	unsigned int generateId();//获取按钮ID
	int SearchButton(int,int);//寻找x,y属于哪个按钮
	unsigned int getId(struct Circle&);
	color_t interpColor(color_t, color_t, float);//颜色
	~CircleButton();
private:
	vector<struct Circle> button;
	bool redraw=true;
	int xMouse,yMouse;
	int PressId;
	int FocusId;
	int HoverId;
};

/*class Rectangle:public CircleButton{
public:
	void initButton(int,int,int,int);//初始化矩形按钮
	void DrawButton();//画按钮
private:
	struct NormalRectangle button;
};*/

#endif
