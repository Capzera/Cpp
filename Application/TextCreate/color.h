#ifndef _COLOR_H_
#define _COLOR_H_
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
void rgb_init() {																// 初始化
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);		//输入句柄
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		//输出句柄
	DWORD dwInMode, dwOutMode;
	GetConsoleMode(hIn, &dwInMode);						//获取控制台输入模式
	GetConsoleMode(hOut, &dwOutMode);					//获取控制台输出模式
	dwInMode |= 0x0200;									//更改
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn, dwInMode);						//设置控制台输入模式
	SetConsoleMode(hOut, dwOutMode);					//设置控制台输出模式
}
void rgb_set(int wr,int wg,int wb) {						//设置RGB
	printf("\033[38;2;%d;%d;%dm",wr,wg,wb);	//\033[38表示前景，\033[48表示背景，三个%d表示混合的数
}
#endif