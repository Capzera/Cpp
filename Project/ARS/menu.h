#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"

class menu{
public:
	menu();
	void showmenu();
	void namein(string);
	int getsize();
	~menu(){};
private:
	string address;
	vector<string> name;
	int quantity;
};

#endif //menu.h
