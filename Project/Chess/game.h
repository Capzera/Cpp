#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "piece.h"


class game{
private:
	Piece br1, br2, bn1, bn2, bb1, bb2, bq, bk;
	Piece wr1, wr2, wn1, wn2, wb1, wb2, wq, wk;
	Piece wp[8], bp[8];
	map<pii, string> grid;
	map<map<pii, string>, int> perpetualcheck;
	bool flag;
public:
	game(){ select = nullptr;};
	Piece* select;
	void init();//棋子初始化
	void show();//展示棋子位置
	void start();//游戏状态
	string checkmsg(mouse_msg, int, bool);//检查鼠标位置
	bool checkcheck(bool);//检查该颜色是否被将军
	string move(Piece*&, int, int, bool);//执行走子
	void getselect(string, Piece*&);//选中棋子
	set<pii> calloc(Piece*&);//计算可行格
	bool checkmove(string);//判断当前这个棋子是否有移动可以解除将军
	int checkmate(bool);//判断将死
	bool checkdraw(bool);//逼和判断
	void over(int);//游戏结束
	~game(){};
};

void showmap();

#endif
