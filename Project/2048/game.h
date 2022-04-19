#ifndef _GAME_H_
#define _GAME_H_

#include<bits/stdc++.h>
#include<vector>
#include "map.h"
#include <iostream>
#include <conio.h>
#include<ctime>
#include <stdio.h>

using namespace std;

typedef pair<int,int> pii;

void Pos(int x, int y);
void HideCursor();

class game{
public:
    game();//默认构造函数
    void NewNumber();//新建数字
    void play(int&);//游戏进行
    void culempty();//计算空格
    int use(char);//进行归整操作
    void check();//判断是否可以操作
    void over();//游戏结束
    ~game(void){};
private:
    vector<pii> New;
    vector<vector<int>> grid;
    int score;
    int x,y;
    set<char> operable;
};
#endif // _GAME_H_
