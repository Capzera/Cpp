#ifndef _BLOCK_H_
#define _BLOCK_H_
#define IMAGEINIT 0
#define FONT "Consolas"
#include "common.h"

using namespace std;

class Block{
public:
	Block();//构造函数，建立一个方格
	void setstate(int);//改变方格状态
	void show();//显示方格
	void setloc(int ,int);//改变方格的位置——内置函数
	void setchar(char);//改变方格内字符
	char whattext();//返回字符接口
private:
	char text;
	int state;
	int x;
	int y;
};

#endif
