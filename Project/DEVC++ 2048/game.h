#ifndef _GAME_H_
#define _GAME_H_

#include "map.h"
#include <vector>
#include <set>
#include <iostream>
#include <stdio.h>

using namespace std;

typedef pair<int,int> pii;

void Pos(int x, int y);

void HideCursor();

class game{
public:
    game();//默认构造函数
    void NewNumber();//新建数字
    void play(int&,char&);//游戏进行
    void culempty();//计算空格
    int use2(char);//进行规整操作2.0
    bool Move(char);//判断当前操作是否完成
    void check();//判断是否可以操作
    void over();//游戏结束
    ~game(void){};
private:
    vector<pii> New;
    vector<vector<int> > grid;
    vector<vector<int> > index;
    int score;
    int x,y;
    set<char> operable;
};

#endif // _GAME_H_

