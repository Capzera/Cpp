#include"Map.h"
void Pos(int x, int y);
void HideCursor();

void Map::DrawMap()
{
	system("color 3F");                      //设置颜色
	int i = 0;
	for (i = 20; i < COL_Map + 20; i += 2)   //打印上下边框（每个■占用两列）
	{
		Pos(i, 5);							 //如果不加20就是打印在控制台边上，下面的5也不用加
		printf("■");
		Pos(i, 5 + ROW_Map - 1);
		printf("■");
	}
	for (i = 5; i < ROW_Map + 5; i++)		 //打印左右边框
	{
		Pos(20, i);
		printf("■");
		Pos(20 + COL_Map - 2, i);
		printf("■");
	}
}