#ifndef _MAP_def_H_
#define _MAP_def_H_

#include<bits/stdc++.h>
#include "game.h"

using namespace std;

class Map
{
private:
    vector<vector<int>> grid;
    int score;
    int x;
    int y;
public:
    Map(){};//默认构造函数
    Map(vector<vector<int>>,int);//复制构造函数
    void printmap();//输出地图
    int getlen(int);//获取数字长度以居中
    void change(vector<vector<int>>,int);//更改map中数据
    void setnew(int ,int);//设置新数字点
    ~Map(void){};
};
#endif // _MAP_H_
