#pragma once
#include"Map.h"
#include"Food.h"
#include"Snake.h"
Food food;              //食物对象
Map map;				//地图对象
Snake snake;            //蛇对象
int score = 0;          //得分变量
int sleeptime = 120;	//游戏速度
int Init_Examine;

void BeginGame(int &x)
{
	if (Init_Examine == 0)
	{
		Init_Examine = 1;
		system("cls");							//清除之前打印的菜单
		HideCursor();
		map.DrawMap();							//画地图
		food.randfood();						//随机产生一个食物
		snake.move(food, score, sleeptime, x);	//蛇移动
		snake.sleep(score, sleeptime);
	}
	else if (Init_Examine == 1)
	{
		snake.move(food, score, sleeptime, x);	//蛇移动
		snake.sleep(score, sleeptime);			//游戏速度
	}
}