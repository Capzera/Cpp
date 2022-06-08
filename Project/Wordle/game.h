#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "Map.h"
#define SUPERANSWER "SABIN"
using namespace std;

class Game{
public:
	Game();//构造函数
	void Start();//游戏运行函数
	void Play(char);//开始游戏
	void Judge(int);//判定答案
	void succ();
	void fail();
	~Game();
private:
	int raw,col;
	int cnt;
	graph G;
	bool success;
	bool failure;
};

#endif
