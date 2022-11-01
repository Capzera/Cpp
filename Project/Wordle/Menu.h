#ifndef _MENU_H_
#define _MENU_H_
#include "common.h"
#include "Button.h"
using namespace std;

class menu{
public:
	menu(){};//构造函数
	void showmenu();//打印菜单
	~menu(){};//析构函数
private:
	CircleButton btn1,btn2;
};

#endif
