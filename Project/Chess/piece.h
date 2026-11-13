#ifndef _PIECE_H_
#define _PIECE_H_

#include "common.h"

class Piece{
private:
	string name;
	bool white;//是否为白方
	bool dead;//是否死亡
	bool ischeck;//是否被将军
	bool pawnstart;//兵是否起步
	bool kingstart;//王是否移动
	bool rookstart;//车是否移动
	bool isselect;//是否被选中
	int type;//棋子类型0-Pawn  1-Rook  2-Knight 3- Bishop 4-Queen 5-King
	PIMAGE origin, image;//贴图变量和压缩贴图变量
	pos Pos;//位置结构体
	void init();//初始化棋子图片
public:
	Piece(){};
	Piece(bool, int, char, int);//构造函数
	void setpiece(string, bool, int, char, int);//棋子预设
	void setloc(char, int);//设置棋子位置
	void showpiece();//显示棋子
	bool getdead();//是否死亡
	bool getcolor();//返回棋子颜色
	bool getpawnstart();//返回兵是否走过
	bool getkingstart();//返回王是否走过
	bool getrookstart();//返回车是否走过
	int gettype();//返回棋子类型
	string getname();//返回棋子名称
	pii getloc();//返回当前位置
	void pawnmove();//兵移动则不能走2格
	void kingmove();//王移动过
	void rookmove();//车移动过
	void isdead();//被吃
	void isalive(char, int);//回溯棋子被吃的状态
	void Select();//棋子被选中
	void Check();//插入将军标记
	void checkcancel();//取消将军标记
	void clear();//清空标记
	void promotion();//升变
	~Piece(){};
};

//100 100 -> 80 74
#endif
