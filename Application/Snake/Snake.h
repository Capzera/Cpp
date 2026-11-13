#pragma once
#include<vector>
#include <conio.h>
#include"Food.h"
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")
void Pos(int x, int y);
void HideCursor();
class Snake
{
private:
    struct Snakepoint
    {
        int x;  //第几列
        int y;  //第几列
    };
    vector<Snakepoint> snakepoint;

    char Direction;                                         //蛇行走的方向
    void Show_information(int& score, int& sleeptime);      //打印信息
    void overout(const int score);                          //游戏结束显示
    void Gameover(int& score,int &x);                       //游戏结束
    void Control(Snakepoint& nexthead);                     //控制方向

public:
    Snake();                                                //构造函数
    void move(Food& food, int& score, int& sleeptime,int& x);//蛇移动并打印出来
    void sleep(int& score, int& sleeptime);                 //根据分数调整游戏速度
};