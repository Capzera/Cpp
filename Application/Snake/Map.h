#pragma once
#include <iostream>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
class Map
{
private:
	int ROW_Map = 32;			//地图的行大小;
	int COL_Map = 82;			//地图的列大小;
public:
	void DrawMap();				//画地图;
};