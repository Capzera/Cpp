#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"
#include "BLOCK.h"

using namespace std;

class graph{
public:
	graph(){};//构造函数
	void initlocation(int,int);//初始化地图位置(水平间距，垂直间距)
	void showmap();//打印地图
	void changechar(char,int);//改变方格内元素
	void changestate(int,int);//改变方格内元素的颜色
	char whattext(int cnt);//返回方格内字符
	~graph(){};//析构函数
private:
	int raw=6;
	int len=5;
	Block blocks[30];
	int blockx=70,blocky=70;
};

#endif
