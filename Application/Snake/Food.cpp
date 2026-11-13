#include"Food.h"

void Pos(int x, int y);
void HideCursor();

void Food::randfood()
{
    srand((int)time(NULL));//利用时间添加随机数种子，需要ctime头文件
Loop:
    pos_x = rand() % (76) + 24;
    pos_y = rand() % (29) + 7;
    if (pos_x % 2)         //如果食物的x坐标不是偶数则重新确定食物的坐标
        goto Loop;
    Pos(pos_x, pos_y);     //在确认好的位置输出食物
    cout << "★";
}
int Food::getFood_x()
{
    return pos_x;
}
int Food::getFood_y()
{
    return pos_y;
}