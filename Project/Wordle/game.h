#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "Map.h"

using namespace std;

static string a="";

void init();

class Game{
public:
	Game(){};//构造函数
	void Init();//初始化函数
	void Start();//游戏运行函数
	void Play(char);//开始游戏
	void Judge(int);//判定答案
	void succ();
	void fail();
	~Game(){};
private:
	string Answer;
	int raw,col;
	int cnt;
	graph G;
	bool success;
	bool failure;
};

#endif
