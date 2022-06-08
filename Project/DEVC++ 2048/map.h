#ifndef _MAP_def_H_
#define _MAP_def_H_

#include<iostream>
#include<vector>

using namespace std;

void HideCursor();

void Pos(int x,int y);

class Map
{
public:
    Map(){};//默认构造函数

    Map(vector<vector<int> >,int);//复制构造函数
    void printmap();//打印地图内数字
    int getlen(int);//获取数字长度以居中
    void drawmap();//打印地图模板
    void change(vector<vector<int> >,int);//更改map中数据
    void change(vector<vector<int> >);
    void setnew(int ,int);//设置新数字点
    ~Map(void){};
private:
    vector<vector<int> > grid;
    int score;
    int x;
    int y;
};
#endif // _MAP_H_

