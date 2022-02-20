#pragma once
#include <ctime>
#include "Map.h"
class Food
{
private:
    int pos_x;
    int pos_y;
public:
    void randfood();          //随机产生一个食物
    int getFood_x();          //返回食物的x坐标
    int getFood_y();          //返回食物的y坐标
};